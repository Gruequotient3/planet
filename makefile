PROG = main

CC = g++
CXXFLAGS = -Wall -Wextra -O3
CXXFLAGS += -Ilib/glad/include
CXXFLAGS += -Ilib/glm/
CXXFLAGS += -Ilib/glfw/include
CXXFLAGS += -Ilib/stb
CXXFLAGS += $(shell sdl2-config --cflags)

LDFLAGS = -lm
LDFLAGS += -lX11 -lpthread -lXrandr -lXi -ldl -lSDL2_ttf
LDFLAGS += -lglad -lstb_image -lglfw3 -lglm
LDFLAGS += -Llib/glad/
LDFLAGS += -Llib/stb/
LDFLAGS += -Llib/glfw/src/
LDFLAGS += -Llib/glm/glm/
LDFLAGS += $(shell sdl2-config --libs) -lSDL2_mixer

# Traitement automatique (ne pas modifier)
ifneq (,$(shell ls -d /usr/local/include 2>/dev/null | tail -n 1))
	CPPFLAGS += -I/usr/local/include
endif
ifneq (,$(shell ls -d $(HOME)/local/include 2>/dev/null | tail -n 1))
	CPPFLAGS += -I$(HOME)/local/include
endif
ifneq (,$(shell ls -d /usr/local/lib 2>/dev/null | tail -n 1))
	LDFLAGS += -L/usr/local/lib
endif
ifneq (,$(shell ls -d $(HOME)/local/lib 2>/dev/null | tail -n 1))
	LDFLAGS += -L$(HOME)/local/lib
endif

ifeq ($(shell uname), Darwin)
	MACOSX_DEPLOYMENT_TARGET = 11.0
        CFLAGS += -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
        LDFLAGS += -framework OpenGL -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET)
else
	LDFLAGS += -lGL
endif

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
