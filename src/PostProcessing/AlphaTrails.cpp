//
//  AlphaTrails.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 4/10/17.
//
//

#include "AlphaTrails.h"


//--------------------------------------------------------------
void AlphaTrails::init_fbos(){
    
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
    ofClear( 255, 0, 0, 255 );
    m_fbos[ 0 ].end();
    
    m_fbos[ 1 ].begin();
    ofClear( 255, 0, 0, 255 );
    m_fbos[ 1 ].end();
    
    m_fbos[ 0 ].getTexture().bind( 5 );
    m_fbos[ 1 ].getTexture().bind( 6 );
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
void AlphaTrails::setup(){
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    //ofDisableArbTex();
    
    shader_bufA.load("shaders/passthrough.vert","shaders/AlphaTrail_BufA.frag");
    shader_image.load("shaders/passthrough.vert","shaders/AlphaTrail_Image.frag");
    
    shader_image.begin();
    shader_image.setUniform1i( "iChannel0", 5);
    shader_image.end();
    
    createFullScreenQuad();
    init_fbos();
    
    delay = 0.0;
}
//--------------------------------------------------------------
void AlphaTrails::set_source_texture(ofFbo& tex){
    
    m_src_fbo.begin();
    tex.draw(0,0,ofGetWidth(),ofGetHeight());
    m_src_fbo.end();
}

//--------------------------------------------------------------
void AlphaTrails::runSimulation()
{
    /// Draw GrayScott
    ////////////////////
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    shader_bufA.begin();
    shader_bufA.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),1);
    shader_bufA.setUniform1f("delay", delay);
    shader_bufA.setUniform1f("iTime", ofGetElapsedTimef());
    shader_bufA.setUniform1i("iFrame", ofGetFrameNum());

    shader_bufA.setUniformTexture( "iChannel1", m_src_fbo.getTexture(), 1 );
    
    int fboIndex = 0;
    for ( int i = 0; i < 2; ++ i )
    {
        fboIndex = i % 2;
        shader_bufA.setUniformTexture( "iChannel0", m_fbos[ 1 - fboIndex ].getTexture(), 2 );
        
        m_fbos[ fboIndex ].begin();
        //ofClear( 255, 0, 0, 255 );
        m_fsQuadVbo.draw();
        m_fbos[ fboIndex ].end();
    }
    shader_bufA.end();
}

//--------------------------------------------------------------
void AlphaTrails::set_delay_amount(float _delay){
    delay = _delay;
}

//--------------------------------------------------------------
void AlphaTrails::update(){
    // clear to green as grayScott runs in red and green channels
    ofClear( 0, 255, 0, 255 );
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
        ofClear(255,0,0,255);
        shader_image.begin();
        shader_image.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),1);
        shader_image.setUniform1f("iTime", ofGetElapsedTimef());
        shader_image.setUniform1f("delay", delay);
        shader_image.setUniformTexture( "iChannel1", m_src_fbo.getTexture(), 1 );
        m_fsQuadVbo.draw();
        shader_image.end();
        
    }
    m_renderFbo.end();
    
    glDisable( GL_CULL_FACE );
}

//--------------------------------------------------------------
void AlphaTrails::draw(){

    /// Draw To Screen
    ////////////////////
    ofSetColor(ofColor::white);
    m_renderFbo.draw(0,0,ofGetWidth(),ofGetHeight());
}

ofFbo& AlphaTrails::getFbo(){
    return m_renderFbo;
}
