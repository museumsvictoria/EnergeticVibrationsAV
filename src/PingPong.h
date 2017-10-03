//
//  PingPong.h
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 4/10/17.
//
//

#pragma once
#include "ofMain.h"

class PingPong{
    
public:    
    //--------------------------------------------------------------
    void init_fbos(){
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
    void createFullScreenQuad(){
        // -1.0 to +1.0 is the full viewport (screen) if you use these as vertices in your vertex shader
        // (without multiplying by model, view, and projection matrices)
        
        glm::vec3 vertices[4] =
        {
            glm::vec3(  1.0f,  1.0f, 0.0f ),
            glm::vec3( -1.0f,  1.0f, 0.0f ),
            glm::vec3(  1.0f, -1.0f, 0.0f ),
            glm::vec3( -1.0f, -1.0f, 0.0f )
        };
        
        ofIndexType indices[6] =
        {
            0, 1, 2,
            2, 1, 3
        };
        
        // Texture coordinates vary from 0.0 to 1.0 when they are in normalized format
        // ofDisableArbTex() was called earlier set that we're using normalized texture coordinates
        glm::vec2 texCoords[4] =
        {
            glm::vec2( 1.0f, 0.0f ),
            glm::vec2( 0.0f, 0.0f ),
            glm::vec2( 1.0f, 1.0f ),
            glm::vec2( 0.0f, 1.0f )
        };
        
        m_fsQuadVbo.addVertices( vertices, 4 );
        m_fsQuadVbo.addTexCoords( texCoords, 4 );
        m_fsQuadVbo.addIndices( indices, 6 );
    }
    
    ofVboMesh   m_fsQuadVbo;
    
    /// FBOs
    ofFbo       m_fbos[2]; // ping pong fbos
    ofFbo       m_renderFbo; // final render fbo
    ofFbo       m_src_fbo;
};
