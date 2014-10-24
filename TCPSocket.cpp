//=============================================================
// Name        : TCPSocket.cpp
// Author      : Albin Engström
// Created     : 2014-10-22
// Modified    : 2014-10-24
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
}

TCPSocket:: ~TCPSocket()
{
    //Calls socket_close()
    socket_close();
}

void TCPSocket::server_connect(const char a_ip_address[], in_port_t a_port)
{
    //Sets m_server_address's sin_family to IPv4
    m_server_address.sin_family = AF_INET;

    //If already in network byte order
    //m_server_address.sin_port = port;

    //Sets it's sin_port to a_port
    m_server_address.sin_port = htons(a_port);

/*
    //Tries to set m_server_adress's sin_addr, throw an exception if it fails
    if (!(m_server_address.sin_addr.s_addr = inet_addr(a_ip_address)))
    {
        throw std::runtime_error(strerror(errno));
    }
*/

    //Convert address to binary form--------------------------------------------

    struct addrinfo hints; /* in: hints */
    struct addrinfo *res; /* out: result */

    memset(&hints, 0, sizeof(addrinfo)); // zero out the struct

    // Set relevant members
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(a_ip_address, NULL, &hints, &res) != 0)
    {
        throw std::runtime_error(strerror(errno));
    }

    struct sockaddr_in *saddrp = (sockaddr_in*)(res->ai_addr);

    /* Copy binary address */
    memcpy(&m_server_address.sin_addr.s_addr, &saddrp->sin_addr.s_addr,4);

    freeaddrinfo(res); // release memory allocated by getaddrinfo

    //Done----------------------------------------------------------------------


    //Tries to connect the client to the server, throw an exception if it fails
    if ((connect(m_socket_file_descriptor, (struct sockaddr*)&m_server_address,
            sizeof(m_server_address))) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}

void TCPSocket::server_connect(std::string a_ip_address , in_port_t a_port)
{
    //Calls server_connect(), coverting a_ip_address to a char array
    server_connect(a_ip_address.c_str(), a_port);
}

void TCPSocket::socket_close()
{
    //Tries to close m_socket_file_descriptor, throw an exception if it fails
    if(close(m_socket_file_descriptor) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}
