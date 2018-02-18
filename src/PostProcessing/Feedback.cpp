//
//  Feedback.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 17/2/18.
//
//

#include "Feedback.h"


//--------------------------------------------------------------
void Feedback::init_fbos(){
    
    /// Shader FBOs
    ///-------------------------
    ofFbo::Settings fboSettings;
    fboSettings.width = ofGetWidth();
    fboSettings.height = ofGetHeight();
    fboSettings.internalformat = GL_RGBA32F;
    fboSettings.numSamples = 2;
    fboSettings.useDepth = false;
    fboSettings.useStencil = false;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.minFilter = GL_LINEAR;
    fboSettings.maxFilter = GL_LINEAR;
    fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
    
    // create our FBOs
    m_fbos[ 0 ].allocate( fboSettings );
    m_fbos[ 1 ].allocate( fboSettings );
    
    // setup FBOs
    m_fbos[ 0 ].begin();
    ofClear( 0, 0, 0, 255 );
    m_fbos[ 0 ].end();
    
    m_fbos[ 1 ].begin();
    ofClear( 0, 0, 0, 255 );
    m_fbos[ 1 ].end();
    
    m_fbos[ 0 ].getTexture().bind( 7 );
    m_fbos[ 1 ].getTexture().bind( 8 );
    //------------------------------
    
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
void Feedback::setup(){
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    //ofDisableArbTex();
    
    shader_bufA.load("shaders/passthrough.vert","shaders/Feedback_BufA.frag");
    shader_image.load("shaders/passthrough.vert","shaders/Feedback_Image.frag");
    
    shader_image.begin();
    shader_image.setUniform1i( "iChannel0", 7);
    shader_image.end();
    
    createFullScreenQuad();
    init_fbos();
    
    strength = 0.8;
    zoom = -0.3;
    x_mult = 11.50;
    y_mult = 10.0;
    
    x_amp = 1.0;
    y_amp = 1.0;
    
    x_speed = 0.25;
    y_speed = 0.4;
    
    rotate_speed = 0.2;
    rotate_amp = 0.92;
}
//--------------------------------------------------------------
void Feedback::set_source_texture(ofFbo& tex){
    
    m_src_fbo.begin();
    tex.draw(0,0,ofGetWidth(),ofGetHeight());
    m_src_fbo.end();
}

//--------------------------------------------------------------
void Feedback::runSimulation()
{
    /// Draw GrayScott
    ////////////////////
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    shader_bufA.begin();
    shader_bufA.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),1);
    shader_bufA.setUniform1f("iTime", ofGetElapsedTimef());
    shader_bufA.setUniform1i("iFrame", ofGetFrameNum());
    shader_bufA.setUniform1f("strength",strength);
    shader_bufA.setUniform1f("zoom", zoom);
    shader_bufA.setUniform1f("x_mult", x_mult);
    shader_bufA.setUniform1f("y_mult", y_mult);
    shader_bufA.setUniform1f("x_amp", x_amp);
    shader_bufA.setUniform1f("y_amp", y_amp);
    shader_bufA.setUniform1f("x_speed", x_speed);
    shader_bufA.setUniform1f("y_speed", y_speed);
    shader_bufA.setUniform1f("rotate_speed", rotate_speed);
    shader_bufA.setUniform1f("rotate_amp", rotate_amp);
    
    shader_bufA.setUniformTexture( "iChannel1", m_src_fbo.getTexture(), 1 );
    
    int fboIndex = 0;
    for ( int i = 0; i < 2; ++ i )
    {
        fboIndex = i % 2;
        shader_bufA.setUniformTexture( "iChannel0", m_fbos[ 1 - fboIndex ].getTexture(), 2 );
        
        m_fbos[ fboIndex ].begin();
//        ofClear( 255, 0, 0, 255 ); // origianl
        ofClear(0,0,0,255);
        m_fsQuadVbo.draw();
        m_fbos[ fboIndex ].end();
    }
    shader_bufA.end();
}

//--------------------------------------------------------------
void Feedback::update(){
    
    // clear to green as grayScott runs in red and green channels
    //ofClear( 0, 255, 0, 255 ); //put me back in
    ofDisableDepthTest();
    
    int numSimulations = 1;
    for ( int i = 0; i < numSimulations; ++i )
    {
        runSimulation();
    }
    
    /// Final Render
    ////////////////
    m_renderFbo.begin();
    {
        //ofClear(255,0,0,255); //put me back in
        ofClear(0,0,0,255);
        shader_image.begin();
        shader_image.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),1);
        shader_image.setUniform1f("iTime", ofGetElapsedTimef());
        shader_image.setUniformTexture( "iChannel1", m_src_fbo.getTexture(), 1 );
        m_fsQuadVbo.draw();
        shader_image.end();
        
    }
    m_renderFbo.end();
    
    glDisable( GL_CULL_FACE );
}

//--------------------------------------------------------------
void Feedback::draw(){
    
    /// Draw To Screen
    ////////////////////
    ofSetColor(ofColor::white);
    m_renderFbo.draw(0,0,ofGetWidth(),ofGetHeight());
}

ofFbo& Feedback::getFbo(){
    return m_renderFbo;
}
