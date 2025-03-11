PROG = main

CC = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++11

CXXFLAGS += -Ilib/glad/include
CXXFLAGS += -Ilib/glm/
CXXFLAGS += -Ilib/glfw/include
CXXFLAGS += -Ilib/stb

LDFLAGS = -lm
LDFLAGS += -lGL -lX11 -lpthread -lXrandr -lXi -ldl
LDFLAGS += -lglad -lstb_image -lglfw3 -lglm
LDFLAGS += -Llib/glad/
LDFLAGS += -Llib/stb/
LDFLAGS += -Llib/glfw/src/
LDFLAGS += -Llib/glm/glm/


CXXSRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)

OBJ = $(CXXSRC:.cpp=.o)

RM = rm -rf

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o main $(OBJ) $(LDFLAGS)

libs:
	@cd lib/glm && cmake . -DCGLM_STATIC=ON && make
	@cd lib/glfw && cmake . && make

%.o: %.cpp %.hpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(PROG)
