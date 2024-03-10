CC = gcc

SRC = main.c ssp_window.c ssp_render.c ssp_shader.c ssp_image_loader.c ssp_observer.c
TEST_SRC = ssp_list_test.cpp ssp_list.c
INC = 
LIB = -llogger_static
LIB_PATH = 
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/devil/lib

ifeq ($(OS),Windows_NT)
	INC += -IC:/dev/logger/logger -IC:/dev/glfw/include
	LIB += -lglfw3 -lgdi32 -lopengl32
	LIB_PATH += -LC:/dev/logger/build/logger -LC:/dev/glfw/build/src
else
	OS = $(shell uname)
	ifeq ($(OS),Darwin)
		INC += -I/opt/logger/include -I/opt/homebrew/Cellar/glfw/3.4/include
		LIB += -lglfw
		LIB_PATH += -L/opt/logger/lib -L/opt/homebrew/Cellar/glfw/3.4/lib
	endif
	ifeq ($(OS),Linux)
		INC += -I/opt/logger/include -I/opt/glfw/include -I/opt/devil/include
		LIB += -lglfw3 -lm -lwayland-client -lwayland-egl -lGLESv2 -lEGL -lIL 
		LIB_PATH += -L/opt/logger/lib -L/opt/glfw/lib -L/opt/devil/lib
	endif	
endif

all: $(SRC)
	$(CC) $^ -o ssp $(INC) $(LIB_PATH) $(LIB)

tests: $(TEST_SRC)
	g++ $^ -o ssp_list_test -I/opt/gtest/include  -L/opt/gtest/lib -lgtest -lgtest_main