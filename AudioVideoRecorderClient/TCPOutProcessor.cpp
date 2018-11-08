/*
 * TCPOutProcessor.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: Rebecca Bernstein
 */

#include "TCPOutProcessor.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

/**
 * @brief TCPInProcessor::TCPInProcessor constructs a Processor to write data to a TCP connection
 * @param sock - A ready TCP socket
 * @param bufCount -- number of buffers initialized on the "already processed" queue
 * @param bufSize -- capacity of the buffers that are initialized
 */
TCPOutProcessor::TCPOutProcessor(int socket, int bufCount, int bufSize):
    Processor(bufCount, bufSize), socket(socket) { }

/**
 * @brief TCPOutProcessor::~TCPOutProcessor destructor
 */
TCPOutProcessor::~TCPOutProcessor() { }

/**
 * This method is called repeatedly by the parent Process's thread.
 * (Parent class is abstract.)
 *
 * Data format:
 * - 4 bytes (signed long) denoting the buffer's size
 * - the contents of the buffer
 *
 * @brief TCPOutProcessor::processBuffer write the size and contents of the buffer to a TCP socket.
 * @param buffer - a buffer of data to fill or process
 */
void TCPOutProcessor::processBuffer(std::vector<char>* buffer) {
    // std::cout << "In tcp out processor..." <<std::endl;
    struct iovec iov[2];
    ssize_t nwritten;
    long size = buffer->size();

    iov[0].iov_base = &size;
    iov[0].iov_len = 4; // # bytes to transfer
    iov[1].iov_base = &(*buffer)[0];
    iov[1].iov_len = buffer->size();

    nwritten = writev(socket, iov, 2);

    // std::cout << "Initial write, wrote " << nwritten << " bytes of " << (size+4) << " bytes." << std::endl;

    if(nwritten < size+4) {
        while(nwritten < 4){
            nwritten += write(socket, ((char*)(&size)) + nwritten, 4-nwritten);
            std::cout << "Now wrote: " << nwritten << std::endl;
        }

        nwritten -= 4;

        while (nwritten < size) {
            nwritten += write(socket, &(*buffer)[0] + nwritten, buffer->size()-nwritten);
            std::cout << "Now wrote:: " << nwritten << std::endl;
        }
    }

    // std::cout << "...finish tcp out processor." <<std::endl;
}
