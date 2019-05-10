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
// Base.hpp
//
// Class: BaseMonster
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BASE_INCLUDED
#define BASE_INCLUDED

/*
Class Hierachy:

BaseEntity
	BaseDelay
		BaseAnimating
			BaseToggle
				BaseItem
				BaseMonster
					BaseCycler
					BasePlayer
						Bot
							CSBot
					BaseGroup
*/

#define	MAX_PATH_SIZE	10	// max number of nodes available for a path.

#if 0
#include "saverestore.h"
#include "schedule.h"

#ifndef MONSTEREVENT_H
#include "monsterevent.h"
#endif

// C functions for external declarations that call the appropriate C++ methods

#ifdef _WIN32
#define EXPORT	_declspec (dllexport)
#else
#define EXPORT	/* */
#endif

extern "C" EXPORT int GetEntityAPI (DLL_FUNCTIONS *pFunctionTable, int interfaceVersion);
extern "C" EXPORT int GetEntityAPI2 (DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion);

extern int  DispatchSpawn (Edict *pent);
extern void DispatchKeyValue (Edict *pentKeyvalue, Structures::KeyValueData_t *pkvd);
extern void DispatchTouch (Edict *pentTouched, Edict *pentOther);
extern void DispatchUse (Edict *pentUsed, Edict *pentOther);
extern void DispatchThink (Edict *pent);
extern void DispatchBlocked (Edict *pentBlocked, Edict *pentOther);
extern void DispatchSave (Edict *pent, SaveRestoreData_t *pSaveData);
extern int  DispatchRestore (Edict *pent, SaveRestoreData_t *pSaveData, int globalEntity);
extern void DispatchObjectCollsionBox (Edict *pent);
extern void SaveWriteFields (SaveRestoreData_t *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount);
extern void SaveReadFields (SaveRestoreData_t *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount);
extern void SaveGlobalState (SaveRestoreData_t *pSaveData);
extern void RestoreGlobalState (SaveRestoreData_t *pSaveData);
extern void ResetGlobalState (void);

extern void FireTargets (const char *targetName, BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value);
#endif	// if 0
class BaseMonster;
class BasePlayerItem;

#include <BaseEntity.hpp>
#include <EntityHandle.hpp>

// Ugly technique to override base member functions
// Normally it's illegal to cast a pointer to a member function of a derived class to a pointer to a
// member function of a base class. static_cast is a sleezy way around that problem.
#define SetThink(a) m_pfnThink = static_cast <void (BaseEntity::*)(void)> (a)
#define SetTouch(a) m_pfnTouch = static_cast <void (BaseEntity::*)(BaseEntity *)> (a)
#define SetUse(a) m_pfnUse = static_cast <void (BaseEntity::*) (BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value)> (a)
#define SetBlocked(a) m_pfnBlocked = static_cast <void (BaseEntity::*)(BaseEntity *)> (a)

#include <PointEntity.hpp>

#include <LockSound.hpp>

void PlayLockSounds (Structures::EntityVariables_t *pev, Structures::LockSound_t *pls, int flocked, int fbutton);

//
// MultiSouce
//

#define	MAX_MULTI_TARGETS	16	// maximum number of targets a single multi_manager entity may be assigned.
#define	MS_MAX_TARGETS	32

class CMultiSource : public PointEntity
{
	public:
#if 0
		void Spawn (void);
		void KeyValue (Structures::KeyValueData_t *pkvd);
		void Use (BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value);
		int ObjectCaps (void) { return (PointEntity::ObjectCaps () | Constants::CapBit_MASTER); }
		BOOL IsTriggered (BaseEntity *pActivator);
		void EXPORT Register (void);
		virtual int  Save (CSave &save);
		virtual int  Restore (CRestore &restore);

		static TYPEDESCRIPTION m_SaveData[];
#endif	// if 0
		EntityHandle  m_rgEntities[MS_MAX_TARGETS];
		int   m_rgTriggered[MS_MAX_TARGETS];

		int   m_iTotal;
		TypeDefinitions::StringOffset_t m_globalstate;
};

#include <BaseDelay.hpp>
#include <BaseAnimating.hpp>
#include <BaseToggle.hpp>


// people gib if their health is <= this at the time of death
#define	GIB_HEALTH_VALUE	-30

#define	ROUTE_SIZE			8	// how many waypoints a monster can store at one time
#define MAX_OLD_ENEMIES		4	// how many old enemies to remember

#define	bits_CAP_DUCK			 BIT (0u)	// crouch
#define	bits_CAP_JUMP			 BIT (1u)	// jump/leap
#define bits_CAP_STRAFE			 BIT (2u)	// strafe (walk/run sideways)
#define bits_CAP_SQUAD			 BIT (3u)	// can form squads
#define	bits_CAP_SWIM			 BIT (4u)	// proficiently navigate in water
#define bits_CAP_CLIMB			 BIT (5u)	// climb ladders/ropes
#define bits_CAP_USE			 BIT (6u)	// open doors/push buttons/pull levers
#define bits_CAP_HEAR			 BIT (7u)	// can hear forced sounds
#define bits_CAP_AUTO_DOORS		 BIT (8u)	// can trigger auto doors
#define bits_CAP_OPEN_DOORS		 BIT (9u)	// can open manual doors
#define bits_CAP_TURN_HEAD		 BIT (10u)	// can turn head, always bone controller 0

#define bits_CAP_RANGE_ATTACK1	 BIT (11u)	// can do a range attack 1
#define bits_CAP_RANGE_ATTACK2	 BIT (12u)	// can do a range attack 2
#define bits_CAP_MELEE_ATTACK1	 BIT (13u)	// can do a melee attack 1
#define bits_CAP_MELEE_ATTACK2	 BIT (14u)	// can do a melee attack 2

#define bits_CAP_FLY			 BIT (15u)	// can fly, move all around

#define bits_CAP_DOORS_GROUP   (bits_CAP_USE | bits_CAP_AUTO_DOORS | bits_CAP_OPEN_DOORS)

// used by suit voice to indicate damage sustained and repaired type to player

// instant damage

#define DMG_GENERIC			0u			// generic damage was done
#define DMG_CRUSH			BIT (0u)	// crushed by falling or moving object
#define DMG_BULLET			BIT (1u)	// shot
#define DMG_SLASH			BIT (2u)	// cut, clawed, stabbed
#define DMG_BURN			BIT (3u)	// heat burned
#define DMG_FREEZE			BIT (4u)	// frozen
#define DMG_FALL			BIT (5u)	// fell too far
#define DMG_BLAST			BIT (6u)	// explosive blast damage
#define DMG_CLUB			BIT (7u)	// crowbar, punch, headbutt
#define DMG_SHOCK			BIT (8u)	// electric shock
#define DMG_SONIC			BIT (9u)	// sound pulse shockwave
#define DMG_ENERGYBEAM		BIT (10u)	// laser or other high energy beam
#define DMG_NEVERGIB		BIT (12u)	// with this bit OR'd in, no damage type will be able to gib victims upon death
#define DMG_ALWAYSGIB		BIT (13u)	// with this bit OR'd in, any damage type can be made to gib victims upon death.
#define DMG_DROWN			BIT (14u)	// Drowning
// time-based damage
#define DMG_TIMEBASED		(~0x3fff)	// mask for time-based damage

#define DMG_PARALYZE		BIT (15u)	// slows affected creature down
#define DMG_NERVEGAS		BIT (16u)	// nerve toxins, very bad
#define DMG_POISON			BIT (17u)	// blood poisioning
#define DMG_RADIATION		BIT (18u)	// radiation exposure
#define DMG_DROWNRECOVER	BIT (19u)	// drowning recovery
#define DMG_ACID			BIT (20u)	// toxic chemicals or acid burns
#define DMG_SLOWBURN		BIT (21u)	// in an oven
#define DMG_SLOWFREEZE		BIT (22u)	// in a subzero freezer
#define DMG_MORTAR			BIT (23u)	// Hit by air raid (done to distinguish grenade from mortar)

// these are the damage types that are allowed to gib corpses
#define DMG_GIB_CORPSE		 (DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB)

// these are the damage types that have client hud art
#define DMG_SHOWNHUD		(DMG_POISON | DMG_ACID | DMG_FREEZE | DMG_SLOWFREEZE | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | DMG_RADIATION | DMG_SHOCK)

// NOTE: tweak these values based on gameplay feedback:

#define PARALYZE_DURATION	2		// number of 2 second intervals to take damage
#define PARALYZE_DAMAGE		1.0f	// damage to take each 2 second interval

#define NERVEGAS_DURATION	2
#define NERVEGAS_DAMAGE		5.0f

#define POISON_DURATION		5
#define POISON_DAMAGE		2.0f

#define RADIATION_DURATION	2
#define RADIATION_DAMAGE	1.0f

#define ACID_DURATION		2
#define ACID_DAMAGE			5.0f

#define SLOWBURN_DURATION	2
#define SLOWBURN_DAMAGE		1.0f

#define SLOWFREEZE_DURATION	2
#define SLOWFREEZE_DAMAGE	1.0f


#define	itbd_Paralyze		0
#define	itbd_NerveGas		1
#define	itbd_Poison			2
#define	itbd_Radiation		3
#define	itbd_DrownRecover	4
#define	itbd_Acid			5
#define	itbd_SlowBurn		6
#define	itbd_SlowFreeze		7
#define CDMG_TIMEBASED		8

// when calling KILLED(), a value that governs gib behavior is expected to be
// one of these three values
#define GIB_NORMAL			0// gib if entity was overkilled
#define GIB_NEVER			1// never gib, no matter how much death damage is done (freezing, etc)
#define GIB_ALWAYS			2// always gib (Houndeye Shock, Barnacle Bite)

class CCineMonster;
class CSound;

#include "BaseMonster.hpp"


char *ButtonSound (int sound);	// get string of button sound number


//
// Generic Button
//
class BaseButton : public BaseToggle
{
	public:
#if 0
		void Spawn (void);
		virtual void Precache (void);
		void RotSpawn (void);
		virtual void KeyValue (Structures::KeyValueData_t* pkvd);

		void ButtonActivate (void);
		void SparkSoundCache (void);

		void EXPORT ButtonShot (void);
		void EXPORT ButtonTouch (BaseEntity *pOther);
		void EXPORT ButtonSpark (void);
		void EXPORT TriggerAndWait (void);
		void EXPORT ButtonReturn (void);
		void EXPORT ButtonBackHome (void);
		void EXPORT ButtonUse (BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value);
		virtual int TakeDamage (Structures::EntityVariables_t* pevInflictor, Structures::EntityVariables_t* pevAttacker, float flDamage, Constants::DamageType_t bitsDamageType);
		virtual int Save (CSave &save);
		virtual int Restore (CRestore &restore);

		enum BUTTON_CODE { BUTTON_NOTHING, BUTTON_ACTIVATE, BUTTON_RETURN };
		BUTTON_CODE ButtonResponseToTouch (void);

		static TYPEDESCRIPTION m_SaveData[];
		// Buttons that don't take damage can be IMPULSE used
		virtual int ObjectCaps (void) { return (BaseToggle::ObjectCaps () & ~Constants::CapBit_ACROSS_TRANSITION) | (m_variables->takedamage ? 0 : Constants::CapBit_IMPULSE_USE); }
#endif	// if 0
		BOOL                            m_fStayPushed;	// button stays pushed in until touched again?
		BOOL                            m_fRotating;		// a rotating button? default is a sliding button.

		TypeDefinitions::StringOffset_t m_strChangeTarget;	// if this field is not null, this is an index into the engine string array.
															// when this button is touched, it's target entity's TARGET field will be set
															// to the button's ChangeTarget. This allows you to make a func_train switch paths, etc.

		Structures::LockSound_t         m_ls;					// door lock sounds

		unsigned char                   m_bLockedSound;		// ordinals from entity selection
		unsigned char                   m_bLockedSentence;
		unsigned char                   m_bUnlockedSound;
		unsigned char                   m_bUnlockedSentence;
		int                             m_sounds;
};

//
// Weapons
//

#define	BAD_WEAPON 0x00007FFF

//
// Converts a Structures::EntityVariables_t * to a class pointer
// It will allocate the class and entity if necessary
//
template <class classType> classType *const GetClassPtr (classType *classPointer)
{
	Structures::EntityVariables_t *entityVariables (static_cast <Structures::EntityVariables_t *> (classPointer));

	// allocate entity if necessary
	if (entityVariables == NULL)
		entityVariables = &Globals::g_halfLifeEngine->CreateEntity ()->variables;

	// get the private data
	classPointer = static_cast <classType *> (entityVariables->containingEntity->privateData);

	if (classPointer == NULL)
	{
		// allocate private data
//		classPointer = new (entityVariables) classType;
		classPointer = Globals::g_halfLifeEngine->AllocateEntityPrivateData <classType> (entityVariables->containingEntity);

		classPointer->m_variables = entityVariables;
	}

	return classPointer;
//	return classPointer == NULL ? classPointer = Globals::g_halfLifeEngine->AllocateEntity () : classPointer;
}

/*
bit_PUSHBRUSH_DATA | bit_TOGGLE_DATA
bit_MONSTER_DATA
bit_DELAY_DATA
bit_TOGGLE_DATA | bit_DELAY_DATA | bit_MONSTER_DATA
bit_PLAYER_DATA | bit_MONSTER_DATA
bit_MONSTER_DATA | CYCLER_DATA
bit_LIGHT_DATA
path_corner_data
bit_MONSTER_DATA | wildcard_data
bit_MONSTER_DATA | bit_GROUP_DATA
boid_flock_data
boid_data
CYCLER_DATA
bit_ITEM_DATA
bit_ITEM_DATA | func_hud_data
bit_TOGGLE_DATA | bit_ITEM_DATA
EOFFSET
env_sound_data
env_sound_data
push_trigger_data
*/

#define TRACER_FREQ		4			// Tracers fire every 4 bullets

struct SelAmmo
{
	unsigned char Ammo1Type;
	unsigned char Ammo1;
	unsigned char Ammo2Type;
	unsigned char Ammo2;
};


// this moved here from world.cpp, to allow classes to be derived from it
//=======================
// World
//
// This spawns first when each level begins.
//=======================
class World : public BaseEntity
{
#if 0
	public:
		void Spawn    (void);
		void Precache (void);
		void KeyValue (Structures::KeyValueData_t *pkvd);
#endif	// if 0
};

#endif	// ifndef BASE_INCLUDED