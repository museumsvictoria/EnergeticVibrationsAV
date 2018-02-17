//
//  AssimpModelVertices.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 13/12/17.
//
//

#include "AssimpModelVertices.h"

//--------------------------------------------------------------
AssimpModelVertices::AssimpModelVertices(){
    
    // load the first model
    model.loadModel("models/WallShape_v7.obj");
    
    
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    // these rotation set the model so it is oriented correctly
    model.setRotation(0, 90, 1, 0, 0);
    model.setRotation(0, 90, 0, 1, 0);
    model.setScale(0.9, 0.9, 0.9);
    model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    shape.clear();
    for(int i = 0; i < model.getMeshCount(); i++){
        ofVec3f c = model.getMesh(i).getCentroid();
        // Re position the vertices so that the model fits nicely on the screen
        //c.y -= 600.0;
        //c.z -= 4300.0;
        shape.addVertex(c);
    }
    shape.close();
    
}

