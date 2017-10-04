//
//  AlphaTrails.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 4/10/17.
//
//

#include "AlphaTrails.h"



//--------------------------------------------------------------
void AlphaTrails::setup(){
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    //ofDisableArbTex();
    
    shader_bufA.load("shaders/passthrough.vert","shaders/AlphaTrail_BufA.frag");
    shader_image.load("shaders/passthrough.vert","shaders/AlphaTrail_Image.frag");
    
    shader_image.begin();
    shader_image.setUniform1i( "iChannel0", 4);
    shader_image.end();
    
    createFullScreenQuad();
    init_fbos();
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
void AlphaTrails::draw(){
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
        shader_image.setUniformTexture( "iChannel1", m_src_fbo.getTexture(), 1 );
        m_fsQuadVbo.draw();
        shader_image.end();
        
    }
    m_renderFbo.end();
    
    glDisable( GL_CULL_FACE );
    
    /// Draw To Screen
    ////////////////////
    ofSetColor(ofColor::white);
    m_renderFbo.draw(0,0,ofGetWidth(),ofGetHeight());
}
