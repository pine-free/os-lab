client: src/client.c
	mkdir -p bin
	gcc -o bin/client src/client.c src/common.c src/messages.c

server: src/server.c
	mkdir -p bin
	gcc -o bin/server src/server.c src/common.c src/messages.c

all: client server

test: all
	./bin/client & ./bin/server

test-debug: all
	DEBUG=1 ./bin/client & DEBUG=1 ./bin/server
