//=============================================================
// Name        : GomokuClient.h
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-11-14
// Description : Definition of class GomokuClient
// Purpose     : A TCP Socket that is used from a client
//=============================================================
#include "TCPSocket.h"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <mutex>
#include <exception>


class GomokuClient
{
private:
    //A TCPSocket pointer
    TCPSocket *mp_tcpsocket;

    //Creates 2 dimensinal vectors to represent the players moves
    std::vector< std::vector<bool> > m_player_board;
    std::vector< std::vector<bool> > m_ai_board;

    //Used to make sure cout usage is atomic
    std::mutex m_cout_mutex;

    //Variable to keep track of if program shoudl quit
    bool m_quit = false;

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

    void PrintBoard(std::vector< std::vector<bool> > a_m_player_board,
                    std::vector< std::vector<bool> > a_m_ai_board);
    //Pre:
    //Post: Prints the board to screen

    void getMove(char *message, int &x, int &y);
    //Pre:
    //Post: Sets x and y according to the message

    void ListenToServer();
    //Pre:
    //Post: Reacts to messages from the server

    //A Wrapper allowing ListenToServer() to be called by a thread
    /*
    static void StaticWrapper_ListenToServer(void *o)
    {
        GomokuClient *gomokuclient = static_cast<GomokuClient*>(o);
        gomokuclient->ListenToServer();
    }
    */
};
