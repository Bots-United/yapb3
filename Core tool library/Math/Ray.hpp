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
// Ray.hpp
//
// Class: Ray
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_RAY_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_RAY_INCLUDED

//-----------------------------------------------------------------------------
// A ray...
//-----------------------------------------------------------------------------

inline const float InvRSquared (const Vector3D &vector)
{
	const float r2 (vector | vector);

	return r2 < 1.0f ? 1.0f : 1.0f / r2;
}

inline const float IntersectRayWithAAPlane (const Vector3D &source, const Vector3D &destination, const Vector3D::AxisID_t axisIndex, const float sign, const float distance)
{
	const float denominator (sign * (destination[axisIndex] - source[axisIndex]));

	if (denominator == 0.0f)
		return 0.0f;

	return (distance - sign * source[axisIndex]) * (1.0f / denominator);
}

class Ray
{
	public:
		Vector3D m_start;		// starting point, centered within the extents
		Vector3D m_delta;		// direction + length of the ray
		Vector3D m_startOffset;	// Add this to m_start to get the actual ray start
		Vector3D m_extents;		// Describes an axis aligned box extruded along a ray
		bool     m_isRay;		// are the extents zero?
		bool     m_isSwept;		// is delta != 0.0?

	public:
		inline Ray (const Vector3D &start, const Vector3D &end)
		{
			Initialize (start, end);
		}
		inline Ray (const Vector3D &start, const Vector3D &end, const Vector3D &mins, const Vector3D &maxs)
		{
			Initialize (start, end, mins, maxs);
		}

	public:
		inline void Initialize (const Vector3D &start, const Vector3D &end)
		{
			m_delta = end - start;
			m_isSwept = m_delta != Vector3D::ZeroValue;
			m_extents = Vector3D::ZeroValue;
			m_isRay = true;

			// Offset 'm_start' to be in the center of the box...
			m_startOffset = Vector3D::ZeroValue;
			m_start = start;
		}

		inline void Initialize (const Vector3D &start, const Vector3D &end, const Vector3D &mins, const Vector3D &maxs)
		{
			m_delta = end - start;
			m_isSwept = m_delta != Vector3D::ZeroValue;
			m_extents = mins.GetExtent (maxs);
			m_isRay = m_extents.GetLengthSquared () < ZeroTolerance;

			// Offset 'm_start' to be in the center of the box...
			m_startOffset = mins.GetMidPoint (maxs);
			m_start = start + m_startOffset;

			m_startOffset.Invert ();
		}

		// compute inverse delta
		inline const Vector3D GetInvetedDelta (void) const
		{
			Vector3D invertedDelta;

			for (unsigned char axisIndex (Vector3D::AxisID_X); axisIndex < Vector3D::AxisID_Total; ++axisIndex)
				invertedDelta[axisIndex] = m_delta[axisIndex] != 0.0f ? 1.0f / m_delta[axisIndex] : MaximumFloatValue;

			return invertedDelta;
		}

		//-----------------------------------------------------------------------------
		// Compute the offset in t along the ray that we'll use for the collision
		//-----------------------------------------------------------------------------
		inline const float ComputeBoxOffset (void) const
		{
			if (m_isRay)
				return 1e-3f;

			// Find the projection of the box diagonal along the ray....
			// We need to divide twice: Once to normalize the computation above
			// so we get something in units of extents, and the second to normalize
			// that with respect to the entire raycast.
			return m_extents.GetAbsoluteDotProduct (m_delta) * InvRSquared (m_delta) + 1e-3f;	// 1e-3 is an epsilon
		}

		//-----------------------------------------------------------------------------
		// Intersects a swept box against a triangle
		//-----------------------------------------------------------------------------
		const float IntersectWithTriangle (const Vector3D &v1, const Vector3D &v2, const Vector3D &v3, const bool oneSided) const
		{
			// This is cute: Use barycentric coordinates to represent the triangle
			// Vo(1-u-v) + V1u + V2v and intersect that with a line Po + Dt
			// This gives us 3 equations + 3 unknowns, which we can solve with
			// Cramer's rule...
			//		E1x u + E2x v - Dx t = Pox - Vox
			// There's a couple of other optimizations, Cramer's rule involves
			// computing the determinant of a matrix which has been constructed
			// by three vectors. It turns out that
			// det | A B C | = -(A x C) dot B or -(C x B) dot A
			// which we'll use below..

			const Vector3D &edge1 (v2 - v1), &edge2 (v3 - v1);

			// Cull out one-sided stuff
			if (oneSided && ((edge1 ^ edge2) | m_delta) >= 0.0f)
				return -1.0f;

			// FIXME: This is inaccurate, but fast for boxes
			// We want to do a fast separating axis implementation here
			// with a swept triangle along the reverse direction of the ray.

			// Compute some intermediary terms
			const Vector3D &dirCrossEdge2 (m_delta ^ edge2);

			// Compute the denominator of Cramer's rule:
			//     | -Dx E1x E2x |
			// det | -Dy E1y E2y | = (D x E2) dot E1
			//     | -Dz E1z E2z |
			float denominator (dirCrossEdge2 | edge1);

			if (fabsf (denominator) < ZeroTolerance)
				return -1.0f;

			denominator = 1.0f / denominator;

			// Compute u. It's gotta lie in the range of 0 to 1.
			//                       | -Dx orgx E2x |
			// u = denominator * det | -Dy orgy E2y | = (D x E2) dot org
			//                       | -Dz orgz E2z |
			const Vector3D &org (m_start - v1);
			const float u ((dirCrossEdge2 | org) * denominator);

			if (u < 0.0f || u > 1.0f)
				return -1.0f;

			// Compute t and v the same way...
			// In barycentric coords, u + v < 1
			const Vector3D &orgCrossEdge1 (org ^ edge1);
			const float v ((orgCrossEdge1 | m_delta) * denominator);

			if (v < 0.0f || v + u > 1.0f)
				return -1.0f;

			// Compute the distance along the ray direction that we need to fudge when using swept boxes
			const float boxt (ComputeBoxOffset ());
			const float t ((orgCrossEdge1 | edge2) * denominator);

			if (t < -boxt || t > 1.0f + boxt)
				return -1.0f;

			return GetClampedValueInRange (t, 0.0f, 1.0f);
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_RAY_INCLUDED