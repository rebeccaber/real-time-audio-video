/*
 * TCPOutProcessor.h
 *
 *  Created on: Feb 13, 2018
 *      Author: Rebeca Bernstein
 */

#ifndef TCPOUTPROCESSOR_H_
#define TCPOUTPROCESSOR_H_

#include "processor.h"

/**
 * Child of the abstract Processor class.
 * Processes a buffer by writing its contents to a TCP socket.
 *
 * Waits for and processes buffers on its own thread.
 *
 * @brief The CameraProcessor class writes the contents of available buffers to a
 * TCP Socket and puts buffers on its own queue.
 */
class TCPOutProcessor: public Processor {
	public:
        TCPOutProcessor(int socket, int bufCount, int bufSize);
        ~TCPOutProcessor();

		// virtual method of Processor
		void processBuffer(std::vector<char>* buffer);
	private:
        /**
         * @brief socket - TCP socket to which to write buffer data
         */
        int socket;
};

#endif /* TCPOUTPROCESSOR_H_ */
