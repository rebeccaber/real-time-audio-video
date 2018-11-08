/*
 * audioprocessor.h
 *
 *  Created on: Feb 2018
 *      Author: Rebeca Bernstein
 */

#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include "AudioInterface.h"
#include "processor.h"

/**
 * Childof the abstract Processor class.
 * Processes a buffer by playing its data to a given speaker.
 * Waits for and processes buffers on its own thread.
 *
 * @brief The AudioProcessor class waits for and plays buffers to a speaker.
 */
class AudioProcessor: public Processor {
    public:
        AudioProcessor(AudioInterface& s, int bufCount, int bufSize);
        ~AudioProcessor();

        // virtual method of Processor
        void processBuffer(std::vector<char>* buffer);
    private:
        /**
         * @brief speaker designated speaker to play audio data
         */
        AudioInterface& speaker;
};

#endif // AUDIOPROCESSOR_H
