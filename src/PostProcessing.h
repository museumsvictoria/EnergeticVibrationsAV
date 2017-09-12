//
//  PostProcessing.h
//  vertexshaderart
//
//  Created by Joshua Batty on 11/9/17.
//
//

#pragma once 

#include "ofMain.h"
#include "ofxDOF.h"

class PostProcessing {
public:
    void setup();
    void update();
    void begin();
    void end();
    void draw();
    
    ofxDOF depthOfField;
};
