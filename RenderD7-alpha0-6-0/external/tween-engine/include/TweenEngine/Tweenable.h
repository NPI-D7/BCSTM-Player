#ifndef __Tweenable__
#define __Tweenable__

namespace TweenEngine {
	class Tweenable {
	public:
		virtual int getValues(int tweenType, float *returnValues) = 0;
		virtual void setValues(int tweenType, float *newValues) = 0;
	};
}

#endif
