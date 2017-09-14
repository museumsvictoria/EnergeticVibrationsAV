//
//  PostProcessing.cpp
//  vertexshaderart
//
//  Created by Joshua Batty on 11/9/17.
//
//

#include "PostProcessing.h"

void PostProcessing::init(){
    depthOfField.setup(ofGetWidth(), ofGetHeight());
}

void PostProcessing::setup(){
    init();
    dof_blur_amount = 1.7;
    dof_focal_range = 100.0;
    dof_focal_distance = 150.0;
}

void PostProcessing::update(){
    // where is the focal plane from the camera
    depthOfField.setFocalDistance(dof_focal_distance);
    
    //usually between 0 and 2 or 3
    depthOfField.setBlurAmount(dof_blur_amount);
    
    // how much of the scene is in focus, smaller number is a narrower focal distance
    depthOfField.setFocalRange(dof_focal_range);
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
