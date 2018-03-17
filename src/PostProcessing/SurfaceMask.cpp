//
//  SurfaceMask.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 8/3/18.
//
//

#include "SurfaceMask.h"


//--------------------------------------------------------------
void SurfaceMask::init_fbos(){
    
    /// Shader FBOs
    ///-------------------------
    ofFbo::Settings fboSettings;
    fboSettings.width = ofGetWidth();
    fboSettings.height = ofGetHeight();
    fboSettings.internalformat = GL_RGBA;
    fboSettings.numSamples = 8;//1
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
    
    m_fbos[ 0 ].getTexture().bind( 10 );
    m_fbos[ 1 ].getTexture().bind( 11 );
    
    //------------------------------
    
    /// Final Render FBO
    ///-------------------------
    ofFbo::Settings renderFboSettings;
    renderFboSettings.width = ofGetWidth();
    renderFboSettings.height = ofGetHeight();
    renderFboSettings.internalformat = GL_RGBA;
    renderFboSettings.numSamples = 4;
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
void SurfaceMask::setup(){
    // Use GL_TEXTURE_2D Textures (normalized texture coordinates 0..1)
    //ofDisableArbTex();
    
    shader_image.load("shaders/passthrough.vert","shaders/blend_mask.frag");
    
    shader_image.begin();
    shader_image.setUniform1i( "iChannel0", 10);
    shader_image.end();
    
    createFullScreenQuad();
    init_fbos();
    
    masking_model.loadModel("models/ProjectionSurface.obj");
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    // these rotation set the model so it is oriented correctly
    masking_model.setRotation(0, 90, 1, 0, 0);
    masking_model.setRotation(0, 90, 0, 1, 0);
    masking_model.setScale(1.0, 1.0, 1.0);
    masking_model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    cam.setupPerspective(false, 50, 0.001, 0);
    cam.setDistance(2710); // set default camera distance to 1000
    //mCam1.boom(5); // move camera up a little
    cam.lookAt(ofVec3f(0)); // look at centre of the world
}

//--------------------------------------------------------------
void SurfaceMask::set_active_source_texture(ofFbo& tex){
    m_fbos[ 0 ].begin();
    tex.draw(0,0,ofGetWidth(),ofGetHeight());
    m_fbos[ 0 ].end();
}
//--------------------------------------------------------------
void SurfaceMask::set_idle_source_texture(ofFbo& tex){
    m_fbos[ 1 ].begin();
    tex.draw(0,0,ofGetWidth(),ofGetHeight());
    m_fbos[ 1 ].end();
}

//--------------------------------------------------------------
void SurfaceMask::update(){
    // clear to green as grayScott runs in red and green channels
    ofClear( 0, 255, 0, 255 );
    ofDisableDepthTest();
    
    cam.setDistance(2710); // set default camera distance to 1000
    cam.disableMouseInput();

    m_src_fbo.begin();
    ofClear(0,0,0,0);
    cam.begin();
    ofSetColor(255,255);
    masking_model.getMesh(0).draw();
    cam.end();
    m_src_fbo.end();

    
    /// Final Render
    ////////////////
    m_renderFbo.begin();
    {
        ofClear(0,0,0,255);
        shader_image.begin();
        shader_image.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(),1);
        shader_image.setUniform1f("iTime", ofGetElapsedTimef());
        shader_image.setUniformTexture( "iChannel0", m_src_fbo.getTexture(), 1);
        shader_image.setUniformTexture( "iChannel1",  m_fbos[ 0 ].getTexture(), 2 );
        shader_image.setUniformTexture( "iChannel2",  m_fbos[ 1 ].getTexture(), 3 );

        m_fsQuadVbo.draw();
        shader_image.end();
    }
    m_renderFbo.end();
    
    glDisable( GL_CULL_FACE );
}

//--------------------------------------------------------------
void SurfaceMask::draw(){
    
    /// Draw To Screen
    ////////////////////
    ofSetColor(ofColor::white);
    m_renderFbo.draw(0,0,ofGetWidth(),ofGetHeight());

}

ofFbo& SurfaceMask::getFbo(){
    return m_renderFbo;
}
