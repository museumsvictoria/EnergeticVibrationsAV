//
//  MaterialTextures.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 1/12/17.
//
//

#pragma once 

#include "ofMain.h"
#include "PingPong.h"

class MaterialTextures : public PingPong{
public:
    void setup();
   
    void load_idle_texture(string path);
    void load_active_texture(string path);
    
    void load_random_idle_texture();
    void load_random_active_texture();
    
    ofTexture& getIdleTexture();
    ofTexture& getActiveTexture();
    
    ofVideoPlayer vid_active;
    ofVideoPlayer vid_idle;

    int idle_idx;
    int active_idx;
    
    string idle_path;
    string active_path;
    
private:
    ofFbo fbo_active;
    
    ofDirectory active_dir;
    ofDirectory idle_dir;
    
    ofShader shader;

    vector<glm::vec4> colours;
    
    vector<ofTexture> idle_textures;
    vector<ofTexture> active_textures;

};
