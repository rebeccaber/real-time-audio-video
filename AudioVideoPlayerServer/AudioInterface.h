/*
 * AudioInterface.h
 *
 *  Created on: Mar 28, 2018
 *      Author: Rebecca Bernstein
 *
 *      This class interfaces with the ALSA library for audio capture and playback.
 *      An instance may be created to either stream data to (playback)
 *      or data from (capture) the sound card using ALSA.
 */

#ifndef AUDOINTERFACE_H_
#define AUDOINTERFACE_H_

#include <alsa/asoundlib.h>

class AudoInterface {
public:
    /**
     * Input streaming direction for which an instance may be configured.
     * Used as a param in the constructor.
     */
    const static snd_pcm_stream_t  stream_playback = SND_PCM_STREAM_PLAYBACK;

    /**
     * Output streaming direction for which an instance may be configured.
     * Used as a param in the constructor.
     */
    const static snd_pcm_stream_t  stream_capture = SND_PCM_STREAM_CAPTURE;

private:
    /** PCM handle.  Device identifier. */
    snd_pcm_t *handle;

    /** ASCII identifier of the PCM handle.  Device names are defined by ALSA. */
    char* deviceName; // default to "default"

    /** Samples per second */
    unsigned int samplingRate;

    /** Frames per period (or per buffer) */
    snd_pcm_uframes_t frames;

    /** Contains a set of possible PCM hardware configurations */
    snd_pcm_hw_params_t *hw_params;

    /** The number of channels.  Typically two. */
    int channels;

    /**
     * The direction that this class will stream the audio.
     * Either:  SND_PCM_STREAM_CAPTURE or SND_PCM_STREAM_PLAYBACK
     *          See class constants stream_capture and stream_playback
     */
    snd_pcm_stream_t stream;

public:
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
    AudoInterface(char* deviceName, unsigned int samplingRate,
            int channels, snd_pcm_stream_t stream);

    /**
     * @brief AudioInterface::~AudioInterface - Class destructor.
     */
    virtual ~AudoInterface();

    /**
     * @brief AudioInterface::open - Configure settings for the PCM device and open it
     *                                 for playback or capture
     */
    void open();

    /**
     * @brief AudioInterface::close - Shut down this instance: stop the stream & release resources
     */
    void close();


    /**
     * @brief AudioInterface::read - Reads a segment of audio capture data into a buffer
     *      (See getRequiredBufferSize())
     *
     * @param buffer - the buffer to which to write the data
     * @return - the number of samples (not bytes) written into the buffer
     */
    int read(char* buffer);

    /**
     * @brief AudioInterface::write - Writes data to the soundcard for playback.
     *
     * @param buffer - The buffer that will be played back on the audio device.
     * 					It must be of size getRequiredBufferSize().
     * @param bufferLength - The number of valid bytes in the buffer.
     */
    void write(char* buffer, int bufferLength);

    /**
     * @brief AudioInterface::getRequiredBufferSize -
     *      Calculate the necessary size of the buffer based on the frame count between
     *      interrupts and the frame size
     *
     * @return - the required buffer size (in bytes)
     */
    int getRequiredBufferSize();
};

#endif /* AUDOINTERFACE_H_ */
