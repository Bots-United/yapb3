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
// ArmouryItemType.hpp
//
// Enumeration: ArmouryItemType_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_ARMOURY_ITEM_TYPE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_ARMOURY_ITEM_TYPE_INCLUDED

enum ArmouryItemType_t
{
	ArmouryItemType_MP5,													// HK MP5-Navy
	ArmouryItemType_TMP,													// Steyr Tactical Machine Pistol
	ArmouryItemType_P90,													// FN P90
	ArmouryItemType_MAC10,													// Ingram MAC-10
	ArmouryItemType_AK47,													// Automat Kalashnikov AK-47
	ArmouryItemType_SG552,													// Sig SG-552 Commando
	ArmouryItemType_M4A1,													// Colt M4A1 Carbine
	ArmouryItemType_Aug,													// Steyr Aug
	ArmouryItemType_Scout,													// Steyr Scout
	ArmouryItemType_G3SG1,													// HK G3/SG-1 Sniper Rifle
	ArmouryItemType_AWP,													// AI Arctic Warfare/Magnum
	ArmouryItemType_M3,														// Benelli M3 Super90
	ArmouryItemType_XM1014,													// Benelli XM1014
	ArmouryItemType_M249,													// FN M249 Para

	ArmouryItemType_PrimaryWeaponsTotal,

	ArmouryItemType_FlashBangGrenade = ArmouryItemType_PrimaryWeaponsTotal,	// Concussion Grenade
	ArmouryItemType_ExplosiveGrenade,										// High-Explosive Grenade
	ArmouryItemType_Armor,													// Kevlar Vest
	ArmouryItemType_ArmorHelm,												// Kevlar Vest and Helmet
	ArmouryItemType_SmokeGrenade,											// Smoke Grenade

	ArmouryItemType_Total
};

const WeaponID_t armouryItemTypeToWeaponID[ArmouryItemType_Total] =
{
	WeaponID_MP5,				// HK MP5-Navy
	WeaponID_TMP,				// Steyr Tactical Machine Pistol
	WeaponID_P90,				// FN P90
	WeaponID_MAC10,				// Ingram MAC-10
	WeaponID_AK47,				// Automat Kalashnikov AK-47
	WeaponID_SG552,				// Sig SG-552 Commando
	WeaponID_M4A1,				// Colt M4A1 Carbine
	WeaponID_Aug,				// Steyr Aug
	WeaponID_Scout,				// Steyr Scout
	WeaponID_G3SG1,				// HK G3/SG-1 Sniper Rifle
	WeaponID_AWP,				// AI Arctic Warfare/Magnum
	WeaponID_M3,				// Benelli M3 Super90
	WeaponID_XM1014,			// Benelli XM1014
	WeaponID_M249,				// FN M249 Para

	WeaponID_FlashBangGrenade,	// Concussion Grenade
	WeaponID_ExplosiveGrenade,	// High-Explosive Grenade
	WeaponID_Armor,				// Kevlar Vest
	WeaponID_ArmorHelm,			// Kevlar Vest and Helmet
	WeaponID_SmokeGrenade		// Smoke Grenade
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_ARMOURY_ITEM_TYPE_INCLUDED