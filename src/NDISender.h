//
//  NDISender.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 12/12/17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxNDI.h" // NDI classes

// BGRA definition should be in glew.h
// but define it here just in case it is not
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT 0x80E1
#endif


class NDISender {
public:
    
    void setup();
    void send();
    void draw();
    void exit();
    
    ofxNDIsender ndiSender;    // NDI sender object
    char senderName[256];      // Sender name
    unsigned int senderWidth;  // Width of the sender output
    unsigned int senderHeight; // Height of the sender output
    ofFbo ndiFbo;              // Fbo used for data transfer
    ofPixels ndiBuffer[2];     // Two pixel buffers for async sending
    int idx;                   // Index used for async buffer swapping

    
    GLuint ndiPbo[2];
    int PboIndex;
    int NextPboIndex;
    bool ReadFboPixels(ofFbo fbo, unsigned int width, unsigned int height, unsigned char *data);
    
};
