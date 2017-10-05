//
//  Ngon.h
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

#pragma once

#include "ofMain.h"
#include "BaseShape.h"

class Ngon : public BaseShape{
public:
    Ngon();
    
    void draw();
    void updateShape();
    void toggleVisible(bool val);
    void setWidth(float width);
    void setHeight(float height);
    void setSize(int size);
    
    void setNumSides(int numSides);
    void setOctave(int octave);
    void setRotateAmount(float rotationAmount);
    
    void updateNoise();
    void setNoiseStep(float noiseStep);
    void setNoiseAmount(float noiseAmount);
    void setNoiseSpeed(float noiseSpeed);
    
    ofPolyline getShape();
private:
    int numSides;
    int octave;
    bool isVisible;
    
    /// Noise
    vector<float> signedNoiseData;
    float noiseStep;
    float noiseAmount;
    float noiseSpeed;
};