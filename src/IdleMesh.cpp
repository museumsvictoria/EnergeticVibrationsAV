//
//  IdleMesh.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 9/2/18.
//
//

#include "IdleMesh.h"


//------------------------------------------
void IdleMesh::setup(ofxAssimpModelLoader &_model){

    model = _model;

    for(int i = 0; i < model.getMeshCount(); i++){
        //ofVec3f c = model.getMesh(i).getCentroid();
        
        ofMesh m = model.getMesh(i);
        vector<glm::vec3> vertices = m.getVertices();
        for(int x = 0; x < vertices.size(); x++){
            vertices[x].z -= 0.0;
            for(int j = 0; j < 3; j++ ) {
                ofVec2f coord = m.getTexCoord(j);
                m.setVertex(j,vertices[x]);
            }
        }
        meshes.push_back(m);
    }
    
    isShaderDirty = true; // initialise dirty shader
}

//------------------------------------------
void IdleMesh::update(){
    
}

//------------------------------------------
void IdleMesh::draw(ofTexture &texture, vector<int> chair_states){
    
    if (isShaderDirty){
        // dirty shader pattern:
        shared_ptr<ofxUboShader> tmpShader = shared_ptr<ofxUboShader>(new ofxUboShader);
        
        tmpShader->setGeometryInputType(GL_TRIANGLES);
        tmpShader->setGeometryOutputType(GL_TRIANGLE_STRIP);
        tmpShader->setGeometryOutputCount(4);
        
        if (tmpShader->load("shaders/idle_mesh.vert", "shaders/idle_mesh.frag")){
            mShd1 = tmpShader;
            ofLogNotice() << ofGetTimestampString() << " reloaded Shader.";
        } else {
            ofLogError() << "Shader did not load successfully.";
        }
        
        isShaderDirty = false;
    }
    
    texture.bind();
    
    if (mShd1) {
        
        mShd1->begin();
        mShd1->setUniform1i("is_active", 0);
        mShd1->setUniform1f("tick_position", (int)(ofGetElapsedTimef() * 4.4) % NUM_INSTANCES);
        mShd1->setUniform1f("time", ofGetElapsedTimef());
        mShd1->setUniform1f("alpha", abs(sin(ofGetElapsedTimef())*255));
        
        //---- Geometry shader pass
        //mShd1->setUniform1i("active_idx", primitives.get_index());
        
        //---- Fragment shader pass
        mShd1->setUniformTexture("tex_unit_1", texture, 1); // first texture unit has index 0, name is not that important!
        mShd1->setUniform1i("is_active", 1);
        
        /*
        mShd1->setUniform1i("fill_lfo_type", xray.lfo_type1);
        mShd1->setUniform1i("wireframe_lfo_type", xray.lfo_type2);
        mShd1->setUniform1f("xray_lfo_offset", xray.lfo_offset);
        mShd1->setUniform1f("xray_lfo_speed", xray.lfo_speed);
        mShd1->setUniform1f("xray_lfo_amp", xray.lfo_amp);
        mShd1->setUniform1f("xray_mix", xray.mix);
         */
        
        // draw our 3d model mesh as a wireframe into the shader
        ofSetColor(255);
        ofPushMatrix();
        ofScale(glm::vec3(0.12));
        for(int i = 0; i < meshes.size(); i++){
            if(chair_states[i] != 1)
            meshes[i].drawWireframe();
        }
        ofPopMatrix();
        
        mShd1->end();
    }

    texture.unbind();


}
