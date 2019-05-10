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
// Vector3D.hpp
//
// Class: Vector3D
//
// Description: Encapsulates standart 3-dimensional (3D) (math) vector used by the both Gold and Source engines.
//
// Warning: DO NOT CHANGE MEMBERS OF THIS CLASS AND ALSO DO NOT MAKE THIS CLASS AS VIRTUAL!!! (THIS CLASS USED BY HL ENGINE)
//
// Remarks:
//	A direction in 3D space represented as distances along the 3
//	orthoganal axes (x, y, z). Note that positions, directions and
//	scaling factors can be represented by a vector, depending on how
//	you interpret the values.
//
// Link: http://developer.valvesoftware.com/wiki/Vector
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef VECTOR_3D_INCLUDED
#define VECTOR_3D_INCLUDED

template <typename elementType> class TemplateVector3D
{
	elementType x, y, z;
};

class Vector3D
{
	//
	// Group: Constants.
	//
	public:
		enum AxisID_t
		{
			AxisID_X,	// +forward/-backward
			AxisID_Y,	// +left/-right
			AxisID_Z,	// +up/-down

			AxisID_Total
		};
		DECLARE_TYPED_ENUMERATION (unsigned char, AxisBit_t)
		{
			AxisBit_None,

			AxisBit_X = BIT (AxisID_X),
			AxisBit_Y = BIT (AxisID_Y),
			AxisBit_Z = BIT (AxisID_Z),

			AxisBit_All = AxisBit_X | AxisBit_Y | AxisBit_Z
		};

		typedef float Pointer_t[AxisID_Total];

		typedef AxisBit_t SignBitsType_t;

	//
	// Group: X, Y and Z axis members.
	//
	public:
		float x, y;	// horizontal
		float z;	// vertical

	//
	// Group: Special points.
	//
	public:
		CLASS_GLOBAL static const Vector3D ZeroValue;	// Zero vector value.

		CLASS_GLOBAL static const Vector3D UnitX;
		CLASS_GLOBAL static const Vector3D UnitY;
		CLASS_GLOBAL static const Vector3D UnitZ;
		CLASS_GLOBAL static const Vector3D NegativeUnitX;
		CLASS_GLOBAL static const Vector3D NegativeUnitY;
		CLASS_GLOBAL static const Vector3D NegativeUnitZ;
		CLASS_GLOBAL static const Vector3D UnitScale;

		// From Cry SDK....
		static const AxisID_t inc_mod[AxisID_Total];
		static const AxisID_t dec_mod[AxisID_Total];
/*
		#ifdef PHYSICS_EXPORTS
			#define incm(i) inc_mod[i]
			#define decm(i) dec_mod[i]
		#else*/
			static inline const AxisID_t incm (const AxisID_t i) { return static_cast <AxisID_t> (i + 1 & (i - 2) >> 31); }
			static inline const AxisID_t decm (const AxisID_t i) { return static_cast <AxisID_t> (i - 1 + ((i - 1) >> 31 & AxisID_Total)); }
//		#endif

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: Vector3D
		//
		// Description: Constructs Vector3D from float, and assign it's value to all axises.
		//
		// Parameters:
		//	scaler - Value for axises.
		//
		inline const Vector3D (const float scaler = 0.0f) : x (scaler), y (scaler), z (scaler) { /*VOID*/ }

		//
		// Function: Vector3D
		//
		// Description: Standard Vector3D Constructor.
		//
		// Parameters:
		//	inputX - Input X axis.
		//	inputY - Input Y axis.
		//	inputZ - Input Z axis.
		//
		inline const Vector3D (const float inputX, const float inputY, const float inputZ) : x (inputX), y (inputY), z (inputZ) { /*VOID*/ }

		//
		// Function: Vector3D
		//
		// Description: Constructs Vector3D from float pointer.
		//
		// Parameters:
		//	vector3D - Float pointer to assign.
		//
		inline const Vector3D (const float *const vector3D) : x (vector3D[AxisID_X]), y (vector3D[AxisID_Y]), z (vector3D[AxisID_Z]) { /*VOID*/ }

		//
		// Function: Vector3D
		//
		// Description: Constructs vector 3D from vector 2D, and assign it's value to all possible axises.
		//
		// Parameters:
		//	right - Other vector 2D, that should be assigned.
		//
		inline const Vector3D (const Vector2D &right) : x (right.x), y (right.y), z (0.0f) { /*VOID*/ }

		//
		// Function: Vector3D
		//
		// Description: Constructs Vector3D from another Vector3D.
		//
		// Parameters:
		//	right - Other Vector3D, that should be assigned.
		//
		inline const Vector3D (const Vector3D &right) : x (right.x), y (right.y), z (right.z) { /*VOID*/ }

	//
	// Group: Operators.
	//
	public:
		// Equality.
		inline const bool             operator ==                 (const Vector2D &right)                     const { return x == right.x && y == right.y; }
		inline const bool             operator ==                 (const Vector3D &right)                     const { return x == right.x && y == right.y && z == right.z; }
		inline const bool             operator !=                 (const Vector2D &right)                     const { return x != right.x || y != right.y; }
		inline const bool             operator !=                 (const Vector3D &right)                     const { return x != right.x || y != right.y || z != right.z; }

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
		inline const bool             operator <                  (const float value)                         const { return x < value && y < value && z < value; }

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
		inline const bool             operator <                  (const Vector3D &right)                     const { return x < right.x && y < right.y && z < right.z; }

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
		inline const bool             operator <=                 (const float value)                         const { return x <= value && y <= value && z <= value; }

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
		inline const bool             operator <=                 (const Vector3D &right)                     const { return x <= right.x && y <= right.y && z <= right.z; }

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
		inline const bool             operator >                  (const float value)                         const { return x > value && y > value && z > value; }

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
		inline const bool             operator >                  (const Vector3D &right)                     const { return x > right.x && y > right.y && z > right.z; }

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
		inline const bool             operator >=                 (const float value)                         const { return x >= value && y >= value && z >= value; }

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
		inline const bool             operator >=                 (const Vector3D &right)                     const { return x >= right.x && y >= right.y && z >= right.z; }

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
		inline                        operator       Vector2D     (void)                                      const { return Vector2D (x, y); }	// vectors will now automatically convert to 'Vector2D' when needed
		inline                        operator       Vector2D &   (void)                                            { return AsVector2D (); }	// vectors will now automatically convert to 'Vector2D &' when needed
		inline                        operator const Vector2D &   (void)                                      const { return AsVector2D (); }	// vectors will now automatically convert to 'const Vector2D &' when needed

		// arithmetic operations
		inline const Vector3D         operator +                  (const float scaler)                        const { return Vector3D (x + scaler, y + scaler, z + scaler); }
		inline const Vector3D         operator +                  (const Vector2D &right)                     const { return Vector3D (x + right.x, y + right.y, z); }
		inline const Vector3D         operator +                  (const Vector3D &right)                     const { return Vector3D (x + right.x, y + right.y, z + right.z); }
		inline       Vector3D        &operator +=                 (const float scaler)
		{
			x += scaler;
			y += scaler;
			z += scaler;

			return *this;
		}
		inline       Vector3D        &operator +=                 (const Vector2D &right)
		{
			x += right.x;
			y += right.y;

			return *this;
		}
		inline       Vector3D        &operator +=                 (const Vector3D &right)
		{
			x += right.x;
			y += right.y;
			z += right.z;

			return *this;
		}

		inline const Vector3D         operator -                  (const float scaler)                        const { return Vector3D (x - scaler, y - scaler, z - scaler); }
		inline       Vector3D         operator -                  (const Vector2D &right)                     const { return Vector3D (x - right.x, y - right.y, z); }
		inline       Vector3D         operator -                  (const Vector3D &right)                     const { return Vector3D (x - right.x, y - right.y, z - right.z); }
		inline       Vector3D        &operator -=                 (const float scaler)
		{
			x -= scaler;
			y -= scaler;
			z -= scaler;

			return *this;
		}
		inline       Vector3D        &operator -=                 (const Vector2D &right)
		{
			x -= right.x;
			y -= right.y;

			return *this;
		}
		inline       Vector3D        &operator -=                 (const Vector3D &right)
		{
			x -= right.x;
			y -= right.y;
			z -= right.z;

			return *this;
		}

		inline       Vector3D         operator *                  (const float scaler)                        const { return Vector3D (x * scaler, y * scaler, z * scaler); }
		inline       Vector3D         operator *                  (const Vector2D &right)                     const { return Vector3D (x * right.x, y * right.y, z); }
		inline       Vector3D         operator *                  (const Vector3D &right)                     const { return Vector3D (x * right.x, y * right.y, z * right.z); }
		friend inline Vector3D        operator *                  (const float scaler, const Vector3D &right)       { return right * scaler; }
		inline       Vector3D        &operator *=                 (const float scaler)
		{
			x *= scaler;
			y *= scaler;
			z *= scaler;

			return *this;
		}
		inline       Vector3D        &operator *=                 (const Vector2D &right)
		{
			x *= right.x;
			y *= right.y;

			return *this;
		}
		inline       Vector3D        &operator *=                 (const Vector3D &right)
		{
			x *= right.x;
			y *= right.y;
			z *= right.z;

			return *this;
		}

		inline const Vector3D         operator /                  (const float scaler)                        const
		{
			// Reliability check.
//			InternalAssert (scaler != 0.0f);

			return *this * (1.0f / scaler);
		}
		inline const Vector3D         operator /                  (const Vector2D &right)                     const
		{
			// Reliability check.
			InternalAssert (right.x != 0.0f && right.y != 0.0f);

			return Vector3D (x / right.x, y / right.y, z);
		}
		inline const Vector3D         operator /                  (const Vector3D &right)                     const
		{
			// Reliability check.
			InternalAssert (right.x != 0.0f && right.y != 0.0f && right.z != 0.0f);

			return Vector3D (x / right.x, y / right.y, z / right.z);
		}
		inline       Vector3D        &operator /=                 (const float scaler)
		{
			// Reliability check.
//			InternalAssert (scaler != 0.0f);

			return *this *= 1.0f / scaler;
		}
		inline       Vector3D        &operator /=                 (const Vector2D &right)
		{
			// Reliability check.
			InternalAssert (right.x != 0.0f && right.y != 0.0f);

			x /= right.x;
			y /= right.y;

			return *this;
		}
		inline       Vector3D        &operator /=                 (const Vector3D &right)
		{
			// Reliability check.
			InternalAssert (right.x != 0.0f && right.y != 0.0f && right.z != 0.0f);

			x /= right.x;
			y /= right.y;
			z /= right.z;

			return *this;
		}

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
		inline const float            operator |                  (const Vector2D &right)                     const { return GetDotProduct2D (right); }
		inline const float            operator |                  (const Vector3D &right)                     const { return GetDotProduct (right); }

		//
		// Function: operator ^
		//
		// Description: Calculates the cross-product of 2 vectors, i.e. the vector that lies perpendicular to them both.
		//
		// Remarks:
		//	The cross-product is normally used to calculate the normal vector
		//	of a plane, by calculating the cross-product of 2 non-equivalent
		//	vectors which lie on the plane (e.g. 2 edges of a triangle).
		//
		// Par:
		//	For a clearer explanation, look a the left and the bottom edges
		//	of your monitor's screen. Assume that the first vector is the
		//	left edge and the second vector is the bottom edge, both of
		//	them starting from the lower-left corner of the screen. The
		//	resulting vector is going to be perpendicular to both of them
		//	and will go <i>inside</i> the screen, towards the cathode tube
		//	(assuming you're using a CRT monitor, of course).
		//
		// Parameters:
		//	right - Vector which, together with this one, will be used to calculate the cross-product.
		//
		// Returns:
		//	A vector which is the result of the cross-product. This vector will
		//	<b>NOT</b> be normalised, to maximise efficiency
		//	- call GetNormalized() on the result if you wish this to be done.
		//	As for which side the resultant vector will be on, the returned vector
		//	will be on the side from which the arc from 'this' to 'right' is anticlockwise, e.g.
		//	UnitY ^ UnitZ = UnitX, whilst
		//	UnitZ ^ UnitY = NegativeUnitX.
		//	This is because OGRE uses a right-handed coordinate system.
		//
		inline       Vector3D         operator ^                  (const Vector3D &right)                     const { return GetCrossProduct (right); }
		inline       Vector3D        &operator ^=                 (const Vector3D &right)                           { return *this = GetCrossProduct (right); }

		inline       Vector3D        &operator ~                  (void)                                            { return Normalize (); }

		inline const Vector3D         operator -                  (void)                                      const { return GetInverted (); }

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
		inline Vector3D              &operator =                  (const float value)
		{
			x = y = z = value;

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current vector with float pointer.
		//
		// Parameters:
		//	vector3D - Float pointer that should be assigned.
		//
		// Returns: Reassigned vector.
		//
		inline Vector3D              &operator =                  (const float *const vector3D)
		{
			// Reliability check.
			InternalAssert (vector3D != NULL);

			x = vector3D[AxisID_X];
			y = vector3D[AxisID_Y];
			z = vector3D[AxisID_Z];

			return *this;
		}

		inline Vector3D              &operator =                  (const Vector2D &right)
		{
			x = right.x;
			y = right.y;
			z = 0.0f;

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
		inline Vector3D              &operator =                  (const Vector3D &right)
		{
			x = right.x;
			y = right.y;
			z = right.z;

			return *this;
		}

	//
	// Group: Methods.
	//
	public:
		// Cast to Vector2D....
		inline       Vector2D &AsVector2D (void)       { return *reinterpret_cast <Vector2D *const> (this); }
		inline const Vector2D &AsVector2D (void) const { return *reinterpret_cast <const Vector2D *const> (this); }

		//
		// Function: SkipZ
		//
		// Description: Gets vector without Z axis.
		//
		// Returns: 2D vector from 3D vector.
		//
		inline const Vector3D SkipZ (void) const { return Vector3D (x, y, 0.0f); }

		// permutate coordinates so that z goes to 'newZaxis' slot (From Cry SDK)
		inline Vector3D &Permutate (const AxisID_t newZaxis) { return *this = GetPermutated (newZaxis); }
		inline Vector3D GetPermutated (const AxisID_t newZaxis) const { return Vector3D (operator [] (/*inc_mod - ORIGINAL IN Cry SDK*/dec_mod[newZaxis]), operator [] (/*dec_mod - ORIGINAL IN Cry SDK*/inc_mod[newZaxis]), operator [] (newZaxis)); }

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
		inline const float GetDotProduct2D (const Vector2D &other) const { return x * other.x + y * other.y; }
		inline const float GetDotProduct   (const Vector3D &other) const { return x * other.x + y * other.y + z * other.z; }

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
		inline const float GetAbsoluteDotProduct (const Vector3D &other) const { return fabsf (x * other.x) + fabsf (y * other.y) + fabsf (z * other.z); }

		//
		// Function: GetCrossProduct
		//
		// Description: Calculates the cross-product of 2 vectors, i.e. the vector that lies perpendicular to them both.
		//
		// Remarks:
		//	The cross-product is normally used to calculate the normal vector
		//	of a plane, by calculating the cross-product of 2 non-equivalent
		//	vectors which lie on the plane (e.g. 2 edges of a triangle).
		//
		// Par:
		//	For a clearer explanation, look a the left and the bottom edges
		//	of your monitor's screen. Assume that the first vector is the
		//	left edge and the second vector is the bottom edge, both of
		//	them starting from the lower-left corner of the screen. The
		//	resulting vector is going to be perpendicular to both of them
		//	and will go <i>inside</i> the screen, towards the cathode tube
		//	(assuming you're using a CRT monitor, of course).
		//
		// Parameters:
		//	right - Vector which, together with this one, will be used to calculate the cross-product.
		//
		// Returns:
		//	A vector which is the result of the cross-product. This vector will
		//	<b>NOT</b> be normalised, to maximise efficiency
		//	- call GetNormalized() on the result if you wish this to be done.
		//	As for which side the resultant vector will be on, the returned vector
		//	will be on the side from which the arc from 'this' to 'right' is anticlockwise, e.g.
		//	UnitY ^ UnitZ = UnitX, whilst
		//	UnitZ ^ UnitY = NegativeUnitX.
		//	This is because OGRE uses a right-handed coordinate system.
		//
		inline Vector3D GetCrossProduct     (const Vector3D &right) const { return Vector3D (y * right.z - z * right.y, z * right.x - x * right.z, x * right.y - y * right.x); }
		inline Vector3D GetUnitCrossProduct (const Vector3D &right) const { return GetCrossProduct (right).Normalize (); }

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
		inline Vector3D GetMidPoint (const Vector3D &vector) const
		{
			return Math::GetMidPoint (*this, vector);
		}
		inline Vector3D &SetMidPoint (const Vector3D &vector)
		{
			return Math::SetMidPoint (*this, vector);
		}
		inline Vector3D GetExtent (const Vector3D &vector) const
		{
			return Math::GetExtent (*this, vector);
		}
		inline const bool AreEqual2D (const Vector2D &other) const
		{
			return Math::AreEqual (x, other.x) && Math::AreEqual (y, other.y);
		}
		inline const bool AreEqual (const Vector3D &other) const
		{
			return Math::AreEqual (x, other.x) && Math::AreEqual (y, other.y) && Math::AreEqual (z, other.z);
		}
		inline const bool AreEqual2D (const Vector2D &other, const float tolerance) const
		{
			return Math::AreEqual (x, other.x, tolerance) && Math::AreEqual (y, other.y, tolerance);
		}
		inline const bool AreEqual (const Vector3D &other, const float tolerance) const
		{
			return Math::AreEqual (x, other.x, tolerance) && Math::AreEqual (y, other.y, tolerance) && Math::AreEqual (z, other.z, tolerance);
		}
		inline const bool IsPointBelongsToSegment (const Vector3D &segmentDestination, const Vector3D &point) const
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
		inline void Swap (Vector3D &other)
		{
			CoreToolLibrary::SwapElements (*this, other);
		}

		//
		// Function: GetLength
		//
		// Description: Gets length (magnitude) of 3D vector.
		//
		// Warning:
		//	This operation requires a square root and is expensive in terms of CPU operations.
		//	If you don't need to know the exact length (e.g. for just comparing lengths) use GetLengthSquared() instead.
		//
		// Returns: Length (magnitude) of the 3D vector.
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
		inline const float GetDistance (const Vector3D &other) const
		{
			return (other - *this).GetLength ();
		}

		//
		// Function: GetLength2D
		//
		// Description: Gets length (magnitude) of vector ignoring Z axis.
		//
		// Warning:
		//	This operation requires a square root and is expensive in terms of CPU operations.
		//	If you don't need to know the exact length (e.g. for just comparing lengths) use GetLengthSquared2D() instead.
		//
		// Returns: 2D length (magnitude) of the vector.
		//
		inline const float GetLength2D (void) const { return sqrtf (GetLengthSquared2D ()); }

		//
		// Function: GetDistance2D
		//
		// Description: Gets distance to another vector ignoring Z axis.
		//
		// Warning:
		//	This operation requires a square root and is expensive in
		//	terms of CPU operations. If you don't need to know the exact
		//	distance (e.g. for just comparing distances) use GetDistanceSquared2D()
		//	instead.
		//
		// Parameters:
		//	other - Vector which, together with this one, will be used to calculate the squared distance.
		//
		// Returns: Distance to another vector.
		//
		inline const float GetDistance2D (const Vector2D &other) const
		{
			return (other - *this).GetLength ();
		}

		//
		// Function: GetLengthSquared
		//
		// Description: Gets squared length (magnitude) of 3D vector.
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
		// Returns: Squared length (magnitude) of the 3D vector.
		//
		inline const float GetLengthSquared (void) const { return MATH_GET_SQUARED (x) + MATH_GET_SQUARED (y) + MATH_GET_SQUARED (z); }

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
		inline const float GetDistanceSquared (const Vector3D &other) const
		{
			return (other - *this).GetLengthSquared ();
		}

		//
		// Function: GetLengthSquared2D
		//
		// Description: Gets squared length (magnitude) of vector ignoring Z axis.
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
		// Returns: 2D squared length (magnitude) of the vector.
		//
		inline const float GetLengthSquared2D (void) const { return MATH_GET_SQUARED (x) + MATH_GET_SQUARED (y); }

		//
		// Function: GetDistanceSquared2D
		//
		// Description: Gets squared distance to another vector ignoring Z axis.
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
		// Returns: 2D squared distance to another vector.
		//
		inline const float GetDistanceSquared2D (const Vector2D &other) const
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
		inline Vector3D &Invert (void)
		{
			return *this = GetInverted ();
		}
		inline const Vector3D GetInverted (void) const
		{
			return Vector3D (-x, -y, -z);
		}

		// Returns if the specified point is inside the bounding box specified by origin, mins, maxs.
		inline const bool IsInsideBoundingBox (const Vector3D &origin, const Vector2D &mins, const Vector2D &maxs) const
		{
			const Vector2D delta (origin - *this);

			return delta >= mins && delta <= maxs;
		}
		inline const bool IsInsideBoundingBox (const Vector3D &origin, const Vector3D &mins, const Vector3D &maxs) const
		{
			const Vector3D delta (origin - *this);

			return delta >= mins && delta <= maxs;
		}
		inline const bool IsInsideBoundingBox (const Vector3D &absoluteMins, const Vector3D &absoluteMaxs) const
		{
			return *this >= absoluteMins && *this <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Vector3D &absoluteMins, const Vector3D &absoluteMaxs, const float tolerance) const
		{
			return *this + tolerance >= absoluteMins && *this - tolerance <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs) const
		{
			return *this >= absoluteMins && *this <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs, const float tolerance) const
		{
			return *this + tolerance >= absoluteMins && *this - tolerance <= absoluteMaxs;
		}
		inline const bool IsInsideBoundingBox (const Extent2D &extent) const;							// Note: This function declared in 'Core tool library\Extent2D.hpp'.
		inline const bool IsInsideBoundingBox (const Extent2D &extent, const float tolerance) const;	// Note: This function declared in 'Core tool library\Extent2D.hpp'.
		inline const bool IsInsideBoundingBox (const Extent3D &extent) const;							// Note: This function declared in 'Core tool library\Extent3D.hpp'.
		inline const bool IsInsideBoundingBox (const Extent3D &extent, const float tolerance) const;	// Note: This function declared in 'Core tool library\Extent3D.hpp'.

		inline const Vector3D  GetMA (const Vector3D &direction, const float scale)                         const { return Math::GetMA (*this, direction, scale); }
		inline       Vector3D &MA    (const Vector3D &direction, const float scale)                               { return Math::MA (*this, direction, scale); }
		inline       Vector3D &MA    (const Vector3D &source, const Vector3D &direction, const float scale)       { return Math::MA (*this, source, direction, scale); }

		//
		// Function: GetInterpolated
		//
		// Description: Gets a interpolated vector between this vector and another vector.
		//
		// Parameters:
		//	destination - Vector to interpolate between.
		//	fraction - Interpolation value between 0.0 (all the destination vector) and 1.0 (all this vector).
		//
		// Returns: Interpolated vector.
		//
		inline const Vector3D GetInterpolated (const Vector3D &destination, const float fraction) const { return Math::GetInterpolated (*this, destination, fraction); }

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
		inline Vector3D &Interpolate (const Vector3D &source, const Vector3D &destination, const float fraction) { return Math::Interpolate (*this, source, destination, fraction); }

		//
		// Function: GetMinimum
		//
		// Description: Gets a vector with the minimum in X, Y, and Z.
		//
		// Returns: Minimum vector in X, Y, and Z.
		//
		inline const float GetMinimum (void) const
		{
			return GetMinimumValueBetween (x, GetMinimumValueBetween (y, z));
		}
		inline const Vector3D GetMinimum (const Vector3D &other) const
		{
			return Vector3D (GetMinimumValueBetween (x, other.x), GetMinimumValueBetween (y, other.y), GetMinimumValueBetween (z, other.z));
		}
		inline Vector3D &SetMinimum (const Vector3D &other) { return *this = GetMinimum (other); }

		//
		// Function: GetMaximum
		//
		// Description: Gets a vector with the maximum in X, Y, and Z.
		//
		// Returns: Maximum vector in X, Y, and Z.
		//
		inline const float GetMaximum (void) const
		{
			return GetMaximumValueBetween (x, GetMaximumValueBetween (y, z));
		}
		inline const Vector3D GetMaximum (const Vector3D &other) const
		{
			return Vector3D (GetMaximumValueBetween (x, other.x), GetMaximumValueBetween (y, other.y), GetMaximumValueBetween (z, other.z));
		}
		inline Vector3D &SetMaximum (const Vector3D &other) { return *this = GetMaximum (other); }

		inline Vector3D &RotateX (const float roll)
		{
			// Rotate a vector around the X axis (roll).

			return *this = GetRotatedX (roll);
		}
		inline const Vector3D GetRotatedX (const float roll) const
		{
			// Rotate a vector around the X axis (roll).

			float sineRoll (0.0f), cosineRoll (0.0f);

			GetCosineSine (DegreeToRadian (roll), cosineRoll, sineRoll);	// compute the sine and cosine of the roll component

			return Vector3D (x, y * cosineRoll - z * sineRoll, y * sineRoll + z * cosineRoll);
		}
		inline Vector3D &RotateY (const float pitch)
		{
			// Rotate a vector around the Y axis (pitch).

			return *this = GetRotatedY (pitch);
		}
		inline const Vector3D GetRotatedY (const float pitch) const
		{
			// Rotate a vector around the Y axis (pitch).

			float sinePitch (0.0f), cosinePitch (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component

			return Vector3D (x * cosinePitch + z * sinePitch, y, x * -sinePitch + z * cosinePitch);	// FROM CHEAT
//			return Vector3D (x * cosinePitch - z * sinePitch, y, z * cosinePitch - x * sinePitch);	// FROM OPEN STEER
//			return Vector3D (x * cosinePitch - z * sinePitch, y, x * sinePitch + z * cosinePitch);	// FROM HL2
		}
		inline Vector3D &RotateZ (const float yaw)
		{
			// Rotate a vector around the Z axis (yaw).

			return *this = GetRotatedZ (yaw);
		}
		inline const Vector3D GetRotatedZ (const float yaw) const
		{
			// Rotate a vector around the Z axis (yaw).

			float sineYaw (0.0f), cosineYaw (0.0f);

			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);	// compute the sine and cosine of the yaw component

			return Vector3D (x * cosineYaw - y * sineYaw, x * sineYaw + y * cosineYaw, z);
		}
		inline Vector3D       &Rotate     (const Angles2D &angles);			// Note: This function declared in 'Core tool library\Angles2D.hpp'.
		inline const Vector3D  GetRotated (const Angles2D &angles) const;	// Note: This function declared in 'Core tool library\Angles2D.hpp'.
		inline Vector3D       &Rotate     (const Angles3D &angles);			// Note: This function declared in 'Core tool library\Angles3D.hpp'.
		inline const Vector3D  GetRotated (const Angles3D &angles) const;	// Note: This function declared in 'Core tool library\Angles3D.hpp'.

		//
		// Function: GetNormalized
		//
		// Description: Gets normalized 3D vector.
		//
		// Remarks: This method normalises the vector such that it's length / magnitude is 1.0f. The result is called a unit vector.
		//
		// Note: This function will not crash for zero-sized vectors, but there will be no changes made to their components.
		//
		// Returns: Normalized 3D vector.
		//
		inline const Vector3D GetNormalized (void) const
		{
/*			const float length = GetLength ();

			// Reliability check.
			if (Math::IsZero (length))
				return UnitZ;

			return *this / length;
*/
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
		// Returns: The previous length of the 3D vector.
		//
		inline Vector3D &Normalize (void)
		{
/*			const float length (GetLength ());

			// Reliability check.
			if (Math::IsZero (length))
			{
				// Reliability check.
				InternalAssert (IsZero2D ());

				z = 1.0f;

				return *this;
//				return UnitZ;
			}

			return *this /= length;
*/
			// 'Epsilon' is added to the radius to eliminate the possibility of divide by zero.
			return *this /= GetLength ()/* + Epsilon*/;
		}
		inline Vector3D &Normalize2D (void)
		{
/*			const float length2D (GetLength2D ());

			// Reliability check.
			if (Math::IsZero (length2D))
			{
				// Reliability check.
				InternalAssert (Math::IsZero (x));

				y = 1.0f;
				z = 0.0f;

				return *this;
//				return UnitY;
			}

			*this /= length2D;

			z = 0.0f;

			return *this;
*/
			// 'Epsilon' is added to the radius to eliminate the possibility of divide by zero.
			*this /= GetLength2D ()/* + Epsilon*/;

			z = 0.0f;

			return *this;
		}
/*		inline const float Normalize (void)
		{
			const float length (GetLength ());

			*this = GetNormalized ();

			return length;
		}*/
/*		inline const float Normalize2D (void)
		{
			const float length (GetLength2D ());

			*this = GetNormalized2D ();

			return length;
		}*/

		//
		// Function: GetNormalized2D
		//
		// Description: Gets normalized 2D vector.
		//
		// Remarks: This method normalises the vector such that it's length / magnitude is 1.0f. The result is called a unit vector.
		//
		// Note: This function will not crash for zero-sized vectors, but there will be no changes made to their components.
		//
		// Returns: Normalized 2D vector (the previous length of the 2D vector).
		//
		inline const Vector3D GetNormalized2D (void) const
		{
/*			float length2D (GetLength2D ());

			// Reliability check.
			if (Math::IsZero (length2D))
				return UnitY;

			length2D = 1.0f / length2D;

			return Vector3D (x * length2D, y * length2D, 0.0f);
*/
			// 'Epsilon' is added to the radius to eliminate the possibility of divide by zero.
			Vector3D result (*this / (GetLength2D ()/* + Epsilon*/));

			result.z = 0.0f;

			return result;
		}

		//
		// Function: IsZero
		//
		// Description: Checks whether vector axises are zero.
		//
		// Returns: True if this vector is (0.0f, 0.0f, 0.0f) within tolerance, false otherwise.
		//
		inline const bool IsZero (void) const
		{
			return Math::IsZero (x) && Math::IsZero (y) && Math::IsZero (z);
		}

		//
		// Function: IsZero2D
		//
		// Description: Checks whether vector axises x and y are zero.
		//
		// Returns: True if this vector is (0.0f, 0.0f) within tolerance, false otherwise.
		//
		inline const bool IsZero2D (void) const
		{
			return Math::IsZero (x) && Math::IsZero (y);
		}

		//
		// Function: ToPitch
		//
		// Description: Converts a spatial location determined by the vector passed into an absolute X angle (pitch) from the origin of the world.
		//
		// Returns: Pitch angle.
		//
		inline const float ToPitch (void) const
		{
			if (IsZero2D ())
				return z > 0.0f ? 90.0f : -90.0f;

			return RadianToDegree (atan2f (z, GetLength2D ()));
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
			if (IsZero2D ())
				return 0.0f;

			return RadianToDegree (atan2f (y, x));
		}

		inline const float ToRoll (void) const
		{
			return 0.0f;	// Ignore roll component.
		}

		inline Angles2D ToAngles2D (void) const;
		//
		// Function: ToAngles3D
		//
		// Description: Convert a spatial location determined by the vector passed in into constant absolute angles from the origin of the world.
		//
		// Note: This function declared in 'Core tool library\Angles3D.hpp'
		//
		// Returns: Converted from vector, constant angles.
		//
		inline Angles3D ToAngles3D (void) const;
/*
		inline Vector3D BuildForwardVector (void) const
		{
			/// @todo !?!?!?!
			return *this;
		}
*/
		inline Vector3D BuildRightVector (void) const
		{
			// Is the input vector absolutely vertical?
			if (IsZero2D ())
				return NegativeUnitY;	// pitch 90 degrees up/down from identity

			// Else....
			return (*this ^ UnitZ).Normalize ();
		}

		inline Vector3D BuildUpwardVector (void) const
		{
			// Is the input vector absolutely vertical?
			if (IsZero2D ())
				return Vector3D (-z, 0.0f, 0.0f);	// pitch 90 degrees up/down from identity

			// Else....
			return ((*this ^ UnitZ).Normalize () ^ *this).Normalize ();
		}

		inline void BuildVectors (/*Vector3D &forward, */Vector3D &right, Vector3D &upward) const
		{
			const Vector3D &forward (*this);
//			forward = *this;	/// NEEDED !?!?!?!

			// Is the input vector absolutely vertical?
			if (IsZero2D ())
			{
				// pitch 90 degrees up/down from identity
				right = NegativeUnitY;

				upward.x = -z;
				upward.y = upward.z = 0.0f;

				return;
			}

			// Else....
			right = (forward ^ UnitZ).Normalize ();
			upward = (right ^ forward).Normalize ();
/*! FROM XASH ENGINE!!!!
	right.x = forward.z;
	right.y = -forward.x;
	right.z = forward.y;

	const float dot (forward | right);

	right.MA (forward, -dot).Normalize ();
	upward = right ^ forward;*/
		}

		/*inline */void MakeRightVector  (void) const;
		/*inline */void MakeUpwardVector (void) const;
		/*inline */void MakeVectors      (void) const;

		//
		// Function: GetAngleBetween
		//
		// Description: Returns the angle in degrees between the two vectors, regardless of the axial planes (i.e., considering the plane formed by the vectors themselves).
		//
		// Returns: Angle in degrees between two vectors.
		//
		inline const float GetAngleBetween (const Vector3D &other) const
		{
			// this function returns the angle in degrees between the 'v_vector1' and 'v_vector2' vectors,
			// regardless of the axial planes (ie, considering the plane formed by the 'v_vector1' and
			// 'v_vector2' vectors themselves).

			// Reliability check. (avoid zero divide)
//			InternalAssert (*this != ZeroValue && other != ZeroValue);	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if (*this == ZeroValue || other == ZeroValue)
				return 0.0f;

			// normalize vec1 and vec2 (tip from botman)
			// reminder: dot product = (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z)
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
		inline Vector3D GetPerpendicular (void) const
		{
			Vector3D perp (GetCrossProduct (UnitX));

			// Check length
			if (perp.GetLengthSquared () < ZeroToleranceSquared)
			{
				// This vector is the Y axis multiplied by a scalar, so we have to use another axis.
				perp = GetCrossProduct (UnitY);
			}

			return perp.Normalize ();
		}

        /** Calculates a reflection vector to the plane with the given normal .
        @remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        */
        inline const Vector3D GetReflection (const Vector3D &normal) const
        {
            return *this - 2.0f * GetDotProduct (normal) * normal;
        }

		inline const float GetAvg (void) const { return (x + y + z) / AxisID_Total; }

		inline const Vector3D GetTransformed (const Matrix3x4 &matrix) const
		{
			return Vector3D ((*this | Vector3D (matrix[0u])) + matrix[0u][3u], (*this | Vector3D (matrix[1u])) + matrix[1u][3u], (*this | Vector3D (matrix[2u])) + matrix[2u][3u]);
		}

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
			return FormatBuffer ("(%f, %f, %f)", x, y, z);
		}

		static inline const float GetVectorsCone (const Vector3D &source1, const Vector3D &destination1, const Vector3D &source2, const Vector3D &destination2)
		{
			const Vector3D direction1 ((source1 - destination1).Normalize ());
			const Vector3D direction2 ((source2 - destination2).Normalize ());

			return (direction1 - direction2).GetLength ();
		}
};

// Reliability check.
CompileTimeAssert (sizeof (Vector3D) == 12u);

// Vector3D renamed to be Point3D just to avoid usage confusion.
typedef Vector3D Point3D;

inline Vector2D::Vector2D (const Vector3D &right) : x (right.x), y (right.y) { /*VOID*/ }

inline const bool Vector2D::operator == (const Vector3D &right) const { return x == right.x && y == right.y; }
inline const bool Vector2D::operator != (const Vector3D &right) const { return x != right.x || y != right.y; }
inline const bool Vector2D::operator <  (const Vector3D &right) const { return x <  right.x && y <  right.y; }
inline const bool Vector2D::operator >  (const Vector3D &right) const { return x >  right.x && y >  right.y; }
inline const bool Vector2D::operator >= (const Vector3D &right) const { return x >= right.x && y >= right.y; }
inline const bool Vector2D::operator <= (const Vector3D &right) const { return x <= right.x && y <= right.y; }

//inline                        Vector2D::operator       Vector3D     (void)                                      const { return Vector3D (*this); }	// vectors will now automatically convert to 'Vector3D' when needed
//inline                        Vector2D::operator const Vector3D     (void)                                      const { return Vector3D (*this); }	// vectors will now automatically convert to 'const Vector3D' when needed

inline const Vector2D         Vector2D::operator +                  (const Vector3D &right)                     const { return Vector2D (x + right.x, y + right.y); }
inline const Vector2D         Vector2D::operator -                  (const Vector3D &right)                     const { return Vector2D (x - right.x, y - right.y); }
inline       Vector2D        &Vector2D::operator -=                 (const Vector3D &right)
{
	x -= right.x;
	y -= right.y;

	return *this;
}
inline const Vector2D         Vector2D::operator *                  (const Vector3D &right)                     const { return Vector2D (x * right.x, y * right.y); }
inline       Vector2D        &Vector2D::operator *=                 (const Vector3D &right)
{
	x *= right.x;
	y *= right.y;

	return *this;
}
inline const Vector2D         Vector2D::operator /                  (const Vector3D &right)                     const
{
	// Reliability check.
	InternalAssert (right.x != 0.0f && right.y != 0.0f);

	return Vector2D (x / right.x, y / right.y);
}
inline       Vector2D        &Vector2D::operator /=                 (const Vector3D &right)
{
	// Reliability check.
	InternalAssert (right.x != 0.0f && right.y != 0.0f);

	x /= right.x;
	y /= right.y;

	return *this;
}

inline Vector2D              &Vector2D::operator =                  (const Vector3D &right)
{
	x = right.x;
	y = right.y;

	return *this;
}

#endif	// ifndef VECTOR_3D_INCLUDED