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
// Task_AttackEnemy.cpp
//
// Class: Task_AttackEnemy
//
// Version: $ID:$
//

#include <Core.hpp>

const bool YaPB::CheckFirePause (const float distanceToLookAt, const FireDelay_t &fireDelay)
{
	// returns true if bot needs to pause between firing to compensate for punch angle & weapon spread.

	if (!m_currentWeapon->IsValid ())
		return true;	// Always pause....

//	return false;	// FOR NO RECOIL AND NO SPREAD TESTS

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> ()/* >= 2u*/ && GetProfile ()->skill == BotSkillLevel_Godlike)
		return false;	// Always fire....

	if (m_currentWeapon->IsSniperRifle ())
	{
		m_shootTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();

		return false;
	}

	if (m_firePause > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		#if defined _DEBUG
			DebugMessageFormat ("is not fireing because of the pause. (%.2f)", m_firePause - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
		#endif	// if defined _DEBUG

		return true;
	}

	// check if we need to compensate recoil (not 180.0, but 360.0, because (2 x angles) / 2 / 180.0 * Pi)
//	if (Math::tanf (Math::sqrtf (MATH_GET_SQUARED (Math::DegreeToRadian (Math::fabsf (m_edict->GetPunchAngles ().pitch))) + MATH_GET_SQUARED (Math::DegreeToRadian (Math::fabsf (m_edict->GetPunchAngles ().yaw))))) * distanceToLookAt > s_skillTables[m_profile->skill / 20u].recoilAmount)
	if (Math::tanf ((Math::fabsf (m_edict->GetPunchAngles ().pitch) + Math::fabsf (m_edict->GetPunchAngles ().yaw)) * Math::Pi / 360.0f) * (distanceToLookAt + distanceToLookAt / 4.0f) > s_skillTables[m_profile->skill / 20u].recoilAmount)
	{
		if (m_firePause < HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - 0.4f)
			m_firePause = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (0.4f, 0.4f + 1.2f * m_profile->skill.GetReducingOffset ());

		#if defined _DEBUG
			DebugMessageFormat ("has to wait random time(%.2f) because of punch angles.", m_firePause - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
		#endif	// if defined _DEBUG

		return true;
	}

	if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> () && fireDelay.maximumFireBullets + g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 1u) <= static_cast <CurrentWeapon_Reloadable *> (m_currentWeapon)->GetBurstShotsFired ())
	{
		float delayTime (0.1f * distanceToLookAt / fireDelay.minimumBurstPauseFactor);
		const float maximumDelayTime (125.0f / (m_profile->skill + 1u));

		if (delayTime > maximumDelayTime)
			delayTime = maximumDelayTime;

		m_firePause = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + delayTime;
		static_cast <CurrentWeapon_Reloadable *> (m_currentWeapon)->m_burstShotsFired = 0u;

		#if defined _DEBUG
			DebugMessageFormat ("has to wait max burst wait time. (%.2f)", m_firePause - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
		#endif	// if defined _DEBUG

		return true;
	}

	return false;
}

void YaPB::FireWeapon (void)
{
	// This function will return true if weapon was fired, false otherwise.

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DontShoot)->GetValue <bool> ())
		return;

	const float distanceToLookAt (m_aim->m_lookAt.GetDistance (m_edict->GetEyePosition ()));	// how far away is the enemy?

	// bot can't hurt teammates, if friendly fire is not enabled...
	if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_FriendlyFire)->GetValue <bool> () && g_server->IsTeamplay ())
	{
		// if friend in line of fire, stop this too but do not update shoot time
		if (IsTemmateInLineOfFire (distanceToLookAt))
			return;
	}

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" in attack = %u.", GetName ().GetData (), (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK));

	// need to care for minimum burst fire distance if not blind?
	if (distanceToLookAt < s_minimumBurstDistance || m_lastScreenFadeInformation.duration > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		if (m_currentWeapon->GetID () == HalfLifeEngine::SDK::Constants::WeaponID_Knife)
		{
			// if good distance for secondary attack...
			if (distanceToLookAt <= HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].maximumShootDistance2)
			{
				if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < (m_profile->skill >= 80u ? 15u : 80u) || HasShield ())
				{
					PressPrimaryAttack ();	// use primary attack

					m_shootTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
				}
				else
					m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;	// use secondary attack
			}
			else if (distanceToLookAt <= HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].maximumShootDistance1)	// else if good distance for primary attack...
			{
				PressPrimaryAttack ();

				m_shootTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
			}
		}
		else
		{
			// if automatic weapon, just press attack button, if not, toggle buttons
			if (HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].primaryFireHold || !(m_edict->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
			{
				PressPrimaryAttack ();

				m_shootTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
			}
		}
	}
	else
	{
		if (m_currentWeapon->GetID () == HalfLifeEngine::SDK::Constants::WeaponID_Knife)
			return;	// don't attack with knife over long distance

		unsigned char fireDelayIndex (0u);

		/// @todo MAKE SOME FASTER WAY TO FIND 'fireDelayIndex'....
		{
			for (/* Empty */; fireDelayIndex <= NumberWeapons; ++fireDelayIndex)
				if (s_fireDelays[fireDelayIndex].weaponID == m_currentWeapon->GetID ())
					break;
		}

		if (CheckFirePause (distanceToLookAt, s_fireDelays[fireDelayIndex]))
			return;

		if (HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].primaryFireHold)
		{
			PressPrimaryAttack ();	// use primary attack

			m_shootTime = /*m_zoomCheckTime = */HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}
		else
		{
			PressPrimaryAttack ();	// use primary attack

			const float baseDelay (s_fireDelays[fireDelayIndex].primaryBaseDelay);
			const unsigned char index (static_cast <unsigned char> (abs (static_cast <char> (m_profile->skill / 20u) - 5)));
			const float minDelay (s_fireDelays[fireDelayIndex].primaryMinDelay[index]);
			const float maxDelay (s_fireDelays[fireDelayIndex].primaryMaxDelay[index]);

			m_shootTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + baseDelay + g_randomNumberGenerator.GetValueBetween (minDelay, maxDelay);
//			m_zoomCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}
	}
}

void YaPB::CheckFocusEnemy (void)
{
	static float distanceToLookAtSquared, dot;
	static Math::Vector3D lookAt;
/*
	// aim for the head and/or body
	m_lookAt = GetEyeForwardPosition ();	/// @todo MOVE IT OUT OF HERE!!!

	if (m_enemySurpriseTime >= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		return;
*/
	lookAt = m_aim->m_lookAt;//m_enemy->origin;
	distanceToLookAtSquared = lookAt.GetDistanceSquared (m_edict->GetEyePosition ());	// how far away is the enemy scum?

	InternalAssert (m_currentWeapon->IsValid ());

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::CheckFocusEnemy(): cur cone=%.2f, ZBot ideal cone=%.2f.", GetName ().GetData (), GetShootingConeDeviationWithPunchAngles (lookAt), 1.0f - Math::cosf (Math::atan2f (1.0f, (m_currentWeapon->IsSniperRifle () ? 16.0f : 16.0f + 16.0f) / Math::sqrtf (distanceToLookAtSquared))));
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::CheckFocusEnemy(): dot=%.10f, ideal cone=%.10f.", GetName ().GetData (), GetShootingConeDeviationWithPunchAngles (lookAt), GetMinimumShootingConeDeviation (distanceToLookAtSquared, m_enemy->shootRadius));

	if (distanceToLookAtSquared <= MATH_GET_SQUARED (128.0f))
	{
		if (m_currentWeapon->GetID () == HalfLifeEngine::SDK::Constants::WeaponID_Knife)
		{
			if (distanceToLookAtSquared <= MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].maximumShootDistance1))
			{
				dot = GetShootingConeDeviationWithPunchAngles (lookAt);

				// bot faces enemy?
				m_wantsToFire = dot >= GetMinimumShootingConeDeviation (distanceToLookAtSquared, HalfLifeEngine::SDK::Constants::HumanWidth * 2.0f)/*0.6f*/;	/// @todo TUNE dot VALUE!!!
			}

			return;
		}

		dot = GetShootingConeDeviationWithPunchAngles (lookAt);

		// bot faces enemy?
		m_wantsToFire = dot >= GetMinimumShootingConeDeviation (distanceToLookAtSquared, m_enemy->shootRadius)/*0.6f*/;	/// @todo TUNE dot VALUE!!!
	}
	else
	{
		if (m_currentWeapon->GetID () == HalfLifeEngine::SDK::Constants::WeaponID_Knife || (!IsOnFloor () && distanceToLookAtSquared > MATH_GET_SQUARED (s_minimumBurstDistance)))
		{
			m_wantsToFire = false;

			return;
		}

		dot = GetShootingConeDeviationWithPunchAngles (lookAt);

		// enemy faces bot?
		m_wantsToFire = dot < 0.9f ? false : m_enemy->pointer->GetShootingConeDeviation (GetOrigin ()) >= 0.9f ? true : dot >= GetMinimumShootingConeDeviation (distanceToLookAtSquared, m_enemy->shootRadius)/*0.99f*/;	/// @todo TUNE dot VALUES!!!
	}
}

void YaPB::Task_AttackEnemy::Execute (void)
{
//	AddLogEntry (true, LogLevel_Default, false, "Task_AttackEnemy::Execute() called for bot %s.", m_owner->GetName ().GetData ());

	m_owner->m_aim->Think ();

	m_owner->FindEnemy ();

	if (!m_owner->m_enemy->IsValid ())
	{
		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AttackEnemy::Execute(): Task completed!", m_owner->GetName ().GetData ());

		Complete ();

		return;
	}

	m_owner->m_wantsToFire = false;

	m_owner->SelectBestWeapon ();

	if (m_owner->m_currentWeapon->GetID () == HalfLifeEngine::SDK::Constants::WeaponID_Knife)
	{
		if (m_owner->GetLastNavigationArea () != m_owner->m_enemy->pointer->GetLastNavigationArea ())
		{
			if (!m_owner->m_navigator->GoalIsValid () || m_owner->m_navigator->GetGoal () != m_owner->m_enemy->pointer->GetLastNavigationArea ())
			{
				if (!m_owner->m_navigator->SetMoveTo (m_owner->m_enemy->pointer->GetLastNavigationArea ()))
					HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_AttackEnemy::Execute(): Can't SetMoveTo(). Possibly standing out of navmesh....", m_owner->GetName ().GetData ());
			}
		}
		else
		{
			m_owner->m_navigator->SetDestinationOrigin (m_owner->m_enemy->pointer->GetOrigin ());
			m_owner->m_navigator->MoveForward ();
			m_owner->m_navigator->Think ();
		}
	}
/*
	{
	if (m_owner->IsDebuggingBot ())
	{
		if (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK)
			m_owner->GetEdict ()->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK;
	}

	return;
	}
*
	if (m_owner->IsOnFloor ())
		m_owner->GetNavigator ()->Jump ();
	m_owner->PressPrimaryAttack ();
*/
	if (!m_owner->m_currentWeapon->IsInReload () && static_cast <const unsigned char> (m_owner->m_currentWeapon->GetAmmoInClip ()) > 0u)
		m_owner->CheckFocusEnemy ();

	// the bots wants to fire at something?
	if (m_owner->m_wantsToFire && m_owner->m_shootTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()/* && !m_owner->m_isUsingGrenade*/)
	{
		// if bot didn't fire a bullet try again next frame
		m_owner->FireWeapon ();
	}
}