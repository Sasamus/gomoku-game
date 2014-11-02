//=============================================================
// Name        : GomokuClient.h
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-11-01
// Description : Definition of class GomokuClient
// Purpose     : A TCP Socket that is used from a client
//=============================================================
#include "TCPSocket.h"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>


class GomokuClient
{
private:
    //A TCPSocket pointer
    TCPSocket *mp_tcpsocket;


public:

    GomokuClient();
    //Pre:
    //Post: Establishes a connection to the server

    ~GomokuClient();
    //Pre:
    //Post: Closes the connection to the server

    void Run();
    //Pre:
    //Post: Runs a game of gomoku

    void PrintBoard(std::vector< std::vector<bool> > a_player_board,
                    std::vector< std::vector<bool> > a_ai_board);
    //Pre:
    //Post: Prints the board to screen

    void getMove(char *message, int &x, int &y);
    //Pre:
    //Post: Sets x and y according to the message


};
