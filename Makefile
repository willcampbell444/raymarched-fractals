IMGUI_DIR = imgui
SOURCES = $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addprefix build/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

LIBS = -lGL -lglfw -lGLEW -DGLEW_STATIC -lstdc++fs 

CC=g++ -std=c++17 -g -pthread 

all: build/main.o build/shader.o $(OBJS)
    #$(CC) $^ $(CXXFLAGS) $(LIBS)
	$(CC) build/shader.o build/main.o $(OBJS) $(LIBS)

build/shader.o: src/shader.cpp src/shader.h imgui build
	$(CC) -c src/shader.cpp -o build/shader.o

build/main.o: src/main.cpp src/shader.h imgui build
	$(CC) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/%.o:$(IMGUI_DIR)/%.cpp imgui build
	$(CC) $(CXXFLAGS) -c -o $@ $<

build/%.o:$(IMGUI_DIR)/backends/%.cpp imgui build
	$(CC) $(CXXFLAGS) -c -o $@ $<

imgui:
	git clone https://github.com/ocornut/imgui.git

build:
	mkdir build

clean:
	rm -r build
