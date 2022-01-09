#CC=aarch64-linux-g++
CC=gcc
#CC=g++
LN=ln

#execute the makefile in release last
SUBDIRS=$(shell ls -l|grep ^d|awk '{if($$9 != "release"&&$$9 != "include"&&$$9!="lib") print $$9}')
DEBUG_VERSION=1
ROOT_DIR=$(shell pwd)

ifeq ($(DEBUG_VERSION),1) 
BIN=jcenter_bin_debug
MEMDEBUG= -fsanitize=address -fno-omit-frame-pointer -lrt -lasan
DEBUGOPT=-g -DDEBUG_VERSION -fsanitize=address -fno-omit-frame-pointer
RMBIN=jcenter_bin
else
MEMDEBUG=
BIN=jcenter_bin
RMBIN=jcenter_bin_debug
DEBUGOPT=
endif

OBJS_DIR=release/obj
LIB_DIR=lib/
BIN_DIR=release/bin
CUR_SOURCE=${wildcard *.c}
CUR_SOURCE+=$(wildcard log/*.c)
#CUR_SOURCE := $(filter-out video/test.c, $(CUR_SOURCE))
CUR_OBJS=${patsubst %.c,%.o,$(CUR_SOURCE)}
CUR_OBJS_PATH=$(addprefix $(OBJS_DIR)/,$(CUR_OBJS))
LDIR=lib/


CC_OPT=-Iinclude -Iinclude/log $(DEBUGOPT)
#x64 link with imi
LN_OPT=-L$(LIB_DIR) -L$(LDIR) -lpthread -ldl $(MEMDEBUG)


export CC BIN OBJS_DIR BIN_DIR ROOT_DIR

#excute each makefile
all:$(SUBDIRS)  $(CUR_OBJS)
	rm -f $(BIN_DIR)/$(RMBIN)
	$(CC) -g -o $(ROOT_DIR)/$(BIN_DIR)/$(BIN) $(CUR_OBJS)  $(LN_OPT)

$(SUBDIRS):
		make -C $@

$(CUR_OBJS):%.o:%.c
		$(CC) $(CC_OPT) -c $^ -o $@

clean:
		@rm -f *.o
		@rm -f common/*.o
		@rm -f $(OBJS_DIR)/*.o
		@rm -rf $(BIN_DIR)/*
