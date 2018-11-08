/*
 * audioconfig.h
 *
 *  Configuration values for the AudioInterface class
 */

#ifndef AUDIOCONFIG_H
#define AUDIOCONFIG_H

#define DEFAULT_FRAME_SIZE (4096*4)
#define SAMPLING_RATE (8000)
#define NUMBER_OF_CHANNELS (2)
#define BYTES_PER_SAMPLE (2)

#define DEVICE_NAME_HOST ("default")
#define DEVICE_NAME_EMBEDDED ("plughw:1")


#endif
