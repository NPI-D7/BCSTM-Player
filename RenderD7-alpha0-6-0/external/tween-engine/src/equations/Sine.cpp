//
//  Sine.cpp
//
//  This code is derived from Universal Tween Engine
//  Licensed under Apache License 2.0 - http://www.apache.org/licenses/LICENSE-2.0
//

#include <math.h>
#include <TweenEngine/equations/Sine.h>

#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923

namespace TweenEngine
{
    float SineIn::compute(float t) { return (float)-cos(t * (M_PI_2)) + 1; }
    const char *SineIn::toString() { return "Sine.IN"; }
    
    float SineOut::compute(float t) { return (float)sin(t * (M_PI_2)); }
    const char *SineOut::toString() { return "Sine.OUT"; }
    
    float SineInOut::compute(float t) { return -0.5f * ((float)cos(M_PI*t) - 1); }
    const char *SineInOut::toString() { return "Sine.INOUT"; }
}
