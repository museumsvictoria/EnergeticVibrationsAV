//
//  PathTracer.h
//  pathTracing
//
//  Created by Joshua Batty on 18/11/2015.
//
//

#pragma once

#include "ofMain.h"

class PathTracer{
public:
    PathTracer();
    
    void traceShape(ofPolyline shape);
    void setNumPointsInPath(int num);
    int getNumPointsInPath();
    
    void setTraceSpeed(float speed);
    
    void draw();
    void drawPoints();
    void drawLines();
    void drawLaserBeam();
    
    void toggleLaserBeam(bool val);
    void togglePoints(bool val);
    void toggleLines(bool val);
    
    void toggleSequential(bool val);
    void toggleBiDirectional(bool val);
    
    vector<ofPoint> getPointsLinear();
    vector<ofPoint> getPointsBiDirectional();
    
    
    void setColour1();
    void setColour2();
    
private:
    float traceSpeed;
    int numPointsInPath;
    
    ofPoint prevP1;
    ofPoint prevP2;
    
    vector<ofPoint> pathPointsLinear;
    vector<ofPoint> pathPointsBiDirectional;
    
    vector<ofColor> c1;
    vector<ofColor> c2;
    
    bool bDrawPoints;
    bool bDrawLines;
    bool bDrawLaserBeam;
    
    bool bSequential;
    bool bBiDirectional;
};