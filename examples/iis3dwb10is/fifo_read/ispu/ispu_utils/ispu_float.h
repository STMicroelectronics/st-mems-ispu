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

#ifndef ISPU_FLOAT_H
#define ISPU_FLOAT_H

#include <stdint.h>
#include "reg_map.h"

#if FAST_FLOAT || FAST_DIVF
/**
  @brief         This function returns the quotient of a and b; that is, a/b.
  @details       It is based on the fast inverse_square root with 4 newton iterations.
  @param[in]     a        float numerator
  @param[in]     b        float denominator
  @return        a/b
 */
float __divsf3(float a, float b);
#endif

#if FAST_FLOAT || FAST_FLOAT_CMP
/**
  @brief         This function returns a value greater than zero if a is strictly greater than b.
  @param[in]     a        float value a
  @param[in]     b        float value b
  @return                 bool value
 */
int __gtsf2(float a, float b);

/**
  @brief         This function returns a value greater than or equal to zero if a is greater than or equal to b.
  @param[in]     a        float value a
  @param[in]     b        float value b
  @return                 bool value
 */
int __gesf2(float a, float b);

/**
  @brief         This function returns zero if and a and b are equal.
  @param[in]     a        float value a
  @param[in]     b        float value b
  @return                 bool value
 */
int __eqsf2(float a, float b);

/**
  @brief         This function returns a nonzero value if a and b are unequal.
  @param[in]     a        float value a
  @param[in]     b        float value b
  @return                 bool value
 */
int __nesf2(float a, float b);

/**
  @brief         This function returns a value less than or equal to zero if a is less than or equal to b.
  @param[in]     a        float value a
  @param[in]     b        float value b
  @return                 bool value
 */
int __lesf2(float a, float b);

/**
  @brief         This function returns a value less than zero if a is strictly less than b.
  @param[in]     a        float value a
  @param[in]     b        float value b
  @return                 bool value
 */
int __ltsf2(float a, float b);
#endif

/**
  @brief         This function converts input, a signed integer, to floating point.
  @param[in]     input        signed integer
  @return                     converted float
 */
float __floatsisf(int input);

/**
  @brief         This function converts input, an unsigned integer, to floating point.
  @param[in]     input        unsigned integer
  @return                     converted float
 */
float __floatunsisf(unsigned int input);

/**
  @brief         This function converts input, a floating point, to signed integer.
  @param[in]     input          float
  @return                       converted signed integer
 */
int __fixsfsi(float input);

/**
  @brief         Signed integer 20 bit to float
  @param[in]     input          signed integer 20 bit
  @return                       converted float
 */
__attribute__ ((always_inline, optimize("-O2")))
static inline float sint20_to_float(int32_t input)
{
	union {
		float f_val;
		unsigned int u_val;
	} out;

	out.u_val = input;

	__asm__(
		"addw   %0, #0x4b200000\n" // 2^23 float + 524288, add 2^23.0f and add int value, shift value (0: 1048575)
		"fsubs  %0, #0x4b200000\n" // sub float (2^23 + 524288), shift down to 0
		: "+r"(out.u_val)
	);

	return out.f_val;
}

#if FAST_FLOAT || FAST_SQRTF
/**
  @brief         This function returns the sqrt of a floting input using hardware computation.
  @param[in]     input        float input value
  @return        floating inverse quare root
 */
__attribute__ ((always_inline, optimize("-O2")))
static inline float sqrtf(float input)
{
	float output;

	__asm__(
		"stw	[%2], %1\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ldw	%0, [%3]\n"
		: "=r"(output) : "r"(input), "i" ((ISPU_SQRT_I)), "i" ((ISPU_SQRT_O))
	);

	return output;
}
#endif

#endif

