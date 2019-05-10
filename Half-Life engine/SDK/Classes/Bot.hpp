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
// Bot.hpp
//
// Class: Bot
//
// Description: This is Half-Life Bot entity.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BOT_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_BOT_INCLUDED

template <typename basePlayerClassType> class TemplateBot : public basePlayerClassType
{
	public:
		DECLARE_TYPED_ENUMERATION (unsigned char, VisiblePartType_t)
		{
			VisiblePartType_None,

			VisiblePartType_Waist     = BIT (1u),	// Chest (is 'enemy->variables.origin' visible)
			VisiblePartType_Head      = BIT (2u),	// (is 'enemy->variables.origin + Math::Vector3D (0.0f, 0.0f, 25.0f)' visible)

			/// Math::Vector2D direction ((enemy->variables.origin.AsVector2D () - bot->m_variables->origin.AsVector2D ()).Normalize ())
			/// direction.y = -direction.y;	// Invert.
			/// direction *= 13.0f;
			/// SwapElements (direction.x, direction.y);
			VisiblePartType_LeftSide  = BIT (3u),	// (is 'enemy->variables.origin + direction' visible)
			VisiblePartType_RightSide = BIT (4u),	// (is 'enemy->variables.origin - direction' visible)

			VisiblePartType_Legs      = BIT (5u)	// (is 'enemy->variables.origin - Math::Vector3D (0.0f, 0.0f, (enemy->variables.flags & FL_DUCKING) ? 14.0f : 34.0f)' visible)
		};

		class Profile
		{
			public:
				enum { MaximumWeaponsNumber = 16u };

/*! off=0(0) */				char                 *name;
/*! off=4(1) */				float                 aggression;
/*! off=8(2) */				float                 skill;
/*! off=12(3) */				float                 teamwork;
/*! off=16(4) */				Constants::WeaponID_t weaponPreferences[MaximumWeaponsNumber];
/*! off=80(20) */				unsigned int          weaponPreferencesNumber;
/*! off=84(21) */				int                   cost;
/*! off=88(22) */				int                   skin;
/*! off=92(23) */				unsigned char         difficulty;
/*! off=96(24) */				int                   voicePitch;
/*! off=100(25) */				float                 reactionTime;
/*! off=104(26) */				float                 attackDelay;
/*! off=108(27) */				Constants::TeamID_t   team;
/*! off=112(28) */				bool                  preferUsingSilencer;
/*! off=116(29) */				int                   voiceBankIndex;
/*
			public:
				inline const DynamicString &GetWeaponPreferenceAsString (const unsigned char weaponIndex) const
				{
					InternalAssert (weaponIndex < weaponPreferencesNumber);

					return Constants::weaponProperties[weaponPreferences[weaponIndex]].buyAlias1;
				}
				inline const bool HasPrimaryPreference (void) const
				{
					if (weaponPreferencesNumber == 0u)
						return false;

					return true;	/// @todo !!!
				}
				inline const bool HasPistolPreference (void) const
				{
					if (weaponPreferencesNumber == 0u)
						return false;

					return true;	/// @todo !!!
				}
				inline const bool IsValidForTeam (const Constants::TeamID_t teamID) const
				{
					return teamID == Constants::TeamID_CounterTerrorist || team == Constants::TeamID_CounterTerrorist || team == teamID;
				}
				inline const int GetCustomSkin (const unsigned char skinIndex) const
				{
					return 0;
				}
				// ....*/
		};	// sizeof (Profile) == 120 (30)
		enum { MaximumPostureContexts = 8u };
		struct PostureContext_t
		{
			bool doRun;
			bool doCrouch;
		};	// sizeof (PostureContext_t) == 2 (0.5)
		enum MoraleType_t
		{
			// (-3 ... 3)
			MoraleType_Terrible = -3,	// TERRIBLE
			MoraleType_Bad,				// BAD
			MoraleType_Negative,		// NEGATIVE

			MoraleType_Neutral,			// NEUTRAL

			MoraleType_Positive,		// POSITIVE
			MoraleType_Good,			// GOOD
			MoraleType_Excellent		// EXCELLENT
		};

/*! off=2480(620) */		Profile                   *m_profile;	// Pointer to bot profile.
/*! off=2484(621) */		unsigned int               m_ID;	// Unique bot ID. (Started from 1. Used in function CSBotManager::GetPlayerPriority())
/*! off=2488(622) */		float                      m_nextCommandExecutionTime;	// Time of next call Bot::Upkeep() and Bot::ExecuteCommand() functions.
/*! off=2492(623) */		float                      m_nextThinkExecutionTime;	// Time of next call Bot::Update() function.

/*! off=2496(624) */		float                      m_lastThinkTime;	// Time bot executed Bot::ExecuteCommand() function.

/*! off=2500(625) */		bool                       m_doRun;
/*! off=2501(625.25) */		bool                       m_doCrouch;

/*! off=2504(626) */		float                      m_speedMove;		// Forward/backward speed.
/*! off=2508(627) */		float                      m_speedStrafe;	// Side speed.
/*! off=2512(628) */		float                      m_speedVertical;	// Upwad/downward speed. (in water)

/*! off=2516(629) */		unsigned short             m_pressedButtons;	// (From 'Constants::ButtonType_t' enumeration)

/*! off=2520(630) */		float                      m_jumpTimestamp;
/*! off=2524(631) */		PostureContext_t           m_postureContexts[MaximumPostureContexts];
/*! off=2540(635) */		unsigned int               m_postureContextsNumber;
/*! off=2544(636) */		char                       m_name[Constants::MaximumClientNameLength];
/*! off=2576(644) */		unsigned int               m_UNKNOWN93[8u];	// OR THIS IS PART OF 'm_name'?!?!?!??!?!?!?!?!?!?!??!?!?!?!?!?!?!?!?!?!?!?!??!?!?!?!??!
/*! off=2608(652) */		float                      m_combatRange;	// Combat range.
/*! off=2612(653) */		bool                       m_isRogue;
/*! off=2616(654) */		Utilities::CountdownTimer  m_isRogueCheckTimer;	// Timer to set 'm_isRogue' value depending on 'Profile::teamwork' variable.
/*! off=2624(656) */		MoraleType_t               m_morale;
/*! off=2628(657) */		bool                       m_isDead;
/*! off=2632(658) */		float                      m_safeEndTime;
/*! off=2636(659) */		bool                       m_isSafe;

						enum BlindActionType_t
						{
							BlindActionType_MoveForward,
							BlindActionType_StrafeRight,
							BlindActionType_MoveBackward,
							BlindActionType_StrafeLeft,

							/// @note For below actions (from 'BlindActionType_Crouch' to +infinity) bot will just do crouch - at all.
							BlindActionType_Crouch,

							BlindActionType_Total
						};
/*! off=2640(660) */		BlindActionType_t          m_blindAction;	// (Assigns by random in CSBot::Blind() function) (0 ... 5)
/*! off=2644(661) */		bool                       m_blindDoPrimaryAttack;	// (Assigns in CSBot::Blind() function)
/*! off=2648(662) */		float                      m_surpriseTime;	// Enemy surprise time.
/*! off=2652(663) */		float                      m_surprisedTime;	// My surprise time.
/*! off=2656(664) */		bool                       m_isFollowing;
/*! off=2660(665) */		EntityHandle               m_followingTarget;
/*! off=2668(667) */		float                      m_followingStartTime;
/*! off=2672(668) */		float                      m_nextAutoFollowTime;	// From CCSBot__CanAutoFollow() function in cs_i386.so only!
/*! off=2676(669) */		Utilities::CountdownTimer  m_hurryTimer;	// From CCSBot__Hurry() function in cs_i386.so only!

	public:
#if 0
		/// @todo FIND AND DEFINE RETURN TYPES!
		virtual void Initialize          (const Profile *const profile) = 0;
		virtual void SpawnBot            (void) = 0;
		virtual void Upkeep              (void) = 0;
		virtual void Update              (void) = 0;
		virtual void Run                 (void) = 0;
		virtual void Walk                (void) = 0;
		virtual void Crouch              (void) = 0;
		virtual void StandUp             (void) = 0;
		virtual void MoveForward         (void) = 0;
		virtual void MoveBackward        (void) = 0;
		virtual void StrafeLeft          (void) = 0;
		virtual void StrafeRight         (void) = 0;
		virtual void Jump                (bool ) = 0;
		virtual void ClearMovement       (void) = 0;
		virtual void UseEnvironment      (void) = 0;
		virtual void PrimaryAttack       (void) = 0;
		virtual void ClearPrimaryAttack  (void) = 0;
		virtual void TogglePrimaryAttack (void) = 0;
		virtual void SecondaryAttack     (void) = 0;
		virtual void Reload              (void) = 0;
		virtual void OnEvent             (GameEventType gameEvent, BaseEntity *const , BaseEntity *const ) = 0;
		virtual void IsVisible           (const Math::Vector3D &origin, bool ) const = 0;
		virtual void IsVisible           (BasePlayer *const player, bool , VisiblePartType_t *const visiblePartMask) const = 0;
		virtual void IsEnemyPartVisible  (VisiblePartType_t part) const = 0;
		virtual void IsPlayerFacingMe    (BasePlayer *const player) const = 0;
		virtual void IsPlayerLookingAtMe (BasePlayer *const player) const = 0;
		virtual void ExecuteCommand      (void) = 0;
		virtual void SetModel            (const char *const modelName) = 0;
#endif	// if 0
};

class Bot      : public TemplateBot <BasePlayer>      { /* Empty */ };
class NonCZBot : public TemplateBot <NonCZBasePlayer> { /* Empty */ };

inline       Bot      *const BaseEntity::GetBotPointer      (void)       { return static_cast <Bot *const> (this); }
inline const Bot      *const BaseEntity::GetBotPointer      (void) const { return static_cast <const Bot *const> (this); }
inline       NonCZBot *const BaseEntity::GetNonCZBotPointer (void)       { return static_cast <NonCZBot *const> (this); }
inline const NonCZBot *const BaseEntity::GetNonCZBotPointer (void) const { return static_cast <const NonCZBot *const> (this); }

template <typename baseBotClassType> class TemplateCSBot : public baseBotClassType
{
	public:
		enum TaskType_t
		{
			TaskType_SeekAndDestroy,				// SEEK_AND_DESTROY
			TaskType_PlantingBomb,					// PLANT_BOMB
			TaskType_FindTickingBomb,				// FIND_TICKING_BOMB
			TaskType_DefusingBomb,					// DEFUSE_BOMB
			TaskType_GuardTickingBomb,				// GUARD_TICKING_BOMB
			TaskType_GuardBombDefuser,				// GUARD_BOMB_DEFUSER
			TaskType_GuardLooseBomb,				// GUARD_LOOSE_BOMB
			TaskType_GuardingBombsite,				// GUARD_BOMB_ZONE
			TaskType_EscapeFromBomb,				// ESCAPE_FROM_BOMB
			TaskType_HoldPosition,					// HOLD_POSITION
			TaskType_Follow,						// FOLLOW
			TaskType_VIPEscape,						// VIP_ESCAPE
			TaskType_GuardingEscapeZone,			// GUARD_VIP_ESCAPE_ZONE
			TaskType_CollectingHostages,			// COLLECT_HOSTAGES
			TaskType_RescueHostages,				// RESCUE_HOSTAGES
			TaskType_GuardingHostages,				// GUARD_HOSTAGES
			TaskType_GuardingRescueZone,			// GUARD_HOSTAGE_RESCUE_ZONE
			TaskType_MoveToLastKnownEnemyPosition,	// MOVE_TO_LAST_KNOWN_ENEMY_POSITION
			TaskType_MoveToSniperSpot,				// MOVE_TO_SNIPER_SPOT
			TaskType_Sniping						// SNIPING
		};
		enum LadderMovementState_t
		{
			APPROACH_ASCENDING_LADDER,
			APPROACH_DESCENDING_LADDER,
			FACE_ASCENDING_LADDER,
			FACE_DESCENDING_LADDER,
			MOUNT_ASCENDING_LADDER,
			MOUNT_DESCENDING_LADDER,
			ASCEND_LADDER,
			DESCEND_LADDER,
			DISMOUNT_ASCENDING_LADDER,
			DISMOUNT_DESCENDING_LADDER,	// EXISTS?? RIGHT NAME??
			MOVE_TO_DESTINATION	// Fell from ladder.
		};
		enum LadderDismountAscendingMoveType_t
		{
			LadderDismountAscendingMoveType_Forward,
			LadderDismountAscendingMoveType_Right,
			LadderDismountAscendingMoveType_None,	// EXISTS?? RIGHT NAME??
			LadderDismountAscendingMoveType_Left
		};
		enum NoisePriority_t
		{
			NoisePriority_Low,
			NoisePriority_Medium,
			NoisePriority_High
		};
		enum LookAction_t
		{
			LookAction_None,			// Not aiming at all....

			LookAction_LookTowardsSpot,	// Just got new target origin and trying to aim at it. (LOOK_TOWARDS_SPOT)
			LookAction_LookAtSpot		// Already looking at target origin. (LOOK_AT_SPOT)
		};
		enum PriorityType_t
		{
			PriorityType_Low,
			PriorityType_Medium,
			PriorityType_High,
			PriorityType_VeryHigh	// When throwing grenade.
		};
		enum DispositionType_t
		{
			DispositionType_EngageAndInvestigate,	// ENGAGE_AND_INVESTIGATE
			DispositionType_OpportunityFire,		// OPPORTUNITY_FIRE
			DispositionType_SelfDefense,			// SELF_DEFENSE
			DispositionType_IgnoreEnemies			// IGNORE_ENEMIES
		};
		enum WiggleMoveDirection_t
		{
			WiggleMoveDirection_Forward,
			WiggleMoveDirection_StrafeRight,
			WiggleMoveDirection_Backward,
			WiggleMoveDirection_StrafeLeft
		};
		enum GenerationStateType_t
		{
			GenerationStateType_None,	// Normal process....

			SAMPLE_WALKABLE_SPACE_AND_CREATE_AREAS_FROM_SAMPLES,
			FIND_HIDING_SPOTS_AND_APPROACH_AREAS,
			FIND_ENCOUNTER_SPOTS_AND_SNIPER_SPOTS,
			SAVE_NAV_MESH,

			NUM_GENERATION_STATES
		};

		abstract_class BotState// : public VoidBaseHookedClassWithVirtualFunctionTable
		{
			public:
				virtual       void        OnEnter  (TemplateCSBot *const/* owner*/) { /* Empty */ }//= 0;
				virtual       void        OnUpdate (TemplateCSBot *const/* owner*/) { /* Empty */ }//= 0;
				virtual       void        OnExit   (TemplateCSBot *const/* owner*/) { /* Empty */ }//= 0;
				virtual const char *const GetName  (void)                       { return NULL; }//= 0;	// Pure virtual.
		};
		class IdleState : public BotState
		{
/*			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "Idle"; }
*/		};	// sizeof (IdleState) == 4 (1)
		class HuntState : public BotState
		{
			public:
/*! off=4(1)/2692(673) */				NavigationArea *m_huntArea;
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "Hunt"; }
*/		};	// sizeof (HuntState) == 8 (2)
		class AttackState : public BotState
		{
			public:
/*! off=4(1)/2700(675) */				unsigned int              m_UNKNOWN0_RandomValue;
/*! off=8(2)/2704(676) */				float                     m_UNKNOWN1_Timestamp;
/*! off=12(3)/2708(677) */				Utilities::CountdownTimer m_UNKNOWN2_Timer;
/*! off=20(5)/2716(679) */				float                     m_UNKNOWN3_Timestamp;
/*! off=24(6)/2720(680) */				bool                      m_UNKNOWN4;	// Sets from 'CSBot::m_isEnemyVisible'.
/*! off=25(6.25)/2721(680.25) */				bool                      m_UNKNOWN5;
/*! off=28(7)/2724(681) */				float                     m_startShootingTimestamp;	// Sets from 'Bot::Profile::reactionTime + gpGlobals->time'. After this time elapsed bot will FireWeaponAtEnemy().
/*! off=32(8)/2728(682) */				float                     m_UNKNOWN7_Timestamp;
/*! off=36(9)/2732(683) */				bool                      m_UNKNOWN8;
/*! off=40(10)/2736(684) */				float                     m_UNKNOWN9_Timestamp;
/*! off=44(11)/2740(685) */				bool                      m_crouchAndHoldAttack;
/*! off=45(11.25)/2741(685.25) */				bool                      m_UNKNOWN11;
/*! off=46(11.5)/2742(685.5) */				bool                      m_UNKNOWN12;
/*! off=47(11.75)/2743(685.75) */				bool                      m_UNKNOWN13;
/*! off=48(12)/2744(686) */				bool                      m_UNKNOWN14;
/*! off=52(13)/2748(687) */				Utilities::CountdownTimer m_UNKNOWN15_Timer;
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "Attack"; }
*/		};	// sizeof (AttackState) == 60 (15)
		class InvestigateNoiseState : public BotState
		{
			public:
/*! off=4(1)/2760(690) */				unsigned int m_UNKNOWN[3u];
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "InvestigateNoise"; }
*/		};	// sizeof (InvestigateNoiseState) == 16 (4)
		class BuyState : public BotState
		{
			public:
/*! off=4(1)/2776(694) */				unsigned int m_UNKNOWN[6u];
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "Buy"; }
*/		};	// sizeof (BuyState) == 28 (7)
		class MoveToState : public BotState
		{
			public:
/*! off=4(1)/2804(701) */				Math::Vector3D m_goalPosition;
/*! off=16(4)/2816(704) */				RouteType_t    m_routeType;
/*! off=20(5)/2820(705) */				bool           m_isAlreadySaidGoingToPlantTheBomb;	/// @warning I'M NOT SHURE ABOUT THIS!!!
/*! off=21(5.25)/2821(705.25) */				bool           m_isAlreadySaidPlantingTheBomb;	/// @warning I'M NOT SHURE ABOUT THIS!!!
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "MoveTo"; }
*/		};	// sizeof (MoveToState) == 24 (6)
		class FetchBombState : public BotState
		{
/*			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "FetchBomb"; }
*/		};	// sizeof (FetchBombState) == 4 (1)
		class PlantBombState : public BotState
		{
/*			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "PlantBomb"; }
*/		};	// sizeof (PlantBombState) == 4 (1)
		class DefuseBombState : public BotState
		{
/*			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "DefuseBomb"; }
*/		};	// sizeof (DefuseBombState) == 4 (1)
		class HideState : public BotState
		{
			public:
				enum { MaximumHidingSpotsToPickNumber = 3u };

/*! off=4(1)/2840(710) */				NavigationArea *m_searchArea;
/*! off=8(2)/2844(711) */				float           m_searchRange;
/*! off=12(3)/2848(712) */				Math::Vector3D  m_hiddingSpot;
/*! off=24(6)/2860(715) */				bool            m_isAtSpot;
/*! off=28(7)/2864(716) */				float           m_duration;
/*! off=32(8)/2868(717) */				bool            m_holdPosition;	// Can bot hold position?
/*! off=36(9)/2872(718) */				float           m_holdPositionTimeAmount;
/*! off=40(10)/2876(719) */				bool            m_holdPositionAndWaitingEnemy;
/*! off=44(11)/2880(720) */				float           m_holdPositionStartTimestamp;
/*! off=48(12)/2884(721) */				unsigned int    m_hidingSpotPickTriesNumber;	// Up to 'MaximumHidingSpotsToPickNumber'.
/*! off=52(13)/2888(722) */				Math::Vector3D  m_followingTargetPosition;
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "Hide"; }
*/		};	// sizeof (HideState) == 64 (16)
		class EscapeFromBombState : public BotState
		{
/*			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "EscapeFromBomb"; }
*/		};	// sizeof (EscapeFromBombState) == 4 (1)
		class FollowState : public BotState
		{
			public:
/*! off=4(1)/2908(727) */				EntityHandle              m_leader;
/*! off=12(3)/2916(729) */				Math::Vector3D            m_UNKNOWN0;
/*! off=24(6)/2928(732) */				bool                      m_UNKNOWN1;
/*! off=28(7)/2932(733) */				unsigned int              m_UNKNOWN2;
/*! off=32(8)/2936(734) */				unsigned int              m_UNKNOWN3_Type;
/*! off=36(9)/2940(735) */				unsigned int              m_UNKNOWN4_Timestamp;
/*! off=40(10)/2944(736) */				bool                      m_UNKNOWN5;
/*! off=44(11)/2948(737) */				float                     m_UNKNOWN6_Timestamp;
/*! off=48(12)/2952(738) */				Utilities::CountdownTimer m_UNKNOWN7_Timer;
/*! off=56(14)/2960(740) */				float                     m_UNKNOWN8_Timestamp;
/*! off=60(15)/2964(741) */				bool                      m_UNKNOWN9;
/*! off=64(16)/2968(742) */				float                     m_UNKNOWN10_TimeAmount;
/*! off=68(17)/2972(743) */				Utilities::CountdownTimer m_UNKNOWN11_Timer;
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "Follow"; }
*/		};	// sizeof (FollowState) == 76 (19)
		class UseEntityState : public BotState
		{
			public:
/*! off=4(1)/2984(746) */				EntityHandle m_target;
/*
			public:
				virtual       void        OnEnter  (TemplateCSBot *const owner) = 0;
				virtual       void        OnUpdate (TemplateCSBot *const owner) = 0;
				virtual       void        OnExit   (TemplateCSBot *const owner) = 0;
				virtual const char *const GetName  (void) = 0;	// { return "UseEntity"; }
*/		};	// sizeof (UseEntityState) == 12 (3)
		class CSGameState
		{
			public:
				enum BombState_t
				{
					BombState_AtBomber,
					BombState_Loosed,
					BombState_Planted
				};
/*! @warning SEPARATED VARIABLES - ENSURE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				struct MapZone_t	/// @todo RENAME ME!!!
				{
					bool         isBombsiteClear;
					unsigned int index;	// Index to this zone from 'm_mapZones' array.
				};
*/
				enum { MaximumHostageInfo = 12u };
				struct HostageInfo_t
				{
/*! off=0(0)/92(23)/9380(2345) */					BaseEntity     *entity;
/*! off=4(1)/96(24)/9384(2346) */					Math::Vector3D  origin;
/*! off=16(4)/108(27)/9396(2349) */					bool            isVisible/*isNotGone*/;	/// @warning NOT SHURE ABOUT THIS!!!
/*! off=17(4.25)/109(27.25)/9397(2349.25) */					bool            isValid;	/// @warning NOT SHURE ABOUT THIS!!!
/*! off=18(4.5)/110(27.5)/9398(2349.5) */					bool            isFree;	/// @warning NOT SHURE ABOUT THIS!!!
				};	// sizeof (HostageInfo_t) == 20 (5)

			public:
/*! off=0(0)/9288(2322) */				TemplateCSBot             *m_outer;					// Bot owning this 'CSGameState'.

/*! off=4(1)/9292(2323) */				bool                       m_isRoundOver;
/*! off=8(2)/9296(2324) */				BombState_t                m_bombState;
/*! off=12(3)/9300(2325) */				float                      m_updateBomberTimestamp;
/*! off=16(4)/9304(2326) */				Math::Vector3D             m_bomberOrigin;
/*! off=28(7)/9316(2329) */				float                      m_updateLooseBombTimestamp;
/*! off=32(8)/9320(2330) */				Math::Vector3D             m_loosedBombOrigin;
//*! off=44(11)/9332(2333) */				MapZone_t                  m_mapZones[CSBotManager::MaximumMapZones];
/*! off=44(11)/9332(2333) */				bool                       m_isBombsiteClear[CSBotManager::MaximumMapZones];
/*! off=48(12)/9336(2334) */				unsigned int               m_mapZonesIndexes[CSBotManager::MaximumMapZones];
/*! off=64(16)/9352(2338) */				unsigned int               m_mapZonesNumber;		// Assigned from 'CSBotManager::m_mapZonesNumber' variable.
/*! off=68(17)/9356(2339) */				unsigned int               m_bombsiteIndexToSearch;	// Index of bombsite in a 'CSBot::CSGameState::m_mapZones' array.
/*! off=72(18)/9360(2340) */				int                        m_plantedBombsiteIndex;	// Index of bombsite in a 'CSBotManager::m_mapZones' array. (Can be -1)
/*! off=76(19)/9364(2341) */				bool                       m_isPlantedBombLocationKnown;
/*! off=80(20)/9368(2342) */				Math::Vector3D             m_plantedBombOrigin;
/*! off=92(23)/9380(2345) */				HostageInfo_t              m_hostageInfo[MaximumHostageInfo];
/*! off=332(83)/9620(2405) */				unsigned int               m_hostageInfoNumber;
/*! off=336(84)/9624(2406) */				Utilities::CountdownTimer  m_validateHostagePositionsNextCallTimer;
/*! off=344(86)/9632(2408) */				bool                       m_UNKNOWN0;
/*! off=345(86.25)/9633(2408.25) */				bool                       m_UNKNOWN1;
		};	// sizeof (CSGameState) == 348 (87)
		enum { MaximumHidingSpotsToCheck = 64u };
		struct HidingSpotCheckInfo_t
		{
/*! off=0(0)/9984(2496) */			HidingSpot *spot;
/*! off=4(1)/9988(2497) */			float       checkTimestamp;
		};
		struct AliveVisiblePlayersInfo_t
		{
/*! off=0(0)/10636(2659) */			float setTimestamp;	// Time when was last updated....
/*! off=4(1)/10640(2660) */			bool  isEnemy;
		};	// sizeof (AliveVisiblePlayersInfo_t) == 8 (2)
		enum { MaximumReactionQueue = 20u };
		struct RecognizedEnemyInfo_t
		{
/*! off=0(0)/10944(2736) */			EntityHandle handle;
/*! off=8(2)/10952(2737) */			bool         isReloading;
/*! off=9(2.25)/10953(2737.25) */			bool         isProtectedByShield;
		};	// sizeof (RecognizedEnemyInfo_t) == 12 (3)
		abstract_class BotMeme
		{
			public:
				virtual void Interpret (TemplateCSBot *, TemplateCSBot *) = 0;
		};
		class BotPhrase
		{
			public:
/*! off=4(1) */				unsigned int m_index;	// Type can be as 'Place_t' if 'm_isPlace' equals to true.
/*! off=8(2) */				bool         m_isPlace;

/*! off=24(6) */				BotMeme     *m_meme;
		};	// sizeof (BotPhrase) == 80 (20)
		class BotChatterInterface;
		class BotStatement
		{
			public:
				enum BotStatementType_t
				{
					/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				};
				enum ConditionType_t
				{
					/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				};
				enum ContextType_t
				{
					/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				};

				struct PhraseInfo_t
				{
/*! off=0(0)/52(13) */					bool               isPhrase;	// sets to true if 'phrase', otherwise if 'context'.
					union /* Unnamed */
					{
						BotPhrase     *phrase;
						ContextType_t  context;
/*! off=4(1)/56(14) */					};
				};

/*! off=0(0) */				BotChatterInterface *m_chatterInterface;
/*! off=4(1) */				BotStatement        *m_prev;	/// @warning NOT SHURE!!!!
/*! off=8(2) */				BotStatement        *m_next;	/// @warning NOT SHURE!!!!
/*! off=12(3) */				BotStatementType_t   m_type;
/*! off=16(4) */				int                  m_UNKNOWN0;
/*! off=20(5) */				Place_t              m_place;
/*! off=24(6) */				BotMeme             *m_meme;
/*! off=28(7) */				float                m_UNKNOWN3_Timestamp;	/// (@info Assigned to 'gpGlobals->time' on Statement create.)
/*! off=32(8) */				float                m_UNKNOWN4_Timestamp;	/// (@info Assigned to 'gpGlobals->time' on Statement create.)
/*! off=36(9) */				float                m_UNKNOWN5_Timestamp;	/// (@info Assigned to 'gpGlobals->time + delay' on Statement create.)
/*! off=40(10) */				unsigned int         m_UNKNOWN6[4u];
/*! off=52(13) */				PhraseInfo_t         m_phraseInfos[4u];
/*! off=84(21) */				ConditionType_t      m_conditions[4u];
/*! off=100(25) */				unsigned int         m_conditionsNumber;
/*! off=104(26) */				int                  m_UNKNOWN7;
/*! off=108(27) */				unsigned int         m_phraseInfosNumber;
		};	// sizeof (BotStatement) == 112 (28)
		class BotChatterInterface
		{
			public:
/*! off=0(0)/11296(2824) */				BotStatement  *m_statement;
/*! off=4(1)/11300(2825) */				TemplateCSBot *m_outer;	// Bot owning this 'BotChatterInterface'.
/*! off=8(2)/11304(2826) */				unsigned int   m_UNKNOWN0[12u];
		};	// sizeof (BotChatterInterface) == 56 (14)

	public:
/*! off=2684(671) */		IdleState              m_idleState;
/*! off=2688(672) */		HuntState              m_huntState;
/*! off=2696(674) */		AttackState            m_attackState;											// Executed when variable 'm_isAttacking' is true no metter what....
/*! off=2756(689) */		InvestigateNoiseState  m_investigateNoiseState;
/*! off=2772(693) */		BuyState               m_buyState;
/*! off=2800(700) */		MoveToState            m_moveToState;
/*! off=2824(706) */		FetchBombState         m_fetchBombState;
/*! off=2828(707) */		PlantBombState         m_plantBombState;
/*! off=2832(708) */		DefuseBombState        m_defuseBombState;
/*! off=2836(709) */		HideState              m_hideState;
/*! off=2900(725) */		EscapeFromBombState    m_escapeFromBombState;
/*! off=2904(726) */		FollowState            m_followState;
/*! off=2980(745) */		UseEntityState         m_useEntityState;

/*! off=2992(748) */		BotState              *m_currentState;											// Executed when variable 'm_isAttacking' is false.
/*! off=2996(749) */		float                  m_setStateTimestamp;
/*! off=3000(750) */		bool                   m_isAttacking;
/*! off=3004(751) */		TaskType_t             m_currentTaskID;
/*! off=3008(752) */		EntityHandle           m_taskEntity;
/*! off=3016(754) */		Math::Vector3D         m_goalOrigin;
/*! off=3028(757) */		EntityHandle           m_goalEntity;
/*! off=3036(759) */		NavigationArea        *m_currentArea;
/*! off=3040(760) */		NavigationArea        *m_lastKnownArea;
/*! off=3044(761) */		EntityHandle           m_touchPlayerEntity;
/*! off=3052(763) */		float                  m_touchPlayerTimestamp;
/*! off=3056(764) */		bool                   m_isUNKNOWN0;											// sets to true in CCSBot__DiscontinuityJump() function.... (Is discontinuity jumping?)
/*! off=3057(764.25) */	bool                   m_isUNKNOWN1;											// sets to false in CCSBot__DiscontinuityJump() function and no more used....
/*! off=3060(765) */		float                  m_discontinuityJumpTimestamp;

		enum { MaximumPathNodesNumber = 256u };
		struct PathInformation_t
		{
/*! off=0(0)/3064(766) */			NavigationArea    *area;
/*! off=3(1)/3068(767) */			NavTraverseType_t  traverseType;
/*! off=8(2)/3072(768) */			Math::Vector3D     point;
/*! off=20(5)/3084(771) */			NavigationLadder  *ladder;
		};	// sizeof (PathInformation_t) == 24 (6)

/*! off=3064(766) */		PathInformation_t                  m_path[MaximumPathNodesNumber];				// Path information.
/*! off=9208(2302) */		unsigned int                       m_currentPathInformationNumber;				// Just number of areas in a path....
/*! off=9212(2303) */		unsigned int                       m_currentPathIndex;
/*! off=9216(2304) */		float                              m_currentPathIndexSetTimestamp;				// @todo RENAME ME!
/*! off=9220(2305) */		Utilities::CountdownTimer          m_computePathNextCallTimer;					// Time when function CSBot::ComputePath() can be called.
/*! off=9228(2307) */		Utilities::CountdownTimer          m_isFriendInTheWayNextCallTimer;				// Time when function CSBot::IsFriendInTheWay() can be called.
/*! off=9236(2309) */		bool                               m_isFriendInTheWay;
/*! off=9240(2310) */		Utilities::CountdownTimer          m_resetStuckMonitorNextCallTimer;
/*! off=9248(2312) */		bool                               m_canResetStuckMonitor;
/*! off=9252(2313) */		LadderMovementState_t              m_ladderMovementState;
/*! off=9256(2314) */		bool                               m_ladderApproachFaceIn;						// Always true when ascending on ladder.
/*! off=9260(2315) */		NavigationLadder                  *m_currentLadder;
/*! off=9264(2316) */		LadderDismountAscendingMoveType_t  m_ladderDismountAscendingMoveType;
/*! off=9268(2317) */		float                              m_ladderDismountAscendingMoveStartTimestamp;	// Bot will do move only 0.4 seconds.
/*! off=9272(2318) */		float                              m_ladderEndpoint;
/*! off=9276(2319) */		float                              m_ladderGlobalTimeout;						// Time during which the bot has to overcome the ladder.
/*! off=9280(2320) */		Utilities::CountdownTimer          m_forceRunTimer;								// From CCSBot__ForceRun() function in cs_i386.so only!
/*! off=9288(2322) */		CSGameState                        m_gameState;
/*! off=9636(2409) */		unsigned char                      m_hostageEscortCount;						// Used hostages number.
/*! off=9640(2410) */		float                              m_updateHostageEscortCountTimestamp;
/*! off=9644(2411) */		bool                               m_waitForHostagePatience;					// From CCSBot__ResetWaitForHostagePatience() function in cs_i386.so only!
/*! off=9648(2412) */		Utilities::CountdownTimer          m_checkWaitForHostagePatienceTimer;			// From CCSBot__ResetWaitForHostagePatience() function in cs_i386.so only! (delay is 3.0)
/*! off=9656(2414) */		Utilities::CountdownTimer          m_waitForHostagePatienceTimer;				// (delay is 10.0)
/*! off=9664(2416) */		Math::Vector3D                     m_noisePosition;
/*! off=9676(2419) */		float                              m_noiseHearGlobalTimeDuration;
/*! off=9680(2420) */		NavigationArea                    *m_noiseArea;
/*! off=9684(2421) */		float                              m_shouldInvestigateNoiseCallTimestamp;
/*! off=9688(2422) */		NoisePriority_t                    m_noisePriority;
/*! off=9692(2423) */		bool                               m_isUNKNOWN96;
/*! off=9696(2424) */		float                              m_nextLookAtHidingApproachPointGlobalTime;
/*! off=9700(2425) */		float                              m_lookAheadAngle;
/*! off=9704(2426) */		float                              m_lookForwardAngle;
/*! off=9708(2427) */		float                              m_inhibitLookAroundGlobalTime;				// From CCSBot__InhibitLookAround() function.
/*! off=9712(2428) */		LookAction_t                       m_lookAction;
/*! off=9716(2429) */		Math::Vector3D                     m_lookAtVector;
/*! off=9728(2432) */		PriorityType_t                     m_lookAtPriorityType;
/*! off=9732(2433) */		float                              m_lookAtDuration;
/*! off=9736(2434) */		float                              m_lookAtStartTimestamp;						// Time when bot just aimed to target. After this is set bot will look at target 'm_lookAtDuration' seconds.
/*! off=9740(2435) */		float                              m_lookAtMaximumAngleDifference;				// The maximum amount of angle difference between target and bot look angles. Used for check if bot aimed at target to start look countdown timer.
/*! off=9744(2436) */		bool                               m_lookAtCheckDistance;						// If bot close to the 'm_lookAtVector' more than or equal 100 units - do not look at this vector any more....
/*! off=9748(2437) */		char                              *m_lookAtString;								// "Approach Point (Hiding)", "Encounter Spot", "Last Enemy Position", "Check dangerous noise", "GrenadeThrow", "Nearby enemy gunfire", "Defuse bomb", "Hostage", "Plant bomb on floor", "Use entity", "Breakable", "Panic" or "Noise"
/*! off=9752(2438) */		float                              m_updatePeripheralVisionCallTimestamp;		// (Delay between calls is 0.3 seconds.)
/*! off=9756(2439) */		Math::Vector3D                     m_approachPoints[NavigationArea::MAX_APPROACH_AREAS];
/*! off=9948(2487) */		unsigned char                      m_approachPointsNumber;
/*! off=9952(2488) */		Math::Vector3D                     m_approachPointsCenter;						// Just bot origin vector, used when bot moved on distance more than 50 units to compute approach points for new origin again.
/*! off=9964(2491) */		bool                               m_isThrowingGrenade;							// Sets to true in CCSBot__ThrowGrenade() function.
/*! off=9968(2492) */		Utilities::CountdownTimer          m_throwGrenadeAimTimer;						// Amount of time to aim at throw position. (3.0 seconds)
/*! off=9976(2494) */		SpotEncounter                     *m_currentSpotEncounter;
/*! off=9980(2495) */		float                              m_nextLookAtCurrentSpotEncounterGlobalTime;
/*! off=9984(2496) */		HidingSpotCheckInfo_t              m_hidingSpotsToCheck[MaximumHidingSpotsToCheck];
/*! off=10496(2624) */		unsigned int                       m_hidingSpotsToCheckNumber;
/*! off=10500(2625) */		float                              m_lookAnglePitch;
/*! off=10504(2626) */		float                              m_lookAnglePitchUNKNOWN;
/*! off=10508(2627) */		float                              m_lookAngleYaw;
/*! off=10512(2628) */		float                              m_lookAngleYawUNKNOWN;
/*! off=10516(2629) */		Math::Vector3D                     m_eyePosition;	// Calculated in CSBot::GetEyePosition() function.
/*! off=10528(2632) */		Math::Vector3D                     m_aimOffset;								// (From CCSBot__UpdateAimOffset() function)
/*! off=10540(2635) */		Math::Vector3D                     m_aimOffset2;							// (From CCSBot__SetAimOffset() function)
/*! off=10552(2638) */		float                              m_setAimOffsetNextCallTimestamp;
/*! off=10556(2639) */		float                              m_attackBeginTimestamp;					/// @warning REALLY NOT SHURE ABOUT THIS!!!!!!!!!!!!!!!!!!!!!!!!!
/*! off=10560(2640) */		Math::Vector3D                     m_enemyShootPosition;					/// @warning not shure about this!!!!!!!!!!!!!!!!!!!!!!!!!
/*! off=10572(2643) */		DispositionType_t                  m_disposition;
/*! off=10576(2644) */		Utilities::CountdownTimer          m_ignoreEnemiesTimer;
/*! off=10584(2646) */		EntityHandle                       m_enemy;									// Current enemy.
/*! off=10592(2648) */		bool                               m_isEnemyVisible;						// Is aiming at enemy.
/*! off=10593(2648.25) */		typename baseBotClassType::VisiblePartType_t         m_enemyVisibleBodyParts;	// Enemy visible body part bits. (Calculated by function CSBot::IsVisible())
/*! off=10596(2649) */		Math::Vector3D                     m_lastKnownEnemyPosition;
/*! off=10608(2652) */		float                              m_lastSawEnemyTimestamp;
/*! off=10612(2653) */		float                              m_firstSawEnemyTimestamp;
/*! off=10616(2654) */		float                              m_acquiredCurrentEnemyTimestamp;
/*! off=10620(2655) */		float                              m_enemyDeathTimestamp;
/*! off=10624(2656) */		bool                               m_isEnemyDead;
/*! off=10628(2657) */		unsigned int                       m_nearbyEnemyCount;
/*! off=10632(2658) */		Place_t                            m_enemyPlace;
/*! off=10636(2659) */		AliveVisiblePlayersInfo_t          m_alivePlayersInfo[Constants::MaximumClients];
/*! off=10892(2723) */		EntityHandle                       m_bomber;
/*! off=10900(2725) */		unsigned int                       m_nearbyFriendCount;
/*! off=10904(2726) */		EntityHandle                       m_closestVisibleFriend;
/*! off=10912(2728) */		EntityHandle                       m_closestVisibleHumanFriend;
/*! off=10920(2730) */		BaseEntity                        *m_attacker;								// Current attacker.
/*! off=10924(2731) */		float                              m_attackedTimestamp;
/*! off=10928(2732) */		unsigned int                       m_lastVictimID;
/*! off=10932(2733) */		bool                               m_isAimingAtEnemy;
/*! off=10933(2733.25) */		bool                               m_isRapidFiring;
/*! off=10936(2734) */		float                              m_weaponEquipTimestamp;					// Time of last weapon equip.
/*! off=10940(2735) */		float                              m_fireWeaponAtEnemyNextCallTimestamp;
/*! off=10944(2736) */		RecognizedEnemyInfo_t              m_recognizedEnemiesInfo[MaximumReactionQueue];
/*! off=11184(2796) */		unsigned char                      m_updateReactionQueue_UNKNOWN0;
/*! off=11185(2796.25) */		unsigned char                      m_updateReactionQueue_UNKNOWN1;
/*! off=11186(2796.5) */		unsigned char                      m_currentRecognizedEnemiesQueueIndex;
/*! off=11187(2796.75) */		bool                               m_isStucked;
/*! off=11188(2797) */		float                              m_stuckTimestamp;
/*! off=11192(2798) */		Math::Vector3D                     m_stuckOrigin;
/*! off=11204(2801) */		WiggleMoveDirection_t              m_wiggleMoveDirection;
/*! off=11208(2802) */		float                              m_wiggleMoveInDirectionGlobalDuration;
/*! off=11212(2803) */		float                              m_wiggleJumpTimestamp;					// Set when bot just stucked, in CSBot::Wiggle() function when time expired bot jumps and again sets next jump timestamp.
		enum { MaximumStuckTable = 5u };
/*! off=11216(2804) */		float                              m_distancesToPreviousOriginSinceStuck[MaximumStuckTable];
/*! off=11236(2809) */		unsigned int                       m_distancesToPreviousOriginSinceStuckCurrentIndex0;
/*! off=11240(2810) */		unsigned int                       m_distancesToPreviousOriginSinceStuckCurrentIndex1;
/*! off=11244(2811) */		Math::Vector3D                     m_previousOrigin;						// For CSBot::StuckCheck() function.
/*! off=11256(2814) */		Constants::GameEventType_t         m_currentRadioCommand;
/*! off=11260(2815) */		float                              m_receiveRadioMessageTimestamp;
/*! off=11264(2816) */		float                              m_sendRadioMessageTimestamp;
/*! off=11268(2817) */		EntityHandle                       m_radioMessageSender;
/*! off=11276(2819) */		Math::Vector3D                     m_radioMessageSenderOrigin;
/*! off=11288(2822) */		float                              m_startVoiceFeedbackTimestamp;			// Time chatter icon last showed.
/*! off=11292(2823) */		float                              m_voiceFeedbackGlobalDuration;			// Chatter icon hide time.
/*! off=11296(2824) */		BotChatterInterface                m_chatter;
/*! off=11352(2838) */		Utilities::CountdownTimer          m_UNKNOWN98;
/*! off=11360(2840) */		unsigned int                       m_UNKNOWN99;

	// Navigation mesh....
/*! off=11364(2841) */		NavigationNode                    *m_currentSamplingNode;					// The current navigation node we are sampling from.
/*! off=11368(2842) */		NavDirType_t                       m_currentGenerationDirection;
/*! off=11372(2843) */		NavAreaList::Node_t               *m_currentGenerationNode;					// Used for iterating nav areas during generation process.
/*! off=11376(2844) */		GenerationStateType_t              m_currentGenerationState;				// The state of the generation process.
/*! off=11380(2845) */		Utilities::CountdownTimer          m_UNKNOWN_CountdownTimer0;
/*! off=11388(2847) */		Utilities::CountdownTimer          m_UNKNOWN_CountdownTimer1;
/*! off=11396(2849) */		Utilities::CountdownTimer          m_UNKNOWN_CountdownTimer2;
};	// sizeof (CSBot) == [CS 1.6: 11404 (2851), CS: CZ: 11372 (2843)]

class CSBot      : public TemplateCSBot <Bot>      { /* Empty */ };
class NonCZCSBot : public TemplateCSBot <NonCZBot> { /* Empty */ };

inline       CSBot      *const BaseEntity::GetCSBotPointer      (void)       { return static_cast <CSBot *const> (this); }
inline const CSBot      *const BaseEntity::GetCSBotPointer      (void) const { return static_cast <const CSBot *const> (this); }
inline       NonCZCSBot *const BaseEntity::GetNonCZCSBotPointer (void)       { return static_cast <NonCZCSBot *const> (this); }
inline const NonCZCSBot *const BaseEntity::GetNonCZCSBotPointer (void) const { return static_cast <const NonCZCSBot *const> (this); }

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BOT_INCLUDED