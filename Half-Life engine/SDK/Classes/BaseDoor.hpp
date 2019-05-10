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
// BaseDoor.hpp
//
// Class: BaseDoor
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BASE_DOOR_INCLUDED
#define BASE_DOOR_INCLUDED

class BaseDoor : public BaseToggle
{
#if 0
	public:
		void         Spawn          (void);
		void         Precache       (void);
		virtual void KeyValue       (Structures::KeyValueData_t *pkvd);
		virtual void Use            (BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value);
		virtual void Blocked        (BaseEntity *pOther);

		virtual int ObjectCaps      (void)
		{
			if (m_variables->spawnFlags & Constants::SF_ITEM_USE_ONLY)
				return (BaseToggle::ObjectCaps () & ~Constants::Constants::CapBit_ACROSS_TRANSITION) | Constants::Constants::CapBit_IMPULSE_USE;
			else
				return (BaseToggle::ObjectCaps () & ~Constants::Constants::CapBit_ACROSS_TRANSITION);
		};
		virtual int  Save           (CSave &save);
		virtual int  Restore        (CRestore &restore);

		static TYPEDESCRIPTION m_SaveData[];

		virtual void SetToggleState (int state);

		// used to selectivly override defaults
		void EXPORT  DoorTouch      (BaseEntity *pOther);

		// local functions
		int          DoorActivate   (void);
		void EXPORT  DoorGoUp       (void);
		void EXPORT  DoorGoDown     (void);
		void EXPORT  DoorHitTop     (void);
		void EXPORT  DoorHitBottom  (void);
#endif	// if 0
	public:
		unsigned char m_bHealthValue;	// some doors are medi-kit doors, they give players health

		unsigned char m_bMoveSnd;		// sound a door makes while moving
		unsigned char m_bStopSnd;		// sound a door makes when it stops

		Structures::LockSound_t   m_ls;				// door lock sounds

		unsigned char m_bLockedSound;	// ordinals from entity selection
		unsigned char m_bLockedSentence;
		unsigned char m_bUnlockedSound;
		unsigned char m_bUnlockedSentence;
};

/*QUAKED FuncRotDoorSpawn (0 .5 .8) ? START_OPEN REVERSE
DOOR_DONT_LINK TOGGLE X_AXIS Y_AXIS
if two doors touch, they are assumed to be connected and operate as
a unit.

TOGGLE causes the door to wait in both the start and end states for
a trigger event.

START_OPEN causes the door to move to its destination when spawned,
and operate in reverse. It is used to temporarily or permanently
close off an area when triggered (not usefull for touch or
takedamage doors).

You need to have an origin brush as part of this entity. The
center of that brush will be
the point around which it is rotated. It will rotate around the Z
axis by default. You can
check either the X_AXIS or Y_AXIS box to change that.

"distance" is how many degrees the door will be rotated.
"speed" determines how fast the door moves; default value is 100.

REVERSE will cause the door to rotate in the opposite direction.

"angle"		determines the opening direction
"targetname" if set, no touch field will be spawned and a remote
button or trigger field activates the door.
"health"	if set, door must be shot open
"speed"		movement speed (100 default)
"wait"		wait before returning (3 default, -1 = never return)
"dmg"		damage to inflict when blocked (2 default)
"sounds"
0)	no sound
1)	stone
2)	base
3)	stone chain
4)	screechy metal
*/
class RotationDoor : public BaseDoor
{
#if 0
	public:
		void         Spawn          (void);
		virtual void SetToggleState (int state);
#endif	// if 0
};

class MomentaryDoor : public BaseToggle
{
	public:
#if 0
		void        Spawn      (void);
		void        Precache   (void);

		void        KeyValue   (Structures::KeyValueData_t *pkvd);
		void        Use        (BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value);
		virtual int ObjectCaps (void) { return BaseToggle::ObjectCaps () & ~Constants::CapBit_ACROSS_TRANSITION; }

		virtual int Save       (CSave &save);
		virtual int Restore    (CRestore &restore);

		static TYPEDESCRIPTION m_SaveData[];
#endif	// if 0
		unsigned char m_bMoveSnd;	// sound a door makes while moving
};

#endif	// ifndef BASE_DOOR_INCLUDED