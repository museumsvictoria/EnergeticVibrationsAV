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
    
    idle_dir.listDir("movies/active/");
    idle_dir.sort();
    
    active_dir.listDir("movies/active/");
    active_dir.sort();
    
    
    // step 1: load our height map image
    
    ofDisableArbTex(); 	///< we need normalised image coordinates
    
    tex_idle.allocate(500, 500, GL_RGBA);
    tex_active.allocate(500, 500, GL_RGBA);

    //ofLoadImage(mTex1, "elevation2.png");
    ofLoadImage(tex_active, "images/Gilmore1.jpg");
    ofLoadImage(tex_idle, "images/Gilmore1.jpg");

    //ofLoadImage(tex, "/Users/joshuabatty/Pictures/gifs/automatagraphics/tripped3.gif");
    load_random_idle_texture();
    load_random_active_texture();

//    ofEnableArbTex();

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
    tex_active = vid_active.getTexture();
    return tex_active;
}
ofTexture& MaterialTextures::getIdleTexture(){
    ofDisableArbTex();
    vid_idle.update();
    tex_idle = vid_idle.getTexture();
    return tex_idle;
}
