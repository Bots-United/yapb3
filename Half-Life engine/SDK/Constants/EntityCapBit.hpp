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
// EntityCapBit.hpp
//
// Description: These are caps bits to indicate what an object's capabilities (currently used for save/restore and level transitions).
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_ENTITY_CAP_BIT_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_ENTITY_CAP_BIT_INCLUDED

enum CapBit_t
{
	CapBit_None,

	CapBit_CUSTOMSAVE        = 0x00000001,
	CapBit_ACROSS_TRANSITION = 0x00000002,	// should transfer between transitions
	CapBit_MUST_SPAWN        = 0x00000004,	// Spawn after restore
	CapBit_DONT_SAVE         = 0x80000000,	// Don't save this
	CapBit_IMPULSE_USE       = 0x00000008,	// can be used by the player
	CapBit_CONTINUOUS_USE    = 0x00000010,	// can be used by the player
	CapBit_ONOFF_USE         = 0x00000020,	// can be used by the player
	CapBit_DIRECTIONAL_USE   = 0x00000040,	// Player sends ±1 when using (currently only tracktrains)
	CapBit_MASTER            = 0x00000080,	// Can be used to "master" other entities (like multisource)

	// UNDONE: This will ignore transition volumes (trigger_transition), but not the PVS!!!
	CapBit_FORCE_TRANSITION  = 0x00000080		// ALWAYS goes across transitions
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_ENTITY_CAP_BIT_INCLUDED