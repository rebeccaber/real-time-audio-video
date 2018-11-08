/*
 * tcpinprocessor.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Rebeca Bernstein
 */

#include "tcpinprocessor.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

/**
 * @brief TCPInProcessor::TCPInProcessor constructs a Processor to read data from a TCP connection
 * @param sock - A ready TCP socket
 * @param bufCount -- number of buffers initialized on the "already processed" queue
 * @param bufSize -- capacity of the buffers that are initialized
 */
TCPInProcessor::TCPInProcessor(int sock, int bufCount, int bufSize)
    : Processor(bufCount, bufSize), socket(sock) { }

/**
 * @brief TCPInProcessor::~TCPInProcessor destructor shuts down the TCP interface.
 */
TCPInProcessor::~TCPInProcessor()
{
    close(socket);
}

/**
 * This method is called repeatedly by the parent Process's thread.
 *
 * @brief TCPInProcessor::processBuffer reads some data into the buffer (usually a "frame")
 * @param buffer - a buffer of data to fill or process
 */
void TCPInProcessor::processBuffer(std::vector<char>* buffer) {
    ssize_t nreturn;
    char* buf;
    long size;

    // read the buf size (long) first so we don't over-read
    nreturn = read(socket,&size, 4);
    while((nreturn < 4) && (nreturn!=0)){
        nreturn += read(socket, (((char*) &size) + nreturn), 4-nreturn);
    }

    // buffer->resize(defaultBufSize); // buffer size may have been modified by other processes
    buffer->resize(size);
    buf = &(*buffer)[0];

    nreturn = read(socket,buf, size);
    while((nreturn < size) && (nreturn!=0)){
        nreturn += read(socket, (buf+nreturn), size-nreturn);
    }
}
