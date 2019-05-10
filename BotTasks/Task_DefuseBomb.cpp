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
// Task_DefuseBomb.cpp
//
// Class: Task_DefuseBomb
//
// Description: Bomb defusing behaviour.
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPB::Task_DefuseBomb::Execute (void)
{
//	AddLogEntry (true, LogLevel_Default, false, "Task_DefuseBomb::Execute() called for bot %s.", m_owner->GetName ().GetData ());

	m_owner->m_aim->Think ();
	m_owner->m_chatterManager.Think (m_owner);

	// fight the enemy if we found one
	if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
	{
		// Reliability check.
		InternalAssert (m_owner->m_enemy->IsValid ());

		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_DefuseBomb::Execute(): Find enemy \"%s\" - taking pause....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

		m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));

		return;
	}

	const float timeToBlowUp (m_timeToBlowUp - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());

	// exception: bomb has been defused
	if (!g_server->GetC4Manager ().IsÑ4Planted ())
	{
		if (g_server->IsTeamplay () && g_server->GetClientManager ()->GetAliveClientsNumber (m_owner->GetRealTeam ()) > 0u && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 70u)
		{
			if (timeToBlowUp <= 2.0f)
			{
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						m_owner->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_SectorClear);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						m_owner->ChatterMessage (::ChatterManager::ChatterMessage_BarelyDefused);

						break;
				}
			}
			else switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
			{
				case Console::Bot::VariableValue_Communication_Radio:
					m_owner->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_SectorClear);

					break;

				case Console::Bot::VariableValue_Communication_Voice:
					m_owner->ChatterMessage (::ChatterManager::ChatterMessage_BombSiteSecure);

					break;
			}
		}

		// finish task.

//		m_owner->m_checkTerrain = m_owner->m_moveToGoal = true;

		m_owner->m_navigator->m_destinationOrigin = /*m_owner->m_entityOrigin = */Math::Vector3D::ZeroValue;

		Complete ();

		return;
	}

	const Client *const c4Defuser (g_server->GetClientManager ()->GetC4Defuser ());

	// Someone already start defusing....
	if (c4Defuser != NULL && m_owner != c4Defuser)
	{
		// Finish task.

//		m_owner->m_checkTerrain = m_owner->m_moveToGoal = true;

		m_owner->m_navigator->m_destinationOrigin = /*m_owner->m_entityOrigin = */Math::Vector3D::ZeroValue;

		Complete ();

		return;
	}

	const Math::Vector3D bombOrigin (g_server->GetC4Manager ().GetC4 ()->GetOrigin ());
	const unsigned char fullDefuseTime (HalfLifeEngine::SDK::Constants::BombDefuseTime[m_owner->IsHasDefuseKit ()]);
	bool isExceptionCaught (false);

	if (m_owner == c4Defuser)
	{
		// Is just started defusing?
		if (m_defuseEndTime == 0.0f)
		{
			m_defuseEndTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + fullDefuseTime;

			// notify team of defusing
			if (g_server->IsTeamplay () && g_server->GetClientManager ()->GetAliveClientsNumber (m_owner->GetRealTeam ()) > 0u && m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 256.0f) == 0u)
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						m_owner->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						m_owner->ChatterMessage (::ChatterManager::ChatterMessage_DefusingC4);

						break;
				}
		}
	}
	else
	{
		// bot is reloading AND we close enough to start defusing...
		if (m_owner->m_currentWeapon->IsInReload () && bombOrigin.GetDistanceSquared2D (m_owner->GetOrigin ()) <= MATH_GET_SQUARED (100.0f) && timeToBlowUp <= fullDefuseTime + m_owner->m_currentWeapon->GetNextAttackDelay () && (m_owner->GetNearbyEnemiesNearPosition (m_owner->GetOrigin (), 500.0f) == 0u || m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 200.0f) == 0u))
			m_owner->ExecuteCommand ("lastinv; lastinv");	// Stop the reloading....

		m_defuseEndTime = 0.0f;
	}

	const float defuseRemainingTime (m_defuseEndTime == 0.0f ? fullDefuseTime : m_defuseEndTime - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());

	if (defuseRemainingTime > timeToBlowUp)	// exception: not time left for defusing
		isExceptionCaught = true;
/*	else if (m_owner->m_states & State_SeeingEnemy)	// exception: saw/seeing enemy
	{
		if (m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 128.0f) == 0u)
		{
			if (defuseRemainingTime > 0.75f)
			{
				if (m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 128.0f) > 0u)
					m_owner->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup);

				isExceptionCaught = true;
			}
		}
		else if (timeToBlowUp > fullDefuseTime + 3.0f && defuseRemainingTime > 1.0f)
			isExceptionCaught = true;
	}
	else if (m_owner->m_states & State_SuspectEnemy)	// exception: suspect enemy
	{
		if (m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 128.0f) == 0u && timeToBlowUp > fullDefuseTime + 10.0f)
		{
			if (m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 128.0f) > 0u)
				m_owner->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup);

			isExceptionCaught = true;
		}
	}
*/
	// one of exceptions is thrown. finish task.
	if (isExceptionCaught)
	{
//		m_owner->m_checkTerrain = m_owner->m_moveToGoal = true;

		m_owner->m_navigator->m_destinationOrigin = /*m_owner->m_entityOrigin = */Math::Vector3D::ZeroValue;

		Complete ();

		return;
	}

	m_owner->m_navigator->m_destinationOrigin = /*m_owner->m_entityOrigin = */bombOrigin;

	if (m_owner->m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Vector)
		m_owner->m_aim->SetLookTask (new Aim::Task_Vector (bombOrigin));
	else
		static_cast <Aim::Task_Vector *const> (m_owner->m_aim->GetLookTask ())->SetVector (bombOrigin);

	// head to bomb and press use button
//	m_owner->m_aimFlags |= AimPos_Entity;

	// near to the bomb?
	if (bombOrigin.GetDistanceSquared (m_owner->GetOrigin ()) > HalfLifeEngine::SDK::Constants::MaximumPlayerUseDistanceSquared)
	{
		m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_FORWARD;

		m_owner->m_navigator->Think ();

		return;
	}

	// no one of exceptions is thrown AND faces bomb?
	if (m_owner->GetShootingConeDeviation (bombOrigin) < HalfLifeEngine::SDK::Constants::VIEW_FIELD_NARROW || !m_owner->IsOnFloor ())
		return;

	if (m_duckDefuseCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		if (m_owner->m_profile->skill >= BotSkillLevel_Advanced && m_owner->IsHasEnemies ())
			m_isDuckDefuse = true;

		Math::Vector3D botDuckOrigin (m_owner->GetOrigin ()), botStandOrigin (m_owner->GetOrigin ());

		if (m_owner->IsDucking ())
			botStandOrigin.z += HalfLifeEngine::SDK::Constants::HalfDuckHumanHeight;
		else
			botDuckOrigin.z -= HalfLifeEngine::SDK::Constants::HalfDuckHumanHeight;

		const float duckLengthSquared (bombOrigin.GetDistanceSquared (botDuckOrigin));
		const float standLengthSquared (bombOrigin.GetDistanceSquared (botStandOrigin));

		if (duckLengthSquared > HalfLifeEngine::SDK::Constants::MaximumPlayerUseDistanceSquared || standLengthSquared > HalfLifeEngine::SDK::Constants::MaximumPlayerUseDistanceSquared)
			m_isDuckDefuse = standLengthSquared >= duckLengthSquared;	// duck or else stand

		m_duckDefuseCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + HalfLifeEngine::SDK::Constants::TimeToDuck + 1.0f;
	}

	// press duck button
	if (m_isDuckDefuse)
		m_owner->PressDuck ();
	else
		m_owner->ReleaseDuck ();

	// we are starting defusing bomb if we are on a floor
	m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_USE;

	// we are defusing bomb...
	if (c4Defuser != NULL)
	{
		// don't move when defusing
//		m_owner->m_moveToGoal = false;
//		m_owner->m_checkTerrain = false;
		m_owner->m_navigator->m_speed.move = m_owner->m_navigator->m_speed.strafe = 0.0f;

//		m_owner->m_navigationTimeSet = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	}
}