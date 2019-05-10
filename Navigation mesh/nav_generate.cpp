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
// nav_generate.hpp
//
// Description: Auto-generate a Navigation Mesh by sampling the current map.
//
// Version: $ID:$
//

#include <Core.hpp>

enum { MAX_BLOCKED_AREAS = 256u };
static unsigned short blockedID[MAX_BLOCKED_AREAS];
static unsigned short blockedIDCount (0u);
static float generationStartTime (0.0f);
static float creatingNavigationAreasFromSampledDataStartTime (0.0f);
static float findingHidingSpotsStartTime (0.0f);
static float findingApproachAreasStartTime (0.0f);
static float findingEncounterSpotsStartTime (0.0f);
static float findingSniperSpotsStartTime (0.0f);
static float findingEarliestOccupyTimesStartTime (0.0f);

/*
ConVar nav_slope_limit ("nav_slope_limit", "0.7", FCVAR_GAMEDLL, "The ground unit normal's Z component must be greater than this for navigation areas to be generated.");
ConVar nav_restart_after_analysis ("nav_restart_after_analysis", "1", FCVAR_GAMEDLL, "When navigation nav_restart_after_analysis finishes, restart the server. Turning this off can cause crashes, but is useful for incremental generation.");
*/

//--------------------------------------------------------------------------------------------------------------
/**
* Shortest path cost, paying attention to "blocked" areas
*/
class ApproachAreaCost
{
	public:
		inline const float operator () (const NavigationMesh::NavigationArea *const area, const NavigationMesh::NavigationArea *const fromArea, const NavigationMesh::NavigationLadder *const ladder) const
		{
			// check if this area is "blocked"
			for (unsigned short index (0u); index < blockedIDCount; ++index)
				if (area->GetID () == blockedID[index])
					return -1.0f;

			if (fromArea == NULL)
				return 0.0f;	// first area in path, no cost

			// compute distance traveled along path so far
			return (ladder != NULL ? ladder->GetLength () : area->GetCenter ().GetDistance (fromArea->GetCenter ())) + fromArea->GetCostSoFar ();
		}
};


//--------------------------------------------------------------------------------------------------------------
/**
* Determine the set of "approach areas".
* An approach area is an area representing a place where players
* move into/out of our local neighborhood of areas.
* @todo Optimize by search from eye outward and modifying pathfinder to treat all links as bi-directional
*/
void NavigationMesh::NavigationArea::ComputeApproachAreas (void)
{
	m_approach.RemoveAll ();

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_quicksave)->GetValue <bool> ())
		return;

	// use the center of the navigation area as the "view" point
	Math::Vector3D eye (m_center);

	if (NavigationMesh::GetGroundHeight (eye, &eye.z) == false)
		return;

	// approximate eye position
	eye.z += 0.9f * ((m_attributeFlags & NAV_MESH_CROUCH) ? HalfLifeEngine::SDK::Constants::HalfHumanHeight : HalfLifeEngine::SDK::Constants::HumanHeight);

	enum { MAX_PATH_LENGTH = 256u };
	NavigationArea *path[MAX_PATH_LENGTH];
	ApproachAreaCost cost;

	DECLARE_TYPED_ENUMERATION (unsigned char, SearchType_t)
	{
		FROM_EYE,		///< start search from our eyepoint outward to farArea
		TO_EYE,			///< start search from farArea beack towards our eye
		SEARCH_FINISHED
	};

	//
	// In order to *completely* enumerate all of the approach areas, we
	// need to search from our eyepoint outward, as well as from outwards
	// towards our eyepoint
	//
	for (SearchType_t searchType (FROM_EYE); searchType < SEARCH_FINISHED; ++searchType)
	{
		//
		// In order to enumerate all of the approach areas, we need to
		// run the algorithm many times, once for each "far away" area
		// and keep the union of the approach area sets
		//
		for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationArea *const farArea (g_server->GetNavigationMeshManager ().GetAreas ()[index]);
			const float minSize (200.0f);	// 150
			const Math::Extent3D &extent (farArea->GetExtent ());

			blockedIDCount = 0u;

			// skip the small areas
			if (extent.GetSizeX () < minSize || extent.GetSizeY () < minSize)
				continue;

			// if we can see 'farArea', try again - the whole point is to go "around the bend", so to speak
			if (farArea->IsVisible (eye))
				continue;

			//
			// Keep building paths to 'farArea' and blocking them off until we
			// cant path there any more.
			// As areas are blocked off, all exits will be enumerated.
			//
/// ORIGINAL LOOP			while (m_approach.GetElementNumber () < MAX_APPROACH_AREAS)
/** NEW LOOP */			for (/* Empty */; /* Empty */; /* Empty */)
			{
				NavigationArea *from, *to;

				if (searchType == FROM_EYE)
				{
					// find another path *to* 'farArea'
					// we must pathfind from us in order to pick up one-way paths OUT OF our area
					from = this;
					to = farArea;
				}
				else	// TO_EYE
				{
					// find another path *from* 'farArea'
					// we must pathfind to us in order to pick up one-way paths INTO our area
					from = farArea;
					to = this;
				}

				// build the actual path
				if (NavAreaBuildPath (from, to, NULL, cost) == false)
					break;

				unsigned short count (0u);
				NavigationArea *area;

				// find number of areas on path
				for (area = to; area != NULL; area = area->GetParent ())
					++count;

				// if the path is only two areas long, there can be no approach points
				if (count <= 2u)
					break;
				else if (count > MAX_PATH_LENGTH)
					count = MAX_PATH_LENGTH;

				// build path starting from eye
				unsigned short i (0u);

				if (searchType == FROM_EYE)
				{
					for (area = to; i < count && area != NULL; area = area->GetParent (), ++i)
						path[count - i - 1u] = area;
				}
				else for (area = to; i < count && area != NULL; area = area->GetParent (), ++i)	// TO_EYE
					path[i] = area;

				// traverse path to find first area we cannot see (skip the first area)
				for (i = 1u; i < count; ++i)
				{
					// if we see this area, continue on
					if (path[i]->IsVisible (eye))
						continue;

					// we can't see this area - mark this area as "blocked" and unusable by subsequent approach paths
					if (blockedIDCount == MAX_BLOCKED_AREAS)
					{
						AddLogEntry (true, LogLevel_Default, false, "Overflow computing approach areas for area #%u.", m_id);

						return;
					}

					// if the area to be blocked is actually farArea, block the one just prior
					// (blocking farArea will cause all subsequent pathfinds to fail)
					const unsigned short block (path[i] == farArea != NULL ? i - 1u : i);

					// dont block the start area, or all subsequence pathfinds will fail
					if (block == 0u)
						continue;

					blockedID[blockedIDCount] = path[block]->GetID ();

					++blockedIDCount;

					ApproachArray::IndexType_t approachIndex;

					// store new approach area if not already in set
					for (approachIndex = 0u; approachIndex < m_approach.GetElementNumber (); ++approachIndex)
						if (m_approach[approachIndex].here == path[block - 1u])
							break;

					if (approachIndex == m_approach.GetElementNumber ())
					{
						const ApproachInfo_t newApproach = {path[block - 1u], block >= 2u ? path[block - 2u] : NULL, path[block - 1u]->GetParentHow (), path[block], path[block]->GetParentHow ()};

						m_approach += newApproach;
/// NEW CHECK
						if (m_approach.GetElementNumber () == MAX_APPROACH_AREAS)
							return;
					}

					// we are done with this path
					break;
				}
			}
		}
	}
}

void NavigationMesh::NavigationArea::BuildOverlapAreas (void)
{
	// build overlap areas list

	m_overlapAreas.RemoveAll ();

	/// @todo Optimize this
	for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
	{
		NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

		// skip self
		if (area != this && IsOverlapping (area))
			m_overlapAreas += area;
	}
}

void NavigationMesh::BuildOverlapAreas (void)
{
	// build overlap areas list

	AddLogEntry (true, LogLevel_Default, false, "Building overlapping navigation areas...");

	/// @todo Optimize this
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->BuildOverlapAreas ();

	AddLogEntry (true, LogLevel_Default, false, "Building overlapping navigation areas...DONE!");
}

//--------------------------------------------------------------------------------------------------------------
/**
* Start at given position and find first area in given direction
*/
NavigationMesh::NavigationArea *const NavigationMesh::FindFirstAreaInDirection (const Math::Vector3D &start, const NavDirType_t direction, const float range, const float beneathLimit, HalfLifeEngine::SDK::Classes::Edict *const/* traceIgnore*//* = NULL*/, Math::Vector3D *const closePos/* = NULL*/) const
{
	NavigationArea *area;
	Math::Vector3D pos (start);
	const unsigned char end (static_cast <unsigned char> (range / GenerationStepSize + 0.5f));
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	for (unsigned char i (0u); i < end; ++i)
	{
		AddDirectionVector (pos, direction, GenerationStepSize);

		// make sure we dont look thru the wall
//		UTIL_TraceLine (start, pos, MASK_PLAYERSOLID_BRUSHONLY, traceIgnore, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (start, pos, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceIgnore, traceResult);
		TraceLineThruWalkableEntities (start, pos, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

		if (traceResult.fraction < 1.0f)
			break;

		area = GetNavArea (pos, beneathLimit);

		if (area == NULL)
			continue;

		if (closePos != NULL)
		{
			closePos->x = pos.x;
			closePos->y = pos.y;
			closePos->z = area->GetZ (pos);
		}

		return area;
	}

	return NULL;
}


//--------------------------------------------------------------------------------------------------------------
/**
* For each ladder in the map, create a navigation representation of it.
*/
void NavigationMesh::BuildLadders (void)
{
	// remove any left-over ladders
	DestroyLadders ();

	for (HalfLifeEngine::SDK::Classes::Edict *ladderEdict (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_ladder")); ladderEdict->IsValid (); ladderEdict = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ladderEdict, "func_ladder"))
		CreateLadder (ladderEdict);
}


//--------------------------------------------------------------------------------------------------------------
/**
* Create a navigation representation of a ladder.
*/
void NavigationMesh::CreateLadder (HalfLifeEngine::SDK::Classes::Edict *const ladderEdict)
{
	NavigationLadder *const ladder (new NavigationLadder (ladderEdict));

	#if defined _DEBUG
		// Reliability check.
		if (ladder == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation ladder at location: %s!", sizeof (*ladder), ladderEdict->GetCenter ().ToString ());

			return;
		}
	#endif	// if defined _DEBUG

	// add ladder to global list
	m_ladders += ladder;
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationLadder::ConnectGeneratedLadder (void)
{
	const unsigned char nearLadderRange (75u);	// 50

	//
	// Find naviagtion area at bottom of ladder
	//

	// get approximate postion of player on ladder
	Math::Vector3D center (m_bottom);

	center.z += GenerationStepSize;

	AddDirectionVector (center, m_direction, HalfLifeEngine::SDK::Constants::HalfHumanWidth);

	m_connectedAreas[ConnectionType_Bottom] = g_server->GetNavigationMeshManager ().GetNearestNavArea (center/*, true*/);

	if (m_connectedAreas[ConnectionType_Bottom] == NULL)
	{
		if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
			AddLogEntry (true, LogLevel_Error, false, "Unconnected ladder bottom at %s.", m_bottom.ToString ());
	}
	else
		m_connectedAreas[ConnectionType_Bottom]->ConnectTo (this, Direction_Up);	// store reference to ladder in the area

	//
	// Find adjacent navigation areas at the top of the ladder
	//

	// get approximate postion of player on ladder
	center = m_top;

	center.z += GenerationStepSize;

	AddDirectionVector (center, m_direction, HalfLifeEngine::SDK::Constants::HalfHumanWidth);

	const float beneathLimit (Math::GetMinimumValueBetween (120.0f, m_top.z - m_bottom.z + HalfLifeEngine::SDK::Constants::HalfHumanWidth));

	// find "behind" area - must look farther, since ladder is against the wall away from this area
	m_connectedAreas[ConnectionType_TopBehind] = g_server->GetNavigationMeshManager ().FindFirstAreaInDirection (center, GetOppositeDirection (m_direction), nearLadderRange, beneathLimit);

	if (m_connectedAreas[ConnectionType_TopBehind] == m_connectedAreas[ConnectionType_Bottom])
		m_connectedAreas[ConnectionType_TopBehind] = NULL;

	// find "left" area
	m_connectedAreas[ConnectionType_TopLeft] = g_server->GetNavigationMeshManager ().FindFirstAreaInDirection (center, GetDirectionLeft (m_direction), nearLadderRange, beneathLimit);

	if (m_connectedAreas[ConnectionType_TopLeft] == m_connectedAreas[ConnectionType_Bottom])
		m_connectedAreas[ConnectionType_TopLeft] = NULL;

	// find "right" area
	m_connectedAreas[ConnectionType_TopRight] = g_server->GetNavigationMeshManager ().FindFirstAreaInDirection (center, GetDirectionRight (m_direction), nearLadderRange, beneathLimit);

	if (m_connectedAreas[ConnectionType_TopRight] == m_connectedAreas[ConnectionType_Bottom])
		m_connectedAreas[ConnectionType_TopRight] = NULL;

	// find "ahead" area
	m_connectedAreas[ConnectionType_TopForward] = g_server->GetNavigationMeshManager ().FindFirstAreaInDirection (center, m_direction, 2.0f * nearLadderRange, beneathLimit);

	if (m_connectedAreas[ConnectionType_TopForward] == m_connectedAreas[ConnectionType_Bottom])
		m_connectedAreas[ConnectionType_TopForward] = NULL;

	// can't include behind area, since it is not used when going up a ladder
	if (m_connectedAreas[ConnectionType_TopBehind] == NULL && m_connectedAreas[ConnectionType_TopLeft] == NULL && m_connectedAreas[ConnectionType_TopRight] == NULL)
	{
		if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
			AddLogEntry (true, LogLevel_Error, false, "Unconnected ladder top at %s.", m_top.ToString ());
	}
	// store reference to ladder in the area(s)
	else for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_TopForward; ++connectionType)
		if (m_connectedAreas[connectionType] != NULL)
			m_connectedAreas[connectionType]->ConnectTo (this, Direction_Down);

	if (m_connectedAreas[ConnectionType_TopForward] != NULL)
	{
		m_connectedAreas[ConnectionType_TopForward]->ConnectTo (this, Direction_Down);

		Disconnect (m_connectedAreas[ConnectionType_TopForward]);
	}

	// adjust top of ladder to highest connected area
	float topZ (m_bottom.z + 5.0f);
	bool topAdjusted (false);

	for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_Bottom; ++connectionType)
	{
		const NavigationArea *const topArea (m_connectedAreas[connectionType]);

		if (topArea == NULL)
			continue;

		const Math::Vector3D &close (topArea->GetClosestPointOnArea (m_top));

		if (topZ < close.z)
		{
			topZ = close.z;
			topAdjusted = true;
		}
	}

	if (topAdjusted)
		m_top.z = topZ;

	//
	// Determine whether this ladder is "dangling" or not
	// "Dangling" ladders are too high to go up
	//
	if (m_connectedAreas[ConnectionType_Bottom] != NULL)
	{
		const Math::Vector3D &bottomSpot (m_connectedAreas[ConnectionType_Bottom]->GetClosestPointOnArea (m_bottom));

		if (m_bottom.z - bottomSpot.z > HalfLifeEngine::SDK::Constants::HumanHeight)
			m_connectedAreas[ConnectionType_Bottom]->Disconnect (this);
	}

	//
	// Find adjacent navigation areas at the ladder gaps
	//

	m_connectedGapAreasNumber = static_cast <unsigned char> (m_length / GenerationStepSize) + 1u;

	delete [] m_connectedGapAreas;

	m_connectedGapAreas = new ConnectedAreas_t[m_connectedGapAreasNumber];

	#if defined _DEBUG
		// Reliability check.
		if (m_connectedGapAreas == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating connected navigation areas array!", sizeof (ConnectedAreas_t) * m_connectedGapAreasNumber);

			return;
		}
	#endif	// if defined _DEBUG

	for (unsigned char index (0u); index < m_connectedGapAreasNumber; ++index)
	{
		// get approximate postion of player on ladder
		center = GetPosAtHeight (m_bottom.z + index * GenerationStepSize);

		AddDirectionVector (center, m_direction, HalfLifeEngine::SDK::Constants::HalfHumanWidth);

		const float beneathLimit (Math::GetMinimumValueBetween (120.0f, m_top.z - center.z + HalfLifeEngine::SDK::Constants::HalfHumanWidth));

		// find "ahead" area
		m_connectedGapAreas[index][ConnectionType_TopBehind] = g_server->GetNavigationMeshManager ().FindFirstAreaInDirection (center, m_direction, 2.0f * nearLadderRange, beneathLimit);

		if (/*m_connectedGapAreas[index][ConnectionType_TopBehind] == m_connectedGapAreas[0u][ConnectionType_TopBehind] || *//*(index > 0u && m_connectedGapAreas[index][ConnectionType_TopBehind] == m_connectedGapAreas[index - 1u][ConnectionType_TopBehind]) || */(m_connectedGapAreas[index][ConnectionType_TopBehind] != NULL && (m_connectedGapAreas[index][ConnectionType_TopBehind]->GetCenter ().z + HalfLifeEngine::SDK::Constants::HalfHumanHeight > center.z || IsConnected2 (m_connectedGapAreas[index][ConnectionType_TopBehind]))))
			m_connectedGapAreas[index][ConnectionType_TopBehind] = NULL;

		// find "left" area
		m_connectedGapAreas[index][ConnectionType_TopLeft] = g_server->GetNavigationMeshManager ().FindFirstAreaInDirection (center, GetDirectionLeft (m_direction), nearLadderRange, beneathLimit);

		if (/*m_connectedGapAreas[index][ConnectionType_TopLeft] == m_connectedGapAreas[0u][ConnectionType_TopLeft] || *//*(index > 0u && m_connectedGapAreas[index][ConnectionType_TopLeft] == m_connectedGapAreas[index - 1u][ConnectionType_TopLeft]) || */(m_connectedGapAreas[index][ConnectionType_TopLeft] != NULL && (m_connectedGapAreas[index][ConnectionType_TopLeft]->GetCenter ().z + HalfLifeEngine::SDK::Constants::HalfHumanHeight > center.z || IsConnected2 (m_connectedGapAreas[index][ConnectionType_TopLeft]))))
			m_connectedGapAreas[index][ConnectionType_TopLeft] = NULL;

		// find "right" area
		m_connectedGapAreas[index][ConnectionType_TopRight] = g_server->GetNavigationMeshManager ().FindFirstAreaInDirection (center, GetDirectionRight (m_direction), nearLadderRange, beneathLimit);

		if (/*m_connectedGapAreas[index][ConnectionType_TopRight] == m_connectedGapAreas[0u][ConnectionType_TopRight] || *//*(index > 0u && m_connectedGapAreas[index][ConnectionType_TopRight] == m_connectedGapAreas[index - 1u][ConnectionType_TopRight]) || */(m_connectedGapAreas[index][ConnectionType_TopRight] != NULL && (m_connectedGapAreas[index][ConnectionType_TopRight]->GetCenter ().z + HalfLifeEngine::SDK::Constants::HalfHumanHeight > center.z || IsConnected2 (m_connectedGapAreas[index][ConnectionType_TopRight]))))
			m_connectedGapAreas[index][ConnectionType_TopRight] = NULL;

		#if defined _DEBUG
			if (m_connectedGapAreas[index][ConnectionType_TopBehind] != NULL)
			{
				// Reliability checks.
				InternalAssert (m_connectedGapAreas[index][ConnectionType_TopBehind] != m_connectedGapAreas[index][ConnectionType_TopLeft]);
				InternalAssert (m_connectedGapAreas[index][ConnectionType_TopBehind] != m_connectedGapAreas[index][ConnectionType_TopRight]);
			}
/*
			if (m_connectedGapAreas[index][ConnectionType_TopLeft] != NULL)
			{
				// Reliability check.
				InternalAssert (m_connectedGapAreas[index][ConnectionType_TopLeft] != m_connectedGapAreas[index][ConnectionType_TopRight]);
			}*/
		#endif	// if defined _DEBUG
	}
/*
	for (unsigned char index (1u); index < m_connectedGapAreasNumber; ++index)
	{
		for (unsigned char direction (ConnectionType_TopBehind); direction < ConnectionType_GapTotal; ++direction)
		{
			if (m_connectedGapAreas[index - 1u][direction] == m_connectedGapAreas[index][direction])
			{
				if (m_connectedGapAreas[index - 1u][direction]->GetCenter ().z < m_connectedGapAreas[index][direction]->GetCenter ().z)
			}
		}
	}
*/
	/// @todo NEED TO REMOVE 'ConnectionType_Bottom' - USE INSTEAD IT 'm_connectedGapAreas[0u]', CONNECT AREAS TO LADDERS AND RESIZE 'm_connectedGapAreas' ARRAY AFTER ALL OF THIS!
}


//--------------------------------------------------------------------------------------------------------------
/**
* Mark all areas that require a jump to get through them.
* This currently relies on jump areas having extreme slope.
*/
void NavigationMesh::MarkJumpAreas (void)
{
	AddLogEntry (true, LogLevel_Default, false, "Marking jump navigation areas...");

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->TestJump ();

	AddLogEntry (true, LogLevel_Default, false, "Marking jump navigation areas...DONE!");
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::MarkStairAreas (void)
{
	AddLogEntry (true, LogLevel_Default, false, "Marking stair navigation areas...");

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->TestStairs ();

	AddLogEntry (true, LogLevel_Default, false, "Marking stair navigation areas...DONE!");
}

//--------------------------------------------------------------------------------------------------------------
/**
* Jump areas that are connected to only one non-jump area won't be used. Delete them.
*/
void NavigationMesh::RemoveUnusedJumpAreas (void)
{
	NavAreaArray unusedAreas;

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const testArea (m_areas[index]);

		if (!testArea->HasAttributes (NAV_MESH_JUMP))
			continue;

		const NavigationArea *connectedArea (NULL);
		const NavigationLadder *connectedLadder (NULL);
		bool doubleConnected (false);
		unsigned char direction;

		// Look at testArea->ladder connections
		for (direction = NavigationLadder::Direction_Up; direction < NavigationLadder::Direction_Total; ++direction)
		{
			const NavLadderConnectArray &ladderList (testArea->GetConnectedLadders (static_cast <NavigationLadder::Direction_t> (direction)));

			if (ladderList.IsEmpty ())
				continue;

			if (ladderList.GetElementNumber () > 1u)
			{
				doubleConnected = true;

				break;
			}

			const NavigationLadder *const ladderConnect (ladderList.GetFirstElement ());

			if (connectedArea != NULL || (connectedLadder != NULL && connectedLadder != ladderConnect))
			{
				doubleConnected = true;

				break;
			}

			connectedLadder = ladderConnect;
		}

		if (doubleConnected)
			continue;

		// Look at testArea->area connections
		for (direction = NORTH; direction < NUM_DIRECTIONS; ++direction)
		{
			const NavAreaConnectArray &areaList (testArea->GetAdjacentList (static_cast <NavDirType_t> (direction)));

			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < areaList.GetElementNumber (); ++index2)
			{
				const NavigationArea *const areaConnect (areaList[index2]);

				if (areaConnect->HasAttributes (NAV_MESH_JUMP))
					continue;

				if (connectedLadder != NULL || (connectedArea != NULL && connectedArea != areaConnect))
				{
					doubleConnected = true;

					break;
				}

				connectedArea = areaConnect;
			}
		}

		if (doubleConnected)
			continue;

		// Look at ladder->testArea connections
		for (NavLadderArray::IndexType_t index2 (0u); index2 < m_ladders.GetElementNumber (); ++index2)
		{
			NavigationLadder *const ladder (m_ladders[index2]);

			if (!ladder->IsConnected (testArea))
				continue;

			if (connectedArea != NULL)
			{
				doubleConnected = true;

				break;
			}

			if (ladder == connectedLadder)
				continue;

			if (connectedLadder != NULL)
			{
				doubleConnected = true;

				break;
			}

			connectedLadder = ladder;
		}

		if (doubleConnected)
			continue;

		// Look at area->testArea connections
		for (NavAreaArray::IndexType_t index2 (0u); index2 < m_areas.GetElementNumber (); ++index2)
		{
			NavigationArea *const area (m_areas[index2]);

			if (area->HasAttributes (NAV_MESH_JUMP) || !area->IsConnected (testArea))
				continue;

			if (connectedLadder != NULL)
			{
				doubleConnected = true;

				break;
			}

			if (area == connectedArea)
				continue;

			if (connectedArea != NULL)
			{
				doubleConnected = true;

				break;
			}

			connectedArea = area;
		}

		if (doubleConnected)
			continue;

		// Since we got here, at most one ladder or non-jump area is connected to us, so we can be deleted.
		unusedAreas += testArea;
	}

	for (NavAreaArray::IndexType_t index (0u); index < unusedAreas.GetElementNumber (); ++index)
		DestroyArea (unusedAreas[index]);

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::StichJumpAreas (void)
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->Stich ();
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::RemoveJumpAreas (void)
{
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const jumpArea (m_areas[index]);

		if (jumpArea->HasAttributes (NAV_MESH_JUMP))
			DestroyArea (jumpArea);
	}

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::RemoveUnusedDoubleJumpAreas (void)
{
#if 0
	NavAreaArray unusedAreas;

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const testArea (m_areas[index]);

		if (!testArea->HasAttributes (NAV_MESH_DOUBLE_JUMP))
			continue;

		// Look at area->testArea connections
		for (NavAreaArray::IndexType_t index2 (0u); index2 < m_areas.GetElementNumber (); ++index2)
		{
			NavigationArea *const area (m_areas[index2]);

			if (area->HasAttributes (NAV_MESH_DOUBLE_JUMP) || !area->IsConnected (testArea))
				continue;

			if (NavAreaTravelDistance (testArea, area, ShortestPathCost ()) >= NavAreaTravelDistance (testAreaUpConnectedArea, area, ShortestPathCost ()) * 0.8f)
				unusedAreas += testArea;
		}
	}

	for (NavAreaArray::IndexType_t index (0u); index < unusedAreas.GetElementNumber (); ++index)
		DestroyArea (unusedAreas[index]);

	StripNavigationAreas ();

	SetMarkedArea (NULL);			// unmark the mark area
	m_markedCorner = NUM_CORNERS;	// clear the corner selection
#endif	// if 0
}

void NavigationMesh::RaiseAreasWithInternalObstacles (void)
{
#if 0
	// PURE ASSEMBLER :(
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const area (m_areas[index]);

		for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
		{
			const NavAreaConnectArray &areaList (area->GetAdjacentList (static_cast <NavDirType_t> (direction)));

			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < areaList.GetElementNumber (); ++index2)
			{
				NavigationArea *const areaConnect (areaList[index2]);

				//
			}
		}
	}
#endif	// if 0
}

void NavigationMesh::CreateObstacleTopAreas (void)
{
#if 0
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const area (m_areas[index]);

		if (area->HasAttributes (NAV_MESH_JUMP | NAV_MESH_OBSTACLE))
			continue;	/// @warning IN WINDOWS - break !!!

		for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
		{
			const NavAreaConnectArray &areaList (area->GetAdjacentList (static_cast <NavDirType_t> (direction)));

			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < areaList.GetElementNumber (); ++index2)
			{
				NavigationArea *const areaConnect (areaList[index2]);

				if (areaConnect->HasAttributes (NAV_MESH_JUMP | NAV_MESH_OBSTACLE))
					continue;

				for (NavAreaConnectArray::IndexType_t index3 (index2), index4 (0u); index3 > 0u && ++index4 < areaList.GetElementNumber (); --index3);

				NavigationArea *const someArea (index4 >= areaList.GetElementNumber () ? NULL : areaList[index4]);

				if (someArea != NULL/*! @note IN DECOMPILED CODE THIS CHECK IS NOT EXISTS!!! */ && !someArea->HasAttributes (NAV_MESH_JUMP | NAV_MESH_OBSTACLE))
					if (!CreateObstacleTopAreaIfNecessary (area, areaConnect, static_cast <NavDirType_t> (direction), false))
						CreateObstacleTopAreaIfNecessary (area, areaConnect, static_cast <NavDirType_t> (direction), true);
			}
		}
	}
#endif	// if 0
}

const bool NavigationMesh::CreateObstacleTopAreaIfNecessary (NavigationArea *const/* area*/, NavigationArea *const/* areaConnect*/, const NavDirType_t/* direction*/, const bool/* isUNKNOWN*/)
{
#if 0
	// PURE ASSEMBLER :(
	Math::Vector2D center;
	float halfWidth;

	area->ComputePortal (areaConnect, direction, center, &halfWidth);

	if (halfWidth <= 0.0f || area->m_nodes[direction] == NULL)
		return false;

	NavigationNode *node (area->m_nodes[direction]);

	do
	{
		if (isUNKNOWN)
		{
			if (areaConnect->Contains (node) || areaConnect->m_to[direction] == NULL)
			{
				//
			}
		}
		else
		{
			//
		}
	} while (node != NULL);

	if (isUNKNOWN)
		v31 = 50.0;
	else
		v31 = 25.0;

	AdjustObstacleDistances (, , );

	AddDirectionVector (, direction, );

	switch (direction)
	{
		case NORTH:
		case SOUTH:
			//

			break;

		case EAST:
		case WEST:
			//

			break;
	}

	v33 = (*(int (**)(void))(*(_DWORD *)v44 + 12))();
	a5 = v33;
	CNavArea__Build(v33, (int)&v59, (int)&v56, (int)&v53, (int)&v50);
	sub_102E92C0_CUtlVector__InsertBefore((int)&dword_1056F2E4, dword_1056F2F0, (int)&a5);
	CNavMesh__AddNavArea(v44, v33);
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint("Created new fencetop area %d(%x) between %d(%x) and %d(%x)\n",
		*(_DWORD *)(v33 + 136),
		*(_DWORD *)(v33 + 140),
		*(_DWORD *)(v5 + 136),
		*(_DWORD *)(v5 + 140),
		*(_DWORD *)(v6 + 136),
		*(_DWORD *)(v6 + 140));
	*(_DWORD *)(v33 + 84) = *(_DWORD *)(v5 + 84);
	*(_DWORD *)(v33 + 84) = *(_DWORD *)(v5 + 84) | 0x6000;
	v5->CNavArea__Disconnect (v6);
	v5->CNavArea__ConnectTo (v33, _EDI);
	v32 = GetOppositeDirection (_EDI);
	v33->CNavArea__ConnectTo (v5, v32);
	v33->CNavArea__ConnectTo (a3, _EDI);
	if (a3->CNavArea__IsConnected (v5, v32))
	{
		a3->CNavArea__Disconnect (v5);
		a3->CNavArea__ConnectTo (v33, v32);
	}
#endif	// if 0
	return true;
}

void NavigationMesh::RemoveOverlappingObstacleTopAreas (void)
{
	/// @todo !!!
}

void NavigationMesh::FixCornerOnCornerAreas (void)
{
	/// @todo !!!
}

void NavigationMesh::FixConnections (void)
{
	/// @todo !!!
}

//--------------------------------------------------------------------------------------------------------------
/**
* Recursively chop area in half along X until child areas are roughly square.
*/
void NavigationMesh::NavigationArea::SplitX (void)
{
	if (IsRoughlySquare ())
		return;

	const float split (g_server->GetNavigationMeshManager ().SnapToGrid (m_extent.GetSizeX () * 0.5f + m_extent.mins.x));

	if (Math::AreEqual (split, m_extent.mins.x, Math::RawEpsilon) || Math::AreEqual (split, m_extent.maxs.x, Math::RawEpsilon))
		return;	// too small to subdivide

	NavigationArea *alpha, *beta;

	if (SplitEdit (false, split, &alpha, &beta))
	{
		// split each new area until square
		alpha->SplitX ();
		beta->SplitX ();
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
* Recursively chop area in half along Y until child areas are roughly square.
*/
void NavigationMesh::NavigationArea::SplitY (void)
{
	if (IsRoughlySquare ())
		return;

	const float split (g_server->GetNavigationMeshManager ().SnapToGrid (m_extent.GetSizeY () * 0.5f + m_extent.mins.y));

	if (Math::AreEqual (split, m_extent.mins.y, Math::RawEpsilon) || Math::AreEqual (split, m_extent.maxs.y, Math::RawEpsilon))
		return;	// too small to subdivide

	NavigationArea *alpha, *beta;

	if (SplitEdit (true, split, &alpha, &beta))
	{
		// split each new area until square
		alpha->SplitY ();
		beta->SplitY ();
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
* Split any long, thin, areas into roughly square chunks.
*/
void NavigationMesh::SquareUpAreas (void)
{
	AddLogEntry (true, LogLevel_Default, false, "Splitting navigation areas...");

	// move the index in case the current area is split and deleted
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const area (m_areas[index]);

		if (!area->HasNodes () || area->IsRoughlySquare ())
			continue;

		// chop this area into square pieces
		if (area->GetExtent ().GetSizeX () > area->GetExtent ().GetSizeY ())
			area->SplitX ();
		else
			area->SplitY ();
	}

	AddLogEntry (true, LogLevel_Default, false, "Splitting navigation areas...DONE!");
}


//--------------------------------------------------------------------------------------------------------------
/**
* Determine if we can "jump down" from given point
*/
const bool NavigationMesh::TestJumpDown (const NavigationArea *const downArea, const Math::Vector3D &fromPos, const Math::Vector3D &toPos)
{
	const float dz (fromPos.z - toPos.z);

	// drop can't be too far, or too short (or nonexistant)
	if (dz <= HalfLifeEngine::SDK::Constants::JumpCrouchHeight || (dz >= HalfLifeEngine::SDK::Constants::DeathDrop && !downArea->IsUnderwater ()))
		return false;

	//
	// Check LOS out and down
	//
	// ----->+
	// From  |
	//       |
	//       |
	//       V
	//       To
	//
#if 0
	Math::Vector3D from (fromPos.x, fromPos.y, fromPos.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight/*HumanHeight - original, but I think it's wrong.... (BY EDWARD)*/);
	Math::Vector3D to (toPos.x, toPos.y, from.z);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceLine (from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//	if (traceResult.isStartSolid || traceResult.fraction < 1.0f)
//		return false;

	// CHANGED TraceLine() to TraceHull() BY EDWARD!!!!!!!!!!!!!!!!!
	if (!Math::AreEqual2 (fromPos.x, toPos.x, HalfLifeEngine::SDK::Constants::HalfHumanWidth) || !Math::AreEqual2 (fromPos.y, toPos.y, HalfLifeEngine::SDK::Constants::HalfHumanWidth))
	{
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (from, to, true, HalfLifeEngine::SDK::Constants::Hull_Head, NULL, traceResult);

//		if (traceResult.isStartSolid || traceResult.fraction < 1.0f)
		if (!TraceHullThruWalkableEntities (from, to, true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult))
			return false;
	}

	from = to;
	to.z = toPos.z + 2.0f;

//	UTIL_TraceLine (from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

	// ADDED BY EDWARD FOR TraceHull()!!!!!!!!!!!!!!!!!
	to.z += HalfLifeEngine::SDK::Constants::HalfHumanHeight;

	// CHANGED TraceLine() to TraceHull() BY EDWARD!!!!!!!!!!!!!!!!!
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (from, to, true, HalfLifeEngine::SDK::Constants::Hull_Head, NULL, traceResult);

//	return !traceResult.isStartSolid && traceResult.fraction == 1.0f;
	return TraceHullThruWalkableEntities (from, to, true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
#endif	// if 0
/// FROM NEW SOURCE ENGINE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	float offsetZ (1.0f);
	Math::Vector3D from, to;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		from = to = fromPos;

		to.z += offsetZ;

//		UTIL_TraceHull (from, to, NavTraceMins, NavTraceMaxs, GetGenerationTraceMask (), NULL, NULL, &traceResult);
		TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

		if (traceResult.fraction <= 0.0f || traceResult.isStartSolid)
			continue;

		from.x = fromPos.x;
		from.y = fromPos.y;
		from.z = traceResult.endPosition.z - 0.5f;
		to.x = toPos.x;
		to.y = toPos.y;
		to.z = from.z;

//		UTIL_TraceHull (from, to, NavTraceMins, NavTraceMaxs, GetGenerationTraceMask (), NULL, NULL, &traceResult);
		TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

		if (traceResult.fraction == 1.0f && !traceResult.isStartSolid)
			break;

		if (++offsetZ > HalfLifeEngine::SDK::Constants::JumpCrouchHeight)
			return false;
	}

	from = to;
	to.z = toPos.z + 2.0f;

//	UTIL_TraceHull (from, to, NavTraceMins, NavTraceMaxs, GetGenerationTraceMask (), NULL, NULL, &traceResult);
	TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

	return traceResult.fraction > 0.0f && !traceResult.isStartSolid && to.z + HalfLifeEngine::SDK::Constants::StepHeight >= traceResult.endPosition.z;
//#endif	// if 0
}

//--------------------------------------------------------------------------------------------------------------
NavigationMesh::NavigationArea *const NavigationMesh::FindJumpDownArea (const Math::Vector3D &fromPos, const NavDirType_t direction) const
{
	Math::Vector3D start (fromPos.x, fromPos.y, fromPos.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight);

	AddDirectionVector (start, direction, GenerationStepSize * 0.5f);

	Math::Vector3D toPos;
	NavigationArea *const downArea (FindFirstAreaInDirection (start, direction, 4.0f * GenerationStepSize, HalfLifeEngine::SDK::Constants::MapSize/*DeathDrop - ORIGINAL*/, NULL, &toPos));
//** ORIGINAL CODE BLOCK
	if (downArea != NULL && TestJumpDown (downArea, fromPos, toPos))
		return downArea;
#if 0
{/** NEW CODE BLOCK */
	if (downArea != NULL)
	{
		// ADDED BY EDWARD FOR TraceHull()!!!!!!!!!!!!!!!!!
		AddDirectionVector (toPos, direction, HalfLifeEngine::SDK::Constants::HalfHumanWidth);

		if (TestJumpDown (downArea, fromPos, toPos))
			return downArea;
	}
}
#endif	// if 0
	return NULL;
}


//--------------------------------------------------------------------------------------------------------------
/**
* Define connections between adjacent generated areas
*/
void NavigationMesh::ConnectGeneratedAreas (void)
{
	AddLogEntry (true, LogLevel_Default, false, "Connecting navigation areas...");

	const NavigationNode *node;

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const area (m_areas[index]);

		// scan along edge nodes, stepping one node over into the next area
		// for now, only use bi-directional connections

		// north edge
		for (node = area->m_node[NORTH_WEST]; node != area->m_node[NORTH_EAST]; node = node->GetConnectedNode (EAST))
		{
			const NavigationNode *const adj (node->GetConnectedNode (NORTH));

			if (adj != NULL && adj->GetArea () != NULL && adj->GetConnectedNode (SOUTH) == node)
				area->ConnectTo (adj->GetArea (), NORTH);
			else
			{
				NavigationArea *const downArea (FindJumpDownArea (node->GetPosition (), NORTH));

				if (downArea != NULL && downArea != area)
					area->ConnectTo (downArea, NORTH);
			}
		}

		// west edge
		for (node = area->m_node[NORTH_WEST]; node != area->m_node[SOUTH_WEST]; node = node->GetConnectedNode (SOUTH))
		{
			const NavigationNode *const adj (node->GetConnectedNode (WEST));

			if (adj != NULL && adj->GetArea () != NULL && adj->GetConnectedNode (EAST) == node)
				area->ConnectTo (adj->GetArea (), WEST);
			else
			{
				NavigationArea *const downArea (FindJumpDownArea (node->GetPosition (), WEST));

				if (downArea != NULL && downArea != area)
					area->ConnectTo (downArea, WEST);
			}
		}

		// south edge - this edge's nodes are actually part of adjacent areas
		// move one node north, and scan west to east
		/// @todo This allows one-node-wide areas - do we want this?
		node = area->m_node[SOUTH_WEST];
		if (node != NULL)	// pre-existing areas in incremental generates won't have nodes
			node = node->GetConnectedNode (NORTH);

		if (node != NULL)
		{
			/// @todo Figure out why cs_backalley gets a NULL node in here...
			for (const NavigationNode *const end (area->m_node[SOUTH_EAST]->GetConnectedNode (NORTH)); node != NULL && node != end; node = node->GetConnectedNode (EAST))
			{
				const NavigationNode *const adj (node->GetConnectedNode (SOUTH));

				if (adj != NULL && adj->GetArea () != NULL && adj->GetConnectedNode (NORTH) == node)
					area->ConnectTo (adj->GetArea (), SOUTH);
				else
				{
					NavigationArea *const downArea (FindJumpDownArea (node->GetPosition (), SOUTH));

					if (downArea != NULL && downArea != area)
						area->ConnectTo (downArea, SOUTH);
				}
			}
		}
{//!!! FROM NEW SOURCE ENGINE !!!
		// south edge
		for (node = area->m_node[SOUTH_WEST]; node != area->m_node[SOUTH_EAST]; node = node->GetConnectedNode (EAST))
		{
			if (node->GetArea () != NULL || node->m_crouch[NORTH] || node->m_crouch[EAST] || node->m_crouch[SOUTH] || node->m_crouch[WEST])
				continue;

			const NavigationNode *const adj (node->GetConnectedNode (SOUTH));

			if (adj != NULL && (adj->m_crouch[NORTH] || adj->m_crouch[EAST] || adj->m_crouch[SOUTH] || adj->m_crouch[WEST] || adj->GetArea () != NULL))
				continue;

			NavigationArea *const downArea (FindJumpDownArea (node->GetPosition (), SOUTH));

			if (downArea != NULL && downArea != area)
				area->ConnectTo (downArea, SOUTH);
		}
}
		// east edge - this edge's nodes are actually part of adjacent areas
		node = area->m_node[NORTH_EAST];
		if (node != NULL)	// pre-existing areas in incremental generates won't have nodes
			node = node->GetConnectedNode (WEST);

		if (node != NULL)
			for (const NavigationNode *const end (area->m_node[SOUTH_EAST]->GetConnectedNode (WEST)); node != NULL && node != end; node = node->GetConnectedNode (SOUTH))
			{
				const NavigationNode *const adj (node->GetConnectedNode (EAST));

				if (adj != NULL && adj->GetArea () != NULL && adj->GetConnectedNode (WEST) == node)
					area->ConnectTo (adj->GetArea (), EAST);
				else
				{
					NavigationArea *const downArea (FindJumpDownArea (node->GetPosition (), EAST));

					if (downArea != NULL && downArea != area)
						area->ConnectTo (downArea, EAST);
				}
			}
{//!!! FROM NEW SOURCE ENGINE !!!
		// east edge
		for (node = area->m_node[NORTH_EAST]; node != area->m_node[SOUTH_EAST]; node = node->GetConnectedNode (SOUTH))
		{
			if (node->GetArea () != NULL || node->m_crouch[NORTH] || node->m_crouch[EAST] || node->m_crouch[SOUTH] || node->m_crouch[WEST])
				continue;

			const NavigationNode *const adj (node->GetConnectedNode (EAST));

			if (adj != NULL && (adj->m_crouch[NORTH] || adj->m_crouch[EAST] || adj->m_crouch[SOUTH] || adj->m_crouch[WEST] || adj->GetArea () != NULL))
				continue;

			NavigationArea *const downArea (FindJumpDownArea (node->GetPosition (), EAST));

			if (downArea != NULL && downArea != area)
				area->ConnectTo (downArea, EAST);
		}
}
	}

	StitchGeneratedAreas ();

	AddLogEntry (true, LogLevel_Default, false, "Connecting navigation areas...DONE!");
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::StitchAreaIntoMesh_IncrementallyGeneratedAreas_ (NavigationArea *const/* area*/, const NavDirType_t/* direction*/)
{
	/// @todo THIS FUNCTION IS NOT FULLY DECOMPILED, SOME PARTS IS WRONG !!!
#if 0
	Math::Vector3D vector3D;
	Math::Vector2D vector2D;

	switch
	{
		case NORTH:
			vector3D.x = area->GetExtent ().mins.x;
			vector3D.y = area->GetExtent ().mins.y;
			vector3D.z = area->GetExtent ().mins.z;

			vector2D.x = area->GetExtent ().maxs.x;
			vector2D.y = area->GetExtent ().mins.y;

			break;

		case EAST:
			vector3D.x = area->GetExtent ().maxs.x;
			vector3D.y = area->GetExtent ().mins.y;
			vector3D.z = area->m_neZ;

			vector2D.x = area->GetExtent ().maxs.x;
			vector2D.y = area->GetExtent ().maxs.y;

			break;

		case SOUTH:
			vector3D.x = area->GetExtent ().mins.x;
			vector3D.y = area->GetExtent ().maxs.y;
			vector3D.z = m_swZ;

			vector2D.x = area->GetExtent ().maxs.x;
			vector2D.y = area->GetExtent ().maxs.y;

			break;

		default:	// WEST
			vector3D.x = area->GetExtent ().mins.x;
			vector3D.y = area->GetExtent ().mins.y;
			vector3D.z = area->GetExtent ().mins.z;

			vector2D.x = area->GetExtent ().mins.x;
			vector2D.y = area->GetExtent ().maxs.y;

			break;
	}

	Math::Vector3D directionVector (vector2D - vector3D);
	const float yValue (directionVector.y - 1.0f);

	if (yValue <= 0.0f)
		return;

	Math::Vector3D position1, position2;
	float length (0.0f);

	directionVector.Normalize ();

	do
	{
		const float tempLength (length + 0.5f);

		position1.x = position2.x = vector3D.x + directionVector.x * tempLength;
		position1.y = position2.y = vector3D.y + directionVector.y * tempLength;
		position1.z = vector3D.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight + directionVector.z * tempLength;

		AddDirectionVector (position2, direction, GenerationStepSize * 0.5f);

		NavigationArea *const nearArea (GetNavArea (position2));

		if (nearArea == NULL || nearArea->HasNodes ())
		{
			position1.z -= HalfLifeEngine::SDK::Constants::HalfHumanHeight;

			++position1.z;

			NavigationArea *const jumpDownArea (FindJumpDownArea (position1, direction));

			if (jumpDownArea != NULL && jumpDownArea != area && !jumpDownArea->HasNodes ())
				area->ConnectTo (jumpDownArea, direction);
		}
		else
		{
			position2.z = nearArea->GetZ (position2) + HalfLifeEngine::SDK::Constants::HalfHumanHeight;

			if (TestStitchConnection (position2, position1))
				area->ConnectTo (nearArea, direction);

			if (TestStitchConnection (position1, position2))
				nearArea->ConnectTo (area, OppositeDirection (direction));
		}
	} while ((length += GenerationStepSize) <= yValue);
#endif	// if 0
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::StitchAreaIntoMesh_IncrementallyGeneratedAreas (void)
{
	AddLogEntry (true, LogLevel_Default, false, "Stitching navigation areas...");

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		NavigationArea *const area (m_areas[index]);

		if (!area->HasNodes ())
			continue;

		StitchAreaIntoMesh_IncrementallyGeneratedAreas_ (area, NORTH);
		StitchAreaIntoMesh_IncrementallyGeneratedAreas_ (area, EAST);
		StitchAreaIntoMesh_IncrementallyGeneratedAreas_ (area, SOUTH);
		StitchAreaIntoMesh_IncrementallyGeneratedAreas_ (area, WEST);
	}

	AddLogEntry (true, LogLevel_Default, false, "Stitching navigation areas...DONE!");
}

//--------------------------------------------------------------------------------------------------------------
/**
* Merge areas together to make larger ones (must remain rectangular - convex).
* Areas can only be merged if their attributes match.
*/
void NavigationMesh::MergeGeneratedAreas (void)
{
	AddLogEntry (true, LogLevel_Default, false, "Merging navigation areas...");

	bool merged;
	const float maximumAreaSize (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_area_max_size)->GetValue <float> () * GenerationStepSize);

	do
	{
		merged = false;

		for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		{
			NavigationArea *const area (m_areas[index]);

			if (!area->IsAbleToMerge ())
				continue;

			// north edge
			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < area->m_connectedAreas[NORTH].GetElementNumber (); ++index2)
			{
				NavigationArea *const adjArea (area->m_connectedAreas[NORTH][index2]);

				// pre-existing areas in incremental generates won't have nodes
				if (!adjArea->IsAbleToMerge () || area->GetExtent ().GetSizeY () + adjArea->GetExtent ().GetSizeY () > maximumAreaSize)
					continue;

				if
				(
					area->m_node[NORTH_WEST] == adjArea->m_node[SOUTH_WEST] &&
					area->m_node[NORTH_EAST] == adjArea->m_node[SOUTH_EAST] &&
					area->GetAttributes () == adjArea->GetAttributes () &&
					area->IsCoplanar (adjArea)
				)
				{
					// merge vertical
					area->m_node[NORTH_WEST] = adjArea->m_node[NORTH_WEST];
					area->m_node[NORTH_EAST] = adjArea->m_node[NORTH_EAST];

					merged = true;
					//CONSOLE_ECHO ("  Merged (north) areas #%u and #%u\n", area->m_id, adjArea->m_id);

					area->FinishMerge (adjArea);

					// restart scan - iterator is invalidated
					break;
				}
			}

			if (merged)
				break;

			// south edge
			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < area->m_connectedAreas[SOUTH].GetElementNumber (); ++index2)
			{
				NavigationArea *const adjArea (area->m_connectedAreas[SOUTH][index2]);

				// pre-existing areas in incremental generates won't have nodes
				if (!adjArea->IsAbleToMerge () || area->GetExtent ().GetSizeY () + adjArea->GetExtent ().GetSizeY () > maximumAreaSize)
					continue;

				if
				(
					adjArea->m_node[NORTH_WEST] == area->m_node[SOUTH_WEST] &&
					adjArea->m_node[NORTH_EAST] == area->m_node[SOUTH_EAST] &&
					area->GetAttributes () == adjArea->GetAttributes () &&
					area->IsCoplanar (adjArea)
				)
				{
					// merge vertical
					area->m_node[SOUTH_WEST] = adjArea->m_node[SOUTH_WEST];
					area->m_node[SOUTH_EAST] = adjArea->m_node[SOUTH_EAST];

					merged = true;
					//CONSOLE_ECHO ("  Merged (south) areas #%u and #%u\n", area->m_id, adjArea->m_id);

					area->FinishMerge (adjArea);

					// restart scan - iterator is invalidated
					break;
				}

			}

			if (merged)
				break;

			// west edge
			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < area->m_connectedAreas[WEST].GetElementNumber (); ++index2)
			{
				NavigationArea *const adjArea (area->m_connectedAreas[WEST][index2]);

				// pre-existing areas in incremental generates won't have nodes
				if (!adjArea->IsAbleToMerge () || area->GetExtent ().GetSizeX () + adjArea->GetExtent ().GetSizeX () > maximumAreaSize)
					continue;

				if
				(
					area->m_node[NORTH_WEST] == adjArea->m_node[NORTH_EAST] &&
					area->m_node[SOUTH_WEST] == adjArea->m_node[SOUTH_EAST] &&
					area->GetAttributes () == adjArea->GetAttributes () &&
					area->IsCoplanar (adjArea)
				)
				{
					// merge horizontal
					area->m_node[NORTH_WEST] = adjArea->m_node[NORTH_WEST];
					area->m_node[SOUTH_WEST] = adjArea->m_node[SOUTH_WEST];

					merged = true;
					//CONSOLE_ECHO ("  Merged (west) areas #%u and #%u\n", area->m_id, adjArea->m_id);

					area->FinishMerge (adjArea);

					// restart scan - iterator is invalidated
					break;
				}

			}

			if (merged)
				break;

			// east edge
			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < area->m_connectedAreas[EAST].GetElementNumber (); ++index2)
			{
				NavigationArea *const adjArea (area->m_connectedAreas[EAST][index2]);

				// pre-existing areas in incremental generates won't have nodes
				if (!adjArea->IsAbleToMerge () || area->GetExtent ().GetSizeX () + adjArea->GetExtent ().GetSizeX () > maximumAreaSize)
					continue;

				if
				(
					adjArea->m_node[NORTH_WEST] == area->m_node[NORTH_EAST] &&
					adjArea->m_node[SOUTH_WEST] == area->m_node[SOUTH_EAST] &&
					area->GetAttributes () == adjArea->GetAttributes () &&
					area->IsCoplanar (adjArea)
				)
				{
					// merge horizontal
					area->m_node[NORTH_EAST] = adjArea->m_node[NORTH_EAST];
					area->m_node[SOUTH_EAST] = adjArea->m_node[SOUTH_EAST];

					merged = true;
					//CONSOLE_ECHO ("  Merged (east) areas #%u and #%u\n", area->m_id, adjArea->m_id);

					area->FinishMerge (adjArea);

					// restart scan - iterator is invalidated
					break;
				}
			}

			if (merged)
				break;
		}
	} while (merged);

	AddLogEntry (true, LogLevel_Default, false, "Merging navigation areas...DONE!");
}

void NavigationMesh::SplitAreasUnderOverhangs (void)
{
	/// @warning @todo CHECK ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	AddLogEntry (true, LogLevel_Default, false, "Splitting areas under overhangs...");

	bool done;	// v22

	for (;;)
	{
		done = false;

		for (NavAreaArray::IndexType_t index (0u); !done && index < m_areas.GetElementNumber (); ++index)
		{
			NavigationArea *const area (m_areas[index]);
			const Math::Extent3D &areaExtent (area->GetExtent ());
			NavigationArea *connectedOverlappingArea (NULL);	// v8

			for (unsigned char direction (NORTH); !done && direction < NUM_DIRECTIONS; ++direction)
			{
				for (NavAreaConnectArray::IndexType_t index2 (0u); !done && index2 < area->m_connectedAreas[direction].GetElementNumber (); ++index2)
				{
					NavigationArea *const connectedArea (area->m_connectedAreas[direction][index2]);
					const Math::Extent3D &connectedAreaExtent (connectedArea->GetExtent ());

					connectedOverlappingArea = connectedArea;

					if (!area->IsOverlapping (connectedArea))
						continue;

					if (areaExtent.mins.z <= connectedAreaExtent.maxs.z + 55.0f && areaExtent.maxs.z + 55.0f >= connectedAreaExtent.mins.z)
						continue;

					NavDirType_t direction1/* v10 */ (GetOppositeDirection (static_cast <NavDirType_t> (direction))), direction2/* v11 */;
					NavigationArea *someArea (area);	// v9

					if (areaExtent.mins.z > connectedAreaExtent.mins.z)
					{
						someArea = connectedOverlappingArea;
						connectedOverlappingArea = area;

						switch (direction1)
						{
							case NORTH:
								direction1 = SOUTH;

								break;

							case EAST:
								direction1 = WEST;

								break;

							case SOUTH:
								direction1 = NORTH;

								break;

							default:	// WEST
								direction1 = EAST;

								break;
						}
					}

					switch (direction1)
					{
						case NORTH:
							direction2 = SOUTH;

							break;

						case EAST:
							direction2 = WEST;

							break;

						case SOUTH:
							direction2 = NORTH;

							break;

						default:	// WEST
							direction2 = EAST;

							break;
					}

					bool splitAlongX (false);	// v31
					float someVectorX1/* v13 */, someVectorX2/* v14 */, someVector1/* v12 */, someVector2/* v15 */;

					if (direction1 == EAST || direction1 == WEST)
					{
						someVectorX1 = someArea->GetExtent ().mins.x;
						someVector1 = someArea->GetExtent ().maxs.x;
						someVector2 = connectedOverlappingArea->GetExtent ().maxs.x;
						someVectorX2 = someVector1 - someVectorX1;

						if (someVector2 >= someVector1)
						{
							someVectorX1 = connectedOverlappingArea->GetExtent ().mins.x;
							someVector2 = someVectorX1;
						}
						else
							someVector1 = someVector2;
					}
					else
					{
						splitAlongX = true;
						someVectorX1 = someArea->GetExtent ().mins.y;
						someVector1 = someArea->GetExtent ().maxs.y;
						someVector2 = connectedOverlappingArea->GetExtent ().maxs.y;
						someVectorX2 = someVector1 - someVectorX1;

						if (someVector2 < someVector1)
							someVector1 = someVector2;
						else
						{
							someVectorX1 = connectedOverlappingArea->GetExtent ().mins.y;
							someVector2 = someVectorX1;
						}
					}

					float splitEdge (someVector2);	// v25
					float v19 (someVector1 - someVectorX1);	// v19

					if (v19 >= GenerationStepSize)
					{
						LABEL_38:
						bool isSomeAreaConnectedWithConnectedOverlappingArea (false)/* v23 */, isConnectedOverlappingAreaConnectedWithSomeArea (false)/* v24 */;

						if (someArea->IsConnected (connectedOverlappingArea, direction2))
						{
							isSomeAreaConnectedWithConnectedOverlappingArea = true;

							someArea->Disconnect (connectedOverlappingArea);
						}

						if (connectedOverlappingArea->IsConnected (someArea, direction1))
						{
							isConnectedOverlappingAreaConnectedWithSomeArea = true;

							connectedOverlappingArea->Disconnect (someArea);
						}

						NavigationArea *alpha/* v34 */, *beta/* v29 */;

						if (someArea->SplitEdit (splitAlongX, splitEdge, &alpha, &beta))
						{
							NavigationArea *tempArea;	// v20

							if (direction1 == NORTH || (tempArea = beta, direction1 == WEST))
								tempArea = alpha;

							if (isConnectedOverlappingAreaConnectedWithSomeArea)
								connectedOverlappingArea->ConnectTo (tempArea, direction1);

							if (isSomeAreaConnectedWithConnectedOverlappingArea)
								switch (direction1)
								{
									case NORTH:
										someArea->ConnectTo (tempArea, SOUTH);

										break;

									case EAST:
										someArea->ConnectTo (tempArea, WEST);

										break;

									case SOUTH:
										someArea->ConnectTo (tempArea, NORTH);

										break;

									default:	// WEST
										someArea->ConnectTo (tempArea, EAST);

										break;
								}

							done = true;
						}
					}
					else if (v19 >= 7.5f && someVectorX2 > 50.0f)
					{
						char modif;

						if (direction1 == NORTH || (modif = 1, direction1 == WEST))
							modif = -1;

						splitEdge = someVector2 + (GenerationStepSize - v19) * modif;

						goto LABEL_38;
					}
				}
			}
		}

		if (!done)
			break;
	}

	AddLogEntry (true, LogLevel_Default, false, "Splitting areas under overhangs...DONE!");
}

//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::CheckObstacles (const NavigationNode *const/* node*/, const unsigned char width, const unsigned char height, const unsigned char/* x*/, const unsigned char/* y*/)
{
	if (width <= 1u && height <= 1u)
		return true;
#if 0
	const float maximumSize (HalfLifeEngine::SDK::Constants::StepHeight);

	if
	(
		(x > 0u          && node->m_UNKNOWN1[WEST]  > maximumSize) ||
		(y > 0u          && node->m_UNKNOWN1[NORTH] > maximumSize) ||
		(x < width  - 1u && node->m_UNKNOWN1[EAST]  > maximumSize) ||
		(y < height - 1u && node->m_UNKNOWN1[SOUTH] > maximumSize)
	)
		return false;
#endif	// if 0
	return true;
}
#if 0
const bool NavigationMesh::TestForValidCrouchArea (const Math::Vector3D &position)
{
	CTraceFilterWalkableEntities traceFilter (NULL/*?*/, COLLISION_GROUP_NONE/*?*/, WALK_THRU_EVERYTHING/*?*/);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	UTIL_TraceHull (position, position + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::JumpCrouchHeight), Math::Vector3D (0.0f, 0.0f, 0.0f), Math::Vector3D (GenerationStepSize, GenerationStepSize, HalfLifeEngine::SDK::Constants::DuckHumanHeight/*?*/), GetGenerationTraceMask (), &traceFilter, &traceResult);

	return traceResult.isAllSolid == false;
}
#endif	// if 0
//--------------------------------------------------------------------------------------------------------------
/**
* Check if an rectangular area of the given size can be
* made starting from the given node as the NW corner.
* Only consider fully connected nodes for this check.
* All of the nodes within the test area must have the same attributes.
* All of the nodes must be approximately co-planar w.r.t the NW node's normal, with the
* exception of 1x1 areas which can be any angle.
*/
const bool NavigationMesh::TestArea (const NavigationNode *const nwNode, const unsigned char width, const unsigned char height) const
{
	// Reliability checks.
	InternalAssert (width > 0u && height > 0u);

	const bool nodeCrouch (nwNode->m_crouch[SOUTH_EAST]);
{/*! FROM NEW SOURCE ENGINE !*/
	// Quick check....
	if (nodeCrouch)
		return false;
}
	const Math::Vector3D &normal (nwNode->GetNormal ());
	const float dot (-(nwNode->GetPosition () | normal));
	const bool isAreaSizeMoreThanOne (width > 1u || height > 1u);
	const NavAttributeFlag_t nodeAttributes (nwNode->GetAttributes () & ~NAV_MESH_CROUCH);
	const float offPlaneTolerance (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_NavigationMesh_MaximumOffPlaneTolerance)->GetValue <float> ());
	const NavigationNode *vertNode (nwNode), *horizNode;
	unsigned char x;

	for (unsigned char y (0u); y < height; ++y)
	{
		const bool northEdge (y == 0u);
		const bool southEdge (y == height - 1u);

		horizNode = vertNode;

		for (x = 0u; x < width; ++x)
		{
			// Compute the crouch attributes for the test node, taking into account only the side(s) of the node that are in the area
			const bool westEdge (x == 0u);
			const bool eastEdge (x == width - 1u);
			const bool horizNodeCrouch
			(
				// Check corners first
				northEdge && westEdge ? horizNode->m_crouch[SOUTH_EAST] :
				northEdge && eastEdge ? horizNode->m_crouch[SOUTH_EAST] || horizNode->m_crouch[SOUTH_WEST] :
				southEdge && westEdge ? horizNode->m_crouch[SOUTH_EAST] || horizNode->m_crouch[NORTH_EAST] :
				southEdge && eastEdge ? horizNode->HasAttributes (NAV_MESH_CROUCH) :

				// check sides next
				northEdge ? horizNode->m_crouch[SOUTH_EAST] || horizNode->m_crouch[SOUTH_WEST] :
				southEdge ? horizNode->HasAttributes (NAV_MESH_CROUCH) :
				eastEdge ? horizNode->HasAttributes (NAV_MESH_CROUCH) :
				westEdge ? horizNode->m_crouch[SOUTH_EAST] || horizNode->m_crouch[NORTH_EAST] :

				// finally, we have a center node
				horizNode->HasAttributes (NAV_MESH_CROUCH)
			);
			const bool horizNodeCrouch_OPT
			(
				// Check corners first
				northEdge ? westEdge ? horizNode->m_crouch[SOUTH_EAST] : horizNode->m_crouch[SOUTH_EAST] || horizNode->m_crouch[SOUTH_WEST] :

				// check sides next
				westEdge ? horizNode->m_crouch[SOUTH_EAST] || horizNode->m_crouch[NORTH_EAST] :

				// finally, we have a center node
				horizNode->HasAttributes (NAV_MESH_CROUCH)
			);

			InternalAssert (horizNodeCrouch == horizNodeCrouch_OPT);

			// all nodes must be crouch/non-crouch
			if (nodeCrouch != horizNodeCrouch)
				return false;

			// all nodes must have the same non-crouch attributes
			if ((horizNode->GetAttributes () & ~NAV_MESH_CROUCH) != nodeAttributes)
				return false;

			if (horizNode->IsCovered () || !horizNode->IsClosedCell ())
				return false;

			// nodes must lie on/near the plane
			if ((horizNode = horizNode->GetConnectedNode (EAST)) == NULL || (isAreaSizeMoreThanOne && /*!Math::AreEqual2 (horizNode->GetPosition ().z, nwNode->GetPosition ().z, HalfLifeEngine::SDK::Constants::StepHeight)))/*/Math::fabsf ((horizNode->GetPosition () | normal/*/horizNode->GetNormal ()*/) + dot) > offPlaneTolerance))
				return false;
		}

		// nodes must lie on/near the plane
		if ((vertNode = vertNode->GetConnectedNode (SOUTH)) == NULL || (isAreaSizeMoreThanOne && /*!Math::AreEqual2 (vertNode->GetPosition ().z, nwNode->GetPosition ().z, HalfLifeEngine::SDK::Constants::StepHeight)))/*/Math::fabsf ((vertNode->GetPosition () | normal/*/vertNode->GetNormal ()*/) + dot) > offPlaneTolerance))
			return false;
	}

	// check planarity of southern edge
	if (isAreaSizeMoreThanOne)
	{
		horizNode = vertNode;

		for (x = 0u; x < width; ++x)
		{
			// nodes must lie on/near the plane
			if ((horizNode = horizNode->GetConnectedNode (EAST)) == NULL || /*!Math::AreEqual2 (horizNode->GetPosition ().z, nwNode->GetPosition ().z, HalfLifeEngine::SDK::Constants::StepHeight))/*/Math::fabsf ((horizNode->GetPosition () | normal/*/horizNode->GetNormal ()*/) + dot) > offPlaneTolerance)
				return false;
		}
	}
#if 0
{/*! FROM NEW SOURCE ENGINE !*/
	const NavigationNode *testNode1 (nwNode);
	const NavigationNode *testNode2 (nwNode);

	for (y = 0u; y < height; ++y)
	{
		testNode2 = testNode1;

		for (x = 0u; x < width; ++x)
		{
			if (nwNode->m_crouch2[SOUTH_EAST] && !TestForValidCrouchArea (testNode2))
				return false;

			testNode2 = testNode2->m_to[NORTH_EAST];
		}

		testNode1 = testNode1->m_to[SOUTH_EAST];
	}

	if (m_generationMode == GENERATE_INCREMENTAL)
	{
		for (y = 0u; y < height; ++y)
			testNode1 = testNode1->m_to[SOUTH_EAST];

		if (SOME_FUNC (nwNode))
			return false;
	}
}
#endif	// if 0
	return true;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Create a navigation area, and mark all nodes it overlaps as "covered"
* NOTE: Nodes on the east and south edges are not included.
* Returns number of nodes covered by this area, or 0 for error;
*/
const unsigned short NavigationMesh::BuildArea (NavigationNode *const nwNode, const unsigned char width, const unsigned char height)
{
	NavigationNode *neNode (NULL);
	NavigationNode *seNode (NULL);
	NavigationNode *swNode (NULL);
	NavigationNode *vertNode (nwNode);
	NavigationNode *horizNode;
	unsigned short coveredNodes (0u);
	unsigned char x;

	for (unsigned char y (0u); y < height; ++y)
	{
		horizNode = vertNode;

		for (x = 0u; x < width; ++x)
		{
			horizNode->Cover ();

			++coveredNodes;

			horizNode = horizNode->GetConnectedNode (EAST);
		}

		if (y == 0u)
			neNode = horizNode;

		vertNode = vertNode->GetConnectedNode (SOUTH);
	}

	swNode = horizNode = vertNode;

	for (x = 0u; x < width; ++x)
		horizNode = horizNode->GetConnectedNode (EAST);

	seNode = horizNode;

	#if defined _DEBUG
		if (neNode == NULL || seNode == NULL || swNode == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "BuildArea - NULL node.");

			return 0u;
		}
	#endif	// if defined _DEBUG

	NavigationArea *const area (new NavigationArea (nwNode, neNode, seNode, swNode));

	#if defined _DEBUG
		// Reliability check.
		if (area == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (*area), nwNode->GetPosition ().GetMidPoint (seNode->GetPosition ()).ToString ());

			return 0u;
		}
	#endif	// if defined _DEBUG

	m_areas += area;

	// since all internal nodes have the same attributes, set this area's attributes
	area->SetAttributes (nwNode->GetAttributes ());
/*! FROM NEW SOURCE ENGINE !!!
	const float maximumSize (HalfLifeEngine::SDK::Constants::StepHeight);

	if
	(
		nwNode->m_UNKNOWN1[EAST]  > maximumSize ||
		nwNode->m_UNKNOWN1[SOUTH] > maximumSize ||

		neNode->m_UNKNOWN1[SOUTH] > maximumSize ||
		neNode->m_UNKNOWN1[WEST]  > maximumSize ||

		seNode->m_UNKNOWN1[NORTH] > maximumSize ||
		seNode->m_UNKNOWN1[WEST]  > maximumSize ||

		swNode->m_UNKNOWN1[NORTH] > maximumSize ||
		swNode->m_UNKNOWN1[EAST]  > maximumSize
	)
		area->AddAttributes (NAV_MESH_UNMERGEABLE);
*/
	// Check that the node was crouch in the right direction
	if (area->HasAttributes (NAV_MESH_CROUCH) && !nwNode->m_crouch[SOUTH_EAST])
		area->RemoveAttributes (NAV_MESH_CROUCH);

	return coveredNodes;
}


//--------------------------------------------------------------------------------------------------------------
/**
* This function uses the NavigationNodes that have been sampled from the map to
* generate NavigationAreas - rectangular areas of "walkable" space. These areas
* are connected to each other, proving information on know how to move from area to area.
*
* This is a "greedy" algorithm that attempts to cover the walkable area
* with the fewest, largest, rectangles.
*/
void NavigationMesh::CreateNavAreasFromNodes (void)
{
	int uncoveredNodes (m_nodes.GetElementNumber ());

	// haven't yet seen a map use larger than 30...
	for
	(
		unsigned char tryWidth (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_area_max_size)->GetValue <unsigned char> ()),
		              tryHeight (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_area_max_size)->GetValue <unsigned char> ());

		uncoveredNodes > 0 && tryWidth > 0u/* && tryHeight > 0u*/;

		/* Empty */
	)
	{
		for (const NavNodeStack::NodeType_t *node (m_nodes.GetFirst ()); node != NULL; node = node->GetNext ())
		{
			NavigationNode *const navigationNode (node->GetData ());

			if (navigationNode->IsCovered () || !TestArea (navigationNode, tryWidth, tryHeight))
				continue;

			const unsigned short covered (BuildArea (navigationNode, tryWidth, tryHeight));

			#if defined _DEBUG
				if (covered == 0u)
				{
					AddLogEntry (true, LogLevel_Error, false, "Generate: Error - Data corrupt.");

					return;
				}
			#endif	// if defined _DEBUG

			uncoveredNodes -= covered;
		}

		if (tryWidth >= tryHeight)
			--tryWidth;
		else
		{
			--tryHeight;

			InternalAssert (tryHeight > 0u);
		}
	}

	if (!AllocateGrid ())
		return;

	ConnectGeneratedAreas ();
	MarkJumpAreas ();
	MergeGeneratedAreas ();
	SplitAreasUnderOverhangs ();

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_NavigationMesh_SquareUpAreas)->GetValue <bool> ())
		SquareUpAreas ();

	MarkStairAreas ();
//	StichAreas ();
	RemoveJumpAreas ();
//	HandleObstacleTopAreas ();
//	FixUpGeneratedAreas ();
	BuildOverlapAreas ();

	/// @TODO: incremental generation doesn't create ladders yet
	if (m_generationMode != GENERATE_INCREMENTAL)
		ConnectGeneratedLadders ();

	YaPB::RebuildAreaMapZones ();
}

//--------------------------------------------------------------------------------------------------------------
/**
* Initiate the generation process
*/
void NavigationMesh::BeginGeneration (const bool incremental/* = false*/)
{
/*	IGameEvent *const event (gameeventmanager->CreateEvent ("nav_generate"));

	if (event != NULL)
		gameeventmanager->FireEvent (event);
MADE BY EDWARD!!!!!!!!*/
	if (g_server->GetYaPBManager ()->GetBotsCount () > 0u)
		g_server->GetYaPBManager ()->KickAll ();

	// Right now, incrementally-generated areas won't connect to existing areas automatically.
	// Since this means hand-editing will be necessary, don't do a full analyze.
	if (incremental)
		Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_quicksave)->SetValue <bool> (true);

	m_generationState = SAMPLE_WALKABLE_SPACE;
	m_maximumSampleTicks = static_cast <unsigned int> (m_worldMap.GetBSPFile ().models[0u].boundingBox.GetArea2D () / MATH_GET_SQUARED (GenerationStepSize));
	m_sampleTick = 0u;
	m_generationMode = incremental ? GENERATE_INCREMENTAL : GENERATE_FULL;
	generationStartTime = g_commonTimer.GetSeconds <float> ();

	// clear any previous mesh
	DestroyNavigationMesh (incremental);

	SetNavPlace (UNDEFINED_PLACE);

	// build internal representations of ladders, which are used to find new walkable areas
	if (!incremental)	///< @incremental update doesn't build ladders to avoid overlapping existing ones
	{
/*FROM NEW SOURCE ENGINE - INSTEAD OF ALL BELOW!!!!!
		DestroyLadders ();
		AddWalkableSeeds ();
*/
		BuildLadders ();

		// start sampling from a spawn point
		const Server::SpawnPointsContainer::SpawnPointsArray &playerSpawnPoints (g_server->GetSpawnPointsContainer ()[GetPlayersSpawnPointsTeam ()]);

		if (!playerSpawnPoints.IsEmpty ())
		{
			Math::Vector3D pos (playerSpawnPoints.GetRandomElement ());
			Math::Vector3D normal;

			// snap it to the sampling grid
			pos.x = SnapToGrid (pos.x);
			pos.y = SnapToGrid (pos.y);

			if (GetGroundHeight (pos, &pos.z, &normal))
			{
				m_currentNode = new NavigationNode (pos, normal);

				#if defined _DEBUG
					// Reliability check.
					if (m_currentNode == NULL)
						AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation node at location: %s!", sizeof (NavigationNode), pos.ToString ());
				#endif	// if defined _DEBUG
			}
		}
		else
		{
			// the system will see this NULL and select the next walkable seed
			m_currentNode = NULL;
		}
	}
	else
	{
		// the system will see this NULL and select the next walkable seed
		m_currentNode = NULL;
	}

	if (!m_walkableSeeds.IsEmpty ())
		m_seedIndex = 0u;	// initialize seed list index
	else
	{
		if (m_currentNode == NULL)	// if there are no seed and player spawn points, we can't generate
		{
			m_generationMode = GENERATE_NONE;

			AddLogEntry (true, LogLevel_Default, false, "No valid walkable seed positions. Cannot generate Navigation Mesh.");

			return;
		}

		m_seedIndex = m_walkableSeeds.BadPosition;	// initialize seed list index
	}

	AddLogEntry (true, LogLevel_Default, false, "Generating Navigation Mesh...");
}


//--------------------------------------------------------------------------------------------------------------
/**
* Re-analyze an existing Mesh. Determine Hiding Spots, Encounter Spots, etc.
*/
void NavigationMesh::BeginAnalysis (void)
{
	DestroyHidingSpots ();

	m_generationState = FIND_HIDING_SPOTS;
	m_generationIndex = 0u;
	m_generationMode = GENERATE_ANALYSIS_ONLY;
	generationStartTime = g_commonTimer.GetSeconds <float> ();
}

//--------------------------------------------------------------------------------------------------------------
const unsigned char  maximumTrailingDots (GET_STATIC_STRING_LENGTH ("....") + 1u);
static unsigned char currentTrailingDotIndex (maximumTrailingDots);
static const char    trailingDots[maximumTrailingDots][maximumTrailingDots] = {"    ", ".   ", "..  ", "... ", "...."};

void NavigationMesh::ShowAnalysisProgress (const char *const message, const unsigned char currentTick)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_BROADCAST, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotProgress), NULL, NULL);
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (HalfLifeEngine::SDK::Constants::ProgressBarType_Update);	// Flag (update)
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (currentTick);	// Progress
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteString (FormatBuffer ("%s%s", message, trailingDots[++currentTrailingDotIndex %= maximumTrailingDots]));	// Header
	HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();
}
void NavigationMesh::ShowAnalysisProgressWithPercent (const char *const message, const unsigned char currentTick)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_BROADCAST, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotProgress), NULL, NULL);
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (HalfLifeEngine::SDK::Constants::ProgressBarType_Update);	// Flag (update)
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (currentTick);	// Progress
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteString (FormatBuffer ("%s%s %u%%", message, trailingDots[++currentTrailingDotIndex %= maximumTrailingDots], currentTick));	// Header
	HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::HideAnalysisProgress (void)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_BROADCAST, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotProgress), NULL, NULL);
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (HalfLifeEngine::SDK::Constants::ProgressBarType_Remove);	// Flag (kill)
	HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();
}


//--------------------------------------------------------------------------------------------------------------
/**
* Process the auto-generation for 'maxTime' seconds. return false if generation is complete.
*/
const bool NavigationMesh::UpdateGeneration (const float maxTime/* = 0.25f*/)
{
	const float startTime (g_commonTimer.GetSeconds <float> ());

	switch (m_generationState)
	{
		//---------------------------------------------------------------------------
		case SAMPLE_WALKABLE_SPACE:
		{
			while (SampleStep ())
				if (g_commonTimer.GetSeconds <float> () - startTime > maxTime)	// don't go over our time allotment
				{
					// increase the current sample tick and show pseudo progress....
					ShowAnalysisProgressWithPercent ("Sampling walkable space", static_cast <unsigned char> (100u * m_sampleTick / m_maximumSampleTicks));

					return true;
				}

			// sampling is complete, now build navigation areas
			m_generationState = CREATE_AREAS_FROM_SAMPLES;

			creatingNavigationAreasFromSampledDataStartTime = g_commonTimer.GetSeconds <float> ();

			AddLogEntry (true, LogLevel_Default, false, "Sampling walkable space...DONE! Elapsed time is %.2f sec.", creatingNavigationAreasFromSampledDataStartTime - generationStartTime);

			return true;
		}

		//---------------------------------------------------------------------------
		case CREATE_AREAS_FROM_SAMPLES:
		{
			AddLogEntry (true, LogLevel_Default, false, "Creating navigation areas from sampled data...");

			CreateNavAreasFromNodes ();

			m_generationState = SAVE_NAV_MESH;//FIND_HIDING_SPOTS;
			m_generationIndex = 0u;

			findingHidingSpotsStartTime = g_commonTimer.GetSeconds <float> ();

			AddLogEntry (true, LogLevel_Default, false, "Creating navigation areas from sampled data...DONE! Elapsed time is %.2f sec.", findingHidingSpotsStartTime - creatingNavigationAreasFromSampledDataStartTime);

			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_HIDING_SPOTS:
		{
			while (m_generationIndex < m_areas.GetElementNumber ())
			{
				NavigationArea *const area (m_areas[m_generationIndex]);

				++m_generationIndex;

				area->ComputeHidingSpots ();

				// don't go over our time allotment
				if (g_commonTimer.GetSeconds <float> () - startTime > maxTime)
				{
					ShowAnalysisProgressWithPercent ("Finding hiding spots", static_cast <unsigned char> (100u * m_generationIndex / m_areas.GetElementNumber ()));

					return true;
				}
			}

			m_generationState = FIND_APPROACH_AREAS;
			m_generationIndex = 0u;

			findingApproachAreasStartTime = g_commonTimer.GetSeconds <float> ();

			AddLogEntry (true, LogLevel_Default, false, "Finding hiding spots...DONE! Elapsed time is %.2f sec.", findingApproachAreasStartTime - findingHidingSpotsStartTime);

			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_APPROACH_AREAS:
		{
			while (m_generationIndex < m_areas.GetElementNumber ())
			{
				NavigationArea *const area (m_areas[m_generationIndex]);

				++m_generationIndex;

				area->ComputeApproachAreas ();

				// don't go over our time allotment
				if (g_commonTimer.GetSeconds <float> () - startTime > maxTime)
				{
					ShowAnalysisProgressWithPercent ("Finding approach areas", static_cast <unsigned char> (100u * m_generationIndex / m_areas.GetElementNumber ()));

					return true;
				}
			}

			m_generationState = FIND_ENCOUNTER_SPOTS;
			m_generationIndex = 0u;

			findingEncounterSpotsStartTime = g_commonTimer.GetSeconds <float> ();

			AddLogEntry (true, LogLevel_Default, false, "Finding approach areas...DONE! Elapsed time is %.2f sec.", findingEncounterSpotsStartTime - findingApproachAreasStartTime);

			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_ENCOUNTER_SPOTS:
		{
			while (m_generationIndex < m_areas.GetElementNumber ())
			{
				NavigationArea *const area (m_areas[m_generationIndex]);

				++m_generationIndex;

				area->ComputeSpotEncounters ();

				// don't go over our time allotment
				if (g_commonTimer.GetSeconds <float> () - startTime > maxTime)
				{
					ShowAnalysisProgressWithPercent ("Finding encounter spots", static_cast <unsigned char> (100u * m_generationIndex / m_areas.GetElementNumber ()));

					return true;
				}
			}

			m_generationState = FIND_SNIPER_SPOTS;
			m_generationIndex = 0u;

			findingSniperSpotsStartTime = g_commonTimer.GetSeconds <float> ();

			AddLogEntry (true, LogLevel_Default, false, "Finding encounter spots...DONE! Elapsed time is %.2f sec.", findingSniperSpotsStartTime - findingEncounterSpotsStartTime);

			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_SNIPER_SPOTS:
		{
			while (m_generationIndex < m_areas.GetElementNumber ())
			{
				NavigationArea *const area (m_areas[m_generationIndex]);

				++m_generationIndex;

				area->ComputeSniperSpots ();

				// don't go over our time allotment
				if (g_commonTimer.GetSeconds <float> () - startTime > maxTime)
				{
					ShowAnalysisProgressWithPercent ("Finding sniper spots", static_cast <unsigned char> (100u * m_generationIndex / m_areas.GetElementNumber ()));

					return true;
				}
			}

			m_generationState = FIND_EARLIEST_OCCUPY_TIMES;
			m_generationIndex = 0u;

			findingEarliestOccupyTimesStartTime = g_commonTimer.GetSeconds <float> ();

			AddLogEntry (true, LogLevel_Default, false, "Finding sniper spots...DONE! Elapsed time is %.2f sec.", findingEarliestOccupyTimesStartTime - findingSniperSpotsStartTime);

			return true;
		}

		//---------------------------------------------------------------------------
		case FIND_EARLIEST_OCCUPY_TIMES:
		{
			while (m_generationIndex < m_areas.GetElementNumber ())
			{
				NavigationArea *const area (m_areas[m_generationIndex]);

				++m_generationIndex;

				area->ComputeEarliestOccupyTimes ();

				// don't go over our time allotment
				if (g_commonTimer.GetSeconds <float> () - startTime > maxTime)
				{
					ShowAnalysisProgressWithPercent ("Finding earliest occupy times", static_cast <unsigned char> (100u * m_generationIndex / m_areas.GetElementNumber ()));

					return true;
				}
			}

			m_generationState = SAVE_NAV_MESH;
			m_generationIndex = 0u;

			AddLogEntry (true, LogLevel_Default, false, "Finding earliest occupy times...DONE! Elapsed time is %.2f sec.", g_commonTimer.GetSeconds <float> () - findingEarliestOccupyTimesStartTime);

			return true;
		}

		//---------------------------------------------------------------------------
		case SAVE_NAV_MESH:
		{
			// generation complete!
			AddLogEntry (true, LogLevel_Default, false, "Generation complete! Total elapsed time is %.2f min.", (g_commonTimer.GetSeconds <float> () - generationStartTime) / 60.0f);

			m_generationMode = GENERATE_NONE;
			m_loadState = LoadState_FromGeneration;

			HideAnalysisProgress ();

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_restart_after_analysis)->GetValue <bool> ())
			{
				// save the generated mesh
				if (Save ())
					AddLogEntry (true, LogLevel_Default, false, "Navigation map '%s' saved.", GetFilename ().GetData ());
				else
					AddLogEntry (true, LogLevel_Error, false, "Cannot save navigation map '%s'.", GetFilename ().GetData ());

				// Restart the current map now.
				HalfLifeEngine::Globals::g_halfLifeEngine->ChangeLevel (HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName (), NULL);
//				HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommand ("changelevel " + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + '\n');
			}

			return false;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Add a navigation node and connect it.
* Node Z positions are ground level.
*/
const NavigationMesh::NavigationNode *const NavigationMesh::AddNode (const Math::Vector3D &destPos, const Math::Vector3D &normal, const NavDirType_t direction, NavigationNode *const source/*, const bool isDisplacementSurface, float UNKNOWN1, const float UNKNOWN2, const float UNKNOWN3*/)
{
	NavigationNode *node (GetNode (destPos));	// check if a node exists at this location

	// if no node exists, create one
	if (node == NULL)
	{
		// new node becomes current node
		m_currentNode = node = new NavigationNode (destPos, normal, source/*, isDisplacementSurface*/);

		#if defined _DEBUG
			// Reliability check.
			if (m_currentNode == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation node at location: %s!", sizeof (NavigationNode), destPos.ToString ());

				return NULL;
			}
		#endif	// if defined _DEBUG
	}

	// connect source node to new node
	source->ConnectTo (node, direction/*, UNKNOWN1, UNKNOWN2, UNKNOWN3*/);

	const float deltaZ (Math::fabsf (source->GetPosition ().z - destPos.z));
	const float absoluteDeltaZ (Math::fabsf (deltaZ));
	const float jumpZTolerance (/*50.0f - ORIGINAL VALUE*/HalfLifeEngine::SDK::Constants::JumpCrouchHeight);
//	const float doubleJumpZTolerance (HalfLifeEngine::SDK::Constants::HumanHeight + jumpZTolerance);

	// optimization: if 'absoluteDeltaZ' changes very little, assume connection is commutative
	if (absoluteDeltaZ <= jumpZTolerance/*! EDWARD!! || absoluteDeltaZ <= doubleJumpZTolerance*/)
	{
/*		if (UNKNOWN1 <= 0.0f)
			UNKNOWN1 = 0.0f;
		else
		{
			UNKNOWN1 += deltaZ;

			if (UNKNOWN1 <= 0.0f)
				UNKNOWN1 = 0.0f;
		}
*/
		const NavDirType_t oppositeDirection (GetOppositeDirection (direction));

		node->ConnectTo (source, oppositeDirection/*, UNKNOWN1, GenerationStepSize - UNKNOWN3, GenerationStepSize - UNKNOWN2*/);
		node->MarkAsVisited (oppositeDirection);
	}

	node->CheckCrouch ();

	return node;
}

//--------------------------------------------------------------------------------------------------------------
NavigationMesh::NavigationNode *const NavigationMesh::LadderEndSearch (Math::Vector3D center, const NavDirType_t mountDir) const
{
	const Math::Vector3D fudge (0.0f, 0.0f, 2.0f);
	Math::Vector3D tryPos;
	Math::Vector3D tryNormal;
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	AddDirectionVector (center, mountDir, HalfLifeEngine::SDK::Constants::HalfHumanWidth);

	//
	// Test the ladder dismount point first, then each cardinal direction one and two steps away
	//
	for (char d (-1); d < 2 * NUM_DIRECTIONS; ++d)
	{
		tryPos = center;

		if (d >= NORTH)
			AddDirectionVector (tryPos, static_cast <NavDirType_t> (d), GenerationStepSize);
		else if (d >= NUM_DIRECTIONS)
			AddDirectionVector (tryPos, static_cast <NavDirType_t> (d - NUM_DIRECTIONS), 2.0f * GenerationStepSize);

		// snap it to the sampling grid
		tryPos.x = SnapToGrid (tryPos.x);
		tryPos.y = SnapToGrid (tryPos.y);

		tryPos.z += GenerationStepSize;	// step up a rung, to ensure adjacent floors are below us

		// adjust height to account for sloping areas
		if (GetGroundHeight (tryPos, &tryPos.z, &tryNormal) == false)
			continue;

		// make sure this point is not on the other side of a wall
//		UTIL_TraceLine (center + fudge, tryPos + fudge, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (center + fudge, tryPos + fudge, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//		if (traceResult.isStartSolid || traceResult.fraction < 1.0f)
		if (!TraceLineThruWalkableEntities (center + fudge, tryPos + fudge, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult))
			continue;

		// if no node exists here, create one and continue the search
		if (GetNode (tryPos) == NULL)
		{
			NavigationNode *const newNode (new NavigationNode (tryPos, tryNormal));

			#if defined _DEBUG
				// Reliability check.
				if (newNode == NULL)
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation node at location: %s!", sizeof (NavigationNode), tryPos.ToString ());
			#endif	// if defined _DEBUG

			return newNode;
		}
	}

	return NULL;
}
NavigationMesh::NavigationNode *const NavigationMesh::TeleportDestinationEndSearch (Math::Vector3D teleportDestination) const
{
	Math::Vector3D teleportDestinationGroundNormal;

	// snap it to the sampling grid
	teleportDestination.x = SnapToGrid (teleportDestination.x);
	teleportDestination.y = SnapToGrid (teleportDestination.y);

	// adjust height to account for sloping areas
	if (GetGroundHeight (teleportDestination, &teleportDestination.z, &teleportDestinationGroundNormal) == false)
		return NULL;

	// if no node exists here, create one and continue the search
	if (GetNode (teleportDestination) != NULL)
		return NULL;

	NavigationNode *const newNode (new NavigationNode (teleportDestination, teleportDestinationGroundNormal));

	#if defined _DEBUG
		// Reliability check.
		if (newNode == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation node at location: %s!", sizeof (NavigationNode), teleportDestination.ToString ());
	#endif	// if defined _DEBUG

	return newNode;
}
NavigationMesh::NavigationNode *const NavigationMesh::PlayerSpawnPointEndSearch (Math::Vector3D spawnOrigin) const
{
	Math::Vector3D normal;

	// snap it to the sampling grid
	spawnOrigin.x = SnapToGrid (spawnOrigin.x);
	spawnOrigin.y = SnapToGrid (spawnOrigin.y);

	if (GetGroundHeight (spawnOrigin, &spawnOrigin.z, &normal) == false)
		return NULL;

	// if no node exists here, create one and continue the search
	if (GetNode (spawnOrigin) != NULL)
		return NULL;

	NavigationNode *const newNode (new NavigationNode (spawnOrigin, normal));

	#if defined _DEBUG
		// Reliability check.
		if (newNode == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation node at location: %s!", sizeof (NavigationNode), spawnOrigin.ToString ());
	#endif	// if defined _DEBUG

	return newNode;
}


//--------------------------------------------------------------------------------------------------------------
/**
* Search the world and build a map of possible movements.
* The algorithm begins at the bot's current location, and does a recursive search
* outwards, tracking all valid steps and generating a directed graph of NavigationNodes.
*
* Sample the map one "step" in a cardinal direction to learn the map.
*
* Returns true if sampling needs to continue, or false if done.
*/
const bool NavigationMesh::SampleStep (void)
{
	// take a step
	for (/* Empty */; /* Empty */; /* Empty */)
	{
		if (m_currentNode == NULL)
		{
			// sampling is complete from current seed, try next one
			m_currentNode = GetNextWalkableSeedNode ();

			if (m_currentNode == NULL)
			{
				// search is exhausted - continue search from ends of ladders
				for (NavLadderArray::IndexType_t index (0u); index < m_ladders.GetElementNumber (); ++index)
				{
					const NavigationLadder *const ladder (m_ladders[index]);

					// check ladder bottom/top
					if ((m_currentNode = LadderEndSearch (ladder->GetBottom (), ladder->GetDirection ())) != NULL || (m_currentNode = LadderEndSearch (ladder->GetTop (), ladder->GetDirection ())) != NULL)
						break;
				}

				// ADDDED BY EDWARD!!!!!!!!!!!!!!
				if (m_currentNode == NULL)
				{
					// search is exhausted - continue search from destinations of teleports
					for (unsigned short index (0u); index < m_worldMap.GetBSPFile ().entities.GetElementNumber (); ++index)
					{
						const WorldMap::Entity_t &entity (m_worldMap.GetBSPFile ().entities[index]);

						if (entity.className != "info_teleport_destination")
							continue;

						if ((m_currentNode = TeleportDestinationEndSearch (entity.origin)) != NULL)
							break;
					}

					if (m_currentNode == NULL)
					{
						// search is exhausted - continue search from other player spawn points
						for (unsigned char teamIndex (TeamArrayID_Terrorist); teamIndex < TeamArrayID_Total; ++teamIndex)
						{
							const Server::SpawnPointsContainer::SpawnPointsArray &playerSpawnPoints (g_server->GetSpawnPointsContainer ()[static_cast <TeamArrayID_t> (teamIndex)]);

							for (Server::SpawnPointsContainer::SpawnPointsArray::IndexType_t index (0u); index < playerSpawnPoints.GetElementNumber (); ++index)
								if ((m_currentNode = PlayerSpawnPointEndSearch (playerSpawnPoints[index])) != NULL)
									break;

							if (m_currentNode != NULL)
								break;
						}

						if (m_currentNode == NULL)
						{
							// all seeds exhausted, sampling complete
							return false;
						}
					}
				}
			}
		}

		//
		// Take a step from this node
		//
		for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
		{
			if (m_currentNode->HasVisited (static_cast <NavDirType_t> (direction)))
				continue;

			// have not searched in this direction yet

			if (m_sampleTick < m_maximumSampleTicks)
				++m_sampleTick;

			m_generationDirection = static_cast <NavDirType_t> (direction);

			// mark direction as visited
			m_currentNode->MarkAsVisited (m_generationDirection);

			// start at current node position
			const Math::Vector3D &from (m_currentNode->GetPosition ());
			Math::Vector3D to (static_cast <short> (SnapToGrid (from.x)), static_cast <short> (SnapToGrid (from.y)), from.z);	// snap to grid

			// attempt to move to adjacent node....
			AddDirectionVector (to, m_generationDirection, static_cast <unsigned char> (GenerationStepSize));
#if 0
/// FROM NEW SOURCE ENGINE
			{
			// test if we can move to new position
//			trace_t traceResult;
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
			Math::Vector3D from (m_currentNode->GetPosition ());
			float someFloatValue, distanceFromTo, distanceToFrom;

			if (TraceAdjacentNode (0u, from, to, traceResult, HalfLifeEngine::SDK::Constants::DeathDrop))
			{
				someFloatValue = 0.0f;

				if (!(traceResult.surface.flags & (SURF_SKY2D | SURF_SKY)))
				{
					if
					(
						IsNodeOverlapped (traceResult.endPosition, Math::Vector3D ( 1.0f,  1.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)) &&
						IsNodeOverlapped (traceResult.endPosition, Math::Vector3D (-1.0f,  1.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)) &&
						IsNodeOverlapped (traceResult.endPosition, Math::Vector3D ( 1.0f, -1.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)) &&
						IsNodeOverlapped (traceResult.endPosition, Math::Vector3D (-1.0f, -1.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)) &&

						m_generationMode != GENERATE_SIMPLIFY_SELECTED_AREAS
					)
						return true;

					if (nav_generate_incremental_tolerance.value <= 0.0f || m_generationMode != GENERATE_INCREMENTAL)
					{
						goto LABEL_59;
					}

					if (m_walkableSeedList.Count () > 0)
					{
						//
					}
				}
			}
			else
			{
				float heightOffset (HalfLifeEngine::SDK::Constants::StepHeight);
				const float maximumHeightOffset (HalfLifeEngine::SDK::Constants::JumpCrouchHeight);

				distanceFromTo = 0.0f;
				distanceToFrom = GenerationStepSize;

				do
				{
					from.z += heightOffset;
					to.z += heightOffset;

					UTIL_TraceHull (from, to, NavTraceMins, NavTraceMaxs, GetGenerationTraceMask (), &traceFilter, &traceResult);

					const float distanceFromToSquared (traceResult.endPosition.GetDistanceSquared (from));

					if (distanceFromToSquared </*=*/ MATH_GET_SQUARED (GenerationStepSize))
					{
						UTIL_TraceHull (to, from, NavTraceMins, NavTraceMaxs, GetGenerationTraceMask (), &traceFilter, &traceResult);

						if (!traceResult.isStartSolid && traceResult.fraction </*=*/ 1.0f)
						{
							const float distanceToFromSquared (traceResult.endPosition.GetDistanceSquared (from));

							if (distanceToFromSquared </*=*/ MATH_GET_SQUARED (GenerationStepSize))
							{
								distanceFromTo = Math::sqrtf (distanceFromToSquared);
								distanceToFrom = Math::sqrtf (distanceToFromSquared);
							}
						}
					}
				} while (++heightOffset <= maximumHeightOffset);
			}
			}
#endif	// if 0
			Math::Vector3D toNormal;

			// modify position to account for change in ground level during step, check for maximum allowed slope
			if (GetGroundHeight (to, &to.z, &toNormal) == false || /* ADDED BY EDWARD!!! */toNormal.z < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
				return true;

			// test if we can move to new position
//			trace_t traceResult;
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
			const Math::Vector3D offset (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight);

//			CTraceFilterWalkableEntities filter (NULL, COLLISION_GROUP_NONE, WALK_THRU_EVERYTHING);

//			UTIL_TraceLine (from + offset, to + offset, MASK_PLAYERSOLID_BRUSHONLY, &filter, &traceResult);

			if (!TraceLineThruWalkableEntities (from + offset, to + offset, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult))
				return true;

			// the trace line didnt hit anything - clear....

			const float toGround (to.z);
			const float fromGround (from.z);

			// check if ledge is too high to reach or will cause us to fall to our death
			if (toGround - fromGround > HalfLifeEngine::SDK::Constants::JumpCrouchHeight + Math::RawEpsilon || fromGround - toGround > HalfLifeEngine::SDK::Constants::DeathDrop)
				return true;

			// check surface normals along this step to see if we would cross any impassable slopes
			const unsigned char inc (2u);
			Math::Vector3D normal;

			for (unsigned char along (inc); /* ADDED BY EDWARD!!!, WAS /* Empty *//* */along < GenerationStepSize; along += inc)
			{
/*REMOVED BY EDWARD!!!				// need to guarantee that we test the exact edges
				if (along >= GenerationStepSize)
				{
					// check for maximum allowed slope
					if (GetGroundHeight (to, NULL, &normal) == false || normal.z < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
						return true;

					break;
				}
*/
				// check for maximum allowed slope
				if (GetGroundHeight (from.GetInterpolated (to, along / GenerationStepSize), NULL, &normal) == false || normal.z < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())
					return true;
			}

			// if we're incrementally generating, don't overlap existing navigaion areas
			if (GetNavArea (to, HalfLifeEngine::SDK::Constants::HumanHeight) == NULL)
			{
#if 0
/// FROM NEW SOURCE ENGINE
				if (someFloatValue </*=*/ 18.0f || traceResult.endPosition.z - m_currentNode->GetPosition ().z >/*=*/ someFloatValue - 2.0f)
				{
					someFloatValue = distanceFromTo = 0.0f;
					distanceToFrom = GenerationStepSize;
				}
#endif	// if 0
				// Reliability check.
//				InternalAssert (TraceLineThruWalkableEntities (to, to + offset, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult));	// OCCURS!!! (cs_italy)

				// we can move here, create a new navigation node, and update current node pointer
				AddNode (to, toNormal, m_generationDirection, m_currentNode/*, traceResult.IsDispSurface (), someFloatValue, distanceFromTo, distanceToFrom*/);
			}

			return true;
		}

		// all directions have been searched from this node - pop back to its parent and continue....
		m_currentNode = m_currentNode->GetParent ();
	}
}

//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::FindGroundForNode (Math::Vector3D &pos, Math::Vector3D &normal)
{
//	CTraceFilterSimple traceFilter (NULL, COLLISION_GROUP_NONE);
//	CTraceFilterWalkableEntities traceFilter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT, WALK_THRU_EVERYTHING);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceHull (Math::Vector3D (pos.x, pos.y, pos.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight - 0.1f), Math::Vector3D (pos.x, pos.y, pos.z - 200.0f), NavTraceMins, NavTraceMaxs, GetGenerationTraceMask (), &traceFilter, &traceResult);
	TraceLineThruWalkableEntities (Math::Vector3D (pos.x, pos.y, pos.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight - 0.1f), Math::Vector3D (pos.x, pos.y, pos.z - 200.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

	pos = traceResult.endPosition;
	normal = traceResult.planeNormal;

	return traceResult.isAllSolid == false;
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::AddWalkableSeeds (void)
{
	// start sampling from a spawn point
	const Server::SpawnPointsContainer::SpawnPointsArray &playerSpawnPoints (g_server->GetSpawnPointsContainer ()[GetPlayersSpawnPointsTeam ()]);

	if (playerSpawnPoints.IsEmpty ())
		return;

	Math::Vector3D pos (playerSpawnPoints.GetRandomElement ());
	Math::Vector3D normal;

	// snap it to the sampling grid
	pos.x = SnapToGrid (pos.x);
	pos.y = SnapToGrid (pos.y);

	if (FindGroundForNode (pos, normal))
		AddWalkableSeed (pos, normal);
}

//--------------------------------------------------------------------------------------------------------------
/**
* Return the next walkable seed as a node
*/
NavigationMesh::NavigationNode *const NavigationMesh::GetNextWalkableSeedNode (void)
{
	if (m_seedIndex == m_walkableSeeds.BadPosition)
		return NULL;

	const WalkableSeedSpot_t &spot (m_walkableSeeds[m_seedIndex]);

	if (++m_seedIndex >= m_walkableSeeds.GetElementNumber ())
		m_seedIndex = m_walkableSeeds.BadPosition;

	NavigationNode *const newNode (new NavigationNode (spot.pos, spot.normal));

	#if defined _DEBUG
		// Reliability check.
		if (newNode == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation node at location: %s!", sizeof (NavigationNode), spot.pos.ToString ());
	#endif	// if defined _DEBUG

	return newNode;
}


//--------------------------------------------------------------------------------------------------------------
/**
* Check LOS, ignoring any entities that we can walk through
*/
const bool IsWalkableTraceLineClear (const Math::Vector3D &from, const Math::Vector3D &to, const unsigned char flags)
{
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	Math::Vector3D useFrom (from);
	const Math::Vector3D direction ((to - from).Normalize () * 5.0f);	// 5 units long
//	CTraceFilterWalkableEntities traceFilter (NULL, COLLISION_GROUP_NONE, flags);
	const unsigned char maxTries (50u);

	for (unsigned char trie (0u); trie < maxTries; ++trie)
	{
//		UTIL_TraceLine (useFrom, to, MASK_PLAYERSOLID, &traceFilter, &traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (useFrom, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

		// if we hit a walkable entity, try again
		if (/*!traceResult.isStartSolid && */traceResult.fraction == 1.0f)
			return true;
		else if (!IsCanWalkThruEntity (traceResult.hitEntity, flags))
			return false;

		// start from just beyond where we hit to avoid infinite loops
		useFrom = traceResult.endPosition + direction;
	}

	return false;
}