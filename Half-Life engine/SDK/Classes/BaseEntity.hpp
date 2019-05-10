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
// BaseEntity.hpp
//
// Class: BaseEntity
//
// Description: Base Entity. All entity types derive from this.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BASE_ENTITY_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_BASE_ENTITY_INCLUDED

class BaseEntity : public VoidBaseHookedClassWithVirtualFunctionTable
{
	//
	// Group: Type definitions.
	//
	public:
		typedef void (BaseEntity::*FunctionThink_t)   (void);
		typedef void (BaseEntity::*FunctionTouch_t)   (BaseEntity *const other);
		typedef void (BaseEntity::*FunctionUse_t)     (BaseEntity *const activator, BaseEntity *const caller, const Constants::UseType_t useType, const float value);
		typedef void (BaseEntity::*FunctionBlocked_t) (BaseEntity *const other);

		typedef void                               (BaseEntity::*FunctionSpawn_t)            (void);
		typedef const bool                         (BaseEntity::*FunctionTakeDamage_t)       (Structures::EntityVariables_t *const inflictor, Structures::EntityVariables_t *const attacker, const float damage, const Constants::DamageType_t bitsDamageType);
		typedef void                               (BaseEntity::*FunctionKilled_t)           (Structures::EntityVariables_t *const attacker, const Constants::GibType_t shouldGib);
		typedef const bool                         (BaseEntity::*FunctionAddPlayerItem_t)    (BasePlayerItem *const item);
		typedef const bool                         (BaseEntity::*FunctionRemovePlayerItem_t) (BasePlayerItem *const item);
		typedef const Constants::EntityClassType_t (BaseEntity::*FunctionClassify_t)         (void) const;

	public:
		// Constructor. Set engine to use C/C++ callback functions pointers to engine data
/*!off=4(1)*/		Structures::EntityVariables_t *m_variables;	// Don't need to save/restore this pointer, the engine resets it

		// path corners
/*!off=8(2)*/		BaseEntity *m_pGoalEnt;	// path corner we are heading towards
/*!off=12(3)*/		BaseEntity *m_pLink;	// used for temporary link-list operations.
#if 0
		// initialization functions
		virtual void          Spawn                 (void)                             { /* Empty */ }
		virtual void          Precache              (void)                             { /* Empty */ }
		virtual void          Restart               (void)                             { /* Empty */ }
		virtual void          KeyValue              (Structures::KeyValueData_t *keyValueData) { keyValueData->isHandled = false; }
		virtual int           Save                  (CSave &save);
		virtual int           Restore               (CRestore &restore);
		virtual int           ObjectCaps            (void)                             { return Constants::CapBit_ACROSS_TRANSITION; }
		virtual void          Activate              (void)                             { /* Empty */ }

		// Setup the object->object collision box (m_variables->mins / m_variables->maxs is the object->world collision box)
		virtual void          SetObjectCollisionBox (void);

		// Classify - returns the type of group (i.e, "houndeye", or "human military" so that monsters with different class names
		// still realize that they are teammates. (overridden for monsters that form groups)
		virtual Constants::EntityClassType_t Classify              (void) { return Constants::CLASS_NONE; }
		virtual void          DeathNotice           (Structures::EntityVariables_t *pevChild) { /* Empty */ }	// monster maker children use this to tell the monster maker that they have died.

		static TYPEDESCRIPTION m_SaveData[];

		virtual void          TraceAttack           (Structures::EntityVariables_t *pevAttacker, float flDamage, Math::Vector3D vecDir, Structures::TraceResult_t &ptr, int bitsDamageType);
		virtual BOOL          TakeDamage            (Structures::EntityVariables_t *inflictor, Structures::EntityVariables_t *attacker, float damage, Constants::DamageType_t bitsDamageType);
		virtual int           TakeHealth            (float flHealth, int bitsDamageType);
		virtual void          Killed                (Structures::EntityVariables_t *attacker, SDK::Constants::GibType_t shouldGib);
		virtual int           BloodColor            (void) { return DONT_BLEED; }
		virtual void          TraceBleed            (float flDamage, Math::Vector3D vecDir, Structures::TraceResult_t &ptr, int bitsDamageType);
		virtual BOOL          IsTriggered           (BaseEntity *pActivator) {return TRUE;}
		virtual BaseMonster  *MyMonsterPointer      (void) { return NULL; }
		virtual SquadMonster *MySquadMonsterPointer (void) { return NULL; }
		virtual int           GetToggleState        (void) { return TS_AT_TOP; }
		virtual void          AddPoints             (int score, BOOL bAllowNegativeScore) { /* Empty */ }
		virtual void          AddPointsToTeam       (int score, BOOL bAllowNegativeScore) { /* Empty */ }
		virtual BOOL          AddPlayerItem         (BasePlayerItem *pItem) { return FALSE; }
		virtual BOOL          RemovePlayerItem      (BasePlayerItem *pItem) { return FALSE; }
		virtual int           GiveAmmo              (int iAmount, char *szName, int iMax) { return -1; }
		virtual float         GetDelay              (void) { return 0.0f; }
		virtual int           IsMoving              (void) { return m_variables->velocity != Math::Vector3D::ZeroValue; }
		virtual void          OverrideReset         (void) { /* Empty */ }
		virtual int           DamageDecal           (int bitsDamageType);
		// This is ONLY used by the node graph to test movement through a door
		virtual void          SetToggleState        (int state) { /* Empty */ }
		virtual void          StartSneaking         (void) { /* Empty */ }
		virtual void          StopSneaking          (void) { /* Empty */ }
		virtual BOOL          OnControls            (Structures::EntityVariables_t *onpev) { return FALSE; }
		virtual BOOL          IsSneaking            (void) { return FALSE; }
		virtual BOOL          IsAlive               (void) { return m_variables->deadValue == Constants::DEAD_NO && m_variables->health > 0.0f; }
		virtual BOOL          IsBSPModel            (void) { return m_variables->solidType == Constants::SOLID_BSP || m_variables->moveType == Constants::MOVETYPE_PUSHSTEP; }
		virtual BOOL          ReflectGauss          (void) { return IsBSPModel () && m_variables->takeDamageType == Constants::DAMAGE_NO; }
		virtual BOOL          HasTarget             (TypeDefinitions::StringOffset_t targetName) { return FStrEq (STRING (targetName), STRING (m_variables->targetName)); }
		virtual BOOL          IsInWorld             (void);
		virtual BOOL          IsPlayer              (void) { return FALSE; }
		virtual BOOL          IsNetClient           (void) { return FALSE; }
		virtual const char   *TeamID                (void) { return ""; }

		virtual BaseEntity   *GetNextTarget         (void);
#endif	// if 0
		// fundamental callbacks
/*!off=16(4)*/		FunctionThink_t   m_pfnThink;
/*!off=20(5)*/		FunctionTouch_t   m_pfnTouch;
/*!off=24(6)*/		FunctionUse_t     m_pfnUse;
/*!off=28(7)*/		FunctionBlocked_t m_pfnBlocked;
#if 0
		virtual void           Think (void)
		{
			if (m_pfnThink != NULL)
				(this->*m_pfnThink) ();
		}
		virtual void           Touch (BaseEntity *pOther)
		{
			if (m_pfnTouch != NULL)
				(this->*m_pfnTouch) (pOther);
		}
		virtual void           Use (BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value)
		{
			if (m_pfnUse != NULL)
				(this->*m_pfnUse) (pActivator, pCaller, useType, value);
		}
		virtual void Blocked (BaseEntity *pOther)
		{
			if (m_pfnBlocked != NULL)
				(this->*m_pfnBlocked) (pOther);
		}

	public:
		#if defined MEMORY_MANAGER
		#	undef new
		#	undef delete
		#endif	// if defined MEMORY_MANAGER

			// allow engine to allocate instance data
			void *const operator new (size_t allocateBlockSize, Structures::EntityVariables_t *const newEntityVariables);	// Note: This function declared in 'Half-Life engine/Core.hpp'.

			// don't use this.
			#if defined _MSC_VER && _MSC_VER >= 1200	// only build this code if MSVC++ 6.0 or higher
				void operator delete (void *const memory, Structures::EntityVariables_t *const entityVariables)
				{
					entityVariables->flags |= Constants::FL_KILLME;
				}
			#endif

		#if defined MEMORY_MANAGER
		#	define new    (m_setOwner (__FILE__, __LINE__, __FUNCTION__), false) ? NULL : new
		#	define delete (m_setOwner (__FILE__, __LINE__, __FUNCTION__), false) ? ("", 0u, "") : delete
		#endif	// if defined MEMORY_MANAGER

		void UpdateOnRemove (void);

		// common member functions
		void EXPORT SUB_Remove (void);
		void EXPORT SUB_DoNothing (void);
		void EXPORT SUB_StartFadeOut (void);
		void EXPORT SUB_FadeOut (void);
		void EXPORT SUB_CallUseToggle (void) { Use (this, this, Constants::UseType_Toggle, 0.0f); }
		int    ShouldToggle (Constants::UseType_t useType, BOOL currentState);
		void   FireBullets (ULONG cShots, Math::Vector3D  vecSrc, Math::Vector3D vecDirShooting, Math::Vector3D vecSpread, float flDistance, int iBulletType, int iTracerFreq = 4, int iDamage = 0, Structures::EntityVariables_t *pevAttacker = NULL );
		Math::Vector3D FireBulletsPlayer (ULONG cShots, Math::Vector3D  vecSrc, Math::Vector3D vecDirShooting, Math::Vector3D vecSpread, float flDistance, int iBulletType, int iTracerFreq = 4, int iDamage = 0, Structures::EntityVariables_t *pevAttacker = NULL, int shared_rand = 0);

		virtual BaseEntity *Respawn (void) { return NULL; }

		void SUB_UseTargets (BaseEntity *pActivator, Constants::UseType_t useType, float value);
		// Do the bounding boxes of these two intersect?
		int  Intersects (BaseEntity *pOther);
		void MakeDormant (void);
		int IsDormant (void);
		BOOL IsLockedByMaster (void) { return FALSE; }

		static BaseEntity *Instance (Edict *pent)
		{
			if (pent == NULL)
				pent = Globals::g_halfLifeEngine->GetEntityOfEdictOffset (0u);

			return static_cast <BaseEntity *> (pent->privateData);
		}

		static BaseEntity *Instance (Structures::EntityVariables_t *instpev) { return Instance (instpev->containingEntity); }
		static BaseEntity *Instance (TypeDefinitions::EntityOffset_t inst_eoffset) { return Instance (Globals::g_halfLifeEngine->GetEntityOfEdictOffset (inst_eoffset)); }

		BaseMonster *GetMonsterPointer (Structures::EntityVariables_t *pevMonster)
		{
			BaseEntity *pEntity = Instance (pevMonster);

			if (pEntity != NULL)
				return pEntity->MyMonsterPointer ();

			return NULL;
		}
		BaseMonster *GetMonsterPointer (Edict *pentMonster)
		{
			BaseEntity *pEntity = Instance (pentMonster);

			if (pEntity != NULL)
				return pEntity->MyMonsterPointer ();

			return NULL;
		}

		// virtual functions used by a few classes

		// used by monsters that are created by the MonsterMaker
		virtual void UpdateOwner (void) { /* Empty */ }

		//
		static BaseEntity *Create (char *szName, const Math::Vector3D &vecOrigin, const Math::Vector3D &vecAngles, Edict *pentOwner = NULL);

		virtual BOOL  FBecomeProne (void) { return FALSE; }
#endif	// if 0
		inline Edict *const                          GetEdict  (void) const
		{
/*			// Reliability checks.
			InternalAssert (this != NULL);
			InternalAssert (m_variables != NULL && m_variables->containingEntity != NULL);
*/
			return m_variables->containingEntity;
		}
		// Note: The below 2 functions are declared in 'Half-Life engine/SDK/Classes/Edict.hpp'.
		inline const TypeDefinitions::EntityOffset_t GetOffset (void) const;
		inline const unsigned short                  GetIndex  (void) const;

		// Note: The below 6 functions are declared in 'Half-Life engine/SDK/Classes/BasePlayer.hpp'.
		inline       BasePlayer      *const GetBasePlayerPointer      (void);
		inline const BasePlayer      *const GetBasePlayerPointer      (void) const;
		inline       NonCZBasePlayer *const GetNonCZBasePlayerPointer (void);
		inline const NonCZBasePlayer *const GetNonCZBasePlayerPointer (void) const;

		inline       Bot        *const GetBotPointer        (void);
		inline const Bot        *const GetBotPointer        (void) const;
		inline       NonCZBot   *const GetNonCZBotPointer   (void);
		inline const NonCZBot   *const GetNonCZBotPointer   (void) const;
		inline       CSBot      *const GetCSBotPointer      (void);
		inline const CSBot      *const GetCSBotPointer      (void) const;
		inline       NonCZCSBot *const GetNonCZCSBotPointer (void);
		inline const NonCZCSBot *const GetNonCZCSBotPointer (void) const;

		// Note: The below 2 functions are declared in 'Half-Life engine/SDK/Classes/Grenade.hpp'.
		inline       CGrenade *const GetGrenadePointer (void);
		inline const CGrenade *const GetGrenadePointer (void) const;

		// Note: The below 2 functions are declared in 'Half-Life engine/SDK/Classes/BasePlayerItem.hpp'.
		inline       BasePlayerItem *const GetBasePlayerItemPointer (void);
		inline const BasePlayerItem *const GetBasePlayerItemPointer (void) const;

		// Note: The below 2 functions are declared in 'Half-Life engine/SDK/Classes/BasePlayerWeapon.hpp'.
		inline       BasePlayerWeapon *const GetBasePlayerWeaponPointer (void);
		inline const BasePlayerWeapon *const GetBasePlayerWeaponPointer (void) const;

		// Note: The below 2 functions are declared in 'Half-Life engine/SDK/Classes/Hostage.hpp'.
		inline       Hostage *const GetHostagePointer (void);
		inline const Hostage *const GetHostagePointer (void) const;
#if 0
		virtual Math::Vector3D Center (void) { return m_variables->absoluteMax.GetMidPoint (m_variables->absoluteMin); }		// center point of entity
		virtual Math::Vector3D EyePosition (void) { return m_variables->origin + m_variables->viewOffset; }		// position of eyes
		virtual Math::Vector3D EarPosition (void) { return m_variables->origin + m_variables->viewOffset; }		// position of ears
		virtual Math::Vector3D BodyTarget (const Math::Vector3D &posSrc) { return Center (); }	// position to shoot at

		virtual int Illumination (void) { return GETENTITYILLUM (m_variables->containingEntity); }

		virtual BOOL FVisible (BaseEntity *pEntity);
		virtual BOOL FVisible (const Math::Vector3D &vecOrigin);
#endif	// if 0
/*!off=32(8)*/		unsigned int m_UNKNOWN0;
/*!off=36(9)*/		unsigned int m_UNKNOWN1;
/*!off=40(10)*/		unsigned int m_UNKNOWN2;

		// We use this variables to store each ammo count.
/*!off=44(11)*/		int   ammo_buckshot;

/*!off=48(12)*/		unsigned int m_UNKNOWN3;

/*!off=52(13)*/		int   ammo_9mm;

/*!off=56(14)*/		unsigned int m_UNKNOWN4;

/*!off=60(15)*/		int   ammo_556Nato;

/*!off=64(16)*/		unsigned int m_UNKNOWN5;

/*!off=68(17)*/		int   ammo_556NatoBox;

/*!off=72(18)*/		unsigned int m_UNKNOWN6;

/*!off=76(19)*/		int   ammo_762Nato;

/*!off=80(20)*/		unsigned int m_UNKNOWN7;

/*!off=84(21)*/		int   ammo_45acp;

/*!off=88(22)*/		unsigned int m_UNKNOWN8;

/*!off=92(23)*/		int   ammo_50AE;

/*!off=96(24)*/		unsigned int m_UNKNOWN9;

/*!off=100(25)*/		int   ammo_338Magnum;

/*!off=104(26)*/		unsigned int m_UNKNOWN10;

/*!off=108(27)*/		int   ammo_57mm;

/*!off=112(28)*/		unsigned int m_UNKNOWN11;

/*!off=116(29)*/		int   ammo_357SIG;

		// Special stuff for grenades and satchels.
/*!off=120(30)*/		float m_flStartThrow;
/*!off=124(31)*/		float m_flReleaseThrow;
/*!off=128(32)*/		int   m_chargeReady;

/*!off=132(33)*/		bool m_isPlayerHasDisconnected;
/*
		int   m_fInAttack;

		enum EGON_FIRESTATE { FIRE_OFF, FIRE_CHARGE };
		int   m_fireState;
*/
	public:
		//
		// Function: GetIllumination
		//
		// Description: Gets the entity illumination.
		//
		// Note: This function declared in Core.hpp.
		//
		// Returns: Value between 0 and 255 corresponding to the entity's illumination.
		//
		inline const unsigned char GetIllumination (void) const;

		inline const Constants::EntityClassType_t CallClassify (void) const
		{
			return (this->*reinterpret_cast <const FunctionClassify_t &> (m_pseudoVirtualFunctionTable[Constants::EntityVirtualTableFunctionID_Classify])) ();
		}
		inline const bool CallIsPlayer (void) const
		{
			return (this->*reinterpret_cast <const FunctionClassify_t &> (m_pseudoVirtualFunctionTable[Constants::EntityVirtualTableFunctionID_IsPlayer])) () == TRUE;
		}

		inline void CallSpawn (void)
		{
			(this->*reinterpret_cast <const FunctionSpawn_t &> (m_pseudoVirtualFunctionTable[Constants::EntityVirtualTableFunctionID_Spawn])) ();
		}
		inline void CallTouch (BaseEntity *const other)
		{
			(this->*reinterpret_cast <const FunctionTouch_t &> (m_pseudoVirtualFunctionTable[Constants::EntityVirtualTableFunctionID_Touch])) (other);
		}
		inline void CallUse (BaseEntity *const caller, BaseEntity *const activator, const HalfLifeEngine::SDK::Constants::UseType_t useType = Constants::UseType_Toggle, const float value = 0.0f)
		{
			(this->*reinterpret_cast <const FunctionUse_t &> (m_pseudoVirtualFunctionTable[Constants::EntityVirtualTableFunctionID_Use])) (caller, activator, useType, value);
		}
		inline void CallUse (BaseEntity *const other, const HalfLifeEngine::SDK::Constants::UseType_t useType = Constants::UseType_Toggle, const float value = 0.0f)
		{
			CallUse (other, other, useType, value);
		}
};	// sizeof (BaseEntity) == 136 (34)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BASE_ENTITY_INCLUDED