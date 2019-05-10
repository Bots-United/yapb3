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
// Angles2D.hpp
//
// Class: Angles2D
//
// Link: http://developer.valvesoftware.com/wiki/QAngle
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_ANGLES_2D_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_ANGLES_2D_INCLUDED

class Angles2D
{
	//
	// Group: Constants.
	//
	public:
		enum AngleID_t
		{
			AngleID_Pitch,	// +down/-up
			AngleID_Yaw,	// +left/-right

			AngleID_Total
		};

		typedef float Pointer_t[AngleID_Total];

	//
	// Group: pitch and yaw angle members.
	//
	public:
		float pitch;	// Counterclockwise rotation around the CAMERA_RIGHT axis, in degrees [-90, 90]. (horizontal)
		float yaw;		// Counterclockwise rotation around the CAMERA_UP axis, in degrees [-359, 359]. (vertical)

	//
	// Group: Special points.
	//
	public:
		CLASS_GLOBAL static const Angles2D ZeroValue;	// Zero angles value.

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: Angles2D
		//
		// Description: Constructs zero Angles2D.
		//
		inline const Angles2D (void) : pitch (0.0f), yaw (0.0f) { /*VOID*/ }

		//
		// Function: Angles2D
		//
		// Description: Standard Angles2D Constructor.
		//
		// Parameters:
		//	inputX - Input pitch angle.
		//	inputY - Input yaw angle.
		//
		inline const Angles2D (const float inputPitch, const float inputYaw) : pitch (inputPitch), yaw (inputYaw) { /*VOID*/ }

		//
		// Function: Angles2D
		//
		// Description: Constructs Angles2D from float pointer.
		//
		// Parameters:
		//	angles2D - Float pointer to assign.
		//
		inline const Angles2D (const float *const angles2D) : pitch (angles2D[AngleID_Pitch]), yaw (angles2D[AngleID_Yaw]) { /*VOID*/ }

		//
		// Function: Angles2D
		//
		// Description: Constructs Angles2D from another Angles2D.
		//
		// Parameters:
		//	right - Other Angles2D, that should be assigned.
		//
		inline const Angles2D (const Angles2D &right) : pitch (right.pitch), yaw (right.yaw) { /*VOID*/ }

		inline Angles2D (const Angles3D &right);	// Note: This function declared in 'Core tool library/Angles3D.hpp'.

	//
	// Group: Operators.
	//
	public:
		// Equality.
		inline const bool             operator ==                 (const Angles2D &right)                     const { return pitch == right.pitch && yaw == right.yaw; }
		inline const bool             operator !=                 (const Angles2D &right)                     const { return pitch != right.pitch && yaw != right.yaw; }

		// array access
		inline float                 &operator []                 (const AngleID_t angleIndex)
		{
			// Reliability check.
			InternalAssert (angleIndex < AngleID_Total);

			return (&pitch)[angleIndex];
		}
		inline const float           &operator []                 (const AngleID_t angleIndex)                const
		{
			// Reliability check.
			InternalAssert (angleIndex < AngleID_Total);

			return (&pitch)[angleIndex];
		}

		// casting operators
		inline                        operator       float *      (void)                                            { return &pitch; }	// angles will now automatically convert to 'float *' when needed
		inline                        operator const float *const (void)                                      const { return &pitch; }	// angles will now automatically convert to 'const float *const' when needed

		// arithmetic operations
		inline Angles2D               operator +                  (const Angles2D &right)                     const { return Angles2D (pitch + right.pitch, yaw + right.yaw); }
		inline Angles2D               operator +                  (const Angles2D &right)                           { return Angles2D (pitch + right.pitch, yaw + right.yaw); }
		inline Angles2D               operator +                  (const float scaler)                        const { return Angles2D (pitch + scaler, yaw + scaler); }
		inline Angles2D              &operator +=                 (const Angles2D &right)
		{
			pitch += right.pitch;
			yaw += right.yaw;

			return *this;
		}
		inline Angles2D              &operator +=                 (const float scaler)
		{
			pitch += scaler;
			yaw += scaler;

			return *this;
		}

		inline Angles2D               operator -                  (const Angles2D &right)                     const { return Angles2D (pitch - right.pitch, yaw - right.yaw); }
		inline Angles2D               operator -                  (const Angles2D &right)                           { return Angles2D (pitch - right.pitch, yaw - right.yaw); }
		inline Angles2D               operator -                  (const float scaler)                        const { return Angles2D (pitch - scaler, yaw - scaler); }
		inline Angles2D              &operator -=                 (const Angles2D &right)
		{
			pitch -= right.pitch;
			yaw -= right.yaw;

			return *this;
		}
		inline Angles2D              &operator -=                 (const float scaler)
		{
			pitch -= scaler;
			yaw -= scaler;

			return *this;
		}

		inline Angles2D               operator *                  (const Angles2D &right)                     const { return Angles2D (pitch * right.pitch, yaw * right.yaw); }
		inline Angles2D               operator *                  (const float scaler)                        const { return Angles2D (pitch * scaler, yaw * scaler); }
		friend inline Angles2D        operator *                  (const float scaler, const Angles2D &right)       { return right * scaler; }
		inline Angles2D              &operator *=                 (const Angles2D &right)
		{
			pitch *= right.pitch;
			yaw *= right.yaw;

			return *this;
		}
		inline Angles2D              &operator *=                 (const float scaler)
		{
			pitch *= scaler;
			yaw *= scaler;

			return *this;
		}

		inline Angles2D               operator /                  (const Angles2D &right)                        const
		{
			// Reliability check.
			InternalAssert (right.pitch != 0.0f && right.yaw != 0.0f);

			return Angles2D (pitch / right.pitch, yaw / right.yaw);
		}
		inline Angles2D               operator /                  (const float scaler)                        const
		{
			// Reliability check.
			InternalAssert (scaler != 0.0f);

			return *this * (1.0f / scaler);
		}
		inline Angles2D              &operator /=                 (const Angles2D &right)
		{
			// Reliability check.
			InternalAssert (right.pitch != 0.0f && right.yaw != 0.0f);

			pitch /= right.pitch;
			yaw /= right.yaw;

			return *this;
		}
		inline Angles2D              &operator /=                 (const float scaler)
		{
			// Reliability check.
			InternalAssert (scaler != 0.0f);

			return *this *= 1.0f / scaler;
		}

		inline Angles2D               operator -                  (void)                                      const { return GetInverted (); }	// negate angles, in general not the inverse rotation

		// assignment

		//
		// Function: operator =
		//
		// Description: Assigns the value to existing angles.
		//
		// Parameters:
		//	value - Value that should be assigned.
		//
		// Returns: Reassigned angles.
		//
		inline Angles2D              &operator =                  (const float value)
		{
			pitch = yaw = value;

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current angles with float pointer.
		//
		// Parameters:
		//	Angles2D - Float pointer that should be assigned.
		//
		// Returns: Reassigned angles.
		//
		inline Angles2D              &operator =                  (const float *const angles2D)
		{
			// Reliability check.
			InternalAssert (angles2D != NULL);

			pitch = angles2D[0u];
			yaw = angles2D[1u];

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current angles with specified one.
		//
		// Parameters:
		//	right - Other angles that should be assigned.
		//
		// Returns: Reassigned angles.
		//
		inline Angles2D              &operator =                  (const Angles2D &right)
		{
			pitch = right.pitch;
			yaw = right.yaw;

			return *this;
		}

		inline       Angles2D        &operator =                  (const Angles3D &right);	// Note: This function declared in 'Core tool library/Angles3D.hpp'.

	//
	// Group: Methods.
	//
	public:
		//
		// Function: Clamp
		//
		// Description: Clamps the angles.
		//
		// Returns: Clamped angles.
		//
		inline Angles2D &Clamp (void)
		{
			return ClampPitch ().ClampYaw ();
		}
		inline const Angles2D GetClamped (void) const
		{
			return Angles2D (AngleNormalize (pitch), AngleNormalize (yaw));
		}
		inline Angles2D &ClampPitch (void)
		{
			pitch = AngleNormalize (pitch);

			return *this;
		}
		inline const Angles2D GetWithClampedPitch (void) const
		{
			return Angles2D (AngleNormalize (pitch), yaw);
		}
		inline Angles2D &ClampYaw (void)
		{
			yaw = AngleNormalize (yaw);

			return *this;
		}
		inline const Angles2D GetWithClampedYaw (void) const
		{
			return Angles2D (pitch, AngleNormalize (yaw));
		}

		inline Angles2D &Clamp360 (void)
		{
			return ClampPitch360 ().ClampYaw360 ();
		}
		inline const Angles2D GetClamped360 (void) const
		{
			return Angles2D (AngleMod (pitch), AngleMod (yaw));
		}
		inline Angles2D &ClampPitch360 (void)
		{
			pitch = AngleMod (pitch);

			return *this;
		}
		inline const Angles2D GetWithClampedPitch360 (void) const
		{
			return Angles2D (AngleMod (pitch), yaw);
		}
		inline Angles2D &ClampYaw360 (void)
		{
			yaw = AngleMod (yaw);

			return *this;
		}
		inline const Angles2D GetWithClampedYaw360 (void) const
		{
			return Angles2D (pitch, AngleMod (yaw));
		}

		inline const Angles2D GetDifferenceBetweenAngles (const Angles2D &other) const
		{
			return Angles2D (Math::GetDifferenceBetweenAngles (other.pitch, pitch), Math::GetDifferenceBetweenAngles (other.yaw, yaw));
		}
		inline Angles2D &SetDifferenceBetweenAngles (const Angles2D &other)
		{
			return *this = GetDifferenceBetweenAngles (other);
		}

		inline const bool AreEqual (const Angles2D &other) const
		{
			return Math::AreEqual (pitch, other.pitch) && Math::AreEqual (yaw, other.yaw);
		}
		inline const bool AreEqual (const Angles2D &other, const float tolerance) const
		{
			return Math::AreEqual (pitch, other.pitch, tolerance) && Math::AreEqual (yaw, other.yaw, tolerance);
		}

		//
		// Function: Swap
		//
		// Description: Exchange the contents of this vector with another.
		//
		// Parameters:
		//	vector - Other vector, to swap with this one.
		//
		inline void Swap (Angles2D &other)
		{
			CoreToolLibrary::SwapElements (*this, other);
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
		inline const float GetLengthSquared (void) const { return MATH_GET_SQUARED (pitch) + MATH_GET_SQUARED (yaw); }

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
		// Function: Invert
		//
		// Description: Inverts the vector.
		//
		// Returns: Inverted vector.
		//
		inline Angles2D &Invert (void)
		{
			return *this = GetInverted ();
		}
		inline const Angles2D GetInverted (void) const
		{
			return Angles2D (-pitch, -yaw);
		}

		//
		// Function: BuildVectors
		//
		// Description:
		//	This function builds a 3D referential from a view angle, that is to say, the relative
		//	"forward", "right" and "upward" direction that a player would have if he were facing this
		//	view angle. World angles are stored in Vector structs too, the "pitch" component corresponding
		//	to the X angle (horizontal angle), and the "yaw" component corresponding to the Y angle (vertical angle).
		//
		inline void BuildVectors (Vector3D /**const */&forward, Vector3D /**const */&right, Vector3D /**const */&upward) const
		{
			float sinePitch (0.0f), cosinePitch (0.0f), sineYaw (0.0f), cosineYaw (0.0f), sineRoll (0.0f), cosineRoll (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component
			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);		// compute the sine and cosine of the yaw component
/*! @todo COMPUTE AND STORE CONSTANT INSTEAD OF IT!!!!!*/			GetCosineSine (DegreeToRadian (0.0f), cosineRoll/* 1.0f */, sineRoll/* 0.0f */);	// compute the sine and cosine of the roll component

			// build the FORWARD vector
//			if (forward != NULL)
			{
				forward/*->*/.x = cosinePitch * cosineYaw;
				forward/*->*/.y = cosinePitch * sineYaw;
				forward/*->*/.z = -sinePitch;
			}

			// build the RIGHT vector
//			if (right != NULL)
			{
				right/*->*/.x = -sineRoll * sinePitch * cosineYaw + cosineRoll * sineYaw;
				right/*->*/.y = -sineRoll * sinePitch * sineYaw - cosineRoll * cosineYaw;
				right/*->*/.z = -sineRoll * cosinePitch;
			}

			// build the UPWARD vector
//			if (upward != NULL)
			{
				upward/*->*/.x = cosineRoll * sinePitch * cosineYaw + sineRoll * sineYaw;
				upward/*->*/.y = cosineRoll * sinePitch * sineYaw - sineRoll * cosineYaw;
				upward/*->*/.z = cosineRoll * cosinePitch;
			}
		}

		inline Vector3D BuildForwardVector (void) const
		{
/*			// compute the sine and cosine of the pitch component
			float angle = DegreeToRadian (pitch);
			Vector3D forward (0.0f, cosf (angle), -sinf (angle));

			// compute the sine and cosine of the yaw component
			angle = DegreeToRadian (yaw);

			// build the FORWARD vector
			forward.x = forward.y * cosf (angle);
			forward.y *= sinf (angle);

			return forward;*/
			float sinePitch (0.0f), cosinePitch (0.0f), sineYaw (0.0f), cosineYaw (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component
			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);		// compute the sine and cosine of the yaw component

			// build the FORWARD vector
			return Vector3D (cosinePitch * cosineYaw, cosinePitch * sineYaw, -sinePitch);
		}

		inline Vector3D BuildRightVector (void) const
		{
/*			// compute the sine and cosine of the yaw component
			float angle (DegreeToRadian (yaw));
			const float sineYaw (sinf (angle));
			const float cosineYaw (cosf (angle));

			// compute the sine and cosine of the roll component
			angle = DegreeToRadian (0.0f);
			const float cosineRoll (cosf (angle)/* 1.0f *//*);
			const float sineRoll (sinf (angle)/* 0.0f *//*);

			// compute the sine and cosine of the pitch component
			angle = DegreeToRadian (pitch);
			const float sinePitch (sinf (angle));

			// build the RIGHT vector
			return Vector3D (-sineRoll * sinePitch * cosineYaw + cosineRoll * sineYaw, -sineRoll * sinePitch * sineYaw - cosineRoll * cosineYaw, -sineRoll * cosf (angle));*/
			float sinePitch (0.0f), cosinePitch (0.0f), sineYaw (0.0f), cosineYaw (0.0f), sineRoll (0.0f), cosineRoll (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component
			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);		// compute the sine and cosine of the yaw component
/*! @todo COMPUTE AND STORE CONSTANT INSTEAD OF IT!!!!!*/			GetCosineSine (DegreeToRadian (0.0f), cosineRoll/* 1.0f */, sineRoll/* 0.0f */);	// compute the sine and cosine of the roll component

			// build the RIGHT vector
			return Vector3D (-sineRoll * sinePitch * cosineYaw + cosineRoll * sineYaw, -sineRoll * sinePitch * sineYaw - cosineRoll * cosineYaw, -sineRoll * cosinePitch);
		}

		inline Vector3D BuildUpwardVector (void) const
		{
/*			// compute the sine and cosine of the yaw component
			float angle (DegreeToRadian (yaw));
			const float sineYaw (sinf (angle));
			float cosineYaw (cosf (angle));

			// compute the sine and cosine of the roll component
			angle = DegreeToRadian (0.0f);
			const float cosineRoll (cosf (angle)/* 1.0f *//*);
			const float sineRoll (sinf (angle)/* 0.0f *//*);

			// compute the sine and cosine of the pitch component
			angle = DegreeToRadian (pitch);
			const float sinePitch (sinf (angle));

			// build the UPWARDS vector
			return Vector3D (cosineRoll * sinePitch * cosineYaw + sineRoll * sineYaw, cosineRoll * sinePitch * sineYaw - sineRoll * cosineYaw, cosineRoll * cosf (angle));*/
			float sinePitch (0.0f), cosinePitch (0.0f), sineYaw (0.0f), cosineYaw (0.0f), sineRoll (0.0f), cosineRoll (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component
			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);		// compute the sine and cosine of the yaw component
/*! @todo COMPUTE AND STORE CONSTANT INSTEAD OF IT!!!!!*/			GetCosineSine (DegreeToRadian (0.0f), cosineRoll/* 1.0f */, sineRoll/* 0.0f */);	// compute the sine and cosine of the roll component

			// build the UPWARD vector
			return Vector3D (cosineRoll * sinePitch * cosineYaw + sineRoll * sineYaw, cosineRoll * sinePitch * sineYaw - sineRoll * cosineYaw, cosineRoll * cosinePitch);
		}

		/*inline */void MakeVectors       (void) const;

		/*inline */void MakeForwardVector (void) const;
		/*inline */void MakeRightVector   (void) const;
		/*inline */void MakeUpwardVector  (void) const;
};

inline Angles2D Vector2D::ToAngles2D (void) const
{
	// this function computes the world angles towards which is directed the vector passed in by
	// this vector. World angles are how much degrees on the horizontal plane and how much on the
	// vertical plane one has to turn to face a certain direction (here, the direction the vector
	// is pointing towards).

	// Is the input vector absolutely vertical?
	if (IsZero ())
		return Angles2D (-90.0f, 0.0f);	// is the input vector pointing up?, if so look upwards, otherwise look downwards

	// Else it's another sort of vector, compute individually the pitch and yaw corresponding to this vector and return the corresponding view angles.
	return Angles2D (0.0f, RadianToDegree (atan2f (y, x)));
}

inline Vector3D &Vector3D::Rotate (const Angles2D &angles)
{
	return RotateZ (angles.yaw).RotateY (angles.pitch);
}
inline const Vector3D Vector3D::GetRotated (const Angles2D &angles) const
{
	return GetRotatedZ (angles.yaw).GetRotatedY (angles.pitch);
}

inline Angles2D Vector3D::ToAngles2D (void) const
{
	// this function computes the world angles towards which is directed the vector passed in by
	// this vector. World angles are how much degrees on the horizontal plane and how much on the
	// vertical plane one has to turn to face a certain direction (here, the direction the vector
	// is pointing towards).

	// Is the input vector absolutely vertical?
	if (IsZero2D ())
		return Angles2D (z > 0.0f ? 90.0f : -90.0f, 0.0f);	// is the input vector pointing up?, if so look upwards, otherwise look downwards

	// Else it's another sort of vector, compute individually the pitch and yaw corresponding to this vector and return the corresponding view angles.
	return Angles2D (RadianToDegree (atan2f (z, GetLength2D ())), RadianToDegree (atan2f (y, x)));
}

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_ANGLES_2D_INCLUDED