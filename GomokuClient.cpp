//=============================================================
// Name        : GomokuClient.cpp
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-10-29
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
        mp_tcpsocket->connect("luffarserv.miun.se", 6500);
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
    //Variables to hold the messages sent and recieved
    char return_message[100];
    std::string send_message;

    while(1<2)
    {
        //Get input
        std::cout << "<- ";
        std::cin >> send_message;

        //Creates a char array for the message
        char send_message_char[send_message.size() + 1];

        //Copies the message from send_message to send_message_char
        std::strcpy(send_message_char, send_message.c_str());

        //Adds a newline character to send_message_char
        send_message_char[sizeof(send_message_char) -2] = '\n';

        //Sends the message to a file descriptor
        write(mp_tcpsocket->get_descriptor(),
            send_message_char, sizeof(send_message_char));

        //Reads the return message from a file descriptor and prints it to screen
        while(read(mp_tcpsocket->get_descriptor(),
            return_message, sizeof(return_message)) == 0){}

        std::cout << "-> " << return_message;

        //Break loop if quiting
        if(send_message == "QUI")
        {
            break;
        }

    }



}
