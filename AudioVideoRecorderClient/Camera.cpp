/*
 * Camera.cpp
 * Author: Rebecca Bernstein
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include "Camera.h"

using namespace std;

/**
 * The constructor initializes a GPIO for a light that turns on while photo is being saved,
 * then configures a VideoCapture for images on the specified size
 * @param port - port number for VideoCapture
 * @param gpioPin - pin number for LED output
 * @param width - image width
 * @param height - image height
 */
Camera::Camera(int width, int height)
{
	this->width = width;
	this->height = height;
	this->capture = new VideoCapture(0);
	this->threadRunning = false;
	this->thread = 0; // initalized in start()

	capture->set(CV_CAP_PROP_FRAME_WIDTH,this->width);
	capture->set(CV_CAP_PROP_FRAME_HEIGHT,this->height);
	// capture->set(CV_CAP_PROP_FPS, 30);

	// If we are unable to connect exit.
	if(!capture->isOpened())
	{
		cout << "Failed to connect to the camera." << endl;
	}

    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(40);
}

/**
 * Constructor used to cast a Camera object.
 * @param c
 */
Camera::Camera(Camera &c) { }

/**
 * Destructor method
 */
Camera::~Camera()
{
	delete capture;
}

/**
 * Image-capture thread continuously grabs a new capture.
 *
 * Retrieve the image separately.
 * (OpenCV recommended use.)
 */
void Camera::run()
{
	while(threadRunning)
	{
		cameraMutex.lock();
		capture->grab();
		cameraMutex.unlock();
	}
}

/**
 * Shutdown by stopping the continuous capture thread.
 */
void Camera::shutdown()
{
	this->threadRunning = false;
	pthread_join(thread, NULL);
}

/**
 * Starts the continuous capture action for the Camera.
 * Friend-type originally used in C-style threads.
 *
 * @param value - Camera object whose thread to start.
 */
void* startCamera(void* value)
{
	Camera *camera = static_cast<Camera*>(value);
	camera->run();
	return 0;
}

/**
 * Creates a new thread and starts it running.
 */
void Camera::start()
{
	if(!threadRunning)
	{
		this->threadRunning = true;
		pthread_create(&thread, NULL, &startCamera, this);
    }

    // wait and make sure camera is connected
    sleep(5); // 5 sec
}

/**
 * Retrieve the most recent image from the VideoCapture.
 * Store it as a JPG in the given buffer.
 *
 * @brief Camera::getPicture
 * @param buffer
 * @return
 */
int Camera::getPicture(std::vector<unsigned char>* buffer){
    // use mutex because capture is shared with the continuous-capture thread
    cameraMutex.lock();
    capture->retrieve(frame, 0);
    cameraMutex.unlock();

    imencode(".jpg", frame, *buffer, compression_params);

    return buffer->size(); // buffer was resized
}

/**
 * Retrieves an image frame from the capture stream and stores it.
 * (Used in a previous lab)
 *
 * @param pictureType - a Camera::IMG_TYPE, file type for the stored image
 * @param number - a number to append to the filename
 */
void Camera::storePicture(int pictureType, int number)
{
	struct timespec start_t, stop_t, save_t;

	cout << "Taking picture...\n";

    if( clock_gettime( CLOCK_REALTIME, &start_t) == -1 ) {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }

	Mat frame;

	// use mutex because capture is shared with the continuous-capture thread
	cameraMutex.lock();
	capture->retrieve(frame, 0);
	cameraMutex.unlock();

	if( clock_gettime( CLOCK_REALTIME, &stop_t) == -1 ) {
	  perror( "clock gettime" );
	  exit( EXIT_FAILURE );
	}
	// turn LED light on while storing image
	// cameraLight->setValue(GPIO::HIGH);
	string filename;
	stringstream sstm;

	switch(pictureType)
	{
	case PNG:
		sstm << "capture" << number << ".png";
		break;
	case JPG:
		sstm << "capture" << number << ".jpg";
		break;
	}

	filename = sstm.str();
	imwrite(filename, frame);
	// cameraLight->setValue(GPIO::LOW);
	if( clock_gettime( CLOCK_REALTIME, &save_t) == -1 ) {
	  perror( "clock gettime" );
	  exit( EXIT_FAILURE );
	}
	cout << "Picture saved: "<< filename << endl;

	double accumCapture = (stop_t.tv_sec - start_t.tv_sec)/1000.0
			+ ( stop_t.tv_nsec - start_t.tv_nsec )/ 1000000.0;
	double accumStore = ( save_t.tv_sec - stop_t.tv_sec )/1000.0
			+ ( save_t.tv_nsec - stop_t.tv_nsec )/ 1000000.0;
	std::cout << "Capture elapsed time: " << accumCapture << " ms" << std::endl;
	std::cout << "Storing elapsed time: " << accumStore << " ms" << std::endl;
}
