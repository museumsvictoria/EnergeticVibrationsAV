#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxUbo.h"
#include "ofxImGui.h"
#include "GuiTheme.h"
#include "PostProcessing.h"
#include "Primitives.h"
#include "MaterialTextures.h"
#include "Easings.h"
#include "TranslationPaths.h"
#include "ReceiverOSC.h"


struct ShaderParams {
    glm::vec3 instance_model_grid[NUM_INSTANCES];
    int active_chair[NUM_INSTANCES];
    float object_size[NUM_INSTANCES];
    float vibration_hz[NUM_INSTANCES];
};


struct LfoControl {
    int lfo_type1;
    int lfo_type2;
    float lfo_offset;
    float lfo_speed;
    float lfo_amp;
    float mix;
};

class ofApp : public ofBaseApp{
public:
    void init();
    void setup();
    void update();
    void draw();
    void keyReleased(int key);
    
    void randomise_params_active();
    void randomise_params_idle();
    
    void randomise_lfos_active();
    void randomise_lfos_idle();

    // param
    vector<float> seeds;
    vector<float> speeds;
    
    // deviation
    vector<float> dev_seeds;
    vector<float> dev_speeds;
    
    ///------------- GUI
    ofxImGui::Gui gui;
    GuiTheme gui_theme;
    void setupGui();
    void drawGui(ofEventArgs & args);

    TranslationPaths paths;
    PostProcessing post;
    ReceiverOSC osc;
    MaterialTextures textures;
    Primitives primitives;

    ofEasyCam mCam1;
    Easings cam_tweens;
    vector<int> cam_tween_types = {0,0,0};

    shared_ptr<ofxUboShader> mShd1;
    shared_ptr<ofxUboShader> mShd2;
    ShaderParams params;
    bool isShaderDirty;

    // Camera
    bool toggle_camera_automation;
    float cam_near_clip;
    float cam_far_clip;

    // OPEN GL STATE
    bool toggle_post_processing;
    bool toggle_blending;
    bool toggle_backface_cull;
    
    ///------------------ACTIVE
    // GEOMETERY SHADER
    float active_geom_max_height;
    int active_geom_num_copies;
    LfoControl active_geom;
    LfoControl active_geom_effect;
    
    // FRAGMENT SHADER
    LfoControl active_xray;
    
    ///------------------IDLE
    // GEOMETERY SHADER
    float idle_geom_max_height;
    int idle_geom_num_copies;
    LfoControl idle_geom;
    LfoControl idle_geom_effect;
    
    // FRAGMENT SHADER
    LfoControl idle_xray;


};
