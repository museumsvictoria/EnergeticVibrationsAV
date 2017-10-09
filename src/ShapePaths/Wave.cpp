//
//  Wave.cpp
//  pathTracing
//
//  Created by Joshua Batty on 17/11/2015.
//
//

#include "Wave.h"

//----------------------------------------------------------------
Wave::Wave(){
    shm.setSize(NUM_INSTANCES);

    shm.amplitude = 1.0;
    shm.position_offset = 0.0;
    shm.speed = 0.01;
    shm.offset = .001;
    
    shm_oscillator = 0;
    shm_phaseLock = 0;
    shm_direction = 1;
    shm_invert = 0;
    shm_mirror_mode = 0;
    shm_offset_pattern = 1;
    
    setOscillator(shm_oscillator);
    setPhaseLock(shm_phaseLock);
    setDirection(shm_direction);
    setMirrorMode(shm_invert);
    setInvert(shm_mirror_mode);
    setOffsetPattern(shm_offset_pattern);
    
    updateShape();
}

//--------------------------------------------------------------
void Wave::setOscillator(int osc){
    shm_oscillator = osc;
    shm.setShape(shm_oscillator);
}
void Wave::setPhaseLock(int val){
    shm_phaseLock = val;
    shm.setLockPhase(shm_phaseLock);
}
void Wave::setDirection(int val){
    shm_direction = val;
    shm.setDirection(shm_direction);
}
void Wave::setInvert(int val){
    shm_invert = val;
    shm.setInvert(shm_invert);
}
void Wave::setMirrorMode(int val){
    shm_mirror_mode = val;
    shm.setMirrorMode(shm_mirror_mode);
}
void Wave::setOffsetPattern(int val){
    shm_offset_pattern = val;
    shm.setOffsetHarmonic(shm_offset_pattern);
}
void Wave::triggerRotatePhase(int val){
    shm.rotatePhase();
}

void Wave::setAmplitude(float val){
    shm.amplitude = val;
}
void Wave::setPositionOffset(float val){
    shm.position_offset = val;
}
void Wave::setPeriod(float val){
    shm.speed = val;
}
void Wave::setOffsetSpeed(float val){
    shm.offset = val;
    shm.currentOffset = val;
}
void Wave::set_phase(float val){
    shm.theta = val;
}
float Wave::get_current_phase(){
    return shm.current_phase;
}

void Wave::set_dx_init(float val){
    shm.dx = val;
}
float Wave::get_current_dx(){
    return shm.current_dx;
}

float Wave::get_shm_amplitude() {return shm.amplitude;}
float Wave::get_shm_position_offset() {return shm.position_offset;}
float Wave::get_shm_period() {return shm.speed;}
float Wave::get_shm_offset_speed() {return shm.offset;}
int Wave::get_shm_oscillator() {return shm_oscillator;}
int Wave::get_shm_phaseLock() {return shm_phaseLock;}
int Wave::get_shm_direction() {return shm_direction;}
int Wave::get_shm_mirror_mode() {return shm_mirror_mode;}
int Wave::get_shm_invert() {return shm_invert;}
int Wave::get_shm_offset_pattern() {return shm_offset_pattern;}

//----------------------------------------------------------------
void Wave::updateShape(){
    cout << " size = "  << endl;

    shm.update();

    shape.clear();
    for(int i = 0; i < shm.getSignal().size(); i++){
        float x = 0.50 - (TILE_LENGTH*2.) + i;
        float y = 0.5 + shm.getSignal()[i] * 6.0;
        shape.addVertex(x,y);
    }
}

//----------------------------------------------------------------
void Wave::draw(){
    ofSetColor(255);
    shape.draw();
}
