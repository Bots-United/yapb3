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
// entity_move_mype.hpp
//
// Description: Edict->moveType values.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_MOVE_MYPE_INCLUDED
#define ENTITY_MOVE_MYPE_INCLUDED

enum EntityMoveType_t
{
	MOVETYPE_NONE,			// never moves

	//! @note Not used!
	MOVETYPE_ANGLENOCLIP,
	MOVETYPE_ANGLECLIP,

	MOVETYPE_WALK,			// gravity, player only - moving on the ground
	MOVETYPE_STEP,			// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY,			// no gravity, but still collides with stuff
	MOVETYPE_TOSS,			// gravity/collisions
	MOVETYPE_PUSH,			// no clip to world, push and crush
	MOVETYPE_NOCLIP,		// no gravity, no collisions, still do velocity/angleVelocity (no clip)
	MOVETYPE_FLYMISSILE,	// extra size to monsters
	MOVETYPE_BOUNCE,		// just like toss, but reflect velocity when contacting surfaces
	MOVETYPE_BOUNCEMISSILE,	// bounce w/o gravity
	MOVETYPE_FOLLOW,		// track movement of 'aimEntity'
	MOVETYPE_PUSHSTEP		// BSP model that needs physics/world collisions (uses nearest hull for world collision)
};

#endif	// ifndef ENTITY_MOVE_MYPE_INCLUDED