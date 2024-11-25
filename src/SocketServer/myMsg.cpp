#include "myMsg.h"

myMsg::myMsg() {
    this->mtype = msgType::ERROR;
    this->msgContent = "\0";
}

myMsg:: myMsg(enum msgType mtype) {
    this->mtype = mtype;
    this->msgContent = "\0";
}

myMsg:: myMsg(enum msgType mtype, std::string mcontent){
    this->mtype = mtype;
    this->msgContent = mcontent;
}

msgType myMsg:: getmsgType(){
    return this->mtype;
}

std::string myMsg:: getmsgContent(){
    return this->msgContent;
}

void myMsg:: setmsgType(enum msgType mtype){
    this->mtype = mtype;
}

void myMsg:: setmsgContent(std::string mcontent){
    this->msgContent = mcontent;
}

std::ostream& operator<<(std::ostream& os, msgType type) {
    switch (type) {
        case msgType::ERROR: os << "msgType::ERROR"; break;
        case msgType::CONNECTION_REQUEST: os << "msgType::CONNECTION_REQUEST"; break;
        case msgType::CONNECTION_SUCCESS: os << "msgType::CONNECTION_SUCCESS"; break;
        case msgType::CONNECTION_FAILED: os << "msgType::CONNECTION_FAILED"; break;
        case msgType::FILE_REQUEST: os << "msgType::FILE_REQUEST"; break;
        case msgType::FILE_RECEIVE_SUCCESS: os << "msgType::FILE_RECEIVE_SUCCESS"; break;
        case msgType::FILE_RECEIVE_FAILED: os << "msgType::FILE_RECEIVE_FAILED"; break;
        case msgType::SQL_REQUEST: os << "msgType::SQL_REQUEST"; break;
        case msgType::SQL_SUCCESS: os << "msgType::SQL_SUCCESS"; break;
        case msgType::SQL_FAILED: os << "msgType::SQL_FAILED"; break;
        default: os << "Error"; break;
    }
    return os;
}