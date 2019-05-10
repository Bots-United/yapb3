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
// ClientHostageManager.hpp
//
// Class: HostageManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CLIENT_HOSTAGE_MANAGER_INCLUDED
#define CLIENT_HOSTAGE_MANAGER_INCLUDED

class HostageManager
{
	//
	// Group: Private members.
	//
	private:
		::HostageManager::HostagesArray m_hostages;

	//
	// Group: Private functions.
	//
	private:
		inline void AddHostage    (HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			// Reliability checks.
			InternalAssert (hostage->IsValid ());
			InternalAssert (hostage->IsHostageValid ());
			InternalAssert (hostage->IsHostageAlive ());
			InternalAssert (!m_hostages.IsMember (hostage));	// OCCURS!!!!

			m_hostages += hostage;
		}
		inline void RemoveHostage     (HalfLifeEngine::SDK::Classes::Edict *const hostage) { m_hostages -= hostage; }
		inline void RemoveAllHostages (void)                                               { m_hostages.RemoveAll (); }

	//
	// Group: Functions.
	//
	public:
		inline       ::HostageManager::HostagesArray &GetHostagesArray (void)
		{
			// This function returns a reference to non-constant hostages array.

			return m_hostages;
		}
		inline const ::HostageManager::HostagesArray &GetHostagesArray (void) const
		{
			// This function returns a reference to constant hostages array.

			return m_hostages;
		}
		inline const ::HostageManager::HostagesArray::IndexType_t GetHostagesNumber (void) const { return m_hostages.GetElementNumber (); }
		inline const bool                                         IsEmpty           (void) const { return m_hostages.IsEmpty (); }

		#if defined _DEBUG
			inline void ShowHostages (void) const
			{
				for (::HostageManager::HostagesArray::IndexType_t index (0u); index < m_hostages.GetElementNumber (); ++index)
				{
					// Reliability check.
					InternalAssert (m_hostages[index]->IsHostageValid ());

					const HalfLifeEngine::SDK::Classes::Edict *const owner (m_hostages[index]->GetHostageRescuer ());

					// Reliability check.
					InternalAssert (owner->IsValid ());

					HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Hostage[%u]: owner: \"%s\", health=%i.\n", index, owner->GetNetName ().GetData (), m_hostages[index]->GetHealth ());
					HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (owner->variables.origin, m_hostages[index]->GetHostageOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
				}
			}
		#endif	// if defined _DEBUG

	//
	// Group: Engine callbacks.
	//
	public:
		inline void HostageKilled (HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const/* killer*/)
		{
			RemoveHostage (hostage);
		}
		inline void HostageRescued (HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			RemoveHostage (hostage);
		}
		inline void HostageUsePost (HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			AddHostage (hostage);
		}
		inline void HostageUnusePost (HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			RemoveHostage (hostage);
		}
		inline void AllHostagesRescued (void)
		{
			RemoveAllHostages ();
		}
		inline void RoundStarted (void)
		{
			RemoveAllHostages ();
		}
};

#endif	// ifndef CLIENT_HOSTAGE_MANAGER_INCLUDED