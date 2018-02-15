// vertex shader
#version 400

//     _____    ___
//    /    /   /  /     triPlanar.vert
//   /  __/ * /  /__    (c) ponies & light, 2015. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  Created by tim on 26/03/2015.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#pragma include <of_default_uniforms.glsl>
#pragma include <of_default_vertex_in_attributes.glsl>
#pragma include <noise.glsl>

#define TWO_PI 6.2831853072
#define PI 3.14159265359
#define HALF_PI 1.57079632679

uniform mat4 viewMatrix;

out VertexAttrib {
	vec4 position;
	vec4 color;
	vec3 normal;
	vec2 texcoord;
    float height;
    float instance_ID;
} vertex;

out mat4 perInstanceModelViewMatrix;

out mat4 vertexTransformMatrix;

uniform float time;
uniform int is_active;
uniform sampler2D tex_unit_0; 		// 2d texture


mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

float remap( float value, float inMin, float inMax, float outMin, float outMax )
{
    return ( (value - inMin) / ( inMax - inMin ) * ( outMax - outMin ) ) + outMin;
}


void main()
{
    vertex.instance_ID = gl_InstanceID;
    vertex.color = vec4(1); // we have to set our own color here.
    vertex.texcoord = texcoord;
    

    // For instanced rendering, we will get `gl_InstanceID`, a unique id for each instance.
    // We use this, to manipulate each instance individually.
    
    mat4 perInstanceModelMatrix;

    vec4 translation;
    translation.x = 0.0;	// translate x
    translation.y = 0.0;//1.75 - (tile_length/2) + gl_InstanceID / tile_length * 1.5; 	// translate z
    translation.z = 0.0;// params.instance_pos[gl_InstanceID].z;//abs(sin(gl_InstanceID+time))*4.0;// z_res * distance_offset * 100.;
    translation.w =	1;	// needs to remain 1.

    
    // nice! now, let's move everything apart a little.
    //translation.xyz *= .12;
    
    // store the height as a vertex attribute.
    vertex.height = translation.z;
    
    perInstanceModelMatrix[0] = vec4(1,0,0,0);
    perInstanceModelMatrix[1] = vec4(0,1,0,0);
    perInstanceModelMatrix[2] = vec4(0,0,1,0);
    perInstanceModelMatrix[3] = translation;
    
    float scale = 1.;
    mat4 scaleMatrix;
    scaleMatrix[0] = vec4(scale,0,0,0);
    scaleMatrix[1] = vec4(0,scale,0,0); // we use translation value here
    scaleMatrix[2] = vec4(0,0,scale,0);
    scaleMatrix[3] = vec4(0,0,0,1);
    
    //perInstanceModelMatrix *= rotationMatrix(vec3(1,1,0), 1.0);
    // ---------------------------
    
    // We move the box, before we even apply all the other matrices.
    // This works, because the next line really says:
    //vertex.position = viewMatrix * modelMatrix * perInstanceModelMatrix * position;
    mat4 translatedModelView = modelViewMatrix * perInstanceModelMatrix;
    
    perInstanceModelViewMatrix = modelViewMatrix * perInstanceModelMatrix * scaleMatrix;
    
    //vertex.position = perInstanceModelViewMatrix * position;
    vertex.position =  modelViewMatrix * perInstanceModelMatrix * position;
    
    vertexTransformMatrix = perInstanceModelViewMatrix;
    
    // ---------------------------
    // # Let's calculate per-vertex normals:
    
    // Note that we disregard the scale operation, since in our case it must not 
    // contribute to the normal matrix. 
    //vertex.normal =  mat3(translatedModelView) * normal;
     vertex.normal = (transpose(inverse(modelViewMatrix * perInstanceModelMatrix)) * vec4(normal,0.0)).xyz;

    gl_Position = projectionMatrix * vertex.position;
    
    vertex.position = position;
}
