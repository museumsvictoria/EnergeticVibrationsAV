//
//  Primitives.hpp
//  vertexshaderart
//
//  Created by Joshua Batty on 13/9/17.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "OptimisedBox.h"

class Primitives{
public:
    void setup();
    void init();
    void draw_idle_mesh();
    void draw_active_mesh();
    
    void randomise_objects();
    void randomise_mesh_resolution();
    
    OptimisedBox optimisedBox;
    ofSpherePrimitive sphere;
    ofIcoSpherePrimitive icoSphere;
    ofCylinderPrimitive cylinder;
    ofConePrimitive cone;
    ofBoxPrimitive box;
    
    ofVboMesh idle_primitive_mesh;
    ofVboMesh active_primitive_mesh;
    
    vector<of3dPrimitive> primitives;
    
private:
    int idle_idx;
    int active_idx;
};
