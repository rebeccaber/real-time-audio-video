/*
 * tcpsocketserver.cpp
 *
 *  Created on: Feb 23, 2018
 *      Author: Rebeca Bernstein
 */

#include "tcpsocketserver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include <sys/uio.h>

/**
 * @brief TCPSocketServer::TCPSocketServer creates a server to accept tcp connections.
 * @param port - the port on which to listen for client connections
 */
TCPSocketServer::TCPSocketServer(int port)
{
    setupServer(port);
}

/**
 * @brief TCPSocketServer::~TCPSocketServer - shuts down the TCP server
 */
TCPSocketServer::~TCPSocketServer()
{
    // close the server
    close(serverfd);
}

/**
 * @brief TCPSocketServer::shutdown - shuts down the TCP server
 */
void TCPSocketServer::shutdown()
{
    // close the server
    close(serverfd);
}

/**
 * @brief TCPSocketServer::acceptConnection - accepts the next connection request made by a client
 * @return - the TCP socket created to communicate with the client
 */
int TCPSocketServer::acceptConnection()
{
    struct sockaddr_in cli_addr;

    socklen_t clilen = sizeof(cli_addr);

    int socket;

    std::cout << "\tAccepting client connection..." << std::endl;
    socket = accept(serverfd,
              (struct sockaddr *) &cli_addr,
              &clilen);
    std::cout << "...Accepted." << std::endl;
        std::cout << std::flush;
    if (socket < 0){
        std::cout << "Error on accept." << std::endl;
        std::cout << std::flush;

        perror("ERROR on accept");
        return -1;
    }

    return socket;
}

/**
 * @brief TCPSocketServer::setupServer sets up the server, waits for one connection, and closes the server.
 * @return 0 on success
 */
int TCPSocketServer::setupServer(int portno) {
    std::cout << "Setting up server..." << std::endl;

    int err;
    struct sockaddr_in serv_addr;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0){
        perror("ERROR opening socket");
        return -1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons((unsigned short)portno);

    std::cout << "\tCreating server socket..." << std::endl;

    if (bind(serverfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0){
          perror("ERROR on binding");
          return -1;
    }

    std::cout << "\tListenning..." << std::endl;
    err = listen(serverfd,2);

    if(err != 0) {
        perror("ERROR on listening");
        return err;
    }

    std::cout << "Creating server socket complete." << std::endl;

    return 0;
}

/**
 * @brief TCPSocketServer::clientConnection - statically allows a client to connect to a server
 * @param servername
 * @param port
 * @return - the TCP socket created
 */
int TCPSocketServer::clientConnection(char* servername, int port)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int cli_socket;

    if ((cli_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    server = gethostbyname(servername);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);

    if (connect(cli_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    return 0;
}
