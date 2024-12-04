cmake_minimum_required(VERSION 3.29)



# vcpkg配置，必须位于project之前
if(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE PATH "") # 从环境变量中读取vcpkg根目录
else()
    set(VCPKG_ROOT "/home/syf/vcpkg" CACHE PATH "")
endif ()

message("set VCPKG_ROOT: ${VCPKG_ROOT}")
#set(VCPKG_FEATURE_FLAGS "version") # 用于支持自定义依赖版本
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

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
        src/dataObject/EncryptedMultiMap.cpp
        src/dataObject/EncryptedMultiMap.h
        src/dataObject/RowMultiMap.cpp
        src/dataObject/RowMultiMap.h
        src/main.h
        src/EncryptManager.cpp
        src/EncryptManager.h
        src/testsArchive.cpp
        src/testsArchive.h
        src/Sql/SqlPlanExecutor.h
        src/Sql/SqlQueryGenerator.cpp
        src/Sql/TokenGenerator.cpp
        src/dataObject/SqlPlan.cpp
        src/Sql/SqlPlanExecutor.cpp
        src/EncryptService.h
        src/EncryptService.cpp
        src/common/result/result.h
        src/SocketServer/msgSocket.cpp
        src/SocketServer/myMsg.cpp
        src/paser/paser.cpp
        src/paser/paser.h
        src/dataObject/Table.cpp
)

find_package(PostgreSQL REQUIRED)
target_link_libraries(EncryptSqlClient PRIVATE PostgreSQL::PostgreSQL)
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(EncryptSqlClient PRIVATE JsonCpp::JsonCpp)
find_package(SEAL CONFIG REQUIRED)
target_link_libraries(EncryptSqlClient PRIVATE SEAL::seal)
find_package(OpenSSL REQUIRED)
target_link_libraries(EncryptSqlClient PRIVATE OpenSSL::SSL)
target_link_libraries(EncryptSqlClient PRIVATE OpenSSL::Crypto)
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(EncryptSqlClient PRIVATE Drogon::Drogon)
#target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libssl.3.dylib /opt/homebrew/Cellar/openssl@3/3.3.2/lib/libcrypto.dylib)
#target_link_libraries(EncryptSqlClient /usr/local/mysql-connector-c++-9.0.0/lib64/libmysqlcppconn.10.dylib)
#target_link_libraries(EncryptSqlClient /usr/local/pgsql/lib/libpq.5.14.dylib)
#target_link_libraries(EncryptSqlClient /opt/homebrew/Cellar/seal/4.1.2/lib/libseal.4.1.2.dylib)


