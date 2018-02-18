//
//  Downsample.h
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 18/2/18.
//
//

#pragma once
#include "ofMain.h"
#include "PingPong.h"

class Downsample : public PingPong{
    
public:
    void setup();
    void init_fbos();
    void set_source_texture(ofFbo& tex);
    void update();
    void draw();
    
    ofFbo& getFbo();
    
    ofShader shader;
    
    float dry_wet;
    
};
