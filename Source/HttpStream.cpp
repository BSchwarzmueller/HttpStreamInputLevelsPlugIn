//
//  HttpStream.cpp
//  HttpStreamInputLevels
//
//  Created by Bastian Schwarzmüller on 28/04/2020.
//  Copyright © 2020 PlugInAndPlay. All rights reserved.
//

#include "HttpStream.h"

// =====

HttpStream::HttpStream()
{
    // Wird am Anfang auf 0 gesetzt um den Wert im Processblock zuzuweisen falls 0
    this->bufferSize = 0;
}

// =====

void HttpStream::getRMSLevelPerChannel(int &channelID, float &RMSLevel)
{
    //  Pegelwerte werden pro Channel aufsummiert
    this->channel[channelID].RMSLevelMeanPerSecond += RMSLevel;
    //  und die Anzahl der Buffer mitgezählt
    this->channel[channelID].bufferID++;
    
    // Wenn eine Sekunde vorbei ist
    if(this->channel[channelID].bufferID == this->buffersPerSecond)
    {
        //  Wird der Mittelwert berechnet, indem die aufsummierten Werte durch die Anzahl der Buffer geteilt wird
        this->channel[channelID].RMSLevelMeanPerSecond /= this->buffersPerSecond;
        // Wenn der Mittelwert des letzten Channels brechnet wurde
        if(channelID == NUMBER_OF_CHANNELS - 1)
        {
            sendHttpRequest();  // Werte abschicken
            resetValues();      // Channel auf 0 setzen
        }
    }
}

// =====

void HttpStream::sendHttpRequest()
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    Logger* log = Logger::getCurrentLogger();
    
    String data = "{\n";
    
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++)
    {
        // Um die Werte lesbarer zu machen werden die normalisierten Pegelwerte in dBFS umgerechnet
        this->RMSLeveldB[i] = static_cast<int>(Decibels::gainToDecibels(this->channel[i].RMSLevelMeanPerSecond));
        data.append("\t",2);
        data.append(String(i),2);
        data.append(":",1);
        data.append(String(static_cast<int>(Decibels::gainToDecibels(this->channel[i].RMSLevelMeanPerSecond))),3);
        data.append("\n",2);
    }
    data.append("}",1);
    
    log->writeToLog(data);
    
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, HTTP_ENDPOINT);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "http");   // ich nehme an es ist http da es ein lokaler Server ist
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Connection: keep-alive");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, &data);
        res = curl_easy_perform(curl);
    }
}

// =====

unsigned int HttpStream::getBufferSize()
{
    return this->bufferSize;
}

// =====

void HttpStream::setBufferSize(int &bufferSize)
{
    this->bufferSize = bufferSize;
    this->buffersPerSecond = static_cast<int>(this->sampleRate/this->bufferSize);
}
// =====

void HttpStream::setSampleRate(double &sampleRate){
    this->sampleRate = sampleRate;
}

// =====
void HttpStream::resetValues()
{
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++)
    {
        this->channel[i].bufferID = 0;
        this->channel[i].RMSLevelMeanPerSecond = 0;
    }
}
