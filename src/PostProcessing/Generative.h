//
//  Generative.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 11/3/18.
//
//

#pragma once
#include "ofMain.h"

class Generative{
    
public:
    //--------------------------------------------------------------
    void init_params();
    void set_generative_param(float *param, float min_range, float max_range);
    void init_generative();
    void update_noise_walks();
    void random_dry_wet();
    void bypass();
    void randomise_all_params();
    //--------------------------------------------------------------
    
private:
    vector<float*> params;
    vector<float> default_values;
    vector<glm::vec2> ranges;
protected:
    
    // param
    vector<float> seeds;
    vector<float> speeds;
    
    // deviation
    vector<float> dev_seeds;
    vector<float> dev_speeds;
};
