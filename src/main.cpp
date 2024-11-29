#include "main.h"

#include <iostream>
#include <libpq-fe.h>
#include <string>

#include <vector>
#include "DataMapper.h"
#include "EncryptManager.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "EncryptTools/EncryptUtil.h"
#include "SocketServer//msgSocket.h"
#include "dataObject/EncKey.h"
#include "dataObject/EncryptedMultiMap.h"
#include "seal/seal.h"
#include "dataObject/SqlPlan.h"
#include "Sql/SqlPlanExecutor.h"
#include "dataObject/SqlPlan.h"


using namespace std;
using namespace seal;


void testSocket() {
    int serverSock = -1,acceptSock=-1;
    serverSock = create_serverSocket();
    if(serverSock < 0){
        printf("failed to create server socket\n");
        exit(-1);
    }

    signal(SIGPIPE, SIG_IGN);

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    while (1){ //服务器循环接受客户端socket请求
        printf("start to accept\n");
        acceptSock = accept(serverSock,(struct sockaddr*)&client_addr, &client_addr_size);
        if (acceptSock == -1) {
            perror("Accept failed");
            continue;
        }
        printf("Succeed to accept client: %d\n",acceptSock);
        threadArgs aArg = {serverSock,acceptSock};
        thread_func((void *)&aArg);
        close(acceptSock);
    }
}

void testSql() {
    EncryptionParameters parms(scheme_type::bfv);
    PGconn *conn = PQconnectdb(PGSQL_CONNINFO.c_str());
    //检查连接状态
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "连接数据库失败: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return;
    } else {
        std::cout << "已成功连接到数据库！\n";
    }
    // 设置 SEAL 参数
    size_t poly_modulus_degree = 2048;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
    EncryptService encrypt_service(parms);
    encrypt_service.setConn(conn);

    Table table = DataMapper::fileReader("../Resource/data/people.csv",true);
    table.set_name("people");
    encrypt_service.uploadTableIntoSql(table);
   //vector<vector<string>> res = encrypt_service.executeSql("SELECT ID FROM student WHERE Name = 'Alice';");
   //cout << res[0][0] << endl;
}

int main() {
//    testSql();
    testSocket();
    //std::string hex = prfFunctionReturnString("people,1,aaaomksqh");
    //cout << hex << endl;
    return 0;
}
