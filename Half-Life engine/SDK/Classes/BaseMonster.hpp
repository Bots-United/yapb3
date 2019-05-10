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
// BaseMonster.hpp
//
// Class: BaseMonster
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BASE_MONSTER_INCLUDED
#define BASE_MONSTER_INCLUDED

class BaseMonster : public BaseToggle
{
	public:
/*! off=292(73) */		Constants::MonsterActivity_t       m_Activity;			// what the monster is doing (animation)
/*! off=296(74) */		Constants::MonsterActivity_t       m_IdealActivity;		// monster should switch to this activity
/*! off=300(75) */		int            m_LastHitGroup;		// the last body region that took damage
/*! off=304(76) */		int            m_bitsDamageType;	// what types of damage has monster (player) taken
/*! off=308(77) */		unsigned char  m_rgbTimeBasedDamage[Constants::TimeBasedDamageType_Total];
/*! off=316(79) */		Constants::MonsterState_t   m_MonsterState;		// monster's current state
/*! off=320(80) */		Constants::MonsterState_t   m_IdealMonsterState;	// monster should change to this state
/*! off=324(81) */		int            m_afConditions;
/*! off=328(82) */		int            m_afMemory;
/*! off=332(83) */		float          m_flNextAttack;		// cannot attack again until this time
/*! off=336(84) */		EntityHandle   m_hEnemy;			// the entity that the monster is fighting.
/*! off=344(86) */		EntityHandle   m_hTargetEnt;		// the entity that the monster is trying to reach
/*! off=352(88) */		float          m_flFieldOfView;		// width of monster's field of view (dot product)
/*! off=356(89) */		int            m_bloodColor;		// color of blood particless
/*! off=360(90) */		Math::Vector3D m_HackedGunPos;		// HACK until we can query end of gun
/*! off=372(93) */		Math::Vector3D m_vecEnemyLKP;		// last known position of enemy. (enemy's origin)
#if 0
	public:
		void                KeyValue               (Structures::KeyValueData_t *pkvd);

		void                MakeIdealYaw           (Math::Vector3D vecTarget);
		virtual float       ChangeYaw              (int speed);
		virtual BOOL        HasHumanGibs           (void);
		virtual BOOL        HasAlienGibs           (void);
		virtual void        FadeMonster            (void);	// Called instead of GibMonster() when gibs are disabled
		virtual void        GibMonster             (void);
		virtual Constants::MonsterActivity_t    GetDeathActivity       (void);
		Constants::MonsterActivity_t            GetSmallFlinchActivity (void);
		virtual void        BecomeDead             (void);
		BOOL                ShouldGibMonster       (int iGib);
		void                CallGibMonster         (void);
		virtual BOOL        ShouldFadeOnDeath      (void);
		BOOL                FCheckAITrigger        (void);	// checks and, if necessary, fires the monster's trigger target.
		virtual int         IRelationship          (BaseEntity *pTarget);
		virtual int         TakeHealth             (float flHealth, int bitsDamageType);
		virtual int         TakeDamage             (Structures::EntityVariables_t *pevInflictor, Structures::EntityVariables_t *pevAttacker, float flDamage, Constants::DamageType_t bitsDamageType);
		int                 DeadTakeDamage         (Structures::EntityVariables_t *pevInflictor, Structures::EntityVariables_t *pevAttacker, float flDamage, Constants::DamageType_t bitsDamageType);
		float               DamageForce            (float damage);
		virtual void        Killed                 (Structures::EntityVariables_t *pevAttacker, int iGib);
		virtual void        PainSound              (void) { /* Empty */ }
		virtual void        ResetMaxSpeed          (void) { /* Empty */ }

		void                RadiusDamage           (Structures::EntityVariables_t *pevInflictor, Structures::EntityVariables_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType);
		void                RadiusDamage           (Math::Vector3D vecSrc, Structures::EntityVariables_t *pevInflictor, Structures::EntityVariables_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType);

		inline void         SetConditions          (int conditions) { m_afConditions |= conditions; }
		inline void         ClearConditions        (int conditions) { m_afConditions &= ~conditions; }
		inline BOOL         HasConditions          (int conditions) { return (m_afConditions & conditions) != 0; }
		inline BOOL         HasAllConditions       (int conditions) { return (m_afConditions & conditions) == conditions; }

		inline void         Remember               (int memory) { m_afMemory |= memory; }
		inline void         Forget                 (int memory) { m_afMemory &= ~memory; }
		inline BOOL         HasMemory              (int memory) { return (m_afMemory & memory) != 0; }
		inline BOOL         HasAllMemories         (int memory) { return (m_afMemory & memory) == memory; }

		// This will stop animation until you call ResetSequenceInfo() at some point in the future
		inline void         StopAnimation          (void) { m_variables->frameRate = 0.0f; }

		virtual void        ReportAIState          (void);
		virtual void        MonsterInitDead        (void);	// Call after animation/pose is set up
		void EXPORT         CorpseFallThink        (void);

		virtual void        Look                   (int iDistance);	// basic sight function for monsters
		virtual BaseEntity *BestVisibleEnemy       (void);	// finds best visible enemy for attack
		BaseEntity         *CheckTraceHullAttack   (float flDist, int iDamage, int iDmgType);
		virtual BOOL        FInViewCone            (BaseEntity *pEntity);	// see if pEntity is in monster's view cone
		virtual BOOL        FInViewCone            (Math::Vector3D &pOrigin);	// see if given location is in monster's view cone
		void                TraceAttack            (Structures::EntityVariables_t *pevAttacker, float flDamage, Math::Vector3D vecDir, Structures::TraceResult_t &ptr, int bitsDamageType);
		void                MakeDamageBloodDecal   (int cCount, float flNoise, Structures::TraceResult_t &ptr, const Math::Vector3D &vecDir);
		virtual BOOL        IsAlive                (void) { return m_variables->deadValue != Constants::DEAD_DEAD; }
#endif	// if 0
};	// sizeof (BaseMonster) == 384 (96)

#endif	// ifndef BASE_MONSTER_INCLUDED