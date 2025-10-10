client: src/client.c
	mkdir -p bin
	gcc -o bin/client src/client.c src/common.c src/debug.c

server: src/server.c
	mkdir -p bin
	gcc -o bin/server src/server.c src/common.c src/debug.c

all: client server

teardown:
	sleep 1
	killall server

run-test: all
	./bin/server & ./bin/client


test: run-test teardown
