#### PROJECT SETTINGS FOR DARWIN (GNU GCC >=9) ####
include common.mk
include common_unix.mk

COMPILE_FLAGS = -std=c++17 -Wall -Wextra -DLOGURU_WITH_STREAMS
LINK_FLAGS = -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -std=c++17
RLINK_FLAGS = 
DLINK_FLAGS = -rdynamic -g
