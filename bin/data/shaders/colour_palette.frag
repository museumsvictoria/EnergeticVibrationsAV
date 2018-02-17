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

uniform vec4 color_1;
uniform vec4 color_2;
uniform vec4 color_3;
uniform vec4 color_4;
uniform vec4 color_5;

// inputs
in vec2 vTexCoord;

// outputs
out vec4 outputColor;

const float eps = 0.0000001;

vec3 rgb2hsv( in vec3 c)
{
    vec4 k = vec4(0.0, -1.0/3.0, 2.0/3.0, -1.0);
    vec4 p = mix(vec4(c.zy, k.wz), vec4(c.yz, k.xy), (c.z<c.y) ? 1.0 : 0.0);
    vec4 q = mix(vec4(p.xyw, c.x), vec4(c.x, p.yzx), (p.x<c.x) ? 1.0 : 0.0);
    float d = q.x - min(q.w, q.y);
    return vec3(abs(q.z + (q.w - q.y) / (6.0*d+eps)), d / (q.x+eps), q.x);
}

vec3 rgb2hsl( vec3 col )
{
    float minc = min( col.r, min(col.g, col.b) );
    float maxc = max( col.r, max(col.g, col.b) );
    vec3  mask = step(col.grr,col.rgb) * step(col.bbg,col.rgb);
    vec3 h = mask * (vec3(0.0,2.0,4.0) + (col.gbr-col.brg)/(maxc-minc + eps)) / 6.0;
    return vec3( fract( 1.0 + h.x + h.y + h.z ),              // H
                (maxc-minc)/(1.0-abs(minc+maxc-1.0) + eps),   // S
                (minc+maxc)*0.5 );                            // L
}

vec3 hsl2rgb( in vec3 c )
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}

void main( void )
{
    
    // The screen coordinates.
    vec2 uv = vTexCoord; // Screen coordinates. Range: [0, 1]
    
    vec4 colours[8];
    colours[0] = vec4(0.255,0.745,0.670,1.0); // aqua
    colours[1] = vec4(0.235,0.596,0.807,1.0); // blue
    colours[2] = vec4(0.545,0.411,0.827,1.0); // light pu
    colours[3] = vec4(0.149,0.133,0.384,1.0); // dark pur
    colours[4] = vec4(0.305,0.658,0.239,1.0); // green
    colours[5] = vec4(0.494,0.098,0.286,1.0); // mitch ma
    colours[6] = vec4(0.000,0.607,0.490,1.0); // aqua gre
    colours[7] = vec4(0.784,0.180,0.152,1.0); // orange
    
    
    // Time varying pixel color
    vec3 tex = texture(iChannel0,uv).rgb;
    
    vec3 hsl = rgb2hsl(tex);
    
    vec3 final_tex;
    
    int closest = 0;
    float closest_diff = 1000.0;
    vec3 closest_hsl = vec3(0.0);
    float closest_hue = 0.0;
    float closest_sat = 0.0;
    for(int i = 0; i < 8; i++){
        vec3 colours_hsl = rgb2hsl(colours[i].rgb);
        
        float hue_diff = abs(colours_hsl.r - hsl.r);
        hue_diff = min(hue_diff, abs(hue_diff - 1.0));
        float diff = hue_diff
        + abs(colours_hsl.g - hsl.g)
        + abs(colours_hsl.b - hsl.b);
        if (diff < closest_diff) {
            closest = i;
            closest_diff = diff;
            closest_hsl = colours_hsl;
        }
    }
    
    hsl.r = closest_hsl.r;
    hsl.g = closest_hsl.g;
    //hsl.b = closest_hsl.b;
    final_tex = hsl2rgb(hsl);
    //final_tex = colours[closest].rgb;
    //final_tex = tex;
    // Output to screen
    outputColor = vec4(final_tex,1.0);
    
}

