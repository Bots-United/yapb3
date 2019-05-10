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
// NavigationArea.hpp
//
// Class: NavigationArea
//
// Description: AI Navigation areas.
//				A NavigationArea is a rectangular region defining a walkable area in the environment.
//
// Version: $ID:$
//

#include <Core.hpp>

//--------------------------------------------------------------------------------------------------------------
unsigned int NavigationMesh::NavigationArea::HidingSpot::m_nextID (0u);	// Reset the next hiding spot ID to 0
unsigned int NavigationMesh::NavigationArea::HidingSpot::m_masterMarker (0u);

unsigned int NavigationMesh::NavigationArea::m_masterMarker (1u);
NavigationMesh::NavigationArea *NavigationMesh::NavigationArea::m_openList (NULL);
/*
ConVar nav_coplanar_slope_limit ("nav_coplanar_slope_limit", "0.99", FCVAR_GAMEDLL);
ConVar nav_split_place_on_ground ("nav_split_place_on_ground", "0", FCVAR_GAMEDLL, "If true, navigation areas will be placed flush with the ground when split.");
ConVar nav_area_bgcolor ("nav_area_bgcolor", "0 0 0 30", FCVAR_GAMEDLL, "RGBA color to draw as the background color for navigation areas while editing.");
ConVar nav_corner_adjust_adjacent ("nav_corner_adjust_adjacent", "18", FCVAR_GAMEDLL, "radius used to raise/lower corners in nearby areas when raising/lowering corners.");
*/

//--------------------------------------------------------------------------------------------------------------
/*** To keep constructors consistent*/
void NavigationMesh::NavigationArea::Initialize (void)
{
	m_marker = 0u;
	m_parent = NULL;
	m_parentHow = GO_NORTH;

	ResetNodes ();

//	m_prevHash = m_nextHash = NULL;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Assumes Z is flat
*/
NavigationMesh::NavigationArea::NavigationArea (const Math::Vector3D &corner, const Math::Vector3D &otherCorner) :
	BaseNavigationArea (g_server->GetNavigationMeshManager ().IncreaseAndGetNextAreaID ())	// set an ID for splitting and other interactive editing - loads will overwrite this
{
	Initialize ();

	if (corner.x < otherCorner.x)
	{
		m_extent.mins.x = corner.x;
		m_extent.maxs.x = otherCorner.x;
	}
	else
	{
		m_extent.maxs.x = corner.x;
		m_extent.mins.x = otherCorner.x;
	}

	if (corner.y < otherCorner.y)
	{
		m_extent.mins.y = corner.y;
		m_extent.maxs.y = otherCorner.y;
	}
	else
	{
		m_extent.maxs.y = corner.y;
		m_extent.mins.y = otherCorner.y;
	}

	m_extent.mins.z = m_extent.maxs.z = corner.z;

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

	m_neZ = corner.z;
	m_swZ = otherCorner.z;

	CheckWaterLevel ();
}

//--------------------------------------------------------------------------------------------------------------
/**
*
*/
NavigationMesh::NavigationArea::NavigationArea (const Math::Vector3D &nwCorner, const Math::Vector3D &neCorner, const Math::Vector3D &seCorner, const Math::Vector3D &swCorner) :
	BaseNavigationArea (g_server->GetNavigationMeshManager ().IncreaseAndGetNextAreaID ())	// set an ID for splitting and other interactive editing - loads will overwrite this
{
	Initialize ();

	m_extent.mins = nwCorner;
	m_extent.maxs = seCorner;

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

	m_neZ = neCorner.z;
	m_swZ = swCorner.z;

	CheckWaterLevel ();
}


//--------------------------------------------------------------------------------------------------------------
/**
* Constructor used during generation phase.
*/
NavigationMesh::NavigationArea::NavigationArea (NavigationNode *const nwNode, NavigationNode *const neNode, NavigationNode *const seNode, NavigationNode *const swNode) :
	BaseNavigationArea (g_server->GetNavigationMeshManager ().IncreaseAndGetNextAreaID ())	// set an ID for splitting and other interactive editing - loads will overwrite this
{
	Initialize ();

	m_extent.mins = nwNode->GetPosition ();
	m_extent.maxs = seNode->GetPosition ();

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

	m_neZ = neNode->GetPosition ().z;
	m_swZ = swNode->GetPosition ().z;

	m_node[NORTH_WEST] = nwNode;
	m_node[NORTH_EAST] = neNode;
	m_node[SOUTH_EAST] = seNode;
	m_node[SOUTH_WEST] = swNode;

	// mark internal nodes as part of this area
	AssignNodes (this);

	CheckWaterLevel ();
}

//--------------------------------------------------------------------------------------------------------------
/**
* This is invoked when an area is going away.
* Remove any references we have to it.
*/
void NavigationMesh::NavigationArea::OnDestroyNotify (NavigationArea *const dead)
{
	Disconnect (dead);

	m_overlapAreas -= dead;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Connect this area to given area in given direction
*/
void NavigationMesh::NavigationArea::ConnectTo (NavigationArea *const area, const NavDirType_t direction)
{
	// don't allow self-referential connections, check if already connected
	if (area == this || m_connectedAreas[direction].IsMember (area))
		return;

	m_connectedAreas[direction] += area;

	//static char *dirName[] = {"NORTH", "EAST", "SOUTH", "WEST"};
	//CONSOLE_ECHO ("  Connected area #%u to #%u, %s\n", m_id, area->m_id, dirName[direction]);
}

//--------------------------------------------------------------------------------------------------------------
/**
* Disconnect this area from given area
*/
void NavigationMesh::NavigationArea::Disconnect (NavigationArea *const area)
{
	for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
		m_connectedAreas[direction] -= area;
}

//--------------------------------------------------------------------------------------------------------------
/**
* Connect this area to given ladder
*/
void NavigationMesh::NavigationArea::ConnectTo (NavigationLadder *const ladder)
{
	ConnectTo (ladder, m_center.z <= ladder->GetCenter ().z ? NavigationLadder::Direction_Up : NavigationLadder::Direction_Down);
}

//--------------------------------------------------------------------------------------------------------------
/**
* Connect this area to given ladder in given direction
*/
void NavigationMesh::NavigationArea::ConnectTo (NavigationLadder *const ladder, const NavigationLadder::Direction_t direction)
{
	Disconnect (ladder);	// just in case

	m_connectedLadders[direction] += ladder;
}

//--------------------------------------------------------------------------------------------------------------
/*** Disconnect this area from given ladder*/
void NavigationMesh::NavigationArea::Disconnect (NavigationLadder *const ladder)
{
	for (unsigned char direction (NavigationLadder::Direction_Up); direction < NavigationLadder::Direction_Total; ++direction)
		m_connectedLadders[direction] -= ladder;
}

//--------------------------------------------------------------------------------------------------------------
/*** Recompute internal data once nodes have been adjusted during merge* Destroy 'adjArea'.*/
void NavigationMesh::NavigationArea::FinishMerge (NavigationArea *const adjArea)
{
	// update extent
	m_extent.mins = m_node[NORTH_WEST]->GetPosition ();
	m_extent.maxs = m_node[SOUTH_EAST]->GetPosition ();

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

	m_neZ = m_node[NORTH_EAST]->GetPosition ().z;
	m_swZ = m_node[SOUTH_WEST]->GetPosition ().z;

	// Update a area position in a navigation mesh grid after we have updated area extent....
	g_server->GetNavigationMeshManager ().RemoveAreaFromGrid (this);
	g_server->GetNavigationMeshManager ().AddAreaToGrid (this);

	// reassign the adjacent area's internal nodes to the final area
	adjArea->AssignNodes (this);

	// merge adjacency links - we gain all the connections that 'adjArea' had
	MergeAdjacentConnections (adjArea);

	// remove subsumed adjacent area
	g_server->GetNavigationMeshManager ().DestroyArea (adjArea);
}

//--------------------------------------------------------------------------------------------------------------
/*** For merging with "adjArea" - pick up all of "adjArea"s connections*/
void NavigationMesh::NavigationArea::MergeAdjacentConnections (NavigationArea *const adjArea)
{
	// merge adjacency links - we gain all the connections that adjArea had
	unsigned char direction;

	for (direction = NORTH; direction < NUM_DIRECTIONS; ++direction)
		for (NavAreaConnectArray::IndexType_t index (0u); index < adjArea->m_connectedAreas[direction].GetElementNumber (); ++index)
		{
			NavigationArea *const connect (adjArea->m_connectedAreas[direction][index]);

			if (connect != adjArea && connect != this)
				ConnectTo (connect, static_cast <NavDirType_t> (direction));
		}

	// remove any references from this area to the adjacent area, since it is now part of us
	Disconnect (adjArea);

	// Change other references to adjArea to refer instead to us
	// We can't just replace existing connections, as several adjacent areas may have been merged into one,
	// resulting in a large area adjacent to all of them ending up with multiple redunandant connections
	// into the merged area, one for each of the adjacent subsumed smaller ones.
	// If an area has a connection to the merged area, we must remove all references to adjArea, and add
	// a single connection to us.
	for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
	{
		NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

		if (area == this || area == adjArea)
			continue;

		for (direction = NORTH; direction < NUM_DIRECTIONS; ++direction)
		{
			// check if there are any references to adjArea in this direction
			for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < area->m_connectedAreas[direction].GetElementNumber (); ++index2)
				if (area->m_connectedAreas[direction][index2] == adjArea)
				{
					// remove reference to 'adjArea'
					area->m_connectedAreas[direction].Remove (index2, 1u);

					if (!area->m_connectedAreas[direction].IsMember (this))
						area->m_connectedAreas[direction] += this;	// add a single connection to the new area

					break;
				}
		}
	}

	// We gain all ladder connections 'adjArea' had
	for (direction = NavigationLadder::Direction_Up; direction < NavigationLadder::Direction_Total; ++direction)
		for (NavLadderConnectArray::IndexType_t index (0u); index < adjArea->m_connectedLadders[direction].GetElementNumber (); ++index)
			ConnectTo (adjArea->m_connectedLadders[direction][index]);

	// All ladders that point to adjArea should point to us now
	for (NavLadderArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetLadders ().GetElementNumber (); ++index)
	{
		NavigationLadder *const ladder (g_server->GetNavigationMeshManager ().GetLadders ()[index]);

		for (direction = NavigationLadder::ConnectionType_TopBehind; direction < NavigationLadder::ConnectionType_Total; ++direction)
			if (ladder->GetConnectedArea (static_cast <NavigationLadder::ConnectionType_t> (direction)) == adjArea)
				ladder->SetConnectedArea (static_cast <NavigationLadder::ConnectionType_t> (direction), this);
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Assign internal nodes to the given area* NOTE: "internal" nodes do not include the east or south border nodes*/
void NavigationMesh::NavigationArea::AssignNodes (NavigationArea *const area)
{
	const NavigationNode *horizLast (m_node[NORTH_EAST]);

	for (NavigationNode *vertNode (m_node[NORTH_WEST]); vertNode != m_node[SOUTH_WEST]; vertNode = vertNode->GetConnectedNode (SOUTH))
	{
		for (NavigationNode *horizNode (vertNode); horizNode != horizLast; horizNode = horizNode->GetConnectedNode (EAST))
			horizNode->AssignArea (area);

		horizLast = horizLast->GetConnectedNode (SOUTH);
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Split this area into two areas at the given edge.* Preserve all adjacency connections.* NOTE: This does not update node connections, only areas.*/
const bool NavigationMesh::NavigationArea::SplitEdit (const bool splitAlongX, const float splitEdge, NavigationArea **const outAlpha/* = NULL*/, NavigationArea **const outBeta/* = NULL*/)
{
	NavigationArea *alpha, *beta;

	if (splitAlongX)
	{
		// +-----+--> X
		// |  A  |
		// +-----+
		// |  B  |
		// +-----+
		// |
		// Y

		// don't do split if at edge of area
		if (splitEdge <= m_extent.mins.y + 1.0f || splitEdge >= m_extent.maxs.y - 1.0f)
			return false;

		alpha = new NavigationArea ();

		#if defined _DEBUG
			// Reliability check.
			if (alpha == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (*alpha), GetCenter ().ToString ());

				return false;
			}
		#endif	// if defined _DEBUG

		alpha->m_extent.mins = m_extent.mins;

		alpha->m_extent.maxs.x = m_extent.maxs.x;
		alpha->m_extent.maxs.y = splitEdge;
		alpha->m_extent.maxs.z = GetZ (alpha->m_extent.maxs);

		beta = new NavigationArea ();

		#if defined _DEBUG
			// Reliability check.
			if (beta == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (*beta), GetCenter ().ToString ());

				return false;
			}
		#endif	// if defined _DEBUG

		beta->m_extent.mins.x = m_extent.mins.x;
		beta->m_extent.mins.y = splitEdge;
		beta->m_extent.mins.z = GetZ (beta->m_extent.mins);

		beta->m_extent.maxs = m_extent.maxs;

		alpha->ConnectTo (beta, SOUTH);
		beta->ConnectTo (alpha, NORTH);

		FinishSplitEdit (alpha, SOUTH);
		FinishSplitEdit (beta, NORTH);
	}
	else
	{
		// +---+---+--> X
		// |   |   |
		// | A | B |
		// |   |   |
		// +---+---+
		// |
		// Y

		// don't do split if at edge of area
		if (splitEdge <= m_extent.mins.x + 1.0f || splitEdge >= m_extent.maxs.x - 1.0f)
			return false;

		alpha = new NavigationArea ();

		#if defined _DEBUG
			// Reliability check.
			if (alpha == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (*alpha), GetCenter ().ToString ());

				return false;
			}
		#endif	// if defined _DEBUG

		alpha->m_extent.mins = m_extent.mins;

		alpha->m_extent.maxs.x = splitEdge;
		alpha->m_extent.maxs.y = m_extent.maxs.y;
		alpha->m_extent.maxs.z = GetZ (alpha->m_extent.maxs);

		beta = new NavigationArea ();

		#if defined _DEBUG
			// Reliability check.
			if (beta == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (*beta), GetCenter ().ToString ());

				return false;
			}
		#endif	// if defined _DEBUG

		beta->m_extent.mins.x = splitEdge;
		beta->m_extent.mins.y = m_extent.mins.y;
		beta->m_extent.mins.z = GetZ (beta->m_extent.mins);

		beta->m_extent.maxs = m_extent.maxs;

		alpha->ConnectTo (beta, EAST);
		beta->ConnectTo (alpha, WEST);

		FinishSplitEdit (alpha, EAST);
		FinishSplitEdit (beta, WEST);
	}

	if (!g_server->GetNavigationMeshManager ().IsGenerating () && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_split_place_on_ground)->GetValue <bool> ())
	{
		alpha->PlaceOnGround (NUM_CORNERS);
		beta->PlaceOnGround (NUM_CORNERS);
	}

	// For every ladder we pointed to, alpha or beta should point to it, based on their distance to the ladder
	for (unsigned char direction (NavigationLadder::Direction_Up); direction < NavigationLadder::Direction_Total; ++direction)
		for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[direction].GetElementNumber (); ++index)
		{
			NavigationLadder *const ladder (m_connectedLadders[direction][index]);
			const Math::Vector3D &ladderPos (ladder->GetTop ());	// doesn't matter if we choose top or bottom

			const float alphaDistance (alpha->GetDistanceSquaredToPoint (ladderPos));
			const float betaDistance (beta->GetDistanceSquaredToPoint (ladderPos));

			(alphaDistance < betaDistance ? alpha : beta)->ConnectTo (ladder);
		}

	g_server->GetNavigationMeshManager ().OnSplitArea (this, alpha, beta);

	alpha->m_attributeFlags = beta->m_attributeFlags = m_attributeFlags;	// new areas inherit attributes from original area
	alpha->m_place = beta->m_place = m_place;	// new areas inherit place from original area

	// return new areas
	if (outAlpha != NULL)
		*outAlpha = alpha;

	if (outBeta != NULL)
		*outBeta = beta;

	// remove original area
	g_server->GetNavigationMeshManager ().DestroyArea (this);

	return true;
}

//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::NavigationArea::IsConnected (const NavigationArea *const area) const
{
	// we are connected to ourself
	if (area == this)
		return true;

	// search connections in all directions
	for (unsigned char d (NORTH); d < NUM_DIRECTIONS; ++d)
		for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[d].GetElementNumber (); ++index)
			if (m_connectedAreas[d][index] == area)
				return true;

	// check ladder connections
	{
		for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[NavigationLadder::Direction_Up].GetElementNumber (); ++index)
			if (m_connectedLadders[NavigationLadder::Direction_Up][index]->IsConnected (area, NavigationLadder::Direction_Up))
				return true;

		for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[NavigationLadder::Direction_Down].GetElementNumber (); ++index)
			if (m_connectedLadders[NavigationLadder::Direction_Down][index]->GetConnectedArea (NavigationLadder::ConnectionType_Bottom) == area)
				return true;

		for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[NavigationLadder::Direction_Both].GetElementNumber (); ++index)
			if (m_connectedLadders[NavigationLadder::Direction_Up][index]->IsConnected (area, NavigationLadder::Direction_Both))
				return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------
/*** Return true if given area is connected in given direction* if direction == NUM_DIRECTIONS, check all directions (direction is unknown)* @todo Formalize "asymmetric" flag on connections*/
const bool NavigationMesh::NavigationArea::IsConnected (const NavigationArea *const area, const NavDirType_t direction) const
{
	// Reliability check.
	InternalAssert (direction < NUM_DIRECTIONS);

	// we are connected to ourself
	if (area == this)
		return true;

	// check specific direction
	for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[direction].GetElementNumber (); ++index)
		if (m_connectedAreas[direction][index] == area)
			return true;

	return false;
}

//--------------------------------------------------------------------------------------------------------------
/*** Given the portion of the original area, update its internal data* The "ignoreEdge" direction defines the side of the original area that the new area does not include*/
void NavigationMesh::NavigationArea::FinishSplitEdit (NavigationArea *const newArea, const NavDirType_t ignoreEdge) const
{
	newArea->m_center = newArea->m_extent.mins.GetMidPoint (newArea->m_extent.maxs);
	newArea->m_neZ = GetZ (Math::Vector2D (newArea->m_extent.maxs.x, newArea->m_extent.mins.y));
	newArea->m_swZ = GetZ (Math::Vector2D (newArea->m_extent.mins.x, newArea->m_extent.maxs.y));

	if (newArea->IsDegenerate ())
	{
		newArea->m_extentInvertedSizeX = 0.0f;
		newArea->m_extentInvertedSizeY = 0.0f;
	}
	else
	{
		newArea->m_extentInvertedSizeX = newArea->m_extent.GetInvertedSizeX ();
		newArea->m_extentInvertedSizeY = newArea->m_extent.GetInvertedSizeY ();
	}

	// connect to adjacent areas
	for (unsigned char d (NORTH); d < NUM_DIRECTIONS; ++d)
	{
		if (d == ignoreEdge)
			continue;

		const NavAreaConnectArray::IndexType_t count (GetAdjacentCount (static_cast <NavDirType_t> (d)));

		for (NavAreaConnectArray::IndexType_t a (0u); a < count; ++a)
		{
			NavigationArea *const adj (GetAdjacentArea (static_cast <NavDirType_t> (d), a));

			switch (d)
			{
				case NORTH:
				case SOUTH:
					if (newArea->IsOverlappingX (adj))
					{
						newArea->ConnectTo (adj, static_cast <NavDirType_t> (d));

						// add reciprocal connection if needed
						if (adj->IsConnected (this, GetOppositeDirection (static_cast <NavDirType_t> (d))))
							adj->ConnectTo (newArea, GetOppositeDirection (static_cast <NavDirType_t> (d)));
					}

					break;

				default:	// EAST or WEST
					if (newArea->IsOverlappingY (adj))
					{
						newArea->ConnectTo (adj, (static_cast <NavDirType_t> (d)));

						// add reciprocal connection if needed
						if (adj->IsConnected (this, GetOppositeDirection (static_cast <NavDirType_t> (d))))
							adj->ConnectTo (newArea, GetOppositeDirection (static_cast <NavDirType_t> (d)));
					}

					break;
			}
		}
	}

	g_server->GetNavigationMeshManager ().m_areas += newArea;

	g_server->GetNavigationMeshManager ().AddArea (newArea);

	// Assign nodes
	if (!HasNodes ())
		return;

	// first give it all our nodes...
	newArea->m_node[NORTH_WEST] = m_node[NORTH_WEST];
	newArea->m_node[NORTH_EAST] = m_node[NORTH_EAST];
	newArea->m_node[SOUTH_EAST] = m_node[SOUTH_EAST];
	newArea->m_node[SOUTH_WEST] = m_node[SOUTH_WEST];

	// ... then pull in one edge...
	struct DirectionAndCorners_t
	{
		NavDirType_t    direction;
		NavCornerType_t corner[2u];
	};
	static const DirectionAndCorners_t directionsAndCorners[NUM_DIRECTIONS] =
	{
		{SOUTH, {NORTH_WEST, NORTH_EAST}},
		{WEST,  {NORTH_EAST, SOUTH_EAST}},
		{NORTH, {SOUTH_WEST, SOUTH_EAST}},
		{EAST,  {NORTH_WEST, SOUTH_WEST}}
	};
	const DirectionAndCorners_t &directionAndCorners (directionsAndCorners[ignoreEdge]);

	while (!newArea->IsOverlapping (newArea->m_node[directionAndCorners.corner[0u]]->GetPosition (), GenerationStepSize * 0.5f))
		for (unsigned char i (0u); i < 2u; ++i)
		{
			InternalAssert (newArea->m_node[directionAndCorners.corner[i]] != NULL);
			InternalAssert (newArea->m_node[directionAndCorners.corner[i]]->GetConnectedNode (directionAndCorners.direction) != NULL);

			newArea->m_node[directionAndCorners.corner[i]] = newArea->m_node[directionAndCorners.corner[i]]->GetConnectedNode (directionAndCorners.direction);
		}

	// assign internal nodes...
	newArea->AssignNodes (newArea);

	// ... and grab the node heights for our corner heights.
	newArea->m_neZ = newArea->m_node[NORTH_EAST]->GetPosition ().z;
	newArea->m_extent.mins.z = newArea->m_node[NORTH_WEST]->GetPosition ().z;
	newArea->m_swZ = newArea->m_node[SOUTH_WEST]->GetPosition ().z;
	newArea->m_extent.maxs.z = newArea->m_node[SOUTH_EAST]->GetPosition ().z;
}

//--------------------------------------------------------------------------------------------------------------
/*** Create a new area between this area and given area*/
const bool NavigationMesh::NavigationArea::SpliceEdit (NavigationArea *const other)
{
	NavigationArea *newArea (NULL);
	Math::Vector3D nw, ne, se, sw;

	if (m_extent.mins.x > other->m_extent.maxs.x)
	{
		// 'this' is east of 'other'

		nw.x = other->m_extent.maxs.x;
		nw.y = Math::GetMaximumValueBetween (m_extent.mins.y, other->m_extent.mins.y);
		nw.z = other->GetZ (nw);

		se.x = m_extent.mins.x;
		se.y = Math::GetMinimumValueBetween (m_extent.maxs.y, other->m_extent.maxs.y);
		se.z = GetZ (se);

		ne.x = se.x;
		ne.y = nw.y;
		ne.z = GetZ (ne);

		sw.x = nw.x;
		sw.y = se.y;
		sw.z = other->GetZ (sw);

		newArea = new NavigationArea (nw, ne, se, sw);

		#if defined _DEBUG
			// Reliability check.
			if (newArea == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (NavigationArea), nw.GetMidPoint (se).ToString ());

				return false;
			}
		#endif	// if defined _DEBUG

		ConnectTo (newArea, WEST);
		newArea->ConnectTo (this, EAST);

		other->ConnectTo (newArea, EAST);
		newArea->ConnectTo (other, WEST);
	}
	else if (m_extent.maxs.x < other->m_extent.mins.x)
	{
		// 'this' is west of 'other'

		nw.x = m_extent.maxs.x;
		nw.y = Math::GetMaximumValueBetween (m_extent.mins.y, other->m_extent.mins.y);
		nw.z = GetZ (nw);

		se.x = other->m_extent.mins.x;
		se.y = Math::GetMinimumValueBetween (m_extent.maxs.y, other->m_extent.maxs.y);
		se.z = other->GetZ (se);

		ne.x = se.x;
		ne.y = nw.y;
		ne.z = other->GetZ (ne);

		sw.x = nw.x;
		sw.y = se.y;
		sw.z = GetZ (sw);

		newArea = new NavigationArea (nw, ne, se, sw);

		#if defined _DEBUG
			// Reliability check.
			if (newArea == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (NavigationArea), nw.GetMidPoint (se).ToString ());

				return false;
			}
		#endif	// if defined _DEBUG

		ConnectTo (newArea, EAST);
		newArea->ConnectTo (this, WEST);

		other->ConnectTo (newArea, WEST);
		newArea->ConnectTo (other, EAST);
	}
	else	// 'this' overlaps in X
	{
		if (m_extent.mins.y > other->m_extent.maxs.y)
		{
			// 'this' is south of 'other'

			nw.x = Math::GetMaximumValueBetween (m_extent.mins.x, other->m_extent.mins.x);
			nw.y = other->m_extent.maxs.y;
			nw.z = other->GetZ (nw);

			se.x = Math::GetMinimumValueBetween (m_extent.maxs.x, other->m_extent.maxs.x);
			se.y = m_extent.mins.y;
			se.z = GetZ (se);

			ne.x = se.x;
			ne.y = nw.y;
			ne.z = other->GetZ (ne);

			sw.x = nw.x;
			sw.y = se.y;
			sw.z = GetZ (sw);

			newArea = new NavigationArea (nw, ne, se, sw);

			#if defined _DEBUG
				// Reliability check.
				if (newArea == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (NavigationArea), nw.GetMidPoint (se).ToString ());

					return false;
				}
			#endif	// if defined _DEBUG

			ConnectTo (newArea, NORTH);
			newArea->ConnectTo (this, SOUTH);

			other->ConnectTo (newArea, SOUTH);
			newArea->ConnectTo (other, NORTH);
		}
		else if (m_extent.maxs.y < other->m_extent.mins.y)
		{
			// 'this' is north of 'other'

			nw.x = Math::GetMaximumValueBetween (m_extent.mins.x, other->m_extent.mins.x);
			nw.y = m_extent.maxs.y;
			nw.z = GetZ (nw);

			se.x = Math::GetMinimumValueBetween (m_extent.maxs.x, other->m_extent.maxs.x);
			se.y = other->m_extent.mins.y;
			se.z = other->GetZ (se);

			ne.x = se.x;
			ne.y = nw.y;
			ne.z = GetZ (ne);

			sw.x = nw.x;
			sw.y = se.y;
			sw.z = other->GetZ (sw);

			newArea = new NavigationArea (nw, ne, se, sw);

			#if defined _DEBUG
				// Reliability check.
				if (newArea == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation area at location: %s!", sizeof (NavigationArea), nw.GetMidPoint (se).ToString ());

					return false;
				}
			#endif	// if defined _DEBUG

			ConnectTo (newArea, SOUTH);
			newArea->ConnectTo (this, NORTH);

			other->ConnectTo (newArea, NORTH);
			newArea->ConnectTo (other, SOUTH);
		}
		else
		{
			// areas overlap
			return false;
		}
	}

	// if both areas have the same place, the new area inherits it
	if (m_place == other->m_place)
		newArea->m_place = m_place;
	else if (m_place == UNDEFINED_PLACE)
		newArea->m_place = other->m_place;
	else if (other->m_place == UNDEFINED_PLACE)
		newArea->m_place = m_place;
	else
	{
		// both have valid, but different places - pick on at random
		newArea->m_place = g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 100u) < 50u ? m_place : other->m_place;
	}

	g_server->GetNavigationMeshManager ().m_areas += newArea;

	g_server->GetNavigationMeshManager ().AddArea (newArea);

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/*** Merge this area and given adjacent area*/
const bool NavigationMesh::NavigationArea::MergeEdit (NavigationArea *const adj)
{
	// can only merge if attributes of both areas match

	// check that these areas can be merged
	const float tolerance (1.0f);

	if ((!Math::AreEqual (m_extent.mins.x, adj->m_extent.mins.x, tolerance) || !Math::AreEqual (m_extent.maxs.x, adj->m_extent.maxs.x, tolerance)) && (!Math::AreEqual (m_extent.mins.y, adj->m_extent.mins.y, tolerance) || !Math::AreEqual (m_extent.maxs.y, adj->m_extent.maxs.y, tolerance)))
		return false;

	const Math::Extent3D origExtent (m_extent);

	// update extent
	if (m_extent.mins.x > adj->m_extent.mins.x || m_extent.mins.y > adj->m_extent.mins.y)
		m_extent.mins = adj->m_extent.mins;

	if (m_extent.maxs.x < adj->m_extent.maxs.x || m_extent.maxs.y < adj->m_extent.maxs.y)
		m_extent.maxs = adj->m_extent.maxs;

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

	m_neZ = m_extent.maxs.x > origExtent.maxs.x || m_extent.mins.y < origExtent.mins.y ? adj->GetZ (Math::Vector2D (m_extent.maxs.x, m_extent.mins.y)) : GetZ (Math::Vector2D (m_extent.maxs.x, m_extent.mins.y));
	m_swZ = m_extent.mins.x < origExtent.mins.x || m_extent.maxs.y > origExtent.maxs.y ? adj->GetZ (Math::Vector2D (m_extent.mins.x, m_extent.maxs.y)) : GetZ (Math::Vector2D (m_extent.mins.x, m_extent.maxs.y));
/*
	// Update a area position in a navigation mesh grid after we have updated area extent....
	g_server->GetNavigationMeshManager ().RemoveAreaFromGrid (this);
	g_server->GetNavigationMeshManager ().AddAreaToGrid (this);
*/
	// merge adjacency links - we gain all the connections that adjArea had
	MergeAdjacentConnections (adj);

	// remove subsumed adjacent area
	g_server->GetNavigationMeshManager ().DestroyArea (adj);

	return true;
}

//--------------------------------------------------------------------------------------------------------------
void ApproachAreaAnalysisPrep (void)
{
}

//--------------------------------------------------------------------------------------------------------------
void CleanupApproachAreaAnalysisPrep (void)
{
}

//--------------------------------------------------------------------------------------------------------------
/*** Return true if given point is on or above this area, but no others*/
const bool NavigationMesh::NavigationArea::Contains (const Math::Vector3D &pos) const
{
	// check 2D overlap
	if (!IsOverlapping (pos))
		return false;

	// the point overlaps us, check that it is above us, but not above any areas that overlap us
	const float myZ (GetZ (pos));

	// if the navigation area is above the given position, fail
	if (myZ > pos.z)
		return false;

	for (NavAreaArray::IndexType_t index (0u); index < m_overlapAreas.GetElementNumber (); ++index)
	{
		const NavigationArea *const area (m_overlapAreas[index]);

		// skip self
		InternalAssert (area != this);
/*		if (area == this)
			continue;
*/
		// check 2D overlap
//		if (area->Contains (pos))
//			return true;
		if (!area->IsOverlapping (pos))
			continue;

		const float theirZ (area->GetZ (pos));

		if (theirZ > pos.z)
		{
			// they are above the point
			continue;
		}

		if (theirZ > myZ)
		{
			// we are below an area that is beneath the given position
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/*** Compute "portal" between to adjacent areas.* Return center of portal opening, and half-width defining sides of portal from center.*/
void NavigationMesh::NavigationArea::ComputePortal (const NavigationArea *const to, const NavDirType_t direction, Math::Vector2D &center, float *const halfWidth/* = NULL*/) const
{
	switch (direction)
	{
		case NORTH:
		case SOUTH:
		{
			// clamp to our extent in case areas are disjoint
			const float left (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.x, to->m_extent.mins.x), m_extent.mins.x, m_extent.maxs.x));
			const float right (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.x, to->m_extent.maxs.x), m_extent.mins.x, m_extent.maxs.x));

			center.x = Math::GetMidPoint (left, right);
			center.y = direction == NORTH ? m_extent.mins.y : m_extent.maxs.y;

			if (halfWidth != NULL)
				*halfWidth = Math::GetExtent (left, right);

			break;
		}

		default:	// EAST or WEST
		{
			// clamp to our extent in case areas are disjoint
			const float top (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.y, to->m_extent.mins.y), m_extent.mins.y, m_extent.maxs.y));
			const float bottom (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.y, to->m_extent.maxs.y), m_extent.mins.y, m_extent.maxs.y));

			center.x = direction == WEST ? m_extent.mins.x : m_extent.maxs.x;
			center.y = Math::GetMidPoint (top, bottom);

			if (halfWidth != NULL)
				*halfWidth = Math::GetExtent (top, bottom);

			break;
		}
	}
}
void NavigationMesh::NavigationArea::ComputeLargestPortal (const NavigationArea *const/* to*/, const NavDirType_t/* direction*/, Math::Vector2D &/*center*/, float *const/* halfWidth*//* = NULL*/) const
{
	/// @todo DECOMPILE FROM NEW SOURCE ENGINE !!!
}

//--------------------------------------------------------------------------------------------------------------
/*** Compute closest point within the "portal" between to adjacent areas.*/
const Math::Vector2D NavigationMesh::NavigationArea::GetClosestPointInPortal2D (const NavigationArea *const to, const NavDirType_t direction, const Math::Vector2D &fromPos) const
{
	const float margin (GenerationStepSize * 0.5f/*! @note ' * 0.5f' - NOT EXISTS IN NEW SOURCE ENGINE*/);

	switch (direction)
	{
		case NORTH:
		case SOUTH:
		{
			// clamp to our extent in case areas are disjoint
			const float left (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.x, to->m_extent.mins.x), m_extent.mins.x, m_extent.maxs.x));
			const float right (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.x, to->m_extent.maxs.x), m_extent.mins.x, m_extent.maxs.x));

			// keep margin if against edge
			const float leftMargin (to->IsEdge (WEST) ? left + margin : left);
			const float rightMargin (to->IsEdge (EAST) ? right - margin : right);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (leftMargin <= rightMargin);
///			if (leftMargin > rightMargin)
///				leftMargin = rightMargin = Math::GetMidPoint (left, right);
}
			// limit x to within portal
			return Math::Vector2D (fromPos.x < leftMargin ? leftMargin : fromPos.x > rightMargin ? rightMargin : fromPos.x, direction == NORTH ? m_extent.mins.y : m_extent.maxs.y);
		}

		default:	// EAST or WEST
		{
			// clamp to our extent in case areas are disjoint
			const float top (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.y, to->m_extent.mins.y), m_extent.mins.y, m_extent.maxs.y));
			const float bottom (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.y, to->m_extent.maxs.y), m_extent.mins.y, m_extent.maxs.y));

			// keep margin if against edge
			const float topMargin (to->IsEdge (NORTH) ? top + margin : top);
			const float bottomMargin (to->IsEdge (SOUTH) ? bottom - margin : bottom);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (topMargin <= bottomMargin);
///			if (topMargin > bottomMargin)
///				topMargin = bottomMargin = Math::GetMidPoint (top, bottom);
}
			// limit y to within portal
			return Math::Vector2D (direction == WEST ? m_extent.mins.x : m_extent.maxs.x, fromPos.y < topMargin ? topMargin : fromPos.y > bottomMargin ? bottomMargin : fromPos.y);
		}
	}
}

void NavigationMesh::NavigationArea::ComputePathPortals3D (Connection::Portal3D_t *const pathPortals, unsigned short &pathPortalsNumber, const NavigationArea **const areas, unsigned int &areasNumber) const
{/*
	// Portal between navmesh polygons
	for (const NavigationMesh::NavigationArea *area (this), *parent (area->GetParent ()); parent != NULL; area = parent, parent = parent->GetParent ())
	{
		if (area->GetParentHow () > GO_WEST)
			continue;

		area->ComputePortal3D (parent, static_cast <NavDirType_t> (area->GetParentHow ()), pathPortals[pathPortalsNumber]);

		++pathPortalsNumber;
	}
*/
	struct PathNode_t
		{
			const NavigationMesh::NavigationArea    *area;
			NavigationMesh::NavTraverseType_t  traverseType;
		};
		enum { MaximumPathNodesNumber = 256u };
	PathNode_t m_path[MaximumPathNodesNumber];	// Path information.
	unsigned short m_currentPathInformationNumber (0u);	// Just number of areas in a path....

	// Store some path information....
	{
	// Fill route....
	for (const NavigationMesh::NavigationArea *area (this); area != NULL; area = area->GetParent ())
		++m_currentPathInformationNumber;

	if (m_currentPathInformationNumber >= MaximumPathNodesNumber)
		m_currentPathInformationNumber = MaximumPathNodesNumber - 1u;

	const NavigationMesh::NavigationArea *area (this);

	for (/*PathNodeArray_t::IndexType_t*/unsigned short index (m_currentPathInformationNumber); index > 0u && area != NULL; area = area->GetParent ())
	{
		--index;
		m_path[index].area = area;
		m_path[index].traverseType = area->GetParentHow ();
	}
	}

	for (unsigned short index (1u); index < m_currentPathInformationNumber; ++index)
	{
		const PathNode_t &path (m_path[index - 1u]);
		const PathNode_t &nextPath (m_path[index]);

		const NavigationMesh::NavigationArea *area (path.area), *parent (nextPath.area);

		if (nextPath.traverseType > GO_WEST)
			continue;

		areas[areasNumber] = area;
		areas[++areasNumber] = parent;
		++areasNumber;
/*
		InternalAssert (area->IsConnected (parent));

		{
		NavDirType_t dir (static_cast <NavDirType_t> (-1));
	// search connections in all directions
	for (unsigned char d (NORTH); d < NUM_DIRECTIONS; ++d)
		for (NavAreaConnectArray::IndexType_t index (0u); index < area->m_connectedAreas[d].GetElementNumber (); ++index)
			if (area->m_connectedAreas[d][index] == parent)
			{
				dir = static_cast <NavDirType_t> (d);
				break;
			}
			InternalAssert (dir == static_cast <NavDirType_t> (nextPath.traverseType));
		}
*/
		area->ComputePortal3D (parent, static_cast <NavDirType_t> (nextPath.traverseType), pathPortals[pathPortalsNumber]);

		++pathPortalsNumber;
	}
}
void NavigationMesh::NavigationArea::ComputePortal3D (const NavigationArea *const to, const NavDirType_t direction, Connection::Portal3D_t &portal) const
{
	const float margin (/*0.0f);/*/GenerationStepSize * 0.5f/*! @note ' * 0.5f' - NOT EXISTS IN NEW SOURCE ENGINE*/);

	switch (direction)
	{
/*!ORIGINAL SOURCE CODE		case NORTH:
		case SOUTH:
		{
			// clamp to our extent in case areas are disjoint
			const float left (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.x, to->m_extent.mins.x), m_extent.mins.x, m_extent.maxs.x));
			const float right (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.x, to->m_extent.maxs.x), m_extent.mins.x, m_extent.maxs.x));

			// keep margin if against edge
			const float leftMargin (to->IsEdge (WEST) ? left + margin : left);
			const float rightMargin (to->IsEdge (EAST) ? right - margin : right);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (leftMargin <= rightMargin);
///			if (leftMargin > rightMargin)
///				leftMargin = rightMargin = Math::GetMidPoint (left, right);
}
			// limit x to within portal
			portal.left.x = leftMargin;
			portal.left.y = portal.right.y = direction == NORTH ? m_extent.mins.y : m_extent.maxs.y;
			portal.right.x = rightMargin;

			break;
		}

		default:	// EAST or WEST
		{
			// clamp to our extent in case areas are disjoint
			const float top (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.y, to->m_extent.mins.y), m_extent.mins.y, m_extent.maxs.y));
			const float bottom (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.y, to->m_extent.maxs.y), m_extent.mins.y, m_extent.maxs.y));

			// keep margin if against edge
			const float topMargin (to->IsEdge (NORTH) ? top + margin : top);
			const float bottomMargin (to->IsEdge (SOUTH) ? bottom - margin : bottom);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (topMargin <= bottomMargin);
///			if (topMargin > bottomMargin)
///				topMargin = bottomMargin = Math::GetMidPoint (top, bottom);
}
			// limit y to within portal
			portal.left.x = portal.right.x = direction == WEST ? m_extent.mins.x : m_extent.maxs.x;
			portal.left.y = topMargin;
			portal.right.y = bottomMargin;

			break;
		}
*/
//* CLOCKWISE
		case NORTH:
		{
			// clamp to our extent in case areas are disjoint
			const float left (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.x, to->m_extent.mins.x), m_extent.mins.x, m_extent.maxs.x));
			const float right (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.x, to->m_extent.maxs.x), m_extent.mins.x, m_extent.maxs.x));

			// keep margin if against edge
			const float leftMargin (to->IsEdge (WEST) ? left + margin : left);
			const float rightMargin (to->IsEdge (EAST) ? right - margin : right);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (leftMargin <= rightMargin);
///			if (leftMargin > rightMargin)
///				leftMargin = rightMargin = Math::GetMidPoint (left, right);
}
			// limit x to within portal
			portal.left.x = leftMargin;
			portal.left.y = portal.right.y = m_extent.mins.y;
			portal.right.x = rightMargin;

			portal.width = rightMargin - leftMargin;

			break;
		}

		case SOUTH:
		{
			// clamp to our extent in case areas are disjoint
			const float left (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.x, to->m_extent.maxs.x), m_extent.mins.x, m_extent.maxs.x));
			const float right (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.x, to->m_extent.mins.x), m_extent.mins.x, m_extent.maxs.x));

			// keep margin if against edge
			const float leftMargin (to->IsEdge (WEST) ? left - margin : left);
			const float rightMargin (to->IsEdge (EAST) ? right + margin : right);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (leftMargin >= rightMargin);
///			if (leftMargin < rightMargin)
///				leftMargin = rightMargin = Math::GetMidPoint (left, right);
}
			// limit x to within portal
			portal.left.x = leftMargin;
			portal.left.y = portal.right.y = m_extent.maxs.y;
			portal.right.x = rightMargin;

			portal.width = leftMargin - rightMargin;

			break;
		}

		case EAST:
		{
			// clamp to our extent in case areas are disjoint
			const float top (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.y, to->m_extent.mins.y), m_extent.mins.y, m_extent.maxs.y));
			const float bottom (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.y, to->m_extent.maxs.y), m_extent.mins.y, m_extent.maxs.y));

			// keep margin if against edge
			const float topMargin (to->IsEdge (NORTH) ? top + margin : top);
			const float bottomMargin (to->IsEdge (SOUTH) ? bottom - margin : bottom);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (topMargin <= bottomMargin);
///			if (topMargin > bottomMargin)
///				topMargin = bottomMargin = Math::GetMidPoint (top, bottom);
}
			// limit y to within portal
			portal.left.x = portal.right.x = m_extent.maxs.x;
			portal.left.y = topMargin;
			portal.right.y = bottomMargin;

			portal.width = bottomMargin - topMargin;

			break;
		}

		case WEST:
		{
			// clamp to our extent in case areas are disjoint
			const float top (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.y, to->m_extent.maxs.y), m_extent.mins.y, m_extent.maxs.y));
			const float bottom (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.y, to->m_extent.mins.y), m_extent.mins.y, m_extent.maxs.y));

			// keep margin if against edge
			const float topMargin (to->IsEdge (NORTH) ? top - margin : top);
			const float bottomMargin (to->IsEdge (SOUTH) ? bottom + margin : bottom);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (topMargin >= bottomMargin);
///			if (topMargin < bottomMargin)
///				topMargin = bottomMargin = Math::GetMidPoint (top, bottom);
}
			// limit y to within portal
			portal.left.x = portal.right.x = m_extent.mins.x;
			portal.left.y = topMargin;
			portal.right.y = bottomMargin;

			portal.width = topMargin - bottomMargin;

			break;
		}
/*
/// COUNTER-CLOCKWISE
		case NORTH:
		{
			// clamp to our extent in case areas are disjoint
			const float left (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.x, to->m_extent.maxs.x), m_extent.mins.x, m_extent.maxs.x));
			const float right (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.x, to->m_extent.mins.x), m_extent.mins.x, m_extent.maxs.x));

			// keep margin if against edge
			const float leftMargin (to->IsEdge (WEST) ? left - margin : left);
			const float rightMargin (to->IsEdge (EAST) ? right + margin : right);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (leftMargin >= rightMargin);
///			if (leftMargin < rightMargin)
///				leftMargin = rightMargin = Math::GetMidPoint (left, right);
}
			// limit x to within portal
			portal.left.x = leftMargin;
			portal.left.y = portal.right.y = m_extent.mins.y;
			portal.right.x = rightMargin;

			portal.width = leftMargin - rightMargin;

			break;
		}

		case SOUTH:
		{
			// clamp to our extent in case areas are disjoint
			const float left (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.x, to->m_extent.mins.x), m_extent.mins.x, m_extent.maxs.x));
			const float right (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.x, to->m_extent.maxs.x), m_extent.mins.x, m_extent.maxs.x));

			// keep margin if against edge
			const float leftMargin (to->IsEdge (WEST) ? left + margin : left);
			const float rightMargin (to->IsEdge (EAST) ? right - margin : right);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (leftMargin <= rightMargin);
///			if (leftMargin > rightMargin)
///				leftMargin = rightMargin = Math::GetMidPoint (left, right);
}
			// limit x to within portal
			portal.left.x = leftMargin;
			portal.left.y = portal.right.y = m_extent.maxs.y;
			portal.right.x = rightMargin;

			portal.width = rightMargin - leftMargin;

			break;
		}

		case EAST:
		{
			// clamp to our extent in case areas are disjoint
			const float top (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.y, to->m_extent.maxs.y), m_extent.mins.y, m_extent.maxs.y));
			const float bottom (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.y, to->m_extent.mins.y), m_extent.mins.y, m_extent.maxs.y));

			// keep margin if against edge
			const float topMargin (to->IsEdge (NORTH) ? top - margin : top);
			const float bottomMargin (to->IsEdge (SOUTH) ? bottom + margin : bottom);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (topMargin >= bottomMargin);
///			if (topMargin < bottomMargin)
///				topMargin = bottomMargin = Math::GetMidPoint (top, bottom);
}
			// limit y to within portal
			portal.left.x = portal.right.x = m_extent.maxs.x;
			portal.left.y = topMargin;
			portal.right.y = bottomMargin;

			portal.width = topMargin - bottomMargin;

			break;
		}

		case WEST:
		{
			// clamp to our extent in case areas are disjoint
			const float top (Math::GetClampedValueInRange (Math::GetMaximumValueBetween (m_extent.mins.y, to->m_extent.mins.y), m_extent.mins.y, m_extent.maxs.y));
			const float bottom (Math::GetClampedValueInRange (Math::GetMinimumValueBetween (m_extent.maxs.y, to->m_extent.maxs.y), m_extent.mins.y, m_extent.maxs.y));

			// keep margin if against edge
			const float topMargin (to->IsEdge (NORTH) ? top + margin : top);
			const float bottomMargin (to->IsEdge (SOUTH) ? bottom - margin : bottom);
{/// FROM NEW SOURCE ENGINE
			// Reliability check.
			InternalAssert (topMargin <= bottomMargin);
///			if (topMargin > bottomMargin)
///				topMargin = bottomMargin = Math::GetMidPoint (top, bottom);
}
			// limit y to within portal
			portal.left.x = portal.right.x = m_extent.mins.x;
			portal.left.y = topMargin;
			portal.right.y = bottomMargin;

			portal.width = bottomMargin - topMargin;

			break;
		}*/
	}

	portal.left.z = GetZ (portal.left);
	portal.right.z = GetZ (portal.right);
}

//--------------------------------------------------------------------------------------------------------------
/*** Return direction from this area to the given point*/
const NavigationMesh::NavDirType_t NavigationMesh::NavigationArea::ComputeDirection (const Math::Vector3D &point) const
{
	if (point.x >= m_extent.mins.x && point.x <= m_extent.maxs.x)
	{
		if (point.y < m_extent.mins.y)
			return NORTH;
		else if (point.y > m_extent.maxs.y)
			return SOUTH;
	}
	else if (point.y >= m_extent.mins.y && point.y <= m_extent.maxs.y)
	{
		if (point.x < m_extent.mins.x)
			return WEST;
		else if (point.x > m_extent.maxs.x)
			return EAST;
	}

	// find closest direction
	const Math::Vector2D to (point.AsVector2D () - m_center.AsVector2D ());

	return Math::fabsf (to.x) > Math::fabsf (to.y) ? to.x > 0.0f ? EAST : WEST : to.y > 0.0f ? SOUTH : NORTH;
}


//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::NavigationArea::GetCornerHotspot (const NavCornerType_t corner, Math::Vector3D hotspot[NUM_CORNERS]) const
{
	// make sure the hotspot doesn't extend outside small areas
	const float size (Math::GetMinimumValueBetween (Math::GetMinimumValueBetween (9.0f, m_extent.GetSizeX () / 3.0f), m_extent.GetSizeY () / 3.0f));

	switch (corner)
	{
		case NORTH_WEST:
			hotspot[NORTH_WEST] = GetCorner (NORTH_WEST);
			hotspot[NORTH_EAST] = hotspot[NORTH_WEST] + Math::Vector2D (size, 0.0f);
			hotspot[SOUTH_EAST] = hotspot[NORTH_WEST] + size;
			hotspot[SOUTH_WEST] = hotspot[NORTH_WEST] + Math::Vector2D (0.0f, size);

			break;

		case NORTH_EAST:
			hotspot[NORTH_WEST] = GetCorner (NORTH_EAST);
			hotspot[NORTH_EAST] = hotspot[NORTH_WEST] - Math::Vector2D (size, 0.0f);
			hotspot[SOUTH_EAST] = hotspot[NORTH_WEST] + Math::Vector2D (-size, size);
			hotspot[SOUTH_WEST] = hotspot[NORTH_WEST] + Math::Vector2D (0.0f, size);

			break;

		case SOUTH_WEST:
			hotspot[NORTH_WEST] = GetCorner (SOUTH_WEST);
			hotspot[NORTH_EAST] = hotspot[NORTH_WEST] + Math::Vector2D (size, 0.0f);
			hotspot[SOUTH_EAST] = hotspot[NORTH_WEST] + Math::Vector2D (size, -size);
			hotspot[SOUTH_WEST] = hotspot[NORTH_WEST] - Math::Vector2D (0.0f, size);

			break;

		#if defined _DEBUG
			case SOUTH_EAST:
		#else	// if defined _DEBUG
			default:	// SOUTH_EAST
		#endif	// if !defined _DEBUG
			hotspot[NORTH_WEST] = GetCorner (SOUTH_EAST);
			hotspot[NORTH_EAST] = hotspot[NORTH_WEST] - Math::Vector2D (size, 0.0f);
			hotspot[SOUTH_EAST] = hotspot[NORTH_WEST] - size;
			hotspot[SOUTH_WEST] = hotspot[NORTH_WEST] - Math::Vector2D (0.0f, size);

			break;

		#if defined _DEBUG
			default:
				AddLogEntry (false, LogLevel_Critical, false, "NavigationArea::GetCornerHotspot(): Invalid corner type %u!", corner);

				return false;
		#endif	// if defined _DEBUG
	}

	for (unsigned char index (NORTH_EAST); index < NUM_CORNERS; ++index)
		hotspot[index].z = GetZ (hotspot[index]);

	Math::Vector3D eyePos, eyeForward;

	g_server->GetNavigationMeshManager ().GetEditVectors (eyePos, eyeForward);

	const Math::Ray ray (eyePos, eyePos + eyeForward * 10000.0f);

	return ray.IntersectWithTriangle (hotspot[NORTH_WEST], hotspot[NORTH_EAST], hotspot[SOUTH_EAST], false) > 0.0f || ray.IntersectWithTriangle (hotspot[SOUTH_EAST], hotspot[SOUTH_WEST], hotspot[NORTH_WEST], false) > 0.0f;
}


//--------------------------------------------------------------------------------------------------------------
const NavigationMesh::NavCornerType_t NavigationMesh::NavigationArea::GetCornerUnderCursor (void) const
{
	Math::Vector3D hotspot[NUM_CORNERS];

	return GetCornerAndHotspotUnderCursor (hotspot);
}
const NavigationMesh::NavCornerType_t NavigationMesh::NavigationArea::GetCornerAndHotspotUnderCursor (Math::Vector3D hotspot[NUM_CORNERS]) const
{
	for (unsigned char index (NORTH_WEST); index < NUM_CORNERS; ++index)
		if (GetCornerHotspot (static_cast <NavCornerType_t> (index), hotspot))
			return static_cast <NavCornerType_t> (index);

	return NUM_CORNERS;
}

//--------------------------------------------------------------------------------------------------------------
/*** Draw area for debugging*/
void NavigationMesh::NavigationArea::Draw (void) const
{
	NavEditColor color (NavNormalColor);
	bool useAttributeColors (true);

	if (IsDegenerate ())
	{
		static HalfLifeEngine::Utilities::IntervalTimer blink;
		static bool blinkOn (false);

		if (blink.GetElapsedTime () > 1.0f)
		{
			blinkOn ^= true;	// XOR it.

			blink.Reset ();
		}

		useAttributeColors = false;
		color = blinkOn ? NavDegenerateFirstColor : NavDegenerateSecondColor;

//		NDebugOverlay::Text (GetCenter (), UTIL_VarArgs ("Degenerate area %u", m_id), true, 0.1f);
		Math::Vector2D screenPosition;

		if (g_server->GetHumanManager ()->GetHostClient ()->GetNormalizedScreenPosition (GetCenter (), screenPosition))
			g_server->GetHumanManager ()->GetHostClient ()->HudMessageCenteredFormat
			(
				screenPosition,
				ColorWithAlpha <> (255u, 0u, 0u, 255u),
				ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
				HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_1,

				"Degenerate area %u.\n", m_id
			);
	}
	else if (g_server->GetNavigationMeshManager ().IsPlaceMode ())
	{
		useAttributeColors = false;
		color = m_place == UNDEFINED_PLACE ? NavNoPlaceColor : g_server->GetNavigationMeshManager ().GetNavPlace () == m_place ? NavSamePlaceColor : NavDifferentPlaceColor;
	}
	else
	{
		// normal edit mode
		if (this == g_server->GetNavigationMeshManager ().GetMarkedArea ())
		{
			useAttributeColors = false;
			color = NavMarkedColor;
		}
		else if (this == g_server->GetNavigationMeshManager ().GetSelectedArea ())
			color = NavSelectedColor;
		else if (m_isBlocked)
			color = NavBlockedColor;
	}

	Math::Vector3D corners[NUM_CORNERS];

	GetCorners (corners);

	#if defined MULTIPLE_DRAW_METHODS
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->GetValue <Console::Bot::VariableValue_DrawMethod_t> () == Console::Bot::VariableValue_DrawMethod_OpenGL)
		{
			ColorWithAlpha <unsigned short> bgcolor;

			if (sscanf (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_area_bgcolor)->GetString (), "%hu %hu %hu %hu", &bgcolor.red, &bgcolor.green, &bgcolor.blue, &bgcolor.alpha) == ColorWithAlpha <>::ID_Total && bgcolor.alpha > 0u)
			{
				const Math::Vector3D offset (0.0f, 0.0f, 0.8f);

//				NDebugOverlay::Triangle (corners[NORTH_WEST] + offset, corners[SOUTH_EAST] + offset, corners[NORTH_EAST] + offset, bgcolor.red, bgcolor.green, bgcolor.blue, bgcolor.alpha, true, 0.15f);
///				g_server->GetHumanManager ()->GetHostClient ()->DrawTriangle (corners[NORTH_WEST] + offset, corners[SOUTH_EAST] + offset, corners[NORTH_EAST] + offset, ColorWithAlpha <> (static_cast <unsigned char> (bgcolor.red), static_cast <unsigned char> (bgcolor.green), static_cast <unsigned char> (bgcolor.blue), static_cast <unsigned char> (bgcolor.alpha)), 15u, 255u, 1u, false);
//				NDebugOverlay::Triangle (corners[SOUTH_EAST] + offset, corners[NORTH_WEST] + offset, corners[SOUTH_WEST] + offset, bgcolor.red, bgcolor.green, bgcolor.blue, bgcolor.alpha, true, 0.15f);
///				g_server->GetHumanManager ()->GetHostClient ()->DrawTriangle (corners[SOUTH_EAST] + offset, corners[NORTH_WEST] + offset, corners[SOUTH_WEST] + offset, ColorWithAlpha <> (static_cast <unsigned char> (bgcolor.red), static_cast <unsigned char> (bgcolor.green), static_cast <unsigned char> (bgcolor.blue), static_cast <unsigned char> (bgcolor.alpha)), 15u, 255u, 1u, false);
				g_server->GetHumanManager ()->GetHostClient ()->DrawQuadrangle (corners[NORTH_WEST] + offset, corners[SOUTH_WEST] + offset, corners[SOUTH_EAST] + offset, corners[NORTH_EAST] + offset, ColorWithAlpha <> (static_cast <unsigned char> (bgcolor.red), static_cast <unsigned char> (bgcolor.green), static_cast <unsigned char> (bgcolor.blue), static_cast <unsigned char> (bgcolor.alpha)), 15u, 255u, 1u, false);
			}
		}
	#endif	// if defined MULTIPLE_DRAW_METHODS

	const float inset (0.2f);

	corners[NORTH_WEST].x += inset;
	corners[NORTH_WEST].y += inset;
	corners[NORTH_EAST].x -= inset;
	corners[NORTH_EAST].y += inset;
	corners[SOUTH_WEST].x += inset;
	corners[SOUTH_WEST].y -= inset;
	corners[SOUTH_EAST].x -= inset;
	corners[SOUTH_EAST].y -= inset;

	if (m_attributeFlags & NAV_MESH_TRANSIENT)
	{
		NavDrawDashedLine (corners[NORTH_WEST], corners[NORTH_EAST], color);
		NavDrawDashedLine (corners[NORTH_EAST], corners[SOUTH_EAST], color);
		NavDrawDashedLine (corners[SOUTH_EAST], corners[SOUTH_WEST], color);
		NavDrawDashedLine (corners[SOUTH_WEST], corners[NORTH_WEST], color);
	}
	else
	{
		NavDrawLine (corners[NORTH_WEST], corners[NORTH_EAST], color);
		NavDrawLine (corners[NORTH_EAST], corners[SOUTH_EAST], color);
		NavDrawLine (corners[SOUTH_EAST], corners[SOUTH_WEST], color);
		NavDrawLine (corners[SOUTH_WEST], corners[NORTH_WEST], color);
	}

	if (this == g_server->GetNavigationMeshManager ().GetMarkedArea ())
	{
		if (g_server->GetNavigationMeshManager ().m_markedCorner < NUM_CORNERS)
		{
			Math::Vector3D hotspot[NUM_CORNERS];

			GetCornerHotspot (g_server->GetNavigationMeshManager ().m_markedCorner, hotspot);

			NavDrawLine (hotspot[NORTH_EAST], hotspot[SOUTH_EAST], NavMarkedColor);
			NavDrawLine (hotspot[SOUTH_EAST], hotspot[SOUTH_WEST], NavMarkedColor);
		}
	}
	else if (this == g_server->GetNavigationMeshManager ().GetSelectedArea () && g_server->GetNavigationMeshManager ().m_navEditMode == NAV_EDIT_NORMAL)
	{
		Math::Vector3D hotspot[NUM_CORNERS];
		const NavCornerType_t cornerUnderCursor (GetCornerAndHotspotUnderCursor (hotspot));

		if (cornerUnderCursor < NUM_CORNERS)
		{
			NavDrawLine (hotspot[NORTH_EAST], hotspot[SOUTH_EAST], NavSelectedColor);
			NavDrawLine (hotspot[SOUTH_EAST], hotspot[SOUTH_WEST], NavSelectedColor);
		}
	}

	if (m_attributeFlags & NAV_MESH_CROUCH)
	{
		if (useAttributeColors)
			color = NavAttributeCrouchColor;

		NavDrawLine (corners[NORTH_WEST], corners[SOUTH_EAST], color);
	}

	if (m_attributeFlags & (NAV_MESH_JUMP | NAV_MESH_DOUBLE_JUMP))
	{
		if (useAttributeColors)
			color = NavAttributeJumpColor;

		if (!(m_attributeFlags & NAV_MESH_CROUCH))
			NavDrawLine (corners[NORTH_WEST], corners[SOUTH_EAST], color);

		NavDrawLine (corners[NORTH_EAST], corners[SOUTH_WEST], color);
	}

	if (m_attributeFlags & NAV_MESH_PRECISE)
	{
		if (useAttributeColors)
			color = NavAttributePreciseColor;

		const float size (8.0f);
		const Math::Vector3D up (m_center.x, m_center.y - size, m_center.z);
		const Math::Vector3D down (m_center.x, m_center.y + size, m_center.z);

		NavDrawLine (up, down, color);

		const Math::Vector3D left (m_center.x - size, m_center.y, m_center.z);
		const Math::Vector3D right (m_center.x + size, m_center.y, m_center.z);

		NavDrawLine (left, right, color);
	}

	if (m_attributeFlags & NAV_MESH_NO_JUMP)
	{
		if (useAttributeColors)
			color = NavAttributeNoJumpColor;

		const float size (8.0f);
		const Math::Vector3D up (m_center.x, m_center.y - size, m_center.z);
		const Math::Vector3D down (m_center.x, m_center.y + size, m_center.z);
		const Math::Vector3D left (m_center.x - size, m_center.y, m_center.z);
		const Math::Vector3D right (m_center.x + size, m_center.y, m_center.z);

		NavDrawLine (up, right, color);
		NavDrawLine (right, down, color);
		NavDrawLine (down, left, color);
		NavDrawLine (left, up, color);
	}

	// Stop is represented by an octogon
	if (m_attributeFlags & NAV_MESH_STOP)
	{
		if (useAttributeColors)
			color = NavAttributeStopColor;

		const float dist (8.0f);
		const float length (dist / 2.5f);
		Math::Vector3D start, end;

		start =	m_center + Math::Vector2D (dist, -length);
		end = m_center + Math::Vector2D (dist, length);

		NavDrawLine (start, end, color);

		start =	m_center + Math::Vector2D (dist, length);
		end = m_center + Math::Vector2D (length, dist);

		NavDrawLine (start, end, color);

		start =	m_center - Math::Vector2D (dist, length);
		end = m_center + Math::Vector2D (-dist, length);

		NavDrawLine (start, end, color);

		start =	m_center + Math::Vector2D (-dist, length);
		end = m_center + Math::Vector2D (-length, dist);

		NavDrawLine (start, end, color);

		start =	m_center + Math::Vector2D (-length, dist);
		end = m_center + Math::Vector2D (length, dist);

		NavDrawLine (start, end, color);

		start =	m_center - Math::Vector2D (dist, length);
		end = m_center - Math::Vector2D (length, dist);

		NavDrawLine (start, end, color);

		start =	m_center - Math::Vector2D (length, dist);
		end = m_center + Math::Vector2D (length, -dist);

		NavDrawLine (start, end, color);

		start =	m_center + Math::Vector2D (length, -dist);
		end = m_center + Math::Vector2D (dist, -length);

		NavDrawLine (start, end, color);
	}

	// Walk is represented by an arrow
	if (m_attributeFlags & NAV_MESH_WALK)
	{
		if (useAttributeColors)
			color = NavAttributeWalkColor;

		const float size (8.0f);

		NavDrawHorizontalArrow (m_center - Math::Vector2D (size, 0.0f), m_center + Math::Vector2D (size, 0.0f), 4, color);
	}

	// Walk is represented by a double arrow
	if (m_attributeFlags & NAV_MESH_RUN)
	{
		if (useAttributeColors)
			color = NavAttributeRunColor;

		const float size (8.0f);
		const float dist (4.0f);

		NavDrawHorizontalArrow (m_center + Math::Vector2D (-size, dist), m_center + Math::Vector2D (size, dist), 4, color);
		NavDrawHorizontalArrow (m_center - Math::Vector2D (size, dist), m_center + Math::Vector2D (size, -dist), 4, color);
	}

	// Avoid is represented by an exclamation point
	if (m_attributeFlags & NAV_MESH_AVOID)
	{
		if (useAttributeColors)
			color = NavAttributeAvoidColor;

		const float topHeight (8.0f);
		const float topWidth (3.0f);
		const float bottomHeight (3.0f);
		const float bottomWidth (2.0f);

		NavDrawTriangle (m_center, m_center + Math::Vector2D (-topWidth, topHeight), m_center + Math::Vector2D (topWidth, topHeight), color);
		NavDrawTriangle (m_center - Math::Vector2D (0.0f, bottomHeight), m_center - Math::Vector2D (bottomWidth, bottomHeight * 2.0f), m_center + Math::Vector2D (bottomWidth, -bottomHeight * 2.0f), color);
	}

	if (m_attributeFlags & NAV_MESH_STAIRS)
	{
		if (useAttributeColors)
			color = NavAttributeStairsColor;

		if (Math::fabsf (Math::GetMidPoint (m_neZ, m_extent.mins.z) - Math::GetMidPoint (m_swZ, m_extent.maxs.z)) >= Math::fabsf (Math::GetMidPoint (m_swZ, m_extent.mins.z) - Math::GetMidPoint (m_neZ, m_extent.maxs.z)))
		{
			float fraction (0.0f);
			const float appendFraction (9.0f / (m_extent.maxs.y - m_extent.mins.y));
			Math::Vector3D start, end;

			do
			{
				start.x = m_extent.mins.x;
				end.x = m_extent.maxs.x;
				start.y = end.y = Math::GetInterpolated (m_extent.mins.y, m_extent.maxs.y, fraction);

				start.z = GetZ (start);
				end.z = GetZ (end);

				NavDrawLine (start, end, color);
			} while ((fraction += appendFraction) <= 1.0f);
		}
		else
		{
			float fraction (0.0f);
			const float appendFraction (9.0f / (m_extent.maxs.x - m_extent.mins.x));
			Math::Vector3D start, end;

			do
			{
				start.x = end.x = Math::GetInterpolated (m_extent.mins.x, m_extent.maxs.x, fraction);
				start.y = m_extent.mins.y;
				end.y = m_extent.maxs.y;

				start.z = GetZ (start);
				end.z = GetZ (end);

				NavDrawLine (start, end, color);
			} while ((fraction += appendFraction) <= 1.0f);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Show hiding spots for debugging.*/
void NavigationMesh::NavigationArea::DrawHidingSpots (void) const
{
	for (HidingSpotArray::IndexType_t index (0u); index < m_hidingSpots.GetElementNumber (); ++index)
	{
		const HidingSpot *const spot (m_hidingSpots[index]);

		NavDrawLine (spot->GetPosition (), spot->GetPosition () + Math::Vector3D (0.0f, 0.0f, 50.0f), spot->IsIdealSniperSpot () ? NavIdealSniperColor : spot->IsGoodSniperSpot () ? NavGoodSniperColor : spot->HasGoodCover () ? NavGoodCoverColor : NavExposedColor);
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Show approach points for debugging.*/
void NavigationMesh::NavigationArea::DrawApproachPoints (void) const
{
	Math::Vector3D approachPointOrigin;

	for (ApproachArray::IndexType_t index (0u); index < m_approach.GetElementNumber (); ++index)
	{
		const ApproachInfo_t &info (m_approach[index]);

		// compute approach point origin
		if (info.hereToNextHow <= GO_WEST)
		{
			info.here->ComputePortal (info.next, static_cast <NavDirType_t> (info.hereToNextHow), approachPointOrigin);

			approachPointOrigin.z = info.next->GetZ (approachPointOrigin);
		}
		else
		{
			// use the area's center as an approach point
			approachPointOrigin = info.here->GetCenter ();
		}

		const unsigned char approachLength (50u), approachRadius (10u);

		NavDrawLine (approachPointOrigin + Math::Vector3D (0.0f, 0.0f, approachLength), approachPointOrigin + Math::Vector2D (approachRadius, 0.0f), NavApproachPointColor);
		NavDrawLine (approachPointOrigin + Math::Vector3D (0.0f, 0.0f, approachLength), approachPointOrigin - Math::Vector2D (approachRadius, 0.0f), NavApproachPointColor);
		NavDrawLine (approachPointOrigin + Math::Vector3D (0.0f, 0.0f, approachLength), approachPointOrigin + Math::Vector2D (0.0f, approachRadius), NavApproachPointColor);
		NavDrawLine (approachPointOrigin + Math::Vector3D (0.0f, 0.0f, approachLength), approachPointOrigin - Math::Vector2D (0.0f, approachRadius), NavApproachPointColor);
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Draw ourselves and adjacent areas*/
void NavigationMesh::NavigationArea::DrawConnectedAreas (void) const
{
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_hiding_spots)->GetValue <bool> () && !g_server->GetNavigationMeshManager ().IsPlaceMode ())
		DrawHidingSpots ();

	// draw connected ladders
	{
		for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[NavigationLadder::Direction_Up].GetElementNumber (); ++index)
		{
			const NavigationLadder *const ladder (m_connectedLadders[NavigationLadder::Direction_Up][index]);

			ladder->Draw ();

			if (!ladder->IsConnected (this, NavigationLadder::Direction_Down))
				NavDrawLine (m_center, ladder->GetBottom () + Math::Vector3D (0.0f, 0.0f, GenerationStepSize), NavConnectedOneWayColor);
		}

		for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[NavigationLadder::Direction_Down].GetElementNumber (); ++index)
		{
			const NavigationLadder *const ladder (m_connectedLadders[NavigationLadder::Direction_Down][index]);

			ladder->Draw ();

			if (!ladder->IsConnected (this, NavigationLadder::Direction_Up))
				NavDrawLine (m_center, ladder->GetTop (), NavConnectedOneWayColor);
		}
	}

	// draw connected areas
	for (unsigned char i (NORTH); i < NUM_DIRECTIONS; ++i)
	{
		const NavDirType_t direction (static_cast <NavDirType_t> (i));
		const NavAreaConnectArray::IndexType_t count (GetAdjacentCount (direction));

		for (NavAreaConnectArray::IndexType_t a (0u); a < count; ++a)
		{
			const NavigationArea *const adj (GetAdjacentArea (direction, a));

			adj->Draw ();

			if (g_server->GetNavigationMeshManager ().IsPlaceMode ())
				continue;

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_show_hiding_spots)->GetValue <bool> ())
				adj->DrawHidingSpots ();

			const unsigned char size (5u);
			Math::Vector3D from, to;

			ComputePortal (adj, direction, from);	// Get the hook position
//			from = m_areaConnections[direction][a].position;

			to.x = from.x;
			to.y = from.y;

			switch (direction)
			{
				case NORTH:
					from.y += size;
					to.y -= size;

					break;

				case SOUTH:
					from.y -= size;
					to.y += size;

					break;

				case EAST:
					from.x -= size;
					to.x += size;

					break;

				default:	// WEST
					from.x += size;
					to.x -= size;

					break;
			}

			from.z = GetZ (from);
			to.z = adj->GetZ (to);

			const Math::Vector3D &drawTo (adj->GetClosestPointOnArea (to));

			NavDrawLine (from, drawTo, adj->IsConnected (this, GetOppositeDirection (direction)) ? NavConnectedTwoWaysColor : NavConnectedOneWayColor);
		}
	}
}

void NavigationMesh::NavigationArea::DrawDanger (void)
{
	Math::Vector3D center (GetCenter ());
	Math::Vector3D top;
	float danger (m_danger[TeamArrayID_Terrorist]);

	center.z = GetZ (center);

	if (danger > 0.1f)
	{
		top.x = center.x;
		top.y = center.y;
		top.z = center.z + 10.0f * danger;

//		NDebugOverlay::Line (center, top, 255, 0, 0, true, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (center, top, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 0u), 255u, 0u, 1u);
	}

	danger = m_danger[TeamArrayID_CounterTerrorist];

	if (danger > 0.1f)
	{
		top.x = center.x;
		top.y = center.y;
		top.z = center.z + 10.0f * danger;

//		NDebugOverlay::Line (center, top, 0, 0, 255, true, 0.1f);
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (center, top, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 255u, 0u, 1u);
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Add to open list in decreasing value order*/
void NavigationMesh::NavigationArea::AddToOpenList (void)
{
	// mark as being on open list for quick check
	m_openMarker = m_masterMarker;

	// if list is empty, add and return
	if (m_openList == NULL)
	{
		m_openList = this;
		m_prevOpen = m_nextOpen = NULL;

		return;
	}

	// insert self in ascending cost order
	NavigationArea *area, *last (NULL);
	for (area = m_openList; area != NULL; area = area->m_nextOpen)
	{
		if (GetTotalCost () < area->GetTotalCost ())
			break;

		last = area;
	}

	if (area != NULL)
	{
		// insert before this area
		m_prevOpen = area->m_prevOpen;

		if (m_prevOpen != NULL)
			m_prevOpen->m_nextOpen = this;
		else
			m_openList = this;

		m_nextOpen = area;
		area->m_prevOpen = this;
	}
	else
	{
		// append to end of list
		last->m_nextOpen = this;

		m_prevOpen = last;
		m_nextOpen = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** A smaller value has been found, update this area on the open list* @todo "bubbling" does unnecessary work, since the order of all other nodes will be unchanged - only this node is altered*/
void NavigationMesh::NavigationArea::UpdateOnOpenList (void)
{
	// since value can only decrease, bubble this area up from current spot
	while (m_prevOpen != NULL && GetTotalCost () < m_prevOpen->GetTotalCost ())
	{
		// swap position with predecessor
		NavigationArea *const other (m_prevOpen);
		NavigationArea *const before (other->m_prevOpen);
		NavigationArea *const after (m_nextOpen);

		m_nextOpen = other;
		m_prevOpen = before;

		other->m_prevOpen = this;
		other->m_nextOpen = after;

		if (before != NULL)
			before->m_nextOpen = this;
		else
			m_openList = this;

		if (after != NULL)
			after->m_prevOpen = other;
	}
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationArea::RemoveFromOpenList (void)
{
	if (m_prevOpen != NULL)
		m_prevOpen->m_nextOpen = m_nextOpen;
	else
		m_openList = m_nextOpen;

	if (m_nextOpen != NULL)
		m_nextOpen->m_prevOpen = m_prevOpen;

	// zero is an invalid marker
	m_openMarker = 0u;
}

//--------------------------------------------------------------------------------------------------------------
/*** Clears the open and closed lists for a new search*/
void NavigationMesh::NavigationArea::ClearSearchLists (void)
{
	// effectively clears all open list pointers and closed flags
	MakeNewMarker ();

	m_openList = NULL;
}

//--------------------------------------------------------------------------------------------------------------
static const bool IsHidingSpotInCover (const Math::Vector3D &spot)
{
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	const Math::Vector3D from (spot.x, spot.y, spot.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight);
	Math::Vector3D to (from.x, from.y, from.z + 20.0f);

//	UTIL_TraceLine (from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);
	TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

	// if we are crouched underneath something, that counts as good cover
	if (traceResult.fraction < 1.0f)
		return true;

	const unsigned char coverRange (100u);
	const unsigned char incCount (8u);
	const float inc (Math::Pi / incCount);
	float cosine, sine;
	unsigned char coverCount (0u);

	for (float angle (0.0f); angle < Math::TwoPi; angle += inc)
	{
		Math::GetCosineSine (angle, cosine, sine);

		to = from + Math::Vector3D (coverRange * cosine, coverRange * sine, HalfLifeEngine::SDK::Constants::HalfHumanHeight);

//		UTIL_TraceLine (from, to, MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);
		TraceLineThruWalkableEntities (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

		// if traceline hit something, it hit "cover"
		if (traceResult.fraction < 1.0f)
			++coverCount;
	}

	// if more than half of the circle has no cover, the spot is not "in cover"
	const unsigned char halfCover (incCount);

	return coverCount >= halfCover;
}

//--------------------------------------------------------------------------------------------------------------
/*** Finds the hiding spot position in a corner's area. If the typical inset is off the navigation area (small* hand-constructed areas), it tries to fit the position inside the area.*/
/*static const Math::Vector3D FindPositionInArea (NavigationMesh::NavigationArea *const area, const NavigationMesh::NavCornerType_t corner) const
{
	struct Mult_t { char x, y; };
	static const Mult_t mults[NUM_CORNERS] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
	const Mult_t &mult (mults[corner]);
	const float offset (12.5f);
	const Math::Vector3D &cornerPos (area->GetCorner (corner));
	Math::Vector3D pos (cornerPos + Math::Vector2D (offset * mult.x,  offset * mult.y));	// Try the basic inset

	if (!area->IsOverlapping (pos))
	{
		// Try pulling the Y offset to the area's center
		pos = cornerPos + Math::Vector2D (offset * mult.x, area->GetExtent ().GetSizeY () * 0.5f * mult.y);

		if (!area->IsOverlapping (pos))
		{
			// Try pulling the X offset to the area's center
			pos = cornerPos + Math::Vector2D (area->GetExtent ().GetSizeX () * 0.5f * mult.x,  offset * mult.y);

			if (!area->IsOverlapping (pos))
			{
				// Try pulling the X and Y offsets to the area's center
				pos = cornerPos + Math::Vector2D (area->GetExtent ().GetSizeX () * 0.5f * mult.x, area->GetExtent ().GetSizeY () * 0.5f * mult.y);

				if (!area->IsOverlapping (pos))
				{
//					AssertMsg (false, UTIL_VarArgs ("A Hiding Spot can't be placed on its area at (%.0f %.0f %.0f)", cornerPos.x, cornerPos.y, cornerPos.z));
					AddLogEntry (true, LogLevel_Warning, false, "A Hiding Spot can't be placed on its area at (%.0f %.0f %.0f)", cornerPos.x, cornerPos.y, cornerPos.z);

					// Just pull the position to a small offset
					pos = cornerPos + Math::Vector2D (mult.x,  mult.y);

					if (!area->IsOverlapping (pos))
					{
						// Nothing is working (degenerate area?), so just put it directly on the corner
						pos = cornerPos;
					}
				}
			}
		}
	}

	return pos;
}*/
const Math::Vector3D NavigationMesh::NavigationArea::FindPositionInArea (const NavCornerType_t corner) const
{
	struct Mult_t { char x, y; };
	static const Mult_t mults[NUM_CORNERS] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
	const Mult_t &mult (mults[corner]);
	const float offset (12.5f);
	const Math::Vector3D &cornerPos (GetCorner (corner));
	Math::Vector3D pos (cornerPos + Math::Vector2D (offset * mult.x, offset * mult.y));	// Try the basic inset

	if (!IsOverlapping (pos))
	{
		// Try pulling the Y offset to the area's center
		pos = cornerPos + Math::Vector2D (offset * mult.x, m_extent.GetSizeY () * 0.5f * mult.y);

		if (!IsOverlapping (pos))
		{
			// Try pulling the X offset to the area's center
			pos = cornerPos + Math::Vector2D (m_extent.GetSizeX () * 0.5f * mult.x, offset * mult.y);

			if (!IsOverlapping (pos))
			{
				// Try pulling the X and Y offsets to the area's center
				pos = cornerPos + Math::Vector2D (m_extent.GetSizeX () * 0.5f * mult.x, m_extent.GetSizeY () * 0.5f * mult.y);

				if (!IsOverlapping (pos))
				{
//					AssertMsg (false, UTIL_VarArgs ("A Hiding Spot can't be placed on its area at (%.0f %.0f %.0f)", cornerPos.x, cornerPos.y, cornerPos.z));
					AddLogEntry (true, LogLevel_Warning, false, "A Hiding Spot can't be placed on its area at (%.0f %.0f %.0f)", cornerPos.x, cornerPos.y, cornerPos.z);

					// Just pull the position to a small offset
					pos = cornerPos + Math::Vector2D (mult.x, mult.y);

					if (!IsOverlapping (pos))
					{
						// Nothing is working (degenerate area?), so just put it directly on the corner
						pos = cornerPos;
					}
				}
			}
		}
	}

	return pos;
}

//--------------------------------------------------------------------------------------------------------------
/*** Analyze local area neighborhood to find "hiding spots" for this area*/
void NavigationMesh::NavigationArea::ComputeHidingSpots (void)
{
	DestroyHidingSpots ();

	// "jump areas" or "don't hide areas" cannot have hiding spots
	if (HasAttributes (NAV_MESH_JUMP | NAV_MESH_DOUBLE_JUMP | NAV_MESH_DONT_HIDE))
		return;

	struct Extent_t
	{
		float mins, maxs;
	} extent;
	unsigned short cornerCount[NUM_CORNERS] = {0u, 0u, 0u, 0u};
	const unsigned char cornerSize (20u);

	// for each direction, find extents of adjacent areas along the wall
	for (unsigned char d (NORTH); d < NUM_DIRECTIONS; ++d)
	{
		extent.mins = 999999.9f;
		extent.maxs = -999999.9f;

		const bool isHoriz (d == NORTH || d == SOUTH);

		for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[d].GetElementNumber (); ++index)
		{
			const NavigationArea *const connect (m_connectedAreas[d][index]);

			// if connection is only one-way, it's a "jump down" connection (ie: a discontinuity that may mean cover)
			if (connect->IsConnected (this, GetOppositeDirection (static_cast <NavDirType_t> (d))) == false)
				continue;	// ignore it

			// ignore jump areas
			if (connect->HasAttributes (NAV_MESH_JUMP | NAV_MESH_DOUBLE_JUMP))
				continue;

			if (isHoriz)
			{
				if (connect->m_extent.mins.x < extent.mins)
					extent.mins = connect->m_extent.mins.x;

				if (connect->m_extent.maxs.x > extent.maxs)
					extent.maxs = connect->m_extent.maxs.x;
			}
			else
			{
				if (connect->m_extent.mins.y < extent.mins)
					extent.mins = connect->m_extent.mins.y;

				if (connect->m_extent.maxs.y > extent.maxs)
					extent.maxs = connect->m_extent.maxs.y;
			}
		}

		switch (d)
		{
			case NORTH:
				if (extent.mins - m_extent.mins.x >= cornerSize)
					++cornerCount[NORTH_WEST];

				if (m_extent.maxs.x - extent.maxs >= cornerSize)
					++cornerCount[NORTH_EAST];

				break;

			case SOUTH:
				if (extent.mins - m_extent.mins.x >= cornerSize)
					++cornerCount[SOUTH_WEST];

				if (m_extent.maxs.x - extent.maxs >= cornerSize)
					++cornerCount[SOUTH_EAST];

				break;

			case EAST:
				if (extent.mins - m_extent.mins.y >= cornerSize)
					++cornerCount[NORTH_EAST];

				if (m_extent.maxs.y - extent.maxs >= cornerSize)
					++cornerCount[SOUTH_EAST];

				break;

			default:	// WEST
				if (extent.mins - m_extent.mins.y >= cornerSize)
					++cornerCount[NORTH_WEST];

				if (m_extent.maxs.y - extent.maxs >= cornerSize)
					++cornerCount[SOUTH_WEST];

				break;
		}
	}

	for (unsigned char c (NORTH_WEST); c < NUM_CORNERS; ++c)
	{
		// if a corner count is 2, then it really is a corner (walls on both sides)
		if (cornerCount[c] != 2u)
			continue;

		const Math::Vector3D pos (FindPositionInArea (static_cast <NavCornerType_t> (c)));

		if (c == NORTH_WEST || !IsHidingSpotCollision (pos))
		{
			HidingSpot *const spot (new HidingSpot (pos, IsHidingSpotInCover (pos) ? static_cast <HidingSpot::Flag_t> (HidingSpot::IN_COVER)/*! note I DON'T ASK WHY I CAST 'HidingSpot::IN_COVER' TO 'HidingSpot::Flag_t', OTHERWISE I GOT C4244 WTF??? */ : HidingSpot::EXPOSED));

			#if defined _DEBUG
				// Reliability check.
				if (spot == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating hiding spot at location: %s!", sizeof (HidingSpot), pos.ToString ());

					return;
				}
			#endif	// if defined _DEBUG

			m_hidingSpots += spot;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Determine how much walkable area we can see from the spot, and how far away we can see.*/
void NavigationMesh::NavigationArea::HidingSpot::ClassifySniperSpot (void)
{
	Math::Vector3D eye (m_pos);
	const NavigationArea *const hidingArea (g_server->GetNavigationMeshManager ().GetNavArea (eye));

	// we will be standing at this hiding spot/we are crouching when at this hiding spot
	eye.z += hidingArea != NULL && hidingArea->HasAttributes (NAV_MESH_STAND) ? 0.8f * HalfLifeEngine::SDK::Constants::HumanHeight : HalfLifeEngine::SDK::Constants::HalfHumanHeight;

	Math::Vector3D walkable;
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	Math::Extent2D sniperExtent;
	float farthestRangeSq (0.0f);
	const float minimumSniperRangeSq (MATH_GET_SQUARED (1000.0f));
	bool found (false);

	for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
	{
		const NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);
		const Math::Extent2D &extent (area->GetExtent ());

		// scan this area
		for (walkable.y = extent.mins.y + GenerationStepSize * 0.5f; walkable.y < extent.maxs.y; walkable.y += GenerationStepSize)
			for (walkable.x = extent.mins.x + GenerationStepSize * 0.5f; walkable.x < extent.maxs.x; walkable.x += GenerationStepSize)
			{
				walkable.z = area->GetZ (walkable) + HalfLifeEngine::SDK::Constants::HalfHumanHeight;

				// check line of sight
//				UTIL_TraceLine (eye, walkable, CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP, NULL, COLLISION_GROUP_NONE, &traceResult);
//				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (eye, walkable, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

//				if (traceResult.isStartSolid || traceResult.fraction < 1.0f)
				if (!TraceLineThruWalkableEntities (eye, walkable, HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult))
					continue;

				// can see this spot

				// keep track of how far we can see
				const float rangeSq (eye.GetDistanceSquared (walkable));
				if (rangeSq > farthestRangeSq)
				{
					farthestRangeSq = rangeSq;

					if (rangeSq >= minimumSniperRangeSq)
					{
						// this is a sniper spot
						// determine how good of a sniper spot it is by keeping track of the snipable area
						if (found)
						{
							if (sniperExtent.mins.x > walkable.x)
								sniperExtent.mins.x = walkable.x;
							else if (sniperExtent.maxs.x < walkable.x)
								sniperExtent.maxs.x = walkable.x;

							if (sniperExtent.mins.y > walkable.y)
								sniperExtent.mins.y = walkable.y;
							else if (sniperExtent.maxs.y < walkable.y)
								sniperExtent.maxs.y = walkable.y;
						}
						else
						{
							sniperExtent = walkable.AsVector2D ();
							found = true;
						}
					}
				}
			}
	}

	if (found == false)
		return;

	const float snipableArea (sniperExtent.GetArea2D ());	// if we can see a large snipable area, it is an "ideal" spot
	const float minIdealSniperArea (MATH_GET_SQUARED (200.0f));
	const float longSniperRangeSq (MATH_GET_SQUARED (1500.0f));

	m_flags |= snipableArea >= minIdealSniperArea || farthestRangeSq >= longSniperRangeSq ? IDEAL_SNIPER_SPOT : GOOD_SNIPER_SPOT;
}


//--------------------------------------------------------------------------------------------------------------
/*** Analyze local area neighborhood to find "sniper spots" for this area*/
void NavigationMesh::NavigationArea::ComputeSniperSpots (void)
{
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_quicksave)->GetValue <bool> ())
		return;

	for (HidingSpotArray::IndexType_t index (0u); index < m_hidingSpots.GetElementNumber (); ++index)
		m_hidingSpots[index]->ClassifySniperSpot ();
}

//--------------------------------------------------------------------------------------------------------------
/*** Given the areas we are moving between, return the spots we will encounter*/
const NavigationMesh::NavigationArea::SpotEncounter *const NavigationMesh::NavigationArea::GetSpotEncounter (const NavigationArea *const from, const NavigationArea *const to) const
{
	InternalAssert (from != NULL && to != NULL);

	for (SpotEncounterArray::IndexType_t index (0u); index < m_spotEncounters.GetElementNumber (); ++index)
	{
		const SpotEncounter *const e (m_spotEncounters[index]);

		if (e->from == from && e->to == to)
			return e;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
/*** Add spot encounter data when moving from area to area*/
void NavigationMesh::NavigationArea::AddSpotEncounters (NavigationArea *const from, const NavDirType_t fromDir, NavigationArea *const to, const NavDirType_t toDir)
{
	SpotEncounter *const e (new SpotEncounter ());

	#if defined _DEBUG
		// Reliability check.
		if (e == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating spot encounter!", sizeof (SpotEncounter));

			return;
		}
	#endif	// if defined _DEBUG

	e->from = from;
	e->fromDir = fromDir;

	e->to = to;
	e->toDir = toDir;

	ComputePortal (from, fromDir, e->path.from);
	ComputePortal (to, toDir, e->path.to);

	const float eyeHeight (HalfLifeEngine::SDK::Constants::HalfHumanHeight);

	e->path.from.z = from->GetZ (e->path.from) + eyeHeight;
	e->path.to.z = to->GetZ (e->path.to) + eyeHeight;

	// step along ray and track which spots can be seen
	Math::Vector3D direction (e->path.to - e->path.from);
	const float length (direction.GetLength ());

	// Reliability check.
	InternalAssert (length > 0.0f);	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	direction /= length/* + Math::Epsilon*/;	// Normalize.

	// create unique marker to flag used spots
	HidingSpot::ChangeMasterMarker ();

	const unsigned char stepSize (25u);	// 50
	const float seeSpotRange (MATH_GET_SQUARED (2000.0f));	// 3000
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	Math::Vector3D eye, delta;
	HidingSpot *spot;
	SpotEncounter::SpotOrder spotOrder;
	float along (0.0f);
	bool done (false);

	// step along path thru this area
	do
	{
		// make sure we check the endpoint of the path segment
		if (along >= length)
		{
			along = length;
			done = true;
		}

		// move the eyepoint along the path segment
		eye = e->path.from + direction * along;

		// check each hiding spot for visibility
		for (NavigationMesh::HidingSpotArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetHidingSpots ().GetElementNumber (); ++index)
		{
			spot = g_server->GetNavigationMeshManager ().GetHidingSpots ()[index];

			// only look at spots with cover (others are out in the open and easily seen)
			if (!spot->HasGoodCover ())
				continue;

			if (spot->IsMarked ())
				continue;

			const Math::Vector3D &spotPos (spot->GetPosition ());

			delta.x = spotPos.x - eye.x;
			delta.y = spotPos.y - eye.y;
			delta.z = spotPos.z + eyeHeight - eye.z;

			// check if in see spot range
			if (delta.GetLengthSquared () > seeSpotRange)
				continue;

			// check if we have LOS
			// BOTPORT: ignore glass here
//			UTIL_TraceLine (eye, Math::Vector3D (spotPos.x, spotPos.y, spotPos.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight), MASK_PLAYERSOLID_BRUSHONLY, NULL, COLLISION_GROUP_NONE, &traceResult);
//			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (eye, Math::Vector3D (spotPos.x, spotPos.y, spotPos.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight), HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);
			TraceLineThruWalkableEntities (eye, Math::Vector3D (spotPos.x, spotPos.y, spotPos.z + HalfLifeEngine::SDK::Constants::HalfHumanHeight), HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult);

			if (traceResult.fraction < 1.0f)
				continue;

			const float dot (direction | delta.Normalize ());

			// if spot is in front of us along our path, ignore it
			if (dot < 0.7071f && dot > -0.7071f)
			{
				// we only want to keep spots that BECOME visible as we walk past them
				// therefore, skip ALL visible spots at the start of the path segment
				if (along > 0.0f)
				{
					spotOrder.spot = spot;
					spotOrder.t = along / length;

					// add spot to encounter
					e->spots += spotOrder;
				}
			}

			// mark spot as encountered
			spot->Mark ();
		}

		along += stepSize;
	} while (done == false);

	// add encounter to list
	m_spotEncounters += e;
}

//--------------------------------------------------------------------------------------------------------------
/*** Compute "spot encounter" data. This is an ordered list of spots to look at for each possible path thru a navigation area.*/
void NavigationMesh::NavigationArea::ComputeSpotEncounters (void)
{
	DestroySpotEncounters ();

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_quicksave)->GetValue <bool> ())
		return;

	// for each adjacent area
	for (unsigned char fromDir (NORTH); fromDir < NUM_DIRECTIONS; ++fromDir)
		for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[fromDir].GetElementNumber (); ++index)
		{
			NavigationArea *const from (m_connectedAreas[fromDir][index]);

			// compute encounter data for path to each adjacent area
			for (unsigned char toDir (NORTH); toDir < NUM_DIRECTIONS; ++toDir)
				for (NavAreaConnectArray::IndexType_t index2 (0u); index2 < m_connectedAreas[toDir].GetElementNumber (); ++index2)
				{
					NavigationArea *const to (m_connectedAreas[toDir][index2]);

					if (from == to)
						continue;

					// just do our direction, as we'll loop around for other direction
					AddSpotEncounters (from, static_cast <NavDirType_t> (fromDir), to, static_cast <NavDirType_t> (toDir));
				}
		}
}

//--------------------------------------------------------------------------------------------------------------
/*** Raise/lower a corner*/
void NavigationMesh::NavigationArea::RaiseCorner (const NavCornerType_t corner, const short amount, const bool raiseAdjacentCorners/* = true*/)
{
	// Reliability check.
	InternalAssert (corner < NUM_CORNERS);

	// Move the corner
	switch (corner)
	{
		case NORTH_WEST:
			m_extent.mins.z += amount;

			Label_RecomputeCenterAndInvertedSize:
			{
				// Recompute the center....
				m_center = m_extent.mins.GetMidPoint (m_extent.maxs);

				// Recompute extent inverted size....
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
			}

			break;

		case NORTH_EAST:
			m_neZ += amount;

			break;

		case SOUTH_WEST:
			m_swZ += amount;

			break;

		default:	// SOUTH_EAST
			m_extent.maxs.z += amount;

			goto Label_RecomputeCenterAndInvertedSize;
	}

	if (!raiseAdjacentCorners || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_corner_adjust_adjacent)->GetValue <float> () <= 0.0f)
		return;

	// Find nearby areas that share the corner
	MakeNewMarker ();
	Mark ();

	const float toleranceSquared (MATH_GET_SQUARED (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_corner_adjust_adjacent)->GetValue <float> ()));
	Math::Vector3D cornerPos (GetCorner (corner));
	cornerPos.z -= amount;	// use the pre-adjustment corner for adjacency checks
	const unsigned char gridX (g_server->GetNavigationMeshManager ().WorldToGridX (cornerPos.x));
	const unsigned char gridY (g_server->GetNavigationMeshManager ().WorldToGridY (cornerPos.y));
	const unsigned char shift (1u);	// try a 3x3 set of grids in case we're on the edge

	for (unsigned char x (gridX - shift), y; x <= gridX + shift; ++x)
	{
		if (x >= g_server->GetNavigationMeshManager ().m_gridSize.x)
			continue;

		for (y = gridY - shift; y <= gridY + shift; ++y)
		{
			if (y >= g_server->GetNavigationMeshManager ().m_gridSize.y)
				continue;

			const NavAreaArray &array (g_server->GetNavigationMeshManager ().m_grid[x + y * g_server->GetNavigationMeshManager ().m_gridSize.x]);

			// find closest area in this cell
			for (NavAreaArray::IndexType_t index (0u); index < array.GetElementNumber (); ++index)
			{
				NavigationArea *const area (array[index]);

				// skip if we've already visited this area
				if (area->IsMarked ())
					continue;

				area->Mark ();

				for (unsigned char i (NORTH_WEST); i < NUM_CORNERS; ++i)
				{
					const Math::Vector3D &areaPos (area->GetCorner (static_cast <NavCornerType_t> (i)));

					if (areaPos.GetDistanceSquared (cornerPos) < toleranceSquared)
					{
						const short heightDiff (static_cast <short> ((cornerPos.z + amount) - areaPos.z));

						area->RaiseCorner (static_cast <NavCornerType_t> (i), heightDiff, false);
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/*** Walks from the start position to the end position in GenerationStepSize increments,* checking the ground height along the way.*/
const float NavigationMesh::NavigationArea::FindGroundZFromPoint (const Math::Vector3D &end, const Math::Vector3D &start)
{
	Math::Vector3D step (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::StepHeight);

	if (Math::fabsf (end.x - start.x) > Math::fabsf (end.y - start.y))
	{
		step.x = GenerationStepSize;

		if (end.x < start.x)
			step.x = -step.x;
	}
	else
	{
		step.y = GenerationStepSize;

		if (end.y < start.y)
			step.y = -step.y;
	}

	Math::Vector3D point (start);
	float z;

	// step towards our end point
	while (point.GetDistanceSquared2D (end) > MATH_GET_SQUARED (GenerationStepSize))
	{
		point += step;

		z = point.z;

		if (GetGroundHeight (point, &z))
			point.z = z;
		else
			point.z -= step.z;
	}

	z = point.z + step.z;
	point = end;
	point.z = z;

	// now do the exact one once we're within GenerationStepSize of it
	if (GetGroundHeight (point, &z))
		point.z = z;
	else
		point.z -= step.z;

	return point.z;
}


//--------------------------------------------------------------------------------------------------------------
/*** Finds the Z value for a corner given two other corner points. This walks along the edges of the navigation area* in GenerationStepSize increments, to increase accuracy.*/
const float NavigationMesh::NavigationArea::FindGroundZ (const Math::Vector3D &original, const Math::Vector3D &corner1, const Math::Vector3D &corner2)
{
	const float first (FindGroundZFromPoint (original, corner1));
	const float second (FindGroundZFromPoint (original, corner2));

	if (!Math::AreEqual2 (first, second, HalfLifeEngine::SDK::Constants::StepHeight))
	{
		// approaching the point from the two directions didn't agree. Take the one closest to the original z.
		return Math::fabsf (original.z - first) > Math::fabsf (original.z - second) ? second : first;
	}

	return first;
}


//--------------------------------------------------------------------------------------------------------------
/*** Places a corner (or all corners if corner == NUM_CORNERS) on the ground*/
void NavigationMesh::NavigationArea::PlaceOnGround (const NavCornerType_t corner, const float inset/* = 0.0f*/)
{
	const Math::Vector3D nw (m_extent.mins + inset);
	const Math::Vector3D se (m_extent.maxs - inset);
	const Math::Vector3D ne (se.x, nw.y, m_neZ), sw (nw.x, se.y, m_swZ);

	switch (corner)
	{
		case NORTH_WEST:
			RaiseCorner (NORTH_WEST, static_cast <short> (FindGroundZ (nw, ne, sw) - nw.z));

			break;

		case NORTH_EAST:
			RaiseCorner (NORTH_EAST, static_cast <short> (FindGroundZ (ne, nw, se) - ne.z));

			break;

		case SOUTH_WEST:
			RaiseCorner (SOUTH_WEST, static_cast <short> (FindGroundZ (sw, nw, se) - sw.z));

			break;

		case SOUTH_EAST:
			RaiseCorner (SOUTH_EAST, static_cast <short> (FindGroundZ (se, ne, sw) - se.z));

			break;

		default:	// NUM_CORNERS
			RaiseCorner (NORTH_WEST, static_cast <short> (FindGroundZ (nw, ne, sw) - nw.z));
			RaiseCorner (NORTH_EAST, static_cast <short> (FindGroundZ (ne, nw, se) - ne.z));
			RaiseCorner (SOUTH_WEST, static_cast <short> (FindGroundZ (sw, nw, se) - sw.z));
			RaiseCorner (SOUTH_EAST, static_cast <short> (FindGroundZ (se, ne, sw) - se.z));

			break;
	}
}

/*
//--------------------------------------------------------------------------------------------------------------
static void CommandNavUpdateBlocked (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	if (g_server->GetNavigationMeshManager ().GetMarkedArea ())
	{
		NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetMarkedArea ());

		area->UpdateBlocked ();

		if (area->IsBlocked () && Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
			AddLogEntry (true, LogLevel_Default, false, "Area #%u %s is blocked.", area->GetID (), (area->GetCenter () + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)).ToString ());
	}
	else
	{
		const float start (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
		NavigationMesh::NavigationArea *blockedArea (NULL);

		for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			area->UpdateBlocked ();

			if (area->IsBlocked ())
			{
				if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
					AddLogEntry (true, LogLevel_Default, false, "Area #%u %s is blocked.", area->GetID (), (area->GetCenter () + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)).ToString ());

				if (blockedArea == NULL)
					blockedArea = area;
			}
		}

		if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
		{
			const float end (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
			const float time ((end - start) * 1000.0f);

			AddLogEntry (true, LogLevel_Default, false, "nav_update_blocked took %2.2f ms.", time);
		}

		if (blockedArea != NULL)
		{
			const Client *const player (g_server->GetHumanManager ()->GetHostClient ());

			if (player->IsValid ())
				if ((!player->IsAlive () || player->IsSpectator ()) && player->GetEdict ()->variables.iuser1 == 5)
					player->GetEdict ()->SetOrigin (blockedArea->GetCenter () + Math::Vector3D (0.0f, 0.0f, 0.75f * HalfLifeEngine::SDK::Constants::HumanHeight));
		}
	}
}
static ConCommand nav_update_blocked ("nav_update_blocked", CommandNavUpdateBlocked, "Updates the blocked/unblocked status for every navigation area.", FCVAR_GAMEDLL);
*/
/*
enum IterationRetval_t
{
	ITERATION_CONTINUE,
	ITERATION_STOP,
};

//-----------------------------------------------------------------------------
// Any search in the CSpatialPartition must use this to filter out entities it doesn't want.
// You're forced to use listMasks because it can filter by listMasks really fast. Any other
// filtering can be done by EnumElement.
//-----------------------------------------------------------------------------

class IPartitionEnumerator
{
public:
	virtual IterationRetval_t EnumElement( IHandleEntity *pHandleEntity ) = 0;
};

//-----------------------------------------------------------------------------------------------------
class CNavBlockerEnumerator : public IPartitionEnumerator
{
	public:
		HalfLifeEngine::SDK::Classes::Edict **m_AlreadyHit;
		unsigned int                          m_nAlreadyHit;
		unsigned int                          m_nMaxHits;

	public:
		// Forced constructor
		inline CNavBlockerEnumerator (HalfLifeEngine::SDK::Classes::Edict **ents, const unsigned int nMaxEnts)
		{
			m_nAlreadyHit = 0u;
			m_AlreadyHit = ents;
			m_nMaxHits = nMaxEnts;
		}

	public:
		// Actual work code
		virtual IterationRetval_t EnumElement (IHandleEntity *pHandleEntity)
		{
			HalfLifeEngine::SDK::Classes::Edict *entity (gEntList.GetBaseEntity (pHandleEntity->GetRefEHandle ()));

			if (entity == NULL)
				return ITERATION_CONTINUE;

			if (entity->GetCollisionGroup () != COLLISION_GROUP_PUSHAWAY && entity->GetCollisionGroup () != COLLISION_GROUP_BREAKABLE_GLASS && entity->GetCollisionGroup () != COLLISION_GROUP_PLAYER && entity->GetCollisionGroup () != COLLISION_GROUP_NONE)
				return ITERATION_CONTINUE;

			IMultiplayerPhysics *pInterface (dynamic_cast <IMultiplayerPhysics *> (entity));

			if (pInterface != NULL)
			{
				if (pInterface->GetMultiplayerPhysicsMode () != PHYSICS_MULTIPLAYER_SOLID)
					return ITERATION_CONTINUE;
			}
			else
			{
				if (entity->GetClassName () == "func_breakable")
				{
					// If we won't be able to break it, don't try
					if (entity.GetTakeDamageType () != DAMAGE_YES)
						return ITERATION_CONTINUE;
				}
				else if (entity->GetClassName ().CompareWithCaseCheck ("func_door", 9u) == 0)	// doors
				{
				}
				else
					return ITERATION_CONTINUE;
			}

			if (m_nAlreadyHit < m_nMaxHits)
				m_AlreadyHit[m_nAlreadyHit++] = entity;

			return ITERATION_CONTINUE;
		}
};


//-----------------------------------------------------------------------------------------------------
static const unsigned int GetPushawayEntsInVolume (const Math::Vector3D &origin, const Math::Vector3D &mins, const Math::Vector3D &maxs, HalfLifeEngine::SDK::Classes::Edict **ents, int nMaxEnts, const SpatialPartitionListMask_t partitionMask, CNavBlockerEnumerator *enumerator)
{
	const Math::Ray ray (origin, origin, mins, maxs);
	CNavBlockerEnumerator *physPropEnum (NULL);

	if (enumerator == NULL)
	{
		physPropEnum = new CNavBlockerEnumerator (ents, nMaxEnts);

		enumerator = physPropEnum;
	}

	partition->EnumerateElementsAlongRay (partitionMask, ray, false, enumerator);

	const unsigned int numHit (enumerator->m_nAlreadyHit);

	if (physPropEnum != NULL)
		delete physPropEnum;

	return numHit;
}
*/

//--------------------------------------------------------------------------------------------------------------
/*** Updates the (un)blocked status of the navigation area*/
void NavigationMesh::NavigationArea::UpdateBlocked (void)
{
	Math::Vector3D origin (GetCenter ());

	origin.z += HalfLifeEngine::SDK::Constants::HalfHumanHeight;
/*
	const float sizeX (Math::GetMaximumValueBetween (1.0f, Math::GetMinimumValueBetween (m_extent.GetSizeX () * 0.5f - 5.0f, HalfLifeEngine::SDK::Constants::HalfHumanWidth)));
	const float sizeY (Math::GetMaximumValueBetween (1.0f, Math::GetMinimumValueBetween (m_extent.GetSizeY () * 0.5f - 5.0f, HalfLifeEngine::SDK::Constants::HalfHumanWidth)));
	const Math::Vector3D mins (-sizeX, -sizeY, 0.0f);
	const Math::Vector3D maxs (sizeX, sizeY, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z - HalfLifeEngine::SDK::Constants::HalfHumanHeight);
	const bool wasBlocked (m_isBlocked);
*/
	// See if spot is valid
//	CTraceFilterWalkableEntities filter (NULL, COLLISION_GROUP_PLAYER_MOVEMENT, WALK_THRU_DOORS | WALK_THRU_BREAKABLES);
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceHull (origin, origin, mins, maxs, MASK_PLAYERSOLID_BRUSHONLY, &filter, &traceResult);
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (origin, origin, true, HalfLifeEngine::SDK::Constants::Hull_Human, NULL, traceResult);

	m_isBlocked = traceResult.isStartSolid == TRUE;
/*MADE BY EDWARD!!!!!!!! - START
	if (wasBlocked != m_isBlocked)
	{
		IGameEvent *const event (gameeventmanager->CreateEvent ("nav_blocked"));

		if (event != NULL)
		{
			event->SetInt ("area", m_id);
			event->SetInt ("blocked", m_isBlocked);

			gameeventmanager->FireEvent (event);
		}
	}
MADE BY EDWARD!!!!!!!! - END/*
	if (m_isBlocked)
		NDebugOverlay::Box (origin, mins, maxs, 255, 0, 0, 64, 3.0f);
	else
		NDebugOverlay::Box (origin, mins, maxs, 0, 255, 0, 64, 3.0f); */
}


//--------------------------------------------------------------------------------------------------------------
/*** Checks if there is a floor under the navigation area, in case a breakable floor is gone*/
void NavigationMesh::NavigationArea::CheckFloor (const HalfLifeEngine::SDK::Classes::Edict *const/* ignore*/)
{
	if (m_isBlocked)
		return;

	const Math::Vector3D origin (m_center.x, m_center.y, m_center.z - HalfLifeEngine::SDK::Constants::JumpCrouchHeight);
/*	const float size (GenerationStepSize * 0.5f);
	const Math::Vector3D mins (Math::Vector3D (-size, -size, 0.0f));
	const Math::Vector3D maxs (Math::Vector3D (size, size, HalfLifeEngine::SDK::Constants::JumpCrouchHeight + 10.0f));
	const bool wasBlocked (m_isBlocked);
*/
	// See if spot is valid
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

//	UTIL_TraceHull (origin, origin, mins, maxs, MASK_PLAYERSOLID_BRUSHONLY, ignore, COLLISION_GROUP_PLAYER_MOVEMENT, &traceResult);
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (origin, origin, true, HalfLifeEngine::SDK::Constants::Hull_Human, NULL, traceResult);

	// If the center is open space, we're effectively blocked
	m_isBlocked = traceResult.isStartSolid == FALSE;
/*MADE BY EDWARD!!!!!!!! - START
	if (wasBlocked != m_isBlocked)
	{
		IGameEvent *const event (gameeventmanager->CreateEvent ("nav_blocked"));

		if (event != NULL)
		{
			event->SetInt ("area", m_id);
			event->SetInt ("blocked", m_isBlocked);

			gameeventmanager->FireEvent (event);
		}
	}
MADE BY EDWARD!!!!!!!! - END/*
	if (m_isBlocked)
		NDebugOverlay::Box (origin, mins, maxs, 255, 0, 0, 64, 3.0f);
	else
		NDebugOverlay::Box (origin, mins, maxs, 0, 255, 0, 64, 3.0f);*/
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavigationArea::CheckWaterLevel (void)
{
	Math::Vector3D pos (m_center);

	if (!NavigationMesh::GetGroundHeight (pos, &pos.z))
	{
		m_isUnderwater = false;

		return;
	}

	++pos.z;

	m_isUnderwater = HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (pos) == HalfLifeEngine::SDK::Constants::Content_Water;
}

//--------------------------------------------------------------------------------------------------------------
const bool NavigationMesh::NavigationArea::TestJump (void)
{
	RemoveAttributes (NAV_MESH_JUMP);

	if (!HasNodes ())
		return false;
#if 0
{/* FROM NEW SOURCE ENGINE - WORKS NOT FINE!!! */
	const float normalZ (ComputeNormal ().z);
	float alternateNormalZ (ComputeAlternateNormal ().z);
	Math::Vector3D groundNormal;

	if (alternateNormalZ > normalZ)
		alternateNormalZ = normalZ;

	if
	(
		alternateNormalZ >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> () &&
		(
			alternateNormalZ >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> () + Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_tolerance)->GetValue <float> () ||
			!GetSimpleGroundHeight (Math::Vector3D (GetCenter ().x, GetCenter ().y, GetCenter ().z + HalfLifeEngine::SDK::Constants::HalfHumanHeight), NULL, &groundNormal) ||
			Math::AreEqual2 (groundNormal.z, alternateNormalZ, Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_tolerance)->GetValue <float> ())
		)
	)
		return false;
}
#endif	// if 0
{/* FROM OLD SOURCE ENGINE */
	const float normalZ (ComputeNormal ().z), alternateNormalZ (ComputeAlternateNormal ().z);

	if
	(
		normalZ >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> () &&
		alternateNormalZ >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ()
	)
		return false;
}
/*
{
	Math::Vector3D corners[NUM_CORNERS];

	GetCorners (corners);

	const Math::Vector3D midPoints[NUM_DIRECTIONS] =
	{
		corners[NORTH_WEST].GetMidPoint (corners[NORTH_EAST]),
		corners[NORTH_EAST].GetMidPoint (corners[SOUTH_EAST]),
		corners[SOUTH_EAST].GetMidPoint (corners[SOUTH_WEST]),
		corners[SOUTH_WEST].GetMidPoint (corners[NORTH_WEST])
	};
	float highestZ (midPoints[NORTH].z);
	NavDirType_t highestDirection (NORTH);

	for (unsigned char direction (EAST); direction < NUM_DIRECTIONS; ++direction)
		if (highestZ < midPoints[direction].z)
		{
			highestZ = midPoints[direction].z;
			highestDirection = static_cast <NavDirType_t> (direction);
		}

	const float upZ (midPoints[highestDirection].z);
	const float downZ (midPoints[GetOppositeDirection (highestDirection)].z);

	// Reliability check.
	InternalAssert (upZ > downZ);	// OCCURS!!! (DE_CONTRA)

	if (upZ - downZ > HalfLifeEngine::SDK::Constants::JumpCrouchHeight)
	{
		AddAttributes (NAV_MESH_DOUBLE_JUMP | NAV_MESH_UNMERGEABLE);

		return false;
	}
}*/
	AddAttributes (NAV_MESH_JUMP | NAV_MESH_UNMERGEABLE);

	return true;
}

//--------------------------------------------------------------------------------------------------------------
const unsigned char NavigationMesh::NavigationArea::IsStairs (const Math::Vector3D &source, const Math::Vector3D &destination, const unsigned char oldResult)
{
	// Reliability check.
	InternalAssert (oldResult != FALSE);

	if (Math::AreEqual2 (source.z, destination.z, HalfLifeEngine::SDK::Constants::StepHeight))
		return oldResult;

/*	Math::Vector2D mins (-2.5f, -2.5f, 0.0f), maxs (2.5f, 2.5f, 1.0f);

	if (Math::fabsf (source.x - destination.x) <= Math::fabsf (source.y - destination.y))
	{
		mins.y = -8.0f;
		maxs.y =  8.0f;
	}
	else
	{
		mins.x = -8.0f;
		maxs.x =  8.0f;
	}
*/
//	TraceFilterNoNPCsOrPlayer traceFilter;
//	trace_t traceResult;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
#if 0
	UTIL_TraceHull
	(
		source + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight),
		source - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight),

		mins,
		maxs,

		33701899,
		&traceFilter,
		&traceResult
	);
#endif	// if 0
	TraceLineThruWalkableEntities
	(
		source + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight),
		source - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight),

		HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult
	);

	if (traceResult.isStartSolid/* || (traceResult.dispFlags & DISPSURF_FLAG_SURFACE)*/)
		return FALSE;

	const float minimumStairHeight (Math::tanf (Math::acosf (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())) * 5.0f);
	const float distance2D (source.GetDistance2D (destination));
	Math::Vector3D from, to;
	float endPositionZ (traceResult.endPosition.z);
	const float step (5.0f / distance2D);
	const float slopeLimit (0.97f);
	float fraction (0.0f);
	unsigned char result (oldResult);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		from = to = source.GetInterpolated (destination, fraction);

		from.z += HalfLifeEngine::SDK::Constants::HalfHumanHeight;
		to.z -= HalfLifeEngine::SDK::Constants::HalfHumanHeight;
/*
		UTIL_TraceHull
		(
			from,
			to,

			mins,
			maxs,

			33701899,
			&traceFilter,
			&traceResult
		);*/
		TraceLineThruWalkableEntities
		(
			from,
			to,

			HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult
		);

		if (traceResult.isStartSolid/* || (traceResult.dispFlags & DISPSURF_FLAG_SURFACE)*/)
			break;

		if (fraction == 0.0f && !Math::AreEqual2 (traceResult.endPosition.z, source.z, HalfLifeEngine::SDK::Constants::StepHeight))
			break;

		if ((fraction == 1.0f && !Math::AreEqual2 (traceResult.endPosition.z, destination.z, HalfLifeEngine::SDK::Constants::StepHeight)) || traceResult.planeNormal.z < slopeLimit)
			break;

		const float deltaZ (Math::fabsf (traceResult.endPosition.z - endPositionZ));

		if (deltaZ > HalfLifeEngine::SDK::Constants::StepHeight)
			break;

		if (deltaZ >= minimumStairHeight)
			result = TRUE;

		if ((fraction += step) > 1.0f)
			return result;

		endPositionZ = traceResult.endPosition.z;
	}

	return FALSE;
}
const bool NavigationMesh::NavigationArea::TestStairs (void)
{
	RemoveAttributes (NAV_MESH_STAIRS);

	// Area too small?
	if (m_extent.GetSizeX () <= GenerationStepSize && m_extent.GetSizeY () <= GenerationStepSize)
		return false;

	const float dot (0.95f);

	if ((ComputeNormal () | ComputeAlternateNormal ()) < dot)
		return false;

	Math::Vector3D corners[NUM_CORNERS];
	unsigned char isStairsResult (2u);
	const float checkOffset (5.0f);

	GetCorners (corners);

	Math::Vector3D source (corners[NORTH_WEST].x + checkOffset, corners[NORTH_WEST].y + checkOffset, corners[NORTH_WEST].z);
	Math::Vector3D destination (corners[NORTH_EAST].x - checkOffset, corners[NORTH_EAST].y + checkOffset, corners[NORTH_EAST].z);

	isStairsResult = IsStairs (source, destination, isStairsResult);

	if (isStairsResult == FALSE)
		return false;

	source.x = corners[SOUTH_WEST].x + checkOffset;
	source.y = corners[SOUTH_WEST].y - checkOffset;
	source.z = corners[SOUTH_WEST].z;
	destination.x = corners[SOUTH_EAST].x - checkOffset;
	destination.y = corners[SOUTH_EAST].y - checkOffset;
	destination.z = corners[SOUTH_EAST].z;

	isStairsResult = IsStairs (source, destination, isStairsResult);

	if (isStairsResult == FALSE)
		return false;

	source.x = corners[NORTH_WEST].x + checkOffset;
	source.y = corners[NORTH_WEST].y + checkOffset;
	source.z = corners[NORTH_WEST].z;
	destination.x = corners[SOUTH_WEST].x + checkOffset;
	destination.y = corners[SOUTH_WEST].y - checkOffset;
	destination.z = corners[SOUTH_WEST].z;

	isStairsResult = IsStairs (source, destination, isStairsResult);

	if (isStairsResult == FALSE)
		return false;

	source.x = corners[NORTH_EAST].x - checkOffset;
	source.y = corners[NORTH_EAST].y + checkOffset;
	source.z = corners[NORTH_EAST].z;
	destination.x = corners[SOUTH_EAST].x - checkOffset;
	destination.y = corners[SOUTH_EAST].y - checkOffset;
	destination.z = corners[SOUTH_EAST].z;

	isStairsResult = IsStairs (source, destination, isStairsResult);

	if (isStairsResult == FALSE)
		return false;

	Math::Vector3D directions[NUM_DIRECTIONS];

	GetDirections (directions);

	source.x = directions[NORTH].x;
	source.y = directions[NORTH].y + checkOffset;
	source.z = directions[NORTH].z;
	destination.x = directions[SOUTH].x;
	destination.y = directions[SOUTH].y - checkOffset;
	destination.z = directions[SOUTH].z;

	isStairsResult = IsStairs (source, destination, isStairsResult);

	if (isStairsResult == FALSE)
		return false;

	source.x = directions[EAST].x - checkOffset;
	source.y = directions[EAST].y;
	source.z = directions[EAST].z;
	destination.x = directions[WEST].x + checkOffset;
	destination.y = directions[WEST].y;
	destination.z = directions[WEST].z;

	isStairsResult = IsStairs (source, destination, isStairsResult);

	if (isStairsResult != TRUE)
		return false;

	AddAttributes (NAV_MESH_STAIRS);

	return true;
/*! BY EDWARD, WORKS PERFECT!!! :P
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	Math::Vector3D from, to;
	unsigned short stairNumber;
	const unsigned char minimumStairNumber (2u);
	const float minimumStairHeight (Math::tanf (Math::acosf (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())) * 5.0f);
	unsigned char tryIndex;
	unsigned char maximumTryNumber (5u);
	Math::Vector3D directions[NUM_DIRECTIONS];
	Math::Vector2D directionVector;

	GetDirections (directions);

	// Scan directions....
	for (unsigned char direction (NORTH), oppositeDirection; direction < NUM_DIRECTIONS; ++direction)
	{
		oppositeDirection = static_cast <unsigned char> (GetOppositeDirection (static_cast <NavDirType_t> (direction)));

		if (directions[oppositeDirection].z - directions[direction].z < minimumStairHeight * minimumStairNumber)
			continue;

		for
		(
			// Reset counts.
			tryIndex = 0u,
			stairNumber = 0u,

			// Initialize positions....
			from = directions[direction],
			to.x = directions[oppositeDirection].x,
			to.y = directions[oppositeDirection].y,
			to.z = from.z;

			tryIndex < maximumTryNumber;

			++tryIndex,
			from = traceResult.endPosition,
			to.x = directions[oppositeDirection].x,
			to.y = directions[oppositeDirection].y,
			to.z = from.z
		)
		{
			// Trace horizontal line....
			TraceLineThruWalkableEntities
			(
				from,
				to,

				HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult
			);

			if (traceResult.isStartSolid)
				break;

			// Reach destination?
			if (traceResult.fraction == 1.0f)
			{
				if (stairNumber >= minimumStairNumber)
				{
					Label_AddFlagAndReturnTrue:
					{
						AddAttributes (NAV_MESH_STAIRS);

						return true;
					}
				}

				return false;
			}

			directionVector = (to.AsVector2D () - from.AsVector2D ()).Normalize ();
			from = to = traceResult.endPosition + directionVector;

			from.z += HalfLifeEngine::SDK::Constants::StepHeight;

			// Trace vertical line....
			TraceLineThruWalkableEntities
			(
				from,
				to,

				HalfLifeEngine::SDK::Constants::TraceIgnore_All, traceResult
			);

			if (traceResult.isStartSolid)
				break;

			// Reliability checks.
			InternalAssert (traceResult.fraction < 1.0f);
			InternalAssert (to.z != traceResult.endPosition.z);

			/// @todo REMIND THIS CHECK!!!
			if (traceResult.planeNormal.z != 1.0f)
				continue;

			const float deltaZ (Math::fabsf (traceResult.endPosition.z - to.z));

			if (deltaZ < minimumStairHeight)
				continue;

			if (++stairNumber >= minimumStairNumber)
				goto Label_AddFlagAndReturnTrue;
		}
	}

	return false;*/
}

void NavigationMesh::NavigationArea::SomeFunction (NavigationArea *const/* area1*/, NavAreaConnectArray &/*connectedAreas2*/, const NavDirType_t/* direction*/)
{
/*	Math::Vector2D center;
	float halfWidth;
	Math::Vector3D position1, position2;

	for (NavAreaConnectArray::IndexType_t index (0u); index < connectedAreas2.GetElementNumber (); ++index)
	{
		NavigationArea *const area (connectedAreas2[index]);

		if (area->HasAttributes (NAV_MESH_JUMP))
			continue;

		area1->ComputePortal (area, direction, center, &halfWidth);

		if (halfWidth <= 0.0f)
			continue;

		position1 = area1->GetClosestPointOnArea (center);
		position2 = area->GetClosestPointOnArea (center);

		if
		(
			(!area1->HasAttributes (NAV_MESH_NO_HOSTAGES) || position1.z + 18.0f >= position2.z) &&
			(position1.x - position2.x) * (position1.x - position2.x) + (position1.y - position2.y) * (position1.y - position2.y) < MATH_GET_SQUARED (75.0f)
		)
			area1->ConnectTo (area, direction);
	}*/
}
void NavigationMesh::NavigationArea::JumpConnector__TryToConnect (NavAreaConnectArray &/*connectedAreas1*/, NavAreaConnectArray &/*connectedAreas2*/, const NavDirType_t/* oppositeDirection*/)
{
/*	for (NavAreaConnectArray::IndexType_t index (0u); index < connectedAreas1.GetElementNumber (); ++index)
	{
		NavigationArea *const area (connectedAreas1[index]);

		if (!area->IsConnected (this, oppositeDirection))
			continue;

		if (area->HasAttributes (NAV_MESH_JUMP))
		{
			const NavDirType_t direction (GetOppositeDirection (oppositeDirection));

			JumpConnector__TryToConnect (area->m_??????????????[direction], connectedAreas2, oppositeDirection);
			JumpConnector__TryToConnect (area->m_connectedAreas[direction], connectedAreas2, oppositeDirection);
		}
		else
			SomeFunction (area, connectedAreas2, oppositeDirection);
	}*/
}
void NavigationMesh::NavigationArea::Stich (void)
{
	if (!HasAttributes (NAV_MESH_JUMP))
		return;
/*
	for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
	{
		const NavDirType_t oppositeDirection (GetOppositeDirection (static_cast <NavDirType_t> (direction)));

		JumpConnector__TryToConnect (m_??????????????[direction], m_connectedAreas[oppositeDirection], oppositeDirection);
		JumpConnector__TryToConnect (m_connectedAreas[direction], m_connectedAreas[oppositeDirection], oppositeDirection);
	}*/
}
/*
//--------------------------------------------------------------------------------------------------------------
static void CommandNavCheckFloor (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	if (g_server->GetNavigationMeshManager ().GetMarkedArea () != NULL)
	{
		NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetMarkedArea ());

		area->CheckFloor (NULL);

		if (area->IsBlocked () && Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
			AddLogEntry (true, LogLevel_Default, false, "Area #%u %s is blocked.", area->GetID (), (area->GetCenter () + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)).ToString ());
	}
	else
	{
		const float start (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());

		for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			area->CheckFloor (NULL);

			if (area->IsBlocked () && Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
				AddLogEntry (true, LogLevel_Default, false, "Area #%u %s is blocked.", area->GetID (), (area->GetCenter () + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight)).ToString ());
		}

		if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
		{
			const float end (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
			const float time ((end - start) * 1000.0f);

			AddLogEntry (true, LogLevel_Default, false, "nav_check_floor took %2.2f ms.", time);
		}
	}
}
static ConCommand nav_check_floor ("nav_check_floor", CommandNavCheckFloor, "Updates the blocked/unblocked status for every navigation area.", FCVAR_GAMEDLL);
*/
#if 0
//--------------------------------------------------------------------------------------------------------------
namespace NEW
{

}
#endif	// if 0