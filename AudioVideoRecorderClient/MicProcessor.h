/*
 * MicProcessor.h
 *
 *  Created on: Feb 15, 2018
 *      Author: Rebecca Bernstein
 */

#ifndef MICPROCESSOR_H_
#define MICPROCESSOR_H_

#include "processor.h"
#include "AudioInterface.h"

/**
 * Child of the abstract Processor class.
 * Processes a buffer by filling it with an audio frame from the AudioInterface.
 * Frame size and sampling rate are pre-configured in the AudioInterface.
 *
 * Waits for and processes buffers on its own thread.
 *
 * @brief The MicProcessor class fills available buffers with an audio frame and puts them on its own queue.
 */
class MicProcessor: public Processor {
public:
	MicProcessor(AudioInterface* m, int bufCount, int bufSize);
	virtual ~MicProcessor();

	// virtual method of Processor
	void processBuffer(std::vector<char>* buffer);

private:
    /**
     * @brief mic - used to capture audio frames.  Frame size and sampling rate are pre-configured
     */
	AudioInterface* mic;

    /**
     * @brief samplesRead - Samples per frame.  Class-level variable to increase processing speed
     */
	int samplesRead;
};

#endif /* MICPROCESSOR_H_ */
