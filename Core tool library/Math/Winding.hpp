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
// Winding.hpp
//
// Class: Winding
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_WINDING_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_WINDING_INCLUDED

#define MAX_POINTS_ON_WINDING 128
// TODO: FIX THIS STUPID SHIT (MAX_POINTS_ON_WINDING)

#define	SIDE_FRONT		0
#define	SIDE_ON			2
#define	SIDE_BACK		1
#define	SIDE_CROSS		-2

class Winding
{
	public:
		// Data
		unsigned int  m_pointsNumber;
		Vector3D     *m_points;

	private:
		unsigned int  m_maximumPoints;

	public:
		// Construction
		inline Winding (void) :	// Do nothing :)
			m_points (NULL),
			m_pointsNumber (0u),
			m_maximumPoints (0u)
		{ /* VOID */ }
		inline Winding (const Vector3D *const points, const unsigned int numpoints)	// Create from raw points
		{
			InternalAssert (numpoints >= 3u);

			m_pointsNumber = numpoints;
			m_maximumPoints = (m_pointsNumber + 3u) & ~3;	// groups of 4

			m_points = new Vector3D[m_maximumPoints];
			MemoryCopy (points, m_points, m_pointsNumber);
		}
//		Winding (const HalfLifeEngine::SDK::Structures::DFace_t &face);
		inline Winding (const Plane &plane, const float size = BOGUS_RANGE) { InitializeFromPlane (plane, size); }
		inline Winding (unsigned int pointsNumber)
		{
			InternalAssert (pointsNumber >= 3u);

			m_pointsNumber = pointsNumber;
			m_maximumPoints = (m_pointsNumber + 3u) & ~3;	// groups of 4

			m_points = new Vector3D[m_maximumPoints];
		}
		inline Winding (const Winding &other)
		{
			m_pointsNumber = other.m_pointsNumber;
			m_maximumPoints = (m_pointsNumber + 3u) & ~3;	// groups of 4

			m_points = new Vector3D[m_maximumPoints];
			MemoryCopy (other.m_points, m_points, m_pointsNumber);
		}
		inline ~Winding (void) { delete [] m_points; }
		inline Winding &operator = (const Winding &other)
		{
			delete [] m_points;

			m_pointsNumber = other.m_pointsNumber;
			m_maximumPoints = (m_pointsNumber + 3u) & ~3;	// groups of 4

			m_points = new Vector3D[m_maximumPoints];
			MemoryCopy (other.m_points, m_points, m_pointsNumber);

			return *this;
		}

	// Misc
	private:
		inline void InitializeFromPlane (const Plane &plane, const float size)
		{
			m_pointsNumber = m_maximumPoints = 4u;
			m_points = new Vector3D[m_pointsNumber];

			static Plane::Points_t points;

			plane.GetWinding (points, size);
			m_points[0] = points[0];
			m_points[1] = points[1];
			m_points[2] = points[2];
			m_points[3] = points[3];
//NOT WORK			plane.GetWinding (reinterpret_cast <Plane::Points_t &> (m_points), size);
		}

		inline void Resize (const unsigned int newsize)
		{
			m_maximumPoints = (newsize + 3u) & ~3;	// groups of 4

			Vector3D *const newpoints (new Vector3D[m_maximumPoints]);

			m_pointsNumber = GetMinimumValueBetween (m_pointsNumber, m_maximumPoints);
			MemoryCopy (m_points, newpoints, m_pointsNumber);
			delete [] m_points;
			m_points = newpoints;
		}

	// General Functions
	public:
/*		#if defined _DEBUG
			inline void Print (void) const
			{
				for (unsigned int x (0u); x < m_pointsNumber; ++x)
					Log (" (%5.2f, %5.2f, %5.2f)\n", m_points[x].x, m_points[x].y, m_points[x].z);
			}
		#endif	// if defined _DEBUG
*/
		inline void GetPlane (Plane &plane) const
		{
			Vector3D v1, v2;

//			InternalAssert (m_pointsNumber >= 3u);

			if (m_pointsNumber >= 3u)
				plane = m_points;
			else
			{
				plane.normal = Vector3D::ZeroValue;
				plane.distance = 0.0f;
			}
		}
		inline const float GetArea (void) const
		{
//			InternalAssert (m_pointsNumber >= 3u);

			float total (0.0f);

			if (m_pointsNumber >= 3u)
			{
				unsigned int i;
				Vector3D d1, d2, cross;

				for (i = 2u; i < m_pointsNumber; ++i)
				{
					d1 = m_points[i - 1] - m_points[0];
					d2 = m_points[i] - m_points[0];
					cross = d1 ^ d2;
					total += cross.GetLength () * 0.5f;
				}
			}

			return total;
		}
/*		inline void GetBounds (BoundingBox &bounds) const
		{
			bounds.Reset ();

			for (unsigned int x (0u); x < m_pointsNumber; ++x)
				bounds.Add (m_points[x]);
		}
		inline void GetBounds (Vector3D &mins, Vector3D &maxs) const
		{
			BoundingBox bounds;
			unsigned int x;

			for (x=0; x<m_pointsNumber; ++x)
				bounds.Add (m_points[x]);

			mins = bounds.m_mins;
			maxs = bounds.m_maxs;
		}*/
		inline void GetCenter (Vector3D &center) const
		{
			if (m_pointsNumber > 0u)
			{
				center = m_points[0u];

				for (unsigned int i (1u); i < m_pointsNumber; ++i)
					center += m_points[i];

				center /= static_cast <const float> (m_pointsNumber);
			}
			else
				center = Vector3D::ZeroValue;
		}
		void Check (void) const;	// Developer check for validity
		inline const bool Valid (void) const	// Runtime/user/normal check for validity
		{
			/// @todo Check to ensure there are 3 non-colinear points
			return m_pointsNumber >= 3u && m_points != NULL;
		}
		inline void AddPoint (const Vector3D &newpoint)
		{
			if (m_pointsNumber >= m_maximumPoints)
				Resize (m_pointsNumber + 1u);

			m_points[m_pointsNumber] = newpoint;

			++m_pointsNumber;
		}
		inline void InsertPoint (const Vector3D newpoint, const unsigned int offset)
		{
			if (offset >= m_pointsNumber)
				AddPoint (newpoint);
			else
			{
				if (m_pointsNumber >= m_maximumPoints)
					Resize (m_pointsNumber + 1u);

				unsigned int x;

				for (x = m_pointsNumber; x > offset; --x)
					m_points[x] = m_points[x - 1u];

				m_points[x] = newpoint;

				++m_pointsNumber;
			}
		}

		// Specialized Functions
		void RemoveColinearPoints (void);
		const bool Clip (const Plane &split, const bool keepon);	// For hlbsp
		void Clip (const Plane &split, Winding *&front, Winding *&back, const float onPlaneEpsilon = OnEpsilon);
		const bool Chop (const Plane &split)
		{
			Winding *front, *back;

			Clip (split, front, back);

			if (back != NULL)
				delete back;

			if (front != NULL)
			{
				delete [] m_points;
				m_pointsNumber = front->m_pointsNumber;
				m_points = front->m_points;
				front->m_points = NULL;
				delete front;

				return true;
			}
			else
			{
				m_pointsNumber = 0u;
				delete [] m_points;
				m_points = NULL;

				return false;
			}
		}

		void Divide (const Plane &split, Winding *&front, Winding *&back);
		const int WindingOnPlaneSide (const Plane &plane);
		inline void CopyPoints (Vector3D *const points, unsigned int &numpoints)
		{
			if (points == NULL)
			{
				numpoints = 0u;

				return;
			}

			MemoryCopy (m_points, points, m_pointsNumber);

			numpoints = m_pointsNumber;
		}

		inline void InitializeFromPoints (const Vector3D *const points, const unsigned int numpoints)
		{
			InternalAssert (numpoints >= 3u);

			Reset ();

			m_pointsNumber = numpoints;
			m_maximumPoints = (m_pointsNumber + 3u) & ~3;	// groups of 4

			m_points = new Vector3D[m_maximumPoints];
			MemoryCopy (points, m_points, m_pointsNumber);
		}
		inline void Reset (void)	// Resets the structure
		{
			if (m_points != NULL)
			{
				delete [] m_points;

				m_points = NULL;
			}

			m_pointsNumber = m_maximumPoints = 0u;
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_WINDING_INCLUDED