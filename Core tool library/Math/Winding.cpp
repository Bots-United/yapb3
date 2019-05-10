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

#include <Core.hpp>

namespace CoreToolLibrary
{
namespace Math
{
#define NORMAL_EPSILON   0.00001f

//
// Winding Public Methods
//

void Winding::Check (void) const
{
	unsigned int i, j;
	Vector3D p1;
	float d, edgedist;
	Vector3D dir, edgenormal;
	Plane facePlane;
	float area;

	if (m_pointsNumber < 3)
		AddLogEntry (false, LogLevel_Critical, true, "Winding::Check : %i points.", m_pointsNumber);

	area = GetArea ();
	if (area < 1.0f)
		AddLogEntry (false, LogLevel_Critical, true, "Winding::Check : %f front area.", area);

	GetPlane (facePlane);

	for (i = 0; i < m_pointsNumber; ++i)
	{
		p1 = m_points[i];

		for (j = Vector3D::AxisID_X; j < Vector3D::AxisID_Total; ++j)
			if (p1[j] > BOGUS_RANGE || p1[j] < -BOGUS_RANGE)
				AddLogEntry (false, LogLevel_Critical, true, "Winding::Check : BOGUS_RANGE: %f front.", p1[j]);

		j = i + 1 == m_pointsNumber ? 0 : i + 1;

		// check the point is on the face plane
		d = facePlane.GetDistanceToPoint (p1);
		if (d < -OnEpsilon || d > OnEpsilon)
			AddLogEntry (false, LogLevel_Critical, true, "Winding::Check : point off plane.");

		// check the edge isn't degenerate
		dir = m_points[j] - p1;

		if (dir.GetLength () < OnEpsilon)
			AddLogEntry (false, LogLevel_Critical, true, "Winding::Check : degenerate edge.");

		edgenormal = facePlane.normal.GetUnitCrossProduct (dir);
		edgedist = p1 | edgenormal;
		edgedist += OnEpsilon;

		// all other points must be on front side
		for (j = 0; j < m_pointsNumber; ++j)
		{
			if (j == i)
				continue;

			d = m_points[j] | edgenormal;
			if (d > edgedist)
				AddLogEntry (false, LogLevel_Critical, true, "Winding::Check : non-convex.");
		}
	}
}
/*
//
// Construction
//
Winding::Winding (const HalfLifeEngine::SDK::Structures::DFace_t& face)
{
	int se;
	dvertex_t *dv;
	int v;

	m_pointsNumber = face.numedges;
	m_points = new Vector3D[m_pointsNumber];

	for (unsigned int i = 0; i < face.numedges; ++i)
	{
		se = g_dsurfedges[face.firstedge + i];
		if (se < 0)
			v = g_dedges[-se].v[1];
		else
			v = g_dedges[se].v[0];

		dv = &g_dvertexes[v];
		m_points[i] = dv->point;
	}

	RemoveColinearPoints ();
}
*/
//
// Specialized Functions
//

void Winding::RemoveColinearPoints (void)
{
	unsigned int i;
	unsigned int nump;
	unsigned int j;
	Vector3D v1, v2;
	Vector3D p[MAX_POINTS_ON_WINDING];

	// JK: Did the optimizations...

	if (m_pointsNumber > 1u)
		v2 = (m_points[0u] - m_points[m_pointsNumber - 1u]).Normalize ();

	nump = 0;
	for (i = 0; i < m_pointsNumber; ++i)
	{
		j = (i + 1u) % m_pointsNumber;	// i + 1

		v1 = (m_points[i] - m_points[j]).Normalize ();

		v2 += v1;

		if (!v2.IsZero ())
		{
			p[nump] = m_points[i];
			++nump;
		}
#if 0
		else
			AddLogEntry (true, LogLevel_Default, false, "v2 was (%4.3f %4.3f %4.3f).", v2.x, v2.y, v2.z);
#endif
		// Set v2 for next round
		v2 = -v1;
	}

	if (nump == m_pointsNumber)
		return;

#if 0
	AddLogEntry (true, LogLevel_Warning, false, "RemoveColinearPoints: Removed %u points, from %u to %u.", m_pointsNumber - nump, m_pointsNumber, nump);
	AddLogEntry (true, LogLevel_Warning, false, "Before :");
	Print ();
#endif
	m_pointsNumber = nump;
	MemoryCopy (p, m_points, nump);

#if 0
	AddLogEntry (true, LogLevel_Warning, false, "After :");
	Print ();

	AddLogEntry (true, LogLevel_Warning, false, "==========");
#endif
}

void Winding::Clip (const Plane &split, Winding *&front, Winding *&back, const float onPlaneEpsilon/* = OnEpsilon*/)
{
	float dists[MAX_POINTS_ON_WINDING + 4];
	unsigned int sides[MAX_POINTS_ON_WINDING + 4];
	unsigned int counts[3];
	float dot;
	unsigned int i;
	unsigned char j;
	unsigned int maxpts;

	counts[0] = counts[1] = counts[2] = 0;

	// determine sides for each point
	for (i = 0; i < m_pointsNumber; ++i)
	{
		dot = split.GetDistanceToPoint (m_points[i]);
		dists[i] = dot;
		if (dot > onPlaneEpsilon)
			sides[i] = SIDE_FRONT;
		else if (dot < -onPlaneEpsilon)
			sides[i] = SIDE_BACK;
		else
			sides[i] = SIDE_ON;

		++counts[sides[i]];
	}
	sides[i] = sides[0];
	dists[i] = dists[0];

	if (!counts[0])
	{
		front = NULL;
		back = new Winding (*this);
		return;
	}
	if (!counts[1])
	{
		// Copy to output verts
		front = new Winding (*this);
		back = NULL;
		return;
	}

	maxpts = m_pointsNumber + 4u;	// can't use counts[0]+2 because
	// of fp grouping errors

	front = new Winding (maxpts);
	back = new Winding (maxpts);

	front->m_pointsNumber = 0;
	back->m_pointsNumber = 0;

	for (i = 0; i < m_pointsNumber; ++i)
	{
		const Vector3D &p1 (m_points[i]);

		if (sides[i] == SIDE_ON)
		{
			front->m_points[front->m_pointsNumber] = p1;
			back->m_points[back->m_pointsNumber] = p1;
			++front->m_pointsNumber;
			++back->m_pointsNumber;
			continue;
		}
		else if (sides[i] == SIDE_FRONT)
		{
			front->m_points[front->m_pointsNumber] = p1;
			++front->m_pointsNumber;
		}
		else if (sides[i] == SIDE_BACK)
		{
			back->m_points[back->m_pointsNumber] = p1;
			++back->m_pointsNumber;
		}

		if ((sides[i + 1] == SIDE_ON) | (sides[i + 1] == sides[i]))	// | instead of || for branch optimization
			continue;

		// generate a split point
		Vector3D mid;
		const Vector3D &p2 (m_points[(i + 1u) % m_pointsNumber]);
		dot = dists[i] / (dists[i] - dists[i + 1]);
#if 0
		for (j = Vector3D::AxisID_X; j < Vector3D::AxisID_Total; ++j)
		{	// avoid round off error when possible
			if (split.normal[j] < 1.0f - NORMAL_EPSILON)
			{
				if (split.normal[j] > -1.0f + NORMAL_EPSILON)
					mid[j] = p1[j] + dot * (p2[j] - p1[j]);
				else
					mid[j] = -split.distance;
			}
			else
				mid[j] = split.distance;
		}
#else
		for (j = Vector3D::AxisID_X; j < Vector3D::AxisID_Total; ++j)
		{	// avoid round off error when possible
			if (split.normal[j] == 1)
				mid[j] = split.distance;
			else if (split.normal[j] == -1)
				mid[j] = -split.distance;
			else
				mid[j] = p1[j] + dot * (p2[j] - p1[j]);
		}
#endif
		front->m_points[front->m_pointsNumber] = mid;
		back->m_points[back->m_pointsNumber] = mid;
		++front->m_pointsNumber;
		++back->m_pointsNumber;
	}

	if ((front->m_pointsNumber > maxpts) | (back->m_pointsNumber > maxpts))	// | instead of || for branch optimization
		AddLogEntry (false, LogLevel_Critical, true, "Winding::Clip : points exceeded estimate.");

	if ((front->m_pointsNumber > MAX_POINTS_ON_WINDING) | (back->m_pointsNumber > MAX_POINTS_ON_WINDING))	// | instead of || for branch optimization
		AddLogEntry (false, LogLevel_Critical, true, "Winding::Clip : MAX_POINTS_ON_WINDING.");

	front->RemoveColinearPoints ();
	back->RemoveColinearPoints ();
}

const int Winding::WindingOnPlaneSide (const Plane &plane)
{
	bool front, back;
	unsigned int i;
	float d;

	front = false;
	back = false;
	for (i = 0; i < m_pointsNumber; ++i)
	{
		d = plane.GetDistanceToPoint (m_points[i]);
		if (d < -OnEpsilon)
		{
			if (front)
				return SIDE_CROSS;

			back = true;
			continue;
		}
		if (d > OnEpsilon)
		{
			if (back)
				return SIDE_CROSS;

			front = true;
			continue;
		}
	}

	if (back)
		return SIDE_BACK;

	if (front)
		return SIDE_FRONT;

	return SIDE_ON;
}


const bool Winding::Clip (const Plane &split, const bool keepon)
{
	float dists[MAX_POINTS_ON_WINDING];
	unsigned int sides[MAX_POINTS_ON_WINDING];
	unsigned int counts[3];
	float dot;
	unsigned int i;
	unsigned char j;

	counts[0] = counts[1] = counts[2] = 0;

	// determine sides for each point
	// do this exactly, with no epsilon so tiny portals still work
	for (i = 0; i < m_pointsNumber; ++i)
	{
		dot = split.GetDistanceToPoint (m_points[i]);
		dists[i] = dot;
		if (dot > OnEpsilon)
			sides[i] = SIDE_FRONT;
		else if (dot < OnEpsilon)
			sides[i] = SIDE_BACK;
		else
			sides[i] = SIDE_ON;

		++counts[sides[i]];
	}
	sides[i] = sides[0];
	dists[i] = dists[0];

	if (keepon && !counts[0] && !counts[1])
		return true;

	if (!counts[0])
	{
		delete [] m_points;
		m_points = NULL;
		m_pointsNumber = 0;
		return false;
	}

	if (!counts[1])
		return true;

	unsigned maxpts = m_pointsNumber + 4;	// can't use counts[0]+2 because of fp grouping errors
	unsigned newNumPoints = 0;
	Vector3D *const newPoints (new Vector3D[maxpts]);

	for (i = 0; i < m_pointsNumber; ++i)
	{
		const Vector3D &p1 (m_points[i]);

		if (sides[i] == SIDE_ON)
		{
			newPoints[newNumPoints] = p1;
			++newNumPoints;
			continue;
		}
		else if (sides[i] == SIDE_FRONT)
		{
			newPoints[newNumPoints] = p1;
			++newNumPoints;
		}

		if (sides[i + 1] == SIDE_ON || sides[i + 1] == sides[i])
			continue;

		// generate a split point
		Vector3D mid;
		unsigned int tmp = i + 1;
		if (tmp >= m_pointsNumber)
			tmp = 0;

		const Vector3D &p2 (m_points[tmp]);
		dot = dists[i] / (dists[i] - dists[i + 1]);
		for (j = Vector3D::AxisID_X; j < Vector3D::AxisID_Total; ++j)
		{	// avoid round off error when possible
			if (split.normal[j] < 1.0f - NORMAL_EPSILON)
			{
				if (split.normal[j] > -1.0f + NORMAL_EPSILON)
					mid[j] = p1[j] + dot * (p2[j] - p1[j]);
				else
					mid[j] = -split.distance;
			}
			else
				mid[j] = split.distance;
		}

		newPoints[newNumPoints] = mid;
		++newNumPoints;
	}

	if (newNumPoints > maxpts)
		AddLogEntry (false, LogLevel_Critical, true, "Winding::Clip : points exceeded estimate.");

	delete [] m_points;
	m_points = newPoints;
	m_pointsNumber = newNumPoints;

	RemoveColinearPoints ();

	return true;
}

/*
* ==================
* Divide
* Divides a winding by a plane, producing one or two windings. The
* original winding is not damaged or freed. If only on one side, the
* returned winding will be the input winding. If on both sides, two
* new windings will be created.
* ==================
*/
void Winding::Divide (const Plane &split, Winding *&front, Winding *&back)
{
	float dists[MAX_POINTS_ON_WINDING];
	unsigned int sides[MAX_POINTS_ON_WINDING];
	unsigned int counts[3];
	float dot;
	unsigned int i;
	unsigned char j;
	unsigned int maxpts;

	counts[0] = counts[1] = counts[2] = 0;

	// determine sides for each point
	for (i = 0; i < m_pointsNumber; ++i)
	{
		dot = split.GetDistanceToPoint (m_points[i]);
		dists[i] = dot;
		if (dot > OnEpsilon)
			sides[i] = SIDE_FRONT;
		else if (dot < -OnEpsilon)
			sides[i] = SIDE_BACK;
		else
			sides[i] = SIDE_ON;

		++counts[sides[i]];
	}
	sides[i] = sides[0];
	dists[i] = dists[0];

	front = back = NULL;

	if (counts[0] == 0)
	{
		back = this;	// Makes this function non-const

		return;
	}

	if (counts[1] == 0)
	{
		front = this;	// Makes this function non-const

		return;
	}

	maxpts = m_pointsNumber + 4u;	// can't use counts[0]+2 because
	// of fp grouping errors

	front = new Winding (maxpts);
	back = new Winding (maxpts);

	front->m_pointsNumber = 0;
	back->m_pointsNumber = 0;

	for (i = 0; i < m_pointsNumber; ++i)
	{
		const Vector3D &p1 (m_points[i]);

		if (sides[i] == SIDE_ON)
		{
			front->m_points[front->m_pointsNumber] = p1;
			back->m_points[back->m_pointsNumber] = p1;
			++front->m_pointsNumber;
			++back->m_pointsNumber;
			continue;
		}
		else if (sides[i] == SIDE_FRONT)
		{
			front->m_points[front->m_pointsNumber] = p1;
			++front->m_pointsNumber;
		}
		else if (sides[i] == SIDE_BACK)
		{
			back->m_points[back->m_pointsNumber] = p1;
			++back->m_pointsNumber;
		}

		if (sides[i + 1] == SIDE_ON || sides[i + 1] == sides[i])
			continue;

		// generate a split point
		Vector3D mid;
		unsigned int tmp = i + 1;
		if (tmp >= m_pointsNumber)
			tmp = 0;

		const Vector3D &p2 (m_points[tmp]);
		dot = dists[i] / (dists[i] - dists[i + 1]);
		for (j = Vector3D::AxisID_X; j < Vector3D::AxisID_Total; ++j)
		{	// avoid round off error when possible
			if (split.normal[j] < 1.0f - NORMAL_EPSILON)
			{
				if (split.normal[j] > -1.0f + NORMAL_EPSILON)
					mid[j] = p1[j] + dot * (p2[j] - p1[j]);
				else
					mid[j] = -split.distance;
			}
			else
				mid[j] = split.distance;
		}

		front->m_points[front->m_pointsNumber] = mid;
		back->m_points[back->m_pointsNumber] = mid;
		++front->m_pointsNumber;
		++back->m_pointsNumber;
	}

	if (front->m_pointsNumber > maxpts || back->m_pointsNumber > maxpts)
		AddLogEntry (false, LogLevel_Critical, true, "Winding::Divide : points exceeded estimate.");

	front->RemoveColinearPoints ();
	back->RemoveColinearPoints ();
}
}
}