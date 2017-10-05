//
//  PathTracer.cpp
//  pathTracing
//
//  Created by Joshua Batty on 18/11/2015.
//
//

#include "PathTracer.h"

//--------------------------------------------------------
PathTracer::PathTracer(){
    numPointsInPath = 17;
    bDrawPoints = false;
    bDrawLines = false;
    bDrawLaserBeam = false;
    
    bSequential = true;
    bBiDirectional = false;
    
    setColour1();
    setColour2();
    
    pathPointsLinear.reserve(50);
    pathPointsBiDirectional.reserve(50);
}

//--------------------------------------------------------
void PathTracer::setNumPointsInPath(int num){
    numPointsInPath = num;
    setColour1();
    setColour2();
}

//--------------------------------------------------------
void PathTracer::setTraceSpeed(float speed){
    traceSpeed = speed*0.90;
}

//--------------------------------------------------------
void PathTracer::traceShape(ofPolyline shape){

    /// Retrieve Point at percentage of shape
    float speed1= ofGetElapsedTimef()*traceSpeed;
    float speed2= ofGetElapsedTimef()*(traceSpeed*0.5);
    float counter1 = fmod(speed1, 1.0);
    float counter2 = fmod(speed2, 1.0);
    float half;
    
    pathPointsLinear.clear();
    pathPointsBiDirectional.clear();

    for(int j = 0; j < numPointsInPath; j++){
        half = (j%numPointsInPath)*1.0/(numPointsInPath/2);
        
        if(j==0){
            prevP1 = shape.getPointAtPercent(fmod(counter1+half,1.0));
            if(ofGetFrameNum()%2 == 0) {
                prevP2 = shape.getPointAtPercent(fmod(counter2+half,1.0));
            } else {
                prevP2 = shape.getPointAtPercent(fmod(1.0-counter2+half,1.0));
            }
        }
        
        pathPointsLinear.push_back(shape.getPointAtPercent(fmod(counter1+half,1.0)));
        
        if(ofGetFrameNum()%2 == 0) {
            pathPointsBiDirectional.push_back(shape.getPointAtPercent(fmod(counter2+half,1.0)));
        } else {
            pathPointsBiDirectional.push_back(shape.getPointAtPercent(fmod(1.0-counter2+half,1.0)));
        }
    }
    
}

//--------------------------------------------------------
void PathTracer::setColour1(){
    c1.clear();
    for(int i = 0; i < numPointsInPath; i++){
        ofColor c;
        c.setHsb(ofMap(i,0,numPointsInPath,0,255), 255, 255);
        c1.push_back(c);
    }
}

//--------------------------------------------------------
void PathTracer::setColour2(){
    c2.clear();
    for(int i = 0; i < numPointsInPath; i++){
        ofColor c;
        c.setHsb(ofMap(i,0,numPointsInPath,0,255), 255, 255);
        c2.push_back(c);
    }
}

//--------------------------------------------------------
int PathTracer::getNumPointsInPath(){
    return numPointsInPath;
}

//--------------------------------------------------------
void PathTracer::togglePoints(bool val){
    bDrawPoints = val;
}
void PathTracer::toggleLines(bool val){
    bDrawLines = val;
}
void PathTracer::toggleLaserBeam(bool val){
    bDrawLaserBeam = val;
}

//--------------------------------------------------------
void PathTracer::toggleSequential(bool val){
    bSequential = val;
}
void PathTracer::toggleBiDirectional(bool val){
    bBiDirectional = val;
}

//--------------------------------------------------------
void PathTracer::draw(){
    
    if(bDrawPoints){
        drawPoints();
    }
    if(bDrawLines){
        drawLines();
    }
    if(bDrawLaserBeam){
        drawLaserBeam();
    }
}

//--------------------------------------------------------
void PathTracer::drawLines(){
    for(int i = 0; i < numPointsInPath; i++){
        
        if(bSequential){
            ofSetColor(c1[i]);
            ofDrawLine(prevP1.x, prevP1.y, pathPointsLinear[i].x, pathPointsLinear[i].y);
            prevP1 = pathPointsLinear[i];
        }
        
        if(bBiDirectional){
            ofSetColor(c2[i]);
            ofDrawLine(prevP2.x, prevP2.y, pathPointsBiDirectional[i].x, pathPointsBiDirectional[i].y);
            prevP2 = pathPointsBiDirectional[i];
        }
    }
}

//--------------------------------------------------------
void PathTracer::drawPoints(){
    for(int i = 0; i < numPointsInPath; i++){

        if(bSequential){
            ofSetColor(255,255,0,255);
            ofDrawCircle(pathPointsLinear[i].x, pathPointsLinear[i].y, 4);
        }
        if(bBiDirectional){
            ofSetColor(0,205,255,255);
            ofDrawCircle(pathPointsBiDirectional[i].x, pathPointsBiDirectional[i].y, 4);
        }
    }
}

//--------------------------------------------------------
void PathTracer::drawLaserBeam(){
    for(int i = 0; i < numPointsInPath; i++){
        
        if(bSequential){
            ofSetColor(c1[i]);
            ofDrawLine(ofGetWidth()/2, ofGetHeight()/2, pathPointsLinear[i].x, pathPointsLinear[i].y);
        }
        
        if(bBiDirectional){
            ofSetColor(c2[i]);
            ofDrawLine(ofGetWidth()/2, ofGetHeight()/2, pathPointsBiDirectional[i].x, pathPointsBiDirectional[i].y);
        }
    }
}

//--------------------------------------------------------
vector<ofPoint> PathTracer::getPointsLinear(){
    return pathPointsLinear;
}
//--------------------------------------------------------
vector<ofPoint> PathTracer::getPointsBiDirectional(){
    return pathPointsBiDirectional;
}