build: clean client server
	@echo "Done."

client:
	gcc client.c -o client
	@echo "Done."

server:
	gcc server.c -o server
	@echo "Done."

test:
	gcc test.c -o tst/test
	@echo "Done."

clean:
	rm -f client
	rm -f server
	@echo "Done."