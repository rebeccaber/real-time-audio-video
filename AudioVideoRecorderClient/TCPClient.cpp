/*
 * TCPClient.cpp
 *
 *  Created on: Dec 19, 2017
 *      Author: Rebecca Bernstein
 */

#include "TCPClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <sys/uio.h>
#include <iostream>

//#define SERVER_PORT 8080

//static const char SERVER_ADDR[] = "localhost";

TCPClient::TCPClient(char* servername, int port):
		sock(0){
	setupSocket(servername, port);

}

TCPClient::~TCPClient() {
	close(sock);
}

int TCPClient::produce(void* buf, int size)
{
	return read(sock,buf,size); // should be blocking by default
}

int TCPClient::writeBuf(char* buf, long size) {
	struct iovec iov[2];
	ssize_t nwritten;

	iov[0].iov_base = &size;
	iov[0].iov_len = 4; // # bytes to transfer
	iov[1].iov_base = buf;
	iov[1].iov_len = size;

	nwritten = writev(sock, iov, 2);

	// std::cout << "Writing size: " << size << " of 2 bytes." << std::endl;

	// std::cout << "Wrote: " << nwritten << " bytes of " << (2+size) << " (2-byte size + " << size << "-byte buffer)" << std::endl;

	// return write(sock, buf, size);
	return nwritten;
}

int TCPClient::setupSocket(char* servername, int port) {
	struct sockaddr_in serv_addr;
	struct hostent *server;

	// char *hello = "Hello from client";
	// char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	return 0;
}
