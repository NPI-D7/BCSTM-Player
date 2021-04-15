//
//  TweenEquations.h
//
//  This code is derived from Universal Tween Engine
//  Licensed under Apache License 2.0 - http://www.apache.org/licenses/LICENSE-2.0
//

#ifndef __TweenEquations__
#define __TweenEquations__

#include <TweenEngine/equations/Quad.h>
#include <TweenEngine/equations/Linear.h>
#include <TweenEngine/equations/Back.h>
#include <TweenEngine/equations/Bounce.h>
#include <TweenEngine/equations/Circ.h>
#include <TweenEngine/equations/Cubic.h>
#include <TweenEngine/equations/Elastic.h>
#include <TweenEngine/equations/Expo.h>
#include <TweenEngine/equations/Quart.h>
#include <TweenEngine/equations/Quint.h>
#include <TweenEngine/equations/Sine.h>

namespace TweenEngine
{
    class TweenEquations
    {
    public:
        static TweenEquation &easeInQuad;
        static TweenEquation &easeOutQuad;
        static TweenEquation &easeInOutQuad;
        static TweenEquation &easeInOutLinear;
        static TweenEquation &easeInBack;
        static TweenEquation &easeOutBack;
        static TweenEquation &easeInOutBack;
        static TweenEquation &easeInBounce;
        static TweenEquation &easeOutBounce;
        static TweenEquation &easeInOutBounce;
        static TweenEquation &easeInCirc;
        static TweenEquation &easeOutCirc;
        static TweenEquation &easeInOutCirc;
        static TweenEquation &easeInCubic;
        static TweenEquation &easeOutCubic;
        static TweenEquation &easeInOutCubic;
        static TweenEquation &easeInElastic;
        static TweenEquation &easeOutElastic;
        static TweenEquation &easeInOutElastic;
        static TweenEquation &easeInExpo;
        static TweenEquation &easeOutExpo;
        static TweenEquation &easeInOutExpo;
        static TweenEquation &easeInQuart;
        static TweenEquation &easeOutQuart;
        static TweenEquation &easeInOutQuart;
        static TweenEquation &easeInQuint;
        static TweenEquation &easeOutQuint;
        static TweenEquation &easeInOutQuint;
        static TweenEquation &easeInSine;
        static TweenEquation &easeOutSine;
        static TweenEquation &easeInOutSine;
    };

}

#endif /* defined(__TweenEquations__) */
