cmake_minimum_required(VERSION 3.29)
project(EncryptSqlClient)
set(CMAKE_CXX_STANDARD 20)
include_directories(./include DIR_INC)
link_directories(./lib)

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
        src/Sql/SqlPlanExecutor.h
        src/Sql/SqlQueryGenerator.cpp
        src/Sql/TokenGenerator.cpp
        src/DO/SqlPlan.cpp
        src/Sql/SqlPlanExecutor.cpp
)

target_link_libraries(EncryptSqlClient libseal-4.1.a libz.a libzstd.a libssl.a libcrypto.a libpq.a)

#target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libssl.3.dylib /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libcrypto.dylib)
#target_link_libraries(EncryptSqlClient /usr/local/mysql-connector-c++-9.0.0/lib64/libmysqlcppconn.10.dylib)
#target_link_libraries(EncryptSqlClient /usr/local/pgsql/lib/libpq.5.14.dylib)
#target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/seal/4.1.2/lib/libseal.4.1.2.dylib)


