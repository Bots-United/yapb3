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
// Link.hpp
//
// Structure: Link_t
//
// Description: Used to get edict from current area.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_LINK_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_LINK_INCLUDED

struct Link_t
{
	// previous and next elements
/*! off=0(0) */	Link_t *prev;
/*! off=4(1) */	Link_t *next;

	inline const bool IsHead (void) const { return prev == NULL; }
	inline const bool IsTail (void) const { return next == NULL; }

	inline void Clear (void)
	{
		// Is used for new headnodes

		prev = next = this;
	}

	inline void Remove (void)
	{
		// remove link from chain

		next->prev = prev;
		prev->next = next;
	}

	inline void InsertBefore (Link_t *const before)
	{
		// kept trigger and solid entities seperate

		next = before;
		prev = before->prev;

		next->prev = this;
		prev->next = this;
	}
	inline void InsertAfter (Link_t *const after)
	{
		next = after->next;
		prev = after;

		next->prev = this;
		prev->next = this;
	}
};	// sizeof (Link_t) == 8 (2)

// (type *)GET_STRUCTURE_FROM_LINK(Link_t *link, elementType, elementMember)
// ent = GET_STRUCTURE_FROM_LINK(link, entity_t, order)
// FIXME: remove this mess!
#define	GET_STRUCTURE_FROM_LINK(link, elementType, elementMember) reinterpret_cast <elementType *> (reinterpret_cast <const unsigned int> (link) - GET_OFFSET_OF_MEMBER_VARIABLE (elementType, elementMember))

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_LINK_INCLUDED