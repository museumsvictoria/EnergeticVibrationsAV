//
//  PostProcessing.cpp
//  vertexshaderart
//
//  Created by Joshua Batty on 11/9/17.
//
//

#include "PostProcessing.h"

//--------------------------------------------------------------
void PostProcessing::init_params(){
    dof_blur_amount = 0.05;
    dof_focal_range = 200.0;
    dof_focal_distance = 200.0;
}


//--------------------------------------------------------------
void PostProcessing::setup(){
    init_params();
    
    ofEnableArbTex();
    depthOfField.setup(ofGetWidth(), ofGetHeight());
    
    toggle_dof = false;
    toggle_reaction_diffusion = true;
    toggle_feedback = false;
    toggle_down_sampling = false;
    
    downsample.setup();
    reaction_diffusion.setup();
    alpha_trails.setup();
    feedback.setup();
    
    
    ofFbo::Settings renderFboSettings;
    renderFboSettings.width = ofGetWidth();
    renderFboSettings.height = ofGetHeight();
    renderFboSettings.internalformat = GL_RGB;
    renderFboSettings.numSamples = 8;
    renderFboSettings.useDepth = true;
    renderFboSettings.useStencil = true;
    renderFboSettings.depthStencilAsTexture = true;
    renderFboSettings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
    
    m_activeFbo.allocate( renderFboSettings );
    m_idleFbo.allocate( renderFboSettings );
    
    // Because we are viewing this fbo through an ofCamera's viewport
    // We need to manually flip the texture.... ?!?!? dumb
    m_activeFbo.getTextureReference().getTextureData().bFlipTexture = true;
    m_activeFbo.begin();
    ofClear(0,0,0,0);
    m_activeFbo.end();
    
    m_idleFbo.begin();
    ofClear(0,0,0,0);
    m_idleFbo.end();
}

//--------------------------------------------------------------
void PostProcessing::trigger_bypass_mode(){
    downsample.bypass();
    alpha_trails.bypass();
    feedback.bypass();
    reaction_diffusion.bypass();
}
void PostProcessing::trigger_atari_mode(){
    downsample.random_dry_wet();
    alpha_trails.init_params();
    feedback.init_params();
    reaction_diffusion.init_params();
}
void PostProcessing::trigger_trails_mode(){
    downsample.init_params();
    alpha_trails.random_dry_wet();
    feedback.init_params();
    reaction_diffusion.init_params();
}
void PostProcessing::trigger_feedback_mode(){
    downsample.init_params();
    alpha_trails.init_params();
    feedback.randomise_all_params();
    feedback.random_dry_wet();
    reaction_diffusion.init_params();
}
void PostProcessing::trigger_reaction_diffusion_mode(){
    downsample.init_params();
    alpha_trails.init_params();
    feedback.init_params();
    reaction_diffusion.randomise_all_params();
    reaction_diffusion.random_dry_wet();
}
void PostProcessing::trigger_random_combo_mode(){
    downsample.randomise_all_params();
    alpha_trails.randomise_all_params();
    feedback.randomise_all_params();
    reaction_diffusion.randomise_all_params();
}
void PostProcessing::run_combo_perlin_mode(){
    downsample.update_noise_walks();
    alpha_trails.update_noise_walks();
    feedback.update_noise_walks();
    reaction_diffusion.update_noise_walks();
}

//--------------------------------------------------------------
void PostProcessing::update(){
    

    /// SET DOF PARAMS
    ////////////////////
    
    // where is the focal plane from the camera
//    depthOfField.setFocalDistance(dof_focal_distance);
//    //	depthOfField.setFocalDistance(ofMap(sin(ofGetElapsedTimef()/2),-1,1, 20, 150));
//    
//    //usually between 0 and 2 or 3
//    depthOfField.setBlurAmount(dof_blur_amount);
//    
//    // how much of the scene is in focus, smaller number is a narrower focal distance
//    depthOfField.setFocalRange(dof_focal_range);
//    
    /// PASS IN TEXTURE TO PIXELATOR
    ////////////////////
    downsample.set_source_texture(m_activeFbo);
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
}

//--------------------------------------------------------------
void PostProcessing::begin_active(){
    m_activeFbo.begin();
    ofClear(0,0,0,0);
}
void PostProcessing::end_active(){
    m_activeFbo.end();
}
ofFbo& PostProcessing::get_active_fbo(){
    return reaction_diffusion.getFbo();
}
//--------------------------------------------------------------
void PostProcessing::begin_idle(){
    m_idleFbo.begin();
    ofClear(0,0,0,0);
}
void PostProcessing::end_idle(){
    m_idleFbo.end();
}
ofFbo& PostProcessing::get_idle_fbo(){
    return m_idleFbo;
}

//--------------------------------------------------------------
void PostProcessing::draw(){
    // Only need to call this funciton if you dont want to send it through the shape mask
    reaction_diffusion.draw();
}
