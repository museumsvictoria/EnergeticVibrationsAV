//
//  Easings.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 2/12/17.
//
//

#include "Easings.h"

//--------------------------------------------------------------
Easings::Easings(){
    positions.resize(easings.size());
    durations.resize(easings.size());
    endPositions.resize(easings.size());
    startPositions.resize(easings.size());
    
    initTime = 0;

    
    for(size_t i= 0;i<easings.size();i++){
        durations[i] = 8.0;
        endPositions[i] = 1.0;
        startPositions[i] = 0.0;
    }
}

//--------------------------------------------------------------
vector<float> Easings::get_value(){
    auto now = ofGetElapsedTimef();
    auto endTime = initTime + durations[0];

    for(size_t i=0;i<easings.size();i++){
        positions[i] = ofxeasing::map_clamp(now, initTime, endTime, startPositions[i], endPositions[i], easings[i]);
    }
    
    return positions;
}

bool Easings::isFinished(){
    if(ofGetElapsedTimef() >= (initTime + durations[0])){
        for(size_t i=0;i<easings.size();i++){
            startPositions[i] = endPositions[i];
        }
        return true;
    } else {
        return false;
    }
}

//--------------------------------------------------------------
void Easings::set_duration(float duration){
    for(auto &d:  durations){
        d = duration;
    }
}
void Easings::set_duration(vector<float> durations){
    this->durations = durations;
}


//--------------------------------------------------------------
void Easings::trigger(){
    initTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void Easings::randomise_distance(){
    for(auto &end: endPositions){
        end = ofRandom(0.0,1.0);
    }
}

//--------------------------------------------------------------
int Easings::size(){
    return easings.size();
}
