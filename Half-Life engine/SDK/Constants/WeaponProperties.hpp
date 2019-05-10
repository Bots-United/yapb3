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
// WeaponProperties.hpp
//
// Structure: WeaponProperty_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_WEAPON_PROPERTY_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_WEAPON_PROPERTY_INCLUDED

// weapon properties structure
const struct WeaponProperty_t
{
	DynamicString                    className;				// the weapon entity class name, in all letters
	DynamicString                    modelName;				// model name to separate cs weapons

	unsigned char                    ammoID;				// ammo index
	DynamicString                    ammoName;				// ammo name
	DynamicString                    ammoClassName;			// ammo class name
	DynamicString                    ammoModelName;			// ammo model name
	unsigned char                    ammoMaximumClip;		// maximum ammo amount in clip
	unsigned char                    ammoMaximumAmount;		// maximum ammo amount
	unsigned char                    ammoPrice;				// ammo price

	WeaponSlotID_t                   slotID;				// weapon slot id
	bool                             canUseUnderwater;		// can use this weapon underwater
	WeaponBulletType_t               bulletType;			// weapon bullet type
	WeaponObstaclePenetrationPower_t penetrationPower1;		// can shoot thru walls
	WeaponObstaclePenetrationPower_t penetrationPower2;		// can shoot thru walls
	unsigned short                   maximumShootDistance1;	// maximum shoot distance 1
	unsigned short                   maximumShootDistance2;	// maximum shoot distance 2

	float                            edgePointOrigin;		// weapon edge point origin (This is used to determine the weapon head point)

	// Information: damage/obstacle pierce for usp/m4a1 - unsilenced/silenced, famas - semi automatic/burst.
	unsigned char                    damage1;				// damage 1
	unsigned char                    damage2;				// damage 2

	float                            obstaclePierce1;		// obstacle pierce 1
	float                            obstaclePierce2;		// obstacle pierce 2

	float                            reloadTime;			// Reload time.

	unsigned short                   speed;					// player speed with this weapon
	bool                             primaryFireHold;		// hold down primary fire button to use?

	unsigned short                   price;					// weapon price
	DynamicString                    buyAlias1;				// buy alias 1
	DynamicString                    buyAlias2;				// buy alias 2

//	char                             teamStandard;			// used by team (number) (standard map)
//	char                             teamAS;				// used by team (AS map)
} weaponProperties[MaximumWeapons - 1u] =	// weapons and their specifications
{
	// Terminator, since weapon id's are starting from 1 in Half-Life engine.
	{"",                    "",                           0u, "",             "",                "",                             0u,   0u,   0u, WeaponSlotID_None,             false, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,          0u,          0u,  0.0f,   0u,  0u,  0.0f,    0.0f, 0.0f,  250u, false,    0u, "",      "",             /*-1, -1*/},	// WeaponID_None

	{"weapon_p228",         "models/w_p228.mdl",          9u, "357SIG",       "ammo_357sig",     "models/w_357ammobox.mdl",     13u,  52u,  60u, WeaponSlotID_SecondaryWeapons, false, WeaponBulletType_15, WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0, HalfMapSize, HalfMapSize, /*35.5*/32.8f,  32u,  0u,  0.8f,    0.0f, 2.7f,  250u, false,  600u, "p228",  "228compact",   /* TeamID_CounterTerrorist,  TeamID_CounterTerrorist*/},	// WeaponID_P228
	{"weapon_shield",       "models/w_shield.mdl",        0u, "",             "",                "",                             0u,   0u,   0u, WeaponSlotID_SecondaryWeapons,  true, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,          0u,          0u,  0.0f,   0u,  0u,  0.0f,    0.0f, 0.0f,  250u,  true, 2200u, "shield", "",            /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_Shield
	{"weapon_scout",        "models/w_scout.mdl",         2u, "762Nato",      "ammo_762nato",    "models/w_crossbow_clip.mdl",  10u,  90u,  80u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_11, WeaponObstaclePenetrationPower_3, WeaponObstaclePenetrationPower_2,     MapSize,     MapSize, /*42.0*/38.9f,  75u,  0u,  0.98f,   0.0f, 2.0f,  260u, false, 2750u, "scout",  "",            /* TeamID_CounterTerrorist,  TeamID_Unassigned*/},	// WeaponID_Scout
	{"weapon_hegrenade",    "models/w_hegrenade.mdl",    12u, "HEGrenade",    "",                "models/w_hegrenade.mdl",       0u,   1u,   0u, WeaponSlotID_Grenades,         false, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,          0u,          0u,  0.0f,  97u,  0u,  0.0f,    0.0f, 0.0f,  250u, false,  300u, "hegren", "",            /*-1, -1*/},	// WeaponID_ExplosiveGrenade
	{"weapon_xm1014",       "models/w_xm1014.mdl",        5u, "buckshot",     "ammo_buckshot",   "models/w_shotbox.mdl",         7u,  32u,  65u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,          0u,          0u, /*35.5*/31.2f, 115u,  0u,  0.0f,    0.0f, 0.55f, 240u, false, 3000u, "xm1014", "autoshotgun", /* TeamID_CounterTerrorist, -1*/},	// WeaponID_XM1014
	{"weapon_c4",           "models/w_c4.mdl",           14u, "C4",           "",                "models/w_c4.mdl",              0u,   1u,   0u, WeaponSlotID_C4,               false, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,          0u,          0u,  0.0f,   0u,  0u,  0.0f,    0.0f, 0.0f,  250u, false,    0u, "",       "",            /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_C4
	{"weapon_mac10",        "models/w_mac10.mdl",         6u, "45acp",        "ammo_45acp",      "models/w_9mmclip.mdl",        30u, 100u,  25u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_9,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,     MapSize,     MapSize, /*37.0*/26.0f,  29u,  0u,  0.82f,   0.0f, 3.15f, 250u,  true, 1400u, "mac10",  "",            /* TeamID_Unassigned,  TeamID_Unassigned*/},	// WeaponID_MAC10
	{"weapon_aug",          "models/w_aug.mdl",           4u, "556Nato",      "ammo_556nato",    "models/w_9mmarclip.mdl",      30u,  90u,  60u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_12, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*37.0*/32.9f,  32u,  0u,  0.96f,   0.0f, 3.3f,  240u,  true, 3500u, "aug",    "bullpup",     /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_Aug
	{"weapon_smokegrenade", "models/w_smokegrenade.mdl", 13u, "SmokeGrenade", "",                "models/w_smokegrenade.mdl",    0u,   1u,   0u, WeaponSlotID_Grenades,         false, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,          0u,          0u,  0.0f,   0u,  0u,  0.0f,    0.0f, 0.0f,  250u, false,  300u, "sgren",  "",            /*-1, -1*/},	// WeaponID_SmokeGrenade
	{"weapon_elite",        "models/w_elite.mdl",        10u, "9mm",          "ammo_9mm",        "models/w_357ammobox.mdl",     30u, 120u,  20u, WeaponSlotID_SecondaryWeapons, false, WeaponBulletType_1,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,     MapSize,     MapSize, /*35.5*/23.5f,  36u,  0u,  0.75f,   0.0f, 4.5f,  250u, false,  800u, "elites", "",            /* TeamID_Unassigned,  TeamID_Unassigned*/},	// WeaponID_Elite
	{"weapon_fiveseven",    "models/w_fiveseven.mdl",     7u, "57mm",         "ammo_57mm",       "models/w_357ammobox.mdl",     20u, 100u,  50u, WeaponSlotID_SecondaryWeapons, false, WeaponBulletType_15, WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0, HalfMapSize, HalfMapSize, /*35.5*/32.7f,  20u,  0u,  0.885f,  0.0f, 2.7f,  250u, false,  750u, "fn57",   "fiveseven",   /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_FiveSeven
	{"weapon_ump45",        "models/w_ump45.mdl",         6u, "45ACP",        "ammo_45acp",      "models/w_9mmclip.mdl",        25u, 100u,  25u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_9,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,     MapSize,     MapSize, /*32.0*/27.0f,  30u,  0u,  0.82f,   0.0f, 3.5f,  250u,  true, 1700u, "ump45",  "",            /* TeamID_CounterTerrorist,  TeamID_CounterTerrorist*/},	// WeaponID_UMP45
	{"weapon_sg550",        "models/w_sg550.mdl",         4u, "556Nato",      "ammo_556nato",    "models/w_crossbow_clip.mdl",  30u,  90u,  25u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_12, WeaponObstaclePenetrationPower_3, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*41.0*/40.0f,  70u,  0u,  0.98f,   0.0f, 3.35f, 210u, false, 4200u, "sg550",  "krieg55",     /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_SG550
	{"weapon_galil",        "models/w_galil.mdl",         4u, "556Nato",      "ammo_556nato",    "models/w_9mmarclip.mdl",      35u,  90u,  60u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_12, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*32.0*/26.5f,  30u,  0u,  0.98f,   0.0f, 2.45f, 240u,  true, 2000u, "galil",  "defender",    /* TeamID_Unassigned,  TeamID_Unassigned*/},	// WeaponID_Galil
	{"weapon_famas",        "models/w_famas.mdl",         4u, "556Nato",      "ammo_556nato",    "models/w_9mmarclip.mdl",      25u,  90u,  60u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_12, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*36.0*/32.6f,  30u, 34u,  0.96f,   0.0f, 3.3f,  240u,  true, 2250u, "famas",  "clarion",     /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_Famas
	{"weapon_usp",          "models/w_usp.mdl",           6u, "45ACP",        "ammo_45acp",      "models/w_357ammobox.mdl",     12u, 100u,  25u, WeaponSlotID_SecondaryWeapons,  true, WeaponBulletType_9,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0, HalfMapSize, HalfMapSize, /*41.0*/38.9f/* without silencer 23.5 */,  34u, 30u,  0.79f,  0.79f, 2.7f,  250u, false,  500u, "usp",    "km45",        /*-1, -1*/},	// WeaponID_USP
	{"weapon_glock18",      "models/w_glock18.mdl",      10u, "9mm",          "ammo_9mm",        "models/w_357ammobox.mdl",     20u, 120u,  20u, WeaponSlotID_SecondaryWeapons, false, WeaponBulletType_1,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,     MapSize,     MapSize, /*35.5*/32.6f,  25u,  0u,  0.75f,   0.0f, 2.2f,  250u, false,  400u, "glock",  "9x19mm",      /*-1, -1*/},	// WeaponID_Glock18
	{"weapon_awp",          "models/w_awp.mdl",           1u, "338Magnum",    "ammo_338magnum",  "models/w_crossbow_clip.mdl",  10u,  30u, 125u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_10, WeaponObstaclePenetrationPower_3, WeaponObstaclePenetrationPower_2,     MapSize,     MapSize, /*41.0*/39.5f, 115u,  0u,  0.99f,   0.0f, 2.5f,  210u, false, 4750u, "awp",    "magnum",      /* TeamID_CounterTerrorist,  TeamID_Unassigned*/},	// WeaponID_AWP
	{"weapon_mp5navy",      "models/w_mp5.mdl",          10u, "9mm",          "ammo_9mm",        "models/w_9mmclip.mdl",        30u, 120u,  20u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_1,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,     MapSize,     MapSize, /*32.0*/30.4f,  26u,  0u,  0.84f,   0.0f, 2.63f, 250u,  true, 1500u, "mp5",    "smg",         /* TeamID_CounterTerrorist,  TeamID_Terrorist*/},	// WeaponID_MP5
	{"weapon_m249",         "models/w_m249.mdl",          3u, "556NatoBox",   "ammo_556natobox", "models/w_chainammo.mdl",     100u, 200u,  60u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_12, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*37.0*/30.5f,  32u,  0u,  0.97f,   0.0f, 4.7f,  220u,  true, 5750u, "m249",   "",            /* TeamID_CounterTerrorist,  TeamID_Terrorist*/},	// WeaponID_M249
	{"weapon_m3",           "models/w_m3.mdl",            5u, "buckshot",     "ammo_buckshot",   "models/w_shotbox.mdl",         8u,  32u,  65u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0, HalfMapSize, HalfMapSize, /*35.5*/30.1f, 172u,  0u,  0.0f,    0.0f, 0.55f, 230u, false, 1700u, "m3",     "12gauge",     /* TeamID_CounterTerrorist, -1*/},	// WeaponID_M3
	{"weapon_m4a1",         "models/w_m4a1.mdl",          4u, "556Nato",      "ammo_556nato",    "models/w_9mmarclip.mdl",      30u,  90u,  60u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_12, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*42.0*/41.4f/* without silencer 32.6 */,  32u, 33u,  0.97f,  0.95f, 3.05f, 230u,  true, 3100u, "m4a1",   "",            /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_M4A1
	{"weapon_tmp",          "models/w_tmp.mdl",          10u, "9mm",          "ammo_9mm",        "models/w_9mmclip.mdl",        30u, 120u,  20u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_1,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,     MapSize,     MapSize, /*41.0*/39.2f,  20u,  0u,  0.85f,   0.0f, 2.12f, 250u,  true, 1250u, "tmp",    "mp",          /* TeamID_Terrorist,  TeamID_Terrorist*/},	// WeaponID_TMP
	{"weapon_g3sg1",        "models/w_g3sg1.mdl",         2u, "762Nato",      "ammo_762nato",    "models/w_crossbow_clip.mdl",  20u,  90u,  80u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_11, WeaponObstaclePenetrationPower_3, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*44.0*/42.2f,  80u,  0u,  0.98f,   0.0f, 3.5f,  210u, false, 5000u, "g3sg1",  "d3au1",       /* TeamID_Unassigned,  TeamID_CounterTerrorist*/},	// WeaponID_G3SG1
	{"weapon_flashbang",    "models/w_flashbang.mdl",    11u, "Flashbang",    "",                "models/w_flashbang.mdl",       0u,   2u,   0u, WeaponSlotID_Grenades,         false, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,          0u,          0u,  0.0f,   0u,  0u,  0.0f,    0.0f, 0.0f,  250u, false,  200u, "flash",  "",            /*-1, -1*/},	// WeaponID_FlashBangGrenade
	{"weapon_deagle",       "models/w_deagle.mdl",        8u, "50AE",         "ammo_50ae",       "models/w_357ammobox.mdl",      7u,  35u,  20u, WeaponSlotID_SecondaryWeapons, false, WeaponBulletType_13, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1, HalfMapSize, HalfMapSize, /*35.5*/34.1f,  54u,  0u,  0.81f,   0.0f, 2.2f,  250u, false,  400u, "deagle", "nighthawk",   /* TeamID_CounterTerrorist,  TeamID_CounterTerrorist*/},	// WeaponID_Deagle
	{"weapon_sg552",        "models/w_sg552.mdl",         4u, "556Nato",      "ammo_556nato",    "models/w_9mmarclip.mdl",      30u,  90u,  60u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_12, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*37.0*/34.0f,  33u,  0u,  0.955f,  0.0f, 3.0f,  235u,  true, 3500u, "sg552",  "krieg552",    /* TeamID_Unassigned, -1*/},	// WeaponID_SG552
	{"weapon_ak47",         "models/w_ak47.mdl",          2u, "762Nato",      "ammo_762nato",    "models/w_9mmarclip.mdl",      30u,  90u,  80u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_11, WeaponObstaclePenetrationPower_2, WeaponObstaclePenetrationPower_1,     MapSize,     MapSize, /*32.0*/24.8f,  36u,  0u,  0.98f,   0.0f, 2.45f, 221u,  true, 2500u, "ak47",   "cv47",        /* TeamID_Unassigned,  TeamID_Unassigned*/},	// WeaponID_AK47
	{"weapon_knife",        "models/w_knife.mdl",         0u, "",             "",                "",                             0u,   0u,   0u, WeaponSlotID_Knife,             true, WeaponBulletType_0,  WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,         80u,         64u,  0.0f,  16u, 66u,  0.0f,    0.0f, 0.0f,  250u,  true,    0u, "",       "",            /*-1, -1*/},	// WeaponID_Knife
	{"weapon_p90",          "models/w_p90.mdl",           7u, "57mm",         "ammo_57mm",       "models/w_9mmclip.mdl",        50u, 100u,  50u, WeaponSlotID_PrimaryWeapons,   false, WeaponBulletType_14, WeaponObstaclePenetrationPower_0, WeaponObstaclePenetrationPower_0,     MapSize,     MapSize, /*32.0*/25.4f,  21u,  0u,  0.885f,  0.0f, 3.4f,  245u,  true, 2350u, "p90",    "c90",         /* TeamID_CounterTerrorist,  1*/}	// WeaponID_P90
};

static const float DefaultWeaponSpreadRadius (0.5f);

struct RestrictableItem_t
{
	DynamicString buyAlias;			// buy alias

	unsigned char restrictIndex;	// Index in the "Restrict Weapons" AMXX plugin.

	inline RestrictableItem_t (const DynamicString &inputBuyAlias, const unsigned char inputRestrictIndex) :
		buyAlias (inputBuyAlias),

		restrictIndex (inputRestrictIndex)
	{ /* VOID */ }
};
struct BaseItemInformation_t : RestrictableItem_t
{
	unsigned short price;	// weapon price

	inline BaseItemInformation_t (const DynamicString &inputBuyAlias, const unsigned char inputRestrictIndex, const unsigned short inputPrice) :
		RestrictableItem_t (inputBuyAlias, inputRestrictIndex),

		price (inputPrice)
	{ /* VOID */ }
};
struct ItemInformation_Pickupable_t : BaseItemInformation_t
{
	DynamicString className;	// the weapon entity class name, in all letters
	DynamicString modelName;	// model name to separate Counter-Strike weapons

	inline ItemInformation_Pickupable_t (const DynamicString &inputBuyAlias, const unsigned char inputRestrictIndex, const unsigned short inputPrice, const DynamicString &inputClassName, const DynamicString &inputModelName) :
		BaseItemInformation_t (inputBuyAlias, inputRestrictIndex, inputPrice),

		className (inputClassName),
		modelName (inputModelName)
	{ /* VOID */ }
};

const BaseItemInformation_t        armor       ("vest",     0u,  800u);	// Kevlar Vest
const BaseItemInformation_t        armorHelm   ("vesthelm", 1u, 1000u);	// Kevlar Vest and Helmet
const ItemInformation_Pickupable_t defuseKit   ("defuser",  5u,  200u, "item_thighpack", "models/w_thighpack.mdl");	// Defuser Kit
const BaseItemInformation_t        nightVision ("nvgs",     6u, 1250u);	// Night Vision Goggles

// Ammunition (without price, as price is different for other types of weapons)
const RestrictableItem_t primaryAmmo   ("primammo", 7u);	// Primary weapon ammo
const RestrictableItem_t secondaryAmmo ("secammo",  8u);	// Secondary weapon ammo
#if 0
const BaseItemInformation_t *const itemProperties[] =
{
	&armor,		// index = 0
	&armorHelm,	// index = 1
/*
	WeaponID_FlashBangGrenade,	// index = 2
	WeaponID_ExplosiveGrenade,	// index = 3
	WeaponID_SmokeGrenade,	// index = 4
*/
	&defuseKit,		// index = 5
	&nightVision,	// index = 6

	// Ammunition
//	Prim ammo,	// index = 7
//	Sec ammo,	// index = 8
};
#endif	// if 0
#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_WEAPON_PROPERTY_INCLUDED