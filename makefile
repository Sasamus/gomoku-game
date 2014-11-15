CC 			= g++
CCFLAGS		= -std=c++0x -Wall -Wextra
PROGS		= gomoku_game
LIBS		= -pthread
FILES		= main.cpp TCPSocket.cpp GomokuClient.cpp
OBJS		= main.o TCPSocket.o GomokuClient.o
GCH			= GomokuClient.h.gch

all: compile gomoku_game

gomoku_game: $(OBJS)
	$(CC) $(CCFLAGS) -o $@  $(OBJS) $(LIBS)

compile: $(FILES)
	$(CC) $(CCFLAGS) -c $(FILES)

clean:
	rm -f $(PROGS) $(OBJS) $(GCH)
