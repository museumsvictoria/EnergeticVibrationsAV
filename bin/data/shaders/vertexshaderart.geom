/***********************************************
 Geometry shader to convert lines into triangle strips
 Memo Akten
 www.memo.tv

************************************************/

#version 330
#pragma include <of_default_uniforms.glsl>
#pragma include "Util/easing_lfo.glsl"

layout(triangles) in;
layout(triangle_strip, max_vertices=45) out; //15) out;

//uniform float normal_length;
//uniform float time;
uniform float time;
uniform float explode_amount;

uniform int active_idx;
uniform float geom_max_height;
uniform int geom_num_copies;

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
    
    if(active_idx < 4){
        normal *= -1.0; // Flip our normal. oF's normals seem inverted?
    }
    
    float instance_offset = (vertexOut.instance_ID / 18.0) * PI;
    float primitive_id =gl_PrimitiveIDIn;//  mod(gl_PrimitiveIDIn,2.0) ;
    
    float effect_lfo = easing_lfo(geom_effect_lfo_type,instance_offset + ((primitive_id*(geom_effect_lfo_offset*0.02))+time*(geom_effect_lfo_speed*10.0)))*geom_effect_lfo_amp;

    float lfo_scale = easing_lfo(geom_lfo_type,instance_offset + ((primitive_id*(geom_lfo_offset*0.1))+time*(geom_lfo_speed*20.0)))*geom_lfo_amp;
    
    //------ Pass Through
    for( int z = 0; z < geom_num_copies; z++){
        for (int i = 0; i < gl_in.length(); i++)
        {
            //---------------------------
            // Calculate the centroid point (just sum up all coordinates and divide by 3
            // You can see built-in variable gl_in here, notice adding normal multiplied by bender value
            vec3 vMiddle = (gl_in[0].gl_Position.xyz+gl_in[1].gl_Position.xyz+gl_in[2].gl_Position.xyz)/3.0;

            vec4 tri_size = mix(vertex[i].position, vec4(vMiddle,1.0) , fract(lfo_scale*((1+z)*(1.0/geom_num_copies))));


            vec4 tri_height = mix(vertex[i].position, (vertex[i].position-(vec4(vMiddle,1.0))*.01) + vec4(normal.xyz,1.0) * (geom_max_height*z), lfo_scale);
        
           
            gl_Position = projectionMatrix * mix(tri_height, tri_size,mix(geom_effect_mix,effect_lfo,geom_effect_lfo_amp ));
            
           // gl_Position = projectionMatrix * gl_in[i].gl_Position  ;
            EmitVertex();
        }
        EndPrimitive();
    }
    
    
//    for (int i = 0; i < gl_in.length(); i++)
//    {
//        //---------------------------
//        // Calculate the centroid point (just sum up all coordinates and divide by 3
//        // You can see built-in variable gl_in here, notice adding normal multiplied by bender value
//        vec3 vMiddle = (gl_in[0].gl_Position.xyz+gl_in[1].gl_Position.xyz+gl_in[2].gl_Position.xyz)/3.0;
//        
//        vec4 tri_size = mix(vertex[i].position, vec4(vMiddle,1.0) , lfo_scale);
//        
//        
//        vec4 tri_height = mix(vertex[i].position, (vertex[i].position-(vec4(vMiddle,1.0))*.01) + vec4(normal.xyz,1.0) *max_height, lfo_scale);
//        
//        
//        gl_Position = projectionMatrix * mix(tri_height, tri_size,mix(geom_effect_mix,effect_lfo,geom_effect_lfo_amp));
//        
//        // gl_Position = projectionMatrix * gl_in[i].gl_Position  ;
//        EmitVertex();
//    }
//    EndPrimitive();

}


/*********** JAMES ACRES TIPS
 ///--------------------------------------------
 
 //modelViewMatriz*position;
 
 //projectiionMatrix;
 
 //1 model space =  raw vertex position + translate to model view prjotion space
 //2 view space = transform vetticves to view space. raltive to camera
 
 
 // model veiwMATERIX * vertex.psotion = gl_potiions
 
 // Gl_in = view view space
 
 // ws or vs - go with world space!
 //if(lfo_scale < 0.1 + abs(sin(time*0.2)))
 
 // vertex out = modelMatrix* vertex.position !!
 
 // Normal * inversetranspose(modeviewmatriz); /// Normals might be in view space.
 
 // make a fragment sahder that just draws the normal.
 
 // Just saty in world sapce, colour will stay the same.
 
 // Texrure buffer object. 4 byte floats per pixel. vertexIf as alookup into that texture.
 
 //viewSpace *
 
 //projectionMatirx * result.
 
 //multiply by inverse of viewMatrix
 
 // Normals use sine, use the faces to move along the normal. if direction chages, then you know that your normal isn't being calcuated properlt. You need the normal matrix. inverse transpose of modelViewmatrix.
 
 // Normal * normalMatrix;
 */
