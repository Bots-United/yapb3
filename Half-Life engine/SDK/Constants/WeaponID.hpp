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
// WeaponID.hpp
//
// Description: Counter-Strike weapon ID's.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef WEAPON_ID_INCLUDED
#define WEAPON_ID_INCLUDED

enum WeaponID_t
{
	// weapon terminator
	WeaponID_None,

	WeaponID_P228,					// SIG P228
	WeaponID_Shield,				// Tactical Shield
	WeaponID_Scout,					// Steyr Scout
	WeaponID_ExplosiveGrenade,		// High-Explosive Grenade
	WeaponID_XM1014,				// Benelli XM1014
	WeaponID_C4,					// Bomb
	WeaponID_MAC10,					// Ingram MAC-10
	WeaponID_Aug,					// Steyr Aug
	WeaponID_SmokeGrenade,			// Smoke Grenade
	WeaponID_Elite,					// Dual Beretta 96G Elite
	WeaponID_FiveSeven,				// FN Five-Seven
	WeaponID_UMP45,					// HK UMP45
	WeaponID_SG550,					// Sig SG-550 Sniper
	WeaponID_Galil,					// IMI Galil
	WeaponID_Famas,					// GIAT FAMAS
	WeaponID_USP,					// HK USP .45 Tactical
	WeaponID_Glock18,				// Glock18 Select Fire
	WeaponID_AWP,					// AI Arctic Warfare/Magnum
	WeaponID_MP5,					// HK MP5-Navy
	WeaponID_M249,					// FN M249 Para
	WeaponID_M3,					// Benelli M3 Super90
	WeaponID_M4A1,					// Colt M4A1 Carbine
	WeaponID_TMP,					// Steyr Tactical Machine Pistol
	WeaponID_G3SG1,					// HK G3/SG-1 Sniper Rifle
	WeaponID_FlashBangGrenade,		// Concussion Grenade
	WeaponID_Deagle,				// Desert Eagle .50AE
	WeaponID_SG552,					// Sig SG-552 Commando
	WeaponID_AK47,					// Automat Kalashnikov AK-47
	WeaponID_Knife,					// Knife
	WeaponID_P90,					// FN P90

	WeaponID_Armor,					// Kevlar Vest (suit)
	WeaponID_ArmorHelm				// Kevlar Vest and Helmet (assault suit)
};

enum WeponClass_t
{
	WeponClass_None,

	WeponClass_Knife,
	WeponClass_Pistol,
	WeponClass_Grenade,
	WeponClass_SubMachineGun,
	WeponClass_Shotgun,
	WeponClass_MachineGun,
	WeponClass_Rifle,
	WeponClass_SniperRifle,
//NOT EXISTS???	WeponClass_C4,
//NOT EXISTS???	WeponClass_Shield
};

#endif	// ifndef WEAPON_ID_INCLUDED