#pragma once

////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2017 Nicholas Frechette & Animation Compression Library contributors
// Copyright (c) 2018 Nicholas Frechette & Realtime Math contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "rtm/math.h"
#include "rtm/scalard.h"
#include "rtm/impl/compiler_utils.h"
#include "rtm/impl/memory_utils.h"
#include "rtm/impl/vector_common.h"

RTM_IMPL_FILE_PRAGMA_PUSH

namespace rtm
{
	//////////////////////////////////////////////////////////////////////////
	// Setters, getters, and casts
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned vector4 from memory.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_load(const double* input) RTM_NO_EXCEPT
	{
		return vector_set(input[0], input[1], input[2], input[3]);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an input scalar from memory into the [x] component and sets the [yzw] components to zero.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_load1(const double* input) RTM_NO_EXCEPT
	{
		return vector_set(input[0], 0.0, 0.0, 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned vector2 from memory and sets the [zw] components to zero.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_load2(const double* input) RTM_NO_EXCEPT
	{
		return vector_set(input[0], input[1], 0.0, 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned vector3 from memory and sets the [w] component to zero.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_load3(const double* input) RTM_NO_EXCEPT
	{
		return vector_set(input[0], input[1], input[2], 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned vector4 from memory.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_load(const float4d* input) RTM_NO_EXCEPT
	{
		return vector_set(input->x, input->y, input->z, input->w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned vector2 from memory and sets the [zw] components to zero.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_load2(const float2d* input) RTM_NO_EXCEPT
	{
		return vector_set(input->x, input->y, 0.0, 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an unaligned vector3 from memory and sets the [w] component to zero.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_load3(const float3d* input) RTM_NO_EXCEPT
	{
		return vector_set(input->x, input->y, input->z, 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Loads an input scalar from memory into the [xyzw] components.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_broadcast(const double* input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		const __m128d value = _mm_load1_pd(input);
		return vector4d{ value, value };
#else
		return vector_set(*input);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Casts a quaternion to a vector4.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d quat_to_vector(const quatd& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ input.xy, input.zw };
#else
		return vector4d{ input.x, input.y, input.z, input.w };
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Casts a vector4 float32 variant to a float64 variant.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_cast(const vector4f& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_cvtps_pd(input), _mm_cvtps_pd(_mm_shuffle_ps(input, input, _MM_SHUFFLE(3, 2, 3, 2))) };
#elif defined(RTM_NEON_INTRINSICS)
		return vector4d{ double(vgetq_lane_f32(input, 0)), double(vgetq_lane_f32(input, 1)), double(vgetq_lane_f32(input, 2)), double(vgetq_lane_f32(input, 3)) };
#else
		return vector4d{ double(input.x), double(input.y), double(input.z), double(input.w) };
#endif
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_get_x
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(input.xy);
#else
				return input.x;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ input.xy };
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the vector4 [x] component.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_get_x vector_get_x(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_get_x{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_get_y
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(_mm_shuffle_pd(input.xy, input.xy, 1));
#else
				return input.y;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ _mm_shuffle_pd(input.xy, input.xy, 1) };
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the vector4 [y] component.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_get_y vector_get_y(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_get_y{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_get_z
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(input.zw);
#else
				return input.z;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ input.zw };
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the vector4 [z] component.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_get_z vector_get_z(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_get_z{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_get_w
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				return _mm_cvtsd_f64(_mm_shuffle_pd(input.zw, input.zw, 1));
#else
				return input.w;
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				return scalard{ _mm_shuffle_pd(input.zw, input.zw, 1) };
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the vector4 [w] component.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_get_w vector_get_w(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_get_w{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		template<mix4 component>
		struct vector4d_vector_get_component_static
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const mix4 xyzw = mix4(int(component) % 4);
				if (rtm_impl::static_condition<xyzw == mix4::x>::test())
					return vector_get_x(input);
				else if (rtm_impl::static_condition<xyzw == mix4::y>::test())
					return vector_get_y(input);
				else if (rtm_impl::static_condition<xyzw == mix4::z>::test())
					return vector_get_z(input);
				else
					return vector_get_w(input);
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const mix4 xyzw = mix4(int(component) % 4);
				if (rtm_impl::static_condition<xyzw == mix4::x>::test())
					return vector_get_x(input);
				else if (rtm_impl::static_condition<xyzw == mix4::y>::test())
					return vector_get_y(input);
				else if (rtm_impl::static_condition<xyzw == mix4::z>::test())
					return vector_get_z(input);
				else
					return vector_get_w(input);
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the vector4 desired component.
	//////////////////////////////////////////////////////////////////////////
	template<mix4 component>
	constexpr rtm_impl::vector4d_vector_get_component_static<component> vector_get_component(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_get_component_static<component>{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_get_component
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const mix4 xyzw = mix4(int(component) % 4);
				if (xyzw == mix4::x)
					return vector_get_x(input);
				else if (xyzw == mix4::y)
					return vector_get_y(input);
				else if (xyzw == mix4::z)
					return vector_get_z(input);
				else
					return vector_get_w(input);
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const mix4 xyzw = mix4(int(component) % 4);
				if (xyzw == mix4::x)
					return vector_get_x(input);
				else if (xyzw == mix4::y)
					return vector_get_y(input);
				else if (xyzw == mix4::z)
					return vector_get_z(input);
				else
					return vector_get_w(input);
			}
#endif

			vector4d input;
			mix4 component;
			int padding[3];
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the vector4 desired component.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_get_component vector_get_component(const vector4d& input, mix4 component) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_get_component{ input, component, { 0 } };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the smallest component in the input vector as a scalar.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_get_min_component vector_get_min_component(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_get_min_component{ input };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the largest component in the input vector as a scalar.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_get_max_component vector_get_max_component(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_get_max_component{ input };
	}

	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [x] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_x(const vector4d& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_move_sd(input.xy, _mm_set_sd(lane_value)), input.zw };
#else
		return vector4d{ lane_value, input.y, input.z, input.w };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [x] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_x(const vector4d& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return vector4d{ _mm_move_sd(input.xy, lane_value.value), input.zw };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [y] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_y(const vector4d& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_shuffle_pd(input.xy, _mm_set_sd(lane_value), 0), input.zw };
#else
		return vector4d{ input.x, lane_value, input.z, input.w };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [y] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_y(const vector4d& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return vector4d{ _mm_shuffle_pd(input.xy, lane_value.value, 0), input.zw };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [z] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_z(const vector4d& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ input.xy, _mm_move_sd(input.zw, _mm_set_sd(lane_value)) };
#else
		return vector4d{ input.x, input.y, lane_value, input.w };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [z] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_z(const vector4d& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return vector4d{ input.xy, _mm_move_sd(input.zw, lane_value.value) };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [w] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_w(const vector4d& input, double lane_value) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ input.xy, _mm_shuffle_pd(input.zw, _mm_set_sd(lane_value), 0) };
#else
		return vector4d{ input.x, input.y, input.z, lane_value };
#endif
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Sets the vector4 [w] component and returns the new value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_set_w(const vector4d& input, const scalard& lane_value) RTM_NO_EXCEPT
	{
		return vector4d{ input.xy, _mm_shuffle_pd(input.zw, lane_value.value, 0) };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Returns a floating point pointer to the vector4 data.
	//////////////////////////////////////////////////////////////////////////
	inline const double* vector_to_pointer(const vector4d& input) RTM_NO_EXCEPT
	{
		return reinterpret_cast<const double*>(&input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector4 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store(const vector4d& input, double* output) RTM_NO_EXCEPT
	{
		output[0] = vector_get_x(input);
		output[1] = vector_get_y(input);
		output[2] = vector_get_z(input);
		output[3] = vector_get_w(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector1 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store1(const vector4d& input, double* output) RTM_NO_EXCEPT
	{
		output[0] = vector_get_x(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector2 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store2(const vector4d& input, double* output) RTM_NO_EXCEPT
	{
		output[0] = vector_get_x(input);
		output[1] = vector_get_y(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector3 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store3(const vector4d& input, double* output) RTM_NO_EXCEPT
	{
		output[0] = vector_get_x(input);
		output[1] = vector_get_y(input);
		output[2] = vector_get_z(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector4 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store(const vector4d& input, uint8_t* output) RTM_NO_EXCEPT
	{
		std::memcpy(output, &input, sizeof(vector4d));
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector1 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store1(const vector4d& input, uint8_t* output)
	{
		std::memcpy(output, &input, sizeof(double) * 1);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector2 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store2(const vector4d& input, uint8_t* output)
	{
		std::memcpy(output, &input, sizeof(double) * 2);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector3 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store3(const vector4d& input, uint8_t* output)
	{
		std::memcpy(output, &input, sizeof(double) * 3);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector4 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store(const vector4d& input, float4d* output) RTM_NO_EXCEPT
	{
		output->x = vector_get_x(input);
		output->y = vector_get_y(input);
		output->z = vector_get_z(input);
		output->w = vector_get_w(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector2 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store2(const vector4d& input, float2d* output) RTM_NO_EXCEPT
	{
		output->x = vector_get_x(input);
		output->y = vector_get_y(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Writes a vector3 to unaligned memory.
	//////////////////////////////////////////////////////////////////////////
	inline void vector_store3(const vector4d& input, float3d* output) RTM_NO_EXCEPT
	{
		output->x = vector_get_x(input);
		output->y = vector_get_y(input);
		output->z = vector_get_z(input);
	}



	//////////////////////////////////////////////////////////////////////////
	// Arithmetic
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Per component addition of the two inputs: lhs + rhs
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_add(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_add_pd(lhs.xy, rhs.xy), _mm_add_pd(lhs.zw, rhs.zw) };
#else
		return vector_set(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component subtraction of the two inputs: lhs - rhs
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_sub(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_sub_pd(lhs.xy, rhs.xy), _mm_sub_pd(lhs.zw, rhs.zw) };
#else
		return vector_set(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component multiplication of the two inputs: lhs * rhs
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_mul(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_mul_pd(lhs.xy, rhs.xy), _mm_mul_pd(lhs.zw, rhs.zw) };
#else
		return vector_set(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component multiplication of the vector by a scalar: lhs * rhs
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_mul(const vector4d& lhs, double rhs) RTM_NO_EXCEPT
	{
		return vector_mul(lhs, vector_set(rhs));
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Per component multiplication of the vector by a scalar: lhs * rhs
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_mul(const vector4d& lhs, const scalard& rhs) RTM_NO_EXCEPT
	{
		const __m128d rhs_xx = _mm_shuffle_pd(rhs.value, rhs.value, 0);
		return vector4d{ _mm_mul_pd(lhs.xy, rhs_xx), _mm_mul_pd(lhs.zw, rhs_xx) };
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Per component division of the two inputs: lhs / rhs
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_div(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_div_pd(lhs.xy, rhs.xy), _mm_div_pd(lhs.zw, rhs.zw) };
#else
		return vector_set(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component maximum of the two inputs: max(lhs, rhs)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_max(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_max_pd(lhs.xy, rhs.xy), _mm_max_pd(lhs.zw, rhs.zw) };
#else
		return vector_set(scalar_max(lhs.x, rhs.x), scalar_max(lhs.y, rhs.y), scalar_max(lhs.z, rhs.z), scalar_max(lhs.w, rhs.w));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component minimum of the two inputs: min(lhs, rhs)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_min(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		return vector4d{ _mm_min_pd(lhs.xy, rhs.xy), _mm_min_pd(lhs.zw, rhs.zw) };
#else
		return vector_set(scalar_min(lhs.x, rhs.x), scalar_min(lhs.y, rhs.y), scalar_min(lhs.z, rhs.z), scalar_min(lhs.w, rhs.w));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component clamping of an input between a minimum and a maximum value: min(max_value, max(min_value, input))
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_clamp(const vector4d& input, const vector4d& min_value, const vector4d& max_value) RTM_NO_EXCEPT
	{
		return vector_min(max_value, vector_max(min_value, input));
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component absolute of the input: abs(input)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_abs(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		vector4d zero{ _mm_setzero_pd(), _mm_setzero_pd() };
		return vector_max(vector_sub(zero, input), input);
#else
		return vector_set(scalar_abs(input.x), scalar_abs(input.y), scalar_abs(input.z), scalar_abs(input.w));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component negation of the input: -input
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_neg(const vector4d& input) RTM_NO_EXCEPT
	{
		return vector_mul(input, -1.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component reciprocal of the input: 1.0 / input
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_reciprocal(const vector4d& input) RTM_NO_EXCEPT
	{
		return vector_div(vector_set(1.0), input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component returns the smallest integer value not less than the input.
	// vector_ceil([1.8, 1.0, -1.8, -1.0]) = [2.0, 1.0, -1.0, -1.0]
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_ceil(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		// NaN, +- Infinity, and numbers larger or equal to 2^23 remain unchanged
		// since they have no fractional part.

		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);
		const __m128d fractional_limit = _mm_set1_pd(4503599627370496.0); // 2^52

		// Build our mask, larger values that have no fractional part, and infinities will be true
		// Smaller values and NaN will be false
		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));
		__m128d abs_input_zw = _mm_and_pd(input.zw, _mm_castsi128_pd(abs_mask));
		__m128d is_input_large_xy = _mm_cmpge_pd(abs_input_xy, fractional_limit);
		__m128d is_input_large_zw = _mm_cmpge_pd(abs_input_zw, fractional_limit);

		// Test if our input is NaN with (value != value), it is only true for NaN
		__m128d is_nan_xy = _mm_cmpneq_pd(input.xy, input.xy);
		__m128d is_nan_zw = _mm_cmpneq_pd(input.zw, input.zw);

		// Combine our masks to determine if we should return the original value
		__m128d use_original_input_xy = _mm_or_pd(is_input_large_xy, is_nan_xy);
		__m128d use_original_input_zw = _mm_or_pd(is_input_large_zw, is_nan_zw);

		// Convert to an integer and back
		__m128d integer_part_xy = _mm_cvtepi32_pd(_mm_cvtpd_epi32(input.xy));
		__m128d integer_part_zw = _mm_cvtepi32_pd(_mm_cvtpd_epi32(input.zw));

		// Test if the returned value is smaller than the original.
		// A positive input will round towards zero and be lower when we need it to be greater.
		__m128d is_positive_xy = _mm_cmplt_pd(integer_part_xy, input.xy);
		__m128d is_positive_zw = _mm_cmplt_pd(integer_part_zw, input.zw);

		// Our mask output is 64 bit wide but to convert to a bias, we need 32 bit integers
		is_positive_xy = _mm_castps_pd(_mm_shuffle_ps(_mm_castpd_ps(is_positive_xy), _mm_castpd_ps(is_positive_xy), _MM_SHUFFLE(2, 0, 2, 0)));
		is_positive_zw = _mm_castps_pd(_mm_shuffle_ps(_mm_castpd_ps(is_positive_zw), _mm_castpd_ps(is_positive_zw), _MM_SHUFFLE(2, 0, 2, 0)));

		// Convert our mask to a float, ~0 yields -1.0 since it is a valid signed integer
		// Negative values will yield a 0.0 bias
		__m128d bias_xy = _mm_cvtepi32_pd(_mm_castpd_si128(is_positive_xy));
		__m128d bias_zw = _mm_cvtepi32_pd(_mm_castpd_si128(is_positive_zw));

		// Subtract our bias to properly handle positive values
		integer_part_xy = _mm_sub_pd(integer_part_xy, bias_xy);
		integer_part_zw = _mm_sub_pd(integer_part_zw, bias_zw);

		__m128d result_xy = _mm_or_pd(_mm_and_pd(use_original_input_xy, input.xy), _mm_andnot_pd(use_original_input_xy, integer_part_xy));
		__m128d result_zw = _mm_or_pd(_mm_and_pd(use_original_input_zw, input.zw), _mm_andnot_pd(use_original_input_zw, integer_part_zw));
		return vector4d{ result_xy, result_zw };
#else
		return vector_set(scalar_ceil(vector_get_x(input)), scalar_ceil(vector_get_y(input)), scalar_ceil(vector_get_z(input)), scalar_ceil(vector_get_w(input)));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component returns the largest integer value not greater than the input.
	// vector_floor([1.8, 1.0, -1.8, -1.0]) = [1.0, 1.0, -2.0, -1.0]
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_floor(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE4_INTRINSICS)
		return vector4d{ _mm_floor_pd(input.xy), _mm_floor_pd(input.zw) };
#elif defined(RTM_SSE2_INTRINSICS)
		// NaN, +- Infinity, and numbers larger or equal to 2^23 remain unchanged
		// since they have no fractional part.

		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);
		const __m128d fractional_limit = _mm_set1_pd(4503599627370496.0); // 2^52

		// Build our mask, larger values that have no fractional part, and infinities will be true
		// Smaller values and NaN will be false
		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));
		__m128d abs_input_zw = _mm_and_pd(input.zw, _mm_castsi128_pd(abs_mask));
		__m128d is_input_large_xy = _mm_cmpge_pd(abs_input_xy, fractional_limit);
		__m128d is_input_large_zw = _mm_cmpge_pd(abs_input_zw, fractional_limit);

		// Test if our input is NaN with (value != value), it is only true for NaN
		__m128d is_nan_xy = _mm_cmpneq_pd(input.xy, input.xy);
		__m128d is_nan_zw = _mm_cmpneq_pd(input.zw, input.zw);

		// Combine our masks to determine if we should return the original value
		__m128d use_original_input_xy = _mm_or_pd(is_input_large_xy, is_nan_xy);
		__m128d use_original_input_zw = _mm_or_pd(is_input_large_zw, is_nan_zw);

		// Convert to an integer and back
		__m128d integer_part_xy = _mm_cvtepi32_pd(_mm_cvtpd_epi32(input.xy));
		__m128d integer_part_zw = _mm_cvtepi32_pd(_mm_cvtpd_epi32(input.zw));

		// Test if the returned value is greater than the original.
		// A negative input will round towards zero and be greater when we need it to be smaller.
		__m128d is_negative_xy = _mm_cmpgt_pd(integer_part_xy, input.xy);
		__m128d is_negative_zw = _mm_cmpgt_pd(integer_part_zw, input.zw);

		// Our mask output is 64 bit wide but to convert to a bias, we need 32 bit integers
		is_negative_xy = _mm_castps_pd(_mm_shuffle_ps(_mm_castpd_ps(is_negative_xy), _mm_castpd_ps(is_negative_xy), _MM_SHUFFLE(2, 0, 2, 0)));
		is_negative_zw = _mm_castps_pd(_mm_shuffle_ps(_mm_castpd_ps(is_negative_zw), _mm_castpd_ps(is_negative_zw), _MM_SHUFFLE(2, 0, 2, 0)));

		// Convert our mask to a float, ~0 yields -1.0 since it is a valid signed integer
		// Positive values will yield a 0.0 bias
		__m128d bias_xy = _mm_cvtepi32_pd(_mm_castpd_si128(is_negative_xy));
		__m128d bias_zw = _mm_cvtepi32_pd(_mm_castpd_si128(is_negative_zw));

		// Add our bias to properly handle negative values
		integer_part_xy = _mm_add_pd(integer_part_xy, bias_xy);
		integer_part_zw = _mm_add_pd(integer_part_zw, bias_zw);

		__m128d result_xy = _mm_or_pd(_mm_and_pd(use_original_input_xy, input.xy), _mm_andnot_pd(use_original_input_xy, integer_part_xy));
		__m128d result_zw = _mm_or_pd(_mm_and_pd(use_original_input_zw, input.zw), _mm_andnot_pd(use_original_input_zw, integer_part_zw));
		return vector4d{ result_xy, result_zw };
#else
		return vector_set(scalar_floor(vector_get_x(input)), scalar_floor(vector_get_y(input)), scalar_floor(vector_get_z(input)), scalar_floor(vector_get_w(input)));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// 3D cross product: lhs x rhs
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_cross3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
		// cross(a, b) = (a.yzx * b.zxy) - (a.zxy * b.yzx)
		const double lhs_x = vector_get_x(lhs);
		const double lhs_y = vector_get_y(lhs);
		const double lhs_z = vector_get_z(lhs);
		const double rhs_x = vector_get_x(rhs);
		const double rhs_y = vector_get_y(rhs);
		const double rhs_z = vector_get_z(rhs);
		return vector_set((lhs_y * rhs_z) - (lhs_z * rhs_y), (lhs_z * rhs_x) - (lhs_x * rhs_z), (lhs_x * rhs_y) - (lhs_y * rhs_x));
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_dot
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard lhs_x = vector_get_x(lhs);
				const scalard lhs_y = vector_get_y(lhs);
				const scalard lhs_z = vector_get_z(lhs);
				const scalard lhs_w = vector_get_w(lhs);
				const scalard rhs_x = vector_get_x(rhs);
				const scalard rhs_y = vector_get_y(rhs);
				const scalard rhs_z = vector_get_z(rhs);
				const scalard rhs_w = vector_get_w(rhs);
				const scalard xx = scalar_mul(lhs_x, rhs_x);
				const scalard yy = scalar_mul(lhs_y, rhs_y);
				const scalard zz = scalar_mul(lhs_z, rhs_z);
				const scalard ww = scalar_mul(lhs_w, rhs_w);
				return scalar_cast(scalar_add(scalar_add(xx, yy), scalar_add(zz, ww)));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard lhs_x = vector_get_x(lhs);
				const scalard lhs_y = vector_get_y(lhs);
				const scalard lhs_z = vector_get_z(lhs);
				const scalard lhs_w = vector_get_w(lhs);
				const scalard rhs_x = vector_get_x(rhs);
				const scalard rhs_y = vector_get_y(rhs);
				const scalard rhs_z = vector_get_z(rhs);
				const scalard rhs_w = vector_get_w(rhs);
				const scalard xx = scalar_mul(lhs_x, rhs_x);
				const scalard yy = scalar_mul(lhs_y, rhs_y);
				const scalard zz = scalar_mul(lhs_z, rhs_z);
				const scalard ww = scalar_mul(lhs_w, rhs_w);
				return scalar_add(scalar_add(xx, yy), scalar_add(zz, ww));
			}
#endif

			inline RTM_SIMD_CALL operator vector4d() const RTM_NO_EXCEPT
			{
				const scalard dot = *this;
				return vector_set(dot);
			}

			vector4d lhs;
			vector4d rhs;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// 4D dot product: lhs . rhs
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_dot vector_dot(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_dot{ lhs, rhs };
	}

	//////////////////////////////////////////////////////////////////////////
	// 4D dot product: lhs . rhs
	//////////////////////////////////////////////////////////////////////////
	RTM_DEPRECATED("Use vector_dot instead, to be removed in v2.0")
	inline scalard vector_dot_as_scalar(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
		return scalar_set(vector_dot(lhs, rhs));
	}

	//////////////////////////////////////////////////////////////////////////
	// 4D dot product replicated in all components: lhs . rhs
	//////////////////////////////////////////////////////////////////////////
	RTM_DEPRECATED("Use vector_dot instead, to be removed in v2.0")
	inline vector4d vector_dot_as_vector(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
		const scalard dot = vector_dot(lhs, rhs);
		return vector_set(dot);
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_dot3
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
#if defined(RTM_SSE2_INTRINSICS)
				__m128d x2_y2 = _mm_mul_pd(lhs.xy, rhs.xy);
				__m128d z2_w2 = _mm_mul_pd(lhs.zw, rhs.zw);
				__m128d y2 = _mm_shuffle_pd(x2_y2, x2_y2, 1);
				__m128d x2y2 = _mm_add_sd(x2_y2, y2);
				return _mm_cvtsd_f64(_mm_add_sd(x2y2, z2_w2));
#else
				return (vector_get_x(lhs) * vector_get_x(rhs)) + (vector_get_y(lhs) * vector_get_y(rhs)) + (vector_get_z(lhs) * vector_get_z(rhs));
#endif
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				__m128d x2_y2 = _mm_mul_pd(lhs.xy, rhs.xy);
				__m128d z2_w2 = _mm_mul_pd(lhs.zw, rhs.zw);
				__m128d y2 = _mm_shuffle_pd(x2_y2, x2_y2, 1);
				__m128d x2y2 = _mm_add_sd(x2_y2, y2);
				return scalard{ _mm_add_sd(x2y2, z2_w2) };
			}
#endif

			vector4d lhs;
			vector4d rhs;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// 3D dot product: lhs . rhs
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_dot3 vector_dot3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_dot3{ lhs, rhs };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the squared length/norm of the vector4.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_dot vector_length_squared(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_dot{ input, input };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the squared length/norm of the vector3.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_dot3 vector_length_squared3(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_dot3{ input, input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_length
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared(input);
				return scalar_cast(scalar_sqrt(len_sq));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared(input);
				return scalar_sqrt(len_sq);
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the length/norm of the vector4.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_length vector_length(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_length{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_length3
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared3(input);
				return scalar_cast(scalar_sqrt(len_sq));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared3(input);
				return scalar_sqrt(len_sq);
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the length/norm of the vector3.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_length3 vector_length3(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_length3{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_length_reciprocal
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared(input);
				return scalar_cast(scalar_sqrt_reciprocal(len_sq));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared(input);
				return scalar_sqrt_reciprocal(len_sq);
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the reciprocal length/norm of the vector4.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_length_reciprocal vector_length_reciprocal(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_length_reciprocal{ input };
	}

	namespace rtm_impl
	{
		//////////////////////////////////////////////////////////////////////////
		// This is a helper struct to allow a single consistent API between
		// various vector types when the semantics are identical but the return
		// type differs. Implicit coercion is used to return the desired value
		// at the call site.
		//////////////////////////////////////////////////////////////////////////
		struct vector4d_vector_length_reciprocal3
		{
			inline RTM_SIMD_CALL operator double() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared3(input);
				return scalar_cast(scalar_sqrt_reciprocal(len_sq));
			}

#if defined(RTM_SSE2_INTRINSICS)
			inline RTM_SIMD_CALL operator scalard() const RTM_NO_EXCEPT
			{
				const scalard len_sq = vector_length_squared3(input);
				return scalar_sqrt_reciprocal(len_sq);
			}
#endif

			vector4d input;
		};
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the reciprocal length/norm of the vector3.
	//////////////////////////////////////////////////////////////////////////
	constexpr rtm_impl::vector4d_vector_length_reciprocal3 vector_length_reciprocal3(const vector4d& input) RTM_NO_EXCEPT
	{
		return rtm_impl::vector4d_vector_length_reciprocal3{ input };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns the distance between two 3D points.
	//////////////////////////////////////////////////////////////////////////
	inline rtm_impl::vector4d_vector_length3 vector_distance3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
		const vector4d difference = vector_sub(lhs, rhs);
		return rtm_impl::vector4d_vector_length3{ difference };
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns a normalized vector3.
	// If the length of the input is not finite or zero, the result is undefined.
	// For a safe alternative, supply a fallback value and a threshold.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_normalize3(const vector4d& input) RTM_NO_EXCEPT
	{
		// Reciprocal is more accurate to normalize with
		const scalard len_sq = vector_length_squared3(input);
		return vector_mul(input, scalar_sqrt_reciprocal(len_sq));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns a normalized vector3.
	// If the length of the input is below the supplied threshold, the
	// fall back value is returned instead.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_normalize3(const vector4d& input, const vector4d& fallback, double threshold = 1.0E-8) RTM_NO_EXCEPT
	{
		// Reciprocal is more accurate to normalize with
		const scalard len_sq = vector_length_squared3(input);
		if (scalar_cast(len_sq) >= threshold)
			return vector_mul(input, scalar_sqrt_reciprocal(len_sq));
		else
			return fallback;
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the fractional part of the input.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_fraction(const vector4d& input) RTM_NO_EXCEPT
	{
		return vector_set(scalar_fraction(vector_get_x(input)), scalar_fraction(vector_get_y(input)), scalar_fraction(vector_get_z(input)), scalar_fraction(vector_get_w(input)));
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component multiplication/addition of the three inputs: v2 + (v0 * v1)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_mul_add(const vector4d& v0, const vector4d& v1, const vector4d& v2) RTM_NO_EXCEPT
	{
		return vector_add(vector_mul(v0, v1), v2);
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component multiplication/addition of the three inputs: v2 + (v0 * s1)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_mul_add(const vector4d& v0, double s1, const vector4d& v2) RTM_NO_EXCEPT
	{
		return vector_add(vector_mul(v0, s1), v2);
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Per component multiplication/addition of the three inputs: v2 + (v0 * s1)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_mul_add(const vector4d& v0, const scalard& s1, const vector4d& v2) RTM_NO_EXCEPT
	{
		return vector_add(vector_mul(v0, s1), v2);
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Per component negative multiplication/subtraction of the three inputs: -((v0 * v1) - v2)
	// This is mathematically equivalent to: v2 - (v0 * v1)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_neg_mul_sub(const vector4d& v0, const vector4d& v1, const vector4d& v2) RTM_NO_EXCEPT
	{
		return vector_sub(v2, vector_mul(v0, v1));
	}

	//////////////////////////////////////////////////////////////////////////
	// Per component negative multiplication/subtraction of the three inputs: -((v0 * s1) - v2)
	// This is mathematically equivalent to: v2 - (v0 * s1)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_neg_mul_sub(const vector4d& v0, double s1, const vector4d& v2) RTM_NO_EXCEPT
	{
		return vector_sub(v2, vector_mul(v0, s1));
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Per component negative multiplication/subtraction of the three inputs: -((v0 * s1) - v2)
	// This is mathematically equivalent to: v2 - (v0 * s1)
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_neg_mul_sub(const vector4d& v0, const scalard& s1, const vector4d& v2) RTM_NO_EXCEPT
	{
		return vector_sub(v2, vector_mul(v0, s1));
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	// Per component linear interpolation of the two inputs at the specified alpha.
	// The formula used is: ((1.0 - alpha) * start) + (alpha * end).
	// Interpolation is stable and will return 'start' when alpha is 0.0 and 'end' when it is 1.0.
	// This is the same instruction count when FMA is present but it might be slightly slower
	// due to the extra multiplication compared to: start + (alpha * (end - start)).
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_lerp(const vector4d& start, const vector4d& end, double alpha) RTM_NO_EXCEPT
	{
		// ((1.0 - alpha) * start) + (alpha * end) == (start - alpha * start) + (alpha * end)
		return vector_mul_add(end, alpha, vector_neg_mul_sub(start, alpha, start));
	}

#if defined(RTM_SSE2_INTRINSICS)
	//////////////////////////////////////////////////////////////////////////
	// Per component linear interpolation of the two inputs at the specified alpha.
	// The formula used is: ((1.0 - alpha) * start) + (alpha * end).
	// Interpolation is stable and will return 'start' when alpha is 0.0 and 'end' when it is 1.0.
	// This is the same instruction count when FMA is present but it might be slightly slower
	// due to the extra multiplication compared to: start + (alpha * (end - start)).
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_lerp(const vector4d& start, const vector4d& end, const scalard& alpha) RTM_NO_EXCEPT
	{
		// ((1.0 - alpha) * start) + (alpha * end) == (start - alpha * start) + (alpha * end)
		const vector4d alpha_v = vector_set(alpha);
		return vector_mul_add(end, alpha_v, vector_neg_mul_sub(start, alpha_v, start));
	}
#endif



	//////////////////////////////////////////////////////////////////////////
	// Comparisons and masking
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Returns per component ~0 if equal, otherwise 0: lhs == rhs ? ~0 : 0
	//////////////////////////////////////////////////////////////////////////
	inline mask4d vector_equal(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmpeq_pd(lhs.xy, rhs.xy);
		__m128d zw_lt_pd = _mm_cmpeq_pd(lhs.zw, rhs.zw);
		return mask4d{ xy_lt_pd, zw_lt_pd };
#else
		return mask4d{ rtm_impl::get_mask_value(lhs.x == rhs.x), rtm_impl::get_mask_value(lhs.y == rhs.y), rtm_impl::get_mask_value(lhs.z == rhs.z), rtm_impl::get_mask_value(lhs.w == rhs.w) };
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component ~0 if less than, otherwise 0: lhs < rhs ? ~0 : 0
	//////////////////////////////////////////////////////////////////////////
	inline mask4d vector_less_than(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmplt_pd(lhs.xy, rhs.xy);
		__m128d zw_lt_pd = _mm_cmplt_pd(lhs.zw, rhs.zw);
		return mask4d{xy_lt_pd, zw_lt_pd};
#else
		return mask4d{rtm_impl::get_mask_value(lhs.x < rhs.x), rtm_impl::get_mask_value(lhs.y < rhs.y), rtm_impl::get_mask_value(lhs.z < rhs.z), rtm_impl::get_mask_value(lhs.w < rhs.w)};
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component ~0 if less equal, otherwise 0: lhs <= rhs ? ~0 : 0
	//////////////////////////////////////////////////////////////////////////
	inline mask4d vector_less_equal(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmple_pd(lhs.xy, rhs.xy);
		__m128d zw_lt_pd = _mm_cmple_pd(lhs.zw, rhs.zw);
		return mask4d{ xy_lt_pd, zw_lt_pd };
#else
		return mask4d{ rtm_impl::get_mask_value(lhs.x <= rhs.x), rtm_impl::get_mask_value(lhs.y <= rhs.y), rtm_impl::get_mask_value(lhs.z <= rhs.z), rtm_impl::get_mask_value(lhs.w <= rhs.w) };
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component ~0 if greater than, otherwise 0: lhs > rhs ? ~0 : 0
	//////////////////////////////////////////////////////////////////////////
	inline mask4d vector_greater_than(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpgt_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpgt_pd(lhs.zw, rhs.zw);
		return mask4d{ xy_ge_pd, zw_ge_pd };
#else
		return mask4d{ rtm_impl::get_mask_value(lhs.x > rhs.x), rtm_impl::get_mask_value(lhs.y > rhs.y), rtm_impl::get_mask_value(lhs.z > rhs.z), rtm_impl::get_mask_value(lhs.w > rhs.w) };
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component ~0 if greater equal, otherwise 0: lhs >= rhs ? ~0 : 0
	//////////////////////////////////////////////////////////////////////////
	inline mask4d vector_greater_equal(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpge_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpge_pd(lhs.zw, rhs.zw);
		return mask4d{ xy_ge_pd, zw_ge_pd };
#else
		return mask4d{ rtm_impl::get_mask_value(lhs.x >= rhs.x), rtm_impl::get_mask_value(lhs.y >= rhs.y), rtm_impl::get_mask_value(lhs.z >= rhs.z), rtm_impl::get_mask_value(lhs.w >= rhs.w) };
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all 4 components are less than, otherwise false: all(lhs < rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_less_than(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmplt_pd(lhs.xy, rhs.xy);
		__m128d zw_lt_pd = _mm_cmplt_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_lt_pd) & _mm_movemask_pd(zw_lt_pd)) == 3;
#else
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w < rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xy] components are less than, otherwise false: all(lhs < rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_less_than2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmplt_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_lt_pd) == 3;
#else
		return lhs.x < rhs.x && lhs.y < rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xyz] components are less than, otherwise false: all(lhs < rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_less_than3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmplt_pd(lhs.xy, rhs.xy);
		__m128d zw_lt_pd = _mm_cmplt_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_lt_pd) == 3 && (_mm_movemask_pd(zw_lt_pd) & 1) == 1;
#else
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any 4 components are less than, otherwise false: any(lhs < rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_less_than(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmplt_pd(lhs.xy, rhs.xy);
		__m128d zw_lt_pd = _mm_cmplt_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_lt_pd) | _mm_movemask_pd(zw_lt_pd)) != 0;
#else
		return lhs.x < rhs.x || lhs.y < rhs.y || lhs.z < rhs.z || lhs.w < rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xy] components are less than, otherwise false: any(lhs < rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_less_than2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmplt_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_lt_pd) != 0;
#else
		return lhs.x < rhs.x || lhs.y < rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xyz] components are less than, otherwise false: any(lhs < rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_less_than3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_lt_pd = _mm_cmplt_pd(lhs.xy, rhs.xy);
		__m128d zw_lt_pd = _mm_cmplt_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_lt_pd) != 0 || (_mm_movemask_pd(zw_lt_pd) & 0x1) != 0;
#else
		return lhs.x < rhs.x || lhs.y < rhs.y || lhs.z < rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all 4 components are less equal, otherwise false: all(lhs <= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_less_equal(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_le_pd = _mm_cmple_pd(lhs.xy, rhs.xy);
		__m128d zw_le_pd = _mm_cmple_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_le_pd) & _mm_movemask_pd(zw_le_pd)) == 3;
#else
		return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z && lhs.w <= rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xy] components are less equal, otherwise false: all(lhs <= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_less_equal2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_le_pd = _mm_cmple_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_le_pd) == 3;
#else
		return lhs.x <= rhs.x && lhs.y <= rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xyz] components are less equal, otherwise false: all(lhs <= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_less_equal3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_le_pd = _mm_cmple_pd(lhs.xy, rhs.xy);
		__m128d zw_le_pd = _mm_cmple_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_le_pd) == 3 && (_mm_movemask_pd(zw_le_pd) & 1) != 0;
#else
		return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any 4 components are less equal, otherwise false: any(lhs <= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_less_equal(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_le_pd = _mm_cmple_pd(lhs.xy, rhs.xy);
		__m128d zw_le_pd = _mm_cmple_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_le_pd) | _mm_movemask_pd(zw_le_pd)) != 0;
#else
		return lhs.x <= rhs.x || lhs.y <= rhs.y || lhs.z <= rhs.z || lhs.w <= rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xy] components are less equal, otherwise false: any(lhs <= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_less_equal2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_le_pd = _mm_cmple_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_le_pd) != 0;
#else
		return lhs.x <= rhs.x || lhs.y <= rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xyz] components are less equal, otherwise false: any(lhs <= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_less_equal3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_le_pd = _mm_cmple_pd(lhs.xy, rhs.xy);
		__m128d zw_le_pd = _mm_cmple_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_le_pd) != 0 || (_mm_movemask_pd(zw_le_pd) & 1) != 0;
#else
		return lhs.x <= rhs.x || lhs.y <= rhs.y || lhs.z <= rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all 4 components are greater than, otherwise false: all(lhs > rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_greater_than(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpgt_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpgt_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_ge_pd) & _mm_movemask_pd(zw_ge_pd)) == 3;
#else
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z && lhs.w > rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xy] components are greater than, otherwise false: all(lhs > rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_greater_than2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpgt_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_ge_pd) == 3;
#else
		return lhs.x > rhs.x && lhs.y > rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xyz] components are greater than, otherwise false: all(lhs > rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_greater_than3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpgt_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpgt_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_ge_pd) == 3 && (_mm_movemask_pd(zw_ge_pd) & 1) != 0;
#else
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any 4 components are greater than, otherwise false: any(lhs > rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_greater_than(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpgt_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpgt_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_ge_pd) | _mm_movemask_pd(zw_ge_pd)) != 0;
#else
		return lhs.x > rhs.x || lhs.y > rhs.y || lhs.z > rhs.z || lhs.w > rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xy] components are greater than, otherwise false: any(lhs > rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_greater_than2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpgt_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_ge_pd) != 0;
#else
		return lhs.x > rhs.x || lhs.y > rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xyz] components are greater than, otherwise false: any(lhs > rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_greater_than3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpgt_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpgt_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_ge_pd) != 0 || (_mm_movemask_pd(zw_ge_pd) & 1) != 0;
#else
		return lhs.x > rhs.x || lhs.y > rhs.y || lhs.z > rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all 4 components are greater equal, otherwise false: all(lhs >= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_greater_equal(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpge_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpge_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_ge_pd) & _mm_movemask_pd(zw_ge_pd)) == 3;
#else
		return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z && lhs.w >= rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xy] components are greater equal, otherwise false: all(lhs >= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_greater_equal2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpge_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_ge_pd) == 3;
#else
		return lhs.x >= rhs.x && lhs.y >= rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xyz] components are greater equal, otherwise false: all(lhs >= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_greater_equal3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpge_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpge_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_ge_pd) == 3 && (_mm_movemask_pd(zw_ge_pd) & 1) != 0;
#else
		return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any 4 components are greater equal, otherwise false: any(lhs >= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_greater_equal(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpge_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpge_pd(lhs.zw, rhs.zw);
		return (_mm_movemask_pd(xy_ge_pd) | _mm_movemask_pd(zw_ge_pd)) != 0;
#else
		return lhs.x >= rhs.x || lhs.y >= rhs.y || lhs.z >= rhs.z || lhs.w >= rhs.w;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xy] components are greater equal, otherwise false: any(lhs >= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_greater_equal2(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpge_pd(lhs.xy, rhs.xy);
		return _mm_movemask_pd(xy_ge_pd) != 0;
#else
		return lhs.x >= rhs.x || lhs.y >= rhs.y;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xyz] components are greater equal, otherwise false: any(lhs >= rhs)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_greater_equal3(const vector4d& lhs, const vector4d& rhs) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy_ge_pd = _mm_cmpge_pd(lhs.xy, rhs.xy);
		__m128d zw_ge_pd = _mm_cmpge_pd(lhs.zw, rhs.zw);
		return _mm_movemask_pd(xy_ge_pd) != 0 || (_mm_movemask_pd(zw_ge_pd) & 1) != 0;
#else
		return lhs.x >= rhs.x || lhs.y >= rhs.y || lhs.z >= rhs.z;
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all 4 components are near equal, otherwise false: all(abs(lhs - rhs) <= threshold)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_near_equal(const vector4d& lhs, const vector4d& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_all_less_equal(vector_abs(vector_sub(lhs, rhs)), vector_set(threshold));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xy] components are near equal, otherwise false: all(abs(lhs - rhs) <= threshold)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_near_equal2(const vector4d& lhs, const vector4d& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_all_less_equal2(vector_abs(vector_sub(lhs, rhs)), vector_set(threshold));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xyz] components are near equal, otherwise false: all(abs(lhs - rhs) <= threshold)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_all_near_equal3(const vector4d& lhs, const vector4d& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_all_less_equal3(vector_abs(vector_sub(lhs, rhs)), vector_set(threshold));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any 4 components are near equal, otherwise false: any(abs(lhs - rhs) <= threshold)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_near_equal(const vector4d& lhs, const vector4d& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_any_less_equal(vector_abs(vector_sub(lhs, rhs)), vector_set(threshold));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xy] components are near equal, otherwise false: any(abs(lhs - rhs) <= threshold)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_near_equal2(const vector4d& lhs, const vector4d& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_any_less_equal2(vector_abs(vector_sub(lhs, rhs)), vector_set(threshold));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if any [xyz] components are near equal, otherwise false: any(abs(lhs - rhs) <= threshold)
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_any_near_equal3(const vector4d& lhs, const vector4d& rhs, double threshold = 0.00001) RTM_NO_EXCEPT
	{
		return vector_any_less_equal3(vector_abs(vector_sub(lhs, rhs)), vector_set(threshold));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all 4 components are finite (not NaN/Inf), otherwise false: all(finite(input))
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_is_finite(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);
		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));
		__m128d abs_input_zw = _mm_and_pd(input.zw, _mm_castsi128_pd(abs_mask));

		const __m128d infinity = _mm_set1_pd(std::numeric_limits<double>::infinity());
		__m128d is_infinity_xy = _mm_cmpeq_pd(abs_input_xy, infinity);
		__m128d is_infinity_zw = _mm_cmpeq_pd(abs_input_zw, infinity);

		__m128d is_nan_xy = _mm_cmpneq_pd(input.xy, input.xy);
		__m128d is_nan_zw = _mm_cmpneq_pd(input.zw, input.zw);

		__m128d is_not_finite_xy = _mm_or_pd(is_infinity_xy, is_nan_xy);
		__m128d is_not_finite_zw = _mm_or_pd(is_infinity_zw, is_nan_zw);
		__m128d is_not_finite = _mm_or_pd(is_not_finite_xy, is_not_finite_zw);
		return _mm_movemask_pd(is_not_finite) == 0x0;
#else
		return scalar_is_finite(vector_get_x(input)) && scalar_is_finite(vector_get_y(input)) && scalar_is_finite(vector_get_z(input)) && scalar_is_finite(vector_get_w(input));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xy] components are finite (not NaN/Inf), otherwise false: all(finite(input))
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_is_finite2(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);
		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));

		const __m128d infinity = _mm_set1_pd(std::numeric_limits<double>::infinity());
		__m128d is_infinity_xy = _mm_cmpeq_pd(abs_input_xy, infinity);

		__m128d is_nan_xy = _mm_cmpneq_pd(input.xy, input.xy);

		__m128d is_not_finite_xy = _mm_or_pd(is_infinity_xy, is_nan_xy);
		return _mm_movemask_pd(is_not_finite_xy) == 0x0;
#else
		return scalar_is_finite(vector_get_x(input)) && scalar_is_finite(vector_get_y(input));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns true if all [xyz] components are finite (not NaN/Inf), otherwise false: all(finite(input))
	//////////////////////////////////////////////////////////////////////////
	inline bool vector_is_finite3(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);
		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));
		__m128d abs_input_zw = _mm_and_pd(input.zw, _mm_castsi128_pd(abs_mask));

		const __m128d infinity = _mm_set1_pd(std::numeric_limits<double>::infinity());
		__m128d is_infinity_xy = _mm_cmpeq_pd(abs_input_xy, infinity);
		__m128d is_infinity_zw = _mm_cmpeq_pd(abs_input_zw, infinity);

		__m128d is_nan_xy = _mm_cmpneq_pd(input.xy, input.xy);
		__m128d is_nan_zw = _mm_cmpneq_pd(input.zw, input.zw);

		__m128d is_not_finite_xy = _mm_or_pd(is_infinity_xy, is_nan_xy);
		__m128d is_not_finite_zw = _mm_or_pd(is_infinity_zw, is_nan_zw);
		return _mm_movemask_pd(is_not_finite_xy) == 0 && (_mm_movemask_pd(is_not_finite_zw) & 0x1) == 0;
#else
		return scalar_is_finite(vector_get_x(input)) && scalar_is_finite(vector_get_y(input)) && scalar_is_finite(vector_get_z(input));
#endif
	}



	//////////////////////////////////////////////////////////////////////////
	// Swizzling, permutations, and mixing
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Per component selection depending on the mask: mask != 0 ? if_true : if_false
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_select(const mask4d& mask, const vector4d& if_true, const vector4d& if_false) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		__m128d xy = _mm_or_pd(_mm_andnot_pd(mask.xy, if_false.xy), _mm_and_pd(if_true.xy, mask.xy));
		__m128d zw = _mm_or_pd(_mm_andnot_pd(mask.zw, if_false.zw), _mm_and_pd(if_true.zw, mask.zw));
		return vector4d{ xy, zw };
#else
		return vector4d{ rtm_impl::select(mask.x, if_true.x, if_false.x), rtm_impl::select(mask.y, if_true.y, if_false.y), rtm_impl::select(mask.z, if_true.z, if_false.z), rtm_impl::select(mask.w, if_true.w, if_false.w) };
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Mixes two inputs and returns the desired components.
	// [xyzw] indexes into the first input while [abcd] indexes in the second.
	//////////////////////////////////////////////////////////////////////////
	template<mix4 comp0, mix4 comp1, mix4 comp2, mix4 comp3>
	inline vector4d vector_mix(const vector4d& input0, const vector4d& input1) RTM_NO_EXCEPT
	{
		// Slow code path, not yet optimized or not using intrinsics
		const double x = rtm_impl::is_mix_xyzw(comp0) ? vector_get_component<comp0>(input0) : vector_get_component<comp0>(input1);
		const double y = rtm_impl::is_mix_xyzw(comp1) ? vector_get_component<comp1>(input0) : vector_get_component<comp1>(input1);
		const double z = rtm_impl::is_mix_xyzw(comp2) ? vector_get_component<comp2>(input0) : vector_get_component<comp2>(input1);
		const double w = rtm_impl::is_mix_xyzw(comp3) ? vector_get_component<comp3>(input0) : vector_get_component<comp3>(input1);
		return vector_set(x, y, z, w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Replicates the [x] component in all components.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_dup_x(const vector4d& input) RTM_NO_EXCEPT { return vector_mix<mix4::x, mix4::x, mix4::x, mix4::x>(input, input); }

	//////////////////////////////////////////////////////////////////////////
	// Replicates the [y] component in all components.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_dup_y(const vector4d& input) RTM_NO_EXCEPT { return vector_mix<mix4::y, mix4::y, mix4::y, mix4::y>(input, input); }

	//////////////////////////////////////////////////////////////////////////
	// Replicates the [z] component in all components.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_dup_z(const vector4d& input) RTM_NO_EXCEPT { return vector_mix<mix4::z, mix4::z, mix4::z, mix4::z>(input, input); }

	//////////////////////////////////////////////////////////////////////////
	// Replicates the [w] component in all components.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_dup_w(const vector4d& input) RTM_NO_EXCEPT { return vector_mix<mix4::w, mix4::w, mix4::w, mix4::w>(input, input); }


	//////////////////////////////////////////////////////////////////////////
	// Miscellaneous
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Returns per component the sign of the input vector: input >= 0.0 ? 1.0 : -1.0
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_sign(const vector4d& input) RTM_NO_EXCEPT
	{
		const mask4d mask = vector_greater_equal(input, vector_zero());
		return vector_select(mask, vector_set(1.0), vector_set(-1.0));
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the input with the sign of the control value.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_copy_sign(const vector4d& input, const vector4d& control_sign) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE2_INTRINSICS)
		const __m128d sign_bit = _mm_set1_pd(-0.0);
		__m128d signs_xy = _mm_and_pd(sign_bit, control_sign.xy);
		__m128d signs_zw = _mm_and_pd(sign_bit, control_sign.zw);
		__m128d abs_input_xy = _mm_andnot_pd(sign_bit, input.xy);
		__m128d abs_input_zw = _mm_andnot_pd(sign_bit, input.zw);
		__m128d xy = _mm_or_pd(abs_input_xy, signs_xy);
		__m128d zw = _mm_or_pd(abs_input_zw, signs_zw);
		return vector4d{ xy, zw };
#else
		double x = vector_get_x(input);
		double y = vector_get_y(input);
		double z = vector_get_z(input);
		double w = vector_get_w(input);

		double x_sign = vector_get_x(control_sign);
		double y_sign = vector_get_y(control_sign);
		double z_sign = vector_get_z(control_sign);
		double w_sign = vector_get_w(control_sign);

		return vector_set(std::copysign(x, x_sign), std::copysign(y, y_sign), std::copysign(z, z_sign), std::copysign(w, w_sign));
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the rounded input using a symmetric algorithm.
	// vector_round_symmetric(1.5) = 2.0
	// vector_round_symmetric(1.2) = 1.0
	// vector_round_symmetric(-1.5) = -2.0
	// vector_round_symmetric(-1.2) = -1.0
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_round_symmetric(const vector4d& input) RTM_NO_EXCEPT
	{
		// NaN, +- Infinity, and numbers larger or equal to 2^23 remain unchanged
		// since they have no fractional part.

#if defined(RTM_SSE4_INTRINSICS)
		const __m128d sign_mask = _mm_set_pd(-0.0, -0.0);
		__m128d sign_xy = _mm_and_pd(input.xy, sign_mask);
		__m128d sign_zw = _mm_and_pd(input.zw, sign_mask);

		// For positive values, we add a bias of 0.5.
		// For negative values, we add a bias of -0.5.
		__m128d half = _mm_set1_pd(0.5);
		__m128d bias_xy = _mm_or_pd(sign_xy, half);
		__m128d bias_zw = _mm_or_pd(sign_zw, half);
		__m128d biased_input_xy = _mm_add_pd(input.xy, bias_xy);
		__m128d biased_input_zw = _mm_add_pd(input.zw, bias_zw);

		__m128d floored_xy = _mm_floor_pd(biased_input_xy);
		__m128d floored_zw = _mm_floor_pd(biased_input_zw);
		__m128d ceiled_xy = _mm_ceil_pd(biased_input_xy);
		__m128d ceiled_zw = _mm_ceil_pd(biased_input_zw);
		__m128d zero = _mm_setzero_pd();
		__m128d is_positive_xy = _mm_cmpge_pd(input.xy, zero);
		__m128d is_positive_zw = _mm_cmpge_pd(input.zw, zero);

#if defined(RTM_AVX_INTRINSICS)
		__m128d result_xy = _mm_blendv_pd(ceiled_xy, floored_xy, is_positive_xy);
		__m128d result_zw = _mm_blendv_pd(ceiled_zw, floored_zw, is_positive_zw);
#else
		__m128d result_xy = _mm_or_pd(_mm_and_pd(is_positive_xy, floored_xy), _mm_andnot_pd(is_positive_xy, ceiled_xy));
		__m128d result_zw = _mm_or_pd(_mm_and_pd(is_positive_zw, floored_zw), _mm_andnot_pd(is_positive_zw, ceiled_zw));
#endif
		return vector4d{ result_xy, result_zw };
#elif defined(RTM_SSE2_INTRINSICS)
		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);
		const __m128d fractional_limit = _mm_set1_pd(4503599627370496.0); // 2^52

		// Build our mask, larger values that have no fractional part, and infinities will be true
		// Smaller values and NaN will be false
		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));
		__m128d abs_input_zw = _mm_and_pd(input.zw, _mm_castsi128_pd(abs_mask));
		__m128d is_input_large_xy = _mm_cmpge_pd(abs_input_xy, fractional_limit);
		__m128d is_input_large_zw = _mm_cmpge_pd(abs_input_zw, fractional_limit);

		// Test if our input is NaN with (value != value), it is only true for NaN
		__m128d is_nan_xy = _mm_cmpneq_pd(input.xy, input.xy);
		__m128d is_nan_zw = _mm_cmpneq_pd(input.zw, input.zw);

		// Combine our masks to determine if we should return the original value
		__m128d use_original_input_xy = _mm_or_pd(is_input_large_xy, is_nan_xy);
		__m128d use_original_input_zw = _mm_or_pd(is_input_large_zw, is_nan_zw);

		const __m128d sign_mask = _mm_set_pd(-0.0, -0.0);
		__m128d sign_xy = _mm_and_pd(input.xy, sign_mask);
		__m128d sign_zw = _mm_and_pd(input.zw, sign_mask);

		// For positive values, we add a bias of 0.5.
		// For negative values, we add a bias of -0.5.
		__m128d half = _mm_set1_pd(0.5);
		__m128d bias_xy = _mm_or_pd(sign_xy, half);
		__m128d bias_zw = _mm_or_pd(sign_zw, half);
		__m128d biased_input_xy = _mm_add_pd(input.xy, bias_xy);
		__m128d biased_input_zw = _mm_add_pd(input.zw, bias_zw);

		// Convert to an integer with truncation and back, this rounds towards zero.
		__m128d integer_part_xy = _mm_cvtepi32_pd(_mm_cvttpd_epi32(biased_input_xy));
		__m128d integer_part_zw = _mm_cvtepi32_pd(_mm_cvttpd_epi32(biased_input_zw));

		__m128d result_xy = _mm_or_pd(_mm_and_pd(use_original_input_xy, input.xy), _mm_andnot_pd(use_original_input_xy, integer_part_xy));
		__m128d result_zw = _mm_or_pd(_mm_and_pd(use_original_input_zw, input.zw), _mm_andnot_pd(use_original_input_zw, integer_part_zw));

		return vector4d{ result_xy, result_zw };
#else
		const vector4d half = vector_set(0.5);
		const vector4d floored = vector_floor(vector_add(input, half));
		const vector4d ceiled = vector_ceil(vector_sub(input, half));
		const mask4d is_greater_equal = vector_greater_equal(input, vector_zero());
		return vector_select(is_greater_equal, floored, ceiled);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the rounded input using a symmetric algorithm.
	// vector_symmetric_round(1.5) = 2.0
	// vector_symmetric_round(1.2) = 1.0
	// vector_symmetric_round(-1.5) = -2.0
	// vector_symmetric_round(-1.2) = -1.0
	//////////////////////////////////////////////////////////////////////////
	RTM_DEPRECATED("Use vector_round_symmetric instead, to be removed in v2.0")
	inline vector4d vector_symmetric_round(const vector4d& input) RTM_NO_EXCEPT
	{
		return vector_round_symmetric(input);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the rounded input using banker's rounding (half to even).
	// vector_round_bankers(2.5) = 2.0
	// vector_round_bankers(1.5) = 2.0
	// vector_round_bankers(1.2) = 1.0
	// vector_round_bankers(-2.5) = -2.0
	// vector_round_bankers(-1.5) = -2.0
	// vector_round_bankers(-1.2) = -1.0
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_round_bankers(const vector4d& input) RTM_NO_EXCEPT
	{
#if defined(RTM_SSE4_INTRINSICS)
		return vector4d{ _mm_round_pd(input.xy, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC), _mm_round_pd(input.zw, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC) };
#elif defined(RTM_SSE2_INTRINSICS)
		const __m128i abs_mask = _mm_set_epi64x(0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFFULL);

		const __m128d sign_mask = _mm_set_pd(-0.0, -0.0);
		__m128d sign_xy = _mm_and_pd(input.xy, sign_mask);
		__m128d sign_zw = _mm_and_pd(input.zw, sign_mask);

		// We add the largest integer that a 64 bit floating point number can represent and subtract it afterwards.
		// This relies on the fact that if we had a fractional part, the new value cannot be represented accurately
		// and IEEE 754 will perform rounding for us. The default rounding mode is Banker's rounding.
		// This has the effect of removing the fractional part while simultaneously rounding.
		// Use the same sign as the input value to make sure we handle positive and negative values.
		const __m128d fractional_limit = _mm_set1_pd(4503599627370496.0); // 2^52
		__m128d truncating_offset_xy = _mm_or_pd(sign_xy, fractional_limit);
		__m128d truncating_offset_zw = _mm_or_pd(sign_zw, fractional_limit);
		__m128d integer_part_xy = _mm_sub_pd(_mm_add_pd(input.xy, truncating_offset_xy), truncating_offset_xy);
		__m128d integer_part_zw = _mm_sub_pd(_mm_add_pd(input.zw, truncating_offset_zw), truncating_offset_zw);

		__m128d abs_input_xy = _mm_and_pd(input.xy, _mm_castsi128_pd(abs_mask));
		__m128d abs_input_zw = _mm_and_pd(input.zw, _mm_castsi128_pd(abs_mask));
		__m128d is_input_large_xy = _mm_cmpge_pd(abs_input_xy, fractional_limit);
		__m128d is_input_large_zw = _mm_cmpge_pd(abs_input_zw, fractional_limit);
		__m128d result_xy = _mm_or_pd(_mm_and_pd(is_input_large_xy, input.xy), _mm_andnot_pd(is_input_large_xy, integer_part_xy));
		__m128d result_zw = _mm_or_pd(_mm_and_pd(is_input_large_zw, input.zw), _mm_andnot_pd(is_input_large_zw, integer_part_zw));
		return vector4d{ result_xy, result_zw };
#else
		scalard x = scalar_round_bankers(scalard(vector_get_x(input)));
		scalard y = scalar_round_bankers(scalard(vector_get_y(input)));
		scalard z = scalar_round_bankers(scalard(vector_get_z(input)));
		scalard w = scalar_round_bankers(scalard(vector_get_w(input)));
		return vector_set(x, y, z, w);
#endif
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the sine of the input angle.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_sin(const vector4d& input) RTM_NO_EXCEPT
	{
		scalard x = scalar_sin(scalard(vector_get_x(input)));
		scalard y = scalar_sin(scalard(vector_get_y(input)));
		scalard z = scalar_sin(scalard(vector_get_z(input)));
		scalard w = scalar_sin(scalard(vector_get_w(input)));
		return vector_set(x, y, z, w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the arc-sine of the input.
	// Input value must be in the range [-1.0, 1.0].
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_asin(const vector4d& input) RTM_NO_EXCEPT
	{
		scalard x = scalar_asin(scalard(vector_get_x(input)));
		scalard y = scalar_asin(scalard(vector_get_y(input)));
		scalard z = scalar_asin(scalard(vector_get_z(input)));
		scalard w = scalar_asin(scalard(vector_get_w(input)));
		return vector_set(x, y, z, w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the cosine of the input angle.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_cos(const vector4d& input) RTM_NO_EXCEPT
	{
		scalard x = scalar_cos(scalard(vector_get_x(input)));
		scalard y = scalar_cos(scalard(vector_get_y(input)));
		scalard z = scalar_cos(scalard(vector_get_z(input)));
		scalard w = scalar_cos(scalard(vector_get_w(input)));
		return vector_set(x, y, z, w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the arc-cosine of the input.
	// Input value must be in the range [-1.0, 1.0].
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_acos(const vector4d& input) RTM_NO_EXCEPT
	{
		scalard x = scalar_acos(scalard(vector_get_x(input)));
		scalard y = scalar_acos(scalard(vector_get_y(input)));
		scalard z = scalar_acos(scalard(vector_get_z(input)));
		scalard w = scalar_acos(scalard(vector_get_w(input)));
		return vector_set(x, y, z, w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the tangent of the input angle.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_tan(const vector4d& angle) RTM_NO_EXCEPT
	{
		// Use the identity: tan(angle) = sin(angle) / cos(angle)
		vector4d sin_ = vector_sin(angle);
		vector4d cos_ = vector_cos(angle);

		mask4d is_cos_zero = vector_equal(cos_, vector_zero());
		vector4d signed_infinity = vector_copy_sign(vector_set(std::numeric_limits<double>::infinity()), angle);
		vector4d result = vector_div(sin_, cos_);
		return vector_select(is_cos_zero, signed_infinity, result);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the arc-tangent of the input.
	// Note that due to the sign ambiguity, atan cannot determine which quadrant
	// the value resides in.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_atan(const vector4d& input) RTM_NO_EXCEPT
	{
		scalard x = scalar_atan(scalard(vector_get_x(input)));
		scalard y = scalar_atan(scalard(vector_get_y(input)));
		scalard z = scalar_atan(scalard(vector_get_z(input)));
		scalard w = scalar_atan(scalard(vector_get_w(input)));
		return vector_set(x, y, z, w);
	}

	//////////////////////////////////////////////////////////////////////////
	// Returns per component the arc-tangent of [y/x] using the sign of the arguments to
	// determine the correct quadrant.
	// Y represents the proportion of the y-coordinate.
	// X represents the proportion of the x-coordinate.
	//////////////////////////////////////////////////////////////////////////
	inline vector4d vector_atan2(const vector4d& y, const vector4d& x) RTM_NO_EXCEPT
	{
		scalard x_ = scalar_atan2(scalard(vector_get_x(y)), scalard(vector_get_x(x)));
		scalard y_ = scalar_atan2(scalard(vector_get_y(y)), scalard(vector_get_y(x)));
		scalard z_ = scalar_atan2(scalard(vector_get_z(y)), scalard(vector_get_z(x)));
		scalard w_ = scalar_atan2(scalard(vector_get_w(y)), scalard(vector_get_w(x)));
		return vector_set(x_, y_, z_, w_);
	}
}

RTM_IMPL_FILE_PRAGMA_POP
