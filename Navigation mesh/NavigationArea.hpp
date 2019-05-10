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

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_AREA_INCLUDED
#define NAVIGATION_AREA_INCLUDED

typedef DynamicArray <NavigationArea *, unsigned char> NavAreaConnectArray;
typedef DynamicArray <NavigationLadder *, unsigned char> NavLadderConnectArray;
typedef DynamicArray <NavigationArea *> NavAreaArray;
#if 0
class NavigationRoute
{
	//
	// Group: Private members.
	//
	private:
		DynamicArray <BaseNavigationArea *> m_route;
		float                               m_distance;

	public:
		inline NavigationRoute (void) : m_distance (-1.0f) { /*VOID*/ }
		template <typename CostFunctor> inline NavigationRoute (const BaseNavigationArea *const from, const BaseNavigationArea *const to, const CostFunctor &costFunc, NavigationArea **const closestArea = NULL)
		{
			Build (from, to, costFunc, closestArea);
		}

	public:
		template <typename CostFunctor> const bool Build (const BaseNavigationArea *const from, const BaseNavigationArea *const to, const CostFunctor &costFunc, NavigationArea **const closestArea = NULL);
};
#endif	// if 0
class NavigationArea : public BaseNavigationArea
{
	friend NavigationMesh;
	friend NavigationLadder;

	//
	// Group: Type definitions.
	//
	public:
		#include <Navigation mesh/Spot.hpp>
		#include <Navigation mesh/HidingSpot.hpp>	// A HidingSpot is a good place for a bot to crouch and wait for enemies.
		#include <Navigation mesh/SpotEncounter.hpp>	// This structure stores possible path segments thru a NavigationArea, and the dangerous spots to look at as we traverse that path segment.
		#include <Navigation mesh/AreaApproachInfo.hpp>

	//
	// Group: Private members.
	//
	private:
		//- hiding spots ------------------------------------------------------------------------------------
		HidingSpotArray          m_hidingSpots;

		//- encounter spots ---------------------------------------------------------------------------------
		SpotEncounterArray       m_spotEncounters;																///< list of possible ways to move thru this area, and the spots to look at as we do

		//- approach areas ----------------------------------------------------------------------------------
		ApproachArray            m_approach;

		//- A* pathfinding algorithm ------------------------------------------------------------------------
		static unsigned int      m_masterMarker;
		unsigned int             m_marker;																		///< used to flag the area as visited
		NavigationArea          *m_parent;																		///< the area just prior to this on in the search path
		NavTraverseType_t        m_parentHow;																	///< how we get from parent to us
		float                    m_totalCost;																	///< the distance so far plus an estimate of the distance left
		float                    m_costSoFar;																	///< distance travelled so far

		static NavigationArea   *m_openList;
		NavigationArea          *m_nextOpen, *m_prevOpen;														///< only valid if m_openMarker == m_masterMarker
		unsigned int             m_openMarker;																	///< if this equals the current marker value, we are on the open list

		//- connections to adjacent areas -------------------------------------------------------------------
#if 0
		AreaConnectionArray      m_areaConnections[NUM_DIRECTIONS];												///< a list of adjacent areas and portals for each direction
		LadderConnectionArray    m_ladderConnections[NavigationLadder::Direction_Total];						///< list of ladders and portals leading up and down from this area
		inline void BuildPortals (void)
		{
			AreaConnection_t areaConnection;

			for (unsigned char directionIndex (NORTH); directionIndex < NUM_DIRECTIONS; ++directionIndex)
			{
				const NavDirType_t direction (static_cast <NavDirType_t> (directionIndex));
				const NavAreaConnectArray::IndexType_t count (GetAdjacentCount (direction));

				m_areaConnections[directionIndex].RemoveAll ();

				for (NavAreaConnectArray::IndexType_t index (0u); index < count; ++index)
				{
					areaConnection.to = GetAdjacentArea (direction, index);

					ComputePortal (areaConnection.to, direction, areaConnection.position, &areaConnection.halfWidth);	// Get the hook position

					areaConnection.position.z = Math::GetMidPoint (GetZ (areaConnection.position), areaConnection.to->GetZ (areaConnection.position));

					areaConnection.distance = GetCenter ().GetDistance (areaConnection.to->GetCenter ());

					m_areaConnections[direction] += areaConnection;
				}
			}
		}
#endif	// if 0
		NavAreaConnectArray      m_connectedAreas[NUM_DIRECTIONS];												///< a list of adjacent areas for each direction
		NavLadderConnectArray    m_connectedLadders[NavigationLadder::Direction_Total];							///< list of ladders leading up and down from this area

		//---------------------------------------------------------------------------------------------------
		NavigationNode          *m_node[NUM_CORNERS];															///< navigation nodes at each corner of the area

		NavAreaArray             m_overlapAreas;																///< list of areas that overlap this area

//		NavigationArea          *m_prevHash, *m_nextHash;														///< for hash table in 'NavigationMesh'

	//
	// Group: (Con/De)structors.
	//
	public:
		inline NavigationArea (void);	///< Constructor used during normal runtime. (Note: This function declared in Server.hpp)
		NavigationArea (NavigationNode *const nwNode, NavigationNode *const neNode, NavigationNode *const seNode, NavigationNode *const swNode);
		NavigationArea (const Math::Vector3D &corner, const Math::Vector3D &otherCorner);
		NavigationArea (const Math::Vector3D &nwCorner, const Math::Vector3D &neCorner, const Math::Vector3D &seCorner, const Math::Vector3D &swCorner);

		virtual inline ~NavigationArea (void)
		{
			// Destructor

			m_hidingSpots.DeleteAll ();
			m_spotEncounters.DeleteAll ();
		}

	//
	// Group: Private functions.
	//
	private:
		void Initialize (void);									///< to keep constructors consistent

		//- hiding spots ------------------------------------------------------------------------------------
		inline const bool IsHidingSpotCollision (const Math::Vector3D &pos) const		///< Returns true if an existing hiding spot is too close to given position.
		{
			for (HidingSpotArray::IndexType_t index (0u); index < m_hidingSpots.GetElementNumber (); ++index)
				if (m_hidingSpots[index]->GetPosition ().GetDistanceSquared (pos) < MATH_GET_SQUARED (30.0f))
					return true;

			return false;
		}

		//- encounter spots ---------------------------------------------------------------------------------
		void AddSpotEncounters (NavigationArea *const from, NavDirType_t fromDir, NavigationArea *const to, const NavDirType_t toDir);	///< add spot encounter data when moving from area to area
		inline void DestroySpotEncounters (void)
		{
			m_spotEncounters.DeleteAndPurgeAll ();	// this calls delete on each element
		}

		//- Earliest occupy times ----------------------------------------------------------------------------------
		void ComputeEarliestOccupyTimes (void);

		inline void Strip (void)											///< Remove "analyzed" data from navigation area
		{
			m_approach.RemoveAll ();

			DestroyHidingSpots ();
			DestroySpotEncounters ();	// this calls delete on each element
		}

		//---------------------------------------------------------------------------------------------------
		inline void ResetNodes (void)									///< This is invoked at the start of an incremental navigation generation on pre-existing areas.
		{
			for (unsigned char index (NORTH_WEST); index < NUM_CORNERS; ++index)
				m_node[index] = NULL;
		}
		inline const bool HasNodes (void) const
		{
			for (unsigned char index (NORTH_WEST); index < NUM_CORNERS; ++index)
				if (m_node[index] != NULL)
					return true;

			return false;
		}

		inline const bool IsAbleToMerge (void) const { return !HasAttributes (NAV_MESH_UNMERGEABLE) && HasNodes (); }
		inline const bool IsAbleToMergeWith (const NavigationArea *const area) const { return IsAbleToMerge () && area->IsAbleToMerge (); }
		void FinishMerge (NavigationArea *const adjArea);						///< recompute internal data once nodes have been adjusted during merge
		void MergeAdjacentConnections (NavigationArea *const adjArea);			///< for merging with "adjArea" - pick up all of "adjArea"s connections
		void AssignNodes (NavigationArea *const area);							///< assign internal nodes to the given area

		void FinishSplitEdit (NavigationArea *const newArea, const NavDirType_t ignoreEdge) const;	///< given the portion of the original area, update its internal data
		const Math::Vector3D FindPositionInArea (const NavCornerType_t corner) const;

		void OnDestroyNotify (NavigationArea *const dead);						///< invoked when given area is going away
		inline void OnDestroyNotify (NavigationLadder *const dead) { Disconnect (dead); }					///< This is invoked when a ladder is going away. Remove any references we have to it.

		inline void SaveConnectedAreas (BaseFile &file) const	///< store connected areas to a file
		{
			for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
			{
				const NavAreaConnectArray::IndexType_t count (m_connectedAreas[direction].GetElementNumber ());

				// save number of connections for this direction
				file.Write (count);

				for (NavAreaConnectArray::IndexType_t index (0u); index < count; ++index)
					file.Write (m_connectedAreas[direction][index]->m_id);
			}
		}
		inline void SaveHidingSpots (BaseFile &file) const	///< store hiding spots to a file
		{
			const HidingSpotArray::IndexType_t count (m_hidingSpots.GetElementNumber ());

			file.Write (count);

			// store HidingSpot objects
			for (HidingSpotArray::IndexType_t index (0u); index < count; ++index)
				m_hidingSpots[index]->Save (file);
		}
		inline void SaveApproachAreas (BaseFile &file) const	///< store approach areas to a file
		{
			const ApproachArray::IndexType_t count (m_approach.GetElementNumber ());

			// save number of approach areas
			file.Write (count);

			// save approach area info
			for (ApproachArray::IndexType_t approachIndex (0u); approachIndex < count; ++approachIndex)
				m_approach[approachIndex].Save (file);
		}
		inline void SaveSpotEncounters (BaseFile &file) const	///< store spot encounters to a file
		{
			const SpotEncounterArray::IndexType_t count (m_spotEncounters.GetElementNumber ());

			// save number of encounter paths for this area
			file.Write (count);

			for (SpotEncounterArray::IndexType_t index (0u); index < count; ++index)
				m_spotEncounters[index]->Save (file);
		}
		inline void SavePlaceData (BaseFile &file) const;	///< save place data to a file
		inline void SaveConnectedLadders (BaseFile &file) const	///< save connected ladders to a file
		{
			for (unsigned char direction (NavigationLadder::Direction_Up); direction < NavigationLadder::Direction_Total; ++direction)
			{
				// save number of encounter paths for this area
				const NavLadderConnectArray::IndexType_t count (m_connectedLadders[direction].GetElementNumber ());

				file.Write (count);

				for (NavLadderConnectArray::IndexType_t index (0u); index < count; ++index)
					file.Write (m_connectedLadders[direction][index]->m_id);
			}
		}
		inline void SaveEarliestOccupyTimes (BaseFile &file) const	///< save earliest occupy times to a file
		{
			for (unsigned char teamIndex (TeamArrayID_Terrorist); teamIndex < TeamArrayID_Total; ++teamIndex)
			{
				// no spot in the map should take longer than this to reach
				file.Write (m_earliestOccupyTime[teamIndex]);
			}
		}

		inline const bool LoadConnectedAreas (const BaseFile &file)	///< load connected areas from a file
		{
			NavAreaConnectArray::IndexType_t count;
			NavAreaArray::IndexType_t id;

			for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
			{
				// load number of connections for this direction
				if (!file.Read (count))
					return false;

				for (NavAreaConnectArray::IndexType_t index (0u); index < count; ++index)
				{
					if (!file.Read (id))
						return false;

					m_connectedAreas[direction] += reinterpret_cast <NavigationArea *> (id);
				}
			}

			return true;
		}
		inline const bool LoadHidingSpots (const BaseFile &file)	///< load hiding spots from a file
		{
			HidingSpotArray::IndexType_t hidingSpotCount;

			// load number of hiding spots
			if (!file.Read (hidingSpotCount))
				return false;

			// load HidingSpot objects for this area
			for (HidingSpotArray::IndexType_t h (0u); h < hidingSpotCount; ++h)
			{
				// create new hiding spot and put on master list
				HidingSpot *const spot (new HidingSpot ());

				#if defined _DEBUG
					// Reliability check.
					if (spot == NULL)
					{
						AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating hiding spot!", sizeof (*spot));

						return false;
					}
				#endif	// if defined _DEBUG

				if (!spot->Load (file))
				{
					delete spot;

					return false;
				}

				m_hidingSpots += spot;
			}

			return true;
		}
		inline const bool LoadApproachAreas (const BaseFile &file)	///< load approach areas from a file
		{
			ApproachArray::IndexType_t count;

			// Load number of approach areas
			if (!file.Read (count))
				return false;

			ApproachInfo_t approach;

			// load approach area info (IDs)
			for (ApproachArray::IndexType_t approachIndex (0u); approachIndex < count; ++approachIndex)
			{
				if (!approach.Load (file))
					return false;

				m_approach += approach;
			}

			return true;
		}
		inline const bool LoadSpotEncounters (const BaseFile &file)	///< load spot encounters from a file
		{
			SpotEncounterArray::IndexType_t count;

			if (!file.Read (count))
				return false;

			for (SpotEncounterArray::IndexType_t e (0u); e < count; ++e)
			{
				SpotEncounter *const encounter (new SpotEncounter ());

				#if defined _DEBUG
					// Reliability check.
					if (encounter == NULL)
					{
						AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating spot encounter!", sizeof (*encounter));

						return false;
					}
				#endif	// if defined _DEBUG

				if (!encounter->Load (file))
				{
					delete encounter;

					return false;
				}

				m_spotEncounters += encounter;
			}

			return true;
		}
		inline const bool LoadPlaceData (const BaseFile &file);	///< load place data from a file
		inline const bool LoadConnectedLadders (const BaseFile &file)	///< load connected ladders from a file
		{
			NavLadderConnectArray::IndexType_t count;
			NavLadderArray::IndexType_t id;

			for (unsigned char direction (NavigationLadder::Direction_Up); direction < NavigationLadder::Direction_Total; ++direction)
			{
				if (!file.Read (count))
					return false;

				for (NavLadderConnectArray::IndexType_t ladderIndex (0u); ladderIndex < count; ++ladderIndex)
				{
					if (!file.Read (id))
						return false;

					// Reliability check.
					InternalAssert (!m_connectedLadders[direction].IsMember (reinterpret_cast <NavigationLadder *> (id)));

					m_connectedLadders[direction] += reinterpret_cast <NavigationLadder *> (id);
				}
			}

			return true;
		}
		inline const bool LoadEarliestOccupyTimes (const BaseFile &file)	///< load earliest occupy times from a file
		{
			for (unsigned char teamIndex (TeamArrayID_Terrorist); teamIndex < TeamArrayID_Total; ++teamIndex)
			{
				// no spot in the map should take longer than this to reach
				if (!file.Read (m_earliestOccupyTime[teamIndex]))
					return false;
			}

			return true;
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool IsNavigationLadder (void) const { return false; }

		void ConnectTo  (NavigationArea *const area, const NavDirType_t direction);		///< connect this area to given area in given direction
		void Disconnect (NavigationArea *const area);							///< disconnect this area from given area

		//- ladders -----------------------------------------------------------------------------------------
		void ConnectTo  (NavigationLadder *const ladder);						///< connect this area to given ladder
		void ConnectTo  (NavigationLadder *const ladder, const NavigationLadder::Direction_t direction);	///< connect this area to given ladder in given direction
		void Disconnect (NavigationLadder *const ladder);						///< disconnect this area from given ladder

		void                 Save     (BaseFile &file) const;
		const bool           Load     (const BaseFile &file);
		const NavErrorType_t PostLoad (void);

		const bool MakeSniperSpots (void);

		void BuildOverlapAreas (void);

		void SplitX (void);	///< Recursively chop area in half along X until child areas are roughly square.
		void SplitY (void);	///< Recursively chop area in half along Y until child areas are roughly square.

		void UpdateBlocked (void);									///< Updates the (un)blocked status of the navigation area
		void CheckFloor (const HalfLifeEngine::SDK::Classes::Edict *const ignore);						///< Checks if there is a floor under the navigation area, in case a breakable floor is gone
		void CheckWaterLevel (void);
		const bool TestJump (void);
		static const unsigned char IsStairs (const Math::Vector3D &source, const Math::Vector3D &destination, const unsigned char oldResult);
		const bool TestStairs (void);

		void SomeFunction (NavigationArea *const area1, NavAreaConnectArray &connectedAreas2, const NavDirType_t direction);
		void JumpConnector__TryToConnect (NavAreaConnectArray &connectedAreas1, NavAreaConnectArray &connectedAreas2, const NavDirType_t oppositeDirection);
		void Stich (void);

		const bool Contains (const Math::Vector3D &pos) const;					///< return true if given point is on or above this area, but no others

		inline const bool IsEdge (const NavDirType_t direction) const						///< Return true if there are no bi-directional links on the given side.
		{
			for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[direction].GetElementNumber (); ++index)
				if (m_connectedAreas[direction][index]->IsConnected (this, GetOppositeDirection (direction)))
					return false;

			return true;
		}

		const bool IsVisible (const Math::Vector3D &eye, Math::Vector3D *const visSpot = NULL) const;	///< return true if area is visible from the given eyepoint, return visible spot

		inline const NavAreaConnectArray::IndexType_t GetAdjacentCount (void) const	///< return total number of connected areas in all directions
		{
			NavAreaConnectArray::IndexType_t totalCount (0u);

			for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
				totalCount += GetAdjacentCount (static_cast <NavDirType_t> (direction));

			return totalCount;
		}
		inline const NavAreaConnectArray::IndexType_t GetAdjacentCount (const NavDirType_t direction) const { return m_connectedAreas[direction].GetElementNumber (); }	///< return number of connected areas in given direction
		inline NavigationArea *const GetAdjacentArea (const NavDirType_t direction, unsigned char i) const	/// return the i'th adjacent area in the given direction
		{
			for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[direction].GetElementNumber (); ++index)
				if (i-- == 0u)
					return m_connectedAreas[direction][index];

			return NULL;
		}
		inline NavigationArea *const GetRandomAdjacentArea (const NavDirType_t direction) const
		{
/*			const NavAreaConnectArray::IndexType_t which (m_connectedAreas[direction].GetRandomElementIndex ());
			NavAreaConnectArray::IndexType_t count (0u);

			for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[direction].GetElementNumber (); ++index)
				if (count++ == which)
					return m_connectedAreas[direction][index];

			return NULL;*/
			return m_connectedAreas[direction].IsEmpty () ? NULL : m_connectedAreas[direction].GetRandomElement ();
		}

		inline const NavAreaConnectArray &GetAdjacentList (const NavDirType_t direction) const { return m_connectedAreas[direction]; }
		const bool IsConnected (const NavigationArea *const area) const;	///< return true if given area is connected with this area
		const bool IsConnected (const NavigationArea *const area, const NavDirType_t direction) const;	///< return true if given area is connected in given direction
		inline const bool IsConnected (const NavigationLadder *const ladder, NavigationLadder::Direction_t direction) const	///< Return true if given ladder is connected in given direction. @TODO Formalize "asymmetric" flag on connections
		{
			// Reliability check.
			InternalAssert (direction < NavigationLadder::Direction_Total);

			for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[direction].GetElementNumber (); ++index)
				if (ladder == m_connectedLadders[direction][index])
					return true;

			return false;
		}
		inline const float ComputeHeightChange (const NavigationArea *const area) const			///< Compute change in height from this area to given area.
		{
			const float ourZ (GetZ (GetCenter ()));
			const float areaZ (area->GetZ (area->GetCenter ()));

			return areaZ - ourZ;
		}
		inline const float ComputeGroundHeightChange (const NavigationArea *const area) const			///< Compute ground change in height from this area to given area.
		{
			Math::Vector3D ourGround (GetCenter ());
			float ourHeight;
			const float appendOffsetZ (18.0f);

			ourGround.z = GetZ (ourGround);

			ourGround.z += appendOffsetZ;

			if (!GetSimpleGroundHeight (ourGround, &ourHeight))
				return 0.0f;

			Math::Vector3D areaGround (area->GetCenter ());
			float areaHeight;

			areaGround.z = area->GetZ (areaGround);

			areaGround.z += appendOffsetZ;

			if (!GetSimpleGroundHeight (areaGround, &areaHeight))
				return 0.0f;

			return ourHeight - areaHeight;
		}

		const NavLadderConnectArray &GetConnectedLadders (const NavigationLadder::Direction_t direction) const { return m_connectedLadders[direction]; }

		void ComputePortal (const NavigationArea *const to, const NavDirType_t direction, Math::Vector2D &center, float *const halfWidth = NULL) const;	///< compute portal to adjacent area
		void ComputeLargestPortal (const NavigationArea *const to, const NavDirType_t direction, Math::Vector2D &center, float *const halfWidth = NULL) const;	///< compute largest portal to adjacent area
		const Math::Vector2D GetClosestPointInPortal2D (const NavigationArea *const to, const NavDirType_t direction, const Math::Vector2D &fromPos) const;	///< compute closest point within the "portal" between to adjacent areas
		inline const Math::Vector3D GetClosestPointInPortal3D (const NavigationArea *const to, const NavDirType_t direction, const Math::Vector2D &fromPos) const	///< compute closest point within the "portal" between to adjacent areas
		{
			Math::Vector3D point (GetClosestPointInPortal2D (to, direction, fromPos));

			point.z = GetZ (point);

			return point;
		}
		const NavDirType_t ComputeDirection (const Math::Vector3D &point) const;			///< return direction from this area to the given point

		//- hiding spots ------------------------------------------------------------------------------------
		inline const HidingSpotArray &GetHidingSpots (void) const { return m_hidingSpots; }
		void ComputeHidingSpots (void);							///< analyze local area neighborhood to find "hiding spots" in this area - for map learning
		void ComputeSniperSpots (void);							///< analyze local area neighborhood to find "sniper spots" in this area - for map learning

		inline void DestroyHidingSpots (void)
		{
			m_hidingSpots.DeleteAndPurgeAll ();	// this calls delete on each element
		}

		const SpotEncounter *const GetSpotEncounter (const NavigationArea *const from, const NavigationArea *const to) const;	///< given the areas we are moving between, return the spots we will encounter
		void ComputeSpotEncounters (void);							///< compute spot encounter data - for map learning
		inline const SpotEncounterArray::IndexType_t GetSpotEncounterCount (void) const { return m_spotEncounters.GetElementNumber (); }

		//- approach areas ----------------------------------------------------------------------------------
		inline const ApproachInfo_t &GetApproachInfo (const ApproachArray::IndexType_t index) const { return m_approach[index]; }
		inline const ApproachArray::IndexType_t GetApproachInfoCount (void) const { return m_approach.GetElementNumber (); }
		void ComputeApproachAreas (void);							///< determine the set of "approach areas" - for map learning

		//- A* pathfinding algorithm ------------------------------------------------------------------------
		static inline void MakeNewMarker (void)
		{
			if (++m_masterMarker == 0u)
				m_masterMarker = 1u;
		}
		inline void Mark (void) { m_marker = m_masterMarker; }
		inline const bool IsMarked (void) const { return m_marker == m_masterMarker; }

		inline void SetParent (NavigationArea *const parent, const NavTraverseType_t how = NUM_TRAVERSE_TYPES) { m_parent = parent; m_parentHow = how; }
		inline NavigationArea *const GetParent (void) const { return m_parent; }
		inline const NavTraverseType_t GetParentHow (void) const { return m_parentHow; }

		inline const bool IsOpen (void) const { return m_openMarker == m_masterMarker; }									///< true if on "open list"
		void AddToOpenList (void);									///< add to open list in decreasing value order
		void UpdateOnOpenList (void);								///< a smaller value has been found, update this area on the open list
		void RemoveFromOpenList (void);
		static inline const bool IsOpenListEmpty (void) { return m_openList == NULL; }
		static inline NavigationArea *const PopOpenList (void)						///< remove and return the first element of the open list
		{
			if (m_openList != NULL)
			{
				NavigationArea *const area (m_openList);

				// disconnect from list
				area->RemoveFromOpenList ();

				return area;
			}

			return NULL;
		}

		inline const bool IsClosed (void) const { return IsMarked () && !IsOpen (); }								///< true if on "closed list"

		//--------------------------------------------------------------------------------------------------------------
		/**
		* Do a breadth-first search, invoking functor on each area.
		* If functor returns 'true', continue searching from this area.
		* If functor returns 'false', the area's adjacent areas are not explored (dead end).
		* If 'maxRange' is 0 or less, no range check is done (all areas will be examined).
		*
		* NOTE: Returns all areas that overlap range, even partially
		*
		* @todo Use ladder connections
		*/
		// helper function
		inline void AddToOpenList (NavigationArea *const parent, const Math::Vector3D &startPos, const float maxRange)
		{
			InternalAssert (this != NULL);

			if (IsMarked ())
				return;

			Mark ();
			SetTotalCost (0.0f);
			SetParent (parent);

			if (maxRange > 0.0f)
			{
				const Math::Vector3D closePos (GetClosestPointOnArea (startPos));

				// make sure this area overlaps range
				if (closePos.GetDistanceSquared2D (startPos) < MATH_GET_SQUARED (maxRange))
				{
					// compute approximate distance along path to limit travel range, too
					const float distAlong (parent->GetCostSoFar () + GetCenter ().GetDistance (parent->GetCenter ()));

					SetCostSoFar (distAlong);

					// allow for some fudge due to large size areas
					if (distAlong <= 1.5f * maxRange)
						AddToOpenList ();
				}
			}
			else
			{
				// infinite range
				AddToOpenList ();
			}
		}
		inline void AddToClosedList (void) { Mark (); }								///< add to the closed list
		void RemoveFromClosedList (void);

		static void ClearSearchLists (void);						///< clears the open and closed lists for a new search

		inline void SetTotalCost (const float value) { m_totalCost = value; }
		inline const float GetTotalCost (void) const { return m_totalCost; }

		inline void SetCostSoFar (const float value) { m_costSoFar = value; }
		inline const float GetCostSoFar (void) const { return m_costSoFar; }

		//- Draw navigation areas and edit them ------------------------------------------------------------------------
		void Draw               (void) const;	///< Draw area for debugging & editing.
		void DrawConnectedAreas (void) const;	///< Draws connected areas.
		void DrawHidingSpots    (void) const;	///< Show hiding spots for debugging.
		void DrawApproachPoints (void) const;	///< Show approach points for debugging.
		void DrawDanger         (void);			///< Show danger levels for debugging.

		const bool SplitEdit (const bool splitAlongX, const float splitEdge, NavigationArea **const outAlpha = NULL, NavigationArea **const outBeta = NULL);	///< split this area into two areas at the given edge
		const bool MergeEdit (NavigationArea *const adj);							///< merge this area and given adjacent area
		const bool SpliceEdit (NavigationArea *const other);							///< create a new area between this area and given area
		void RaiseCorner (const NavCornerType_t corner, const short amount, const bool raiseAdjacentCorners = true);	///< raise/lower a corner
		inline void RaiseCorners (const short amount, const bool raiseAdjacentCorners = true)	///< raise/lower all corners
		{
			RaiseCorner (NORTH_WEST, amount, raiseAdjacentCorners);
			RaiseCorner (NORTH_EAST, amount, raiseAdjacentCorners);
			RaiseCorner (SOUTH_WEST, amount, raiseAdjacentCorners);
			RaiseCorner (SOUTH_EAST, amount, raiseAdjacentCorners);
		}
		static const float FindGroundZFromPoint (const Math::Vector3D &end, const Math::Vector3D &start);	///< Walks from the start position to the end position in GenerationStepSize increments, checking the ground height along the way.
		static const float FindGroundZ (const Math::Vector3D &original, const Math::Vector3D &corner1, const Math::Vector3D &corner2);	///< Finds the Z value for a corner given two other corner points. This walks along the edges of the navigation area in GenerationStepSize increments, to increase accuracy.
		void PlaceOnGround (const NavCornerType_t corner, const float inset = 0.0f);	///< places a corner (or all corners if corner == NUM_CORNERS) on the ground
		const NavCornerType_t GetCornerUnderCursor (void) const;
		const NavCornerType_t GetCornerAndHotspotUnderCursor (Math::Vector3D hotspot[NUM_CORNERS]) const;
		const bool GetCornerHotspot (const NavCornerType_t corner, Math::Vector3D hotspot[NUM_CORNERS]) const;	///< returns true if the corner is under the cursor

		void ComputePathPortals3D (Connection::Portal3D_t *const pathPortals, unsigned short &pathPortalsNumber, const NavigationArea **const areas, unsigned int &areasNumber) const;
		void ComputePortal3D (const NavigationArea *const to, const NavDirType_t direction, Connection::Portal3D_t &portal) const;
};

class NavigationArea_Thinkable : public NavigationArea
{
	//
};

#endif	// ifndef NAVIGATION_AREA_INCLUDED