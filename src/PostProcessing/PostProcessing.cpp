//
//  PostProcessing.cpp
//  vertexshaderart
//
//  Created by Joshua Batty on 11/9/17.
//
//

#include "PostProcessing.h"

//--------------------------------------------------------------
void PostProcessing::init(){
    depthOfField.setup(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void PostProcessing::setup(){
    init();
    dof_blur_amount = 0.17;
    dof_focal_range = 100.0;
    dof_focal_distance = 150.0;
    
    trail_delay = 0.5;
    
    reaction_diffusion.setup();
    //alpha_trails.setup();
}

//--------------------------------------------------------------
void PostProcessing::update(){
    /// SET DOF PARAMS
    ////////////////////
    
    // where is the focal plane from the camera
    depthOfField.setFocalDistance(dof_focal_distance);
    
    //usually between 0 and 2 or 3
    depthOfField.setBlurAmount(dof_blur_amount);
    
    // how much of the scene is in focus, smaller number is a narrower focal distance
    depthOfField.setFocalRange(dof_focal_range);
    
    /// PASS IN TEXTURE TO REACTION DUFFUSION
    ////////////////////
    reaction_diffusion.set_source_texture(depthOfField.getFbo());
    
    /// PASS IN TEXTURE TO ALPHA TRAILS
    ////////////////////
    alpha_trails.set_delay_amount(trail_delay);
    alpha_trails.set_source_texture(depthOfField.getFbo());
}

//--------------------------------------------------------------
void PostProcessing::dof_begin(){
    depthOfField.begin();

}
void PostProcessing::dof_end(){
    depthOfField.end();
    
}
//--------------------------------------------------------------
void PostProcessing::draw(){
    if(ofGetKeyPressed('f')){
        depthOfField.drawFocusAssist(0, 0);
    }
    else{
        depthOfField.getFbo().draw(0, 0);
    }
    
//    reaction_diffusion.draw();
//    alpha_trails.draw();
}
