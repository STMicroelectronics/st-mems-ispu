/**
  ******************************************************************************
  * @file
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

#ifndef ISPU_DSP_H
#define ISPU_DSP_H

#include <stdint.h>

/**
 * @defgroup basic_math Basic math
 * @details This group provides basic math on vectors.
 * @{
 */

/**
 * @brief Floating-point vector absolute value.
 * @param[in] src Pointer to the input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_abs_f32(const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector addition.
 * @param[in] src0 Pointer to the first input vector
 * @param[in] src1 Pointer to the second input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_add_f32(const float *src0, const float *src1, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector clipping.
 * @param[in] src Pointer to the input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] low Lower bound value
 * @param[in] high Higher bound value
 * @param[in] block_size Number of samples in each vector
 */
void ispu_clip_f32(const float *src, float *dst, float low, float high, uint32_t block_size);

/**
 * @brief Floating-point vector element-wise mutiplication.
 * @param[in] src0 Pointer to the first input vector
 * @param[in] src1 Pointer to the second input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_mult_f32(const float *src0, const float *src1, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector negation.
 * @param[in] src Pointer to the input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_negate_f32(const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector offset addition.
 * @param[in] src Pointer to the input vector
 * @param[in] offset Offset value
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_offset_f32(const float *src, float offset, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector scalar multiplication.
 * @param[in] src Pointer to the input vector
 * @param[in] scale Scalar value
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_scale_f32(const float *src, float scale, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector subtraction.
 * @param[in] src0 Pointer to the first input vector
 * @param[in] src1 Pointer to the second input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_sub_f32(const float *src0, const float *src1, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector element-wise squaring.
 * @param[in] src Pointer to the input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_square_f32(const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point vector square root.
 * @param[in] src Pointer to the input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_vsqrt_f32(const float *src, float *dst, uint32_t block_size);

/** @} */

/**
 * @defgroup complex_math Complex math
 * @details This group provides complex math on vectors.
 *          Each complex value of the vectors is composed by two floating-point values, the first one representing the real part and the second one representing the
 *          imaginary part.
 * @{
 */

/**
 * @brief Floating-point complex vector element-wise magnitude.
 * @param[in] src Pointer to the input complex vector
 * @param[out] dst Pointer to the output real vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_cmplx_mag_f32(const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point complex vector element-wise squared magnitude.
 * @param[in] src Pointer to the input complex vector
 * @param[out] dst Pointer to the output real vector
 * @param[in] block_size Number of samples in each vector
 */
void ispu_cmplx_mag_squared_f32(const float *src, float *dst, uint32_t block_size);

/** @} */

/**
 * @defgroup filtering Filtering
 * @details This group provides digital filters.
 * @{
 */

/**
 * @brief Floating-point biquad cascade filter (direct form I) instance structure.
 */
struct ispu_biquad_casd_df1_inst_f32 {
	uint8_t num_stages; /**< Number of biquad stages. */
	float *state; /**< Pointer to the state array (length must be num_stages * 4). */
	const float *coeffs; /**< Pointer to the coefficients array (length must num_stages * 5).
	                          The array must contain, in order, b0, b1, b2, -a1, -a2 for each biquad stage. */
};

/**
 * @brief Floating-point biquad cascade filter (direct form I) initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] num_stages Number of biquad stages
 * @param[in] coeffs Pointer to the coefficients array
 * @param[in] state Pointer to the state array
 */
void ispu_biquad_cascade_df1_init_f32(struct ispu_biquad_casd_df1_inst_f32 *inst, uint8_t num_stages, const float *coeffs, float *state);

/**
 * @brief Floating-point biquad cascade filter (direct form I) processing function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] src Pointer to the input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples to process
 */
void ispu_biquad_cascade_df1_f32(const struct ispu_biquad_casd_df1_inst_f32 *inst, const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point biquad cascade filter (direct form II transposed) instance structure.
 */
struct ispu_biquad_casd_df2T_inst_f32 {
	uint8_t num_stages; /**< Number of biquad stages. */
	float *state; /**< Pointer to the state array (length must be num_stages * 2). */
	const float *coeffs; /**< Pointer to the coefficients array (length must num_stages * 5).
	                          The array must contain, in order, b0, b1, b2, -a1, -a2 for each biquad stage. */
};

/**
 * @brief Floating-point biquad cascade filter (direct form II transposed) initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] num_stages Number of biquad stages
 * @param[in] coeffs Pointer to the coefficients array
 * @param[in] state Pointer to the state array
 */
void ispu_biquad_cascade_df2T_init_f32(struct ispu_biquad_casd_df2T_inst_f32 *inst, uint8_t num_stages, const float *coeffs, float *state);

/**
 * @brief Floating-point biquad cascade filter (direct form II transposed) processing function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] src Pointer to the input vector
 * @param[out] dst Pointer to the output vector
 * @param[in] block_size Number of samples to process
 */
void ispu_biquad_cascade_df2T_f32(const struct ispu_biquad_casd_df2T_inst_f32 *inst, const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point FIR filter instance structure.
 */
struct ispu_fir_instance_f32 {
	uint16_t num_taps; /**< Number of taps/coefficients. */
	float *state; /**< Pointer to the state array (length must be num_taps + block_size - 1). */
	const float *coeffs; /**< Pointer to the coefficients array (length must be num_taps). */
	void (*impl)(const struct ispu_fir_instance_f32 *inst, const float *src, float *dst, uint32_t block_size); /**< Pointer to the implementation function. */
};

/**
 * @brief Floating-point FIR filter initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] num_taps Number of taps/coefficients
 * @param[in] coeffs Pointer to the coefficients array
 * @param[in] state Pointer to the state array
 * @param[in] block_size Maximum number of samples to process at each call
 */
void ispu_fir_init_f32(struct ispu_fir_instance_f32 *inst, uint16_t num_taps, const float *coeffs, float *state, uint32_t block_size);

/**
 * @brief Floating-point FIR filter processing function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] src Pointer to the input vector
 * @param[in] dst Pointer to the output vector
 * @param[in] block_size Number of samples to process
 */
void ispu_fir_f32(const struct ispu_fir_instance_f32 *inst, const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point FIR filter (optimized) instance structure.
 * @details The FIR filter (optimized) internally uses the hardware circular buffer accelerator. The accelerator registers must be saved and restored by the user if the
 *          execution of the filter is interrupted and the accelerator is needed for other purposes during the interruption.
 */
struct ispu_fir_instance_f32_opt {
	uint16_t num_taps; /**< Number of taps/coefficients (must be lower than or equal to 1024). */
	float *state; /**< Pointer to the state array (must be aligned to 4096 bytes and its length must be num_taps). */
	float *state_curr; /**< Pointer to the current element of the state array. */
	const float *coeffs; /**< Pointer to the coefficients array (length must be num_taps). */
	void (*impl)(const struct ispu_fir_instance_f32_opt *inst, const float *src, float *dst, uint32_t block_size); /**< Pointer to the implementation function. */
};

/**
 * @brief Floating-point FIR filter (optimized) initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] num_taps Number of taps/coefficients
 * @param[in] coeffs Pointer to the coefficients array
 * @param[in] state Pointer to the state array
 * @return 0 on success or -1 on failure
 */
int ispu_fir_init_f32_opt(struct ispu_fir_instance_f32_opt *inst, uint16_t num_taps, const float *coeffs, float *state);

/**
 * @brief Floating-point FIR filter (optimized) processing function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] src Pointer to the input vector
 * @param[in] dst Pointer to the output vector
 * @param[in] block_size Number of samples to process
 */
void ispu_fir_f32_opt(const struct ispu_fir_instance_f32_opt *inst, const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point FIR decimator instance structure.
 */
struct ispu_fir_decimate_instance_f32 {
	uint8_t dec; /**< Decimation factor (must be an integer divisor of block_size). */
	uint16_t num_taps; /**< Number of taps/coefficients. */
	float *state; /**< Pointer to the state array (length must be num_taps + block_size - 1). */
	const float *coeffs; /**< Pointer to the coefficients array (length must be num_taps). */
	void (*impl)(const struct ispu_fir_decimate_instance_f32 *inst, const float *src, float *dst, uint32_t block_size); /**< Pointer to the implementation function. */
};

/**
 * @brief Floating-point FIR decimator initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] num_taps Number of taps/coefficients
 * @param[in] dec Decimation factor
 * @param[in] coeffs Pointer to the coefficients array
 * @param[in] state Pointer to the state array
 * @param[in] block_size Number of samples to process at each call
 */
int ispu_fir_decimate_init_f32(struct ispu_fir_decimate_instance_f32 *inst, uint16_t num_taps, uint8_t dec, const float *coeffs, float *state, uint32_t block_size);

/**
 * @brief Floating-point FIR decimator processing function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] src Pointer to the input vector
 * @param[in] dst Pointer to the output vector
 * @param[in] block_size Number of samples to process
 */
void ispu_fir_decimate_f32(const struct ispu_fir_decimate_instance_f32 *inst, const float *src, float *dst, uint32_t block_size);

/**
 * @brief Floating-point FIR decimator (optimized) instance structure.
 * @details The FIR decimator (optimized) internally uses the hardware circular buffer accelerator. The accelerator registers must be saved and restored by the user if the
 *          execution of the filter is interrupted and the accelerator is needed for other purposes during the interruption.
 */
struct ispu_fir_decimate_instance_f32_opt {
	uint8_t dec; /**< Decimation factor (must be an integer divisor of block_size). */
	uint16_t num_taps; /**< Number of taps/coefficients (must be lower than or equal to 1024). */
	float *state; /**< Pointer to the state array (must be aligned to 4096 bytes and its length must be num_taps). */
	float *state_curr; /**< Pointer to the current element of the state array. */
	const float *coeffs; /**< Pointer to the coefficients array (length must be num_taps). */
	void (*impl)(const struct ispu_fir_decimate_instance_f32_opt *inst, const float *src, float *dst, uint32_t block_size); /**< Pointer to the implementation function. */
};

/**
 * @brief Floating-point FIR decimator (optimized) initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] num_taps Number of taps/coefficients
 * @param[in] dec Decimation factor
 * @param[in] coeffs Pointer to the coefficients array
 * @param[in] state Pointer to the state array
 * @param[in] block_size Number of samples to process at each call
 * @return 0 on success or -1 on failure
 */
int ispu_fir_decimate_init_f32_opt(struct ispu_fir_decimate_instance_f32_opt *inst, uint16_t num_taps, uint8_t dec, const float *coeffs, float *state, uint32_t block_size);

/**
 * @brief Floating-point FIR decimator (optimized) processing function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] src Pointer to the input vector
 * @param[in] dst Pointer to the output vector
 * @param[in] block_size Number of samples to process
 */
void ispu_fir_decimate_f32_opt(const struct ispu_fir_decimate_instance_f32_opt *inst, const float *src, float *dst, uint32_t block_size);

/** @} */

/**
 * @defgroup statistics Statistics
 * @details This group provides statistical features.
 * @{
 */

/**
 * @brief Floating-point vector maximum of the asbolute values.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[out] idx Index of the result
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_absmax_f32(const float *src, float *res, uint32_t *idx, uint32_t block_size);

/**
 * @brief Floating-point vector maximum of the asbolute values.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_absmax_no_idx_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector minimum of the asbolute values.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[out] idx Index of the result
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_absmin_f32(const float *src, float *res, uint32_t *idx, uint32_t block_size);

/**
 * @brief Floating-point vector minimum of the asbolute values.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_absmin_no_idx_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector accumulation.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_accumulate_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector maximum.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[out] idx Index of the result
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_max_f32(const float *src, float *res, uint32_t *idx, uint32_t block_size);

/**
 * @brief Floating-point vector maximum.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_max_no_idx_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector mean.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_mean_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector minimum.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[out] idx Index of the result
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_min_f32(const float *src, float *res, uint32_t *idx, uint32_t block_size);

/**
 * @brief Floating-point vector minimum.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_min_no_idx_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector power (sum of the squared values).
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_power_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector RMS (root mean square).
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_rms_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector standard deviation.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_std_f32(const float *src, float *res, uint32_t block_size);

/**
 * @brief Floating-point vector variance.
 * @param[in] src Pointer to the input vector
 * @param[out] res Pointer to the result value
 * @param[in] block_size Number of samples in the input vector
 */
void ispu_var_f32(const float *src, float *res, uint32_t block_size);

/** @} */

/**
 * @defgroup transform Transform
 * @details This group provides tranforms.
 * @{
 */

/**
 * @brief Floating-point RFFT (real fast Fourier transform) instance structure.
 */
struct ispu_rfft_instance_f32 {
	float *twiddle_table; /**< Pointer to the twiddle table. */
	uint16_t fft_len; /**< Length of the FFT. */
	uint8_t fft_n_elem_bra; /**< Length of the FFT coded for ISPU_FFT_N_ELEM_BRA register. */
};

/**
 * @brief Floating-point RFFT (real fast Fourier transform) initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] fft_len Length of the FFT (32, 64, 128, 256, 512, 1024, or 2048)
 * @return 0 on success or -1 on failure
 */
int ispu_rfft_init_f32(struct ispu_rfft_instance_f32 *inst, uint16_t fft_len);

/**
 * @brief Floating-point RFFT (real fast Fourier transform) processing function.
 * @details The output is the first half of the spectrum represented as complex numbers.
 *          Each complex number is composed by two floating-point values, the first one representing the real part and the second one representing the imaginary part.
 *          The first complex number in the output contains the DC bin in the real part and the Nyquist bin in the imaginary part.
 * @param[in] inst Pointer to the instance structure
 * @param[in,out] data Pointer to the input/output data.
 *                     It must be aligned as follows:
 *                      - fft_len = 32 requires alignment to 128 bytes
 *                      - fft_len = 64 requires alignment to 256 bytes
 *                      - fft_len = 128 requires alignment to 512 bytes
 *                      - fft_len = 256 requires alignment to 1024 bytes
 *                      - fft_len = 512 requires alignment to 2048 bytes
 *                      - fft_len = 1024 requires alignment to 4096 bytes
 *                      - fft_len = 2048 requires alignment to 8192 bytes
 */
void ispu_rfft_f32(struct ispu_rfft_instance_f32 *inst, float *data);

/**
 * @brief Floating-point IRFFT (inverse real fast Fourier transform) processing function.
 * @details The output is a real signal corresponding to the inverse of the input half spectrum.
 *          The input must be in the same format as the output of the ispu_rfft_f32 function.
 * @param[in] inst Pointer to the instance structure
 * @param[in,out] data Pointer to the input/output data.
 *                     It must be aligned as follows:
 *                      - fft_len = 32 requires alignment to 128 bytes
 *                      - fft_len = 64 requires alignment to 256 bytes
 *                      - fft_len = 128 requires alignment to 512 bytes
 *                      - fft_len = 256 requires alignment to 1024 bytes
 *                      - fft_len = 512 requires alignment to 2048 bytes
 *                      - fft_len = 1024 requires alignment to 4096 bytes
 *                      - fft_len = 2048 requires alignment to 8192 bytes
 */
void ispu_irfft_f32(struct ispu_rfft_instance_f32 *inst, float *data);

/**
 * @brief Floating-point real Hilbert transform instance structure.
 */
struct ispu_hilbert_instance_f32 {
	struct ispu_rfft_instance_f32 rfft_inst; /**< Internal RFFT instance */
	uint16_t hilbert_len; /**< Length of the Hilbert transform. */
};

/**
 * @brief Floating-point real Hilbert transform initialization function.
 * @param[in] inst Pointer to the instance structure
 * @param[in] hilbert_len Length of the Hilbert transform (32, 64, 128, 256, 512, 1024, or 2048)
 * @return 0 on success or -1 on failure
 */
int ispu_hilbert_init_f32(struct ispu_hilbert_instance_f32 *inst, uint16_t hilbert_len);

/**
 * @brief Floating-point real Hilbert transform processing function.
 * @details The output is the Hilbert transform of the real input signal, resulting in hilbert_len complex numbers representing the analytic signal.
 *          Each complex number is composed by two floating-point values, the first one representing the real part and the second one representing the imaginary part.
 *          The real part represents the in-phase component while the imaginary part represents the quadrature component of the analytic signal.
 * @param[in] inst Pointer to the instance structure
 * @param[in] src Pointer to the input vector.
 *                     It must be aligned as follows:
 *                      - hilbert_len = 32 requires alignment to 128 bytes
 *                      - hilbert_len = 64 requires alignment to 256 bytes
 *                      - hilbert_len = 128 requires alignment to 512 bytes
 *                      - hilbert_len = 256 requires alignment to 1024 bytes
 *                      - hilbert_len = 512 requires alignment to 2048 bytes
 *                      - hilbert_len = 1024 requires alignment to 4096 bytes
 *                      - hilbert_len = 2048 requires alignment to 8192 bytes
 * @param[out] dst Pointer to the output vector. Must be different from src pointer and must have double the size of src to accomodate for complex numbers.
 */
void ispu_hilbert_f32(struct ispu_hilbert_instance_f32 *inst, float *src, float *dst);

/** @} */

/**
 * @defgroup window Window
 * @details This group provides window functions generation.
 * @{
 */

/**
 * @brief Floating-point Hamming window generator.
 * @param[in] dst Pointer to the output vector
 * @param[in] block_size Number of samples in the output vector
 */
void ispu_hamming_f32(float *dst, uint32_t block_size);

/**
 * @brief Floating-point Hanning window generator.
 * @param[in] dst Pointer to the output vector
 * @param[in] block_size Number of samples in the output vector
 */
void ispu_hanning_f32(float *dst, uint32_t block_size);

/** @} */

#endif

