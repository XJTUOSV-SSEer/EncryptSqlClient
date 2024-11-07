cmake_minimum_required(VERSION 3.29)
project(EncryptSqlClient)
find_package(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD 20)
#set(MYSQL_HEADER /usr/local/mysql/include)
#set(MYSQL_LIB /usr/local/mysql/lib)
#
## 添加 MySQL 连接头文件
#include_directories(${MYSQL_HEADER})
# 添加 MySQL 外部库

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -O1")

add_executable(EncryptSqlClient src/main.cpp
        src/main.h
        src/DataMapper.cpp
        src/DataMapper.h
        src/EncryptTools/Crypto_Primitives.cpp
        src/EncryptTools/Crypto_Primitives.h
        src/EncryptTools/EncryptUtil.cpp
        src/EncryptTools/EncryptUtil.h
        src/DO/EncKey.cpp
        src/DO/EncKey.h
        src/DO/EncryptedMultiMap.cpp
        src/DO/EncryptedMultiMap.h
        src/DO/RowMultiMap.cpp
        src/DO/RowMultiMap.h
        src/main.h
        src/EncryptManager.cpp
        src/EncryptManager.h
        src/testsArchive.cpp
        src/testsArchive.h
)



#target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libssl.3.dylib /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libcrypto.dylib)
#target_link_libraries(EncryptSqlClient /usr/local/mysql-connector-c++-9.0.0/lib64/libmysqlcppconn.10.dylib)

target_link_libraries(EncryptSqlClient /usr/local/ssl/lib64/libcrypto.so /usr/local/ssl/lib64/libssl.a)
target_link_libraries(EncryptSqlClient /usr/local/pgsql/lib/libpq.so)
target_link_libraries(EncryptSqlClient /usr/local/lib/libseal-4.1.a)
target_link_libraries(EncryptSqlClient /usr/local/lib/libzstd.a)
target_link_libraries(EncryptSqlClient /usr/local/lib/libz.a)
#target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/gmp/6.3.0/lib/libgmp.10.dylib /opt/homebrew/Cellar/gmp/6.3.0/lib/libgmpxx.4.dylib)

