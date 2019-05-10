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
// Task_Normal.hpp
//
// Class: Task_Normal
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_NORMAL_INCLUDED
#define TASK_NORMAL_INCLUDED

class Task_Normal : public Task
{
	//
	// Group: Private members.
	//
	private:
		

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_Normal (YaPB *const bot) : Task (bot) { /*VOID*/ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_Normal; }

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);
/*
		inline void OnRoundStarted (void)
		{
			switch (m_owner->GetRealTeam ())
			{
				case HalfLifeEngine::SDK::Constants::TeamID_Terrorist:
					if ((g_server->GetMapType () & Server::MapType_De) && m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4) && !s_bombPlaceAreaMapZones.IsEmpty ())
						m_owner->m_scheduleManager.AddFront (new Schedule_PlantBomb (m_owner));

					break;

				case HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist:
					if ((g_server->GetMapType () & Server::MapType_Cs) && !s_hostageAreaMapZones.IsEmpty () && !s_hostageRescueAreaMapZones.IsEmpty ())
						m_owner->m_scheduleManager.AddFront (new Schedule_RescueHostages (m_owner));

					break;
			}
		}
*/
		inline void OnBombPlanted (void)
		{
			if (m_owner->GetRealTeam () == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
				m_owner->m_scheduleManager.AddFront (new Schedule_DefuseBomb (m_owner));
		}
		inline void OnEnteredBombPlace (void)
		{
			if (m_owner->GetRealTeam () == HalfLifeEngine::SDK::Constants::TeamID_Terrorist && m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4))
				m_owner->m_scheduleManager.ChangeAction (this, new Task_PlantBomb (m_owner));
		}
		inline void OnEnteredBuyZone (void)
		{
			// Don't buy if we're the VIP or not in buy zone or bot variable deathmatch is on, OR bot variable buy is off, buying is off then no need to buy....
			if (m_owner->IsCanBuy ())
				m_owner->m_scheduleManager.AddFront (new Task_AutoBuy (m_owner));	// buy weapons first
		}

		inline void OnTakeDamage (const HalfLifeEngine::SDK::Classes::Edict *const/* inflictor*/, const unsigned short/* damage*/, const unsigned char/* armor*/, const HalfLifeEngine::SDK::Constants::DamageType_t/* bits*/)
		{
//			InternalAssert (!m_owner->m_enemy->IsValid ());	// OCCURS!!!
			if (m_owner->m_enemy->IsValid ())	// OCCURS!!!
			{
				AddLogEntry (true, LogLevel_Error, true, "\"%s\"::Task_Normal::OnTakeDamage(): Bot attacked by \"%s\" and already has enemy \"%s\", has action attack=%u.", m_owner->GetName ().GetData (), m_owner->m_lastAttackerInformation.pointer == NULL ? "NULL" : m_owner->m_lastAttackerInformation.pointer->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData (), m_owner->m_scheduleManager.HasAction (ID_AttackEnemy));
				return;
			}

			if (m_owner->m_lastAttackerInformation.pointer == NULL)
				return;

			Math::Vector3D enemyOrigin;
			Visibility_t enemyVisibility;

			if (!m_owner->CheckVisibility (*m_owner->m_lastAttackerInformation.pointer, enemyOrigin, enemyVisibility)/* && !m_owner->IsShootableThruObstacle (m_owner->GetEyePosition (), m_owner->m_lastAttackerInformation.pointer->GetEyePosition ())*/)
				return;

			m_owner->m_enemy = new Enemy_t (m_owner->m_lastAttackerInformation.pointer, enemyOrigin, enemyVisibility, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());

			#if defined _DEBUG
				// Reliability check.
				if (m_owner->m_enemy == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new bot \"%s\" enemy \"%s\"!", sizeof (*m_owner->m_enemy), m_owner->GetName ().GetData (), m_owner->m_lastAttackerInformation.pointer->GetName ().GetData ());

					return;
				}

				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_Normal::OnTakeDamage(): Attacked by enemy \"%s\", he will fight!", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());
			#endif	// if defined _DEBUG

			m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));
		}

		inline void OnHearNoisedSound (const Client *const from, const NoisedSound_t &/*noisedSound*/)
		{
			if (m_owner->m_enemy->IsValid () || !m_owner->IsEnemy (from))
				return;

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::OnHearNoisedSound(): Noised sound from enemy client: \"%s\".", m_owner->GetName ().GetData (), from->GetName ().GetData ());

			// Be ready!
			m_owner->SelectBestWeapon ();

			if (!m_owner->IsPositionVisible (from->GetEyePosition ()))
				return;

			m_owner->m_enemy = new Enemy_t (from, from->GetEyePosition (), Visibility_Head, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());

			#if defined _DEBUG
				// Reliability check.
				if (m_owner->m_enemy == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new bot \"%s\" enemy \"%s\"!", sizeof (*m_owner->m_enemy), m_owner->GetName ().GetData (), from->GetName ().GetData ());

					return;
				}

				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_Normal::OnTakeDamage(): Found new enemy: \"%s\" - taking pause....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());
			#endif	// if defined _DEBUG

			m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));
		}
};

#endif	// ifndef TASK_NORMAL_INCLUDED