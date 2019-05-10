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
// Task_MoveToPosition.hpp
//
// Class: Task_MoveToPosition
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_TASKS_TASK_MOVE_TO_POSITION_INCLUDED
#define BOT_TASKS_TASK_MOVE_TO_POSITION_INCLUDED

class Task_MoveToPosition : public Task
{
	//
	// Group: Private members.
	//
	protected:
		Math::Vector3D m_destinationOrigin;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_MoveToPosition (YaPB *const bot, const Math::Vector3D &destination) :
			Task (bot),

			m_destinationOrigin (destination)
		{ /* VOID */ }	// going here

		virtual inline ~Task_MoveToPosition (void) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		virtual inline const ID_t GetID (void) const { return ID_MoveToPosition; }

		virtual inline const bool IsReachedDestination (void)
		{
			// Default check.
			return m_destinationOrigin.GetDistanceSquared2D (m_owner->GetOrigin ()) <= MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::HumanWidth);
		}

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline void Run (void)
		{
			// Call original function in base class....
			Task::Run ();

			if (!m_owner->m_navigator->SetMoveTo (m_destinationOrigin))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_MoveToPosition::Run(): Can't SetMoveTo(m_destinationOrigin)!!!", m_owner->GetName ().GetData ());

				Fail ();
/*
				InternalAssert (m_owner->m_lastNavigationArea != NULL);

				g_origins.RemoveAll ();

				g_origins += m_owner->GetOrigin ();
				g_origins += m_destinationOrigin;*/
			}
		}
		virtual inline void Execute (void)
		{
			m_owner->m_aim->Think ();
			m_owner->m_chatterManager.Think (m_owner);

			// fight the enemy if we found one
			if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
			{
				// Reliability check.
				InternalAssert (m_owner->m_enemy->IsValid ());

				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_MoveToPosition::Execute(): Find enemy \"%s\" - taking pause....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

				m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));

				return;
			}

			if (m_owner->m_reloadState == ReloadState_None && m_owner->m_currentWeapon->GetAmmo () > 0u)
				m_owner->m_reloadState = ReloadState_Primary;

			m_owner->TryCheckReload ();
			m_owner->TryCheckSilencer ();

			// spray logo sometimes if allowed to do so
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Spray)->GetValue <bool> () && m_owner->m_timeLogoSpray <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && m_owner->m_navigator->m_speed.move > m_owner->GetWalkSpeed ())
			{
				if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 80u)
				{
					m_owner->m_scheduleManager.AddFront (new Task_SprayLogo (m_owner, g_randomNumberGenerator.GetValueBetween (1.0f, 2.0f)));

					return;
				}
				else
					m_owner->m_timeLogoSpray = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (m_owner->m_profile->baseReactTime, m_owner->m_profile->baseReactTime + 1.6f);
			}

			if (IsReachedDestination ())
			{
				Complete ();

				return;
			}

			if (!m_owner->m_navigator->HasRoute ())
				m_owner->m_navigator->SetDestinationOrigin (m_destinationOrigin);

			m_owner->m_edict->variables.buttons = HalfLifeEngine::SDK::Constants::IN_FORWARD;

			m_owner->m_navigator->Think ();
		}
};

class Task_MoveToRescueZoneWithHostages : public Task_MoveToPosition
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_MoveToRescueZoneWithHostages (YaPB *const bot) :
			Task_MoveToPosition (bot, GetRandomPositionInAreaZone (s_hostageRescueAreaMapZones.GetRandomElement ()))
		{ /* VOID */ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_MoveToRescueZoneWithHostages; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void OnHostageKilled (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const killer)
		{
			// Call this function in base class....
			Task_MoveToPosition::OnHostageKilled (hostage, killer);

			m_owner->ExecuteCommandFormat ("say %s WTF YOU KILL MY HOSTAGE?!?!?!", killer->GetName ().GetData ());

			if (!m_owner->GetHostageManager ().IsEmpty ())
				return;

			InternalAssert (m_owner->m_scheduleManager.IsCurrentAction (Schedule::ID_RescueHostages));

			const AreaMapZone_t &hostagesZone (s_hostageAreaMapZones.GetRandomElement ());

			m_owner->m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->AddTask (new Task_MoveToPosition (m_owner, GetRandomPositionInAreaZone (hostagesZone)));
			m_owner->m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->AddTask (new Task_RescueHostages (m_owner, hostagesZone));

			CompleteAndDelete ();
		}
		inline void OnHostageRescued (const HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			// Call this function in base class....
			Task_MoveToPosition::OnHostageRescued (hostage);

			if (!m_owner->GetHostageManager ().IsEmpty ())
				return;

			InternalAssert (m_owner->m_scheduleManager.IsCurrentAction (Schedule::ID_RescueHostages));

			const AreaMapZone_t &hostagesZone (s_hostageAreaMapZones.GetRandomElement ());

			m_owner->m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->AddTask (new Task_MoveToPosition (m_owner, GetRandomPositionInAreaZone (hostagesZone)));
			m_owner->m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->AddTask (new Task_RescueHostages (m_owner, hostagesZone));

			CompleteAndDelete ();
		}
		inline void OnHostageUnusePost (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const currentRescuer)
		{
			// Call this function in base class....
			Task_MoveToPosition::OnHostageUnusePost (hostage, currentRescuer);

			if (currentRescuer != NULL)
				m_owner->ExecuteCommandFormat ("say %s WTF YOU STEAL MY HOSTAGE?!?!?!", currentRescuer->GetName ().GetData ());

			if (!m_owner->GetHostageManager ().IsEmpty ())
				return;

			InternalAssert (m_owner->m_scheduleManager.IsCurrentAction (Schedule::ID_RescueHostages));

			const AreaMapZone_t &hostagesZone (s_hostageAreaMapZones.GetRandomElement ());

			m_owner->m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->AddTask (new Task_MoveToPosition (m_owner, GetRandomPositionInAreaZone (hostagesZone)));
			m_owner->m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->AddTask (new Task_RescueHostages (m_owner, hostagesZone));

			CompleteAndDelete ();
		}
		inline void OnAllHostagesRescued (void)
		{
			// Call this function in base class....
			Task_MoveToPosition::OnAllHostagesRescued ();

			CompleteAndDelete ();
		}
};

class Task_MoveAndPickupItem : public Task_MoveToPosition
{
	//
	// Group: Private members.
	//
	private:
		const HalfLifeEngine::SDK::Classes::Edict *const m_itemEdict;
		const HalfLifeEngine::SDK::Constants::WeaponID_t m_weaponID;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_MoveAndPickupItem (YaPB *const bot, const HalfLifeEngine::SDK::Classes::Edict *const item, const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID) :
			Task_MoveToPosition (bot, item->GetOrigin ()),

			m_itemEdict (item),
			m_weaponID (weaponID)
		{
			++m_destinationOrigin.z;	// Sometimes weapons spawns partially in ground!
		}	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_MoveAndPickupItem; }

		inline const bool IsReachedDestination (void)
		{
			// We must touch the item!
			return false;
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Execute (void)
		{
			// someone owns this weapon, OR it hasn't respawned yet
			if (!m_itemEdict->IsValid () || (m_itemEdict->variables.effects & HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw) || m_owner->HasWeapon (m_weaponID))
			{
				Complete ();

				return;
			}

			m_destinationOrigin = m_itemEdict->GetOrigin ();
			++m_destinationOrigin.z;	// Sometimes weapons spawns partially in ground!

			// Call this function in base class....
			Task_MoveToPosition::Execute ();
		}

		inline void OnAddWeapon (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Call this function in base class....
			Task_MoveToPosition::OnAddWeapon (weapon);

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_MoveAndPickupItem::OnAddWeapon(%s)!!! wpn: \"%s\"", m_owner->GetName ().GetData (), weapon->GetClassName ().GetData (), HalfLifeEngine::SDK::Constants::weaponProperties[m_weaponID].className.GetData ());

//			if (weapon == m_itemEdict)	// NOT WORK FOR "armoury_entity"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if (weapon->GetWeaponID () == m_weaponID)
				CompleteAndDelete ();
		}
		inline void OnTouchPost (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			// Call this function in base class....
			Task_MoveToPosition::OnTouchPost (entity);

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_MoveAndPickupItem::OnTouchPost(%s)!!! PRE, wpn: \"%s\"", m_owner->GetName ().GetData (), entity->GetClassName ().GetData (), HalfLifeEngine::SDK::Constants::weaponProperties[m_weaponID].className.GetData ());

			if (entity != m_itemEdict)
				return;

			if (entity->variables.flags & HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw)
				return;

			if (entity->isFree)
				return;

			if (HalfLifeEngine::Utilities::WeaponIsPistol (m_weaponID))
				m_owner->DropAllSecondaryWeapons ();
			else if (HalfLifeEngine::Utilities::WeaponIsPrimary (m_weaponID))
				m_owner->DropAllPrimaryWeapons ();

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_MoveAndPickupItem::OnTouchPost(%s)!!! POST, wpn: \"%s\"", m_owner->GetName ().GetData (), entity->GetClassName ().GetData (), HalfLifeEngine::SDK::Constants::weaponProperties[m_weaponID].className.GetData ());
		}
};

#endif	// ifndef BOT_TASKS_TASK_MOVE_TO_POSITION_INCLUDED