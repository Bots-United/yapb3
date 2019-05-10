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
// Angles3D.hpp
//
// Class: Angles3D
//
// Link: http://developer.valvesoftware.com/wiki/QAngle
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ANGLES_INCLUDED
#define ANGLES_INCLUDED

class Angles3D
{
	//
	// Group: Constants.
	//
	public:
		enum AngleID_t
		{
			AngleID_Pitch,	// +down/-up
			AngleID_Yaw,	// +left/-right
			AngleID_Roll,	// +right/-left (fall over)

			AngleID_Total
		};

		typedef float Pointer_t[AngleID_Total];

	//
	// Group: Pitch, yaw and roll angle members.
	//
	public:
		float pitch;	// Counterclockwise rotation around the CAMERA_RIGHT axis, in degrees [-90, 90]. (horizontal)
		float yaw;		// Counterclockwise rotation around the CAMERA_UP axis, in degrees [-359, 359]. (vertical)
		float roll;		// Counterclockwise rotation around the CAMERA_FORWARD axis, in degrees [-359, 359]. (also vertical)

	//
	// Group: Special points.
	//
	public:
		CLASS_GLOBAL static const Angles3D ZeroValue;	// Zero angles value.

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: Angles3D
		//
		// Description: Constructs zero Angles3D.
		//
		inline const Angles3D (void) : pitch (0.0f), yaw (0.0f), roll (0.0f) { /*VOID*/ }

		//
		// Function: Angles3D
		//
		// Description: Standard Angles3D Constructor.
		//
		// Parameters:
		//	inputX - Input pitch angle.
		//	inputY - Input yaw angle.
		//	inputZ - Input roll angle.
		//
		inline const Angles3D (const float inputPitch, const float inputYaw, const float inputRoll) : pitch (inputPitch), yaw (inputYaw), roll (inputRoll) { /*VOID*/ }

		//
		// Function: Angles3D
		//
		// Description: Constructs Angles3D from float pointer.
		//
		// Parameters:
		//	angles3D - Float pointer to assign.
		//
		inline const Angles3D (const float *const angles3D) : pitch (angles3D[AngleID_Pitch]), yaw (angles3D[AngleID_Yaw]), roll (angles3D[AngleID_Roll]) { /*VOID*/ }

		//
		// Function: Angles3D
		//
		// Description: Constructs Angles3D from Angles2D.
		//
		// Parameters:
		//	right - Angles2D, that should be assigned.
		//
		inline const Angles3D (const Angles2D &right) : pitch (right.pitch), yaw (right.yaw), roll (0.0f) { /*VOID*/ }

		//
		// Function: Angles3D
		//
		// Description: Constructs Angles3D from another Angles3D.
		//
		// Parameters:
		//	right - Other Angles3D, that should be assigned.
		//
		inline const Angles3D (const Angles3D &right) : pitch (right.pitch), yaw (right.yaw), roll (right.roll) { /*VOID*/ }

	//
	// Group: Operators.
	//
	public:
		// Equality.
		inline const bool           operator ==                 (const Angles3D &right)                     const { return pitch == right.pitch && yaw == right.yaw && roll == right.roll; }
		inline const bool           operator !=                 (const Angles3D &right)                     const { return pitch != right.pitch && yaw != right.yaw && roll != right.roll; }

		// array access
		inline float               &operator []                 (const AngleID_t angleID)
		{
			// Reliability check.
			InternalAssert (angleID < AngleID_Total);

			return (&pitch)[angleID];
		}
		inline const float         &operator []                 (const AngleID_t angleID)                 const
		{
			// Reliability check.
			InternalAssert (angleID < AngleID_Total);

			return (&pitch)[angleID];
		}

		// casting operators
		inline                      operator       float *      (void)                                          { return &pitch; }	// angles will now automatically convert to 'float *' when needed
		inline                      operator const float *const (void)                                    const { return &pitch; }	// angles will now automatically convert to 'const float *const' when needed
		inline                      operator       Angles2D     (void)                                    const { return Angles2D (pitch, yaw); }	// vectors will now automatically convert to 'Angles2D' when needed
		inline                      operator       Angles2D &   (void)                                          { return AsAngles2D (); }	// vectors will now automatically convert to 'Angles2D &' when needed
		inline                      operator const Angles2D &   (void)                                    const { return AsAngles2D (); }	// vectors will now automatically convert to 'const Angles2D &' when needed

		// arithmetic operations
		inline Angles3D               operator +                  (const Angles2D &right)                     const { return Angles3D (pitch + right.pitch, yaw + right.yaw, roll); }
		inline Angles3D               operator +                  (const Angles3D &right)                     const { return Angles3D (pitch + right.pitch, yaw + right.yaw, roll + right.roll); }
		inline Angles3D               operator +                  (const float scaler)                        const { return Angles3D (pitch + scaler, yaw + scaler, roll + scaler); }
		inline Angles3D              &operator +=                 (const Angles3D &right)
		{
			pitch += right.pitch;
			yaw += right.yaw;
			roll += right.roll;

			return *this;
		}
		inline Angles3D              &operator +=                 (const float scaler)
		{
			pitch += scaler;
			yaw += scaler;
			roll += scaler;

			return *this;
		}

		inline Angles3D               operator -                  (const Angles2D &right)                     const { return Angles3D (pitch - right.pitch, yaw - right.yaw, roll); }
		inline Angles3D               operator -                  (const Angles3D &right)                     const { return Angles3D (pitch - right.pitch, yaw - right.yaw, roll - right.roll); }
		inline Angles3D               operator -                  (const float scaler)                        const { return Angles3D (pitch - scaler, yaw - scaler, roll - scaler); }
		inline Angles3D              &operator -=                 (const Angles3D &right)
		{
			pitch -= right.pitch;
			yaw -= right.yaw;
			roll -= right.roll;

			return *this;
		}
		inline Angles3D              &operator -=                 (const float scaler)
		{
			pitch -= scaler;
			yaw -= scaler;
			roll -= scaler;

			return *this;
		}

		inline Angles3D               operator *                  (const Angles3D &right)                     const { return Angles3D (pitch * right.pitch, yaw * right.yaw, roll * right.roll); }
		inline Angles3D               operator *                  (const float scaler)                        const { return Angles3D (pitch * scaler, yaw * scaler, roll * scaler); }
		friend inline const Angles3D  operator *                  (const float scaler, const Angles3D &right)       { return right * scaler; }
		inline Angles3D              &operator *=                 (const Angles3D &right)
		{
			pitch *= right.pitch;
			yaw *= right.yaw;
			roll *= right.roll;

			return *this;
		}
		inline Angles3D              &operator *=                 (const float scaler)
		{
			pitch *= scaler;
			yaw *= scaler;
			roll *= scaler;

			return *this;
		}

		inline Angles3D               operator /                  (const Angles3D &right)                     const
		{
			// Reliability check.
			InternalAssert (right.pitch != 0.0f && right.yaw != 0.0f && right.roll != 0.0f);

			return Angles3D (pitch / right.pitch, yaw / right.yaw, roll / right.roll);
		}
		inline Angles3D               operator /                  (const float scaler)                      const
		{
			// Reliability check.
			InternalAssert (scaler != 0.0f);

			return *this * (1.0f / scaler);
		}
		inline Angles3D              &operator /=                 (const Angles3D &right)
		{
			// Reliability check.
			InternalAssert (right.pitch != 0.0f && right.yaw != 0.0f && right.roll != 0.0f);

			pitch /= right.pitch;
			yaw /= right.yaw;
			roll /= right.roll;

			return *this;
		}
		inline Angles3D              &operator /=                 (const float scaler)
		{
			// Reliability check.
			InternalAssert (scaler != 0.0f);

			return *this *= 1.0f / scaler;
		}

		inline Angles3D               operator -                  (void)                                    const { return GetInverted (); }	// negate angles, in general not the inverse rotation

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
		inline Angles3D              &operator =                  (const float value)
		{
			pitch = yaw = roll = value;

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current angles with float pointer.
		//
		// Parameters:
		//	Angles3D - Float pointer that should be assigned.
		//
		// Returns: Reassigned angles.
		//
		inline Angles3D              &operator =                  (const float *const angles3D)
		{
			// Reliability check.
			InternalAssert (angles3D != NULL);

			pitch = angles3D[0u];
			yaw = angles3D[1u];
			roll = angles3D[2u];

			return *this;
		}

		inline Angles3D              &operator =                  (const Angles2D &right)
		{
			pitch = right.pitch;
			yaw = right.yaw;
			roll = 0.0f;

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
		inline Angles3D              &operator =                  (const Angles3D &right)
		{
			pitch = right.pitch;
			yaw = right.yaw;
			roll = right.roll;

			return *this;
		}

	//
	// Group: Methods.
	//
	public:
		// Cast to Angles2D....
		inline       Angles2D &AsAngles2D (void)       { return *reinterpret_cast <Angles2D *const> (this); }
		inline const Angles2D &AsAngles2D (void) const { return *reinterpret_cast <const Angles2D *const> (this); }

		//
		// Function: Clamp
		//
		// Description: Clamps the angles.
		//
		// Returns: Clamped angles.
		//
		inline Angles3D &Clamp (void)
		{
			return ClampPitch ().ClampYaw ().ClampRoll ();
		}
		inline const Angles3D GetClamped (void) const
		{
			return Angles3D (AngleNormalize (pitch), AngleNormalize (yaw), 0.0f);
		}
		inline Angles3D &ClampPitch (void)
		{
			pitch = AngleNormalize (pitch);

			return *this;
		}
		inline const Angles3D GetWithClampedPitch (void) const
		{
			return Angles3D (AngleNormalize (pitch), yaw, 0.0f);
		}
		inline Angles3D &ClampYaw (void)
		{
			yaw = AngleNormalize (yaw);

			return *this;
		}
		inline const Angles3D GetWithClampedYaw (void) const
		{
			return Angles3D (pitch, AngleNormalize (yaw), 0.0f);
		}
		inline Angles3D &ClampRoll (void)
		{
			roll = 0.0f;	// ignore roll component

			return *this;
		}
		inline const Angles3D GetWithClampedRoll (void) const
		{
			return Angles3D (pitch, yaw, 0.0f);	// ignore roll component
		}

		inline Angles3D &Clamp360 (void)
		{
			return ClampPitch360 ().ClampYaw360 ().ClampRoll360 ();
		}
		inline const Angles3D GetClamped360 (void) const
		{
			return Angles3D (AngleMod (pitch), AngleMod (yaw), 0.0f);
		}
		inline Angles3D &ClampPitch360 (void)
		{
			pitch = AngleMod (pitch);

			return *this;
		}
		inline const Angles3D GetWithClampedPitch360 (void) const
		{
			return Angles3D (AngleMod (pitch), yaw, 0.0f);
		}
		inline Angles3D &ClampYaw360 (void)
		{
			yaw = AngleMod (yaw);

			return *this;
		}
		inline const Angles3D GetWithClampedYaw360 (void) const
		{
			return Angles3D (pitch, AngleMod (yaw), 0.0f);
		}
		inline Angles3D &ClampRoll360 (void)
		{
			roll = 0.0f;	// ignore roll component

			return *this;
		}
		inline const Angles3D GetWithClampedRoll360 (void) const
		{
			return Angles3D (pitch, yaw, 0.0f);	// ignore roll component
		}

		inline const Angles3D GetDifferenceBetweenAngles (const Angles3D &other) const
		{
			return Angles3D (Math::GetDifferenceBetweenAngles (other.pitch, pitch), Math::GetDifferenceBetweenAngles (other.yaw, yaw), Math::GetDifferenceBetweenAngles (other.roll, roll));
		}
		inline Angles3D &SetDifferenceBetweenAngles (const Angles3D &other)
		{
			return *this = GetDifferenceBetweenAngles (other);
		}

		inline const bool AreEqual (const Angles3D &other) const
		{
			return Math::AreEqual (pitch, other.pitch) && Math::AreEqual (yaw, other.yaw) && Math::AreEqual (roll, other.roll);
		}
		inline const bool AreEqual (const Angles3D &other, const float tolerance) const
		{
			return Math::AreEqual (pitch, other.pitch, tolerance) && Math::AreEqual (yaw, other.yaw, tolerance) && Math::AreEqual (roll, other.roll, tolerance);
		}

		//
		// Function: Swap
		//
		// Description: Exchange the contents of this vector with another.
		//
		// Parameters:
		//	vector - Other vector, to swap with this one.
		//
		inline void Swap (Angles3D &other)
		{
			CoreToolLibrary::SwapElements (*this, other);
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
		inline const float GetLengthSquared (void) const { return MATH_GET_SQUARED (pitch) + MATH_GET_SQUARED (yaw) + MATH_GET_SQUARED (roll); }

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
		inline const float GetLengthSquared2D (void) const { return MATH_GET_SQUARED (pitch) + MATH_GET_SQUARED (yaw); }

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
		// Function: Invert
		//
		// Description: Inverts the vector.
		//
		// Returns: Inverted vector.
		//
		inline Angles3D &Invert (void)
		{
			return *this = GetInverted ();
		}
		inline const Angles3D GetInverted (void) const
		{
			return Angles3D (-pitch, -yaw, -roll);
		}

		//
		// Function: SkipRoll
		//
		// Description: Gets vector without Z axis.
		//
		// Returns: 2D vector from 3D vector.
		//
		inline const Angles3D SkipRoll (void) const { return Angles3D (pitch, yaw, 0.0f); }

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
			GetCosineSine (DegreeToRadian (roll), cosineRoll, sineRoll);	// compute the sine and cosine of the roll component

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
		inline void BuildTransposedVectors (Vector3D /**const */&forward, Vector3D /**const */&right, Vector3D /**const */&upward) const
		{
			float sinePitch (0.0f), cosinePitch (0.0f), sineYaw (0.0f), cosineYaw (0.0f), sineRoll (0.0f), cosineRoll (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component
			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);		// compute the sine and cosine of the yaw component
			GetCosineSine (DegreeToRadian (roll), cosineRoll, sineRoll);	// compute the sine and cosine of the roll component

			// build the FORWARD vector
//			if (forward != NULL)
			{
				forward/*->*/.x = cosinePitch * cosineYaw;
				forward/*->*/.y = sineRoll * sinePitch * cosineYaw + cosineRoll * -sineYaw;
				forward/*->*/.z = cosineRoll * sinePitch * cosineYaw + -sineRoll * -sineYaw;
			}

			// build the RIGHT vector
//			if (right != NULL)
			{
				right/*->*/.x = cosinePitch * sineYaw;
				right/*->*/.y = sineRoll * sinePitch * sineYaw + cosineRoll * cosineYaw;
				right/*->*/.z = cosineRoll * sinePitch * sineYaw + -sineRoll * cosineYaw;
			}

			// build the UPWARD vector
//			if (upward != NULL)
			{
				upward/*->*/.x = -sinePitch;
				upward/*->*/.y = sineRoll * cosinePitch;
				upward/*->*/.z = cosineRoll * cosinePitch;
			}
		}

		inline Vector3D BuildForwardVector (void) const
		{
/*			// compute the sine and cosine of the pitch component
			float angle (DegreeToRadian (pitch));
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
			angle = DegreeToRadian (roll);
			const float sineRoll (sinf (angle));
			const float cosineRoll (cosf (angle));

			// compute the sine and cosine of the pitch component
			angle = DegreeToRadian (pitch);
			const float sinePitch (sinf (angle));

			// build the RIGHT vector
			return Vector3D (-sineRoll * sinePitch * cosineYaw + cosineRoll * sineYaw, -sineRoll * sinePitch * sineYaw - cosineRoll * cosineYaw, -sineRoll * cosf (angle));*/
			float sinePitch (0.0f), cosinePitch (0.0f), sineYaw (0.0f), cosineYaw (0.0f), sineRoll (0.0f), cosineRoll (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component
			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);		// compute the sine and cosine of the yaw component
			GetCosineSine (DegreeToRadian (roll), cosineRoll, sineRoll);	// compute the sine and cosine of the roll component

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
			angle = DegreeToRadian (roll);
			const float sineRoll (sinf (angle));
			const float cosineRoll (cosf (angle));

			// compute the sine and cosine of the pitch component
			angle = DegreeToRadian (pitch);
			const float sinePitch (sinf (angle));

			// build the UPWARDS vector
			return Vector3D (cosineRoll * sinePitch * cosineYaw + sineRoll * sineYaw, cosineRoll * sinePitch * sineYaw - sineRoll * cosineYaw, cosineRoll * cosf (angle));*/
			float sinePitch (0.0f), cosinePitch (0.0f), sineYaw (0.0f), cosineYaw (0.0f), sineRoll (0.0f), cosineRoll (0.0f);

			GetCosineSine (DegreeToRadian (pitch), cosinePitch, sinePitch);	// compute the sine and cosine of the pitch component
			GetCosineSine (DegreeToRadian (yaw), cosineYaw, sineYaw);		// compute the sine and cosine of the yaw component
			GetCosineSine (DegreeToRadian (roll), cosineRoll, sineRoll);	// compute the sine and cosine of the roll component

			// build the UPWARD vector
			return Vector3D (cosineRoll * sinePitch * cosineYaw + sineRoll * sineYaw, cosineRoll * sinePitch * sineYaw - sineRoll * cosineYaw, cosineRoll * cosinePitch);
		}

		/*inline */void MakeVectors           (void) const;
		/*inline */void MakeTransposedVectors (void) const;

		/*inline */void MakeForwardVector     (void) const;
		/*inline */void MakeRightVector       (void) const;
		/*inline */void MakeUpwardVector      (void) const;

		inline const char *const ToString (void) const
		{
			return FormatBuffer ("(%f, %f, %f)", pitch, yaw, roll);
		}
};

inline Angles2D::Angles2D (const Angles3D &right) : pitch (right.pitch), yaw (right.yaw) { /*VOID*/ }

inline Angles2D &Angles2D::operator = (const Angles3D &right)
{
	pitch = right.pitch;
	yaw = right.yaw;

	return *this;
}

inline Vector3D &Vector3D::Rotate (const Angles3D &angles)
{
	return RotateZ (angles.yaw).RotateY (angles.pitch).RotateX (angles.roll);
}
inline const Vector3D Vector3D::GetRotated (const Angles3D &angles) const
{
	return GetRotatedZ (angles.yaw).GetRotatedY (angles.pitch).GetRotatedX (angles.roll);
}

inline Angles3D Vector3D::ToAngles3D (void) const
{
	// this function computes the world angles towards which is directed the vector passed in by
	// this vector. World angles are how much degrees on the horizontal plane and how much on the
	// vertical plane one has to turn to face a certain direction (here, the direction the vector
	// is pointing towards).

	// Is the input vector absolutely vertical?
	if (IsZero2D ())
		return Angles3D (z > 0.0f ? 90.0f : -90.0f, 0.0f, 0.0f);	// is the input vector pointing up?, if so look upwards, otherwise look downwards

	// Else it's another sort of vector, compute individually the pitch and yaw corresponding to this vector and return the corresponding view angles.
	return Angles3D (RadianToDegree (atan2f (z, GetLength2D ())), RadianToDegree (atan2f (y, x)), 0.0f);
}

class AnglesAxes
{
	// 3D referential structure definition

	// this class considers a system of angles (most common example, a view angle), and sets
	// a vector referential upon it, in order to have the relative direction of "up", "right" and "forward".

	public:
		Vector3D forward;	// normalized vector pointing forward, perpendicular both to 'upward' and 'right'.
		Vector3D right;		// normalized vector pointing right, perpendicular both to 'forward' and 'upward'.
		Vector3D upward;	// normalized vector pointing upwards, perpendicular both to 'forward' and 'right'.

	public:
		inline AnglesAxes (const Vector3D &inputForward, const Vector3D &inputRight, const Vector3D &inputUpward) :
			forward (inputForward),
			right (inputRight),
			upward (inputUpward)
		{ /* VOID */ }
		inline AnglesAxes (const Angles3D &angles) { angles.BuildVectors (forward, right, upward); }

	public:
		inline AnglesAxes &operator = (const Angles3D &angles) { angles.BuildVectors (forward, right, upward); }	// return the new referential
};

#endif	// ifndef ANGLES_INCLUDED