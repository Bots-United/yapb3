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
// entity_private_data_offset_flag.hpp
//
// Description: Offset flags.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_PRIVATE_DATA_OFFSET_FLAG_INCLUDED
#define ENTITY_PRIVATE_DATA_OFFSET_FLAG_INCLUDED

// for 'Offset_Weapon_State'
DECLARE_TYPED_ENUMERATION (unsigned int, WeaponState_t)
{
	WeaponState_USP_UNSILENCED       = 0u,
	WeaponState_USP_SILENCED         = BIT (0u),

	WeaponState_GLOCK_SEMI_AUTOMATIC = 0u,
	WeaponState_GLOCK18_BURST_MODE   = BIT (1u),

	WeaponState_M4A1_UNSILENCED      = 0u,
	WeaponState_M4A1_SILENCED        = BIT (2u),

	WeaponState_ELITE_RIGHT          = 0u,
	WeaponState_ELITE_LEFT           = BIT (3u),

	WeaponState_FAMAS_AUTOMATIC      = 0u,
	WeaponState_FAMAS_BURST_MODE     = BIT (4u),

	WeaponState_SHIELD_DRAWN         = BIT (5u)
};

enum EntityPrivateDataOffsetFlag_t
{
	// for entities with "weapon_*" class name....

	

	// for entities with "player" class name....
};

#endif	// ifndef ENTITY_PRIVATE_DATA_OFFSET_FLAG_INCLUDED