CC 			= g++
CCFLAGS		= -std=c++0x -Wall -Wextra
PROGS		= gomoku_game
LIBS		=
FILES		= main.cpp TCPSocket.cpp
OBJS		= main.o TCPSocket.o


all: compile gomoku_game

gomoku_game: $(OBJS)
	$(CC) -o $@ $(CCFLAGS) $(OBJS) $(LIBS)

compile: $(FILES)
	$(CC) -c $(CCFLAGS) $(FILES)

clean:
	rm -f $(PROGS) $(OBJS)
