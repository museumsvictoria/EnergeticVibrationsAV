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
    ofEnableArbTex();
    depthOfField.setup(ofGetWidth(), ofGetHeight());
}


//--------------------------------------------------------------
void PostProcessing::setup(){
    init();
    
    toggle_dof = false;
    toggle_reaction_diffusion = true;
    toggle_feedback = false;
    toggle_down_sampling = false;
    
    dof_blur_amount = 0.05;
    dof_focal_range = 200.0;
    dof_focal_distance = 200.0;
    
    
    downsample.setup();
    reaction_diffusion.setup();
    alpha_trails.setup();
    feedback.setup();
    surface_mask.setup();
    
    createFullScreenQuad();
}

//--------------------------------------------------------------
void PostProcessing::update(){
    
    /// SET DOF PARAMS
    ////////////////////
    
    // where is the focal plane from the camera
    depthOfField.setFocalDistance(dof_focal_distance);
    //	depthOfField.setFocalDistance(ofMap(sin(ofGetElapsedTimef()/2),-1,1, 20, 150));
    
    //usually between 0 and 2 or 3
    depthOfField.setBlurAmount(dof_blur_amount);
    
    // how much of the scene is in focus, smaller number is a narrower focal distance
    depthOfField.setFocalRange(dof_focal_range);
    
    /// PASS IN TEXTURE TO PIXELATOR
    ////////////////////
    downsample.set_source_texture(depthOfField.getFbo());
    downsample.update();
    
    /// PASS IN TEXTURE TO ALPHA TRAILS
    ////////////////////
    alpha_trails.set_source_texture(downsample.getFbo());
    alpha_trails.update();
    
    /// PASS IN TEXTURE TO FEEDBACK
    ////////////////////
    feedback.set_source_texture(alpha_trails.getFbo());
    feedback.update();
    
    /// PASS IN TEXTURE TO REACTION DUFFUSION
    ////////////////////
    reaction_diffusion.set_source_texture(feedback.getFbo());
    reaction_diffusion.update();
    
    /// PASS IN TEXTURE TO SURFACE MASK
    ////////////////////
    surface_mask.set_source_texture(reaction_diffusion.getFbo());
    surface_mask.update();
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
    
//    if(toggle_reaction_diffusion){
//        reaction_diffusion.draw();
//    } else {
//        feedback.draw();
//    }
//    
    surface_mask.draw();

}
