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
#include "AlphaTrails.h"
#include "Feedback.h"
#include "ofxAssimpModelLoader.h"
#include "PingPong.h"

class PostProcessing : public PingPong{
public:
    void init();
    void setup();
    void update();
    void dof_begin();
    void dof_end();
    void draw();
    
    ofxDOF depthOfField;
    ReactionDiffusion reaction_diffusion;
    AlphaTrails alpha_trails;
    Feedback feedback;
    
    ofxAssimpModelLoader masking_model;
    ofEasyCam cam;
    
    ofShader blend_shader;
    
    //----- DOF
    float dof_blur_amount;
    float dof_focal_range;
    float dof_focal_distance;
        
    bool toggle_dof;
    bool toggle_reaction_diffusion;
    bool toggle_feedback;
    bool toggle_down_sampling;
};
