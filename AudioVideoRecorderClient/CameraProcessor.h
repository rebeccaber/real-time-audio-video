/*
 * CameraProcessor.h
 *
 *  Created on: Feb 13, 2018
 *      Author: Rebecca Bernstein
 */

#ifndef CAMERAPROCESSOR_H_
#define CAMERAPROCESSOR_H_

#include "processor.h"
#include "Camera.h"

/**
 * Child of the abstract Processor class.
 * Processes a buffer by filling it with a JPG frame from the camera.
 * Image height and width are pre-configured in the Camera.
 *
 * Waits for and processes buffers on its own thread.
 *
 * @brief The CameraProcessor class waits for available buffers,
 * fills them, and puts them on its own queue.
 */
class CameraProcessor: public Processor {
	public:
		CameraProcessor(Camera* c, int bufCount, int bufSize);
		~CameraProcessor();

        // virtual method of Processor
		void processBuffer(std::vector<char>* buffer);
	private:
        /**
         * @brief camera - takes pictures, able to store to a buffer
         */
		Camera* camera;
};

#endif /* CAMERAPROCESSOR_H_ */
