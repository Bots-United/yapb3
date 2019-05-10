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
// Frustum.hpp
//
// Class: Frustum
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_FRUSTUM_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_FRUSTUM_INCLUDED

//-----------------------------------------------------------------------------
// Purpose: Renderer setup data.  
//-----------------------------------------------------------------------------
class CViewSetup
{
public:
	inline CViewSetup (void)
	{
		m_flAspectRatio = 0.0f;
		m_bRenderToSubrectOfLargerScreen = false;
		m_bDoBloomAndToneMapping = true;
		m_bOffCenter = false;
		m_bCacheFullSceneState = false;
//		m_bUseExplicitViewVector = false;
	}

// shared by 2D & 3D views

	int      x;			// left side of view window
	int      y;			// top side of view window
	int      width;		// width of view window
	int      height;	// height of view window

// the rest are only used by 3D views

	bool     m_bOrtho;		// Orthographic projection?

	// View-space rectangle for ortho projection.
	float    m_OrthoLeft;
	float    m_OrthoTop;
	float    m_OrthoRight;
	float    m_OrthoBottom;

	float    fov;			// horizontal FOV in degrees
	float    fovViewmodel;	// horizontal FOV in degrees for in-view model

	Vector3D origin;	// 3D origin of camera
	Angles3D angles;	// heading of camera (pitch, yaw, roll)
	float    zNear;		// local Z coordinate of near plane of camera
	float    zFar;		// local Z coordinate of far plane of camera

	float    zNearViewmodel;	// local Z coordinate of near plane of camera (when rendering view model)
	float    zFarViewmodel;		// local Z coordinate of far plane of camera (when rendering view model)

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool     m_bRenderToSubrectOfLargerScreen;

	float    m_flAspectRatio;	// The aspect ratio to use for computing the perspective projection matrix (0.0f means use the viewport)

	// Controls for off-center projection (needed for poster rendering)
	bool     m_bOffCenter;
	float    m_flOffCenterTop;
	float    m_flOffCenterBottom;
	float    m_flOffCenterLeft;
	float    m_flOffCenterRight;

	bool     m_bDoBloomAndToneMapping;	// Control that the SFM needs to tell the engine not to do certain post-processing steps

	bool     m_bCacheFullSceneState;	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
};

/*
=============================================================

FRUSTUM AND PVS CULLING

=============================================================
*/

inline void ProjectPointOnPlane( Vector3D &dst, const Vector3D &p, const Vector3D &normal )
{
	float d;
	Vector3D n;
	float inv_denom;

	inv_denom = 1.0F / (normal | normal);

	d = (normal | p) * inv_denom;

	n[0] = normal[0] * inv_denom;
	n[1] = normal[1] * inv_denom;
	n[2] = normal[2] * inv_denom;

	dst[0] = p[0] - d * n[0];
	dst[1] = p[1] - d * n[1];
	dst[2] = p[2] - d * n[2];
}
/*
** assumes "src" is normalized
*/
inline void PerpendicularVector( Vector3D &dst, const Vector3D &src )
{
	int	pos;
	int i;
	float minelem = 1.0F;
	Vector3D tempvec;

	/*
	** find the smallest magnitude axially aligned vector
	*/
	for ( pos = 0, i = 0; i < 3; i++ )
	{
		if ( fabsf( src[i] ) < minelem )
		{
			pos = i;
			minelem = fabsf( src[i] );
		}
	}
	tempvec[0] = tempvec[1] = tempvec[2] = 0.0F;
	tempvec[pos] = 1.0F;

	/*
	** project the point onto the plane defined by src
	*/
	ProjectPointOnPlane( dst, tempvec, src );

	/*
	** normalize the result
	*/
	dst.Normalize ();
}
/*
================
R_ConcatRotations
================
*/
inline void R_ConcatRotations (float in1[3][3], float in2[3][3], float out[3][3])
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
				in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
				in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
				in1[0][2] * in2[2][2];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
				in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
				in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
				in1[1][2] * in2[2][2];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
				in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
				in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
				in1[2][2] * in2[2][2];
}
inline void RotatePointAroundVector( Vector3D &dst, const Vector3D &dir, const Vector3D &point, float degrees )
{/*
	float	m[3][3];
	float	im[3][3];
	float	zrot[3][3];
	float	tmpmat[3][3];
	float	rot[3][3];
	int	i;
	Vector3D vr, vup, vf;

	vf[0] = dir[0];
	vf[1] = dir[1];
	vf[2] = dir[2];

	PerpendicularVector( vr, dir );
	vup = vr ^ vf;

	m[0][0] = vr[0];
	m[1][0] = vr[1];
	m[2][0] = vr[2];

	m[0][1] = vup[0];
	m[1][1] = vup[1];
	m[2][1] = vup[2];

	m[0][2] = vf[0];
	m[1][2] = vf[1];
	m[2][2] = vf[2];

	memcpy( im, m, sizeof( im ) );

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

	memset( zrot, 0, sizeof( zrot ) );
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	zrot[0][0] = cosf( DegreeToRadian( degrees ) );
	zrot[0][1] = sinf( DegreeToRadian( degrees ) );
	zrot[1][0] = -sinf( DegreeToRadian( degrees ) );
	zrot[1][1] = cosf( DegreeToRadian( degrees ) );

	R_ConcatRotations( m, zrot, tmpmat );
	R_ConcatRotations( tmpmat, im, rot );

	for ( i = 0; i < 3; i++ )
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
*/
	float	t0, t1;
	float	angle, c, s;
	Vector3D	vr, vu, vf;

	angle = DegreeToRadian( degrees );
	GetCosineSine(angle, c, s);
	vf = dir;
	vf.BuildVectors (vr, vu);

	t0 = vr[0] * c + vu[0] * -s;
	t1 = vr[0] * s + vu[0] *  c;
	dst[0] = (t0 * vr[0] + t1 * vu[0] + vf[0] * vf[0]) * point[0]
	       + (t0 * vr[1] + t1 * vu[1] + vf[0] * vf[1]) * point[1]
	       + (t0 * vr[2] + t1 * vu[2] + vf[0] * vf[2]) * point[2];

	t0 = vr[1] * c + vu[1] * -s;
	t1 = vr[1] * s + vu[1] *  c;
	dst[1] = (t0 * vr[0] + t1 * vu[0] + vf[1] * vf[0]) * point[0]
	       + (t0 * vr[1] + t1 * vu[1] + vf[1] * vf[1]) * point[1]
	       + (t0 * vr[2] + t1 * vu[2] + vf[1] * vf[2]) * point[2];

	t0 = vr[2] * c + vu[2] * -s;
	t1 = vr[2] * s + vu[2] *  c;
	dst[2] = (t0 * vr[0] + t1 * vu[0] + vf[2] * vf[0]) * point[0]
	       + (t0 * vr[1] + t1 * vu[1] + vf[2] * vf[1]) * point[1]
	       + (t0 * vr[2] + t1 * vu[2] + vf[2] * vf[2]) * point[2];
}

/*
==================
BoxOnPlaneSide

Returns 1, 2, or 1 + 2
==================
*/
inline const unsigned char BoxOnPlaneSide (const Vector3D &emins, const Vector3D &emaxs, const PlaneExtended &p)
{
	// fast axial cases
	if (p.type <= PlaneExtended::Type_Z)
	{
		if (p.distance <= emins[p.type])
			return 1u;
		if (p.distance >= emaxs[p.type])
			return 2u;
		return 1u | 2u;
	}

	float dist1, dist2;

	// general case
	switch (p.signBits)
	{
		case Vector3D::AxisBit_None:
			dist1 = p.normal.x*emaxs.x + p.normal.y*emaxs.y + p.normal.z*emaxs.z;
			dist2 = p.normal.x*emins.x + p.normal.y*emins.y + p.normal.z*emins.z;

			break;

		case Vector3D::AxisBit_X:
			dist1 = p.normal.x*emins.x + p.normal.y*emaxs.y + p.normal.z*emaxs.z;
			dist2 = p.normal.x*emaxs.x + p.normal.y*emins.y + p.normal.z*emins.z;

			break;

		case Vector3D::AxisBit_Y:
			dist1 = p.normal.x*emaxs.x + p.normal.y*emins.y + p.normal.z*emaxs.z;
			dist2 = p.normal.x*emins.x + p.normal.y*emaxs.y + p.normal.z*emins.z;

			break;

		case Vector3D::AxisBit_X | Vector3D::AxisBit_Y:
			dist1 = p.normal.x*emins.x + p.normal.y*emins.y + p.normal.z*emaxs.z;
			dist2 = p.normal.x*emaxs.x + p.normal.y*emaxs.y + p.normal.z*emins.z;

			break;

		case Vector3D::AxisBit_Z:
			dist1 = p.normal.x*emaxs.x + p.normal.y*emaxs.y + p.normal.z*emins.z;
			dist2 = p.normal.x*emins.x + p.normal.y*emins.y + p.normal.z*emaxs.z;

			break;

		case Vector3D::AxisBit_X | Vector3D::AxisBit_Z:
			dist1 = p.normal.x*emins.x + p.normal.y*emaxs.y + p.normal.z*emins.z;
			dist2 = p.normal.x*emaxs.x + p.normal.y*emins.y + p.normal.z*emaxs.z;

			break;

		case Vector3D::AxisBit_Y | Vector3D::AxisBit_Z:
			dist1 = p.normal.x*emaxs.x + p.normal.y*emins.y + p.normal.z*emins.z;
			dist2 = p.normal.x*emins.x + p.normal.y*emaxs.y + p.normal.z*emaxs.z;

			break;

		case Vector3D::AxisBit_All:
			dist1 = p.normal.x*emins.x + p.normal.y*emins.y + p.normal.z*emins.z;
			dist2 = p.normal.x*emaxs.x + p.normal.y*emaxs.y + p.normal.z*emaxs.z;

			break;

		default:
			dist1 = dist2 = 0.0f;	// shut up compiler

			InternalAssert (false);

			break;
	}

	unsigned char sides (0u);

	if (dist1 >= p.distance)
		sides = 1u;

	if (dist2 < p.distance)
		sides |= 2u;

	InternalAssert (sides > 0u);

	return sides;
}

//! Defines the view frustum. That's the space visible by the camera.
/** The view frustum is enclosed by 6 planes. These six planes share
	four points. A bounding box around these four points is also stored in
	this structure.
*/
class Frustum
{
	public:
		//-----------------------------------------------------------------------------
		// Frustum plane indices.
		// WARNING: there is code that depends on these values
		//-----------------------------------------------------------------------------
		enum PlaneID_t
		{
			FRUSTUM_RIGHT,		// Right plane of the frustum.
			FRUSTUM_LEFT,		// Left plane of the frustum.
			FRUSTUM_TOP,		// Top plane of the frustum.
			FRUSTUM_BOTTOM,		// Bottom plane of the frustum.
//			FRUSTUM_NEARZ,		// Near plane of the frustum. That is the plane nearest to the eye.
			FRUSTUM_FARZ,		// Far plane of the frustum. That is the plane farest away from the eye.

			FRUSTUM_NUMPLANES	// Amount of planes enclosing the view frustum. Should be 6.
		};

	public:
		PlaneExtended m_Plane[FRUSTUM_NUMPLANES];	//! all planes enclosing the view frustum.
		Vector3D      m_AbsNormal[FRUSTUM_NUMPLANES];

	public:
		inline void SetPlane (const PlaneID_t i, const PlaneExtended::Type_t nType, const Plane &plane)
		{
			m_Plane[i].normal = plane.normal;
			m_Plane[i].distance = plane.distance;
			m_Plane[i].type = nType;
			m_Plane[i].signBits = m_Plane[i].normal.ComputeAndGetSignBits ();

			m_AbsNormal[i].x = fabsf (plane.normal.x);
			m_AbsNormal[i].y = fabsf (plane.normal.y);
			m_AbsNormal[i].z = fabsf (plane.normal.z);
		}

		inline const PlaneExtended &GetPlane     (const PlaneID_t i) const { return m_Plane[i]; }
		inline const Vector3D      &GetAbsNormal (const PlaneID_t i) const { return m_AbsNormal[i]; }

	inline Vector3D GetFarLeftUp (void) const
	{
		//! returns the point which is on the far left upper corner inside the the view frustum.

		Vector3D p;
		m_Plane[FRUSTUM_FARZ].GetIntersectionWithPlanes (m_Plane[FRUSTUM_TOP], m_Plane[FRUSTUM_LEFT], p);

		return p;
	}

	inline Vector3D GetFarLeftDown (void) const
	{
		//! returns the point which is on the far left bottom corner inside the the view frustum.

		Vector3D p;
		m_Plane[FRUSTUM_FARZ].GetIntersectionWithPlanes (m_Plane[FRUSTUM_BOTTOM], m_Plane[FRUSTUM_LEFT], p);

		return p;
	}

	inline Vector3D GetFarRightUp (void) const
	{
		//! returns the point which is on the far right top corner inside the the view frustum.

		Vector3D p;
		m_Plane[FRUSTUM_FARZ].GetIntersectionWithPlanes (m_Plane[FRUSTUM_TOP], m_Plane[FRUSTUM_RIGHT], p);

		return p;
	}

	inline Vector3D GetFarRightDown (void) const
	{
		//! returns the point which is on the far right bottom corner inside the the view frustum.

		Vector3D p;
		m_Plane[FRUSTUM_FARZ].GetIntersectionWithPlanes (m_Plane[FRUSTUM_BOTTOM], m_Plane[FRUSTUM_RIGHT], p);

		return p;
	}

	inline Extent3D GetCalculatdBoundingBox (void) const
	{
		//! calculates and returns the bounding box based on the planes

		Extent3D boundingBox (GetFarLeftUp ());
//		Extent3D boundingBox (cameraPosition);

//		boundingBox.AddInternalPoint (GetFarLeftUp ());
		boundingBox.AddInternalPoint (GetFarRightUp ());
		boundingBox.AddInternalPoint (GetFarLeftDown ());
		boundingBox.AddInternalPoint (GetFarRightDown ());

		return boundingBox;
	}

//-----------------------------------------------------------------------------
// Computes Y fov from an X fov and a screen aspect ratio
//-----------------------------------------------------------------------------
static inline const float CalcFovY (float flFovX, const float flAspect)
{
	if (flFovX < 1.0f || flFovX > 179.0f)
		flFovX = 90.0f;	// error, set to 90

	// The long, but illustrative version (more closely matches CShaderAPIDX8::PerspectiveX, which
	// is what it's based on).
	//
	//float width = 2 * zNear * tan (DegreeToRadian (fov_x / 2.0));
	//float height = width / screenaspect;
	//float yRadians = atan ((height/2.0) / zNear);
	//return RadianToDegree (yRadians) * 2;

	// The short and sweet version.
	float val (atanf (tanf (DegreeToRadian (flFovX) * 0.5f) / flAspect));
	val = RadianToDegree (val) * 2.0f;
	return val;
}
static inline const float CalcFovX (const float flFovY, const float flAspect)
{
	return RadianToDegree (atanf (tanf (DegreeToRadian (flFovY) * 0.5f) * flAspect)) * 2.0f;
}

		//-----------------------------------------------------------------------------
		// Generate a frustum based on perspective view parameters
		//-----------------------------------------------------------------------------
		void GeneratePerspectiveFrustum (const Vector3D &origin, const Vector3D &forward, const Vector3D &right, const Vector3D &up/*, const float flZNear*/, const float flZFar, const float flFovX, const float flFovY)
		{
			const float flIntercept (origin | forward);

			// Setup the near and far planes.
			SetPlane (FRUSTUM_FARZ, PlaneExtended::Type_AnyZ, Plane (-forward, -flZFar - flIntercept));
//			SetPlane (FRUSTUM_NEARZ, PlaneExtended::Type_AnyZ, Plane (forward, flZNear + flIntercept));

			const float flTanX (tanf (DegreeToRadian (flFovX * 0.5f)));
			const float flTanY (tanf (DegreeToRadian (flFovY * 0.5f)));

			// OPTIMIZE: Normalizing these planes is not necessary for culling
			Vector3D normalPos, normalNeg;

			normalPos.MA (right, forward, flTanX).Normalize ();
/// @warning WRONG!			normalNeg.MA (normalPos, right, -2.0f).Normalize ();
			normalNeg.MA (-right, forward, flTanX).Normalize ();

			SetPlane (FRUSTUM_LEFT, PlaneExtended::Type_AnyZ, Plane (normalPos, normalPos | origin));
			SetPlane (FRUSTUM_RIGHT, PlaneExtended::Type_AnyZ, Plane (normalNeg, normalNeg | origin));

			normalPos.MA (up, forward, flTanY).Normalize ();
/// @warning WRONG!			normalNeg.MA (normalPos, up, -2.0f).Normalize ();
			normalNeg.MA (-up, forward, flTanY).Normalize ();

			SetPlane (FRUSTUM_BOTTOM, PlaneExtended::Type_AnyZ, Plane (normalPos, normalPos | origin));
			SetPlane (FRUSTUM_TOP, PlaneExtended::Type_AnyZ, Plane (normalNeg, normalNeg | origin));
		}

		//-----------------------------------------------------------------------------
		// Version that accepts angles instead of vectors
		//-----------------------------------------------------------------------------
		inline void GeneratePerspectiveFrustum (const Vector3D &origin, const Angles3D &angles/*, const float flZNear*/, const float flZFar, const float flFovX, const float flAspectRatio)
		{
			Vector3D vecForward, vecRight, vecUp;

			angles.BuildVectors (vecForward, vecRight, vecUp);

			const float flFovY (CalcFovY (flFovX, flAspectRatio));

			GeneratePerspectiveFrustum (origin, vecForward, vecRight, vecUp/*, flZNear*/, flZFar, flFovX, flFovY);
		}
/*! OLD
inline void R_SetupFrustum(const Vector3D &origin, const Angles3D &angles, const float flZFar, const float flFovX, const float flAspectRatio)
{
			Vector3D vecForward, vecRight, vecUp;

			angles.BuildVectors (vecForward, vecRight, vecUp);

			const float flFovY (CalcFovY (flFovX, flAspectRatio));

	// 0 - left
	// 1 - right
	// 2 - down
	// 3 - up
	// 4 - farclip

	// rotate vecForward right by FOV_X/2 degrees
	RotatePointAroundVector (m_Plane[FRUSTUM_LEFT].normal, vecUp, vecForward, -(90 - flFovX * 0.5f));
	// rotate vecForward left by FOV_X/2 degrees
	RotatePointAroundVector (m_Plane[FRUSTUM_RIGHT].normal, vecUp, vecForward, 90 - flFovX * 0.5f);
	// rotate vecForward up by FOV_X/2 degrees
	RotatePointAroundVector (m_Plane[FRUSTUM_BOTTOM].normal, vecRight, vecForward, 90 - flFovY * 0.5f);
	// rotate vecForward down by FOV_X/2 degrees
	RotatePointAroundVector (m_Plane[FRUSTUM_TOP].normal, vecRight, vecForward, -(90 - flFovY * 0.5f));
	// negate forward vector
	m_Plane[FRUSTUM_FARZ].normal = -vecForward;

	for (unsigned char i (FRUSTUM_RIGHT); i <= FRUSTUM_BOTTOM; ++i)
	{
		m_Plane[i].type = PlaneExtended::Type_AnyZ;
		m_Plane[i].distance = origin | m_Plane[i].normal;
		m_Plane[i].signBits = m_Plane[i].normal.ComputeAndGetSignBits ();
	}

	Vector3D farPoint;

	farPoint.MA (origin, vecForward, flZFar);
	m_Plane[FRUSTUM_FARZ].type = PlaneExtended::Type_AnyZ;
	m_Plane[FRUSTUM_FARZ].distance = farPoint | m_Plane[FRUSTUM_FARZ].normal;
	m_Plane[FRUSTUM_FARZ].signBits = m_Plane[FRUSTUM_FARZ].normal.ComputeAndGetSignBits ();
}
*/
		inline const bool R_CullPoint (const Vector3D &point) const
		{
			for (unsigned char index (FRUSTUM_RIGHT); index < FRUSTUM_NUMPLANES; ++index)
				if (GetPlane (static_cast <PlaneID_t> (index)).GetDistanceToPoint (point) <= 0.0f)
					return true;

			return false;
		}

		// Cull the world-space bounding box to the specified (4-plane) frustum.
		// Returns true if the box is completely outside the frustum.
		inline const bool R_CullBox (const Vector3D &mins, const Vector3D &maxs) const
		{
			for (unsigned char index (FRUSTUM_RIGHT); index < FRUSTUM_NUMPLANES; ++index)
				if (BoxOnPlaneSide (mins, maxs, GetPlane (static_cast <PlaneID_t> (index))) == 2)
					return true;

			return false;
		}
/*		inline const bool R_CullBoxSkipNear (const Vector3D &mins, const Vector3D &maxs) const
		{
			return
			(
				BoxOnPlaneSide (mins, maxs, GetPlane (FRUSTUM_RIGHT)) == 2 ||
				BoxOnPlaneSide (mins, maxs, GetPlane (FRUSTUM_LEFT)) == 2 ||
				BoxOnPlaneSide (mins, maxs, GetPlane (FRUSTUM_TOP)) == 2 ||
				BoxOnPlaneSide (mins, maxs, GetPlane (FRUSTUM_BOTTOM)) == 2 ||
				BoxOnPlaneSide (mins, maxs, GetPlane (FRUSTUM_FARZ)) == 2
			);
		}
*/
		inline const bool R_CullSphere (const Vector3D &center, const float radius) const
		{
			for (unsigned char index (FRUSTUM_RIGHT); index < FRUSTUM_NUMPLANES; ++index)
				if (GetPlane (static_cast <PlaneID_t> (index)).GetDistanceToPoint (center) <= -radius)
					return true;

			return false;
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_FRUSTUM_INCLUDED