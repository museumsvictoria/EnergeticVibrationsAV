//
//  AlphaTrails.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 4/10/17.
//
//

#pragma once
#include "ofMain.h"
#include "PingPong.h"
#include "Generative.h"

class AlphaTrails : public PingPong, public Generative{
    
public:
    void setup();
    void init_fbos();
    void set_source_texture(ofFbo& tex);
    void update();
    void draw();
    void runSimulation();
    void set_delay_amount(float _delay);
    
    ofFbo& getFbo();
    
    ofShader shader_bufA;
    ofShader shader_image;
    
    float delay;
};
