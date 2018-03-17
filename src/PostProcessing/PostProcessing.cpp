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
    surface_mask.setup();
    
    /// Final Render FBO
    ///-------------------------
    ofFbo::Settings renderFboSettings;
    renderFboSettings.width = ofGetWidth();
    renderFboSettings.height = ofGetHeight();
    renderFboSettings.internalformat = GL_RGBA;
    renderFboSettings.numSamples = 1;
    renderFboSettings.useDepth = false;
    renderFboSettings.useStencil = false;
    renderFboSettings.textureTarget = GL_TEXTURE_2D;
    renderFboSettings.minFilter = GL_LINEAR;
    renderFboSettings.maxFilter = GL_LINEAR;
    renderFboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    renderFboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
    
    ofFbo::Settings dofBuffersSettings;
    dofBuffersSettings.width = ofGetWidth();
    dofBuffersSettings.height = ofGetHeight();
    dofBuffersSettings.internalformat = GL_RGB;
    dofBuffersSettings.numSamples = 0;
    dofBuffersSettings.useDepth = true;
    dofBuffersSettings.useStencil = true;
    dofBuffersSettings.depthStencilAsTexture = true;
    dofBuffersSettings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
    
    m_renderFbo.allocate( dofBuffersSettings );
    // Because we are viewing this fbo through an ofCamera's viewport
    // We need to manually flip the texture.... ?!?!? dumb
    m_renderFbo.getTextureReference().getTextureData().bFlipTexture = true;
    m_renderFbo.begin();
    ofClear(0,0,0,0);
    m_renderFbo.end();
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
    downsample.set_source_texture(m_renderFbo);
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
void PostProcessing::begin(){
    m_renderFbo.begin();
    ofClear(0,0,0,0);

}
void PostProcessing::end(){
    m_renderFbo.end();
    
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

//    reaction_diffusion.draw();

//    m_renderFbo.draw(0,0);
}
