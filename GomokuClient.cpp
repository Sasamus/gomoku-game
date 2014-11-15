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
    //Start ListenToServer() in another thread_from_server
    std::thread listenThread ([&] { this->ListenToServer(); });

    //Variable to keep track of if a game is running
    bool game_running = false;

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
    std::cout << "----------------------------------"
        "-------------------------------------------------" << std::endl;
    std::cout << "The command for moves are '00,00' "
        "representing the tile numbers on the board" << std::endl;
    std::cout << "----------------------------------"
        "-------------------------------------------------" << std::endl;

    //Strings for input
    std::string opponent;
    std::string name;
    std::string start_game;
    std::string name_length;

    //Ask what opponent the user wants
    std::cout << "Do you want to play against Striker[0] or Defender[1]?"
        << std::endl;
    std::cin >> opponent;

    //Ask for the user's name
    std::cout << "What is your name?" << std::endl;
    std::cin >> name;

    //Create a string for the name lenght part of the command
    if(name.length() < 10)
    {
        name_length = "0" + std::to_string(name.length());
    }
    else
    {
        name_length = std::to_string(name.length());
    }

    //Create a CHA message
    start_game = "CHA:" + opponent + ":" + std::to_string(coin_flip) + ":"
        + name_length + ":" + name + "\n";

    bool first_run = true;
    while(1 < 2)
    {
        if(first_run)
        {
            send_message = start_game;
            first_run = false;
        }
        else
        {
            //Gets input
            std::cin >> send_message;
        }

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

            //Copies tmp_message to move_message
            std::strcpy(move_message, tmp_message.c_str());

            //Adds a newline character to move_message
            move_message[sizeof(move_message) -1] = '\n';

            //Sends the message to a file descriptor
            write(mp_tcpsocket->get_descriptor(),
                move_message, sizeof(move_message));

            //Make char_send_message into a valid move command------------------

        }
        //breaks loop if quiting
        else if(send_message == "QUI")
        {
            std::exit(0);
        }
        //send_messagage isn't a MOV or QUI, send as CHT if game is runnig
        // send as is otherwise
        else
        {
            if(game_running)
            {
                //Make char_send_message into a valid CHT command---------------

                //Create a string for it
                std::string tmp_message = "CHT:";

                //A string to hold the size part of the command
                std::string message_size;

                //Create a string for the size part of the command
                if(sizeof(char_send_message) <= 9)
                {
                    message_size
                    = "0" + std::to_string(sizeof(char_send_message) - 1);
                }
                else if(sizeof(char_send_message) > 9)
                {
                    message_size
                    = std::to_string(sizeof(char_send_message) - 1);
                }
                else
                {
                    std::cout
                    << "To big message to be sent as chat" << std::endl;
                }

                //Create a valid message
                tmp_message
                = tmp_message + message_size + ":" + char_send_message;

                //Create a char array fpr the message
                char chat_message[tmp_message.size()];

                //Copies tmp_message to chat_message
                std::strcpy(chat_message, tmp_message.c_str());

                //Adds a newline character to chat_message
                chat_message[sizeof(chat_message) -1 ] = '\n';

                //Sends the message to a file descriptor
                write(mp_tcpsocket->get_descriptor(),
                    chat_message, sizeof(chat_message));

                //Make char_send_message into a valid CHT command---------------
            }
            else
            {
                //Copies tmp_message to char_send_message
                std::strcpy(char_send_message, send_message.c_str());

                //Adds a newline character to char_send_message
                char_send_message[sizeof(char_send_message) -1] = '\n';

                //Sends the message to a file descriptor
                write(mp_tcpsocket->get_descriptor(),
                    char_send_message, sizeof(char_send_message));

                //Set game_running to true
                game_running = true;

            }

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
    	//Get the native thread_from_server handle
    	std::thread::native_handle_type threadHandle
            = listenThread.native_handle();

    	// Call the native cancel with the handle
    	pthread_cancel(threadHandle);

        //Wait for the thread_from_server to finish
    	listenThread.join();
	}
	catch(std::exception &e){
		std::cout << e.what() << std::endl;
	}
}

void GomokuClient::ListenToServer()
{
    //Variables to hold the message
    char read_from_server[200];
    char message[200];

    while(1 < 2)
    {
        //Attempts to read_from_server a message from a file descriptor
        int bytes_read = read(mp_tcpsocket->get_descriptor(),
                read_from_server, sizeof(read_from_server));

        //If something was read_from_server
        if(bytes_read != 0 && bytes_read != -1)
        {
            std::lock_guard<std::mutex> lock_guard (m_cout_mutex);

            //read_from_server a message
            int count_chars = 0;
            for(char c : read_from_server)
            {
                if(c != '\n')
                {
                    message[count_chars] = c;
                    count_chars++;
                }
                else
                {
                    message[count_chars] = c;
                    message[count_chars + 1] = '\0';
                    count_chars++;

                    //Acts depending on server response message
                    if(message[0] == 'I' && message[1] == 'L'
                        && message[2] == 'C')
                    {
                        //Print message
                        std::cout << "Server: " << message;

                        std::exit(1);
                    }
                    else if(message[0] == 'W' && message[1] == 'I'
                        && message[2] == 'N')
                    {
                        //Print message
                        std::cout << "Server: " << message;

                        std::exit(0);
                    }
                    else if(message[0] == 'I' && message[1] == 'L'
                        && message[2] == 'M')
                    {
                        //Print message
                        std::cout << "Server: " << message;

                        std::exit(1);
                    }
                    else if(message[0] == 'N' && message[1] == 'A'
                        && message[2] == 'P')
                    {
                        //Print message
                        std::cout << "Server: " << message;

                        std::exit(1);
                    }
                    //Checks if message is CHT
                    else if(message[0] == 'C' && message[1] == 'H'
                        && message[2] == 'T')
                    {
                        //Print message
                        std::cout << "Server: " << message;
                        std::cout.flush();
                    }
                    //Checks if message is OKR
                    else if(message[0] == 'O' && message[1] == 'K'
                        && message[2] == 'R')
                    {
                        //Print message
                        std::cout << "Server: " << message;

                        //Prints empty board
                        PrintBoard(m_player_board, m_ai_board);
                    }
                    //Checks if a move was made by the AI, if so, print board
                    else if(message[0] == 'M' && message[1] == 'O'
                        && message[2] == 'V' && message[3] == ':')
                    {
                        //Variables to hold the move coordinates
                        int x, y;

                        //Gets the coordiniates
                        getMove(message, x, y);

                        //Adds the move to m_ai_board
                        m_ai_board[x][y] = true;

                        //Print message
                        std::cout << "Server: " << message;

                        //Prints the board with PrintBoard()
                        PrintBoard(m_player_board, m_ai_board);
                    }
                    //Reset message and count_chars
                    memset(message, '\0', count_chars);
                    count_chars = 0;
                }
            }
            m_cout_mutex.unlock();
        }
    }
}
