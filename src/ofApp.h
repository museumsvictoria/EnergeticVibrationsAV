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



struct ShaderParams {
    float scale_speed;
    float rot_speed;
    float transducer_speed[NUM_INSTANCES];
    int active_chair[NUM_INSTANCES];
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
    
        void setupGui();
        void drawGui(ofEventArgs & args);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        MaterialTextures textures;
        Primitives primitives;
    
        ofEasyCam mCam1;
        Easings cam_tweens;
        vector<int> cam_tween_types = {0,0,0};
    
        shared_ptr<ofxUboShader> mShd1;
        ShaderParams params;
        bool isShaderDirty;

        PostProcessing post;
    
        ///------------- GUI
        ofxImGui::Gui gui;
        GuiTheme gui_theme;
    
    
        // Shader Params
        float explode_amount;
    
        // Camera
        float cam_near_clip;
        float cam_far_clip;
    
        // OPEN GL STATE
    bool toggle_post_processing;
    bool toggle_blending;
    bool toggle_backface_cull;
    
    // GEOMETERY SHADER
    LfoControl geom;

    LfoControl geom_effect;
//    int geom_effect_lfo_type;
//    float geom_effect_lfo_offset;
//    float geom_effect_lfo_speed;
//    float geom_effect_lfo_amp;
//    float geom_effect_mix;
    
    // FRAGMENT SHADER
    LfoControl xray;
//    int xray_lfo_type;
//    float xray_lfo_offset;
//    float xray_lfo_speed;
//    float xray_lfo_amp;
//    float xray_mix;

};
