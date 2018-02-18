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

uniform float dry_wet;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 outputColor;


//Sercan Turkmen - sercanturkmen@outlook.com

vec2 pixelDim = vec2(6, 6); // you can alter with the pixel dimensions here

float remap( float value, float inMin, float inMax, float outMin, float outMax )
{
    return ( (value - inMin) / ( inMax - inMin ) * ( outMax - outMin ) ) + outMin;
}

void main( void )
{
    
    vec2 uv = vTexCoord;
    uv.y *= -1.;
    uv.y += 1.0;
    
    vec3 avg = vec3(0.0);
    
    pixelDim.x = 2. + (abs(sin(iTime* 0.5)) * 20.);
    pixelDim.y = 2. + abs(cos(iTime* 0.35)) * 20.;
    
    /*
     for(int j=0; j<int(pixelDim.y); j++){
     for(int i=0; i<int(pixelDim.x); i++){
     avg += texture(
     iChannel0,
     (fragCoord + vec2(i,j)-mod(fragCoord, pixelDim))/ iResolution.xy).rgb;
     }
     }
     avg /= pixelDim.x * pixelDim.y;
     
     fragColor = vec4(avg, 1.0);
     */
    
    
    for(int j=0; j<int(pixelDim.y/3.); j++){
        for(int i=0; i<int(pixelDim.x/3.); i++){
            avg += texture(iChannel0,(gl_FragCoord.xy + vec2(i,j)-mod(gl_FragCoord.xy, pixelDim))/ iResolution.xy).rgb;
        }
    }
    
    //avg *= remap(pixelDim.x, 0.0, 28.0, 1.0, .150);
    avg /= (pixelDim.x * pixelDim.y);
    avg *= 20.0;
    //fragColor = vec4(avg, 1.0);
    
    vec3 t = texture(iChannel0, (gl_FragCoord.xy + vec2(11,11)-mod(gl_FragCoord.xy, pixelDim))/ iResolution.xy).rgb;
    
    outputColor = vec4(mix(texture(iChannel0,uv).rgb, t + (avg*0.2),dry_wet) ,1.0);
    
}

