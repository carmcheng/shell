#Makefile for Project 4

all:
	gcc -Wall -Werror -o shell shell.c
	
clean:
	rm shell