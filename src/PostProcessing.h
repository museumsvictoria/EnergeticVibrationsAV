//
//  PostProcessing.h
//  vertexshaderart
//
//  Created by Joshua Batty on 11/9/17.
//
//

#pragma once 

#include "ofMain.h"
#include "ofxDOF.h"
#include "ReactionDiffusion.h"

class PostProcessing {
public:
    void init();
    void setup();
    void update();
    void dof_begin();
    void dof_end();
    void draw();
    
    ofxDOF depthOfField;
    ReactionDiffusion reaction_diffusion;
    
    float dof_blur_amount;
    float dof_focal_range;
    float dof_focal_distance;
};
