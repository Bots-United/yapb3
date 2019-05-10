/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef BASE_PLAYER_WEAPON_H
#define BASE_PLAYER_WEAPON_H

// inventory items that
class BasePlayerWeapon : public BasePlayerItem
{
	//
	// Group: Type definitions.
	//
	public:
		typedef void (BasePlayerWeapon::*FunctionReload_t) (void);

		// Weapon m_iState.
		DECLARE_TYPED_ENUMERATION (unsigned int, StateFlag_t)
		{
			WEAPON_NOT_CARRIED = 0u,	// Weapon is on the ground
			WEAPON_IS_ACTIVE   = 1u,	// This client is carrying this weapon and it's the currently held weapon

			WEAPON_IS_ONTARGET = 0x40u
		};

	public:
/*!off=176(44)*/		BOOL                            m_iPlayEmptySound;
/*!off=180(45)*/		BOOL                            m_fFireOnEmpty;				// True when the gun is empty and the player is still holding down the attack key(s)

		// Weapon firing
/*!off=184(46)*/		float                           m_flNextPrimaryAttack;		// soonest time ItemPostFrame will call PrimaryAttack
/*!off=188(47)*/		float                           m_flNextSecondaryAttack;	// soonest time ItemPostFrame will call SecondaryAttack
/*!off=192(48)*/		float                           m_flTimeWeaponIdle;			// soonest time ItemPostFrame will call WeaponIdle
/*!off=196(49)*/		int                             m_iPrimaryAmmoType;			// "primary" ammo index into players m_rgAmmo[]
/*!off=200(50)*/		int                             m_iSecondaryAmmoType;		// "secondary" ammo index into players m_rgAmmo[]
/*!off=204(51)*/		int                             m_iClip;					// number of shots left in the primary weapon clip, -1 it not used
/*!off=208(52)*/		int                             m_iClientClip;				// the last version of m_iClip sent to hud dll
/*!off=212(53)*/		StateFlag_t                     m_iClientWeaponState;		// the last version of the weapon state sent to hud dll (is current weapon, is on target)
/*!off=216(54)*/		BOOL                            m_fInReload;				// Are we in the middle of a reload
/*!off=220(55)*/		unsigned int                    m_fInSpecialReload;			// Are we in the middle of a reload for the shotguns (state) (0 ... 2)
/*!off=224(56)*/		int                             m_iDefaultAmmo;				// how much ammo you get when you pick up this weapon as placed by a level designer.
/*!off=228(57)*/		unsigned int                    m_iShellId;					// Shell model index of awp, g3sg1, glock18, m3, scout, sg550, xm1014.
/*!off=232(58)*/		float                           m_ownerMaximumSpeed;		/// CHECK ME!!!!!!
/*!off=236(59)*/		bool                            m_bDelayFire;				// This variable is used to delay the time between subsequent button pressing.
/*!off=240(60)*/		int                             m_iDirection;				// The current lateral kicking direction; 1 = right, 0 = left
/*!off=244(61)*/		unsigned int                    m_UNKNOWN0;
/*!off=248(62)*/		float                           m_flAccuracy;
/*!off=252(63)*/		float                           m_lastFireTime;
/*!off=256(64)*/		unsigned int                    m_iShotsFired;				// Pistols reset it to 0 when the attack button is released.
/*!off=260(65)*/		unsigned int                    m_UNKNOWN1[3u];
/*!off=272(68)*/		TypeDefinitions::StringOffset_t m_viewModelName;
/*!off=276(69)*/		float                           m_flGlock18Shoot;			// time to shoot the remaining bullets of the glock18 burst fire
/*!off=280(70)*/		int                             m_iGlock18ShotsFired;		// used to keep track of the shots fired during the Glock18 burst fire mode.
/*!off=284(71)*/		float                           m_flFamasShoot;				// time to shoot the remaining bullets of the famas burst fire
/*!off=288(72)*/		int                             m_iFamasShotsFired;			// used to keep track of the shots fired during the Famas burst fire mode....
/*!off=292(73)*/		float                           m_fBurstSpread;				// used to keep track of current spread factor so that all bullets in spread use same spread
/*!off=296(74)*/		Constants::WeaponState_t        m_iWeaponState;				// miscellaneous weapon flags
/*!off=300(75)*/		float                           m_nextReloadTime;
/*!off=304(76)*/		float                           m_flDecreaseShotsFired;		// (Other name 'm_lastTakeInHandTime') (in fact this is not true for primary weapons)
/*!off=308(77)*/		unsigned short                  m_usFireGlock18;
/*!off=310(77.5)*/		unsigned short                  m_usFireFamas;

/*!off=312(78)*/		unsigned int                    m_iShell;					// Shell model index, gun only.

/// FROM OTHER CLASSES 'CM3' OR 'XM1014'
//*!off=316(79)*/		float m_flPumpTime;
#if 0
public:
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );

	static	TYPEDESCRIPTION m_SaveData[];

	// generic weapon versions of BasePlayerItem calls
	virtual int AddToPlayer( BasePlayer *pPlayer );
	virtual int AddDuplicate( BasePlayerItem *pItem );

	virtual int ExtractAmmo( BasePlayerWeapon *pWeapon ); //{ return TRUE; };			// Return TRUE if you can add ammo to yourself when picked up
	virtual int ExtractClipAmmo( BasePlayerWeapon *pWeapon );// { return TRUE; };			// Return TRUE if you can add ammo to yourself when picked up

	virtual int AddWeapon( void ) { ExtractAmmo( this ); return TRUE; };	// Return TRUE if you want to add yourself to the player

	// generic "shared" ammo handlers
	BOOL AddPrimaryAmmo( int iCount, char *szName, int iMaxClip, int iMaxCarry );
	BOOL AddSecondaryAmmo( int iCount, char *szName, int iMaxCarry );

	virtual void UpdateItemInfo( void ) {};	// updates HUD state

	virtual BOOL PlayEmptySound( void );
	virtual void ResetEmptySound( void );

	virtual void SendWeaponAnim( int iAnim, int skiplocal = 1, int body = 0 );  // skiplocal is 1 if client is predicting weapon animations

	virtual BOOL CanDeploy( void );
	virtual BOOL IsUseable( void );
	BOOL DefaultDeploy( char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal = 0, int body = 0 );
	int DefaultReload( int iClipSize, int iAnim, float fDelay, int body = 0 );

	virtual void ItemPostFrame( void );	// called each frame by the player PostThink
	// called by BasePlayerWeapons ItemPostFrame()
	virtual void PrimaryAttack( void ) { return; }				// do "+ATTACK"
	virtual void SecondaryAttack( void ) { return; }			// do "+ATTACK2"
	virtual void Reload( void ) { return; }						// do "+RELOAD"
	virtual void WeaponIdle( void ) { return; }					// called when no buttons pressed
	virtual int UpdateClientData( BasePlayer *pPlayer );		// sends hud info to client dll, if things have changed
	virtual void RetireWeapon( void );
	virtual BOOL ShouldWeaponIdle( void ) {return FALSE; };
	virtual void Holster( int skiplocal = 0 );
	virtual BOOL UseDecrement( void ) { return FALSE; };

	int	PrimaryAmmoIndex();
	int	SecondaryAmmoIndex();

	void PrintState( void );

	virtual BasePlayerItem *GetWeaponPtr( void ) { return (BasePlayerItem *)this; }
#endif	// if 0
};

inline       BasePlayerWeapon *const BaseEntity::GetBasePlayerWeaponPointer (void)       { return static_cast <BasePlayerWeapon *const> (this); }
inline const BasePlayerWeapon *const BaseEntity::GetBasePlayerWeaponPointer (void) const { return static_cast <const BasePlayerWeapon *const> (this); }

#endif // BASE_PLAYER_WEAPON_H