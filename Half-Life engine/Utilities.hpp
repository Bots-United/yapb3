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
// Utilities.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_UTILITIES_INCLUDED
#define HALF_LIFE_ENGINE_UTILITIES_INCLUDED

/*
=============================================================================

LIGHT SAMPLING

=============================================================================
*/
namespace Light
{
//extern const SDK::Structures::MPlane_t *lightplane;
//extern Math::Vector3D lightspot;
extern Color <unsigned int> g_pointColor;

template <typename nodeType, typename surfaceType> const bool RecursiveLightPoint (const int *const d_lightstylevalue, const SDK::Structures::Model_t *const worldmodel, const nodeType *const node, const Math::Vector3D &start, const Math::Vector3D &end);
const bool ActualRecursiveLightPoint (const int *const d_lightstylevalue, const SDK::Structures::Model_t *const worldmodel, const Math::Vector3D &start, const Math::Vector3D &end);

inline const unsigned char R_LightPoint (const int *const d_lightstylevalue, const SDK::Structures::Model_t *const worldmodel, const Math::Vector3D &p)
{
	if (worldmodel->lightdata == NULL)
		return 255u;

	Math::Vector3D end (p);

	end.z -= 2048.0f;

	return ActualRecursiveLightPoint (d_lightstylevalue, worldmodel, p, end) == false ?
		0u :
		static_cast <unsigned char> (g_pointColor.GetAvg ());
}
}

/// @note This function declared in 'Half-Life engine/Core.hpp'.
inline const bool IsInShootCone (const Math::Vector3D &origin, const SDK::Classes::Edict *const edict, const float distance, const float radius, const float minAngle);

inline const SDK::Constants::WeaponID_t GetWeaponIDFromAlias (const DynamicString &alias)
{
	for (unsigned char index (SDK::Constants::WeaponID_P228); index < SDK::Constants::MaximumWeapons - 1u; ++index)
		if (SDK::Constants::weaponProperties[index].buyAlias1 == alias)
			return static_cast <SDK::Constants::WeaponID_t> (index);

	return SDK::Constants::WeaponID_None;
}

// some conversion stuff
inline const short          GetFixedSigned16   (const float value, const unsigned char scale)
{
	return static_cast <short> (Math::GetClampedValueInRange (static_cast <int> (value * BIT (scale)), -32768, 32767));
}
inline const unsigned short GetFixedUnsigned16 (const float value, const unsigned char scale)
{
	return static_cast <unsigned short> (Math::GetClampedValueInRange (static_cast <int> (value * BIT (scale)), 0, 65535));
}

inline bool IsInWorld (const SDK::Structures::EntityVariables_t &entityVariables)
{
	// check position
	if (entityVariables.origin.x >= SDK::Constants::HalfMapSize)
		return false;

	if (entityVariables.origin.y >= SDK::Constants::HalfMapSize)
		return false;

	if (entityVariables.origin.z >= SDK::Constants::HalfMapSize)
		return false;

	if (entityVariables.origin.x <= -SDK::Constants::HalfMapSize)
		return false;

	if (entityVariables.origin.y <= -SDK::Constants::HalfMapSize)
		return false;

	if (entityVariables.origin.z <= -SDK::Constants::HalfMapSize)
		return false;

	// check speed
	if (entityVariables.velocity.x >= 2000.0f)
		return false;

	if (entityVariables.velocity.y >= 2000.0f)
		return false;

	if (entityVariables.velocity.z >= 2000.0f)
		return false;

	if (entityVariables.velocity.x <= -2000.0f)
		return false;

	if (entityVariables.velocity.y <= -2000.0f)
		return false;

	if (entityVariables.velocity.z <= -2000.0f)
		return false;

	return true;
}

/// @note This function declared in 'Half-Life engine/Core.hpp'.
inline SDK::Classes::Edict *const GetEdictFromAreaLink (const SDK::Structures::Link_t *const link);

/// @note The following 2 functions are declared in 'Half-Life engine/Core.hpp'.
inline const bool IsPlayerEdictIndex (const unsigned short index);
inline const bool IsNotWorldspawnPlayerEdictIndex (const unsigned short index);

inline const bool IsSpectatorTeam/*IsObserverTeam*/ (const SDK::Constants::TeamID_t teamID)
{
	switch (teamID)
	{
		case SDK::Constants::TeamID_Unassigned:
		case SDK::Constants::TeamID_Spectator:
			return true;

		default:
			return false;
	}
}

inline const bool WeaponIsKnife (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is knife, false otherwise.

	return id == SDK::Constants::WeaponID_Knife;
}

inline const bool WeaponIsPistol (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is pistol, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_Glock18:
		case SDK::Constants::WeaponID_USP:
		case SDK::Constants::WeaponID_P228:
		case SDK::Constants::WeaponID_Deagle:
		case SDK::Constants::WeaponID_Elite:
		case SDK::Constants::WeaponID_FiveSeven:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_Pistols) > 0u;
}

inline const bool WeaponIsSecondary (const SDK::Constants::WeaponID_t id)
{
	return WeaponIsPistol (id);
}

inline const bool WeaponIsShield (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is shield, false otherwise.

	return id == SDK::Constants::WeaponID_Shield;
}

inline const bool WeaponIsShotgun (const SDK::Constants::WeaponID_t id)
{
	// Returns if the specified weapon is shotgun, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_M3:
		case SDK::Constants::WeaponID_XM1014:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_Shotguns) > 0u;
}

inline const bool WeaponIsSubMachineGun (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is sub machine gun, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_MAC10:
		case SDK::Constants::WeaponID_TMP:
		case SDK::Constants::WeaponID_MP5:
		case SDK::Constants::WeaponID_UMP45:
		case SDK::Constants::WeaponID_P90:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_SubMachineGuns) > 0u;
}

inline const bool WeaponIsZoomableRifle (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is zoomable rifle, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_SG552:
		case SDK::Constants::WeaponID_Aug:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_ZoomableRifles) > 0u;
}

inline const bool WeaponIsRifle (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is rifle, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_Galil:
		case SDK::Constants::WeaponID_Famas:
		case SDK::Constants::WeaponID_AK47:
		case SDK::Constants::WeaponID_M4A1:
		case SDK::Constants::WeaponID_SG552:
		case SDK::Constants::WeaponID_Aug:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_Rifles) > 0u;
}

inline const bool WeaponIsSniperRifle (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is sniper rifle, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_Scout:
		case SDK::Constants::WeaponID_AWP:
		case SDK::Constants::WeaponID_SG550:
		case SDK::Constants::WeaponID_G3SG1:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_SniperRifles) > 0u;
}

inline const bool WeaponIsMachineGun (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is machine gun, false otherwise.

	return id == SDK::Constants::WeaponID_M249;
}

inline const bool WeaponIsPrimary (const SDK::Constants::WeaponID_t id)
{
	// Returns true if the specified weapon is primary weapon, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_M3:
		case SDK::Constants::WeaponID_XM1014:

		case SDK::Constants::WeaponID_MAC10:
		case SDK::Constants::WeaponID_TMP:
		case SDK::Constants::WeaponID_MP5:
		case SDK::Constants::WeaponID_UMP45:
		case SDK::Constants::WeaponID_P90:

		case SDK::Constants::WeaponID_Galil:
		case SDK::Constants::WeaponID_Famas:
		case SDK::Constants::WeaponID_AK47:
		case SDK::Constants::WeaponID_M4A1:
		case SDK::Constants::WeaponID_SG552:
		case SDK::Constants::WeaponID_Aug:

		case SDK::Constants::WeaponID_Scout:
		case SDK::Constants::WeaponID_SG550:
		case SDK::Constants::WeaponID_AWP:
		case SDK::Constants::WeaponID_G3SG1:

		case SDK::Constants::WeaponID_M249:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_Primary) > 0u;
}

inline const bool WeaponIsGrenade (const SDK::Constants::WeaponID_t id)
{
	// Returns if the specified weapon is grenade, false otherwise.
/*
	switch (id)
	{
		case SDK::Constants::WeaponID_ExplosiveGrenade:
		case SDK::Constants::WeaponID_SmokeGrenade:
		case SDK::Constants::WeaponID_FlashBangGrenade:
			return true;

		default:
			return false;
	}
*/
	return (BIT (id) & SDK::Constants::WeaponBits_Grenades) > 0u;
}

inline const bool WeaponIsC4 (const SDK::Constants::WeaponID_t id)
{
	// Returns if the specified weapon is C4, false otherwise.

	return id == SDK::Constants::WeaponID_C4;
}

inline void DropPunchAngles (Math::Angles3D &punchAngles, const float frameTime)
{
	float length (punchAngles.GetLength ());

	if (length > 0.0f)
	{
		punchAngles /= length;	// Normalize.

		length -= (10.0f + length * 0.5f) * frameTime;

		if (length > 0.0f)
		{
			punchAngles *= length;

			return;
		}
	}

	punchAngles = Math::Angles3D::ZeroValue;
}

template <typename indexType> inline const indexType GetDrawStringLength (const char *string)
{
	indexType length (0u);

	while (*string != '\0')
	{
		length += SDK::Constants::charactersWidths[*string];

		++string;
	}

	return length;
}
template <typename indexType> inline const indexType GetDrawStringLength (const char *string, const indexType count)
{
	indexType length (0u);
	const char *const endPointer (string + count);

	// Reliability checks.
	InternalAssert (endPointer != NULL);
	InternalAssert (GetStringLength <indexType> (string) >= count);

	while (string < endPointer)
	{
		length += SDK::Constants::charactersWidths[*string];

		++string;
	}

	return length;
}
template <typename indexType> inline const float GetNormalizedDrawStringLength (const char *const string, const unsigned short screenWidth)
{
	return GetDrawStringLength <indexType> (string) / static_cast <const float> (screenWidth);
}

class SharedRandomNumberGenerator
{
	private:
		unsigned int m_glSeed;

		enum
		{
			MaximumSeedTableSize = 256u,
			MaximumSeedTableMask = MaximumSeedTableSize - 1u
		};
		static const unsigned int sm_seed_table[MaximumSeedTableSize];

	public:
		inline SharedRandomNumberGenerator (void) : m_glSeed (0u) { /* VOID */ }

	private:
		inline const unsigned int U_Random (void)
		{ 
			m_glSeed *= 69069u;
			m_glSeed += sm_seed_table[m_glSeed & MaximumSeedTableMask];

			return ++m_glSeed & 0x0FFFFFFF;
		}

		inline void U_Srand (const unsigned int seed) { m_glSeed = sm_seed_table[seed & MaximumSeedTableMask]; }

	public:
		inline const int UTIL_SharedRandomLong (const unsigned int seed, const int low, const int high)
		{
			const unsigned int range (high - low + 1u);

			if (range - 1u == 0u)
				return low;

			U_Srand (static_cast <int> (seed) + low + high);

			const int rnum (U_Random ());
			const int offset (rnum % range);

			return low + offset;
		}

		inline const float UTIL_SharedRandomFloat (const unsigned int seed, const float low, const float high)
		{
			const unsigned int range (static_cast <const unsigned int> (high - low));

			if (range == 0u)
				return low;

			U_Srand (static_cast <int> (seed) + *reinterpret_cast <const int *> (&low) + *reinterpret_cast <const int *> (&high));

			U_Random ();
			U_Random ();

			const int tensixrand (U_Random () & 65535u);
			const float offset (tensixrand / 65536.0f);

			return low + offset * range;
		}
};
extern SharedRandomNumberGenerator g_sharedRandomNumberGenerator;

/// @note This is derived from bot move code for a bot on ground and not in water.
///{
inline const float getStopDistDetailed (const float mVel_Initial, const float T/*timestep*/, const float StopSpeed/* sv_stopspeed */, const float Friction/* sv_friction */)
{
	// On a stop command, ie the player releasing the speed button or the bot setting UE_mVel to zero;
	// Assuming a constant Timestep T, straight move and computing distance as distancen = distancen-1+ T* Veln like the engine does,
	// one can "integrate" velocity piece-wise; this gives a formula for stopping distance as a function of the initial velocity modulus mVel_Initial and Timestep T.
	// Returns a value applying when a moving bot/player sets the speed to zero.

	float NumStopSpeed = mVel_Initial;
	float StopDist = 0.0f;

	if (mVel_Initial > StopSpeed)
	{
		const float K = 1.0f - Friction * T;
		//_ exponential speed decay portion
		float z = Math::logf (StopSpeed / mVel_Initial) / Math::logf (K);
		z = Math::ceilf (z);
		const int N1 = (int)z;
		NumStopSpeed *= Math::powf (K, static_cast <float> (N1));	// last reached speed value above Stopspeed
		StopDist = (mVel_Initial - NumStopSpeed) * (1.0f / Friction - T);
	}

	//_ constant speed decay portion
	const float z = NumStopSpeed / (Friction * StopSpeed * T);
	const int N2 = (int)z;	// corresponding to last reached speed value above zero

	if (N2 > 0)
		StopDist += N2 * T * (NumStopSpeed - 0.5f * T * (1 + N2) * Friction * StopSpeed);

	return StopDist;
}
inline const float getStopDist (const float mVel_Initial, const float T/*timestep*/, const float StopSpeed/* sv_stopspeed */, const float Friction/* sv_friction */)
{
	// This piece of code correspond to exact computation; in particular, the curve of StopDist as a function of mVel_Initial is a bit "jumpy",
	// due to quantization for N1 and N2. For practical purpose, a far simpler computation which returns a very good match and is continuous (ie one can compute the reciprocal function) is the following:

	if (mVel_Initial > StopSpeed)
		return (mVel_Initial - 0.5f * StopSpeed) * (1.0f / Friction - T);

	return Math::GetMaximumValueBetween (0.0f, 0.5f * mVel_Initial * (mVel_Initial / (StopSpeed * Friction) - T));
}

inline const float getEmergStopDistDetailed (const float mVel_Initial, const float OppositeVelCmd/* must be negative */, const float T/*timestep*/, const float StopSpeed/* sv_stopspeed */, const float Friction/* sv_friction */, const float AccelCoeff/* sv_accelerate */)
{
	// Another faster way to stop - referred to as "emergency stop" herunder- is for a player to depress the "backwards" button ie
	// commanding a velocity in a direction opposite to the current velocity ; it should ideally be depressed until speed is zero then released
	// to avoid moving back; players obviously cannot do it and depress a little bit before, or a bit after when they can see that a backwards move is initiated.
	// This can also be done for a bot, with the advantage that it can be more accurate.
	// emergStopDist() returns the distance at which actual speed changes sign, ie the max distance value reached before starting to move the opposite way.

	float NumStopSpeed = mVel_Initial;
	float StopDist = 0.0f;
	const float Decel = Friction * StopSpeed - AccelCoeff * OppositeVelCmd;

	if (mVel_Initial > StopSpeed)
	{
		float Coeff;
		const float K = 1.0f - Friction * T;

		const float EquivSpeed = OppositeVelCmd * AccelCoeff / Friction;
		const float Ratio = (StopSpeed - EquivSpeed) / (mVel_Initial - EquivSpeed);
		float N1 = Math::logf (Ratio) / Math::logf (K);
		N1 = Math::ceilf (N1);
		N1 = static_cast <float> (static_cast <int> (N1));
		Coeff = Math::powf (K, N1);
		NumStopSpeed = Coeff * (mVel_Initial - EquivSpeed) + EquivSpeed;
		StopDist = (mVel_Initial - NumStopSpeed) * (1.0f / Friction - T);
		StopDist += N1 * T * EquivSpeed;
	}

	const int N2 = (int)(NumStopSpeed / (Decel * T));

	if (N2 > 0)
		StopDist += N2 * T * (NumStopSpeed - 0.5f * T * (1 + N2) * Decel);

	return Math::GetMaximumValueBetween (0.0f, StopDist);
}
inline const float getEmergStopDist (const float mVel_Initial, const float OppositeVelCmd/* must be negative */, const float T/*timestep*/, const float StopSpeed/* sv_stopspeed */, const float Friction/* sv_friction */, const float AccelCoeff/* sv_accelerate */)
{
	// Approximate, but continuous version:

	const float Decel = Friction * StopSpeed - AccelCoeff * OppositeVelCmd;
	float StopDist;

	if (mVel_Initial > StopSpeed)
	{
		StopDist = (mVel_Initial - StopSpeed) * (1.0f / Friction - T);

		const float K = 1.0f - Friction * T;
		const float EquivSpeed = OppositeVelCmd * AccelCoeff / Friction;
		const float Ratio = (StopSpeed - EquivSpeed) / (mVel_Initial - EquivSpeed);
		const float N1 = Math::logf (Ratio) / Math::logf (K);

		StopDist += N1 * T * EquivSpeed;
		StopDist += 0.5f * StopSpeed * (-T + StopSpeed / Decel);
	}
	else
		StopDist = 0.5f * mVel_Initial * (-T + mVel_Initial / Decel);

	return Math::GetMaximumValueBetween (0.0f, StopDist);
}
///}
/*! @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
inline const float GetExplosionDamage ()
{
	
}*/
#if 0	//! @TODO: ADAPT THIS FUNCTION FOR THE BOT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// RadiusDamage - this entity is exploding, or otherwise needs to inflict damage upon entities within a certain range.
//
// only damage ents that can clearly be seen by the explosion!
void RadiusDamage( Math::Vector3D vecSrc, Structures::EntityVariables_t *pevInflictor, Structures::EntityVariables_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, SDK::Constants::DamageType_t bitsDamageType )
{
	BaseEntity *pEntity = NULL;
	TraceResult	tr;
	float		flAdjustedDamage, falloff;
	Math::Vector3D		vecSpot;

	if ( flRadius )
		falloff = flDamage / flRadius;
	else
		falloff = 1.0;

	int bInWater = (Globals::g_halfLifeEngine->GetPointContents (vecSrc) == SDK::Constants::Content_Water);

	++vecSrc.z;	// in case grenade is lying on the ground

	if ( !pevAttacker )
		pevAttacker = pevInflictor;

	// iterate on all entities in the vicinity.
	while ((pEntity = Globals::g_halfLifeEngine->FindEntityInSphere (pEntity, vecSrc, flRadius )) != NULL)
	{
		if ( pEntity->pev->takedamage != DAMAGE_NO )
		{
			// UNDONE: this should check a damage mask, not an ignore
			if ( iClassIgnore != CLASS_NONE && pEntity->Classify() == iClassIgnore )
			{// houndeyes don't hurt other houndeyes with their attack
				continue;
			}

			// blast's don't tavel into or out of water
			if (bInWater && pEntity->pev->waterLevel == 0)
				continue;
			if (!bInWater && pEntity->pev->waterLevel == 3)
				continue;

			vecSpot = pEntity->BodyTarget( vecSrc );

			UTIL_TraceLine ( vecSrc, vecSpot, dont_ignore_monsters, ENT(pevInflictor), &tr );

			if ( tr.flFraction == 1.0 || tr.pHit == pEntity->edict() )
			{// the explosion can 'see' this entity, so hurt them!
				if (tr.fStartSolid)
				{
					// if we're stuck inside them, fixup the position and distance
					tr.vecEndPos = vecSrc;
					tr.flFraction = 0.0;
				}

				// decrease damage for an ent that's farther from the bomb.
				flAdjustedDamage = vecSrc.GetDistance (tr.vecEndPos) * falloff;
				flAdjustedDamage = flDamage - flAdjustedDamage;

				if ( flAdjustedDamage < 0 )
				{
					flAdjustedDamage = 0;
				}

				// ALERT( at_console, "hit %s\n", STRING( pEntity->pev->classname ) );
				if (tr.flFraction != 1.0)
				{
					ClearMultiDamage( );
					pEntity->TraceAttack( pevInflictor, flAdjustedDamage, (tr.vecEndPos - vecSrc).Normalize( ), &tr, bitsDamageType );
					ApplyMultiDamage( pevInflictor, pevAttacker );
				}
				else
				{
					pEntity->TakeDamage ( pevInflictor, pevAttacker, flAdjustedDamage, bitsDamageType );
				}
			}
		}
	}
}

{
enum _t
{
	BONE_HIT_HEAD = 8,
	BONE_HIT_CHEST = 6,
	BONE_HIT_STOMACH = 4,
	BONE_HIT_LEFTARM = 24,
	BONE_HIT_RIGHTARM = 39,
	BONE_HIT_LEFTLEG = 48,
	BONE_HIT_RIGHTLEG = 54,
	HEAD_NECK = 40,
	BONE_L_BUTT = 41,
	BONE_R_BUTT = 42
};

stock get_bone_hitgroup(number)
{
	// This gets the hitgroup of the bone.

	switch (number)
	{
		case HEAD_NECK:
			return HIT_HEAD;

		case BONE_L_BUTT:
			return HIT_LEFTLEG;

		case BONE_R_BUTT:
			return HIT_RIGHTLEG;
	}

	if (1 <= number <= BONE_HIT_STOMACH)
		return HIT_STOMACH;

	if (BONE_HIT_STOMACH < number <= BONE_HIT_CHEST)
		return HIT_CHEST;

	if (BONE_HIT_CHEST < number <= BONE_HIT_HEAD)
		return HIT_HEAD;

	if (BONE_HIT_HEAD < number <= BONE_HIT_LEFTARM)
		return HIT_LEFTARM;

	if (BONE_HIT_LEFTARM < number <= BONE_HIT_RIGHTARM)
		return HIT_RIGHTARM;

	if (BONE_HIT_RIGHTARM < number <= BONE_HIT_LEFTLEG)
		return HIT_LEFTLEG;

	if (BONE_HIT_LEFTLEG < number <= BONE_HIT_RIGHTLEG)
		return HIT_RIGHTLEG;

	return HIT_GENERIC;
}
}

{
enum { DISTANCE_CLEAR_HIT = 2u };

stock find_closest_bone_to_gunshot(victim, Float:endtrace[3])
{
	new Float:angles[3], Float:origin[3], Float:dist = 9999999.99, Float:curorigin[3], bone_nr;
	for (new i=1;i<=54;i++)
	{
		// Get the bone position
		engfunc(EngFunc_GetBonePosition, victim, i, curorigin, angles);
		// Calculate the distance vector
		xs_vec_sub(curorigin, endtrace, angles);

		// If this is smaller than the last small distance remember the value!
		if (xs_vec_len(angles) <= dist)
		{
			origin = curorigin;
			dist = xs_vec_len(angles);
			bone_nr = i;
		}

		// If distance is smaller than CLEARHIT! Break (We accept the last value!)
		if (dist <= DISTANCE_CLEAR_HIT)
			break;
	}

	// Return the bone
	return bone_nr;
}
}
#endif	// if 0
inline void R_StudioPlayerBlend (const SDK::Structures::mstudioseqdesc_t &seqdesc, int &blend, float &pitch)
{
	// Player specific data
	// Determine pitch and blending amounts for players

	// calc up/down pointing
	blend = static_cast <int> (pitch * 3.0f);

	if (blend < seqdesc.blendstart[0u])
	{
		pitch -= seqdesc.blendstart[0u] * (1.0f / 3.0f);

		blend = 0;
	}
	else if (blend > seqdesc.blendend[0u])
	{
		pitch -= seqdesc.blendend[0u] * (1.0f / 3.0f);

		blend = 255;
	}
	else
	{
		const float blendDelta (seqdesc.blendend[0u] - seqdesc.blendstart[0u]);

		if (blendDelta < 0.1f)	// catch qc error
			blend = 127;
		else
			blend = static_cast <int> (255.0f * (blend - seqdesc.blendstart[0u]) / blendDelta);

		pitch = 0.0f;
	}
}

/*
=============================================================================

						PARSING STUFF

=============================================================================
*/

extern char com_token[1024u];
extern bool s_com_token_unget;
extern bool com_ignorecolons;	// YWB: Ignore colons as token separators in COM_Parse

inline void COM_UngetToken (void)
{
	s_com_token_unget = true;
}

/*
==============
COM_Parse

  Parse a token out of a string (into global: char com_token[1024])
==============
*/
extern char *const COM_Parse (char *data);

#endif	// ifndef HALF_LIFE_ENGINE_UTILITIES_INCLUDED