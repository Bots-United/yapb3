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
// NavigationNode.hpp
//
// Class: NavigationNode
//
// Description: AI Navigation Nodes.
//				Navigation Nodes are used when generating a Navigation Mesh by point sampling the map.
//				These Nodes encapsulate world locations, and ways to get from one location to an adjacent one.
//				Note that these links are not necessarily commutative (falling off of a ledge, for example).
//
// Version: $ID:$
//

#include <Core.hpp>

unsigned int NavigationMesh::NavigationNode::m_nextID (0u);	// Reset the next node ID to 0
/*
ConVar nav_show_nodes ("nav_show_nodes", "0");
*/

//--------------------------------------------------------------------------------------------------------------
/*** Constructor*/
NavigationMesh::NavigationNode::NavigationNode (const Math::Vector3D &pos, const Math::Vector3D &normal, NavigationNode *const parent/* = NULL*/) :
	m_pos (pos),
	m_normal (normal),

	m_id (++m_nextID),

	m_visited (0u),
	m_parent (parent),

	m_isCovered (false),
	m_area (NULL),

	m_attributeFlags (NavAttributeFlag_None)
{
	// Reliability check.
	InternalAssert (m_normal != Math::Vector3D::ZeroValue);

	unsigned char index;

	for (index = NORTH; index < NUM_DIRECTIONS; ++index)
		m_to[index] = NULL;

	for (index = NORTH_WEST; index < NUM_CORNERS; ++index)
		m_crouch[index] = false;

	g_server->GetNavigationMeshManager ().AddNavigationNode (this);	// Add this node to the main navigation mesh nodes array.

	#if defined DEBUG_NAV_NODES
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_nodes)->GetValue <bool> ())
		{
//			NDebugOverlay::Cross3D (m_pos, 10.0f, 128, 128, 128, true, 10.0f);
			g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (m_pos, 10.0f, 128u, 255u, 255u, 1u, false);
//			NDebugOverlay::Cross3D (m_pos, 10.0f, 255, 255, 255, false, 10.0f);
			g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (m_pos, 10.0f, 255u, 255u);

			{
				//! @TODO: Make changing view angles for humans right!
				HalfLifeEngine::SDK::Structures::EntityVariables_t &clientVariables (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables);

				// set the body angles to point the gun correctly
				clientVariables.modelAngles = clientVariables.viewAngles = (m_pos - clientVariables.origin).ToAngles3D ();

				clientVariables.modelAngles.pitch *= -(1.0f / 3.0f);	// invert for engine (adjust the view angle pitch to aim correctly)

				clientVariables.fixAngle = HalfLifeEngine::SDK::Constants::FixAngleType_Absolute;
			}
		}
	#endif	// if defined DEBUG_NAV_NODES
}

/*
//--------------------------------------------------------------------------------------------------------------
#if defined DEBUG_NAV_NODES
ConVar nav_show_node_id ("nav_show_node_id", "0");
ConVar nav_test_node ("nav_test_node", "0");
ConVar nav_test_node_crouch ("nav_test_node_crouch", "0");
ConVar nav_test_node_crouch_dir ("nav_test_node_crouch_dir", "4");
#endif	// if defined DEBUG_NAV_NODES
*/

#if defined DEBUG_NAV_NODES
//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationNode::Draw (void)
{
	Color <> color;

	if (m_isCovered)
	{
		if (m_attributeFlags & NAV_MESH_CROUCH)
			color.blue = 255u;
		else
			color.red = 255u;
	}
	else
	{
		if (m_attributeFlags & NAV_MESH_CROUCH)
			color.blue = 255u;

		color.green = 255u;
	}

//	NDebugOverlay::Cross3D (m_pos, 2.0f, color.red, color.green, color.blue, true, 0.1f);
	g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (m_pos, 2.0f, color, 1u, 255u, 1u, false);

	if ((!m_isCovered && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_node_id)->GetValue <bool> ()) || (m_isCovered && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_node_id)->GetValue <int> () < 0))
	{
//		NDebugOverlay::Text (m_pos, FormatBuffer ("%u", m_id), true, 0.1f);
		Math::Vector2D screenPosition;

		if (g_server->GetHumanManager ()->GetHostClient ()->GetNormalizedScreenPosition (m_pos, screenPosition))
			g_server->GetHumanManager ()->GetHostClient ()->HudMessageCenteredFormat
			(
				screenPosition,
				ColorWithAlpha <> (255u, 255u, 255u, 255u),
				ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
				HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_1,

				"%u\n", m_id
			);
	}

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node)->GetValue <unsigned int> () == m_id)
	{
		g_server->GetNavigationMeshManager ().TestArea (this, 1u, 1u);

		Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node)->SetValue <unsigned char> (0u);
	}

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch)->GetValue <unsigned int> () == m_id)
	{
		CheckCrouch ();

		Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch)->SetValue <unsigned char> (0u);
	}

	if (m_attributeFlags & NAV_MESH_CROUCH)
	{
		const unsigned char scale (3u);

		for (unsigned char index (NORTH_WEST); index < NUM_CORNERS; ++index)
		{
			if (!m_crouch[index])
				continue;

			const Math::Vector2D &scaled (GetCornerVector2D (static_cast <NavCornerType_t> (index)) * scale);

//			NDebugOverlay::HorzArrow (m_pos, m_pos + scaled, 0.5, 0, 0, 255, 255, true, 0.1f);
			g_server->GetHumanManager ()->GetHostClient ()->DrawHorizontalArrow (m_pos, m_pos + scaled, Color <> (0u, 0u, 255u), 1u, 1u, false);
		}
	}
}
#endif	// if defined DEBUG_NAV_NODES

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationNode::CheckCrouch (void)
{
/*
	/// FROM HL1 CCSBot::AddNode()!!!
	{
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		float mins (-HalfLifeEngine::SDK::Constants::HalfHumanWidth), maxs;
		Math::Vector3D from, to;
		bool isCrouch (false);

		do
		{
			maxs = -HalfLifeEngine::SDK::Constants::HalfHumanWidth;

			for (;;)
			{
				from.x = to.x = maxs + m_pos.x;
				from.y = to.y = mins + m_pos.y;
				from.z = m_pos.z + 5.0f;

				to.z = m_pos.z + HalfLifeEngine::SDK::Constants::HumanHeight - Math::RawEpsilon;

				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

				if (traceResult.fraction < 1.0f)
					break;

				maxs += HalfLifeEngine::SDK::Constants::HalfHumanWidth;

				if (maxs > HalfLifeEngine::SDK::Constants::HalfHumanWidth + Math::RawEpsilon)
				{
					mins += HalfLifeEngine::SDK::Constants::HalfHumanWidth;

					goto Label_SetCrouch;
				}
			}

			isCrouch = true;

Label_SetCrouch:
			if (isCrouch)
			{
				m_crouch[NORTH_WEST] = m_crouch[NORTH_EAST] = m_crouch[SOUTH_EAST] = m_crouch[SOUTH_WEST] = true;

				return;
			}
		} while (mins <= HalfLifeEngine::SDK::Constants::HalfHumanWidth + Math::RawEpsilon);

		mins += HalfLifeEngine::SDK::Constants::HalfHumanWidth;

		if (isCrouch)
		{
			m_crouch[NORTH_WEST] = m_crouch[NORTH_EAST] = m_crouch[SOUTH_EAST] = m_crouch[SOUTH_WEST] = true;

			return;
		}
	}
*/
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	// For each direction, trace upwards from our best ground height to HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z to see if we have standing room.
	for (unsigned char corner (NORTH_WEST); corner < NUM_CORNERS; ++corner)
	{
		const Math::Vector2D &cornerDirection (m_pos + GetCornerVector2D (static_cast <NavCornerType_t> (corner)) * HalfLifeEngine::SDK::Constants::HalfHumanWidth);

		TraceHullThruWalkableEntities (cornerDirection + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), cornerDirection - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::MapSize)/* + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z)*/, true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
		TraceHullThruWalkableEntities (traceResult.endPosition, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_DUCK.z), true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);

		if (/*traceResult.isStartSolid || */traceResult.fraction < 1.0f)
		{
			SetAttributes (NAV_MESH_CROUCH);

			m_crouch[corner] = true;
		}

		/// @todo MAKE POINT CONTENTS CHECK INSTEAD OF TRACING!!!!!!!!!!!!!??????????????????
	}
#if 0
{// ANOTHER METHOD BY EDWARD!!!! (NOT COMPLETED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
	const unsigned char maximumOffsetZ (5u);
	unsigned char currentOffsetZ (0u);
	Math::Vector3D position (m_pos);

	/// @todo REMIND ABOUT THIS OFFSET!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	position.z += HalfLifeEngine::SDK::Constants::HumanWidth * m_normal.z;

	do
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->GetHullPointContents (traceResult.endPosition, HalfLifeEngine::SDK::Constants::Hull_Head) != HalfLifeEngine::SDK::Constants::Content_Solid)
			return;

		position.z += ++currentOffsetZ;
	} while (currentOffsetZ <= maximumOffsetZ);

	// We in solid....
	m_crouch[NORTH_WEST] = m_crouch[NORTH_EAST] = m_crouch[SOUTH_EAST] = m_crouch[SOUTH_WEST] = true;

	return;
}
#endif	// if 0
#if 0
//	CTraceFilterWalkableEntities filter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT, WALK_THRU_EVERYTHING);
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
//	Math::Vector2D mins (-HalfLifeEngine::SDK::Constants::HalfHumanWidth, -HalfLifeEngine::SDK::Constants::HalfHumanWidth);
//	Math::Vector2D maxs (HalfLifeEngine::SDK::Constants::HalfHumanWidth, HalfLifeEngine::SDK::Constants::HalfHumanWidth);
	Math::Vector3D start (m_pos.x, m_pos.y, m_pos.z + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z - Math::RawEpsilon);

	// Trace downward from duck height to find the max floor height for the node's surroundings
//	UTIL_TraceHull (start, m_pos, mins, maxs, MASK_PLAYERSOLID_BRUSHONLY, &filter, &traceResult);
//	TraceHullThruWalkableEntities (start, m_pos, true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
	TraceLineThruWalkableEntities (start, m_pos, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);
//	TraceHull (start, m_pos, mins, maxs, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

//	g_server->GetHumanManager ()->GetHostClient ()->DrawLine (start, m_pos, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 255u);
//	AddLogEntry (true, LogLevel_Default, false, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s.", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle");

	Math::Vector3D groundPos (traceResult.endPosition);

	if (traceResult.isStartSolid && !traceResult.isAllSolid)
	{
		// Try going down out of the solid and re-check for the floor height
		start.z -= traceResult.endPosition.z - Math::RawEpsilon;

//		UTIL_TraceHull (start, m_pos, mins, maxs, MASK_PLAYERSOLID_BRUSHONLY, &filter, &traceResult);
//		TraceHullThruWalkableEntities (start, m_pos, true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
		TraceLineThruWalkableEntities (start, m_pos, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);
//		TraceHull (start, m_pos, mins, maxs, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

//		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (start, m_pos, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 255u);
//		AddLogEntry (true, LogLevel_Default, false, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s.", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle");

		groundPos = traceResult.endPosition;
	}

	if (traceResult.isStartSolid)
	{
		// We don't even have duck height clear. Try a simple check to find floor height.
// - MADE BY EDWARD START!!!!!!!!!!!!!!!!!!!!
//		Math::Vector2D tempVector2D;
//		float maxFloorZ (m_pos.z);
		float currentFloorZ;
/*
		// Find the highest floor z - for a player to stand in this area, we need a full HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z of clearance above this height at all points.
		for (tempVector2D.y = -HalfLifeEngine::SDK::Constants::HalfHumanWidth; tempVector2D.y <= HalfLifeEngine::SDK::Constants::HalfHumanWidth + Math::RawEpsilon; tempVector2D.y += HalfLifeEngine::SDK::Constants::HalfHumanWidth)
			for (tempVector2D.x = -HalfLifeEngine::SDK::Constants::HalfHumanWidth; tempVector2D.x <= HalfLifeEngine::SDK::Constants::HalfHumanWidth + Math::RawEpsilon; tempVector2D.x += HalfLifeEngine::SDK::Constants::HalfHumanWidth)
				if (NavigationMesh::GetGroundHeight (m_pos, &currentFloorZ))
					maxFloorZ = Math::GetMaximumValueBetween (maxFloorZ, currentFloorZ + Math::RawEpsilon);

		groundPos.x = m_pos.x;
		groundPos.y = m_pos.y;
		groundPos.z = maxFloorZ;
*/
		if (NavigationMesh::GetGroundHeight (m_pos, &currentFloorZ) && m_pos.z <= currentFloorZ += Math::RawEpsilon)
		{
			groundPos.x = m_pos.x;
			groundPos.y = m_pos.y;
			groundPos.z = currentFloorZ;
		}
		else
			groundPos = m_pos;
// - MADE BY EDWARD END!!!!!!!!!!!!!!!!!!!!
	}

	// For each direction, trace upwards from our best ground height to HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z to see if we have standing room.
	for (unsigned char corner (NORTH_WEST); corner < NUM_CORNERS; ++corner)
	{
#if defined DEBUG_NAV_NODES
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch_dir)->GetValue <unsigned char> () != NUM_CORNERS && i != Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch_dir)->GetValue <unsigned char> ())
			continue;
#endif	// if defined DEBUG_NAV_NODES

		const Math::Vector3D &destination (groundPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z - 0.2f));

		start = groundPos;

		start.z += Math::RawEpsilon;
/*
		// Build a mins/maxs pair for the HumanWidth x HalfLifeEngine::SDK::Constants::HalfHumanWidth box facing the appropriate direction
		mins = Math::Vector2D::ZeroValue;
		maxs = GetCornerVector2D (static_cast <NavCornerType_t> (corner)) * HalfLifeEngine::SDK::Constants::HalfHumanWidth;

		// now make sure that mins is smaller than maxs
		for (unsigned char axisIndex (Math::Vector2D::AxisID_X); axisIndex < Math::Vector2D::AxisID_Total; ++axisIndex)
			if (mins[axisIndex] > maxs[axisIndex])
				SwapElements (mins[axisIndex], maxs[axisIndex]);
*/
//		UTIL_TraceHull (start, destination, mins, maxs, MASK_PLAYERSOLID_BRUSHONLY, &filter, &traceResult);
//		TraceHullThruWalkableEntities (start, destination, true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
		TraceLineThruWalkableEntities (start, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);
//		TraceHull (start, destination, mins, maxs, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

//		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (start, destination, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 255u);
//		AddLogEntry (true, LogLevel_Default, false, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s.", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle");
// - MADE BY EDWARD START!!!!!!!!!!!!!!!!!!!!
		if (traceResult.isStartSolid)
		{
			// set conditions....
			const Math::Vector3D &direction ((destination - start).Normalize ());	// 1 unit long
			Math::Vector3D point (start + direction);

			while (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (point) == HalfLifeEngine::SDK::Constants::Content_Solid)
				point += direction;

			// calculate time we left a solid, only valid if we started in solid
			groundPos.z += point.GetDistance (start) / destination.GetDistance (start) * HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z;
		}
//		groundPos.z += traceResult.fractionleftsolid * HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z;// - MADE BY EDWARD END!!!!!!!!!!!!!!!!!!!!

		for (const float maxHeight (groundPos.z + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z); traceResult.isStartSolid && groundPos.z <= maxHeight; ++groundPos.z)
		{
			// In case we didn't find a good ground pos above, we could start in the ground. Move us up some.
//			UTIL_TraceHull (groundPos + Math::Vector3D (0.0f, 0.0f, Math::RawEpsilon), groundPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z - 0.2f), mins, maxs, MASK_PLAYERSOLID_BRUSHONLY, &filter, &traceResult);
//			TraceHullThruWalkableEntities (groundPos + Math::Vector3D (0.0f, 0.0f, Math::RawEpsilon), groundPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z - 0.2f), true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
			TraceLineThruWalkableEntities (groundPos + Math::Vector3D (0.0f, 0.0f, Math::RawEpsilon), groundPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z - 0.2f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);
//			TraceHull (groundPos + Math::Vector3D (0.0f, 0.0f, Math::RawEpsilon), groundPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z - 0.2f), mins, maxs, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

//			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (groundPos + Math::Vector3D (0.0f, 0.0f, Math::RawEpsilon), groundPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z - 0.2f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 255u);
//			AddLogEntry (true, LogLevel_Default, false, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s.", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle");
		}

		if (traceResult.isStartSolid || traceResult.fraction < 1.0f)
		{
			SetAttributes (NAV_MESH_CROUCH);

			m_crouch[corner] = true;
		}
#if defined DEBUG_NAV_NODES
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_nodes)->GetValue <bool> () && (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch_dir)->GetValue <unsigned char> () == i || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch_dir)->GetValue <unsigned char> () == NUM_CORNERS))
		{
			maxs.z += HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z;

			if (traceResult.isStartSolid)
				NDebugOverlay::Box (groundPos, mins, maxs, 255, 0, 0, 10, 20.0f);
			else if (m_crouch[corner])
				NDebugOverlay::Box (groundPos, mins, maxs, 0, 0, 255, 10, 20.0f);
			else
				NDebugOverlay::Box (groundPos, mins, maxs, 0, 255, 0, 10, 10.0f);
		}
#endif	// if defined DEBUG_NAV_NODES
	}
#endif
#if 0
{/*! FROM NEW SOURCE ENGINE !*/
	Math::Vector3D mins, maxs;

	// For each direction, trace upwards from our best ground height to HalfLifeEngine::SDK::Constants::VEC_HULL_MAX.z to see if we have standing room.
	for (unsigned char corner (NORTH_WEST); corner < NUM_CORNERS; ++corner)
	{
		#if defined DEBUG_NAV_NODES
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch_dir)->GetValue <unsigned char> () != NUM_CORNERS && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch_dir)->GetValue <unsigned char> () != corner)
				continue;
		#endif	// if defined DEBUG_NAV_NODES

		const Math::Vector2D cornerDirection (GetCornerVector2D (static_cast <NavCornerType_t> (corner)));

		mins = Math::Vector3D::ZeroValue;
		maxs = Math::Vector3D::ZeroValue;

		// Build a mins/maxs pair for the HumanWidth x HalfLifeEngine::SDK::Constants::HalfHumanWidth box facing the appropriate direction
		if (cornerDirection.x > 0.0f)
			maxs.x = HalfLifeEngine::SDK::Constants::HalfHumanWidth;
		else if (cornerDirection.x < 0.0f)
			mins.x = -HalfLifeEngine::SDK::Constants::HalfHumanWidth;

		if (cornerDirection.y > 0.0f)
			maxs.y = HalfLifeEngine::SDK::Constants::HalfHumanWidth;
		else if (cornerDirection.y < 0.0f)
			mins.y = -HalfLifeEngine::SDK::Constants::HalfHumanWidth;

		maxs.z = HalfLifeEngine::SDK::Constants::HumanHeight;

		// now make sure that mins is smaller than maxs
		for (unsigned char axisIndex (Math::Vector2D::AxisID_X); axisIndex < Math::Vector2D::AxisID_Total; ++axisIndex)
			if (mins[axisIndex] > maxs[axisIndex])
				SwapElements (mins[axisIndex], maxs[axisIndex]);

		if (!TestForCrouchArea (corner, mins, maxs, m_maximumPossibleHeights[corner]))
		{
			SetAttributes (NAV_MESH_CROUCH);

			m_crouch2[corner] = true;
		}
	}
}
#endif	// if 0
}
#if 0
const float NavigationMesh::NavigationNode::GetGroundHeightAboveNode (const NavCornerType_t corner) const
{
	return m_maximumPossibleHeights[corner];
}
const float NavigationMesh::NavigationNode::GetGroundHeightAboveNode (void) const
{
	// Return the highest maximum possible height....

	float maximumPossibleHeight (m_maximumPossibleHeights[NORTH_WEST]);

	if (maximumPossibleHeight < m_maximumPossibleHeights[NORTH_EAST])
		maximumPossibleHeight = m_maximumPossibleHeights[NORTH_EAST];

	if (maximumPossibleHeight < m_maximumPossibleHeights[SOUTH_EAST])
		maximumPossibleHeight = m_maximumPossibleHeights[SOUTH_EAST];

	if (maximumPossibleHeight < m_maximumPossibleHeights[SOUTH_WEST])
		maximumPossibleHeight = m_maximumPossibleHeights[SOUTH_WEST];

	return maximumPossibleHeight;
}

const bool NavigationMesh::NavigationNode::TestForCrouchArea (const NavCornerType_t corner, const Math::Vector3D &mins, const Math::Vector3D &maxs, float &maximumPossibleHeight)
{
	CTraceFilterSimple traceFilter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT);
//	CTraceFilterWalkableEntities traceFilter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT, WALK_THRU_EVERYTHING);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	UTIL_TraceHull (m_pos, m_pos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::JumpCrouchHeight), GenerationHull, MASK_PLAYERSOLID_BRUSHONLY/*?*/, &traceFilter, &traceResult);

	const float deltaZ (traceResult.endPosition.z - m_pos.z);

	if (deltaZ < 0.0f)
	{
		Label_ReturnFalse:
		{
			maximumPossibleHeight = HalfLifeEngine::SDK::Constants::JumpCrouchHeight;

			m_crouch[corner] = true;

			return false;
		}
	}

	Math::Vector3D from (m_pos);

	for (float offsetZ (0.0f); /* Empty */; ++from.z)
	{
		UTIL_TraceHull (from, from, mins, Math::Vector3D (maxs.x, maxs.y, HalfLifeEngine::SDK::Constants::DuckHumanHeight/*?*/), MASK_PLAYERSOLID_BRUSHONLY/*?*/, &traceFilter, &traceResult);

		if (!traceResult.isStartSolid)
			break;

		if (deltaZ < ++offsetZ)
			goto Label_ReturnFalse;
	}

	maximumPossibleHeight = from.z - m_pos.z;

	UTIL_TraceHull (from, from, mins, Math::Vector3D (maxs.x, maxs.y, HalfLifeEngine::SDK::Constants::HumanHeight), MASK_PLAYERSOLID_BRUSHONLY/*?*/, &traceFilter, &traceResult);

	if (traceResult.isStartSolid)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch)->GetValue <unsigned char> () == m_id)
			NDebugOverlay::Box (groundPos, mins, maxs, 255, 0, 0, 100, 100.0f);

		return false;
	}

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_test_node_crouch)->GetValue <unsigned char> () == m_id)
		NDebugOverlay::Box (groundPos, mins, maxs, 0, 255, 255, 100, 100.0f);

	return true;
}
#endif	// if 0
//--------------------------------------------------------------------------------------------------------------
/*** Return node at given position.* @todo Need a hash table to make this lookup fast. - EDWARD: You welcome! ;P*/
NavigationMesh::NavigationNode *const NavigationMesh::GetNode (const Math::Vector3D &pos) const
{
	const float tolerance (0.45f * GenerationStepSize);	// 1.0
/*
	for (const NavNodeStack::NodeType_t *node (m_nodes.GetFirst ()); node != NULL; node = node->GetNext ())
	{
		NavigationNode *const navigationNode (node->GetData ());

//		if (Math::AreEqual (navigationNode->m_pos.x, pos.x, tolerance) && Math::AreEqual (navigationNode->m_pos.y, pos.y, tolerance) && Math::AreEqual (navigationNode->m_pos.z, pos.z, tolerance))
		if (navigationNode->m_pos.GetDistanceSquared (pos) < MATH_GET_SQUARED (tolerance))	// MADE BY EDWARD!!!!
			return navigationNode;
	}
*/
/*	NavigationNode *result (NULL);
	const double startTime1 (g_commonTimer.GetMicroseconds ());

	for (const NavNodeStack::NodeType_t *node (m_nodes.GetFirst ()); node != NULL; node = node->GetNext ())
	{
		NavigationNode *const navigationNode (node->GetData ());

//		if (Math::AreEqual (navigationNode->m_pos.x, pos.x, tolerance) && Math::AreEqual (navigationNode->m_pos.y, pos.y, tolerance) && Math::AreEqual (navigationNode->m_pos.z, pos.z, tolerance))
		if (navigationNode->m_pos.GetDistanceSquared (pos) < MATH_GET_SQUARED (tolerance))	// MADE BY EDWARD!!!!
		{
			result = navigationNode;

			break;
		}
	}

	const double startTime2 (g_commonTimer.GetMicroseconds ());
	const double endTime1 (startTime2 - startTime1);
*/
	const unsigned short gridIndex (WorldToNodesGridX (pos.x) + WorldToNodesGridY (pos.y) * m_nodesGridSize.x + WorldToNodesGridZ (pos.z) * m_nodesGridSize.y);

	// Reliability check.
	InternalAssert (gridIndex < m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z);

	// get list in cell that contains position
	const NavigationNodesGridArray &array (m_nodesGrid[gridIndex]);

	for (NavigationNodesGridArray::IndexType_t index (0u); index < array.GetElementNumber (); ++index)
	{
		NavigationNode *const navigationNode (array[index]);

//		if (Math::AreEqual (navigationNode->m_pos.x, pos.x, tolerance) && Math::AreEqual (navigationNode->m_pos.y, pos.y, tolerance) && Math::AreEqual (navigationNode->m_pos.z, pos.z, tolerance))
		if (navigationNode->m_pos.GetDistanceSquared (pos) < MATH_GET_SQUARED (tolerance))	// MADE BY EDWARD!!!!
		{
/*			const double endTime2 (g_commonTimer.GetMicroseconds () - startTime2);

			if (endTime1 < endTime2)
				AddLogEntry (false, LogLevel_Default, true, "BAD: endTime1=%f, endTime2=%f.", endTime1, endTime2);
			else if (endTime1 > endTime2)
				AddLogEntry (false, LogLevel_Default, true, "GOOD: endTime1=%f, endTime2=%f.", endTime1, endTime2);

			InternalAssert (result == navigationNode);
*/
			return navigationNode;
		}
	}
/*
	const double endTime2 (g_commonTimer.GetMicroseconds () - startTime2);

	if (endTime1 < endTime2)
		AddLogEntry (false, LogLevel_Default, true, "BAD: endTime1=%f, endTime2=%f.", endTime1, endTime2);
	else if (endTime1 > endTime2)
		AddLogEntry (false, LogLevel_Default, true, "GOOD: endTime1=%f, endTime2=%f.", endTime1, endTime2);

	InternalAssert (result == NULL);
*/
	return NULL;
}