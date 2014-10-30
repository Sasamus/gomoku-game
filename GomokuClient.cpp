//=============================================================
// Name        : GomokuClient.cpp
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-10-30
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

    //Creates 2 dimensinal vectors to represent the players moves
    std::vector< std::vector<bool> > players_board(15, std::vector<bool>(15));
    std::vector< std::vector<bool> > ais_board(15, std::vector<bool>(15));

    //Sets all the locations on the boards to false
    for(unsigned int i=0; i < 15; i++)
    {
        for(unsigned int j=0; j < 15; j++)
        {
            players_board[i][j] = false;
            ais_board[i][j] = false;
        }
    }

    //Prints the board to screen
    std::cout << "   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14" << std::endl;
    for(unsigned int i=0; i < 15; i++)
    {
        if(i < 10)
        {
            std::cout << "0" << i;
        }
        else
        {
            std::cout << i;
        }

        for(unsigned int j=0; j < 15; j++)
        {
            if(players_board[i][j])
            {
                std::cout << "  " << "X";
            }
            else if(ais_board[i][j])
            {
                std::cout << "  " << "O";
            }
            else
            {
                std::cout << "  " << "-";
            }
        }
        std::cout << std::endl;
    }



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
