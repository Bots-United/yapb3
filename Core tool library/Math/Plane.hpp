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
// Plane.hpp
//
// Class: Plane
//
// Description: The plane is represented as Dot(N,X) = c where N is a unit-length
//				normal vector, c is the plane constant, and X is any point on the
//				plane. The user must ensure that the normal vector satisfies this condition.
//
// Warning: DO NOT CHANGE MEMBERS OF THIS CLASS AND ALSO DO NOT MAKE THIS CLASS AS VIRTUAL!!! (THIS CLASS USED BY HL ENGINE)
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_PLANE_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_PLANE_INCLUDED

class Plane
{
	public:
		enum PointIndex_t
		{
			PointIndex_0,
			PointIndex_1,
			PointIndex_2,
			PointIndex_3,

			PointIndex_Total
		};
		typedef Vector3D Points_t[PointIndex_Total];
		enum PointSide_t
		{
			PointSide_Front,	// The point is in front of the plane.
			PointSide_Back,		// The point is behind of the plane.

			PointSide_OnPlane	// The point is within the plane.
		};

		#define BOGUS_RANGE 10000.0f

	public:
		Vector3D normal;	// vector coordinates of the plane normal - surface normal (Vector orthogonal to plane (Ax, Ay, Az) with Ax2+Ay2+Az2 = 1.0)
		float    distance;	// distance from the map center (0.0, 0.0, 0.0) to a plane along normal - closest appoach to world origin. (Offset to plane, along the normal vector) (Plane equation is: normal * X = distance)

	public:
		inline Plane (void) :
			normal (Vector3D::ZeroValue),
			distance (0.0f)
		{ /* VOID */ }

		inline Plane (const Plane &otherPlane) :
			normal (otherPlane.normal),
			distance (otherPlane.distance)
		{ /* VOID */ }

		// specify N and c directly
		inline Plane (const Vector3D &inputNormal, const float inputDistance) :
			normal (inputNormal),
			distance (inputDistance)
		{ /* VOID */ }

		// N is specified, c = Dot(N,P) where P is on the plane
		inline Plane (const Vector3D &inputNormal, const Vector3D &point) :
			normal (inputNormal),
			distance (inputNormal | point)
		{ /* VOID */ }

		// N = Cross(P1-P0,P2-P0)/Length(Cross(P1-P0,P2-P0)), c = Dot(N,P0) where P0, P1, P2 are points on the plane.
		inline Plane (const Vector3D &point0, const Vector3D &point1, const Vector3D &point2)
		{
			const Vector3D edge1 (point1 - point0);
			const Vector3D edge2 (point2 - point0);

			normal = edge1.GetUnitCrossProduct (edge2);
			distance = normal | point0;
		}
		inline Plane (const Vector3D *const points)
		{
			// Reliability check.
			InternalAssert (points != NULL);

			const Vector3D edge1 (points[PointIndex_1] - points[PointIndex_0]);
			const Vector3D edge2 (points[PointIndex_2] - points[PointIndex_0]);

			normal = edge1.GetUnitCrossProduct (edge2);
			distance = normal | points[PointIndex_0];
		}

	public:
		inline Plane &operator = (const Plane &right)
		{
			normal = right.normal;
			distance = right.distance;

			return *this;
		}
		inline Plane &operator = (const Vector3D *const points)
		{
			// Reliability check.
			InternalAssert (points != NULL);

			const Vector3D edge1 (points[PointIndex_1] - points[PointIndex_0]);
			const Vector3D edge2 (points[PointIndex_2] - points[PointIndex_0]);

			normal = edge1.GetUnitCrossProduct (edge2);
			distance = normal | points[PointIndex_0];

			return *this;
		}

		inline const bool operator == (const Plane &right) const { return normal == right.normal && distance == right.distance; }
		inline const bool operator != (const Plane &right) const { return normal != right.normal || distance != right.distance; }

	public:
		// N = Cross(P1-P0,P2-P0)/Length(Cross(P1-P0,P2-P0)) where P0, P1, P2 are points on the plane.
		static Vector3D CalculateAndGetNormal (const Vector3D &point0, const Vector3D &point1, const Vector3D &point2)
		{
			const Vector3D edge1 (point1 - point0);
			const Vector3D edge2 (point2 - point0);

			return edge1.GetUnitCrossProduct (edge2);
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from
		which the triangle is definitely visible when looking into
		the given direction.
		Note that this only works if the normal is Normalized.
		Do not use this method with points as it will give wrong results!
		\param lookDirection: Look direction.
		\return True if the plane is front facing and
		false if it is backfacing. */
		inline const bool IsFrontFacing (const Vector3D &lookDirection) const
		{
			const float dot (normal | lookDirection);

			return dot <= 0.0f;
		}

		inline const float GetDistanceToPoint (const Vector3D &point) const
		{
			// Get the distance to a point.
			// How far off the plane is this point?

			// Compute d = Dot(N,Q)-c where N is the plane normal and c is the plane
			// constant. This is a signed distance. The sign of the return value is
			// positive if the point is on the positive side of the plane, negative if
			// the point is on the negative side, and zero if the point is on the
			// plane.

			const float face ((normal | point) - distance);

			return face;
		}
		inline const bool PointInFront (const Vector3D &point) const
		{
			// Determines whether the given vector is in front of the plane.

			return GetDistanceToPoint (point) > 0.0f;
		}
		inline const PointSide_t GetPointSide (const Vector3D &point, const float offset = 0.0f) const
		{
			// Classifies the relation of a point to this plane.
			// The "positive side" of the plane is the half space to which the plane
			// normal points. The "negative side" is the other half space. The
			// function returns +1 for the positive side, -1 for the negative side,
			// and 0 for the point being on the plane.

			const float distanceToPoint (GetDistanceToPoint (point));

			if (distanceToPoint < -offset)
				return PointSide_Back;

			if (distanceToPoint > +offset)
				return PointSide_Front;

			return PointSide_OnPlane;
		}

		inline const Vector3D GetMemberPoint (void) const
		{
			// Gets a member point of the plane.

			return normal * distance;
		}

		inline const bool IsIntersects (const Plane &other) const
		{
			// If Cross(N0,N1) is zero, then either planes are parallel and separated
			// or the same plane. In both cases, 'false' is returned. Otherwise, the
			// planes intersect. To avoid subtle differences in reporting between
			// Test() and Find(), the same parallel test is used. Mathematically,
			//   |Cross(N0,N1)|^2 = Dot(N0,N0)*Dot(N1,N1)-Dot(N0,N1)^2
			//                    = 1 - Dot(N0,N1)^2
			// The last equality is true since planes are required to have unit-length
			// normal vectors. The test |Cross(N0,N1)| = 0 is the same as
			// |Dot(N0,N1)| = 1. I test the latter condition in Test() and Find().

			const float dot (normal | other.normal);

			return IsZero (dot, 1.0f - ZeroTolerance);
		}

		//! Get an intersection with a 3d line.
		/** \param rayPoint Point of the line to intersect with.
		\param rayDirection Direction of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		inline const bool GetIntersectionWithRay (const Vector3D &rayPoint, const Vector3D &rayDirection, Vector3D &outIntersection) const
		{
			const float DdN (normal | rayDirection);

			if (IsZero (DdN))
				return false;	// The Line and plane are parallel.

			const float signedDistance (GetDistanceToPoint (rayPoint));
			const float t (-signedDistance / DdN);

			outIntersection = rayPoint + rayDirection * t;

			return true;
		}

		//! Intersects this plane with another.
		/** \param other Other plane to intersect with.
		\param outRayPoint Base point of intersection line.
		\param outRayDirection Line direction of intersection.
		\return True if there is a intersection, false if not. */
		inline const bool GetIntersectionWithPlane (const Plane &other, Vector3D &outRayPoint, Vector3D &outRayDirection) const
		{
			// If N0 and N1 are parallel, either the planes are parallel and separated
			// or the same plane. In both cases, 'false' is returned. Otherwise,
			// the intersection line is
			//   L(t) = t*Cross(N0,N1)/|Cross(N0,N1)| + c0*N0 + c1*N1
			// for some coefficients c0 and c1 and for t any real number (the line
			// parameter). Taking dot products with the normals,
			//   d0 = Dot(N0,L) = c0*Dot(N0,N0) + c1*Dot(N0,N1) = c0 + c1*d
			//   d1 = Dot(N1,L) = c0*Dot(N0,N1) + c1*Dot(N1,N1) = c0*d + c1
			// where d = Dot(N0,N1). These are two equations in two unknowns. The
			// solution is
			//   c0 = (d0 - d*d1)/det
			//   c1 = (d1 - d*d0)/det
			// where det = 1 - d^2.

			const float dot (normal | other.normal);

			if (!IsZero (dot, 1.0f - ZeroTolerance))
				return false;	// The planes are parallel.

			const float det (1.0f - MATH_GET_SQUARED (dot));
			const float invDet (1.0f / det);
			const float c0 ((distance - dot * other.distance) * invDet);
			const float c1 ((other.distance - dot * distance) * invDet);

			outRayPoint = normal * c0 + other.normal * c1;
			outRayDirection = normal.GetUnitCrossProduct (other.normal);

			return true;
		}

		//! Get the intersection point with two other planes if there is one.
		inline const bool GetIntersectionWithPlanes (const Plane &plane1, const Plane &plane2, Vector3D &outPoint) const
		{
			Vector3D rayPoint, rayDirection;

			if (GetIntersectionWithPlane (plane1, rayPoint, rayDirection))
				return plane2.GetIntersectionWithRay (rayPoint, rayDirection, outPoint);

			return false;
		}

		inline void GetWinding (Points_t &points, const float size = BOGUS_RANGE) const
		{
			// Creates normal-oriented rectangle with size, provided by 'size' value.

			float maximumPoint (-BOGUS_RANGE), value;
			Vector3D upward (Vector3D::ZeroValue);
			Vector3D::AxisID_t majorAxisIndex (Vector3D::AxisID_Total);

			// find the major axis
			for (unsigned char axisIndex (Vector3D::AxisID_X); axisIndex < Vector3D::AxisID_Total; ++axisIndex)
			{
				value = fabsf (normal[axisIndex]);

				if (maximumPoint < value)
				{
					maximumPoint = value;
					majorAxisIndex = static_cast <Vector3D::AxisID_t> (axisIndex);
				}
			}

			// Build a unit vector along something other than the major axis.
			switch (majorAxisIndex)
			{
				case Vector3D::AxisID_X:
				case Vector3D::AxisID_Y:
					upward.z = 1.0f;

					break;

				case Vector3D::AxisID_Z:
					upward.x = 1.0f;

					break;

				#if defined _DEBUG
					default:	// Vector3D::AxisID_Total
						AddLogEntry (false, LogLevel_Critical, true, "Plane::GetWinding(): No major axis found for normal: %s!", normal.ToString ());
				#endif	// if defined _DEBUG
			}
/*
			// find the major axis
			// Build a unit vector along something other than the major axis.
			if (fabsf (normal.z) > fabsf (normal.x) && fabsf (normal.z) > fabsf (normal.y))
				upward.x = 1.0f;
			else
				upward.z = 1.0f;
*/
			upward += -(upward | normal) * normal;	// Remove the component of this vector along the normal
			upward.Normalize ();	// Make it a unit (perpendicular)

			const Vector3D direction (GetMemberPoint ());	// Center of the poly is at normal * distance
			Vector3D right (upward ^ normal);	// Calculate the third orthonormal basis vector for our plane space (this one and vup are in the plane)

			// Make the plane's basis vectors big (these are the half-sides of the polygon we're making)
			upward *= size;
			right *= size;

			const Vector3D directionLeft  (direction - right);	// left
			const Vector3D directionRight (direction + right);	// right

			// project a really big axis aligned box onto the plane
			// Move diagonally away from org to create the corner verts
			points[PointIndex_0] = directionLeft;	// left
			points[PointIndex_0] += upward;			// up

			points[PointIndex_1] = directionRight;	// right
			points[PointIndex_1] += upward;			// up

			points[PointIndex_2] = directionRight;	// right
			points[PointIndex_2] -= upward;			// down

			points[PointIndex_3] = directionLeft;	// left
			points[PointIndex_3] -= upward;			// down

			// The four corners form a planar quadrilateral normal to "normal"
		}
		inline void GetWinding (const Vector3D &origin, Points_t &points, const float size = BOGUS_RANGE) const
		{
			// Same as above function, but this function also shifts computed points to giving 'origin'.

			GetWinding (points, size);

			const Vector3D center ((points[PointIndex_0] + points[PointIndex_1] + points[PointIndex_2] + points[PointIndex_3]) / PointIndex_Total);
			const Vector3D offset (origin - center);

			// Shift the points by direction offset....
			points[PointIndex_0] += offset;
			points[PointIndex_1] += offset;
			points[PointIndex_2] += offset;
			points[PointIndex_3] += offset;
		}
};

class PlaneExtended : public Plane
{
	public:
		DECLARE_TYPED_ENUMERATION (unsigned char, Type_t)
		{
			// 'Type_X', 'Type_Y' and 'Type_Z' are axial planes (Plane is perpendicular to given axis)
			Type_X,		// Axial plane, in X. (normal == [±1.0,  0.0,  0.0])
			Type_Y,		// Axial plane, in Y. (normal == [ 0.0, ±1.0,  0.0])
			Type_Z,		// Axial plane, in Z. (normal == [ 0.0,  0.0, ±1.0])

			// 'Type_AnyX', 'Type_AnyY' and 'Type_AnyZ' are non-axial planes snapped to the nearest (Dominant axis (axis along which projection of normal has greatest magnitude))
			Type_AnyX,	// Non axial plane, roughly toward X.
			Type_AnyY,	// Non axial plane, roughly toward Y.
			Type_AnyZ	// Non axial plane, roughly toward Z.
		};

	public:
		Type_t                   type;		// Type of plane, depending on normal vector. (for texture axis selection and fast side tests)
		Vector3D::SignBitsType_t signBits;	// signX + (signY << 1) + (signZ << 1)
};

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_PLANE_INCLUDED