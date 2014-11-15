//=============================================================
// Name        : GomokuClient.cpp
// Author      : Albin Engström
// Created     : 2014-10-26
// Modified    : 2014-11-14
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
        mp_tcpsocket->connect("luffarserv.miun.se", 6501);
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    //Creates 2 dimensinal vectors to represent the players moves
    m_player_board
    = std::vector< std::vector<bool> > (15, std::vector<bool>(15));
    m_ai_board = std::vector< std::vector<bool> > (15, std::vector<bool>(15));
}


GomokuClient::~GomokuClient()
{
    delete mp_tcpsocket;
}

void GomokuClient::PrintBoard(std::vector< std::vector<bool> > a_m_player_board,
                                std::vector< std::vector<bool> > a_m_ai_board)
{
    //Prints the board to screen
    std::cout << "   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14" << std::endl;
    for(unsigned int i=0; i < a_m_player_board.size(); i++)
    {
        if(i < 10)
        {
            std::cout << "0" << i;
        }
        else
        {
            std::cout << i;
        }

        for(unsigned int j=0; j < a_m_player_board.size(); j++)
        {
            if(a_m_player_board[i][j])
            {
                std::cout << "  " << "X";
            }
            else if(a_m_ai_board[i][j])
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
    //Start ListenToServer() in another thread
    std::thread listenThread ([&] { this->ListenToServer(); });

    //Variables to hold message to send
    std::string send_message;

    //Sets all the locations on the boards to false
    for(unsigned int i=0; i < 15; i++)
    {
        for(unsigned int j=0; j < 15; j++)
        {
            m_player_board[i][j] = false;
            m_ai_board[i][j] = false;
        }
    }

    //Seeds rand
    srand(time(NULL));

    //Flips a coin
    int coin_flip = rand() % 2 + 1;

    //Inform the user of the move command
    std::cout << "The command for moves are '00,00' "
        "representing the tile numbers on the board" << std::endl;
    std::cout << "----------------------------------"
        "-------------------------------------------------" << std::endl;

    //Tells the user the result of the coin flip,
    // and appeals to their sense of honor
    std::cout << "Coin flip in progress..." << std::endl;
    if(coin_flip == 1)
    {
        std::cout << "You won, you get to go first." << std::endl;
    }
    else
    {
        std::cout << "You lost, the AI get to go first." << std::endl;
        std::cout << "The AI don't know this so you could claim you won."
                                                                << std::endl;
        std::cout << "But that wouldn't be too honorable, would it?"
                                                                << std::endl;
    }

    //Print imput prompt
    std::cout << "<- ";

    while(1 < 2)
    {
        //Gets input
        std::cin >> send_message;

        std::lock_guard<std::mutex> lock_guard (m_cout_mutex);

        //Creates a char arrays for the messages
        char char_send_message[send_message.size() + 1];
        char move_message[10];

        //Copies the message from send_message to char_send_message
        std::strcpy(char_send_message, send_message.c_str());

        //Check if send_message is an move command
        if(send_message.size() == 5 && char_send_message[2] == ',')
        {
            //Make char_send_message into a valid move command------------------

            //Create a string for it
            std::string tmp_message = "MOV:";

            //Create a valid char_send_message
            tmp_message
            = tmp_message + char_send_message[0] + char_send_message[1] + ":"
            + char_send_message[3] +  char_send_message[4];

            //Copies tmp_message to char_send_message
            std::strcpy(move_message, tmp_message.c_str());

            //Adds a newline character to char_send_message
            move_message[sizeof(move_message) -1] = '\n';

            //Sends the message to a file descriptor
            write(mp_tcpsocket->get_descriptor(),
                move_message, sizeof(move_message));

            //Make char_send_message into a valid move command------------------

        }
        else
        {
            //Adds a newline character to char_send_message
            char_send_message[sizeof(char_send_message) -1] = '\n';

            //Sends the message to a file descriptor
            write(mp_tcpsocket->get_descriptor(),
                char_send_message, sizeof(char_send_message));
        }

        //breaks loop if quiting
        if(send_message == "QUI")
        {
            std::exit(0);
        }

        //Checks if a move was made by the user, if so, print board
        if(move_message[0] == 'M' && move_message[1] == 'O' &&
            move_message[2] == 'V' && move_message[3] == ':' )
        {
            //Variables to hold the move coordinates
            int x, y;

            //Gets the coordiniates
            getMove(move_message, x, y);

            //Adds the move to m_player_board
            m_player_board[x][y] = true;

            //Prints the board with PrintBoard()
            PrintBoard(m_player_board, m_ai_board);
        }
        m_cout_mutex.unlock();
    }

    try
    {
    	//Get the native thread handle
    	std::thread::native_handle_type threadHandle
            = listenThread.native_handle();

    	// Call the native cancel with the handle
    	pthread_cancel(threadHandle);

        //Wait for the thread to finish
    	listenThread.join();
	}
	catch(std::exception &e){
		std::cout << e.what() << std::endl;
	}
}

void GomokuClient::ListenToServer()
{
    //Variable to hold the message
    char return_message[100];

    while(1 < 2)
    {
        //Attempts to read a message from a file descriptor
        int bytes_read = read(mp_tcpsocket->get_descriptor(),
                return_message, sizeof(return_message));

        //If something was read
        if(bytes_read != 0 && bytes_read != -1)
        {
            std::lock_guard<std::mutex> lock_guard (m_cout_mutex);

            //Acts depending on server response message
            if(return_message[0] == 'I' && return_message[1] == 'L' &&
                    return_message[2] == 'C')
            {
                //Mark where the message ends
                return_message[4] = '\0';

                //Print return_message
                std::cout << "-> " << return_message;

                std::exit(1);
            }
            else if(return_message[0] == 'W' && return_message[1] == 'I' &&
                    return_message[2] == 'N')
            {
                //Mark where the message ends
                return_message[4] = '\0';

                //Print return_message
                std::cout << "-> " << return_message;

                std::exit(0);
            }
            else if(return_message[0] == 'I' && return_message[1] == 'L' &&
                    return_message[2] == 'M')
            {
                //Mark where the message ends
                return_message[10] = '\0';

                //Print return_message
                std::cout << "-> " << return_message;

                std::exit(1);
            }
            else if(return_message[0] == 'N' && return_message[1] == 'A' &&
                    return_message[2] == 'P')
            {
                //Mark where the message ends
                return_message[4] = '\0';

                //Print return_message
                std::cout << "-> " << return_message;

                std::exit(1);
            }
            //Checks if return_message is CHT
            else if(return_message[0] == 'C' && return_message[1] == 'H' &&
                    return_message[2] == 'T')
            {
                //Print return_message
                std::cout << std::endl;
                std::cout << "-> " << return_message;

                //Print imput prompt
                std::cout << "<- ";
                std::cout.flush();
            }
            //Checks if return_message is OKR
            else if(return_message[0] == 'O' && return_message[1] == 'K' &&
                    return_message[2] == 'R')
            {
                //Print return_message
                std::cout << "-> " << return_message;

                //Prints empty board
                PrintBoard(m_player_board, m_ai_board);

                //Print imput prompt
                std::cout << "<- ";
            }
            //Checks if a move was made by the AI, if so, print board
            else if(return_message[0] == 'M' && return_message[1] == 'O' &&
                return_message[2] == 'V' && return_message[3] == ':' )
            {
                //Variables to hold the move coordinates
                int x, y;

                //Gets the coordiniates
                getMove(return_message, x, y);

                //Adds the move to m_ai_board
                m_ai_board[x][y] = true;

                //Mark where the message ends
                return_message[10] = '\0';

                //Print return_message
                std::cout << "-> " << return_message;

                //Prints the board with PrintBoard()
                PrintBoard(m_player_board, m_ai_board);

                //Print imput prompt
                std::cout << "<- ";
                std::cout.flush();

            }

            m_cout_mutex.unlock();
        }
    }
    m_quit = true;
}
