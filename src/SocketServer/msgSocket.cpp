#include "msgSocket.h"
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "../DataMapper.h"

#include "../EncryptService.h"
void * thread_func(void* arg){

    EncryptionParameters parms(scheme_type::bfv);

    // 设置 SEAL 参数
    size_t poly_modulus_degree = 2048;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
    EncryptService service(parms);

    int tableId = 0;

    threadArgs *args = (threadArgs *)arg;
    int serversocket = args->serverSocket, msgsocket = args->acceptSocket;
    while(1){
        // 初始化并接受msg
        myMsg msg(msgType::ERROR);
        int ret = recvMsg(msgsocket,msg);
        if(ret < 0){
            close(msgsocket);
            exit(-1);
        }
        msgType type = msg.getmsgType();

        // 连接数据库
         if(type==msgType::CONNECTION_REQUEST){
            std::string conninfo = msg.getmsgContent();
            PGconn *conn= PQconnectdb(conninfo.c_str());
            // 连接成功
            if (PQstatus(conn) != CONNECTION_OK) {
                std::cerr << "Failed to connect to database: " << PQerrorMessage(conn) << std::endl;
                PQfinish(conn);
                myMsg msg2(msgType::CONNECTION_FAILED);
                sendMsg(msgsocket,msg2);
                continue;
            }
            // 连接失败
            else {
                myMsg msg2(msgType::CONNECTION_SUCCESS);
                sendMsg(msgsocket,msg2);
                std::cout << "Succeeded to connect to database\n";
            }
             service.setConn(conn);
        }
        // 上传文件
        else if(type==msgType::FILE_REQUEST){
            // 获取 文件名、文件大小
            std::string fileinfo = msg.getmsgContent();
            size_t spacePos = fileinfo.find(' ');
            std::string filename = fileinfo.substr(0,spacePos);
            int filesize = std::stoi(fileinfo.substr(spacePos + 1));
            // 创建 文件专用socket
            struct sockaddr_in client_addr;
            socklen_t client_addr_size = sizeof(client_addr);
            printf("start to accept socket for file\n");
            int filesocket = accept(serversocket,(struct sockaddr*)&client_addr, &client_addr_size);
            if (filesocket == -1) {
                perror("Failed to accept socket for file\n");
                exit(-1);
            }else{
                printf("Succeeded to accept socket for file\n");
                // 接收文件
                bool ret2 = recvFile(filesocket,filename,filesize);
                close(filesocket);
                if(ret2){
                    myMsg msg2(msgType::FILE_RECEIVE_SUCCESS);
                    sendMsg(msgsocket,msg2);
                }else{
                    myMsg msg2(msgType::FILE_RECEIVE_FAILED);
                    sendMsg(msgsocket,msg2);
                }
                // 加密数据并上传到数据库
                ////TODO 1 实现数据上传。
                 service.updateFileIntoSql(filename);
                // 尝试删除文件
                if (remove(filename.c_str()) != 0) {
                    printf("Failed to delete file.\n");
                } else {
                    printf("File deleted successfully.\n");
                }
            }
        }
        // sql查询
        else if(type==msgType::SQL_REQUEST){
            string sqlquery = msg.getmsgContent();
            // 查询加密数据库，查询是否成功

            //// TODO 2 加密数据库查询
            vector<vector<string>> sqlresult = service.excuteSql(sqlquery);
            bool hasResult = sqlresult.empty();
            if(hasResult){
                myMsg msg2(msgType::SQL_SUCCESS);
                sendMsg(msgsocket,msg2);
                // 创建 查询结果专用socket
                struct sockaddr_in client_addr;
                socklen_t client_addr_size = sizeof(client_addr);
                printf("start to accept socket for sql result\n");
                int sqlsocket = accept(serversocket,(struct sockaddr*)&client_addr, &client_addr_size);
                if (sqlsocket == -1) {
                    perror("Failed to accept socket for sql result\n");
                    exit(-1);
                }else {
                    printf("Succeeded to accept socket for sql result\n");
                    bool ret2 = sendSqlResult(sqlsocket, sqlresult);
                    close(sqlsocket);
                }
            }else{
                myMsg msg2(msgType::SQL_FAILED);
                sendMsg(msgsocket,msg2);
            }
        }
        // 非正常消息：接受方或发送方出错，返回一个msgType::ERROR消息提醒对方
        else{
            printf("Warning: a illegal message(msgType::ERROR)\n");
            std::cout<< "Warning: a illegal message("<< type <<")\n";
            myMsg msg2(msgType::ERROR);
            sendMsg(msgsocket,msg2);
        }
    }
}

int recvMsg(int socket,myMsg &msg){
    msg.setmsgType(msgType::ERROR);
    msg.setmsgContent("");
    char buffer[bufSize + 1] = {'\0'};
    int result = -1;
    while(1){
        result = recv(socket, buffer, bufSize, 0);
        if (result > 0) {
            msg = myMsg::fromString(buffer);
            if(msg.getmsgType() == msgType::ERROR) {
                std::cerr << "Recive empty message\n";
                result = -1;
                return result;
            }
            return result;
        }

    }

}

int sendMsg(int msgsocket,myMsg &msg){
    std::string msgstr = msg.toString();
    const char *buffer = msgstr.c_str();
    int ret = -1;
    int cnt = strlen(buffer);
    ret = send(msgsocket,buffer,cnt,0);
    return ret;
}

int create_serverSocket()
{
    int serverSock = -1;
    serverSock  = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSock < 0){
        printf("socket creation failed\n");
        return -1;
    }
    printf("socket create successfully.\n");

    unsigned int timeout = 1000;
    int opt = 1;
    unsigned int len = sizeof(opt);
    setsockopt(serverSock, IPPROTO_TCP, 18, &timeout, sizeof(timeout));
    setsockopt(serverSock,SOL_SOCKET,SO_REUSEADDR,&opt,len);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons((u_short)msgPort);
    serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
    if (::bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("Bind error.\n");
        return -1;
    }
    printf("Bind successful.\n");

    if(listen(serverSock,10)==-1){
        printf("Listen error!\n");
    }
    printf("Start to listen!\n");

    return serverSock;
}

bool recvFile(int filesocket,std::string fileName,int filesize)
{
    FILE* fp = fopen(fileName.c_str(), "wb");
    if (!fp){
        perror("file fp failed:\n");
        return false;
    }

    int recvlen = 0, writelen;
    uint writecnt = 0, recvcnt = 0;
    char buffer[bufSize + 1] = {'\0'};
    do{
        memset(buffer,'\0',bufSize + 1);
        recvlen = recv(filesocket, buffer, bufSize, 0);
        if(recvlen > 0){
            recvcnt += recvlen;
            writelen = fwrite(buffer,1, recvlen, fp);
            if(writelen > 0){
                writecnt += writelen;
            }else{
                printf("File write error\n");
                return false;
            }
        }else if(recvlen == 0){
            printf("File reception completed\n");
            break;
        }else{
            printf("recv failed: %s\n", strerror(errno));
            return false;
        }
    } while (true);
    fclose(fp);

    if(recvcnt < filesize || writecnt < filesize){
        printf("receive or write file with error!\n");
        return false;
    }else{
        printf("receive and write file success!\n");
        return true;
    }
}

bool sqlQuery(string sqlquery, vector<vector<string>> &sqlresult){
    return true;
}
bool sendSqlResult(int sqlsocket,vector<vector<string>> &sqlresult){

    return true;
}
