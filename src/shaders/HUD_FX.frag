#version 410

#pragma include "Common/random.glsl"

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform sampler2DRect iChannel0;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 outputColor;

//--------------- Bloom Parameters
const float blurSize = 1.0/512.0;
const float intensity = 0.35;


void bloom(inout vec4 tex, in vec2 uv){
    vec2 texcoord = uv;
    int j;
    int i;
    
    //thank you! http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/ for the
    //blur tutorial
    // blur in y (vertical)
    // take nine samples, with the distance blurSize between them
    tex += texture(iChannel0, vec2(texcoord.x - 4.0*blurSize, texcoord.y)) * 0.05;
    tex += texture(iChannel0, vec2(texcoord.x - 3.0*blurSize, texcoord.y)) * 0.09;
    tex += texture(iChannel0, vec2(texcoord.x - 2.0*blurSize, texcoord.y)) * 0.12;
    tex += texture(iChannel0, vec2(texcoord.x - blurSize, texcoord.y)) * 0.15;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y)) * 0.16;
    tex += texture(iChannel0, vec2(texcoord.x + blurSize, texcoord.y)) * 0.15;
    tex += texture(iChannel0, vec2(texcoord.x + 2.0*blurSize, texcoord.y)) * 0.12;
    tex += texture(iChannel0, vec2(texcoord.x + 3.0*blurSize, texcoord.y)) * 0.09;
    tex += texture(iChannel0, vec2(texcoord.x + 4.0*blurSize, texcoord.y)) * 0.05;
    
    // blur in y (vertical)
    // take nine samples, with the distance blurSize between them
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y - 4.0*blurSize)) * 0.05;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y - 3.0*blurSize)) * 0.09;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y - 2.0*blurSize)) * 0.12;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y - blurSize)) * 0.15;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y)) * 0.16;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y + blurSize)) * 0.15;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y + 2.0*blurSize)) * 0.12;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y + 3.0*blurSize)) * 0.09;
    tex += texture(iChannel0, vec2(texcoord.x, texcoord.y + 4.0*blurSize)) * 0.05;
    
    //increase blur with intensity!
    tex = tex*intensity + texture(iChannel0, texcoord);
}

//------------ Blur
float normpdf(in float x, in float sigma)
{
    return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

void blur(inout vec4 tex, in vec2 uv){
    vec3 c = texture(iChannel0, uv).rgb;
    
    //declare stuff
    const int mSize = 11;
    const int kSize = (mSize-1)/2;
    float kernel[mSize];
    vec3 final_colour = vec3(0.0);
    
    //create the 1-D kernel
    float sigma = 7.0;
    float Z = 0.0;
    for (int j = 0; j <= kSize; ++j)
    {
        kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
    }
    
    //get the normalization factor (as the gaussian has been clamped)
    for (int j = 0; j < mSize; ++j)
    {
        Z += kernel[j];
    }
    
    //read out the texels
    for (int i=-kSize; i <= kSize; ++i)
    {
        for (int j=-kSize; j <= kSize; ++j)
        {
            final_colour += kernel[kSize+j]*kernel[kSize+i]*texture(iChannel0, (gl_FragCoord.xy+vec2(float(i),float(j))) ).rgb;
            
            /*
            final_colour += kernel[kSize+j]*kernel[kSize+i]*texture(iChannel0, (gl_FragCoord.xy+vec2(float(i),float(j))) / iResolution.xy).rgb;
            */
            
        }
    }

    tex = vec4(final_colour/(Z*Z), 1.0);
}

//--------------- TV GLITCH
// change these values to 0.0 to turn off individual effects
float vertJerkOpt = 0.0;
float vertMovementOpt = 0.0;
float bottomStaticOpt = 0.0;
float scalinesOpt = 1.0;
float rgbOffsetOpt = 0.0;
float horzFuzzOpt = step(0.95,random(iGlobalTime*0.3))*2.;

// Noise generation functions borrowed from:
// https://github.com/ashima/webgl-noise/blob/master/src/noise2D.glsl

vec3 mod289(vec3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
    return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,  // -1.0 + 2.0 * C.x
                        0.024390243902439); // 1.0 / 41.0
    // First corner
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);
    
    // Other corners
    vec2 i1;
    //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
    //i1.y = 1.0 - i1.x;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    // x0 = x0 - 0.0 + 0.0 * C.xx ;
    // x1 = x0 - i1 + 1.0 * C.xx ;
    // x2 = x0 - 1.0 + 2.0 * C.xx ;
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    
    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
                     + i.x + vec3(0.0, i1.x, 1.0 ));
    
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;
    
    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
    
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    
    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
    
    // Compute final noise value at P
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

float staticV(vec2 uv) {
    float staticHeight = snoise(vec2(9.0,iGlobalTime*1.2+3.0))*0.3+5.0;
    float staticAmount = snoise(vec2(1.0,iGlobalTime*1.2-6.0))*0.1+0.3;
    float staticStrength = snoise(vec2(-9.75,iGlobalTime*0.6-3.0))*2.0+2.0;
    return (1.0-step(snoise(vec2(5.0*pow(iGlobalTime,2.0)+pow(uv.x*7.0,1.2),pow((mod(iGlobalTime,100.0)+100.0)*uv.y*0.3+3.0,staticHeight))),staticAmount))*staticStrength;
}

void glitch(inout vec4 tex, in vec2 uv){
    
    uv = gl_FragCoord.xy / iResolution.xy;
    
    float jerkOffset = (1.0-step(snoise(vec2(iGlobalTime*1.3,5.0)),0.8))*0.05;
    
    float fuzzOffset = snoise(vec2(iGlobalTime*15.0,uv.y*80.0))*0.003;
    float largeFuzzOffset = snoise(vec2(iGlobalTime*1.0,uv.y*25.0))*0.004;
    
    float vertMovementOn = (1.0-step(snoise(vec2(iGlobalTime*0.2,8.0)),0.4))*vertMovementOpt;
    float vertJerk = (1.0-step(snoise(vec2(iGlobalTime*1.5,5.0)),0.6))*vertJerkOpt;
    float vertJerk2 = (1.0-step(snoise(vec2(iGlobalTime*5.5,5.0)),0.2))*vertJerkOpt;
    float yOffset = abs(sin(iGlobalTime)*4.0)*vertMovementOn+vertJerk*vertJerk2*0.3;
    float y = mod(uv.y+yOffset,1.0);
    
    
    float xOffset = (fuzzOffset + largeFuzzOffset) * horzFuzzOpt;
    
    float staticVal = 0.0;
    
    for (float y = -1.0; y <= 1.0; y += 1.0) {
        float maxDist = 5.0/200.0;
        float dist = y/200.0;
        staticVal += staticV(vec2(uv.x,uv.y+dist))*(maxDist-abs(dist))*1.5;
    }
    
    staticVal *= bottomStaticOpt;
    
    float red 	=   texture(	iChannel0, 	vec2(uv.x + xOffset -0.01*rgbOffsetOpt,y) * iResolution.xy ).r+staticVal;
    float green = 	texture(	iChannel0, 	vec2(uv.x + xOffset,	  y) * iResolution.xy ).g+staticVal;
    float blue 	=	texture(	iChannel0, 	vec2(uv.x + xOffset +0.01*rgbOffsetOpt,y) * iResolution.xy ).b+staticVal;
    
    vec3 color = vec3(red,green,blue);
    float scanline = sin(uv.y*800.0)*0.04*scalinesOpt;
    color -= scanline;
    
    tex = vec4(color,1.0);
    
   // tex = texture(iChannel0, uv * iResolution.xy );
}

void main( void )
{
    vec2 uv = gl_FragCoord.xy;
    
    vec4 final = vec4(0.0);
    final *= bloom(final, uv);
    final *= glitch(final,uv);
    //final *= blur(final,uv);
    outputColor = final;
}

