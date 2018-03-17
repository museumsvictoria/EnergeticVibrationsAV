//
//  Downsample.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 18/2/18.
//
//

#include "Downsample.h"


//--------------------------------------------------------------
void Downsample::init_fbos(){
    
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
    
    m_renderFbo.allocate( renderFboSettings );
    m_src_fbo.allocate( renderFboSettings );
}

//--------------------------------------------------------------
void Downsample::setup(){
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    //ofDisableArbTex();
    
    shader.load("shaders/passthrough.vert","shaders/Downsample.frag");
    
    shader.begin();
    shader.setUniform1i( "iChannel0", 9);
    shader.end();
    
    createFullScreenQuad();
    init_fbos();
    
    dry_wet = 0.0;
    set_generative_param(&dry_wet,0.0,1.0);
    init_generative();
}
//--------------------------------------------------------------
void Downsample::set_source_texture(ofFbo& tex){
    
    m_src_fbo.begin();
    ofClear(0,0,0,0);
    tex.draw(0,0,ofGetWidth(),ofGetHeight());
    m_src_fbo.end();
}


//--------------------------------------------------------------
void Downsample::update(){
   // shader.load("shaders/passthrough.vert","shaders/Downsample.frag");

//    ofDisableDepthTest();
    
    /// Final Render
    ////////////////
    m_renderFbo.begin();
    {
//        ofClear(255,0,0,255);
        ofClear(0,0,0,0);
        shader.begin();
        shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),0);
        shader.setUniform1f("iTime", ofGetElapsedTimef());
        shader.setUniform1f("dry_wet", dry_wet);
        shader.setUniformTexture( "iChannel0", m_src_fbo.getTexture(), 1 );
        m_fsQuadVbo.draw();
        shader.end();
        
    }
    m_renderFbo.end();
    
//    glDisable( GL_CULL_FACE );
}

//--------------------------------------------------------------
void Downsample::draw(){
    
    /// Draw To Screen
    ////////////////////
    ofSetColor(ofColor::white);
    m_renderFbo.draw(0,0,ofGetWidth(),ofGetHeight());
}

//--------------------------------------------------------------
ofFbo& Downsample::getFbo(){
    return m_renderFbo;
}
