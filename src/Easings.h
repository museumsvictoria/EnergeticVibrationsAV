//
//  Easings.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 2/12/17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxEasing.h"

class Easings{
    
public:
    Easings();
    void set_duration(float duration);
    void set_duration(vector<float> durations);
    void trigger();
    void randomise_distance();
    vector<float> get_value();
    bool isFinished();
    int size();
    
    std::vector<ofxeasing::function> easings{
        ofxeasing::linear::easeIn,
        ofxeasing::quad::easeIn,
        ofxeasing::cubic::easeIn,
        ofxeasing::quart::easeIn,
        ofxeasing::quint::easeIn,
        ofxeasing::circ::easeIn,
        ofxeasing::sine::easeIn,
        ofxeasing::exp::easeIn,
        ofxeasing::elastic::easeOut,
        ofxeasing::bounce::easeOut,
        ofxeasing::back::easeOut,
    };
    
    std::vector<std::string> easingNames{
        "linear",
        "quad",
        "cubic",
        "quart",
        "quint",
        "circ",
        "sine",
        "exp",
        "elastic",
        "bounce",
        "back",
    };
    

    std::vector<float> positions;
    float initTime;
    
    vector<float> endPositions;
    vector<float> startPositions;
    
    vector<float> durations;
};
