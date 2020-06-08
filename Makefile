build: clean client server
	@echo "Done."

client:
	gcc client.c -o tst/client
	@echo "Done."

server:
	gcc server.c -o ../serverFiles/server
	@echo "Done."

clean:
	rm -f client
	rm -f ../serverFiles/server
	rm -f tst/client
	@echo "Done."