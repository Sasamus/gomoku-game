//=============================================================
// Name        : TCPSocket.cpp
// Author      : Albin Engström
// Created     : 2014-10-22
// Modified    : 2014-11-05
// Description : Implementation of class TCPSocket
//=============================================================
#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
    //Tries to create a socket, throw an exception if it fails
    if ((m_socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }

    //Set recieve timeout for m_socket_file_descriptor
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt (m_socket_file_descriptor, SOL_SOCKET, SO_RCVTIMEO,
    (char *)&timeout, sizeof(timeout));
}

TCPSocket:: ~TCPSocket()
{
    //Calls socket_close()
    socket_close();
}

void TCPSocket::connect(const char a_ip_address[], in_port_t a_port)
{
    //Sets m_server_address's sin_family to IPv4
    m_server_address.sin_family = AF_INET;

    //Sets it's sin_port to a_port
    m_server_address.sin_port = htons(a_port);

    //Convert address to binary form--------------------------------------------

    //An addrinfo struct for getaddrinfo()'s hints argument
    struct addrinfo hints;

    //An addrinfo struct for getaddrinfo()'s res argument
    struct addrinfo *res; /* out: result */

    //Zero out the hints struct
    memset(&hints, 0, sizeof(addrinfo));

    //Set relevant members
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    //Tries to get address info about a_ip_address
    if(getaddrinfo(a_ip_address, NULL, &hints, &res) != 0)
    {
        throw std::runtime_error(strerror(errno));
    }

    //Gets the address and puts it in a sockaddr_in struct
    struct sockaddr_in *saddrp = (sockaddr_in*)(res->ai_addr);

    //Copy binary address
    memcpy(&m_server_address.sin_addr.s_addr, &saddrp->sin_addr.s_addr, 4);

    //Release memory allocated by getaddrinfo
    freeaddrinfo(res);

    //Done----------------------------------------------------------------------

    //Tries to connect the client to the server, throw an exception if it fails
    if ((::connect(m_socket_file_descriptor, (struct sockaddr*)&m_server_address,
            sizeof(m_server_address))) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}

void TCPSocket::connect(std::string a_ip_address , in_port_t a_port)
{
    //Calls connect(), coverting a_ip_address to a char array
    connect(a_ip_address.c_str(), a_port);
}

void TCPSocket::socket_close()
{
    //Tries to close m_socket_file_descriptor, throw an exception if it fails
    if(close(m_socket_file_descriptor) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}
