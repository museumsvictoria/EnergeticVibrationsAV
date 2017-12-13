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

uniform vec2      uTexelSize;

uniform float intensity;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 outputColor;


// Reaction-diffusion pass.
//
// Here's a really short, non technical explanation:
//
// To begin, sprinkle the buffer with some initial noise on the first few frames (Sometimes, the
// first frame gets skipped, so you do a few more).
//
// During the buffer loop pass, determine the reaction diffusion value using a combination of the
// value stored in the buffer's "X" channel, and a the blurred value - stored in the "Y" channel
// (You can see how that's done in the code below). Blur the value from the "X" channel (the old
// reaction diffusion value) and store it in "Y", then store the new (reaction diffusion) value
// in "X." Display either the "X" value  or "Y" buffer value in the "Image" tab, add some window
// dressing, then repeat the process. Simple... Slightly confusing when I try to explain it, but
// trust me, it's simple. :)
//
// Anyway, for a more sophisticated explanation, here are a couple of references below:
//
// Reaction-Diffusion by the Gray-Scott Model - http://www.karlsims.com/rd.html
// Reaction-Diffusion Tutorial - http://www.karlsims.com/rd.html

// Cheap vec2 to vec3 hash. Works well enough, but there are other ways.
vec3 hash33(in vec2 p){
    float n = sin(dot(p, vec2(41, 289)));
    return fract(vec3(2097152, 262144, 32768)*n);
}

// Serves no other purpose than to save having to write this out all the time. I could write a
// "define," but I'm pretty sure this'll be inlined.
vec4 tx(in vec2 p){ return texture(iChannel0, p); }

// Weighted blur function. Pretty standard.
float blur(in vec2 p){
    
    // Used to move to adjoining pixels. - uv + vec2(-1, 1)*px, uv + vec2(1, 0)*px, etc.
    vec3 e = vec3(1, 0, -1);
    vec2 px = 1./iResolution.xy;
    
    // Weighted 3x3 blur, or a cheap and nasty Gaussian blur approximation.
    float res = 0.0;
    // Four corners. Those receive the least weight.
    res += tx(p + e.xx*px ).x + tx(p + e.xz*px ).x + tx(p + e.zx*px ).x + tx(p + e.zz*px ).x;
    // Four sides, which are given a little more weight.
    res += (tx(p + e.xy*px ).x + tx(p + e.yx*px ).x + tx(p + e.yz*px ).x + tx(p + e.zy*px ).x)*2.;
    // The center pixel, which we're giving the most weight to, as you'd expect.
    res += tx(p + e.yy*px ).x*4.;
    // Normalizing.
    return res/16.;
    
}

// The reaction diffusion loop.
//
void main( void )
{
    //vec2 uv = gl_FragCoord.xy;
    vec2 uv = vTexCoord;// gl_FragCoord.xy/iResolution.xy; // Screen coordinates. Range: [0, 1]
    uv.y *= -1.;
    uv.y += 1.0;
    
    vec2 pw = 1./iResolution.xy; // Relative pixel width. Used for neighboring pixels, etc.

    
    // The blurred pixel. This is the result that's used in the "Image" tab. It's also reused
    // in the next frame in the reaction diffusion process (see below).
    float avgReactDiff = blur(uv);
    
    
    // The noise value. Because the result is blurred, we can get away with plain old static noise.
    // However, smooth noise, and various kinds of noise textures will work, too.
    vec3 noise = hash33(uv + vec2(53, 43)*iTime)*.6 + .2;
    
    // Used to move to adjoining pixels. - uv + vec2(-1, 1)*px, uv + vec2(1, 0)*px, etc.
    vec3 e = vec3(1, 0, -1);
    
    // Gradient epsilon value. The "1.5" figure was trial and error, but was based on the 3x3 blur radius.
    vec2 pwr = pw*1.5;
    
    // Use the blurred pixels (stored in the Y-Channel) to obtain the gradient. I haven't put too much
    // thought into this, but the gradient of a pixel on a blurred pixel grid (average neighbors), would
    // be analogous to a Laplacian operator on a 2D discreet grid. Laplacians tend to be used to describe
    // chemical flow, so... Sounds good, anyway. :)
    //
    // Seriously, though, take a look at the formula for the reacion-diffusion process, and you'll see
    // that the following few lines are simply putting it into effect.
    
    // Gradient of the blurred pixels from the previous frame.
    vec2 lap = vec2(tx(uv + e.xy*pwr).y - tx(uv - e.xy*pwr).y, tx(uv + e.yx*pwr).y - tx(uv - e.yx*pwr).y);//
    
    // Add some diffusive expansion, scaled down to the order of a pixel width.
    uv = uv + lap*pw*3.0;
    
    // Stochastic decay. Ie: A differention equation, influenced by noise.
    // You need the decay, otherwise things would keep increasing, which in this case means a white screen.
    float newReactDiff = tx(uv).x + (noise.z - 0.5)*0.0025 - (0.002 + (0.1-(intensity*0.1)));
    
    // Reaction-diffusion.
    newReactDiff += dot(tx(uv + (noise.xy-0.5)*pw).xy, vec2(1, -1))*0.145;
    
    
    // Storing the reaction diffusion value in the X channel, and avgReactDiff (the blurred pixel value)
    // in the Y channel. However, for the first few frames, we add some noise. Normally, one frame would
    // be enough, but for some weird reason, it doesn't always get stored on the very first frame.
    float ifr = min(0.97, float(iFrame) / 100.0);

    //outputColor.xy += mix(texture(iChannel1, uv).xz, clamp(vec2(newReactDiff, avgReactDiff/.98), 0., 1.), ifr);
    
    outputColor = mix(texture(iChannel1, uv), vec4(clamp(vec2(newReactDiff, avgReactDiff/.98), 0., 1.),0.0,.950),ifr);
    
//    vec4 a = texture(iChannel0, uv);
//    vec4 b = texture(iChannel0, vTexCoord);
//
//    vec4 final = b*vec4(1.0,0.0,1.0,1.0);
//    outputColor = final;
}

