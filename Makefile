build: clean client server
	@echo "Done."

client:
	gcc client.c -o client

server:
	gcc server.c -o server

clean:
	rm -f client
	rm -f server
	@echo "Cleaned."