//
//  Ellipse.cpp
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

#include "Ellipse.h"

//----------------------------------------------------------------
Ellipse::Ellipse(){
    
    //--------------------- Create our Ellipse Shape
    shape.clear();
    float angleStep 	= TWO_PI/(100.0f + sin(ofGetElapsedTimef()/5.0f) * 60);
    for (int i = 0; i < 200; i++){
        float anglef = (i) * angleStep;
        float x = (width*size) * cos(anglef) + (ofGetWidth()/2);
        float y = (height*size) * sin(anglef) + (ofGetHeight()/2);
        shape.addVertex(x,y);
    }
    
    //// Original squirly Circle
    /*
     // Circle
     circle.clear();
     float angleStep 	= TWO_PI/(100.0f + sin(ofGetElapsedTimef()/5.0f) * 60);
     float radiusAdder 	= 0.5f;
     float radius 		= 0;
     for (int i = 0; i < 200; i++){
     float anglef = (i) * angleStep;
     float x = radius * cos(anglef) + (ofGetWidth()/2);
     float y = radius * sin(anglef) + (ofGetHeight()/2);
     circle.addVertex(x,y);
     radius 	+= radiusAdder;
     }
     */
}

//----------------------------------------------------------------
void Ellipse::setWidth(float width){
    this->width = width;
    updateShape();
}

//----------------------------------------------------------------
void Ellipse::setHeight(float height){
    this->height = height;
    updateShape();
}

//----------------------------------------------------------------
void Ellipse::setSize(int size){
    this->size = size;
    updateShape();
}

//----------------------------------------------------------------
void Ellipse::updateShape(){
    shape.clear();
    float angleStep 	= TWO_PI/(100.0f + sin(ofGetElapsedTimef()/5.0f) * 60);
    for (int i = 0; i < 200; i++){
        float anglef = (i) * angleStep;
        float x = (width*size) * cos(anglef) + (ofGetWidth()/2);
        float y = (height*size) * sin(anglef) + (ofGetHeight()/2);
        shape.addVertex(x,y);
    }
}

//----------------------------------------------------------------
void Ellipse::draw(){
    ofSetColor(255);
    shape.draw();
}