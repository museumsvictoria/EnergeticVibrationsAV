#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "ofxUbo.h"
#include "ofxImGui.h"
#include "GuiTheme.h"
#include "PostProcessing.h"
#include "Primitives.h"
#include "IdleMesh.h"
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
    
        TranslationPaths paths;

        MaterialTextures textures;
        Primitives primitives;
    
        ofEasyCam mCam1;
        Easings cam_tweens;
        vector<int> cam_tween_types = {0,0,0};
    
        shared_ptr<ofxUboShader> mShd1;
        ShaderParams params;
        bool isShaderDirty;

        IdleMesh idle_mesh;
    
        PostProcessing post;
        
        ReceiverOSC osc;
    
        ///------------- GUI
        ofxImGui::Gui gui;
        GuiTheme gui_theme;
    
    
        // Shader Params
        float explode_amount;
    
        // Camera
        bool toggle_camera_automation;
        float cam_near_clip;
        float cam_far_clip;
    
        // OPEN GL STATE
    bool toggle_post_processing;
    bool toggle_blending;
    bool toggle_backface_cull;
    
    // GEOMETERY SHADER
    float geom_max_height;
    int geom_num_copies;
    LfoControl geom;

    LfoControl geom_effect;

    
    // FRAGMENT SHADER
    LfoControl xray;


};
