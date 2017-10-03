//
//  VisualRingBuffer.h
//  example
//
//  Created by Joshua Batty on 6/05/2016.
//
//

#pragma once

#include "ofMain.h"
#include "VideoBuffer.h"
#include "VideoHeader.h"
#include "TextureGrabber.h"
#include "VideoRenderer.h"
#include "VideoMixerGPU.h"

class VisualRingBuffer{
    
public:
    ~VisualRingBuffer();
    void setup(int w, int h);
    void setupBufferTrails(int w, int h, int bufferNumFrames);
    void setupBufferChaser(int w, int h, int bufferNumFrames, int numHeaders);
    
    
    void stepChaserForwardOneFrame();
    void stepTrailsForwardOneFrame();

    void setTexture(ofVideoGrabber grabber);
    void setTexture(ofVideoPlayer player);
    void setTexture(ofTexture texture);
    void setTexture(ofFbo fbo);
    
    void drawTrails(int x, int y, int w, int h);
    void drawChaserFrame(int num, int x, int y, int w, int h);
    
    ofTexture& getChaserFrameRef(int num);
    
    void toggleRecord();
    
    void begin();
    void end();
    
    void setFps(int fps);
    void setDelayMs(int delay);
    void setWidth(int width);
    void setHeight(int height);
    
    int getNumHeaders();
    
    ofxPm::TextureGrabber tGrabber;
    ofxPm::VideoBuffer vBuffer;
    ofxPm::VideoMixerGPU vMixer;
    vector<ofxPm::VideoHeader*> vHeaders;
    
private:
    int width;
    int height;
};
