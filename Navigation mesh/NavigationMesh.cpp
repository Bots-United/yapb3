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
// NavigationMesh.hpp
//
// Class: NavigationMesh
//
// Description: Implementation of Navigation Mesh interface.
//				The NavigationMesh is the global interface to the Navigation Mesh.
//
// Version: $ID:$
//

#include <Core.hpp>

const DynamicString NavigationMesh::FileExtention ("NAV");

const float NavigationMesh::GenerationStepSize (25.0f);	///< (30) was 20, but bots can't fit always fit.
const Math::Vector3D NavigationMesh::GenerationHull (0.45f, 0.45f, HalfLifeEngine::SDK::Constants::HalfDuckHumanHeight);	// 0.90 x 0.90 x DuckHumanHeight
const float NavigationMesh::BotRadius (10.0f);	///< circular extent that contains bot.
const unsigned short NavigationMesh::NodesGridCellSize (static_cast <const unsigned char> (GenerationStepSize) * MaximumNavigationNodesPerCell);	///< the width/height of a grid cell for spatially partitioning navigation nodes for fast access.

const DynamicString NavigationMesh::NavAttributeStrings[NavAttributeType_Total] =
{
	"CROUCH",		///< must crouch to use this node/area
	"JUMP",			///< must jump to traverse this area
	"DOUBLE JUMP",	///< must jump from head of some player to traverse this area
	"PRECISE",		///< do not adjust for obstacles, just move along area
	"NO JUMP",		///< inhibit discontinuity jumping
	"STOP",			///< must stop when entering this area
	"RUN",			///< must run to traverse this area
	"WALK",			///< must walk to traverse this area
	"AVOID",		///< avoid this area unless alternatives are too dangerous
	"TRANSIENT",	///< area may become blocked, and should be periodically checked
	"DONT HIDE",	///< area should not be considered for hiding spot generation
	"STAND",		///< bots hiding in this area should stand
	"NO HOSTAGES",	///< hostages shouldn't use this area
	"STAIRS",
	"UNMERGEABLE",
	"OBSTACLE",
	"CLIFF"
};
/*
ConVar nav_edit ("nav_edit", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Set to one to interactively edit the Navigation Mesh. Set to zero to leave edit mode.");
ConVar nav_quicksave ("nav_quicksave", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Set to one to skip the time consuming phases of the analysis. Useful for data collection and testing.");
ConVar nav_show_approach_points ("nav_show_approach_points", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Show Approach Points in the Navigation Mesh.");
ConVar nav_show_danger ("nav_show_danger", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Show current 'danger' levels.");
ConVar nav_show_player_counts ("nav_show_player_counts", "0", FCVAR_GAMEDLL | FCVAR_CHEAT, "Show current player counts in each area.");
*/
#if 0
//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::IsNodeOverlapped (Math::Vector3D source, Math::Vector3D offset) const
{
	if (GetNavArea (source + offset, HalfLifeEngine::SDK::Constants::HumanHeight) != NULL)
		return true;

	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	CTraceFilterWalkableEntities traceFilter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT, WALK_THRU_EVERYTHING);

	offset.z += HalfLifeEngine::SDK::Constants::HalfHumanHeight;

	source.x *= GenerationStepSize;
	source.x += offset.x;

	source.y *= GenerationStepSize;
	source.y += offset.y;

	source.z = offset.z;

	UTIL_TraceHull (offset, source, -0.5f, 0.5f, GetGenerationTraceMask (), &traceFilter, &traceResult);

	if (traceResult.isStartSolid || traceResult.isAllSolid || traceResult.fraction < 0.1f)
		return true;

	UTIL_TraceHull (traceResult.endPosition, source, -0.5f, 0.5f, GetGenerationTraceMask (), &traceFilter, &traceResult);

	return traceResult.isStartSolid || traceResult.isAllSolid || traceResult.fraction == 1.0f || traceResult.planeNormal.z < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ();
}
//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::StayOnFloor (HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult, const float distance)
{
	CTraceFilterSimple traceFilter (NULL, COLLISION_GROUP_NONE);
//	CTraceFilterWalkableEntities traceFilter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT, WALK_THRU_EVERYTHING);

	UTIL_TraceHull (traceResult.endPosition, traceResult.endPosition - Math::Vector3D (0.0f, 0.0f, distance), GenerationHull, GetGenerationTraceMask (), &traceFilter, &traceResult);

	return !traceResult.isStartSolid && traceResult.fraction < 1.0f && traceResult.planeNormal.z >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ();
}

const bool NavigationMesh::TraceAdjacentNode (const unsigned int callsNumber, const Math::Vector3D &source, const Math::Vector3D &destination, HalfLifeEngine::SDK::Structures::TraceResult_t &traceResult, const float distance)
{
	CTraceFilterSimple traceFilter (NULL, COLLISION_GROUP_NONE);
//	CTraceFilterWalkableEntities traceFilter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT, WALK_THRU_EVERYTHING);

	UTIL_TraceHull (source, destination, GenerationHull, GetGenerationTraceMask (), &traceFilter, &traceResult);

	if (traceResult.isStartSolid)
		return false;

	if (traceResult.endPosition.x == destination.x && traceResult.endPosition.y == destination.y)
		return StayOnFloor (traceResult, distance);

	if ((callsNumber > 0u && source.GetDistanceSquared2D (traceResult.endPosition) < 1.0f) || !StayOnFloor (traceResult, distance))
		return false;

	UTIL_TraceHull (traceResult.endPosition, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::StepHeight), GenerationHull, GetGenerationTraceMask (), &traceFilter, &traceResult);

	return TraceAdjacentNode (callsNumber + 1u, traceResult.endPosition, Math::Vector3D (destination.x, destination.y, traceResult.endPosition.z), traceResult, HalfLifeEngine::SDK::Constants::DeathDrop/* MAY BE 'distance' ??? */);
}

const bool NavigationMesh::TestStitchConnection (const Math::Vector3D &source, const Math::Vector3D &destination)
{
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	if (TraceAdjacentNode (0u, destination, source/*RIGHT?? - OR 'source, destination'*/, traceResult, HalfLifeEngine::SDK::Constants::DeathDrop))
		return true;

	/// @todo ...
}
#endif	// if 0
//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::CheckProgressBarResourceFile (void)
{
	if (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModID () == HalfLifeEngine::Engine::Version_ConditionZero)
		return;	// Condition Zero should already has this file....

	const DynamicString filename (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Resource/UI/CSProgressBar.res");
	STDIOFile progressBarResource (filename, "rt");

	if (progressBarResource.IsValid ())
		return;	// All fine....

	progressBarResource.Open (filename, "wt");

	if (!progressBarResource.IsValid ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Unable to create file \"%s\"!", filename.GetData ());

		return;
	}

	// Fill the file!
	progressBarResource.Print
	(
		"\"Resource/UI/CSProgressBar.res\"\n"
		"{\n"
		"\t\"CSProgressBar\"\n"
		"\t{\n"
		"\t\t\"ControlName\"\t\t\t\"Frame\"\n"
		"\t\t\"fieldName\"\t\t\t\t\"CSProgressBar\"\n"
		"\t\t\"xpos\"\t\t\t\t\t\"200\"\n"
		"\t\t\"ypos\"\t\t\t\t\t\"215\"\n"
		"\t\t\"wide\"\t\t\t\t\t\"240\"\n"
		"\t\t\"tall\"\t\t\t\t\t\"40\"\n"
		"\t\t\"autoResize\"\t\t\t\"0\"\n"
		"\t\t\"pinCorner\"\t\t\t\t\"0\"\n"
		"\t\t\"visible\"\t\t\t\t\"1\"\n"
		"\t\t\"enabled\"\t\t\t\t\"1\"\n"
		"\t\t\"tabPosition\"\t\t\t\"0\"\n"
		"\t\t\"settitlebarvisible\"\t\"0\"\n"
		"\t}\n"
		"\t\"RoundedCornerBackground\"\n"
		"\t{\n"
		"\t\t\"ControlName\"\t\t\t\"Panel\"\n"
		"\t\t\"fieldName\"\t\t\t\t\"RoundedCornerBackground\"\n"
		"\t\t\"xpos\"\t\t\t\t\t\"0\"\n"
		"\t\t\"ypos\"\t\t\t\t\t\"0\"\n"
		"\t\t\"wide\"\t\t\t\t\t\"240\"\n"
		"\t\t\"tall\"\t\t\t\t\t\"40\"\n"
		"\t\t\"autoResize\"\t\t\t\"0\"\n"
		"\t\t\"pinCorner\"\t\t\t\t\"2\"\n"
		"\t\t\"visible\"\t\t\t\t\"1\"\n"
		"\t\t\"enabled\"\t\t\t\t\"1\"\n"
		"\t\t\"tabPosition\"\t\t\t\"0\"\n"
		"\t\t\"settitlebarvisible\"\t\"0\"\n"
		"\t\t\"zpos\"\t\t\t\t\t\"0\"\n"
		"\t}\n"
		"\t\"TitleLabel\"\n"
		"\t{\n"
		"\t\t\"ControlName\"\t\"Label\"\n"
		"\t\t\"fieldName\"\t\t\"TitleLabel\"\n"
		"\t\t\"xpos\"\t\t\t\"5\"\n"
		"\t\t\"ypos\"\t\t\t\"5\"\n"
		"\t\t\"wide\"\t\t\t\"80\"\n"
		"\t\t\"tall\"\t\t\t\"15\"\n"
		"\t\t\"autoResize\"\t\"0\"\n"
		"\t\t\"pinCorner\"\t\t\"2\"\n"
		"\t\t\"visible\"\t\t\"0\"\n"
		"\t\t\"enabled\"\t\t\"1\"\n"
		"\t\t\"tabPosition\"\t\"0\"\n"
		"\t\t\"labelText\"\t\t\"\"\n"
		"\t\t\"textAlignment\"\t\"north-west\"\n"
		"\t\t\"dulltext\"\t\t\"0\"\n"
		"\t\t\"brighttext\"\t\"0\"\n"
		"\t\t\"zpos\"\t\t\t\"1\"\n"
		"\t}\n"
		"\t\"TextLabel\"\n"
		"\t{\n"
		"\t\t\"ControlName\"\t\"Label\"\n"
		"\t\t\"fieldName\"\t\t\"TextLabel\"\n"
		"\t\t\"xpos\"\t\t\t\"5\"\n"
		"\t\t\"ypos\"\t\t\t\"5\"\n"
		"\t\t\"wide\"\t\t\t\"230\"\n"
		"\t\t\"tall\"\t\t\t\"15\"\n"
		"\t\t\"autoResize\"\t\"0\"\n"
		"\t\t\"pinCorner\"\t\t\"2\"\n"
		"\t\t\"visible\"\t\t\"1\"\n"
		"\t\t\"enabled\"\t\t\"1\"\n"
		"\t\t\"tabPosition\"\t\"0\"\n"
		"\t\t\"labelText\"\t\t\"\"\n"
		"\t\t\"textAlignment\"\t\"north-west\"\n"
		"\t\t\"dulltext\"\t\t\"0\"\n"
		"\t\t\"brighttext\"\t\"0\"\n"
		"\t\t\"zpos\"\t\t\t\"1\"\n"
		"\t}\n"
		"\t\"ProgressBarSizer\"\n"
		"\t{\n"
		"\t\t\"ControlName\"\t\"Panel\"\n"
		"\t\t\"fieldName\"\t\t\"ProgressBarSizer\"\n"
		"\t\t\"xpos\"\t\t\t\"9\"\n"
		"\t\t\"ypos\"\t\t\t\"24\"\n"
		"\t\t\"wide\"\t\t\t\"222\"\n"
		"\t\t\"tall\"\t\t\t\"7\"\n"
		"\t\t\"autoResize\"\t\"0\"\n"
		"\t\t\"pinCorner\"\t\t\"2\"\n"
		"\t\t\"visible\"\t\t\"1\"\n"
		"\t\t\"enabled\"\t\t\"1\"\n"
		"\t\t\"tabPosition\"\t\"0\"\n"
		"\t\t\"zpos\"\t\t\t\"1\"\n"
		"\t}\n"
		"\t\"ProgressBar\"\n"
		"\t{\n"
		"\t\t\"ControlName\"\t\"Panel\"\n"
		"\t\t\"fieldName\"\t\t\"ProgressBar\"\n"
		"\t\t\"xpos\"\t\t\t\"9\"\n"
		"\t\t\"ypos\"\t\t\t\"24\"\n"
		"\t\t\"wide\"\t\t\t\"222\"\n"
		"\t\t\"tall\"\t\t\t\"7\"\n"
		"\t\t\"autoResize\"\t\"0\"\n"
		"\t\t\"pinCorner\"\t\t\"2\"\n"
		"\t\t\"visible\"\t\t\"1\"\n"
		"\t\t\"enabled\"\t\t\"1\"\n"
		"\t\t\"tabPosition\"\t\"0\"\n"
		"\t\t\"zpos\"\t\t\t\"1\"\n"
		"\t}\n"
		"\t\"ProgressBarBorder\"\n"
		"\t{\n"
		"\t\t\"ControlName\"\t\"Panel\"\n"
		"\t\t\"fieldName\"\t\t\"ProgressBarBorder\"\n"
		"\t\t\"xpos\"\t\t\t\"7\"\n"
		"\t\t\"ypos\"\t\t\t\"22\"\n"
		"\t\t\"wide\"\t\t\t\"226\"\n"
		"\t\t\"tall\"\t\t\t\"10\"\n"
		"\t\t\"autoResize\"\t\"0\"\n"
		"\t\t\"pinCorner\"\t\t\"2\"\n"
		"\t\t\"visible\"\t\t\"1\"\n"
		"\t\t\"enabled\"\t\t\"1\"\n"
		"\t\t\"tabPosition\"\t\"0\"\n"
		"\t\t\"zpos\"\t\t\t\"2\"\n"
		"\t}\n"
		"}"
	);
}

//--------------------------------------------------------------------------------------------------------------
/*** Reset the Navigation Mesh to initial values*/
void NavigationMesh::Reset (void)
{
	DestroyNavigationMesh ();

	m_generationMode = GENERATE_NONE;
	m_currentNode = NULL;

	m_isFromCurrentMap = true;
	m_isEditing = false;
	m_isNavigationMeshChanged = false;

	SetNavPlace (UNDEFINED_PLACE);

	m_markedArea = m_selectedArea = NULL;

	m_lastSelectedArea = NULL;
	m_navEditMode = NAV_EDIT_NORMAL;
	m_isPlacePainting = false;

	m_climbableSurface = false;
	m_markedLadder = m_selectedLadder = NULL;

	SetPlayersSpawnPointsTeam (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);	// set the default value

	m_walkableSeeds.RemoveAll ();
}


//--------------------------------------------------------------------------------------------------------------
/*** Free all resources of the mesh and reset it to empty state*/
void NavigationMesh::DestroyNavigationMesh (const bool incremental)
{
	if (!incremental)
	{
		// We must reset 'm_lastNavigationArea' pointer for all clients!
		g_server->GetClientManager ()->OnDestroyNavigationMesh ();

		m_areas.DeleteAndPurgeAll ();	// destroy all areas
		m_ladders.DeleteAndPurgeAll ();	// destroy ladder representations
	}
	else for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->ResetNodes ();

	// destroy navigation nodes created during map generation
	{
		m_nodes.DeleteAndPurgeAll ();

		m_nodesGridSize.Initialize ();

		delete [] m_nodesGrid;

		m_nodesGrid = new NavigationNodesGridArray[m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z];
	}

	NavigationNode::m_nextID = 0u;	// Reset the next node ID to 0

	// destroy all hiding spots
	DestroyHidingSpots ();

	if (!incremental)
	{
		// destroy the grid
		delete [] m_grid;

		m_grid = NULL;

		m_gridSize.x = m_gridSize.y = 0u;
	}
/*
	// clear the hash table
	for (unsigned short index (0u); index < HASH_TABLE_SIZE; ++index)
		m_hashTable[index] = NULL;
*/
	if (!incremental)
	{
		// Reset the next area and ladder IDs to 0
		CompressAreasIDs ();
		CompressLaddersIDs ();

		m_loadState = LoadState_None;
	}

	m_markedArea = m_selectedArea = m_lastSelectedArea = NULL;
	m_navEditMode = NAV_EDIT_NORMAL;
	m_climbableSurface = false;
	m_markedLadder = m_selectedLadder = NULL;
}


//--------------------------------------------------------------------------------------------------------------
/*** Invoked on each game frame*/
void NavigationMesh::Think (void)
{
//	VPROF ("NavigationMesh::Think");

	if (IsGenerating ())
	{
		UpdateGeneration (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_NavigationMesh_MaximumSampleStepTime)->GetValue <float> ());

		return;	// don't bother trying to draw stuff while we're generating
	}

	if (!g_server->GetHumanManager ()->IsHostClientInServer ())
		return;	// this function is only valid on listen server.

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->GetValue <bool> ())
	{
		if (!m_isEditing)
		{
			OnEditModeStart ();

			m_isEditing = true;
		}

		DrawEditMode ();
	}
	else if (m_isEditing)
	{
		OnEditModeEnd ();

		m_isEditing = false;
	}

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_danger)->GetValue <bool> ())
		DrawDanger ();

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_player_counts)->GetValue <bool> ())
		DrawPlayerCounts ();

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_occupy_time)->GetValue <bool> ())
		DrawOccupyTime ();

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_battlefront)->GetValue <bool> ())
		DrawBattlefront ();

	const unsigned char height (50u), width (25u);

	// draw any walkable seeds that have been marked
	for (unsigned short index (0u); index < m_walkableSeeds.GetElementNumber (); ++index)
	{
		const WalkableSeedSpot_t &spot (m_walkableSeeds[index]);

//		NDebugOverlay::Line (spot.pos, spot.pos + height * spot.normal, 255, 0, 255, true, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (spot.pos, spot.pos + height * spot.normal, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 255u), 255u, 0u, 1u);
//		NDebugOverlay::Line (spot.pos + Math::Vector2D (width, 0.0f), spot.pos + height * spot.normal, 255, 0, 255, true, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (spot.pos + Math::Vector2D (width, 0.0f), spot.pos + height * spot.normal, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 255u), 255u, 0u, 1u);
//		NDebugOverlay::Line (spot.pos - Math::Vector2D (width, 0.0f), spot.pos + height * spot.normal, 255, 0, 255, true, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (spot.pos - Math::Vector2D (width, 0.0f), spot.pos + height * spot.normal, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 255u), 255u, 0u, 1u);
//		NDebugOverlay::Line (spot.pos + Math::Vector2D (0.0f, width), spot.pos + height * spot.normal, 255, 0, 255, true, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (spot.pos + Math::Vector2D (0.0f, width), spot.pos + height * spot.normal, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 255u), 255u, 0u, 1u);
//		NDebugOverlay::Line (spot.pos - Math::Vector2D (0.0f, width), spot.pos + height * spot.normal, 255, 0, 255, true, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (spot.pos - Math::Vector2D (0.0f, width), spot.pos + height * spot.normal, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 255u), 255u, 0u, 1u);
	}
}


//--------------------------------------------------------------------------------------------------------------
/*** Allocate the grid and define its extents*/
void NavigationMesh::AllocateGrid (const Math::Extent2D &extent)
{
	delete [] m_grid;	// destroy the old grid

	m_mins = extent.mins;
	m_gridSize.x = static_cast <unsigned char> ((extent.maxs.x - extent.mins.x) / GridCellSize) + 1u;
	m_gridSize.y = static_cast <unsigned char> ((extent.maxs.y - extent.mins.y) / GridCellSize) + 1u;

	m_grid = new NavAreaArray[m_gridSize.x * m_gridSize.y];

	#if defined _DEBUG
		// Reliability check.
		if (m_grid == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation areas grid!", sizeof (NavAreaArray) * m_gridSize.x * m_gridSize.y);
	#endif	// if defined _DEBUG
}

const bool NavigationMesh::AllocateGrid (void)
{
	AddLogEntry (true, LogLevel_Default, false, "Allocating navigation areas grid...");

	if (m_areas.IsEmpty ())
	{
		// If we somehow have no areas, don't try to create an impossibly-large grid
		AllocateGrid (0.0f);

		AddLogEntry (true, LogLevel_Default, false, "Allocating navigation areas grid...DONE!");

		return false;
	}

	Math::Extent2D totalExtent (m_worldMap.GetBSPFile ().models[0u].boundingBox/*9999999999.9f, -9999999999.9f*/);

	// compute total extent
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		const Math::Extent3D &areaExtent (m_areas[index]->GetExtent ());

		totalExtent.mins.SetMinimum (areaExtent.mins);
		totalExtent.maxs.SetMaximum (areaExtent.maxs);
	}

	AllocateGrid (totalExtent);

	// add the areas to the navigation mesh
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		AddArea (m_areas[index]);

	AddLogEntry (true, LogLevel_Default, false, "Allocating navigation areas grid...DONE!");

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/*** Add an area to the mesh*/
void NavigationMesh::AddAreaToGrid (NavigationArea *const area)
{
	// Reliability check.
	if (m_grid == NULL)
	{
		// If we somehow have no grid (manually creating a navigation area without loading or generating a mesh), don't crash
		AllocateGrid (0.0f);
	}

	// add to grid
	const Math::Extent3D &extent (area->GetExtent ());
	const unsigned char loX (WorldToGridX (extent.mins.x));
	const unsigned char hiX (WorldToGridX (extent.maxs.x));
	const unsigned char hiY (WorldToGridY (extent.maxs.y));

	for (unsigned char loY (WorldToGridY (extent.mins.y)), x; loY <= hiY; ++loY)
		for (x = loX; x <= hiX; ++x)
		{
			InternalAssert (!m_grid[x + loY * m_gridSize.x].IsMember (area));

			m_grid[x + loY * m_gridSize.x] += area;
		}
}
/*
void NavigationMesh::AddAreaToHash (NavigationArea *const area)
{
	// add to hash table

	const unsigned char key (ComputeHashKey (area->GetID ()));

	if (m_hashTable[key] != NULL)
	{
		// add to head of list in this slot
		area->m_prevHash = NULL;
		area->m_nextHash = m_hashTable[key];
		m_hashTable[key]->m_prevHash = m_hashTable[key] = area;
	}
	else
	{
		// first entry in this slot
		m_hashTable[key] = area;
		area->m_nextHash = area->m_prevHash = NULL;
	}
}
*/
//--------------------------------------------------------------------------------------------------------------
/*** Remove an area from the mesh*/
void NavigationMesh::RemoveAreaFromGrid (NavigationArea *const area)
{
	// remove from grid
	const Math::Extent3D &extent (area->GetExtent ());
	const unsigned char loX (WorldToGridX (extent.mins.x));
	const unsigned char hiX (WorldToGridX (extent.maxs.x));
	const unsigned char hiY (WorldToGridY (extent.maxs.y));

	for (unsigned char loY (WorldToGridY (extent.mins.y)), x; loY <= hiY; ++loY)
		for (x = loX; x <= hiX; ++x)
			m_grid[x + loY * m_gridSize.x] -= area;
}
/*
void NavigationMesh::RemoveAreaFromHash (NavigationArea *const area)
{
	// remove from hash table

	if (area->m_prevHash != NULL)
		area->m_prevHash->m_nextHash = area->m_nextHash;
	else
	{
		const unsigned char key (ComputeHashKey (area->GetID ()));

		// area was at start of list
		m_hashTable[key] = area->m_nextHash;

		if (m_hashTable[key] != NULL)
			m_hashTable[key]->m_prevHash = NULL;
	}

	if (area->m_nextHash != NULL)
		area->m_nextHash->m_prevHash = area->m_prevHash;
}
*/
//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::BuildTransientAreaList (void)
{
	m_transientAreas.RemoveAll ();

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const area (m_areas[index]);

		if (area->HasAttributes (NAV_MESH_TRANSIENT))
			m_transientAreas += area;
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Given a position, return the navigation area that IsOverlapping() and is *immediately* beneath it*/
NavigationMesh::NavigationArea *const NavigationMesh::GetNavArea (const Math::Vector3D &pos, const float beneathLimit/* = 120.0f*/) const
{
//	VPROF ("NavigationMesh::GetNavArea");

	// Reliability check.
	if (m_grid == NULL)
		return NULL;

	const NavAreaArray &array (m_grid[WorldToGridX (pos.x) + WorldToGridY (pos.y) * m_gridSize.x]);	// get list in cell that contains position
	NavigationArea *use (NULL);
	float useZ (-99999999.9f);
	const float testPos (pos.z + 5.0f);

	// search cell list to find correct area
	for (NavAreaArray::IndexType_t index (0u); index < array.GetElementNumber (); ++index)
	{
		NavigationArea *const area (array[index]);

		// check if position is within 2D boundaries of this area
		if (!area->IsOverlapping (pos))
			continue;

		// project position onto area to get Z
		const float z (area->GetZ (pos));

		// if area is above us/too far below us, skip it
		if (z > testPos || z < pos.z - beneathLimit)
			continue;

		// if area is higher than the one we have, use this instead
		if (z > useZ)
		{
			use = area;
			useZ = z;
		}
	}

	return use;
}

//--------------------------------------------------------------------------------------------------------------
/*** Given a position in the world, return the navigation area that is closest* and at the same height, or beneath it.* Used to find initial area if we start off of the mesh.*/
NavigationMesh::NavigationArea *const NavigationMesh::GetNearestNavArea (Math::Vector3D pos/*, const bool anyZ/* = false*/, const float maxDist/* = 10000.0f*/, const bool checkLOS/* = false*/, const bool UNKNOWN1/* = true*/, const char/* UNKNOWN2*//* = -2*/) const
{
//	VPROF ("NavigationMesh::GetNearestNavArea");

	// Reliability check.
	if (m_grid == NULL)
		return NULL;

	NavigationArea *close (GetNavArea (pos));

	// quick check
	if (!checkLOS && !UNKNOWN1 && close != NULL)
		return close;

	// ensure source position is well behaved
	if (GetGroundHeight (pos, &pos.z) == false && UNKNOWN1)
		return NULL;

	pos.z += HalfLifeEngine::SDK::Constants::HalfHumanHeight;

	// find closest navigation area
	NavigationArea::MakeNewMarker ();

	// get list in cell that contains position
	const unsigned char originX (WorldToGridX (pos.x));
	const unsigned char originY (WorldToGridY (pos.y));

	float closeDistSq (MATH_GET_SQUARED (maxDist));

	//
	// Search in increasing rings out from origin, starting with cell
	// that contains the given position.
	// Once we find a close area, we must check one more step out in
	// case our position is just against the edge of the cell boundary
	// and an area in an adjacent cell is actually closer.
	//
	for (short shift (0u), shiftLimit (static_cast <unsigned char> (maxDist / GridCellSize)), x, y; shift <= shiftLimit; ++shift)
		for (x = originX - shift; x <= originX + shift; ++x)
		{
			if (x < 0 || x >= m_gridSize.x)
				continue;

			for (y = originY - shift; y <= originY + shift; ++y)
			{
				if (y < 0 || y >= m_gridSize.y)
					continue;

				// only check these areas if we're on the outer edge of our spiral
				if (x > originX - shift && x < originX + shift && y > originY - shift && y < originY + shift)
					continue;

				const NavAreaArray &array (m_grid[x + y * m_gridSize.x]);

				// find closest area in this cell
				for (NavAreaArray::IndexType_t index (0u); index < array.GetElementNumber (); ++index)
				{
					NavigationArea *const area (array[index]);

					// skip if we've already visited this area
					if (area->IsMarked ())
						continue;

					area->Mark ();

					const Math::Vector3D &areaPos (area->GetClosestPointOnArea (pos));
					const float distSq (areaPos.GetDistanceSquared (pos));

					// keep the closest area
					if (distSq < closeDistSq)
					{
						// check LOS to area
						// REMOVED: If we do this for !anyZ, it's likely we wont have LOS and will enumerate every area in the mesh
						// It is still good to do this in some isolated cases, however
						if (checkLOS)
						{
							HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//							UTIL_TraceLine (pos, areaPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//							HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (pos, areaPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);
							TraceLineThruWalkableEntities (pos, areaPos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

							if (traceResult.fraction < 1.0f)
								continue;
						}

						closeDistSq = distSq;
						close = area;

						// look one more step outwards
						shiftLimit = shift + 1u;
					}
				}
			}
		}

	return close;
}

//--------------------------------------------------------------------------------------------------------------
/*** Given an ID, return the associated area*/
NavigationMesh::NavigationArea *const NavigationMesh::GetNavAreaByID (const BaseNavigationArea::ID_t id) const
{
	// Reliability check.
	if (id == 0u)
		return NULL;

//	for (NavigationArea *area (m_hashTable[ComputeHashKey (id)]); area != NULL; area = area->m_nextHash)
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const area (m_areas[index]);

		if (area->GetID () == id)
			return area;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
/*** Given an ID, return the associated ladder*/
NavigationMesh::NavigationLadder *const NavigationMesh::GetLadderByID (const BaseNavigationArea::ID_t id) const
{
	// Reliability check.
	if (id == 0u)
		return NULL;

	for (NavLadderArray::IndexType_t index (0u); index < m_ladders.GetElementNumber (); ++index)
	{
		NavigationLadder *const ladder (m_ladders[index]);

		if (ladder->GetID () == id)
			return ladder;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
/*** Given the first part of a place name, return a place ID or zero if no place is defined, or the partial match is ambiguous*/
const NavigationMesh::Place_t NavigationMesh::PlaceContainer::PartialNameToPlace (const DynamicString &name) const
{
	if (name.IsEmpty ())
		return UNDEFINED_PLACE;

	Place_t found (UNDEFINED_PLACE);
	bool isAmbiguous (false);

	for (PlacesArray::IndexType_t index (0u); index < m_places.GetElementNumber (); ++index)
	{
		const DynamicString &place (m_places[index]);

		if (place.CompareWithoutCaseCheck (name, name.GetElementNumber ()) != 0)
			continue;

		// check for exact match in case of subsets of other strings
		if (place.CompareWithoutCaseCheck (name) == 0)
		{
			found = NameToPlace (place);
			isAmbiguous = false;

			break;
		}

		if (found != UNDEFINED_PLACE)
			isAmbiguous = true;
		else
			found = NameToPlace (place);
	}

	return isAmbiguous ? UNDEFINED_PLACE : found;
}


//--------------------------------------------------------------------------------------------------------------
/*** Given a partial place name, fill in possible place names for 'ConCommand' autocomplete*/
int NavigationMesh::PlaceContainer::PlaceNameAutocomplete (const char *partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
{
	unsigned char numMatches (0u);

	partial += GET_STATIC_STRING_LENGTH ("nav_use_place ");

	const unsigned short partialLength (GetStringLength <unsigned short> (partial));

	for (PlacesArray::IndexType_t index (0u); index < m_places.GetElementNumber (); ++index)
	{
		if (m_places[index].CompareWithoutCaseCheck (partial, partialLength) != 0)
			continue;

		// Add the place name to the autocomplete array
		_snprintf (commands[numMatches], COMMAND_COMPLETION_ITEM_LENGTH, "nav_use_place %s", m_places[index].GetData ());

		// Make sure we don't try to return too many place names
		if (++numMatches == COMMAND_COMPLETION_MAXITEMS)
			return numMatches;
	}

	return numMatches;
}

//--------------------------------------------------------------------------------------------------------------
/*** Output a list of names to the console*/
void NavigationMesh::PlaceContainer::PrintAllPlaces (void) const
{
	if (IsEmpty ())
	{
		AddLogEntry (true, LogLevel_Default, false, "There are no entries in the Place database.");

		return;
	}

	for (PlacesArray::IndexType_t index (0u); index < m_places.GetElementNumber (); /* Empty */)
	{
		if (NameToPlace (m_places[index]) == g_server->GetNavigationMeshManager ().GetNavPlace ())
			AddLogEntry (true, LogLevel_Default, false, "--> %-26s.", m_places[index].GetData ());
		else
			AddLogEntry (true, LogLevel_Default, false, "%-30s.", m_places[index].GetData ());

		if (++index % 3u == 0u)
			AddLogEntry (true, LogLevel_Default, false, "\n");
	}

	AddLogEntry (true, LogLevel_Default, false, "\n");
}


//--------------------------------------------------------------------------------------------------------------
/*** Return the ground height below this point in "height".* Return false if position is invalid (outside of map, in a solid area, etc).*/
const bool NavigationMesh::GetGroundHeight (const Math::Vector3D &pos, float *const height/* = NULL*/, Math::Vector3D *const normal/* = NULL*/, const float maximumOffset/* = 100.0f*/)
{
//	VPROF ("NavigationMesh::GetGroundHeight");
//#if 0
/// FROM OLD SOURCE ENGINE
	Math::Vector3D from (pos);
	const Math::Vector3D to (pos.x, pos.y, pos.z - 9999.9f);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
//	HalfLifeEngine::SDK::Classes::Edict *ignore (NULL);	// MADE BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!!
	const unsigned char inc (10u/*maximumOffset * 0.1f*/);
	const unsigned char MaximumGroundLayersNumber (16u);
	struct GroundLayerInfo_t
	{
		float          groundZ;
		Math::Vector3D normal;
	} layers[MaximumGroundLayersNumber];
	unsigned char layerCount (0u);

	for (float offsetZ (1.0f); offsetZ < maximumOffset; offsetZ += inc)
	{
//		CTraceFilterWalkableEntities filter (ignore, COLLISION_GROUP_NONE, WALK_THRU_EVERYTHING);

		from.z += offsetZ;

//		UTIL_TraceLine (from, to, MASK_PLAYERSOLID_BRUSHONLY, &filter, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, ignore, traceResult);	// MADE BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!! - START
		TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

		if (traceResult.isStartSolid)
			continue;
/*
		// if the trace came down thru a door, ignore the door and try again also ignore breakable floors
		if (/*traceResult.hitEntity->IsValid ()*//*traceResult.fraction < 1.0f && IsCanWalkThruEntity (traceResult.hitEntity, WALK_THRU_EVERYTHING))
		{
			ignore = traceResult.hitEntity;

			// keep incrementing to avoid infinite loop if more than one entity is along the traceline...
			/// @todo Deal with multiple ignore entities in a single TraceLine()
			//offsetZ -= inc;

			continue;
		}
*/
		// if we didnt start inside a solid area, the trace hit a ground layer, if this is a new ground layer, add it to the set
		if (/*!traceResult.isStartSolid && (*/layerCount == 0u || traceResult.endPosition.z > layers[layerCount - 1u].groundZ)		// MADE BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!! - END
		{
			layers[layerCount].groundZ = traceResult.endPosition.z;
			layers[layerCount].normal = traceResult.planeNormal.IsZero () ? Math::Vector3D::UnitZ : traceResult.planeNormal;

			if (++layerCount == MaximumGroundLayersNumber)
				break;
		}
	}

	if (layerCount == 0u)
		return false;

	unsigned char index;

	// find the lowest layer that allows a player to stand or crouch upon it
	for (index = 0u; index < layerCount - 1u; ++index)
		if (layers[index + 1u].groundZ - layers[index].groundZ >= HalfLifeEngine::SDK::Constants::HalfHumanHeight)
			break;

	if (height != NULL)
		*height = layers[index].groundZ;

	if (normal != NULL)
		*normal = layers[index].normal;

	return true;
//#endif	// if 0
#if 0
{/*!!! FROM NEW SOURCE ENGINE !!!*/
	Math::Vector3D from (pos);
	const Math::Vector3D to (pos.x, pos.y, pos.z - 9999.9f);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	const float inc (HalfLifeEngine::SDK::Constants::HalfHumanHeight/*/10.0f/*maximumOffset * 0.1f*/ + Math::RawEpsilon);

	for (float offsetZ (inc); offsetZ < maximumOffset; offsetZ += inc)
	{
		from.z += offsetZ;

		TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

		if (traceResult.isStartSolid)
			continue;

		// Check if player can be here....
		if (traceResult.fraction < 1.0f && from.z - traceResult.endPosition.z < HalfLifeEngine::SDK::Constants::HalfHumanHeight)
			continue;

		if (height != NULL)
			*height = traceResult.endPosition.z;

		if (normal != NULL)
			*normal = traceResult.planeNormal.IsZero () ? Math::Vector3D::UnitZ : traceResult.planeNormal;

		return true;
	}
/* COMMENTED BY ME
	if (height != NULL)
		*height = 0.0f;

	if (normal != NULL)
		*normal = Math::Vector3D::UnitZ;
*/
	return false;
}
#endif	// if 0
#if 0
{/*BY ME - UNDONE!!!*/
	Math::Vector3D from (pos);
	const Math::Vector3D to (pos.x, pos.y, pos.z - 9999.9f);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	unsigned short cnt (static_cast <unsigned short> (maximumOffset));

	TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

	if (traceResult.isStartSolid || traceResult.fraction == 1.0f)
	{
//Label_Repeat:
		do
		{
			if (--cnt == 0u)
				return false;

			++from.z;
		} while (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (from) == HalfLifeEngine::SDK::Constants::Content_Solid);

		TraceLineThruWalkableEntities (from + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), from - Math::Vector3D::UnitZ, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

		if (traceResult.isStartSolid || traceResult.fraction < HalfLifeEngine::SDK::Constants::HalfHumanHeight / (HalfLifeEngine::SDK::Constants::HalfHumanHeight + 1.0f))
			return false;
#if 0
		TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

		const float oldFromZ (/*from.z/*/traceResult.endPosition.z);

		// Check if player can be here....
		for (;;)
		{
			++from.z;

			if (from.z - oldFromZ < HalfLifeEngine::SDK::Constants::HalfHumanHeight)
			{
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (from) == HalfLifeEngine::SDK::Constants::Content_Solid)
					goto Label_Repeat;
			}
			else
				break;
		}
#endif	// if 0
	}

	if (height != NULL)
		*height = traceResult.endPosition.z;

	if (normal != NULL)
		*normal = traceResult.planeNormal.IsZero () ? Math::Vector3D::UnitZ : traceResult.planeNormal;

	return true;
}
#endif	// if 0
}

//--------------------------------------------------------------------------------------------------------------
/*** Return the "simple" ground height below this point in "height".* This function is much faster, but less tolerant. Make sure the give position is "well behaved".* Return false if position is invalid (outside of map, in a solid area, etc).*/
const bool NavigationMesh::GetSimpleGroundHeight (const Math::Vector3D &pos, float *const height/* = NULL*/, Math::Vector3D *const normal/* = NULL*/)
{
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceLine (pos, Math::Vector3D (pos.x, pos.y, pos.z - 9999.9f), MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (pos, Math::Vector3D (pos.x, pos.y, pos.z - 9999.9f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);
	TraceLineThruWalkableEntities (pos, Math::Vector3D (pos.x, pos.y, pos.z - 9999.9f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

	if (traceResult.isStartSolid)
		return false;

	if (height != NULL)
		*height = traceResult.endPosition.z;

	if (normal != NULL)
		*normal = traceResult.planeNormal;

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/*** Show danger levels for debugging.*/
void NavigationMesh::DrawDanger (void) const
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->DrawDanger ();
}

//--------------------------------------------------------------------------------------------------------------
/*** Show current player counts for debugging.* NOTE: Assumes two teams.*/
void NavigationMesh::DrawPlayerCounts (void) const
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->DrawPlayerCounts ();
}

void NavigationMesh::DrawOccupyTime (void) const
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->DrawOccupyTime ();
}
void NavigationMesh::DrawBattlefront (void) const
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->DrawBattlefront ();
}


//--------------------------------------------------------------------------------------------------------------
/*** Increase the danger of navigation areas containing and near the given position*/
void NavigationMesh::IncreaseDangerNearby (const TeamArrayID_t teamArrayID, const float amount, NavigationArea *const startArea, const Math::Vector3D &pos, const float maxRadius)
{
	if (startArea == NULL)
		return;

	NavigationArea::MakeNewMarker ();
	NavigationArea::ClearSearchLists ();

	startArea->AddToOpenList ();
	startArea->SetTotalCost (0.0f);
	startArea->Mark ();
	startArea->IncreaseDanger (teamArrayID, amount);

	while (!NavigationArea::IsOpenListEmpty ())
	{
		// get next area to check
		NavigationArea *const area (NavigationArea::PopOpenList ());

		// explore adjacent areas
		for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
		{
			const NavAreaConnectArray::IndexType_t count (area->GetAdjacentCount (static_cast <NavDirType_t> (direction)));

			for (NavAreaConnectArray::IndexType_t i (0u); i < count; ++i)
			{
				NavigationArea *const adjArea (area->GetAdjacentArea (static_cast <NavDirType_t> (direction), i));

				if (!adjArea->IsMarked ())
				{
					// compute distance from danger source
					const float cost (adjArea->GetCenter ().GetDistance (pos));

					if (cost <= maxRadius)
					{
						adjArea->AddToOpenList ();
						adjArea->SetTotalCost (cost);
						adjArea->Mark ();
						adjArea->IncreaseDanger (teamArrayID, amount * cost / maxRadius);
					}
				}
			}
		}
	}
}
/*
//--------------------------------------------------------------------------------------------------------------
void CommandNavRemoveUnusedJumpAreas (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavRemoveUnusedJumpAreas ();
}
static ConCommand nav_remove_unused_jump_areas ("nav_remove_unused_jump_areas", CommandNavRemoveUnusedJumpAreas, "Removes jump areas with at most 1 connection to a ladder or non-jump area.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavDelete (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavDelete ();
}
static ConCommand nav_delete ("nav_delete", CommandNavDelete, "Deletes the currently highlighted Area.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavDeleteMarked (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavDeleteMarked ();
}
static ConCommand nav_delete_marked ("nav_delete_marked", CommandNavDeleteMarked, "Deletes the currently marked Area (if any).", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavSplit (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavSplit ();
}
static ConCommand nav_split ("nav_split", CommandNavSplit, "To split an Area into two, align the split line using your cursor and invoke the split command.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavMakeSniperSpots (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavMakeSniperSpots ();
}
static ConCommand nav_make_sniper_spots ("nav_make_sniper_spots", CommandNavMakeSniperSpots, "Chops the marked area into disconnected sub-areas suitable for sniper spots.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavMerge (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavMerge ();
}
static ConCommand nav_merge ("nav_merge", CommandNavMerge, "To merge two Areas into one, mark the first Area, highlight the second by pointing your cursor at it, and invoke the merge command.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavMark (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavMark ();
}
static ConCommand nav_mark ("nav_mark", CommandNavMark, "Marks the Area or Ladder under the cursor for manipulation by subsequent editing commands.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavUnmark (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavUnmark();
}
static ConCommand nav_unmark ("nav_unmark", CommandNavUnmark, "Clears the marked Area or Ladder.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavBeginArea (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavBeginArea ();
}
static ConCommand nav_begin_area ("nav_begin_area", CommandNavBeginArea, "Defines a corner of a new Area or Ladder. To complete the Area or Ladder, drag the opposite corner to the desired location and issue a 'nav_end_area' command.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavEndArea (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavEndArea ();
}
static ConCommand nav_end_area ("nav_end_area", CommandNavEndArea, "Defines the second corner of a new Area or Ladder and creates it.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavConnect (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavConnect ();
}
static ConCommand nav_connect ("nav_connect", CommandNavConnect, "To connect two Areas, mark the first Area, highlight the second Area, then invoke the connect command. Note that this creates a ONE-WAY connection from the first to the second Area. To make a two-way connection, also connect the second area to the first.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavDisconnect (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavDisconnect ();
}
static ConCommand nav_disconnect ("nav_disconnect", CommandNavDisconnect, "To disconnect two Areas, mark an Area, highlight a second Area, then invoke the disconnect command. This will remove all connections between the two Areas.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavSplice (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavSplice ();
}
static ConCommand nav_splice ("nav_splice", CommandNavSplice, "To splice, mark an area, highlight a second area, then invoke the splice command to create a new, connected area between them.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavCrouch (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_CROUCH);
}
static ConCommand nav_crouch ("nav_crouch", CommandNavCrouch, "Toggles the 'must crouch in this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavPrecise (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_PRECISE);
}
static ConCommand nav_precise ("nav_precise", CommandNavPrecise, "Toggles the 'dont avoid obstacles' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavJump (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_JUMP);
}
static ConCommand nav_jump ("nav_jump", CommandNavJump, "Toggles the 'traverse this area by jumping' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavNoJump (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_NO_JUMP);
}
static ConCommand nav_no_jump ("nav_no_jump", CommandNavNoJump, "Toggles the 'dont jump in this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavStop (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_STOP);
}
static ConCommand nav_stop ("nav_stop", CommandNavStop, "Toggles the 'must stop when entering this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavWalk (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_WALK);
}
static ConCommand nav_walk ("nav_walk", CommandNavWalk, "Toggles the 'traverse this area by walking' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavRun (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_RUN);
}
static ConCommand nav_run ("nav_run", CommandNavRun, "Toggles the 'traverse this area by running' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavAvoid (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_AVOID);
}
static ConCommand nav_avoid ("nav_avoid", CommandNavAvoid, "Toggles the 'avoid this area when possible' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavTransient (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_TRANSIENT);
}
static ConCommand nav_transient ("nav_transient", CommandNavTransient, "Toggles the 'area is transient and may become blocked' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavDontHide (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_DONT_HIDE);
}
static ConCommand nav_dont_hide ("nav_dont_hide", CommandNavDontHide, "Toggles the 'area is not suitable for hiding spots' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavStand (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_STAND);
}
static ConCommand nav_stand ("nav_stand", CommandNavStand, "Toggles the 'stand while hiding' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavNoHostages (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NAV_MESH_NO_HOSTAGES);
}
static ConCommand nav_no_hostages ("nav_no_hostages", CommandNavNoHostages, "Toggles the 'hostages cannot use this area' flag used by the AI system.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavStrip (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().StripNavigationAreas ();
}
static ConCommand nav_strip ("nav_strip", CommandNavStrip, "Strips all Hiding Spots, Approach Points, and Encounter Spots from the current Area.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavSave (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	if (g_server->GetNavigationMeshManager ().Save ())
		AddLogEntry (true, LogLevel_Default, false, "Navigation map '%s' saved.", g_server->GetNavigationMeshManager ().GetFilename ().GetData ());
	else
		AddLogEntry (true, LogLevel_Error, false, "Cannot save navigation map '%s'.", g_server->GetNavigationMeshManager ().GetFilename ().GetData ());
}
static ConCommand nav_save ("nav_save", CommandNavSave, "Saves the current Navigation Mesh to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavLoad (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	if (g_server->GetNavigationMeshManager ().Load () != NAV_OK)
		AddLogEntry (true, LogLevel_Error, false, "Navigation Mesh load failed.");
}
static ConCommand nav_load ("nav_load", CommandNavLoad, "Loads the Navigation Mesh for the current map.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
static int PlaceNameAutocompleteCallback (const char *partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
{
	return g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceNameAutocomplete (partial, commands);
}


//--------------------------------------------------------------------------------------------------------------
void CommandNavUsePlace (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount () == 1u)
	{
		// no arguments = list all available places
		g_server->GetNavigationMeshManager ().GetPlaceContainer ().PrintAllPlaces ();
	}
	else
	{
		// single argument = set current place
		const NavigationMesh::Place_t place (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u)));

		if (place == NavigationMesh::UNDEFINED_PLACE)
			AddLogEntry (true, LogLevel_Default, false, "Ambiguous.");
		else
		{
			AddLogEntry (true, LogLevel_Default, false, "Current place set to '%s'.", g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (place).GetData ());

			g_server->GetNavigationMeshManager ().SetNavPlace (place);
		}
	}
}
static ConCommand nav_use_place ("nav_use_place", CommandNavUsePlace, "If used without arguments, all available Places will be listed. If a Place argument is given, the current Place is set.", FCVAR_GAMEDLL | FCVAR_CHEAT, PlaceNameAutocompleteCallback);


//--------------------------------------------------------------------------------------------------------------
void CommandNavPlaceReplace (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount () != 3u)
	{
		// no arguments
		AddLogEntry (true, LogLevel_Default, false, "Usage: nav_place_replace <OldPlace> <NewPlace>.");
	}
	else
	{
		// two arguments - replace the first place with the second
		const NavigationMesh::Place_t oldPlace (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u)));
		const NavigationMesh::Place_t newPlace (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u)));

		if (oldPlace == NavigationMesh::UNDEFINED_PLACE || newPlace == NavigationMesh::UNDEFINED_PLACE)
			AddLogEntry (true, LogLevel_Default, false, "Ambiguous.");
		else for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			if (area->GetPlace () == oldPlace)
				area->SetPlace (newPlace);
		}
	}
}
static ConCommand nav_place_replace ("nav_place_replace", CommandNavPlaceReplace, "Replaces all instances of the first place with the second place.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavPlaceList (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	DynamicArray <NavigationMesh::Place_t> placeDirectory;

	for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
	{
		const NavigationMesh::Place_t place (g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetPlace ());

		if (place > NavigationMesh::UNDEFINED_PLACE && !placeDirectory.IsMember (place))
			placeDirectory += place;
	}

	AddLogEntry (true, LogLevel_Default, false, "Map uses %u place names:", placeDirectory.GetElementNumber ());

	for (unsigned short index (0u); index < placeDirectory.GetElementNumber (); ++index)
		AddLogEntry (true, LogLevel_Default, false, "    %s.", g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (placeDirectory[index]).GetData ());
}
static ConCommand nav_place_list ("nav_place_list", CommandNavPlaceList, "Lists all place names used in the map.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavTogglePlaceMode (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavTogglePlaceMode ();
}
static ConCommand nav_toggle_place_mode ("nav_toggle_place_mode", CommandNavTogglePlaceMode, "Toggle the editor into and out of Place mode. Place mode allows labelling of Area with Place names.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavSetPlaceMode (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	bool on (true);

	if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount () == 2u)
		on = HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u).GetValue <bool> ();

	if (on != g_server->GetNavigationMeshManager ().IsPlaceMode ())
		g_server->GetNavigationMeshManager ().CommandNavTogglePlaceMode ();
}
static ConCommand nav_set_place_mode ("nav_set_place_mode", CommandNavSetPlaceMode, "Sets the editor into or out of Place mode. Place mode allows labelling of Area with Place names.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavPlaceFloodFill (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavPlaceFloodFill ();
}
static ConCommand nav_place_floodfill ("nav_place_floodfill", CommandNavPlaceFloodFill, "Sets the Place of the Area under the cursor to the curent Place, and 'flood-fills' the Place to all adjacent Areas. Flood-filling stops when it hits an Area with the same Place, or a different Place than that of the initial Area.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavPlacePick (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavPlacePick ();
}
static ConCommand nav_place_pick ("nav_place_pick", CommandNavPlacePick, "Sets the current Place to the Place of the Area under the cursor.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavTogglePlacePainting (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavTogglePlacePainting ();
}
static ConCommand nav_toggle_place_painting ("nav_toggle_place_painting", CommandNavTogglePlacePainting, "Toggles Place Painting mode. When Place Painting, pointing at an Area will 'paint' it with the current Place.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavMarkUnnamed (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavMarkUnnamed ();
}
static ConCommand nav_mark_unnamed ("nav_mark_unnamed", CommandNavMarkUnnamed, "Mark an Area with no Place name. Useful for finding stray areas missed when Place Painting.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavCornerSelect (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavCornerSelect ();
}
static ConCommand nav_corner_select ("nav_corner_select", CommandNavCornerSelect, "Select a corner of the currently marked Area. Use multiple times to access all four corners.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavCornerRaise (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavCornerRaise ();
}
static ConCommand nav_corner_raise ("nav_corner_raise", CommandNavCornerRaise, "Raise the selected corner of the currently marked Area.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavCornerLower (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavCornerLower ();
}
static ConCommand nav_corner_lower ("nav_corner_lower", CommandNavCornerLower, "Lower the selected corner of the currently marked Area.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavCornerPlaceOnGround (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavCornerPlaceOnGround ();
}
static ConCommand nav_corner_place_on_ground ("nav_corner_place_on_ground", CommandNavCornerPlaceOnGround, "Places the selected corner of the currently marked Area on the ground.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavWarpToMark (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavWarpToMark ();
}
static ConCommand nav_warp_to_mark ("nav_warp_to_mark", CommandNavWarpToMark, "Warps the player to the marked area.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavLadderFlip (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavLadderFlip ();
}
static ConCommand nav_ladder_flip ("nav_ladder_flip", CommandNavLadderFlip, "Flips the selected ladder's direction.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavGenerate (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().BeginGeneration ();
}
static ConCommand nav_generate ("nav_generate", CommandNavGenerate, "Generate a Navigation Mesh for the current map and save it to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavGenerateIncremental (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().BeginGeneration (true);
}
static ConCommand nav_generate_incremental ("nav_generate_incremental", CommandNavGenerateIncremental, "Generate a Navigation Mesh for the current map and save it to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavAnalyze (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().BeginAnalysis ();
}
static ConCommand nav_analyze ("nav_analyze", CommandNavAnalyze, "Re-analyze the current Navigation Mesh and save it to disk.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavMarkWalkable (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	Math::Vector3D pos;

	if ((Console::Bot::VariableID_nav_edit)->GetValue <bool> ())
	{
		// we are in edit mode, use the edit cursor's location
		pos = g_server->GetNavigationMeshManager ().GetEditCursorPosition ();
	}
	else
	{
		// we are not in edit mode, use the position of the local player
		const Client *const player (g_server->GetHumanManager ()->GetHostClient ());

		if (!player->IsValid ())
		{
			AddLogEntry (true, LogLevel_Error, false, "No local player!");

			return;
		}

		pos = player->GetOrigin ();
	}

	// snap position to the sampling grid
	pos.x = g_server->GetNavigationMeshManager ().SnapToGrid (pos.x, true);
	pos.y = g_server->GetNavigationMeshManager ().SnapToGrid (pos.y, true);

	Math::Vector3D normal;
	if (NavigationMesh::GetGroundHeight (pos, &pos.z, &normal) == false)
	{
		AddLogEntry (true, LogLevel_Error, false, "Invalid ground position.");

		return;
	}

	g_server->GetNavigationMeshManager ().AddWalkableSeed (pos, normal);

	AddLogEntry (true, LogLevel_Default, false, "Walkable position marked.");
}
static ConCommand nav_mark_walkable ("nav_mark_walkable", CommandNavMarkWalkable, "Mark the current location as a walkable position. These positions are used as seed locations when sampling the map to generate a Navigation Mesh.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavClearWalkableMarks (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().ClearWalkableSeeds ();
}
static ConCommand nav_clear_walkable_marks ("nav_clear_walkable_marks", CommandNavClearWalkableMarks, "Erase any previously placed walkable positions.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavCompressID (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CompressAreasIDs ();
	g_server->GetNavigationMeshManager ().CompressLaddersIDs ();
}
static ConCommand nav_compress_id ("nav_compress_id", CommandNavCompressID, "Re-orders area and ladder ID's so they are continuous.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
inline void CommandNavShowLadderBounds (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	for (HalfLifeEngine::SDK::Classes::Edict *ladder (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_ladder")); ladder->IsValid (); ladder = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ladder, "func_ladder"))
	{
//		NDebugOverlay::Box (Math::Vector3D::ZeroValue, ladder->variables.boundingBox.mins, ladder->variables.boundingBox.maxs, 0, 255, 0, 0, 600);
		g_server->GetHumanManager ()->GetHostClient ()->DrawBox (ladder->variables.boundingBox.mins, ladder->variables.boundingBox.maxs, Color <> (0u, 255, 0u), 255u);
//		NDebugOverlay::Box (ladder->GetOrigin (), ladder->variables.boundingBox.mins, ladder->variables.boundingBox.maxs, 0, 0, 255, 0, 600);
		g_server->GetHumanManager ()->GetHostClient ()->DrawBox (ladder->variables.absoluteBoundingBox.mins, ladder->variables.absoluteBoundingBox.maxs, Color <> (0u, 0u, 255u), 255u);
	}
}
static ConCommand nav_show_ladder_bounds ("nav_show_ladder_bounds", CommandNavShowLadderBounds, "Draws the bounding boxes of all func_ladders in the map.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavBuildLadder (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavBuildLadder ();
}
static ConCommand nav_build_ladder ("nav_build_ladder", CommandNavBuildLadder, "Attempts to build a navigation ladder on the climbable surface under the cursor.", FCVAR_GAMEDLL | FCVAR_CHEAT);


/* IN PROGRESS:
//--------------------------------------------------------------------------------------------------------------
void CommandNavPickArea (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavPickArea ();
}
static ConCommand nav_pick_area ("nav_pick_area", CommandNavPickArea, "Marks an area (and corner) based on the surface under the cursor.", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavResizeHorizontal (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavResizeHorizontal ();
}
static ConCommand nav_resize_horizontal ("nav_resize_horizontal", CommandNavResizeHorizontal, "TODO", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavResizeVertical (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavResizeVertical ();
}
static ConCommand nav_resize_vertical ("nav_resize_vertical", CommandNavResizeVertical, "TODO", FCVAR_GAMEDLL | FCVAR_CHEAT);


//--------------------------------------------------------------------------------------------------------------
void CommandNavResizeEnd (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	g_server->GetNavigationMeshManager ().CommandNavResizeEnd ();
}
static ConCommand nav_resize_end ("nav_resize_end", CommandNavResizeEnd, "TODO", FCVAR_GAMEDLL | FCVAR_CHEAT);
*/

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::DestroyHidingSpots (void)
{
	// remove all hiding spot references from the navigation areas
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		// free all the HidingSpots
		m_areas[index]->DestroyHidingSpots ();
	}

	NavigationArea::HidingSpot::m_nextID = 0u;	// Reset the next hiding spot ID to 0
}

//--------------------------------------------------------------------------------------------------------------
/*** Disconnect given area from all others.*/
void NavigationMesh::Disconnect (NavigationArea *const area)
{
	// tell the other areas we are going away
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		// Should be already removed from 'm_areas' array!
		InternalAssert (m_areas[index] != area);

		m_areas[index]->OnDestroyNotify (area);
	}

	// unhook from ladders
	for (NavLadderArray::IndexType_t index (0u); index < m_ladders.GetElementNumber (); ++index)
		m_ladders[index]->OnDestroyNotify (area);
}

//--------------------------------------------------------------------------------------------------------------
/*** Disconnect given area from all others, remove from a grid and navigation mesh.*/
void NavigationMesh::DestroyArea (NavigationArea *const area)
{
	g_server->GetClientManager ()->OnDestroyNavigationArea (area);

	m_areas -= area;

	Disconnect (area);	// tell the other areas we are going away
	RemoveArea (area);	// remove the area from the navigation mesh

	delete area;	// Delete it.
}

//--------------------------------------------------------------------------------------------------------------
/*** Disconnect given ladder from all connected areas.*/
void NavigationMesh::Disconnect (NavigationLadder *const ladder)
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->OnDestroyNotify (ladder);
}

//--------------------------------------------------------------------------------------------------------------
/*** Disconnect given ladder from all connected areas and remove from a navigation mesh.*/
void NavigationMesh::DestroyLadder (NavigationLadder *const ladder)
{
	m_ladders -= ladder;

	Disconnect (ladder);	// tell the other areas we are going away

	delete ladder;	// Delete it.
}

//--------------------------------------------------------------------------------------------------------------
/*** Destroy ladder representations*/
void NavigationMesh::DestroyLadders (void)
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		for (NavLadderArray::IndexType_t index2 (0u); index2 < m_ladders.GetElementNumber (); ++index2)
			m_areas[index]->OnDestroyNotify (m_ladders[index2]);

	m_ladders.DeleteAndPurgeAll ();	// remove each element of the array and delete them

	m_markedLadder = m_selectedLadder = NULL;
}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
/*** Spot post-load processing*/
const bool NavigationMesh::NavigationArea::HidingSpot::PostLoad (void)
{
	// set our area
	m_area = g_server->GetNavigationMeshManager ().GetNavArea (m_pos + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight));

//	AssertMsg (m_area != NULL, UTIL_VarArgs ("A Spot is off of the Navigation Mesh at (%.0f %.0f %.0f)", m_pos.x, m_pos.y, m_pos.z));
	if (m_area == NULL)
	{
		AddLogEntry (true, LogLevel_Warning, false, "A Spot is off of the Navigation Mesh at (%.0f %.0f %.0f)", m_pos.x, m_pos.y, m_pos.z);

		return false;
	}

	return true;
}