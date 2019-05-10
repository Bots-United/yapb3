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
// WeaponBits.hpp
//
// Description: Primary/Secondary weapon bit masks.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef WEAPON_BITS_INCLUDED
#define WEAPON_BITS_INCLUDED

enum WeaponBits_t
{
	WeaponBits_Pistols        = WeaponBit_Glock18 | WeaponBit_USP | WeaponBit_P228 | WeaponBit_Deagle | WeaponBit_Elite | WeaponBit_FiveSeven,
	WeaponBits_Shotguns       = WeaponBit_M3 | WeaponBit_XM1014,
	WeaponBits_SubMachineGuns = WeaponBit_MAC10 | WeaponBit_TMP | WeaponBit_MP5 | WeaponBit_UMP45 | WeaponBit_P90,
	WeaponBits_ZoomableRifles = WeaponBit_SG552 | WeaponBit_Aug,
	WeaponBits_Rifles         = WeaponBit_Galil | WeaponBit_Famas | WeaponBit_AK47 | WeaponBit_M4A1 | WeaponBits_ZoomableRifles,
	WeaponBits_SniperRifles   = WeaponBit_Scout | WeaponBit_AWP | WeaponBit_SG550 | WeaponBit_G3SG1,
	WeaponBits_MachineGun     = WeaponBit_M249,
	WeaponBits_Grenades       = WeaponBit_ExplosiveGrenade | WeaponBit_SmokeGrenade | WeaponBit_FlashBangGrenade,

	WeaponBits_Primary        = WeaponBits_Shotguns | WeaponBits_SubMachineGuns | WeaponBits_Rifles | WeaponBits_SniperRifles | WeaponBits_MachineGun,
	WeaponBits_Secondary      = WeaponBits_Pistols
};

#endif	// ifndef WEAPON_BITS_INCLUDED