//
//  ReactionDiffusion.hpp
//  vertexshaderart
//
//  Created by Joshua Batty on 20/9/17.
//
//

#pragma once
#include "ofMain.h"
#include "PingPong.h"
#include "Generative.h"

class ReactionDiffusion : public PingPong, public Generative{
    
public:
    void setup();
    void init_fbos();
    void set_source_texture(ofFbo& tex);
    void draw();
    void update();
    void runSimulation();

    ofFbo& getFbo();

    ofShader reaction_shader_bufA;
    ofShader reaction_shader_image;
    
    float intensity;
    float bleed_amount;
    float dry_wet;
};
