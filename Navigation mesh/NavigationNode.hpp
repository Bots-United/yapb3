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

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_NODE_INCLUDED
#define NAVIGATION_NODE_INCLUDED

class NavigationNode
{
	friend NavigationMesh;

	//
	// Group: Private members.
	//
	private:
		Math::Vector3D       m_pos;					///< position of this node in the world
		Math::Vector3D       m_normal;				///< surface normal at this location
		NavigationNode      *m_to[NUM_DIRECTIONS];	///< links to north, south, east, and west. NULL if no link
/*! FROM NEW SOURCE ENGINE		struct Link_t
		{
			NavigationNode *to;			///< links to north, south, east, and west. NULL if no link
			float           UNKNOWN1;
			float           UNKNOWN2;
			float           UNKNOWN3;
		} m_links[NUM_DIRECTIONS];*/
		unsigned int         m_id;					///< unique ID of this node
		NavAttributeFlag_t   m_attributeFlags;		///< set of attribute bit flags

		static unsigned int  m_nextID;
///*
		// below are only needed when generating
		DirectionBit_t       m_visited;				///< flags for automatic node generation. If direction bit is clear, that direction hasn't been explored yet.
		NavigationNode      *m_parent;				///< the node prior to this in the search, which we pop back to when this node's search is done (a stack)
		bool                 m_isCovered;			///< true when this node is "covered" by a NavigationArea
		NavigationArea      *m_area;				///< the area this node is contained within

		bool                 m_crouch[NUM_CORNERS];
//*/
	//
	// Group: (Con/De)structors.
	//
	public:
		NavigationNode (const Math::Vector3D &pos, const Math::Vector3D &normal/*/**/, NavigationNode *const parent = NULL/**/);

	//
	// Group: Private functions.
	//
	private:
		void                             CheckCrouch      (void);

	//
	// Group: Functions.
	//
	public:
		inline NavigationNode *const     GetConnectedNode (const NavDirType_t direction)                             const { return m_to[direction]; }				///< get navigation node connected in given direction, or NULL if cant go that way
		inline const Math::Vector3D     &GetPosition      (void)                                                     const { return m_pos; }
		inline const Math::Vector3D     &GetNormal        (void)                                                     const { return m_normal; }
		inline const unsigned int        GetID            (void)                                                     const { return m_id; }

		#if defined DEBUG_NAV_NODES
			void Draw (void);
		#endif	// if defined DEBUG_NAV_NODES

		inline void                      ConnectTo        (NavigationNode *const node, const NavDirType_t direction/*, float UNKNOWN1, float UNKNOWN2, float UNKNOWN3*/)				///< create a connection FROM this node TO the given node, in the given direction
		{
			m_to[direction] = node;
/*! FROM NEW SOURCE ENGINE
			m_links[direction].to       = node;
			m_links[direction].UNKNOWN1 = UNKNOWN1;
			m_links[direction].UNKNOWN2 = UNKNOWN2;
			m_links[direction].UNKNOWN3 = UNKNOWN3;
*/
		}

		inline void                      SetAttributes    (const NavAttributeFlag_t bits)                                  { m_attributeFlags = bits; }
		inline void                      AddAttributes    (const NavAttributeFlag_t bits)                                  { m_attributeFlags |= bits; }
		inline void                      RemoveAttributes (const NavAttributeFlag_t bits)                                  { m_attributeFlags &= ~bits; }
		inline const NavAttributeFlag_t  GetAttributes    (void)                                                     const { return m_attributeFlags; }
		inline const bool                HasAttributes    (const NavAttributeFlag_t bits)                            const { return (m_attributeFlags & bits) > 0u; }
///*
		inline NavigationNode *const     GetParent        (void)                                                     const { return m_parent; }

		inline void                      MarkAsVisited    (const NavDirType_t direction)                                   { m_visited |= BIT (direction); }						///< mark the given direction as having been visited
		inline void                      MarkAsVisited    (const DirectionBit_t directionBits)                             { m_visited |= directionBits; }						///< mark the given direction as having been visited
		inline const bool                HasVisited       (const NavDirType_t direction)                             const { return (m_visited & BIT (direction)) > 0u; }			///< return TRUE if the given direction has already been searched
		inline const bool                HasVisited       (const DirectionBit_t directionBits)                       const { return (m_visited & directionBits) > 0u; }			///< return TRUE if the given direction has already been searched

		inline const bool                IsBiLinked       (const NavDirType_t direction)                             const	///< Return true if this node is bidirectionally linked to another node in the given direction
		{
			return m_to[direction] != NULL && m_to[direction]->m_to[GetOppositeDirection (direction)] == this;
		}
		inline const bool                IsClosedCell     (void)                                                     const	///< Return true if this node is the NW corner of a quad of nodes that are all bidirectionally linked.
		{
			return IsBiLinked (SOUTH) && IsBiLinked (EAST) && m_to[EAST]->IsBiLinked (SOUTH) && m_to[SOUTH]->IsBiLinked (EAST) && m_to[EAST]->m_to[SOUTH] == m_to[SOUTH]->m_to[EAST];
		}

		inline void                      Cover            (void)                                                           { m_isCovered = true; }			///< @todo Should pass in area that is covering
		inline const bool                IsCovered        (void)                                                     const { return m_isCovered; }			///< return true if this node has been covered by an area

		inline void                      AssignArea       (NavigationArea *const area)                                     { m_area = area; }								///< assign the given area to this node
		inline NavigationArea *const     GetArea          (void)                                                     const { return m_area; }								///< return associated area
#if 0
/*! FROM NEW SOURCE ENGINE !*/
		inline const bool CheckObstacles (const unsigned char width, const unsigned char height, const unsigned char x, const unsigned char y) const
		{
			if (width <= 1u && height <= 1u)
				return true;

			const float offset (18.0f);

			return (x == 0u || m_links[WEST].UNKNOWN1 <= offset) && (y == 0u || m_links[NORTH].UNKNOWN1 <= offset) && (x >= width - 1u || m_links[EAST].UNKNOWN1 <= offset) && (y >= height - 1u || m_links[SOUTH].UNKNOWN1 <= offset);
		}
#endif	// if 0
//*/
};

typedef Stack <NavigationNode *, unsigned int> NavNodeStack;

//typedef DynamicArray <NavigationNode *, unsigned char> NavigationNodesGridArray;
typedef StaticArray <NavigationNode *, unsigned char, MATH_GET_CUBIC (MaximumNavigationNodesPerCell)> NavigationNodesGridArray;

#endif	// ifndef NAVIGATION_NODE_INCLUDED