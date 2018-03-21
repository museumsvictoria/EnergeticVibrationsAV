//
//  MaterialTextures.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 1/12/17.
//
//

#include "MaterialTextures.h"

//-------------------------------------
void MaterialTextures::setup(){
    
    idle_dir.listDir("images/idle/");
    idle_dir.sort();
    
    active_dir.listDir("images/active/");
    active_dir.sort();
    
    createFullScreenQuad();
    shader.load("shaders/colour_palette");
    
    ofDisableArbTex(); 	///< we need normalised image coordinates

    ///---------------------
    for(int i = 0; i < active_dir.size(); i++){
        active_textures.push_back(ofTexture());
        ofLoadImage(active_textures[i], active_dir.getPath(i));
    }
    for(int i = 0; i < idle_dir.size(); i++){
        idle_textures.push_back(ofTexture());
        ofLoadImage(idle_textures[i], idle_dir.getPath(i));
    }
    ///---------------------
    fbo_active.allocate(500, 500, GL_RGBA);

    load_random_idle_texture();
    load_random_active_texture();
    
    colours.push_back(glm::vec4(0.255,0.745,0.670,1.0)); // aqua
    colours.push_back(glm::vec4(0.235,0.596,0.807,1.0)); // blue
    colours.push_back(glm::vec4(0.545,0.411,0.827,1.0)); // light purple
    colours.push_back(glm::vec4(0.149,0.133,0.384,1.0)); // dark purple
    colours.push_back(glm::vec4(0.305,0.658,0.239,1.0)); // green
    colours.push_back(glm::vec4(0.494,0.098,0.286,1.0)); // mitch magenta
    colours.push_back(glm::vec4(0.000,0.607,0.490,1.0)); // aqua green
    colours.push_back(glm::vec4(0.784,0.180,0.152,1.0)); // orange

    random_shuffle(std::begin(colours), std::end(colours));
}

//-------------------------------------
void MaterialTextures::load_idle_texture(string path){
    for(int i = 0; i < idle_dir.size(); i++){
        if(path == idle_dir.getPath(i)){
            idle_idx = i;
        }
    }
}
void MaterialTextures::load_active_texture(string path){
    for(int i = 0; i < active_dir.size(); i++){
        if(path == active_dir.getPath(i)){
            active_idx = i;
        }
    }
}
void MaterialTextures::load_random_idle_texture(){
    idle_idx = (int)ofRandom(idle_dir.size());
}
void MaterialTextures::load_random_active_texture(){
    active_idx = (int)ofRandom(active_dir.size());
}

//-------------------------------------
ofTexture& MaterialTextures::getActiveTexture(){
    ofDisableArbTex();
    
    active_path = active_dir.getPath(active_idx);
    
    fbo_active.begin();
    ofClear(0,0,0,0);
    shader.begin();
    shader.setUniform3f("iResolution", fbo_active.getWidth(), fbo_active.getHeight(), 0);
    shader.setUniformTexture("iChannel0", active_textures[active_idx], 0);
    for(int i = 0; i < colours.size(); i++){
        shader.setUniform4f("color_" + ofToString(1+i), colours[i].r, colours[i].g, colours[i].b, colours[i].a);
    }
    m_fsQuadVbo.draw();
    shader.end();
    fbo_active.end();
    
    return fbo_active.getTexture();
}
ofTexture& MaterialTextures::getIdleTexture(){
    ofDisableArbTex();
    idle_path = idle_dir.getPath(idle_idx);

    return idle_textures[idle_idx];
}
