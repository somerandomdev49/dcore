#### PROJECT SETTINGS FOR WSL (GCC >=9) ####
$(info including common_unix.mk)
include make/common_unix.mk

COMPILE_FLAGS += -ggdb3
LINK_FLAGS = -lglfw -ldl -lpthread -std=c++17
RLINK_FLAGS = 
DLINK_FLAGS = -rdynamic -g -ggdb3
