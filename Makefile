CC = gcc

SRC = main.c ssp_window.c
INC = 
LIB = -llogger_static
LIB_PATH = 
# gcc main_triangle.c -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL
ifeq ($(OS),Windows_NT)
	INC += -IC:/dev/logger/logger -IC:/dev/glfw/include
	LIB += -lglfw3 -lgdi32 -lopengl32
	LIB_PATH += -LC:/dev/logger/build/logger -LC:/dev/glfw/build/src
else
	OS = $(shell uname)
	ifeq ($(OS),Darwin)
		INC += -I/opt/logger/include -I/opt/homebrew/include
		LIB += -lglfw  -lGLEW -framework OpenGL
		LIB_PATH += -L/opt/logger/lib -L/opt/homebrew/lib
	endif
endif

all: $(SRC)
	$(CC) $^ -o ssp $(INC) $(LIB_PATH) $(LIB)