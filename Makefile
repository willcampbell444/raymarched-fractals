CC=g++ -g -pthread

all: build/main.o build/shader.o
	$(CC) build/shader.o build/main.o -lGL -lglfw -lGLEW -DGLEW_STATIC

build/shader.o: src/shader.cpp src/shader.h
	$(CC) -c shader.cpp -o build/shader.o

build/main.o: src/main.cpp src/shader.h
	$(CC) -c src/main.cpp -o build/main.o

clean:
	rm build/*
