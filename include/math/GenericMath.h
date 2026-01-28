#pragma once

#include <cmath>
#include <assert.h>
#include <functional>

typedef signed long long int64;
typedef unsigned __int64 uint64;
typedef signed int int32;
typedef unsigned int uint32;

typedef unsigned short uint16;
typedef signed short int16;

typedef char int8;
typedef unsigned char uint8;

#include <math/Types.h>

#undef PI
#define PI  (3.14159265358979323846f)
#define INF (1e8)

#define ABS_MASK (1u << 31)

//#define GCC_ALIGN(size) __attribute__ ((aligned (size)))
#define MVS_ALIGN(size) __declspec(align(size))

#include <math/SSEMath.h>
//Static math functions
struct SMath
{
	/* http://www.lomont.org/papers/2003/InvSqrt.pdf */

	//static inline constexpr float Sqrt(float x)
	//{
	//	float xhalf = 0.5f*x;
	//	int i = *(int*)&x;       // get bits for floating value
	//	i = 0x5f375a86 - (i >> 1);  // gives initial guess y0
	//	x = *(float*)&i;         // convert bits back to float
	//	x = x * (1.5f - xhalf * x*x);  // Newton step, repeating increases accuracy
	//	return x;
	//}

	//static inline float Sqrt(float x)
	//{
	//	const float xhalf = 0.5f*x;

	//	union // get bits for floating value
	//	{
	//		float x;
	//		int i;
	//	} u;
	//	u.x = x;
	//	u.i = 0x5f3759df - (u.i >> 1);  // gives initial guess y0
	//	return x * u.x*(1.5f - xhalf * u.x*u.x);// Newton step, repeating increases accuracy 
	//}

	static inline float Sqrt(float x)
	{
		//union
		//{
		//	int i;
		//	float x;
		//} u;
		//u.x = x;
		//u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

		//// Two Babylonian Steps (simplified from:)
		//// u.x = 0.5f * (u.x + x/u.x);
		//// u.x = 0.5f * (u.x + x/u.x);
		//u.x = u.x + x / u.x;
		//u.x = 0.25f*u.x + x / u.x;

		//return u.x;
		return sqrt(x);
	}

	template< class T >
	static inline constexpr T Abs(const T A)
	{
		return (A >= (T)0) ? A : -A;
	}


	template< class T >
	static inline constexpr T Max(const T A, const T B)
	{
		return (A < B) ? B : A;
	}


	template< class T >
	static inline constexpr T Min(const T A, const T B)
	{
		return (A > B) ? B : A;
	}

	static inline float Sin(float Value) { return sin(Value); }
	static inline float Asin(float Value) { return asin((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
	static inline float Sinh(float Value) { return sinh(Value); }
	static inline float Cos(float Value) { return cos(Value); }
	static inline float Acos(float Value) { return acos((Value < -1.f) ? -1.f : ((Value < 1.f) ? Value : 1.f)); }
	static inline float Tan(float Value) { return tan(Value); }
	static inline float Atan(float Value) { return atan(Value); }
	static inline float Cotan(float Value) { return tan((PI * 0.5f) - Value); }
	
	template <class T>
	static inline constexpr float Floor(T Value) { return floor(Value); }

	template <class T>
	static inline constexpr float Ceil(T Value) { return ceil(Value); }


	// Returns e^Value
	static inline float Exp(float Value) { return exp(Value); }
	
	// Returns 2^Value
	static inline float Exp2(float Value) { return pow(2.f, Value);  } /*exp2f(Value);*/
	
	static inline float Loge(float Value) { return log(Value); }
	static inline float LogX(float Base, float Value) { return Loge(Value) / Loge(Base); }
	
	// 1.0 / Loge(2) = 1.4426950f
	static inline float Log2(float Value) { return Loge(Value) * 1.4426950f; }

	static inline uint64 HighestPowerOf2(uint64 Value) {
		if (Value == 0) return 0;

		uint64 Power = 1;
		while (Power << 1 <= Value) {
			Power <<= 1;
		}

		return Power;
	}

};