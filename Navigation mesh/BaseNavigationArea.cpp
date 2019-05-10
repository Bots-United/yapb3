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
// BaseNavigationArea.cpp
//
// Class: BaseNavigationArea
//
// Description: AI Navigation areas.
//				A BaseNavigationArea is a rectangular region defining a walkable area in the environment.
//
// Version: $ID:$
//

#include <Core.hpp>

//--------------------------------------------------------------------------------------------------------------
/**
* Return the coordinates of the area's corner.
* NOTE: Do not retain the returned pointer - it is temporary.
*/
const Math::Vector3D NavigationMesh::BaseNavigationArea::GetCorner (const NavCornerType_t corner) const
{
	switch (corner)
	{
		case NORTH_WEST:
			return m_extent.mins;

		case NORTH_EAST:
			return Math::Vector3D (m_extent.maxs.x, m_extent.mins.y, m_neZ);

		case SOUTH_WEST:
			return Math::Vector3D (m_extent.mins.x, m_extent.maxs.y, m_swZ);

		#if defined _DEBUG
			case SOUTH_EAST:
		#else	// if defined _DEBUG
			default:	// SOUTH_EAST
		#endif	// if !defined _DEBUG
			return m_extent.maxs;

		#if defined _DEBUG
			default:
				AddLogEntry (false, LogLevel_Critical, false, "BaseNavigationArea::GetCorner(): Invalid type %u!", corner);

				return Math::Vector3D::ZeroValue;
		#endif	// if !defined _DEBUG
	}
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::BaseNavigationArea::SetCorner (const NavCornerType_t corner, const Math::Vector3D &newPosition)
{
	switch (corner)
	{
		case NORTH_WEST:
			m_extent.mins = newPosition;

			break;

		case NORTH_EAST:
			m_extent.maxs.x = newPosition.x;
			m_extent.mins.y = newPosition.y;
			m_neZ = newPosition.z;

			break;

		case SOUTH_WEST:
			m_extent.mins.x = newPosition.x;
			m_extent.maxs.y = newPosition.y;
			m_swZ = newPosition.z;

			break;

		case SOUTH_EAST:
			m_extent.maxs = newPosition;

			break;

		default:
		{
			const Math::Vector3D delta (newPosition - m_center);

			m_extent.mins += delta;
			m_extent.maxs += delta;
			m_neZ += delta.z;
			m_swZ += delta.z;

			break;
		}
	}

	m_center = m_extent.mins.GetMidPoint (m_extent.maxs);

	if (IsDegenerate ())
	{
		m_extentInvertedSizeX = 0.0f;
		m_extentInvertedSizeY = 0.0f;
	}
	else
	{
		m_extentInvertedSizeX = m_extent.GetInvertedSizeX ();
		m_extentInvertedSizeY = m_extent.GetInvertedSizeY ();
	}
/*
	// Update a area position in a navigation mesh grid after we have updated area extent....
	g_server->GetNavigationMeshManager ().RemoveAreaFromGrid (this);
	g_server->GetNavigationMeshManager ().AddAreaToGrid (this);*/
}

//--------------------------------------------------------------------------------------------------------------
/**
* Return true if given point is on or above this area, but no others
*/
const bool NavigationMesh::BaseNavigationArea::Contains (const Math::Vector3D &pos) const
{
	// check 2D overlap
	if (!IsOverlapping (pos))
		return false;

	// the point overlaps us, check that it is above us, but not above any areas that overlap us
	const float myZ (GetZ (pos));

	// if the navigation area is above the given position, fail
	return myZ <= pos.z;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Return true if this area and given area are approximately co-planar
*/
const bool NavigationMesh::BaseNavigationArea::IsCoplanar (const BaseNavigationArea *const area) const
{
	if (!IsFlat () || !area->IsFlat ())
		return false;

	// compute our unit surface normal
	const Math::Vector3D &normal (ComputeNormal ()), &otherNormal (area->ComputeNormal ());

	// can only merge areas that are nearly planar, to ensure areas do not differ from underlying geometry much
	return (normal | otherNormal) > Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_coplanar_slope_limit)->GetValue <float> ();	//0.99f;// 0.7071f;// 0.9
}

//--------------------------------------------------------------------------------------------------------------
/**
* Return shortest distance squared between point and this area
*/
const float NavigationMesh::BaseNavigationArea::GetDistanceSquaredToPoint (const Math::Vector3D &pos) const
{
	if (pos.x < m_extent.mins.x)
	{
		if (pos.y < m_extent.mins.y)
		{
			// position is north-west of area
			return m_extent.mins.GetDistanceSquared (pos);
		}
		else if (pos.y > m_extent.maxs.y)
		{
			// position is south-west of area
			return Math::Vector3D (m_extent.mins.x - pos.x, m_extent.maxs.y - pos.y, m_swZ - pos.z).GetLengthSquared ();
		}
		else
		{
			// position is west of area
			const float d (m_extent.mins.x - pos.x);

			return MATH_GET_SQUARED (d);
		}
	}
	else if (pos.x > m_extent.maxs.x)
	{
		if (pos.y < m_extent.mins.y)
		{
			// position is north-east of area
			return Math::Vector3D (m_extent.maxs.x - pos.x, m_extent.mins.y - pos.y, m_neZ - pos.z).GetLengthSquared ();
		}
		else if (pos.y > m_extent.maxs.y)
		{
			// position is south-east of area
			return m_extent.maxs.GetDistanceSquared (pos);
		}
		else
		{
			// position is east of area
			const float d (pos.z - m_extent.maxs.x);

			return MATH_GET_SQUARED (d);
		}
	}
	else if (pos.y < m_extent.mins.y)
	{
		// position is north of area
		const float d (m_extent.mins.y - pos.y);

		return MATH_GET_SQUARED (d);
	}
	else if (pos.y > m_extent.maxs.y)
	{
		// position is south of area
		const float d (pos.y - m_extent.maxs.y);

		return MATH_GET_SQUARED (d);
	}
	else
	{
		// position is inside of 2D extent of area - find delta Z
		const float d (GetZ (pos) - pos.z);

		return MATH_GET_SQUARED (d);
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
* Return Z of area at (x, y) of 'pos'
* Trilinear interpolation of Z values at quad edges.
*/
const float NavigationMesh::BaseNavigationArea::GetZ (const Math::Vector2D &pos) const
{
	const float dx (m_extent.GetSizeX ());
	const float dy (m_extent.GetSizeY ());

	// guard against division by zero due to degenerate areas
	if (dx == 0.0f || dy == 0.0f)
//	if (m_extentInvertedSizeX == 0.0f || m_extentInvertedSizeY == 0.0f)
		return m_neZ;

	// clamp Z values to (x, y) volume
	const float u (Math::GetClampedValueInRange ((pos.x - m_extent.mins.x) / dx, 0.0f, 1.0f));
//	const float u (Math::GetClampedValueInRange ((pos.x - m_extent.mins.x) * m_extentInvertedSizeX, 0.0f, 1.0f));
	const float v (Math::GetClampedValueInRange ((pos.y - m_extent.mins.y) / dy, 0.0f, 1.0f));
//	const float v (Math::GetClampedValueInRange ((pos.y - m_extent.mins.y) * m_extentInvertedSizeY, 0.0f, 1.0f));

	const float northZ (Math::GetInterpolated (m_extent.mins.z, m_neZ, u));
	const float southZ (Math::GetInterpolated (m_swZ, m_extent.maxs.z, u));

	return Math::GetInterpolated (northZ, southZ, v);
}

//--------------------------------------------------------------------------------------------------------------
/**
* Return closest point to 'pos' on 'area'.
* Returned point is in 'close'.
*/
const Math::Vector3D NavigationMesh::BaseNavigationArea::GetClosestPointOnArea (const Math::Vector2D &pos) const
{
	Math::Vector3D close;
/*
	if (pos.x < m_extent.mins.x)
	{
		close.x = m_extent.mins.x;

		if (pos.y < m_extent.mins.y)
		{
			// position is north-west of area
			close.y = m_extent.mins.y;
		}
		else if (pos.y > m_extent.maxs.y)
		{
			// position is south-west of area
			close.y = m_extent.maxs.y;
		}
		else
		{
			// position is west of area
			close.y = pos.y;
		}
	}
	else if (pos.x > m_extent.maxs.x)
	{
		close.x = m_extent.maxs.x;

		if (pos.y < m_extent.mins.y)
		{
			// position is north-east of area
			close.y = m_extent.mins.y;
		}
		else if (pos.y > m_extent.maxs.y)
		{
			// position is south-east of area
			close.y = m_extent.maxs.y;
		}
		else
		{
			// position is east of area
			close.y = pos.y;
		}
	}
	else
	{
		close.x = pos.x;

		if (pos.y < m_extent.mins.y)
		{
			// position is north of area
			close.y = m_extent.mins.y;
		}
		else if (pos.y > m_extent.maxs.y)
		{
			// position is south of area
			close.y = m_extent.maxs.y;
		}
		else
		{
			// position is inside of area - it is the 'closest point' to itself
			close.y = pos.y;
		}
	}
*/{/*! MORE READABLE (IMHO) CODE FROM NEW SOURCE ENGINE !*/
	close.x = pos.x - m_extent.mins.x < 0.0f ? m_extent.mins.x : pos.x;

	if (close.x - m_extent.maxs.x >= 0.0f)
		close.x = m_extent.maxs.x;

	close.y = pos.y - m_extent.mins.y < 0.0f ? m_extent.mins.y : pos.y;

	if (close.y - m_extent.maxs.y >= 0.0f)
		close.y = m_extent.maxs.y;
}
	close.z = GetZ (close);

	return close;
}

void NavigationMesh::BaseNavigationArea::DrawPlayerCounts (void) const
{
	if (GetPlayerCount () == 0u)
		return;

//	CFmtStr msg;

//	NDebugOverlay::Text (GetCenter (), msg.sprintf ("%d (%d/%d)", GetPlayerCount (), GetPlayerCount (1), GetPlayerCount (2)), false, 0.1f);
	Math::Vector2D screenPosition;

	if (g_server->GetHumanManager ()->GetHostClient ()->GetNormalizedScreenPosition (GetCenter (), screenPosition))
		g_server->GetHumanManager ()->GetHostClient ()->HudMessageCenteredFormat
		(
			screenPosition,
			ColorWithAlpha <> (255u, 255u, 0u, 128u),
			ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
			HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

			"Area %u contains %u (%u/%u) players.", m_id, GetPlayerCount (), GetPlayerCount (TeamArrayID_Terrorist), GetPlayerCount (TeamArrayID_CounterTerrorist)
		);
}

void NavigationMesh::BaseNavigationArea::DrawOccupyTime (void) const
{
	const float freezeEndTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - g_server->GetRoundInformation ().freezeEndTime);
	Color <> color;

	if (freezeEndTime > GetEarliestOccupyTime (TeamArrayID_Terrorist))
	{
		color.red = 255u;

		if (freezeEndTime > GetEarliestOccupyTime (TeamArrayID_CounterTerrorist))
			color.blue = 255u;
	}
	else
	{
		if (freezeEndTime <= GetEarliestOccupyTime (TeamArrayID_CounterTerrorist))
			return;

		color.blue = 255u;
	}

	Math::Vector3D corners[NUM_CORNERS];

	GetCorners (corners);

	for (unsigned char cornerIndex (NORTH_EAST); cornerIndex < NUM_CORNERS; ++cornerIndex)
//		NDebugOverlay::Line (corners[cornerIndex - 1u], corners[cornerIndex], color.red, color.green, color.blue, 1u, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (corners[cornerIndex - 1u], corners[cornerIndex], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 0u, 1u);
}

void NavigationMesh::BaseNavigationArea::DrawBattlefront (void) const
{
	if (Math::fabsf (GetEarliestOccupyTime (TeamArrayID_Terrorist) - GetEarliestOccupyTime (TeamArrayID_CounterTerrorist)) > 1.0f)
		return;

	Math::Vector3D corners[NUM_CORNERS];
	const Color <> color (255u, 50u, 0u);

	GetCorners (corners);

	for (unsigned char cornerIndex (NORTH_EAST); cornerIndex < NUM_CORNERS; ++cornerIndex)
//		NDebugOverlay::Line (corners[cornerIndex - 1u], corners[cornerIndex], color.red, color.green, color.blue, 1u, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (corners[cornerIndex - 1u], corners[cornerIndex], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 0u, 1u);
}