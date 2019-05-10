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
// BoundingBox.hpp
//
// Class: BoundingBox
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_BOUNDING_BOX_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_BOUNDING_BOX_INCLUDED

class BoundingBox
{
	public:
		enum State_t
		{
			eDisjoint,	// neither boxes touch
			eUnion,		// this box intersects with the other box
			eSubset,	// this box is inside the other box
			eSuperset	// this box is completly envelops the other box
		};


	public:
		// Bounding box
		Vector3D m_mins;
		Vector3D m_maxs;

	public:
		inline BoundingBox (void)
		{
			Reset ();
		}
		inline BoundingBox (const Vector3D &mins, const Vector3D &maxs) :
			m_mins (mins),
			m_maxs (maxs)
		{ /* VOID */ }

	public:
		// Tests if other box is completely outside of this box
		inline const bool TestDisjoint (const BoundingBox &other) const
		{
			return (m_mins <= other.m_maxs && m_maxs >= other.m_mins) == false;
		}
		// returns true if this box is completely inside other box
		inline const bool TestSubset (const BoundingBox &other) const
		{
			return m_mins >= other.m_mins && m_maxs <= other.m_maxs;
		}
		// returns true if this box contains the other box completely
		inline const bool TestSuperset (const BoundingBox &other) const
		{
			return other.TestSubset (*this);
		}
		// returns true if this box partially intersects the other box
		inline const bool TestUnion (const BoundingBox &other) const
		{
			const BoundingBox tempBox (m_mins.GetMaximum (other.m_mins), m_maxs.GetMinimum (other.m_maxs));

			return tempBox.m_mins.x <= tempBox.m_maxs.x;
		}
		inline const State_t Test (const BoundingBox &other) const
		{
			if (TestDisjoint (other))
				return eDisjoint;

			if (TestSubset (other))
				return eSubset;

			if (TestSuperset (other))
				return eSuperset;

			return eUnion;
		}

		inline void Set (const Vector3D &mins, const Vector3D &maxs)
		{
			m_mins = mins;
			m_maxs = maxs;
		}
		inline void Reset (void)
		{
			m_mins =  999999999.999f;
			m_maxs = -999999999.999f;
		}
		inline void Add (const Vector3D &point)
		{
			m_mins = m_mins.GetMinimum (point);
			m_maxs = m_maxs.GetMaximum (point);
		}
		inline void Add (const BoundingBox &other)
		{
			Add (other.m_mins);
			Add (other.m_maxs);
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_BOUNDING_BOX_INCLUDED