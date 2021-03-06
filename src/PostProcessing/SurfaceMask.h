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
    void set_active_source_texture(ofFbo& tex);
    void set_idle_source_texture(ofFbo& tex);
    void update();
    void draw();
    
    ofFbo& getFbo();

    ofShader shader_image;
    ofxAssimpModelLoader masking_model;
    ofEasyCam cam;
    
    int debug_mode;

};
