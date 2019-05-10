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
// Task_DoubleJump.cpp
//
// Class: Task_DoubleJump
//
// Description: Bot helps human player (or other bot) to get somewhere.
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPB::Task_DoubleJump::Execute (void)
{
	if (m_timeToComplete <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		Complete ();

		return;
	}

	// fight the enemy if we found one
	if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
	{
		// Reliability check.
		InternalAssert (m_owner->m_enemy->IsValid ());

		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_DoubleJump::Execute(): Find enemy \"%s\" - break task....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

		m_owner->m_scheduleManager.AddAction (new Task_AttackEnemy (m_owner));

		Complete ();

		return;
	}

	m_owner->m_aim->Think ();
	m_owner->m_chatterManager.Think (m_owner);

	if (m_owner->m_reloadState == ReloadState_None && m_owner->m_currentWeapon->GetAmmo () > 0u)
		m_owner->m_reloadState = ReloadState_Primary;

	m_owner->TryCheckReload ();

	if (m_owner->GetOrigin ().GetDistanceSquared (m_client->GetOrigin ()) > MATH_GET_SQUARED (150.0f))
		return;

	/// @TODO: Timer for change aiming slowly.
	m_owner->m_aim->m_lookAt = m_client->GetOrigin () + Math::Vector3D (g_randomNumberGenerator.GetValueBetween (m_client->GetEdict ()->variables.boundingBox.mins.x, m_client->GetEdict ()->variables.boundingBox.maxs.x), g_randomNumberGenerator.GetValueBetween (m_client->GetEdict ()->variables.boundingBox.mins.y, m_client->GetEdict ()->variables.boundingBox.maxs.y), g_randomNumberGenerator.GetValueBetween (m_client->GetEdict ()->variables.boundingBox.mins.z, m_client->GetEdict ()->variables.boundingBox.maxs.z));

	if (m_client->GetEdict ()->variables.groundEntity != *m_owner)
		m_owner->PressDuck ();
	else
	{
		m_owner->ReleaseDuck ();

		if (m_owner->IsOnFloor () && m_client->GetOrigin ().z - (m_owner->GetOrigin ().z + HalfLifeEngine::SDK::Constants::HalfHumanHeight) >= 80.0f)
			m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_JUMP;
	}
}