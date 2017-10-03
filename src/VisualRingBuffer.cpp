//
//  VisualRingBuffer.cpp
//  example
//
//  Created by Joshua Batty on 6/05/2016.
//
//

#include "VisualRingBuffer.h"

using namespace ofxPm;

//--------------------------------------------------------------
VisualRingBuffer::~VisualRingBuffer(){
    for(int i = 0; i < vHeaders.size(); i++){
        delete vHeaders[i];
    }
}

//--------------------------------------------------------------
void VisualRingBuffer::setupBufferTrails(int w, int h, int bufferNumFrames){
    width = w;
    height = h;
    
    tGrabber.initGrabber(width,height);
    
    VideoHeader *vHeader = new VideoHeader();
    vHeaders.push_back(vHeader);
    vHeaders[0]->setup(vBuffer);
    vMixer.setup(tGrabber,*vHeaders[0]);
    vBuffer.setup(tGrabber, bufferNumFrames, true,false);
    vHeaders[0]->setDelayMs(300);
}

//--------------------------------------------------------------
void VisualRingBuffer::setupBufferChaser(int w, int h, int bufferNumFrames, int numHeaders){
    width = w;
    height = h;
    
    tGrabber.initGrabber(width,height);
    
    for(int i = 0; i < numHeaders; i++){
        VideoHeader *vHeader = new VideoHeader();
        vHeaders.push_back(vHeader);
        vHeaders[i]->setup(vBuffer);
        vHeaders[i]->setDelayMs(300);
    }
    
    vBuffer.setup(tGrabber, bufferNumFrames, true,false);
}

//--------------------------------------------------------------
void VisualRingBuffer::stepChaserForwardOneFrame(){
    ofxPm::VideoFrame tGrabberFrame = tGrabber.getNextVideoFrame();
    vBuffer.pushNewVideoFrameTracer(tGrabberFrame);
}

//--------------------------------------------------------------
void VisualRingBuffer::stepTrailsForwardOneFrame(){
    ofxPm::VideoFrame tGrabberFrame = tGrabber.getNextVideoFrame();
    ofxPm::VideoFrame vHeaderFrame = vHeaders[0]->getNextVideoFrame();
    
    ofxPm::VideoFrame mixedFrame;
    if (vHeaderFrame == NULL) {
        mixedFrame = vMixer.mixVideoFrames(tGrabberFrame, tGrabberFrame);
    } else {
        mixedFrame = vMixer.mixVideoFrames(tGrabberFrame, vHeaderFrame);
    }
    vBuffer.pushNewVideoFrameTracer(mixedFrame);
    
    // Steps
    // 1. tGrabber 'VideoFrame' is written to
    // 2. Get a 'VideoFrame' from vHeader
    // 3. Pass in the tGrabber and vHeaders 'VideoFrames' in vMixers draw shader
    // 4. vMixer calls new video fram with back Frame //still unsure about this
    // 5. vMixer sends its VideoFrame to vRate which.
    // 6. ON A SEPARATE THREAD, vRate sends its frame to vBuffer.
    // 7. vBuffer sends its VideoFrame to
}

//--------------------------------------------------------------
void VisualRingBuffer::begin(){
    tGrabber.begin();
    ofClear(0,0,0,0);
}
//--------------------------------------------------------------
void VisualRingBuffer::end(){
    tGrabber.end();
}

//--------------------------------------------------------------
void VisualRingBuffer::setDelayMs(int delay){
    for(int i = 0; i < vHeaders.size(); i++){
        vHeaders[i]->setDelayMs(delay*(i+1));
    }
}

//--------------------------------------------------------------
void VisualRingBuffer::setFps(int fps){
    for(int i = 0; i < vHeaders.size(); i++){
        vHeaders[i]->setFps(fps);
    }
}

//--------------------------------------------------------------
void VisualRingBuffer::setWidth(int width){
    this->width = width;
}
//--------------------------------------------------------------
void VisualRingBuffer::setHeight(int height){
    this->height = height;
}

//--------------------------------------------------------------
void VisualRingBuffer::drawTrails(int x, int y, int w, int h){
//    vMixer.getNextVideoFrame().getTextureRef().draw(x, y, w, h);
    tGrabber.getNextVideoFrame().getTextureRef().draw(1280, y, w, h);

    vHeaders[0]->getNextVideoFrame().getTextureRef().draw(x, y, w, h);
}

//--------------------------------------------------------------
void VisualRingBuffer::drawChaserFrame(int num, int x, int y, int w, int h){
    vHeaders[num]->getNextVideoFrame().getTextureRef().draw(x, y, w, h);
}

//--------------------------------------------------------------
ofTexture& VisualRingBuffer::getChaserFrameRef(int num){
    return vHeaders[num]->getNextVideoFrame().getTextureRef();
}

//--------------------------------------------------------------
int VisualRingBuffer::getNumHeaders(){
    return vHeaders.size();
}

//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofVideoGrabber grabber){
    begin();
    grabber.draw(0, 0, width, height);
    end();
}
//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofVideoPlayer player){
    begin();
    player.draw(0, 0, width, height);
    end();
}
//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofTexture texture){
    begin();
    texture.draw(0, 0, width, height);
    end();
}
//--------------------------------------------------------------
void VisualRingBuffer::setTexture(ofFbo fbo){
    begin();
    fbo.draw(0,0,width,height);
    end();
}

//--------------------------------------------------------------
void VisualRingBuffer::toggleRecord(){
    if(vBuffer.isStopped()){
        vBuffer.resume();
    } else{
        vBuffer.stop();
    }
}

