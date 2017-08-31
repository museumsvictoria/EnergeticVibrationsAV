#pragma once

#include "ofMain.h"
#include "ofxUbo.h"
#include "OptimisedBox.h"
#include "ofxImGui.h"

const int TILE_LENGTH = 8; ///< how many boxes in each x and z direction
const int NUM_ROWS = 4;
const int NUM_INSTANCES = TILE_LENGTH * NUM_ROWS; ///< total number of boxes

struct ShaderParams {
    float time;
    float transducer_speed[NUM_INSTANCES];
    int active_chair[NUM_INSTANCES];
};

class ofApp : public ofBaseApp{

        
	public:
		void setup();
		void update();
		void draw();

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
    
        OptimisedBox optimisedBox;
        ofVboMesh mMshBox;
        ofVboMesh mMshOptimisedBox;
    
        vector<of3dPrimitive> primitives;
        ofCylinderPrimitive cylinder;
    
        ofEasyCam mCam1;
        
        shared_ptr<ofxUboShader> mShd1;
        ShaderParams params;
        bool isShaderDirty;
        ofTexture mTex1;
		
        ofxImGui::Gui gui;

};
