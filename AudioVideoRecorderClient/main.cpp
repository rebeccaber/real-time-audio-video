/*
 * main.cpp
 *
 *  Created on: Jan 14, 2018
 *      Author: Rebecca Bernstein
 */

#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <unistd.h> // sleep

#include <string>
#include <sstream>

#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

#include "Camera.h"
#include "CameraProcessor.h"

#include "AudioInterface.h"
#include "MicProcessor.h"
#include "audioconfig.h"

#include "TCPOutProcessor.h"
#include "TCPClient.h"
#include "tcpsocketserver.h"

/**
 * The sending end of a one-way video conferencing application.
 * Captutes buffers (frames) of audio and video data and sends the data across a TCP connection.
 *
 *  Audio is accomplished with the ALSA library.  Video capture uses the OpenCV library.
 *
 * This application is build around a group of Processes, each running on its own thread.
 * These processes each consume/produce a buffer of data before sending it to the next Process.
 * Processes:
 * - MicProcessor - records a frame of audio to a buffer
 * - CameraProcessor - stores a video capture frame to a buffer
 * - TCPOutProcessor x2
 *    - one for each, writes out audio and video frames
 *
 * The main() method sets up the processes and starts them
 *
 * @brief main sets up the processes and starts them (video and audio recorders and tcp writers)
 * @param argc -- the arg count of argv
 * @param argv --  the arguments run with this application
 * @return Qt app.exec()
 */
int main( int argc, const char* argv[] )
{
    // ----------------------------------------------------------
    // Check args

    if (argc < 6) {
       fprintf(stderr,"usage: %s servername port imgheight imgwidth audioDeviceName\n", argv[0]);
       exit(0);
    }

    std::cout << "Args: \n\targc: " << argc << "\n\targv: [ ";
    for(int i = 0; i< argc-1; i++){
        std::cout << argv[i] << ", ";
    }
    std::cout << argv[argc-1] << " ]" << std::endl;

    char servername[255];
    strcpy(servername, argv[1]);
    int port = atoi(argv[2]);

    int height = atoi(argv[3]);
    int width = atoi(argv[4]);

    char deviceName[255];
    strcpy(deviceName, argv[5]);

    int samplingRate = SAMPLING_RATE;
    //if(argc >= 7)
    //    samplingRate = atoi(argv[6]);

    int frameSize = DEFAULT_FRAME_SIZE;
    //if(argc >= 8)
    //    frameSize = atoi(argv[7]);


    // *****************************************************************
    //                              MIC
    // *****************************************************************

    // mic process and tcp process are circulating buffers

    // -----------------------------------------------------------------
    // Set up microphone

    //AudioInterface* mic = new AudioInterface(deviceName, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_CAPTURE, DEFAULT_FRAME_SIZE);
    AudioInterface* mic = new AudioInterface(deviceName, samplingRate, NUMBER_OF_CHANNELS, SND_PCM_STREAM_CAPTURE, frameSize);
    mic->open();

    // ---------------------------------------------------------------------
    // Set up TCP Client

    std::cout << "Set up the client connection..." << std::endl;

    int audioSocket = TCPSocketServer::clientConnection(servername, port);

    //----------------------------------------------------------------------
    // Set up processes

    std::cout << "Set up the processes..." << std::endl;

    int bufCount = 4;
    int bufSize = mic->getRequiredBufferSize();

    MicProcessor* micProcess = new MicProcessor(mic, 0, bufSize);
    TCPOutProcessor* tcpMicProcess = new TCPOutProcessor(audioSocket, bufCount, bufSize); // 4 bufs ready for camera

    micProcess->prev = tcpMicProcess;
    tcpMicProcess->prev = micProcess;



    // *****************************************************************
    //                            CAMERA
    // *****************************************************************

    // camera process and tcp process are circulating buffers

    // -----------------------------------------------------------------
    // Set up camera

    Camera* cam = new Camera(width, height);
    cam->start();

    // ---------------------------------------------------------------------
    // Set up TCP Client

    std::cout << "Set up the client connection..." << std::endl;

    int videoSocket = TCPSocketServer::clientConnection(servername, port);

    //----------------------------------------------------------------------
    // Set up processes

    std::cout << "Set up the camera processes..." << std::endl;

    bufCount = 4;
    bufSize = width*height;

    CameraProcessor* cameraProcess = new CameraProcessor(cam, 0, bufSize);
    TCPOutProcessor* tcpCameraProcess = new TCPOutProcessor(videoSocket, bufCount, bufSize); // 4 bufs ready for camera

    cameraProcess->prev = tcpCameraProcess;
    tcpCameraProcess->prev = cameraProcess;

    std::cout << "Start processes..." << std::endl;
    cameraProcess->start();
    tcpCameraProcess->start();
    micProcess->start();
    tcpMicProcess->start();

    // -----------------------------------------------------------------------------
    // Capture and send for x seconds
    /*
    std::cout << "Capture and send 3000 seconds..." << std::endl;

    int secToCapture = 3000;

    for(int i = 0; i < secToCapture; i+=5) {
        std::cout << i << " seconds captured. (Of " << secToCapture << ")" << std::endl;
        sleep(5);
    }
    */

    char exit;
    std::cout << "Press \"Enter\" to exit." << std::endl;
    std::cin >> exit;

    tcpCameraProcess->stop();
    cameraProcess->stop();
    tcpMicProcess->stop();
    micProcess->stop();

    mic->close();
    cam->shutdown();

    delete cameraProcess;
    delete tcpCameraProcess;
    delete micProcess;
    delete tcpMicProcess;
}
