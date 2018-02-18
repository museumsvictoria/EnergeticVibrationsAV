//
//  PingPong.cpp
//  EnergeticVibrationsAV
//
//  Created by Joshua Batty on 4/10/17.
//
//

#include "PingPong.h"

//--------------------------------------------------------------
void PingPong::createFullScreenQuad(){
    // -1.0 to +1.0 is the full viewport (screen) if you use these as vertices in your vertex shader
    // (without multiplying by model, view, and projection matrices)
    
    glm::vec3 vertices[4] =
    {
        glm::vec3(  1.0f,  1.0f, 0.0f ),
        glm::vec3( -1.0f,  1.0f, 0.0f ),
        glm::vec3(  1.0f, -1.0f, 0.0f ),
        glm::vec3( -1.0f, -1.0f, 0.0f )
    };
    
    ofIndexType indices[6] =
    {
        0, 1, 2,
        2, 1, 3
    };
    
    // Texture coordinates vary from 0.0 to 1.0 when they are in normalized format
    // ofDisableArbTex() was called earlier set that we're using normalized texture coordinates
    glm::vec2 texCoords[4] =
    {
        glm::vec2( 1.0f, 0.0f ),
        glm::vec2( 0.0f, 0.0f ),
        glm::vec2( 1.0f, 1.0f ),
        glm::vec2( 0.0f, 1.0f )
    };
    
    m_fsQuadVbo.addVertices( vertices, 4 );
    m_fsQuadVbo.addTexCoords( texCoords, 4 );
    m_fsQuadVbo.addIndices( indices, 6 );
}


//--------------------------------------------------------------
void PingPong::init_fbos(){
    
}
