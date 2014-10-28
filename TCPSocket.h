//=============================================================
// Name        : TCPSocket.h
// Author      : Albin Engström
// Created     : 2014-10-22
// Modified    : 2014-10-28
// Description : Definition of class TCPSocket
// Purpose     : A TCP Socket that is used from a client
//=============================================================
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <cstring>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>



class TCPSocket
{
private:
    //A socket file descriptor
    int m_socket_file_descriptor;

    //A sockaddr_in struct
    struct sockaddr_in m_server_address;

public:

    TCPSocket();
    //Pre:
    //Post: Creates the socket, may throw runtime_error

    ~TCPSocket();
    //Pre:
    //Post: Closes the socket, if not done before

    void connect(const char a_ip_address[], in_port_t a_port);
    //Pre:
    //Post: Connects to server using a dotten decimal adress
    // may throw runtime_error

    void connect(std::string host_name, in_port_t a_port);
    //Pre:
    //Post: Connects to server using a hostname, may throw runtime_error

    int get_descriptor() const {return m_socket_file_descriptor;}
    //Pre:
    //Post: Returns m_socket_file_descriptor

    void socket_close();
    //Pre:
    //Post: Closes the socket, may throw runtime_error

};
