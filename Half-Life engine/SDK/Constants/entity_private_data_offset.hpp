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
// entity_private_data_offset.hpp
//
// Description: Edict->privateData offsets.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_PRIVATE_DATA_OFFSET_INCLUDED
#define ENTITY_PRIVATE_DATA_OFFSET_INCLUDED

enum PlayerAutoHelpHUDText_t
{
//	PlayerAutoHelpHUDText_Unknown                     = BIT (0u),
	PlayerAutoHelpHUDText_RoundStarted                = BIT (1u),	// #Hint_press_buy_to_purchase
	PlayerAutoHelpHUDText_RescueZoneHelp              = BIT (2u),	// #Hint_prevent_hostage_rescue - T / #Hint_rescue_the_hostages - CT
	PlayerAutoHelpHUDText_PressUseSoHostageWillFollow = BIT (3u),	// #Hint_press_use_so_hostage_will_follow - CT
	PlayerAutoHelpHUDText_LeadHostageToRescuePoint    = BIT (4u),	// #Hint_lead_hostage_to_rescue_point - CT
	PlayerAutoHelpHUDText_CarefulAroundHostages       = BIT (5u),	// #Hint_careful_around_hostages
	PlayerAutoHelpHUDText_LostMoney                   = BIT (6u),	// #Hint_lost_money (hostage killed)
	PlayerAutoHelpHUDText_SpottedFriend               = BIT (7u),	// #Hint_spotted_a_friend
	PlayerAutoHelpHUDText_SpottedEnemy                = BIT (8u),	// #Hint_spotted_an_enemy
//	PlayerAutoHelpHUDText_Unknown                     = BIT (9u),
	PlayerAutoHelpHUDText_CarefulAroundTeammates      = BIT (10u),	// #Hint_careful_around_teammates
	PlayerAutoHelpHUDText_WinRoundByKillingEnemy      = BIT (11u),	// #Hint_win_round_by_killing_enemy
	PlayerAutoHelpHUDText_BombRetrieved               = BIT (12u),	// #Hint_you_have_the_bomb
//	PlayerAutoHelpHUDText_Unknown                     = BIT (13u),
//	PlayerAutoHelpHUDText_Unknown                     = BIT (14u),
	PlayerAutoHelpHUDText_OutOfAmmo                   = BIT (15u),	// #Hint_out_of_ammo
	PlayerAutoHelpHUDText_YouAreInTargetZone          = BIT (16u),	// #Hint_you_are_in_targetzone
	PlayerAutoHelpHUDText_HostageRescueZone           = BIT (17u),	// #Hint_hostage_rescue_zone - CT
//	PlayerAutoHelpHUDText_Unknown                     = BIT (18u),
	PlayerAutoHelpHUDText_TeamVIPZone                 = BIT (19u),	// #Hint_terrorist_vip_zone - T / #Hint_ct_vip_zone - CT
//	PlayerAutoHelpHUDText_Unknown                     = BIT (20u),
	PlayerAutoHelpHUDText_OnlyCTCanMoveHostages       = BIT (21u),	// #Only_CT_Can_Move_Hostages - T
	PlayerAutoHelpHUDText_SpecDuck                    = BIT (22u)	// #Spec_Duck
};

enum C4WaveID_t
{
	C4BeepType_0,	// Interval == 1.5, sound name: "weapons/c4_beep1.wav".
	C4BeepType_1,	// Interval == 1.0, sound name: "weapons/c4_beep2.wav".
	C4BeepType_2,	// Interval == 0.8, sound name: "weapons/c4_beep3.wav".
	C4BeepType_3,	// Interval == 0.5, sound name: "weapons/c4_beep4.wav".
	C4BeepType_4,	// Interval == 0.2, sound name: "weapons/c4_beep5.wav".

	C4BeepType_Total
};

// The various states the player can be in during the join game process.
enum PlayerJoiningState_t
{
	// Happily running around in the game.
	// You can't move though if CSGameRules()->IsFreezePeriod() returns true.
	// This state can jump to a bunch of other states like STATE_PICKINGCLASS or STATE_DEATH_ANIM.
	STATE_JOINED,
/// @todo CHECK BELOW VALUES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// This is the state you're in when you first enter the server.
	// It's switching between intro cameras every few seconds, and there's a level info
	// screen up.
	STATE_SHOWLTEXT,			// Show the level intro screen.

	// During these states, you can either be a new player waiting to join, or
	// you can be a live player in the game who wants to change teams.
	// Either way, you can't move while choosing team or class (or while any menu is up).
	STATE_PICKINGTEAM,			// Choosing team.
	STATE_PICKINGCLASS,			// Choosing class.

	STATE_DEATH_ANIM,			// Playing death anim, waiting for that to finish.
	STATE_DEATH_WAIT_FOR_KEY,	// Done playing death anim. Waiting for keypress to go into observer mode.
	STATE_OBSERVER_MODE,		// Noclipping around, watching players, etc.

	NUM_PLAYER_STATES
};

// Things that toggle (buttons/triggers/doors) need this
enum ToggleState_t
{
	ToggleState_AtTop,
	ToggleState_AtBottom,
	ToggleState_GoingUp,
	ToggleState_GoingDown
};

enum TimeBasedDamageType_t
{
	TimeBasedDamageType_Paralyze,
	TimeBasedDamageType_NerveGas,
	TimeBasedDamageType_Poison,
	TimeBasedDamageType_Radiation,
	TimeBasedDamageType_DrownRecover,
	TimeBasedDamageType_Acid,
	TimeBasedDamageType_SlowBurn,
	TimeBasedDamageType_SlowFreeze,

	TimeBasedDamageType_Total
};

enum MonsterState_t
{
	MonsterState_None,
	MonsterState_Idle,
	MonsterState_Combat,
	MonsterState_Alert,
	MonsterState_Hunt,
	MonsterState_Prone,
	MonsterState_Script,
	MonsterState_PlayDead,
	MonsterState_Dead
};

enum ArmouryWeaponID_t
{
	ArmouryWeaponID_MP5,				// HK MP5-Navy
	ArmouryWeaponID_TMP,				// Steyr Tactical Machine Pistol
	ArmouryWeaponID_P90,				// FN P90
	ArmouryWeaponID_MAC10,				// Ingram MAC-10
	ArmouryWeaponID_AK47,				// Automat Kalashnikov AK-47
	ArmouryWeaponID_SG552,				// Sig SG-552 Commando
	ArmouryWeaponID_M4A1,				// Colt M4A1 Carbine
	ArmouryWeaponID_Aug,				// Steyr Aug
	ArmouryWeaponID_Scout,				// Steyr Scout
	ArmouryWeaponID_G3SG1,				// HK G3/SG-1 Sniper Rifle
	ArmouryWeaponID_AWP,				// AI Arctic Warfare/Magnum

	ArmouryWeaponID_M3,					// Benelli M3 Super90
	ArmouryWeaponID_XM1014,				// Benelli XM1014

	ArmouryWeaponID_M249,				// FN M249 Para

	ArmouryWeaponID_FlashBangGrenade,	// Concussion Grenade
	ArmouryWeaponID_ExplosiveGrenade,	// High-Explosive Grenade

	ArmouryWeaponID_Armor,				// Kevlar Vest
	ArmouryWeaponID_ArmorHelm,			// Kevlar Vest and Helmet

	ArmouryWeaponID_SmokeGrenade		// Smoke Grenade
};

enum EntityPrivateDataOffset_t
{
	// for entities with "weapon_*" class name
	OFFSET_WEAPON_OWNER                               = 41u,	// (as Classes::BasePlayer *)
	OFFSET_WEAPON_ID                                  = 43u,	// Get weapon type. Corresponds to WeaponID_t

	OFFSET_WEAPON_NEXT_PRIMARY_ATTACK                 = 46u,	// Soonest time Classes::BasePlayerWeapon::ItemPostFrame() will call Classes::BasePlayerWeapon::PrimaryAttack(). (as float)
	OFFSET_WEAPON_NEXT_SECONDARY_ATTACK               = 47u,	// Soonest time Classes::BasePlayerWeapon::ItemPostFrame() will call Classes::BasePlayerWeapon::SecondaryAttack(). (as float)
	OFFSET_WEAPON_IDLE_TIME                           = 48u,	// Soonest time Classes::BasePlayerWeapon::ItemPostFrame() will call Classes::BasePlayerWeapon::WeaponIdle(). (as float)
	OFFSET_WEAPON_CLIP_AMMO                           = 51u,	// number of shots left in the primary weapon clip, -1 it not used. (as int)
	Offset_Weapon_SpecialReloadState                  = 55u,	// Are we in the middle of a reload for the shotguns. (as unsigned int)
	Offset_Weapon_DefaultAmmo                         = 56u,	// (as unsigned int)
	OFFSET_WEAPON_ACCURACY                            = 62u,	// (as float)
	OFFSET_WEAPON_LAST_FIRE_TIME                      = 63u,	// (as float)
	Offset_Weapon_ShotsFired                          = 64u,	// Pistols reset it to 0 when the attack button is released. (as unsigned int)
	OFFSET_WEAPON_GLOCK18_SHOOT_TIME                  = 69u,	// time to shoot the remaining bullets of the glock18 burst fire. (as float)
	OFFSET_WEAPON_GLOCK18_SHOTS_FIRED                 = 70u,	// used to keep track of the shots fired during the Glock18 burst fire mode. (as unsigned int)
	Offset_Weapon_State                               = 74u,	// (as WeaponState_t)
	Offset_Weapon_NextReloadTime                      = 75u,	// (as float)
	OFFSET_WEAPON_LAST_TAKE_IN_HAND_TIME              = 76u,	// (in fact this is not true for primary weapons) (as float)
//	Offset_Weapon_UNKNOWN                             = 156u,	// (as unsigned short)
	OFFSET_WEAPON_PLAY_EMPTY_SOUND                    = 176u,	// Get weapon type. (as bool, but BOOL)
	OFFSET_WEAPON_PLAY_FIRE_ON_EMPTY                  = 180u,	// True when the gun is empty and the player is still holding down the attack key(s) (as bool, but BOOL)
	OFFSET_WEAPON_IN_RELOAD                           = 216u,	// Are we in the middle of a reload? (as bool, but BOOL)
/*
	// for entities with "weapon_c4" class name
	Offset_WeaponC4_PlantState                        = 41u,	// BIT (0u) when planting, BIT (0u) | BIT (8u) when it's time to create the planted bomb. (as unsigned int)
	Offset_WeaponC4_PlantedTime                       = 43u,	// Get weapon type. (as float)
	Offset_WeaponC4_IsArmingStarted                   = 312u,	// (as bool)

	// for entities with "game_player_equip" class name
	Offset_PlayerEquip_WeaponNames                    = 140u,	// (as char[MaximumPlayerEquip])
	Offset_PlayerEquip_WeaponCount                    = 67u,	// (as unsigned int[MaximumPlayerEquip])
*/
	// for entities with "armoury_entity" class name
	Offset_Armoury_Type                               = 34u,	// (as int, but 'ArmouryWeaponID_t')

	// for entities with "hostage_entity" or "monster_scientist" class name
	OFFSET_HOSTAGE_TARGET                             = 86u,	// Hostage target pointer. (Rescuer) (Note: always NULL for CS: CZ) (as Classes::Edict *, but Classes::EntityHandle)
	OFFSET_HOSTAGE_NEXT_USE                           = 100u,	// get time that the hostage will be next used. (as float)
	Offset_Hostage_IsWasUsed                          = 388u,	// Is hostage was used? (as bool, but BOOL)
	OFFSET_HOSTAGE_IS_RESCUED                         = 392u,	// Is hostage rescued? (as bool)
	OFFSET_Hostage_IsStopped                          = 420u,	// (Note: always 'true' for CS: CZ) (as bool, but BOOL)
	OFFSET_Hostage_SpawnOrigin                        = 106u,	// (as float[3] (Math::Vector3D))
	OFFSET_HOSTAGE_LAST_USE                           = 483u,	// get time that the hostage was last used. (as float)
	OFFSET_Hostage_ID                                 = 487u,	// Unique id of a CS hostage. (as int)
	OFFSET_Hostage_ImprovPointer                      = 490u,	// (as Classes::HostageImprov *)
	OFFSET_HostageImprov_UNKNOWN0                     = 14u,	// Unknown address. (as unsigned int, but Classes::BaseEntity *)
	OFFSET_HostageImprov_UseTime                      = 15u,	// VALID NAME? (as float)
	OFFSET_HostageImprov_UNKNOWN1                     = 63u,	// Unknown address. (as unsigned int, but Classes::BaseEntity *)
	OFFSET_HostageImprov_Target                       = 66u,	// (as Classes::Edict *, but Classes::EntityHandle)

	// for entities with "func_rotating" class name
	Offset_FunctionRotating_FanFriction               = 34u,	// (as float)
	Offset_FunctionRotating_Attenuation               = 35u,	// (as float (from 'SoundAttenuation_t'))
	Offset_FunctionRotating_Volume                    = 36u,	// (as float)
	Offset_FunctionRotating_Pitch                     = 37u,	// (as float)
	Offset_FunctionRotating_Sounds                    = 38u,	// (as unsigned int)

	// for entities with "env_explosion" class name
	Offset_EnvExplosion_Magnitude                     = 96u,	// How large is the fireball? how much damage? (as int)
	Offset_EnvExplosion_SpriteScale                   = 97u,	// What's the exact fireball sprite scale? (as unsigned int)

	// for entities with "func_healthcharger" class name
	Offset_FunctionHealthCharger_NextChargeTime       = 73u,	// (delay is 0.1) (as float)
	Offset_FunctionHealthCharger_Reactivate           = 74u,	// DeathMatch Delay until reactvated (HealthChargerRechargeTime) (as unsigned int)
	Offset_FunctionHealthCharger_Juice                = 75u,	// (as unsigned int)
	Offset_FunctionHealthCharger_State                = 76u,	// 0 = off, 1 = startup, 2 = going (as int)
	Offset_FunctionHealthCharger_SoundTime            = 77u,	// (as float)

	// for entities with "grenade" class name (c4)
	OFFSET_C4_IsStartDefuse                           = 384u,	// is c4 defusing? (as bool)
	Offset_Grenade_IsC4                               = 385u,	// is c4? (as bool)
	OFFSET_C4_DefuseCountDown                         = 99u,	// (as float)
	OFFSET_C4_EXPLODE_TIME                            = 100u,	// get c4 time to explode (as float)
	// Info for the beeping.
	OFFSET_C4_NextFrequencyIntervalTime               = 101u,	// (as float)
	OFFSET_C4_NextBeepTime                            = 102u,	// Beep interval time. (delay is 1.4 second) (as float)
	OFFSET_C4_NextFrequencyTime                       = 103u,	// (as float)
	OFFSET_C4_BeepName                                = 416u,	// (as char *)
	OFFSET_C4_Attenuation                             = 105u,	// (as float)
	// Info for blinking.
	OFFSET_C4_NextBlinkTime                           = 106u,	// (as float)

	OFFSET_C4_NextDefuseTime                          = 107u,	// (as float)
	OFFSET_C4_IsJustBlew                              = 432u,	// (as bool)
	OFFSET_Grenade_HE_OwnerTeam                       = 109u,	// Note: for other type of grenade this always == 0(TeamID_Unassigned)! (as TeamID_t)
	OFFSET_C4_CurrentWave                             = 110u,	// Which sound we're playing. (as unsigned int (from 'C4WaveID_t'))
	OFFSET_C4_CurrentBombTarget                       = 111u,	// (as Classes::Edict *)
	OFFSET_Grenade_Smoke_SmokePuffsNumber             = 112u,	// Increases each second, maximum amount is 21. (as unsigned int)
	OFFSET_Grenade_TypeID                             = 114u,	// grenade type (flash=0, he=25, smoke=26, C4=0) (as GrenadeTypeID_t (but can as 'GrenadeType_t') - Need to truncate the result by GrenadeType_Hand_Total! (GetPrivateDataReference() & GrenadeType_Hand_Total)). Note: For C4 this always == 0! - C4 can be found by 'Offset_Grenade_IsC4'.
	OFFSET_Grenade_Event                              = 115u,	// (as unsigned short)
	OFFSET_Grenade_NumberFloorHits                    = 118u,	// (as unsigned int)
	OFFSET_Grenade_IsRegisteredSound                  = 476u,	// (as bool)
//	Offset_Grenade_UNKNOWN                            = 228u,	// (as unsigned short)

	// for entities with "func_breakable" class name
	Offset_FunctionBreakable_MaterialType             = 36u,	// (as FunctionBreakableMaterialType_t)
	Offset_FunctionBreakable_ExplosionType            = 37u,	// (as FunctionBreakableExplosionType_t)

	// for entities with "trigger_camera" class name
	Offset_TriggerCamera_Player                       = 36u,	// (as Classes::BaseEntity *)
	Offset_TriggerCamera_Target                       = 38u,	// (as Classes::BaseEntity *)
	Offset_TriggerCamera_PathEntity                   = 40u,	// (entity with target name as 'Offset_TriggerCamera_PathTargetName') (as Classes::BaseEntity *)
	Offset_TriggerCamera_PathTargetName               = 41u,	// (as TypeDefinitions::StringOffset_t)
	Offset_TriggerCamera_WaitTime                     = 42u,	// (time for 'Offset_TriggerCamera_ReturnTime') (as float)
	Offset_TriggerCamera_ReturnTime                   = 43u,	// (as float)
	Offset_TriggerCamera_StopTime                     = 44u,	// (as float)
	Offset_TriggerCamera_MoveDistance                 = 45u,	// (as float)
	Offset_TriggerCamera_TargetSpeed                  = 46u,	// (as float)
	Offset_TriggerCamera_InitialSpeed                 = 47u,	// (as float)
	Offset_TriggerCamera_Acceleration                 = 48u,	// (as float)
	Offset_TriggerCamera_Deceleration                 = 49u,	// (as float)
	Offset_TriggerCamera_State                        = 200u,	// (as bool, but BOOL)

	// for entities with "info_map_parameters" class name
	OFFSET_TEAM_BUYING_STATUS                         = 34u,	// (as unsigned int)
	OFFSET_C4_EXPLOSION_RADIUS                        = 35u,	// C4 explosion radius. (as float)

	Offset_Player_HasDisconnected                     = 132u,	// (as bool)

	// for Classes::Armoury entities....
	Offset_Armoury_Item                               = 34u,	// (as unsigned int)
	Offset_Armoury_Count                              = 35u,	// (as unsigned int)
	Offset_Armoury_TotalCount                         = 36u,	// (as unsigned int)
	Offset_Armoury_IsRestarted                        = 148u,	// (as bool)

	// for Classes::BaseDelay entities....
	Offset_BaseDelay_Delay                                = 34u,	// (as float)
	Offset_BaseDelay_KillTarget                           = 35u,	// (as TypeDefinitions::StringOffset_t)

		// for Classes::BaseAnimating entities....
		// animation needs
		Offset_BaseAnimating_FrameRate                    = 36u,	// Computed FPS for current sequence. (as float)
		Offset_BaseAnimating_GroundSpeed                  = 37u,	// Computed linear movement rate for current sequence. (as float)
		Offset_BaseAnimating_LastEventCheckTime           = 38u,	// Last time the event list was checked. (as float)
		Offset_BaseAnimating_IsSequenceFinished           = 156u,	// Flag set when StudioAdvanceFrame moves across a frame boundry. (as bool, but BOOL)
		Offset_BaseAnimating_IsSequenceLoops              = 160u,	// True if the sequence loops. (as bool, but BOOL)

			// for Classes::BaseToggle entities....
			Offset_BaseToggle_State                       = 41u,	// (as ToggleState_t)
			Offset_BaseToggle_ActivateFinished            = 42u,	// Like attack_finished, but for doors. (as float)
			Offset_BaseToggle_MoveDistance                = 43u,	// How far a door should slide or rotate. (as float)
			Offset_BaseToggle_Wait                        = 44u,	// (as float)
			Offset_BaseToggle_Lip                         = 45u,	// (as float)

			// for plats
			Offset_BaseToggle_Width                       = 46u,	// (as float)
			Offset_BaseToggle_Length                      = 47u,	// (as float)

			Offset_BaseToggle_Position1                   = 48u,	// (as float[3] (Math::Vector3D))
				Offset_BaseToggle_Position1_x             = Offset_BaseToggle_Position1,		// (as float)
				Offset_BaseToggle_Position1_y             = Offset_BaseToggle_Position1 + 1u,	// (as float)
				Offset_BaseToggle_Position1_z             = Offset_BaseToggle_Position1 + 2u,	// (as float)
			Offset_BaseToggle_Position2                   = 51u,	// (as float[3] (Math::Vector3D))
				Offset_BaseToggle_Position2_x             = Offset_BaseToggle_Position2,		// (as float)
				Offset_BaseToggle_Position2_y             = Offset_BaseToggle_Position2 + 1u,	// (as float)
				Offset_BaseToggle_Position2_z             = Offset_BaseToggle_Position2 + 2u,	// (as float)
			Offset_BaseToggle_Angle1                      = 54u,	// (as float *, but Math::Angles3D)
				Offset_BaseToggle_Angle1_x                = Offset_BaseToggle_Angle1,		// (as float)
				Offset_BaseToggle_Angle1_y                = Offset_BaseToggle_Angle1 + 1u,	// (as float)
				Offset_BaseToggle_Angle1_z                = Offset_BaseToggle_Angle1 + 2u,	// (as float)
			Offset_BaseToggle_Angle2                      = 57u,	// (as float *, but Math::Angles3D)
				Offset_BaseToggle_Angle2_x                = Offset_BaseToggle_Angle2,		// (as float)
				Offset_BaseToggle_Angle2_y                = Offset_BaseToggle_Angle2 + 1u,	// (as float)
				Offset_BaseToggle_Angle2_z                = Offset_BaseToggle_Angle2 + 2u,	// (as float)
			Offset_BaseToggle_TriggersLeft                = 60u,	// trigger_counter only, # of activations remaining. (as unsigned int)
			Offset_BaseToggle_Height                      = 61u,	// (as float)
			Offset_BaseToggle_Activator                   = 62u,	// (as Classes::Edict *, but Classes::EntityHandle)
			Offset_BaseToggle_FunctionCallWhenMoveDone    = 64u,	// (as void *, but void (Classes::BaseToggle::*m_functionCallWhenMoveDone) (void))
			Offset_BaseToggle_FinalDestination            = 65u,	// (as float[3] (Math::Vector3D))
				Offset_BaseToggle_FinalDestination_x      = Offset_BaseToggle_FinalDestination,			// (as float)
				Offset_BaseToggle_FinalDestination_y      = Offset_BaseToggle_FinalDestination + 1u,	// (as float)
				Offset_BaseToggle_FinalDestination_z      = Offset_BaseToggle_FinalDestination + 2u,	// (as float)
			Offset_BaseToggle_FinalAngle                  = 68u,	// (as float *, but Math::Angles3D)
				Offset_BaseToggle_FinalAngle_x            = Offset_BaseToggle_FinalAngle,		// (as float)
				Offset_BaseToggle_FinalAngle_y            = Offset_BaseToggle_FinalAngle + 1u,	// (as float)
				Offset_BaseToggle_FinalAngle_z            = Offset_BaseToggle_FinalAngle + 2u,	// (as float)
			Offset_BaseToggle_DamageInflictBits           = 71u,	// Damage type that the door or tigger does. (as unsigned int (flags from 'DamageType_t'))
			Offset_BaseToggle_Master                      = 72u,	// If this button has a master switch, this is the target name. (as TypeDefinitions::StringOffset_t)
																	// A master switch must be of the multisource type. If all
																	// of the switches in the multisource have been triggered, then
																	// the button will be allowed to operate. Otherwise, it will be deactivated.

				// for Classes::BaseMonster entities....
				Offset_BaseMonster_Activity                     = 73u,	// What the monster is doing (animation). (as Activity_t)
				Offset_BaseMonster_IdealActivity                = 74u,	// monster should switch to this activity (as Activity_t)
				Offset_BaseMonster_LastHitGroup                 = 75u,	// the last body region that took damage (as PlayerBodyPart_t)
				Offset_BaseMonster_LastDamageTypeBits           = 76u,	// What types of damage has monster taken. (as DamageType_t)
				Offset_BaseMonster_TimeBasedDamage              = 308u,	// (as unsigned char[TimeBasedDamageType_Total])
				Offset_BaseMonster_MonsterState                 = 79u,	// monster's current state (as MonsterState_t)
				Offset_BaseMonster_IdealMonsterState            = 80u,	// monster should change to this state (as MonsterState_t)
				Offset_BaseMonster_Conditions                   = 81u,	// (as unsigned int)
				Offset_BaseMonster_Memory                       = 82u,	// (as unsigned int)
				Offset_BaseMonster_NextAttackDelay              = 83u,	// get client next weapon attack delay time (can't attack again until this time) (as float)
				Offset_BaseMonster_Enemy                        = 84u,	// the entity that the monster is fighting. (as Classes::Edict *, but Classes::EntityHandle)
				Offset_BaseMonster_Target                       = 86u,	// the entity that the monster is trying to reach. (as Classes::Edict *, but Classes::EntityHandle)
				Offset_BaseMonster_FieldOfView                  = 88u,	// width of monster's field of view (dot product). (as float)
				Offset_BaseMonster_BloodColor                   = 89u,	// Color of blood particless. (as unsigned int)

				// HACK until we can query end of gun
				Offset_BaseMonster_HackedGunPosition            = 90u,	// (as float[3] (Math::Vector3D))
					Offset_BaseMonster_HackedGunPosition_x      = Offset_BaseMonster_HackedGunPosition,			// (as float)
					Offset_BaseMonster_HackedGunPosition_y      = Offset_BaseMonster_HackedGunPosition + 1u,	// (as float)
					Offset_BaseMonster_HackedGunPosition_z      = Offset_BaseMonster_HackedGunPosition + 2u,	// (as float)

				// last known position of enemy. (enemy's origin)
				Offset_BaseMonster_EnemyLastKnownPosition       = 93u,	// (as float[3] (Math::Vector3D))
					Offset_BaseMonster_EnemyLastKnownPosition_x = Offset_BaseMonster_EnemyLastKnownPosition,		// (as float)
					Offset_BaseMonster_EnemyLastKnownPosition_y = Offset_BaseMonster_EnemyLastKnownPosition + 1u,	// (as float)
					Offset_BaseMonster_EnemyLastKnownPosition_z = Offset_BaseMonster_EnemyLastKnownPosition + 2u,	// (as float)

	// CBasePlayer

	// for entities with "player" class name
	Offset_Player_RandomSeed                          = 96u,	// See that is shared between client & server for shared weapons code. (as unsigned int)

	Offset_Player_ObserverTarget                      = 98u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_Player_NextObserverInput                   = 100u,	// (as float)
	Offset_Player_SpectatedPlayerWeaponID             = 101u,	// (as WeaponID_t)
	Offset_Player_SpectatedPlayerBombState            = 102u,	// 0 = hasn't bomb, 1 = has, 2 = has bomb and in target zone (as Classes::BasePlayer::SpectatedPlayerBombState_t)
	Offset_Player_IsSpectatedPlayerHasDefuseKit       = 412u,	// (as bool)
	Offset_Player_SpectatorViewType                   = 104u,	// (as SpectatorObserverMode_t)

//	OFFSET_IS_LIGHT_DAMAGED                           = 428u,	// (as bool)
	Offset_Player_VelocityModifier                    = 108u,	// (value between 0.0 and 1.0) (as float)
	Offset_Player_LastZoom                            = 109u,	// After firing a shot, set the FOV to 90, and after showing the animation, bring the FOV back to last zoom level. (last field of view) (as unsigned int)
	Offset_Player_ResumeZoom                          = 440u,	// (as bool)
	Offset_Player_EjectBrass                          = 111u,	// Time to eject a shell. This is used for guns that do not eject shells right after being shot. (as float)
	OFFSET_PLAYER_ARMOR_TYPE                          = 112u,	// get client armor type (as PlayerArmorType_t)
	OFFSET_IS_NOT_KILLED                              = 452u,	// Has the player been killed or has he just respawned. (as bool)
	OFFSET_TEAM                                       = 114u,	// get client team (as unsigned int)
	OFFSET_MONEY                                      = 115u,	// get client money amount (as unsigned int)
	OFFSET_PLAYER_HAS_PRIMARY_WEAPON                  = 464u,	// (as bool)

	OFFSET_PLAYER_LAST_CHAT_TIME                      = 119u,	// (delay is 0.66) (as float)
	OFFSET_IS_JUST_CONNECTED                          = 480u,	// Is client just connected? (as bool)
	Offset_Player_JoiningState                        = 121u,	// (as PlayerJoiningState_t)

	// When the player joins, it cycles their view between trigger_camera entities.
	// This is the current camera, and the time that we'll switch to the next one.
	Offset_Player_IntroCamera                         = 122u,	// (as Classes::BaseEntity *)
	Offset_Player_IntroCameraTime                     = 123u,	// (updates every 6 seconds) (as float)

	Offset_Player_LastMovementTime                    = 124u,	// Amount of time we've been motionless. (as float)
	OFFSET_PLAYER_IS_MISSION_BRIEFING_DISPLAYED       = 500u,	// (as bool)
	OFFSET_IS_TEAM_CHANGED                            = 501u,	// This is set to false every death. Players are restricted to one team change per death (when dead) (as bool)
	OFFSET_PLAYER_INTERNAL_MODEL                      = 126u,	// (as unsigned int)
	OFFSET_PLAYER_TEAM_KILL_NUMBER                    = 127u,	// (as unsigned int)
	OFFSET_PLAYER_IGNORE_MESSAGES                     = 128u,	// 0 = Accept All Messages, 1 = Ignore Broadcast Messages, 2 = Ignore Broadcast Team Messages (as unsigned int)
	OFFSET_HAS_NVGOGGLES                              = 516u,	// is client has night vision goggles (as bool)
	OFFSET_USES_NVGOGGLES                             = 517u,	// get client night vision goggles state (as bool)

	Offset_Player_IdleCheckTime                       = 190u,	// (updates every 5.0 seconds) (as float)
	OFFSET_NEXT_RADIO_TIME                            = 191u,	// get next radio message time (as float)
	OFFSET_RADIO_COMMANDS_USE_AMOUNT                  = 192u,	// get radio commands amount (it resets to 60 in each player spawn) (as unsigned int)
	OFFSET_CANT_HEAR_RADIO                            = 772u,	// is client can't hear radio commands (as bool)
	OFFSET_CAN_PLANT_BOMB                             = 773u,	// is client can plant a bomb if he has one (as bool)
	OFFSET_HAS_DEFUSE_KIT                             = 774u,	// is client has defuse kit (as bool)
	Offset_Player_HasDiedByExplosion                  = 775u,	// (as bool)

	Offset_Player_HasDiedByHEGrenade                  = 788u,	// (as bool)

	//--> Memory START
//	Offset_Player_HintMessagesQueue                   = 198u,
//	Offset_Player_HintMessagesQueue                   = 792u,	// (as char *)
//	Offset_Player_HintMessages                        = 199u,
//	Offset_Player_HintMessages                        = 796u,
	//<-- Memory END

//	OFFSET_PLAYER_HintMessagesQueue                   = 198u,	// (as HintMessageQueue)
//	OFFSET_PLAYER_HintMessages                        = 199u,	// (as CUtlMemory)
//	OFFSET_PLAYER_UNKNOWN0                            = 200u,	// (as unsigned int)
//	OFFSET_PLAYER_UNKNOWN1                            = 201u,	// (as unsigned int)
//	OFFSET_PLAYER_UNKNOWN2                            = 202u,	// (as unsigned int)
//	OFFSET_PLAYER_UNKNOWN3                            = 203u,	// (as unsigned int)

	Offset_Player_DisplayAutoHelpHUDTextHistoryBits   = 204u,	// (as unsigned int (flags from 'PlayerAutoHelpHUDText_t'))
	OFFSET_PLAYER_OPENED_MENU                         = 205u,	// (as MenuID_t)

	OFFSET_IS_ESCAPED                                 = 836u,	// Has this terrorist/VIP escaped yet? (as bool)
	OFFSET_IS_VIP                                     = 837u,	// Are we the VIP? (as bool)
	OFFSET_PLAYER_NEXT_RADAR_UPDATE_TIME              = 210u,	// (as float)

	OFFSET_PLAYER_LAST_POSITION                       = 211u,	// (as float[3] (Math::Vector3D))
		OFFSET_PLAYER_LAST_POSITION_X                 = OFFSET_PLAYER_LAST_POSITION,		// (as float)
		OFFSET_PLAYER_LAST_POSITION_Y                 = OFFSET_PLAYER_LAST_POSITION + 1u,	// (as float)
		OFFSET_PLAYER_LAST_POSITION_Z                 = OFFSET_PLAYER_LAST_POSITION + 2u,	// (as float)

	Offset_Player_LastNominatedToKickPlayerUserID     = 214u,	// (vote command) (as unsigned int)
	OFFSET_PLAYER_NEXT_VOTE_USAGE_TIME                = 215u,	// (for "vote" or "votemap" commands) (as float)
	OFFSET_IS_JUST_KILLED_TEAMMATE                    = 864u,	// (as bool)
	OFFSET_PLAYER_HOSTAGE_KILLS_NUMBER                = 217u,	// (as unsigned int)
	OFFSET_PLAYER_LAST_VOTED_MAP                      = 218u,	// (votemap command) (as unsigned int)
	Offset_Player_IsPrimaryFireAllowed                = 876u,	// (as bool)
	OFFSET_PLAYER_LAST_ATTACK_TIME                    = 220u,	// Game time of last weapon attack, including nade trhow and c4 start planting. (as float)

	OFFSET_IS_WAS_KILLED_WITH_HEADSHOT                = 888u,	// (as bool)
	OFFSET_IS_PUNISHED_FOR_TEAM_KILL                  = 889u,	// (as bool)
	Offset_Player_IsMarkedAsNotReceivingMoney         = 890u,	// (as bool)

	OFFSET_IS_DEFUSING                                = 929u,	// Tracks whether this player is currently defusing a bomb (as bool)

	Offset_Player_MapZone                             = 234u,	// get client special zones bitmask (as MapZone_t)
	Offset_Player_ClientMapZone                       = 235u,	// get client special zones bitmask of client (as MapZone_t)
	OFFSET_PLAYER_LAST_TOUCHED_BOMB_TARGET            = 236u,	// get client last touched bomb target entity edict (nulled on each round restart) (as Classes::Edict *)
	Offset_Player_SoundIndex                          = 237u,	// the index of the sound list slot reserved for this player (as unsigned int)
	OFFSET_Player_GroundSpeed/*TargetVolume*/                         = 238u,	// Ideal sound volume. (as float/*int*/)
	Offset_Player_WeaponVolume                        = 239u,	// how loud the player's weapon is right now. (as int)
	Offset_Player_ExtraSoundTypes                     = 240u,	// additional classification for this weapon's sound (as int)
	OFFSET_WEAPON_FLASH                               = 241u,	// get brightness of the weapon flash (as unsigned int)
	Offset_Player_StopExtraSoundTime                  = 242u,	// (as float)
	Offset_Player_FlashlightTime                      = 243u,	// Time until next battery draw/Recharge (as float)
	OFFSET_FLASH_LIGHT_BATTERY                        = 244u,	// get client falshlight battery amount (as unsigned int)
	Offset_Player_ButtonsLast                         = 245u,	// (as ButtonType_t)
	Offset_Player_ButtonsPressed                      = 246u,	// The changed ones still down are "pressed". (as ButtonType_t)
	Offset_Player_ButtonsReleased                     = 247u,	// The ones not down are "released". (as ButtonType_t)
	Offset_Player_SoundLast                           = 248u,	// Last sound entity to modify player room type. (as Edict *)
	Offset_Player_SoundRoomType                       = 249u,	// Last roomtype set by sound entity. (as float)
	Offset_Player_SoundRange                          = 250u,	// Distance from player to sound entity. (as float)
	OFFSET_FALL_VELOCITY                              = 251u,	// get client fall velocity (as float)
	Offset_Player_Items                               = 252u,	// (as int[PlayerItemType_Total])
	OFFSET_PLAYER_PHYSICS_FLAGS                       = 257u,	// physics flags - set when 'normal' physics should be revisited or overriden (as unsigned int (flags from 'PlayerPhysicsFlag_t'))
	OFFSET_PLAYER_NEXT_SUICIDE_TIME                   = 258u,	// the time after which the player can next use the suicide command (increases by 1.0 second) (as float)
	OFFSET_PLAYER_STEP_SOUND_TIME                     = 259u,	// when the last stepping sound was made. (as float)
	OFFSET_PLAYER_LAST_DAMAGE_AMOUNT                  = 334u,	// Last damage taken. (as unsigned int)
	Offset_Player_TimeBasedDamagePreviousTime         = 335u,	// Time-based damage timer. (as float)

	OFFSET_Player_IsNeedInitializeHUD                 = 1392u,	// True when deferred HUD restart message needs to be sent. (as bool, but BOOL)
	OFFSET_Player_IsNeedInitializeGameHUD             = 1396u,	// Player joined? (as bool, but BOOL)
	OFFSET_PLAYER_TRAIN_FLAGS                         = 350u,	// Train control position. (as unsigned int (flags from 'TrainFlag_t'))
//	Offset_Player_Time                                = 352u,	// (as float)
	OFFSET_PLAYER_DEAD_TIME                           = 354u,	// the time at which the player died (used in Classes::BasePlayer::PlayerDeathThink()) (as float)
	Offset_Player_HideHUD                             = 361u,	// the players hud weapon info is to be hidden (as unsigned int)
	Offset_Player_ClientHideHUD                       = 362u,	// (as unsigned int)
	OFFSET_PLAYER_STATIONARY                          = 362u,	// (as int)
	Offset_Player_FieldOfView                         = 363u,	// get client field of view. (as unsigned int)
	Offset_Player_ClientFieldOfView                   = 364u,	// client's known field of view. (as unsigned int)
	OFFSET_SPAWNS_NUMBER                              = 365u,	// Number of times player has spawned this round. (as unsigned int)
//	Offset_Player_UNKNOWN_ENTITY                      = 366u,	// (If not NULL removed in HalfLifeMultiplay::ClientDisconnect() by BaseEntity::SUB_Remove()) (as BaseEntity *)
	OFFSET_PLAYER_ITEMS                               = 367u,	// (as Classes::BasePlayerItem *[MaximumItemTypes])
	OFFSET_PLAYER_LAST_PRIMARY_ITEM                   = 368u,	// (as Classes::BasePlayerItem *)
	OFFSET_PLAYER_LAST_SECONDARY_ITEM                 = 369u,	// (as Classes::BasePlayerItem *)
	OFFSET_PLAYER_LAST_KNIFE_ITEM                     = 370u,	// (as Classes::BasePlayerItem *)
	OFFSET_PLAYER_LAST_GRENADE_ITEM                   = 371u,	// (as Classes::BasePlayerItem *)
	OFFSET_PLAYER_LAST_C4_ITEM                        = 372u,	// (as Classes::BasePlayerItem *)
	OFFSET_PLAYER_ACTIVE_ITEM                         = 373u,	// (as Classes::BasePlayerItem *)
	OFFSET_PLAYER_LAST_ITEM                           = 375u,	// (as Classes::BasePlayerItem *)
	OFFSET_PLAYER_AMMO                                = 376u,	// shared ammo slots. (as unsigned int[MaximumWeapons])
	OFFSET_PLAYER_AMMO_LAST                           = 408u,	// shared ammo slots. (as unsigned int[MaximumWeapons])
	OFFSET_DEATHS                                     = 444u,	// get client deaths

//	Offset_Player_UNKNOWN                             = 1784u,	// (Something in Classes::BasePlayer::UpdateStatusBar()) (as char *)
//	Offset_Player_UNKNOWN                             = 447u,	// (as unsigned int)
//	Offset_Player_UNKNOWN                             = 448u,	// (as unsigned int)
//	Offset_Player_UNKNOWNTime                         = 450u,	// (as float)
	Offset_Player_StatusText                          = 1804u,	// (as char *)
	OFFSET_PLAYER_NEXT_DECAL_TIME                     = 486u,	// Next time this player can spray a decal (as float)

	OFFSET_PLAYER_ANIMATION_EXTENTION                 = 1968u,	// (as char[32])

	Offset_Player_StudioBlendPitch                    = 506u,	// (as float)
	Offset_Player_StudioBlendYaw                      = 507u,	// (as float)

	OFFSET_PLAYER_AUTO_WEAPON_SWITCH                  = 2036u,	// (setinfo _cl_autowepswitch 1) (as bool, but BOOL)

	OFFSET_PLAYER_VGUI_MENUS                          = 2040u,	// (setinfo _vgui_menus 1) (as bool)
	OFFSET_PLAYER_AUTO_HELP                           = 2041u,	// Auto-help. If true - prints "You have spotted an enemy", etc.... (setinfo _ah 1) (as bool)
	OFFSET_USES_SHIELD                                = 2042u,	// get client shield state (as bool)
	OFFSET_HAS_SHIELD                                 = 2043u,	// is client has tactical shield (as bool)

	Offset_Player_ObserverFindNextPlayerDelayTime     = 512u,	// (as float)

	Offset_Player_BlindEndTime                        = 514u,	// (as float)
	Offset_Player_BlindStartTime                      = 515u,	// (as float)
	Offset_Player_BlindFadeHold                       = 516u,	// (as float)
	Offset_Player_BlindFadeTime                       = 517u,	// (as float)
	Offset_Player_BlindAlpha                          = 518u,	// (as unsigned int)

//	Offset_Player_AutoBuyData                         = 2080u,	// (as char *)

//	Offset_Player_RebuyData                           = 2336u,	// (as char *)

//	Offset_Player_IS_UNKNOWN                          = 2380u,	// (as bool)

	Offset_Player_ProgressBarStartTime                = 605u,	// (as float)
	Offset_Player_ProgressBarEndTime                  = 606u,	// (as float)

	Offset_Player_IsSpecSetAD                         = 2428u,	// is spec_set_ad > 0.0 (Command: "spec_set_ad") (as bool)
	OFFSET_Player_IsCanSwitchObserverModes            = 2429u,	// (as bool)
/*
	/// @warning Below variables are does not exits in Counter-Strike: Condition Zero 'BasePlayer' class!!!
		OFFSET_Player_NextChatUseTime                 = 612u,	// get client next chat time (as float)
		OFFSET_Player_NextTeamChatUseTime             = 613u,	// get client next team chat time (as float)
		OFFSET_Player_NextFullUpdateCommandUseTime    = 614u,	// (Command: "fullupdate") (as float)
		OFFSET_Player_NextVoteKickTime                = 615u,	// get client next team chat time (as float)
		OFFSET_Player_NextVoteMapTime                 = 616u,	// get client next vote map time (as float)
		Offset_Player_NextListMapsCommandUseTime      = 617u,	// (Command: "listmaps") (Delay is 0.3 sec.) (as float)
		Offset_Player_NextListPlayersCommandUseTime   = 618u,	// (Command: "listplayers") (Delay is 0.3 sec.) (as float)
		Offset_Player_NextNightVisionUseTime          = 619u,	// get client next night vision use time (Command: "nightvision") (Delay is 0.3 sec.) (as float)

	Offset_Player_DifferenceBetweenOriginalAndConditionZero = 8u,	// (as unsigned int, or 32 as unsigned char)
*/
//	Player sizeof == [CS 1.6: 2480 (620), CS: CZ: 2448 (612)]

	// CBot

	Offset_CSBot_VirtualFunctionTablePointer          = 196u,	// Offset to 'Classes::CSBot' virtual table relative to 'Classes::BasePlayer' virtual table, cuz we can't just create CSBot entity to hook it's virtual table....

	Offset_Bot_Profile                                = 620u,	// Pointer to bot profile. (as Classes::Bot::Profile *)
	Offset_Bot_ID                                     = 621u,	// Unique bot ID. (Started from 1) (Used in Classes::CSBotManager::GetPlayerPriority()) (as unsigned int)
	Offset_Bot_NextCommandExecutionTime               = 622u,	// Time of next call Classes::Bot::Upkeep() and Classes::Bot::ExecuteCommand() functions. (as float)
	Offset_Bot_NextThinkExecutionTime                 = 623u,	// Time of next call Classes::Bot::Update() function. (as float)
	Offset_Bot_LastThinkTime                          = 624u,	// Time bot executed Classes::Bot::ExecuteCommand() function. (as float)
	Offset_Bot_IsDoRun                                = 2500u,	// (as bool)
	Offset_Bot_IsDoCrouch                             = 2501u,	// (as bool)
	Offset_Bot_SpeedMove                              = 626u,	// Forward/backward speed. (as float)
	Offset_Bot_SpeedStrafe                            = 627u,	// Side speed. (as float)
	Offset_Bot_SpeedVertical                          = 628u,	// Upwad/downward speed. (in water) (as float)
	Offset_Bot_PressedButtons                         = 1258u,	// (as unsigned short (from 'ButtonType_t'))
	Offset_Bot_JumpTimestamp                          = 630u,	// (as float)

	// CCSBot

	Offset_CSBot_CombatRange                          = 652u,	// Combat range. (as float)
	Offset_CSBot_IsRogue                              = 2612u,	// (as bool)
	Offset_CSBot_IsRogueLastCheckTimestamp            = 654u,	// (as float)
	Offset_CSBot_IsRogueCheckTime                     = 655u,	// (as float)

	Offset_CSBot_Morale                               = 656u,	// (-3 ... 3) (as Classes::Bot::MoraleType_t)
	Offset_CSBot_IsDead                               = 2628u,	// (as bool)
	Offset_CSBot_SafeEndTime                          = 658u,	// (as float)
	Offset_CSBot_IsSafe                               = 2636u,	// (as bool)
	Offset_CSBot_BlindAction                          = 660u,	// (as Classes::Bot::BlindActionType_t)
	Offset_CSBot_BlindDoPrimaryAttack                 = 2644u,	// (as bool)
	Offset_CSBot_SurpriseTime                         = 662u,	// Enemy surprise time. (as float)
	Offset_CSBot_SurprisedTime                        = 663u,	// My surprise time. (as float)
	Offset_CSBot_IsFollowing                          = 2656u,	// (as bool)
	Offset_CSBot_FollowingTarget                      = 665u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_FollowingStartTime                   = 667u,	// (as float)
	Offset_CSBot_NextAutoFollowTime                   = 668u,	// From CCSBot__CanAutoFollow() function in cs_i386.so only! (as float)
	Offset_CSBot_HurryTimeAmount                      = 669u,	// From CCSBot__Hurry() function in cs_i386.so only! (as float)
	Offset_CSBot_HurryGlobalTime                      = 670u,	// From CCSBot__Hurry() function in cs_i386.so only! (as float)

	///@ingroup Bot States....
	///@{
	Offset_CSBot_IdleState                            = 671u,	// (as Classes::CSBot::IdleState)
	Offset_CSBot_HuntState                            = 672u,	// (as Classes::CSBot::HuntState)
		Offset_CSBot_HuntState_HuntArea                   = 673u/*4(1)*/,	// (as Classes::NavigationArea *)
	Offset_CSBot_AttackState                          = 674u,	// Executed when variable 'Offset_CSBot_IsAttacking' is true no metter what.... (as Classes::CSBot::AttackState)
		Offset_CSBot_AttackState_UNKNOWN0                 = 675u/*4(1)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN1                 = 676u/*8(2)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN2                 = 677u/*12(3)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN3                 = 678u/*16(4)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN4                 = 679u/*20(5)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_IsEnemyVisible           = 2720u/*24(6)*/,	// Assigned from 'Offset_CSBot_IsEnemyVisible' (as bool)
		Offset_CSBot_AttackState_UNKNOWN5                 = 2721u/*25(6.25)*/,	// (as bool)
		Offset_CSBot_AttackState_UNKNOWN6                 = 681u/*28(7)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN7                 = 682u/*32(8)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN8                 = 683u/*36(9)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN9                 = 684u/*40(10)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN10                = 2740u/*44(11)*/,	// From AttackState__SetCrouchAndHold() function in cs_i386.so only! (as bool)
		Offset_CSBot_AttackState_UNKNOWN11                = 2741u/*45(11.25)*/,	// (as bool)
		Offset_CSBot_AttackState_UNKNOWN12                = 2742u/*46(11.5)*/,	// (as bool)
		Offset_CSBot_AttackState_UNKNOWN13                = 2743u/*47(11.75)*/,	// (as bool)
		Offset_CSBot_AttackState_UNKNOWN14                = 686u/*48(12)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN15                = 687u/*52(13)*/,	// (as unsigned int)
		Offset_CSBot_AttackState_UNKNOWN16                = 688u/*56(14)*/,	// (as unsigned int)
	Offset_CSBot_InvestigateNoiseState                = 689u,	// (as Classes::CSBot::InvestigateNoiseState)
		Offset_CSBot_InvestigateNoiseState_UNKNOWN0       = 690u/*4(1)*/,	// (as unsigned int)
		Offset_CSBot_InvestigateNoiseState_UNKNOWN1       = 691u/*8(2)*/,	// (as unsigned int)
		Offset_CSBot_InvestigateNoiseState_UNKNOWN2       = 692u/*12(3)*/,	// (as unsigned int)
	Offset_CSBot_BuyState                             = 693u,	// (as Classes::CSBot::BuyState)
		Offset_CSBot_BuyState_UNKNOWN0                    = 694u/*4(1)*/,	// (as unsigned int)
		Offset_CSBot_BuyState_UNKNOWN1                    = 695u/*8(2)*/,	// (as unsigned int)
		Offset_CSBot_BuyState_UNKNOWN2                    = 696u/*12(3)*/,	// (as unsigned int)
		Offset_CSBot_BuyState_UNKNOWN3                    = 697u/*16(4)*/,	// (as unsigned int)
		Offset_CSBot_BuyState_UNKNOWN4                    = 698u/*20(5)*/,	// (as unsigned int)
		Offset_CSBot_BuyState_UNKNOWN5                    = 699u/*24(6)*/,	// (as unsigned int)
	Offset_CSBot_MoveToState                          = 700u,	// (as Classes::CSBot::MoveToState)
		Offset_CSBot_MoveToState_UNKNOWN0                 = 701u/*4(1)*/,	// (as unsigned int)
		Offset_CSBot_MoveToState_UNKNOWN1                 = 702u/*8(2)*/,	// (as unsigned int)
		Offset_CSBot_MoveToState_UNKNOWN2                 = 703u/*12(3)*/,	// (as unsigned int)
		Offset_CSBot_MoveToState_UNKNOWN3                 = 704u/*16(4)*/,	// (as unsigned int)
		Offset_CSBot_MoveToState_UNKNOWN4                 = 705u/*20(5)*/,	// (as unsigned int)
	Offset_CSBot_FetchBombState                       = 706u,	// (as Classes::CSBot::FetchBombState)
	Offset_CSBot_PlantBombState                       = 707u,	// (as Classes::CSBot::PlantBombState)
	Offset_CSBot_DefuseBombState                      = 708u,	// (as Classes::CSBot::DefuseBombState)
	Offset_CSBot_HideState                            = 709u,	// (as Classes::CSBot::HideState)
		Offset_CSBot_HideState_UNKNOWN0                   = 710u/*4(1)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN1                   = 711u/*8(2)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN2                   = 712u/*12(3)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN3                   = 713u/*16(4)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN4                   = 714u/*20(5)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN5                   = 715u/*24(6)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN6                   = 716u/*28(7)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN7                   = 717u/*32(8)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN8                   = 718u/*36(9)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN9                   = 719u/*40(10)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN10                  = 720u/*44(11)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN11                  = 721u/*48(12)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN12                  = 722u/*52(13)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN13                  = 723u/*56(14)*/,	// (as unsigned int)
		Offset_CSBot_HideState_UNKNOWN14                  = 724u/*60(15)*/,	// (as unsigned int)
	Offset_CSBot_EscapeFromBombState                  = 725u,	// (as Classes::CSBot::EscapeFromBombState)
	Offset_CSBot_FollowState                          = 726u,	// (as Classes::CSBot::FollowState)
		Offset_CSBot_FollowState_UNKNOWN0                 = 727u/*4(1)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN1                 = 728u/*8(2)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN2                 = 729u/*12(3)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN3                 = 730u/*16(4)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN4                 = 731u/*20(5)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN5                 = 732u/*24(6)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN6                 = 733u/*28(7)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN7                 = 734u/*32(8)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN8                 = 735u/*36(9)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN9                 = 736u/*40(10)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN10                = 737u/*44(11)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN11                = 738u/*48(12)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN12                = 739u/*52(13)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN13                = 740u/*56(14)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN14                = 741u/*60(15)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN15                = 742u/*64(16)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN16                = 743u/*68(17)*/,	// (as unsigned int)
		Offset_CSBot_FollowState_UNKNOWN17                = 744u/*72(18)*/,	// (as unsigned int)
	Offset_CSBot_UseEntityState                       = 745u,	// (as Classes::CSBot::UseEntityState)
		Offset_CSBot_UseEntityState_Target                = 746u/*4(1)*/,	// (as Classes::EntityHandle)
	///@}

	Offset_CSBot_CurrentState                         = 748u,	// Executed when variable 'Offset_CSBot_IsAttacking' is false. (as Classes::CSBot::BotState *)
	Offset_CSBot_SetStateTimestamp                    = 749u,	// (as float)
	Offset_CSBot_IsAttacking                          = 3000u,	// (as bool)
	Offset_CSBot_CurrentTaskID                        = 751u,	// (as Classes::CSBot::TaskType_t)
	Offset_CSBot_TaskEntity                           = 752u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_GoalOrigin                           = 754u/*(3016)*/,	// (as Math::Vector3D)
	Offset_CSBot_GoalEntity                           = 757u/*(3028)*/,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_CurrentArea                          = 759u/*(3036)*/,	// (as Classes::NavigationArea *)
	Offset_CSBot_LastKnownArea                        = 760u/*(3040)*/,	// (as Classes::NavigationArea *)
	Offset_CSBot_TouchPlayer                          = 761u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_TouchPlayerTime                      = 763u,	// (as float)
	Offset_CSBot_IsUNKNOWN0                           = 3056u,	// sets to true in CCSBot__DiscontinuityJump() function.... (Is jumped?) (as bool)
	Offset_CSBot_IsUNKNOWN1                           = 3057u,	// sets to false in CCSBot__DiscontinuityJump() function and no more used.... (as bool)
	Offset_CSBot_DiscontinuityJumpTimestamp           = 765u,	// (as float)

	///@{ Array....
	Offset_CSBot_PathInformation                      = 766u/*(3064)*/,	// Path information. (as Classes::Bot::PathInformation_t[Classes::Bot::MaximumPathNodesNumber])
		// For path[0] is a source always....
		Offset_CSBot_PathInformation_0_NavigationArea    = Offset_CSBot_PathInformation,	// (as Classes::NavigationArea *)
		Offset_CSBot_PathInformation_0_TraverseType      = 767u/*(3068)*/,	// (as NavTraverseType_t)
		Offset_CSBot_PathInformation_0_SourceOrigin      = 768u/*(3072)*/,	// (as Math::Vector3D)
		Offset_CSBot_PathInformation_0_NavigationLadder  = 771u/*(3084)*/,	// (as Classes::NavigationLadder *)

		// For trivial path[1] is a destination....
		Offset_CSBot_PathInformation_1_NavigationArea    = 772u/*(3088)*/,	// (For trivial path equals to source 'Offset_CSBot_PathInformation_0_NavigationArea') (as Classes::NavigationArea *)
		Offset_CSBot_PathInformation_1_TraverseType      = 773u/*(3092)*/,	// (as NavTraverseType_t)
		Offset_CSBot_PathInformation_1_DestinationOrigin = 774u/*(3096)*/,	// (as Math::Vector3D)
		Offset_CSBot_PathInformation_1_NavigationLadder  = 775u/*(3108)*/,	// (as Classes::NavigationLadder *)
	///@}

	Offset_CSBot_AreasNumberInPath                    = 2302u,	// (as unsigned int) (LINUX = 9208)
	Offset_CSBot_CurrentPathIndex                     = 2303u,	// (as unsigned int) (LINUX = 9212)
	Offset_CSBot_CurrentPathIndexSetTime              = 2304u,	// @todo RENAME ME! (as float)
	///@{ SOMETHING LIKE 'Utilities::CountdownTimer' CLASS!!!
	Offset_CSBot_ComputePathNextCallDelay             = 2305u,	// Time when function Classes::CSBot::ComputePath() can be called. (as float)
	Offset_CSBot_ComputePathNextCallGlobalTime        = 2306u,	// Amount of delay time between function Classes::CSBot::ComputePath() calls. (as float)
	///@}
	Offset_CSBot_IsFriendInTheWayNextCallDelay        = 2307u,	// Amount of delay time between function Classes::CSBot::IsFriendInTheWay() calls. (as float)
	Offset_CSBot_IsFriendInTheWayNextCallGlobalTime   = 2308u,	// Time when function Classes::CSBot::IsFriendInTheWay() can be called. (as float)
	Offset_CSBot_IsFriendInTheWay                     = 9236u,	// (as bool)
	Offset_CSBot_ResetStuckMonitorNextCallDelay       = 2310u,	// (as float)
	Offset_CSBot_ResetStuckMonitorNextCallGlobalTime  = 2311u,	// (as float)
	Offset_CSBot_CanResetStuckMonitor                 = 9248u,	// (as bool)
	Offset_CSBot_LadderMovementState                  = 2313u,	// (as Classes::CSBot::LadderMovementState_t)
	Offset_CSBot_LadderApproachFaceIn                 = 9256u,	// Always true when ascending on ladder. (as bool)
	Offset_CSBot_CurrentLadder                        = 2315u,	// (as Classes::NavigationLadder *)
	Offset_CSBot_LadderDismountAscendingMoveType      = 2316u,	// (as Classes::CSBot::LadderDismountAscendingMoveType_t)
	Offset_CSBot_LadderDismountAscendingMoveStartTimestamp = 2317u,	// Bot will do move only 0.4 seconds. (as float)
	Offset_CSBot_LadderEndpoint                       = 2318u,	// (as float)
	Offset_CSBot_LadderGlobalTimeout                  = 2319u,	// Time during which the bot has to overcome the ladder. (as float)
	Offset_CSBot_ForceRunTimeDuration                 = 2320u,	// From CCSBot__ForceRun() function in cs_i386.so only! (as float)
	Offset_CSBot_ForceRunGlobalTimeDuration           = 2321u,	// From CCSBot__ForceRun() function in cs_i386.so only! (as float)
	Offset_CSBot_CSGameState                          = 2322u,	// (as Classes::CSBot::CSGameState)
		Offset_CSBot_CSGameState_Outer                    = Offset_CSBot_CSGameState,	// Bot owning this 'Classes::CSBot::CSGameState'. (as Classes::CSBot *)
		Offset_CSBot_CSGameState_IsRoundOver              = (Offset_CSBot_CSGameState + 1u) * 4u/*2323(9292)*/,	// (as bool)
		Offset_CSBot_CSGameState_BombState                = Offset_CSBot_CSGameState + 2u/*2324(9296)*/,	// (as Classes::CSBot::CSGameState::BombState_t)
		Offset_CSBot_CSGameState_UpdateBomberTimestamp    = Offset_CSBot_CSGameState + 3u/*2325(9300)*/,	// (as float)
		Offset_CSBot_CSGameState_BomberOrigin             = Offset_CSBot_CSGameState + 4u/*2326(9304)*/,	// (as Math::Vector3D)
		Offset_CSBot_CSGameState_UpdateLooseBombTimestamp = Offset_CSBot_CSGameState + 7u/*2329(9316)*/,	// (as float)
		Offset_CSBot_CSGameState_LoosedBombOrigin         = Offset_CSBot_CSGameState + 8u/*2330(9320)*/,	// (as Math::Vector3D)

		/// @warning OR THIS IS NOT STRUCT, JUST 'm_isBombsiteClear[MaximumMapZones]; m_index[MaximumMapZones]'.
		/// @todo CHECK ME!!!!
		Offset_CSBot_CSGameState_MapZones                 = Offset_CSBot_CSGameState + 11u/*2333(9332)*/,	// (as Classes::CSBot::CSGameState::MapZone_t[Classes::CSBotManager::MaximumMapZones])
		Offset_CSBot_CSGameState_MapZonesNumber           = Offset_CSBot_CSGameState + 16u/*2338(9352)*/,	// Assigned from 'Classes::CSBotManager::m_mapZonesNumber' variable. (as unsigned int)

		Offset_CSBot_CSGameState_BombsiteIndexToSearch    = Offset_CSBot_CSGameState + 17u/*2339(9356)*/,	// Index of bombsite in a 'Classes::CSBot::CSGameState::m_mapZones' array. (as unsigned int)
		Offset_CSBot_CSGameState_PlantedBombsiteIndex     = Offset_CSBot_CSGameState + 18u/*2340(9360)*/,	// Index of bombsite in a 'Classes::CSBotManager::m_mapZones' array. Can be -1. (as int)
		Offset_CSBot_CSGameState_IsPlantedBombLocationKnown = Offset_CSBot_CSGameState + 19u/*2341(9364)*/,	// (as bool)
		Offset_CSBot_CSGameState_PlantedBombOrigin        = Offset_CSBot_CSGameState + 20u/*2342(9368)*/,	// (as Math::Vector3D)
		Offset_CSBot_CSGameState_HostageInfo              = Offset_CSBot_CSGameState + 23u/*2345(9380)*/,	// (as Classes::CSBot::CSGameState::HostageInfo_t[Classes::CSBot::CSGameState::MaximumHostageInfo])
		Offset_CSBot_CSGameState_HostageInfoNumber        = Offset_CSBot_CSGameState + 83u/*2405(9620)*/,	// (as unsigned int)
		Offset_CSBot_CSGameState_ValidateHostagePositionsNextCallDelay      = Offset_CSBot_CSGameState + 84u/*2406(9624)*/,	// (as float)
		Offset_CSBot_CSGameState_ValidateHostagePositionsNextCallGlobalTime = Offset_CSBot_CSGameState + 85u/*2407(9628)*/,	// (as float)
		Offset_CSBot_CSGameState_UNKNOWN0                 = Offset_CSBot_CSGameState + 86u/*2408(9632)*/,	// (as bool)
		Offset_CSBot_CSGameState_UNKNOWN1                 = (Offset_CSBot_CSGameState + 86u) * 4u + 1u/*2408.25(9633)*/,	// (as bool)

	Offset_CSBot_HostageEscortCount                   = 9636u,	// Used hostages number. (as unsigned char)
	Offset_CSBot_UpdateHostageEscortCountTimestamp    = 2410u,	// (as float)
	Offset_CSBot_IsUNKNOWN2                             = 9644u,	// From CCSBot__ResetWaitForHostagePatience() function in cs_i386.so only! (as bool)
	Offset_CSBot_WaitForHostagePatienceDelayTime      = 2412u,	// 3.0 (as float)
	Offset_CSBot_WaitForHostagePatienceGlobalTime     = 2413u,	// From CCSBot__ResetWaitForHostagePatience() function in cs_i386.so only! (as float)
	Offset_CSBot_UNKNOWN0_DelayTime                   = 2414u,	// 10.0 (as float)
	Offset_CSBot_UNKNOWN0_GlobalTime                  = 2415u,	// (as float)
	Offset_CSBot_NoisePosition                        = 2416u,	// (as Math::Vector3D)
	Offset_CSBot_NoiseHearGlobalTimeDuration          = 2419u,	// (as float)
	Offset_CSBot_NoiseArea                            = 2420u,	// (as Classes::NavigationArea *)
	Offset_CSBot_ShouldInvestigateNoiseCallTimestamp  = 2421u,	// (as float)
	Offset_CSBot_NoisePriority                        = 2422u,	// (as Classes::CSBot::NoisePriority_t)
	Offset_CSBot_IsUNKNOWN3                           = 9692u,	// (as bool)
	Offset_CSBot_NextLookAtHidingApproachPointGlobalTime = 2424u,	// (as float)
	Offset_CSBot_LookAheadAngle                       = 2425u,	// (as float)
	Offset_CSBot_LookForwardAngle                     = 2426u,	// (as float)
	Offset_CSBot_InhibitLookAroundGlobalTime          = 2427u,	// From CCSBot__InhibitLookAround() function. (as float)
	Offset_CSBot_LookAction                           = 2428u,	// (as Classes::CSBot::LookAction_t)
	Offset_CSBot_LookAtVector                         = 2429u,	// (as Math::Vector3D)
		Offset_CSBot_LookAtVector_X                       = Offset_CSBot_LookAtVector,		// (as float)
		Offset_CSBot_LookAtVector_Y                       = Offset_CSBot_LookAtVector + 1u,	// (as float)
		Offset_CSBot_LookAtVector_Z                       = Offset_CSBot_LookAtVector + 2u,	// (as float)
	Offset_CSBot_LookAtPriorityType                   = 2432u,	// (as Classes::CSBot::PriorityType_t)
	Offset_CSBot_LookAtDuration                       = 2433u,	// (as float)
	Offset_CSBot_LookAtStartTimestamp                 = 2434u,	// Time when bot just aimed to target. After this is set bot will look at target 'Offset_CSBot_LookAtDuration' seconds. (as float)
	Offset_CSBot_LookAtMaximumAngleDifference         = 2435u,	// The maximum amount of angle difference between target and bot look angles. Used for check if bot aimed at target to start look countdown timer. (as float)
	Offset_CSBot_LookAtIsUNKNOWN0                     = 9744u,	// (as bool)
	Offset_CSBot_LookAtString                         = 2437u,	// "Approach Point (Hiding)", "Encounter Spot", "Last Enemy Position", "Check dangerous noise", "GrenadeThrow", "Nearby enemy gunfire", "Defuse bomb", "Hostage", "Plant bomb on floor", "Use entity", "Breakable", "Panic" or "Noise" (as char *)
	Offset_CSBot_UpdatePeripheralVisionCallTimestamp  = 2438u,	// (Delay between calls is 0.3 seconds.) (as float)
	Offset_CSBot_ApproachPoints                       = 2439u,	// (as Math::Vector3D[Classes::NavigationAra::MAX_APPROACH_AREAS])
	Offset_CSBot_ApproachPointsNumber                 = 9948u,	// (as unsigned char)
	Offset_CSBot_ApproachPointsCenter                 = 2488u,	// Just bot origin vector, used when bot moved on distance more than 50 units to compute approach points for new origin again. (as Math::Vector3D)
	Offset_CSBot_IsThrowingGrenade                    = 9964u,	// Sets to true in CCSBot__ThrowGrenade() function. (as bool)
	Offset_CSBot_ThrowGrenadeAimTimeAmount            = 2492u,	// Amount of time to aim at throw position. (3.0 seconds) (as float)
	Offset_CSBot_ThrowGrenadeAimGlobalTimeAmount      = 2493u,	// Amount of time to aim at throw position. (as float)
	Offset_CSBot_CurrentSpotEncounter                 = 2494u,	// (as Classes::SpotEncounter *)
	Offset_CSBot_NextLookAtCurrentSpotEncounterGlobalTime = 2495u,	// (as float)
	Offset_CSBot_HidingSpotCheckTimers                = 2496u,	// (as Utilities::CountdownTimer[MaximumHidingSpotCheckTimers])
	Offset_CSBot_HidingSpotCheckTimersNumber          = 2624u,	// (as unsigned int)
	Offset_CSBot_LookAnglePitch                       = 2625u,	// (as float)
	Offset_CSBot_LookAnglePitchUNKNOWN                = 2626u,	// (as float)
	Offset_CSBot_LookAngleYaw                         = 2627u,	// (as float)
	Offset_CSBot_LookAngleYawUNKNOWN                  = 2628u,	// (as float)
	Offset_CSBot_UNKNOWNVector0                       = 2629u,	// (as Math::Vector3D)
	Offset_CSBot_AimOffset                            = 2632u,	// (From CCSBot__UpdateAimOffset() function) (as Math::Vector3D)
	Offset_CSBot_AimOffset2                           = 2635u,	// (From CCSBot__SetAimOffset() function) (as Math::Vector3D)
	Offset_CSBot_SetAimOffsetNextCallTimestamp        = 2638u,	// (as float)
	Offset_CSBot_AttackBeginTimestamp                 = 2639u,	// (as float)
	Offset_CSBot_UNKNOWN1                             = 2640u,	// (as Math::Vector3D)
	Offset_CSBot_Disposition                          = 2643u,	// (as Classes::CSBot::DispositionType_t)
	Offset_CSBot_IgnoreEnemiesTimer                   = 2644u,	// (as Utilities::CountdownTimer)
	Offset_CSBot_Enemy                                = 2646u,	// Current enemy. (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_IsEnemyVisible                       = 10592u,	// Is aiming at enemy. (as bool)
	Offset_CSBot_EnemyVisibleBodyParts                = 10593u,	// Enemy visible body part bits. (Calculated by function CSBot::IsVisible()) (as Classes::Bot::VisiblePartType_t)
	Offset_CSBot_LastKnownEnemyPosition               = 2649u,	// (as Math::Vector3D)
	Offset_CSBot_LastSawEnemyTimestamp                = 2652u,	// (as float)
	Offset_CSBot_FirstSawEnemyTimestamp               = 2653u,	// (as float)
	Offset_CSBot_AcquiredCurrentEnemyTimestamp        = 2654u,	// (as float)
	Offset_CSBot_EnemyDeathTimestamp                  = 2655u,	// (as float)
	Offset_CSBot_IsEnemyDead                          = 10624u,	// (as bool)
	Offset_CSBot_NearbyEnemyCount                     = 2657u,	// (as unsigned int)
	Offset_CSBot_EnemyPlace                           = 2658u,	// (as Classes::Place_t)
	Offset_CSBot_AlivePlayersInfo                     = 2659u,	// (as Classes::CSBot::AliveVisiblePlayersInfo_t[MaximumClients])
	Offset_CSBot_Bomber                               = 2723u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_NearbyFriendCount                    = 2725u,	// (as unsigned int)
	Offset_CSBot_ClosestVisibleFriend                 = 2726u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_ClosestVisibleHumanFriend            = 2728u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_Attacker                             = 2730u,	// Current attacker. (as Classes::BaseEntity *)
	Offset_CSBot_AttackedTimestamp                    = 2731u,	// (as float)
	Offset_CSBot_LastVictimID                         = 2732u,	// (as unsigned int)
	Offset_CSBot_IsAimingAtEnemy                      = 10932u,	// Is aiming at enemy. (as bool)
	Offset_CSBot_IsRapidFiring                        = 10933u,	// (as bool)
	Offset_CSBot_WeaponEquipTimestamp                 = 2734u,	// Time of last weapon equip. (as float)
	Offset_CSBot_FireWeaponAtEnemyNextCallTimestamp   = 2735u,	// (as float)
	Offset_CSBot_RecognizedEnemiesInfo                = 2736u,	// (as Classes::CSBot::RecognizedEnemyInfo_t[Classes::CSBot::MaximumReactionQueue])
	Offset_CSBot_UpdateReactionQueue_UNKNOWN0         = 11184u,	// (as unsigned char)
	Offset_CSBot_UpdateReactionQueue_UNKNOWN1         = 11185u,	// (as unsigned char)
	Offset_CSBot_CurrentRecognizedEnemiesQueueIndex   = 11186u,	// (as unsigned char)
	Offset_CSBot_IsStucked                            = 11187u,	// (as bool)
	Offset_CSBot_StuckTimestamp                       = 2797u,	// (as float)
	Offset_CSBot_StuckOrigin                          = 2798u,	// (as Math::Vector3D)
	Offset_CSBot_WiggleMoveDirection                  = 2801u,	// (as Classes::CSBot::WiggleMoveDirection_t)
	Offset_CSBot_WiggleMoveInDirectionGlobalDuration  = 2802u,	// (as float)
	Offset_CSBot_WiggleJumpTimestamp                  = 2803u,	// Set when bot just stucked, in CSBot::Wiggle() function when time expired bot jumps and again sets next jump timestamp. (as float)
	Offset_CSBot_DistancesToPreviousOriginSinceStuck  = 2804u,	// (as float[5])
	Offset_CSBot_DistancesToPreviousOriginSinceStuckCurrentIndex0 = 2809u,	// (as unsigned int)
	Offset_CSBot_DistancesToPreviousOriginSinceStuckCurrentIndex1 = 2810u,	// (as unsigned int)
	Offset_CSBot_PreviousOrigin                       = 2811u,	// For CSBot::StuckCheck() function. (as Math::Vector3D)
	Offset_CSBot_CurrentRadioCommand                  = 2814u,	// (as GameEventType_t)
	Offset_CSBot_ReceiveRadioMessageTimestamp         = 2815u,	// (as float)
	Offset_CSBot_SendRadioMessageTimestamp            = 2816u,	// (as float)
	Offset_CSBot_RadioMessageSender                   = 2817u,	// (as Classes::Edict *, but, Classes::EntityHandle)
	Offset_CSBot_RadioMessageSenderOrigin             = 2819u,	// (as Math::Vector3D)
	Offset_CSBot_StartVoiceFeedbackTimestamp          = 2822u,	// Time chatter icon last showed. (as float)
	Offset_CSBot_VoiceFeedbackGlobalDuration          = 2823u,	// Chatter icon hide time. (as float)

	// Chatter....
	Offset_CSBot_Chatter                              = 2824u,	// (as Classes::CSBot::BotChatterInterface)
		Offset_CSBot_Chatter_Statement                    = Offset_CSBot_Chatter/*2824(11296)*/,	// (as Classes::CSBot::BotStatement *)
		Offset_CSBot_Chatter_Outer                        = Offset_CSBot_Chatter + 1u/*2825(11300)*/,	// Bot owning this 'Classes::CSBot::BotChatterInterface'. (as Classes::CSBot *)
		// ....

	Offset_CSBot_UNKNOWN2                             = 2838u,	// (as Utilities::CountdownTimer)
	Offset_CSBot_UNKNOWN3                             = 2840u,	// (as unsigned int)

	// Navigation mesh....
	Offset_CSBot_CurrentSamplingNode                  = 2841u,	// The current navigation node we are sampling from. (as Classes::NavigationNode *)
	Offset_CSBot_CurrentGenerationDirection           = 2842u,	// (as NavDirType_t)
	Offset_CSBot_CurrentGenerationNode                = 2843u,	// Used for iterating nav areas during generation process. (as Classes::CUtlLinkedList::Node_t *)
	Offset_CSBot_CurrentGenerationState               = 2844u,	// The state of the generation process. (as Classes::CSBot::GenerationStateType_t)
	Offset_CSBot_UNKNOWN_CountdownTimer0              = 2845u,	// (as Utilities::CountdownTimer)
	Offset_CSBot_UNKNOWN_CountdownTimer1              = 2847u,	// (as Utilities::CountdownTimer)
	Offset_CSBot_UNKNOWN_CountdownTimer2              = 2849u	// (as Utilities::CountdownTimer)

	// sizeof (CSBot) == [CS 1.6: 11404 (2851), CS: CZ: 11372 (2843)]
};

#endif	// ifndef ENTITY_PRIVATE_DATA_OFFSET_INCLUDED