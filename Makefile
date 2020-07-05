  
CC = g++
OBJS = $(wildcard */*.cpp)
INCLUDE_PATHS = -I./include -I/usr/local/include -I/usr/local/Cellar/sdl2_image/2.0.4/include -I/usr/local/Cellar/glfw/3.3.2/include -I/usr/local/Cellar/sdl_mixer/1.2.12_3/include/SDL -I/usr/local/Cellar/sdl_ttf/2.0.11_1/include/SDL
LIBRARY_PATHS = -L/usr/local/lib
COMPILER_FLAGS = -Wall -Wextra -Werror -std=c++11
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL_mixer -lSDL_ttf
TARGET = game

all:
	$(CC) -o $(TARGET) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(OBJS)

clean:
	$(RM) $(TARGET)

.PHONY: clean all
