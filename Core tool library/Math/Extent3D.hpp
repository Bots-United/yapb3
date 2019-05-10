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
// Extent3D.hpp
//
// Class: Extent3D
//
// Description: Axis aligned bounding box in 3D dimensional space.
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef CORE_TOOL_LIBRARY_EXTENT_3D_INCLUDED
#define CORE_TOOL_LIBRARY_EXTENT_3D_INCLUDED

class Extent3D
{
	public:
		Vector3D mins, maxs;

	public:
		inline Extent3D (const float value = 0.0f)                             : mins (value), maxs (value)         { /* VOID */ }
		inline Extent3D (const Vector3D &minsMaxs)                             : mins (minsMaxs), maxs (minsMaxs)   { /* VOID */ }
		inline Extent3D (const Vector3D &inputMins, const Vector3D &inputMaxs) : mins (inputMins), maxs (inputMaxs) { /* VOID */ }

	public:
		inline void Initialize (void) { mins = maxs = Vector3D::ZeroValue; }

		// Cast to Extent2D....
		inline Extent2D       AsExtent2D       (void) const;

		inline const float    GetSizeX         (void) const { return maxs.x - mins.x; }
		inline const float    GetInvertedSizeX (void) const { return 1.0f / GetSizeX (); }
		inline const float    GetMidX          (void) const { return GetMidPoint (maxs.x, mins.x); }
		inline const float    GetSizeY         (void) const { return maxs.y - mins.y; }
		inline const float    GetInvertedSizeY (void) const { return 1.0f / GetSizeY (); }
		inline const float    GetMidY          (void) const { return GetMidPoint (maxs.y, mins.y); }
		inline const float    GetSizeZ         (void) const { return maxs.z - mins.z; }
		inline const float    GetInvertedSizeZ (void) const { return 1.0f / GetSizeZ (); }
		inline const float    GetMidZ          (void) const { return GetMidPoint (maxs.z, mins.z); }
		inline const float    GetArea2D        (void) const { return GetSizeX () * GetSizeY (); }
		inline const float    GetArea3D        (void) const { return GetArea2D () * GetSizeZ (); }
		inline const Vector3D GetCenter        (void) const { return mins.GetMidPoint (maxs); }
		inline const Vector3D GetExtent        (void) const { return mins.GetExtent (maxs); }
		inline const float    GetRadiusSquared (void) const
		{
			Vector3D corner;

			for (unsigned char axisIndex (Vector3D::AxisID_X); axisIndex < Vector3D::AxisID_Total; ++axisIndex)
				corner[axisIndex] = GetMaximumValueBetween (fabsf (mins[axisIndex]), fabsf (maxs[axisIndex]));

			return corner.GetLengthSquared ();
		}
		inline const float    GetRadius        (void) const { return sqrtf (GetRadiusSquared ()); }

		//! Stores all 8 edges of the box into an array
		/** \param edges: Pointer to array of 8 edges. */
		inline void GetVertexes (Vector3D edges[8u]) const
		{
			const Vector3D middle (GetCenter ());
			const Vector3D diag (middle - maxs);

			/*! @note Edges are stored in this way:
                  /3--------/7
                 /  |      / |
                /   |     /  |
                1---------5  |
                |   2- - -| -6
                |  /      |  /
                |/        | /
                0---------4/
			*/

			edges[0u] = Vector3D (middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[1u] = Vector3D (middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[2u] = Vector3D (middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[3u] = Vector3D (middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
			edges[4u] = Vector3D (middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[5u] = Vector3D (middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[6u] = Vector3D (middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[7u] = Vector3D (middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
		}

		//! Check if the box is empty.
		/** This means that there is no space between the min and max
		edge.
		\return True if box is empty, else false. */
		inline const bool IsEmpty (void) const { return mins.AreEqual (maxs); }

		// Increase bounds to contain the given point
		inline void Encompass (const Vector3D &pos)
		{
			for (unsigned char axisIndex (Vector3D::AxisID_X); axisIndex < Vector3D::AxisID_Total; ++axisIndex)
			{
				if (mins[axisIndex] > pos[axisIndex])
					mins[axisIndex] = pos[axisIndex];
				else if (maxs[axisIndex] < pos[axisIndex])
					maxs[axisIndex] = pos[axisIndex];
			}
		}

		//! Adds a point to the bounding box
		/** The box grows bigger, if point was outside of the box.
		\param point: Point to add into the box. */
		inline void AddInternalPoint (const Vector3D &point) { Encompass (point); }

		//! Adds another bounding box
		/** The box grows bigger, if the new box was outside of the box.
		\param extent: Other bounding box to add into this box. */
		inline void AddInternalExtent (const Extent3D &extent)
		{
			AddInternalPoint (extent.mins);
			AddInternalPoint (extent.maxs);
		}

		inline Extent3D &MoveBounds (const Vector3D &start, const Vector3D &end)
		{
			for (unsigned char axisIndex (Vector3D::AxisID_X); axisIndex < Vector3D::AxisID_Total; ++axisIndex)
			{
				if (end[axisIndex] > start[axisIndex])
				{
					mins[axisIndex] = start[axisIndex] + mins[axisIndex] - 1.0f;
					maxs[axisIndex] = end[axisIndex] + maxs[axisIndex] + 1.0f;
				}
				else
				{
					mins[axisIndex] = end[axisIndex] + mins[axisIndex] - 1.0f;
					maxs[axisIndex] = start[axisIndex] + maxs[axisIndex] + 1.0f;
				}
			}

			return *this;
		}
		inline const Extent3D GetWithMovedBounds (const Vector3D &start, const Vector3D &end) const
		{
			Extent3D extent (*this);

			return extent.MoveBounds (start, end);
		}

		inline void Shift (const Vector2D &offset)
		{
			mins += offset;
			maxs += offset;
		}
		inline void Shift (const Vector3D &offset)
		{
			mins += offset;
			maxs += offset;
		}

		/// return true if 'pos' is inside of this extent
		inline const bool Contains (const Vector2D &pos) const
		{
			return pos.IsInsideBoundingBox (*this);
		}
		inline const bool Contains (const Vector3D &pos) const
		{
			return pos.IsInsideBoundingBox (*this);
		}
		inline const bool Contains (const Vector2D &pos, const float tolerance) const
		{
			return pos.IsInsideBoundingBox (*this, tolerance);
		}
		inline const bool Contains (const Vector3D &pos, const float tolerance) const
		{
			return pos.IsInsideBoundingBox (*this, tolerance);
		}

		inline const bool IsIntersects2D (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs) const
		{
			return mins < absoluteMaxs && maxs > absoluteMins;
		}
		inline const bool IsIntersects2D (const Extent3D &otherExtent) const
		{
			return IsIntersects2D (otherExtent.mins, otherExtent.maxs);
		}
		inline const bool IsIntersects (const Vector3D &absoluteMins, const Vector3D &absoluteMaxs) const
		{
			return mins < absoluteMaxs && maxs > absoluteMins;
		}
		inline const bool IsIntersects (const Extent3D &otherExtent) const
		{
			return IsIntersects (otherExtent.mins, otherExtent.maxs);
		}

		inline const bool IsTouching2D (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs) const
		{
			return maxs >= absoluteMins && mins <= absoluteMaxs;
		}
		inline const bool IsTouching2D (const Extent3D &otherExtent) const
		{
			return IsTouching2D (otherExtent.mins, otherExtent.maxs);
		}
		inline const bool IsTouching (const Vector3D &absoluteMins, const Vector3D &absoluteMaxs) const
		{
			return maxs >= absoluteMins && mins <= absoluteMaxs;
		}
		inline const bool IsTouching (const Extent3D &otherExtent) const
		{
			return IsTouching (otherExtent.mins, otherExtent.maxs);
		}
};

inline const bool Vector2D::IsInsideBoundingBox (const Extent3D &extent) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs);
}
inline const bool Vector2D::IsInsideBoundingBox (const Extent3D &extent, const float tolerance) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs, tolerance);
}

inline const bool Vector3D::IsInsideBoundingBox (const Extent3D &extent) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs);
}
inline const bool Vector3D::IsInsideBoundingBox (const Extent3D &extent, const float tolerance) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs, tolerance);
}

#endif	// ifndef CORE_TOOL_LIBRARY_EXTENT_3D_INCLUDED