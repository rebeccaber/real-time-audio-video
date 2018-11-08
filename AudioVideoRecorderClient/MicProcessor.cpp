/*
 * MicProcessor.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: Rebecca Bernstein
 */

#include "MicProcessor.h"
#include <iostream>

/**
 * @brief MicProcessor::MicProcessor constructs a Processor to store audio frame captures to a buffer.
 * @param m - microphone object
 * @param bufCount -- number of buffers initialized on the "already processed" queue
 * @param bufSize -- capacity of the buffers that are initialized
 */
MicProcessor::MicProcessor(AudioInterface* m, int bufCount, int bufSize):
		Processor(bufCount, bufSize), mic(m), samplesRead(0) { }

/**
 * @brief MicProcessor::~MicProcessor destructor
 */
MicProcessor::~MicProcessor() { }

/**
 * This method is called repeatedly by the parent Process's thread.
 * (Parent class is abstract.)
 *
 * @brief MicProcessor::processBuffer fills the buffer with an audio frame captured with the AudioInterface
 * @param buffer - a buffer of data to fill or process
 */
void MicProcessor::processBuffer(std::vector<char>* buffer) {
    // std::cout << "In mic processor..." <<std::endl;

	buffer->resize(this->defaultBufSize);
	samplesRead = mic->read(buffer->data());
	buffer->resize(samplesRead*2); // 2 bytes per sample

    // std::cout << "Samples read: " << samplesRead << ", Bytes calculated: " << samplesRead*2 << std::endl;
    // std::cout << "...finish mic processor." <<std::endl;
}

