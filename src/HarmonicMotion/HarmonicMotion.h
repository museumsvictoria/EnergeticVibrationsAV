//
//  SimpleHarmonicMotion.h
//  Kortex_v3
//
//  Created by Joshua Batty on 11/11/2014.
//
//

#pragma once

#include "ofMain.h"
#include "Algebraic.h"

class HarmonicMotion{
    
public:
    ~HarmonicMotion();
    HarmonicMotion(){
        HarmonicMotion::init();
    }
    
    void init();
    void reInit();
    void clear();
    void setSize(int size);
    void setupPatterns(int size);
    void update();
    void calculateMotion();
    void setTheta(float _theta);
    void rotatePhase();
    void resetPhase();
    void draw(int _x, int _y, int _width, int _height);
    void setOffsetHarmonic(int offsetPattern);
    string getOffsetPatternName();
    float getOffsetValue();
    bool getDMXMode();

    
    void setLockPhase(bool val);
    
    void setShape(int _shape);
    void setRotatePatternType(int _pattern);
    void setDirection(bool _direction);
    void setMirrorMode(bool _mirrorMode);
    void setInvert(bool _invert);
    
    float getTheta();
    
    vector<float> getSignal();
    
    vector<Algebraic> osc;
    vector<float> signal;
    vector<float> phases;
    vector<vector<int> > patterns;
    int shape;
    
    float theta;             // Start angle at 0
    float dx;                // Value for incrementing X, to be calculated as a function of period and xspacing
    float current_phase;     // Store the phase of the SHM
    float current_dx;
    
    float amplitude;         // Height of wave
    float position_offset;
    float speed;
    float position;
    float offset;
    int rotate_pattern_type;
    int pattern_idx;
    int phase_offset;
    bool direction;
    bool mirrorMode;
    
    bool bInitSwitch;
    float currentOffset;
    bool bLockPhase;
    bool bInvert;
    
    
private:
    vector<string> offsetPatternNames;
    vector<float> offsetPatternValues;
    int offsetPatternIndex;
    int numOscilattors;
    float min,max;
    int xspacing; // How far apart should each horizontal location be spaced
    ofVec2f origin;          // Where does the wave's first point start
};
