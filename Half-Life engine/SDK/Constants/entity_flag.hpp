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
// entity_flag.hpp
//
// Description: Edict->flags flags.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_FLAG_INCLUDED
#define ENTITY_FLAG_INCLUDED

DECLARE_TYPED_ENUMERATION (/*unsigned */int, EntityFlag_t)
{
	EntityFlag_None,

	FL_FLY           = BIT (0u),	// changes the SV_Movestep() behavior to not need to be on ground
	FL_SWIM          = BIT (1u),	// changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
	FL_CONVEYOR      = BIT (2u),	// entity is conveyor
	FL_CLIENT        = BIT (3u),	// entity is client
	FL_INWATER       = BIT (4u),	// entity is in water
	FL_MONSTER       = BIT (5u),	// entity is monster
	FL_GODMODE       = BIT (6u),	// godmode enabled for this entity
	FL_NOTARGET      = BIT (7u),	// AI disabled for this entity
	FL_SKIPLOCALHOST = BIT (8u),	// don't send entity to local host, it's predicting this entity itself
	FL_ONGROUND      = BIT (9u),	// at rest/on the ground
	FL_PARTIALGROUND = BIT (10u),	// not all corners are valid
	FL_WATERJUMP     = BIT (11u),	// player jumping out of water
	FL_FROZEN        = BIT (12u),	// player is frozen for 3rd person camera
	FL_FAKECLIENT    = BIT (13u),	// JAC: fake client, simulated server side; don't send network messages to them
	FL_DUCKING       = BIT (14u),	// player flag -- player is fully crouched
	FL_FLOAT         = BIT (15u),	// apply floating force to this entity when in water
	FL_GRAPHED       = BIT (16u),	// worldgraph has this ent listed as something that blocks a connection

	// UNDONE: do we need these?
	FL_IMMUNE_WATER  = BIT (17u),
	FL_IMMUNE_SLIME  = BIT (18u),
	FL_IMMUNE_LAVA   = BIT (19u),

	FL_PROXY         = BIT (20u),	// this is a spectator proxy
	FL_ALWAYSTHINK   = BIT (21u),	// brush model flag -- call think every frame regardless of 'nextThinkTime' - 'localTime' (for constantly changing velocity/path)
	FL_BASEVELOCITY  = BIT (22u),	// base velocity has been applied this frame (used to convert base velocity into momentum)
	FL_MONSTERCLIP   = BIT (23u),	// only collide in with monsters who have 'FL_MONSTERCLIP' set
	FL_ONTRAIN       = BIT (24u),	// player is _controlling_ a train, so movement commands should be ignored on client during prediction.
	FL_WORLDBRUSH    = BIT (25u),	// not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
	FL_SPECTATOR     = BIT (26u),	// this client is a spectator, don't run touch functions, etc.

	FL_CUSTOMENTITY  = BIT (29u),	// this is a custom entity
	FL_KILLME        = BIT (30u),	// this entity is marked for death -- this allows the engine to kill entities at the appropriate time
	FL_DORMANT       = BIT (31u)	// entity is dormant, no updates to client
};

#endif	// ifndef ENTITY_FLAG_INCLUDED