//=============================================================
// Name        : GomokuClient.cpp
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-10-26
// Description : Implementation of class GomokuClient
//=============================================================
#include "GomokuClient.h"


GomokuCLient::GomokuClient()
{
    try
    {
        //Creates a TCPSocket object
        mp_tcpsocket = new TCPSocket();

        //Connects to server
        tcpsocket->server_connect("luffarserv.miun.se", 6500);
    }
    catch (exception &e)
    {
        cout << "Error: " << e.what() << endl;
    }
}


GomokuCLient::~GomokuClient()
{
    delete mp_tcpsocket;
}


void GomokuCLient::Run()
{
    
}
