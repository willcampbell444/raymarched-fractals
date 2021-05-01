IMGUI_DIR = imgui
SOURCES = $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addprefix build/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends

LIBS = -lGL -lglfw -lGLEW -DGLEW_STATIC -lstdc++fs 

CC=g++ -std=c++17 -g -pthread 

#$(info $(shell mkdir -p build))
$(shell mkdir -p build)

all: build/main.o build/shader.o build/options.o $(OBJS)
    #$(CC) $^ $(CXXFLAGS) $(LIBS)
	$(CC) build/options.o build/shader.o build/main.o $(OBJS) $(LIBS)

build/shader.o: src/shader.cpp src/shader.h
	$(CC) -c src/shader.cpp -o build/shader.o

build/options.o: src/options.cpp src/options.h
	$(CC) $(CXXFLAGS) -c src/options.cpp -o build/options.o

build/main.o: src/main.cpp src/shader.h imgui
	$(CC) $(CXXFLAGS) -c src/main.cpp -o build/main.o

build/%.o: $(IMGUI_DIR)/%.cpp imgui
	$(CC) $(CXXFLAGS) -c -o $@ $<

build/%.o: $(IMGUI_DIR)/backends/%.cpp imgui
	$(CC) $(CXXFLAGS) -c -o $@ $<

imgui:
	git clone https://github.com/ocornut/imgui.git

clean:
	rm -r build
