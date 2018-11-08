# Audio-Video Streaming Application
A pair of applications stream real-time audio and video in a Linux environment.
### Purpose
This project integrates concepts taught in a real-time systems course.

### Libraries and frameworks
- ALSA (Advanced Linux Sound Architecture) for audio
- OpenCV for video capture
- Qt for video display user interface
- TCP to transmit video and audio frames

### Hardware setup
Development targets:
- a Linux virtual machine development computer.  
- a Beaglebone Black running embedded Linux.  Beaglebone Black uses perhiferals including a USB audio adapter and USB Camera.

### Software design
Producer-consumer problem: produce, transmit, and consume audio and video frames.

Solved through sequencial buffer-processors that run concurrently. The "Processor" abstract class is the base definition of a buffer-processor.

For example, the life of a video frame follows this sequence:
1. Created by _CameraProcessor_
2. Queued for next Processor
3. Consumed by _TCPOutProcessor_
4. Sent across TCP connection.  (Frame is deleted/reused.)
3. Re-created by _TCPInProcessor_
4. Queued for next Processor
4. Consumed by _VideoProcessor_
5. Displayed on screen. (Frame is deleted/reused.)

![alt text](https://raw.githubusercontent.com/chandlerebecca/oneway-video-conference/master/diagrams/Record_ClassDiagram.jpg "Class diagram for the recording application.")
Class diagram for the recording application.

![alt text](https://raw.githubusercontent.com/chandlerebecca/oneway-video-conference/master/diagrams/Playback_ClassDiagram.jpg "Class diagram for the playback application.")
Class diagram for the playback application.

[Sequence diagram for receiving and playing audio frames.](https://raw.githubusercontent.com/chandlerebecca/oneway-video-conference/master/diagrams/Playback_AudioProcessors_SequenceDiagram.png "Sequence diagram for receiving and playing audio frames.")

__Problems Encountered:__
- _Audio buffer underrun_: The soundcard needs audio frames continuously delivered for playback.  If there are no frames available (a frame does not arrive fast enough), there is a break in audio, and a buffer underrun error occurs.  TCP frames do not always arive with precisely the same latency.
<br/>This problem is native to real-time audio across a network.  From my understanding apps like Skype compress or extend the length an audio clip plays to accomodate frames being recieved in a variable time frame.  And, rather than TCP they use UDP which is faster but with unreliable delivery.  It is part of the Realtime Transfer Protocol which is widely used in media streaming.
<br/>On the Beaglebone, the longer latencies can be improved by speeding up the CPU to check for TCP messages more frequently.  Even with this advantage, buffer underruns eventually occur.


### (Short) Build and run instructions:
1. __Build__
a. Install ALSA, OpenCV, and Qt.
b. Build using Qmake or g++.  
   _Qmake:_ Run with the .pro file
   
   _g++:_ Compile and build parameters can be found in the .pro file
   
   Embedded app build: I built on a chroot simulating the Beaglebone's ARM architecture.  The app could also be built directly on the BeagleBone if libraries are installed.
4. __Run__
Run with parameters, start the server first:

   Playback:
   ```
   ./VideoAudioServer [server port] [img height] [img width]  [audio device name]
   ```
   
   Record:
   ```
   ./VideoAudioClient [server ip address] [server port] [img height] [img width] [audio device name]
   ```
   Compatible image dimensions vary by camera.  I used 320x240.
   
   Audio device names are defined by ALSA.  The name `default` may access the computer's default audio.  This worked for desktop Linux.  The Beaglebone used `plughw:1`.

### Author
Rebecca Bernstein
