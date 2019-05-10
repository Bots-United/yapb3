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
// BasePlayer.hpp
//
// Class: BasePlayer
//
// Description: This is Half-Life player entity.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BASE_PLAYER_INCLUDED
#define BASE_PLAYER_INCLUDED

//
// generic player
//
//-----------------------------------------------------
//This is Half-Life player entity
//-----------------------------------------------------
#define CSUITPLAYLIST	4		// max of 4 suit sentences queued up at any time

#define SUIT_GROUP			TRUE
#define	SUIT_SENTENCE		FALSE

#define	SUIT_REPEAT_OK		0
#define SUIT_NEXT_IN_30SEC	30
#define SUIT_NEXT_IN_1MIN	60
#define SUIT_NEXT_IN_5MIN	300
#define SUIT_NEXT_IN_10MIN	600
#define SUIT_NEXT_IN_30MIN	1800
#define SUIT_NEXT_IN_1HOUR	3600

#define CSUITNOREPEAT		32

#define	SOUND_FLASHLIGHT_ON		"items/flashlight1.wav"
#define	SOUND_FLASHLIGHT_OFF	"items/flashlight1.wav"

// Enumerations for setting player animation.
enum PLAYER_ANIM
{
	PLAYER_IDLE,

	PLAYER_WALK,
	PLAYER_JUMP,
	PLAYER_SUPERJUMP,
	PLAYER_DIE,
	PLAYER_ATTACK1,

	/// @todo CHECK BELOW!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TF Player animations
	PLAYER_RELOAD = 9u,
//	PLAYER_START_AIMING,
//	PLAYER_LEAVE_AIMING,

	PlayerAnimation_Total = 11u
};

#define MAX_ID_RANGE 2048
#define SBAR_STRING_SIZE 128

enum sbar_data
{
	SBAR_ID_TARGETNAME = 1,
	SBAR_ID_TARGETHEALTH,
	SBAR_ID_TARGETARMOR,
	SBAR_END
};

#define CHAT_INTERVAL 1.0f

class BasePlayer : public BaseMonster
{
	//
	// Group: Type definitions.
	//
	public:
		typedef const bool (BasePlayer::*FunctionIsBot_t)            (void) const;
		typedef void       (BasePlayer::*FunctionRoundRespawn_t)     (void);
		typedef void       (BasePlayer::*FunctionImpulseCommands_t)  (void);
		typedef void       (BasePlayer::*FunctionOnTouchingWeapon_t) (WeaponBox *const weaponBox);

	public:
		enum SpectatedPlayerBombState_t
		{
			SpectatedPlayerBombState_NotHave,			// hasn't bomb

			SpectatedPlayerBombState_Have,				// has bomb
			SpectatedPlayerBombState_HaveAndInBombPlace	// has bomb and in target zone
		};
		enum IgnoreMessagesState_t
		{
			IgnoreMessagesState_AcceptAllMessages,
			IgnoreMessagesState_IgnoreBroadcastMessages,
			IgnoreMessagesState_IgnoreBroadcastTeamMessages
		};

		class HintMessage
		{
			private:
/*! off=0(0) */				const char          *m_hintString;	///< hintString is a pointer to a string that should never be deleted.
/*! off=4(1) */				bool                 m_isUNKNOWN;
/*! off=8(2) */				CUtlVector <char *>  m_args;		///< list of arguments. The memory for these strings is internal to the HintMessage.
/*! off=28(7) */				float                m_duration;	///< time until the next message can be displayed
/*
			public:
				HintMessage( const char * hintString, const bool isUNKNOWN = true, CUtlVector< const char * > * args, float duration );
				~HintMessage();

				float GetDuration() const { return m_duration; }
				void Send( BasePlayer *client );

				bool IsEquivalent( const char *hintString, CUtlVector< const char * > * args ) const;
*/		};	// sizeof (HintMessage) == 32 (8)
		class HintMessageQueue
		{
			private:
/*! off=0(0) */				float                      m_tmMessageEnd;
/*! off=4(1) */				CUtlVector <HintMessage *> m_messages;
/*
			public:
				HintMessageQueue(BasePlayer *pPlayer);
				void		Reset();
				void		Update(BasePlayer *pPlayer);
				bool		AddMessage(const char* message, float duration = 6.0f, const bool isUNKNOWN = true, CUtlVector< const char * > *args = NULL);
				inline bool IsEmpty() { return m_messages.Count() == 0; }
*/		};	// sizeof (HintMessageQueue) == 24 (6)

		struct Rebuy_t
		{
/*! off=0(0)/2340(585) */			Constants::WeaponID_t        secondaryWeaponID;	/// @note if player hasn't secondary weapon, but have shield - value for this variable will be 99.
/*! off=4(1)/2344(586) */			int                          secondaryWeaponAmmo;

/*! off=8(2)/2348(587) */			Constants::WeaponID_t        knifeID;	// WTF?
/*! off=12(3)/2352(588) */			int                          knifeAmmo;	// WTF?

/*! off=16(4)/2356(589) */			int                          HEGrenadeAmmo;	// WTF?
/*! off=20(5)/2360(590) */			int                          flashGrenadeAmmo;
/*! off=24(6)/2364(591) */			int                          smokeGrenadeAmmo;	// WTF?

/*! off=28(7)/2368(592) */			BOOL                         hasDefuseKit;
/*! off=32(8)/2372(593) */			BOOL                         hasNightVisionGoggles;
/*! off=36(9)/2376(594) */			Constants::PlayerArmorType_t armorType;
		};	// sizeof (Rebuy_t) == 40 (10)

	public:
/*! off=384(96) */		int					random_seed;	// See that is shared between client & server for shared weapons code

/*! off=388(97) */		unsigned int        m_UNKNOWN12;

/*! off=392(98) */		EntityHandle        m_observerTarget;
/*! off=400(100) */		float               m_nextObserverInput;
/*! off=404(101) */		Constants::WeaponID_t               m_spectatedPlayerWeaponID;
/*! off=408(102) */		SpectatedPlayerBombState_t          m_spectatedPlayerBombState;
/*! off=412(103) */		bool                m_isSpectatedPlayerHasDefuseKit;
/*! off=416(104) */		Constants::SpectatorObserverMode_t  m_spectatorViewType;

/*! off=420(105) */		unsigned int        m_UNKNOWN13;
/*! off=424(106) */		unsigned int        m_UNKNOWN14;

/*! off=428(107) */		bool                m_isLightDamaged;
/*! off=432(108) */		float               m_punishVelocityModifier;	// 0.0, ... 1.0
/*! off=436(109) */		unsigned int        m_lastZoom;	// Default 90.
/*! off=440(110) */		bool                m_resumeZoom;
/*! off=444(111) */		float               m_ejectBrass;
/*! off=448(112) */		Constants::PlayerArmorType_t        m_armorType;
/*! off=452(113) */		bool                m_isNotKilled;
/*! off=456(114) */		Constants::TeamID_t        m_team;
/*! off=460(115) */		unsigned int        m_money;
/*! off=464(116) */		bool                m_hasPrimaryWeapon;

/*! off=468(117) */		unsigned int        m_UNKNOWN15;
/*! off=472(118) */		unsigned int        m_UNKNOWN16;

/*! off=476(119) */		float               m_lastChatTime;
/*! off=480(120) */		bool                m_isJustConnected;
/*! off=484(121) */		Constants::PlayerJoiningState_t  m_joiningState;

		// When the player joins, it cycles their view between trigger_camera entities.
		// This is the current camera, and the time that we'll switch to the next one.
/*! off=488(122) */		BaseEntity         *m_introCamera;
/*! off=492(123) */		float               m_introCameraTime;

/*! off=496(124) */		float               m_lastMovementTime;
/*! off=500(125) */		bool                m_isMissionBriefingDisplayed;
/*! off=501(125.25) */		bool                m_isTeamChanged;
/*! off=504(126) */		unsigned int        m_internalModelIndex;
/*! off=508(127) */		unsigned int        m_teamKillsNumber;
/*! off=512(128) */		IgnoreMessagesState_t        m_ignoreMessagesState;
/*! off=516(129) */		bool                m_hasNightVisionGoggles;
/*! off=517(129.25) */		bool                m_isUsesNightVisionGoggles;

/*! off=520(130) */		Math::Vector3D      m_UNKNOWN17[20u];	// Some unused vectors table....

/*! off=760(190) */		float               m_idleCheckTime;
/*! off=764(191) */		float               m_nextRadioTime;
/*! off=768(192) */		unsigned int        m_radioCommandsUseAmount;
/*! off=772(193) */		bool                m_isCantHearRadioCommands;
/*! off=773(193.25) */		bool                m_isCanPlantBomb;
/*! off=774(193.5) */		bool                m_hasDefuseKit;
/*! off=775(193.75) */		bool                m_hasDiedByExplosion;

/*! off=776(194) */		Math::Vector3D        m_lastTakeDamageDirection;

/*! off=788(197) */		bool                m_hasDiedByHEGrenade;
/*! off=792(198) */		HintMessageQueue    m_hintMessageQueue;
/*! off=816(204) */		unsigned int        m_displayAutoHelpHUDTextHistoryBits;
/*! off=820(205) */		Constants::MenuID_t m_openedMenuIndex;

/*! off=824(206) */		unsigned int        m_UNKNOWN79_;	// UNUSED!!! Only sets to 1 in CBasePlayer__Spawn() function and no more used.
/*! off=828(207) */		unsigned int        m_UNKNOWN80_;	// UNUSED!!!
/*! off=832(208) */		unsigned int        m_UNKNOWN81_;	// UNUSED!!! Only sets to 0 in CBasePlayer__Spawn() function and no more used.

/*! off=836(209) */		bool                m_isEscaped;
/*! off=837(209.25) */		bool                m_isVIP;
/*! off=840(210) */		float               m_nextRadarUpdateTime;
/*! off=844(211) */		Math::Vector3D      m_lastPosition;
/*! off=856(214) */		unsigned int        m_lastNominatedToKickPlayerUserID;
/*! off=860(215) */		float               m_nextVoteCommandUsageTime;
/*! off=864(216) */		bool                m_isJustKilledTeammate;
/*! off=868(217) */		unsigned int        m_hostageKillsNumber;
/*! off=872(218) */		unsigned int        m_lastVotedMapID;
/*! off=876(219) */		bool                m_isPrimaryFireAllowed;	// False when freezetime or player shield is drawn.
/*! off=880(220) */		float               m_lastAttackTime;

/*! off=884(221) */		float               m_lastGameTeammateAttackMessageTimestamp;	// Delay is 0.6 seconds.

/*! off=888(222) */		bool                m_isWasKilledWithHeadShot;
/*! off=889(222.25) */		bool                m_isPunishedForTeamKill;
/*! off=890(222.5) */		bool                m_isMarkedAsNotReceivingMoney;

/*! off=892(223) */		float        m_lastTimeLeftCommandUsingTimestamp;	// "timeleft"
/*! off=896(224) */		bool        m_isNameChanged;	// Should real name changed to new one?
/*! off=897(224.25) */		char        m_newTempName[Constants::MaximumClientNameLength];	// This name will be assigned and reset at next player spawn if not same with real name.
/*! off=929(232.25) */		bool                m_isDefusingBomb;

/*! off=932(233) */		float                 m_handleSignalsNextCallGlobalTime;

/*! off=936(234) */		Constants::MapZone_t  m_mapZone;
/*! off=940(235) */		Constants::MapZone_t  m_clientMapZone;
/*! off=944(236) */		Edict              *m_lastTouchedBombTargetZone;

/*! off=948(237) */		int					m_soundIndex;			// The index of the sound list slot reserved for this player.
/*! off=952(238) */		int					m_targetVolume;			// ideal sound volume.
/*! off=956(239) */		int					m_weaponVolume;			// how loud the player's weapon is right now.
/*! off=960(240) */		int					m_extraSoundTypes;		// additional classification for this weapon's sound
/*! off=964(241) */		int					m_weaponFlash;			// brightness of the weapon flash
/*! off=968(242) */		float				m_stopExtraSoundTime;

/*! off=972(243) */		float				m_flashlightTime;		// Time until next battery draw/Recharge
/*! off=976(244) */		int					m_flashlightBattery;	// Flashlight Battery Draw

/*! off=980(245) */		int					m_afButtonLast;
/*! off=984(246) */		int					m_afButtonPressed;
/*! off=988(247) */		int					m_afButtonReleased;

/*! off=992(248) */		Edict			   *m_pentSndLast;			// last sound entity to modify player room type
/*! off=996(249) */		float				m_flSndRoomtype;		// last roomtype set by sound entity
/*! off=1000(250) */		float				m_flSndRange;			// distance from player to sound entity

/*! off=1004(251) */		float				m_flFallVelocity;

/*! off=1008(252) */		int					m_rgItems[Constants::PlayerItemType_Total];

/*! off=1028(257) */		unsigned int		m_afPhysicsFlags;	// physics flags - set when 'normal' physics should be revisited or overriden
/*! off=1032(258) */		float				m_fNextSuicideTime;	// the time after which the player can next use the suicide command

	// these are time-sensitive things that we keep track of
/*! off=1036(259) */		float				m_flTimeStepSound;	// when the last stepping sound was made
/*! off=1040(260) */		float				m_flTimeWeaponIdle;	// when to play another weapon idle animation.
/*! off=1044(261) */		float				m_flSwimTime;		// how long player has been underwater
/*! off=1048(262) */		float				m_flDuckTime;		// how long we've been ducking
/*! off=1052(263) */		float				m_flWallJumpTime;	// how long until next walljump

/*! off=1056(264) */		float				m_flSuitUpdate;					// when to play next suit update
/*! off=1060(265) */		int					m_rgSuitPlayList[CSUITPLAYLIST];// next sentencenum to play for suit update
/*! off=1076(269) */		int					m_iSuitPlayNext;				// next sentence slot for queue storage;
/*! off=1080(270) */		int					m_rgiSuitNoRepeat[CSUITNOREPEAT];		// suit sentence no repeat list
/*! off=1208(302) */		float				m_rgflSuitNoRepeatTime[CSUITNOREPEAT];	// how long to wait before allowing repeat
/*! off=1336(334) */		int					m_lastDamageAmount;		// Last damage taken
/*! off=1340(335) */		float				m_tbdPrev;				// Time-based damage timer

/*! off=1344(336) */		float				m_flgeigerRange;		// range to nearest radiation source
/*! off=1348(337) */		float				m_flgeigerDelay;		// delay per update of range msg to client
/*! off=1352(338) */		int					m_igeigerRangePrev;
/*! off=1356(339) */		int					m_iStepLeft;			// alternate left/right foot stepping sound
/*! off=1360(340) */		char				m_szTextureName[Constants::MaximumTextureNameLength];	// current texture name we're standing on
/*! off=1376(344) */		char				m_chTextureType;		// current texture type

/*! off=1380(345) */		int					m_idrowndmg;			// track drowning damage taken
/*! off=1384(346) */		int					m_idrownrestored;		// track drowning damage restored

/*! off=1388(347) */		int					m_bitsHUDDamage;		// Damage bits for the current fame. These get sent to
													// the hude via the DAMAGE message
/*! off=1392(348) */		BOOL				m_fInitHUD;				// True when deferred HUD restart msg needs to be sent
/*! off=1396(349) */		BOOL				m_fGameHUDInitialized;
/*! off=1400(350) */		int					m_iTrain;				// Train control position
/*! off=1404(351) */		BOOL				m_fWeapon;				// Set this to FALSE to force a reset of the current weapon HUD info

/*! off=1408(352) */		EntityHandle				m_pTank;				// the tank which the player is currently controlling, NULL if no tank
/*! off=1416(354) */		float				m_fDeadTime;			// the time at which the player died  (used in PlayerDeathThink())

/*! off=1420(355) */		BOOL			m_fNoPlayerSound;	// a debugging feature. Player makes no sound if this is true.
/*! off=1424(356) */		BOOL			m_fLongJump;	// does this player have the longjump module?

/*! off=1428(357) */		float       m_tSneaking;
/*! off=1432(358) */		int			m_iUpdateTime;		// stores the number of frame ticks before sending HUD update messages
/*! off=1436(359) */		int			m_iClientHealth;	// the health currently known by the client. If this changes, send a new
/*! off=1440(360) */		int			m_iClientBattery;	// the Battery currently known by the client. If this changes, send a new
/*! off=1444(361) */		Constants::HideHUDBit_t			m_iHideHUD;	// the players hud weapon info is to be hidden
/*! off=1448(362) */		Constants::HideHUDBit_t			m_iClientHideHUD;
/*! off=1452(363) */		int			m_iFOV;			// field of view
/*! off=1456(364) */		int			m_iClientFOV;	// client's known FOV
/*! off=1460(365) */		unsigned int  m_spawnsNumber;
/*! off=1464(366) */		BaseEntity   *m_UNKNOWN92;	// UNUSED!!! (If not NULL removed in HalfLifeMultiplay::ClientDisconnect() by BaseEntity::SUB_Remove())
		// usable player items
/*! off=WeaponSlotID_UNKNOWN=1468(367) - UNUSED!!!!!!, WeaponSlotID_PrimaryWeapons + 1=1472(368), WeaponSlotID_SecondaryWeapons + 1=1476(369), WeaponSlotID_Knife + 1=1480(370), WeaponSlotID_Grenades + 1=1484(371), WeaponSlotID_C4 + 1=1488(372) */		BasePlayerItem	*m_rgpPlayerItems[Constants::MaximumPlayerItemTypes];	/// @warning 1 based!!!!
/*! off=1492(373) */		BasePlayerItem *m_pActiveItem;
/*! off=1496(374) */		BasePlayerItem *m_pClientActiveItem;	// client version of the active item
/*! off=1500(375) */		BasePlayerItem *m_pLastItem;
		// shared ammo slots
/*! off=1504(376) */		int	m_rgAmmo[Constants::MaximumWeapons];
/*! off=1632(408) */		int	m_rgAmmoLast[Constants::MaximumWeapons];

/*! off=1760(440) */		Math::Vector3D				m_vecAutoAim;
/*! off=1772(443) */		BOOL				m_fOnTarget;
/*! off=1776(444) */		int					m_iDeaths;
/*! off=1780(445) */		float				m_iRespawnFrames;	// used in PlayerDeathThink() to make sure players can always respawn

		// Player ID
		//{
//		void InitStatusBar( void );
//		void UpdateStatusBar( void );
/*! off=1784(446) */		int m_statusBarState[SBAR_END];
/*! off=1800(450) */		float m_statusBarDisappearDelay;
/*! off=1804(451) */		char m_statusBarString[SBAR_STRING_SIZE];
		//}

/*! off=1932(483)/1936(484) */		int m_lastx, m_lasty;	// These are the previous update's crosshair angles, DON"T SAVE/RESTORE

/*! off=1940(485) */		int m_nCustomSprayFrames;	// Custom clan logo frames for this player
/*! off=1944(486) */		float	m_flNextDecalTime;	// next time this player can spray a decal

/*! off=1948(487) */		char m_szTeamName[Constants::MaximumTeamNameLength];
#if 0
		virtual void Spawn( void );
		void Pain( void );

		virtual void Jump( void );
		virtual void Duck( void );
		virtual void PreThink( void );
		virtual void PostThink( void );
		virtual Math::Vector3D GetGunPosition( void );
		virtual BOOL IsBot (void) { return FALSE; }
		virtual int TakeHealth( float flHealth, Constants::DamageType_t bitsDamageType );
		virtual void TraceAttack( Structures::EntityVariables_t *pevAttacker, float flDamage, Math::Vector3D vecDir, TraceResult *ptr, Constants::DamageType_t bitsDamageType);
		virtual int TakeDamage( Structures::EntityVariables_t* pevInflictor, Structures::EntityVariables_t* pevAttacker, float flDamage, Constants::DamageType_t bitsDamageType);
		virtual void Killed( Structures::EntityVariables_t *pevAttacker, int iGib );
		virtual Math::Vector3D BodyTarget( const Math::Vector3D &posSrc ) { return Center () + m_variables->viewOffset * RANDOM_FLOAT( 0.5f, 1.1f ); }	// position to shoot at
		virtual void StartSneaking( void ) { m_tSneaking = gpGlobals->time - 1.0f; }
		virtual void StopSneaking( void ) { m_tSneaking = gpGlobals->time + 30.0f; }
		virtual BOOL IsSneaking( void ) { return m_tSneaking <= gpGlobals->time; }
		virtual BOOL IsAlive( void ) { return m_variables->deadFlag == DEAD_NO && m_variables->health > 0.0f; }
		virtual BOOL ShouldFadeOnDeath( void ) { return FALSE; }
		virtual	BOOL IsPlayer( void ) { return TRUE; }			// Spectators should return FALSE for this, they aren't "players" as far as game logic is concerned

		virtual BOOL IsNetClient( void ) { return TRUE; }		// Bots should return FALSE for this, they can't receive NET messages
																// Spectators should return TRUE for this
		virtual const char *TeamID( void );

		virtual int		Save( CSave &save );
		virtual int		Restore( CRestore &restore );
		void RenewItems(void);
		void PackDeadPlayerItems( void );
		void RemoveAllItems( BOOL removeSuit );
		BOOL SwitchWeapon( BasePlayerItem *pWeapon );

		// JOHN:  sends custom messages if player HUD data has changed  (eg health, ammo)
		virtual void UpdateClientData( void );

		static	TYPEDESCRIPTION m_playerSaveData[];

		// Player is moved across the transition by other means
		virtual int		ObjectCaps( void ) { return BaseMonster::ObjectCaps () & ~Constants::CapBit_ACROSS_TRANSITION; }
		virtual void	Precache( void );
		BOOL			IsOnLadder( void );
		BOOL			FlashlightIsOn( void );
		void			FlashlightTurnOn( void );
		void			FlashlightTurnOff( void );

		void UpdatePlayerSound ( void );
		void DeathSound ( void );

		Constants::EntityClassType_t Classify ( void );
		void SetAnimation( PLAYER_ANIM playerAnim );
		void SetWeaponAnimType( const char *szExtention );
#endif	// if 0
/*! off=1964(491) */		int  m_newModelIndex;
/*! off=1968(492) */		char m_szAnimExtention[32u];
#if 0
		// custom player functions
		virtual void ImpulseCommands( void );
		void CheatImpulseCommands( int iImpulse );

		virtual void RoundRespawn (void);

		void StartDeathCam( void );
		void StartObserver( Math::Vector3D vecPosition, Math::Vector3D vecViewAngle );

		void AddPoints( int score, BOOL bAllowNegativeScore );
		void AddPointsToTeam( int score, BOOL bAllowNegativeScore );
		BOOL AddPlayerItem( BasePlayerItem *pItem );
		BOOL RemovePlayerItem( BasePlayerItem *pItem );
		void DropPlayerItem ( char *pszItemName );
		BOOL HasPlayerItem( BasePlayerItem *pCheckItem );
		BOOL HasNamedPlayerItem( const char *pszItemName );
		BOOL HasWeapons( void );// do I have ANY weapons?
		void SelectPrevItem( int iItem );
		void SelectNextItem( int iItem );
		void SelectLastItem(void);
		void SelectItem(const char *pstr);
		void ItemPreFrame( void );
		void ItemPostFrame( void );
		void GiveNamedItem( const char *szName );
		void EnableControl(BOOL fControl);

		int  GiveAmmo( int iAmount, char *szName, int iMax );
		void SendAmmoUpdate(void);

		void WaterMove( void );
		void EXPORT PlayerDeathThink( void );
		void PlayerUse( void );

		void CheckSuitUpdate();
		void SetSuitUpdate(char *name, int fgroup, int iNoRepeat);
		void UpdateGeigerCounter( void );
		void CheckTimeBasedDamage( void );

		BOOL FBecomeProne ( void );
		void BarnacleVictimBitten ( Structures::EntityVariables_t *pevBarnacle );
		void BarnacleVictimReleased ( void );
		static int GetAmmoIndex(const char *psz);
		int AmmoInventory( int iAmmoIndex );
		int Illumination( void );

		void ResetAutoaim( void );
		virtual Math::Vector3D GetAutoaimVector( float flDelta  );
		Math::Vector3D AutoaimDeflection( Math::Vector3D &vecSrc, float flDist, float flDelta  );


		void ForceClientDllUpdate( void );	// Forces all client .dll specific data to be resent to client.

		void DeathMessage( Structures::EntityVariables_t *pevKiller );

		void SetCustomDecalFrames( int nFrames );
		int GetCustomDecalFrames( void );

		void TabulateAmmo( void );

		virtual void Blind (float holdTime, float fadeHold, float fadeTime, int alpha);
		virtual void OnTouchingWeapon (WeaponBox *const weaponBox);
#endif	// if 0
/*! off=2000(500) */		int            m_gaitSequence;
/*! off=2004(501) */		float          m_gaitUNKNOWN0;
/*! off=2008(502) */		float          m_gaitYaw;
/*! off=2012(503) */		Math::Vector3D m_gaitUNKNOWN1_PlayerLastOrigin;
/*! off=2024(506) */		Math::Angles2D m_studioBlendGaitAngles;	// GetPlayerPitch()/GetPlayerYaw()
/*! off=2032(508) */		float          m_gaitUNKNOWN2_LengthFromLastOrigin;
/*! off=2036(509) */		BOOL           m_autoWeaponSwitch;
/*! off=2040(510) */		bool           m_showVGUIMenus;
/*! off=2041(510.25) */		bool           m_showAutoHelp;
/*! off=2042(510.5) */		bool           m_isShieldDrawn;
/*! off=2043(510.75) */		bool           m_hasShield;
/*! off=2044(511) */		bool           m_observerUNKNOWN0;
/*! off=2048(512) */		float          m_observerFindNextPlayerNextCallTimestamp;
/*! off=2052(513) */		float          m_gaitUNKNOWN3;
/*! off=2056(514) */		float          m_blindEndTime;
/*! off=2060(515) */		float          m_blindStartTime;
/*! off=2064(516) */		float          m_blindFadeHold;
/*! off=2068(517) */		float          m_blindFadeTime;
/*! off=2072(518) */		unsigned int   m_blindAlpha;
/*! off=2076(519) */		float          m_botAllowAutoFollowTime;	// Used by CZ bots to autofollow for this player.
/*! off=2080(520) */		char           m_autoBuyData[256u];
/*! off=2336(584) */		char          *m_rebuyData/*[256u]*/;
/*! off=2340(585) */		Rebuy_t m_rebuy;
/*! off=2380(595) */		bool    m_cantBuildRebuyStruct;
/*! off=2384(596) */		float   m_updateLocationSendTimestamp;
/*! off=2388(597) */		char    m_locationPhraseName[32u];
/*! off=2420(605) */		float   m_progressBarStartTime;
/*! off=2424(606) */		float   m_progressBarEndTime;
/*! off=2428(607) */		bool    m_isSpecSetAD;
/*! off=2429(607.25) */		bool    m_isCanSwitchObserverModes;

/// @warning I'M NOT SHURE ABOUT THIS, MAYBE THIS IS NOT THIS VARIABLES - IT UNUSED!!!
///@{
/*! off=2432(608) */		float m_startCharge;
/*! off=2436(609) */		float m_ammoStartCharge;
/*! off=2440(610) */		float m_playAfterShock;
/*! off=2444(611) */		float m_nextAmmoBurn;	// while charging, when to absorb another unit of player's ammo?
///@}

	public:
		inline const bool HasShield           (void) const { return m_hasShield; }
		inline const bool IsShieldDrawn       (void) const { return m_isShieldDrawn; }
		inline const bool IsProtectedByShield (void) const { return HasShield () && IsShieldDrawn (); }
/*
//MIKETODO: this probably should let the shield model catch the trace attacks.
inline const bool IsHittingShield (const Math::Vector2D &vecDirection, const Structures::TraceResult_t &ptr) const
{
	if (m_pActiveItem != NULL && m_pActiveItem->GetID () == Constants::WeaponID_C4)
		return false;

	if (HasShield () == false)
		return false;

	if (ptr.hitGroup == Constants::PlayerBodyPart_Shield)
		return true;

	if (IsShieldDrawn () == false)
		return false;
/// @todo VERIFY ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Math::Vector2D vec2LOS (vecDirection);

	vec2LOS.Normalize ();

	const Math::Vector2D vForward (GetEyeForwardDirection ());
	const float flDot (vec2LOS | vForward);

	return flDot < -0.87f;
}
*/
};	// sizeof (BasePlayer) == CS: CZ: 2448 (612)

inline       BasePlayer *const BaseEntity::GetBasePlayerPointer (void)       { return static_cast <BasePlayer *const> (this); }
inline const BasePlayer *const BaseEntity::GetBasePlayerPointer (void) const { return static_cast <const BasePlayer *const> (this); }

class NonCZBasePlayer : public BasePlayer
{
	public:
		/// @warning IN CS: CZ THIS IS NOT EXISTS!!!!
		enum CommandTimerID_t
		{
			CommandTimerID_Say,
			CommandTimerID_SayTeam,
			CommandTimerID_FullUpdate,
			CommandTimerID_Vote,
			CommandTimerID_VoteMap,
			CommandTimerID_ListMaps,
			CommandTimerID_ListPlayers,
			CommandTimerID_NightVision,

			CommandTimerID_Total
		};
/*! off=2448(612) */		float   m_nextCommandCallTimestamps[CommandTimerID_Total];
};	// sizeof (NonCZBasePlayer) == CS 1.6: 2480 (620)

inline       NonCZBasePlayer *const BaseEntity::GetNonCZBasePlayerPointer (void)       { return static_cast <NonCZBasePlayer *const> (this); }
inline const NonCZBasePlayer *const BaseEntity::GetNonCZBasePlayerPointer (void) const { return static_cast <const NonCZBasePlayer *const> (this); }

#define AUTOAIM_2DEGREES  0.0348994967025
#define AUTOAIM_5DEGREES  0.08715574274766
#define AUTOAIM_8DEGREES  0.1391731009601
#define AUTOAIM_10DEGREES 0.1736481776669

#endif	// ifndef BASE_PLAYER_INCLUDED