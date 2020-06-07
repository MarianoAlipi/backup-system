build:
	gcc client.c -o client
	@echo "Done."

clean:
	rm client
	@echo "Done."