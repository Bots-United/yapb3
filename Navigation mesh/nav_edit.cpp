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
// nav_edit.hpp
//
// Description: Implementation of Navigation Mesh edit mode.
//
// Version: $ID:$
//

#include <Core.hpp>
/*
ConVar nav_show_area_info ("nav_show_area_info", "0.5", FCVAR_GAMEDLL, "Duration in seconds to show navigation area ID and attributes while editing.");
ConVar nav_snap_to_grid ("nav_snap_to_grid", "0", FCVAR_GAMEDLL, "Snap to the navigation generation grid when creating new navigation areas.");
ConVar nav_create_place_on_ground ("nav_create_place_on_ground", "0", FCVAR_GAMEDLL, "If true, navigation areas will be placed flush with the ground when created by hand.");
*/

//--------------------------------------------------------------------------------------------------------------
NavigationMesh::BaseNavigationArea *const NavigationMesh::GetNavAreaOrLadderAlongRay (const Math::Vector3D &source, const Math::Vector3D &destination, const BaseNavigationArea *const areaToSkip/* = NULL*/) const
{
	// Try to clip our trace to navigation areas
	if (m_grid == NULL)
		return NULL;

	BaseNavigationArea *bestArea (NULL);
	const Math::Ray ray (source, destination);
	float bestDistance (1.0f);
	Math::Extent3D extent (source);

	extent.Encompass (destination);

	const unsigned char loX (WorldToGridX (extent.mins.x));
	const unsigned char hiX (WorldToGridX (extent.maxs.x));
	const unsigned char hiY (WorldToGridY (extent.maxs.y));
	//unsigned short navAreasTouched (0u);
	//unsigned short gridBucketsTouched (0u);

	for (NavLadderArray::IndexType_t ladderIndex (0u); ladderIndex < m_ladders.GetElementNumber (); ++ladderIndex)
	{
		NavigationLadder *const ladder (m_ladders[ladderIndex]);

		// BY ME!!!
		if (ladder == areaToSkip)
			continue;

		const Math::Vector3D right (ladder->GetWidth () * 0.5f * ladder->GetNormal ().BuildRightVector ());
		const Math::Vector3D left (-right);
		const Math::Vector3D topLeftCorner (ladder->GetTop () + left);
		const Math::Vector3D topRightCorner (ladder->GetTop () + right);
		const Math::Vector3D bottomLeftCorner (ladder->GetBottom () + left);
		const Math::Vector3D bottomRightCorner (ladder->GetBottom () + right);
		float distance (ray.IntersectWithTriangle (topRightCorner, topLeftCorner, bottomLeftCorner, false));

		if (distance > 0.0f && distance < bestDistance)
		{
			bestArea = ladder;
			bestDistance = distance;
		}

		distance = ray.IntersectWithTriangle (topRightCorner, bottomLeftCorner, bottomRightCorner, false);

		if (distance > 0.0f && distance < bestDistance)
		{
			bestArea = ladder;
			bestDistance = distance;
		}
	}

	for (unsigned char loY (WorldToGridY (extent.mins.y)), x; loY <= hiY; ++loY)
		for (x = loX; x <= hiX; ++x)
		{
			const NavAreaArray &areaGrid (m_grid[x + loY * m_gridSize.x]);
			//++gridBucketsTouched;

			for (NavAreaArray::IndexType_t areaIndex (0u); areaIndex < areaGrid.GetElementNumber (); ++areaIndex)
			{
				//++navAreasTouched;

				NavigationArea *const area (areaGrid[areaIndex]);

				if (area == areaToSkip)
					continue;

				const Math::Vector3D &nw (area->m_extent.mins);
				const Math::Vector3D &se (area->m_extent.maxs);
				const Math::Vector3D ne (se.x, nw.y, area->m_neZ);
				const Math::Vector3D sw (nw.x, se.y, area->m_swZ);
				float distance (ray.IntersectWithTriangle (nw, ne, se, false));

				if (distance > 0.0f && distance < bestDistance)
				{
					bestArea = area;
					bestDistance = distance;
				}

				distance = ray.IntersectWithTriangle (se, sw, nw, false);

				if (distance > 0.0f && distance < bestDistance)
				{
					bestArea = area;
					bestDistance = distance;
				}
			}
		}

	//engine->Con_NPrintf (20, "%u areas queried in %u grid buckets, instead of %u areas", navAreasTouched, gridBucketsTouched, m_areas.GetElementNumber ());

	return bestArea;
}

//--------------------------------------------------------------------------------------------------------------
/***  Convenience function to find the navigation area a player is looking at, for editing commands*/
const bool NavigationMesh::FindActiveNavArea (void)
{
//	VPROF ("NavigationMesh::FindActiveNavArea");

	m_splitAlongX = false;
	m_splitEdge = 0.0f;
	m_selectedArea = NULL;
	m_climbableSurface = false;
	m_selectedLadder = NULL;

	const Human *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return false;

	float maxRange (2000.0f/*1000.0f - FROM OLD HL2 ENGINE/*MAYBE VARIABLE?!*/);	// 500
	Math::Vector3D from, direction;
	bool someBOOL (false);

	GetEditVectors (from, direction);

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_create_area_at_feet)->GetValue <bool> () && direction.z < 0.0f)
	{
		const float viewOffsetZ (player->GetEdict ()->variables.viewOffset.z);

		if (viewOffsetZ != 0.0f)
		{
			maxRange = viewOffsetZ / (-direction.z * maxRange) * maxRange;
			someBOOL = true;
		}
	}

	Math::Vector3D to (from + direction * maxRange);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceLine (from, to, MASK_PLAYERSOLID_BRUSHONLY, *player, COLLISION_GROUP_NONE, &traceResult);
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, *player, traceResult);

	if (traceResult.fraction == 1.0f)
		return false;

	if (m_navEditMode != NAV_EDIT_CREATE_AREA)
	{
		const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (traceResult.endPosition));

		m_climbableSurface = pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder || (traceResult.planeNormal.z >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> () && (pointContents == HalfLifeEngine::SDK::Constants::Content_Empty || (pointContents >= HalfLifeEngine::SDK::Constants::Content_Water && pointContents <= HalfLifeEngine::SDK::Constants::Content_Lava)));
		m_surfaceNormal = traceResult.planeNormal;
/*
		{
//			player->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "planeNormal=%s, m_climbableSurface: %s, point contents: %s.\n", traceResult.planeNormal.ToString (), m_climbableSurface ? "true" : "false", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");

			DynamicArray <HalfLifeEngine::SDK::Classes::Edict *> ladders;

			for (HalfLifeEngine::SDK::Classes::Edict *ladder (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_ladder")); ladder->IsValid (); ladder = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ladder, "func_ladder"))
				ladders += ladder;

			for (unsigned short index (0u); index < ladders.GetElementNumber (); ++index)
			{
				HalfLifeEngine::SDK::Structures::TraceResult_t traceResult2;
				Math::Vector3D ladderLeft (ladders[index]->variables.absoluteBoundingBox.mins);
				Math::Vector3D ladderRight (ladders[index]->variables.absoluteBoundingBox.maxs);

				ladderLeft.z = ladderRight.z;

				const Math::Vector3D &diff (((ladderLeft - ladderRight) ^ Math::Vector3D::ZeroValue).Normalize () * 15.0f);
				const Math::Vector3D &front (ladders[index]->GetOrigin () + diff);	// front
				const Math::Vector3D &back (ladders[index]->GetOrigin () - diff);	// back
				Math::Vector3D up (front), down (front);

				down.z = ladders[index]->variables.absoluteBoundingBox.maxs.z;

				HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (down, up, true, HalfLifeEngine::SDK::Constants::Hull_Point, NULL, traceResult2);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (up) == HalfLifeEngine::SDK::Constants::Content_Solid || traceResult2.fraction < 1.0f)
				{
					up = down = back;
					down.z = ladders[index]->variables.absoluteBoundingBox.maxs.z;
				}

				HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (down, up - Math::Vector3D (0.0f, 0.0f, 1000.0f), true, HalfLifeEngine::SDK::Constants::Hull_Point, NULL, traceResult2);

				up = traceResult2.endPosition;

				const Math::Vector3D &pointOrigin (up + Math::Vector3D (0.0f, 0.0f, 39.0f));
				const Math::Vector3D &pointOrigin2 (down + Math::Vector3D (0.0f, 0.0f, 38.0f));
//				const Math::Vector3D &pointOrigin3 (down.z - 40.0f);

				player->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "%u ladders, ladders[%u]: absolute: Min=%s, Max=%s.\n", ladders.GetElementNumber (), index, ladders[index]->variables.absoluteBoundingBox.mins.ToString (), ladders[index]->variables.absoluteBoundingBox.maxs.ToString ());
//				player->DrawLine (from, ladders[index]->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 255u, 0u, 1u);
				player->DrawLine (pointOrigin, pointOrigin2, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 255u, 0u, 1u);
			}
		}
*/
		// check if we're on the same plane as the original point when we're building a ladder
		if ((m_navEditMode == NAV_EDIT_CREATE_LADDER && m_surfaceNormal != m_ladderNormal) || m_surfaceNormal.z > 0.9f)
			m_climbableSurface = false;	// don't try to build ladders on flat ground
	}

	if ((m_climbableSurface && m_navEditMode != NAV_EDIT_CREATE_LADDER) || m_navEditMode != NAV_EDIT_CREATE_AREA)
	{
		float closestDistSqr (MATH_GET_SQUARED (200.0f));

		for (NavLadderArray::IndexType_t index (0u); index < m_ladders.GetElementNumber (); ++index)
		{
			NavigationLadder *const ladder (m_ladders[index]);
			Math::Vector3D absoluteMins (ladder->GetBottom ());
			Math::Vector3D absoluteMaxs (ladder->GetTop ());
			const Math::Vector3D right (ladder->GetWidth () * 0.5f * ladder->GetNormal ().BuildRightVector ());
			const Math::Vector3D left (-right);

			absoluteMins.x += Math::GetMinimumValueBetween (left.x, right.x);
			absoluteMins.y += Math::GetMinimumValueBetween (left.y, right.y);

			absoluteMaxs.x += Math::GetMaximumValueBetween (left.x, right.x);
			absoluteMaxs.y += Math::GetMaximumValueBetween (left.y, right.y);

			if (Math::Extent3D (absoluteMins - 5.0f, absoluteMaxs + 5.0f).Contains (m_editCursorPos))
			{
				m_selectedLadder = ladder;

				break;
			}

			if (!m_climbableSurface)
				continue;

			const float distSqr (ladder->GetCenter ().GetDistanceSquared (m_editCursorPos));

			if (closestDistSqr > distSqr)
			{
				m_selectedLadder = ladder;
				closestDistSqr = distSqr;
			}
		}
	}

	m_editCursorPos = traceResult.endPosition;

	// find the area the player is pointing at
	if (!m_climbableSurface && m_selectedLadder == NULL)
	{
		to = traceResult.endPosition + direction * 100.0f;	// extend a few units into the ground

		/// @note static_cast<NavigationArea *> is used temporarily while 'm_selectedArea' is pointer to 'NavigationArea' - it should be pointer to 'BaseNavigationArea' !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		m_selectedArea = static_cast <NavigationArea *> (GetNavAreaOrLadderAlongRay (from, to));

		if (m_selectedArea == NULL)
			m_selectedArea = GetNearestNavArea (traceResult.endPosition/*, false*/, 500.0f);	// Failing that, get the closest area to the end point
		else if (m_selectedArea->IsNavigationLadder ())
		{
			m_selectedLadder = m_selectedArea->GetLadderPointer <NavigationLadder> ();
			m_selectedArea = NULL;
		}
	}

	if (m_selectedArea != NULL)
	{
		const float yaw (Math::AngleMod (player->GetViewAngles ().yaw));

		if ((yaw < 45.0f || yaw > 315.0f) || (yaw > 135.0f && yaw < 225.0f))
		{
			m_splitEdge = SnapToGrid (traceResult.endPosition.y, true);
			m_splitAlongX = true;
		}
		else
		{
			m_splitEdge = SnapToGrid (traceResult.endPosition.x, true);
			m_splitAlongX = false;
		}
	}

	if (!m_climbableSurface && m_navEditMode != NAV_EDIT_CREATE_LADDER)
		m_editCursorPos = SnapToGrid (m_editCursorPos);
/*{// FROM NEW SOURCE ENGINE
	if (someBOOL)
		m_editCursorPos = SnapToGrid (m_editCursorPos);

	if (m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER)
		return false;

	return someBOOL || GetNavAreaOrLadderAlongRay (from, to) != NULL;
}*/
	return true;
}


//--------------------------------------------------------------------------------------------------------------
inline const bool CheckForClimbableSurface (const Math::Vector3D &start, const Math::Vector3D &end)
{
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceLine (start, end, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (start, end, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);
	TraceLineThruWalkableEntities (start, end, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

	if (traceResult.fraction == 1.0f)
		return false;

	const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (traceResult.endPosition));

	return pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder || (traceResult.planeNormal.z >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> () && (pointContents == HalfLifeEngine::SDK::Constants::Content_Empty || (pointContents >= HalfLifeEngine::SDK::Constants::Content_Water && pointContents <= HalfLifeEngine::SDK::Constants::Content_Lava)));
}


//--------------------------------------------------------------------------------------------------------------
inline void StepAlongClimbableSurface (Math::Vector3D &pos, const Math::Vector3D &increment, const Math::Vector3D &probe)
{
	while (CheckForClimbableSurface (pos + increment - probe, pos + increment + probe))
		pos += increment;
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavBuildLadder (void)
{
	if (m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || !m_climbableSurface)
		return;

	// We've got a ladder at m_editCursorPos, with a normal of m_surfaceNormal
	Math::Vector3D right, up;

	(-m_surfaceNormal).BuildVectors (right, up);

	const Math::Vector3D &startPos (m_editCursorPos);
	Math::Vector3D leftEdge (startPos);
	Math::Vector3D rightEdge (startPos);
	Math::Vector3D topEdge (startPos);
	Math::Vector3D bottomEdge (startPos);
	const Math::Vector3D &probe (m_surfaceNormal * -HalfLifeEngine::SDK::Constants::HalfHumanWidth);
	const unsigned char StepSize (1u);

	// trace to the sides to find the width
	StepAlongClimbableSurface (leftEdge, right * -StepSize, probe);
	StepAlongClimbableSurface (rightEdge, right * StepSize, probe);
	StepAlongClimbableSurface (topEdge, up * StepSize, probe);
	StepAlongClimbableSurface (bottomEdge, up * -StepSize, probe);

	NavigationLadder *const ladder (new NavigationLadder (topEdge, bottomEdge, leftEdge, rightEdge, m_surfaceNormal));

	#if defined _DEBUG
		// Reliability check.
		if (ladder == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation ladder at location: %s!", sizeof (NavigationLadder), startPos.ToString ());

			return;
		}
	#endif	// if defined _DEBUG

	// add ladder to global list
	m_ladders += ladder;
}


//--------------------------------------------------------------------------------------------------------------
/*** Flood fills all areas with current place*/
class PlaceFloodFillFunctor
{
	private:
		const NavigationMesh::Place_t m_initialPlace;

	public:
		inline PlaceFloodFillFunctor (const NavigationMesh::NavigationArea *const area) : m_initialPlace (area->GetPlace ()) { /*VOID*/ }

	private:
		inline PlaceFloodFillFunctor &operator = (const PlaceFloodFillFunctor &/*right*/);	// Avoid "warning C4512: 'PlaceFloodFillFunctor' : assignment operator could not be generated".

	public:
		inline const bool operator () (NavigationMesh::NavigationArea *const area) const
		{
			if (area->GetPlace () != m_initialPlace)
				return false;

			area->SetPlace (g_server->GetNavigationMeshManager ().GetNavPlace ());

			return true;
		}
};


//--------------------------------------------------------------------------------------------------------------
/*** Called when edit mode has just been enabled*/
void NavigationMesh::OnEditModeStart (void)
{
	const Host *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	// Hide crosshair for editor....
	player->HideCrosshair ();
/*! ORIGINAL COMMENT BY VALVE!
	const Math::Vector3D eye (player->GetEdict ()->GetEyePosition ());

	player->GetViewAngles ().MakeVectors ();

	// darken the world so the edit lines show up clearly
	const float screenHalfSize (1000.0f);
	const float screenRange (50.0f);
	const Math::Vector3D upLeft (eye + screenRange * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward + screenHalfSize * (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->upward - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right));
	const Math::Vector3D upRight (eye + screenRange * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward + screenHalfSize * (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->upward + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right));
	const Math::Vector3D downLeft (eye + screenRange * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward - screenHalfSize * (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->upward + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right));
	const Math::Vector3D downRight (eye + screenRange * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward + screenHalfSize * (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->upward));
	const unsigned char alpha (200u);

//	NDebugOverlay::Triangle (downLeft, upLeft, upRight, 0, 0, 0, alpha, true, 999999.9f);
///	player->DrawTriangle (downLeft, upLeft, upRight, ColorWithAlpha <> (0u, 0u, 0u, alpha), 255u);
//	NDebugOverlay::Triangle (downLeft, upRight, downRight, 0, 0, 0, alpha, true, 999999.9f);
///	player->DrawTriangle (downLeft, upRight, downRight, ColorWithAlpha <> (0u, 0u, 0u, alpha), 255u, 255u, 1u, false);
	player->DrawQuadrangle (upLeft, downLeft, downRight, upRight, ColorWithAlpha <> (0u, 0u, 0u, alpha), 255u, 255u, 1u, false);
*/}


//--------------------------------------------------------------------------------------------------------------
/*** Called when edit mode has just been disabled*/
void NavigationMesh::OnEditModeEnd (void)
{
	const Host *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	// Enable crosshair for editor....
	player->DrawCrosshair ();

	if (!m_isNavigationMeshChanged)
		return;

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Note: Navigation mesh changed, but not saved!\n");
}


//--------------------------------------------------------------------------------------------------------------
/*** Draw navigation areas and edit them* @todo Clean the whole edit system up - its structure is legacy from peculiarities in GoldSrc.*/
void NavigationMesh::DrawEditMode (void)
{
//	VPROF ("NavigationMesh::DrawEditMode");

	Human *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	#if defined DEBUG_NAV_NODES
		switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_nodes)->GetValue <Console::Bot::VariableValue_nav_show_nodes_t> ())
		{
			case Console::Bot::VariableValue_nav_show_nodes_GridUnderCursor:
			{
				const unsigned short gridIndex (WorldToNodesGridX (m_editCursorPos.x) + WorldToNodesGridY (m_editCursorPos.y) * m_nodesGridSize.x + WorldToNodesGridZ (m_editCursorPos.z) * m_nodesGridSize.y);

				// Reliability check.
				InternalAssert (gridIndex < m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z);

				// get list in cell that contains cursor position
				const NavigationNodesGridArray &array (m_nodesGrid[gridIndex]);

				for (NavigationNodesGridArray::IndexType_t index (0u); index < array.GetElementNumber (); ++index)
					array[index]->Draw ();
			}

			case Console::Bot::VariableValue_nav_show_nodes_InRadiusUnderCursor:
				for (const NavNodeStack::NodeType_t *node (m_nodes.GetFirst ()); node != NULL; node = node->GetNext ())
				{
					NavigationNode *const navigationNode (node->GetData ());

					if (m_editCursorPos.GetDistanceSquared (navigationNode->GetPosition ()) < MATH_GET_SQUARED (150.0f))
						navigationNode->Draw ();
				}

				break;

			case Console::Bot::VariableValue_nav_show_nodes_All:
				for (const NavNodeStack::NodeType_t *node (m_nodes.GetFirst ()); node != NULL; node = node->GetNext ())
				{
					NavigationNode *const navigationNode (node->GetData ());

					navigationNode->Draw ();
				}

				break;
		}
	#endif	// if defined DEBUG_NAV_NODES

	// draw approach points for marked area
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_approach_points)->GetValue <bool> () && m_markedArea != NULL)
		m_markedArea->DrawApproachPoints ();
/// @note ORIGINAL COMMENT BY MIKE!	/* IN_PROGRESS:
	if (m_navEditMode != NAV_EDIT_PLACE && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_snap_to_grid)->GetValue <bool> ())
	{
		const Math::Vector3D center (SnapToGrid (m_editCursorPos));
		const unsigned char GridCount (3u);
		const unsigned char GridArraySize (GridCount * 2u + 1u);
		const unsigned char GridSize (GetGridSize ());
		Math::Vector3D pos[GridArraySize][GridArraySize];
		GridSize_t grid;

		// fill in an array of heights for the grid
		for (grid.x = 0u; grid.x < GridArraySize; ++grid.x)
			for (grid.y = 0u; grid.y < GridArraySize; ++grid.y)
			{
				pos[grid.x][grid.y].x = center.x + (grid.x - GridCount) * GridSize;
				pos[grid.x][grid.y].y = center.y + (grid.y - GridCount) * GridSize;
				pos[grid.x][grid.y].z = center.z + 36.0f;

				GetGroundHeight (pos[grid.x][grid.y], &pos[grid.x][grid.y].z);
			}

		for (grid.x = 1u; grid.x < GridArraySize; ++grid.x)
			for (grid.y = 1u; grid.y < GridArraySize; ++grid.y)
			{
				NavDrawLine (pos[grid.x - 1u][grid.y - 1u], pos[grid.x - 1u][grid.y], NavGridColor);
				NavDrawLine (pos[grid.x - 1u][grid.y - 1u], pos[grid.x][grid.y - 1u], NavGridColor);

				if (grid.x == GridArraySize - 1u)
					NavDrawLine (pos[grid.x][grid.y - 1u], pos[grid.x][grid.y], NavGridColor);

				if (grid.y == GridArraySize - 1u)
					NavDrawLine (pos[grid.x - 1u][grid.y], pos[grid.x][grid.y], NavGridColor);
			}
	}
//*/
	if (!FindActiveNavArea ())
		return;

	// draw cursor
	const unsigned char cursorSize (10u);

	if (m_climbableSurface)
	{
//		NDebugOverlay::Cross3D (m_editCursorPos, cursorSize, 0, 255, 0, true, 0.1f);
		player->DrawCross3D (m_editCursorPos, cursorSize, Color <> (0u, 255u, 0u), 1u);
	}
	else
	{
		player->DrawCross3DWithoutNegativeZ (m_editCursorPos, cursorSize, NavColors[NavCursorColor], 1u);

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_compass)->GetValue <bool> ())
		{
			// draw compass
			const unsigned char compassOffset (cursorSize + 5u);
#if 0
			NDebugOverlay::Text (m_editCursorPos - Math::Vector2D (0.0f, compassOffset), "N", true, 0.1f/*false, 0.01f*/);
			NDebugOverlay::Text (m_editCursorPos + Math::Vector2D (compassOffset, 0.0f), "E", true, 0.1f/*false, 0.01f*/);
			NDebugOverlay::Text (m_editCursorPos + Math::Vector2D (0.0f, compassOffset), "S", true, 0.1f/*false, 0.01f*/);
			NDebugOverlay::Text (m_editCursorPos - Math::Vector2D (compassOffset, 0.0f), "W", true, 0.1f/*false, 0.01f*/);
#endif	// if 0
			Math::Vector3D screenPosition;

			if (player->GetNormalizedScreenPosition (m_editCursorPos - Math::Vector2D (0.0f, compassOffset), screenPosition))
				player->HudMessageCentered
				(
					screenPosition,
					ColorWithAlpha <> (NavColors[NavCursorColor], 128u),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
					HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_1,

					"N"
				);

			if (player->GetNormalizedScreenPosition (m_editCursorPos + Math::Vector2D (compassOffset, 0.0f), screenPosition))
				player->HudMessageCentered
				(
					screenPosition,
					ColorWithAlpha <> (NavColors[NavCursorColor], 128u),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
					HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_2,

					"E"
				);

			if (player->GetNormalizedScreenPosition (m_editCursorPos + Math::Vector2D (0.0f, compassOffset), screenPosition))
				player->HudMessageCentered
				(
					screenPosition,
					ColorWithAlpha <> (NavColors[NavCursorColor], 128u),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
					HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_3,

					"S"
				);

			if (player->GetNormalizedScreenPosition (m_editCursorPos - Math::Vector2D (compassOffset, 0.0f), screenPosition))
				player->HudMessageCentered
				(
					screenPosition,
					ColorWithAlpha <> (NavColors[NavCursorColor], 128u),
					ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
					HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_4,

					"W"
				);
		}
	}

	// show drag rectangle when creating areas and ladders
	if (m_navEditMode == NAV_EDIT_CREATE_AREA)
	{
		const float z (m_anchor.z + 2.0f);

		NavDrawLine (Math::Vector3D (m_editCursorPos.x, m_editCursorPos.y, z), Math::Vector3D (m_anchor.x, m_editCursorPos.y, z), NavCreationColor);
		NavDrawLine (Math::Vector3D (m_anchor.x, m_anchor.y, z), Math::Vector3D (m_anchor.x, m_editCursorPos.y, z), NavCreationColor);
		NavDrawLine (Math::Vector3D (m_anchor.x, m_anchor.y, z), Math::Vector3D (m_editCursorPos.x, m_anchor.y, z), NavCreationColor);
		NavDrawLine (Math::Vector3D (m_editCursorPos.x, m_editCursorPos.y, z), Math::Vector3D (m_editCursorPos.x, m_anchor.y, z), NavCreationColor);
	}
	else
	{
		if (m_navEditMode == NAV_EDIT_CREATE_LADDER)
		{
			NavDrawLine (m_editCursorPos, Math::Vector3D (m_editCursorPos.x, m_editCursorPos.y, m_ladderAnchor.z), NavCreationColor);
			NavDrawLine (Math::Vector3D (m_editCursorPos.x, m_editCursorPos.y, m_ladderAnchor.z), m_ladderAnchor, NavCreationColor);
			NavDrawLine (m_ladderAnchor, Math::Vector3D (m_ladderAnchor.x, m_ladderAnchor.y, m_editCursorPos.z), NavCreationColor);
			NavDrawLine (Math::Vector3D (m_ladderAnchor.x, m_ladderAnchor.y, m_editCursorPos.z), m_editCursorPos, NavCreationColor);
		}

		if (m_navEditMode != NAV_EDIT_PLACE)
		{
			if (m_markedLadder != NULL)
			{
				// draw the "marked" ladder
				m_markedLadder->Draw ();
			}

			if (m_markedArea != NULL)
			{
				// draw the "marked" area
				m_markedArea->Draw ();
			}
		}
	}

	if (m_selectedLadder != NULL)
	{
		m_lastSelectedArea = NULL;

		// if ladder changed, print its ID
		if (m_selectedLadder != m_lastSelectedLadder)
		{
			m_showAreaInfoTimer.Start (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_area_info)->GetValue <float> ());

			m_lastSelectedLadder = m_selectedLadder;
		}

		if (!m_showAreaInfoTimer.IsElapsed ())
		{
			// print ladder info
//			NDebugOverlay::ScreenText (0.5f, 0.53f, FormatBuffer ("Ladder #%u.\n", m_selectedLadder->GetID ()), 255, 255, 0, 128, Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_area_info)->GetValue <bool> () ? 0.1f : 0.5f);
			player->HudMessageFormat (Math::Vector2D (0.5f, 0.53f), ColorWithAlpha <> (255u, 255u, 0u, 128u), ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u), HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 0.1f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose, "Ladder #%u.", m_selectedLadder->GetID ());
		}

		// draw the ladder we are pointing at and all connected areas
		m_selectedLadder->Draw ();
		m_selectedLadder->DrawConnectedAreas ();
	}
	else if (m_selectedArea != NULL)	// find the area the player is pointing at
	{
#if 0
/// @note JUST TEST
/// @todo REMOVE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (m_markedArea != NULL && m_markedArea != m_selectedArea)
		{
/*			const Math::Vector3D point (m_markedArea->GetClosestPointInPortal3D (m_selectedArea, m_markedArea->ComputeDirection (m_selectedArea->GetCenter ()), m_editCursorPos/*m_selectedArea->GetCenter ()*//*));

			player->DrawCross3D (point, 10.0f, Color <> (0u, 255u, 0u), 1u);
			NavDrawLine (m_markedArea->GetCenter (), m_editCursorPos, NavGridColor);
*/
			BaseNavigationArea::Connection::Portal3D_t portal;
			Math::Vector3D portalsMid;
			const float zOffset (5.0f);
			m_markedArea->ComputePortal3D (m_selectedArea, m_markedArea->ComputeDirection (m_selectedArea->GetCenter ()), portal);

			portalsMid = portal.left.GetMidPoint (portal.right);

			player->DrawCross3D (portalsMid, 10.0f, Color <> (255u, 0u, 0u), 1u);
			NavDrawLine (Math::Vector3D (portal.left.x, portal.left.y, portalsMid.z + zOffset), Math::Vector3D (portal.right.x, portal.right.y, portalsMid.z + zOffset), NavCornerColor);
		}
#endif	// if 0
		m_lastSelectedLadder = NULL;

		// if area changed, print its ID
		if (m_selectedArea != m_lastSelectedArea)
		{
			m_showAreaInfoTimer.Start (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_area_info)->GetValue <float> ());

			m_lastSelectedArea = m_selectedArea;
		}

		if (!m_showAreaInfoTimer.IsElapsed ())
		{
			char locName[80u] = {'\0'/*, ... */};

			if (m_selectedArea->GetPlace () != UNDEFINED_PLACE)
			{
				const DynamicString &name (GetPlaceContainer ().PlaceToName (m_selectedArea->GetPlace ()));

				strcpy (locName, name.IsEmpty () ? "ERROR" : name);
			}

			if (m_navEditMode != NAV_EDIT_PLACE)
			{
/** ORIGINAL CODE!!!				char attributes[80u] = {'\0'/*, ...*//*};
				const NavAttributeFlag_t attributeFlags (m_selectedArea->GetAttributes ());

				if (attributeFlags & NAV_MESH_CROUCH)      strncat (attributes, "CROUCH ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_JUMP)        strncat (attributes, "JUMP ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_DOUBLE_JUMP) strncat (attributes, "DOUBLE JUMP ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_PRECISE)     strncat (attributes, "PRECISE ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_NO_JUMP)     strncat (attributes, "NO JUMP ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_STOP)        strncat (attributes, "STOP ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_RUN)         strncat (attributes, "RUN ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_WALK)        strncat (attributes, "WALK ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_AVOID)       strncat (attributes, "AVOID ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_TRANSIENT)   strncat (attributes, "TRANSIENT ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_DONT_HIDE)   strncat (attributes, "DONT HIDE ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_STAND)       strncat (attributes, "STAND ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_NO_HOSTAGES) strncat (attributes, "NO HOSTAGES ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_STAIRS)      strncat (attributes, "STAIRS ", sizeof (attributes));
//				if (attributeFlags & NAV_MESH_UNMERGEABLE) strncat (attributes, "UNMERGEABLE ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_OBSTACLE)    strncat (attributes, "OBSTACLE ", sizeof (attributes));
				if (attributeFlags & NAV_MESH_CLIFF)       strncat (attributes, "CLIFF ", sizeof (attributes));
				if (m_selectedArea->IsBlocked ())          strncat (attributes, "BLOCKED ", sizeof (attributes));
				if (m_selectedArea->IsUnderwater ())       strncat (attributes, "UNDERWATER ", sizeof (attributes));
*/
				DynamicString attributes (m_selectedArea->PrintAttributes ());

				if (attributes.IsEmpty ())
				{
					if (m_selectedArea->IsBlocked ())
					{
						attributes += "BLOCKED";

						if (m_selectedArea->IsUnderwater ())
							attributes += " + UNDERWATER";
					}
					else if (m_selectedArea->IsUnderwater ())
						attributes += "UNDERWATER";
				}
				else
				{
					if (m_selectedArea->IsBlocked ())
						attributes += " + BLOCKED";

					if (m_selectedArea->IsUnderwater ())
						attributes += " + UNDERWATER";
				}

				// print area info
//				NDebugOverlay::ScreenText (0.5f, 0.53f, FormatBuffer ("Area #%u %s %s %u Connections\n", m_selectedArea->GetID (), locName, attributes, m_selectedArea->GetAdjacentCount ()), 255, 255, 0, 128, 0.1f);
				player->HudMessageFormat (Math::Vector2D (0.5f, 0.53f), ColorWithAlpha <> (255u, 255u, 0u, 128u), ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u), HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 0.1f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose, "Area #%u %s %s %u Connections", m_selectedArea->GetID (), locName, attributes.GetData (), m_selectedArea->GetAdjacentCount ());
			}
			else
			{
				// print area info
//				NDebugOverlay::ScreenText (0.5f, 0.53f, FormatBuffer ("Area #%u %s %u Connections\n", m_selectedArea->GetID (), locName, m_selectedArea->GetAdjacentCount ()), 255, 255, 0, 128, 0.1f);
				player->HudMessageFormat (Math::Vector2D (0.5f, 0.53f), ColorWithAlpha <> (255u, 255u, 0u, 128u), ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u), HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 0.1f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose, "Area #%u %s %u Connections", m_selectedArea->GetID (), locName, m_selectedArea->GetAdjacentCount ());
			}

			// do "place painting"
			if (m_isPlacePainting && m_selectedArea->GetPlace () != m_navPlace)
			{
				m_selectedArea->SetPlace (m_navPlace);

				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/lightswitch2.wav");
			}
		}

		// normal editing mode
		if (m_navEditMode != NAV_EDIT_PLACE)
		{
			const Math::Extent3D &extent (m_selectedArea->GetExtent ());
			Math::Vector3D from, to;

			if (m_splitAlongX)
			{
				from.x = extent.mins.x;
				from.y = m_splitEdge;
				from.z = m_selectedArea->GetZ (from);

				to.x = extent.maxs.x;
				to.y = m_splitEdge;
				to.z = m_selectedArea->GetZ (to);
			}
			else
			{
				from.x = m_splitEdge;
				from.y = extent.mins.y;
				from.z = m_selectedArea->GetZ (from);

				to.x = m_splitEdge;
				to.y = extent.maxs.y;
				to.z = m_selectedArea->GetZ (to);
			}

			// draw split line
			NavDrawLine (from, to, NavSplitLineColor);
		}

		// draw the area we are pointing at and all connected areas/ladders
		m_selectedArea->Draw ();
		m_selectedArea->DrawConnectedAreas ();
	}
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::SetPlacePaintingMode (const bool painting)
{
	if (m_navEditMode != NAV_EDIT_PLACE)
		return;

	m_markedLadder = NULL;
	m_markedArea = NULL;
	m_markedCorner = NUM_CORNERS;

	m_isPlacePainting = painting;
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::SetMarkedLadder (NavigationLadder *const ladder)
{
	m_markedLadder = ladder;
	m_markedArea = NULL;
	m_markedCorner = NUM_CORNERS;
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::SetMarkedArea (NavigationArea *const area)
{
	m_markedLadder = NULL;
	m_markedArea = area;
	m_markedCorner = NUM_CORNERS;
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavDelete (void)
{
	Human *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		DestroyArea (m_selectedArea);
	}
	else if (m_selectedLadder != NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		DestroyLadder (m_selectedLadder);
	}

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavDeleteMarked (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	if (m_markedArea != NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		DestroyArea (m_markedArea);
	}

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavSplit (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, m_selectedArea->SplitEdit (m_splitAlongX, m_splitEdge) ? "weapons/knife_hitwall1.wav" : "buttons/button11.wav");

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::NavigationArea::MakeSniperSpots (void)
{
	InternalAssert (this != NULL);

	bool splitAlongX;
	float splitEdge;
	const unsigned char minSplitSize (2u);	// ensure the first split is larger than this
	const float sizeX (m_extent.GetSizeX ());
	const float sizeY (m_extent.GetSizeY ());

	if (sizeX > GenerationStepSize && sizeX > sizeY)
	{
		splitEdge = Round (m_extent.mins.x, GenerationStepSize);

		if (splitEdge < m_extent.mins.x + minSplitSize)
			splitEdge += GenerationStepSize;

		splitAlongX = false;
	}
	else if (sizeY > GenerationStepSize && sizeY > sizeX)
	{
		splitEdge = Round (m_extent.mins.y, GenerationStepSize);

		if (splitEdge < m_extent.mins.y + minSplitSize)
			splitEdge += GenerationStepSize;

		splitAlongX = true;
	}
	else
		return false;

	NavigationArea *first, *second;

	if (!SplitEdit (splitAlongX, splitEdge, &first, &second))
		return false;

	first->Disconnect (second);
	second->Disconnect (first);

	first->MakeSniperSpots ();
	second->MakeSniperSpots ();

	return true;
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavMakeSniperSpots (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		// recursively split the area
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, m_selectedArea->MakeSniperSpots () ? "weapons/knife_hitwall1.wav" : "buttons/button11.wav");
	}
	else
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavMerge (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedArea != NULL && m_markedArea != m_selectedArea)
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, m_selectedArea->MergeEdit (m_markedArea) ? "buttons/blip1.wav" : "buttons/button11.wav");
		else
		{
			AddLogEntry (true, LogLevel_Default, false, "To merge, mark an area, highlight a second area, then invoke the merge command.");
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
		}
	}

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavMark (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_markedArea != NULL || m_markedLadder != NULL)
	{
		// Unmark area or ladder
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		AddLogEntry (true, LogLevel_Default, false, "Area unmarked.");
		SetMarkedArea (NULL);
	}
/*	else if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount () > 1u)
	{
		const DynamicString areaIDNameToMark (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u));

		if (!areaIDNameToMark.IsEmpty ())
		{
			const unsigned short areaIDToMark (areaIDNameToMark.GetValue <unsigned short> ());

			if (areaIDToMark > 0u)
			{
				NavigationArea *areaToMark (NULL);

				for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
					if (m_areas[index]->GetID () == areaIDToMark)
					{
						areaToMark = m_areas[index];

						break;
					}

				if (areaToMark != NULL)
				{
					player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

					// Mark an area
					SetMarkedArea (areaToMark);

					unsigned short connected (0u);

					connected += m_markedArea->GetAdjacentCount (NORTH);
					connected += m_markedArea->GetAdjacentCount (SOUTH);
					connected += m_markedArea->GetAdjacentCount (EAST);
					connected += m_markedArea->GetAdjacentCount (WEST);

					AddLogEntry (true, LogLevel_Default, false, "Marked Area is connected to %u other Areas.", connected);
				}
			}
			else
				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
	}*/
	else if (m_selectedArea != NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

		// Mark an area
		SetMarkedArea (m_selectedArea);

		unsigned short connected (0u);

		connected += m_markedArea->GetAdjacentCount (NORTH);
		connected += m_markedArea->GetAdjacentCount (SOUTH);
		connected += m_markedArea->GetAdjacentCount (EAST);
		connected += m_markedArea->GetAdjacentCount (WEST);

		AddLogEntry (true, LogLevel_Default, false, "Marked Area is connected to %u other Areas.", connected);
	}
	else if (m_selectedLadder != NULL)
	{
		// Mark a ladder
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");
		SetMarkedLadder (m_selectedLadder);

		unsigned char connected (0u);

		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopBehind) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopLeft) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopRight) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopForward) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_Bottom) != NULL;

		AddLogEntry (true, LogLevel_Default, false, "Marked Ladder is connected to %u Areas.", connected);
	}

	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavUnmark (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavBeginArea (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	if (m_navEditMode == NAV_EDIT_PLACE)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

		return;
	}

	FindActiveNavArea ();

	if (m_navEditMode == NAV_EDIT_CREATE_AREA)
	{
		m_navEditMode = NAV_EDIT_NORMAL;
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
	}
	else if (m_navEditMode == NAV_EDIT_CREATE_LADDER)
	{
		m_navEditMode = NAV_EDIT_NORMAL;
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
	}
	else if (m_climbableSurface)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");
		m_navEditMode = NAV_EDIT_CREATE_LADDER;

		// m_ladderAnchor starting corner
		m_ladderAnchor = m_editCursorPos;
		m_ladderNormal = m_surfaceNormal;
	}
	else
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");
		m_navEditMode = NAV_EDIT_CREATE_AREA;

		// m_anchor starting corner
		m_anchor = m_editCursorPos;
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavEndArea (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	if (m_navEditMode == NAV_EDIT_PLACE)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

		return;
	}

	if (m_navEditMode == NAV_EDIT_CREATE_AREA)
	{
		// create the new navigation area
		NavigationArea *const newArea (new NavigationArea (m_anchor, Math::Vector3D (m_editCursorPos.x, m_editCursorPos.y, m_anchor.z)));

		#if defined _DEBUG
			// Reliability check.
			if (newArea == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (NavigationArea), m_editCursorPos.ToString ());

				m_markedCorner = NUM_CORNERS;	// clear the corner selection

				return;
			}
		#endif	// if defined _DEBUG

		m_areas += newArea;

		AddArea (newArea);

		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_create_place_on_ground)->GetValue <bool> ())
			newArea->PlaceOnGround (NUM_CORNERS);

		// if we have a marked area, inter-connect the two
		if (m_markedArea != NULL)
		{
			const Math::Extent3D &extent (m_markedArea->GetExtent ());

			if (m_anchor.x > extent.maxs.x && m_editCursorPos.x > extent.maxs.x)
			{
				m_markedArea->ConnectTo (newArea, EAST);
				newArea->ConnectTo (m_markedArea, WEST);
			}
			else if (m_anchor.x < extent.mins.x && m_editCursorPos.x < extent.mins.x)
			{
				m_markedArea->ConnectTo (newArea, WEST);
				newArea->ConnectTo (m_markedArea, EAST);
			}
			else if (m_anchor.y > extent.maxs.y && m_editCursorPos.y > extent.maxs.y)
			{
				m_markedArea->ConnectTo (newArea, SOUTH);
				newArea->ConnectTo (m_markedArea, NORTH);
			}
			else if (m_anchor.y < extent.mins.y && m_editCursorPos.y < extent.mins.y)
			{
				m_markedArea->ConnectTo (newArea, NORTH);
				newArea->ConnectTo (m_markedArea, SOUTH);
			}

			// propogate marked area to new area
			SetMarkedArea (newArea);
		}

		m_navEditMode = NAV_EDIT_NORMAL;
	}
	else if (m_navEditMode == NAV_EDIT_CREATE_LADDER)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		// the two points defining the ladder are m_ladderAnchor and m_editCursorPos. The normal is m_ladderNormal.
		CreateLadder (NavigationLadder::FindEdict (Math::Vector3D (Math::GetMinimumValueBetween (m_ladderAnchor.x, m_editCursorPos.x), Math::GetMinimumValueBetween (m_ladderAnchor.y, m_editCursorPos.y), Math::GetMinimumValueBetween (m_ladderAnchor.z, m_editCursorPos.z)).GetMidPoint (Math::Vector3D (Math::GetMaximumValueBetween (m_ladderAnchor.x, m_editCursorPos.x), Math::GetMaximumValueBetween (m_ladderAnchor.y, m_editCursorPos.y), Math::GetMaximumValueBetween (m_ladderAnchor.z, m_editCursorPos.z)))));

		m_navEditMode = NAV_EDIT_NORMAL;
	}
	else
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavConnect (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedLadder != NULL)
		{
			m_markedLadder->ConnectTo (m_selectedArea);

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
		else if (m_markedArea != NULL)
		{
			const NavDirType_t direction (m_markedArea->ComputeDirection (m_editCursorPos));

			if (direction == NUM_DIRECTIONS)
				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
			else
			{
				m_markedArea->ConnectTo (m_selectedArea, direction);

				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
			}
		}
		else
		{
			AddLogEntry (true, LogLevel_Default, false, "To connect areas, mark an area, highlight a second area, then invoke the connect command. Make sure the cursor is directly north, south, east, or west of the marked area.");

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
		}
	}
	else if (m_selectedLadder != NULL)
	{
		if (m_markedArea != NULL)
		{
			m_markedArea->ConnectTo (m_selectedLadder);

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
		else
		{
			AddLogEntry (true, LogLevel_Default, false, "To connect areas, mark an area, highlight a second area, then invoke the connect command. Make sure the cursor is directly north, south, east, or west of the marked area.");

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
		}
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavDisconnect (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedArea != NULL)
		{
			m_markedArea->Disconnect (m_selectedArea);
			m_selectedArea->Disconnect (m_markedArea);

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
		else
		{
			if (m_markedLadder != NULL)
			{
				m_markedLadder->Disconnect (m_selectedArea);
				m_selectedArea->Disconnect (m_markedLadder);

				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
			}
			else
			{
				AddLogEntry (true, LogLevel_Default, false, "To disconnect areas, mark an area, highlight a second area, then invoke the disconnect command. This will remove all connections between the two areas.");

				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
			}
		}
	}
	else if (m_selectedLadder != NULL)
	{
		if (m_markedArea != NULL)
		{
			m_markedArea->Disconnect (m_selectedLadder);
			m_selectedLadder->Disconnect (m_markedArea);

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
		else
		{
			AddLogEntry (true, LogLevel_Default, false, "To disconnect areas, mark an area, highlight a second area, then invoke the disconnect command. This will remove all connections between the two areas.");

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
		}
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavSplice (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedArea != NULL)
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, m_selectedArea->SpliceEdit (m_markedArea) ? "buttons/blip1.wav" : "buttons/button11.wav");
		else
		{
			AddLogEntry (true, LogLevel_Default, false, "To splice, mark an area, highlight a second area, then invoke the splice command to create an area between them.");
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
		}
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavToggleAttribute (const NavAttributeFlag_t attribute)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/bell1.wav");
		m_selectedArea->SetAttributes (m_selectedArea->GetAttributes () ^ attribute);

		if (attribute == NAV_MESH_TRANSIENT)
		{
			if (m_selectedArea->HasAttributes (NAV_MESH_TRANSIENT))
				m_transientAreas += m_selectedArea;
			else
				m_transientAreas -= m_selectedArea;
		}
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavTogglePlaceMode (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER)
		return;

	player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
	m_navEditMode = m_navEditMode == NAV_EDIT_PLACE ? NAV_EDIT_NORMAL : NAV_EDIT_PLACE;

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavPlaceFloodFill (void)
{
	const Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode != NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
		SearchSurroundingAreas (m_selectedArea, m_selectedArea->GetCenter (), PlaceFloodFillFunctor (m_selectedArea));

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavPlacePick (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode != NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		SetNavPlace (m_selectedArea->GetPlace ());
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavTogglePlacePainting (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode != NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_isPlacePainting)
		{
			m_isPlacePainting = false;

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/latchunlocked2.wav");
		}
		else
		{
			m_isPlacePainting = true;

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/lightswitch2.wav");

			// paint the initial area
			m_selectedArea->SetPlace (m_navPlace);
		}
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavMarkUnnamed (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedArea != NULL)
		{
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
			SetMarkedArea (NULL);
		}
		else
		{
			SetMarkedArea (NULL);

			for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
			{
				NavigationArea *const area (m_areas[index]);

				if (area->GetPlace () == UNDEFINED_PLACE)
				{
					SetMarkedArea (area);

					break;
				}
			}

			if (m_markedArea == NULL)
				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
			else
			{
				player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

				unsigned short connected (0u);

				connected += m_markedArea->GetAdjacentCount (NORTH);
				connected += m_markedArea->GetAdjacentCount (SOUTH);
				connected += m_markedArea->GetAdjacentCount (EAST);
				connected += m_markedArea->GetAdjacentCount (WEST);

				NavAreaArray::IndexType_t totalUnnamedAreas (0u);

				for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
					if (m_areas[index]->GetPlace () == UNDEFINED_PLACE)
						++totalUnnamedAreas;

				AddLogEntry (true, LogLevel_Default, false, "Marked Area is connected to %u other Areas - there are %u total unnamed areas.", connected, totalUnnamedAreas);
			}
		}
	}

	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavPickArea (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	if (m_markedLadder != NULL)
	{
		// Unmark ladder
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		AddLogEntry (true, LogLevel_Default, false, "Ladder unmarked.");
		SetMarkedArea (NULL);
		m_markedCorner = NUM_CORNERS;	// clear the corner selection
		return;
	}
	else if (m_selectedArea != NULL)
	{
		const NavCornerType_t bestCorner (m_selectedArea->GetCornerUnderCursor ());

		if (m_markedCorner == bestCorner && m_selectedArea == m_markedArea)
		{
			// Unmark area
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
			AddLogEntry (true, LogLevel_Default, false, "Area unmarked.");
			SetMarkedArea (NULL);
			m_markedCorner = NUM_CORNERS;	// clear the corner selection
			return;
		}


		// Mark an area
		SetMarkedArea (m_selectedArea);
		m_markedCorner = bestCorner;

		unsigned short connected (0u);

		connected += m_markedArea->GetAdjacentCount (NORTH);
		connected += m_markedArea->GetAdjacentCount (SOUTH);
		connected += m_markedArea->GetAdjacentCount (EAST);
		connected += m_markedArea->GetAdjacentCount (WEST);

		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");
		AddLogEntry (true, LogLevel_Default, false, "Marked Area is connected to %d other Areas.", connected);
	}
	else if (m_selectedLadder != NULL)
	{
		// Mark a ladder
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");
		SetMarkedLadder (m_selectedLadder);

		unsigned char connected (0u);

		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopBehind) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopLeft) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopRight) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_TopForward) != NULL;
		connected += m_markedLadder->GetConnectedArea (NavigationLadder::ConnectionType_Bottom) != NULL;

		AddLogEntry (true, LogLevel_Default, false, "Marked Ladder is connected to %u Areas.", connected);

		m_markedCorner = NUM_CORNERS;	// clear the corner selection
	}
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavResizeHorizontal (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	if (m_markedArea == NULL)
		CommandNavPickArea ();	// try to grab an area first

	if (m_markedArea == NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		return;
	}

	if (m_navEditMode == NAV_EDIT_RESIZE_HORIZONTAL)
	{
		m_navEditMode = NAV_EDIT_NORMAL;
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");

		return;
	}

	if (m_navEditMode != NAV_EDIT_NORMAL)
		return;

	m_navEditMode = NAV_EDIT_RESIZE_HORIZONTAL;

	player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

	m_anchor = m_editCursorPos;

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_snap_to_grid)->GetValue <bool> ())
		m_anchor = SnapToGrid (m_anchor);
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavResizeVertical (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	if (m_markedArea == NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		return;
	}

	if (m_navEditMode == NAV_EDIT_RESIZE_VERTICAL)
	{
		m_navEditMode = NAV_EDIT_NORMAL;

		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");

		return;
	}

	if (m_navEditMode != NAV_EDIT_NORMAL)
		return;

	m_navEditMode = NAV_EDIT_RESIZE_VERTICAL;

	player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

	m_anchor = m_editCursorPos;

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_snap_to_grid)->GetValue <bool> ())
		m_anchor = SnapToGrid (m_anchor);
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavResizeEnd (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return;

	if (m_navEditMode != NAV_EDIT_RESIZE_HORIZONTAL && m_navEditMode != NAV_EDIT_RESIZE_VERTICAL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		return;
	}

	if (m_markedArea == NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		m_navEditMode = NAV_EDIT_NORMAL;

		return;
	}

	if (m_navEditMode == NAV_EDIT_RESIZE_HORIZONTAL)
	{
		// find the new pos
		Math::Vector3D from, direction;

		GetEditVectors (from, direction);

		const float distance (10000.0f);
		const float t (Math::IntersectRayWithAAPlane (from, from + direction * distance, Math::Vector3D::AxisID_Z, 1.0f, m_markedCorner == NUM_CORNERS ? m_markedArea->GetCenter ().z : m_markedArea->GetCorner (m_markedCorner).z));

		if (t > 0.0f && t < 1.0f)
		{
			const Math::Vector3D &newPos (from + direction * distance * t);

			m_markedArea->SetCorner (m_markedCorner, Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_snap_to_grid)->GetValue <bool> () ? SnapToGrid (newPos) : newPos);
		}
	}

	player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip2.wav");

	m_navEditMode = NAV_EDIT_NORMAL;
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavCornerSelect (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedArea != NULL)
		{
			m_markedCorner = static_cast <NavCornerType_t> ((m_markedCorner + 1) % (NUM_CORNERS + 1));

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
		else
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
	}
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavCornerRaise (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedArea != NULL)
		{
			if (m_markedCorner == NUM_CORNERS)
				m_markedArea->RaiseCorners (1);
			else
				m_markedArea->RaiseCorner (m_markedCorner, 1);

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
		else
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
	}
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavCornerLower (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		if (m_markedArea != NULL)
		{
			if (m_markedCorner == NUM_CORNERS)
				m_markedArea->RaiseCorners (-1);
			else
				m_markedArea->RaiseCorner (m_markedCorner, -1);

			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
		}
		else
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
	}
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavCornerPlaceOnGround (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedArea != NULL)
	{
		const float inset (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount () == 2u ? HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u).GetValue <float> () : 0.0f);

		if (m_markedArea != NULL)
			m_markedArea->PlaceOnGround (m_markedCorner, inset);
		else
			m_selectedArea->PlaceOnGround (NUM_CORNERS, inset);

		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
	}
	else
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/button11.wav");
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavWarpToMark (void) const
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	if (m_markedArea != NULL)
	{
		if ((!player->IsAlive () || player->IsSpectator ()) && player->GetEdict ()->variables.iuser1 == 5)
			player->GetEdict ()->SetOrigin (m_markedArea->GetCenter () + Math::Vector3D (0.0f, 0.0f, 0.75f * HalfLifeEngine::SDK::Constants::HumanHeight));
		else
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
	}
	else if (m_markedLadder != NULL)
	{
		if ((!player->IsAlive () || player->IsSpectator ()) && player->GetEdict ()->variables.iuser1 == 5)
		{
			Math::Vector3D origin (m_markedLadder->GetCenter ());

			origin.x += m_markedLadder->GetNormal ().x * GenerationStepSize;
			origin.y += m_markedLadder->GetNormal ().y * GenerationStepSize;

			player->GetEdict ()->SetOrigin (origin);
		}
		else
			player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
	}
	else
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CommandNavLadderFlip (void)
{
	Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid () || m_navEditMode == NAV_EDIT_CREATE_AREA || m_navEditMode == NAV_EDIT_CREATE_LADDER || m_navEditMode == NAV_EDIT_PLACE)
		return;

	FindActiveNavArea ();

	if (m_selectedLadder != NULL)
	{
		player->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "buttons/blip1.wav");

		m_selectedLadder->SetDirection (GetOppositeDirection (m_selectedLadder->GetDirection ()));	// flip direction
		m_selectedLadder->ReverseTopConnectedAreas ();	// and reverse ladder's area pointers
	}

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}
//--------------------------------------------------------------------------------------------------------------