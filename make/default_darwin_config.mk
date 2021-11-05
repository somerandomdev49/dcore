#### PROJECT SETTINGS FOR DARWIN (GNU GCC >=9) ####
BIN_NAME := dcore
CXX = g++-10
SRC_EXT = cpp
SRC_PATH = src
LIBS = freetype2 nlohmann_json # pkg-config files
COMPILE_FLAGS = -std=c++17 -Wall -Wextra -DLOGURU_WITH_STREAMS -rdynamic -g
RCOMPILE_FLAGS = -D NDEBUG
DCOMPILE_FLAGS = -D DEBUG -rdynamic -g
INCLUDES = -Iinclude -I3rd-party/loguru -I3rd-party/include
LINK_FLAGS = -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -std=c++17 -rdynamic -g
RLINK_FLAGS = 
DLINK_FLAGS = -rdynamic -g
DESTDIR = /
INSTALL_PREFIX = usr/local
