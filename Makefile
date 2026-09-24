CC = gcc
CFLAGS = -Wall -Wextra

all: my_shell project

my_shell: src/shell.c
	$(CC) $(CFLAGS) src/shell.c -o my_shell

project: src/project.c
	$(CC) $(CFLAGS) src/project.c -o project -pthread

clean:
	rm -f my_shell project
