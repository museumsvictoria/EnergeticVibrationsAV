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
    
    idle_dir.listDir("movies/idle/");
    idle_dir.sort();
    
    active_dir.listDir("movies/active/");
    active_dir.sort();
    
    createFullScreenQuad();
    shader.load("shaders/colour_palette");
    
    
    ofDisableArbTex(); 	///< we need normalised image coordinates
    
    tex_idle.allocate(500, 500, GL_RGBA);
    tex_active.allocate(500, 500, GL_RGBA);

    fbo_active.allocate(500, 500, GL_RGBA);

    ofLoadImage(tex_active, "images/Gilmore1.jpg");
    ofLoadImage(tex_idle, "images/Gilmore1.jpg");

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
void MaterialTextures::load_random_idle_texture(){
    vid_idle.load(get_random_idle_path());
    vid_idle.play();
}
void MaterialTextures::load_random_active_texture(){
    vid_active.load(get_random_active_path());
    vid_active.play();
}

//-------------------------------------
string MaterialTextures::get_random_idle_path(){
    cout << "random idle path = " << idle_dir.getPath((int)ofRandom(idle_dir.size())) << endl;
    return idle_dir.getPath((int)ofRandom(idle_dir.size()));
}
//-------------------------------------
string MaterialTextures::get_random_active_path(){
    cout << "random active path = " << active_dir.getPath((int)ofRandom(active_dir.size())) << endl;
    return active_dir.getPath((int)ofRandom(active_dir.size()));
}

//-------------------------------------
ofTexture& MaterialTextures::getActiveTexture(){
    ofDisableArbTex();
    vid_active.update();
    
    fbo_active.begin();
    ofClear(0,0,0,0);
    shader.begin();
    shader.setUniform3f("iResolution", fbo_active.getWidth(), fbo_active.getHeight(), 0);
    shader.setUniformTexture("iChannel0", vid_active.getTexture(), 0);
    for(int i = 0; i < colours.size(); i++){
        shader.setUniform4f("color_" + ofToString(1+i), colours[i].r, colours[i].g, colours[i].b, colours[i].a);
    }
    m_fsQuadVbo.draw();
    shader.end();
    fbo_active.end();
    
    tex_active = fbo_active.getTexture();
    return tex_active;
}
ofTexture& MaterialTextures::getIdleTexture(){
    ofDisableArbTex();
    vid_idle.update();
    
    tex_idle = vid_idle.getTexture();
    return tex_idle;
}
