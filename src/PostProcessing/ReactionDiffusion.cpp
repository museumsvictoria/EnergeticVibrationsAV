//
//  ReactionDiffusion.cpp
//  vertexshaderart
//
//  Created by Joshua Batty on 20/9/17.
//
//

#include "ReactionDiffusion.h"

//--------------------------------------------------------------
void ReactionDiffusion::init_fbos(){
    
    /// Shader FBOs
    ///-------------------------
    ofFbo::Settings fboSettings;
    fboSettings.width = ofGetWidth();
    fboSettings.height = ofGetHeight();
    fboSettings.internalformat = GL_RGBA;
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
    
    m_fbos[ 0 ].getTexture().bind( 3 );
    m_fbos[ 1 ].getTexture().bind( 4 );
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
void ReactionDiffusion::setup(){
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    //ofDisableArbTex();
    init_params();
    
    reaction_shader_bufA.load("shaders/passthrough.vert","shaders/ReactionDiffusion_BufA.frag");
    reaction_shader_image.load("shaders/passthrough.vert","shaders/ReactionDiffusion_Image.frag");
    
    reaction_shader_image.begin();
    reaction_shader_image.setUniform1i( "iChannel0", 4);
    reaction_shader_image.end();
    
    createFullScreenQuad();
    init_fbos();
    
    intensity = 1.0;
    bleed_amount = 0.0;
    dry_wet = 0.0;
    
    set_generative_param(&intensity,0.0,1.0);
    set_generative_param(&bleed_amount,0.0,1.0);
    set_generative_param(&dry_wet,0.0,1.0);
    init_generative();
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
void ReactionDiffusion::update(){
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
        reaction_shader_image.setUniform1f("dry_wet", dry_wet);

        m_fsQuadVbo.draw();
        reaction_shader_image.end();
        
    }
    m_renderFbo.end();
    
    glDisable( GL_CULL_FACE );
}

//--------------------------------------------------------------
void ReactionDiffusion::draw(){
    
    /// Draw To Screen
    ////////////////////
    ofSetColor(ofColor::white);
    m_renderFbo.draw(0,0,ofGetWidth(),ofGetHeight());
}

ofFbo& ReactionDiffusion::getFbo(){
    return m_renderFbo;
}
