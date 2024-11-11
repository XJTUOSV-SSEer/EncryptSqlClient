MODULE_big = calc_cpp_demo
EXTENSION = calc_cpp_demo
DATA = calc_cpp_demo--1.0.sql

PG_CONFIG = pg_config
# 使用 C++ 编译器
PG_CXXFLAGS = -std=c++17
PG_LIBDIR = /opt/homebrew/Cellar/seal/4.1.2/lib/
CC = g++ -std=c++17 -I/usr/local/include -I/Library/Developer/CommandLineTools/SDKs/MacOSX15.0.sdk/usr/include

# 编译参数
CFLAGS = -fPIC $(PG_CXXFLAGS)

# 链接参数
CFLAGS_SL +=  -fPIC
PG_CPPFLAGS +=  -fPIC
SHLIB_LINK += -lstdc++ -lcurl -lseal -lssl -lcrypto

OBJS += calc_cpp_demo.o Crypto_Primitives.o enc_utils.o
# 包含 PostgreSQL 的标准 Makefile 规则

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
