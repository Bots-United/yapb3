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
// Task_Normal.cpp
//
// Class: Task_Normal
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPB::Task_Normal::Execute (void)
{
/*	const Host *const host (g_server->GetHumanManager ()->GetHostClient ());

	// Reliability check.
	InternalAssert (host->IsValid ());

	if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DontShoot)->GetValue <bool> () && (host->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
		m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK;
/*
	{
		m_owner->m_edict->variables.buttons = host->GetEdict ()->variables.buttons & ~HalfLifeEngine::SDK::Constants::IN_SCORE;

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DontShoot)->GetValue <bool> ())
			m_owner->m_edict->variables.buttons &= ~(HalfLifeEngine::SDK::Constants::IN_ATTACK | HalfLifeEngine::SDK::Constants::IN_ATTACK2);

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Stop)->GetValue <bool> ())
			m_owner->m_edict->variables.buttons &= ~(HalfLifeEngine::SDK::Constants::IN_JUMP | HalfLifeEngine::SDK::Constants::IN_DUCK | HalfLifeEngine::SDK::Constants::IN_FORWARD | HalfLifeEngine::SDK::Constants::IN_BACK | HalfLifeEngine::SDK::Constants::IN_MOVELEFT | HalfLifeEngine::SDK::Constants::IN_MOVERIGHT);
	}
*/
	m_owner->m_aim->Think ();
	m_owner->m_navigator->Think ();
	m_owner->m_chatterManager.Think (m_owner);

	// fight the enemy if we found one
	if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
	{
		// Reliability check.
		InternalAssert (m_owner->m_enemy->IsValid ());

		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_Normal::Execute(): Find enemy \"%s\" - taking pause....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

		m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));

		return;
	}

	{
		m_owner->UpdateEnvironmentEntities ();
		m_owner->CheckItemsToPickup ();

		if (!m_owner->m_environmentEntities.weapons.IsEmpty ())
		{
			const HalfLifeEngine::SDK::Classes::Edict *const weapon (m_owner->GetBestEnvironmentWeapon ());
/*
			{
			const YaPB::Profile::WeaponsPreferences_t::Array_t::IndexType_t bw (m_owner->GetBestWeaponCarried ());
			const YaPB::Profile::WeaponsPreferences_t::Array_t::IndexType_t bsw (m_owner->GetBestSecondaryWeaponCarried ());
			const DynamicString bwcn (bw == YaPB::Profile::WeaponsPreferences_t::Array_t::BadPosition ? "NONE!!!" : HalfLifeEngine::SDK::Constants::weaponProperties[m_owner->GetProfile ()->weaponsPreferences.common[bw]].className);
			const DynamicString bswcn (bsw == YaPB::Profile::WeaponsPreferences_t::Array_t::BadPosition ? "NONE!!!" : HalfLifeEngine::SDK::Constants::weaponProperties[m_owner->GetProfile ()->weaponsPreferences.common[bsw]].className);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_Normal::Execute(): Find %u weapons! (best own: prim=%s, sec=%s, %u)", m_owner->GetName ().GetData (), m_owner->m_environmentEntities.weapons.GetElementNumber (), bwcn.GetData (), bswcn.GetData (), weapon != NULL);
			}
*/
			if (weapon != NULL)
			{
				HalfLifeEngine::SDK::Constants::WeaponID_t weaponID;

				for (weaponID = HalfLifeEngine::SDK::Constants::WeaponID_None; ++reinterpret_cast <unsigned int &> (weaponID) < HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u; /* Empty */)
					if (HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].modelName == weapon->GetNonPlayerModel ())
						break;

				if (weaponID < HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u && !m_owner->HasWeapon (weaponID))
				{
					AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_Normal::Execute(): Find dropped weapon \"%s\" - taking pause....", m_owner->GetName ().GetData (), weapon->GetClassName ().GetData ());

					m_owner->m_scheduleManager.AddFront (new Task_MoveAndPickupItem (m_owner, weapon, weaponID/*weapon->GetWeaponID ()*/));
#if 0
//					g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (weapon->GetOrigin (), 5.0f, Color <> (0u, 255u, 0u), 1u, 255u, 1u/*, false*/);
					for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
					{
						if (g_server->GetNavigationMeshManager ().GetAreas ()[index]->Contains (weapon->GetOrigin ()))
							HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "CONTAINS %u", g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetID ());
						else if (g_server->GetNavigationMeshManager ().GetAreas ()[index]->IsOverlapping (weapon->GetOrigin ()))
						{
							HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "OVERLAPPING %u", g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetID ());

						HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "GETZ(%f) <= (%f)MY.Z %u", g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetZ (weapon->GetOrigin ()), weapon->GetOrigin ().z, g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetID ());
						}
					}
#endif	// if 0
					return;
				}
			}
		}
	}

	{
		if (m_owner->m_reloadState == ReloadState_None && m_owner->m_currentWeapon->GetAmmo () > 0u)
			m_owner->m_reloadState = ReloadState_Primary;

		m_owner->TryCheckReload ();
		m_owner->TryCheckSilencer ();
	}

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

	switch (m_owner->GetRealTeam ())
	{
		case HalfLifeEngine::SDK::Constants::TeamID_Terrorist:
			if ((g_server->GetMapType () & Server::MapType_De) && m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4) && !s_bombPlaceAreaMapZones.IsEmpty ())
			{
				m_owner->m_scheduleManager.AddFront (new Schedule_PlantBomb (m_owner));

				return;
			}

			break;

		case HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist:
			if (g_server->GetC4Manager ().IsÑ4Planted ())
			{
				m_owner->m_scheduleManager.AddFront (new Schedule_DefuseBomb (m_owner));

				return;
			}

			if ((g_server->GetMapType () & Server::MapType_Cs) && !s_hostageAreaMapZones.IsEmpty () && !s_hostageRescueAreaMapZones.IsEmpty ())
			{
				m_owner->m_scheduleManager.AddFront (new Schedule_RescueHostages (m_owner));

				return;
			}

			if (m_owner->IsVIP () && !s_VIPSafetyAreaMapZones.IsEmpty ())
			{
				m_owner->m_scheduleManager.AddFront (new Task_MoveToPosition (m_owner, GetRandomPositionInAreaZone (s_VIPSafetyAreaMapZones.GetRandomElement ())));

				return;
			}

			break;
	}

	if (!m_owner->m_navigator->GoalIsValid ()/* || m_owner->m_navigator->GetGoal ()->GetAreaPointer <NavigationMesh::NavigationArea> () == m_owner->m_lastNavigationArea*/)	// no more nodes to follow - search new ones (or we have a bomb)
	{
		// did we already decide about a goal before?
		if (!m_owner->m_navigator->SetMoveTo (m_owner->FindGoalArea ()))
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_Normal::Execute(): Can't SetMoveTo(). Possibly standing out of navmesh....", m_owner->GetName ().GetData ());
	}

	if (m_owner->m_wantToLeaveCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		if (m_owner->IsWantToLeaveGame ())
		{
			m_owner->Kick ();

			return;
		}

		m_owner->m_wantToLeaveCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (1.0f, 3.0f);
	}
}