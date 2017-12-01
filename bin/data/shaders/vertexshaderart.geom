/***********************************************
 Geometry shader to convert lines into triangle strips
 Memo Akten
 www.memo.tv

************************************************/

#version 330
#pragma include <of_default_uniforms.glsl>
#pragma include "Util/easing_lfo.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

//uniform float normal_length;
//uniform float time;
uniform float time;
uniform float explode_amount;

uniform int geom_lfo_type;
uniform float geom_lfo_offset;
uniform float geom_lfo_speed;
uniform float geom_lfo_amp;
uniform int geom_effect_lfo_type;
uniform float geom_effect_lfo_offset;
uniform float geom_effect_lfo_speed;
uniform float geom_effect_lfo_amp;


uniform float geom_effect_mix;

in VertexAttrib {
    vec4 position;
    vec4 color;
    vec3 normal;
    vec2 texcoord;
    float height;
    float instance_ID;
} vertex[];


out VertexAttrib {
    vec4 position;
    vec4 color;
    vec3 normal;
    vec2 texcoord;
    float height;
    float instance_ID;
    float primitive_ID;
} vertexOut;

//uniform mat4 gxl3d_ModelViewProjectionMatrix;
//in mat4 perInstanceModelViewMatrix;
//out mat4 perInstanceModelViewMatrix;
//uniform mat4 modelViewMatrix;
in mat4 vertexTransformMatrix[];

void main() {
    vertexOut.position = vertex[0].position;
    vertexOut.color = vertex[0].color;
    vertexOut.normal = vertex[0].normal;
    vertexOut.texcoord = vertex[0].texcoord;
    vertexOut.height = vertex[0].height;
    vertexOut.instance_ID = vertex[0].instance_ID;
    vertexOut.primitive_ID = gl_PrimitiveIDIn;
    
  
    // Calculate two vectors in the plane of the input triangle
    vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 ac = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 normal = normalize(cross(ab, ac));
    
    float explode_factor = 2.0;

    float primitive_id =gl_PrimitiveIDIn;//  mod(gl_PrimitiveIDIn,2.0) ;
    float effect_lfo = easing_lfo(geom_effect_lfo_type,((primitive_id*(geom_effect_lfo_offset*0.02))+time*(geom_effect_lfo_speed*10.0)))*geom_effect_lfo_amp;

    
    float lfo_scale = easing_lfo(geom_lfo_type,((primitive_id*(geom_lfo_offset*0.1))+time*(geom_lfo_speed*2.0)))*geom_lfo_amp;
    mat4 scaleMatrix;
    scaleMatrix[0] = vec4(lfo_scale,0,0,0);
    scaleMatrix[1] = vec4(0,lfo_scale,0,0); // we use translation value here
    scaleMatrix[2] = vec4(0,0,lfo_scale,0);
    scaleMatrix[3] = vec4(0,0,0,1);
    
//    //------ Pass Through
    for (int i = 0; i < gl_in.length(); i++)
    {
        //gl_Position = gl_in[i].gl_Position * 0.91 + (center*0.1);// + vec4(explode_factor * normal,0.0);
        //gl_Position = (gl_in[i].gl_Position * (0.5+abs(sin(time))*2.5)) + center*1.0;
        
        //float scaleFactor = abs(sin(time))*14.0;//0.005;
//        gl_Position =  (gl_in[i].gl_Position * (0.5+(lfo_scale*.5 )))  + vec4(normal,1.0) * scaleMatrix;//* scaleFactor;
        
        //        gl_Position = vertex[i].position  * scaleFactor;
        //        gl_Position = projectionMatrix*cameraMatrix*modelMatrix*vec4(vertex.xyz,1.0)+ normal*scaleFactor;
        
        //gl_Position = mix(gl_in[i].gl_Position, vec4(normal.xyz,1.0), abs(sin(time*0.5)));//* scaleFactor; // THIS ONE WORKS BEST SO FAR
        
        vec4 tri_height = mix(vertex[i].position, vertexTransformMatrix[i] * vertex[i].position + vec4(normal.xyz,1.0), lfo_scale);

        vec4 tri_size = mix(vertex[i].position, vertexTransformMatrix[i] * vec4(normal.xyz,1.0), lfo_scale);
        
        
        gl_Position = mix(tri_height, tri_size,mix(geom_effect_mix,effect_lfo,geom_effect_lfo_amp));


        //if(lfo_scale < 0.1 + abs(sin(time*0.2)))
        EmitVertex();
    }
    EndPrimitive();
    

    /*
    //------ Face normal
    //
    vec3 P0 = gl_in[0].gl_Position.xyz;
    vec3 P1 = gl_in[1].gl_Position.xyz;
    vec3 P2 = gl_in[2].gl_Position.xyz;
    
    vec3 V0 = P0 - P1;
    vec3 V1 = P2 - P1;
    
    // If the diff between V0 and V1 is too small,
    // the normal will be incorrect as well as the deformation.
    //
    vec3 diff = V1 - V0;
    float diff_len = length(diff);
    
    vec3 N = normalize(cross(V1, V0));

    float normal_length = 1.0;
    
    //------ Generate a new face along the direction of the face normal
    // only if diff_len is not too small.
    //
    if (length(diff_len) > 0.001)
    {
        int i;
        for(i=0; i<gl_in.length(); i++)
        {
            vec4 P = gl_in[i].gl_Position;
            vec3 N = normalize(cross(V1, V0));
            //            float len = sqrt(P.x*P.x + P.z*P.z);
            float len = sqrt(P.z*P.z);
            float scale = (0.5 + cos(time*2.0 + len)*0.5) * explode_amount ;
            
            vec4 face = (P + vec4(N * normal_length * scale,1.0)) + vec4(N * vec3(0.05, 0.05, 0.05),1.0);
            gl_Position = face;


            EmitVertex();
        }
        EndPrimitive();
    }
    */
    
}
