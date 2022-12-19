CC = gcc -Wall 
LIBS = -lpthread -lncurses 
OBJ = main.o graphics.o game.o gamestring.o outmanager.o gamemanager.o

all:	runner

runner: ${OBJ}
	${CC} -g -o runner ${OBJ} ${LIBS}

%.o: %.c defs.h
	${CC} -c $<

clean: 
	rm runner ${OBJ}

cleanOBJ:
	rm ${OBJ}