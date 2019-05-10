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
// AStarNode.hpp
//
// Class: AStarNode
//
// Description: This class is used to encapsulate all the data relative to a A* node.
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef A_STAR_NODE_INCLUDED
#define A_STAR_NODE_INCLUDED

template <typename elementType> class AStarNode
{
	//
	// Group: Constants.
	//
	public:
		DECLARE_TYPED_ENUMERATION (unsigned char, Flag_t)
		{
			Flag_None,

			Flag_Closed       = BIT (0u),
			Flag_Open         = BIT (1u),
			Flag_HeuristicSet = BIT (2u)
		};

	//
	// Group: Private members.
	//
	private:
		float              m_cost;		// Equivalent of the g in algorithms given.
		float              m_heuristic;	// Equivalent of the h in algorithms given.
		Flag_t             m_flags;		// Node flags from 'Flag_t' enumeration.
		const elementType *m_element;

		const AStarNode   *m_parent;	// The node parent.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline AStarNode (void) :
			m_cost (0.0f),
			m_heuristic (0.0f),
			m_flags (Flag_None),
			m_element (NULL),

			m_parent (NULL)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline void                     Close         (void)                                   { SetFlag (Flag_Closed); }
		inline void                     UnClose       (void)                                   { RemoveFlag (Flag_Closed); }
		inline const bool               IsClosed      (void)                             const { return HasFlag (Flag_Closed); }
		inline const bool               HasParent     (void)                             const { return m_parent != NULL; }
		inline const bool               IsOpen        (void)                             const { return HasFlag (Flag_Open); }
		inline void                     UnOpen        (void)                                   { RemoveFlag (Flag_Open); }
		inline void                     Open          (void)                                   { SetFlag (Flag_Open); }

		inline void                     SetHeuristic  (const float heuristic)                  { m_heuristic = heuristic; SetFlag (Flag_HeuristicSet); }
		inline const bool               HeuristicSet  (void)                                   { return HasFlag (Flag_HeuristicSet); }
		inline const float              GetHeuristic  (void)                             const { return m_heuristic; }

		inline const bool               HasFlag       (const Flag_t flags)               const { return (m_flags & flags) == flags; }
		inline void                     SetFlag       (const Flag_t flags)                     { m_flags |= flags; }
		inline void                     RemoveFlag    (const Flag_t flags)                     { m_flags &= ~flags; }
		inline const float              GetCost       (void)                             const { return m_cost; }
		inline void                     SetCost       (const float cost)                       { m_cost = cost; }

		// for comparison
		inline const bool               HasBetterCost (const AStarNode *const other)     const { return m_cost + m_heuristic < other->m_cost + other->m_heuristic; }
		inline void                     SetElement    (const elementType *const element)       { m_element = element; }
		inline const elementType *const GetElement    (void)                             const { return m_element; }

		inline const AStarNode *const   GetParent     (void)                             const { return m_parent; }
		inline void                     SetParent     (const AStarNode *const parent)          { m_parent = parent; }
};

// Insertion sorted list
template <typename elementType> class AStarOpenList : public PriorityStack <AStarNode <elementType> *>
{
	//
	// Group: Private functions.
	//
	private:
		static inline const bool HasBetterCost (const AStarNode <elementType> *const &node1, const AStarNode <elementType> *const &node2)
		{
			// Nodes with greater cost are lower priority.
			return node1->HasBetterCost (node2);
		}

	//
	// Group: (Con/De)structors.
	//
	public:
		inline AStarOpenList (void) : PriorityStack (HasBetterCost) { /* VOID */ }
};

#endif	// ifndef A_STAR_NODE_INCLUDED