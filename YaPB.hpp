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
// YaPB.hpp
//
// Class: YaPB
//
// Description: Contains the 'YaPB' class Function Prototypes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef YAPB_INCLUDED
#define YAPB_INCLUDED

class YaPB : public FakeClient
{
	friend YaPBManager;

	class BaseNavigator;

	class ActionManager;

	class Task;
	class Schedule;

	class Task_Normal;
	class Task_SprayLogo;
	class Task_DoubleJump;

	DECLARE_CLASS_WITH_BASE (FakeClient, YaPB);

	//
	// Group: Constants.
	//
	public:
		#if defined _DEBUG
			// debug levels structure definition
			struct DebugLevel_t
			{
				STDIOFile file;	// pointer to the file where the debug messages are logged
				bool aiconsole;	// set to TRUE when the AI console is enabled
				unsigned char eyes;	// vision debug level
				unsigned char ears;	// hearing debug level
				unsigned char body;	// feeling debug level
				unsigned char legs;	// movement debug level
				unsigned char hand;	// weaponry usage debug level
				unsigned char chat;	// chat debug level
				unsigned char cognition;	// cognition debug level
				unsigned char navigation;	// navigation debug level
				char text_eyes[6][64];	// vision debug text
				char text_ears[6][64];	// hearing debug text
				char text_body[6][64];	// feeling debug text
				char text_legs[6][64];	// movement debug text
				char text_hand[6][64];	// weaponry usage debug text
				char text_chat[6][64];	// chat debug text
				char text_cognition[6][64];	// cognition debug text
				char text_navigation[6][64];	// navigation debug text
				bool is_observer;	// set to TRUE if listen server client is in observer mode (bots ignore him)
				bool is_peacemode;	// set to TRUE if the bots are told not to shoot each other unless provoked
				bool is_dontfindmode;	// set to TRUE if the bots don't check for pickable entities around
				bool is_inhumanturns;	// set to TRUE if the bots can face immediately their ideal angles
				bool eyes_disabled;	// set to TRUE if the AI is completely blind (doesn't see anything)
				bool ears_disabled;	// set to TRUE if the AI is completely deaf (doesn't hear anything)
				bool body_disabled;	// set to TRUE if the AI is completely insensitive (doesn't feel anything)
				bool legs_disabled;	// set to TRUE if the AI is rendered unable to move
				bool hand_disabled;	// set to TRUE if the AI is rendered unable to act and use things
				bool chat_disabled;	// set to TRUE if the AI is rendered unable to chat
				bool is_paused;	// set to TRUE if all AI is completely frozen (doesn't run at all)
				bool is_broke;	// set to TRUE to tell the bot code to break in the current frame
			};

			static DebugLevel_t s_debugLevel;
/*
			DECLARE_TYPED_ENUMERATION (unsigned int, DebugFlags_t)
			{
				DebugFlags_Body = BIT (0u)
			};
			static DebugFlags_t s_debugFlags;*/
		#endif	// if defined _DEBUG

		// YaPB game actions...
		enum GameAction_t
		{
			GameAction_Idle,

			GameAction_TeamSelect,
			GameAction_ModelSelect,

			GameAction_BuyStuff,

			GameAction_RadioMessage,
			GameAction_SayMessage,
			GameAction_TeamSay
		};

		enum CommunicationType_t
		{
			CommunicationType_None,

			CommunicationType_Chat,
			CommunicationType_Radio,
			CommunicationType_Chatter	// is bot will use chatter in a game or radio instead
		};

		// fire delay definiton
		struct FireDelay_t
		{
			HalfLifeEngine::SDK::Constants::WeaponID_t weaponID;

			unsigned char                              maximumFireBullets;
			float                                      minimumBurstPauseFactor;

			float                                      primaryBaseDelay;
			float                                      primaryMinDelay[6u];
			float                                      primaryMaxDelay[6u];

//			float                                      secondaryBaseDelay;
//			float                                      secondaryMinDelay[5u];
//			float                                      secondaryMaxDelay[5u];
		};

		static const float       s_minimumBurstDistance;
		static const FireDelay_t s_fireDelays[NumberWeapons + 1u];	// weapon firing delay based on skill (min and max delay for each weapon)

		// skill definitions
		struct SkillDefinition_t
		{
			float          minSurpriseTime;		// surprise delay (minimum)
			float          maxSurpriseTime;		// surprise delay (maximum)
			float          campStartDelay;		// delay befor start camping
			float          campEndDelay;		// delay before end camping
			float          minTurnSpeed;		// initial minimum turnspeed
			float          maxTurnSpeed;		// initial maximum turnspeed
			Math::Vector3D aimOffset;			// X/Y/Z maximum offsets
			unsigned char  headshotFrequency;	// precent to aiming to player head
			unsigned char  heardShootThruProb;	// precent to shooting throug wall when seen something
			unsigned char  seenShootThruProb;	// precent to shooting throug wall when heard something
			unsigned char  recoilAmount;		// amount of recoil when the bot should pause shooting
		};

		static SkillDefinition_t s_skillTables[];

		template <typename navigationUnitType> struct MapZone_t
		{
			typedef DynamicArray <navigationUnitType *, unsigned char> ContainingNavigationUnitsArray;

			const HalfLifeEngine::SDK::Classes::Edict *edict;

			Math::Vector3D                             origin;
			Math::Extent3D                             absoluteBoundingBox;

			ContainingNavigationUnitsArray             containingNavigationUnits;	// navigation areas or waypoints.
		};

		typedef MapZone_t <Waypoint>                            WaypointMapZone_t;
		typedef DynamicArray <WaypointMapZone_t, unsigned char> WaypointMapZonesArray;
		typedef MapZone_t <NavigationMesh::NavigationArea>      AreaMapZone_t;
		typedef DynamicArray <AreaMapZone_t, unsigned char>     AreaMapZonesArray;

		static WaypointMapZonesArray s_bombPlaceWaypointMapZones;
		static WaypointMapZonesArray s_hostageWaypointMapZones;
		static WaypointMapZonesArray s_hostageRescueWaypointMapZones;
		static WaypointMapZonesArray s_VIPSafetyWaypointMapZones;

		static AreaMapZonesArray     s_bombPlaceAreaMapZones;
		static AreaMapZonesArray     s_hostageAreaMapZones;
		static AreaMapZonesArray     s_hostageRescueAreaMapZones;
		static AreaMapZonesArray     s_VIPSafetyAreaMapZones;

		typedef StaticArray <Math::Vector3D, unsigned char, NavigationMesh::NavigationArea::MAX_APPROACH_AREAS> ApproachPointArray_t;

		template <typename navigationUnitType> class GameState
		{
			public:
				DynamicArray <const MapZone_t <navigationUnitType> *, unsigned char> visitedBombPlaces;
				DynamicArray <const MapZone_t <navigationUnitType> *, unsigned char> visitedHostageZones;

			public:
				inline void Reset (void)
				{
					visitedBombPlaces.RemoveAll ();
					visitedHostageZones.RemoveAll ();
				}
		};

		#include <BotProfile.hpp>

	//
	// Group: Private members.
	//
	private:
		// Enemy body parts seen
		DECLARE_TYPED_ENUMERATION (unsigned char, Visibility_t)
		{
			Visibility_None,

			Visibility_Head  = BIT (0u),
			Visibility_Body  = BIT (1u),
			Visibility_Other = BIT (2u),	// Arm/foot, something else....

			Visibility_All   = Visibility_Head | Visibility_Body | Visibility_Other
		};

		float                                           m_frameIntervalTime;			// Bot's frame interval.
		float                                           m_lastThinkTime;				// Time bot last thinked.

		float                                           m_spawnTime;					// Time this bot spawned.
		float                                           m_fallDownTime;					// Time the bot started to fall down

		float                                           m_nextCheckFollowMeRadioCommandTime;

		GameState <NavigationMesh::NavigationArea>      m_areaGameState;

		#include <Msec.hpp>

		Msec                                            m_msec;							// Calculated msec value for this bot. (for pfnRunPlayerMove() function)

		float                                           m_joinServerTime;				// Time when client was joined to the server.

		DECLARE_TYPED_ENUMERATION (unsigned char, Boredom_t)
		{
			Boredom_Minimum = 0u,
			Boredom_Maximum = static_cast <Boredom_t> (-1)
		};
		Boredom_t                                       m_boredom;						// Negative if really bored.
		float                                           m_wantToLeaveCheckTime;

		unsigned char                                   m_buysNumberInCurrentRound;		// Number of purchases in a current round.

		Profile *const                                  m_profile;						// pointer to this bot profile data in the profiles array.

		GameAction_t                                    m_gameAction;					// team/class selection state.

		float                                           m_timeLogoSpray;				// time bot last spray logo
		float                                           m_illuminationCheckTime;		// time to check current light level (CheckIllumination() function), for some actions: switch flashlight/night vision goggles.... (it only for bot, whose skill is less than 100)

		bool                                            m_buyingFinished;				// done with buying

		bool                                            m_isLeader;						// bot is leader of his team

		float                                           m_firePause;					// time to pause firing
		float                                           m_shootTime;					// time to shoot
		bool                                            m_wantsToFire;					// bot needs consider firing

		float                                           m_agressionLevel;				// dynamic aggression level (in game) (0.0, ..., 1.0)
		float                                           m_fearLevel;					// dynamic fear level (in game) (0.0, ..., 1.0)

		float                                           m_nextEmotionUpdate;			// next time to sanitize emotions

		// reload state
		DECLARE_TYPED_ENUMERATION (char, ReloadState_t)
		{
			ReloadState_None = -1,	// no reload state currrently

			ReloadState_Primary,	// primary weapon reload state
			ReloadState_Secondary,	// secondary weapon reload state

			ReloadState_Total
		};
		ReloadState_t                                   m_reloadState;					// current reload state
		float                                           m_reloadCheckTime/*[ReloadState_Total]*/;				// time to check reloading

		float                                           m_zoomCheckTime;				// time to check zoom again
		float                                           m_shieldCheckTime;				// time to check shiled drawing again
		float                                           m_grenadeCheckTime;				// time to check grenade usage
		float                                           m_silencerCheckTime;			// time to check the silencer state

		ApproachPointArray_t                            m_approachPoints;
		Math::Vector3D                                  m_approachPointsCenter;

		HalfLifeEngine::SDK::Constants::MapZone_t       m_currentMapZone;				// Need only for compare with new map zone in Entered*Zone() functions.

		#include <BotRadioCommandsManager.hpp>

		RadioCommandsManager                            m_radioCommandsManager;

		class ChatterManager
		{
			//
			// Group: Type definitions.
			//
			public:
				HalfLifeEngine::SDK::Classes::Interface::Voice::IVoiceCodec *m_encodeCodec;

				bool m_bVoiceRecording;	// Are we recording at the moment?

				// These are used for recording the wave data into files for debugging.
				#define MAX_WAVEFILEDATA_LEN	1024 * 1024
				char *m_pUncompressedFileData;
				int m_nUncompressedDataBytes;
				const char *m_pUncompressedDataFilename;

				char *m_pDecompressedFileData;
				int m_nDecompressedDataBytes;
				const char *m_pDecompressedDataFilename;

				char *m_pMicInputFileData;
				unsigned int m_nMicInputFileBytes;
				int m_CurMicInputFileByte;
				double m_MicStartTime;

				struct ChatterItem_t
				{
					::ChatterManager::ChatterMessage_t index;
					float                              sayTime;
				};

			//
			// Group: Private members.
			//
			private:
				class ChatterMessagesPriorityQueue : public PriorityQueue <ChatterItem_t, unsigned char>
				{
					//
					// Group: Private functions.
					//
					private:
						static inline const bool IsLowerPriority (const ChatterItem_t &node1, const ChatterItem_t &node2)
						{
							// Nodes with greater time to say are lower priority.
							return node1.sayTime <= node2.sayTime;
						}

					//
					// Group: (Con/De)structors.
					//
					public:
						inline ChatterMessagesPriorityQueue (void) : PriorityQueue (IsLowerPriority) { /* VOID */ }
				}     m_chatterMessagesPriorityQueue;

				float m_chatterIconShowTime;	// time when bot chatter icon should displayed (when > 0.0)
				float m_chatterIconHideTime;	// is bot chatter icon displayed? (when > 0.0)

			public:
				inline ChatterManager (void);	// Note: This function declared in YaPBManager.hpp.
				inline ~ChatterManager (void)
				{
					if (IsCodecInitialized ())
						ReleaseCodec ();
					#if defined _DEBUG
						else
						{
							// Reliability checks.
							InternalAssert (m_pMicInputFileData == NULL);
							InternalAssert (m_pUncompressedFileData == NULL);
							InternalAssert (m_pDecompressedFileData == NULL);
						}
					#endif	// if defined _DEBUG
				}

			//
			// Group: Private functions.
			//
			private:
				#if defined _DEBUG
					inline const bool IsPriorityQueueMember (const ::ChatterManager::ChatterMessage_t chatterMessageIndex) const
					{
						for (unsigned char index (0u); index < m_chatterMessagesPriorityQueue.GetElementNumber (); ++index)
							if (m_chatterMessagesPriorityQueue[index].index == chatterMessageIndex)
								return true;

						return false;
					}
				#endif	// if defined _DEBUG

				inline void InstantChatterMessage (const YaPB *const bot, ChatterItem_t chatterItem);	// Note: This function declared in YaPBManager.hpp.

			//
			// Group: Functions.
			//
			public:
				inline       HalfLifeEngine::SDK::Classes::Interface::Voice::IVoiceCodec *const GetCodec           (void) const { return m_encodeCodec; }
				inline const bool                                                      IsCodecInitialized (void) const;	// Note: This function declared in YaPBManager.hpp.
				inline       void                                                      ReleaseCodec       (void)
				{
					// Reliability check.
					InternalAssert (IsCodecInitialized ());

					Voice_RecordStop ();

					if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
						m_encodeCodec->Release ();
					else
					{
						HalfLifeEngine::SDK::Classes::Interface::CDAudio *const cdaudio (HalfLifeEngine::Globals::g_halfLifeEngine->GetCDAudio ());

						cdaudio->UNKNOWN2 ();

							m_encodeCodec->Release ();

						cdaudio->UNKNOWN1 ();
					}

					m_encodeCodec = NULL;
				}
				inline void OnChangeCodec (const unsigned char quality);	// Note: This function declared in YaPBManager.hpp.

				inline const bool Voice_IsRecording (void) const { return m_bVoiceRecording; }
				const bool Voice_RecordStart (const char *const uncompressedFile, const char *const decompressedFile, const char *const micInputFile);
				void Voice_RecordStop (void);
				const unsigned short Voice_GetCompressedData (char *const destination, const int count, const bool isFinal);

				inline const bool IsChatterIconDisplayed  (void) const { return m_chatterIconHideTime > 0.0f; }
				inline void HideChatterIcon (const YaPB *const bot)
				{
					bot->HideChatterIcon ();	// Hide icon and stop voice communication.

					// Remember the current chatter icon state.
					m_chatterIconHideTime = 0.0f;
				}

				inline void PushChatterMessage (const ::ChatterManager::BankIndex_t bankIndex, const ::ChatterManager::ChatterMessage_t chatterMessageIndex, const float sayTime);	// Note: This function declared in YaPBManager.hpp.

				inline void ClearChatterMessagesPriorityQueue (void) { m_chatterMessagesPriorityQueue.RemoveAll (); }

				inline void InstantChatterMessage (const YaPB *const bot, const ::ChatterManager::ChatterMessage_t chatterMessageIndex);	// Note: This function declared in YaPBManager.hpp.

			//
			// Group: Engine callbacks.
			//
			public:
				inline void Think (const YaPB *const bot)
				{
					if (IsChatterIconDisplayed ())
					{
						if (m_chatterIconHideTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
							HideChatterIcon (bot);	// Hide icon and stop voice communication.
					}
					else if (m_chatterIconShowTime > 0.0f && m_chatterIconShowTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
					{
						bot->DisplayChatterIcon ();

						// Remember the current chatter icon state.
						m_chatterIconShowTime = 0.0f;
						m_chatterIconHideTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.9f + 0.1f;
/*
						#if defined _DEBUG
							if (!m_chatterMessagesPriorityQueue.IsEmpty ())
								InternalAssert (m_chatterMessagesPriorityQueue.GetFirstElement ().sayTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
						#endif	// if defined _DEBUG
*/
						return;
					}

					if (!m_chatterMessagesPriorityQueue.IsEmpty () && m_chatterMessagesPriorityQueue.GetFirstElement ().sayTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
						InstantChatterMessage (bot, m_chatterMessagesPriorityQueue.PopFront ());
				}
		} m_chatterManager;

		struct Enemy_t
		{
			const Client *const pointer;		// pointer to enemy client
			Math::Vector3D      origin;			// target origin chosen for shooting (for last enemy this is position where enemy was last visible)
			float               shootRadius;	// Radius of shoot target origin. (Used for minimum shooting cone calculation. For dumb bots this should be larger than needed....)
			float               distance;		// Distance from bot to 'origin'.
			Visibility_t        visibility;		// enemy visibility flags
			float               seeTime;		// time bot sees enemy (for last enemy this is time when enemy was last visible)

			inline Enemy_t (const Client *const enemy, const Math::Vector3D &inputOrigin, const float shootRadius, const Visibility_t inputVisibility, const float inputSeeTime) :
				pointer (enemy),
				origin (inputOrigin),
				shootRadius (shootRadius),
				distance (0.0f/*! @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */),
				visibility (inputVisibility),
				seeTime (inputSeeTime)
			{ /* VOID */ }
			inline Enemy_t (const Client *const enemy, const Math::Vector3D &inputOrigin, const Visibility_t inputVisibility, const float inputSeeTime) :
				pointer (enemy),
				origin (inputOrigin),
				shootRadius ((inputVisibility & Visibility_Head) > 0u ? 3.8f/*! @todo FIND REAL RADIUS FOR HEAD!!! */ : (inputVisibility & Visibility_Body) > 0u ? HalfLifeEngine::SDK::Constants::HumanWidth : 2.0f/*! @todo FIND OTHER VALUE */),
				distance (0.0f/*! @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */),
				visibility (inputVisibility),
				seeTime (inputSeeTime)
			{ /* VOID */ }

			//
			// Group: Private operators.
			//
			private:
				inline Enemy_t &operator = (const Enemy_t &/*right*/);	// Avoid "warning C4512: 'Enemy_t' : assignment operator could not be generated".

			public:
				inline const bool IsValid (void) const
				{
					#if defined _DEBUG
						if (this == NULL)
							return false;

						// Reliability check.
						InternalAssert (pointer->IsValid ());

						return true;
					#else
						return this != NULL;
					#endif	// if !defined _DEBUG
				}
		};
		Enemy_t                                        *m_enemy;
		Enemy_t                                        *m_lastEnemy;

		class EnemiesManager
		{
			//
			// Group: Type definitions.
			//
			public:
				struct Enemy_t
				{
					const Client                         *pointer;
					float                                 lastSeeTime;
					unsigned char                         lastVisibility;
					Math::Vector3D                        lastSeenOrigin;
					const NavigationMesh::NavigationArea *lastNavigationArea;

					inline Enemy_t (const Client *const client) : pointer (client), lastSeeTime (0.0f), lastVisibility (0u), lastSeenOrigin (Math::Vector3D::ZeroValue), lastNavigationArea (NULL) { /* VOID */ }
				};

				typedef StaticArray <Enemy_t *, unsigned char, HalfLifeEngine::SDK::Constants::MaximumClients> EnemiesArray;

			//
			// Group: Private members.
			//
			private:
				EnemiesArray m_enemies;	// All bot enemies - visible, invisible, hearable, etc....

				EnemiesArray m_visibleEnemies;
				EnemiesArray m_hearableEnemies;

			//
			// Group: Functions.
			//
			public:
				inline       Enemy_t *const GetEnemy    (const Client *const client)
				{
					// Reliability check.
					InternalAssert (client->IsValid ());

					for (unsigned char index (0u); index < m_enemies.GetElementNumber (); ++index)
						if (m_enemies[index]->pointer == client)
							return m_enemies[index];

					return NULL;
				}
				inline const Enemy_t *const GetEnemy    (const Client *const client) const
				{
					// Reliability check.
					InternalAssert (client->IsValid ());

					for (unsigned char index (0u); index < m_enemies.GetElementNumber (); ++index)
						if (m_enemies[index]->pointer == client)
							return m_enemies[index];

					return NULL;
				}
				inline const bool                    IsEnemy     (const Client *const enemy) const
				{
					// Reliability check.
					InternalAssert (enemy->IsValid ());

					return GetEnemy (enemy) != NULL;
				}
				inline void                          AddEnemy    (const Client *const enemy)
				{
					// Reliability check.
					InternalAssert (enemy->IsValid () && !IsEnemy (enemy));

					Enemy_t *const newEnemy (new Enemy_t (enemy));

					#if defined _DEBUG
						// Reliability check.
						if (newEnemy == NULL)
						{
							AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new enemy \"%s\"!", sizeof (*newEnemy), enemy->GetName ().GetData ());

							return;
						}
					#endif	// if defined _DEBUG

					m_enemies += newEnemy;
				}
				inline void                          RemoveEnemy (const Client *const enemy)
				{
					// Reliability check.
					InternalAssert (enemy->IsValid ());

					for (unsigned char index (0u); index < m_enemies.GetElementNumber (); ++index)
						if (m_enemies[index]->pointer == enemy)
						{
							m_enemies.Remove (index, 1u);

							break;
						}
				}
		}                                               m_enemiesManager;		// Enemies manager in a current game.

		struct Attacker_t
		{
			const Client *pointer;	// client/bot, which damaged me. (may be NULL for trigger or something else...)

			struct Damage_t
			{
				float                                        time;		// When damaged.
				unsigned short                               amount;	// Inflicted damage amount.
				Math::Vector3D                               origin;	// Damage origin.
				HalfLifeEngine::SDK::Constants::DamageType_t flags;		// Damage flags.

				inline Damage_t (void) : time (0.0f), amount (0u), origin (Math::Vector3D::ZeroValue), flags (HalfLifeEngine::SDK::Constants::DamageType_None) { /* VOID */ }
			}             damage;	// Information about inflicted damage.

			inline void Reset (void)
			{
				pointer = NULL;

				damage.time = 0.0f;
				damage.amount = 0u;
				damage.origin = Math::Vector3D::ZeroValue;
				damage.flags = HalfLifeEngine::SDK::Constants::DamageType_None;
			}

			inline Attacker_t (void) : pointer (NULL) { /* VOID */ }
		}                                               m_lastAttackerInformation;

		// This structure is sent over the net to describe a screen shake event.
		struct ScreenShakeInformation_t
		{
			float amplitude;	// amount of shake
			float duration;		// seconds duration
			float frequency;	// noise frequency (low frequency is a jerk, high frequency is a rumble)

			inline ScreenShakeInformation_t (void) : amplitude (0.0f), duration (0.0f), frequency (0.0f) { /* VOID */ }
		}                                               m_lastScreenShakeInformation;

		// This structure is sent over the net to describe a screen fade event.
		struct ScreenFadeInformation_t
		{
			const Client *flasher;	// client/bot, which flashed me (may be NULL, if flashed by some event)
			float         duration;	// seconds duration

			inline ScreenFadeInformation_t (void) : flasher (NULL), duration (0.0f) { /* VOID */ }
		}                                               m_lastScreenFadeInformation;

		static float                                    sm_maximumViewDistance;			// Global maximum view distance for all bots, for debugging and fun.
		float                                           m_maximumViewDistance;			// maximum view distance

		HalfLifeEngine::SDK::Structures::TraceResult_t  m_traceResult;					// Last trace result information.

		#include <BotTasks/BotTask.hpp>

		#include <BotAim.hpp>

		Aim                                            *m_aim;

		#include <BotNavigator.hpp>

		BaseNavigator                                  *m_navigator;					// Navigation used for this bot -- environment sensor 1

		#include <BotTasks/Bot_tasks_definitions.hpp>
		#include <BotTasks/BotActionManager.hpp>

		ActionManager                                   m_scheduleManager;				// bot tasks etc -- complex actuators.

		struct EnvironmentEntities_t
		{
			typedef DynamicArray <HalfLifeEngine::SDK::Classes::Edict *, unsigned char> Array_t;

			Array_t weapons;
			float   weaponsSearchRadius;
			float   weaponsCheckTime;

			Array_t shields;
			float   shieldsSearchRadius;
			float   shieldsCheckTime;

			Array_t grenades;
			float   grenadesSearchRadius;
			float   grenadesCheckTime;

			Array_t defuseKits;
			float   defuseKitsSearchRadius;
			float   defuseKitsCheckTime;

			Array_t hostages;
			float   hostagesSearchRadius;
			float   hostagesCheckTime;

			Array_t buttons;
			float   buttonsSearchRadius;
			float   buttonsCheckTime;
		} m_environmentEntities;
/*		// defines for pickup items
		enum PickupType_t	// type of entity which needs to be used/picked up
		{
			PickupType_None = -1,

			PickupType_Weapon,
//			PickupType_DroppedC4,
//			PickupType_PlantedC4,
			PickupType_Hostage,
			PickupType_Button,
			PickupType_ShieldGun,
			PickupType_DefuseKit,
			PickupType_Grenade,

			PickupType_Total
		};

		typedef DynamicArray <HalfLifeEngine::SDK::Classes::Edict *, unsigned char> PickupItemsArray_t;
		PickupItemsArray_t m_pickupItems[PickupType_Total];	// pointer to entity of item to use/pickup
		float              m_itemCheckTime;					// time next search for items needs to be done
*/
		const HalfLifeEngine::SDK::Classes::Edict      *m_button;						// Pointer to using button.

		struct Body_t
		{
			// immediate obstacle definitions
			DECLARE_TYPED_ENUMERATION (unsigned short, ObstacleFlag_t)
			{
				OBSTACLE_NONE,

				OBSTACLE_FRONT_LOWWALL    = BIT (0u),
				OBSTACLE_FRONT_LOWCEILING = BIT (1u),
				OBSTACLE_FRONT_WALL       = BIT (2u),
				OBSTACLE_FRONT_FALL       = BIT (3u),

				OBSTACLE_LEFT_LOWWALL     = BIT (4u),
				OBSTACLE_LEFT_LOWCEILING  = BIT (5u),
				OBSTACLE_LEFT_WALL        = BIT (6u),
				OBSTACLE_LEFT_FALL        = BIT (7u),

				OBSTACLE_RIGHT_LOWWALL    = BIT (8u),
				OBSTACLE_RIGHT_LOWCEILING = BIT (9u),
				OBSTACLE_RIGHT_WALL       = BIT (10u),
				OBSTACLE_RIGHT_FALL       = BIT (11u)
			};

			float          leftCheckDistance;	// distance on the left at which the terrain will be checked for obstacles
			float          frontCheckDistance;	// distance in front at which the terrain will be checked for obstacles
			float          rightCheckDistance;	// distance on the right at which the terrain will be checked for obstacles
			ObstacleFlag_t currentHitState;		// bit map description of the terrain in the surroundings of the bot
			ObstacleFlag_t previousHitState;	// the same description of surrounding terrain one frame before
			float          fallTime;			// date at which the bot last felt fast enough to get some damage
			Math::Vector3D fallPlaneNormal;		// vector describing the normal of a fall edge in front of bot
/*
			struct Obstacle_t
			{
				HalfLifeEngine::SDK::Structures::TraceResult_t traceResult_LowWall;
				HalfLifeEngine::SDK::Structures::TraceResult_t traceResult_LowCeiling;
				HalfLifeEngine::SDK::Structures::TraceResult_t traceResult_Wall;
				HalfLifeEngine::SDK::Structures::TraceResult_t traceResult_Fall;
			};
*/
			inline Body_t (void) :
				leftCheckDistance (0.0f),
				frontCheckDistance (0.0f),
				rightCheckDistance (0.0f),
				currentHitState (OBSTACLE_NONE),
				previousHitState (OBSTACLE_NONE),
				fallTime (0.0f),
				fallPlaneNormal (0.0f, 0.0f, 0.0f)
			{ /* VOID */ }
		};
		Body_t m_body;
		void CheckForObstacles (void);

		float m_avoidPlayersTime;	// next date at which the bot will think about players to avoid
		void AvoidPlayers (void);

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_YaPB; }

		inline void RunMove (void)
		{
			{
				m_frameIntervalTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - m_lastThinkTime;	// calculate frame interval time
				m_lastThinkTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();	// update last think interval for last calculation method

				m_msec.Think (m_frameIntervalTime);	// Calculate the msec value.
			}

			// ask the engine to do the fake client movement on server
			m_edict->RunPlayerMove (m_navigator->m_moveAngles, m_navigator->m_speed.move, m_navigator->m_speed.strafe, m_msec);
		}

		inline void         UpdateFallDownTime              (void)
		{
			// Is bot falling down?
			if (!IsOnLadder () && m_edict->IsFallingDown ())
			{
				if (m_fallDownTime == 0.0f)
					m_fallDownTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
			}
			else
				m_fallDownTime = 0.0f;
		}

		void                AI                              (void);
		void                UpdateEmotions                  (void);
		inline void         CheckUpdateEmotions             (void)
		{
			// slowly increase/decrease dynamic emotions back to their base level
			if (m_nextEmotionUpdate > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				return;

			UpdateEmotions ();

			// next emotion update in 500 milliseconds
			m_nextEmotionUpdate = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;
		}
		void                SetConditions                   (void);

		inline const bool WantsToRetreat (void) const { return false/*! @todo */; }

		// Note: The following 2 functions are declared in CurrentWeapon.hpp.
		inline void         FlashlightTurnOn                (void)
		{
			// This function depending on 'on' boolean, ON/OFF flashlight of a bot.

			if
			(
				IsUsesFlashlight () ||
				m_currentWeapon->GetNextAttackDelay () > 0.0f ||
				!g_server->IsFlashlightAllowed () ||
				GetFlashlightBattery () == 0u ||
				m_edict->GetPlayerModel ().CompareWithCaseCheck ("zombie", GET_STATIC_STRING_LENGTH ("zombie")) == 0
			)
				return;

			// switch ON
			m_edict->variables.impulse = HalfLifeEngine::SDK::Constants::PlayerImpulseID_ToggleFlashlight;	// equal 'impulse 100' in console, or default 'F' button :)
		}
		inline void         FlashlightTurnOff               (void)
		{
			// This function depending on 'on' boolean, ON/OFF flashlight of a bot.

			if
			(
				!IsUsesFlashlight () ||
				m_currentWeapon->GetNextAttackDelay () > 0.0f ||
				m_edict->GetPlayerModel ().CompareWithCaseCheck ("zombie", GET_STATIC_STRING_LENGTH ("zombie")) == 0
			)
				return;

			// switch OFF
			m_edict->variables.impulse = HalfLifeEngine::SDK::Constants::PlayerImpulseID_ToggleFlashlight;	// equal 'impulse 100' in console, or default 'F' button :)
		}

		void                CheckIllumination               (void);
		const bool          CheckVisibility                 (const HalfLifeEngine::SDK::Classes::Edict *const targetEntity, Math::Vector3D &origin, Visibility_t &bodyPart);
		const bool          IsSimpleDeadlyDrop              (const Math::Vector3D &destination);
		const bool          IsDeadlyDrop                    (const Math::Vector3D &destination);	/// DO WE NEED THIS????????????????????

		inline const bool   IsGoodChatPosition              (void) const
		{
			// This function returns true if the bot is at a good position to chat, false otherwise.

			// Do not chat if the bot is alive and under water or not standing on the ground.
			return m_edict->IsImmortal () || IsAlive () == false || (m_edict->IsInWater () == false && IsOnFloor ());
		}

		inline const bool   IsCanBuy                        (void) const
		{
			// This function returns true if the bot can buy something, false otherwise.

			return Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <bool> () && !HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetValue <bool> ("csdm_active") && !HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetValue <bool> ("gg_enabled") && !(g_server->GetMapType () & (Server::MapType_Aim | Server::MapType_Dm | Server::MapType_Ka | Server::MapType_Zm | Server::MapType_Awp)) && !IsVIP () && IsInBuyZone ();
		}

		inline void         GettingBored                    (void)
		{
			// keep boredom within Boredom_Maximum.
			if (m_boredom < Boredom_Maximum)
				++m_boredom;
		}
		inline void         HavingFun                       (void)
		{
			// keep boredom within Boredom_Minimum.
			if (m_boredom > Boredom_Minimum)
				--m_boredom;
		}
		inline const unsigned char GetBoredom                 (void) const { return m_boredom; }
		inline const float         GetBoredomReducingOffset   (void) const { return static_cast <float> (Boredom_Maximum - m_boredom) / Boredom_Maximum; }
		inline const float         GetBoredomIncreasingOffset (void) const { return static_cast <float> (m_boredom) / Boredom_Maximum; }

		inline const bool   IsWantToLeaveGame               (void) const
		{
/*			Cheeseh's leave the game formula!

			leave game prob = (Boredom_Maximum - Boredom) * m_joinServerTime
*/
			float timeLimit (HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetValue <float> ("mp_timelimit"));

			// Is less than 10 minutes?
			if (timeLimit < 10.0f)
				timeLimit = 30.0f;	// Set time limit to 30 minutes.

			// Convert into seconds.
			timeLimit *= 60.0f;

			const float boredomPercent (GetBoredomIncreasingOffset ());
			const float longGameTime (1.0f - (timeLimit - (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - m_joinServerTime)) / timeLimit);

			// Long game time = 0 to 1 number, 0 meaning 0% of time spent on server, 1 means 100% of time spent on server.
			return boredomPercent * longGameTime >= g_randomNumberGenerator.GetValueBetween (0.75f, 1.0f);	// 75% or more of wanting to leave.
		}

		const bool IsTemmateInLineOfFire (const float distance);

		const unsigned char GetVisibleEnemiesNumber        (void);
		const unsigned char GetEnemiesNumber               (void) const;
		const unsigned char GetNearbyTeammatesNearPosition (const Math::Vector3D &origin, float radius) const;
		const unsigned char GetNearbyEnemiesNearPosition   (const Math::Vector3D &origin, float radius) const;
		const bool          IsGroupOfEnemies               (const Math::Vector3D &location, const unsigned char numberOfEnemies, float radius) const;
		const bool          IsGroupOfEnemies               (unsigned char numberOfEnemies) const;
		const bool          IsHasEnemies                   (void) const;

		inline const bool ItemIsVisible (const Math::Vector3D &destination, const HalfLifeEngine::SDK::Classes::Edict *const item, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore)
		{
			// trace a line from bot's eyes to destination..
			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), destination, traceIgnore, m_edict, m_traceResult);

			// check if line of sight to object is not blocked (i.e. visible), check for standard items
			if (m_traceResult.fraction == 1.0f || m_traceResult.hitEntity == item)
				return true;

			return m_traceResult.fraction > 0.98f && m_traceResult.hitEntity->GetClassName ().CompareWithoutCaseCheck ("csdmw_", GET_STATIC_STRING_LENGTH ("csdmw_")) == 0;
		}

		const bool BendLineOfSight (const Math::Vector3D &destination, Math::Vector3D &bendDestination, const float maximumAngle = 135.0f);
		void        ComputeApproachPoints (void);
		inline void ResetApproachPoints   (void)
		{
			m_approachPointsCenter = Math::Vector3D::ZeroValue;

			RemoveApproachPoints ();
		}
		inline void RemoveApproachPoints  (void) { m_approachPoints.RemoveAll (); }
		inline void UpdateApproachPoints  (void)
		{
			const float maximumDistance (50.0f);
			const Math::Vector3D currentApproachPointsCenter (GetOrigin ());

			if (m_approachPointsCenter.GetDistanceSquared (currentApproachPointsCenter) > MATH_GET_SQUARED (maximumDistance))
			{
				ComputeApproachPoints ();

				m_approachPointsCenter = currentApproachPointsCenter;
			}
		}
		#if defined _DEBUG
		inline void DrawApproachPoints    (void) const;	// Note: This function declared in HumanManager.hpp.
		#endif	// if defined _DEBUG

		inline Aim           *const GetAim       (void) const { return m_aim; }
		inline BaseNavigator *const GetNavigator (void) const { return m_navigator; }

		void MoveTowardsPosition (const Math::Vector3D &position);
/*
		inline const bool IsCanDamagedByGrenadeExplosion (const HalfLifeEngine::SDK::Classes::Edict *const grenade) const
		{
			// Reliability check.
			InternalAssert (grenade->IsValid ());

			new target = -1, Float:origin[3]
			pev(ent, pev_origin, origin)

			while ((target = find_ent_in_sphere (target, origin, radius)))
			{
				static Float:o[3]
				pev(target, pev_origin, o)

				xs_vec_sub(origin, o, o)

				// Recheck if the entity is in radius
				if (xs_vec_len(o) > radius)
					continue

				Ham_ExecDamageB(target, ent, pev(ent, pev_owner), damage * (xs_vec_len(o) / radius), HIT_GENERIC, bits)
			}

			return false;
		}

		inline const bool IsCanDamagedByGrenadeExplosion (const HalfLifeEngine::SDK::Classes::Edict *const grenade) const
		{
			// Reliability check.
			InternalAssert (grenade->IsValid ());

			new target = -1, Float:origin[3]
			pev(ent, pev_origin, origin)

			while ((target = find_ent_in_sphere (target, origin, radius)))
			{
				static Float:o[3]
				pev(target, pev_origin, o)

				xs_vec_sub(origin, o, o)

				// Recheck if the entity is in radius
				if (xs_vec_len(o) > radius)
					continue

				Ham_ExecDamageB(target, ent, pev(ent, pev_owner), damage * (xs_vec_len(o) / radius), HIT_GENERIC, bits)
			}

			return false;
		}
*/
		inline void EnteredBombPlace (void)
		{
			{
				if (m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_BOMB_PLACE)
					return;

				m_currentMapZone |= HalfLifeEngine::SDK::Constants::IN_BOMB_PLACE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::EnteredBombPlace(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnEnteredBombPlace ();
		}
		inline void LeaveBombPlace (void)
		{
			{
				if (!(m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_BOMB_PLACE))
					return;

				m_currentMapZone &= ~HalfLifeEngine::SDK::Constants::IN_BOMB_PLACE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::LeaveBombPlace(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnLeaveBombPlace ();
		}
		inline void EnteredRescueZone (void)
		{
			{
				if (m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_HOSTAGES_RESCUE_ZONE)
					return;

				m_currentMapZone |= HalfLifeEngine::SDK::Constants::IN_HOSTAGES_RESCUE_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::EnteredRescueZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnEnteredRescueZone ();
		}
		inline void LeaveRescueZone (void)
		{
			{
				if (!(m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_HOSTAGES_RESCUE_ZONE))
					return;

				m_currentMapZone &= ~HalfLifeEngine::SDK::Constants::IN_HOSTAGES_RESCUE_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::LeaveRescueZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnLeaveRescueZone ();
		}
		inline void EnteredEscapeZone (void)
		{
			{
				if (m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_ESCAPE_ZONE)
					return;

				m_currentMapZone |= HalfLifeEngine::SDK::Constants::IN_ESCAPE_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::EnteredEscapeZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnEnteredEscapeZone ();
		}
		inline void LeaveEscapeZone (void)
		{
			{
				if (!(m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_ESCAPE_ZONE))
					return;

				m_currentMapZone &= ~HalfLifeEngine::SDK::Constants::IN_ESCAPE_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::LeaveEscapeZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnLeaveEscapeZone ();
		}
		inline void EnteredVIPSafetyZone (void)
		{
			{
				if (m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_VIP_RESCUE_ZONE)
					return;

				m_currentMapZone |= HalfLifeEngine::SDK::Constants::IN_VIP_RESCUE_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::EnteredVIPSafetyZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnEnteredVIPSafetyZone ();
		}
		inline void LeaveVIPSafetyZone (void)
		{
			{
				if (!(m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_VIP_RESCUE_ZONE))
					return;

				m_currentMapZone &= ~HalfLifeEngine::SDK::Constants::IN_VIP_RESCUE_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::LeaveVIPSafetyZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnLeaveVIPSafetyZone ();
		}
		inline void EnteredBuyZone (void)
		{
			{
				if (m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_BUY_ZONE)
					return;

				m_currentMapZone |= HalfLifeEngine::SDK::Constants::IN_BUY_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::EnteredBuyZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnEnteredBuyZone ();

//			EquipInBuyzone (0);	// Try to equip in buy zone.
		}
		inline void LeaveBuyZone (void)
		{
			{
				if (!(m_currentMapZone & HalfLifeEngine::SDK::Constants::IN_BUY_ZONE))
					return;

				m_currentMapZone &= ~HalfLifeEngine::SDK::Constants::IN_BUY_ZONE;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::LeaveBuyZone(): Called for bot \"%s\"!\n", GetName ().GetData ());

			m_scheduleManager.OnLeaveBuyZone ();
		}

		inline void ShowMenuTeamSelect (void)
		{
			// Reliability check.
			if (m_gameAction == GameAction_TeamSelect)
				return;

			m_gameAction = GameAction_TeamSelect;

			// if the bot hasn't selected stuff to start the game yet, go do that....
			m_scheduleManager.AddAction (new Task_SelectTeam (this));

//			m_scheduleManager.OnShowMenuTeamSelect ();
		}
		inline void ShowMenuModelSelect (void)
		{
			// Reliability check.
			if (m_gameAction == GameAction_ModelSelect)
				return;

			m_gameAction = GameAction_ModelSelect;

			m_scheduleManager.AddAction (new Task_SelectModel (this));

//			m_scheduleManager.OnShowMenuModelSelect ();
		}

	//
	// Group: (Con/De)structors.
	//
	public:
		YaPB (HalfLifeEngine::SDK::Classes::Edict *const bot, Profile *const profile);

		virtual inline ~YaPB (void)
		{
			// This is bot destructor.

			m_scheduleManager.FreeMemory ();	/// Clear tasks. @note Do it before we delete enemy, aim, and navigator.....
/*! @warning 'm_profile' POINTER IS NOT EXISTS FOR THIS MOMENT (IN SOME CASES - QUIT GAME, LISTEN SERVER HOST DISCONNECT, ....)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			m_profile->DeleteHALBrain ();

			m_profile->isUsed = false;	// This profile is not used now.
//*/
			delete m_enemy;

			// Delete subsystems....
			delete m_aim;
			delete m_navigator;
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsYaPB
		//
		// Description: Checks whether client is YaPB.
		//
		// Returns: True if client is YaPB, false otherwise.
		//
		inline const bool                  IsYaPB                  (void) const { return true; }

		//
		// Function: IsOtherBot
		//
		// Description: Checks whether client is other bot, not a YaPB.
		//
		// Returns: True if client is other bot, not a YaPB, false otherwise.
		//
		inline const bool                  IsOtherBot              (void) const { return false; }

		inline const bool                  CanDoingScenario        (void) const;	/// @note This function declared in HumanManager.hpp.

		inline void                        ChangeTeam              (void) const
		{
			// Reliability check.
			InternalAssert (!IsSpectator ());

			const HalfLifeEngine::SDK::Constants::TeamID_t otherTeamIndex (GetRealTeam () == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

			ExecuteCommandFormat ("chooseteam; menuselect %u; menuselect %u", otherTeamIndex, m_profile->model[otherTeamIndex - 1u]);
		}

		inline const float                 GetFrameIntervalTime    (void) const { return m_frameIntervalTime; }

		inline const unsigned char         GetMsecValue            (void) const { return m_msec; }

		inline const Profile *const        GetProfile              (void) const { return m_profile; }

		inline HalfLifeEngine::SDK::Structures::TraceResult_t &GetTraceResult (void) { return m_traceResult; }

		inline       RadioCommandsManager &GetRadioCommandsManager (void)       { return m_radioCommandsManager; }
		inline const RadioCommandsManager &GetRadioCommandsManager (void) const { return m_radioCommandsManager; }

		inline       ChatterManager       &GetChatterManager       (void)       { return m_chatterManager; }
		inline const ChatterManager       &GetChatterManager       (void) const { return m_chatterManager; }

		inline const unsigned short GetCalculatedPing (void)
		{
			if (m_profile->ping == BotPingValue_BotTag || (m_profile->ping == BotPingValue_Minimum && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PingFlux)->GetValue <unsigned short> () == 0u))
				return 0u;

			// Calculate ping....
			m_profile->ping.Calculate ();

			return m_profile->ping;
		}

		void ReactOnRadio   (const RadioCommandsManager::RadioCommand *const radioMessage);
//		void ReactOnChatter (const ChatterCommandsManager::ChatterCommand *const chatterMessage);

		inline void SelectWeapon (const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID) const
		{
			// Reliability check.
			InternalAssert (weaponID >= HalfLifeEngine::SDK::Constants::WeaponID_P228 && weaponID <= HalfLifeEngine::SDK::Constants::WeaponID_P90);

			// Select this weapon if it isn't already selected.
			ExecuteCommand (HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].className);
		}
		inline void DropWeapon (const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID) const
		{
			// Reliability check.
			InternalAssert (weaponID >= HalfLifeEngine::SDK::Constants::WeaponID_P228 && weaponID <= HalfLifeEngine::SDK::Constants::WeaponID_P90);

			ExecuteCommandFormat ("drop %s", HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].className.GetData ());
		}
		inline void DropPrimaryWeapon (void) const
		{
			if (m_owningItemsManager->HasPrimaryWeapon ())
				DropWeapon (m_owningItemsManager->GetRandomPrimaryWeaponID ());
		}
		inline void DropAllPrimaryWeapons (void) const
		{
			const OwningItemsManager::ItemsArray_t &primaryItems (m_owningItemsManager->GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons));

			for (OwningItemsManager::ItemsArray_t::IndexType_t primaryItemIndex (0u); primaryItemIndex < primaryItems.GetElementNumber (); ++primaryItemIndex)
				DropWeapon (primaryItems[primaryItemIndex]->GetID ());
		}
		inline void DropSecondaryWeapon (void) const
		{
			if (m_owningItemsManager->HasSecondaryWeapon ())
				DropWeapon (m_owningItemsManager->GetRandomSecondaryWeaponID ());
		}
		inline void DropAllSecondaryWeapons (void) const
		{
			const OwningItemsManager::ItemsArray_t &secondaryItems (m_owningItemsManager->GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_SecondaryWeapons));

			for (OwningItemsManager::ItemsArray_t::IndexType_t secondaryItemIndex (0u); secondaryItemIndex < secondaryItems.GetElementNumber (); ++secondaryItemIndex)
				DropWeapon (secondaryItems[secondaryItemIndex]->GetID ());
		}
		inline void DropCurrentWeapon (void) const
		{
			ExecuteCommandFormat ("drop");
		}

		void        SelectBestWeapon                 (void);
		inline void SelectBestPistol                 (void)
		{
			// Save the original 'm_edict->variables.weapons' bits.
			const HalfLifeEngine::SDK::Constants::WeaponBit_t originalWeaponsBits (m_edict->variables.weapons);

			// HACK: Let's deceive the bottom function, having removed the primary weapons from a 'm_edict->variables.weapons' mask, that the bot chose the best weapon only from pistols.
			m_edict->variables.weapons &= ~HalfLifeEngine::SDK::Constants::WeaponBits_Primary;

			SelectBestWeapon ();

			// Restore the original 'm_edict->variables.weapons' bits.
			m_edict->variables.weapons = originalWeaponsBits;
		}
		inline void SelectBestWeaponWithHighestSpeed (void) { SelectWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Scout); }	//! @TODO!!!!!!!!!!!!!!!!

		inline void VoteMap (const DynamicString &mapName) const
		{
			ExecuteCommand ("listmaps");

			const HalfLifeEngine::SDK::Structures::MapCycle_t &mapCycle (g_server->GetMapCycleInformation ());
			const HalfLifeEngine::SDK::Structures::MapCycle_t::Item_t *item (mapCycle.nextItem);

			if (item == NULL)
				return;

			unsigned char mapIndex (0u);

			// Traverse list....
			do
			{
				// Reliability check.
				InternalAssert (item != NULL);

				++mapIndex;

				if (mapName.CompareWithoutCaseCheck (item->mapName) == 0)
				{
					VoteMap (mapIndex);

					return;
				}
			} while ((item = item->next) != mapCycle.nextItem);
		}
		inline void VoteMap (const unsigned char mapIndex) const
		{
			ExecuteCommandFormat ("votemap %u", mapIndex);
		}
		inline void VoteKick (const Client *const client) const
		{
			ExecuteCommandFormat ("vote %u", client->GetEdict ()->GetIndex ());
		}

		void       StartChatter       (const char *const chatterMessageSoundPath) const;
		void       DisplayChatterIcon (void) const;
		void       HideChatterIcon    (void) const;

		// Note: The following 7 functions are declared in HumanManager.hpp.
//		inline void ChatMessage    (const ChatManager::ChatMessage_t type, const bool isTeamSay);
		inline const bool IsCanUseRadio (void) const;
		inline void RadioMessage   (const HalfLifeEngine::SDK::Constants::RadioCommand_t message);
		inline void RadioMessage   (const HalfLifeEngine::SDK::Constants::RadioCommand_t message, const float sayTime);
		inline const bool IsCanUseChatter (void) const;
		inline void       ChatterMessage  (const ::ChatterManager::ChatterMessage_t chatterMessageIndex);
		inline void       ChatterMessage  (const ::ChatterManager::ChatterMessage_t chatterMessageIndex, const float sayTime);

		const bool IsBehindSmokeClouds (const Math::Vector3D &origin);

		const bool IsBombAudible (void) const;

		const bool FindEnemy (void);

		inline void PrintBoredoms (const Human *const to) const
		{
			to->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "%s's boredom = %u (%0.1f percent).\n", GetName ().GetData (), GetBoredom (), GetBoredomIncreasingOffset ());
		}

		inline const bool   IsPositionVisible              (const Math::Vector3D &destination)
		{
			const Math::Vector3D eyePosition (m_edict->GetEyePosition ());

			// don't look through water
			if ((HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (eyePosition) == HalfLifeEngine::SDK::Constants::Content_Water) != (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (destination) == HalfLifeEngine::SDK::Constants::Content_Water))
				return false;

			// trace a line from bot's eyes to destination...
			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (eyePosition, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, m_traceResult);

			// check if line of sight to object is not blocked (i.e. visible)
			return m_traceResult.fraction >= 1.0f;
		}

		inline const bool IsEnemy (const Client *const client) const
		{
			return client->GetTeam () != GetTeam () && !client->GetEdict ()->IsImmortal ();
		}

		const Math::Vector3D &GetEnemyAimPosition (void);

		inline void DeleteEnemy (void)
		{
			delete m_enemy;

			m_enemy = NULL;
		}

		const bool CheckFirePause  (const float distance, const FireDelay_t &fireDelay);
		void       FireWeapon      (void);
		void       CheckFocusEnemy (void);

		const bool          IsShootableThruObstacle         (Math::Vector3D source, const Math::Vector3D &destination);
		const unsigned char GetApproximateBaseDamage        (const Math::Vector3D &source, const Math::Vector3D &destination);

		Waypoint *const                       FindGoalWaypoint (void);
		NavigationMesh::NavigationArea *const FindGoalArea     (void);

		static void LoadBotSkillTables (void);

		static void BuildWaypointMapZones (void);
		static inline void RebuildWaypointMapZones (void)
		{
			{
				s_bombPlaceWaypointMapZones.RemoveAll ();
				s_hostageWaypointMapZones.RemoveAll ();
				s_hostageRescueWaypointMapZones.RemoveAll ();
				s_VIPSafetyWaypointMapZones.RemoveAll ();
			}

			BuildWaypointMapZones ();
		}
		static void BuildAreaMapZones     (void);
		static inline void RebuildAreaMapZones (void)
		{
			{
				s_bombPlaceAreaMapZones.RemoveAll ();
				s_hostageAreaMapZones.RemoveAll ();
				s_hostageRescueAreaMapZones.RemoveAll ();
				s_VIPSafetyAreaMapZones.RemoveAll ();
			}

			BuildAreaMapZones ();
		}
		static const Math::Vector3D GetRandomPositionInAreaZone (const AreaMapZone_t &zone);

		static inline const float GetApproximateFallDamage (float distance)
		{
			distance *= 0.21780001f;	/// @todo MAKE UNDERSTANDABLE VALUE!!!
			distance -= 26.0f;

			return distance < 0.0f ? 0.0f : distance;
		}

		inline const float GetAgressionLevel (void) const { return m_agressionLevel; }
		inline const float GetFearLevel      (void) const { return m_fearLevel; }

		void CheckSilencer (void);
		inline void TryCheckSilencer (void)
		{
			// check for silencer
			if (m_silencerCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				// update silencer status
				m_silencerCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (5.0f, 10.0f);

				CheckSilencer ();
			}
		}
		void CheckReload (void);
		inline void TryCheckReload (void)
		{
			// check for reloading
			if (m_reloadCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				// update reloading status
				m_reloadCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.0f;

				CheckReload ();
			}
		}

		inline const Profile::WeaponsPreferences_t::Array_t::IndexType_t GetBestWeaponCarried (void) const
		{
			// this function returns the best weapon of this bot (based on personality preferences).
/*
			HalfLifeEngine::SDK::Constants::WeaponBit_t weapons (m_edict->variables.weapons);

			// take the shield in account
			if (HasShield ())
				weapons |= HalfLifeEngine::SDK::Constants::WeaponBit_ShieldGun;
*/
			// loop through all weapons
			for (Profile::WeaponsPreferences_t::Array_t::IndexType_t index (0u); index < GetProfile ()->weaponsPreferences.common.GetElementNumber (); ++index)
			{
				// is the bot carrying this weapon?
//				if (weapons & BIT (GetProfile ()->weaponsPreferences.common[index]))
				if (HasWeapon (GetProfile ()->weaponsPreferences.common[index]))
					return index;
			}

			return Profile::WeaponsPreferences_t::Array_t::BadPosition;
		}
		inline const Profile::WeaponsPreferences_t::Array_t::IndexType_t GetBestSecondaryWeaponCarried (void) const
		{
			// this function returns the best secondary weapon of this bot (based on personality prefs).
/*
			HalfLifeEngine::SDK::Constants::WeaponBit_t weapons (m_edict->variables.weapons);

			// take the shield in account
			if (HasShield ())
				weapons |= HalfLifeEngine::SDK::Constants::WeaponBit_ShieldGun;
*/
			// loop through all weapons
			for (Profile::WeaponsPreferences_t::Array_t::IndexType_t index (0u); index < GetProfile ()->weaponsPreferences.common.GetElementNumber (); ++index)
			{
//				if ((weapons & BIT (GetProfile ()->weaponsPreferences.common[index])) && HalfLifeEngine::Utilities::WeaponIsSecondary (GetProfile ()->weaponsPreferences.common[index]))
				if (HasWeapon (GetProfile ()->weaponsPreferences.common[index]) && HalfLifeEngine::Utilities::WeaponIsSecondary (GetProfile ()->weaponsPreferences.common[index]))
					return index;
			}

			return Profile::WeaponsPreferences_t::Array_t::BadPosition;
		}
		inline const bool RateGroundWeapon (const HalfLifeEngine::SDK::Classes::Edict *const weapon) const
		{
			// this function compares weapons on the ground to the one the bot is using.

			HalfLifeEngine::SDK::Constants::WeaponID_t weaponID;

			for (weaponID = HalfLifeEngine::SDK::Constants::WeaponID_None; ++reinterpret_cast <unsigned int &> (weaponID) < HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u; /* Empty */)
				if (HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].modelName == weapon->GetNonPlayerModel ())
					break;

			if (weaponID == HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u)
				return false;

//			const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID (weapon->GetWeaponID ());

			for (Profile::WeaponsPreferences_t::Array_t::IndexType_t index (0u); index < GetProfile ()->weaponsPreferences.common.GetElementNumber (); ++index)
			{
				if (GetProfile ()->weaponsPreferences.common[index] != weaponID)
					continue;

				// check if pistol
				return index < (HalfLifeEngine::Utilities::WeaponIsPistol (weaponID) ? GetBestSecondaryWeaponCarried () : GetBestWeaponCarried ());
			}

			return false;
		}
		inline const HalfLifeEngine::SDK::Classes::Edict *const GetBestEnvironmentWeapon (void) const
		{
			float closestSquaredDistance (MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::MapSize));
			const HalfLifeEngine::SDK::Classes::Edict *closestWeapon (NULL);

			for (EnvironmentEntities_t::Array_t::IndexType_t index (0u); index < m_environmentEntities.weapons.GetElementNumber (); ++index)
			{
				// Reliability check.
				InternalAssert (m_environmentEntities.weapons[index]->IsValid ());

				if (!RateGroundWeapon (m_environmentEntities.weapons[index]))
					continue;

				const float squaredDistance (GetOrigin ().GetDistanceSquared (m_environmentEntities.weapons[index]->GetOrigin ()));

				if (closestSquaredDistance > squaredDistance)
				{
					closestSquaredDistance = squaredDistance;
					closestWeapon = m_environmentEntities.weapons[index];
				}
			}

			return closestWeapon;
		}

		void UpdateEnvironmentWeapons    (void);
		void UpdateEnvironmentShields    (void) { /* TODO!!! */ }
		void UpdateEnvironmentGrenades   (void) { /* TODO!!! */ }
		void UpdateEnvironmentDefuseKits (void) { /* TODO!!! */ }
		void UpdateEnvironmentHostages   (void) { /* TODO!!! */ }
		void UpdateEnvironmentButtons    (void) { /* TODO!!! */ }
		void UpdateEnvironmentEntities (void)
		{
			UpdateEnvironmentWeapons    ();
			UpdateEnvironmentShields    ();
			UpdateEnvironmentGrenades   ();
			UpdateEnvironmentDefuseKits ();
			UpdateEnvironmentHostages   ();
			UpdateEnvironmentButtons    ();
		}
		void CheckItemsToPickup (void);

		#if defined _DEBUG
			inline const bool IsDebuggingBot (void) const;	// Note: This function declared in YaPBManager.hpp.

			void DebugMessage       (const char *const message)     const;
			void DebugMessageFormat (const char *const format, ...) const;
		#endif	// if defined _DEBUG

		inline void PressDuck (void)
		{
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_DUCK;
		}
		inline void ReleaseDuck (void)
		{
			m_edict->variables.buttons &= ~HalfLifeEngine::SDK::Constants::IN_DUCK;
		}
		inline const bool IsDuckPressed (void)
		{
			return (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_DUCK) > 0u;
		}

		inline void PressJump (void)
		{
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_JUMP;
		}
		inline void ReleaseJump (void)
		{
			m_edict->variables.buttons &= ~HalfLifeEngine::SDK::Constants::IN_JUMP;
		}
		inline const bool IsJumpPressed (void)
		{
			return (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_JUMP) > 0u;
		}

		inline void PressPrimaryAttack (void)
		{
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK;
		}
		inline void ReleasePrimaryAttack (void)
		{
			m_edict->variables.buttons &= ~HalfLifeEngine::SDK::Constants::IN_ATTACK;
		}
		inline const bool IsPrimaryAttackPressed (void)
		{
			return (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK) > 0u;
		}
		inline void TogglePrimaryAttack (void)
		{
			if (IsPrimaryAttackPressed ())
				ReleasePrimaryAttack ();
			else
				PressPrimaryAttack ();
		}
		inline void PressSecondaryAttack (void)
		{
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;
		}
		inline void PressReload (void)
		{
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_RELOAD;
		}

	//
	// Group: Game message handler callbacks.
	//
	public:
		inline void AmmoAmountChanged (const HalfLifeEngine::SDK::Constants::WeaponID_t ammoID, const unsigned char newAmount)
		{
			m_scheduleManager.OnAmmoAmountChanged (ammoID, newAmount);

			// Post call this function in base class....
			FakeClient::AmmoAmountChanged (ammoID, newAmount);
		}

		void RoundStarted (void);
		inline void RoundEnded (void)
		{
			// This function is called at the end of each round (i.e. after next messages: Terrorists Win!, Counter-Terrorists Win! or something else....).

			m_scheduleManager.OnRoundEnded ();

			m_reloadCheckTime = 0.0f;	// Force to check reload.
		}
		inline void TerroristsWin (void)
		{
			// This function is called at the end of each round.
		}
		inline void CounterTerroristsWin (void)
		{
			// This function is called at the end of each round.
		}
		inline void BombPlanted (void)
		{
			m_scheduleManager.OnBombPlanted ();
		}
		inline void BombDetonated (void)
		{
			m_scheduleManager.OnBombDetonated ();
		}
		inline void BombDefused (void)
		{
			m_scheduleManager.OnBombDefused ();
		}

		inline void StatusIconChanged (const HalfLifeEngine::SDK::Constants::StatusIcon_StatusType_t status, const DynamicString &spriteName)
		{
			switch (status)
			{
				case HalfLifeEngine::SDK::Constants::StatusIcon_StatusType_Hide:
				{
					if (spriteName == "buyzone")
						LeaveBuyZone ();
					else if (spriteName == "c4")
						LeaveBombPlace ();
					else if (spriteName == "rescue")
						LeaveRescueZone ();
					else if (spriteName == "escape")
						LeaveEscapeZone ();
					else if (spriteName == "vipsafety")
						LeaveVIPSafetyZone ();

					break;
				}

				case HalfLifeEngine::SDK::Constants::StatusIcon_StatusType_Show:
				{
					if (spriteName == "buyzone")
						EnteredBuyZone ();
					else if (spriteName == "c4")
						LeaveBombPlace ();
					else if (spriteName == "rescue")
						EnteredRescueZone ();
					else if (spriteName == "escape")
						EnteredEscapeZone ();
					else if (spriteName == "vipsafety")
						EnteredVIPSafetyZone ();

					break;
				}

				case HalfLifeEngine::SDK::Constants::StatusIcon_StatusType_Flash:
				{
					if (spriteName == "c4")
						EnteredBombPlace ();

					break;
				}
			}
		}

		void TakeDamage  (const HalfLifeEngine::SDK::Classes::Edict *const inflictor/*, const HalfLifeEngine::SDK::Classes::Edict *const attacker*/, const unsigned short damage, const unsigned char armor, const HalfLifeEngine::SDK::Constants::DamageType_t bits);
		void TakeBlinded (const float duration, const Client *const flasher);
		void TakeShakes  (const float amplitude, const float duration, const float frequency);

		inline void ShowMenu (const DynamicString &menuTitle)
		{
			// team select menu?
			if
			(
				menuTitle == "#Team_Select" ||
				menuTitle == "#Team_Select_Spect" ||
				menuTitle == "#IG_Team_Select_Spect" ||
				menuTitle == "#IG_Team_Select" ||
				menuTitle == "#IG_VIP_Team_Select" ||
				menuTitle == "#IG_VIP_Team_Select_Spect"
			)
				ShowMenuTeamSelect ();
			else if	// model select menu?
			(
				menuTitle == "#Terrorist_Select" ||	// terrorist model select?
				menuTitle == "#CT_Select"			// counter-terrorist model select menu?
			)
				ShowMenuModelSelect ();
		}
		inline void OnCloseMenu (void)
		{
			/// @todo !!!
		}
		inline void VGUIMenuShow (const HalfLifeEngine::SDK::Constants::VGUIMenuID_t menuID)
		{
			switch (menuID)
			{
				// team select menu
				case HalfLifeEngine::SDK::Constants::VGUIMenuID_TeamSelect:
					ShowMenuTeamSelect ();

					break;

				// terrorist OR counter-terrorist select menu
				case HalfLifeEngine::SDK::Constants::VGUIMenuID_ModelSelect_Terrorist:
				case HalfLifeEngine::SDK::Constants::VGUIMenuID_ModelSelect_CounterTerrorist:
					ShowMenuModelSelect ();

					break;
			}
		}

	//
	// Group: Engine callbacks.
	//
	public:
		void        AliveSpawnPost   (void);
		inline void RoundRespawnPost (void)
		{
			// Call this function in base class....
			FakeClient::RoundRespawnPost ();

			if (m_chatterManager.IsChatterIconDisplayed ())
				HideChatterIcon ();	// Hide icon and stop voice communication.
		}
		void        Killed           (const HalfLifeEngine::SDK::Classes::Edict *const killer);	// Is called when the bot is killed.
		void        Kill             (const Client *const victim);
		void        ClientKilled     (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const killer);

		inline void FreezePeriodExpired (void)
		{
			/// @todo !!!
		}

		void OnEmitSound (const HalfLifeEngine::SDK::Classes::Edict *const edict, const NoisedSound_t &sound);

		inline void OnClientDisconnect (Client *const client)
		{
			// This function called when somebody disconnects from server.

			m_scheduleManager.OnClientDisconnect (client);
/*EXECUTES FROM Task_AttackEnemy task.
			if (m_enemy->IsValid () && m_enemy->pointer == client)
			{
				// null out the enemy pointer as it's no longer valid
				DeleteEnemy ();

				return;
			}
*/
			/// @todo Do some actions here....
		}
		inline void Disconnect       (void) const
		{
			// This function called when bot kicked off from the server.
/*
			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Default, false, "YaPB::Disconnect() called for bot \"%s\"!", GetName ().GetData ());
			#endif	// if defined _DEBUG
*/
			if (m_chatterManager.IsChatterIconDisplayed ())
				HideChatterIcon ();	// Remove displayed chatter icon.

			// save this personality's HAL brain
//			m_profile->SaveHALBrainForPersonality ();
/*// @fixme TEMORAILY MOVED HERE!!!!!!!!!!!
			m_profile->DeleteHALBrain ();

			m_profile->isUsed = false;	// This profile is not used now.
*/		}

		inline void Kick             (void);	// Note: This function declared in YaPBManager.hpp.

		void        Think            (void);

		inline void HearRadioCommand (const Client *const from, const HalfLifeEngine::SDK::Constants::RadioCommand_t radioCommand)
		{
//			AddLogEntry (true, LogLevel_Default, false, "YaPB::HearRadioCommand(): Bot \"%s\" hear radio command: %s, from client: \"%s\".", GetName ().GetData (), radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe ? "CoverMe" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_YouTakePoint ? "YouTakePoint" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_HoldPosition ? "HoldPosition" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_RegroupTeam ? "RegroupTeam" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe ? "FollowMe" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_TakingFire ? "TakingFire" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_GoGoGo ? "GoGoGo" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_FallBack ? "FallBack" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_StickTogether ? "StickTogether" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_GetInPosition ? "GetInPosition" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_StormTheFront ? "StormTheFront" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_ReportTeam ? "ReportTeam" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_Affirmative ? "Affirmative" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_EnemySpotted ? "EnemySpotted" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup ? "NeedBackup" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_SectorClear ? "SectorClear" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_InPosition ? "InPosition" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_ReportingIn ? "ReportingIn" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_ShesGonnaBlow ? "ShesGonnaBlow" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_Negative ? "Negative" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_EnemyDown ? "EnemyDown" : "UNKNOWN!!!", from->GetName ().GetData ());

			m_radioCommandsManager.HearRadioCommand (from, radioCommand, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_profile->baseReactTime);
		}
		inline void HearChatterMessage (const Client *const/* from*/, const ::ChatterManager::ChatterMessage_t/* chatterMessage*/)
		{
/*			AddLogEntry (true, LogLevel_Default, false, "YaPB::HearChatterMessage(): Bot \"%s\" hear chatter message: %s, from client: \"%s\".", GetName ().GetData (), radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe ? "CoverMe" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_YouTakePoint ? "YouTakePoint" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_HoldPosition ? "HoldPosition" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_RegroupTeam ? "RegroupTeam" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe ? "FollowMe" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_TakingFire ? "TakingFire" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_GoGoGo ? "GoGoGo" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_FallBack ? "FallBack" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_StickTogether ? "StickTogether" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_GetInPosition ? "GetInPosition" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_StormTheFront ? "StormTheFront" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_ReportTeam ? "ReportTeam" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_Affirmative ? "Affirmative" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_EnemySpotted ? "EnemySpotted" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup ? "NeedBackup" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_SectorClear ? "SectorClear" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_InPosition ? "InPosition" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_ReportingIn ? "ReportingIn" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_ShesGonnaBlow ? "ShesGonnaBlow" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_Negative ? "Negative" : radioCommand == HalfLifeEngine::SDK::Constants::RadioCommand_EnemyDown ? "EnemyDown" : "UNKNOWN!!!", from->GetName ().GetData ());

			m_chatterMessagesManager.HearChatterMessage (from, radioCommand, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_profile->baseReactTime);
*/		}
		inline void HearNoisedSound (const Client *const from, const NoisedSound_t &noisedSound)
		{
			m_scheduleManager.OnHearNoisedSound (from, noisedSound);
		}
		inline const bool IsCanHearSound (const Client *const from, const NoisedSound_t &noisedSound) const
		{
//			return from->GetOrigin ().GetDistanceSquared (GetOrigin ()) <= MATH_GET_SQUARED (noisedSound.hearingDistance);
//#if 0
			// calculate stereo seperation and distance attenuation

			Math::Vector3D directionNormalized (GetOrigin () - from->GetOrigin ());
			float distance (directionNormalized.GetLength ());
			float rightScale, leftScale;

			if (GetProfile ()->hasStereo)
			{
				directionNormalized /= distance;	// Normalize.

				const float dot (GetViewAngles ().BuildRightVector () | directionNormalized);

				rightScale = 1.0f + dot;
				leftScale = 1.0f - dot;
			}
			else
				rightScale = leftScale = 1.0f;

			distance = 1.0f - distance * noisedSound.dist_mult;

			// add in distance effect
			float volumeScale (distance * rightScale);
			int rightvol (static_cast <int> (noisedSound.master_vol * volumeScale));	// 0-255 volume
			if (rightvol < 0)
				rightvol = 0;

			return rightvol >= 8;

			volumeScale = distance * leftScale;
			int leftvol (static_cast <int> (noisedSound.master_vol * volumeScale));	// 0-255 volume
			if (leftvol < 0)
				leftvol = 0;

			return leftvol >= 8;
//			return rightvol >= 8 || leftvol >= 8;
//#endif	// if 0
		}

		void Touched (const HalfLifeEngine::SDK::Classes::Edict *const entity);

		inline void HostageKilled (HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const killer)
		{
			// Call this function in base class....
			FakeClient::HostageKilled (hostage, killer);

			m_scheduleManager.OnHostageKilled (hostage, killer);
		}
		inline void HostageRescued (HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			// Call this function in base class....
			FakeClient::HostageRescued (hostage);

			m_scheduleManager.OnHostageRescued (hostage);
		}
		inline void HostageUnusePost (HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const currentRescuer)
		{
			// Call this function in base class....
			FakeClient::HostageUnusePost (hostage, currentRescuer);

			m_scheduleManager.OnHostageUnusePost (hostage, currentRescuer);
		}
		inline void HostageUsePost (HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const previousRescuer)
		{
			// Call this function in base class....
			FakeClient::HostageUsePost (hostage, previousRescuer);

			m_scheduleManager.OnHostageUsePost (hostage, previousRescuer);
		}
		inline void AllHostagesRescued (void)
		{
			// Call this function in base class....
			FakeClient::AllHostagesRescued ();

			m_scheduleManager.OnAllHostagesRescued ();
		}

	//
	// Group: Private engine callbacks.
	//
	private:
		// Functions for 'Client::m_functionThink' pointer.
		void        AliveThink       (void);
		void        DeathThink       (void);

		inline void AddWeapon             (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Call this function in base class....
			FakeClient::AddWeapon (weapon);

			m_scheduleManager.OnAddWeapon (weapon);
		}
		inline void RemoveWeapon          (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Call this function in base class....
			FakeClient::RemoveWeapon (weapon);

			m_scheduleManager.OnRemoveWeapon (weapon);
		}
		inline void OnTouchingWeapon      (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Call this function in base class....
			FakeClient::OnTouchingWeapon (weapon);

			m_scheduleManager.OnTouchingWeapon (weapon);
		}
		void        Use                   (const HalfLifeEngine::SDK::Classes::Edict *const entity, const HalfLifeEngine::SDK::Classes::Edict *const caller, const HalfLifeEngine::SDK::Constants::UseType_t type, const float value)
		{
			// Call this function in base class....
			FakeClient::Use (entity, caller, type, value);

			m_scheduleManager.OnUse (entity, caller, type, value);
		}
		void        TouchPost             (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			// Call this function in base class....
			FakeClient::TouchPost (entity);

			m_scheduleManager.OnTouchPost (entity);
		}

		inline void ItemDeployPost        (HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Call this function in base class....
			FakeClient::ItemDeployPost (weapon);

			// update reloading status
			m_reloadState = ReloadState_None;
			m_reloadCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentWeapon->GetNextAttackDelay ();

			m_scheduleManager.OnItemDeployPost (weapon);
		}
		inline void WeaponReloadPost      (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Call this function in base class....
			FakeClient::WeaponReloadPost (weapon);

			// update reloading status
			m_reloadState = ReloadState_None;
			m_reloadCheckTime = m_currentWeapon->GetReloadEndTime () + 1.0f;

			m_scheduleManager.OnWeaponReloadPost (weapon);
		}

		inline void ImpulseCommand (const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t impulseCommand)
		{
			// Call this function in base class....
			FakeClient::ImpulseCommand (impulseCommand);

			m_scheduleManager.OnImpulseCommand (impulseCommand);
		}
};

inline       YaPB *const Client::GetYaPBPointer (void)
{
	// Reliability check.
	InternalAssert (IsYaPB ());

	return static_cast <YaPB *const> (this);
}
inline const YaPB *const Client::GetYaPBPointer (void) const
{
	// Reliability check.
	InternalAssert (IsYaPB ());

	return static_cast <const YaPB *const> (this);
}

inline const float NavigationMesh::PathCost::operator () (NavigationArea *const area, const NavigationArea *const fromArea, const NavigationLadder *const ladder) const
{
	const float cost ((1.0f - m_bot->GetAgressionLevel () * 0.95f) * 100.0f);

	if (fromArea == NULL)
		return m_routeType == SAFEST_ROUTE ? area->GetDanger (m_bot->GetRealTeam ()) * cost : 0.0f;	// first area in path, no cost

	if (fromArea->HasAttributes (NAV_MESH_JUMP) && area->HasAttributes (NAV_MESH_JUMP))
		return -1.0f;

	if (fromArea->HasAttributes (NAV_MESH_NO_HOSTAGES) && !m_bot->GetHostageManager ().IsEmpty ())
		return -1.0f;

	// compute distance travelled along path so far
	const float distance (ladder != NULL ? ladder->GetLength () : fromArea->GetCenter ().GetDistance (area->GetCenter ()));
	float totalCost (fromArea->GetCostSoFar () + distance);
/*
	if (cvar_bot_zombie.value > 0.0f)
		return totalCost;
*/
	if (!area->IsUnderwater () && !area->IsConnected (fromArea))
	{
		const float fallDistance (ladder != NULL && fromArea->GetCenter ().z > ladder->GetBottom ().z && area->GetCenter ().z < ladder->GetBottom ().z ? ladder->GetBottom ().z - area->GetCenter ().z : -fromArea->ComputeHeightChange (area));
		const float fallDamage (YaPB::GetApproximateFallDamage (fallDistance));

		if (fallDamage > 0.0f)
		{
			if (fallDamage + 10.0f >= m_bot->GetEdict ()->GetHealth ())
				return -1.0f;

			if (m_routeType == SAFEST_ROUTE || m_bot->GetAgressionLevel () * 15.0f + 10.0f < fallDamage)
				totalCost += MATH_GET_SQUARED (fallDamage) * 100.0f;
		}
	}

	// if this is a "crouch" or "walk" area, add penalty
	if (area->HasAttributes (NAV_MESH_CROUCH | NAV_MESH_WALK))
	{
		float penalty (m_routeType == SAFEST_ROUTE ? 5.0f : 20.0f);

		if (area->HasAttributes (NAV_MESH_CROUCH) && !m_bot->GetHostageManager ().IsEmpty ())
			penalty *= 3.0f;

		totalCost += penalty * distance;
	}

	// if this is a "jump" area, add penalty
	if (area->HasAttributes (NAV_MESH_JUMP))
		totalCost += distance;

	if (m_routeType == SAFEST_ROUTE)
		totalCost += area->GetDanger (m_bot->GetRealTeam ()) * distance * cost;

	if (m_bot->IsAttacking ())
		return totalCost;

	const float areaHalfSize (Math::GetMidPoint (area->GetExtent ().GetSizeX (), area->GetExtent ().GetSizeY ()));

	if (areaHalfSize < 1.0f)
		return totalCost;

	return area->GetPlayerCount (m_bot->GetRealTeam ()) * 50000.0f / areaHalfSize + totalCost;
}

#endif	// ifndef YAPB_INCLUDED