//
//  MaterialTextures.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 1/12/17.
//
//

#pragma once 

#include "ofMain.h"

class MaterialTextures {
public:
    void setup();
    void load_random_texture();
    string get_random_path();
    ofTexture& getTexture();
    
    ofVideoPlayer vid;

private:
    ofFbo fbo;
    ofTexture tex;
    
    ofDirectory dir;

};
