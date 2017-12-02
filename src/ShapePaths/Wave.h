//
//  Wave.h
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

#pragma once

#include "ofMain.h"
#include "BaseShape.h"
#include "HarmonicMotion.h"

class Wave : public BaseShape{
public:
    Wave();
    
    void draw();
    void updateShape();

    void setOscillator(int osc);
    void setPhaseLock(int val);
    void setDirection(int val);
    void setMirrorMode(int val);
    void setInvert(int val);
    void setOffsetPattern(int val);
    void triggerRotatePhase(int val);
    
    void setAmplitude(float val);
    void setPositionOffset(float val);
    void setPeriod(float val);
    void setOffsetSpeed(float val);
    
    void set_phase(float val);
    void set_dx_init(float val);
    
    float get_current_phase();
    float get_current_dx();
    
    float get_shm_amplitude();
    float get_shm_position_offset();
    float get_shm_period();
    float get_shm_offset_speed();
    int get_shm_oscillator();
    int get_shm_phaseLock();
    int get_shm_direction();
    int get_shm_invert();
    int get_shm_mirror_mode();
    int get_shm_offset_pattern();
    
    HarmonicMotion shm;
    
private:
    int shm_oscillator;
    int shm_phaseLock;
    int shm_direction;
    int shm_mirror_mode;
    int shm_invert;
    int shm_offset_pattern;
};
