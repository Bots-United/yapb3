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
// Vector2D.hpp
//
// Class: Vector2D
//
// Description: Vector2D - used for many pathfinding and many other operations that are treated as planar rather than Vector3D.
//
// Warning: DO NOT CHANGE MEMBERS OF THIS CLASS AND ALSO DO NOT MAKE THIS CLASS AS VIRTUAL!!! (THIS CLASS USED BY HL ENGINE)
//
// Link: http://developer.valvesoftware.com/wiki/Vector
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef VECTOR_2D_INCLUDED
#define VECTOR_2D_INCLUDED

class Vector2D
{
	//
	// Group: Constants.
	//
	public:
		enum AxisID_t
		{
			AxisID_X,	// +forward/-backward
			AxisID_Y,	// +left/-right

			AxisID_Total
		};
		DECLARE_TYPED_ENUMERATION (unsigned char, AxisBit_t)
		{
			AxisBit_None,

			AxisBit_X = BIT (AxisID_X),
			AxisBit_Y = BIT (AxisID_Y),

			AxisBit_All = AxisBit_X | AxisBit_Y
		};

		typedef float Pointer_t[AxisID_Total];

		typedef AxisBit_t SignBitsType_t;

	//
	// Group: X and Y axis members.
	//
	public:
		float x, y;	// horizontal

	//
	// Group: Special points.
	//
	public:
		CLASS_GLOBAL static const Vector2D ZeroValue;	// Zero vector value.

		CLASS_GLOBAL static const Vector2D UnitX;
		CLASS_GLOBAL static const Vector2D UnitY;
		CLASS_GLOBAL static const Vector2D NegativeUnitX;
		CLASS_GLOBAL static const Vector2D NegativeUnitY;
		CLASS_GLOBAL static const Vector2D UnitScale;

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: Vector2D
		//
		// Description: Constructs Vector2D from float, and assign it's value to all axises.
		//
		// Parameters:
		//	scaler - Value for axises.
		//
		inline const Vector2D (const float scaler = 0.0f) : x (scaler), y (scaler) { /*VOID*/ }

		//
		// Function: Vector2D
		//
		// Description: Standard Vector2D Constructor.
		//
		// Parameters:
		//	x - Input X axis.
		//	y - Input Y axis.
		//
		inline const Vector2D (const float inputX, const float inputY) : x (inputX), y (inputY) { /*VOID*/ }

		//
		// Function: Vector2D
		//
		// Description: Constructs Vector2D from float pointer.
		//
		// Parameters:
		//	vector2D - Float pointer.
		//
		inline const Vector2D (const float *const vector2D) : x (vector2D[AxisID_X]), y (vector2D[AxisID_Y]) { /*VOID*/ }

		//
		// Function: Vector2D
		//
		// Description: Constructs Vector2D from another Vector2D.
		//
		// Parameters:
		//	right - Other Vector2D, that should be assigned.
		//
		inline const Vector2D (const Vector2D &right) : x (right.x), y (right.y) { /*VOID*/ }

		//
		// Function: Vector2D
		//
		// Description: Constructs Vector2D from Vector3D.
		//
		// Parameters:
		//	right - Vector3D, that should be assigned.
		//
		// Note: This function declared in 'Core tool library/Vector3D.hpp'.
		//
		inline Vector2D (const Vector3D &right);

	//
	// Group: Operators.
	//
	public:
		// Equality.
		inline const bool             operator ==                 (const Vector2D &right)                     const { return x == right.x && y == right.y; }
		inline const bool             operator ==                 (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.
		inline const bool             operator !=                 (const Vector2D &right)                     const { return x != right.x || y != right.y; }
		inline const bool             operator !=                 (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		//
		// Function: operator <
		//
		// Description: Checks whether vector's all scalar components are smaller that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the vector's all scalar components are smaller that the value it is compared against, false otherwise.
		//
		inline const bool             operator <                  (const float value)                         const { return x < value && y < value; }

		//
		// Function: operator <
		//
		// Description: Checks whether vector's scalar components are all smaller that the ones of the vector it is compared against.
		//
		// Parameters:
		//	right - Other vector to compare with.
		//
		// Returns: Returns true if the vector's scalar components are all smaller that the ones of the vector it is compared against, false otherwise.
		//
		inline const bool             operator <                  (const Vector2D &right)                     const { return x < right.x && y < right.y; }
		inline const bool             operator <                  (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		//
		// Function: operator <=
		//
		// Description: Checks whether vector's all scalar components are smaller or equal that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the vector's all scalar components are smaller or equal that the value it is compared against, false otherwise.
		//
		inline const bool             operator <=                 (const float value)                         const { return x <= value && y <= value; }

		//
		// Function: operator <=
		//
		// Description: Checks whether vector's scalar components are all smaller or equal that the ones of the vector it is compared against.
		//
		// Parameters:
		//	right - Other vector to compare with.
		//
		// Returns: Returns true if the vector's scalar components are all smaller or equal that the ones of the vector it is compared against, false otherwise.
		//
		inline const bool             operator <=                 (const Vector2D &right)                     const { return x <= right.x && y <= right.y; }
		inline const bool             operator <=                 (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		//
		// Function: operator >
		//
		// Description: Checks whether vector's all scalar components are greater that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the vector's all scalar components are greater that the value it is compared against, false otherwise.
		//
		inline const bool             operator >                  (const float value)                         const { return x > value && y > value; }

		//
		// Function: operator >
		//
		// Description: Checks whether vector's scalar components are all greater that the ones of the vector it is compared against.
		//
		// Parameters:
		//	right - Other vector to compare with.
		//
		// Returns: Returns true if the vector's scalar components are all greater that the ones of the vector it is compared against, false otherwise.
		//
		inline const bool             operator >                  (const Vector2D &right)                     const { return x > right.x && y > right.y; }
		inline const bool             operator >                  (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		//
		// Function: operator >=
		//
		// Description: Checks whether vector's all scalar components are greater or equal that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the vector's all scalar components are greater or equal that the value it is compared against, false otherwise.
		//
		inline const bool             operator >=                 (const float value)                         const { return x >= value && y >= value; }

		//
		// Function: operator >=
		//
		// Description: Checks whether vector's scalar components are all greater or equal that the ones of the vector it is compared against.
		//
		// Parameters:
		//	right - Other vector to compare with.
		//
		// Returns: Returns true if the vector's scalar components are all greater or equal that the ones of the vector it is compared against, false otherwise.
		//
		inline const bool             operator >=                 (const Vector2D &right)                     const { return x >= right.x && y >= right.y; }
		inline const bool             operator >=                 (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		// array access
		inline float                 &operator []                 (const AxisID_t axisID)
		{
			// Reliability check.
			InternalAssert (axisID < AxisID_Total);

			return (&x)[axisID];
		}
		inline const float           &operator []                 (const AxisID_t axisID)                     const
		{
			// Reliability check.
			InternalAssert (axisID < AxisID_Total);

			return (&x)[axisID];
		}

		// casting operators
		inline                        operator       float *const (void)                                            { return &x; }	// vectors will now automatically convert to 'float *' when needed
		inline                        operator const float *const (void)                                      const { return &x; }	// vectors will now automatically convert to 'const float *const' when needed
//		inline                        operator       Vector3D     (void)                                      const;				// vectors will now automatically convert to 'Vector3D' when needed
//		inline                        operator const Vector3D     (void)                                      const;				// vectors will now automatically convert to 'const Vector3D' when needed

		// arithmetic operations
		inline const Vector2D         operator +                  (const float scaler)                        const { return Vector2D (x + scaler, y + scaler); }
		inline const Vector2D         operator +                  (const Vector2D &right)                     const { return Vector2D (x + right.x, y + right.y); }
		inline const Vector2D         operator +                  (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.
		inline       Vector2D        &operator +=                 (const float scaler)
		{
			x += scaler;
			y += scaler;

			return *this;
		}
		inline       Vector2D        &operator +=                 (const Vector2D &right)
		{
			x += right.x;
			y += right.y;

			return *this;
		}

		inline const Vector2D         operator -                  (const float scaler)                        const { return Vector2D (x - scaler, y - scaler); }
		inline       Vector2D         operator -                  (const Vector2D &right)                     const { return Vector2D (x - right.x, y - right.y); }
		inline const Vector2D         operator -                  (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.
		inline       Vector2D        &operator -=                 (const float scaler)
		{
			x -= scaler;
			y -= scaler;

			return *this;
		}
		inline       Vector2D        &operator -=                 (const Vector2D &right)
		{
			x -= right.x;
			y -= right.y;

			return *this;
		}
		inline       Vector2D        &operator -=                 (const Vector3D &right);	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		inline const Vector2D         operator *                  (const float scaler)                        const { return Vector2D (x * scaler, y * scaler); }
		inline const Vector2D         operator *                  (const Vector2D &right)                     const { return Vector2D (x * right.x, y * right.y); }
		inline const Vector2D         operator *                  (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.
		friend inline const Vector2D  operator *                  (const float scaler, const Vector2D &right)       { return right * scaler; }
		inline       Vector2D        &operator *=                 (const float scaler)
		{
			x *= scaler;
			y *= scaler;

			return *this;
		}
		inline       Vector2D        &operator *=                 (const Vector2D &right)
		{
			x *= right.x;
			y *= right.y;

			return *this;
		}
		inline       Vector2D        &operator *=                 (const Vector3D &right);	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		inline const Vector2D         operator /                  (const float scaler)                        const
		{
			// Reliability check.
//			InternalAssert (scaler != 0.0f);

			return *this * (1.0f / scaler);
		}
		inline const Vector2D         operator /                  (const Vector2D &right)                     const
		{
			// Reliability check.
			InternalAssert (right.x != 0.0f && right.y != 0.0f);

			return Vector2D (x / right.x, y / right.y);
		}
		inline const Vector2D         operator /                  (const Vector3D &right)                     const;	// Note: This function declared in 'Core tool library/Vector3D.hpp'.
		inline       Vector2D        &operator /=                 (const float scaler)
		{
			// Reliability check.
//			InternalAssert (scaler != 0.0f);

			return *this *= 1.0f / scaler;
		}
		inline       Vector2D        &operator /=                 (const Vector2D &right)
		{
			// Reliability check.
			InternalAssert (right.x != 0.0f && right.y != 0.0f);

			x /= right.x;
			y /= right.y;

			return *this;
		}
		inline       Vector2D        &operator /=                 (const Vector3D &right);	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

		//
		// Function: operator |
		//
		// Description: Calculates the dot (scalar) product of this vector with another.
		//
		// Remarks:
		//	The dot product can be used to calculate the angle between 2
		//	vectors. If both are unit vectors, the dot product is the
		//	cosine of the angle; otherwise the dot product must be
		//	divided by the product of the lengths of both vectors to get
		//	the cosine of the angle. This result can further be used to
		//	calculate the distance of a point from a plane.
		//
		// Parameters:
		//	right - Vector with which to calculate the dot product (together with this one).
		//
		// Returns: A float representing the dot product value.
		//
		inline const float            operator |                  (const Vector2D &right)                     const { return GetDotProduct (right); }

		inline const float            operator ^                  (const Vector2D &right)                     const { return GetCrossProduct (right); }

		inline       Vector2D        &operator ~                  (void)                                            { return Normalize (); }

		inline const Vector2D         operator -                  (void)                                      const { return GetInverted (); }

		// assignment

		//
		// Function: operator =
		//
		// Description: Assigns the value to existing vector.
		//
		// Parameters:
		//	value - Value that should be assigned.
		//
		// Returns: Reassigned vector.
		//
		inline Vector2D              &operator =                  (const float value)
		{
			x = y = value;

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current vector with float pointer.
		//
		// Parameters:
		//	vector2D - Float pointer that should be assigned.
		//
		// Returns: Reassigned vector.
		//
		inline Vector2D              &operator =                  (const float *const vector2D)
		{
			// Reliability check.
			InternalAssert (vector2D != NULL);

			x = vector2D[AxisID_X];
			y = vector2D[AxisID_Y];

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current vector with specified one.
		//
		// Parameters:
		//	right - Other vector that should be assigned.
		//
		// Returns: Reassigned vector.
		//
		inline Vector2D              &operator =                  (const Vector2D &right)
		{
			x = right.x;
			y = right.y;

			return *this;
		}

		inline Vector2D              &operator =                  (const Vector3D &right);	// Note: This function declared in 'Core tool library/Vector3D.hpp'.

	//
	// Group: Methods.
	//
	public:
		//
		// Function: GetDotProduct
		//
		// Description: Calculates the dot (scalar) product of this vector with another.
		//
		// Remarks:
		//	The dot product can be used to calculate the angle between 2
		//	vectors. If both are unit vectors, the dot product is the
		//	cosine of the angle; otherwise the dot product must be
		//	divided by the product of the lengths of both vectors to get
		//	the cosine of the angle. This result can further be used to
		//	calculate the distance of a point from a plane.
		//
		// Parameters:
		//	right - Vector with which to calculate the dot product (together with this one).
		//
		// Returns: A float representing the dot product value.
		//
		inline const float GetDotProduct (const Vector2D &other) const { return x * other.x + y * other.y; }

		//
		// Function: GetAbsoluteDotProduct
		//
		// Description: Calculates the absolute dot (scalar) product of this vector with another.
		//
		// Remarks:
		//	This function work similar GetDotProduct, except it use absolute value of each component of the vector to computing.
		//
		// Parameters:
		//	right - Vector with which to calculate the absolute dot product (together with this one).
		//
		// Returns: A float representing the absolute dot product value.
		//
		inline const float GetAbsoluteDotProduct (const Vector2D &other) const { return fabsf (x * other.x) + fabsf (y * other.y); }

		/** Calculates the 2 dimensional cross-product of 2 vectors, which results
			in a single floating point value which is 2 times the area of the triangle.
		*/
		inline const float GetCrossProduct (const Vector2D &right) const { return x * right.y - y * right.x; }

		//
		// Function: GetMidPoint
		//
		// Description: Gets half way between this and the passed in vector.
		//
		// Parameters:
		//	vector - Other vector, to calculate the half way between this one.
		//
		// Returns: A vector at a point half way between this and the passed in vector.
		//
		inline Vector2D GetMidPoint (const Vector2D &vector) const
		{
			return Math::GetMidPoint (*this, vector);
		}
		inline Vector2D &SetMidPoint (const Vector2D &vector)
		{
			return Math::SetMidPoint (*this, vector);
		}
		inline Vector2D GetExtent (const Vector2D &vector) const
		{
			return Math::GetExtent (*this, vector);
		}
		inline const bool AreEqual (const Vector2D &other) const
		{
			return Math::AreEqual (x, other.x) && Math::AreEqual (y, other.y);
		}
		inline const bool AreEqual (const Vector2D &other, const float tolerance) const
		{
			return Math::AreEqual (x, other.x, tolerance) && Math::AreEqual (y, other.y, tolerance);
		}
		inline const bool IsPointBelongsToSegment (const Vector2D &segmentDestination, const Vector2D &point) const
		{
			return GetDistance (segmentDestination) == GetDistance (point) + segmentDestination.GetDistance (point);
//			return GetDistanceSquared (segmentDestination) == GetDistanceSquared (point) + segmentDestination.GetDistanceSquared (point);	/// @warning BUGS OCCURS!!!!!!!
//			return AreEqual (GetDistanceSquared (segmentDestination), GetDistanceSquared (point) + segmentDestination.GetDistanceSquared (point), ZeroTolerance);
		}

		//
		// Function: Swap
		//
		// Description: Exchange the contents of this vector with another.
		//
		// Parameters:
		//	vector - Other vector, to swap with this one.
		//
		inline void Swap (Vector2D &other)
		{
			CoreToolLibrary::SwapElements (*this, other);
		}

		//
		// Function: GetLength
		//
		// Description: Gets length (magnitude) of vector.
		//
		// Warning:
		//	This operation requires a square root and is expensive in terms of CPU operations.
		//	If you don't need to know the exact length (e.g. for just comparing lengths) use GetLengthSquared() instead.
		//
		// Returns: Length (magnitude) of the vector.
		//
		inline const float GetLength (void) const { return sqrtf (GetLengthSquared ()); }

		//
		// Function: GetDistance
		//
		// Description: Gets distance to another vector.
		//
		// Warning:
		//	This operation requires a square root and is expensive in
		//	terms of CPU operations. If you don't need to know the exact
		//	distance (e.g. for just comparing distances) use GetDistanceSquared()
		//	instead.
		//
		// Parameters:
		//	other - Vector which, together with this one, will be used to calculate the squared distance.
		//
		// Returns: Distance to another vector.
		//
		inline const float GetDistance (const Vector2D &other) const
		{
			return (other - *this).GetLength ();
		}

		//
		// Function: GetLengthSquared
		//
		// Description: Gets squared length (magnitude) of vector.
		//
		// Remarks:
		//	This  method is for efficiency - calculating the actual
		//	length of a vector requires a square root, which is expensive
		//	in terms of the operations required. This method returns the
		//	squared length of the vector, i.e. the same as the
		//	length but before the square root is taken. Use this if you
		//	want to find the longest/shortest vector without incurring
		//	the square root.
		//
		// Returns: Squared length (magnitude) of the 2D vector.
		//
		inline const float GetLengthSquared (void) const { return MATH_GET_SQUARED (x) + MATH_GET_SQUARED (y); }

		//
		// Function: GetDistanceSquared
		//
		// Description: Gets squared distance to another vector.
		//
		// Remarks:
		//	This method is for efficiency - calculating the actual
		//	distance to another vector requires a square root, which is
		//	expensive in terms of the operations required. This method
		//	returns the squared distance to another vector, i.e.
		//	the same as the distance but before the square root is taken.
		//	Use this if you want to find the longest / shortest distance
		//	without incurring the square root.
		//
		// Parameters:
		//	other - Vector which, together with this one, will be used to calculate the squared distance.
		//
		// Returns: Squared distance to another vector.
		//
		inline const float GetDistanceSquared (const Vector2D &other) const
		{
			return (other - *this).GetLengthSquared ();
		}

		//
		// Function: Invert
		//
		// Description: Inverts the vector.
		//
		// Returns: Inverted vector.
		//
		inline Vector2D &Invert (void)
		{
			return *this = GetInverted ();
		}
		inline const Vector2D GetInverted (void) const
		{
			return Vector2D (-x, -y);
		}

		// Returns if the specified point is inside the bounding box specified by origin, mins, maxs.
		inline const bool IsInsideBoundingBox (const Vector2D &origin, const Vector2D &mins, const Vector2D &maxs) const
		{
			const Vector2D delta (origin - *this);

			return delta >= mins && delta <= maxs;
		}
		inline const bool IsInsideBoundingBox (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs) const
		{
			return *this >= absoluteMins && *this <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Vector3D &absoluteMins, const Vector3D &absoluteMaxs) const
		{
			return *this >= absoluteMins && *this <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs, const float tolerance) const
		{
			return *this + tolerance >= absoluteMins && *this - tolerance <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Vector3D &absoluteMins, const Vector3D &absoluteMaxs, const float tolerance) const
		{
			return *this + tolerance >= absoluteMins && *this - tolerance <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Extent2D &extent) const;							// Note: This function declared in 'Core tool library\Extent2D.hpp'.
		inline const bool IsInsideBoundingBox (const Extent2D &extent, const float tolerance) const;	// Note: This function declared in 'Core tool library\Extent2D.hpp'.
		inline const bool IsInsideBoundingBox (const Extent3D &extent) const;							// Note: This function declared in 'Core tool library\Extent3D.hpp'.
		inline const bool IsInsideBoundingBox (const Extent3D &extent, const float tolerance) const;	// Note: This function declared in 'Core tool library\Extent3D.hpp'.

		//
		// Function: GetInterpolated
		//
		// Description: Gets a interpolated vector between this vector and another vector.
		//
		// Parameters:
		//	other - Vector to interpolate between.
		//	fraction - Interpolation value between 0.0 (all the other vector) and 1.0 (all this vector).
		//
		// Returns: Interpolated vector.
		//
		inline const Vector2D GetInterpolated (const Vector2D &other, const float fraction) const { return Math::GetInterpolated (*this, other, fraction); }

		//
		// Function: Interpolate
		//
		// Description: Sets this vector to the linearly interpolated vector between source and destination.
		//
		// Parameters:
		//	source - First vector to interpolate with, maximum at 1.0.
		//	destination - Second vector to interpolate with, maximum at 0.0.
		//	fraction - Interpolation value between 0.0 (all vector destination) and 1.0 (all vector source).
		//
		// Returns: Interpolated vector.
		//
		inline Vector2D &Interpolate (const Vector2D &source, const Vector2D &destination, const float fraction) { return Math::Interpolate (*this, source, destination, fraction); }

		//
		// Function: GetNormalized
		//
		// Description: Gets normalized 2D vector.
		//
		// Remarks: This method normalises the vector such that it's length / magnitude is 1.0f. The result is called a unit vector.
		//
		// Note: This function will not crash for zero-sized vectors, but there will be no changes made to their components.
		//
		// Returns: Normalized 2D vector.
		//
		inline const Vector2D GetNormalized (void) const
		{
			// 'Epsilon' is added to the radius to eliminate the possibility of divide by zero.
			return *this / (GetLength ()/* + Epsilon*/);
		}

		//
		// Function: Normalize
		//
		// Description: Normalizes a vector.
		//
		// Remarks: This method normalises the vector such that it's length / magnitude is 1.0f. The result is called a unit vector.
		//
		// Note: This function will not crash for zero-sized vectors, but there will be no changes made to their components.
		//
		// Returns: The previous length of the 2D vector.
		//
		inline Vector2D &Normalize (void)
		{
			// 'Epsilon' is added to the radius to eliminate the possibility of divide by zero.
			return *this /= GetLength ()/* + Epsilon*/;
		}
/*		inline const float Normalize (void)
		{
			const float length = GetLength ();

			*this = GetNormalized ();

			return length;
		}*/

		//
		// Function: IsZero
		//
		// Description: Checks whether vector axises are zero.
		//
		// Returns: True if this vector is (0.0f, 0.0f) within tolerance, false otherwise.
		//
		inline const bool IsZero (void) const
		{
			return Math::IsZero (x) && Math::IsZero (y);
		}

		//
		// Function: ToYaw
		//
		// Description: Converts a spatial location determined by the vector passed into an absolute Y angle (yaw) from the origin of the world.
		//
		// Returns: Yaw angle.
		//
		inline const float ToYaw (void) const
		{
			if (IsZero ())
				return 0.0f;

			return RadianToDegree (atan2f (y, x));
		}

		inline Angles2D ToAngles2D (void) const;

		//
		// Function: GetMinimum
		//
		// Description: Gets a vector with the minimum in X and Y.
		//
		// Returns: Minimum vector in X and Y.
		//
		inline const float GetMinimum (void) const
		{
			return GetMinimumValueBetween (x, y);
		}
		inline const Vector2D GetMinimum (const Vector2D &other) const
		{
			return Vector2D (GetMinimumValueBetween (x, other.x), GetMinimumValueBetween (y, other.y));
		}
		inline Vector2D &SetMinimum (const Vector2D &other) { return *this = GetMinimum (other); }

		//
		// Function: GetMaximum
		//
		// Description: Gets a vector with the maximum in X and Y.
		//
		// Returns: Maximum vector in X and Y.
		//
		inline const float GetMaximum (void) const
		{
			return GetMaximumValueBetween (x, y);
		}
		inline const Vector2D GetMaximum (const Vector2D &other) const
		{
			return Vector2D (GetMaximumValueBetween (x, other.x), GetMaximumValueBetween (y, other.y));
		}
		inline Vector2D &SetMaximum (const Vector2D &other) { return *this = GetMaximum (other); }

		inline Vector2D &RotateZ (const float yaw)
		{
			// Rotate a vector around the Z axis (yaw).

			return *this = GetRotatedZ (yaw);
		}
		inline const Vector2D GetRotatedZ (const float yaw) const
		{
			// Rotate a vector around the Z axis (yaw).

			float sineYaw (0.0f), cosineYaw (0.0f);

			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);	// compute the sine and cosine of the yaw component

			return Vector2D (x * cosineYaw - y * sineYaw, x * sineYaw + y * cosineYaw);
		}

		//
		// Function: GetAngleBetween
		//
		// Description: Returns the angle in degrees between the two vectors, regardless of the axial planes (i.e., considering the plane formed by the vectors themselves).
		//
		// Returns: Angle in degrees between two vectors.
		//
		inline const float GetAngleBetween (const Vector2D &other) const
		{
			// this function returns the angle in degrees between the 'v_vector1' and 'v_vector2' vectors,
			// regardless of the axial planes (ie, considering the plane formed by the 'v_vector1' and
			// 'v_vector2' vectors themselves).

			// Reliability check (avoid zero divide).
//			InternalAssert (*this != ZeroValue && other != ZeroValue);	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if (*this == ZeroValue || other == ZeroValue)
				return 0.0f;	// Reliability check. (avoid zero divide)

			// normalize vec1 and vec2 (tip from botman)
			// reminder: dot product = (vec1.x * vec2.x + vec1.y * vec2.y)
			const float normalizedDotProduct (GetNormalized () | other.GetNormalized ());

			// how on Earth come that a dotproduct of normalized vectors can outbound [-1, 1]???
			// A couple 'forestry worker' casts to double seem to make the problem occur less often, but
			// still, we have to check the value to ensure it will be in range...

//			InternalAssert (normalizedDotProduct >= -1.0f && normalizedDotProduct <= 1.0f);	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if (normalizedDotProduct < -1.0f)
				return 180.0f;	// reliability check (avoid acos range error)
			else if (normalizedDotProduct > 1.0f)
				return 0.0f;	// reliability check (avoid acos range error)

			// do math
			InternalAssert (AreEqual2 (RadianToDegree (acosf (normalizedDotProduct)), AngleNormalize (RadianToDegree (acosf (normalizedDotProduct)))));

//			return acosf (normalizedDotProduct);	// and return it (in radians)
			return AngleNormalize (RadianToDegree (acosf (normalizedDotProduct)));	// compute the angle, wrap and return it
		}

		/** Generates a vector perpendicular to this vector (eg an 'up' vector).
			@remarks
				This method will return a vector which is perpendicular to this
				vector. There are an infinite number of possibilities but this
				method will guarantee to generate one of them. If you need more
				control you should use the Quaternion class.
		*/
		inline Vector2D GetPerpendicular (void) const { return Vector2D (-y, x); }

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline const Vector2D GetReflection (const Vector3D &normal) const
        {
            return *this - 2.0f * GetDotProduct (normal) * normal;
        }

		inline const float GetAvg (void) const { return (x + y) / AxisID_Total; }

		inline const SignBitsType_t ComputeAndGetSignBits (void) const
		{
			// for fast box on planeside test

			SignBitsType_t bits (AxisBit_None);

			for (unsigned char axisIndex (AxisID_X); axisIndex < AxisID_Total; ++axisIndex)
				if (operator [] (static_cast <AxisID_t> (axisIndex)) < 0.0f)
					bits |= BIT (axisIndex);

			return bits;
		}

		inline const char *const ToString (void) const
		{
			return FormatBuffer ("(%f, %f)", x, y);
		}
};

// Vector2D renamed to be Point2D just to avoid usage confusion.
typedef Vector2D Point2D;

#endif	// ifndef VECTOR_2D_INCLUDED