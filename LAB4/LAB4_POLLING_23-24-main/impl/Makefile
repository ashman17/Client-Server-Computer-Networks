.PHONY: server client clean FORCE

server:
	gcc -o server server.c
	gcc -o client client.c
	./server 127.0.0.1 8000

client:
	./client 127.0.0.1 8000 $(USR)

clean:
	rm -f server client 

FORCE: ;