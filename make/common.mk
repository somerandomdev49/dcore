#### COMMON PROJECT SETTINGS FOR ALL PLATFORMS ####
BIN_NAME := dcore
CXX = g++-10
DESTDIR = /
SRC_EXT = cpp
SRC_PATH = src
INCLUDES = -Iinclude -I3rd-party/loguru -I3rd-party/include -Iinclude/gsl

# NO_WARNS = unused-private-field
NO_WARNS = 
COMPILE_FLAGS = -std=c++17 -Wall -Wextra -DLOGURU_WITH_STREAMS ${addprefix -Wno-,$(NO_WARNS)} -DDCORE_STD_64BIT
RCOMPILE_FLAGS = -D NDEBUG
DCOMPILE_FLAGS = -D DEBUG # -rdynamic -g
