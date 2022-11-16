CC = gcc
SRC = src/main.c src/tools.c src/thread_task.c
TAGS = -o piTest -lpthread

build: $(SRC)
	@$(CC) $(SRC) $(TAGS)

debug: $(SRC)
	@$(CC) $(SRC) $(TAGS) -g
	@gdb --args ./piTest 1000000

run: build
	@./piTest $(STRENGTH)

clean:
	rm piTest

.PHONY: clean, run