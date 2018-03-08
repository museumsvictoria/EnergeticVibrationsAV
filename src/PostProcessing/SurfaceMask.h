//
//  SurfaceMask.h
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 8/3/18.
//
//

#pragma once
#include "ofMain.h"
#include "PingPong.h"
#include "ofxAssimpModelLoader.h"

class SurfaceMask : public PingPong{
    
public:
    void setup();
    void init_fbos();
    void set_source_texture(ofFbo& tex);
    void update();
    void draw();
    void runSimulation();
    
    ofFbo& getFbo();

    ofShader shader_image;
    ofxAssimpModelLoader masking_model;
    ofEasyCam cam;    

};
