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
// Description: The Navigation Mesh interface.
//				The NavigationMesh is the global interface to the Navigation Mesh.
//				Data structures and constants for the Navigation Mesh system.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_INCLUDED
#define NAVIGATION_MESH_INCLUDED

#define NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "nav"

#define COMMAND_COMPLETION_MAXITEMS		64u
#define COMMAND_COMPLETION_ITEM_LENGTH	64u

//--------------------------------------------------------------------------------------------------------
class NavigationMesh
{
	public:
		class NavigationArea;
		class NavigationLadder;
		class NavigationNode;

		friend NavigationArea;
		friend NavigationLadder;
		friend NavigationNode;

	//
	// Group: Private constants.
	//
	private:
		static const unsigned short GridCellSize = 300u;	///< the width/height of a grid cell for spatially partitioning navigation areas for fast access
		static const unsigned char  MaximumNavigationNodesPerCell = 5u;
		static const unsigned short NodesGridCellSize;	///< the width/height of a grid cell for spatially partitioning navigation nodes for fast access

	//
	// Group: Constants.
	//
	public:
		/**
		* Below are several constants used by the navigation system.
		*/
		static const DynamicString  FileExtention;		///< Navigation mesh file extention.
		static const float          GenerationStepSize;	///< (30) was 20, but bots can't fit always fit.
		static const Math::Vector3D GenerationHull;
		static const float          BotRadius;			///< circular extent that contains bot.

		static const unsigned char NavCurrentVersion = 1u;			/// The current version of the navigation mesh file format.
		static const unsigned int NAV_MAGIC_NUMBER = 0xFeedFace;	///< To help identify navigation mesh files.

		/**
		* A place is a named group of navigation areas
		*/
		DECLARE_TYPED_ENUMERATION (unsigned short, Place_t)
		{
			UNDEFINED_PLACE,	// i.e.: "no place"
			ANY_PLACE = static_cast <Place_t> (-1)
		};
		enum PlaceConstant_t
		{
			MAX_PLACE_NAME_LENGTH = 18u
		};

		enum NavErrorType_t
		{
			NAV_OK,

			NAV_CANT_ACCESS_FILE,
			NAV_INVALID_FILE,
			NAV_BAD_FILE_VERSION,
			NAV_FILE_OUT_OF_DATE,
			NAV_CORRUPT_DATA,
			NavErrorType_NotEnoughMemory
		};

		enum NavAttributeType_t
		{
			NavAttributeType_CROUCH,		///< must crouch to use this node/area
			NavAttributeType_JUMP,			///< must jump to traverse this area
			NavAttributeType_DOUBLE_JUMP,	///< must jump from head of some player to traverse this area
			NavAttributeType_PRECISE,		///< do not adjust for obstacles, just move along area
			NavAttributeType_NO_JUMP,		///< inhibit discontinuity jumping
			NavAttributeType_STOP,			///< must stop when entering this area
			NavAttributeType_RUN,			///< must run to traverse this area
			NavAttributeType_WALK,			///< must walk to traverse this area
			NavAttributeType_AVOID,			///< avoid this area unless alternatives are too dangerous
			NavAttributeType_TRANSIENT,		///< area may become blocked, and should be periodically checked
			NavAttributeType_DONT_HIDE,		///< area should not be considered for hiding spot generation
			NavAttributeType_STAND,			///< bots hiding in this area should stand
			NavAttributeType_NO_HOSTAGES,	///< hostages shouldn't use this area
			NavAttributeType_STAIRS,
			NavAttributeType_UNMERGEABLE,
			NavAttributeType_OBSTACLE,
			NavAttributeType_CLIFF,

			NavAttributeType_Total
		};
		static const DynamicString NavAttributeStrings[NavAttributeType_Total];
		DECLARE_TYPED_ENUMERATION (unsigned short, NavAttributeFlag_t)
		{
			NavAttributeFlag_None,

			NAV_MESH_CROUCH      = BIT (NavAttributeType_CROUCH),		///< must crouch to use this node/area
			NAV_MESH_JUMP        = BIT (NavAttributeType_JUMP),			///< must jump to traverse this area
			NAV_MESH_DOUBLE_JUMP = BIT (NavAttributeType_DOUBLE_JUMP),	///< must jump from head of some player to traverse this area
			NAV_MESH_PRECISE     = BIT (NavAttributeType_PRECISE),		///< do not adjust for obstacles, just move along area
			NAV_MESH_NO_JUMP     = BIT (NavAttributeType_NO_JUMP),		///< inhibit discontinuity jumping
			NAV_MESH_STOP        = BIT (NavAttributeType_STOP),			///< must stop when entering this area
			NAV_MESH_RUN         = BIT (NavAttributeType_RUN),			///< must run to traverse this area
			NAV_MESH_WALK        = BIT (NavAttributeType_WALK),			///< must walk to traverse this area
			NAV_MESH_AVOID       = BIT (NavAttributeType_AVOID),		///< avoid this area unless alternatives are too dangerous
			NAV_MESH_TRANSIENT   = BIT (NavAttributeType_TRANSIENT),	///< area may become blocked, and should be periodically checked
			NAV_MESH_DONT_HIDE   = BIT (NavAttributeType_DONT_HIDE),	///< area should not be considered for hiding spot generation
			NAV_MESH_STAND       = BIT (NavAttributeType_STAND),		///< bots hiding in this area should stand
			NAV_MESH_NO_HOSTAGES = BIT (NavAttributeType_NO_HOSTAGES),	///< hostages shouldn't use this area
			NAV_MESH_STAIRS      = BIT (NavAttributeType_STAIRS),
			NAV_MESH_UNMERGEABLE = BIT (NavAttributeType_UNMERGEABLE),
			NAV_MESH_OBSTACLE    = BIT (NavAttributeType_OBSTACLE),
			NAV_MESH_CLIFF       = BIT (NavAttributeType_CLIFF)
		};

		enum NavDirType_t
		{
			NORTH,
			EAST,
			SOUTH,
			WEST,

			NUM_DIRECTIONS
		};
		DECLARE_TYPED_ENUMERATION (unsigned char, DirectionBit_t)
		{
			DirectionBit_North = BIT (NORTH),
			DirectionBit_East  = BIT (EAST),
			DirectionBit_South = BIT (SOUTH),
			DirectionBit_West  = BIT (WEST)
		};	// For NavigationNode::m_visited variable.

		/**
		* Defines possible ways to move from one area to another
		*/
		enum NavTraverseType_t
		{
			// NOTE: First 4 directions MUST match 'NavDirType_t'.
			GO_NORTH,
			GO_EAST,
			GO_SOUTH,
			GO_WEST,

			// NOTE: Next 2 directions MUST match 'NavigationLadder::Direction_t'.
			GO_LADDER_UP,
			GO_LADDER_DOWN,

			GO_JUMP,

			NUM_TRAVERSE_TYPES
		};

		enum NavCornerType_t
		{
			NORTH_WEST,
			NORTH_EAST,
			SOUTH_EAST,
			SOUTH_WEST,

			NUM_CORNERS
		};

		enum NavRelativeDirType_t
		{
			FORWARD,
			RIGHT,
			BACKWARD,
			LEFT,
			UP,
			DOWN,

			NUM_RELATIVE_DIRECTIONS
		};

		#include <Navigation mesh/nav_colors.hpp>
		#include <Navigation mesh/BaseNavigationArea.hpp>
		#include <Navigation mesh/NavigationLadder.hpp>
		#include <Navigation mesh/NavigationNode.hpp>
		#include <Navigation mesh/NavigationArea.hpp>
		#include <Navigation mesh/PlaceDirectory.hpp>
		#include <Navigation mesh/PlaceContainer.hpp>
		#include <Navigation mesh/nav_pathfind.hpp>

		typedef DynamicArray <NavigationArea::HidingSpot *> HidingSpotArray;
		typedef DynamicArray <NavigationArea_Thinkable *>   ThinkableAreasArray;
		typedef DynamicArray <NavigationLadder_Thinkable *> ThinkableLaddersArray;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline NavigationMesh (void) :
			m_grid (NULL),

			m_nodesGrid (NULL),

			m_nextBaseAreaID (0u),	// Set the next base area ID to 0
			m_nextAreaID (0u),		// Set the next area ID to 0
			m_nextLadderID (0u),	// Set the next ladder ID to 0

			m_generationMode (GENERATE_NONE),
			m_currentNode (NULL),
			m_isFromCurrentMap (true),
			m_isEditing (false),
			m_isNavigationMeshChanged (false),
			m_navPlace (UNDEFINED_PLACE),
			m_markedArea (NULL),
			m_selectedArea (NULL),
			m_lastSelectedArea (NULL),
			m_navEditMode (NAV_EDIT_NORMAL),
			m_spawnTeam (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist),	// set the default value
			m_isPlacePainting (false),
			m_climbableSurface (false),
			m_loadState (LoadState_None),
			m_markedLadder (NULL),
			m_selectedLadder (NULL),

			m_generator (NULL),
			m_editor (NULL)
		{
/*			#if defined _DEBUG
				// Reliability check.
				if (m_nodesGrid == NULL)
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation nodes grid!", sizeof (NavigationNodesGridArray) * m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z);
			#endif	// if defined _DEBUG
*/
			// clear the hash table
//			for (unsigned short index (0u); index < HASH_TABLE_SIZE; ++index)
//				m_hashTable[index] = NULL;

			CheckProgressBarResourceFile ();
		}
		inline ~NavigationMesh (void)
		{
			// Destroy the Navigation Mesh interface.

			m_areas.DeleteAll ();	// destroy all areas
			m_ladders.DeleteAll ();	// destroy ladder representations

			// destroy navigation nodes created during map generation
			m_nodes.DeleteAndPurgeAll ();	// destroy all nodes
			delete [] m_nodesGrid;	// destroy all nodes

			// destroy the grid
			delete [] m_grid;

			delete m_generator;
			delete m_editor;
		}

	//
	// Group: Private functions.
	//
	private:
		inline const unsigned char GetGridSize (const bool forceGrid = false) const
		{
			if (IsGenerating ())
				return static_cast <unsigned char> (GenerationStepSize);

			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_snap_to_grid)->GetValue <unsigned char> ())
			{
				case 0u:
					return forceGrid ? static_cast <unsigned char> (GenerationStepSize) : 0u;

				case 2u:
					return 5u;

				case 3u:
					return 1u;

				default:
					return static_cast <unsigned char> (GenerationStepSize);
			}
		}

		const NavErrorType_t LoadAreas   (const BaseFile &file);		///< load navigation areas from a file
		const NavErrorType_t LoadLadders (const BaseFile &file);		///< load navigation ladders from a file
		inline void          SaveAreas   (BaseFile &file) const	///< store Navigation Areas to a file
		{
			const NavAreaArray::IndexType_t count (m_areas.GetElementNumber ());

			// store number of areas
			file.Write (count);

			// store each area
			for (NavAreaArray::IndexType_t index (0u); index < count; ++index)
				m_areas[index]->Save (file);
		}
		inline void          SaveLadders (BaseFile &file) const	///< store Navigation Ladders to a file
		{
			const NavLadderArray::IndexType_t count (m_ladders.GetElementNumber ());

			// store number of ladders
			file.Write (count);

			// store each ladder
			for (NavLadderArray::IndexType_t index (0u); index < count; ++index)
				m_ladders[index]->Save (file);
		}

		NavigationNode *const LadderEndSearch              (Math::Vector3D center, const NavDirType_t mountDir) const;
		NavigationNode *const TeleportDestinationEndSearch (Math::Vector3D teleportDestination) const;
		NavigationNode *const PlayerSpawnPointEndSearch    (Math::Vector3D spawnOrigin) const;

		void WarnIfMeshNeedsAnalysis (void) const;

		static const bool     TestJumpDown     (const NavigationArea *const downArea, const Math::Vector3D &fromPos, const Math::Vector3D &toPos);
		NavigationArea *const FindJumpDownArea (const Math::Vector3D &fromPos, const NavDirType_t direction) const;

		static inline const float Round (const float value, const float unit)
		{
			return static_cast <short> ((value < 0.0f ? -unit : unit) * 0.5f + value) / static_cast <short> (unit) * unit;
		}

		static void ShowAnalysisProgress            (const char *const message, const unsigned char currentTick);
		static void ShowAnalysisProgressWithPercent (const char *const message, const unsigned char currentTick);
		static void HideAnalysisProgress            (void);

	//
	// Group: Functions.
	//
	public:
		//--------------------------------------------------------------------------------------------------------------
		static inline const NavDirType_t &GetOppositeDirection (const NavDirType_t direction)
		{
			static const NavDirType_t oppositeDirections[NUM_DIRECTIONS] = {SOUTH, WEST, NORTH, EAST};

			return oppositeDirections[direction];
		}

		//--------------------------------------------------------------------------------------------------------------
		static inline const NavDirType_t &GetDirectionLeft (const NavDirType_t direction)
		{
			static const NavDirType_t directionLeft[NUM_DIRECTIONS] = {WEST, NORTH, EAST, SOUTH};

			return directionLeft[direction];
		}

		//--------------------------------------------------------------------------------------------------------------
		static inline const NavDirType_t &GetDirectionRight (const NavDirType_t direction)
		{
			static const NavDirType_t directionRight[NUM_DIRECTIONS] = {EAST, SOUTH, WEST, NORTH};

			return directionRight[direction];
		}

		//--------------------------------------------------------------------------------------------------------------
		static inline void AddDirectionVector (Math::Vector2D &v, const NavDirType_t direction, const float amount = 1.0f)
		{
			switch (direction)
			{
				case NORTH:
					v.y -= amount;

					break;

				case EAST:
					v.x += amount;

					break;

				case SOUTH:
					v.y += amount;

					break;

				default:	// WEST
					v.x -= amount;

					break;
			}
		}

		//--------------------------------------------------------------------------------------------------------------
		static inline const float &DirectionToAngle (const NavDirType_t direction)
		{
			static const float directionAngles[NUM_DIRECTIONS] = {270.0f, 0.0f, 90.0f, 180.0f};

			return directionAngles[direction];
		}

		//--------------------------------------------------------------------------------------------------------------
		static inline const NavDirType_t AngleToDirection (float angle)
		{
			angle = Math::AngleMod (angle);

			if (angle < 45.0f || angle > 315.0f)
				return EAST;

			if (angle >= 45.0f && angle < 135.0f)
				return SOUTH;

			if (angle >= 135.0f && angle < 225.0f)
				return WEST;

			return NORTH;
		}

		//--------------------------------------------------------------------------------------------------------------
		static inline const Math::Vector2D &GetDirectionToVector2D (const NavDirType_t direction)
		{
			static const Math::Vector2D directionVector2Ds[NUM_DIRECTIONS] = {Math::Vector2D (0.0f, -1.0f), Math::Vector2D (1.0f, 0.0f), Math::Vector2D (0.0f, 1.0f), Math::Vector2D (-1.0f, 0.0f)};

			return directionVector2Ds[direction];
		}

		//--------------------------------------------------------------------------------------------------------------
		static inline const Math::Vector2D &GetCornerVector2D (const NavCornerType_t corner)
		{
			static const Math::Vector2D cornerVector2Ds[NUM_CORNERS] = {Math::Vector2D (-1.0f, -1.0f).Normalize (), Math::Vector2D (1.0f, -1.0f).Normalize (), Math::Vector2D (1.0f, 1.0f).Normalize (), Math::Vector2D (-1.0f, 1.0f).Normalize ()};

			return cornerVector2Ds[corner];
		}

		void Reset (void);											///< destroy Navigation Mesh data and revert to initial state
		void Think (void);											///< invoked on each game frame

		const NavErrorType_t Load (void);									///< load navigation data from a file
		inline const bool IsLoaded (void) const { return m_loadState > LoadState_None; }				///< return true if a Navigation Mesh has been loaded
		const bool Save (void);									///< store Navigation Mesh to a file
		const bool SaveToOBJFile (void) const;									///< store Navigation Mesh to a "*.OBJ" file
		inline const bool IsFromCurrentMap (void) const { return m_isFromCurrentMap; }	///< return true if the Navigation Mesh was last edited with the current map version

		void WriteNavigationAreasBitmap (const DynamicString &filename) const;

		inline const BaseNavigationArea::ID_t  IncreaseAndGetNextBaseAreaID (void)                                     { return ++m_nextBaseAreaID; }
		inline const BaseNavigationArea::ID_t  GetNextBaseAreaID            (void)                               const { return m_nextBaseAreaID; }
		inline const void                      SetNextBaseAreaID            (const BaseNavigationArea::ID_t id)        { m_nextBaseAreaID = id; }
		inline const NavAreaArray::IndexType_t IncreaseAndGetNextAreaID     (void)                                     { return ++m_nextAreaID; }
		inline const NavAreaArray::IndexType_t GetNextAreaID                (void)                               const { return m_nextAreaID; }
		inline const void                      SetNextAreaID                (const NavAreaArray::IndexType_t id)       { m_nextAreaID = id; }
		inline const BaseNavigationArea::ID_t  IncreaseAndGetNextLadderID   (void)                                     { return ++m_nextLadderID; }
		inline const BaseNavigationArea::ID_t  GetNextLadderID              (void)                               const { return m_nextLadderID; }
		inline const void                      SetNextLadderID              (const BaseNavigationArea::ID_t id)        { m_nextLadderID = id; }

		inline void CompressAreasIDs   (void)							///< re-orders area ID's so they are continuous
		{
			// Reset the next area ID to 0
			m_nextAreaID = 0u;

			for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
			{
				NavigationArea *const area (m_areas[index]);

				area->m_id = ++m_nextAreaID;

				// remove and re-add the area from the navigation mesh to update the hashed ID
//				RemoveAreaFromHash (area);
//				AddAreaToHash (area);
			}
		}
		inline void CompressLaddersIDs (void)							///< re-orders ladder ID's so they are continuous
		{
			// Reset the next ladder ID to 0
			m_nextLadderID = 0u;

			for (NavLadderArray::IndexType_t index (0u); index < m_ladders.GetElementNumber (); ++index)
				m_ladders[index]->m_id = ++m_nextLadderID;
		}

		NavigationArea *const GetNavArea               (const Math::Vector3D &pos, const float beneathLimt = 120.0f) const;	///< given a position, return the navigation area that IsOverlapping and is *immediately* beneath it
		NavigationArea *const GetNavAreaByID           (const BaseNavigationArea::ID_t id) const;
		NavigationArea *const GetNearestNavArea        (Math::Vector3D pos/*, const bool anyZ = false*/, const float maxDist = 10000.0f, const bool checkLOS = false, const bool UNKNOWN1 = true, const char UNKNOWN2 = -2) const;
		NavigationArea *const FindFirstAreaInDirection (const Math::Vector3D &start, const NavDirType_t direction, const float range, const float beneathLimit, HalfLifeEngine::SDK::Classes::Edict *const traceIgnore = NULL, Math::Vector3D *const closePos = NULL) const;

		inline NavigationArea::HidingSpot *const GetHidingSpotByID (const unsigned int id) const	///< Given a NavigationArea::HidingSpot ID, return the associated HidingSpot.
		{
			for (HidingSpotArray::IndexType_t index (0u); index < m_hidingSpots.GetElementNumber (); ++index)
			{
				NavigationArea::HidingSpot *const spot (m_hidingSpots[index]);

				if (spot->GetID () == id)
					return spot;
			}

			return NULL;
		}

		inline const PlaceDirectory &GetPlaceDirectory (void) const { return m_placeDirectory; }
		inline       PlaceContainer &GetPlaceContainer (void)       { return m_placeContainer; }
		inline const PlaceContainer &GetPlaceContainer (void) const { return m_placeContainer; }
		inline const Place_t GetPlace (const Math::Vector3D &pos) const							///< Return radio chatter place for given coordinate
		{
			const NavigationArea *const area (GetNearestNavArea (pos/*, true*/));

			if (area != NULL)
				return area->GetPlace ();

			return UNDEFINED_PLACE;
		}

		static const bool GetGroundHeight       (const Math::Vector3D &pos, float *const height = NULL, Math::Vector3D *const normal = NULL, const float maximumOffset = 100.0f);	///< get the Z coordinate of the topmost ground level below the given point
		static const bool GetSimpleGroundHeight (const Math::Vector3D &pos, float *const height = NULL, Math::Vector3D *const normal = NULL);	///< get the Z coordinate of the ground level directly below the given point

		/// increase "danger" weights in the given navigation area and nearby ones
		void IncreaseDangerNearby (const TeamArrayID_t teamArrayID, const float amount, NavigationArea *const area, const Math::Vector3D &pos, const float maxRadius);
		inline void IncreaseDangerNearby (const HalfLifeEngine::SDK::Constants::TeamID_t teamID, const float amount, NavigationArea *const area, const Math::Vector3D &pos, const float maxRadius) { IncreaseDangerNearby (static_cast <const TeamArrayID_t> (teamID - 1u), amount, area, pos, maxRadius); }
		void DrawDanger (void) const;										///< draw the current danger levels

		inline void RemovePlayers (void)										///< Zero players in all areas
		{
			for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
				m_areas[index]->RemovePlayers ();
		}
		void DrawPlayerCounts (void) const;								///< draw the current player counts for each area
		void DrawOccupyTime   (void) const;
		void DrawBattlefront  (void) const;

		//-------------------------------------------------------------------------------------
		// Auto-generation
		//
		void BeginGeneration (const bool incremental = false);					///< initiate the generation process
		void BeginAnalysis (void);											///< re-analyze an existing Mesh. Determine Hiding Spots, Encounter Spots, etc.

		inline const bool IsGenerating (void) const { return m_generationMode > GENERATE_NONE; }	///< return true while a Navigation Mesh is being generated

		inline const HalfLifeEngine::SDK::Constants::TeamID_t GetPlayersSpawnPointsTeam (void)                                                const { return m_spawnTeam; }	///< Return team of player spawn points
		inline void                                           SetPlayersSpawnPointsTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team)       { m_spawnTeam = team; }	///< Define the team of player spawn points

		inline void AddWalkableSeed (const Math::Vector3D &pos, const Math::Vector3D &normal)	///< Add given walkable position to list of seed positions for map sampling.
		{
			const WalkableSeedSpot_t seed = {pos, normal};

			m_walkableSeeds += seed;
		}
		void AddWalkableSeeds (void);
		inline void ClearWalkableSeeds (void) { m_walkableSeeds.RemoveAll (); }	///< erase all walkable seed positions

		static const bool FindGroundForNode (Math::Vector3D &pos, Math::Vector3D &normal);

		//-------------------------------------------------------------------------------------
		// Edit mode
		//
		inline const Place_t GetNavPlace (void)                const { return m_navPlace; }
		inline void          SetNavPlace (const Place_t place)       { m_navPlace = place; }

		// Edit callbacks from 'ConsoleCommand's
		void CommandNavDelete                       (void);									///< delete current area
		void CommandNavDeleteMarked                 (void);									///< delete current marked area
		void CommandNavSplit                        (void);									///< split current area
		void CommandNavMerge                        (void);									///< merge adjacent areas
		void CommandNavMark                         (void);									///< mark an area for further operations
		void CommandNavUnmark                       (void);									///< removes the mark
		void CommandNavBeginArea                    (void);									///< begin creating a new navigation area
		void CommandNavEndArea                      (void);									///< end creation of the new navigation area
		void CommandNavConnect                      (void);									///< connect marked area to selected area
		void CommandNavDisconnect                   (void);									///< disconnect marked area from selected area
		void CommandNavSplice                       (void);									///< create new area in between marked and selected areas
		void CommandNavCrouch                       (void);									///< toggle crouch attribute on current area
		void CommandNavTogglePlaceMode              (void);									///< switch between normal and place editing
		void CommandNavSetPlaceMode                 (void);									///< switch between normal and place editing
		void CommandNavPlaceFloodFill               (void);									///< floodfill areas out from current area
		void CommandNavPlacePick                    (void);									///< "pick up" the place at the current area
		void CommandNavTogglePlacePainting          (void);									///< switch between "painting" places onto areas
		void CommandNavMarkUnnamed                  (void);									///< mark an unnamed area for further operations
		void CommandNavCornerSelect                 (void);									///< select a corner on the current area
		void CommandNavCornerRaise                  (void);									///< raise a corner on the current area
		void CommandNavCornerLower                  (void);									///< lower a corner on the current area
		void CommandNavCornerPlaceOnGround          (void);									///< position a corner on the current area at ground height
		void CommandNavWarpToMark                   (void) const;							///< warp a spectating local player to the selected mark
		void CommandNavLadderFlip                   (void);									///< Flips the direction a ladder faces
		void CommandNavToggleAttribute              (const NavAttributeFlag_t attribute);	///< toggle an attribute on current area
		void CommandNavMakeSniperSpots              (void);									///< cuts up the marked area into individual areas suitable for sniper spots
		void CommandNavBuildLadder                  (void);									///< builds a navigation ladder on the climbable surface under the cursor
		inline void CommandNavRemoveUnusedJumpAreas (void) { RemoveUnusedJumpAreas (); }	///< removes jump areas with at most 1 connection to a ladder or non-jump area

		// IN-PROGRESS COMMANDS FOR MANIPULATING EXISTING AREAS
		void CommandNavPickArea (void);
		void CommandNavResizeHorizontal (void);
		void CommandNavResizeVertical (void);
		void CommandNavResizeEnd (void);

		inline const bool IsPlaceMode (void) const { return m_navEditMode == NAV_EDIT_PLACE; }

		static inline const bool GetEditVectors (Math::Vector3D &eyePosition, Math::Vector3D &viewDirection);	///< Gets the eye position and view direction of the editing player. (Note: This function declared in HumanManager.hpp)

		inline NavigationArea   *const GetMarkedArea     (void) const { return m_markedArea; }		///< return area marked by user in edit mode
		inline NavigationLadder *const GetMarkedLadder   (void) const { return m_markedLadder; }	///< return ladder marked by user in edit mode

		inline NavigationArea   *const GetSelectedArea   (void) const { return m_selectedArea; }	///< return area selected by user in edit mode
		inline NavigationLadder *const GetSelectedLadder (void) const { return m_selectedLadder; }	///< return ladder selected by user in edit mode

		void CreateLadder (HalfLifeEngine::SDK::Classes::Edict *const ladderEdict);

		inline const float SnapToGrid (const float x, const bool forceGrid = false) const					///< snap given coordinate to generation grid boundary
		{
			const unsigned char scale (GetGridSize (forceGrid));

			if (scale == 0u)
				return x;

			return Round (x, scale);
		}
		inline const Math::Vector3D SnapToGrid (const Math::Vector3D &in, const bool snapX = true, const bool snapY = true, const bool forceGrid = false) const	///< snap given vector's X & Y coordinates to generation grid boundary
		{
			const unsigned char scale (GetGridSize (forceGrid));

			if (scale == 0u)
				return in;

			return Math::Vector3D (snapX ? Round (in.x, scale) : in.x, snapY ? Round (in.y, scale) : in.y, in.z);
		}

		inline const bool            IsEditing             (void) const { return m_isEditing; }	///< true if in edit mode
		inline const Math::Vector3D &GetEditCursorPosition (void) const { return m_editCursorPos; }	///< return position of edit cursor
		inline void StripNavigationAreas (void)	///< Strip the "analyzed" data out of all navigation areas.
		{
			for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
				m_areas[index]->Strip ();
		}
		inline const DynamicString GetFilename (void) const								///< Return the filename for this map's "navigation map" file
		{
			// filename is local to game directory for Steam, so we need to prepend game directory for regular file save

			return "Addons/YaPB/Navigation mesh/Maps/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + '.' + FileExtention;
		}

		//-------------------------------------------------------------------------------------
		/**
		* Apply the functor to all navigation areas.
		* If functor returns false, stop processing and return false.
		*/
		template <typename Functor> inline const bool ForAllAreas (const Functor &function)
		{
			for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
				if (function (m_areas[index]) == false)
					return false;

			return true;
		}

		inline BaseNavigationArea *const GetClosestBaseArea (const Math::Vector3D &position, float minimumDistance = HalfLifeEngine::SDK::Constants::MapSize) const
		{
			BaseNavigationArea *closestArea (NULL);

			minimumDistance *= minimumDistance;	// Square up the minimum distance.

			for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
			{
				BaseNavigationArea *const area (m_areas[index]);
				const float distanceSquared (position.GetDistanceSquared (area->GetCenter ()));

				if (minimumDistance > distanceSquared)
				{
					minimumDistance = distanceSquared;
					closestArea = area;
				}
			}

			return closestArea;
		}

		inline       NavAreaArray    &GetAreas       (void)       { return m_areas; }	///< Returns the list of areas
		inline const NavAreaArray    &GetAreas       (void) const { return m_areas; }	///< Returns the list of areas
		inline       NavLadderArray  &GetLadders     (void)       { return m_ladders; }	///< Returns the list of ladders
		inline const NavLadderArray  &GetLadders     (void) const { return m_ladders; }	///< Returns the list of ladders
		inline       HidingSpotArray &GetHidingSpots (void)       { return m_hidingSpots; }	///< Returns the list of hiding spots
		inline const HidingSpotArray &GetHidingSpots (void) const { return m_hidingSpots; }	///< Returns the list of hiding spots

		NavigationLadder *const GetLadderByID (const BaseNavigationArea::ID_t id) const;

		inline const NavAreaArray &GetTransientAreas (void) const { return m_transientAreas; }

		inline       WorldMap &GetWorldMap (void)       { return m_worldMap; }
		inline const WorldMap &GetWorldMap (void) const { return m_worldMap; }

	//
	// Group: Private members.
	//
	private:
		NavAreaArray   *m_grid;
		struct GridSize_t
		{
			unsigned char x, y;

			inline GridSize_t (void) : x (0u), y (0u) { /*VOID*/ }
		}               m_gridSize;
		Math::Vector2D  m_mins;

		WorldMap m_worldMap;

		enum LoadState_t
		{
			LoadState_None,

			LoadState_FromGeneration,
			LoadState_FromFile
		} m_loadState;												///< true if a Navigation Mesh has been loaded
		bool m_isFromCurrentMap;									///< true if the Navigation Mesh was last saved with the current map

		BaseNavigationArea::ID_t  m_nextBaseAreaID;					///< used to allocate unique IDs
		NavAreaArray::IndexType_t m_nextAreaID;						///< used to allocate unique IDs
		BaseNavigationArea::ID_t  m_nextLadderID;					///< used to allocate unique IDs
/*
		enum { HASH_TABLE_SIZE = 256u };
		NavigationArea *m_hashTable[HASH_TABLE_SIZE];					///< navigation area hash table to optimize lookup by ID
		inline const unsigned char ComputeHashKey (const BaseNavigationArea::ID_t id) const { return id & HASH_TABLE_SIZE - 1u; }				///< returns a hash key for the given navigation area ID
*/
#if 0
		/*
			=============================================================================
			The PVS must include a small area around the client to allow head bobbing
			or other small motion on the client side. Otherwise, a bob might cause an
			entity that should be visible to not show up, especially when the bob
			crosses a waterline.
			=============================================================================
		*/
		DynamicArray <int> m_areasNetworked;
		unsigned char m_areasPVS[WorldMap::MAX_MAP_LEAFS/* / 8u*/];
		unsigned int  m_areasPVSSize;
		unsigned int  m_areasPVSFatBytes;

		inline void ResetPVS (void)
		{
			// Purpose: Zeroes out PVS, this way we can or together multiple PVS's for a player.

			m_areasPVSFatBytes = (m_worldMap.GetBSPFile ().leafs.GetElementNumber ()/*models[0u].visLeafs - CHECK IT!!!*/ + 7u/*31u*/) >> 3u;

			memset (m_areasPVS, 0u, m_areasPVSFatBytes);

			m_areasNetworked.RemoveAll ();
		}
		inline void AddToFatPVS (const Math::Vector3D &origin)
		{
			const unsigned char *const PVS (m_worldMap.CM_ClusterPVS (m_worldMap.CM_LeafCluster (m_worldMap.FindLeaf (origin))));

			for (unsigned int index (0u); index < m_areasPVSFatBytes; ++index)
				m_areasPVS[index] |= PVS[index];
		}
		inline void AddOriginToPVS (const Math::Vector3D &origin)
		{
			// Calculates a PVS that is the inclusive or of all leafs within 8 pixels of the given point.

			AddToFatPVS (origin);

			const int area (m_worldMap.CM_LeafArea (m_worldMap.FindLeaf (origin)));

			if (!m_areasNetworked.IsMember (area))
				m_areasNetworked += area;
		}
#endif	// if 0
		inline const unsigned char WorldToGridX (const float worldX) const				///< given X component, return grid index
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldX - m_mins.x) / GridCellSize), 0, m_gridSize.x - 1));
		}
		inline const unsigned char WorldToGridY (const float worldY) const				///< given Y component, return grid index
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldY - m_mins.y) / GridCellSize), 0, m_gridSize.y - 1));
		}
		void AllocateGrid (const Math::Extent2D &extent);	///< clear and reset the grid to the given extents
		const bool AllocateGrid (void);
		inline const Math::Vector2D GetWorldVectorFromGrid (const GridSize_t &gridIndex) const
		{
			// Reliability checks.
			InternalAssert (gridIndex.x < m_gridSize.x);
			InternalAssert (gridIndex.y < m_gridSize.y);

			return Math::Vector2D (m_mins.x + gridIndex.x * GridCellSize, m_mins.y + gridIndex.y * GridCellSize);
		}

		inline void AddArea     (NavigationArea *const area)			///< add an area to the mesh @note This function does not add given 'area' to the main 'm_areas' array!
		{
			AddAreaToGrid (area);			// add to grid
//			AddAreaToHash (area);			// add to hash table
			AddAreaToTransientAreas (area);	// add to transient areas array
		}
		void AddAreaToGrid      (NavigationArea *const area);			///< add an area to the grid
//		void AddAreaToHash      (NavigationArea *const area);			///< add an area to the hash table
		inline void AddAreaToTransientAreas (NavigationArea *const area)			///< add an area to the transient areas array
		{
			if (!area->HasAttributes (NAV_MESH_TRANSIENT))
				return;

			// Reliability check.
			InternalAssert (!m_transientAreas.IsMember (area));

			m_transientAreas += area;
		}
		inline void RemoveArea  (NavigationArea *const area)			///< remove an area from the mesh. @note This function does not remove given 'area' from the main 'm_areas' array!
		{
			RemoveAreaFromGrid (area);				// remove from grid
//			RemoveAreaFromHash (area);				// remove from hash table
			RemoveAreaFromTransientAreas (area);	// remove from transient areas array
		}
		void RemoveAreaFromGrid (NavigationArea *const area);			///< remove an area from the grid
//		void RemoveAreaFromHash (NavigationArea *const area);			///< remove an area from the hash table
		inline void RemoveAreaFromTransientAreas (NavigationArea *const area)			///< remove an area from the transient areas array
		{
			if (!area->HasAttributes (NAV_MESH_TRANSIENT))
				return;

			// Reliability check.
			InternalAssert (m_transientAreas.IsMember (area));

			m_transientAreas -= area;
		}

		void DestroyNavigationMesh (const bool incremental = false);	///< free all resources of the mesh and reset it to empty state
		void DestroyHidingSpots    (void);
		void Disconnect            (NavigationArea *const area);		///< Disconnect given area from all others.
		void DestroyArea           (NavigationArea *const area);		///< Disconnect given area from all others, remove from a grid and navigation mesh.
		void Disconnect            (NavigationLadder *const ladder);	///< Disconnect given ladder from all connected areas.
		void DestroyLadder         (NavigationLadder *const ladder);	///< Disconnect given ladder from all connected areas and remove from a navigation mesh.

		void ComputeBattlefrontAreas (void);							///< determine areas where rushing teams will first meet

		//----------------------------------------------------------------------------------
		// Place directory
		//
		PlaceDirectory m_placeDirectory;
		PlaceContainer m_placeContainer;	///< master directory of place names (ie: "places")

		//----------------------------------------------------------------------------------
		// Edit mode
		//
		Place_t m_navPlace;											///< current navigation place for editing
		void OnEditModeStart (void);								///< called when edit mode has just been enabled
		void DrawEditMode    (void);								///< draw navigation areas
		void OnEditModeEnd   (void);								///< called when edit mode has just been disabled
		bool m_isEditing;											///< true if in edit mode
		bool m_isNavigationMeshChanged;								///< true if navigation mesh has been changed by editor
		Math::Vector3D m_editCursorPos;								///< current position of the cursor
		NavigationArea *m_markedArea;								///< currently marked area for edit operations
		NavigationArea *m_selectedArea;								///< area that is selected this frame
		NavigationArea *m_lastSelectedArea;							///< area that was selected last frame
		NavCornerType_t m_markedCorner;								///< currently marked corner for edit operations
		Math::Vector3D m_anchor;									///< first corner of an area being created
		bool m_isPlacePainting;										///< if true, we set an area's place by pointing at it
		bool m_splitAlongX;											///< direction the selected navigation area would be split
		float m_splitEdge;											///< location of the possible split
		Math::Extent3D m_generationExtent;							///< @todo INVOLVE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		enum NavEditMode
		{
			NAV_EDIT_NORMAL,
			NAV_EDIT_PLACE,											///< place-editing mode
			NAV_EDIT_CREATE_AREA,									///< manually creating a new navigation area
			NAV_EDIT_CREATE_LADDER,									///< manually creating a new navigation ladder
			NAV_EDIT_RESIZE_HORIZONTAL,
			NAV_EDIT_RESIZE_VERTICAL
		} m_navEditMode;

		bool m_climbableSurface;									///< if true, the cursor is pointing at a climable surface
		Math::Vector3D m_surfaceNormal;								///< Normal of the surface the cursor is pointing at
		Math::Vector3D m_ladderAnchor;								///< first corner of a ladder being created
		Math::Vector3D m_ladderNormal;								///< Normal of the surface of the ladder being created
		NavigationLadder *m_selectedLadder;							///< ladder that is selected this frame
		NavigationLadder *m_lastSelectedLadder;						///< ladder that was selected last frame
		NavigationLadder *m_markedLadder;							///< currently marked ladder for edit operations

		BaseNavigationArea *const GetNavAreaOrLadderAlongRay (const Math::Vector3D &source, const Math::Vector3D &destination, const BaseNavigationArea *const areaToSkip = NULL) const;
		const bool                FindActiveNavArea          (void);								///< Finds the area or ladder the local player is currently pointing at. Returns true if a surface was hit by the traceline.

		inline void SetEditMode (const NavEditMode mode = NAV_EDIT_NORMAL)						///< sets/clears place mode
		{
			m_markedLadder = NULL;
			m_markedArea = NULL;
			m_markedCorner = NUM_CORNERS;
			m_navEditMode = mode;
/*			m_isSelecting = false;
			m_isDeselecting = false;
			m_isUNKNOWN = false;*/
		}
		void SetPlacePaintingMode (const bool painting);					///< Sets place-painting, if we're in place mode
		void SetMarkedLadder (NavigationLadder *const ladder);			///< mark ladder for further edit operations
		void SetMarkedArea (NavigationArea *const area);					///< mark area for further edit operations

		HalfLifeEngine::Utilities::CountdownTimer m_showAreaInfoTimer;							///< Timer that controls how long area info is displayed

		//----------------------------------------------------------------------------------
		// Auto-generation
		//
		const bool UpdateGeneration (const float maxTime = 0.25f);				///< process the auto-generation for 'maxTime' seconds. Return false if generation is complete, true otherwise.

		NavNodeStack              m_nodes;										///< the master stack of all nodes for this map
		NavigationNodesGridArray *m_nodesGrid;									///< the master grid of all nodes for this map
		struct NodesGridSize_t
		{
			unsigned char x, y, z;

			inline void Initialize (void);	// Note: This function declared in Server.hpp.
		}                         m_nodesGridSize;

		inline void AddNavigationNode (NavigationNode *const newNavigationNode)
		{
			// Reliability check.
			InternalAssert (m_nodesGrid != NULL);

			m_nodes += newNavigationNode;	// Add this node to the main navigation mesh nodes stack.

			const unsigned short gridIndex (WorldToNodesGridX (newNavigationNode->GetPosition ().x) + WorldToNodesGridY (newNavigationNode->GetPosition ().y) * m_nodesGridSize.x + WorldToNodesGridZ (newNavigationNode->GetPosition ().z) * m_nodesGridSize.y);

			// Reliability check.
			InternalAssert (gridIndex < m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z);

			m_nodesGrid[gridIndex] += newNavigationNode;
		}
		inline void RemoveNavigationNode (NavigationNode *const newNavigationNode)
		{
			// Reliability check.
			InternalAssert (m_nodesGrid != NULL);

			m_nodes -= newNavigationNode;	// Remove this node from the main navigation mesh nodes stack.

			const unsigned short gridIndex (WorldToNodesGridX (newNavigationNode->GetPosition ().x) + WorldToNodesGridY (newNavigationNode->GetPosition ().y) * m_nodesGridSize.x + WorldToNodesGridZ (newNavigationNode->GetPosition ().z) * m_nodesGridSize.y);

			// Reliability check.
			InternalAssert (gridIndex < m_nodesGridSize.x * m_nodesGridSize.y * m_nodesGridSize.z);

			m_nodesGrid[gridIndex] -= newNavigationNode;
		}

		inline const unsigned char WorldToNodesGridX (const float worldX) const	///< given X component, return grid index
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldX - m_worldMap.GetBSPFile ().models[0u].boundingBox.mins.x) / NodesGridCellSize), 0, m_nodesGridSize.x - 1));
		}
		inline const unsigned char WorldToNodesGridY (const float worldY) const	///< given Y component, return grid index
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldY - m_worldMap.GetBSPFile ().models[0u].boundingBox.mins.y) / NodesGridCellSize), 0, m_nodesGridSize.y - 1));
		}
		inline const unsigned char WorldToNodesGridZ (const float worldZ) const	///< given Z component, return grid index
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldZ - m_worldMap.GetBSPFile ().models[0u].boundingBox.mins.z) / NodesGridCellSize), 0, m_nodesGridSize.z - 1));
		}

		NavigationNode *m_currentNode;									///< the current node we are sampling from
		NavDirType_t    m_generationDirection;							///< the current direction we are sampling from
		NavigationNode *const       GetNode (const Math::Vector3D &pos) const;	///< return navigation node at the position, or NULL if none exists
		const NavigationNode *const AddNode (const Math::Vector3D &destPos, const Math::Vector3D &destNormal, const NavDirType_t direction, NavigationNode *const source);		///< add a navigation node and connect it, update current node

		NavLadderArray m_ladders;									///< list of ladder navigation representations
		void BuildLadders (void);
		void DestroyLadders (void);

		const bool SampleStep (void);									///< sample the walkable areas of the map
		void CreateNavAreasFromNodes (void);						///< cover all of the sampled nodes with navigation areas

		const bool TestArea (const NavigationNode *const nwNode, const unsigned char width, const unsigned char height) const;		///< check if an area of size (width, height) can fit, starting from node as upper left corner
		const unsigned short BuildArea (NavigationNode *const nwNode, const unsigned char width, const unsigned char height);		///< create a NavigationArea of size (width, height) starting fom node at upper left corner

		void CheckProgressBarResourceFile (void);
		static const bool CheckObstacles (const NavigationNode *const node, const unsigned char width, const unsigned char height, const unsigned char y, const unsigned char x);

		void RemoveUnusedJumpAreas (void);	///<  Jump areas that are connected to only one non-jump area won't be used. Delete them.
		void StichJumpAreas (void);
		void RemoveJumpAreas (void);
		inline void StichAndRemoveJumpAreas (void)
		{
			StichJumpAreas ();
			RemoveJumpAreas ();
		}
		void RemoveUnusedDoubleJumpAreas (void);
		void RaiseAreasWithInternalObstacles (void);
		void CreateObstacleTopAreas (void);
		static inline void AdjustObstacleDistances (float &a1, float &a2, const float distance)
		{
			const float difference (a2 - a1);

			if (difference >= 10.0f)
				return;

			float v4 (a1 - (10.0f - difference) * 0.5f);

			if (v4 <= 0.0f)
				v4 = 0.0f;

			a1 = v4;
			a2 = v4 + 10.0f;

			if (distance < a2)
			{
				const float v5 (a2 - distance);

				a1 -= v5;
				a2 -= v5;
			}
		}
		const bool CreateObstacleTopAreaIfNecessary (NavigationArea *const area, NavigationArea *const areaConnect, const NavDirType_t direction, const bool isUNKNOWN);
		void RemoveOverlappingObstacleTopAreas (void);
		inline void HandleObstacleTopAreas (void)
		{
			if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_NavigationMesh_GenerateFenceTopsAreas)->GetValue <bool> ())
				return;

			RaiseAreasWithInternalObstacles ();
			CreateObstacleTopAreas ();
			RemoveOverlappingObstacleTopAreas ();
		}
		void FixCornerOnCornerAreas (void);
		void FixConnections (void);
		inline void FixUpGeneratedAreas (void)
		{
			FixCornerOnCornerAreas ();
			FixConnections ();
		}
		inline void StichAreas (void)
		{
			for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
				m_areas[index]->Stich ();
		}
		void MarkJumpAreas (void);
		void MarkStairAreas (void);
		void SquareUpAreas (void);
		void MergeGeneratedAreas (void);
		void SplitAreasUnderOverhangs (void);
		void ConnectGeneratedAreas (void);
		inline void StitchGeneratedAreas (void)
		{
			if (m_generationMode == GENERATE_INCREMENTAL)
				StitchAreaIntoMesh_IncrementallyGeneratedAreas ();
		}
/*		static inline void TestStitchConnection (const Math::Vector3D &from, const Math::Vector3D &to)
		{
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

			return TraceAdjacentNode (0u, from, to, traceResult, HalfLifeEngine::SDK::Constants::DeathDrop);
		}*/
		void StitchAreaIntoMesh_IncrementallyGeneratedAreas_ (NavigationArea *const area, const NavDirType_t direction);
		void StitchAreaIntoMesh_IncrementallyGeneratedAreas (void);
		void BuildOverlapAreas (void);
		inline void ConnectGeneratedLadders (void)
		{
			for (NavLadderArray::IndexType_t index (0u); index < m_ladders.GetElementNumber (); ++index)
				m_ladders[index]->ConnectGeneratedLadder ();
		}

		inline void OnSplitArea (const NavigationArea *const original, NavigationArea *const alpha, NavigationArea *const beta)
		{
			// For every ladder that pointed to us, connect that ladder to the closer of alpha and beta
			for (NavLadderArray::IndexType_t index (0u); index < m_ladders.GetElementNumber (); ++index)
				m_ladders[index]->OnSplit (original, alpha, beta);
		}

		enum GenerationStateType
		{
			SAMPLE_WALKABLE_SPACE,
			CREATE_AREAS_FROM_SAMPLES,
			FIND_HIDING_SPOTS,
			FIND_APPROACH_AREAS,
			FIND_ENCOUNTER_SPOTS,
			FIND_SNIPER_SPOTS,
			FIND_EARLIEST_OCCUPY_TIMES,
			SAVE_NAV_MESH,

			NUM_GENERATION_STATES
		} m_generationState;											///< the state of the generation process
		enum GenerationModeType
		{
			GENERATE_NONE,

			GENERATE_FULL,
			GENERATE_INCREMENTAL,
			GENERATE_SIMPLIFY_SELECTED_AREAS,
			GENERATE_ANALYSIS_ONLY
		} m_generationMode;						///< true while a Navigation Mesh is being generated
		NavAreaArray::IndexType_t m_generationIndex;										///< used for iterating navigation areas during generation process
		unsigned int m_maximumSampleTicks;
		unsigned int m_sampleTick;											///< counter for displaying pseudo-progress while sampling walkable space

		HalfLifeEngine::SDK::Constants::TeamID_t m_spawnTeam;	///< team of player spawn points, used to initiate sampling

		struct WalkableSeedSpot_t
		{
			Math::Vector3D pos;
			Math::Vector3D normal;
		};
		DynamicArray <WalkableSeedSpot_t> m_walkableSeeds;	///< array of walkable seed spots for sampling
		NavBaseAreaArray           m_baseAreas;
		NavAreaArray               m_areas;

		ThinkableAreasArray   m_thinkableAreas;
		ThinkableLaddersArray m_thinkableLadders;

		HidingSpotArray            m_hidingSpots;

		NavigationNode *const GetNextWalkableSeedNode (void);					///< return the next walkable seed as a node
		unsigned short m_seedIndex;

		void BuildTransientAreaList (void);
		NavAreaArray m_transientAreas;

		#include <Navigation mesh/Generator.hpp>

		Generator *m_generator;

		#include <Navigation mesh/Editor.hpp>

		Editor *m_editor;
};

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//
// Inlines
//

//--------------------------------------------------------------------------------------------------------------
inline void NavigationMesh::NavigationArea::RemoveFromClosedList (void)
{
	// since "closed" is defined as visited (marked) and not on open list, do nothing
}

//--------------------------------------------------------------------------------------------------------------
inline const bool NavigationMesh::NavigationArea::IsVisible (const Math::Vector3D &eye, Math::Vector3D *const visSpot) const
{
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
//	trace_t traceResult;
//	CTraceFilterNoNPCsOrPlayer traceFilter (NULL, COLLISION_GROUP_NONE);
	const float offset (0.75f * HalfLifeEngine::SDK::Constants::HumanHeight);

	// check center first
//	UTIL_TraceLine (eye, GetCenter () + Math::Vector3D (0.0f, 0.0f, offset), MASK_OPAQUE | CONTENTS_MONSTER, &traceFilter, &traceResult);
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (eye, GetCenter () + Math::Vector3D (0.0f, 0.0f, offset), HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

	if (traceResult.fraction == 1.0f)
	{
		// we can see this area

		if (visSpot != NULL)
			*visSpot = GetCenter ();

		return true;
	}

	for (unsigned char c (NORTH_WEST); c < NUM_CORNERS; ++c)
	{
		const Math::Vector3D &corner (GetCorner (static_cast <NavCornerType_t> (c)));

//		UTIL_TraceLine (eye, corner + Math::Vector3D (0.0f, 0.0f, offset), MASK_OPAQUE | CONTENTS_MONSTER, &traceFilter, &traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (eye, corner + Math::Vector3D (0.0f, 0.0f, offset), HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

		if (traceResult.fraction == 1.0f)
		{
			// we can see this area

			if (visSpot != NULL)
				*visSpot = corner;

			return true;
		}
	}

	return false;
}

inline void NavigationMesh::NavigationArea::SpotEncounter::Save (BaseFile &file) const
{
	ID_t id (from != NULL ? from->GetID () : 0u);

	file.Write (id);
	file.Write (fromDir);

	id = to != NULL ? to->GetID () : 0u;

	file.Write (id);
	file.Write (toDir);

	const SpotOrderArray::IndexType_t spotCount (spots.GetElementNumber ());

	// write list of spots along this path
	file.Write (spotCount);

	for (SpotOrderArray::IndexType_t index (0u); index < spotCount; ++index)
		spots[index].Save (file);
}

//--------------------------------------------------------------------------------------------------------------
//
// Function prototypes
//

extern void ApproachAreaAnalysisPrep        (void);
extern void CleanupApproachAreaAnalysisPrep (void);

#endif	// ifndef NAVIGATION_MESH_INCLUDED