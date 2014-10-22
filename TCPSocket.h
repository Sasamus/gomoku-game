//=============================================================
// Name        : TCPSocket.h
// Author      : Albin Engström
// Created     : 2014-10-22
// Modified    : 2014-10-22
// Description : Definition of class TCPSocket
// Purpose     : A TCP Socket that is used from a client
//=============================================================


class TCPsocket
{
private:
    // Socket descriptor
    int iSockDesc;

public:

    TCPsocket();
    //Pre:
    //Post: Creates the socket, may throw runtime_error

    ~TCPsocket();
    //Pre:
    //Post: Closes the socket, if not done before

    void connect(const char ipaddress[], in_port_t port);
    //Pre:
    //Post: Connects to server using a dotten decimal adress
    // may throw runtime_error

    void connect(string ipaddress, in_port_t port);
    //Pre:
    //Post: Connects to server using a hostname, may throw runtime_error

    int getDescriptor() const {return iSockDesc;}
    //Pre:
    //Post: Returns iSockDesc

    void close();
    //Pre:
    //Post: Closes the socet, may throw runtime_error

};
