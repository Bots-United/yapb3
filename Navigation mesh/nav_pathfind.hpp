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
// NavigationMeshPathfind.hpp
//
// Description: Path-finding mechanisms using the Navigation Mesh.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_PATHFIND_INCLUDED
#define NAVIGATION_MESH_PATHFIND_INCLUDED

//-------------------------------------------------------------------------------------------------------------------
/**
* Used when building a path to determine the kind of path to build
*/
enum RouteType_t
{
	FASTEST_ROUTE,
	SAFEST_ROUTE
};

inline const float GetManhattanDistance (const Math::Vector2D &currentNodeOrigin, const Math::Vector2D &goalOrigin)
{
	return (Math::fabsf (currentNodeOrigin.x - goalOrigin.x) + Math::fabsf (currentNodeOrigin.y - goalOrigin.y));
}

//--------------------------------------------------------------------------------------------------------------
/*** Functor used with NavAreaBuildPath()*/
class ShortestPathCost
{
	public:
		inline const float operator () (const NavigationArea *const area, const NavigationArea *const fromArea, const NavigationLadder *const ladder) const
		{
			if (fromArea == NULL)
				return 0.0f;	// first area in path, no cost

			if (area->HasAttributes (NAV_MESH_DOUBLE_JUMP))	// if this is a "double jump" area, add nereal penalty
				return 999999.9f;

			// compute distance travelled along path so far
			const float dist (ladder != NULL ? ladder->GetLength () : area->GetCenter ().GetDistance (fromArea->GetCenter ()));
			float cost (dist + fromArea->GetCostSoFar ());

			// if this is a "crouch" area, add penalty
			if (area->HasAttributes (NAV_MESH_CROUCH))
			{
				const float crouchPenalty (20.0f);	// 10

				cost += crouchPenalty * dist;
			}

			// if this is a "jump" area, add penalty
			if (area->HasAttributes (NAV_MESH_JUMP))
			{
				const float jumpPenalty (5.0f);

				cost += jumpPenalty * dist;
			}

			return cost;
		}
};
class ShortestPathCostAlsoThruDoubleJumpAreas
{
	private:
		const YaPB *const m_bot;

	public:
		inline ShortestPathCostAlsoThruDoubleJumpAreas (const YaPB *const bot) : m_bot (bot) { /* VOID */ }

	public:
		inline const float operator () (const NavigationArea *const area, const NavigationArea *const fromArea, const NavigationLadder *const ladder) const;	// Note: This function declared in ClientManager.hpp.

	private:
		inline ShortestPathCostAlsoThruDoubleJumpAreas &operator = (const ShortestPathCostAlsoThruDoubleJumpAreas &/*right*/);	// Avoid "warning C4512: 'ShortestPathCostAlsoThruDoubleJumpAreas' : assignment operator could not be generated".
};

class PathCost
{
	private:
		const YaPB *const m_bot;
		const RouteType_t m_routeType;

	public:
		inline PathCost (const YaPB *const bot, const RouteType_t routeType) : m_bot (bot), m_routeType (routeType) { /* VOID */ }

	public:
		inline const float operator () (NavigationArea *const area, const NavigationArea *const fromArea, const NavigationLadder *const ladder) const;	// Note: This function declared in YaPB.hpp.

	private:
		inline PathCost &operator = (const PathCost &/*right*/);	// Avoid "warning C4512: 'PathCost' : assignment operator could not be generated".
};

//--------------------------------------------------------------------------------------------------------------
/*** Find path from 'startArea' to 'goalArea' via an A* search, using supplied cost heuristic.* If cost functor returns -1 for an area, that area is considered a dead end.* This doesn't actually build a path, but the path is defined by following parent* pointers back from 'goalArea' to 'startArea'.* If 'closestArea' is non-NULL, the closest area to the goal is returned (useful if the path fails).* If 'goalArea' is NULL, will compute a path as close as possible to 'goalPos'.* If 'goalPos' is NULL, will use the center of 'goalArea' as the goal position.* Returns true if a path exists.*/
template <typename CostFunctor> static const bool NavAreaBuildPath (NavigationArea *const startArea, NavigationArea *goalArea, const Math::Vector3D *const goalPos, const CostFunctor &costFunc, NavigationArea **const closestArea = NULL)
{
	// Reliability check.
//	InternalAssert (startArea != NULL);
	if (startArea == NULL)
		return false;

	if (goalArea != NULL && goalArea->IsBlocked ())
		goalArea = NULL;

	if (goalArea == NULL && goalPos == NULL)
		return false;

	startArea->SetParent (NULL);

	// if we are already in the goal area, build trivial path
	if (startArea == goalArea)
	{
		// keep track of the area that is closest to the goal
		if (closestArea != NULL)
			*closestArea = startArea;

		return true;
	}

	// determine actual goal position
	const Math::Vector3D &actualGoalPos (goalPos != NULL ? *goalPos : goalArea->GetCenter ());

	// start search
	NavigationArea::ClearSearchLists ();

	// compute estimate of path length
	/// @todo Cost might work as "manhattan distance"
	startArea->SetTotalCost (startArea->GetCenter ().GetDistance (actualGoalPos));

	const float initCost (costFunc (startArea, NULL, NULL));

	if (initCost < 0.0f)
		return false;

	startArea->SetCostSoFar (initCost);
	startArea->AddToOpenList ();

	// keep track of the area we visit that is closest to the goal
	if (closestArea != NULL)
		*closestArea = startArea;

	float closestAreaDist (startArea->GetTotalCost ());

	// do A* search
	while (!NavigationArea::IsOpenListEmpty ())
	{
		// get next area to check
		NavigationArea *const area (NavigationArea::PopOpenList ());

		// don't consider blocked areas
		if (area->IsBlocked ())
			continue;

		// check if we have found the goal area or position
		if (area == goalArea || (goalArea == NULL && goalPos != NULL && area->Contains (*goalPos)))
		{
			if (closestArea != NULL)
				*closestArea = area;

			return true;
		}

		// search adjacent areas
		bool searchFloor (true);
		unsigned char direction (NORTH);
		const NavAreaConnectArray *floorList (&area->GetAdjacentList (NORTH));
		NavAreaConnectArray::IndexType_t floorIter (0u);
		bool ladderUp (true);
		const NavLadderConnectArray *ladderList (NULL);
		NavLadderConnectArray::IndexType_t ladderIter (NavLadderConnectArray::BadPosition);
		DECLARE_TYPED_ENUMERATION (unsigned char, TopDirection_t)
		{
			AHEAD,
			LEFT,
			RIGHT,
			BEHIND,
			NUM_TOP_DIRECTIONS
		};
		TopDirection_t ladderTopDir (AHEAD);

		for (/* Empty */; /* Empty */; /* Empty */)
		{
			NavigationArea *newArea;
			NavTraverseType_t how;
			const NavigationLadder *ladder (NULL);

			//
			// Get next adjacent area - either on floor or via ladder
			//
			if (searchFloor)
			{
				// if exhausted adjacent connections in current direction, begin checking next direction
				if (floorIter >= floorList->GetElementNumber ())
				{
					++direction;

					if (direction == NUM_DIRECTIONS)
					{
						// checked all directions on floor - check ladders next
						searchFloor = false;

						ladderList = &area->GetConnectedLadders (NavigationLadder::Direction_Up);
						ladderIter = 0u;
						ladderTopDir = AHEAD;
					}
					else
					{
						// start next direction
						floorList = &area->GetAdjacentList (static_cast <NavDirType_t> (direction));
						floorIter = 0u;
					}

					continue;
				}

				newArea = (*floorList)[floorIter];
				how = static_cast <NavTraverseType_t> (direction);
				++floorIter;
			}
			else	// search ladders
			{
				if (ladderIter >= ladderList->GetElementNumber ())
				{
					if (!ladderUp)
						break;	// checked both ladder directions - done

					// check down ladders
					ladderUp = false;
					ladderList = &area->GetConnectedLadders (NavigationLadder::Direction_Down);
					ladderIter = 0u;

					continue;
				}

				if (ladderUp)
				{
					ladder = (*ladderList)[ladderIter];

					// do not use BEHIND connection, as its very hard to get to when going up a ladder
					if (ladderTopDir == AHEAD)
						newArea = ladder->GetConnectedArea (NavigationLadder::ConnectionType_TopBehind);
					else if (ladderTopDir == LEFT)
						newArea = ladder->GetConnectedArea (NavigationLadder::ConnectionType_TopLeft);
					else if (ladderTopDir == RIGHT)
						newArea = ladder->GetConnectedArea (NavigationLadder::ConnectionType_TopRight);
					else
					{
						++ladderIter;
						ladderTopDir = AHEAD;

						continue;
					}

					how = GO_LADDER_UP;
					++ladderTopDir;
				}
				else
				{
					newArea = (*ladderList)[ladderIter]->GetConnectedArea (NavigationLadder::ConnectionType_Bottom);
					how = GO_LADDER_DOWN;
					ladder = (*ladderList)[ladderIter];
					++ladderIter;
				}

				if (newArea == NULL)
					continue;
			}

			// don't backtrack
			if (newArea == area)
				continue;

			// don't consider blocked areas
			if (newArea->IsBlocked ())
				continue;

			const float newCostSoFar (costFunc (newArea, area, ladder));

			// check if cost functor says this area is a dead-end
			if (newCostSoFar < 0.0f)
				continue;

			if ((newArea->IsOpen () || newArea->IsClosed ()) && newArea->GetCostSoFar () <= newCostSoFar)
				continue;	// this is a worse path - skip it

			// compute estimate of distance left to go
			const float newCostRemaining (newArea->GetCenter ().GetDistance (actualGoalPos));

			// track closest area to goal in case path fails
			if (closestArea != NULL && newCostRemaining < closestAreaDist)
			{
				*closestArea = newArea;
				closestAreaDist = newCostRemaining;
			}

			newArea->SetParent (area, how);
			newArea->SetCostSoFar (newCostSoFar);
			newArea->SetTotalCost (newCostSoFar + newCostRemaining);

			if (newArea->IsClosed ())
				newArea->RemoveFromClosedList ();

			if (newArea->IsOpen ())
			{
				// area already on open list, update the list order to keep costs sorted
				newArea->UpdateOnOpenList ();
			}
			else
				newArea->AddToOpenList ();
		}

		// we have searched this area
		area->AddToClosedList ();
	}

	return false;
}


//--------------------------------------------------------------------------------------------------------------
/*** Compute distance between two areas. Return -1 if can't reach 'endArea' from 'startArea'.*/
template <typename CostFunctor> static const float NavAreaTravelDistance (NavigationArea *const startArea, NavigationArea *const endArea, const CostFunctor &costFunc)
{
	if (startArea == NULL || endArea == NULL)
		return -1.0f;

	if (startArea == endArea)
		return 0.0f;

	// compute path between areas using given cost heuristic
	if (NavAreaBuildPath (startArea, endArea, NULL, costFunc) == false)
		return -1.0f;

	float distance (0.0f);

	// compute distance along path
	for (const NavigationArea *area (endArea); area->GetParent (); area = area->GetParent ())
		distance += area->GetCenter ().GetDistance (area->GetParent ()->GetCenter ());

	return distance;
}


//--------------------------------------------------------------------------------------------------------------
/*** Compute travel distance along shortest path from 'startPos' to 'goalPos'.* Return -1 if can't reach 'endPos' from 'goalPos'.*/
template <typename CostFunctor> static const float NavAreaTravelDistance (const Math::Vector3D &startPos, const Math::Vector3D &goalPos, const CostFunctor &costFunc)
{
	NavigationArea *const startArea (g_server->GetNavigationMeshManager ().GetNearestNavArea (startPos));

	if (startArea == NULL)
		return -1.0f;

	NavigationArea *goalArea (NULL);

	// compute path between areas using given cost heuristic
	if (NavAreaBuildPath (startArea, NULL, &goalPos, costFunc, &goalArea) == false)
		return -1.0f;

	// compute distance along path
	if (goalArea->GetParent () == NULL)
	{
		// both points are in the same area - return euclidean distance
		return goalPos.GetDistance (startPos);
	}
	else
	{
		// goalPos is assumed to be inside goalArea (or very close to it) - skip to next area
		const NavigationArea *area (goalArea->GetParent ());
		float distance (goalPos.GetDistance (area->GetCenter ()));

		for (/* Empty */; area->GetParent () != NULL; area = area->GetParent ())
			distance += area->GetCenter ().GetDistance (area->GetParent ()->GetCenter ());

		// add in distance to startPos
		distance += startPos.GetDistance (area->GetCenter ());

		return distance;
	}
}

template <typename Functor> static void SearchSurroundingAreas (NavigationArea *const startArea, const Math::Vector3D &startPos, const Functor &func, const float maxRange = -1.0f)
{
	if (startArea == NULL)
		return;

	NavigationArea::MakeNewMarker ();
	NavigationArea::ClearSearchLists ();

	startArea->AddToOpenList ();
	startArea->SetTotalCost (0.0f);
	startArea->SetCostSoFar (0.0f);
	startArea->SetParent (NULL);
	startArea->Mark ();

	while (!NavigationArea::IsOpenListEmpty ())
	{
		// get next area to check
		NavigationArea *const area (NavigationArea::PopOpenList ());

		// don't use blocked areas
		if (area->IsBlocked ())
			continue;

		// invoke functor on area
		if (!func (area))
			continue;

		// explore adjacent floor areas
		for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
		{
			const NavAreaConnectArray::IndexType_t count (area->GetAdjacentCount (static_cast <NavDirType_t> (direction)));

			for (NavAreaConnectArray::IndexType_t i (0u); i < count; ++i)
				area->GetAdjacentArea (static_cast <NavDirType_t> (direction), i)->AddToOpenList (area, startPos, maxRange);
		}

		// explore adjacent areas connected by ladders

		// check up ladders
		{
			const NavLadderConnectArray &ladderList (area->GetConnectedLadders (NavigationLadder::Direction_Up));

			for (NavLadderConnectArray::IndexType_t index (0u); index < ladderList.GetElementNumber (); ++index)
			{
				const NavigationLadder *const ladder (ladderList[index]);

				// do not use BEHIND connection, as its very hard to get to when going up a ladder
				for (unsigned char direction (NavigationLadder::ConnectionType_TopBehind); direction < NavigationLadder::ConnectionType_TopForward; ++direction)
					ladder->GetConnectedArea (static_cast <NavigationLadder::ConnectionType_t> (direction))->AddToOpenList (area, startPos, maxRange);
			}
		}

		// check down ladders
		{
			const NavLadderConnectArray &ladderList (area->GetConnectedLadders (NavigationLadder::Direction_Down));

			for (NavLadderConnectArray::IndexType_t index (0u); index < ladderList.GetElementNumber (); ++index)
				ladderList[index]->GetConnectedArea (NavigationLadder::ConnectionType_Bottom)->AddToOpenList (area, startPos, maxRange);
		}
	}
}


//--------------------------------------------------------------------------------------------------------------
/*** Fuctor that returns lowest cost for farthest away areas* For use with FindMinimumCostArea()*/
class FarAwayFunctor
{
	public:
		inline const float operator () (const NavigationArea *const area, const NavigationArea *const fromArea, const NavigationLadder *const/* ladder*/) const
		{
			if (area == fromArea)
				return 9999999.9f;

			return 1.0f / fromArea->GetCenter ().GetDistance (area->GetCenter ());
		}
};

/*** Fuctor that returns lowest cost for areas farthest from given position* For use with FindMinimumCostArea()*/
class FarAwayFromPositionFunctor
{
	private:
		const Math::Vector3D &m_pos;

	public:
		inline FarAwayFromPositionFunctor (const Math::Vector3D &pos) : m_pos (pos) { /*VOID*/ }

	private:
		inline FarAwayFromPositionFunctor &operator = (const FarAwayFromPositionFunctor &/*right*/);	// Avoid "warning C4512: 'FarAwayFromPositionFunctor' : assignment operator could not be generated".

	public:
		inline const float operator () (const NavigationArea *const area, const NavigationArea *const/* fromArea*/, const NavigationLadder *const/* ladder*/) const
		{
			return 1.0f / m_pos.GetDistance (area->GetCenter ());
		}
};


/*** Pick a low-cost area of "decent" size*/
template <typename CostFunctor> static NavigationArea *const FindMinimumCostArea (const NavigationArea *const startArea, const CostFunctor &costFunc)
{
	const unsigned char minSize (150u);

	// collect N low-cost areas of a decent size
	enum { NUM_CHEAP_AREAS = 32u };
	struct
	{
		NavigationArea *area;
		float           cost;
	} cheapAreaSet[NUM_CHEAP_AREAS];
	unsigned char cheapAreaSetCount (0u);

	for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
	{
		NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);
		const Math::Extent3D &extent (area->GetExtent ());

		// skip the small areas
		if (extent.GetSizeX () < minSize || extent.GetSizeY () < minSize)
			continue;

		// compute cost of this area
		const float cost (costFunc (area, startArea, NULL));

		if (cheapAreaSetCount < NUM_CHEAP_AREAS)
		{
			cheapAreaSet[cheapAreaSetCount].area = area;
			cheapAreaSet[cheapAreaSetCount].cost = cost;

			++cheapAreaSetCount;
		}
		else
		{
			unsigned char expensive (0u);

			// replace most expensive cost if this is cheaper
			for (unsigned char i (1u); i < NUM_CHEAP_AREAS; ++i)
				if (cheapAreaSet[i].cost > cheapAreaSet[expensive].cost)
					expensive = i;

			if (cheapAreaSet[expensive].cost > cost)
			{
				cheapAreaSet[expensive].area = area;
				cheapAreaSet[expensive].cost = cost;
			}
		}
	}

	if (cheapAreaSetCount == 0u)
	{
		// degenerate case - no decent sized areas - pick a random area
/*		NavAreaArray::IndexType_t which (g_server->GetNavigationMeshManager ().GetAreas ().GetRandomElementIndex ());

		for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
			if (which-- == 0u)
				return g_server->GetNavigationMeshManager ().GetAreas ()[index];

		// MADE BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return NULL;*/
		return g_server->GetNavigationMeshManager ().GetAreas ().IsEmpty () ? NULL : g_server->GetNavigationMeshManager ().GetAreas ().GetRandomElement ();
	}

	// pick one of the areas at random
	return cheapAreaSet[g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, cheapAreaSetCount - 1u)].area;
}

#endif	// ifndef NAVIGATION_MESH_PATHFIND_INCLUDED