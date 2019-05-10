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
// NavigationLadder.hpp
//
// Class: NavigationLadder
//
// Description: AI Navigation areas.
//				Navigation ladders.
//				The NavigationLadder represents ladders in the Navigation Mesh, and their connections to adjacent Navigation Areas.
//
// Version: $ID:$
//

#include <Core.hpp>

NavigationMesh::NavigationLadder::NavigationLadder (HalfLifeEngine::SDK::Classes::Edict *const ladderEdict) :
	BaseNavigationArea (g_server->GetNavigationMeshManager ().IncreaseAndGetNextLadderID ()),	// set an ID for interactive editing - loads will overwrite this

	m_connectedGapAreasNumber (0u),
	m_connectedGapAreas (NULL),

	// compute top & bottom of ladder
	m_top (Math::GetMidPoint (ladderEdict->variables.absoluteBoundingBox.mins.x, ladderEdict->variables.absoluteBoundingBox.maxs.x), Math::GetMidPoint (ladderEdict->variables.absoluteBoundingBox.mins.y, ladderEdict->variables.absoluteBoundingBox.maxs.y), ladderEdict->variables.absoluteBoundingBox.maxs.z),
	m_bottom (m_top.x, m_top.y, ladderEdict->variables.absoluteBoundingBox.mins.z),

	m_edict (ladderEdict)
{
	// determine facing - assumes "normal" runged ladder
	const Math::Vector2D size (ladderEdict->variables.absoluteBoundingBox.maxs.x - ladderEdict->variables.absoluteBoundingBox.mins.x, ladderEdict->variables.absoluteBoundingBox.maxs.y - ladderEdict->variables.absoluteBoundingBox.mins.y);
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	if (size.x > size.y)
	{
		// ladder is facing north or south - determine which way
		// "pull in" traceline from bottom and top in case ladder abuts floor and/or ceiling
//		UTIL_TraceLine (m_bottom + Math::Vector3D (0.0f, GenerationStepSize, GenerationStepSize * 0.5f), m_top + Math::Vector3D (0.0f, GenerationStepSize, -GenerationStepSize * 0.5f), MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_bottom + Math::Vector3D (0.0f, GenerationStepSize, GenerationStepSize * 0.5f), m_top + Math::Vector3D (0.0f, GenerationStepSize, -GenerationStepSize * 0.5f), HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//		SetDirection (traceResult.isStartSolid || traceResult.fraction < 1.0f ? NORTH : SOUTH);
		SetDirection (TraceLineThruWalkableEntities (m_bottom + Math::Vector3D (0.0f, GenerationStepSize, GenerationStepSize * 0.5f), m_top + Math::Vector3D (0.0f, GenerationStepSize, -GenerationStepSize * 0.5f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult) ? SOUTH : NORTH);

		m_width = size.x;
	}
	else
	{
		// ladder is facing east or west - determine which way
//		UTIL_TraceLine (m_bottom + Math::Vector3D (GenerationStepSize, 0.0f, GenerationStepSize * 0.5f), m_top + Math::Vector3D (GenerationStepSize, 0.0f, -GenerationStepSize * 0.5f), MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_bottom + Math::Vector3D (GenerationStepSize, 0.0f, GenerationStepSize * 0.5f), m_top + Math::Vector3D (GenerationStepSize, 0.0f, -GenerationStepSize * 0.5f), HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//		SetDirection (traceResult.isStartSolid || traceResult.fraction < 1.0f ? WEST : EAST);
		SetDirection (TraceLineThruWalkableEntities (m_bottom + Math::Vector3D (GenerationStepSize, 0.0f, GenerationStepSize * 0.5f), m_top + Math::Vector3D (GenerationStepSize, 0.0f, -GenerationStepSize * 0.5f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult) ? EAST : WEST);

		m_width = size.y;
	}

	// adjust top and bottom of ladder to make sure they are reachable (cs_office has a crate right in front of the base of a ladder)
	Math::Vector3D along (m_top - m_bottom);
	const float length (along.GetLength ());
	Math::Vector3D on, out;
	const unsigned char minLadderClearance (32u);
	const unsigned char inc (10u);
	float t;

	// Reliability check.
	InternalAssert (length > 0.0f);

	along /= length;	// Normalize.

	// adjust bottom to bypass blockages
	for (t = 0.0f; t <= length; t += inc)
	{
		on = m_bottom + t * along;
		out = on + m_normal * minLadderClearance;

//		UTIL_TraceLine (on, out, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//		if (!traceResult.isStartSolid && traceResult.fraction == 1.0f)
		if (TraceLineThruWalkableEntities (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult))
		{
			// found viable ladder bottom
			m_bottom = on;

			break;
		}
	}

	// adjust top to bypass blockages
	for (t = 0.0f; t <= length; t += inc)
	{
		on = m_top - t * along;
		out = on + m_normal * minLadderClearance;

//		UTIL_TraceLine (on, out, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//		if (!traceResult.isStartSolid && traceResult.fraction == 1.0f)
		if (TraceLineThruWalkableEntities (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult))
		{
			// found viable ladder top
			m_top = on;

			break;
		}
	}

	m_center = m_top.GetMidPoint (m_bottom);	// compute center of ladder
	m_length = m_top.GetDistance (m_bottom);	// compute ladder length

	SetDirection (m_direction);	// now that we've adjusted the top and bottom, re-check the normal

	CheckWaterLevel ();

	ConnectGeneratedLadder ();
}

NavigationMesh::NavigationLadder::NavigationLadder (const Math::Vector3D &topEdge, const Math::Vector3D &bottomEdge, const Math::Vector3D &leftEdge, const Math::Vector3D &rightEdge, const Math::Vector2D &direction) :
	BaseNavigationArea (g_server->GetNavigationMeshManager ().IncreaseAndGetNextLadderID ()),	// set an ID for interactive editing - loads will overwrite this

	m_connectedGapAreasNumber (0u),
	m_connectedGapAreas (NULL),

	// compute top & bottom of ladder
	m_top (topEdge),
	m_bottom (bottomEdge),

	m_width (leftEdge.GetDistance (rightEdge)),

	m_edict (FindEdict (m_top.GetMidPoint (m_bottom)))
{
	SetDirection (Math::fabsf (direction.x) > Math::fabsf (direction.y) ? direction.x > 0.0f ? EAST : WEST : direction.y > 0.0f ? SOUTH : NORTH);

	// adjust top and bottom of ladder to make sure they are reachable (cs_office has a crate right in front of the base of a ladder)
	Math::Vector3D along (m_top - m_bottom);
	const float length (along.GetLength ());
	Math::Vector3D on, out;
	const unsigned char minLadderClearance (32u);
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	// adjust bottom to bypass blockages
	const unsigned char inc (10u);
	float t;

	// Reliability check.
	InternalAssert (length > 0.0f);

	along /= length;	// Normalize.

	// adjust bottom to bypass blockages
	for (t = 0.0f; t <= length; t += inc)
	{
		on = m_bottom + t * along;
		out = on + m_normal * minLadderClearance;

//		UTIL_TraceLine (on, out, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//		if (!traceResult.isStartSolid && traceResult.fraction == 1.0f)
		if (TraceLineThruWalkableEntities (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult))
		{
			// found viable ladder bottom
			m_bottom = on;

			break;
		}
	}

	// adjust top to bypass blockages
	for (t = 0.0f; t <= length; t += inc)
	{
		on = m_top - t * along;
		out = on + m_normal * minLadderClearance;

//		UTIL_TraceLine (on, out, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//		if (!traceResult.isStartSolid && traceResult.fraction == 1.0f)
		if (TraceLineThruWalkableEntities (on, out, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult))
		{
			// found viable ladder top
			m_top = on;

			break;
		}
	}

	m_center = m_top.GetMidPoint (m_bottom);	// compute center of ladder
	m_length = m_top.GetDistance (m_bottom);	// compute ladder length

	SetDirection (m_direction);	// now that we've adjusted the top and bottom, re-check the normal

	CheckWaterLevel ();

	ConnectGeneratedLadder ();
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationLadder::OnSplit (const NavigationArea *const original, NavigationArea *const alpha, NavigationArea *const beta)
{
	for (unsigned char con (ConnectionType_TopBehind); con < ConnectionType_Total; ++con)
	{
		if (m_connectedAreas[con] != original)
			continue;

		const float alphaDistance (alpha->GetDistanceSquaredToPoint (m_top));
		const float betaDistance (beta->GetDistanceSquaredToPoint (m_top));

		m_connectedAreas[con] = alphaDistance < betaDistance ? alpha : beta;
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Connect this ladder to given area*/
void NavigationMesh::NavigationLadder::ConnectTo (NavigationArea *const area)
{
	if (area->GetCenter ().z > m_center.z)
	{
		// connect to top
		const Math::Vector3D &dirVector (area->GetCenter () - m_top);
		const NavDirType_t direction (Math::fabsf (dirVector.x) > Math::fabsf (dirVector.y) ? dirVector.x > 0.0f ? EAST : WEST : dirVector.y > 0.0f ? SOUTH : NORTH);

		m_connectedAreas[m_direction == direction ? ConnectionType_TopForward : GetOppositeDirection (m_direction) == direction ? ConnectionType_TopBehind : GetDirectionLeft (m_direction) == direction ? ConnectionType_TopLeft : ConnectionType_TopRight] = area;
	}
	else
	{
		// connect to bottom
		m_connectedAreas[ConnectionType_Bottom] = area;
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Disconnect this ladder from given area*/
void NavigationMesh::NavigationLadder::Disconnect (const NavigationArea *const area)
{
	for (unsigned char con (ConnectionType_TopBehind); con < ConnectionType_Total; ++con)
	{
		if (m_connectedAreas[con] != area)
			continue;

		m_connectedAreas[con] = NULL;

		break;
	}
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationLadder::SetDirection (const NavDirType_t direction)
{
	m_direction = direction;
	m_normal = Math::Vector3D::ZeroValue;

	AddDirectionVector (m_normal, direction, 1.0f);	// worst-case, we have the NavDirType_t as a normal

	// Reliability checks.
	InternalAssert (m_normal != Math::Vector3D::ZeroValue);
	InternalAssert (m_edict != NULL);

	const Math::Vector3D &from (m_center + m_normal * 10.0f/*5.0f - ORIGINAL VALUE!!!*/);
	const Math::Vector3D &to (from - m_normal * 32.0f);
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceLine (from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceEntity (from, to, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, traceResult);

	if (traceResult.fraction < 1.0f/* && HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (traceResult.endPosition) == HalfLifeEngine::SDK::Constants::Content_Ladder*/)
	{
		m_normal = traceResult.planeNormal;

		// Reliability check.
//		InternalAssert (m_normal != Math::Vector3D::ZeroValue);
	}
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationLadder::Draw (void) const
{
	const Human *const player (g_server->GetHumanManager ()->GetHostClient ());
	const Math::Vector2D &eye (player->GetEdict ()->GetEyePosition ());
	bool isSelected (this == g_server->GetNavigationMeshManager ().GetSelectedLadder ());
	bool isMarked (this == g_server->GetNavigationMeshManager ().GetMarkedLadder ());
	bool isFront ((Math::Vector2D (eye.x - m_bottom.x, eye.y - m_bottom.y).Normalize () | m_normal) > 0.0f);

	if (g_server->GetNavigationMeshManager ().IsPlaceMode ())
	{
		isSelected = isMarked = false;
		isFront = true;
	}

	// Draw 'ladder' lines ----------------------------------------------------
	const NavEditColor ladderColor (isFront ? isMarked ? NavMarkedColor : isSelected ? NavSelectedColor : NavSamePlaceColor : isMarked ? NavMarkedColor : isSelected ? NavSelectedColor : NavNormalColor);

	Math::Vector3D right, up;

	m_normal.BuildVectors (right, up);

	right *= m_width * 0.5f;

	Math::Vector3D bottomLeft (m_bottom - right);
	Math::Vector3D bottomRight (m_bottom + right);
	const Math::Vector3D topLeft (m_top - right);
	const Math::Vector3D topRight (m_top + right);

	#if defined MULTIPLE_DRAW_METHODS
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
		{
			ColorWithAlpha <unsigned short> bgcolor;

			if (sscanf (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_area_bgcolor)->GetString (), "%hu %hu %hu %hu", &bgcolor.red, &bgcolor.green, &bgcolor.blue, &bgcolor.alpha) == ColorWithAlpha <>::ID_Total && bgcolor.alpha > 0u)
			{
				Math::Vector3D offset;

				AddDirectionVector (offset, GetOppositeDirection (m_direction), 1.0f);

//				NDebugOverlay::Triangle (topLeft + offset, topRight + offset, bottomRight + offset, bgcolor.red, bgcolor.green, bgcolor.blue, bgcolor.alpha, true, 0.15f);
///				player->DrawTriangle (topLeft + offset, bottomRight + offset, topRight + offset, ColorWithAlpha <> (static_cast <unsigned char> (bgcolor.red), static_cast <unsigned char> (bgcolor.green), static_cast <unsigned char> (bgcolor.blue), static_cast <unsigned char> (bgcolor.alpha)), 15u, 255u, 1u, false);
//				NDebugOverlay::Triangle (bottomRight + offset, bottomLeft + offset, topLeft + offset, bgcolor.red, bgcolor.green, bgcolor.blue, bgcolor.alpha, true, 0.15f);
///				player->DrawTriangle (bottomRight + offset, topLeft + offset, bottomLeft + offset, ColorWithAlpha <> (static_cast <unsigned char> (bgcolor.red), static_cast <unsigned char> (bgcolor.green), static_cast <unsigned char> (bgcolor.blue), static_cast <unsigned char> (bgcolor.alpha)), 15u, 255u, 1u, false);
				player->DrawQuadrangle (topLeft + offset, bottomLeft + offset, bottomRight + offset, topRight + offset, ColorWithAlpha <> (static_cast <unsigned char> (bgcolor.red), static_cast <unsigned char> (bgcolor.green), static_cast <unsigned char> (bgcolor.blue), static_cast <unsigned char> (bgcolor.alpha)), 15u, 255u, 1u, false);
			}
		}
	#endif	// if defined MULTIPLE_DRAW_METHODS

	NavDrawLine (topLeft, bottomLeft, ladderColor);
	NavDrawLine (topRight, bottomRight, ladderColor);

	const Math::Vector3D appendOffset (up * GenerationStepSize);

	InternalAssert (bottomRight.z < topRight.z);

	do
	{
		NavDrawLine (bottomRight, bottomLeft, ladderColor);

		bottomRight += appendOffset;
		bottomLeft += appendOffset;
	} while (bottomRight.z < topRight.z);

	// Draw connector lines ---------------------------------------------------
	if (g_server->GetNavigationMeshManager ().IsPlaceMode ())
		return;
//#if 0
	NavDrawLine (m_top, m_bottom, NavConnectedTwoWaysColor);

	for (unsigned char direction (ConnectionType_TopBehind); direction < ConnectionType_Bottom; ++direction)
	{
		const NavigationArea *const adj (m_connectedAreas[direction]);

		if (adj != NULL)
			NavDrawLine (m_top, adj->GetCenter (), adj->IsConnected (this, Direction_Down) ? NavConnectedTwoWaysColor : NavConnectedOneWayColor);
	}

	if (m_connectedAreas[ConnectionType_Bottom] != NULL)
	{
		const Math::Vector3D &areaBottom (m_connectedAreas[ConnectionType_Bottom]->GetCenter ());
		const float offset (m_top.z - m_bottom.z < GenerationStepSize * 1.5f || m_bottom.z - areaBottom.z > GenerationStepSize * 1.5f ? 0.0f : GenerationStepSize);	// don't draw the bottom connection too high if the ladder is: very short/high above the area

		NavDrawLine (m_bottom + Math::Vector3D (0.0f, 0.0f, offset), areaBottom, m_connectedAreas[ConnectionType_Bottom]->IsConnected (this, Direction_Up) ? NavConnectedTwoWaysColor : NavConnectedOneWayColor);
	}

	if (m_connectedGapAreasNumber == 0u)
		return;

	// Reliability check.
	InternalAssert (m_connectedGapAreas != NULL);

	for (unsigned char index (0u); index < m_connectedGapAreasNumber; ++index)
	{
		const Math::Vector3D &position (GetPosAtHeight (m_bottom.z + index * GenerationStepSize));

		for (unsigned char direction (ConnectionType_TopBehind); direction < ConnectionType_GapTotal; ++direction)
		{
			const NavigationArea *const adj (m_connectedGapAreas[index][direction]);

			if (adj != NULL)
				NavDrawLine (position, adj->GetCenter (), /*adj->IsConnected (this, Direction_Down) ? NavConnectedTwoWaysColor : */NavConnectedOneWayColor);
		}
	}
//#endif	// if 0
#if 0
	// get approximate postion of player on ladder
	Math::Vector3D center (GetPosAtHeight (m_bottom.z));

		center.z += GenerationStepSize;

		AddDirectionVector (center, m_direction, HalfLifeEngine::SDK::Constants::HalfHumanWidth);

		Math::Vector3D center2 (center);

		AddDirectionVector (center2, m_direction, static_cast <unsigned char> (2.0f * 75u / GenerationStepSize + 0.5f) * GenerationStepSize);

		NavDrawLine (center, center2, NavConnectedTwoWaysColor);

		center2 = center;

		AddDirectionVector (center2, GetDirectionLeft (m_direction), static_cast <unsigned char> (75u / GenerationStepSize + 0.5f) * GenerationStepSize);

		NavDrawLine (center, center2, NavConnectedTwoWaysColor);

		center2 = center;

		AddDirectionVector (center2, GetDirectionRight (m_direction), static_cast <unsigned char> (75u / GenerationStepSize + 0.5f) * GenerationStepSize);

		NavDrawLine (center, center2, NavConnectedTwoWaysColor);
#endif	// if 0
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationLadder::DrawConnectedAreas (void) const
{
	for (unsigned char direction (ConnectionType_TopBehind); direction < ConnectionType_Total; ++direction)
	{
		const NavigationArea *const adj (m_connectedAreas[direction]);

		if (adj == NULL)
			continue;

		adj->Draw ();

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_hiding_spots)->GetValue <bool> () && !g_server->GetNavigationMeshManager ().IsPlaceMode ())
			adj->DrawHidingSpots ();
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Return true if someone is on this ladder*/
const bool NavigationMesh::NavigationLadder::IsInUse (void) const
{
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (!client->IsOnLadder ())
			continue;

		// client is on a ladder - is it this one?
		const Math::Vector3D &feet (client->GetOrigin ());

		if (feet.z > m_top.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight || feet.z + HalfLifeEngine::SDK::Constants::HumanHeight < m_bottom.z - HalfLifeEngine::SDK::Constants::HalfHumanHeight)
			return true;

		const Math::Vector2D away (m_bottom.x - feet.x, m_bottom.y - feet.y);

		return away.GetLengthSquared () > MATH_GET_SQUARED (50.0f);
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Return true if someone is on this ladder (other than 'ignore')
*/
const bool NavigationMesh::NavigationLadder::IsInUse (const Client *const ignore) const
{
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (client == ignore || !client->IsOnLadder ())
			continue;

		// client is on a ladder - is it this one?
		const Math::Vector3D &feet (client->GetOrigin ());

		if (feet.z > m_top.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight || feet.z + HalfLifeEngine::SDK::Constants::HumanHeight < m_bottom.z - HalfLifeEngine::SDK::Constants::HalfHumanHeight)
			return true;

		const Math::Vector2D away (m_bottom.x - feet.x, m_bottom.y - feet.y);

		return away.GetLengthSquared () > MATH_GET_SQUARED (50.0f);
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------
const Math::Vector3D NavigationMesh::NavigationLadder::GetPosAtHeight (const float height) const
{
	if (height <= m_bottom.z)	// EDWARD: WAS 'height < m_bottom.z'
		return m_bottom;

	if (height >= m_top.z/* || m_top.z == m_bottom.z*/)	// EDWARD: WAS 'height > m_top.z || m_top.z == m_bottom.z'
		return m_top;

	// Reliability check.
	InternalAssert (m_top.z != m_bottom.z);	// BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	const float percent ((height - m_bottom.z) / (m_top.z - m_bottom.z));

	return m_top * percent + m_bottom * (1.0f - percent);
}
//--------------------------------------------------------------------------------------------------------------