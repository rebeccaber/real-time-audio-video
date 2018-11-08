/*
 * tcpinprocessor.h
 *
 *  Created on: Feb 2018
 *      Author: Rebeca Bernstein
 */

#ifndef TCPINPROCESSOR_H
#define TCPINPROCESSOR_H

#include "processor.h"

/**
 * Child of the abstract Processor class.
 * Processes a buffer by filling it with a frame of data read from a TCP socket.
 *
 * Waits for and processes buffers on its own thread.
 *
 * Expected data format:
 * The data read in is expected to be prefixed by a 4-byte number (signed long)
 * indicating its length.
 *
 * @brief The TCPInProcessor class waits for and plays buffers to a speaker.
 */
class TCPInProcessor: public Processor {
    public:
        TCPInProcessor(int socket, int bufCount, int bufSize);
        ~TCPInProcessor();

        // virtual method of Processor
        void processBuffer(std::vector<char>* buffer);

    private:
        /**
         * @brief socket - tcp connection from which to read data
         */
        int socket;
};
#endif // TCPINPROCESSOR_H
