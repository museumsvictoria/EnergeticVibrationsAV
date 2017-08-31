//
//  OptimisedBox.h
//  vertexshaderart
//
//  Created by Joshua Batty on 24/8/17.
//
//

#pragma once
#include "ofMain.h"

class OptimisedBox {
public:
    
    ofMesh getOptimisedBox() {
        // use optimised flat shaded box
        
        glm::vec3 vertices[] = {
            
            ofVec3f(-1, -1,  1) * 0.5,                // front square vertices
            ofVec3f( 1, -1,  1) * 0.5,
            ofVec3f( 1,  1,  1) * 0.5,
            ofVec3f(-1,  1,  1) * 0.5,
            
            ofVec3f(-1, -1, -1) * 0.5,                // back square vertices
            ofVec3f( 1, -1, -1) * 0.5,
            ofVec3f( 1,  1, -1) * 0.5,
            ofVec3f(-1,  1, -1) * 0.5,
        };
        
        ofIndexType indices[] = {
            // -- winding is counter-clockwise (facing camera)
            0,1,2,                // pos z
            0,2,3,
            
            1,5,6,                // pos x
            1,6,2,
            
            2,6,7,                // pos y
            2,7,3,
            
            // -- winding is clockwise (facing away from camera)
            3,4,0,                // neg x
            3,7,4,
            
            4,5,1,                // neg y
            4,1,0,
            
            5,7,6,                // neg z
            5,4,7,
        };
        
        glm::vec3 normals[] = {
            ofVec3f( 0,  0,  1),
            ofVec3f( 1,  0,  0),
            ofVec3f( 0,  1,  0),
            ofVec3f(-1,  0,  0),
            ofVec3f( 0, -1,  0),
            ofVec3f( 0,  0, -1),
            ofVec3f( 1,  0,  0), // can be anything, will not be used
            ofVec3f( 1,  0,  0), //  -- " --
        };
        
        ofMesh optimisedBox;
        optimisedBox.clear();
        optimisedBox.addVertices(vertices, 8);
        optimisedBox.addNormals(normals, 8);
        optimisedBox.addIndices(indices, 6 * 3 * 2);
        return optimisedBox;
    };

};
