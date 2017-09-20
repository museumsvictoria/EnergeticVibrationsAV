//
//  ReactionDiffusion.hpp
//  vertexshaderart
//
//  Created by Joshua Batty on 20/9/17.
//
//

#pragma once
#include "ofMain.h"

class ReactionDiffusion{
    
public:
    void setup();
    void init_fbos();
    void set_source_texture(ofFbo& tex);
    void draw();
    void createFullScreenQuad();
    void runSimulation();

    ofShader reaction_shader_bufA;
    ofShader reaction_shader_image;
    
    ofVboMesh   m_fsQuadVbo;
    
    /// FBOs
    ofFbo       m_fbos[2]; // ping pong fbos
    ofFbo       m_renderFbo; // final render fbo
    ofFbo       m_src_fbo;
};
