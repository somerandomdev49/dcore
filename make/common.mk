#### COMMON PROJECT SETTINGS FOR ALL PLATFORMS ####
BIN_NAME := dcore
CXX = g++-10
DESTDIR = /
SRC_EXT = cpp
SRC_PATH = src
INCLUDES = -Iinclude -I3rd-party/loguru -I3rd-party/include
RCOMPILE_FLAGS = -D NDEBUG
DCOMPILE_FLAGS = -D DEBUG -rdynamic -g
