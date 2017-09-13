//
//  PostProcessing.cpp
//  vertexshaderart
//
//  Created by Joshua Batty on 11/9/17.
//
//

#include "PostProcessing.h"

void PostProcessing::setup(){
    depthOfField.setup(ofGetWidth(), ofGetHeight());
}

void PostProcessing::update(){
    // where is the focal plane from the camera
    depthOfField.setFocalDistance(ofMap(ofGetMouseX(),0,ofGetWidth(), 0, 550));
    
    //usually between 0 and 2 or 3
    depthOfField.setBlurAmount(0.0);
    
    // how much of the scene is in focus, smaller number is a narrower focal distance
    depthOfField.setFocalRange(0.0);
}

void PostProcessing::begin(){
    depthOfField.begin();

}
void PostProcessing::end(){
    depthOfField.end();
    
}
void PostProcessing::draw(){
    if(ofGetKeyPressed('f')){
        depthOfField.drawFocusAssist(0, 0);
    }
    else{
        depthOfField.getFbo().draw(0, 0);
    }
}
