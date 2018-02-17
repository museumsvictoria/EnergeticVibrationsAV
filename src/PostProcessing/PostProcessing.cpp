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
    
    dof_blur_amount = 0.45;
    dof_focal_range = 200.0;
    dof_focal_distance = 200.0;
    
    trail_delay = 0.05;
    
    reaction_diffusion.setup();
    //alpha_trails.setup();
    
    createFullScreenQuad();
    blend_shader.load("shaders/passthrough.vert","shaders/blend_mask.frag");
    
    masking_model.loadModel("models/WallShape.obj");
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    // these rotation set the model so it is oriented correctly
    masking_model.setRotation(0, 90, 1, 0, 0);
    masking_model.setRotation(0, 90, 0, 1, 0);
    masking_model.setScale(0.9, 0.9, 0.9);
    masking_model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    cam.setupPerspective(false, 50, 0.001, 0);
    cam.setDistance(3200); // set default camera distance to 1000
    //mCam1.boom(5); // move camera up a little
    cam.lookAt(ofVec3f(0)); // look at centre of the world
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
    
    /// PASS IN TEXTURE TO ALPHA TRAILS
    ////////////////////
    alpha_trails.set_delay_amount(trail_delay);
    //alpha_trails.set_source_texture(depthOfField.getFbo());
    //alpha_trails.update();
    
    /// PASS IN TEXTURE TO REACTION DUFFUSION
    ////////////////////
    reaction_diffusion.set_source_texture(depthOfField.getFbo());
    

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
    cam.setDistance(3200); // set default camera distance to 1000
    
//    ofFbo fbo;
//    fbo.allocate(ofGetWidth(), ofGetHeight());
//    
//    fbo.begin();
//    ofClear(0,0,0,0);
//    cam.begin();
//    ofPushStyle();
//    ofScale (1,-1,1);
//    ofSetColor(255,255);
//    masking_model.getMesh(0).draw();
//    ofPopStyle();
//    cam.end();
//    fbo.end();
//    
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    
//    ofSetColor(255,255);
////    fbo.draw(0,0);
//
//    
//    if(ofGetKeyPressed('f')){
//      //  depthOfField.drawFocusAssist(0, 0);
//    }
//    else{
//      //  depthOfField.getFbo().draw(0, 0);
//    }
//    
//    blend_shader.load("shaders/passthrough.vert","shaders/blend_mask.frag");
//
//    
//    depthOfField.getFbo().getTexture().bind();
//    blend_shader.begin();
//    
//    blend_shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0);
//    blend_shader.setUniformTexture("iChannel0", fbo.getTexture(), 0);
//    blend_shader.setUniformTexture("iChannel1", depthOfField.getFbo().getTexture(), 1);
//    m_fsQuadVbo.draw();
//
//    blend_shader.end();
//    depthOfField.getFbo().getTexture().unbind();
//
////    reaction_diffusion.draw();
////    alpha_trails.draw();
//    
//   // ofDisableBlendMode();
    
    //depthOfField.getFbo().draw(0, 0);
    
    if(toggle_reaction_diffusion){
        reaction_diffusion.draw();
    } else {
        alpha_trails.draw();
    }

}
