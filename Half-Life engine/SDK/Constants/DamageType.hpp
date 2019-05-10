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
// DamageType.hpp
//
// Description: Instant damage values for use with "DamageMsg" 3rd value write_long(BIT).
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef DAMAGE_TYPE_INCLUDED
#define DAMAGE_TYPE_INCLUDED

DECLARE_TYPED_ENUMERATION (/*unsigned */int, DamageType_t)
{
	DamageType_None         = -1,			// Manually added.

	DamageType_Generic,						// Generic damage was done.

	DamageType_Crush,						// Crushed by falling or moving object.
	DamageType_Bullet,						// Shot.
	DamageType_Slash        = BIT (2u),		// Cut, clawed, stabbed.
	DamageType_Burn         = BIT (3u),		// Heat burned.
	DamageType_Freeze       = BIT (4u),		// Frozen.
	DamageType_Fall         = BIT (5u),		// Fell too far.
	DamageType_Blast        = BIT (6u),		// Explosive blast damage.
	DamageType_Club         = BIT (7u),		// Crowbar, punch, headbutt.
	DamageType_Shock        = BIT (8u),		// Electric shock.
	DamageType_Sonic        = BIT (9u),		// Sound pulse shockwave.
	DamageType_EnergyBeam   = BIT (10u),	// Laser or other high energy beam.

	DamageType_NeverGib     = BIT (12u),	// With this bit OR'd in, no damage type will be able to gib victims upon death.
	DamageType_AlwaysGib    = BIT (13u),	// With this bit OR'd in, any damage type can be made to gib victims upon death.
	DamageType_Drown        = BIT (14u),	// Drowning.
	DamageType_Paralyze     = BIT (15u),	// Slows affected creature down.
	DamageType_Nervegas     = BIT (16u),	// Nerve toxins, very bad.
	DamageType_Poison       = BIT (17u),	// Blood poisioning.
	DamageType_Radiation    = BIT (18u),	// Radiation exposure.
	DamageType_DrownRecover = BIT (19u),	// Drowning recovery.
	DamageType_Acid         = BIT (20u),	// Toxic chemicals or acid burns.
	DamageType_SlowBurn     = BIT (21u),	// In an oven.
	DamageType_SlowFreeze   = BIT (22u),	// In a subzero freezer.
	DamageType_Mortar       = BIT (23u),	// Hit by air raid (done to distinguish grenade from mortar).
	DamageType_HEGrenade    = BIT (24u),	// Explosive grenade.

	// these are the damage types that are allowed to gib corpses
	DamageType_GibCorpse    = DamageType_Crush | DamageType_Fall | DamageType_Blast | DamageType_Club | DamageType_Sonic,

	// these are the damage types that have client hud art
	DamageType_ShownHUD     = DamageType_Burn | DamageType_Freeze | DamageType_Shock | DamageType_Drown | DamageType_Nervegas | DamageType_Poison | DamageType_Radiation | DamageType_Acid | DamageType_SlowBurn | DamageType_SlowFreeze,

	DamageType_TimeBased    = ~0x3FFF		// Mask for time-based damage.
};

#endif	// ifndef DAMAGE_TYPE_INCLUDED