//
//  Ngon.cpp
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

#include "Ngon.h"
// 0,1,2,3,4,5,6,7
// 0,2,4,6,1,3,5,7
//----------------------------------------------------------------
Ngon::Ngon(){
    numSides = 5;
    octave = 1;
    rotationAmount = 0.0;
    isVisible = false;
    
    noiseStep = 0.0;
    noiseAmount = 0.0;
    noiseSpeed = 0.0;
    
    //--------------------- Create our Square Shape
    updateNoise();
    updateShape();
}

//----------------------------------------------------------------
void Ngon::toggleVisible(bool val){
    isVisible = val;
}

//----------------------------------------------------------------
void Ngon::setWidth(float width){
    this->width = width;
    updateShape();
}

//----------------------------------------------------------------
void Ngon::setHeight(float height){
    this->height = height;
    updateShape();
}

//----------------------------------------------------------------
void Ngon::setSize(int size){
    this->size = size;
    updateShape();
}

//----------------------------------------------------------------
void Ngon::setNumSides(int numSides){
    this->numSides = numSides;
    updateShape();
}

//----------------------------------------------------------------
void Ngon::setOctave(int octave){
    this->octave = octave;
    updateShape();
}

//----------------------------------------------------------------
void Ngon::setRotateAmount(float rotationAmount){
    this->rotationAmount = rotationAmount*PI;
    updateShape();
}

//----------------------------------------------------------------
void Ngon::setNoiseStep(float noiseStep){
    this->noiseStep = noiseStep;
}

//----------------------------------------------------------------
void Ngon::setNoiseAmount(float noiseAmount){
    this->noiseAmount = noiseAmount;
}

//----------------------------------------------------------------
void Ngon::setNoiseSpeed(float noiseSpeed){
    this->noiseSpeed = noiseSpeed * 10.0;
}

//----------------------------------------------------------------
void Ngon::updateNoise(){
    signedNoiseData.clear();
    
    for(int i = 0; i < numSides; i++){
        signedNoiseData.push_back(ofNoise(i*noiseStep,ofGetElapsedTimef()*noiseSpeed)*noiseAmount);
    }
}

//----------------------------------------------------------------
void Ngon::updateShape(){
    shape.clear();

    float quadra = (2*PI) / numSides;
    for (int i = 0; i < numSides; i++) {
        float noise = 1.0-signedNoiseData[i];
        float index = quadra * (ofWrap(i*octave,0,numSides)) + rotationAmount;
        shape.addVertex(center.x+sin(index)*(width*size*noise), center.y+cos(index)*(height*size*noise));
    }
    shape.close();
}

//----------------------------------------------------------------
ofPolyline Ngon::getShape(){
    return shape;
}

//----------------------------------------------------------------
void Ngon::draw(){
    updateNoise();
    updateShape();
    
    if(isVisible){
        ofSetColor(255,125);
        shape.draw();
        
        /// Retrive each pre defined Vertex
        for(int i =0; i < shape.getVertices().size(); i++){
            ofPoint p = shape.getVertices()[i];
            ofSetColor(0,255,0);
            //ofDrawCircle(p.x, p.y, 5);
            ofDrawBitmapString(ofToString(i), p.x, p.y);
        }
    }
}