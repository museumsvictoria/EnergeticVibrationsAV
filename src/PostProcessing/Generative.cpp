//
//  Generative.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 11/3/18.
//
//

#include "Generative.h"


//--------------------------------------------------------------
void Generative::init_params(){
    for(int i = 0; i < params.size(); i++){
        *params[i] = default_values[i];
    }
}

//--------------------------------------------------------------
void Generative::set_generative_param(float *param, float min_range, float max_range){
    params.push_back(param);
    default_values.push_back(*param);
    ranges.push_back(glm::vec2(min_range, max_range));
}

//--------------------------------------------------------------
void Generative::init_generative(){
    seeds.clear();
    speeds.clear();
    dev_seeds.clear();
    dev_speeds.clear();
    
    for(int i = 0; i < params.size(); i++){
        seeds.push_back(ofRandom(100000));
        speeds.push_back(ofRandom(0.3));
        dev_seeds.push_back(ofRandom(200000));
        dev_speeds.push_back(ofRandom(0.15));
    }
}


//--------------------------------------------------------------
void Generative::update_noise_walks(){
    float t = ofGetElapsedTimef();
    for(int i = 0; i < params.size(); i++){
        float n = (ofSignedNoise((t * dev_speeds[i]) + dev_seeds[i])*0.5) + ofNoise((t * speeds[i]) + seeds[i]);
         *params[i] = ofMap(n,0.0,1.0,ranges[i].x,ranges[i].y);
    }
}

//--------------------------------------------------------------
void Generative::random_dry_wet(){
    *params[0] = ofRandom(0.5,1.0);
}

//--------------------------------------------------------------
void Generative::bypass(){
    *params[0] = 0.0;
}


//--------------------------------------------------------------
void Generative::randomise_all_params(){
    for(int i = 0; i < params.size(); i++){
        *params[i] = ofRandomuf();
    }
}
