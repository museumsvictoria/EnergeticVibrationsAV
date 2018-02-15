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
    
    void load_random_idle_texture();
    void load_random_active_texture();
    
    string get_random_idle_path();
    string get_random_active_path();
    
    ofTexture& getIdleTexture();
    ofTexture& getActiveTexture();
    
    ofVideoPlayer vid_active;
    ofVideoPlayer vid_idle;

private:
    ofTexture tex_idle;
    ofTexture tex_active;
    
    ofDirectory active_dir;
    ofDirectory idle_dir;

};
