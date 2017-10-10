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

#define NUM_INSTANCES 32
#define TWO_PI 6.2831853072
#define PI 3.14159265359
#define HALF_PI 1.57079632679

#pragma include <waveforms.glsl>

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


uniform float time;
uniform int is_active;
uniform int tile_length;
uniform sampler2D tex_unit_0; 		// 2d texture


uniform ShaderParams {
    float scale_speed;
    float rot_speed;
    float transducer_speed[NUM_INSTANCES];
    int active_chair[NUM_INSTANCES];
    float shape_morph;
    float circle_motion;
    vec2 instance_pos_grid[NUM_INSTANCES];
    vec2 instance_pos_ngon[NUM_INSTANCES];
    vec2 instance_pos_wave[NUM_INSTANCES];
}params;

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

	// find out the texture dimensions in pixels, at LOD 0
	ivec2 texSize = textureSize(tex_unit_0, 0 );

	// For instanced rendering, we will get `gl_InstanceID`, a unique id for each instance.
	// We use this, to manipulate each instance individually.
	mat4 perInstanceModelMatrix;

    // Arrange the objects in a grid
    vec4 translation_wave;
    float idx = mod(gl_InstanceID, 32.);
    translation_wave.x = params.instance_pos_wave[int(idx)].x;// 0.50 - (tile_length*2.) + gl_InstanceID;// % tile_length;	// translate x
    translation_wave.y = params.instance_pos_wave[int(idx)].y;// 0.5 + lfo(params.waveform_type,(gl_InstanceID+time*params.waveform_speed)*0.2)*6.0;
    translation_wave.z = 0.0;//remap(gl_InstanceID,0,NUM_INSTANCES*8,1.0,4.0)*10; 					// translate y
    translation_wave.w = 1;
    
    // Arrange the objects in a grid
    vec4 translation_grid;
    translation_grid.x = params.instance_pos_grid[gl_InstanceID].x;//0.5 - (tile_length/2) + gl_InstanceID % tile_length;	// translate x
    translation_grid.y = params.instance_pos_grid[gl_InstanceID].y;;//1.75 - (tile_length/2) + gl_InstanceID / tile_length * 1.5; 	// translate z
    translation_grid.z = 0; 					// translate y
    translation_grid.w = 1;						// needs to remain 1.
    
    // Try to make a circle using cos and sin
    vec4 translation_circle;
    float r = (3.0 - params.circle_motion) + sin(1. + gl_InstanceID  / 3. * time * .4) * params.circle_motion;
    translation_circle.x = r * cos(remap(gl_InstanceID,0.0,NUM_INSTANCES,0.0,TWO_PI));
    translation_circle.y = r * sin(remap(gl_InstanceID,0.0,NUM_INSTANCES,0.0,TWO_PI));
    translation_circle.z = 0;
    translation_circle.w = 1;						// needs to remain 1.

    vec4 translation = mix(translation_wave,translation_grid,params.shape_morph);
    
    
    //translation.y = atan(translation.z/128.0,translation.x/128.0);
    //translation.y = sin(length(vec2(translation.z,translation.x)) * 01.105 + params.time);
    // now scale the angle
    //translation.y *= length(vec2(translation.z,translation.x)) * (sin(params.time*0.1) * 02.2);
    
    // store the height as a vertex attribute.
    vertex.height = translation.z;
    
    
    // float offset = params.time; // will perform a full revolution every (3.14 * 2) seconds =)
    // float t = mod( offset + TWO_PI + atan(translation.z, translation.x), TWO_PI) / TWO_PI;
    // vertex.color = vec4(vec3(t),1);
    
    // let's take a step back, and sample the colors for each of our boxes using
    // a texture. First, get the correct sample coordinates.
    
    vec2 sampleCoords = vec2(vec2(translation.x,translation.y) / (tile_length/2));
    
    // let's just visualise these to see if they are correct!
    vertex.color.b = 0;
    vertex.color.rg = sampleCoords;
    
    // hmmm. that looks squished.
    // it's because the texture coordinates run from -1 to +1, but should go from 0 to 1.
    
    // first divide by 2
    
    sampleCoords *= vec2(0.5);
    
    // then add 0.5
    
    sampleCoords += vec2(0.5);
    
    // sampleCoords now mapped to 0..1 !
    // let's have a look
    vertex.color.rg = sampleCoords;
    
    // ok, now sample from the texture.
    
    // now, to hit the pixel fair and square, we need to offset the sampling point by half a pixel
    // because we have normalised texture coordinates, we need to find out how much the offset
    // woud be in normalised pixel coordinates. We get there, by dividing half a pixel in x through
    // width in pixels in x, and half a pixel in y divided by height in pixels in y.
    
    vec2 sampleOffset = vec2(0.5,0.5) * vec2(1.0/texSize.x,1.0/texSize.y);
    
    vertex.color.rgb = texture(tex_unit_0, sampleCoords + sampleOffset).rrr;
    
    
    // nice! now, let's move everything apart a little.
    translation.xyz *= 3;
    
    // let's contain it to elements that sit on a circle,
    // where the distance to the midpoint is less than 128.0
    // vertex.color *= 1.0 - step( 128.0, length(vec2(translation.z,translation.x)));
    
    perInstanceModelMatrix[0] = vec4(1,0,0,0);
    perInstanceModelMatrix[1] = vec4(0,1,0,0);
    perInstanceModelMatrix[2] = vec4(0,0,1,0);
    perInstanceModelMatrix[3] = translation;
    
    float lfo_scale = remap(sin(1. + gl_InstanceID  / 3. * time * (1.2*params.scale_speed)),-1.0,1.0,0.0,-2.);
    
    if(params.active_chair[gl_InstanceID] == 1) lfo_scale = remap(sin(time * params.transducer_speed[gl_InstanceID]),-1.0,1.0,-2.0,0.0);
    
    mat4 scaleMatrix;
    scaleMatrix[0] = vec4(lfo_scale,0,0,0);
    scaleMatrix[1] = vec4(0,lfo_scale,0,0); // we use translation value here
    scaleMatrix[2] = vec4(0,0,lfo_scale,0);
    scaleMatrix[3] = vec4(0,0,0,1);
    
    // Bonus Points: translation alone too boring?
    // how about squeezing in a rotation matrix?
    mat4 perInstanceRotationMatrix = rotationMatrix(vec3(1,1,0), gl_InstanceID / tile_length + (HALF_PI+time*(3.0*params.rot_speed)));
    
    // We move the box, before we even apply all the other matrices.
    // This works, because the next line really says:
    // vertex.position = viewMatrix * modelMatrix * perInstanceModelMatrix * position
    mat4 translatedModelView = modelViewMatrix * perInstanceModelMatrix;
    
    perInstanceModelViewMatrix = modelViewMatrix * perInstanceModelMatrix * scaleMatrix;

	//vertex.position = modelViewMatrix * perInstanceModelMatrix  * perInstanceRotationMatrix *position;
    if(params.active_chair[gl_InstanceID] == is_active)
    vertex.position = perInstanceModelViewMatrix * perInstanceRotationMatrix * position;

    // ---------------------------
    // # Let's calculate per-vertex normals:
    
    // Note that we disregard the scale operation, since in our case it must not
    // contribute to the normal matrix.
    vertex.normal =  mat3(translatedModelView) * normal;

	gl_Position = projectionMatrix * vertex.position;
}
