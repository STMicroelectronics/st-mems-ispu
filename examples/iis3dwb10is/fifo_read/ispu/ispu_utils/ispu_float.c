/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "ispu_float.h"

#include <math.h>

#if FAST_FLOAT || FAST_DIVF
union float_converter_tmp {
	float f_val;
	unsigned int u_val;
};

/**
 * float input accepts normal only
 */
static float isqrtf32_iter_norm(float number, int iterations)
{
	long i, idx;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y  = number;
	i  = *(long *)&y;                         // evil floating point bit level hacking
	i  = 0x5f3759df - (i >> 1);               // what the fuck?
	y  = *(float *) &i;

	for (idx = 0; idx < iterations; idx++)
		y  = y * (threehalfs - (x2 * y * y)); // idx iteration

	return y;
}

/**
 * float input accepts normal and subnormal
 */
static float isqrtf32_iter(float number, int iterations)
{
	float output;
	union float_converter_tmp conv_input;

	conv_input.f_val = number;

	// check if input is subnormal floating (exp=0)
	if ((conv_input.u_val & 0x7f800000) == 0) {
		number = number * (float)0x65078678; // correction factor = 2^22
		output = isqrtf32_iter_norm(number, iterations);
		output = output * (float)0x523a43b7; // correction factor = 2^11
	} else {
		// normal floating input
		output = isqrtf32_iter_norm(number, iterations);
	}

	return output;
}

static float ispu_signf32(float x)
{
	union float_converter_tmp conv_input;

	conv_input.f_val = x;
	conv_input.u_val = (conv_input.u_val & 0x80000000U) | 0x3f800000U;

	return conv_input.f_val;
}

static float ispu_invf32(float x, int iterations)
{
	// compute the inverse of the absolute value and adjust the sign at the end
	float tmp = fabsf(x);
	float r = isqrtf32_iter(tmp, iterations);

	return r * r * ispu_signf32(x);
}

static float ispu_divf32_iter(float a, float b, int iterations)
{
	return a * ispu_invf32(b, iterations);
}

__attribute__ ((visibility("default"), externally_visible))
float __divsf3(float a, float b)
{
	return ispu_divf32_iter(a, b, 4);
}
#endif

#if FAST_FLOAT || FAST_FLOAT_CMP
#pragma GCC diagnostic ignored "-Wreturn-type"
__attribute__ ((noclone, optimize("-Os")))
static int ispu_cmpf32(float a, float b)
{
	// avoid unused warnings
	(void)a; (void)b;

	__asm__ volatile("fsubs %r0, %r1"); // perform comparison
	__asm__ volatile("cmpw %r0, 0x80000000"); // zero negative
	__asm__ volatile("jpdeq .Lispu_cmpf32_ret_0"); // equal? return 0
	__asm__ volatile("cmpw %r0, #0"); // zero positive
	__asm__ volatile("jpdeq .Lispu_cmpf32_ret_0"); // equal? return 0
	__asm__ volatile("jpdng .Lispu_cmpf32_ret_minus1"); // r0 < r1: return -1

	__asm__ volatile("movw %r0, #1"); // r0 > r1? return +1
	__asm__ volatile("ret");

	__asm__ volatile(".Lispu_cmpf32_ret_0:");
	__asm__ volatile("movw %r0, #0");
	__asm__ volatile("ret");

	__asm__ volatile(".Lispu_cmpf32_ret_minus1:");
	__asm__ volatile("movw %r0, #-1");
}
#pragma GCC diagnostic pop

__attribute__ ((visibility("default"), externally_visible, alias("ispu_cmpf32"))) int __gtsf2(float a, float b);
__attribute__ ((visibility("default"), externally_visible, alias("ispu_cmpf32"))) int __gesf2(float a, float b);
__attribute__ ((visibility("default"), externally_visible, alias("ispu_cmpf32"))) int __eqsf2(float a, float b);
__attribute__ ((visibility("default"), externally_visible, alias("ispu_cmpf32"))) int __nesf2(float a, float b);
__attribute__ ((visibility("default"), externally_visible, alias("ispu_cmpf32"))) int __lesf2(float a, float b);
__attribute__ ((visibility("default"), externally_visible, alias("ispu_cmpf32"))) int __ltsf2(float a, float b);
#endif

