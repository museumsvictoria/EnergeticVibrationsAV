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
#include "ofxCameraMove.h"
#include "ofxJson.h"

struct ShaderParams {
    glm::vec3 instance_model_grid[NUM_INSTANCES];
    int active_chair[NUM_INSTANCES];
    float object_size[NUM_INSTANCES];
    float vibration_hz[NUM_INSTANCES];
    float seat_kickers[NUM_INSTANCES];
};


struct LfoControl {
    int lfo_type1;
    int lfo_type2;
    float lfo_offset;
    float lfo_speed;
    float lfo_amp;
    float mix;
};


enum EffectModes{
    Bypass,
    Atari,
    Trails,
    Feedback,
    Reaction_Diffusion,
    Perlin_Combo,
    Random_Combo
};



class ofApp : public ofBaseApp{
public:
    void init();
    void setup();
    void update();
    void draw();
    void keyReleased(int key);

    void on_combo_triggered(bool & e);
    void on_seat_triggered(bool & e);

    void randomise_params_active();
    void randomise_params_idle();
    
    void randomise_lfos_active();
    void randomise_lfos_idle();
    
    //Preset Saving and Loading
    void save_idle_preset();
    void save_active_preset();
    void load_idle_preset(int num);
    void load_active_preset(int num);
    
    int num_active_presets;
    int num_idle_presets;
    
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

    ofxCameraMove moveCam;
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

    ///------------------GENERATIVE MODES
    void update_generative_modes();
    bool seat_triggered;
    bool combo_triggered;
    int seconds_since_last_generative_trigger;
    int init_last_gen_time;

};
