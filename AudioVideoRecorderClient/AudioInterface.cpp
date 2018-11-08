/*
 * AudioInterface.cpp
 *
 *  Created on: Mar 28, 2018
 *      Author: Rebecca Bernstein
 *
 *      Sample code:
 *      - https://www.linuxjournal.com/article/6735
 *      - http://equalarea.com/paul/alsa-audio.html
 *
 *     	This class interfaces with the ALSA library for audio capture and playback.
 *      An instance may be created to either stream data to (playback)
 *      or data from (capture) the sound card using ALSA.
 */

#include "audioconfig.h"
#include <iostream>
#include "AudioInterface.h"

/**
 * @brief AudioInterface::AudioInterface Class constructor.
 * Creates an instance that either streams or plays back audio.
 *
 * @param deviceName - ASCII identifier of the PCM handle.  Device names are defined by ALSA.
 * @param samplingRate - Samples per second
 * @param channels - The number of channels.  Typically two.
 * @param stream - The direction that this class will stream the audio.
 *                  See class constants stream_capture and stream_playback
 */
AudioInterface::AudioInterface(char* deviceName, unsigned int samplingRate,
        int channels, snd_pcm_stream_t stream) {

    // Initialize a new buffer and copy the device name
    int nameLen = strlen(deviceName);
    this->deviceName = new char[nameLen];
    strcpy(this->deviceName, deviceName);

    this->samplingRate = samplingRate;
    this->channels = channels;
    this->stream = stream;
    this->handle = NULL;
    this->hw_params = NULL;
    frames = DEFAULT_FRAME_SIZE;
}

/**
 * @brief AudioInterface::~AudioInterface - Class destructor.
 */
AudioInterface::~AudioInterface() {
    free(this->deviceName);
}

/**
 * @brief AudioInterface::open - Configure settings for the PCM device and open it
 *                                 for playback or capture
 */
void AudioInterface::open() {
    int rc;
    int dir = 0; // sub-unit direction for setting sampling rate (-1,0,1)

    /* Open PCM device for playback or capture. */
    rc = snd_pcm_open(&handle, "default",
            SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    /* Allocate a hardware parameters object. */
      snd_pcm_hw_params_alloca(&hw_params);

      // Fill it in with default values.
      snd_pcm_hw_params_any(handle, hw_params);

      // Set the desired hardware parameters.

      // Interleaved mode */
      snd_pcm_hw_params_set_access(handle, hw_params,
                          SND_PCM_ACCESS_RW_INTERLEAVED);

      // Signed 16-bit little-endian format
      snd_pcm_hw_params_set_format(handle, hw_params,
                                  SND_PCM_FORMAT_S16_LE);

      // Two channels (stereo)
      snd_pcm_hw_params_set_channels(handle, hw_params, 2);

      // Set the sampling rate
      snd_pcm_hw_params_set_rate_near(handle,
              hw_params, &(this->samplingRate), &dir);

      // Copied: -----------------------------------------------------

      // Force an integer period.
        snd_pcm_hw_params_set_period_size_integer(handle, hw_params);


        /* Set period size to the given number of frames. */
        snd_pcm_hw_params_set_period_size_near(handle, hw_params, &frames, &dir);
        printf("Frame size: %d for %d\n", (int)frames, (int)dir );

        /* Write the parameters to the driver */
        rc = snd_pcm_hw_params(handle, hw_params);
        if (rc < 0) {
            fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
            exit(1);
        }
}

/**
 * @brief AudioInterface::close - Shut down this instance: stop the stream & release resources
 */
void AudioInterface::close() {
    snd_pcm_drain(handle); // gracefully stop the stream
    snd_pcm_close(handle); // close pcm handle and release resources
}

/**
 * @brief AudioInterface::getRequiredBufferSize -
 *      Calculate the necessary size of the buffer based on the frame count between
 *      interrupts and the frame size
 *
 * @return - the required buffer size (in bytes)
 */
int AudioInterface::getRequiredBufferSize() {
    int dir; // sub-unit direction
    int bufferSize;

    // calculate frames (frames per period)
    snd_pcm_hw_params_get_period_size(hw_params, &frames, &dir);

    bufferSize = frames * this->channels * 2; /* 2 bytes/sample */
    printf("Buffer size required: %d bytes\n", bufferSize);
    return bufferSize;
}

/**
 * @brief AudioInterface::read - Reads a segment of audio capture data into a buffer
 *      (See getRequiredBufferSize())
 *
 * @param buffer - the buffer to which to write the data
 * @return - the number of samples (not bytes) written into the buffer
 */
int AudioInterface::read(char* buffer) {
    return snd_pcm_readi(handle, buffer, frames);
}

/**
 * @brief AudioInterface::write - Writes data to the soundcard for playback.
 *
 * @param buffer - The buffer that will be played back on the audio device.
 * 					It must be of size getRequiredBufferSize().
 * @param bufferLength - The number of valid bytes in the buffer.
 */
void AudioInterface::write(char* buffer, int bufferLength) {
    int framesWritten;
    int rc; // Used to store the return code from various calls.

    // 2 bytes per sample, "channels" samples per frame
    int frames = bufferLength / (2 * this->channels);

    if (bufferLength > 0) {
      rc = 0;
      framesWritten = 0;
      while (framesWritten<frames)
      {
        // Write the data to the soundcard.
        rc = snd_pcm_writei(this->handle, &buffer[rc*2*this->channels], frames-rc);

        if (rc == -EPIPE) {
            /* EPIPE means underrun */
            fprintf(stderr, "underrun occurred\n");
                snd_pcm_recover(this->handle, rc, 0);
        } else if (rc < 0) {
            fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
        } else if (rc != (int) frames) { // some frames written
            fprintf(stderr, "short write, write %d frames\n", rc);
            framesWritten += rc;
        } else { // all frames written
            framesWritten += rc;
        }
      }
    }
}


