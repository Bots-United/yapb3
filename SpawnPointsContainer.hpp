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
// SpawnPointsContainer.hpp
//
// Class: SpawnPointsContainer
//
// Description: Contains the origins of all player spawn points on this map.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SPAWN_POINTS_CONTAINER_INCLUDED
#define SPAWN_POINTS_CONTAINER_INCLUDED

class SpawnPointsContainer
{
	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <Math::Vector3D, unsigned char> SpawnPointsArray;

	//
	// Group: Private members.
	//
	private:
		SpawnPointsArray m_spawnPoints[TeamArrayID_Total];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline SpawnPointsContainer (void)
		{
			HalfLifeEngine::SDK::Classes::Edict *spawnEntity (NULL);

			// Enumerate all spawn points on this map....

			while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_player_deathmatch"))->IsValid ())
				m_spawnPoints[TeamArrayID_Terrorist] += spawnEntity->variables.origin;

			while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_player_start"))->IsValid ())
				m_spawnPoints[TeamArrayID_CounterTerrorist] += spawnEntity->variables.origin;

			while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_vip_start"))->IsValid ())
				m_spawnPoints[TeamArrayID_CounterTerrorist] += spawnEntity->variables.origin;
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool          IsEmpty              (void) const { return m_spawnPoints[TeamArrayID_Terrorist].IsEmpty () && m_spawnPoints[TeamArrayID_CounterTerrorist].IsEmpty (); }
		inline const unsigned char GetSpawnPointsNumber (void) const { return m_spawnPoints[TeamArrayID_Terrorist].GetElementNumber () + m_spawnPoints[TeamArrayID_CounterTerrorist].GetElementNumber (); }

	//
	// Group: Private operators.
	//
	private:
		inline SpawnPointsContainer &operator = (const SpawnPointsContainer &/*right*/);	// Avoid "warning C4512: 'SpawnPointsContainer' : assignment operator could not be generated".

	//
	// Group: Operators.
	//
	public:
		inline const SpawnPointsArray &operator [] (const TeamArrayID_t teamArrayID) const
		{
			// Reliability check.
			InternalAssert (teamArrayID == TeamArrayID_Terrorist || teamArrayID == TeamArrayID_CounterTerrorist);

			return m_spawnPoints[teamArrayID];
		}
		inline const SpawnPointsArray &operator [] (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (teamID == HalfLifeEngine::SDK::Constants::TeamID_Terrorist || teamID == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);

			return m_spawnPoints[teamID - 1u];
		}
};

#endif	// ifndef SPAWN_POINTS_CONTAINER_INCLUDED