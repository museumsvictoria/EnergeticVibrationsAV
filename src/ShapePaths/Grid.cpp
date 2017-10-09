//
//  Grid.cpp
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

#include "Grid.h"

//----------------------------------------------------------------
Grid::Grid(){
    
    //--------------------- Create our Square Shape
    shape.clear();
    for (int i = 0; i < NUM_INSTANCES; i++) {
//        shape.addVertex((0.5 - (TILE_LENGTH/2) + i % TILE_LENGTH),
//                        (1.75 - (TILE_LENGTH/2) + i / TILE_LENGTH * 1.5));
        
        float x = (0.5 - (TILE_LENGTH/2) + i % TILE_LENGTH) / (TILE_LENGTH/4);
        float y = (1.0 - (TILE_LENGTH/2) + i / TILE_LENGTH * 2.0) / (TILE_LENGTH/2);
        shape.addVertex(x,y);
        
        cout << "x " + ofToString(i+1) + " = "  << shape.getVertices()[i].x <<
        " --- y  = "  << shape.getVertices()[i].y << endl;
    }
    shape.close();
}

//----------------------------------------------------------------
void Grid::setWidth(float width){
    this->width = width;
    updateShape();
}

//----------------------------------------------------------------
void Grid::setHeight(float height){
    this->height = height;
    updateShape();
}

//----------------------------------------------------------------
void Grid::setSize(int size){
    this->size = size;
    updateShape();
}

//----------------------------------------------------------------
void Grid::setRotateAmount(float rotationAmount){
    this->rotationAmount = rotationAmount;
    updateShape();
}

//----------------------------------------------------------------
void Grid::updateShape(){
    shape.clear();
    shape.addVertex(center+glm::vec2(-width*size, -height*size));
    shape.addVertex(center+glm::vec2(width*size, -height*size));
    shape.addVertex(center+glm::vec2(width*size, height*size));
    shape.addVertex(center+glm::vec2(-width*size, height*size));
    shape.close();
}

//----------------------------------------------------------------
void Grid::draw(){
    ofSetColor(255);
    shape.draw();
}

