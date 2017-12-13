#version 330

// OF Attributes
in vec4  position;
in vec4  color;
in vec3  normal;
in vec2  texcoord;

// outputs
out vec2  vTexCoord;

uniform mat4 modelViewProjectionMatrix;

void main( void )
{
    // outputs
    vTexCoord = texcoord;
    
    // pass the position straight through as it's already -1.0f to 1.0f
    gl_Position = modelViewProjectionMatrix * position;
}
