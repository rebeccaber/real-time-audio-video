/*
 * Camera.h
 *
 * Author: Rebecca Bernstein
 *
 */

#ifndef CAMERA_H_
#define CAMERA_H_
#include <mutex>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

/**
 * @class Camera class handles photo and video capture functions using the OpenCV library.
 * Video feed grabbing runs in its own thread.
 *
 * @brief Camera class handles photo and video capture functions using the OpenCV library.
 */
class Camera {
public:
	Camera(Camera &c);
	Camera(int width, int height);
	virtual ~Camera();

	virtual void run();
	virtual void shutdown();
	virtual void start();
    virtual void storePicture(int pictureType, int number);
	int getPicture(std::vector<unsigned char>* buffer);

    friend void* startCamera(void*);

	enum IMG_TYPE{ JPG, PNG };

private:
    /**
     * @brief cameraMutex - exclusive ownership of the VideoCapture
     */
	mutex cameraMutex;

    /**
     * @brief capture - OpenCV camera object
     */
	VideoCapture* capture;

    /**
     * @brief height - image capture height
     */
	int height;

    /**
     * @brief width - image capture width
     */
	int width;

    /**
     * @brief threadRunning - run continuous video capture while True
     */
	bool threadRunning;

    /**
     * @brief thread - Video capture runs continuously on this thread
     */
	pthread_t thread;


    // Initialise class-level vars for faster image capture (frame, ext, compression_params):
    /**
     * @brief frame - stores a video frame
     */
	Mat frame;

    /**
     * @brief ext - type of image to store
     */
	const String& ext = ".jpg";

    /**
     * @brief compression_params - store config or fast img capture, non-concurrent var
     */
	vector<int> compression_params;
};

#endif /* CAMERA_H_ */
