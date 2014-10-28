//=============================================================
// Name        : GomokuClient.cpp
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-10-27
// Description : Implementation of class GomokuClient
//=============================================================
#include "GomokuClient.h"


GomokuClient::GomokuClient()
{
    try
    {
        //Creates a TCPSocket object
        mp_tcpsocket = new TCPSocket();

        //Connects to server
        mp_tcpsocket->server_connect("luffarserv.miun.se", 6500);
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}


GomokuClient::~GomokuClient()
{
    delete mp_tcpsocket;
}


void GomokuClient::Run()
{
    char return_message[100];
    char send_message[] = "CHA:0:0:07:Sasamus\n";


    int byteswritten =
        write(mp_tcpsocket->get_descriptor(),
        send_message, sizeof(send_message));
    std::cout << "<- " << send_message;
    
    int bytesread =
        read(mp_tcpsocket->get_descriptor(),
        return_message, sizeof(return_message));
    std::cout << "-> " << return_message;

}
