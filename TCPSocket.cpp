//=============================================================
// Name        : TCPSocket.cpp
// Author      : Albin Engström
// Created     : 2014-10-22
// Modified    : 2014-10-23
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

}

void TCPSocket::base_server_connect(in_port_t a_port)
{
    //Sets m_server_address's sin_family to IPv4
    m_server_address.sin_family = AF_INET;

    //If already in network byte order
    //m_server_address.sin_port = port;

    //Sets it's sin_port to a_port
    m_server_address.sin_port = htons(a_port);
}


void TCPSocket::server_connect(const char a_ip_address[], in_port_t a_port)
{
    //Calls base_server_connect
    base_server_connect(a_port);

    //Tries to set m-server_adress's sin_addr, throw an exception if it fails
    if ((m_server_address.sin_addr.s_addr = inet_addr(a_ip_address))
            == (unsigned)-1)
    {
        throw std::runtime_error(strerror(errno));
    }

    //Tries to connect the client to the server, throw an exception if it fails
    if ((connect(m_socket_file_descriptor, (struct sockaddr*)&m_server_address,
            sizeof(m_server_address))) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}

void TCPSocket::server_connect(std::string a_host_name, in_port_t a_port)
{
    //Calls base_server_connect
    base_server_connect(a_port);

    //Tries to set m-server_adress's sin_addr, throw an exception if it fails
    if ((m_server_address.sin_addr.s_addr = inet_addr(a_host_name.c_str()))
            == (unsigned)-1)
    {
        throw std::runtime_error(strerror(errno));
    }

    //Tries to connect the client to the server, throw an exception if it fails
    if ((connect(m_socket_file_descriptor, (struct sockaddr*)&m_server_address,
            sizeof(m_server_address))) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}

void TCPSocket::socket_close()
{
    //Tries to close m_socket_file_descriptor, throw an exception if it fails
    if(close(m_socket_file_descriptor) == -1)
    {
        throw std::runtime_error(strerror(errno));
    }
}
