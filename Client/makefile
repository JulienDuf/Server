all: main.cpp
	colorgcc -std=c++11 -I/usr/include -L/usr/lib/x86_64-linux-gnu -o Engine main.cpp -lstdc++ -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lGL

run: Engine
	./Engine

clean: Engine
	rm Engine

install:
	apt-get install build-essential
	apt-get install colorgcc
	apt-get install libsdl2-dev
	apt-get install libsdl2-ttf-dev
	apt-get install libsdl2-image-dev
	apt-get install libsdl2-mixer-dev
	apt-get install libsdl2-net-dev
	mvdir GL3 /usr/include
