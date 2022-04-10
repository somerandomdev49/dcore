#### PROJECT SETTINGS FOR WSL (GCC >=9) ####
include make/common_unix.mk

COMPILE_FLAGS = ${COMPILE_FLAGS} -ggdb3
LINK_FLAGS = -lglfw -ldl -lpthread -std=c++17
LIBS = ${LIBS} uuid
RLINK_FLAGS = 
DLINK_FLAGS = -rdynamic -g -ggdb3
