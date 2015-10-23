all: main.cpp
	colorgcc -std=c++11 -I/usr/include -L/usr/lib/x86_64-linux-gnu -o Server main.cpp -lstdc++ -lSDL2 -lSDL2_net

run: Server
	./Server

clean:
	rm Server
