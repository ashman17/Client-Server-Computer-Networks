.PHONY: server client clean FORCE

server: FORCE
	gcc -o server server.c
	./server 127.0.0.1 8000

client: FORCE
	gcc -o client client.c
	./client 127.0.0.1 8000

clean: FORCE
	rm -f server client

FORCE: ;