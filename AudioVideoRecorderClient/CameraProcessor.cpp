/*
 * CameraProcessor.cpp
 *
 *  Created on: Feb 13, 2018
 *      Author: Rebecca Bernstein
 */

#include "CameraProcessor.h"
#include <iostream>

/**
 * @brief CameraProcessor::CameraProcessor constructs a Processor to store image captures to a buffer.
 * @param c - camera object
 * @param bufCount -- number of buffers initialized on the "already processed" queue
 * @param bufSize -- capacity of the buffers that are initialized
 */
CameraProcessor::CameraProcessor(Camera* c, int bufCount, int bufSize):
	Processor(bufCount, bufSize), camera(c) { }

/**
 * @brief CameraProcessor::~CameraProcessor destructor
 */
CameraProcessor::~CameraProcessor() { }

/**
 * This method is called repeatedly by the parent Process's thread.
 * (Parent class is abstract.)
 *
 * @brief CameraProcessor::processBuffer fills the buffer with an image frame captured from the camera
 * @param buffer - a buffer of data to fill or process
 */
void CameraProcessor::processBuffer(std::vector<char>* buffer) {
    // std::cout << "In camera processor..." <<std::endl;
    camera->getPicture((std::vector<unsigned char>*)buffer);
    // std::cout << "...finish camera processor." <<std::endl;
}
