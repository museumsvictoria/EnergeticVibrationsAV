//
//  Primitives.h
//  vertexshaderart
//
//  Created by Joshua Batty on 13/9/17.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "OptimisedBox.h"
#include "ofxAssimpModelLoader.h"

class Primitives{
public:
    void setup();
    void init();
    void load_model(string path);
    
    void draw_idle_mesh();
    void draw_active_mesh();
    
    void randomise_objects();
    void randomise_mesh_resolution();
    
    int get_index();
    
    OptimisedBox optimisedBox;
    ofSpherePrimitive sphere;
    ofIcoSpherePrimitive icoSphere;
    ofCylinderPrimitive cylinder;
    ofConePrimitive cone;
    ofBoxPrimitive box;
    ofxAssimpModelLoader model;

    ofVboMesh idle_primitive_mesh;
    ofVboMesh active_primitive_mesh;
    
    vector<of3dPrimitive> primitives;
    
private:
    int idle_idx;
    int active_idx;
};
