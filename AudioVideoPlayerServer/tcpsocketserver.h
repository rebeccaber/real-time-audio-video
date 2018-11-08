/*
 * tcpsocketserver.h
 *
 *  Created on: Feb 23, 2018
 *      Author: Rebeca Bernstein
 */

#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

/**
 *  @brief The TCPSocketServer class creates a server to accept tcp connections.  Can also statically connect as a client.
 */
class TCPSocketServer
{
public:
    TCPSocketServer(int port);
    ~TCPSocketServer();

    int acceptConnection();
    static int clientConnection(char* servername, int port);

    void shutdown();

private:
    /**
     * @brief serverfd - the server socket used to lsiten for client connection requests
     */
    int serverfd;

    int setupServer(int portno);
};

#endif // TCPSOCKETSERVER_H
