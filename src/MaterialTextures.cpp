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
    
    dir.listDir("movies/");
    dir.sort();
    
    // step 1: load our height map image
    
    ofDisableArbTex(); 	///< we need normalised image coordinates
    //ofLoadImage(mTex1, "elevation2.png");
    ofLoadImage(tex, "images/Gilmore1.jpg");
    
    ofLoadImage(tex, "/Users/joshuabatty/Pictures/gifs/automatagraphics/tripped3.gif");
    vid.load(get_random_path());
    vid.play();
    
    
    /*
    ofFbo::Settings fboSettings;
    fboSettings.width = 512;
    fboSettings.height = 512;
    fboSettings.internalformat = GL_RGBA32F;
    fboSettings.numSamples = 2;
    fboSettings.useDepth = false;
    fboSettings.useStencil = false;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.minFilter = GL_LINEAR;
    fboSettings.maxFilter = GL_LINEAR;
    fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
    fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
    
    fbo.allocate(fboSettings);
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
*/
    //ofEnableArbTex();

}

//-------------------------------------
void MaterialTextures::load_random_texture(){
    vid.load(get_random_path());
    vid.play();
}

//-------------------------------------
string MaterialTextures::get_random_path(){
    return dir.getPath((int)ofRandom(dir.size()));
}

//-------------------------------------
ofTexture& MaterialTextures::getTexture(){
    vid.update();
    tex = vid.getTexture();

    /*
    ofDisableArbTex(); 	///< we need normalised image coordinates

    

    fbo.begin();
    ofClear(0,0,0,0);
    ofSetColor(255,255);
    tex.draw(0,0,512,512);
    fbo.end();
//    ofEnableArbTex();
*/
    return tex;
}
