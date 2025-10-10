client: src/client.c
	mkdir -p bin
	gcc -o bin/client src/client.c src/common.c src/messages.c

server: src/server.c
	mkdir -p bin
	gcc -o bin/server src/server.c src/common.c src/messages.c

all: client server

define FILE1
lol
kek
cheburek
endef

define FILE2
kek
awk
arbidol
endef

export FILE1
export FILE2

setup:
	echo "$$FILE1" > file1.sh
	echo "$$FILE2" > file2.sh

teardown:
	rm file1.sh file2.sh
	sleep 1
	killall server

run-test: all
	./bin/server & ./bin/client

test: setup run-test teardown
