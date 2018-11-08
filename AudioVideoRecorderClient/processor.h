/*
 * processor.h
 *
 *  Created on: Feb 2018
 *      Author: Rebecca Bernstein
 */

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <queue>
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <thread>
#include <vector>

/**
 * Abstract buffer-processing class.
 *
 * A Processor:
 *  - waits for and processes a buffer
 *  - queues it for the next Processor
 *  - then begins to wait for the next buffer.
 *
 * The processBuffer() function is abstract, so the behavior
 * of a processor is defined in child classes.
 *
 * To create and run a process:
 * Before calling start() on a process the Processor.prev pointer
 * must be set.  Processes are sequential.  Each processes has a
 * pointer to the "prev" process and looks there for new buffers
 * to process.  To reuse buffers, connect the first and last
 * processes.
 *
 * @brief The Processor class is abstract.  Waits for and processes buffers in its own thread.
 */
class Processor {
public:
    Processor(int bufCount, int bufSize); // don't forget to set prev
    virtual ~Processor();

    /**
     * @brief prev - The previous process.  Wait for new buffers in this process's queue.
     */
    Processor* prev;

    void run();
    int start();
    void stop();

    /**
     * @brief processBuffer - Abstract function.  Performs some operation on a buffer.
     * @param buffer
     */
    virtual void processBuffer(std::vector<char>* buffer)=0;


private:
    /**
     * @brief threadRunning - whether this process has begun execution yet
     */
    bool threadRunning;

    /**
     * @brief threadStopped - whether this process has been started and also stopped
     */
    bool threadStopped;

    /**
     * @brief bufferQueue - queue of "already-processed" buffers.  To be consumed by the next process.
     */
    std::queue<std::vector<char>*> bufferQueue;

    /**
     * @brief mtx - mutex for this process's queue
     */
    std::mutex mtx;

    /**
     * @brief cv - used to wait on a new buffer in this process's queue (the next process will use this to wait)
     */
    std::condition_variable cv;

    /**
     * @brief t - the thread on which this processor runs
     */
    std::thread* t;

protected:
    /**
     * @brief defaultBufSize - used for resetting buffer size to a default
     */
    long defaultBufSize;
};


#endif // PROCESSOR_H
