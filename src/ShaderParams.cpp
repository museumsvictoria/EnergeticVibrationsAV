//
//  ShaderParams.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 18/12/17.
//
//

#include "ShaderParams.h"

//--------------------------------------------------------------
void ShaderParams::init(){
    
    for(int i = 0; i < NUM_INSTANCES; i++){
        //Vertex shader
        vertex_params.instance_model_grid[i] = glm::vec3(0.0,0.0,0.0);
        
        //Geometry shader
        geom_max_height[i] = 0.0;
        geom_num_copies[i] = 1;
        
        geom.lfo_type1[i] = (int)ofRandom(33);
        geom.lfo_offset[i] = 0.58;
        geom.lfo_speed[i] = 0.02;
        geom.lfo_amp[i] = 1.0;
        
        geom_effect.lfo_type1[i] = (int)ofRandom(33);
        geom_effect.mix[i] = 0.0;
        geom_effect.lfo_offset[i] = 0.2;
        geom_effect.lfo_speed[i] = 0.1;
        geom_effect.lfo_amp[i] = 1.0;
        
        //Fragment Shader
        xray.lfo_type1[i] = (int)ofRandom(33);
        xray.lfo_type2[i] = (int)ofRandom(33);
        xray.mix[i] = 1.0;
        xray.lfo_offset[i] = 0.0;
        xray.lfo_speed[i] = 0.5;
        xray.lfo_amp[i] = 1.0;
    }
}
