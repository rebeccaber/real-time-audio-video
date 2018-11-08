/*
 * videoprocessor.h
 *
 *  Created on: Feb 13, 2018
 *      Author: Rebeca Bernstein
 */

#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include "videoviewer.h"
#include "processor.h"

/**
 * Child of the abstract Processor class.
 * Processes a buffer by emitting it to an event queue that
 * eventually creates an update to the display.
 *
 * Waits for and processes buffers on its own thread.
 *
 * @brief The VideoProcessor class waits for and emits image frames
 * to an event queue for display updates.
 */
class VideoProcessor: public Processor {
    public:
        VideoProcessor(VideoViewer& v, int bufCount, int bufSize);
        ~VideoProcessor();

        // virtual method of Processor
        void processBuffer(std::vector<char>* buffer);
    private:
        /**
         * @brief viewer - display to send video frame updates
         */
        VideoViewer& viewer;
};

#endif // VIDEOPROCESSOR_H
