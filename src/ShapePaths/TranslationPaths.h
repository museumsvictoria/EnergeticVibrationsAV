//
//  TranslationPaths.hpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 6/10/17.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Wave.h"
#include "Ngon.h"
#include "Grid.h"

class TranslationPaths{
public:
    TranslationPaths(){
        TranslationPaths::init();
    }
    
    void init();
    void update();
    void set_grid_path(glm::vec2 instance_pos_grid[]);
    void set_ngon_path(glm::vec2 instance_ngon_grid[]);
    void set_wave_path(glm::vec2 instance_wave_grid[]);
    
    Grid grid;
    Ngon ngon;
    Wave wave;

    
private:
    
    
};
