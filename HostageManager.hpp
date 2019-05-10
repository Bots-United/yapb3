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
// HostageManager.hpp
//
// Class: HostageManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HOSTAGE_MANAGER_INCLUDED
#define HOSTAGE_MANAGER_INCLUDED

class HostageManager
{
	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <HalfLifeEngine::SDK::Classes::Edict *, unsigned char> HostagesArray;

	//
	// Group: Private members.
	//
	private:
		HostagesArray m_hostages;
		HostagesArray m_unusedAliveHostages;	// Just array of alive unused hostages in a map.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline HostageManager (void)
		{
			FindHostages ();	// Build the hostages.
		}

	//
	// Group: Private functions.
	//
	private:
		inline void FindHostages (void)
		{
			// Find all hostage entities in a map and add them....
			for
			(
				HalfLifeEngine::SDK::Classes::Edict *hostage (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "hostage_entity"));
				hostage->IsValid ();
				hostage = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (hostage, "hostage_entity")
			)
				AddHostage (hostage);
		}
		inline void RemoveAllHostages (void)
		{
			// Free all the memory allocated for all hostages....
			m_hostages.RemoveAll ();
			m_unusedAliveHostages.RemoveAll ();
		}
		inline void RebuildHostages (void)
		{
			// Free all the memory allocated for all hostages....
			RemoveAllHostages ();

			// Find all hostage entities in a map and add them....
			FindHostages ();
		}
		inline void AddHostage (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity)
		{
			// Reliability checks.
			InternalAssert (hostageEntity->IsValid ());
			InternalAssert (!m_hostages.IsMember (hostageEntity));

			m_hostages += hostageEntity;

			// Reliability checks.
			InternalAssert (hostageEntity->IsHostageAlive ())
			InternalAssert (!hostageEntity->IsHostageHasRescuer ())

			m_unusedAliveHostages += hostageEntity;
		}

	//
	// Group: Functions.
	//
	public:
		inline       HostagesArray &GetHostagesArray (void)
		{
			// This function returns a reference to non-constant hostages array.

			return m_hostages;
		}
		inline const HostagesArray &GetHostagesArray (void) const
		{
			// This function returns a reference to constant hostages array.

			return m_hostages;
		}
		inline       HostagesArray &GetUnusedAliveHostagesArray (void)
		{
			// This function returns a reference to non-constant unused alive hostages array.

			return m_unusedAliveHostages;
		}
		inline const HostagesArray &GetUnusedAliveHostagesArray (void) const
		{
			// This function returns a reference to constant unused alive hostages array.

			return m_unusedAliveHostages;
		}

		inline const HostagesArray::IndexType_t GetHostagesNumber            (void) const { return m_hostages.GetElementNumber (); }
		inline const HostagesArray::IndexType_t GetUnusedAliveHostagesNumber (void) const { return m_unusedAliveHostages.GetElementNumber (); }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void HostageSpawnPost (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity)
		{
			// A new hostage has spawned....
			AddHostage (hostageEntity);
		}
		inline void HostageKilled (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity, const Client *const killer);	// Note: This function declared in ClientManager.hpp.
		inline void HostageEscaped (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity) { m_unusedAliveHostages -= hostageEntity; }
		inline void HostageRescued (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity, Client *const rescuer);	// Note: This function declared in ClientManager.hpp.
		inline void HostageUsePost (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity, Client *const previousRescuer, Client *const currentRescuer);	// Note: This function declared in ClientManager.hpp.
		inline void AllHostagesRescued (void);	// Note: This function declared in ClientManager.hpp.
		inline void RoundStarted (void) { m_unusedAliveHostages = m_hostages; }
};

#endif	// ifndef HOSTAGE_MANAGER_INCLUDED