//=============================================================
// Name        : GomokuClient.cpp
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-11-01
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

void GomokuClient::PrintBoard(std::vector< std::vector<bool> > a_player_board,
                                std::vector< std::vector<bool> > a_ai_board)
{
    //Prints the board to screen
    std::cout << "   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14" << std::endl;
    for(unsigned int i=0; i < a_player_board.size(); i++)
    {
        if(i < 10)
        {
            std::cout << "0" << i;
        }
        else
        {
            std::cout << i;
        }

        for(unsigned int j=0; j < a_player_board.size(); j++)
        {
            if(a_player_board[i][j])
            {
                std::cout << "  " << "X";
            }
            else if(a_ai_board[i][j])
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

}

void GomokuClient::getMove(char *a_message, int &x, int &y)
{
    if(a_message[4] - '0' == 0)
    {
        x = a_message[5] - '0';
    }
    else
    {
        x = (a_message[4] - '0') * 10 + (a_message[5] - '0');
    }

    if(a_message[7] - '0' == 0)
    {
        y = a_message[8] - '0';
    }
    else
    {
        y = (a_message[7] - '0') * 10 + (a_message[8] - '0');
    }
}


void GomokuClient::Run()
{
    //Variables to hold the messages sent and recieved
    char return_message[100];
    std::string send_message;

    //Creates 2 dimensinal vectors to represent the players moves
    std::vector< std::vector<bool> > player_board(15, std::vector<bool>(15));
    std::vector< std::vector<bool> > ai_board(15, std::vector<bool>(15));

    //Sets all the locations on the boards to false
    for(unsigned int i=0; i < 15; i++)
    {
        for(unsigned int j=0; j < 15; j++)
        {
            player_board[i][j] = false;
            ai_board[i][j] = false;
        }
    }

    //Prints the board with PrintBoard()
    PrintBoard(player_board, ai_board);


    while(1<2)
    {
        //Get input
        std::cout << "<- ";
        std::cin >> send_message;

        //Creates a char array for the message
        char char_send_message[send_message.size() + 1];

        //Copies the message from send_message to char_send_message
        std::strcpy(char_send_message, send_message.c_str());

        //Adds a newline character to char_send_message
        char_send_message[sizeof(char_send_message) -1] = '\n';

        //Sends the message to a file descriptor
        write(mp_tcpsocket->get_descriptor(),
            char_send_message, sizeof(char_send_message));

        //Checks if a move was made, if so, print board
        if(char_send_message[0] == 'M' && char_send_message[1] == 'O' &&
            char_send_message[2] == 'V' && char_send_message[3] == ':' )
        {
            //Variables to hold the move coordinates
            int x, y;

            //Gets the coordiniates
            getMove(char_send_message, x, y);

            //Adds the move to player_board
            player_board[x][y] = true;

            //Prints the board with PrintBoard()
            PrintBoard(player_board, ai_board);

        }

        //Reads the return message from a file descriptor and prints it to screen
        while(read(mp_tcpsocket->get_descriptor(),
            return_message, sizeof(return_message)) == 0){}

        std::cout << "-> " << return_message;

        //Checks if a move was made, if so, print board
        if(return_message[0] == 'M' && return_message[1] == 'O' &&
            return_message[2] == 'V' && return_message[3] == ':' )
        {
            //Variables to hold the move coordinates
            int x, y;

            //Gets the coordiniates
            getMove(return_message, x, y);

            //Adds the move to ai_board
            ai_board[x][y] = true;

            //Prints the board with PrintBoard()
            PrintBoard(player_board, ai_board);

        }



        //Break loop if quiting
        if(send_message == "QUI")
        {
            break;
        }

    }



}
