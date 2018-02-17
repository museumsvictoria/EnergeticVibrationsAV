//
//  ReactionDiffusion.cpp
//  vertexshaderart
//
//  Created by Joshua Batty on 20/9/17.
//
//

#include "ReactionDiffusion.h"


//--------------------------------------------------------------
void ReactionDiffusion::setup(){
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    //ofDisableArbTex();
    intensity = 1.0;
    bleed_amount = 0.0;
    
    reaction_shader_bufA.load("shaders/passthrough.vert","shaders/ReactionDiffusion_BufA.frag");
    reaction_shader_image.load("shaders/passthrough.vert","shaders/ReactionDiffusion_Image.frag");
    
    reaction_shader_image.begin();
    reaction_shader_image.setUniform1i( "iChannel0", 4);
    reaction_shader_image.end();
    
    createFullScreenQuad();
    init_fbos();
}
//--------------------------------------------------------------
void ReactionDiffusion::set_source_texture(ofFbo& tex){
    
    m_src_fbo.begin();
    tex.draw(0,0,ofGetWidth(),ofGetHeight());
    m_src_fbo.end();
}

//--------------------------------------------------------------
void ReactionDiffusion::runSimulation()
{
    /// Draw GrayScott
    ////////////////////
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    reaction_shader_bufA.begin();
    reaction_shader_bufA.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),1);
    reaction_shader_bufA.setUniform1f("iTime", ofGetElapsedTimef());
    reaction_shader_bufA.setUniform1i("iFrame", ofGetFrameNum());
    reaction_shader_bufA.setUniform1f("intensity", intensity);
    reaction_shader_bufA.setUniform1f("bleed_amount", bleed_amount);
    
    reaction_shader_bufA.setUniformTexture( "iChannel1", m_src_fbo.getTexture(), 1 );
    
    int fboIndex = 0;
    for ( int i = 0; i < 2; ++ i )
    {
        fboIndex = i % 2;
        reaction_shader_bufA.setUniformTexture( "iChannel0", m_fbos[ 1 - fboIndex ].getTexture(), 2 );
        
        m_fbos[ fboIndex ].begin();
        //ofClear( 255, 0, 0, 255 );
        m_fsQuadVbo.draw();
        m_fbos[ fboIndex ].end();
    }
    reaction_shader_bufA.end();
}


//--------------------------------------------------------------
void ReactionDiffusion::draw(){
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
        reaction_shader_image.begin();
        reaction_shader_image.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),1);
        reaction_shader_image.setUniform1f("iTime", ofGetElapsedTimef());
        reaction_shader_image.setUniformTexture( "iChannel1", m_src_fbo.getTexture(), 1 );
        m_fsQuadVbo.draw();
        reaction_shader_image.end();
        
    }
    m_renderFbo.end();
    
    glDisable( GL_CULL_FACE );
    
    /// Draw To Screen
    ////////////////////
    ofSetColor(ofColor::white);
    m_renderFbo.draw(0,0,ofGetWidth(),ofGetHeight());
}
