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
// GameRules.hpp
//
// Class: GameRules
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_GAME_RULES_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_GAME_RULES_INCLUDED

// weapon respawning return codes
enum
{
	GR_NONE,

	GR_WEAPON_RESPAWN_YES,
	GR_WEAPON_RESPAWN_NO,

	GR_AMMO_RESPAWN_YES,
	GR_AMMO_RESPAWN_NO,

	GR_ITEM_RESPAWN_YES,
	GR_ITEM_RESPAWN_NO,

	GR_PLR_DROP_GUN_ALL,
	GR_PLR_DROP_GUN_ACTIVE,
	GR_PLR_DROP_GUN_NO,

	GR_PLR_DROP_AMMO_ALL,
	GR_PLR_DROP_AMMO_ACTIVE,
	GR_PLR_DROP_AMMO_NO
};

// Player relationship return codes
enum
{
	GR_NOTTEAMMATE,
	GR_TEAMMATE,
	GR_ENEMY,
	GR_ALLY,
	GR_NEUTRAL
};

class GameRules : public VoidBaseHookedClassWithVirtualFunctionTable
{
	//
	// Group: Type definitions.
	//
	public:
		typedef void       (GameRules::*FunctionRestartRound_t)        (void);
		typedef const bool (GameRules::*FunctionIsFlashlightAllowed_t) (void) const;
#if 0
public:
	virtual void RefreshSkillData (void);	// fill skill data struct with proper values
	virtual void Think (void) = 0;	// GR_Think - runs every server frame, should handle any timer tasks, periodic events, etc.
	virtual BOOL IsAllowedToSpawn (BaseEntity *pEntity) = 0;	// Can this item spawn (eg monsters don't spawn in deathmatch).

	virtual BOOL FAllowFlashlight (void) = 0;	// Are players allowed to switch on their flashlight?
	virtual BOOL FShouldSwitchWeapon (BasePlayer *pPlayer, BasePlayerItem *pWeapon) = 0;	// should the player switch to this weapon?
	virtual BOOL GetNextBestWeapon (BasePlayer *pPlayer, BasePlayerItem *pCurrentWeapon) = 0;	// I can't use this weapon anymore, get me the next best one.

// Functions to verify the single/multiplayer status of a game
	virtual BOOL IsMultiplayer (void) = 0;	// is this a multiplayer game? (either coop or deathmatch)
	virtual BOOL IsDeathmatch (void) = 0;	// is this a deathmatch game?
	virtual BOOL IsTeamplay (void) { return FALSE; }	// is this deathmatch game being played with team rules?
	virtual BOOL IsCoOp (void) = 0;	// is this a coop game?
	virtual const char *GetGameDescription (void) { return "Counter-Strike"; }	// this is the game name that gets seen in the server browser

// Client connection/disconnection
	virtual BOOL ClientConnected (Edict *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128u]) = 0;	// a client just connected to the server (player hasn't spawned yet)
	virtual void InitHUD (BasePlayer *pl) = 0;		// the client dll is ready for updating
	virtual void ClientDisconnected (Edict *pClient) = 0;	// a client just disconnected from the server
	virtual void UpdateGameMode (BasePlayer *pPlayer) { /* Empty */ }	// the client needs to be informed of the current game mode

// Client damage rules
	virtual float FlPlayerFallDamage (BasePlayer *pPlayer) = 0;	// this client just hit the ground after a fall. How much damage?
	virtual BOOL FPlayerCanTakeDamage (BasePlayer *pPlayer, BaseEntity *pAttacker) { return TRUE; }	// can this player take damage from this attacker?
	virtual BOOL ShouldAutoAim (BasePlayer *pPlayer, Edict *target) { return TRUE; }

// Client spawn/respawn control
	virtual void PlayerSpawn (BasePlayer *pPlayer) = 0;	// called by BasePlayer::Spawn just before releasing player into the game
	virtual void PlayerThink (BasePlayer *pPlayer) = 0;	// called by BasePlayer::PreThink every frame, before physics are run and after keys are accepted
	virtual BOOL FPlayerCanRespawn (BasePlayer *pPlayer) = 0;	// is this player allowed to respawn now?
	virtual float FlPlayerSpawnTime (BasePlayer *pPlayer) = 0;	// When in the future will this player be able to spawn?
	virtual Edict *GetPlayerSpawnSpot (BasePlayer *pPlayer);	// Place this player on their spawnspot and face them the proper direction.

	virtual BOOL AllowAutoTargetCrosshair (void) { return TRUE; }
	virtual BOOL ClientCommand_DeadOrAlive (BasePlayer *pPlayer, const char *pcmd) { return FALSE; }	// handles the user commands; returns TRUE if command handled properly
	virtual BOOL ClientCommand (BasePlayer *pPlayer, const char *pcmd) { return FALSE; }	// handles the user commands; returns TRUE if command handled properly
	virtual void ClientUserInfoChanged (BasePlayer *pPlayer, char *infobuffer) { /* Empty */ }		// the player has changed userinfo; can change it now

// Client kills/scoring
	virtual int IPointsForKill (BasePlayer *pAttacker, BasePlayer *pKilled) = 0;	// how many points do I award whoever kills this player?
	virtual void PlayerKilled (BasePlayer *pVictim, Structures::EntityVariables_t *pKiller, Structures::EntityVariables_t *pInflictor) = 0;	// Called each time a player dies
	virtual void DeathNotice (BasePlayer *pVictim, Structures::EntityVariables_t *pKiller, Structures::EntityVariables_t *pInflictor)= 0;	// Call this from within a GameRules class to report an obituary.
// Weapon retrieval
	virtual BOOL CanHavePlayerItem (BasePlayer *pPlayer, BasePlayerItem *pWeapon);	// The player is touching an BasePlayerItem, do I give it to him?
	virtual void PlayerGotWeapon (BasePlayer *pPlayer, BasePlayerItem *pWeapon) = 0;	// Called each time a player picks up a weapon from the ground

// Weapon spawn/respawn control
	virtual int WeaponShouldRespawn (BasePlayerItem *pWeapon) = 0;	// should this weapon respawn?
	virtual float FlWeaponRespawnTime (BasePlayerItem *pWeapon) = 0;	// when may this weapon respawn?
	virtual float FlWeaponTryRespawn (BasePlayerItem *pWeapon) = 0;	// can i respawn now, and if not, when should i try again?
	virtual Math::Vector3D VecWeaponRespawnSpot (BasePlayerItem *pWeapon) = 0;	// where in the world should this weapon respawn?

// Item retrieval
	virtual BOOL CanHaveItem (BasePlayer *pPlayer, Item *pItem) = 0;	// is this player allowed to take this item?
	virtual void PlayerGotItem (BasePlayer *pPlayer, Item *pItem) = 0;	// call each time a player picks up an item (battery, healthkit, longjump)

// Item spawn/respawn control
	virtual int ItemShouldRespawn (Item *pItem) = 0;	// Should this item respawn?
	virtual float FlItemRespawnTime (Item *pItem) = 0;	// when may this item respawn?
	virtual Math::Vector3D VecItemRespawnSpot (Item *pItem) = 0;	// where in the world should this item respawn?

// Ammo retrieval
	virtual BOOL CanHaveAmmo (BasePlayer *pPlayer, const char *pszAmmoName, int iMaxCarry);	// can this player take more of this ammo?
	virtual void PlayerGotAmmo (BasePlayer *pPlayer, char *szName, int iCount) = 0;	// called each time a player picks up some ammo in the world

// Ammo spawn/respawn control
	virtual int AmmoShouldRespawn (BasePlayerAmmo *pAmmo) = 0;	// should this ammo item respawn?
	virtual float FlAmmoRespawnTime (BasePlayerAmmo *pAmmo) = 0;	// when should this ammo item respawn?
	virtual Math::Vector3D VecAmmoRespawnSpot (BasePlayerAmmo *pAmmo) = 0;	// where in the world should this ammo item respawn? By default, everything spawns

// Healthcharger respawn control
	virtual float FlHealthChargerRechargeTime (void) = 0;	// how long until a depleted HealthCharger recharges itself?
	virtual float FlHEVChargerRechargeTime (void) { return 0.0f; }	// how long until a depleted HealthCharger recharges itself?

// What happens to a dead player's weapons
	virtual int DeadPlayerWeapons (BasePlayer *pPlayer) = 0;	// what do I do with a player's weapons when he's killed?

// What happens to a dead player's ammo
	virtual int DeadPlayerAmmo (BasePlayer *pPlayer) = 0;	// Do I drop ammo when the player dies? How much?

// Teamplay stuff
	virtual const char *GetTeamID (BaseEntity *pEntity) = 0;	// what team is this entity on?
	virtual int PlayerRelationship (BaseEntity *pPlayer, BaseEntity *pTarget) = 0;	// What is the player's relationship with this entity?
	virtual int GetTeamIndex (const char *pTeamName) { return -1; }
	virtual const char *GetIndexedTeamName (int teamIndex) { return ""; }
	virtual BOOL IsValidTeam (const char *pTeamName) { return TRUE; }
	virtual void ChangePlayerTeam (BasePlayer *pPlayer, const char *pTeamName, BOOL bKill, BOOL bGib) { /* Empty */ }
	virtual const char *SetDefaultPlayerTeam (BasePlayer *pPlayer) { return ""; }

// Sounds
	virtual BOOL PlayTextureSounds (void) { return TRUE; }

// Monsters
	virtual BOOL FAllowMonsters (void) = 0;	// are monsters allowed

	// Immediately end a multiplayer game
	virtual void EndMultiplayerGame (void) { /* Empty */ }
	virtual BOOL IsFreezePeriod (void) { return FALSE; }
	virtual void ServerDeactivate (void) { /* Empty */ }
	virtual void CheckMapConditions (void) { /* Empty */ }
#endif	// if 0

	public:
		inline const bool IsFlashlightAllowed (void) const
		{
			return (this->*reinterpret_cast <const FunctionIsFlashlightAllowed_t &> (m_pseudoVirtualFunctionTable[Constants::GameRules::VirtualTableFunctionID_FAllowFlashlight])) ();
		}
};

class VoiceGameManagerHelper : public VoidBaseHookedClassWithVirtualFunctionTable
{
#if 0
public:
	virtual ~VoiceGameManagerHelper (void) { /*VOID*/ }

	// Called each frame to determine which players are allowed to hear each other.	This overrides
	// whatever squelch settings players have.
	virtual bool CanPlayerHearPlayer (BasePlayer *pListener, BasePlayer *pTalker) = 0;
#endif	// if 0
};


// VoiceGameManager manages which clients can hear which other clients.
class VoiceGameManager : public VoidBaseHookedClassWithVirtualFunctionTable
{
#if 0
public:
	VoiceGameManager (void);
	virtual ~VoiceGameManager (void);

public:
	bool Init (VoiceGameManagerHelper *pHelper, int maxClients);

	void SetHelper (VoiceGameManagerHelper *pHelper);

	// Updates which players can hear which other players.
	// If gameplay mode is DM, then only players within the PVS can hear each other.
	// If gameplay mode is teamplay, then only players on the same team can hear each other.
	// Player masks are always applied.
	void Update (double frametime);

	// Called when a new client connects (unsquelches its entity for everyone).
	void ClientConnected (struct Edict *pEdict);

	// Called on ClientCommand. Checks for the squelch and unsquelch commands.
	// Returns true if it handled the command.
	bool ClientCommand (BasePlayer *pPlayer, const char *cmd);

	// Called to determine if the Receiver has muted (blocked) the Sender
	// Returns true if the receiver has blocked the sender
	bool PlayerHasBlockedPlayer (BasePlayer *pReceiver, BasePlayer *pSender);

private:
	// Force it to update the client masks.
	void UpdateMasks (void);
#endif	// if 0
	private:
/*! off=4(1)/20(5) */		unsigned int            m_UNKNOWN0;

/*! off=8(2)/24(6) */		int                     m_messagePlayerVoiceMask;
/*! off=12(3)/28(7) */		int                     m_messageRequestState;

/*! off=16(4)/32(8) */		VoiceGameManagerHelper *m_helper;
/*! off=20(5)/36(9) */		int                     m_maximumPlayers;
/*! off=24(6)/40(10) */		double                  m_updateIntervalTime;	// How long since the last update.
};	// sizeof (VoiceGameManager) == 28 (7)

//===========================================================================
// HalfLifeMultiplay - rules for the basic half life multiplayer competition
//===========================================================================
class HalfLifeMultiplay : public GameRules
{
	public:
/*! off=4(1) */		BOOL              m_isFreezePeriod;								// TRUE at beginning of round, set to FALSE when the period expires
/*! off=8(2) */		BOOL              m_isBombDropped;

/*! off=12(3) */		unsigned int      m_UNKNOWN0;

/*! off=16(4) */		VoiceGameManager  m_voiceGameManager;

/*! off=48(12) */		float             m_restartRoundTime;							// The global time when the round is supposed to end, if this is not 0

/*! off=52(13) */		unsigned int      m_UNKNOWN1;

/*! off=56(14) */		float             m_roundCount;
/*! off=60(15) */		unsigned int      m_roundTime;									// (From mp_roundtime) - How many seconds long this round is.
/*! off=64(16) */		unsigned int      m_roundTimeSecs;								// (Current) round timer - set to 'm_introRoundTime', then 'm_roundTime'.
/*! off=68(17) */		unsigned int      m_introRoundTime;								// (From mp_freezetime) - How many seconds long the intro round (when players are frozen) is.
/*! off=72(18) */		float             m_introRoundCount;							// The global time when the intro round ends and the real one starts.
/*! off=76(19) */		unsigned int      m_account[TeamArrayID_Total];
/*! off=84(21) */		unsigned int      m_playerCount[TeamArrayID_Total];				// The number of players on the team (this is generated at the end of a round)
/*! off=92(23) */		unsigned int      m_spawnableNumber[TeamArrayID_Total];
/*! off=100(25) */		unsigned int      m_spawnPointCount[TeamArrayID_Total];			// Number of team spawn points.
/*! off=108(27) */		unsigned int      m_hostagesRescued;
/*! off=112(28) */		unsigned int      m_hostagesTouched;
/*! off=116(29) */		unsigned int      m_roundWinStatus;								// 1 == CT's won last round, 2 == Terrorists did, 3 == Draw, no winner.
/*! off=120(30) */		unsigned short    m_score[TeamArrayID_Total];					// WARNING: m_score[0] for CT's, m_score[1] for T's!

		// BOMB MAP VARIABLES....
/*! off=124(31) */		bool              m_isTargetBombed;								// Whether or not the bomb has been bombed.
/*! off=125(31.25) */		bool              m_isBombDefused;								// Whether or not the bomb has been defused.
/*! off=126(31.5) */		bool              m_isMapHasBombTarget;
/*! off=127(31.75) */		bool              m_isMapHasBombZone;

/*! off=128(32) */		bool              m_isMapHasBuyZone;
/*! off=129(32.5) */		bool              m_isMapHasRescueZone;
/*! off=130(32.75) */		bool              m_isMapHasEscapeZone;

/*! off=132(33) */		unsigned int      m_isMapHasVIPSafetyZone;						// 0 = uninitialized; 1 = has VIP safety zone; 2 = DOES not have VIP safetyzone
/*! off=136(34) */		unsigned int      m_mapHasCamera;								// initialized to 2 = Not Set, 1 = Has Cam, 0 = No Cam
/*! off=140(35) */		unsigned int      m_c4Timer;									// How long from when the C4 is armed until it blows.
/*! off=144(36) */		unsigned int      m_c4Guy;										// The current Terrorist who has the C4.
/*! off=148(37) */		unsigned int      m_loserBonus;									// The amount of money the losing team gets.
/*! off=152(38) */		unsigned int      m_consecutiveLosesNumber[TeamArrayID_Total];	// The number of rounds the team have lost in a row. Team winning streak. (WARNING: m_consecutiveLosesNumber[0] for CT's, m_consecutiveLosesNumber[1] for T's!)
/*! off=160(40) */		float             m_maximumIdlePeriod;							// For the idle kick functionality. This is tha maximum amount of time that the player has to be idle before being kicked
/*! off=164(41) */		unsigned int      m_limitTeams;									// Maximum # of players 1 team can have over another.
/*! off=168(42) */		bool              m_isLevelInitialized;							// Set to 0 in GameDllInit and checked in Think, if false, spawns are counted in order to fill 'm_spawnPointCount' (see TeamFull)
/*! off=169(42.25) */		bool              m_isRoundTerminating;
/*! off=170(42.5) */		bool              m_isCompleteReset;							// Set to TRUE to have the scores reset next time round restarts.

/*! off=172(43) */		float             m_requiredEscapeRatio;
/*! off=176(44) */		unsigned int      m_escapersNumber;								// Number of escaped terrorist (for escape maps)
/*! off=180(45) */		unsigned int      m_haveEscaped;
/*! off=184(46) */		bool              m_isCantBuy[TeamArrayID_Total];

/*! off=188(47) */		float             m_c4ExplosionRadius;							// initialized to 500.0
/*! off=192(48) */		unsigned int      m_consecutiveVIP;
/*! off=196(49) */		unsigned int      m_weaponsNumber;								// (Including smoke grenade)
/*! off=200(50) */		unsigned int      m_grenadesNumber;								// (Only HE and flashbang grenades)
/*! off=204(51) */		unsigned int      m_armorNumber;								// (Assault suit and kevlar)
/*! off=208(52) */		unsigned int      m_unBalancedRounds;							// Keeps track of the # of consecutive rounds that have gone by where one team outnumbers the other team by more than 2.
/*! off=212(53) */		unsigned int      m_escapeRoundsNumber;							// Keeps track of the # of consecutive rounds of escape played.. Teams will be swapped after 8 rounds.
/*! off=216(54) */		unsigned int      m_mapsVotesNumbers[Constants::MaximumVoteMapListLength];
/*! off=616(154) */		unsigned int      m_lastPick;

/*! off=620(155) */		unsigned int      m_UNKNOWN2;

/*! off=624(156) */		unsigned int      m_maximumRounds;
/*! off=628(157) */		unsigned int      m_totalRoundsPlayed;
/*! off=632(158) */		unsigned int      m_winLimit;
/*! off=636(159) */		int               m_allowSpectators;
/*! off=640(160) */		float             m_forceCamera;
/*! off=644(161) */		float             m_forceChaseCamera;
/*! off=648(162) */		float             m_fadeToBlack;
/*! off=652(163) */		BasePlayer       *m_VIP;
/*! off=656(164) */		BasePlayer       *m_VIPQueue[5u];
/*! off=676(169) */		float             m_intermissionEndTime;
/*! off=680(170) */		float             m_intermissionStartTime;
/*! off=684(171) */		BOOL              m_endIntermissionButtonHit;
/*! off=688(172) */		float             m_nextPeriodicThinkTime;						// (updates every 1 second)
/*! off=692(173) */		bool              m_isGameCommenced;
/*! off=693(173.25) */		bool              m_isCareerMatch;

/*! off=696(174) */		unsigned int      m_UNKNOWN3;
/*! off=700(175) */		unsigned int      m_UNKNOWN4;

/*! off=704(176) */		int               m_winDifference;
/*! off=708(177) */		float             m_careerNextThink;	// initialized to 0 // CHalfLifeMultiplay::Think() // checked in CHalfLifeMultiplay::QueueCareerRoundEndMenu and set to gametime + 3.0, return if < gametime
/*! off=712(178) */		bool              m_isShouldSkipSpawn;

/*! off=716(179) */		unsigned int      m_UNKNOWN5;

	public:
		inline const bool  IsFreezePeriod    (void) const { return reinterpret_cast <const bool *> (&m_isFreezePeriod)[0u]; }
		inline const float GetRoundStartTime (void) const { return m_roundCount; }	// When this round started.

		inline const bool IsTeamFull (const Constants::TeamID_t teamID) const
		{
			switch (teamID)
			{
				case Constants::TeamID_Terrorist:
					return m_playerCount[TeamArrayID_Terrorist] >= m_spawnPointCount[TeamArrayID_Terrorist];

				case Constants::TeamID_CounterTerrorist:
					return m_playerCount[TeamArrayID_CounterTerrorist] >= m_spawnPointCount[TeamArrayID_CounterTerrorist];

				default:
					return false;
			}
		}

		// Note: The following 3 functions are declared in 'Half-Life engine/Core.hpp'.
		inline const bool IsTeamStacked (const Constants::TeamID_t currentTeamID, const Constants::TeamID_t newTeamID) const;	// checks to see if the desired team is stacked, returns true if it is
		inline const float GetRoundRemainingTime (void) const;
		inline const float GetRoundElapsedTime   (void) const;
#if 0
	public:
		HalfLifeMultiplay (void);

	public:
	// GR_Think
		virtual void Think (void);
		virtual void RefreshSkillData (void);
		virtual BOOL IsAllowedToSpawn (BaseEntity *pEntity);
		virtual BOOL FAllowFlashlight (void);

		virtual BOOL FShouldSwitchWeapon (BasePlayer *pPlayer, BasePlayerItem *pWeapon);
		virtual BOOL GetNextBestWeapon (BasePlayer *pPlayer, BasePlayerItem *pCurrentWeapon);

	// Functions to verify the single/multiplayer status of a game
		virtual BOOL IsMultiplayer (void);
		virtual BOOL IsDeathmatch (void);
		virtual BOOL IsCoOp (void);

	// Client connection/disconnection
		// If ClientConnected returns FALSE, the connection is rejected and the user is provided the reason specified in szRejectReason
		// Only the client's name and remote address are provided to the dll for verification.
		virtual BOOL ClientConnected (Edict *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128u]);
		virtual void InitHUD (BasePlayer *pl);		// the client dll is ready for updating
		virtual void ClientDisconnected (Edict *pClient);
		virtual void UpdateGameMode (BasePlayer *pPlayer);	// the client needs to be informed of the current game mode

	// Client damage rules
		virtual float FlPlayerFallDamage (BasePlayer *pPlayer);
		virtual BOOL FPlayerCanTakeDamage (BasePlayer *pPlayer, BaseEntity *pAttacker);

	// Client spawn/respawn control
		virtual void PlayerSpawn (BasePlayer *pPlayer);
		virtual void PlayerThink (BasePlayer *pPlayer);
		virtual BOOL FPlayerCanRespawn (BasePlayer *pPlayer);
		virtual float FlPlayerSpawnTime (BasePlayer *pPlayer);
		virtual Edict *GetPlayerSpawnSpot (BasePlayer *pPlayer);

		virtual BOOL AllowAutoTargetCrosshair (void);
		virtual BOOL ClientCommand (BasePlayer *pPlayer, const char *pcmd);

	// Client kills/scoring
		virtual int IPointsForKill (BasePlayer *pAttacker, BasePlayer *pKilled);
		virtual void PlayerKilled (BasePlayer *pVictim, Structures::EntityVariables_t *pKiller, Structures::EntityVariables_t *pInflictor);
		virtual void DeathNotice (BasePlayer *pVictim, Structures::EntityVariables_t *pKiller, Structures::EntityVariables_t *pInflictor);

	// Weapon retrieval
		virtual void PlayerGotWeapon (BasePlayer *pPlayer, BasePlayerItem *pWeapon);
		virtual BOOL CanHavePlayerItem (BasePlayer *pPlayer, BasePlayerItem *pWeapon);	// The player is touching an BasePlayerItem, do I give it to him?

	// Weapon spawn/respawn control
		virtual int WeaponShouldRespawn (BasePlayerItem *pWeapon);
		virtual float FlWeaponRespawnTime (BasePlayerItem *pWeapon);
		virtual float FlWeaponTryRespawn (BasePlayerItem *pWeapon);
		virtual Math::Vector3D VecWeaponRespawnSpot (BasePlayerItem *pWeapon);

	// Item retrieval
		virtual BOOL CanHaveItem (BasePlayer *pPlayer, Item *pItem);
		virtual void PlayerGotItem (BasePlayer *pPlayer, Item *pItem);

	// Item spawn/respawn control
		virtual int ItemShouldRespawn (Item *pItem);
		virtual float FlItemRespawnTime (Item *pItem);
		virtual Math::Vector3D VecItemRespawnSpot (Item *pItem);

	// Ammo retrieval
		virtual void PlayerGotAmmo (BasePlayer *pPlayer, char *szName, int iCount);

	// Ammo spawn/respawn control
		virtual int AmmoShouldRespawn (BasePlayerAmmo *pAmmo);
		virtual float FlAmmoRespawnTime (BasePlayerAmmo *pAmmo);
		virtual Math::Vector3D VecAmmoRespawnSpot (BasePlayerAmmo *pAmmo);

	// Healthcharger respawn control
		virtual float FlHealthChargerRechargeTime (void);
		virtual float FlHEVChargerRechargeTime (void);

	// What happens to a dead player's weapons
		virtual int DeadPlayerWeapons (BasePlayer *pPlayer);

	// What happens to a dead player's ammo
		virtual int DeadPlayerAmmo (BasePlayer *pPlayer);

	// Teamplay stuff
		virtual const char *GetTeamID (BaseEntity *pEntity) { return ""; }
		virtual int PlayerRelationship (BaseEntity *pPlayer, BaseEntity *pTarget);

		virtual BOOL PlayTextureSounds (void) { return FALSE; }
		virtual BOOL PlayFootstepSounds (BasePlayer *pl, float fvol);

	// Monsters
		virtual BOOL FAllowMonsters (void);

		// Immediately end a multiplayer game
		virtual void EndMultiplayerGame (void) { GoToIntermission (); }

		virtual BOOL IsFreezePeriod (void) { return m_isFreezePeriod; }

		// Recreate all the map entities from the map data (preserving their indices),
		// then remove everything else except the players.
		// Also get rid of all world decals.
		virtual void CleanUpMap (void) { /* Empty */ }

		virtual void RestartRound (void) { /* Empty */ }
		virtual void CheckWinConditions (void) { /* Empty */ }
		virtual void RemoveGuns (void);
		virtual void GiveC4 (void);

	protected:
		virtual void ChangeLevel (void);
		virtual void GoToIntermission (void);

		void SendMOTDToClient (Edict *client);
#endif	// if 0
};	// sizeof (HalfLifeMultiplay) == 720 (180)

class HalfLifeTraining : public HalfLifeMultiplay
{
	public:
/*! off=720(180) */		float        m_UNKNOWN7Time;
/*! off=724(181) */		unsigned int m_UNKNOWN8;
/*! off=728(182) */		BOOL         m_UNKNOWN9;
/*! off=732(183) */		BOOL         m_UNKNOWN10;

/*! off=736(184) */		bool         m_isShowVGUIMenus;

/*! off=740(185) */		unsigned int      m_UNKNOWN11;
};	// sizeof (HalfLifeTraining) == 744 (186)

#endif // HALF_LIFE_ENGINE_SDK_CLASSES_GAME_RULES_INCLUDED