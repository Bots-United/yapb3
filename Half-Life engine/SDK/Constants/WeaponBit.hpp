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
// WeaponBit.hpp
//
// Description: Weapon bit masks.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef WEAPON_BIT_INCLUDED
#define WEAPON_BIT_INCLUDED

DECLARE_TYPED_ENUMERATION (unsigned int, WeaponBit_t)
{
	WeaponBit_None             = WeaponID_None,						// None

	WeaponBit_P228             = BIT (WeaponID_P228),				// SIG P228
	WeaponBit_Shield           = BIT (WeaponID_Shield),				// Tactical Shield
	WeaponBit_Scout            = BIT (WeaponID_Scout),				// Steyr Scout
	WeaponBit_ExplosiveGrenade = BIT (WeaponID_ExplosiveGrenade),	// High-Explosive Grenade
	WeaponBit_XM1014           = BIT (WeaponID_XM1014),				// Benelli XM1014
	WeaponBit_C4               = BIT (WeaponID_C4),					// Bomb
	WeaponBit_MAC10            = BIT (WeaponID_MAC10),				// Ingram MAC-10
	WeaponBit_Aug              = BIT (WeaponID_Aug),				// Steyr Aug
	WeaponBit_SmokeGrenade     = BIT (WeaponID_SmokeGrenade),		// Smoke Grenade
	WeaponBit_Elite            = BIT (WeaponID_Elite),				// Dual Beretta 96G Elite
	WeaponBit_FiveSeven        = BIT (WeaponID_FiveSeven),			// FN Five-Seven
	WeaponBit_UMP45            = BIT (WeaponID_UMP45),				// HK UMP45
	WeaponBit_SG550            = BIT (WeaponID_SG550),				// Sig SG-550 Sniper
	WeaponBit_Galil            = BIT (WeaponID_Galil),				// IMI Galil
	WeaponBit_Famas            = BIT (WeaponID_Famas),				// GIAT FAMAS
	WeaponBit_USP              = BIT (WeaponID_USP),				// HK USP .45 Tactical
	WeaponBit_Glock18          = BIT (WeaponID_Glock18),			// Glock18 Select Fire
	WeaponBit_AWP              = BIT (WeaponID_AWP),				// AI Arctic Warfare/Magnum
	WeaponBit_MP5              = BIT (WeaponID_MP5),				// HK MP5-Navy
	WeaponBit_M249             = BIT (WeaponID_M249),				// FN M249 Para
	WeaponBit_M3               = BIT (WeaponID_M3),					// Benelli M3 Super90
	WeaponBit_M4A1             = BIT (WeaponID_M4A1),				// Colt M4A1 Carbine
	WeaponBit_TMP              = BIT (WeaponID_TMP),				// Steyr Tactical Machine Pistol
	WeaponBit_G3SG1            = BIT (WeaponID_G3SG1),				// HK G3/SG-1 Sniper Rifle
	WeaponBit_FlashBangGrenade = BIT (WeaponID_FlashBangGrenade),	// Concussion Grenade
	WeaponBit_Deagle           = BIT (WeaponID_Deagle),				// Desert Eagle .50AE
	WeaponBit_SG552            = BIT (WeaponID_SG552),				// Sig SG-552 Commando
	WeaponBit_AK47             = BIT (WeaponID_AK47),				// Automat Kalashnikov AK-47
	WeaponBit_Knife            = BIT (WeaponID_Knife),				// Knife
	WeaponBit_P90              = BIT (WeaponID_P90),				// FN P90

	WeaponBit_Armor            = BIT (WeaponID_Armor)				// Kevlar Vest (suit)
};

#endif	// ifndef WEAPON_BIT_INCLUDED