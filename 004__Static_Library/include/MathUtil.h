#ifndef _MATHUTIL_H_
#define _MATHUTIL_H_

namespace MathUtil
{
	static float Max(float a, float b)
	{
		return (a > b) ? a : b;
	}

	static float Clamp(float min, float max, float value)
	{
		float clampedValue = value;
		if (clampedValue < min) clampedValue = min;
		if (clampedValue > max) clampedValue = max;
		return clampedValue;
	}
}

#endif // !_MATHUTIL_H_