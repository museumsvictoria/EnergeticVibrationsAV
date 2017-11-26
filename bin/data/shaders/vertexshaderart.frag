#version 400

//     _____    ___
//    /    /   /  /     triPlanar.frag
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

const float TWO_PI = 6.28318530717959;
const float PI = 3.141592653589793;

uniform mat4 modelViewMatrix;
uniform int is_active;
uniform float tick_position;

in mat4 perInstanceModelViewMatrix;

in VertexAttrib {
    vec4 position;
    vec4 color;
    vec3 normal;
    vec2 texcoord;
    float height;
    float instance_ID;
} vertex;

out vec4 fragColor;


// ----------------------------------------------------------------------
vec3 getHeatmapRGBColourForNormalisedValue(const in float value_){
    float value = 1.0-value_;
    vec3 colour = 	(0.5+0.5*smoothstep(0.0, 0.1, value)) *
    vec3(smoothstep(0.5, 0.3, value),
         value < 0.3 ? smoothstep(0.0, 0.3, value) : smoothstep(1.0, 0.6, value),
         smoothstep(0.4, 0.6, value));
    return colour;
}

// ----------------------------------------------------------------------
void addRimLighting(in vec3 N, in vec3 viewVector, inout vec3 rimLight_){
    // we are interested in the angle between the surface normal and the viewVector.
    // since we are in eye space, our viewing vector for traditional rim lighting
    // would be towards negative z (0,0,-1). But other vectors give nice effects, too!
    
    rimLight_ += vec3(clamp(dot(N, viewVector), 0.0001, 1.0));
}

// See if I can bring over tri planar or flat shading algorithm
// to then create my own textures. 

// have a look at this technique http://greggman.github.io/doodles/lightball03.html
// ----------------------------------------------------------------------
void main(){
    
    float glowAmt = 0.5;
    
    vec3 boxColor = getHeatmapRGBColourForNormalisedValue(vertex.height / 6.0);
    vec3 N = normalize(vertex.normal);
    
    vec3 rimLight = vec3(0);
    addRimLighting(N, vec3(0,0,1), rimLight);

    if(is_active == 1){
        fragColor = vec4(0,0,0,1);

        fragColor.rgb += boxColor;
        
        fragColor.rgb += rimLight * 0.2;

        fragColor.rgb = pow(fragColor.rgb, vec3(-0.5));
        fragColor.rgb *= vec3(0.3,0.85,1.0);

        fragColor.rgb += clamp(glowAmt*0.4,0.,1.)*vec3(.3,.5,.7);
    } else {
        fragColor = vec4((N + vec3(1.0, 1.0, 1.0)) / 2.0,1.0);
    }
    
    /*
    if(tick_position == vertex.instance_ID){
        fragColor = vec4(1.0,0.0,0.0,1.0);
    }
*/
    
    if(vertex.position.y < -0.3 && vertex.position.y > 0.3){
        fragColor = vec4(1.0,0.0,0.0,1.0);
    }
    
    //fragColor.rgb += vertex.normal * vertex.height;
    
    // fragColor = vec4(vertex.texcoord.x, vertex.texcoord.y, 0 , 1.0);
    // fragColor = vec4(vertex.normal * 0.5 + vec3(0.5,0.5,0.5),1.0);
    
}

