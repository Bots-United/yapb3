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
// C4.hpp
//
// Class: C4
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef C4_INCLUDED
#define C4_INCLUDED

class C4
{
	friend C4Manager;

	//
	// Group: Private members.
	//
	private:
		const HalfLifeEngine::SDK::Classes::Edict *const m_edict;
		float                                            m_plantedTime;	// Holds the time when bomb was planted.

	//
	// Group: (Con/De)structors.
	//
	private:
		inline C4 (const HalfLifeEngine::SDK::Classes::Edict *const c4) :
			m_edict (c4),	// Set c4 edict.

			m_plantedTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("C4::C4(): C4 \"%s\" spawned.\n", GetEdict ()->GetClassName ().GetData ());
		}

	//
	// Group: Private operators.
	//
	private:
		inline C4 &operator = (const C4 &/*right*/);	// Avoid "warning C4512: 'C4' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		//
		// Function: GetEdict
		//
		// Description: Gets the C4 entity pointer.
		//
		// Returns: C4 entity pointer.
		//
		inline const HalfLifeEngine::SDK::Classes::Edict *const GetEdict (void) const { return m_edict; }

		inline const bool IsPlanted (void) const
		{
			#if defined _DEBUG
				if (this == NULL)
					return false;

				// Reliability check.
				InternalAssert (GetEdict ()->IsValid ());

				return true;
			#else	// if defined _DEBUG
				return this != NULL;
			#endif	// if !defined _DEBUG
		}

		inline const Client *const  GetDefuser           (void) const;	// Note: This function declared in ClientManager.hpp.
		inline const float          GetPlantedTime       (void) const { return m_plantedTime; }
		inline const Math::Vector3D GetOrigin            (void) const { return GetEdict ()->GetBModelOrigin (); }
		inline const float          GetExplosionTime     (void) const { return GetEdict ()->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_C4_EXPLODE_TIME); }
		inline const float          GetExplosionTimeLeft (void) const { return GetExplosionTime () - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime (); }
		inline const bool           IsDefusing           (void) const { return GetEdict ()->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_C4_IsStartDefuse); }

		inline const float     GetExplosionDamage (void) const;	// Note: This function declared in Server.hpp.
		inline const float     GetExplosionRadius (void) const
		{
			const float explosionDamage (GetExplosionDamage ());

			return explosionDamage <= 80.0f ? /*! @warning I'M NOT SHURE ABOUT THIS CASE!!!!!!!!!!!!! */(explosionDamage + 3.0f) * g_randomNumberGenerator.GetValueBetween (0.5f, 1.5f) : explosionDamage * HalfLifeEngine::SDK::Constants::ExplosionDamageMultiplier;
		}
		inline const float     GetExplosionDamage (const Math::Vector3D &origin) const
		{
			Math::Vector3D bombOrigin (GetEdict ()->variables.origin);

			++bombOrigin.z;	// In case grenade is lying on the ground.

			const float radius (GetExplosionRadius ()), distance (bombOrigin.GetDistance (origin));

			if (distance > radius)
				return 0.0f;

			// Reliability check.
			InternalAssert (radius != 0.0f);

			const float explosionDamage (GetExplosionDamage ());
/*! ONLY FOR HE GRENADE, SO MOVE IT OUT OF HERE!!!
			if (GetEdict ()->variables.flags & DamageType_HEGrenade) && HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModID () == HalfLifeEngine::Engine::Version_ConditionZero)
			{
				if (explosionDamage == 0.0f)
					return 0.0f;

				const float a9 (radius - distance);

				// Decrease damage for an entity that's farther from the bomb.
				const float adjustedDamage (MATH_GET_SQUARED (a9) * 1.25f / MATH_GET_SQUARED (radius) * GetAmountOfPlayerVisible (origin, client) * explosionDamage * 1.5f);

				return adjustedDamage < 0.0f ? 0.0f : adjustedDamage;
			}
*/
			// Decrease damage for an entity that's farther from the bomb.
			const float adjustedDamage (explosionDamage - explosionDamage / radius * distance);

			return adjustedDamage < 0.0f ? 0.0f : adjustedDamage;
		}
		inline const float     GetExplosionDamage (const HalfLifeEngine::SDK::Structures::EntityVariables_t &clientVariables) const
		{
			// Blast's don't travel into or out of water....
/*! @todo CHECK ME!!! */			if (GetEdict ()->variables.waterLevel >= HalfLifeEngine::SDK::Constants::EntityWaterLevel_1/* && !GetEdict ()->IsOnFloor ()*/ || clientVariables.waterLevel == HalfLifeEngine::SDK::Constants::EntityWaterLevel_3)
				return 0.0f;

			return GetExplosionDamage (clientVariables.origin);
		}
		inline const float     GetElapsedTimePercent (void) const
		{
			return ((HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - GetPlantedTime ()) / Console::Server::variableManager.GetVariable (Console::Server::VariableID_C4Timer)->GetValue <float> ()) * 100.0f;
		}
		inline const float     GetAudibleRadius      (void) const
		{/*! OLD CODE!!!
			// temporally save elapsed time before explosion
			const float elapsedTimePercent (GetElapsedTimePercent ());

			// we hear bomb if length greater than desired radius (start the manual calculations)
			return elapsedTimePercent > 28.0f ? 1024.0f : elapsedTimePercent > 52.0f ? 1280.0f : elapsedTimePercent > 68.0f ? 2048.0f : elapsedTimePercent > 85.0f ? HalfLifeEngine::SDK::Constants::HalfMapSize : 768.0f;
*/
			// we hear bomb if distance greater than desired radius (start the manual calculations)
			return SOUND_ATTENUATION_TO_RADIUS (GetEdict ()->privateData->GetGrenadePointer ()->m_beepAttenuation);
		}
};

#endif	// ifndef C4_INCLUDED