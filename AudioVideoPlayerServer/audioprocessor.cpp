/*
 * AudioProcessor.cpp
 *
 *  Created on: Jan 2018
 *      Author: Rebecca Bernstein
 */

#include "audioprocessor.h"
#include <iostream>

/**
 * @brief AudioProcessor::AudioProcessor constructs a Processor to play audio buffers.
 *
 * @param s - An AudioInterface.
 * @param bufCount -- number of buffers initialized on the "already processed" queue
 * @param bufSize -- capacity of the buffers that are initialized
 */
AudioProcessor::AudioProcessor(AudioInterface& s, int bufCount, int bufSize)
    : Processor(bufCount, bufSize), speaker(s) { }

/**
 * @brief AudioProcessor::~AudioProcessor destructor closes the provided AudioInterface.
 */
AudioProcessor::~AudioProcessor()
{
    speaker.close();
}

/**
 * This method is called repeatedly by the parent Process's thread.
 *
 * @brief AudioProcessor::processBuffer plays the given buffer through the AudioInterface.
 * @param buffer - a buffer of data to fill or process
 */
void AudioProcessor::processBuffer(std::vector<char>* buffer) {
    // assume buffer is resized correctly
    // std::cout << "Playing bytes: " << buffer->size() << " bytes..." << std::endl;
    speaker.write(buffer->data(), buffer->size());
    // std::cout << "...Played" << std::endl;
    // std::cout << "Buffer size: " << buffer->size() << std::endl;
}
