client: src/client.c
	mkdir -p bin
	gcc -o bin/client src/client.c src/common.c src/sockets.c src/debug.c

server: src/server.c
	mkdir -p bin
	gcc -o bin/server src/server.c src/common.c src/sockets.c src/debug.c

all: client server

teardown: 
	sleep 2
	killall server
	rm unix_socket

run-test: all
	./bin/server & ./bin/client

test: run-test teardown
