PROG = main

CC = g++
CXXFLAGS = -Wall -Wextra -g -O3

CXXFLAGS += -Ilib/glad/include
CXXFLAGS += -Ilib/glm/
CXXFLAGS += -Ilib/glfw/include
CXXFLAGS += -Ilib/stb

LDFLAGS = -lm
LDFLAGS += -lGL -lX11 -lpthread -lXrandr -lXi -ldl

LDFLAGS += -Llib/glad/ -lglad
LDFLAGS += -Llib/stb/ -lstb_image
LDFLAGS += -Llib/glfw/src/ -lglfw3
LDFLAGS += -Llib/glm/glm/ -lglm
LDFLAGS += -Llib/glfw/src/ -lglfw3


CXXSRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)

OBJ = $(CXXSRC:.cpp=.o)

RM = rm -rf

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o main $(CXXFLAGS) $^ $(LDFLAGS)

libs:
	@cd lib/glm && cmake . -DCGLM_STATIC=ON && make
	@cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	@cd lib/glfw && cmake . && make
	@cd lib/stb && $(CC) -c stb_image.cpp -o stb_image.o

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(OBJ) $(PROG)
