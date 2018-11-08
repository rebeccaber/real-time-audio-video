/*
 * processor.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Rebeca Bernstein
 */

#include "processor.h"

#include <iostream>
#include <unistd.h>

/**
 * A Processor processes buffers and queues them for the next Processor.
 *
 * Before calling start() on a process the Processor.prev pointer must be set.  Processes
 * are sequential.  Each processes has a pointer to the "prev" process and looks there
 * for new buffers to process.
 *
 * @brief Initialises a Processor with bufCount buffers of capacity bufSize.
 *
 * @param bufCount -- number of buffers initialized on the "already processed" buffer queue
 * @param bufSize -- capacity of the buffers that are initialized
 */
Processor::Processor(int bufCount, int bufSize): prev(NULL), threadRunning(false), threadStopped(false), bufferQueue(), t(0), defaultBufSize(bufSize) {
    std::vector<char>* buf;
    for(int i = 0; i < bufCount; i++) {
        buf = new std::vector<char>();
        buf->reserve(bufSize);
        bufferQueue.push(buf);
    }

}

/**
 * @brief Processor::~Processor destructor stops this process's thread and deletes all buffers in its queue.
 */
Processor::~Processor() {
    if (threadRunning)
        this->stop();
    std::vector<char>* buf;

    mtx.lock();
    while(!bufferQueue.empty()) {
        buf = bufferQueue.front();
        bufferQueue.pop();
        delete buf;
    }
    mtx.unlock();
}

/**
 * Defines the behavior of this Processor's thread.
 *
 * Waits for a buffer in the "prev" Processor's queue, processes it,
 * and then adds the buffer to its own queue.  Loops and waits again.
 *
 * @brief Processor::run is the function executed by a Processor's thread.
 */
void Processor::run() {
    std::vector<char>* buf;
    std::unique_lock<std::mutex> lck(prev->mtx);
    // lck.unlock();

    threadRunning = true;
    while(threadRunning){

        while(prev->bufferQueue.empty() && !(prev->threadStopped)) prev->cv.wait(lck);

        if(prev->threadStopped) // if prev process shuts down and activates its cv
            break;

        buf = prev->bufferQueue.front();
        prev->bufferQueue.pop();
        lck.unlock();

        processBuffer(buf);

        mtx.lock();
        bufferQueue.push(buf);
        mtx.unlock();

        cv.notify_one();
        lck.lock();
    }
    threadStopped = true;
    threadRunning = false;
    lck.unlock();
    cv.notify_all();
}

/**
 * Start this Processor's thread. Before calling this method, Processor.prev must be set.
 *
 * @brief Processor::start starts this Processor's thread.  Processor.prev must be set.
 * @return 0 on success.
 */
int Processor::start() {
    std::cout << "In Processor::start.." << std::endl;
    if (prev == NULL) {
        return -1;
    }

    t = new std::thread(&Processor::run, this);

    return 0;
}

/**
 *  @brief Processor::stop stops this Processor's thread.
 */
void Processor::stop() {
    if(threadRunning) {
        std::cout << "Stopping process.." << std::endl;
        threadRunning = false;
        cv.notify_all();
        t->join();
        delete t;
        std::cout << "\tProcess stopped." << std::endl;
    }
}

void Processor::setPrev(Processor* prev)
{
    this->prev = prev;
}
