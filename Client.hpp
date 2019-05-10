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
// Client.hpp
//
// Class: Client
//
// Description: Contains the 'Client' class Function Prototypes.
//
// Client class hierachy:
//	Client:
//		FakeClient:
//			FakeClient_NotYaPB
//				ZBot
//			YaPB
//		Human:
//			Host
//			Player
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CLIENT_INCLUDED
#define CLIENT_INCLUDED

class Client
{
	friend ClientManagerTemplate <Client>;
	friend ClientManagerTemplate <FakeClient>;
	friend ClientManagerTemplate <FakeClient_NotYaPB>;
	friend ClientManagerTemplate <ZBot>;
	friend ClientManagerTemplate <YaPB>;
	friend ClientManagerTemplate <Human>;
	friend ClientManagerTemplate <Host>;
	friend ClientManagerTemplate <Player>;
	friend ClientManager;
	friend FakeClientManager;
	friend OwningItem;
	friend CurrentWeapon;

	DECLARE_CLASS_WITHOUT_BASE (Client);

	//
	// Group: Protected constants.
	//
	protected:
		enum Type_t
		{
			Type_FakeClient,								// Not really type.... (subtype)
				Type_FakeClient_NotYaPB = Type_FakeClient,	// Other bots, not YaPB's.
					Type_ZBot,									// ZBot.
				Type_YaPB,									// This bot.

			Type_Human,										// Not really type.... (subtype)
				Type_Host = Type_Human,						// Listen server hosting client, with full access to YaPB.
				Type_Player									// Just a regular player, without access to YaPB menus, commands, etc....
		};

	//
	// Group: Type definitions.
	//
	public:
		typedef void (Client::*FunctionThink_t) (void);

	//
	// Group: Private members.
	//
	private:
		float                                       m_thinkIntervalTime;											// think timer interval
		bool                                        m_isAlive;														// whether client/bot is alive

		// fundamental callbacks
		FunctionThink_t                             m_functionThink;

//		HalfLifeEngine::SDK::TypeDefinitions::BoneTransformMatrix_t m_boneTransformMatrix;											// individual bone transformation matrix for this client
		Math::Frustum                               m_frustum;

	//
	// Group: Protected members.
	//
	protected:
		template <typename ownerType> class SubSystem
		{
			public:
				typedef ownerType OwnerType_t;

			protected:
				ownerType *const m_owner;	// Pointer to client/bot which owns this sub system.

			protected:
				inline SubSystem (ownerType *const owner) : m_owner (owner) { /* VOID */ }

				virtual inline ~SubSystem (void) { /* VOID */ }

			//
			// Group: Private operators.
			//
			private:
				inline SubSystem &operator = (const SubSystem &/*right*/);	// Avoid "warning C4512: 'SubSystem' : assignment operator could not be generated".

			public:
				//
				// Function: GetOwner
				//
				// Description: Gets a owner, owning this sub system.
				//
				// Returns: Owner, owning this sub system.
				//
				inline       ownerType *const GetOwner (void)       { return m_owner; }
				inline const ownerType *const GetOwner (void) const { return m_owner; }
		};

		HalfLifeEngine::SDK::Classes::Edict *const  m_edict;														// shortcut pointer to client/bot edict pointer, set up on client/bot create nullified on initialise, client/bot functions assume this is not NULL.

		struct Ammo_t
		{
			char          clip;		// ammo in clip
			unsigned char total;	// total ammo amounts
		}                                           m_ammo[HalfLifeEngine::SDK::Constants::MaximumWeapons];			// Stores clip/total ammo amounts for each weapons.

		#include <CurrentWeapon.hpp>

		OwningItemsManager                         *m_owningItemsManager;
		CurrentWeapon                              *m_currentWeapon;												// some info about current client/bot weapon
		Waypoint                                   *m_currentWaypoint;												// some info about current client/bot waypoint

		NavigationMesh::NavigationArea             *m_lastNavigationArea;											// Last known navigation area of client - NULL if unknown.
//		DynamicString                               m_lastPlaceName;												// Last navigation place name.

		#include <ClientHostageManager.hpp>

		HostageManager                              m_hostageManager;

		class MapScenario
		{
			public:
				virtual void Think (void);
		};
		class MapScenario_Assassination : public MapScenario
		{
			private:
				

			public:
				void Think (void);
		};
		class MapScenario_Rescue : public MapScenario
		{
			private:
				HostageManager m_hostageManager;

			public:
				void Think (void);
		};
		class MapScenario_Defusion : public MapScenario
		{
			private:
				

			public:
				void Think (void);
		};
		class MapScenario_Escape : public MapScenario
		{
			private:
				

			public:
				void Think (void);
		};
		class MapScenario_RescueAndDefusion : virtual public MapScenario_Rescue, virtual public MapScenario_Defusion
		{
			private:
				

			public:
				void Think (void);
		};
		class Team
		{
			protected:
				MapScenario *m_mapScenario;

			public:
				inline Team (void) : m_mapScenario (NULL) { /* VOID */ }

			public:
				virtual void RoundStarted (void);
				virtual void Think        (void);
		};
		class Team_Terrorist : public Team
		{
			private:
				

			public:
				inline Team_Terrorist (void) : Team () { /* VOID */ }

			public:
				void RoundStarted (void);
				void Think        (void);
		};
		class Team_CounterTerrorist : public Team
		{
			private:
				

			public:
				inline Team_CounterTerrorist (void) : Team () { /* VOID */ }

			public:
				void RoundStarted (void);
				void Think        (void);
		};

		Team                                       *m_team;	// UNUSED!
		HalfLifeEngine::SDK::Constants::TeamID_t    m_currentTeamID;	// Need only for compare with new team in TeamChanged() function.

		struct NoisedSound_t
		{
			float hearingDistance;		// distance this sound is heared
			float timeLasting;			// time sound is played/heared
			float maximumLastingTime;	// maximum time sound is played/heared (to divide the difference between that above one and the current one)

			unsigned char master_vol;	// 0-255 master volume
			float         dist_mult;	// distance multiplier (attenuation / HalfLifeEngine::SDK::Constants::SoundNominalClipDistance)

			inline NoisedSound_t (void) : hearingDistance (0.0f), timeLasting (0.0f), maximumLastingTime (0.0f) { /* VOID */ }

			inline void Reset (void) { hearingDistance = timeLasting = maximumLastingTime = 0.0f; }

			inline void Initialize (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/, const DynamicString &sample, const float volume, const float attenuation, const HalfLifeEngine::SDK::Constants::SoundFlag_t/* flags*/, const HalfLifeEngine::SDK::Constants::SoundPitch_t/* pitch*/)
			{
				const DynamicString fullFilename ("/sound/" + sample);
				const DynamicString CSfullFilename (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + fullFilename);

				hearingDistance = SOUND_ATTENUATION_TO_RADIUS (attenuation)/* * volume*/;	/// @warning SCALING ON volume GIVES WRONG RESULTS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				maximumLastingTime = STDIOFile::IsExists (CSfullFilename, "rb") ? GetWaveSoundPlayDuration (CSfullFilename) : GetWaveSoundPlayDuration ("valve" + fullFilename);
				timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + maximumLastingTime;

				master_vol = static_cast <unsigned char> (volume * 255.0f);
				dist_mult = attenuation / HalfLifeEngine::SDK::Constants::SoundNominalClipDistance;

//				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::EmitSound(): sample: \"%s\", vol=%.1f, attn=%.2f, radius=%.2f, dur=%.2f.", entity->GetNetName ().GetData (), sample.GetData (), volume, attenuation, hearingDistance, maximumLastingTime);
//				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\"%s\"::EmitSound(): sample: \"%s\", vol=%.1f, attn=%.2f, radius=%.2f, dur=%.2f.\n", entity->GetNetName ().GetData (), sample.GetData (), volume, attenuation, hearingDistance, maximumLastingTime);
			}
		};
		NoisedSound_t                               m_lastNoisedSound, m_currentNoisedSound;						// Last and current noised sounds (for bots, to react on it).

		HalfLifeEngine::SDK::Constants::RadioMenu_t m_radioSelect;													// used radio menu (0 == none, 1 == 'Radio Commands', 2 == 'Group Radio Commands', 3 == 'Radio Responses/Reports')

	//
	// Group: (Con/De)structors.
	//
	public:
		inline          Client (HalfLifeEngine::SDK::Classes::Edict *const client) :
			m_edict (client),	// Set client edict.

			// Assume dead....
			m_isAlive (false),
			m_functionThink (&Client::DeathThink),

			m_thinkIntervalTime (0.0f),

			m_currentWaypoint (NULL),

			m_lastNavigationArea (NULL),

			m_team (NULL),
			m_currentTeamID (HalfLifeEngine::SDK::Constants::TeamID_Unassigned),

			m_radioSelect (HalfLifeEngine::SDK::Constants::RadioMenu_None)
		{
			// Clear all weapon stuff.
			for (unsigned char index (HalfLifeEngine::SDK::Constants::WeaponID_None); index < HalfLifeEngine::SDK::Constants::MaximumWeapons; ++index)
				m_ammo[index].clip = m_ammo[index].total = 0u;

			// Initialize subsystems....
			m_owningItemsManager = new OwningItemsManager (this);
			m_currentWeapon = new CurrentWeapon (this);

			#if defined _DEBUG
				// Reliability check.
				if (m_currentWeapon == NULL || m_owningItemsManager == NULL)
					TerminateOnMalloc ();
			#endif	// if defined _DEBUG
		}
		virtual inline ~Client (void)
		{
			// This is client destructor.

			// Delete subsystems....
			delete m_owningItemsManager;
			delete m_currentWeapon;
		}

	//
	// Group: Private operators.
	//
	private:
		inline Client &operator = (const Client &/*right*/);	// Avoid "warning C4512: 'Client' : assignment operator could not be generated".

	//
	// Group: Operators.
	//
	public:
		// client will now automatically convert to 'HalfLifeEngine::SDK::Classes::Edict *' and 'const HalfLifeEngine::SDK::Classes::Edict *const' when needed
		inline operator       HalfLifeEngine::SDK::Classes::Edict *const (void)       { return GetEdict (); }
		inline operator       HalfLifeEngine::SDK::Classes::Edict *const (void) const { return m_edict; }	// needed too :{O
		inline operator const HalfLifeEngine::SDK::Classes::Edict *const (void) const { return GetEdict (); }

		// client will now automatically convert to 'HalfLifeEngine::SDK::Structures::EntityVariables_t *' and 'const HalfLifeEngine::SDK::Structures::EntityVariables_t *const' when needed
		inline operator       HalfLifeEngine::SDK::Structures::EntityVariables_t *const (void)       { return &GetEdict ()->variables; }
		inline operator       HalfLifeEngine::SDK::Structures::EntityVariables_t *const (void) const { return &m_edict->variables; }	// needed too :{O
		inline operator const HalfLifeEngine::SDK::Structures::EntityVariables_t *const (void) const { return &GetEdict ()->variables; }

		// client will now automatically convert to 'HalfLifeEngine::SDK::Classes::BasePlayer *' and 'const HalfLifeEngine::SDK::Classes::BasePlayer *const' when needed
		inline operator       HalfLifeEngine::SDK::Classes::BasePlayer *const (void)       { return GetEdict ()->privateData->GetBasePlayerPointer (); }
		inline operator       HalfLifeEngine::SDK::Classes::BasePlayer *const (void) const { return GetEdict ()->privateData->GetBasePlayerPointer (); }	// needed too :{O
		inline operator const HalfLifeEngine::SDK::Classes::BasePlayer *const (void) const { return GetEdict ()->privateData->GetBasePlayerPointer (); }

	//
	// Group: Private functions.
	//
	private:
		virtual inline const Type_t GetType (void) const =/*>*/ 0;	// nothing, used by sub classes

		void LastNavigationAreaUpdate (void);
//		void SoundSimulateUpdate      (void);

		inline void TryReceiveLastNoisedSound (void);	// Note: This function declared in YaPBManager.hpp.

		inline void SetAlive (void)
		{
			m_isAlive = true;

			SetFunctionThink (&Client::AliveThink);
		}
		inline void SetDead (void)
		{
			m_isAlive = false;

			SetFunctionThink (&Client::DeathThink);
		}
		inline void CallThinkFunction (void)
		{
			// Reliability check.
			InternalAssert (m_functionThink != NULL);

			(this->*m_functionThink) ();
		}

		virtual inline void CheckWalk (void)
		{
			if (!IsYaPB () && !IsDucking () && IsOnFloor () ? GetEdict ()->GetSpeedSquared2D () <= HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeedSquared : IsOnLadder () && GetEdict ()->GetSpeedSquared () <= HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeedSquared)
				GetEdict ()->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_RUN;
		}

	//
	// Group: Protected functions.
	//
	protected:
		//
		// Function: GetWalkSpeed
		//
		// Description: Gets the client/bot walk speed for his current weapon.
		//
		// Returns: True if entity is valid, false otherwise.
		//
		inline const float GetWalkSpeed (void) const { return static_cast <float> (static_cast <int> (GetEdict ()->variables.maximumSpeed) / 2 + static_cast <int> (GetEdict ()->variables.maximumSpeed) / 50)/* - 18.0f*/; }

		template <typename derivedClassType>     inline void SetFunctionThink (void (derivedClassType::*const newFunctionThink) (void)) { m_functionThink = static_cast <const FunctionThink_t> (newFunctionThink); }
		template </* Client::FunctionThink_t */> inline void SetFunctionThink (const FunctionThink_t newFunctionThink)                  { m_functionThink = newFunctionThink; }

		virtual void LastNavigationAreaChanged (NavigationMesh::NavigationArea *const newNavigationArea);

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsValid
		//
		// Description: Checks whether client/bot and his entity is valid.
		//
		// Note: This function declared in ClientManager.hpp.
		//
		// Returns: True if client/bot and his entity is valid, false otherwise.
		//
		inline const bool           IsValid                   (void) const
		{
			#if defined _DEBUG
				if (this == NULL)
					return false;

				// Reliability checks.
				InternalAssert (GetEdict ()->IsValid ());
				InternalAssert (!GetName ().IsEmpty ());

				return true;
			#else	// if defined _DEBUG
				return this != NULL;
			#endif	// if !defined _DEBUG
		}

		inline const float                                 GetThinkIntervalTime  (void) const { return m_thinkIntervalTime; }
		inline       NavigationMesh::NavigationArea *const GetLastNavigationArea (void) const { return m_lastNavigationArea; }

		//
		// Function: IsAlive
		//
		// Description: Checks whether client/bot is alive.
		//
		// Returns: True if client/bot is alive, false otherwise.
		//
		inline const bool           IsAlive                   (void) const { return m_isAlive; }

		inline const Math::Frustum &GetFrustum                (void) const { return m_frustum; }

		inline const bool           IsPrimaryFireAllowed      (void) const { return GetEdict ()->privateData->GetBasePlayerPointer ()->m_isPrimaryFireAllowed; }

		//
		// Function: IsAttacking
		//
		// Description: Checks whether client/bot is currently attacking.
		//
		// Returns: True if client/bot is currently attacking, false otherwise.
		//
		inline const bool           IsAttacking               (void) const
		{
			return (GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK) && IsPrimaryFireAllowed () && m_currentWeapon->IsValid () && m_currentWeapon->IsCanAttack () && !m_currentWeapon->IsGrenade ();
		}

		//
		// Function: IsShooting
		//
		// Description: Checks whether client/bot is currently shooting.
		//
		// Returns: True if client/bot is currently shooting, false otherwise.
		//
		inline const bool           IsShooting                (void) const { return IsAttacking () && !m_currentWeapon->IsKnife (); }

		//
		// Function: IsUsesFlashlight
		//
		// Description: Checks whether client/bot is uses a flashlight.
		//
		// Returns: True if client/bot is uses a flashlight, false otherwise.
		//
		inline const bool           IsUsesFlashlight          (void) const { return (GetEdict ()->variables.effects & HalfLifeEngine::SDK::Constants::EntityEffect_DimLight) > 0u; }

		//
		// Function: GetOrigin
		//
		// Description: Gets the client's current origin.
		//
		// Returns: Client's current origin.
		//
		inline const Math::Vector3D &GetOrigin                (void) const { return GetEdict ()->variables.origin; }

		inline const Math::Vector3D &GetVelocity              (void) const { return GetEdict ()->variables.velocity; }
		inline const Math::Angles3D &GetModelAngles           (void) const { return GetEdict ()->variables.modelAngles; }
		inline const Math::Angles3D &GetViewAngles            (void) const { return GetEdict ()->variables.viewAngles; }
		inline const Math::Angles3D &GetPunchAngles           (void) const { return GetEdict ()->variables.punchAngles; }
		inline const unsigned short  GetFieldOfView           (void) const { return static_cast <unsigned short> (GetEdict ()->variables.fov); }
		inline const unsigned short  GetSafeFieldOfView       (void) const { return GetFieldOfView () > 0u ? GetFieldOfView () : HalfLifeEngine::SDK::Constants::DefaultPlayerFieldOfView; }

		inline const DynamicString   GetName                  (void) const { return GetEdict ()->GetNetName (); }

		//
		// Function: GetMoney
		//
		// Description: Gets the amount of money in client's bank.
		//
		// Returns: Amount of money in client's bank.
		//
		inline const unsigned short GetMoney                  (void) const { return static_cast <const unsigned short> (GetEdict ()->GetPrivateDataReference <unsigned int> (HalfLifeEngine::SDK::Constants::OFFSET_MONEY)); }
		inline void                 SetMoney                  (const unsigned short newMoneyAmount) { return GetEdict ()->SetPrivateData <unsigned int> (HalfLifeEngine::SDK::Constants::OFFSET_MONEY, newMoneyAmount); }

		//
		// Function: GetSpawnsNumber
		//
		// Description: Gets the number of times client has spawned this round.
		//
		// Returns: Number of times client has spawned this round.
		//
		inline const unsigned short GetSpawnsNumber           (void) const { return static_cast <const unsigned short> (GetEdict ()->GetPrivateDataReference <unsigned int> (HalfLifeEngine::SDK::Constants::OFFSET_SPAWNS_NUMBER)); }

		//
		// Function: GetFlashlightBattery
		//
		// Description: Gets the client's flashlight battery amount.
		//
		// Returns: Amount of client's flashlight battery.
		//
		inline const unsigned char  GetFlashlightBattery      (void) const { return static_cast <const unsigned char> (GetEdict ()->GetPrivateDataReference <unsigned int> (HalfLifeEngine::SDK::Constants::OFFSET_FLASH_LIGHT_BATTERY)); }

		//
		// Function: IsHasNightVisionGoggles
		//
		// Description: Checks whether the client have night vision goggles.
		//
		// Returns: True if client have night vision goggles, false otherwise.
		//
		inline const bool           IsHasNightVisionGoggles   (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_HAS_NVGOGGLES); }
		//
		// Function: IsUsingNightVisionGoggles
		//
		// Description: Checks whether the client using night vision goggles.
		//
		// Returns: True if client using night vision goggles, false otherwise.
		//
		inline const bool           IsUsingNightVisionGoggles (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_USES_NVGOGGLES); }

		//
		// Function: IsDefusing
		//
		// Description: Checks whether the client is defusing C4.
		//
		// Returns: True if client is defusing C4, false otherwise.
		//
		inline const bool           IsDefusing                (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_IS_DEFUSING); }
		//
		// Function: IsHasDefuseKit
		//
		// Description: Checks whether the client have defuse kit.
		//
		// Returns: True if client have defuse kit, false otherwise.
		//
		inline const bool           IsHasDefuseKit            (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_HAS_DEFUSE_KIT); }
		//
		// Function: IsCanPlantBomb
		//
		// Description: Checks whether the client can plant bomb.
		//
		// Returns: True if client can plant bomb, false otherwise.
		//
		inline const bool           IsCanPlantBomb            (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_CAN_PLANT_BOMB); }

		//
		// Function: IsTeamChanged
		//
		// Description: Checks whether the client is changed team after respawn.
		//
		// Returns: True if client is changed team after respawn, false otherwise.
		//
		inline const bool           IsTeamChanged             (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_IS_TEAM_CHANGED); }

		//
		// Function: IsCanHearRadio
		//
		// Description: Checks whether the client can hear radio commands.
		//
		// Returns: True if client can hear radio commands, false otherwise.
		//
		inline const bool           IsCanHearRadio            (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_CANT_HEAR_RADIO) == false; }

		//
		// Function: IsNotKilled
		//
		// Description: Checks whether the client has been just respawned (not killed).
		//
		// Returns: True if client has been just respawned (not killed), false otherwise.
		//
		inline const bool           IsNotKilled               (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_IS_NOT_KILLED); }
/*! DON'T USE US!!!
		//
		// Function: GetNextChatTime
		//
		// Description: Gets the client's chat delay time.
		//
		// Returns: Amount of client's delay between two chat messages.
		//
		inline const float          GetNextChatTime           (void) const { return GetEdict ()->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_Player_NextChatUseTime); }
		//
		// Function: GetNextTeamChatTime
		//
		// Description: Gets the client's team chat delay time.
		//
		// Returns: Amount of client's delay between two team chat messages.
		//
		inline const float          GetNextTeamChatTime       (void) const { return GetEdict ()->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_Player_NextTeamChatUseTime); }
*/
		//
		// Function: GetNextRadioTime
		//
		// Description: Gets the client's radio delay time.
		//
		// Returns: Amount of client's delay between two radio commands.
		//
		inline const float          GetNextRadioTime          (void) const { return GetEdict ()->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_NEXT_RADIO_TIME); }

		//
		// Function: GetRadioCommandsUseAmount
		//
		// Description: Gets the client's radio commands amount.
		//
		// Returns: Amount of client's radio commands.
		//
		inline const unsigned char  GetRadioCommandsUseAmount (void) const { return static_cast <const unsigned char> (GetEdict ()->GetPrivateDataReference <unsigned int> (HalfLifeEngine::SDK::Constants::OFFSET_RADIO_COMMANDS_USE_AMOUNT)); }

		//
		// Function: GetFallDamage
		//
		// Description: Gets the client's fall damage.
		//
		// Returns: Amount of client's fall damage.
		//
		inline const float          GetFallDamage             (void) const
		{
			if (GetEdict ()->variables.fallVelocity <= HalfLifeEngine::SDK::Constants::PlayerMaximumSafeFallSpeed)
				return 0.0f;	// After this point, we start doing damage.

			// Subtract off the speed at which a player is allowed to fall without being hurt, so damage will be based on speed beyond that, not the entire fall.
			return (GetEdict ()->variables.fallVelocity - HalfLifeEngine::SDK::Constants::PlayerMaximumSafeFallSpeed) * HalfLifeEngine::SDK::Constants::DamageForFallSpeed;
		}

		inline const float GetFOVDistanceAdjustFactor (void) const
		{
			const float defaultFOVInverted (1.0f / HalfLifeEngine::SDK::Constants::DefaultPlayerFieldOfView);
			const float localFOV (GetFieldOfView ());

			// If FOV is lower, then we're "zoomed" in and this will give a factor < 1 so apparent LOD distances can be shorted accordingly
			return localFOV * defaultFOVInverted;
		}
		static inline const float GetMinimumShootingConeDeviation (const float distanceSquared, const float radius)
		{
			const float squaredDistanceWithRadius (distanceSquared + MATH_GET_SQUARED (radius));
			const float coneMinimum (distanceSquared / squaredDistanceWithRadius);

			return coneMinimum;
		}
		inline const float GetShootingConeDeviation (const Math::Vector3D &position) const
		{
			// Find the length to the point, get the player's forward direction
			const Math::Vector3D directionToPositionNormalized ((position - GetEyePosition ()).Normalize ());

			// he's facing it, he meant it
			return directionToPositionNormalized | GetEyeForwardDirection ();
		}
		inline const float GetShootingConeDeviationWithPunchAngles (const Math::Vector3D &position) const
		{
			// Find the length to the point, get the player's forward direction
			const Math::Vector3D directionToPositionNormalized ((position - GetEyePosition ()).Normalize ());

			// he's facing it, he meant it
			return directionToPositionNormalized | GetEyeForwardDirectionWithPunchAngles ();
		}

		inline const bool IsInFieldOfView (const Math::Vector3D &location) const
		{
			// this function returns TRUE if the spatial vector location 'location' is located inside
			// the field of view cone of the client entity, FALSE otherwise. It is assumed that entities
			// have a human-like field of view, that is, about 90 degrees.

			// compute deviation angles (angles between player's forward direction and location)
			const Math::Angles2D deviation (((location - GetEyePosition ()).ToAngles2D () - GetViewAngles ()).Clamp ());
			const float fieldOfView (GetFieldOfView ());

			// is location outside player's FOV width (90 degree)?
			if (Math::fabsf (deviation.yaw) > fieldOfView * 0.5f)
				return false;	// then location is not visible

			// is location outside player's FOV height (60 degree: consider the 4:3 screen ratio)?
			if (Math::fabsf (deviation.pitch) > Math::Frustum::CalcFovY (fieldOfView, 4.0f / 3.0f)/*! @todo MAKE PRECALCULATED VALUE!!!!!!!!!!!!!!!!! */ * 0.5f)
				return false;	// then location is not visible

			return true;	// else location has to be in player's field of view cone
		}
		inline const float GetFieldOfViewCone (const float fov) const { return Math::cosf (Math::DegreeToRadian (fov * 0.5f)); }
		inline const float GetFieldOfViewCone (void)            const { return GetFieldOfViewCone (GetSafeFieldOfView ()); }
		inline const bool IsInViewCone (const Math::Vector3D &origin, const float fov) const
		{
			// This function returns true if the spatial vector location origin is located inside the field of view cone of the bot entity, false otherwise.

			// origin has to be in entity's field of view cone?
			return GetShootingConeDeviation (origin) >= GetFieldOfViewCone (fov);
		}
		inline const bool IsInViewCone (const Math::Vector3D &origin) const
		{
			// this function returns true if the spatial vector location origin is located inside the field of view cone of the bot entity, false otherwise.

			// origin has to be in entity's field of view cone?
			return IsInViewCone (origin, GetSafeFieldOfView ());
		}
		inline const bool IsInViewConeWithRadius (const Math::Vector3D &origin, const float radius, const float fov) const
		{
			// This function returns true if the spatial vector location origin is located inside the field of view cone of the bot entity, false otherwise.
			/// @warning SLOW FUNCTION!

			// TODO: For safety sake, we might want to do a more fully qualified test against the frustum using the bbox

			// This is the additional number of degrees to add to account for our distance
			const float arcAddition (Math::atan2f (radius, GetEyePosition ().GetDistance (origin)));

			// Find if the sphere is within our FOV
			// origin has to be in entity's field of view cone?
			return GetShootingConeDeviation (origin) >= GetFieldOfViewCone (fov) - arcAddition;
		}
		inline const bool IsInViewConeWithRadius (const Math::Vector3D &origin, const float radius) const
		{
			return IsInViewConeWithRadius (origin, radius, GetSafeFieldOfView ());
		}

		inline const OwningItemsManager *const GetOwningItemsManager (void) const { return m_owningItemsManager; }
		inline const CurrentWeapon      *const GetCurrentWeapon      (void) const { return m_currentWeapon; }

		inline const unsigned short/*float*/ GetAbsoluteFieldOfView (const Math::Vector3D &destination) const
		{
			// This function returns the absolute value of angle to destination entity.

			float viewAngle (GetViewAngles ().yaw);	// get client's current view angle....
			float entityAngle ((destination - GetEyePosition ()).ToYaw ());	// find yaw angle from source to destination....

			// make yaw angle 0.0 to 360.0 degrees if negative....
			if (entityAngle < 0.0f)
				entityAngle += 360.0f;

			// make view angle 0.0 to 360.0 degrees if negative....
			if (viewAngle < 0.0f)
				viewAngle += 360.0f;

			// Return the absolute value of angle to destination entity. Zero degrees means straight ahead, 45 degrees to the left or 45 degrees to the right is the limit of the normal view angle
			const unsigned short absoluteAngle (static_cast <unsigned short> (abs (static_cast <int> (viewAngle) - static_cast <int> (entityAngle))));	// rsm - START angle bug fix.
//			const float absoluteAngle (Math::fabsf (viewAngle - entityAngle));	// rsm - START angle bug fix.

			// Reliability check.
			InternalAssert (absoluteAngle <= 360u);

			return absoluteAngle > 180u ? 360u - absoluteAngle : absoluteAngle;	// rsm - END
//			return absoluteAngle > 180.0f ? 360.0f - absoluteAngle : absoluteAngle;	// rsm - END
		}
		inline const float GetFieldOfView (const Math::Vector3D &destination) const
		{
			// This function returns the value of angle to destination entity.

			return Math::GetDifferenceBetweenAngles ((destination - GetEyePosition ()).ToYaw (), GetViewAngles ().yaw);
		}
		inline const float GetFieldOfView (const float destinationAngle) const
		{
			// This function returns the value of angle to destination angle.

			return Math::GetDifferenceBetweenAngles (destinationAngle, GetViewAngles ().yaw);
		}
/*		inline const Math::Angles2D GetAbsoluteFieldOfView2D (Math::Vector3D destination) const
		{
			// This function returns the absolute value of angle to destination entity.

			destination -= GetEyePosition ();

			/// @error PITCH IS WRONG!!!

			float viewAngle (GetViewAngles ().pitch);	// get client's current view angle....
			float entityAngle (destination.ToPitch ());	// find pitch angle from source to destination....
			Math::Angles2D absoluteFieldOfView;

			entityAngle = 180.0f - entityAngle;

			// make pitch angle 0.0 to 180.0 degrees if negative....
			if (entityAngle < 0.0f)
				entityAngle += 180.0f;

			// make view angle 0.0 to 180.0 degrees if negative....
			if (viewAngle < 0.0f)
				viewAngle += 180.0f;

			// Return the absolute value of angle to destination entity. Zero degrees means straight ahead, 45 degrees upwards or 45 degrees downwards is the limit of the normal view angle
			absoluteFieldOfView.pitch = static_cast <unsigned short> (abs (static_cast <int> (viewAngle) - static_cast <int> (entityAngle)));	// rsm - START angle bug fix.
//			absoluteFieldOfView.pitch = Math::fabsf (viewAngle - entityAngle);	// rsm - START angle bug fix.

			// Reliability check.
			InternalAssert (absoluteFieldOfView.pitch <= 180.0f);

			if (absoluteFieldOfView.pitch > 90.0f)
				absoluteFieldOfView.pitch = 180.0f - absoluteFieldOfView.pitch;

			// rsm - END

			viewAngle = GetViewAngles ().yaw;	// get client's current view angle....
			entityAngle = destination.ToYaw ();	// find yaw angle from source to destination....

			// make yaw angle 0.0 to 360.0 degrees if negative....
			if (entityAngle < 0.0f)
				entityAngle += 360.0f;

			// make view angle 0.0 to 360.0 degrees if negative....
			if (viewAngle < 0.0f)
				viewAngle += 360.0f;

			// Return the absolute value of angle to destination entity. Zero degrees means straight ahead, 45 degrees to the left or 45 degrees to the right is the limit of the normal view angle
			absoluteFieldOfView.yaw = static_cast <unsigned short> (abs (static_cast <int> (viewAngle) - static_cast <int> (entityAngle)));	// rsm - START angle bug fix.
//			absoluteFieldOfView.yaw = Math::fabsf (viewAngle - entityAngle);	// rsm - START angle bug fix.

			// Reliability check.
			InternalAssert (absoluteFieldOfView.yaw <= 360.0f);

			if (absoluteFieldOfView.yaw > 180.0f)
				absoluteFieldOfView.yaw = 360.0f - absoluteFieldOfView.yaw;

			// rsm - END

			return absoluteFieldOfView;
		}
*/
		inline const bool IsLookingAtPosition (const Math::Vector3D &position, const unsigned char maximumAngleDifference = 20u) const
		{
			const Math::Vector3D direction (position - GetEyePosition ());
			Math::Angles2D angles (direction.ToAngles2D ());

			if (abs (static_cast <short> (Math::AngleNormalize (angles.yaw - GetViewAngles ().yaw))) >= maximumAngleDifference)
				return false;

			angles.pitch = 360.0f - angles.pitch;

			return abs (static_cast <short> (Math::AngleNormalize (angles.pitch - GetViewAngles ().pitch))) < maximumAngleDifference;
		}

		inline const unsigned char                              GetAbsoluteFieldOfView (void) const
		{
			// This function returns the zoom type of a client/bot.

			return static_cast <const unsigned char> (GetEdict ()->GetPrivateDataReference <unsigned int> (HalfLifeEngine::SDK::Constants::Offset_Player_FieldOfView));
		}
		inline const HalfLifeEngine::SDK::Constants::ZoomType_t GetZoomType    (void) const
		{
			// This function returns the zoom type of a client/bot.

			return static_cast <const HalfLifeEngine::SDK::Constants::ZoomType_t> (GetAbsoluteFieldOfView ());
		}

		inline const bool IsUsingScope (void) const { return GetFieldOfView () < HalfLifeEngine::SDK::Constants::DefaultPlayerFieldOfView; }

		//
		// Function: IsOnTrain
		//
		// Description: Checks whether client is on train.
		//
		// Returns: True if client is currently standing on the train, false otherwise.
		//
		inline const bool IsOnTrain (void) const { return GetEdict ()->IsPlayerOnTrain (); }

		//
		// Function: GetEdict
		//
		// Description: Gets the client entity pointer.
		//
		// Returns: Client entity pointer.
		//
		inline       HalfLifeEngine::SDK::Classes::Edict *const GetEdict (void)       { return m_edict; }
		inline const HalfLifeEngine::SDK::Classes::Edict *const GetEdict (void) const { return m_edict; }

		//
		// Function: GetCurrentWaypoint
		//
		// Description: Gets the current client/bot waypoint pointer.
		//
		// Returns: Current client/bot waypoint pointer.
		//
		inline       Waypoint *const GetCurrentWaypoint (void)       { return m_currentWaypoint; }
		inline const Waypoint *const GetCurrentWaypoint (void) const { return m_currentWaypoint; }

		inline const bool HasWeapon          (const HalfLifeEngine::SDK::Constants::WeaponID_t index)       const { return (GetEdict ()->variables.weapons & BIT (index)) > 0u; }
		inline const bool HasWeapons         (const HalfLifeEngine::SDK::Constants::WeaponBit_t weaponBits) const { return (GetEdict ()->variables.weapons & weaponBits) > 0u; }

		inline const bool HasPrimaryWeapon   (void) const
		{
			// This function returns true if bot has a primary weapon, false otherwise.

			return HasWeapons (HalfLifeEngine::SDK::Constants::WeaponBits_Primary);
		}

		inline const bool HasSecondaryWeapon (void) const
		{
			// This function returns true if bot has a secondary weapon, false otherwise.

			return HasWeapons (HalfLifeEngine::SDK::Constants::WeaponBits_Secondary);
		}

		inline const bool HasShield          (void) const
		{
			// This function returns true if client/bot has a tactical shield, false otherwise.

			return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_HAS_SHIELD);
		}

		inline const bool IsShieldDrawn (void) const
		{
			// This function returns true if the tactical shield is drawn, false otherwise.

			return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_USES_SHIELD);
		}

		//
		// Function: GetSpecialMapZones
		//
		// Description: Gets the client special map zones bitmask.
		//
		// Returns: Special map zones bitmask.
		//
		inline const HalfLifeEngine::SDK::Constants::MapZone_t GetSpecialMapZones (void) const { return GetEdict ()->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::MapZone_t> (HalfLifeEngine::SDK::Constants::Offset_Player_ClientMapZone); }

		inline const bool IsInBuyZone            (void) const { return (GetSpecialMapZones () & HalfLifeEngine::SDK::Constants::IN_BUY_ZONE) > 0u; }
		inline const bool IsInBombPlace          (void) const { return (GetSpecialMapZones () & HalfLifeEngine::SDK::Constants::IN_BOMB_PLACE) > 0u; }
		inline const bool IsInHostagesRescueZone (void) const { return (GetSpecialMapZones () & HalfLifeEngine::SDK::Constants::IN_HOSTAGES_RESCUE_ZONE) > 0u; }
		inline const bool IsInEscapeZone         (void) const { return (GetSpecialMapZones () & HalfLifeEngine::SDK::Constants::IN_ESCAPE_ZONE) > 0u; }
		inline const bool IsInVIPRescueZone      (void) const { return (GetSpecialMapZones () & HalfLifeEngine::SDK::Constants::IN_VIP_RESCUE_ZONE) > 0u; }

		inline const HalfLifeEngine::SDK::Constants::SpectatorObserverMode_t GetObserverMode             (void) const { return GetEdict ()->GetPlayerObserverMode (); }
		inline const unsigned char                                           GetObserverTargetEdictIndex (void) const { return GetEdict ()->GetPlayerObserverTargetEdictIndex (); }
		inline const Client *const                                           GetObserverTarget           (void) const;	// Note: This function declared in ClientManager.hpp.

		inline const HalfLifeEngine::SDK::Constants::PlayerArmorType_t GetArmorType (void) const { return GetEdict ()->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::PlayerArmorType_t> (HalfLifeEngine::SDK::Constants::OFFSET_PLAYER_ARMOR_TYPE); }

		// Note: The below 2 functions are declared in FakeClient.hpp.
		inline       FakeClient         *const GetFakeClientPointer        (void);
		inline const FakeClient         *const GetFakeClientPointer        (void) const;

		// Note: The below 2 functions are declared in OtherBot.hpp.
		inline       FakeClient_NotYaPB *const GetFakeClientNotYaPBPointer (void);
		inline const FakeClient_NotYaPB *const GetFakeClientNotYaPBPointer (void) const;

		// Note: The below 2 functions are declared in ZBot.hpp.
		inline       ZBot               *const GetZBotPointer              (void);
		inline const ZBot               *const GetZBotPointer              (void) const;

		// Note: The below 2 functions are declared in YaPB.hpp.
		inline       YaPB               *const GetYaPBPointer              (void);
		inline const YaPB               *const GetYaPBPointer              (void) const;

		// Note: The below 2 functions are declared in Human.hpp.
		inline       Human              *const GetHumanPointer             (void);
		inline const Human              *const GetHumanPointer             (void) const;

		// Note: The below 2 functions are declared in Host.hpp.
		inline       Host               *const GetHostPointer              (void);
		inline const Host               *const GetHostPointer              (void) const;

		// Note: The below 2 functions are declared in Player.hpp.
		inline       Player             *const GetPlayerPointer            (void);
		inline const Player             *const GetPlayerPointer            (void) const;

		inline       HalfLifeEngine::SDK::Structures::Client_t &GetEngineClient (void)       { return g_server->GetStatic ().clients[GetEdict ()->GetIndex () - 1u]; }
		inline const HalfLifeEngine::SDK::Structures::Client_t &GetEngineClient (void) const { return g_server->GetStatic ().clients[GetEdict ()->GetIndex () - 1u]; }

		inline const HalfLifeEngine::SDK::Classes::Edict *const GetViewEdict (void) const { return GetEngineClient ().pViewEntity; }

		//
		// Function: IsFakeClient
		//
		// Description: Checks whether client is bot (fake client).
		//
		// Returns: True if client is bot (fake client), false otherwise.
		//
//		inline const bool                    IsFakeClient       (void) const { return GetType () < Type_Human; }
		virtual inline const bool            IsFakeClient       (void) const =/*>*/ 0;	// nothing, used by sub classes

		//
		// Function: IsYaPB
		//
		// Description: Checks whether client is YaPB.
		//
		// Returns: True if client is YaPB, false otherwise.
		//
//		inline const bool                    IsYaPB             (void) const { return GetType () == Type_YaPB; }
		virtual inline const bool            IsYaPB             (void) const =/*>*/ 0;	// nothing, used by sub classes

		//
		// Function: IsHuman
		//
		// Description: Checks whether client is human, not a bot.
		//
		// Returns: True if client is human, false otherwise.
		//
//		inline const bool                    IsHuman            (void) const { return GetType () >= Type_Human; }
		virtual inline const bool            IsHuman            (void) const =/*>*/ 0;	// nothing, used by sub classes

		//
		// Function: IsHuman
		//
		// Description: Checks whether client is host.
		//
		// Returns: True if client is host, false otherwise.
		//
//		inline const bool                    IsHost             (void) const { return GetType () == Type_Host; }
		virtual inline const bool            IsHost             (void) const =/*>*/ 0;	// nothing, used by sub classes

		//
		// Function: IsPlayer
		//
		// Description: Checks whether client is player.
		//
		// Returns: True if client is player, false otherwise.
		//
//		inline const bool                    IsPlayer           (void) const { return GetType () == Type_Player; }
		virtual inline const bool            IsPlayer           (void) const =/*>*/ 0;	// nothing, used by sub classes

		//
		// Function: IsOtherBot
		//
		// Description: Checks whether client is other bot, not a YaPB.
		//
		// Returns: True if client is other bot, not a YaPB, false otherwise.
		//
//		inline const bool                    IsOtherBot         (void) const { return GetType () == Type_FakeClient_NotYaPB; }
		virtual inline const bool            IsOtherBot         (void) const =/*>*/ 0;	// nothing, used by sub classes

		//
		// Function: IsZBot
		//
		// Description: Checks whether client is Z bot. (The Official Counter-Strike Bot)
		//
		// Returns: True if client is Z bot, false otherwise.
		//
//		inline const bool                    IsZBot             (void) const { return GetType () == Type_ZBot; }
		inline const bool                    IsZBot             (void) const { return GetEdict ()->IsPlayerZBot (); }

		virtual inline const bool            IsSpeaking         (void) const = 0;

		virtual inline void                  ChangeTeam         (void) const
		{
			// Reliability check.
			InternalAssert (!IsSpectator ());

			ExecuteCommandFormat ("chooseteam; menuselect %u; menuselect %u", GetRealTeam () == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModID () == HalfLifeEngine::Engine::Version_Steam ? HalfLifeEngine::SDK::Constants::PlayerModelID_AutoSelect : HalfLifeEngine::SDK::Constants::PlayerModelID_ConditionZero_AutoSelect);
		}

		//
		// Function: GetEyePosition
		//
		// Description: Gets eye position of client.
		//
		// Returns: Eye position of client.
		//
		inline const Math::Vector3D  GetEyePosition    (void) const              { return GetEdict ()->GetEyePosition (); }	// position of eyes/ears/gun

		inline void StudioSetupBones (const char boneID = -1/* means setup all the bones. */) const
		{
//			const_cast <HalfLifeEngine::SDK::Classes::Edict *> (GetEdict ())->variables.modelAngles.pitch = -GetEdict ()->variables.modelAngles.pitch;

			HalfLifeEngine::Globals::g_halfLifeEngine->StudioSetupPlayerBones (GetEdict (), boneID);

//			const_cast <HalfLifeEngine::SDK::Classes::Edict *> (GetEdict ())->variables.modelAngles.pitch = -GetEdict ()->variables.modelAngles.pitch;
		}
		inline const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const GetHitBoxFromBodyPart (const HalfLifeEngine::SDK::Constants::PlayerBodyPart_t bodyPart) const
		{
			const HalfLifeEngine::SDK::Structures::StudioHeader_t *const studioHeader (GetEdict ()->GetModelPointer ());
			const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const pbboxes (reinterpret_cast <const HalfLifeEngine::SDK::Structures::mstudiobbox_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->hitboxindex));
			const HalfLifeEngine::SDK::Structures::mstudiobbox_t *bbox (NULL);

			for (unsigned char hitBoxIndex (0u); hitBoxIndex < studioHeader->numhitboxes; ++hitBoxIndex)
			{
				bbox = pbboxes + hitBoxIndex;

				if (bbox->group == bodyPart)
					break;
			}

			// Reliability check.
			InternalAssert (bbox != NULL);

			return bbox;
		}
		inline const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const GetHitBoxFromBodyParts (const HalfLifeEngine::SDK::Constants::PlayerBodyPartBit_t bodyParts) const
		{
			const HalfLifeEngine::SDK::Structures::StudioHeader_t *const studioHeader (GetEdict ()->GetModelPointer ());
			const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const pbboxes (reinterpret_cast <const HalfLifeEngine::SDK::Structures::mstudiobbox_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->hitboxindex));
			const HalfLifeEngine::SDK::Structures::mstudiobbox_t *bbox (NULL);

			for (unsigned char hitBoxIndex (0u); hitBoxIndex < studioHeader->numhitboxes; ++hitBoxIndex)
			{
				bbox = pbboxes + hitBoxIndex;

				if (BIT (bbox->group) & bodyParts)
					break;
			}

			// Reliability check.
			InternalAssert (bbox != NULL);

			return bbox;
		}
		inline const Math::Vector3D  GetHitBoxPositionFromBodyPart   (const HalfLifeEngine::SDK::Constants::PlayerBodyPart_t bodyPart) const
		{
			return GetHitBoxPosition (GetHitBoxFromBodyPart (bodyPart));
		}
		inline const Math::Vector3D  GetHitBoxPosition   (const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const bbox) const
		{
			// Setup individual bone.
			StudioSetupBones (static_cast <char> (bbox->bone));

			const HalfLifeEngine::SDK::TypeDefinitions::BoneTransformMatrix_t &boneTransformMatrix (HalfLifeEngine::Globals::g_halfLifeEngine->GetBoneTransformMatrix ());
			const Math::Vector3D hitBoxCenter (bbox->boundingBox.GetCenter ().GetTransformed (boneTransformMatrix[bbox->bone]));

			return hitBoxCenter;
		}

		inline const Math::Vector3D GetEyeForwardDirection                (void)                 const { return GetViewAngles ().BuildForwardVector (); }
		inline const Math::Vector3D GetEyeForwardDirectionWithPunchAngles (void)                 const { return (GetViewAngles () + GetEdict ()->GetPunchAngles ()).BuildForwardVector (); }
		inline const Math::Vector3D GetEyeForwardPosition                 (const float distance) const { return GetEyePosition () + GetEyeForwardDirection () * distance; }
		inline const Math::Vector3D GetEyeForwardPositionWithPunchAngles  (const float distance) const { return GetEyePosition () + GetEyeForwardDirectionWithPunchAngles () * distance; }

		virtual inline const Math::Angles2D GetModifiedPunchAngles (void) const = 0;

		inline const bool                    IsVIP              (void) const
		{
			// This function returns true if client/bot is a VIP, false otherwise.

			return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_IS_VIP);
		}

		inline       HostageManager         &GetHostageManager  (void)       { return m_hostageManager; }
		inline const HostageManager         &GetHostageManager  (void) const { return m_hostageManager; }

		inline void UpdateLastNoisedSound (void)
		{
			// Some sound already associated?
			if (m_lastNoisedSound.timeLasting > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				// new sound louder (bigger range) than old one?
				if (m_lastNoisedSound.maximumLastingTime <= 0.0f)
					m_lastNoisedSound.maximumLastingTime = 0.5f;

				// the volume is lowered down when the time of lasting sound is expiring...
				if (m_lastNoisedSound.hearingDistance * (m_lastNoisedSound.timeLasting - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()) / m_lastNoisedSound.maximumLastingTime <= m_currentNoisedSound.hearingDistance)
					m_lastNoisedSound = m_currentNoisedSound;	// override it with new
			}
			else
				m_lastNoisedSound = m_currentNoisedSound;	// just remember it
		}
		inline       void                    SetCurrentNoisedSound (const NoisedSound_t &sound) { m_currentNoisedSound = sound; }
		inline       NoisedSound_t          &GetLastNoisedSound    (void)                       { return m_lastNoisedSound; }
		inline void SetCurrentNoisedSound (const DynamicString &sample, const float volume, const float attenuation, const HalfLifeEngine::SDK::Constants::SoundFlag_t flags, const HalfLifeEngine::SDK::Constants::SoundPitch_t pitch)
		{
			m_currentNoisedSound.Initialize (GetEdict (), sample, volume, attenuation, flags, pitch);

			UpdateLastNoisedSound ();
		}

		inline void                                           SetTeam     (const HalfLifeEngine::SDK::Constants::TeamID_t newTeam) { GetEdict ()->SetPlayerTeam (newTeam); }
		inline const unsigned char                            GetTeam     (void) const { return g_server->IsTeamplay () ? static_cast <const unsigned char> (GetRealTeam ()) : static_cast <const unsigned char> (GetEdict ()->GetIndex ()); }
		inline const HalfLifeEngine::SDK::Constants::TeamID_t GetRealTeam (void) const { return GetEdict ()->GetPlayerTeam (); }
		inline const TeamArrayID_t                            GetRealArrayTeamID (void) const { return static_cast <TeamArrayID_t> (GetRealTeam () - 1u); }
		inline const HalfLifeEngine::SDK::Constants::TeamID_t GetEnemyTeam (void) const
		{
			// Reliability checks.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (GetRealTeam ()));
			InternalAssert (g_server->IsTeamplay ());

			return HalfLifeEngine::SDK::Constants::enemyTeamID[GetRealArrayTeamID ()];
		}

		inline const bool IsSpectator/*IsObserver*/ (void) const
		{
			return HalfLifeEngine::Utilities::IsSpectatorTeam (GetRealTeam ());
		}

		inline const bool                                         HasFollowingHostage        (void) const
		{
			// This function returns true, if client/bot has a hostage, false otherwise.

			return !m_hostageManager.IsEmpty ();
		}
		inline const ::HostageManager::HostagesArray::IndexType_t GetFollowingHostagesNumber (void) const
		{
			// This function returns number of the hostages following this client/bot.

			return m_hostageManager.GetHostagesNumber ();
		}

		inline const bool IsHasMostFrags (void) const;	// Note: This function declared in ClientManager.hpp.

		inline const bool IsImportant (void) const
		{
			// This function returns true if the specified client is "important", false otherwise.

			// Always treats bomb carrier or VIP as important, else this client is not important.
			return HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4) || IsVIP () || HasFollowingHostage () || IsHasMostFrags ();
		}

		//
		// Function: IsOnLadder
		//
		// Description: Checks whether client/bot is on ladder and/or flying.
		//
		// Returns: True if client/bot is currently standing on the ladder, false otherwise.
		//
		inline const bool IsOnLadder (void) const { return GetEdict ()->IsPlayerOnLadder (); }

		//
		// Function: IsOnFloor
		//
		// Description: Checks whether client/bot is standing on the floor.
		//
		// Returns: True if client/bot is on floor, false otherwise.
		//
		inline const bool IsOnFloor (void) const { return GetEdict ()->IsPlayerOnFloor (); }

		//
		// Function: IsDucking
		//
		// Description: Checks whether client/bot is ducking.
		//
		// Returns: True if client/bot is ducking, false otherwise.
		//
		inline const bool IsDucking (void) const { return GetEdict ()->IsDucking (); }

		//
		// Function: GetIllumination
		//
		// Description: Gets the client illumination. (plus virtual muzzle flash)
		//
		// Par:
		//	Thanks to William van der Sterren for the human-like illumination filter computation.
		//	We only consider noticeable the illuminations between 0 and 30 percent of the maximal value,
		//	else it's too bright to be taken in account and we return the full illumination.
		//	The HL engine allows entities to have illuminations up to 255 (hence the 75 as ~30% of 255).
		//	We have to call OUR pfnGetEntityIllum() and not the engine's because of the fake client illumination bug.
		//
		// Returns: Value between 0 and 255 corresponding to the client's illumination.
		//
		inline const unsigned char           GetIllumination           (void) const
		{
			// Ask the engine for the client entity illumination.
			return GetEdict ()->GetPlayerIllumination ();
		}
		//
		// Function: GetIlluminationPercentage
		//
		// Description: Gets the client illumination percentage. (plus virtual muzzle flash)
		//
		// Returns: Value between 0.0 and 100.0 corresponding to the client's illumination percentage.
		//
		inline const float                   GetIlluminationPercentage (void) const
		{
			// Ask the engine for the client entity illumination and filter it so as to return a usable value.
			return static_cast <const float> (GetIllumination ()) / HalfLifeEngine::SDK::Constants::MaximumIlluminationValue * 100.0f;
		}

		inline void                ExecuteRadioCommand       (const HalfLifeEngine::SDK::Constants::RadioMenu_t radioMenu, const unsigned char radioSelect) const
		{
			ExecuteCommandFormat ("radio%u;menuselect %u", radioMenu, radioSelect);
		}

		virtual inline void        ExecuteCommand            (const DynamicString &command)         const =/*>*/ 0;	// nothing, used by sub classes
		inline void                ExecuteCommandFormat      (const char *const format, ...) const
		{
			char command[HalfLifeEngine::SDK::Constants::MaximumClientCommandLength];

			// Concatenate all the arguments in one command....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, command, sizeof (command));

			ExecuteCommand (command);
		}

		virtual inline void        Kick                      (void)
		{
			// Kick this client/bot away from the server (using "kick "name"").
//			ExecuteCommand ("disconnect");
			HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommandFormat/*Now*/ ("kick \"%s\"\n", GetName ().GetData ());

			// Call it here, for the good "Remove Bot Menu" work, since engine pfnClientDisconnect() function is called too late after client kick....
//			g_server->GetClientManager ()->ClientDisconnect (GetEdict ());
		}

		inline void Kill (void) const
		{
	// This function kills a bot (not just using ClientKill(), but like the CSBot does).
	// Base code courtesy of Lazy (from bots-united forums!)

			// kill this client
			HalfLifeEngine::Globals::g_halfLifeEngine->ClientKill (m_edict);
/*	HalfLifeEngine::SDK::Classes::Edict *const hurtEntity (HalfLifeEngine::Globals::g_halfLifeEngine->CreateNamedEntity ("trigger_hurt"));

	// Reliability check.
	InternalAssert (hurtEntity->IsValid ());

	hurtEntity->variables.className = HalfLifeEngine::Globals::g_halfLifeEngine->GetOffsetFromString (HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].className);
	hurtEntity->variables.effects |= HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;
	hurtEntity->variables.damageInflictor = GetEdict ();
	hurtEntity->variables.damageTake = 1.0f;
	hurtEntity->variables.damage = GetEdict ()->variables.health/*! @warning Do not use GetEdict ()->GetHealth() here, because we don't need cast health to integer value! *//*;//9999.0f;
	hurtEntity->variables.damageTime = 2.0f;

	hurtEntity->SetOrigin (-4000.0f);

	HalfLifeEngine::SDK::Structures::KeyValueData_t keyValueData =
	{
		HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].className,
		"damagetype",
		const_cast <char *> (FormatBuffer ("%u", HalfLifeEngine::SDK::Constants::DamageType_Freeze)),
		false
	};

	HalfLifeEngine::Globals::g_halfLifeEngine->KeyValue (hurtEntity, keyValueData);

	hurtEntity->privateData->CallSpawn ();
	hurtEntity->privateData->CallTouch (GetEdict ()->privateData);

	HalfLifeEngine::Globals::g_halfLifeEngine->RemoveEntity (hurtEntity);*/
		}

		inline void StripWeapons (void)
		{
			HalfLifeEngine::SDK::Classes::Edict *const entityPlayerWeaponStrip (HalfLifeEngine::Globals::g_halfLifeEngine->CreateNamedEntity ("player_weaponstrip"));

			// Reliability check.
			InternalAssert (entityPlayerWeaponStrip->IsValid ());

			entityPlayerWeaponStrip->privateData->CallSpawn ();

			entityPlayerWeaponStrip->privateData->CallUse (GetEdict ()->privateData);

			HalfLifeEngine::Globals::g_halfLifeEngine->RemoveEntity (entityPlayerWeaponStrip);
		}
		inline void GiveItem (const char *const className)
		{
			HalfLifeEngine::SDK::Classes::Edict *const item (HalfLifeEngine::Globals::g_halfLifeEngine->CreateNamedEntity (className));

			// Reliability check.
			InternalAssert (item->IsValid ());

			item->variables.spawnFlags |= HalfLifeEngine::SDK::Constants::EntitySpawnFlag_NoRespawn;

			item->privateData->CallSpawn ();

			const HalfLifeEngine::SDK::Constants::EntitySolidType_t oldSolidType (item->variables.solid);

			item->privateData->CallTouch (GetEdict ()->privateData);

			// Remove item entity if not added....
			if (item->variables.solid == oldSolidType)
				HalfLifeEngine::Globals::g_halfLifeEngine->RemoveEntity (item);
		}

	//
	// Group: Game message handler callbacks.
	//
	public:
		virtual inline void  AmmoAmountChanged    (const HalfLifeEngine::SDK::Constants::WeaponID_t ammoID, const unsigned char newAmount)
		{
			m_ammo[ammoID].total = newAmount;	// Update amount of this type of ammo.
		}

		virtual void RoundStarted         (void);

		virtual void CurrentWeaponChanged (const bool state, const HalfLifeEngine::SDK::Constants::WeaponID_t newWeaponIndex, const char newAmmoInClip);

		virtual inline void TeamChanged (const HalfLifeEngine::SDK::Constants::TeamID_t newTeamID)
		{
			// This function caled when client team was changed.

			m_currentTeamID = newTeamID;
		}

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline void SpawnPost             (void) { /* Empty yet?! */ }
		virtual void        AliveSpawnPost        (void);
		virtual inline void RoundRespawnPost      (void) { /* Empty yet?! */ }
		virtual void        Killed                (const HalfLifeEngine::SDK::Classes::Edict *const killer);	// Is called when the client/bot is killed.
		virtual inline void AddWeapon             (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			m_owningItemsManager->AddPlayerItem (weapon);

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::AddWeapon(): Client \"%s\" got weapon %s.\n", GetName ().GetData (), weapon->GetClassName ().GetData ());
		}
		virtual inline void RemoveWeapon          (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			m_owningItemsManager->RemovePlayerItem (weapon);

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::RemoveWeapon(): Client \"%s\" lost his weapon %s.\n", GetName ().GetData (), weapon->GetClassName ().GetData ());
		}
		virtual inline void OnTouchingWeapon      (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::OnTouchingWeapon(): Client \"%s\" touch weapon %s.\n", GetName ().GetData (), weapon->GetClassName ().GetData ());
		}
		virtual void        Use                   (const HalfLifeEngine::SDK::Classes::Edict *const entity, const HalfLifeEngine::SDK::Classes::Edict *const caller, const HalfLifeEngine::SDK::Constants::UseType_t type, const float value);
		virtual void        TouchPost             (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::TouchPost(): Client \"%s\" touch %s.\n", GetName ().GetData (), entity->GetClassName ().GetData ());
		}

		virtual inline void ItemDeployPost        (HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Reliability check.
			InternalAssert (weapon->IsValid ());

			{
				delete m_currentWeapon;

				m_currentWeapon = (*CurrentWeaponConstructorFunctionsContainer::s_currentWeaponConstructorFunctions[weapon->GetWeaponID ()]) (this, weapon);
			}

			// I noised a sound.... - record it!
			if (IsAlive ())	// OCCURS!!!! (GetEdict ()->IsAlive () == true !!!)
			{
				/// @TODO: Others sounds informations for others weapons.
				m_currentNoisedSound.hearingDistance = 512.0f;
				m_currentNoisedSound.maximumLastingTime = 0.5f;
				m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

				UpdateLastNoisedSound ();
				TryReceiveLastNoisedSound ();
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::ItemDeployPost() called for client \"%s\", new weapon is: %s(%u)%s, reloadEndTime == 0.0f.\n", GetName ().GetData (), weapon->GetClassName ().GetData (), m_currentWeapon->m_id, HasShield () ? " + weapon_shield(2)" : "");
		}
		virtual inline void WeaponReloadPost      (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Reliability check.
			InternalAssert (weapon->IsValid ());
			InternalAssert (m_currentWeapon->IsValid ());
			InternalAssert (m_currentWeapon->GetEdict () == weapon);
/*			if (m_currentWeapon->GetEdict () != weapon)
			{
				AddLogEntry (true, LogLevel_Default, false, "Client::WeaponReloadPost(): m_currentWeapon->GetEdict () != weapon, m_currentWeapon->GetEdict ()->GetClassName(): %s, weapon->GetClassName(): %s,\nm_currentWeapon->edict->owner == weapon->owner: %s", m_currentWeapon->GetEdict ()->GetClassName ().GetData (), weapon->GetClassName ().GetData (), m_currentWeapon->GetEdict ()->variables.owner == weapon->variables.owner ? "true" : "false");

				return;
			}
*/
			m_currentWeapon->OnReload ();

			// I noised a sound.... - record it!
			{
				/// @TODO: Others sounds informations for others weapons.
				m_currentNoisedSound.hearingDistance = 512.0f;
				m_currentNoisedSound.maximumLastingTime = 0.5f;
				m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

				UpdateLastNoisedSound ();
				TryReceiveLastNoisedSound ();
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::WeaponReloadPost() called for client \"%s\", m_currentWeapon->GetReloadEndTime()=%.2f.\n", GetName ().GetData (), m_currentWeapon->GetReloadEndTime () - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
		}

		virtual inline void ImpulseCommand (const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t/* impulseCommand*/)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::ImpulseCommand(): Client \"%s\" is going to execute a %i impulse command.\n", GetEdict ()->GetClassName ().GetData (), impulseCommand);
		}
/*!
		inline void OnStudioSetupBones (HalfLifeEngine::SDK::Structures::Model_t *const model, const float frame, const int sequence, const Math::Angles3D &angles, const Math::Vector3D &origin, const unsigned char *const controller, const unsigned char *const blending, int boneID)
		{
			/// @todo CHECK ALL THIS SHIT!!!
			/// @warning WE SHOULD CALL SV_StudioSetupBones() FUNCTION MANUALLY EACH FRAME(WHEN CLIENT ANGLES/ANIMATION CHANGED) FOR EACH CLIENT AND THEN STORE GLOBAL 'm_boneTransformMatrix' TO CLIENT 'm_boneTransformMatrix'!!!

			const HalfLifeEngine::SDK::Structures::StudioHeader_t *const studioHeader (GetEdict ()->GetModelPointer ());

			if (boneID < -1 || boneID >= studioHeader->numbones)
				boneID = 0;

			if (boneID == -1)
			{
				// Update all bones....
				for (boneID = 0; boneID < studioHeader->numbones; ++boneID)
					m_boneTransformMatrix[boneID] = HalfLifeEngine::Globals::g_halfLifeEngine->GetBoneTransformMatrix ()[boneID];
			}
			else
			{
				const HalfLifeEngine::SDK::Structures::mstudiobone_t *const bones (reinterpret_cast <const HalfLifeEngine::SDK::Structures::mstudiobone_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->boneindex));

				do
				{
					m_boneTransformMatrix[boneID] = HalfLifeEngine::Globals::g_halfLifeEngine->GetBoneTransformMatrix ()[boneID];
				} while ((boneID = bones[boneID].parent) != -1);
			}
		}
*/
		void               EmitSound             (const DynamicString &sample, const float volume, const float attenuation, const HalfLifeEngine::SDK::Constants::SoundFlag_t flags, const HalfLifeEngine::SDK::Constants::SoundPitch_t pitch);

		virtual void       Think                 (void);

		enum HandleExecutedCommandResult_t
		{
			HandleExecutedCommandResult_Supercede,	// client function did something, skip real function call

			HandleExecutedCommandResult_Handled,	// client function did something, but real function should still be called
			HandleExecutedCommandResult_Ignored		// client function didn't take any action (Calls real function)
		};
		virtual const HandleExecutedCommandResult_t HandleExecutedCommand (const DynamicString &command, const DynamicString &argument1);

		virtual inline void HostageKilled (HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const killer)
		{
			m_hostageManager.HostageKilled (hostage, killer);
		}
		virtual inline void HostageRescued (HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			m_hostageManager.HostageRescued (hostage);
		}
		virtual inline void HostageUnusePost (HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const/* currentRescuer*/)
		{
			m_hostageManager.HostageUnusePost (hostage);
		}
		virtual inline void HostageUsePost (HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const/* previousRescuer*/)
		{
			m_hostageManager.HostageUsePost (hostage);
		}
		virtual inline void AllHostagesRescued (void)
		{
			m_hostageManager.AllHostagesRescued ();
		}

	//
	// Group: Protected engine callbacks.
	//
	protected:
		// Functions for 'm_functionThink' pointer.
		virtual void       AliveThink            (void);
		virtual void       DeathThink            (void);
};

inline void NavigationMesh::BaseNavigationArea::AddPlayer    (Client *const client) { m_players[client->GetRealTeam () - 1u] += client; }	///< add one player to this area's count
inline void NavigationMesh::BaseNavigationArea::RemovePlayer (Client *const client) { m_players[client->GetRealTeam () - 1u] -= client; }	///< subtract one player from this area's count

#endif	// ifndef CLIENT_INCLUDED