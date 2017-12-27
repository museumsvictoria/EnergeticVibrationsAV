//
//  ShaderParams.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 18/12/17.
//
//

#pragma once 

#include "ofMain.h"
#include "Constants.h"

//struct ShaderParams {
//    //float scale_speed;
//    //float rot_speed;
//    //float transducer_speed[NUM_INSTANCES];
//    //int active_chair[NUM_INSTANCES];
//    //glm::vec2 instance_model_grid[NUM_INSTANCES];
//};


struct GeomLfoControl {
    int lfo_type1[NUM_INSTANCES];
    int lfo_type2[NUM_INSTANCES];
    float lfo_offset[NUM_INSTANCES];
    float lfo_speed[NUM_INSTANCES];
    float lfo_amp[NUM_INSTANCES];
    float mix[NUM_INSTANCES];
};
struct GeomEffectLfoControl {
    int lfo_type1[NUM_INSTANCES];
    int lfo_type2[NUM_INSTANCES];
    float lfo_offset[NUM_INSTANCES];
    float lfo_speed[NUM_INSTANCES];
    float lfo_amp[NUM_INSTANCES];
    float mix[NUM_INSTANCES];
};
struct XrayLfoControl {
    int lfo_type1[NUM_INSTANCES];
    int lfo_type2[NUM_INSTANCES];
    float lfo_offset[NUM_INSTANCES];
    float lfo_speed[NUM_INSTANCES];
    float lfo_amp[NUM_INSTANCES];
    float mix[NUM_INSTANCES];
};


struct VertexParams {
    glm::vec3 instance_model_grid[NUM_INSTANCES];
};

class ShaderParams {
public:
    void init();
    
    // VERTEX SHADER
    VertexParams vertex_params;
    
    // GEOMETERY SHADER
    float geom_max_height[NUM_INSTANCES];
    int geom_num_copies[NUM_INSTANCES];
    GeomLfoControl geom;
    GeomEffectLfoControl geom_effect;
    
    // FRAGMENT SHADER
    XrayLfoControl xray;
};

