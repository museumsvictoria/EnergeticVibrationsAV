#version 410


uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;           // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform sampler2D iChannel0;          // input channel. XX = 2D/Cube
uniform sampler2D iChannel1;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;

uniform float strength;
uniform float zoom;
uniform float x_mult;
uniform float y_mult;
uniform float x_amp;
uniform float y_amp;
uniform float x_speed;
uniform float y_speed;
uniform float rotate_speed;
uniform float rotate_amp;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 outputColor;

// Ported from https://www.shadertoy.com/view/ltVSWW
// by  Jespertheend

mat2 rotate(float angle) {
    return mat2(cos(angle), -sin(angle),
                sin(angle), cos(angle));
}

void main( void )
{
    
    vec2 uv = vTexCoord;
    uv.y *= -1.;
    uv.y += 1.0;
    
    //circle zoom and deformation
    vec2 xy=uv;//iResolution.xy;//xy=-.5*(xy-2.0*fragCoord.xy)/xy.x;
    //xy*=1.0+sin(iTime*.60)*0.05;
    //xy.x+=sin(xy.x*32.0+iTime*16.0)*0.01;
    //xy.y+=sin(xy.y*16.0+iTime*8.0)*0.01;
    //uv = xy;
    
    vec3 col = texture(iChannel0, uv).rgb;
    

    //float
    vec2 uv2 = uv - 0.5;
    uv2 *= rotate((sin(iTime*rotate_speed)*(rotate_amp*0.0314)));
    uv2 *= (1.0 + (zoom*0.05));
    uv2 = uv2 + 0.5;
    

    uv2.x+=sin(xy.x*x_mult+iTime*(x_speed*4.0))*(x_amp*0.01);
    uv2.y+=sin(xy.y*y_mult+iTime*(y_speed*4.0))*(y_amp*0.01);


    // Preserver the position for the top buffer when zooming
   // uv = uv2 * (1.0 + (0.02));
   // uv = uv - 0.01;
    
    //uv-= 0.05;


    vec3 old = texture(iChannel1, uv).rgb;
//    if(g < 0.65 && iFrame > 5){
    float t = 0.00;//0.005;
    if(old.r > t && old.g > t && old.b > t && iFrame > 5){
        outputColor = vec4(old,1.);
    }else{
        col = texture(iChannel0, uv2).rgb;
        outputColor = vec4(col,strength);
    }
    
    //outputColor = vec4(old+(col*0.8),1.0);
    //outputColor = vec4(old,1.);

//
//    vec2 tc = vTexCoord;
//    tc.y *= -1.;
//    tc.y += 1.0;
//    
//    tc.x += sin(iTime*0.6)*.005;
//    tc.y += sin(iTime*0.9)*.005;
//    
//    vec2 uv = tc;
//
//
//    
//    
//    uv.y *= 0.998;
//    
//    vec4 sum = texture(iChannel1, uv);
//    vec4 src = texture(iChannel0, tc);
//    
//    sum.rgb = mix(sum.rbg, src.rgb, 0.5725);
//    
//    outputColor = sum;
}

