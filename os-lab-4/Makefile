client: src/client.c
	mkdir -p bin
	gcc -o bin/client src/client.c src/common.c src/debug.c

server: src/server.c
	mkdir -p bin
	gcc -o bin/server src/server.c src/common.c src/debug.c

all: client server

test: all
	./bin/server & ./bin/client
	sleep 1
	killall server

test-debug: all
	DEBUG=1 ./bin/server & DEBUG=1 ./bin/client
	sleep 1
	killall server
