//
//  SimpleHarmonicMotion.cpp
//  Kortex_v3
//
//  Created by Joshua Batty on 11/11/2014.
//
//

#include "HarmonicMotion.h"

/*
 Harmonic Relationships::
 2.000 = lock
 1.945 = single lock quarter chase
 1.897 = single lock half chase
 1.790 = double lock complete chase
 1.600 = 1/6th lock
 1.330 = sextuplet
 0.660 = tripple
 0.440 = double
 0.220 = single complete chase
 0.110 = single half chase
 0.055 = single quarter chase
 0.000 = uniform
 */

//--------------------------------------------------------------
//--------------------------------------------------------------
HarmonicMotion::~HarmonicMotion(){
    
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::init(){
    position = 0.0;
    amplitude = 0.0;
    position_offset = 0.0;
    speed = 0.0;
    phase_offset = 0;
    rotate_pattern_type = 1;
    pattern_idx = 0;
    offset = 0.101;
    currentOffset = 0.0;
    
    xspacing = 18;
    theta = 1.0;
    
    numOscilattors = 32;
    shape = 0;
    dx = 0.110;//TWO_PI * xspacing;
    direction = true;
    mirrorMode = false;
    
    bInvert = false;
    bLockPhase = false;
    bInitSwitch = true;
    
    offsetPatternIndex = 0;
    offsetPatternNames.push_back("Uniform");
    offsetPatternNames.push_back("Single Quater");
    offsetPatternNames.push_back("Single Half");
    offsetPatternNames.push_back("Single Square");
    offsetPatternNames.push_back("Single Complete");
    offsetPatternNames.push_back("Double");
    offsetPatternNames.push_back("Tripple");
    offsetPatternNames.push_back("Sextuplet");
    offsetPatternNames.push_back("1/6th Lock");
    offsetPatternNames.push_back("Double Complete Lock");
    offsetPatternNames.push_back("Single Half Lock");
    offsetPatternNames.push_back("Single Quater Lock");
    offsetPatternNames.push_back("Lock");
    
    offsetPatternValues.push_back(0.000);
    offsetPatternValues.push_back(0.055);
    offsetPatternValues.push_back(0.110);
    offsetPatternValues.push_back(0.130);
    offsetPatternValues.push_back(0.220);
    offsetPatternValues.push_back(0.440);
    offsetPatternValues.push_back(0.660);
    offsetPatternValues.push_back(1.330);
    offsetPatternValues.push_back(1.600);
    offsetPatternValues.push_back(1.790);
    offsetPatternValues.push_back(1.897);
    offsetPatternValues.push_back(1.945);
    offsetPatternValues.push_back(2.000);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::reInit(){
    if(bInitSwitch == true){
        offset = currentOffset;
    }
    bInitSwitch = false;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::clear(){
    signal.clear();
    phases.clear();
    osc.clear();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setSize(int size){
    // Clear our vectors before we push back new ones
    clear();
    
    numOscilattors = size;
    
    for(int i=0; i < numOscilattors; i++){
        signal.push_back(0);
        phases.push_back(0);
        Algebraic alg;
        osc.push_back(alg);
        osc[i].setDMXMode(true);
    }
    
    setupPatterns(numOscilattors);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setupPatterns(int size){
    int numPatterns = 6;
    
    patterns.clear();
    
    for(int i = 0; i < numPatterns; i++){
        vector<int> pattern;
        patterns.push_back(pattern);
    }
    
    for(int  i = 0; i < patterns.size(); i++){
        for(int j = 0; j < size; j++){
            
            // Pattern 1 Linear
            if(i == 0) {
                patterns[i].push_back(j);
            }
            
            // Pattern 2 = Step Forward / Step Back Mode
            else if(i == 1){
                if(j % 2 == 0){
                    patterns[1].push_back((size/2)+((j+1)/2));
                } else if(j % 2 == 1){
                    patterns[1].push_back((size/2)-((j+1)/2));
                }
            }
            
            // Pattern 3
            else {
                patterns[i].push_back(ofWrap((j)*i, 0, size));
            }
        }
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setRotatePatternType(int _pattern){
    rotate_pattern_type = _pattern;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setLockPhase(bool val){
    bLockPhase = val;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setDirection(bool _direction){
    direction = _direction;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setMirrorMode(bool _mirrorMode){
    mirrorMode = _mirrorMode;
}
//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setInvert(bool _invert){
    bInvert = _invert;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
vector<float> HarmonicMotion::getSignal(){
    float low;
    float high;
    
    if(bInvert == false){
        low = -1.0;
        high = 1.0;
    } else if(bInvert == true){
        low = 1.0;
        high = -1.0;
    }
    for(int i = 0; i < numOscilattors; i++){
        switch (shape) {
            case 0:
                signal[i] = ofMap(osc[i].getSine(),-1.0,1.0,low,high);
                break;
            case 1:
                signal[i] = ofMap(osc[i].getTriangle(),-1.0,1.0,low,high);
                break;
            case 2:
                signal[i] = ofMap(osc[i].getRamp(),-1.0,1.0,low,high);
                break;
            case 3:
                signal[i] = ofMap(osc[i].getSaw(),-1.0,1.0,low,high);
                break;
            case 4:
                signal[i] = ofMap(osc[i].getSquare(),-1.0,1.0,low,high);
                break;
            case 5:
                signal[i] = ofMap(osc[i].getRandom(),-1.0,1.0,low,high);
                break;
        }
    }
    return signal;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::update(){
    calculateMotion();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::rotatePhase(){
    if(direction == true){
        pattern_idx++;
    } else {
        pattern_idx--;
    }
    phase_offset = patterns[rotate_pattern_type][ofWrap(pattern_idx,0,numOscilattors)];
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::resetPhase(){
    pattern_idx = 0;
    phase_offset = 0;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setTheta(float _theta){
    theta = _theta;
}
float HarmonicMotion::getTheta(){
    return theta;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::calculateMotion(){
    reInit();
    
    theta += speed;
    theta = ofWrap(theta, 1.0, 5.0);
    current_phase = theta;

    float x = theta;
    for (int i = 0; i < numOscilattors; i++) {
        if(!bLockPhase){
            if(direction) {
                x+=dx;
                dx+=(offset*0.1)*0.02; //This is my Line of code
            } else {
                x-=dx;
                dx-=(offset*0.1)*0.02; //This is my Line of code
            }
        }
        current_dx = dx;
        phases[i] = x * 0.25;

        osc[i].setFrequency(phases[ofWrap(i+phase_offset,0,phases.size())]);
        osc[i].setAmplitude(amplitude);
        osc[i].setPositionOffset(position_offset);
        
        if(mirrorMode){
            if(i < (numOscilattors/2)){
                osc[i].setFrequency(phases[ofWrap(i+phase_offset,0,phases.size())]);
            } else {
                osc[i].setFrequency(osc[numOscilattors-i-1].getFrequency());
            }
        }
    }
    currentOffset = offset;
}


//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setOffsetHarmonic(int offsetPattern){
    offsetPatternIndex = offsetPattern;
    if(direction == true){
        dx = offsetPatternValues[offsetPatternIndex];
    } else {
        dx = 0.0-offsetPatternValues[offsetPatternIndex];
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
string HarmonicMotion::getOffsetPatternName(){
    return offsetPatternNames[offsetPatternIndex];
}

//--------------------------------------------------------------
//--------------------------------------------------------------
float HarmonicMotion::getOffsetValue(){
    if(direction == true){
        return offsetPatternValues[offsetPatternIndex];
    } else {
        return -offsetPatternValues[offsetPatternIndex];
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
bool HarmonicMotion::getDMXMode(){
    return osc[0].getIsDMXMode();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::draw(int _x, int _y, int _width, int _height){
    
    xspacing = _width / numOscilattors;
    int padding = xspacing / 2;
    float radius = padding / 1.75;
    
    origin.x = padding + _x;
    origin.y = _y;
    
    for (int x = 0; x < numOscilattors; x++) {
        if(osc[x].getIsDMXMode() == true){
            ofDrawLine(origin.x+x*xspacing, origin.y+signal[x]-_height, origin.x+x*xspacing, origin.y+signal[x]*_height);
            ofDrawEllipse(origin.x+x*xspacing,origin.y+signal[x]*_height,radius,radius);
        } else {
            ofDrawLine(origin.x+x*xspacing, origin.y+signal[x], origin.x+x*xspacing, origin.y+signal[x]*_height);
            ofDrawEllipse(origin.x+x*xspacing,origin.y+signal[x]*_height,radius,radius);
        }
    }
    ofSetColor(255);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void HarmonicMotion::setShape(int _shape){
    shape = _shape;
}
