//
//  HttpStream.hpp
//  HttpStreamInputLevels
//
//  Created by Bastian Schwarzmüller on 28/04/2020.
//  Copyright © 2020 PlugInAndPlay. All rights reserved.
//

#ifndef HttpStream_h
#define HttpStream_h

#include <JuceHeader.h>
#include <string.h>
#include <curl/curl.h>

// =====    Define HTTP Properties

#define HTTP_ENDPOINT   "https://httpbin.org/post"

// =====    Define PlugIn Properties

#define NUMBER_OF_CHANNELS  16

// =====

struct channelStruct    // Struct Channel mit einer BufferID und einem Mittelwert der RMS Werte pro Sekunde
{
    float RMSLevelMeanPerSecond = 0;    // Hier werden die Werte zunächst aufsummiert und dann durch die buffesPerSecond geteilt
    unsigned int bufferID = 0;          // Dient als Counter, wieviele Buffer vom Processor übergeben wurden
};

// =====

class HttpStream
{
public:
    HttpStream();
    
    double sampleRate;
    int RMSLeveldB[NUMBER_OF_CHANNELS];
    unsigned int bufferSize;
    unsigned int buffersPerSecond;
    
    void getRMSLevelPerChannel(int &channelID, float &RMSLevel);
    void setBufferSize(int &bufferSize);
    unsigned int getBufferSize();
    void setSampleRate(double &sampleRate);

private:
    channelStruct channel[NUMBER_OF_CHANNELS];
    void sendHttpRequest(void);
    void resetValues(void);
};


#endif /* HttpStream_h */
