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
// Task_SelectTeam.cpp
//
// Class: Task_SelectTeam
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPB::Task_SelectTeam::Execute (void)
{
	// The purpose of this function is to make the bot browse down through the team selection menus of Counter-Strike.

	if (m_delayTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		return;	// YaPB: "I'm thinking....".

	// Handle Counter-Strike stuff here...

	// Reliability checks.
	InternalAssert (m_owner->m_gameAction == YaPB::GameAction_TeamSelect);
	InternalAssert
	(
		m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ||
		m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ||

		m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_AutoSelect
	);

	// switch back to idle
	m_owner->m_gameAction = YaPB::GameAction_Idle;
/*
	const DynamicString forceTeam (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ForceTeam)->GetString ());

	if (!forceTeam.IsEmpty ())
	{
		// Terrorists team.
		if (forceTeam.CompareWithoutCaseCheck ('1') == 0 || forceTeam.CompareWithoutCaseCheck ('t') == 0 || forceTeam.CompareWithoutCaseCheck ("terrorist") == 0)
			m_owner->m_profile->team = HalfLifeEngine::SDK::Constants::TeamID_Terrorist;
		else if (forceTeam.CompareWithoutCaseCheck ('2') == 0 || forceTeam.CompareWithoutCaseCheck ("ct") == 0 || forceTeam.CompareWithoutCaseCheck ("counter-terrorist") == 0)	// Counter-Terrorists team.
			m_owner->m_profile->team = HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist;
		else if (forceTeam.CompareWithoutCaseCheck ('5') == 0 || forceTeam.CompareWithoutCaseCheck ("random") == 0)	// Random team.
			m_owner->m_profile->team = static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (g_randomNumberGenerator.GetValueBetween <unsigned char> (HalfLifeEngine::SDK::Constants::TeamID_Terrorist, HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));
	}

	/// @note If force team console variable string is empty or "unknown" for a bot - choose team by bot profile....

	if (g_server->GetSpawnPointsContainer ()[m_owner->m_profile->team].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (m_owner->m_profile->team))
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		if (g_server->GetSpawnPointsContainer ()[otherTeam].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (otherTeam))
		{
			m_owner->Kick ();

			Fail ();

			return;
		}

		m_owner->m_profile->team = otherTeam;
	}
	else
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		if (clientsNumberInWantedTeam > g_server->GetClientManager ()->GetClientsCount (otherTeam) + 1u || g_server->GetTeamScore (m_owner->m_profile->team) > g_server->GetTeamScore (otherTeam) + 5u)
			m_owner->m_profile->team = otherTeam;
	}
*/
	const DynamicString forceTeam (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ForceTeam)->GetString ());

	if (!forceTeam.IsEmpty ())
	{
		// Terrorists team.
		if (forceTeam.CompareWithCaseCheck ('1') == 0 || forceTeam.CompareWithoutCaseCheck ('t') == 0 || forceTeam.CompareWithoutCaseCheck ("terrorist") == 0)
			m_owner->m_profile->team = HalfLifeEngine::SDK::Constants::TeamID_Terrorist;
		else if (forceTeam.CompareWithCaseCheck ('2') == 0 || forceTeam.CompareWithoutCaseCheck ("ct") == 0 || forceTeam.CompareWithoutCaseCheck ("counter-terrorist") == 0)	// Counter-Terrorists team.
			m_owner->m_profile->team = HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist;
		else if (forceTeam.CompareWithCaseCheck ('5') == 0 || forceTeam.CompareWithoutCaseCheck ("random") == 0)	// Random team.
			m_owner->m_profile->team = static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (g_randomNumberGenerator.GetValueBetween <unsigned char> (HalfLifeEngine::SDK::Constants::TeamID_Terrorist, HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));
		else
			goto Label_ChooseTeamByProfile;

		if (g_server->GetSpawnPointsContainer ()[m_owner->m_profile->team].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (m_owner->m_profile->team))
		{
			YaPBManager::GetCreationTable ().RemoveAll ();	// Reset table of creation.

			m_owner->Kick ();

			Fail ();

			// cancel if unable to choose team for fake client
			return;
		}
	}
	else
	{
		Label_ChooseTeamByProfile:
		if (m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_AutoSelect)
		{
			if
			(
				g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_Terrorist].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) &&
				g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
			)
			{
				YaPBManager::GetCreationTable ().RemoveAll ();	// Reset table of creation.

				m_owner->Kick ();

				Fail ();

				// cancel if unable to choose team for fake client
				return;
			}
		}
		else
		{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		// Is wanted loosing team?
		if (g_server->GetTeamScore (m_owner->m_profile->team) + 5u < g_server->GetTeamScore (otherTeam))
//		if (g_server->GetTeamScore (m_owner->m_profile->team) > g_server->GetTeamScore (otherTeam))
			m_owner->m_profile->team = otherTeam;	// Choose other team to help them....

	const unsigned char clientsNumberInWantedTeam (g_server->GetClientManager ()->GetClientsCount (m_owner->m_profile->team));

	/// @note If force team console variable string is empty or "unknown" for a bot - choose team by bot profile....

	// Is no more player spawn points in wanted team?
	if (clientsNumberInWantedTeam >= g_server->GetSpawnPointsContainer ()[m_owner->m_profile->team].GetElementNumber ())
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		if (g_server->GetSpawnPointsContainer ()[otherTeam].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (otherTeam))
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BothTeamsReachedMaximumPlayers) + '\n', g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist), g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));

			YaPBManager::GetCreationTable ().RemoveAll ();	// Reset table of creation.

			m_owner->Kick ();

			Fail ();

			// cancel if unable to choose team for fake client
			return;
		}

		// Choose other team....
		m_owner->m_profile->team = otherTeam;
	}
	else
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		// Is too many players in wanted team?
		if (clientsNumberInWantedTeam > g_server->GetClientManager ()->GetClientsCount (otherTeam)/* + 1u*/)
		{
			if (g_server->GetClientManager ()->GetClientsCount (otherTeam) < g_server->GetSpawnPointsContainer ()[otherTeam].GetElementNumber ())
				m_owner->m_profile->team = otherTeam;	// Choose other team....
			else
			{
				const unsigned char limitTeams (HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetValue <unsigned char> ("mp_limitteams"));

				if (limitTeams > 0u && clientsNumberInWantedTeam + limitTeams <= g_server->GetClientManager ()->GetClientsCount (otherTeam))
				{
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BothTeamsReachedMaximumPlayers) + '\n', g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist), g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));

					YaPBManager::GetCreationTable ().RemoveAll ();	// Reset table of creation.

					m_owner->Kick ();

					Fail ();

					// cancel if unable to choose team for fake client
					return;
				}
			}
		}
	}
		}
	}

	// select the team the bot wishes to join....
	m_owner->ExecuteCommandFormat ("menuselect %u", m_owner->m_profile->team);

	if
	(
		m_owner->m_profile->team == HalfLifeEngine::SDK::Constants::TeamID_AutoSelect ?
			m_owner->IsSpectator () :
			m_owner->GetRealTeam () != m_owner->m_profile->team
	)
	{
		AddLogEntry (true, LogLevel_Error, true, "YaPB::Task_SelectTeam::Execute(): Bot \"%s\" can't choose wanted team %u!", m_owner->GetName ().GetData (), m_owner->m_profile->team);

		m_owner->Kick ();

		Fail ();

		return;
	}
/*/
	m_owner->ExecuteCommand ("menuselect 5");

	if (m_owner->IsSpectator ())
	{
		AddLogEntry (true, LogLevel_Error, true, "YaPB::Task_SelectTeam::Execute(): Bot \"%s\" can't choose team!", m_owner->GetName ().GetData ());

		m_owner->Kick ();

		Fail ();

		return;
	}
*/
	Complete ();
}