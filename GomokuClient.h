//=============================================================
// Name        : GomokuClient.h
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-10-27
// Description : Definition of class GomokuClient
// Purpose     : A TCP Socket that is used from a client
//=============================================================
#include "TCPSocket.h"

#include <stdexcept>
#include <iostream>


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


};
