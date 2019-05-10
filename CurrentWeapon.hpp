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
// CurrentWeapon.hpp
//
// Class: CurrentWeapon
//
// Description: Contains the client/bot current weapon class.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CURRENT_WEAPON_INCLUDED
#define CURRENT_WEAPON_INCLUDED

class OwningItem
{
	friend OwningItemsManager;

	//
	// Group: Protected members.
	//
	protected:
		const Client *const                         m_client;	// Pointer to client/bot owning this item.

		HalfLifeEngine::SDK::Constants::WeaponID_t  m_id;		// weapon ID
		const HalfLifeEngine::SDK::Classes::Edict  *m_edict;	// pointer to actual edict

	//
	// Group: (Con/De)structors.
	//
	public:
		// Current weapon is not assigned at start.
		inline OwningItem (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			m_client (client),
			m_id (edict == NULL ? HalfLifeEngine::SDK::Constants::WeaponID_None : edict->GetWeaponID ()),	// TEMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			m_edict (edict)
		{ /* VOID */ }

		virtual inline ~OwningItem (void) { /* VOID */ }

	//
	// Group: Private operators.
	//
	private:
		inline OwningItem &operator = (const OwningItem &/*right*/);	// Avoid "warning C4512: 'OwningItem' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsValid
		//
		// Description: Checks whether client/bot current weapon is valid.
		//
		// Returns: True if client/bot current weapon is valid, false otherwise.
		//
		inline const bool                                       IsValid            (void) const
		{
/*NEED TO INVOLVE, FOR NOW IT'S CRASH!!!			#if defined _DEBUG
				if (m_id == HalfLifeEngine::SDK::Constants::WeaponID_None)
					return false;

				// Reliability check.
				InternalAssert (m_edict->IsValid ());

				return true;
			#else	// if defined _DEBUG
				return m_id != HalfLifeEngine::SDK::Constants::WeaponID_None;
			#endif	// if !defined _DEBUG*/
			return m_id != HalfLifeEngine::SDK::Constants::WeaponID_None && m_edict->IsValid ();
		}

		//
		// Function: GetEdict
		//
		// Description: Gets the client current weapon entity pointer.
		//
		// Returns: Client current weapon entity pointer.
		//
		inline const HalfLifeEngine::SDK::Classes::Edict *const GetEdict              (void) const { return m_edict; }

		virtual inline const HalfLifeEngine::SDK::Constants::WeaponID_t GetID                 (void) const { return m_id; }

		inline const char                                       GetAmmoInClip         (void) const { return m_client->m_ammo[m_id].clip; }	// ammo in clip for current weapon
		inline const unsigned char                              GetMaximumAmmoInClip  (void) const { return HalfLifeEngine::SDK::Constants::weaponProperties[m_id].ammoMaximumClip; }	// maximum ammo in clip for current weapon
		inline const unsigned char                              GetAmmo               (void) const { return HalfLifeEngine::SDK::Constants::weaponProperties[m_id].ammoID == 0u ? 0u : m_client->m_ammo[HalfLifeEngine::SDK::Constants::weaponProperties[m_id].ammoID].total; }	// total ammo amounts for current weapon
		inline const float                                      GetClipAmmoRatio      (void) const
		{
			const char clipAmmo (GetAmmoInClip ());

			if (clipAmmo < 0)
				return 1.0f;

			const unsigned char maximumClipAmmo (GetMaximumAmmoInClip ());

			if (maximumClipAmmo == 0u)
				return 0.0f;

			return static_cast <const float> (clipAmmo) / maximumClipAmmo;
		}

		virtual inline const bool                               IsCurrentWeapon       (void) const { return false; }
		virtual inline const bool                               IsInReload            (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return GetAmmoInClip () == 0 && GetAmmo () > 0u;
		}
/*		inline const bool                                       IsInReload2           (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			// is current weapon in reload?
			return m_edict->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_WEAPON_IN_RELOAD);
		}*/
		virtual inline const bool                                       IsSilenced            (void) const
		{
			// Is current weapon silenced? Does only work on M4A1 and USP.

			// Reliability check.
			InternalAssert (IsValid ());

			switch (m_id)
			{
				case HalfLifeEngine::SDK::Constants::WeaponID_USP:
					if (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_USP_SILENCED)
						return true;

				case HalfLifeEngine::SDK::Constants::WeaponID_M4A1:
					if (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_M4A1_SILENCED)
						return true;

				default:
					return false;	// All else return false.
			}
		}
		virtual inline const bool                                       IsInBurstFireMode     (void) const
		{
			// Is weapon in burst mode? Does only work with FAMAS and GLOCK.

			// Reliability check.
			InternalAssert (IsValid ());

			switch (m_id)
			{
				case HalfLifeEngine::SDK::Constants::WeaponID_Glock18:
					if (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_GLOCK18_BURST_MODE)
						return true;

				case HalfLifeEngine::SDK::Constants::WeaponID_Famas:
					if (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_FAMAS_BURST_MODE)
						return true;

				default:
					return false;	// All else return false.
			}
		}

		virtual inline const bool                                       IsKnife               (void) const
		{
			// This function returns true, if client/bot current weapon is knife, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsKnife (m_id);
		}

		virtual inline const bool                                       IsPistol              (void) const
		{
			// This function returns true, if client/bot current weapon is pistol, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsPistol (m_id);
		}

		virtual inline const bool                                       IsShield              (void) const
		{
			// This function returns true, if client/bot current weapon is shield, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsShield (m_id);
		}

		virtual inline const bool                                       IsShotgun             (void) const
		{
			// This function returns true, if client/bot current weapon is shotgun, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsShotgun (m_id);
		}

		virtual inline const bool                                       IsSubMachineGun       (void) const
		{
			// This function returns true, if client/bot current weapon is submachinegun, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsSubMachineGun (m_id);
		}

		virtual inline const bool                                       IsZoomableRifle       (void) const
		{
			// This function returns true, if client/bot current weapon is zoomable rifle, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsZoomableRifle (m_id);
		}

		virtual inline const bool                                       IsRifle               (void) const
		{
			// This function returns true, if client/bot current weapon is rifle, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsRifle (m_id);
		}

		virtual inline const bool                                       IsSniperRifle         (void) const
		{
			// This function returns true, if client/bot current weapon is sniper rifle, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsSniperRifle (m_id);
		}

		virtual inline const bool                                       IsMachineGun          (void) const
		{
			// This function returns true, if client/bot current weapon is machinegun, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsMachineGun (m_id);
		}

		virtual inline const bool                                       IsBadPrimary          (void) const
		{
			// This function returns true, if client/bot is using bad primary weapon, false otherwise.

			switch (m_id)
			{
				case HalfLifeEngine::SDK::Constants::WeaponID_XM1014:
				case HalfLifeEngine::SDK::Constants::WeaponID_M3:
				case HalfLifeEngine::SDK::Constants::WeaponID_MAC10:
				case HalfLifeEngine::SDK::Constants::WeaponID_TMP:
				case HalfLifeEngine::SDK::Constants::WeaponID_UMP45:
				case HalfLifeEngine::SDK::Constants::WeaponID_P90:
					return true;

				default:
					return false;
			}
		}

		virtual inline const bool                                       IsGrenade             (void) const
		{
			// This function returns true, if client/bot current weapon is grenade, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsGrenade (m_id);
		}

		virtual inline const bool                                       IsC4                  (void) const
		{
			// This function returns true, if client/bot current weapon is c4, false otherwise.

			return HalfLifeEngine::Utilities::WeaponIsC4 (m_id);
		}
};

class CurrentWeapon : public OwningItem
{
	friend Client;
	friend YaPB;

	//
	// Group: (Con/De)structors.
	//
	public:
		// Current weapon is not assigned at start.
		inline CurrentWeapon (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict = NULL/*TEMP!!!*/) :
			OwningItem (client, edict)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsCurrentWeapon       (void) const { return true; }

		inline const float                                      GetNextAttackDelay    (void) const { return m_client->m_edict->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::Offset_BaseMonster_NextAttackDelay); }

		virtual inline const float                                      GetReloadEndTime      (void) const { return 0.0f; }
		virtual inline const bool                                       IsInReload            (void) const { return false; }
		virtual inline const bool                                       IsCanAttack           (void) const { return false; }

		virtual inline const bool                                       IsSilenced            (void) const
		{
			// Is current weapon silenced? Does only work on M4A1 and USP.

			return false;	// All cases - return false.
		}
		virtual inline const bool                                       IsInBurstFireMode     (void) const
		{
			// Is weapon in burst mode? Does only work with FAMAS and GLOCK.

			return false;	// All cases - return false.
		}

		virtual inline const bool IsKnife                  (void) const { return false; }
		virtual inline const bool IsPistol                 (void) const { return false; }
		virtual inline const bool IsShield                 (void) const { return false; }
		virtual inline const bool IsShotgun                (void) const { return false; }
		virtual inline const bool IsSubMachineGun          (void) const { return false; }
		virtual inline const bool IsZoomableRifle          (void) const { return false; }
		virtual inline const bool IsRifle                  (void) const { return false; }
		virtual inline const bool IsSniperRifle            (void) const { return false; }
		virtual inline const bool IsMachineGun             (void) const { return false; }
		virtual inline const bool IsBadPrimary             (void) const { return false; }
		virtual inline const bool IsGrenade                (void) const { return false; }
		virtual inline const bool IsC4                     (void) const { return false; }

		inline const float          GetAccuracy              (void) const { return m_edict->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_WEAPON_ACCURACY); }
		virtual inline const float  GetSpreadOffset          (void) const { return 0.0f; }
/*! @todo TEST ME!!! */		inline const float          GetSpreadRadius          (void) const { return HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius * GetSpreadOffset (); }
		inline const float          GetSpreadRadius          (const float distance) const { return GetSpreadRadius () * distance; }
		inline const Math::Vector2D GetSpreadOffset2D          (void) const
		{/*
			typedef const float (*UTIL_SharedRandomFloat_t) (const unsigned int seed, const float low, const float high);

			/// @warning THIS IS TEMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			/// @todo FIND SIGNATURE FOR UTIL_SharedRandomFloat() FUNCTION FROM MP.DLL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			const UTIL_SharedRandomFloat_t UTIL_SharedRandomFloat (reinterpret_cast <UTIL_SharedRandomFloat_t> (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ()) + 0xCD030u));

			// Reliability check.
			InternalAssert (UTIL_SharedRandomFloat != NULL);
*/
			const unsigned int clientRandomSeed (m_client->GetEdict ()->privateData->GetBasePlayerPointer ()->random_seed);
			Math::Vector2D offset
			(
				HalfLifeEngine::Utilities::g_sharedRandomNumberGenerator.UTIL_SharedRandomFloat (clientRandomSeed,      -HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius, HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius) + HalfLifeEngine::Utilities::g_sharedRandomNumberGenerator.UTIL_SharedRandomFloat (clientRandomSeed + 1u, -HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius, HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius),
				HalfLifeEngine::Utilities::g_sharedRandomNumberGenerator.UTIL_SharedRandomFloat (clientRandomSeed + 2u, -HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius, HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius) + HalfLifeEngine::Utilities::g_sharedRandomNumberGenerator.UTIL_SharedRandomFloat (clientRandomSeed + 3u, -HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius, HalfLifeEngine::SDK::Constants::DefaultWeaponSpreadRadius)
			);

			return offset *= GetSpreadOffset ();
		}
		inline Math::Angles2D GetSpreadOffsetAngles (const Math::Angles2D &inputAngles) const
		{
			const Math::Vector2D offset (GetSpreadOffset2D ());

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%s::%s::GetSpreadOffsetAngles(): seed=%u, GetSpreadOffset()=%f, GetSpreadOffset2D()=[%f, %f].\n", m_client->GetName ().GetData (), GetEdict()->GetClassName ().GetData (), m_client->GetEdict ()->privateData->GetBasePlayerPointer ()->random_seed, GetSpreadOffset (), offset.x, offset.y);

			inputAngles.MakeVectors ();

			const Math::Vector3D direction (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward + offset.x * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right + offset.y * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->upward);
			Math::Angles2D outAngles (direction.ToAngles2D ());

			outAngles.pitch = -outAngles.pitch;	// Invert for engine (adjust the view angle pitch to aim correctly).

			return (outAngles -= inputAngles).Clamp ();
		}
		inline Math::Angles2D GetSpreadOffsetAngles (void) const { return GetSpreadOffsetAngles (m_client->GetViewAngles ()); }

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline void OnOwnerSpawned (void) { /* Empty */ }
		virtual inline void OnFire         (void) { /* Empty */ }
		virtual inline void OnReload       (void) { /* Empty */ }
};

class CurrentWeapon_Reloadable : public CurrentWeapon
{
	friend YaPB;

	//
	// Group: Protected members.
	//
	protected:
		float         m_reloadEndTime;		// time to reload end, 0.0f - weapon not reloading

		float         m_timeLastFired;		// time to last firing
		unsigned char m_burstShotsFired;	// number of bullets fired

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Reloadable (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon (client, edict),

			m_reloadEndTime (0.0f),

			m_timeLastFired (0.0f),
			m_burstShotsFired (0u)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		virtual inline void SendAttackNoiseSound (void) const { /* Empty */ }

	//
	// Group: Functions.
	//
	public:
		inline const float                                      GetReloadEndTime      (void) const { return m_reloadEndTime; }
		inline const float                                      GetTimeLastFired      (void) const { return m_timeLastFired; }
		inline const unsigned char                              GetBurstShotsFired    (void) const { return m_burstShotsFired; }
		inline const bool                                       IsInReload            (void) const
		{
			// Reliability check.
//			InternalAssert (IsValid ());	// OCCURS!!!

			return m_reloadEndTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();	// is current weapon in reload?
		}

		inline const bool                                       IsCanAttack           (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return GetAmmoInClip () > 0 && GetNextAttackDelay () <= 0.0f;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return true; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline void OnOwnerSpawned (void)
		{
			m_reloadEndTime = 0.0f;
			m_timeLastFired = 0.0f;
			m_burstShotsFired = 0u;
		}
		virtual inline void OnFire   (void)
		{
			// time fired with in burst firing time?
			if (m_timeLastFired + 1.0f > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				++m_burstShotsFired;

			// remember the last bullet time
			m_timeLastFired = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();

//			SendAttackNoiseSound ();	/// @warning FORCES NEW MESSAGE SENDING WHILE CURRENT MESSAGE PROCESSING!
		}
		virtual inline void OnReload (void)
		{
			m_reloadEndTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + GetNextAttackDelay ();
		}
};

class CurrentWeapon_P228 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_P228 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/p228-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, HalfLifeEngine::SDK::Constants::SoundAttenuation_Normal, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return true; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (1.0f - GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 1.5f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
				return 0.255f * accuracy;

			if (m_client->GetEdict ()->IsDucking ())
				return 0.075f * accuracy;

			return 0.15f * accuracy;
		}
};

class CurrentWeapon_Shield : public CurrentWeapon
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Shield (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon (client, edict)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return true; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }
};

class CurrentWeapon_Scout : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Scout (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/scout_fire-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 1.6f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return true; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			float spreadOffset (0.007f);

			if (!m_client->IsOnFloor ())
				spreadOffset = 0.2f;
			else if (m_client->GetEdict ()->GetSpeed2D () > 170.0f)
				spreadOffset = 0.075f;
			else if (m_client->GetEdict ()->IsDucking ())
				spreadOffset = 0.0f;

			if (!m_client->IsUsingScope ())
				spreadOffset += 0.025f;

			return spreadOffset;
		}
};

class CurrentWeapon_ExplosiveGrenade : public CurrentWeapon
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_ExplosiveGrenade (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon (client, edict)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsCanAttack           (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return GetNextAttackDelay () <= 0.0f;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return true; }
		inline const bool                                                             IsC4                     (void) const { return false; }
};

class CurrentWeapon_XM1014 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_XM1014 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/xm1014-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.52f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return true; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }
};

class CurrentWeapon_C4 : public CurrentWeapon
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_C4 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon (client, edict)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return true; }
};

class CurrentWeapon_MAC10 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_MAC10 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/mac10-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.72f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return true; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.375f * accuracy;

			return 0.03f * accuracy;
		}
};

class CurrentWeapon_Aug : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Aug (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/aug-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.48f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return true; }
		inline const bool                                                             IsRifle                  (void) const { return true; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.035f + 0.4f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 140.0f)
				return 0.035f + 0.07f * accuracy;

			return 0.02f * accuracy;
		}
};

class CurrentWeapon_SmokeGrenade : public CurrentWeapon
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_SmokeGrenade (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon (client, edict)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsCanAttack           (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return GetNextAttackDelay () <= 0.0f;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return true; }
		inline const bool                                                             IsC4                     (void) const { return false; }
};

class CurrentWeapon_Elite : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Elite (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/elite_fire.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, HalfLifeEngine::SDK::Constants::SoundAttenuation_Normal, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return true; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (1.0f - GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 1.3f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
				return 0.175f * accuracy;

			if (m_client->GetEdict ()->IsDucking ())
				return 0.08f * accuracy;

			return 0.1f * accuracy;
		}
};

class CurrentWeapon_FiveSeven : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_FiveSeven (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/fiveseven-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, HalfLifeEngine::SDK::Constants::SoundAttenuation_Normal, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return true; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (1.0f - GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 1.5f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
				return 0.255f * accuracy;

			if (m_client->GetEdict ()->IsDucking ())
				return 0.075f * accuracy;

			return 0.15f * accuracy;
		}
};

class CurrentWeapon_UMP45 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_UMP45 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/ump45-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.64f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return true; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.24f * accuracy;

			return 0.04f * accuracy;
		}
};

class CurrentWeapon_SG550 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_SG550 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/sg550-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.4f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return true; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (1.0f - GetAccuracy ());
			float spreadOffset (0.05f * accuracy);

			if (!m_client->IsOnFloor ())
				spreadOffset = 0.45f * accuracy;
			else if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
				spreadOffset = 0.15f;
			else if (m_client->GetEdict ()->IsDucking ())
				spreadOffset = 0.04f * accuracy;

			if (!m_client->IsUsingScope ())
				spreadOffset += 0.025f;

			return spreadOffset;
		}
};

class CurrentWeapon_Galil : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Galil (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/galil-1.wav"/*"weapons/galil-2.wav"*/, HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.4f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return true; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.04f + 0.3f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 140.0f)
				return 0.04f + 0.07f * accuracy;

			return 0.0375f * accuracy;
		}
};

class CurrentWeapon_Famas : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Famas (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/famas-1.wav"/*"weapons/famas-2.wav"*/, HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.48f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsInBurstFireMode     (void) const
		{
			// Is weapon in burst mode? Does only work with FAMAS and GLOCK.

			// Reliability check.
			InternalAssert (IsValid ());

			return (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_FAMAS_BURST_MODE) > 0u;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return true; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());
			float spreadOffset (0.02f * accuracy);

			// if player is in air
			if (!m_client->IsOnFloor ())
				spreadOffset = 0.03f + 0.3f * accuracy;

			// if player is moving
			if (m_client->GetEdict ()->GetSpeed2D () > 140.0f)
				spreadOffset = 0.03f + 0.07f * accuracy;

			if (!IsInBurstFireMode ())
				spreadOffset += 0.01f;

			return spreadOffset;
		}
};

class CurrentWeapon_USP : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_USP (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			if (IsSilenced ())
				HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/usp1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, HalfLifeEngine::SDK::Constants::SoundAttenuation_Idle, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
			else
				HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/usp_unsil-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, HalfLifeEngine::SDK::Constants::SoundAttenuation_Normal, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 87u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 18u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsSilenced            (void) const
		{
			// Is current weapon silenced? Does only work on M4A1 and USP.

			// Reliability check.
			InternalAssert (IsValid ());

			return (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_USP_SILENCED) > 0u;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return true; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (1.0f - GetAccuracy ());

			if (IsSilenced ())
			{
				if (!m_client->IsOnFloor ())
					return 1.3f * accuracy;

				if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
					return 0.25f * accuracy;

				if (m_client->GetEdict ()->IsDucking ())
					return 0.125f * accuracy;

				return 0.15f * accuracy;
			}
			else
			{
				if (!m_client->IsOnFloor ())
					return 1.2f * accuracy;

				if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
					return 0.225f * accuracy;

				if (m_client->GetEdict ()->IsDucking ())
					return 0.08f * accuracy;

				return 0.1f * accuracy;
			}
		}
};

class CurrentWeapon_Glock18 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Glock18 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/glock18-2.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, HalfLifeEngine::SDK::Constants::SoundAttenuation_Normal, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsInBurstFireMode     (void) const
		{
			// Is weapon in burst mode? Does only work with FAMAS and GLOCK.

			// Reliability check.
			InternalAssert (IsValid ());

			return (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_GLOCK18_BURST_MODE) > 0u;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return true; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (1.0f - GetAccuracy ());

			if (IsInBurstFireMode ())
			{
				if (!m_client->IsOnFloor ())
					return 1.2f * accuracy;

				if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
					return 0.185f * accuracy;

				if (m_client->GetEdict ()->IsDucking ())
					return 0.095f * accuracy;

				return 0.3f * accuracy;
			}
			else
			{
				if (!m_client->IsOnFloor ())
					return accuracy;

				if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
					return 0.165f * accuracy;

				if (m_client->GetEdict ()->IsDucking ())
					return 0.075f * accuracy;

				return 0.1f * accuracy;
			}
		}
};

class CurrentWeapon_AWP : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_AWP (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/awp1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.28f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return true; }
		inline const bool                                                             IsSniperRifle            (void) const { return true; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			float spreadOffset (0.001f);

			if (!m_client->IsOnFloor ())
				spreadOffset = 0.85f;
			else
			{
				const float ownerSpeed2D (m_client->GetEdict ()->GetSpeed2D ());

				if (ownerSpeed2D > 140.0f)
					spreadOffset = 0.25f;
				else if (ownerSpeed2D > 10.0f)
					spreadOffset = 0.1f;
				else if (m_client->GetEdict ()->IsDucking ())
					spreadOffset = 0.0f;
			}

			if (!m_client->IsUsingScope ())
				spreadOffset += 0.08f;

			return spreadOffset;
		}
};

class CurrentWeapon_MP5 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_MP5 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/mp5-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.64f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return true; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.2f * accuracy;

			return 0.04f * accuracy;
		}
};

class CurrentWeapon_M249 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_M249 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/m249-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.52f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return true; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.045f + 0.5f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 140.0f)
				return 0.045f + 0.095f * accuracy;

			return 0.03f * accuracy;
		}
};

class CurrentWeapon_M3 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_M3 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/m3-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.48f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return true; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }
};

class CurrentWeapon_M4A1 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_M4A1 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			if (IsSilenced ())
				HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/m4a1-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 1.4f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
			else
				HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/m4a1_unsil-1.wav"/* "weapons/m4a1_unsil-2.wav" */, HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.52f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsSilenced            (void) const
		{
			// Is current weapon silenced? Does only work on M4A1 and USP.

			// Reliability check.
			InternalAssert (IsValid ());

			return (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_M4A1_SILENCED) > 0u;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return true; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.035f + 0.4f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 140.0f)
				return 0.035f + 0.07f * accuracy;

			if (IsSilenced ())
				return 0.025f * accuracy;

			return 0.02f * accuracy;
		}
};

class CurrentWeapon_TMP : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_TMP (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/tmp-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 1.6f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return true; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.25f * accuracy;

			return 0.03f * accuracy;
		}
};

class CurrentWeapon_G3SG1 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_G3SG1 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/g3sg1-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.4f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return true; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			float spreadOffset (0.055f);

			if (!m_client->IsOnFloor ())
				spreadOffset = 0.45f;
			else if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
				spreadOffset = 0.15f;
			else if (m_client->GetEdict ()->IsDucking ())
				spreadOffset = 0.035f;

			if (!m_client->IsUsingScope ())
				spreadOffset += 0.025f;

			return spreadOffset;
		}
};

class CurrentWeapon_FlashBangGrenade : public CurrentWeapon
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_FlashBangGrenade (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon (client, edict)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsCanAttack           (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return GetNextAttackDelay () <= 0.0f;
		}

		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return true; }
		inline const bool                                                             IsC4                     (void) const { return false; }
};

class CurrentWeapon_Deagle : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Deagle (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/deagle-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.6f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return true; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (1.0f - GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 1.5f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 0.0f)
				return 0.25f * accuracy;

			if (m_client->GetEdict ()->IsDucking ())
				return 0.115f * accuracy;

			return 0.13f * accuracy;
		}
};

class CurrentWeapon_SG552 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_SG552 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/sg552-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.4f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return true; }
		inline const bool                                                             IsRifle                  (void) const { return true; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.035f + 0.45f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 140.0f)
				return 0.035f + 0.075f * accuracy;

			return 0.02f * accuracy;
		}
};

class CurrentWeapon_AK47 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_AK47 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/ak47-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.4f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return true; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.04f + 0.4f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 140.0f)
				return 0.04f + 0.07f * accuracy;

			return 0.0275f * accuracy;
		}
};

class CurrentWeapon_Knife : public CurrentWeapon
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_Knife (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon (client, edict)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const bool                                       IsCanAttack           (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return GetNextAttackDelay () <= 0.0f;
		}

		inline const bool                                                             IsKnife                  (void) const { return true; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return false; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }
};

class CurrentWeapon_P90 : public CurrentWeapon_Reloadable
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CurrentWeapon_P90 (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) :
			CurrentWeapon_Reloadable (client, edict)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline void SendAttackNoiseSound (void) const
		{
			HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (GetEdict (), HalfLifeEngine::SDK::Constants::SoundChannel_Weapon, "weapons/p90-1.wav", HalfLifeEngine::SDK::Constants::SoundVolume_Normal, 0.64f, HalfLifeEngine::SDK::Constants::SoundFlag_None, /* 94u + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 15u) */HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                                             IsKnife                  (void) const { return false; }
		inline const bool                                                             IsPistol                 (void) const { return false; }
		inline const bool                                                             IsShield                 (void) const { return false; }
		inline const bool                                                             IsShotgun                (void) const { return false; }
		inline const bool                                                             IsSubMachineGun          (void) const { return true; }
		inline const bool                                                             IsZoomableRifle          (void) const { return false; }
		inline const bool                                                             IsRifle                  (void) const { return false; }
		inline const bool                                                             IsSniperRifle            (void) const { return false; }
		inline const bool                                                             IsMachineGun             (void) const { return false; }
		inline const bool                                                             IsBadPrimary             (void) const { return false; }
		inline const bool                                                             IsGrenade                (void) const { return false; }
		inline const bool                                                             IsC4                     (void) const { return false; }

		inline const float GetSpreadOffset (void) const
		{
			const float accuracy (GetAccuracy ());

			if (!m_client->IsOnFloor ())
				return 0.3f * accuracy;

			if (m_client->GetEdict ()->GetSpeed2D () > 170.0f)
				return 0.115f * accuracy;

			return 0.045f * accuracy;
		}
};

class CurrentWeaponConstructorFunctionsContainer
{
	private:
		typedef CurrentWeapon *const (*const CurrentWeaponConstructorFunction_t) (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict);

		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_None             (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon                  (client, edict); }

		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_P228             (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_P228             (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Shield           (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Shield           (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Scout            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Scout            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_ExplosiveGrenade (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_ExplosiveGrenade (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_XM1014           (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_XM1014           (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_C4               (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_C4               (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_MAC10            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_MAC10            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Aug              (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Aug              (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_SmokeGrenade     (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_SmokeGrenade     (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Elite            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Elite            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_FiveSeven        (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_FiveSeven        (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_UMP45            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_UMP45            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_SG550            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_SG550            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Galil            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Galil            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Famas            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Famas            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_USP              (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_USP              (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Glock18          (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Glock18          (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_AWP              (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_AWP              (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_MP5              (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_MP5              (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_M249             (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_M249             (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_M3               (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_M3               (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_M4A1             (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_M4A1             (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_TMP              (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_TMP              (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_G3SG1            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_G3SG1            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_FlashBangGrenade (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_FlashBangGrenade (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Deagle           (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Deagle           (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_SG552            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_SG552            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_AK47             (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_AK47             (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_Knife            (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_Knife            (client, edict); }
		static inline CurrentWeapon *const ConstructorFunction_CurrentWeapon_P90              (const Client *const client, const HalfLifeEngine::SDK::Classes::Edict *const edict) { return new CurrentWeapon_P90              (client, edict); }

	public:
		static const CurrentWeaponConstructorFunction_t s_currentWeaponConstructorFunctions[HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u];
};

class OwningItemsManager
{
	friend Client;

	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <OwningItem *, unsigned char> ItemsArray_t;

	//
	// Group: Private members.
	//
	private:
		const Client *const m_client;	// Pointer to client/bot owning this items.

		ItemsArray_t        m_items[HalfLifeEngine::SDK::Constants::WeaponSlotID_Total];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline  OwningItemsManager (const Client *const client) : m_client (client) { /* VOID */ }
		inline ~OwningItemsManager (void) { RemoveAllItems (); }

	//
	// Group: Private operators.
	//
	private:
		inline OwningItemsManager &operator = (const OwningItemsManager &/*right*/);	// Avoid "warning C4512: 'OwningItemsManager' : assignment operator could not be generated".

	//
	// Group: Private functions.
	//
	private:
		inline void AddItem (OwningItem *const item)
		{
			// Reliability check.
			InternalAssert (item->IsValid ());

			m_items[HalfLifeEngine::SDK::Constants::weaponProperties[item->GetID ()].slotID] += item;
		}

		inline const bool IsMember (const HalfLifeEngine::SDK::Classes::Edict *const weapon) const
		{
			const ItemsArray_t &items (m_items[HalfLifeEngine::SDK::Constants::weaponProperties[weapon->GetWeaponID ()].slotID]);

			// Find weapon position....
			for (ItemsArray_t::IndexType_t itemIndex (0u); itemIndex < items.GetElementNumber (); ++itemIndex)
				if (items[itemIndex]->GetEdict () == weapon)
					return true;

			return false;
		}

	//
	// Group: Functions.
	//
	public:
		#if defined _DEBUG
			inline void PrintItems (void) const
			{
				const char *const slotName[HalfLifeEngine::SDK::Constants::WeaponSlotID_Total] =
				{
					"primary",
					"secondary",
					"knife",
					"grenades",
					"C4"
				};

				for (unsigned char itemSlotIndex (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons); itemSlotIndex < HalfLifeEngine::SDK::Constants::WeaponSlotID_Total; ++itemSlotIndex)
				{
					const ItemsArray_t &items (m_items[itemSlotIndex]);

					if (items.IsEmpty ())
					{
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Slot: %s, weapons number is 0:\n{\n}\n", slotName[itemSlotIndex]);

						continue;
					}

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Slot: %s, weapons number is %u:\n{\n", slotName[itemSlotIndex], items.GetElementNumber ());

					for (ItemsArray_t::IndexType_t itemIndex (0u); itemIndex < items.GetElementNumber (); ++itemIndex)
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\t\tweapon[%u]: \"%s\".\n", itemIndex, HalfLifeEngine::SDK::Constants::weaponProperties[items[itemIndex]->GetID ()].className.GetData ());

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("}\n");
				}
			}
		#endif	// if defined _DEBUG

		inline void RemoveAllItems (void)
		{
			for (unsigned char itemSlotIndex (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons); itemSlotIndex < HalfLifeEngine::SDK::Constants::WeaponSlotID_Total; ++itemSlotIndex)
				m_items[itemSlotIndex].DeleteAndPurgeAll ();
		}

		inline const ItemsArray_t *const GetItems (void)                                                        const { return m_items; }
		inline const ItemsArray_t       &GetItems (const HalfLifeEngine::SDK::Constants::WeaponSlotID_t slotID) const { return m_items[slotID]; }

		inline const bool                      HasPrimaryWeapon          (void) const { return GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons).IsEmpty () == false; }
		inline const ItemsArray_t::IndexType_t GetPrimaryWeaponsNumber   (void) const { return GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons).GetElementNumber (); }
		inline const HalfLifeEngine::SDK::Constants::WeaponID_t GetRandomPrimaryWeaponID  (void) const
		{
			// Reliability check.
			InternalAssert (HasPrimaryWeapon ());

			return GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons).GetRandomElement ()->GetID ();
		}

		inline const bool                      HasSecondaryWeapon        (void) const { return GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_SecondaryWeapons).IsEmpty () == false; }
		inline const ItemsArray_t::IndexType_t GetSecondaryWeaponsNumber (void) const { return GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_SecondaryWeapons).GetElementNumber (); }
		inline const HalfLifeEngine::SDK::Constants::WeaponID_t GetRandomSecondaryWeaponID  (void) const
		{
			// Reliability check.
			InternalAssert (HasSecondaryWeapon ());

			return GetItems (HalfLifeEngine::SDK::Constants::WeaponSlotID_SecondaryWeapons).GetRandomElement ()->GetID ();
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void AddPlayerItem (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Reliability checks.
			InternalAssert (weapon->IsValid ());
			InternalAssert (!IsMember (weapon));

			OwningItem *const newItem (new OwningItem (m_client, weapon));

			#if defined _DEBUG
				if (newItem == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new client \"%s\" owning weapon \"%s\"!", sizeof (OwningItem), m_client->GetName ().GetData (), weapon->GetClassName ().GetData ());

					return;
				}
			#endif	// if defined _DEBUG

			AddItem (newItem);
		}
		inline void RemovePlayerItem (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Reliability check.
			InternalAssert (weapon->IsValid ());

//			AddLogEntry (true, LogLevel_Default, true, "OwningItemsManager::RemovePlayerItem(): Called for player \"%s\", item entity is %s (entity index == %u).", m_client->GetName ().GetData (), weapon->GetClassName ().GetData (), weapon->GetIndex ());

			ItemsArray_t &items (m_items[HalfLifeEngine::SDK::Constants::weaponProperties[weapon->GetWeaponID ()].slotID]);

			// Find weapon position....
			for (ItemsArray_t::IndexType_t itemIndex (0u); itemIndex < items.GetElementNumber (); ++itemIndex)
				if (items[itemIndex]->GetEdict () == weapon)
				{
					items.Delete (itemIndex, 1u);

					// Done!
					return;
				}

			#if defined _DEBUG
				// Error.
/*
				PrintItems ();

				AddLogEntry (true, LogLevel_Error, true, "OwningItemsManager::RemovePlayerItem(): Player \"%s\" item entity %s was not found!", m_client->GetName ().GetData (), weapon->GetClassName ().GetData ());
*/				AddLogEntry (false, LogLevel_Critical, true, "OwningItemsManager::RemovePlayerItem(): Player \"%s\" item entity %s was not found!", m_client->GetName ().GetData (), weapon->GetClassName ().GetData ());
			#endif	// if defined _DEBUG
		}
};

#endif	// ifndef CURRENT_WEAPON_INCLUDED