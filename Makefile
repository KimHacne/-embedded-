CC = gcc
CMDS = game driver
OBJS = driver.o

all : $(CMDS)

game : main.c
	gcc -o game main.c -lpthread

driver : driver.o
	gcc -o driver driver.o

driver.o : driver.c
	gcc -c driver.c 
