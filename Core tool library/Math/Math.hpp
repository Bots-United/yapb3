//
// Copyright © 2003-2010, by YaPB Development Team. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Math.hpp
//
// Namespace: Math
//
// Description: Some math utility functions.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MATH_INCLUDED
#define MATH_INCLUDED

namespace Math
{
	#include <math.h>	// Standard math header file

	class Degree;
	class Angles2D;
	class Angles3D;
	class Vector3D;
	class Extent2D;
	class Extent3D;

	#define MATH_GET_SQUARED(value) (value * value)
	#define MATH_GET_CUBIC(value) (MATH_GET_SQUARED (value) * value)

	// Use this definition globally.
	NAMESPACE_GLOBAL static const float Epsilon (1.192092896e-7f);	// Smallest such that 1.0f + Epsilon != 1.0f (from float.h)
	NAMESPACE_GLOBAL static const float MaximumFloatValue (3.402823466e+38f);	// Maximum float value (from float.h)
	NAMESPACE_GLOBAL static const float ZeroTolerance (1e-6f);
	NAMESPACE_GLOBAL static const float ZeroToleranceSquared (MATH_GET_SQUARED (ZeroTolerance));
	NAMESPACE_GLOBAL static const float RawEpsilon (0.1f);
	NAMESPACE_GLOBAL static const float OnEpsilon (0.01f);
	NAMESPACE_GLOBAL static const float EqualEpsilon (0.001f);
//	NAMESPACE_GLOBAL static const double Pi (3.141592653589793238462643383279502884197169399375105820974944);
	NAMESPACE_GLOBAL static const float Pi (4.0f * atanf (1.0f));//(static_cast <float> (4.0 * atan (1.0)));
	NAMESPACE_GLOBAL static const float TwoPi (2.0f * Pi);
	NAMESPACE_GLOBAL static const float Rad2Deg (180.0f / Pi);
	NAMESPACE_GLOBAL static const float Deg2Rad (Pi / 180.0f);

	template <typename elementType> struct Digit
	{
		typedef elementType Type_t;

		static const bool IsInteger;

		static const Type_t GetMinimum (void);
		static const Type_t GetMaximum (void);
	};
	template <typename elementType> struct Digit <const elementType> : public Digit <elementType> { /* Empty */ };	// numeric limits for const types
	template <typename elementType> struct Digit <volatile elementType> : public Digit <elementType> { /* Empty */ };	// numeric limits for volatile types
	template <typename elementType> struct Digit <const volatile elementType> : public Digit <elementType> { /* Empty */ };	// numeric limits for const volatile types
	template </* char */> struct Digit <char>
	{
		typedef char Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1; }						// minimum (signed) char value
		static const Type_t GetMaximum (void) { return static_cast <unsigned char> (-1) / 2u; }	// maximum (signed) char value
	};
	template </* unsigned char */> struct Digit <unsigned char>
	{
		typedef unsigned char Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1u; }		// minimum unsigned char value
		static const Type_t GetMaximum (void) { return static_cast <Type_t> (-1); }	// maximum unsigned char value
	};
	template </* short */> struct Digit <short>
	{
		typedef short Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1; }							// minimum (signed) short value
		static const Type_t GetMaximum (void) { return static_cast <unsigned short> (-1) / 2u; }	// maximum (signed) short value
	};
	template </* unsigned short */> struct Digit <unsigned short>
	{
		typedef unsigned short Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1u; }		// minimum unsigned short value
		static const Type_t GetMaximum (void) { return static_cast <Type_t> (-1); }	// maximum unsigned short value
	};
	template </* int */> struct Digit <int>
	{
		typedef int Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1; }						// minimum (signed) int value
		static const Type_t GetMaximum (void) { return static_cast <unsigned int> (-1) / 2u; }	// maximum (signed) int value
	};
	template </* unsigned int */> struct Digit <unsigned int>
	{
		typedef unsigned int Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1u; }		// minimum unsigned int value
		static const Type_t GetMaximum (void) { return static_cast <Type_t> (-1); }	// maximum unsigned int value
	};
	template </* long */> struct Digit <long>
	{
		typedef long Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1; }						// minimum (signed) long int value
		static const Type_t GetMaximum (void) { return static_cast <unsigned long> (-1) / 2u; }	// maximum (signed) long int value
	};
	template </* unsigned long */> struct Digit <unsigned long>
	{
		typedef unsigned long Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1u; }		// minimum unsigned long value
		static const Type_t GetMaximum (void) { return static_cast <Type_t> (-1); }	// maximum unsigned long value
	};
	template </* long long */> struct Digit <long long>
	{
		typedef long long Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1; }								// minimum (signed) long long int value
		static const Type_t GetMaximum (void) { return static_cast <unsigned long long> (-1) / 2u; }	// maximum (signed) long long int value
	};
	template </* unsigned long long */> struct Digit <unsigned long long>
	{
		typedef unsigned long long Type_t;

		static const bool IsInteger = true;

		static const Type_t GetMinimum (void) { return GetMaximum () + 1u; }
		static const Type_t GetMaximum (void) { return static_cast <Type_t> (-1); }
	};

	template <typename elementType> inline elementType &Square (elementType &value) { return value = MATH_GET_SQUARED (value); }

	template <typename elementType> inline const elementType GetMA (const elementType &source, const elementType &direction, const float scale)
	{
		return source + direction * scale;
	}
	template <typename elementType> inline elementType &MA (elementType &element, const elementType &source, const elementType &direction, const float scale)
	{
		return element = GetMA (source, direction, scale);
	}
	template <typename elementType> inline elementType &MA (elementType &source, const elementType &direction, const float scale)
	{
		return MA (source, source, direction, scale);
	}

	//
	// Function: GetInterpolated
	//
	// Description: Gets a interpolated element between source element and destination element.
	//
	// Parameters:
	//	source - First element to interpolate with, maximum at 1.0.
	//	destination - Second element to interpolate with, maximum at 0.0.
	//	fraction - Interpolation value between 0.0 (all the destination element) and 1.0 (all source element).
	//
	// Returns: Interpolated element.
	//
	template <typename elementType> inline const elementType GetInterpolated (const elementType &source, const elementType &destination, const float fraction)
	{
		const elementType delta (destination - source);

		return source + delta * fraction;
	}

	//
	// Function: Interpolate
	//
	// Description: Sets 'element' element to the linearly interpolated element between source and destination.
	//
	// Parameters:
	//	element - Element to interpolate.
	//	source - First element to interpolate with, maximum at 1.0.
	//	destination - Second element to interpolate with, maximum at 0.0.
	//	fraction - Interpolation value between 0.0 (all element destination) and 1.0 (all element source).
	//
	// Returns: Interpolated element.
	//
	template <typename elementType> inline elementType &Interpolate (elementType &element, const elementType &source, const elementType &destination, const float fraction)
	{
		return element = GetInterpolated (source, destination, fraction);
	}

	template <typename elementType> inline const elementType GetMidPoint (const elementType &source, const elementType &destination)
	{
		return (source + destination) * 0.5f;
	}
	template <typename elementType> inline elementType &SetMidPoint (elementType &source, const elementType &destination)
	{
		return source = GetMidPoint (source, destination);
	}
	template <typename elementType> inline const elementType GetExtent (const elementType &source, const elementType &destination)
	{
		return (destination - source) * 0.5f;
	}

	template <typename elementType> inline const elementType GetRounded (const float value)
	{
		// Return integer part of float, rounded.

		// standard, round to nearest
		return static_cast <elementType> (floorf (value + 0.5f));
	}

	//
	// Function: RadianToDegree
	//
	// Description: Converts radians to degrees.
	//
	// Parameters:
	//	radian - Input radian.
	//
	// Returns: Degree converted from radian.
	//
	// See Also:
	//	<DegreeToRadian>
	//
	inline const float RadianToDegree (const float radian) { return radian * Rad2Deg; }

	//
	// Function: DegreeToRadian
	//
	// Description: Converts degrees to radians.
	//
	// Parameters:
	//	degree - Input degree.
	//
	// Returns: Radian converted from degree.
	//
	// See Also:
	//	<RadianToDegree>
	//
	inline const float DegreeToRadian (const float degree) { return degree * Deg2Rad; }

	//
	// Function: IsZero
	//
	// Description: Checks whether specified float point is zero.
	//
	// Parameters:
	//	entry - Input float.
	//
	// Returns: True if entry is zero, false otherwise.
	//
	inline const bool IsZero (const float entry, const float tolerance = OnEpsilon/*ZeroTolerance*/) { return fabsf (entry) < tolerance; }

	//
	// Function: AreEqual
	//
	// Description: Checks whether two floats are equal.
	//
	// Parameters:
	//	entry1 - First entry.
	//	entry2 - Second entry
	//
	// Returns: True if entries are equal, false otherwise.
	//
	inline const bool AreEqual  (const float entry1, const float entry2, const float tolerance = EqualEpsilon) { return fabsf (entry1 - entry2) < tolerance; }
	inline const bool AreEqual2 (const float entry1, const float entry2, const float tolerance = EqualEpsilon) { return fabsf (entry1 - entry2) <= tolerance; }

	//
	// Function: AngleMod
	//
	// Description: Adds or subtracts 360.0 enough times need to given angle in order to set it into the range [0.0, 360.0).
	//
	// Parameters:
	//	angle - Input angle.
	//
	// Returns: Resulting angle.
	//
	inline const float AngleMod (const float angle)
	{
//		const unsigned int bits (BIT (16u));	// 65536
//		const unsigned int bits (BIT (31u));	// 2147483648
		const unsigned long long bits (BIT64 (32LU));	// 4294967296

//		return 360.0f / bits * (static_cast <int> (bits / 360.0f * angle) & (bits - 1U));
		return static_cast <float> (360.0 / bits * (static_cast <long long> (bits / 360.0 * angle) & (bits - 1LU)));
/*		angle = fmodf (angle, 360.0f);

		// check for wrap around of angle...
		if (angle < 0.0f)
			angle += 360.0f;
		else if (angle > 360.0f)
			angle -= 360.0f;

		return angle;*/
	}

	//
	// Function: AngleNormalize
	//
	// Description: Adds or subtracts 360.0 enough times need to given angle in order to set it into the range [-180.0, 180.0).
	//
	// Parameters:
	//	angle - Input angle.
	//
	// Returns: Resulting angle.
	//
	inline const float PMBAngleNormalize (float angle)
	{
		// this function adds or substracts 360 enough times needed to angle to clamp it into the [-180, 180] bounds.

		if (angle < -180.0f)
			angle += 360 * ::abs ((static_cast <int> (angle) - 180) / 360);
		else if (angle >= 180.0f)
			angle -= 360 * ::abs ((static_cast <int> (angle) + 180) / 360);

		if (angle == 180.0f)
			angle = -180.0f;	// needs 2nd pass to check for floating-point rounding errors

		return angle;	// finished
	}
	inline const float KORAXAngleNormalize (float angle)
	{
		if (angle >= 180.0f)
			angle -= 360 * ((static_cast <int> (angle) + 180) / 360);

		if (angle < -180.0f)
			angle -= 360 * ((static_cast <int> (angle) - 180) / 360);

		return angle;	// finished
	}
	inline const float JKAngleNormalize (float angle)
	{
		// this function returns an angle normalized to the range [-180 < angle <= 180]

		const unsigned int bits (BIT (31u));	// 2147483648

		angle = static_cast <float> (360.0 / bits * (static_cast <long long> (bits / 360.0 * (angle + 180.0)) & (bits - 1u)) - 180.0);
//		angle = AngleMod (angle + 180.0f) - 180.0f;

		if (angle == -180.0f)
			angle = 180.0f;

		return angle;
	}
	inline const float HL2AngleNormalize (float angle)
	{
		angle = fmodf (angle, 360.0f);

		// check for wrap around of angle...
		if (angle < -180.0f)
			angle += 360.0f;
		else if (angle > 180.0f)
			angle -= 360.0f;

		return angle;
	}
	inline const float AngleNormalize (const float angle)
	{
//		return 360.0f / 65536.0f * (static_cast <int> (65536.0f / 360.0f * (angle + 180.0f)) & 65535u) - 180.0f;
		return HL2AngleNormalize (angle);
	}

	inline const float GetDifferenceBetweenAngles (const float destinationAngle, const float sourceAngle)
	{
		return AngleNormalize (destinationAngle - sourceAngle);
	}

	//
	// Function: GetCosineSine
	//
	// Description: Very fast platform-dependent sine and cosine calculation routine.
	//
	// Parameters:
	//	radians - Input degree (angle).
	//	cosine - Output for Cosine.
	//	sine - Output for Sine.
	//
	inline void GetCosineSine (const float radians, float &cosine, float &sine)
	{
		__asm
		{
			fld  dword ptr [radians]	// Load Real
			fsincos						// t<-cos(st); st<-sin(st); push t

			mov  edx, dword ptr [cosine]
			mov  eax, dword ptr [sine]

			fstp dword ptr [edx]		// Store Real and Pop
			fstp dword ptr [eax]		// Store Real and Pop
		}
	}

	//
	// Function: GetMinimumValueBetween
	//
	// Description: Returns the lesser of 'a' and 'b'.
	//
	// Remarks: The comparison uses operator <.
	//
	// Parameters:
	//	elementType - Is any type supporting copy constructions and comparisons with operator <.
	//	a - First item to compare.
	//	b - Second item to compare.
	//
	// Returns: The lesser of its two arguments.
	//
	template <typename elementType> inline const elementType &GetMinimumValueBetween (const elementType &a, const elementType &b)
	{
		return a < b ? a : b;
	}

	//
	// Function: GetMaximumValueBetween
	//
	// Description: Returns the greater of 'a' and 'b'.
	//
	// Remarks: The comparison uses operator >.
	//
	// Parameters:
	//	elementType - Is any type supporting copy constructions and comparisons with operator >.
	//	a - First item to compare.
	//	b - Second item to compare.
	//
	// Returns: The greater of its two arguments.
	//
	template <typename elementType> inline const elementType &GetMaximumValueBetween (const elementType &a, const elementType &b)
	{
		return a > b ? a : b;
	}

	template <typename elementType> inline const elementType GetClampedValueInRange (const elementType &value, const elementType &minimumValue, const elementType &maximumValue)
	{
		return value < minimumValue ? minimumValue : value > maximumValue ? maximumValue : value;
	}
	template <typename elementType> inline elementType &ClampValueInRange (elementType &value, const elementType &minimumValue, const elementType &maximumValue)
	{
		if (value < minimumValue)
			value = minimumValue;
		else if (value > maximumValue)
			value = maximumValue;

		return value;
	}

	class Matrix3x4
	{
		public:
			typedef float Pointer_t[3u][4u];

			Pointer_t m_matrixValues;

		public:
			inline Matrix3x4 (void) { /* VOID */ }
			inline Matrix3x4
			(
				float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23
			)
			{
				m_matrixValues[0u][0u] = m00; m_matrixValues[0u][1u] = m01; m_matrixValues[0u][2u] = m02; m_matrixValues[0u][3u] = m03;
				m_matrixValues[1u][0u] = m10; m_matrixValues[1u][1u] = m11; m_matrixValues[1u][2u] = m12; m_matrixValues[1u][3u] = m13;
				m_matrixValues[2u][0u] = m20; m_matrixValues[2u][1u] = m21; m_matrixValues[2u][2u] = m22; m_matrixValues[2u][3u] = m23;
			}

		public:
			inline       float *const GetData (void)       { return m_matrixValues[0u]; }
			inline const float *const GetData (void) const { return m_matrixValues[0u]; }

			inline void ConcatTransforms (const Matrix3x4 &other, Matrix3x4 &out) const
			{
				out[0u][0u] = m_matrixValues[0u][0u] * other[0u][0u] + m_matrixValues[0u][1u] * other[1u][0u] + m_matrixValues[0u][2u] * other[2u][0u];
				out[0u][1u] = m_matrixValues[0u][0u] * other[0u][1u] + m_matrixValues[0u][1u] * other[1u][1u] + m_matrixValues[0u][2u] * other[2u][1u];
				out[0u][2u] = m_matrixValues[0u][0u] * other[0u][2u] + m_matrixValues[0u][1u] * other[1u][2u] + m_matrixValues[0u][2u] * other[2u][2u];
				out[0u][3u] = m_matrixValues[0u][0u] * other[0u][3u] + m_matrixValues[0u][1u] * other[1u][3u] + m_matrixValues[0u][2u] * other[2u][3u] + m_matrixValues[0u][3u];
				out[1u][0u] = m_matrixValues[1u][0u] * other[0u][0u] + m_matrixValues[1u][1u] * other[1u][0u] + m_matrixValues[1u][2u] * other[2u][0u];
				out[1u][1u] = m_matrixValues[1u][0u] * other[0u][1u] + m_matrixValues[1u][1u] * other[1u][1u] + m_matrixValues[1u][2u] * other[2u][1u];
				out[1u][2u] = m_matrixValues[1u][0u] * other[0u][2u] + m_matrixValues[1u][1u] * other[1u][2u] + m_matrixValues[1u][2u] * other[2u][2u];
				out[1u][3u] = m_matrixValues[1u][0u] * other[0u][3u] + m_matrixValues[1u][1u] * other[1u][3u] + m_matrixValues[1u][2u] * other[2u][3u] + m_matrixValues[1u][3u];
				out[2u][0u] = m_matrixValues[2u][0u] * other[0u][0u] + m_matrixValues[2u][1u] * other[1u][0u] + m_matrixValues[2u][2u] * other[2u][0u];
				out[2u][1u] = m_matrixValues[2u][0u] * other[0u][1u] + m_matrixValues[2u][1u] * other[1u][1u] + m_matrixValues[2u][2u] * other[2u][1u];
				out[2u][2u] = m_matrixValues[2u][0u] * other[0u][2u] + m_matrixValues[2u][1u] * other[1u][2u] + m_matrixValues[2u][2u] * other[2u][2u];
				out[2u][3u] = m_matrixValues[2u][0u] * other[0u][3u] + m_matrixValues[2u][1u] * other[1u][3u] + m_matrixValues[2u][2u] * other[2u][3u] + m_matrixValues[2u][3u];
			}

		public:
			inline                    operator       Pointer_t & (void)       { return m_matrixValues; }	// matrix will now automatically convert to 'float **' when needed
			inline                    operator const Pointer_t & (void) const { return m_matrixValues; }	// matrix will now automatically convert to 'const float **const' when needed

			inline       float *const operator [] (const unsigned char i)       { InternalAssert (i < 3u); return m_matrixValues[i]; }
			inline const float *const operator [] (const unsigned char i) const { InternalAssert (i < 3u); return m_matrixValues[i]; }
	};

	#include <Core tool library/Math/Radian.hpp>
	#include <Core tool library/Math/Degree.hpp>
	#include <Core tool library/Math/Vector2D.hpp>
	#include <Core tool library/Math/Vector3D.hpp>
	#include <Core tool library/Math/Angles2D.hpp>
	#include <Core tool library/Math/Angles3D.hpp>
	#include <Core tool library/Math/Triangle3D.hpp>
	#include <Core tool library/Math/Extent3D.hpp>
	#include <Core tool library/Math/Extent2D.hpp>
	#include <Core tool library/Math/Plane.hpp>
	#include <Core tool library/Math/Ray.hpp>
	#include <Core tool library/Math/Winding.hpp>
	#include <Core tool library/Math/Grid2D.hpp>
	#include <Core tool library/Math/Frustum.hpp>

template <const unsigned char axisTotal> class CoordinateMapping
{
	public:
		enum { AxisIndex_Total = axisTotal };

	private:
		unsigned char m_sourceFor[AxisIndex_Total];
		char          m_sign[AxisIndex_Total];

	public:
		inline CoordinateMapping (void)
		{
			// default to identity mapping
			m_sourceFor[0] = 0u;
			m_sourceFor[1] = 1u;
			m_sourceFor[2] = 2u;
			m_sign[0] = 1;
			m_sign[1] = 1;
			m_sign[2] = 1;
		}

	public:
		inline void Swap (const unsigned char axis1, const unsigned char axis2)
		{
			SwapElements (m_sourceFor[axis1], m_sourceFor[axis2]);
			SwapElements (m_sign[axis1], m_sign[axis2]);
		}
		inline void Negate (const unsigned char axis) { m_sign[axis] = -m_sign[axis]; }

		inline void ApplyTo (float *const coords) const
		{
			float result[AxisIndex_Total];
			unsigned char axisIndex;

			for (axisIndex = 0u; axisIndex < AxisIndex_Total; ++axisIndex)
				result[axisIndex] = coords[m_sourceFor[axisIndex]] * m_sign[axisIndex];

			for (axisIndex = 0u; axisIndex < AxisIndex_Total; ++axisIndex)
				coords[axisIndex] = result[axisIndex];
		}
};

	inline const float IsPointLeftOfSegment2D (const Math::Vector2D &segmentSource, const Math::Vector2D &segmentDestination, const Math::Vector2D &point)
	{
		//  IsPointLeftOfSegment2D(): tests if a point is Left|On|Right of an infinite segment.
		//    Input:  three points P0, P1, and P2
		//    Return: >0 for P2 left of the segment through P0 and P1
		//            =0 for P2 on the segment
		//            <0 for P2 right of the segment
		// Returns true if 'point' is left of line 'segmentSource'-'segmentDestination'.

		/// @note ORIGINAL FUNCTION NAME triarea2()

		const Math::Vector2D direction (segmentDestination - segmentSource);
		const Math::Vector2D b (point - segmentSource);

//		return b ^ direction;	/// @note ORIGINAL MIKKO COUNTER-CLOCKWISE WERSION!
		return direction ^ b;	/// @note CLOCKWISE WERSION!
	}

	inline bool IsBoundingBoxesIntersects (const Vector2D &absoluteMins1, const Vector2D &absoluteMaxs1, const Vector2D &absoluteMins2, const Vector2D &absoluteMaxs2)
	{
		return absoluteMins1 < absoluteMaxs2 && absoluteMaxs1 > absoluteMins2;
	}
	inline bool IsBoundingBoxesIntersects (const Vector3D &absoluteMins1, const Vector3D &absoluteMaxs1, const Vector3D &absoluteMins2, const Vector3D &absoluteMaxs2)
	{
		return absoluteMins1 < absoluteMaxs2 && absoluteMaxs1 > absoluteMins2;
	}

	// Returns if the specified two bounding boxes are touching each other.
	inline const bool IsBoundingBoxesTouching (const Vector3D &absoluteMins1, const Vector3D &absoluteMaxs1, const Vector3D &absoluteMins2, const Vector3D &absoluteMaxs2)
	{
		return absoluteMaxs1 >= absoluteMins2 && absoluteMins1 <= absoluteMaxs2;
	}
	inline const bool IsBoundingBoxesTouching (const Extent3D &extent1, const Vector3D &absoluteMins2, const Vector3D &absoluteMaxs2)
	{
		return IsBoundingBoxesTouching (extent1.mins, extent1.maxs, absoluteMins2, absoluteMaxs2);
	}
	inline const bool IsBoundingBoxesTouching (const Vector3D &absoluteMins1, const Vector3D &absoluteMaxs1, const Extent3D &extent2)
	{
		return IsBoundingBoxesTouching (absoluteMins1, absoluteMaxs1, extent2.mins, extent2.maxs);
	}
	inline const bool IsBoundingBoxesTouching (const Extent3D &extent1, const Extent3D &extent2)
	{
		return IsBoundingBoxesTouching (extent1.mins, extent1.maxs, extent2.mins, extent2.maxs);
	}
	inline const bool IsBoundingBoxesTouching (const Vector3D &origin1, const Vector3D &absoluteMins1, const Vector3D &absoluteMaxs1, const Vector3D &origin2, const Vector3D &absoluteMins2, const Vector3D &absoluteMaxs2)
	{
		return IsBoundingBoxesTouching (origin1 + absoluteMins1, origin1 + absoluteMaxs1, origin2 + absoluteMins2, origin2 + absoluteMaxs2);
	}
}

#endif	// ifndef MATH_INCLUDED