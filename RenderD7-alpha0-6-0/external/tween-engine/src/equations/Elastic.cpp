//
//  Elastic.cpp
//
//  This code is derived from Universal Tween Engine
//  Licensed under Apache License 2.0 - http://www.apache.org/licenses/LICENSE-2.0
//

#include <math.h>
#include <TweenEngine/equations/Elastic.h>

#define M_PI        3.14159265358979323846
#define M_TWOPI         (M_PI * 2.0)

namespace TweenEngine
{
    float ElasticIn::compute(float t) {
        float a = amplitude;
        float p = period;
        if (t == 0) return 0;
        if (t == 1) return 1;
        if (!isPeriodSet) p = 0.3;
        float s;
        if (!isAmplitudeSet || a < 1) {
            a = 1;
            s = p/4.0;
        } else {
            s = p/(M_TWOPI) * (float)asin(1/a);
        }
        t -= 1;
        return -(a*(float)pow(2,10*t) * (float)sin((t-s)*(M_TWOPI)/p ));
    }
    const char *ElasticIn::toString() { return "Elastic.IN"; }
    void ElasticIn::setAmplitude(float a) { this->amplitude = a; this->isAmplitudeSet = true; }
    void ElasticIn::setPeriod(float p) { this->period = p; this->isPeriodSet = true; }
    
    float ElasticOut::compute(float t) {
        float a = amplitude;
        float p = period;
        if (t==0) return 0;
        if (t==1) return 1;
        if (!isPeriodSet) p = 0.3f;
        float s;
        if (!isAmplitudeSet || a < 1) {
            a = 1;
            s = p/4.0;
        } else {
            s = p/(M_TWOPI) * (float)asin(1/a);
        }
        return a*(float)pow(2,-10*t) * (float)sin((t-s)*(M_TWOPI)/p ) + 1;
    }
    const char *ElasticOut::toString() { return "Elastic.OUT"; }
    void ElasticOut::setAmplitude(float a) { this->amplitude = a; this->isAmplitudeSet = true; }
    void ElasticOut::setPeriod(float p) { this->period = p; this->isPeriodSet = true; }
    
    float ElasticInOut::compute(float t) {
        float a = amplitude;
        float p = period;
        if (t==0) return 0;
        t *= 2;
        if (t==2) return 1;
        if (!isPeriodSet) p = 0.3f*1.5f;
        float s;
        if (!isAmplitudeSet || a < 1) {
            a = 1;
            s = p/4.0;
        } else {
            s = p/(M_TWOPI) * (float)asin(1/a);
        }
        if (t < 1) {
            t -= 1;
            return -0.5f*(a*(float)pow(2,10*t) * (float)sin((t-s)*(M_TWOPI)/p));
        } else {
            t -= 1;
            return a*(float)pow(2,-10*t) * (float)sin((t-s)*(M_TWOPI)/p)*0.5f + 1;
        }
    }
    const char *ElasticInOut::toString() { return "Elastic.INOUT"; }
    void ElasticInOut::setAmplitude(float a) { this->amplitude = a; this->isAmplitudeSet = true; }
    void ElasticInOut::setPeriod(float p) { this->period = p; this->isPeriodSet = true; }
}
