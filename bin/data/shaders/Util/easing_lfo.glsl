#pragma include "lfos.glsl"
#pragma include "easings.glsl"

float easing_lfo(int type, float x){
    //----LFOS
    if(type == 0) return sin(x);
    if(type == 1) return tri(x);
    if(type == 2) return saw(x);
    if(type == 3) return pulse(x);
    if(type == 4) return noise(x);
    //----EASINGS
    if(type == 5) return exponentialIn(fract(x));
    if(type == 6) return exponentialOut(fract(x));
    if(type == 7) return exponentialInOut(fract(x));
    if(type == 8) return sineIn(fract(x));
    if(type == 9) return sineOut(fract(x));
    if(type == 10) return sineInOut(fract(x));
    if(type == 11) return qinticIn(fract(x));
    if(type == 12) return qinticOut(fract(x));
    if(type == 13) return qinticInOut(fract(x));
    if(type == 14) return quarticIn(fract(x));
    if(type == 15) return quarticOut(fract(x));
    if(type == 16) return quarticInOut(fract(x));
    if(type == 17) return quadraticInOut(fract(x));
    if(type == 18) return quadraticIn(fract(x));
    if(type == 19) return quadraticOut(fract(x));
    if(type == 20) return cubicIn(fract(x));
    if(type == 21) return cubicOut(fract(x));
    if(type == 22) return cubicInOut(fract(x));
    if(type == 23) return elasticIn(fract(x));
    if(type == 24) return elasticOut(fract(x));
    if(type == 25) return elasticInOut(fract(x));
    if(type == 26) return circularIn(fract(x));
    if(type == 27) return circularInOut(fract(x));
    if(type == 28) return bounceIn(fract(x));
    if(type == 29) return bounceOut(fract(x));
    if(type == 30) return bounceInOut(fract(x));
    if(type == 31) return backIn(fract(x));
    if(type == 32) return backOut(fract(x));
    if(type == 33) return backInOut(fract(x));
}
