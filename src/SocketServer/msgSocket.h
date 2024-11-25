#pragma once
#ifndef MSGSOCKET_H
#define MSGSOCKET_H

#include "myMsg.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <libpq-fe.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <vector>

#include <unistd.h>
#include <sys/signal.h>
#include <netinet/in.h>

using namespace std;

const int msgPort = 20000;
const int filePort = 20001;

inline const int bufSize = 1024 * 1;

typedef struct {
    int serverSocket;
    int acceptSocket;
}threadArgs;

void* thread_func(void* arg);
int recvMsg(int socket,myMsg &msg);
int sendMsg(int socket,myMsg &msg);
int create_serverSocket();
bool recvFile(int socket,std::string fileName,int);
bool sqlQuery(string sqlquery, vector<vector<std::string>> &sqlresult);
bool sendSqlResult(int sqlsocket,vector<vector<std::string>> &sqlresult);

#endif // MSGSOCKET_H
