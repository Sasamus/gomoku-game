//=============================================================
// Name         : main.cpp
// Author       : Albin Engström
// Created      : 2014-10-23
// Modified     : 2014-10-24
// Description  : main for project gomoku_game
// Purpose      : Start the program
//=============================================================
#include "TCPSocket.h"

#include <iostream>

using namespace std;

int main()
{

    try
    {
        //Creates a TCPSocket object
        TCPSocket tcpsocket;

        //Connects to server
        tcpsocket.server_connect("luffarserv.miun.se", 6500);
    }
    catch (exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
