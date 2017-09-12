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
    //swim the depth of field
    //depthOfField.setFocalDistance(ofMap(sin(ofGetElapsedTimef()/2),-1,1, 20, 150));
    depthOfField.setFocalDistance(ofMap(ofGetMouseX(),0,ofGetWidth(), 0, 550));
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
