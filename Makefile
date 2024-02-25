SRC = main.c ssp_display.c ssp_egl.c shader.c image_loader.c

INCLUDE = -I/opt/logger/include
LIB_PATH = -L/opt/logger/lib
LIB = -llogger_static -lwayland-client -lwayland-egl -lGLESv2 -lEGL -ljpeg

all:
	gcc $(SRC) $(INCLUDE) $(LIB_PATH) $(LIB)

clean:
	rm -rf *.o *.out
