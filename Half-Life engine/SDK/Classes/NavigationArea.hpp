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
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_NAVIGATION_AREA_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_NAVIGATION_AREA_INCLUDED

template <typename elementType, typename indexType = unsigned int> class CUtlLinkedList	/// @see 'GList' class from Source engine....
{
	/// @note simple loop example:
	/**
		unsigned int nodeIndex (0u);

		for (const Node_t *node (TheNavAreaList.m_head->next); node != TheNavAreaList.m_head; node = node->next)
			ServerPrintFormat ("area[%u]->m_id = %u.\n", ++nodeIndex, node->element->m_id);
	*/

	public:
		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;

		// list node
		struct Node_t
		{
/*!off=0(0)*/			Node_t      *next;		// successor node, or first element if head
/*!off=4(1)*/			Node_t      *prev;		// predecessor node, or last element if head

/*!off=8(2)*/			elementType  element;	// the stored value, unused if head
		};

	public:
/*!off=0(0)*/		Node_t    *m_tail;	// pointer to tail node (EXISTS?!?!??!?!?!?!?!??!?!?!?!?!?!??!?!?!?)
/*!off=4(1)*/		Node_t    *m_head;	// pointer to head node

/*!off=8(2)*/		indexType  m_elementNumber;	// number of elements
};	// sizeof (CUtlLinkedList) == 12 (3)

class NavigationArea;

//-------------------------------------------------------------------------------------------------------------------
/**
* Used when building a path to determine the kind of path to build
*/
enum RouteType_t
{
	FASTEST_ROUTE,
	SAFEST_ROUTE
};

enum NavDirType_t
{
	NORTH,
	EAST,
	SOUTH,
	WEST,

	NUM_DIRECTIONS
};

enum NavCornerType_t
{
	NORTH_WEST,
	NORTH_EAST,
	SOUTH_EAST,
	SOUTH_WEST,

	NUM_CORNERS
};

DECLARE_TYPED_ENUMERATION (unsigned char, NavAttributeFlag_t)
{
	NavAttributeFlag_None,

	NAV_MESH_CROUCH  = BIT (0u),	///< must crouch to use this node/area
	NAV_MESH_JUMP    = BIT (1u),	///< must jump to traverse this area
	NAV_MESH_PRECISE = BIT (2u),	///< do not adjust for obstacles, just move along area
	NAV_MESH_NO_JUMP = BIT (3u)		///< inhibit discontinuity jumping
};

class NavigationNode
{
	Math::Vector3D m_pos;					///< position of this node in the world (Offset == 0 (0))
	Math::Vector3D m_normal;				///< surface normal at this location (Offset == 12 (3))
	NavigationNode *m_to[NUM_DIRECTIONS];	///< links to north, south, east, and west. NULL if no link (Offset: {NORTH == 24 (6), EAST == 28 (7), SOUTH == 32 (8), WEST == 36 (9)})
	unsigned int m_id;						///< unique ID of this node (Offset == 40 (10))
	NavAttributeFlag_t m_attributeFlags;	///< set of attribute bit flags (Offset == 44 (11))

	NavigationNode *m_next;					///< next link in master list (Offset == 48 (12))

	// below are only needed when generating
	unsigned char m_visited;				///< flags for automatic node generation. If direction bit is clear, that direction hasn't been explored yet. (Offset == 52 (13))
	NavigationNode *m_parent;				///< the node prior to this in the search, which we pop back to when this node's search is done (a stack) (Offset == 56 (14))
	BOOL m_isCovered;						///< true when this node is "covered" by a NavigationArea (Offset == 60 (15))
	NavigationArea *m_area;					///< the area this node is contained within (Offset == 64 (16))
};	// sizeof (NavigationNode) == 68 (17)

class NavigationLadder
{
	public:
		enum Direction_t
		{
			Direction_Up,
			Direction_Down,

			Direction_Total
		};

		enum ConnectionType_t			///< Ladder connection directions, to facilitate iterating over connections
		{
			///< the areas at the top of the ladder
			ConnectionType_TopForward,	///< area at top of ladder "ahead" it - only useful for descending (Offset == 44 (11))
			ConnectionType_TopLeft,		///< (Offset == 48 (12))
			ConnectionType_TopRight,	///< (Offset == 52 (13))
			ConnectionType_TopBehind,	///< (Offset == 56 (14))

			ConnectionType_Bottom,		///< the area at the bottom of the ladder (Offset == 60 (15))

			ConnectionType_Total
		};

	public:
		Math::Vector3D  m_top;									///< world coords of the top of the ladder (Offset == 0 (0))
		Math::Vector3D  m_bottom;								///< world coords of the top of the ladder (Offset == 12 (3))
		float           m_length;								///< the length of the ladder (Offset == 24 (6))

		NavDirType_t    m_dir;									///< which way the ladder faces (ie: surface normal of climbable side) (Offset == 28 (7))
		Math::Vector2D  m_normal;								///< surface normal of the ladder surface (or Vector-ized m_dir, if the traceline fails) (Offset == 32 (8))

		BaseEntity     *m_entity;								///< (Offset == 40 (10))

		NavigationArea *m_connectedAreas[ConnectionType_Total];	///< (Offset == 44 (11))

		bool            m_isDangerBottomJump;					///< (This boolean sets to true when distance (Z-axis) between closest point in bottom area and ladder bottom vector is more than 'Constants::HumanHeight') (Offset == 64 (16))
};	// sizeof (NavigationLadder) == 68 (17)

typedef CUtlLinkedList <NavigationLadder *> NavLadderList;

/**
* A place is a named group of navigation areas
*/
DECLARE_TYPED_ENUMERATION (unsigned int, Place_t)
{
	UNDEFINED_PLACE,	// i.e.: "no place"
	ANY_PLACE = static_cast <unsigned short> (-1)
};

/**
* Defines possible ways to move from one area to another
*/
enum NavTraverseType_t
{
	// NOTE: First 4 directions MUST match NavDirType_t
	GO_NORTH,
	GO_EAST,
	GO_SOUTH,
	GO_WEST,

	// NOTE: Next 2 directions MUST match NavigationLadder::Direction_t
	GO_LADDER_UP,
	GO_LADDER_DOWN,

	GO_JUMP,

	NUM_TRAVERSE_TYPES
};

typedef CUtlLinkedList <NavigationArea *> NavConnectList;
typedef CUtlLinkedList <NavigationLadder *> NavLadderConnectList;

//--------------------------------------------------------------------------------------------------------------
/**
 * A HidingSpot is a good place for a bot to crouch and wait for enemies
 */
class HidingSpot
{
public:
	DECLARE_TYPED_ENUMERATION (unsigned char, Flag_t)
	{ 
		IN_COVER			= 0x01,							///< in a corner with good hard cover nearby
		GOOD_SNIPER_SPOT	= 0x02,							///< had at least one decent sniping corridor
		IDEAL_SNIPER_SPOT	= 0x04,							///< can see either very far, or a large area, or both
		EXPOSED				= 0x08							///< spot in the open, usually on a ledge or cliff
	};

private:
/*!off=0(0)*/	Math::Vector3D m_pos;											///< world coordinates of the spot
/*!off=12(3)*/	unsigned int m_id;										///< this spot's unique ID
/*!off=16(4)*/	unsigned int m_marker;									///< this spot's unique marker
/*!off=20(5)*/	Flag_t m_flags;									///< bit flags
/*
	static unsigned int m_nextID;							///< used when allocating spot ID's
	static unsigned int m_masterMarker;						///< used to mark spots

private:
	friend class CNavMesh;
	friend void ClassifySniperSpot( HidingSpot *spot );

	HidingSpot( void );										///< must use factory to create
	HidingSpot(const Math::Vector3D &pos, const Flag_t flags);										///< must use factory to create

public:
	bool HasGoodCover( void ) const			{ return (m_flags & IN_COVER) ? true : false; }	///< return true if hiding spot in in cover
	bool IsGoodSniperSpot( void ) const		{ return (m_flags & GOOD_SNIPER_SPOT) ? true : false; }
	bool IsIdealSniperSpot( void ) const	{ return (m_flags & IDEAL_SNIPER_SPOT) ? true : false; }
	bool IsExposed( void ) const			{ return (m_flags & EXPOSED) ? true : false; }	

	Flag_t GetFlags( void ) const		{ return m_flags; }

	void Save( FileHandle_t *const file, unsigned int version ) const;
	void Load( FileHandle_t *const file, unsigned int version );

	const Math::Vector3D &GetPosition( void ) const		{ return m_pos; }	///< get the position of the hiding spot
	unsigned int GetID( void ) const			{ return m_id; }

	void Mark( void )							{ m_marker = m_masterMarker; }
	bool IsMarked( void ) const					{ return (m_marker == m_masterMarker) ? true : false; }
	static void ChangeMasterMarker( void )		{ ++m_masterMarker; }

	void SetFlags( Flag_t flags )				{ m_flags |= flags; }	///< FOR INTERNAL USE ONLY
	void SetPosition( const Math::Vector3D &pos )	{ m_pos = pos; }		///< FOR INTERNAL USE ONLY*/
};	// sizeof (HidingSpot) == 24 (6)
typedef CUtlLinkedList <HidingSpot *> HidingSpotList;

//--------------------------------------------------------------------------------------------------------------
/**
 * Stores a pointer to an interesting "spot", and a parametric distance along a path
 */
struct SpotOrder
{
/*!off=0(0)*/	float t;						///< parametric distance along ray where this spot first has LOS to our path
/*!off=4(1)*/	HidingSpot *spot;				///< the spot to look at, or spot ID for save/load
};
typedef CUtlLinkedList <SpotOrder> SpotOrderList;

/**
 * This struct stores possible path segments thru a CNavArea, and the dangerous spots
 * to look at as we traverse that path segment.
 */
struct SpotEncounter
{
/*!off=0(0)*/	NavigationArea *from;
/*!off=4(1)*/	NavDirType_t fromDir;
/*!off=8(2)*/	NavigationArea *to;
/*!off=12(3)*/	NavDirType_t toDir;
	struct Ray_t
	{
/*!off=16(4)*/		Math::Vector3D from;
/*!off=28(7)*/		Math::Vector3D to;
	}             path;	///< the path segment
/*!off=40(10)*/	SpotOrderList spotList;						///< list of spots to look at, in order of occurrence
};
typedef CUtlLinkedList <SpotEncounter> SpotEncounterList;

class NavigationArea
{
	public:
		unsigned int m_id;											///< unique area ID (Offset = 0 (0))
		Math::Extent3D m_extent;									///< extents of area in world coords (NOTE: mins.z is not necessarily the minimum Z, but corresponds to Z at point (mins.x, mins.y), etc (Offset: {mins = 4 (1), maxs = 16 (4)})
		Math::Vector3D m_center;									///< centroid of area (Offset = 28 (7))
		NavAttributeFlag_t m_attributeFlags;						///< set of attribute bit flags (Offset = 40 (10))
		Place_t m_place;											///< place descriptor (Offset == 44 (11))

		/// height of the implicit corners
		float m_neZ;	///< (Offset = 48 (12))
		float m_swZ;	///< (Offset = 52 (13))

		//- for hunting -------------------------------------------------------------------------------------
		float m_clearedTimestamp[Constants::MaximumPlayableTeamsNumber];	///< time this area was last "cleared" of enemies (Offset == 56 (14))

		//- "danger" ----------------------------------------------------------------------------------------
		float m_danger[Constants::MaximumPlayableTeamsNumber];				///< danger of this area, allowing bots to avoid areas where they died in the past - zero is no danger (Offset == 64 (16))
		float m_dangerTimestamp[Constants::MaximumPlayableTeamsNumber];		///< time when danger value was set - used for decaying (Offset == 72 (18))

		//- hiding spots ------------------------------------------------------------------------------------
		HidingSpotList m_hidingSpotList;	///< (Offset == 80 (20))

		//- encounter spots ---------------------------------------------------------------------------------
		SpotEncounterList m_spotEncounterList;						///< list of possible ways to move thru this area, and the spots to look at as we do (Offset == 92 (23))

		//- approach areas ----------------------------------------------------------------------------------
		enum { MAX_APPROACH_AREAS = 16u };
		struct ApproachInfo_t
		{
/*!off=0(0)/104(26)*/			NavigationArea *here;									///< the approach area
/*!off=4(1)/108(27)*/			NavigationArea *prev;									///< the area just before the approach area on the path
/*!off=8(2)/112(28)*/			NavTraverseType_t prevToHereHow;
/*!off=12(3)/116(29)*/			NavigationArea *next;									///< the area just after the approach area on the path
/*!off=16(4)/120(30)*/			NavTraverseType_t hereToNextHow;
		};	// sizeof (ApproachInfo_t) == 20 (5)
		ApproachInfo_t m_approach[MAX_APPROACH_AREAS];	// (offset = 104 (26))
		unsigned char m_approachCount;	// (offset = 424 (106))

		//- A* pathfinding algorithm ------------------------------------------------------------------------
		unsigned int m_marker;										///< used to flag the area as visited (offset = 428 (107))
		NavigationArea *m_parent;									///< the area just prior to this on in the search path (offset = 432 (108))
		NavTraverseType_t m_parentHow;								///< how we get from parent to us (offset = 436 (109))
		float m_totalCost;											///< the distance so far plus an estimate of the distance left (offset = 440 (110))
		float m_costSoFar;											///< distance travelled so far (offset = 444 (111))

		NavigationArea *m_nextOpen, *m_prevOpen;					///< only valid if m_openMarker == m_masterMarker
		unsigned int m_openMarker;									///< if this equals the current marker value, we are on the open list (offset = 456 (114))

		//- connections to adjacent areas -------------------------------------------------------------------
		NavConnectList m_connect[NUM_DIRECTIONS];					///< a list of adjacent areas for each direction (offset = 460 (115))
		NavLadderConnectList m_ladder[NavigationLadder::Direction_Total];	///< list of ladders leading up and down from this area (offsets: {Direction_Up = 508 (127), Direction_Down = 520 (130)})

		//---------------------------------------------------------------------------------------------------
		NavigationNode *m_node[NUM_CORNERS];						///< navigation nodes at each corner of the area (offset: {NORTH_WEST = 532 (133), NORTH_EAST = 536 (134), SOUTH_EAST = 540 (135), SOUTH_WEST = 544 (136)})

		CUtlLinkedList<NavigationArea *> m_overlapList;		///< list of areas that overlap this area (offset = 548 (137))

		NavigationArea *m_prevHash, *m_nextHash;					///< for hash table in NavigationMesh (offsets: {m_prevHash = 560 (140), m_nextHash = 564 (141)})
};	// sizeof (NavigationArea) == 568 (142)

typedef CUtlLinkedList <NavigationArea *> NavAreaList;

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_NAVIGATION_AREA_INCLUDED