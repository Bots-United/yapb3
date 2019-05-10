//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// UserMessages.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_USER_MESSAGES_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_USER_MESSAGES_INCLUDED

namespace UserMessages
{
#pragma pack (push, 1)	// Structures must be packed (byte-aligned) (Very important!)
struct AmmoPickup_t
{
	unsigned char ammoID;	// (From 'Constants::WeaponID_t' enumeration)
	unsigned char amount;
};	// sizeof (AmmoPickup_t) == 2 (0.5)

struct AmmoX_t
{
	unsigned char ammoID;	// (From 'Constants::WeaponID_t' enumeration)
	unsigned char amount;
};	// sizeof (AmmoX_t) == 2 (0.5)

struct CurWeapon_t
{
	bool state;		// Is this weapon is fixed in hands?
	char ID;		// Weapon ID. (From 'Constants::WeaponID_t' enumeration)
	char clipAmmo;	// Ammo currently in the clip for this weapon.
};	// sizeof (CurWeapon_t) == 3 (0.75)

struct Damage_t
{
	unsigned char                  save;
	unsigned char                  take;
	Constants::DamageType_t        type;		// (From 'Constants::DamageType_t' enumeration)
	Math::TemplateVector3D <short> coordinates;	// static_cast <short *> (origin * 8.0)
};	// sizeof (Damage_t) == 12 (3)

// This structure is sent over the net to describe a screen shake event
struct ScreenShake_t
{
	unsigned short amplitude;	// FIXED 4.12 amount of shake
	unsigned short duration;	// FIXED 4.12 seconds duration
	unsigned short frequency;	// FIXED 8.8 noise frequency (low frequency is a jerk, high frequency is a rumble)
};	// sizeof (ScreenShake_t) == 6 (1.5)

// This structure is sent over the net to describe a screen fade event
struct ScreenFade_t
{
	unsigned short    duration;		// FIXED 4.12 seconds duration
	unsigned short    holdTime;		// FIXED 4.12 seconds duration until reset (fade & hold)
	short             fadeFlags;	// flags (From 'Constants::ScreenFadeFlag_t' enumeration)
	ColorWithAlpha <> fadeColor;	// fade to color (max alpha)
};	// sizeof (ScreenFade_t) == 10 (2.5)

struct ShowMenu_t
{
	Constants::Menu::KeyBit_t validSlots;	// a bitfield of keys that are valid input (0 means that the current menu should be closed)
	char                      lifeTime;		// the duration, in seconds, the menu should stay up. -1 means is stays until something is chosen.
	bool                      isNeedMore;	// TRUE if there is more string yet to be received before displaying the menu, FALSE if it's the last string
	char                      string[Constants::MaximumMenuStringLength];	// menu string to display
};	// sizeof (ShowMenu_t) == 192 (48)

struct StatusIcon_t
{
	unsigned char status;	// (From 'Constants::StatusIcon_StatusType_t' enumeration)
	char          spriteName[Constants::MaximumUserMessageSize - sizeof (unsigned char)/* status */];
};	// sizeof (StatusIcon_t) == 192 (48)

struct ReloadSound_t
{
	unsigned char volume;
	bool          isNotShotgun;
};	// sizeof (ReloadSound_t) == 2 (0.5)

struct TeamInfo_t
{
	unsigned char playerEdictIndex;
	char          name[Constants::MaximumUserMessageSize - sizeof (unsigned char)/* playerEdictIndex */];
};	// sizeof (TeamInfo_t) == 192 (48)

struct TextMsg_t
{
	unsigned char destination;	// (From 'Constants::HUDPrintType_t' enumeration)
	char          text[Constants::MaximumUserMessageSize - sizeof (unsigned char)/* destination */];	// (Note: can contain substrings)
};	// sizeof (TextMsg_t) == 192 (48)

struct VGUIMenu_t
{
	unsigned char             ID;	// (From 'Constants::VGUIMenuID_t' enumeration)
	Constants::Menu::KeyBit_t keys;
	char                      lifeTime;
	bool                      isNeedMore;
	char                      name[Constants::MaximumUserMessageSize - (sizeof (unsigned char)/* ID */ + sizeof (Constants::Menu::KeyBit_t)/* keys */ + sizeof (char)/* lifeTime */ + sizeof (bool)/* isNeedMore */)];
};	// sizeof (VGUIMenu_t) == 192 (48)
#pragma pack (pop)	// Reset default packing.

// Reliability checks.
CompileTimeAssert (sizeof (AmmoPickup_t) == 2u);
CompileTimeAssert (sizeof (AmmoX_t) == 2u);
CompileTimeAssert (sizeof (CurWeapon_t) == 3u);
CompileTimeAssert (sizeof (Damage_t) == 12u);
CompileTimeAssert (sizeof (ScreenShake_t) == 6u);
CompileTimeAssert (sizeof (ScreenFade_t) == 10u);
CompileTimeAssert (sizeof (ShowMenu_t) == Constants::MaximumUserMessageSize);
CompileTimeAssert (sizeof (StatusIcon_t) == Constants::MaximumUserMessageSize);
CompileTimeAssert (sizeof (ReloadSound_t) == 2u);
CompileTimeAssert (sizeof (TeamInfo_t) == Constants::MaximumUserMessageSize);
CompileTimeAssert (sizeof (TextMsg_t) == Constants::MaximumUserMessageSize);
CompileTimeAssert (sizeof (VGUIMenu_t) == Constants::MaximumUserMessageSize);
}

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_USER_MESSAGES_INCLUDED