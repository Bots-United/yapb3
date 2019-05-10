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
// Task_RescueHostages.cpp
//
// Class: Task_RescueHostages
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPB::Task_RescueHostages::Execute (void)
{
	m_owner->m_aim->Think ();
	m_owner->m_chatterManager.Think (m_owner);

	// fight the enemy if we found one
	if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
	{
		// Reliability check.
		InternalAssert (m_owner->m_enemy->IsValid ());

		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_RescueHostages::Execute(): Find enemy \"%s\" - taking pause....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

		m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));

		return;
	}

	Math::Vector3D hostageOrigin;

//	if (m_currentHostageToRescue == NULL)
	if (!m_currentHostageToRescue->IsValid ())
	{
		const float searchRadiusSquared (MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::MaximumPlayerUseDistance) * MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::MaximumPlayerUseDistance));

		for (::HostageManager::HostagesArray::IndexType_t hostageIndex (0u); hostageIndex < g_server->GetHostageManager ().GetUnusedAliveHostagesNumber (); ++hostageIndex)
		{
			m_currentHostageToRescue = g_server->GetHostageManager ().GetUnusedAliveHostagesArray ()[hostageIndex];

			if (m_currentHostageToRescue->GetHostageOrigin ().GetDistanceSquared (m_owner->GetOrigin ()) > searchRadiusSquared)
			{
				m_currentHostageToRescue = NULL;

				continue;
			}

			// Reliability check.
			InternalAssert (m_currentHostageToRescue->IsHostageAlive () && !m_currentHostageToRescue->IsHostageHasRescuer ());

			// never pickup dead hostage
			// check if line of sight to object is not blocked (i.e. visible) AND if bot is not using grenade
			if (!m_owner->ItemIsVisible (hostageOrigin = m_currentHostageToRescue->GetHostageOrigin (), m_currentHostageToRescue, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters))
			{
				m_currentHostageToRescue = NULL;

				continue;
			}
/*
			const HalfLifeEngine::SDK::Classes::Edict *const hostageRescuerEdict (m_currentHostageToRescue->GetHostageRescuer ());

			if (hostageRescuerEdict != NULL)
			{
				Client *const hostageRescuer (g_server->GetClientManager ()->GetClient (hostageRescuerEdict));

				if (hostageRescuer == m_owner || (hostageRescuer->IsHuman () && hostageRescuer->GetHumanPointer ()->IsAwayFromKeyboard ()))
				{
					m_currentHostageToRescue = NULL;

					continue;
				}
			}
*/
			break;
		}

		if (m_currentHostageToRescue == NULL)
		{
			Complete ();

			return;
		}
	}
	else
		hostageOrigin = m_currentHostageToRescue->GetHostageOrigin ();

	if (m_owner->m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Vector)
		m_owner->m_aim->SetLookTask (new Aim::Task_Vector (hostageOrigin));
	else
		static_cast <Aim::Task_Vector *const> (m_owner->m_aim->GetLookTask ())->SetVector (hostageOrigin);

	m_owner->m_navigator->SetDestinationOrigin (hostageOrigin);
/*	if (!m_owner->m_navigator->SetMoveTo (hostageOrigin))
	{
		Fail ();

		return;
	}
*/
	// near to the hostage?
	if (hostageOrigin.GetDistanceSquared (m_owner->GetOrigin ()) > HalfLifeEngine::SDK::Constants::MaximumPlayerUseDistanceSquared)
	{
		m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_FORWARD;

		m_owner->m_navigator->Think ();

		return;
	}

	// faces hostage?
	if (m_owner->GetShootingConeDeviation (hostageOrigin) < HalfLifeEngine::SDK::Constants::VIEW_FIELD_NARROW)
		return;

	if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 80u)
		m_owner->ChatterMessage (::ChatterManager::ChatterMessage_UseHostage);

	m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_USE;	// use hostage here...
}