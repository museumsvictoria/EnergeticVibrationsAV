//
//  IdleMesh.h
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 9/2/18.
//
//

#pragma once 

#include "ofMain.h"
#include "ofxUbo.h"
#include "Constants.h"
#include "ofxAssimpModelLoader.h"

class IdleMesh{
public:
    void setup(ofxAssimpModelLoader &_model);
    void update();
    void draw(ofTexture &texture, vector<int> chair_states);
    
    bool isShaderDirty;

    ofxAssimpModelLoader model;
private:
    shared_ptr<ofxUboShader> mShd1;

    vector<ofMesh> meshes;
};
