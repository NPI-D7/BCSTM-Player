//
//  Tween.h
//
//  This code is derived from Universal Tween Engine
//  Licensed under Apache License 2.0 - http://www.apache.org/licenses/LICENSE-2.0
//

#ifndef __Tween__
#define __Tween__

#include <TweenEngine/Tweenable.h>
#include <TweenEngine/BaseTween.h>
#include <TweenEngine/Pool.h>
#include <TweenEngine/TweenEquation.h>
#include <TweenEngine/TweenPath.h>
#include <TweenEngine/TweenEquations.h>
#include <TweenEngine/TweenPaths.h>

namespace TweenEngine
{
    class TweenPool;
    class TweenPoolCallback;
    
    class Tween : public BaseTween
    {
        friend class TweenPoolCallback;
        
    private:
        static int combinedAttrsLimit;
        static int waypointsLimit;
        
        // Main
        Tweenable *targetObj;
        int type;
        TweenEquation *equation;
        TweenPath *pathAlgorithm;

        // General
        bool isFrom;
        bool isRelative;
        int combinedAttrsCnt;
        int waypointsCnt;

        // Values
        float* startValues;
        float* targetValues;
        float* waypoints;
        
        // Buffers
        float *accessorBuffer;
        int accessorBufferSize;
        float *pathBuffer;
        int pathBufferSize;

        //static TweenPoolCallback *poolCallback;
        static TweenPool &pool;
        
        void setup(Tweenable *target, int tweenType, float duration);
        
    protected:
        virtual void reset();
        virtual void forceStartValues();
        virtual void forceEndValues();
        virtual void initializeOverride();
        virtual void updateOverride(int step, int lastStep, bool isIterationStep, float delta);
        
    public:
        static const int ACCESSOR_READ = 0;
        static const int ACCESSOR_WRITE = 1;

        static void setCombinedAttributesLimit(int limit);
        static void setWaypointsLimit(int limit);
        static const char *getVersion();
        
        static int getPoolSize();
        static void ensurePoolCapacity(int minCapacity);

        static Tween &to(Tweenable& target, int tweenType, float duration);
        static Tween &from(Tweenable& target, int tweenType, float duration);
        static Tween &set(Tweenable& target, int tweenType);
        static Tween &call(TweenCallback &callback);
        static Tween &mark();
        
        Tween();
        ~Tween();

        virtual int getTweenCount();
        virtual int getTimelineCount();
        
        virtual Tween &build();
        virtual void free();
        
        Tween &ease(TweenEquation &easeEquation);
        Tween &target(float targetValue);
        Tween &target(float targetValue1, float targetValue2);
        Tween &target(float targetValue1, float targetValue2, float targetValue3);
        Tween &target(float *targetValues, int len);
        Tween &targetRelative(float targetValue);
        Tween &targetRelative(float targetValue1, float targetValue2);
        Tween &targetRelative(float targetValue1, float targetValue2, float targetValue3);
        Tween &targetRelative(float *targetValues, int len);
        Tween &waypoint(float targetValue);
        Tween &waypoint(float targetValue1, float targetValue2);
        Tween &waypoint(float targetValue1, float targetValue2, float targetValue3);
        Tween &waypoint(float *targetValues, int len);
        Tween &path(TweenPath &path);
        int getType();
        TweenEquation *getEasing();
        float *getTargetValues();
        int getCombinedAttributesCount();

        
    };
}

#endif /* defined(__Tween__) */
