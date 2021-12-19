#### PROJECT SETTINGS FOR WINDOWS (MinGW32-w64) ####
BIN_NAME := dcore
CXX = g++
SRC_EXT = cpp
SRC_PATH = src
LIBS = freetype2 nlohmann_json
COMPILE_FLAGS = -std=c++17 -Wall -Wextra -DLOGURU_WITH_STREAMS -g
RCOMPILE_FLAGS = -D NDEBUG
DCOMPILE_FLAGS = -D DEBUG -rdynamic -g
INCLUDES = -Iinclude -I3rd-party/loguru -I3rd-party/include
LINK_FLAGS = -lglfw -ldl -lpthread -std=c++17 -g
RLINK_FLAGS = 
DLINK_FLAGS = -g
DESTDIR = /
INSTALL_PREFIX = usr/local
