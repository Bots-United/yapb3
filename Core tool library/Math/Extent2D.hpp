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
// Extent2D.hpp
//
// Class: Extent2D
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef CORE_TOOL_LIBRARY_EXTENT_2D_INCLUDED
#define CORE_TOOL_LIBRARY_EXTENT_2D_INCLUDED

class Extent2D
{
	public:
		Vector2D mins, maxs;

	public:
		inline Extent2D (const float value = 0.0f)                             : mins (value), maxs (value)                 { /*VOID*/ }
		inline Extent2D (const Vector2D &minsMaxs)                             : mins (minsMaxs), maxs (minsMaxs)           { /*VOID*/ }
		inline Extent2D (const Vector2D &inputMins, const Vector2D &inputMaxs) : mins (inputMins), maxs (inputMaxs)         { /*VOID*/ }
		inline Extent2D (const Extent3D &extent3D)                             : mins (extent3D.mins), maxs (extent3D.maxs) { /*VOID*/ }

	public:
		inline void Initialize (void)
		{
			mins = maxs = Vector2D::ZeroValue;
		}

		inline const float    GetSizeX         (void) const { return maxs.x - mins.x; }
		inline const float    GetInvertedSizeX (void) const { return 1.0f / GetSizeX (); }
		inline const float    GetMidX          (void) const { return GetMidPoint (maxs.x, mins.x); }
		inline const float    GetSizeY         (void) const { return maxs.y - mins.y; }
		inline const float    GetInvertedSizeY (void) const { return 1.0f / GetSizeY (); }
		inline const float    GetMidY          (void) const { return GetMidPoint (maxs.y, mins.y); }
		inline const float    GetArea2D        (void) const { return GetSizeX () * GetSizeY (); }
		inline const Vector2D GetCenter        (void) const { return mins.GetMidPoint (maxs); }
		inline const Vector2D GetExtent        (void) const { return mins.GetExtent (maxs); }
		inline const float    GetRadiusSquared (void) const
		{
			Vector2D corner;

			for (unsigned char axisIndex (Vector2D::AxisID_X); axisIndex < Vector2D::AxisID_Total; ++axisIndex)
				corner[axisIndex] = GetMaximumValueBetween (fabsf (mins[axisIndex]), fabsf (maxs[axisIndex]));

			return corner.GetLengthSquared ();
		}
		inline const float    GetRadius        (void) const { return sqrtf (GetRadiusSquared ()); }

		// Increase bounds to contain the given point
		inline void Encompass (const Vector2D &pos)
		{
			for (unsigned char index (Vector2D::AxisID_X); index < Vector2D::AxisID_Total; ++index)
			{
				if (mins[index] > pos[index])
					mins[index] = pos[index];
				else if (maxs[index] < pos[index])
					maxs[index] = pos[index];
			}
		}

		inline Extent2D &MoveBounds (const Vector2D &start, const Vector2D &end)
		{
			for (unsigned char axisIndex (Vector2D::AxisID_X); axisIndex < Vector2D::AxisID_Total; ++axisIndex)
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
		inline const Extent2D GetWithMovedBounds (const Vector2D &start, const Vector2D &end) const
		{
			Extent2D extent (*this);

			return extent.MoveBounds (start, end);
		}

		inline void Shift (const Vector2D &offset)
		{
			mins += offset;
			maxs += offset;
		}

		/// return true if 'pos' is inside of this extent
		inline const bool Contains (const Vector2D &pos) const
		{
			return pos.IsInsideBoundingBox (*this);
		}
		inline const bool Contains (const Vector2D &pos, const float tolerance) const
		{
			return pos.IsInsideBoundingBox (*this, tolerance);
		}

		inline const bool IsIntersects (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs) const
		{
			return mins < absoluteMaxs && maxs > absoluteMins;
		}
		inline const bool IsIntersects (const Extent2D &otherExtent) const
		{
			return IsIntersects (otherExtent.mins, otherExtent.maxs);
		}
		inline const bool IsIntersects (const Extent3D &otherExtent) const
		{
			return IsIntersects (otherExtent.mins, otherExtent.maxs);
		}

		inline const bool IsTouching (const Vector2D &absoluteMins, const Vector2D &absoluteMaxs) const
		{
			return mins < absoluteMins && maxs > absoluteMaxs;
		}
		inline const bool IsTouching (const Extent2D &otherExtent) const
		{
			return IsTouching (otherExtent.mins, otherExtent.maxs);
		}
		inline const bool IsTouching (const Extent3D &otherExtent) const
		{
			return IsTouching (otherExtent.mins, otherExtent.maxs);
		}

	public:
		inline Extent2D &operator = (const Vector2D &minsMaxs)
		{
			mins = maxs = minsMaxs;

			return *this;
		}
};

inline Extent2D Extent3D::AsExtent2D (void) const { return Extent2D (*this); }

inline const bool Vector2D::IsInsideBoundingBox (const Extent2D &extent) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs);
}
inline const bool Vector2D::IsInsideBoundingBox (const Extent2D &extent, const float tolerance) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs, tolerance);
}
inline const bool Vector3D::IsInsideBoundingBox (const Extent2D &extent) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs);
}
inline const bool Vector3D::IsInsideBoundingBox (const Extent2D &extent, const float tolerance) const
{
	return IsInsideBoundingBox (extent.mins, extent.maxs, tolerance);
}

#endif	// ifndef CORE_TOOL_LIBRARY_EXTENT_2D_INCLUDED