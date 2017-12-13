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

// inputs
in vec2 vTexCoord;

// outputs
out vec4 outputColor;

/*
	Reaction Diffusion - 2 Pass
	---------------------------
 
	Simple 2 pass reaction-diffusion, based off of "Flexi's" reaction-diffusion examples.
	It takes about ten seconds to reach an equilibrium of sorts, and in the order of a
	minute longer for the colors to really settle in.
 
	I'm really thankful for the examples Flexi has been putting up lately. From what I
	understand, he's used to showing his work to a lot more people on much bigger screens,
	so his code's pretty reliable. Reaction-diffusion examples are temperamental. Change
	one figure by a minute fraction, and your image can disappear. That's why it was really
	nice to have a working example to refer to.
	
    Anyway, I've done things a little differently, but in essense, this is just a rehash
	of Flexi's "Expansive Reaction-Diffusion" example. I've stripped this one down to the
	basics, so hopefully, it'll be a little easier to take in than the multitab version.
 
	There are no outside textures, and everything is stored in the A-Buffer. I was
	originally going to simplify things even more and do a plain old, greyscale version,
	but figured I'd better at least try to pretty it up, so I added color and some very
	basic highlighting. I'll put up a more sophisticated version at a later date.
 
	By the way, for anyone who doesn't want to be weighed down with extras, I've provided
	a simpler "Image" tab version below.
 
	One more thing. Even though I consider it conceptually impossible, it wouldn't surprise
	me at all if someone, like Fabrice, produces a single pass, two tweet version. :)
 
	Based on:
	
	// Gorgeous, more sophisticated example:
	Expansive Reaction-Diffusion - Flexi
	https://www.shadertoy.com/view/4dcGW2
 
	// A different kind of diffusion example. Really cool.
	Gray-Scott diffusion - knighty
	https://www.shadertoy.com/view/MdVGRh
 
	
 */


void main( void )
{
    // The screen coordinates.
    vec2 uv = vTexCoord; // Screen coordinates. Range: [0, 1]
//    vec2 uv = gl_FragCoord.xy;
    
    uv.y *= -1.;
    uv.y += 1.0;

    // Read in the blurred pixel value. There's no rule that says you can't read in the
    // value in the "X" channel, but blurred stuff is easier to bump, that's all.
    float c = 1. - texture(iChannel0, uv).y;
    // Reading in the same at a slightly offsetted position. The difference between
    // "c2" and "c" is used to provide the highlighting.
    float c2 = 1. - texture(iChannel0, uv + .5/iResolution.xy).y;
    
    vec3 orig = texture(iChannel1,uv).rgb;

    // Color the pixel by mixing two colors in a sinusoidal kind of pattern.
    //
    float pattern = -cos(uv.x*0.75*3.14159-0.9)*cos(uv.y*1.5*3.14159-0.75)*0.5 + 0.5;
    //
    // Blue and gold, for an abstract sky over a... wheat field look. Very artsy. :)
    vec3 col = vec3(c*1.5, pow(c, 2.25), pow(c, 6.));
    col = mix(col, col.zyx, clamp(pattern-.2, 0., 1.) );
    
    // Extra color variations.
    //col = mix(vec3(c*1.2, pow(c, 8.), pow(c, 2.)), vec3(c*1.3, pow(c, 2.), pow(c, 10.)), pattern );
    //vec3 col = mix(vec3(c*1.3, c*c, pow(c, 10.)), vec3(c*c*c, c*sqrt(c), c), pattern );
    
    col -= orig.rbb;

    // Adding the highlighting. Not as nice as bump mapping, but still pretty effective.
    col += vec3(.6, .85, 1.)*max(c2*c2 - c*c, 0.)*12.;
    
    // Apply a vignette and increase the brightness for that fake spotlight effect.
   // col *= pow( 16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y) , .125)*1.15;
    
    // Fade in for the first few seconds.
    col *= smoothstep(0., 1., iTime/2.);
    
    // Done.
    outputColor = vec4(1.0-min(col, 1.), 1.);

}

