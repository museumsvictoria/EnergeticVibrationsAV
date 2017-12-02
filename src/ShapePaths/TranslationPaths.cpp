//
//  TranslationPaths.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 6/10/17.
//
//

#include "TranslationPaths.h"

//--------------------------------------------------------------
void TranslationPaths::init(){

}

//--------------------------------------------------------------
void TranslationPaths::set_grid_path(glm::vec2 instance_pos_grid[]){

    for(int i = 0; i < grid.get_path().size(); i++){
        instance_pos_grid[i].x = grid.get_path()[i].x;
        instance_pos_grid[i].y = grid.get_path()[i].y;
    }
}

//--------------------------------------------------------------
void TranslationPaths::set_ngon_path(glm::vec2 instance_ngon_grid[]){
    for(int i = 0; i < ngon.get_path().size(); i++){
        instance_ngon_grid[i].x = ngon.get_path()[i].x;
        instance_ngon_grid[i].y = ngon.get_path()[i].y;
    }
}

//--------------------------------------------------------------
void TranslationPaths::set_wave_path(glm::vec2 instance_wave_grid[]){
    for(int i = 0; i < wave.get_path().size(); i++){
        instance_wave_grid[i].x = wave.get_path()[i].x;
        instance_wave_grid[i].y = wave.get_path()[i].y;
    }
}

//--------------------------------------------------------------
void TranslationPaths::update(){
    wave.updateShape();
}
