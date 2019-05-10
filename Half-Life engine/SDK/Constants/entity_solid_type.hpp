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
// entity_solid_type.hpp
//
// Description: Edict->solid values.
//
// NOTE: Some movetypes will cause collisions independent of 'SOLID_NOT'/'SOLID_TRIGGER' when the entity moves
//	SOLID only effects OTHER entities colliding with this one when they move - UGH!
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_SOLID_TYPE_INCLUDED
#define ENTITY_SOLID_TYPE_INCLUDED

enum EntitySolidType_t
{
	SOLID_NOT,			// no interaction with other objects (no solid model) (don't touchable) (No clipping, No touching, No hitboxes)
	SOLID_TRIGGER,		// touch on edge, but not blocking (No clipping, Touching, No hitboxes)
	SOLID_BoundingBox,	// touch on edge, block (Clipping, Touching, Hitboxes)
	SOLID_SLIDEBOX,		// touch on edge, but not an onground (Clipping, Touching, Hitboxes, Friction when moving, Players and monsters use this)
	SOLID_BSP			// bsp clip, touch on edge, block (No Clipping, Touching, Hitboxes)
};

#endif	// ifndef ENTITY_SOLID_TYPE_INCLUDED