OS = $(shell uname)
CC = gcc

SRC = main.c ssp_window.c
INC = 
LIB = -llogger_static
LIB_PATH = 

ifeq ($(OS),Darwin)
	INC += -I/opt/logger/include -I/opt/homebrew/Cellar/glfw/3.4/include
	LIB += -lglfw
	LIB_PATH += -L/opt/logger/lib -L/opt/homebrew/Cellar/glfw/3.4/lib
endif

all:
	$(CC) $(SRC) $(INC) $(LIB_PATH) $(LIB)
	./a.out