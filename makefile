CC = gcc
CFLAGS = -Wall -Wextra -Wno-misleading-indentation -Oh -ggdb -fsanitize=address -static-libasan -ggdb
RAYLIBFLAGS = -Iraylib/include raylib/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
TARGET = main
OBJS = main.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(RAYLIBFLAGS) $(CFLAGS) -o $(TARGET)

main.o: main.c
	$(CC)  -c main.c



emcc:
	emcc -Os -std=c99 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 \
	main.c  -Iraylib/include raylib/lib/libraylib_wasm.a\
	 -o index.html




.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
