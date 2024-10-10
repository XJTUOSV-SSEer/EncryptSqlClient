cmake_minimum_required(VERSION 3.29)
project(EncryptSqlClient)
set(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@3/3.3.2)
find_package(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD 20)
#set(MYSQL_HEADER /usr/local/mysql/include)
#set(MYSQL_LIB /usr/local/mysql/lib)
#
## 添加 MySQL 连接头文件
#include_directories(${MYSQL_HEADER})
# 添加 MySQL 外部库



add_executable(EncryptSqlClient main.cpp
        main.h
        DataMapper.cpp
        DataMapper.h
        EncryptTools/Crypto_Primitives.cpp
        EncryptTools/Crypto_Primitives.h
        EncryptTools/EncryptUtil.cpp
        EncryptTools/EncryptUtil.h
        DO/EncKey.cpp
        DO/EncKey.h
        DO/EncryptedMultiMap.cpp
        DO/EncryptedMultiMap.h
        DO/RowMultiMap.cpp
        DO/RowMultiMap.h
        main.h
        EncryptManager.cpp
        EncryptManager.h
)

target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libssl.3.dylib /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libcrypto.dylib)
#target_link_libraries(EncryptSqlClient /usr/local/mysql-connector-c++-9.0.0/lib64/libmysqlcppconn.10.dylib)
target_link_libraries(EncryptSqlClient /usr/local/pgsql/lib/libpq.5.14.dylib)
target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/seal/4.1.2/lib/libseal.4.1.2.dylib)


