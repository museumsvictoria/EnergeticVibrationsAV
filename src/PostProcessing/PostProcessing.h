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
#include "Downsample.h"
#include "ReactionDiffusion.h"
#include "AlphaTrails.h"
#include "Feedback.h"
#include "SurfaceMask.h"
#include "PingPong.h"

class PostProcessing : public PingPong{
public:
    void init_params();
    void setup();
    void update();
    void begin();
    void end();
    void draw();
    
    ofxDOF depthOfField;
    Downsample downsample;
    ReactionDiffusion reaction_diffusion;
    AlphaTrails alpha_trails;
    Feedback feedback;
    SurfaceMask surface_mask;
    
    //----- DOF
    float dof_blur_amount;
    float dof_focal_range;
    float dof_focal_distance;
        
    bool toggle_dof;
    bool toggle_reaction_diffusion;
    bool toggle_feedback;
    bool toggle_down_sampling;
    
    //----- GENERATIVE MODES
    void trigger_bypass_mode();
    void trigger_atari_mode();
    void trigger_trails_mode();
    void trigger_feedback_mode();
    void trigger_reaction_diffusion_mode();
    void trigger_random_combo_mode();
    void run_combo_perlin_mode();
    
};
