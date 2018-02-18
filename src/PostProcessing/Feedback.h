//
//  Feedback.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 17/2/18.
//
//

#pragma once
#include "ofMain.h"
#include "PingPong.h"

class Feedback : public PingPong{
    
public:
    void setup();
    void init_fbos();
    void set_source_texture(ofFbo& tex);
    void update();
    void draw();
    void runSimulation();
    
    ofFbo& getFbo();
    
    ofShader shader_bufA;
    ofShader shader_image;
    
//private:
    float strength;
    float zoom;
    float x_mult;
    float y_mult;
    
    float x_amp;
    float y_amp;
    
    float x_speed;
    float y_speed;
    
    float rotate_speed;
    float rotate_amp;
};
