/*
 * main.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Rebeca Bernstein
 */

#include "videoviewer.h"
#include <QApplication>
#include <iostream>
#include <stdlib.h>
#include <unistd.h> // sleep

#include "tcpsocketserver.h"
#include "tcpinprocessor.h"
#include "videoprocessor.h"
#include "audioprocessor.h"
#include "processor.h"

#include "audioconfig.h"
#include "AudioInterface.h"

/**
 * The receiving end of a one-way video conferencing application.
 * Receieves buffers (frames) of audio and video data and sends the data to the speaks and a display.
 *
 *  Audio is accomplished with the ALSA library.  Video display utilizes the Qt library.
 *
 * This application is build around a group of Processes, each running on its own thread.
 * These processes produce or consume a buffer of data before sending it to the next Process.
 * Processes:
 * - TCPInProcessor x2
 *      - one for each, reads in audio and video frames
 * - AudioProcessor - plays a buffer of audio data
 * - VideoProcessor - displays a buffer of image data
 *
 * The main() method sets up the processes and starts them
 *
 * @brief main sets up the processes and starts them (tcp readers and video and audio players)
 * @param argc -- the arg count of argv
 * @param argv --  the arguments run with this application
 * @return Qt app.exec()
 */
int main(int argc, char *argv[])
{
    int port, width, height;
    // -------------------------------------------------------------
    // Check args

    if (argc < 5) {
       std::wcerr << "usage: " << argv[0] << " port img-width img-height audioDeviceName" << std::endl;
       std::wcerr.flush();
       exit(0);
    }

    port = atoi(argv[1]);

    width = atoi(argv[2]);
    height = atoi(argv[3]);

    char audioDeviceName[255];
    strcpy(audioDeviceName, argv[4]);


    TCPInProcessor* tcpVideoProcessor;
    TCPInProcessor* tcpAudioProcessor;
    VideoProcessor* videoProcessor;
    AudioProcessor* audioProcessor;

    TCPSocketServer server(port);


    // ***************************************************************
    //                             AUDIO
    // ***************************************************************

    // ---------------------------------------------------------------
    // Create speaker

    std::cout << "Creating speaker..." << std::endl;

    AudioInterface* speaker = new AudioInterface(audioDeviceName, SAMPLING_RATE, NUMBER_OF_CHANNELS, SND_PCM_STREAM_PLAYBACK);
    speaker->open();

    // ---------------------------------------------------------------
    // Create processes

    std::cout << "Setting up Audio processes..." << std::endl;

    int audioSocket = server.acceptConnection();

    int bufCount = 4;
    int bufSize = speaker->getRequiredBufferSize();

    // create bufs ready for socket to use
    tcpAudioProcessor = new TCPInProcessor(audioSocket, 0, bufSize);
    audioProcessor = new AudioProcessor(*speaker, bufCount, bufSize);

    audioProcessor->prev = tcpAudioProcessor;
    tcpAudioProcessor->prev = audioProcessor;

    std::cout << "...Set up Audio processes." << std::endl;



    // ---------------------------------------------------------------
    // Create UI

    std::cout << "Creating UI..." << std::endl;
    QApplication app(argc, argv);
    VideoViewer videoViewer;

    // ***************************************************************
    //                             VIDEO
    // ***************************************************************

    // ---------------------------------------------------------------
    // Create processes

    std::cout << "Setting up video processes..." << std::endl;

    int videoSocket = server.acceptConnection();

    bufCount = 4;
    bufSize = width*height;

    // create bufs ready for socket to use
    tcpVideoProcessor = new TCPInProcessor(videoSocket, 0, bufSize);
    videoProcessor = new VideoProcessor(videoViewer, bufCount, bufSize);

    videoProcessor->prev = tcpVideoProcessor;
    tcpVideoProcessor->prev = videoProcessor;

    std::cout << "...set up video processes." << std::endl;


    // ---------------------------------------------------------------
    // Start processes

    // videoViewer UI handles close operation
    // give it access to processes to shutdown
    // (could have improved separation of concerns)
    std::vector<Processor*>* procs =  new std::vector<Processor*>;
    procs->push_back(tcpVideoProcessor);
    procs->push_back(videoProcessor);
    procs->push_back(tcpAudioProcessor);
    procs->push_back(audioProcessor);

    videoViewer.processes = procs;

    std::cout << "Starting processes..." << std::endl;
    videoProcessor->start();
    tcpVideoProcessor->start();
    audioProcessor->start();
    tcpAudioProcessor->start();
    std::cout << "...processes started." << std::endl;

#if defined(Q_OS_SYMBIAN)
    videoViewer.showMaximized();
#else
    videoViewer.show();
#endif


    return app.exec();
}
