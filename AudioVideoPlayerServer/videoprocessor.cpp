/*
 * videoprocessor.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Rebeca Bernstein
 */

#include "videoprocessor.h"
#include <iostream>

/**
 * @brief VideoProcessor::VideoProcessor constructs a Processor to send video data to a display.
 *
 * @param v - the graphical display
 * @param bufCount -- number of buffers initialized on the "already processed" queue
 * @param bufSize -- capacity of the buffers that are initialized
 */
VideoProcessor::VideoProcessor(VideoViewer& v, int bufCount, int bufSize)
    : Processor(bufCount, bufSize), viewer(v) { }

/**
 * @brief VideoProcessor::~VideoProcessor destructor
 */
VideoProcessor::~VideoProcessor(){ }

/**
 * This method is called repeatedly by the parent Process's thread.
 *
 * @brief VideoProcessor::processBuffer sends the data (image frame) to the VideoViewer to be displayed
 * @param buffer - a frame of JPG image data
 */
void VideoProcessor::processBuffer(std::vector<char>* buffer) {
    viewer.emitImage((std::vector<unsigned char>*) buffer);
}
