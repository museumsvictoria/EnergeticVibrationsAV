//
//  Rectangle.cpp
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

#include "Rectangle.h"

//----------------------------------------------------------------
Rectangle::Rectangle(){
    
    //--------------------- Create our Square Shape
    shape.clear();
    shape.addVertex(center+ofVec2f(-width*size, -height*size));
    shape.addVertex(center+ofVec2f(width*size, -height*size));
    shape.addVertex(center+ofVec2f(width*size, height*size));
    shape.addVertex(center+ofVec2f(-width*size, height*size));
    shape.close();
}

//----------------------------------------------------------------
void Rectangle::setWidth(float width){
    this->width = width;
    updateShape();
}

//----------------------------------------------------------------
void Rectangle::setHeight(float height){
    this->height = height;
    updateShape();
}

//----------------------------------------------------------------
void Rectangle::setSize(int size){
    this->size = size;
    updateShape();
}

//----------------------------------------------------------------
void Rectangle::setRotateAmount(float rotationAmount){
    this->rotationAmount = rotationAmount;
    updateShape();
}

//----------------------------------------------------------------
void Rectangle::updateShape(){
    shape.clear();
    shape.addVertex(center+ofVec2f(-width*size, -height*size));
    shape.addVertex(center+ofVec2f(width*size, -height*size));
    shape.addVertex(center+ofVec2f(width*size, height*size));
    shape.addVertex(center+ofVec2f(-width*size, height*size));
    shape.close();
}

//----------------------------------------------------------------
void Rectangle::draw(){
    ofSetColor(255);
    shape.draw();
}