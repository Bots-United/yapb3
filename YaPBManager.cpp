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
// YaPBManager.cpp
//
// Class: YaPBManager
//
// Version: $ID:$
//

#include <Core.hpp>

#if defined _DEBUG
	const float YaPBManager::ms_debugBotUpdateDelay (1.0f);	// One second.
#endif	// if defined _DEBUG

YaPBManager::YaPBsCreationTableArray_t YaPBManager::ms_creationTable;	// bot creation table

YaPBManager::ConnectedYaPBsArray_t YaPBManager::g_connectedYaPBs;

const bool YaPBManager::CreateBot (const DynamicString &name, const unsigned char/* skill*/, const Personality_t/* personality*/, const HalfLifeEngine::SDK::Constants::TeamID_t/* team*/, const unsigned char/* model*/)
{
	// This function completely prepares bot entity (edict) for creation, creates team, skill, sets name etc, and then sends result to bot constructor.

	/// @todo Make a algorithm for search or creation a new profile according to giving parameters....

	return CreateBot (GetProfileManager ().PickBotProfile (name));
}

const bool YaPBManager::CreateBot (YaPB::Profile *const profile)
{
	// This function completely prepares bot entity (edict) for creation, creates team, skill, sets name etc, and then sends result to bot constructor.
/*
	// don't allow creating bots with no navigation mesh loaded
	if (!g_server->GetNavigationMeshManager ().IsLoaded ())
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_UnableToCreateYaPB_Reason), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_NavigationMesh_NotExits).GetData ());

		Label_RemoveCreationTableResetQuotaAndReturnFailure:
		{
			ResetCreationTableAndQuota ();	// Reset table of creation and quota.

			// cancel if unable to create fake client, or choose team for fake client, or something else....
			return false;
		}
	}

	// don't allow creating bots with no waypoints loaded
	if (g_server->GetWaypointManager ().GetWaypointsNumber () == 0u)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_MapNotWaypointed));

		goto Label_RemoveCreationTableResetQuotaAndReturnFailure;
	}

	// don't allow creating bots with changed waypoints (distance tables are messed up)
	if (g_waypointsChanged)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_WaypointsHasBeenChanged));

		goto Label_RemoveCreationTableResetQuotaAndReturnFailure;
	}
*/
	// Reliability checks.
	InternalAssert (GetProfileManager ().IsValidProfile (profile));
	InternalAssert
	(
		profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ||
		profile->team == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ||

		profile->team == HalfLifeEngine::SDK::Constants::TeamID_AutoSelect
	);

	const DynamicString forceTeam (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ForceTeam)->GetString ());

	if (!forceTeam.IsEmpty ())
	{
		// Terrorists team.
		if (forceTeam.CompareWithCaseCheck ('1') == 0 || forceTeam.CompareWithoutCaseCheck ('t') == 0 || forceTeam.CompareWithoutCaseCheck ("terrorist") == 0)
			profile->team = HalfLifeEngine::SDK::Constants::TeamID_Terrorist;
		else if (forceTeam.CompareWithCaseCheck ('2') == 0 || forceTeam.CompareWithoutCaseCheck ("ct") == 0 || forceTeam.CompareWithoutCaseCheck ("counter-terrorist") == 0)	// Counter-Terrorists team.
			profile->team = HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist;
		else if (forceTeam.CompareWithCaseCheck ('5') == 0 || forceTeam.CompareWithoutCaseCheck ("random") == 0)	// Random team.
			profile->team = static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (g_randomNumberGenerator.GetValueBetween <unsigned char> (HalfLifeEngine::SDK::Constants::TeamID_Terrorist, HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));
		else
			goto Label_ChooseTeamByProfile;

		if (g_server->GetSpawnPointsContainer ()[profile->team].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (profile->team))
		{
			GetCreationTable ().RemoveAll ();	// Reset table of creation.

			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (GetBotsCount ());	// So set bot quota to maximum players.

			return false;
		}
	}
	else
	{
		Label_ChooseTeamByProfile:
		if (profile->team == HalfLifeEngine::SDK::Constants::TeamID_AutoSelect)
		{
			if
			(
				g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_Terrorist].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) &&
				g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
			)
			{
				GetCreationTable ().RemoveAll ();	// Reset table of creation.

				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (GetBotsCount ());	// So set bot quota to maximum players.

				return false;
			}
		}
		else
		{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		// Is wanted loosing team?
		if (g_server->GetTeamScore (profile->team) + 5u < g_server->GetTeamScore (otherTeam))
//		if (g_server->GetTeamScore (profile->team) > g_server->GetTeamScore (otherTeam))
			profile->team = otherTeam;	// Choose other team to help them....

	const unsigned char clientsNumberInWantedTeam (g_server->GetClientManager ()->GetClientsCount (profile->team));

	/// @note If force team console variable string is empty or "unknown" for a bot - choose team by bot profile....

	// Is no more player spawn points in wanted team?
	if (clientsNumberInWantedTeam >= g_server->GetSpawnPointsContainer ()[profile->team].GetElementNumber ())
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		if (g_server->GetSpawnPointsContainer ()[otherTeam].GetElementNumber () <= g_server->GetClientManager ()->GetClientsCount (otherTeam))
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BothTeamsReachedMaximumPlayers) + '\n', g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist), g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));

			Label_RemoveCreationTableSetQuotaToMaximumPlayersAndReturnFailure:
			{
				GetCreationTable ().RemoveAll ();	// Reset table of creation.

				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (GetBotsCount ());	// So set bot quota to maximum players.

				// cancel if unable to create fake client, or choose team for fake client, or something else....
				return false;
			}
		}

		// Choose other team....
		profile->team = otherTeam;
	}
	else
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t otherTeam (profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist : HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

		// Is too many players in wanted team?
		if (clientsNumberInWantedTeam > g_server->GetClientManager ()->GetClientsCount (otherTeam)/* + 1u*/)
		{
			if (g_server->GetClientManager ()->GetClientsCount (otherTeam) < g_server->GetSpawnPointsContainer ()[otherTeam].GetElementNumber ())
				profile->team = otherTeam;	// Choose other team....
			else
			{
				const unsigned char limitTeams (HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetValue <unsigned char> ("mp_limitteams"));

				if (limitTeams > 0u && clientsNumberInWantedTeam + limitTeams <= g_server->GetClientManager ()->GetClientsCount (otherTeam))
				{
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BothTeamsReachedMaximumPlayers) + '\n', g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist), g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));

					goto Label_RemoveCreationTableSetQuotaToMaximumPlayersAndReturnFailure;
				}
			}
		}
	}
		}
	}

	/// @todo Prefix console variable validate checks (length, bracers, etc....).
	const DynamicString prefix (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Prefix)->GetString ());
	DynamicString detailedName;

	if (prefix.IsEmpty ())
	{
		// Whitespace, bracers and skill (Example: "BOT_NAME (BOT_SKILL)")
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SkillTags)->GetValue <bool> () && profile->name.GetElementNumber () + 3u + GetDigitsNumber (profile->skill.GetValue ()) < HalfLifeEngine::SDK::Constants::MaximumClientNameLength)
			detailedName.AssignFormat ("%s (%u)", profile->name.GetData (), profile->skill.GetValue ());
	}
	else
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SkillTags)->GetValue <bool> ())
		{
			// Prefix, whitespace (Example: "BOT_PREFIX BOT_NAME")
			if (prefix.GetElementNumber () + 1u + profile->name.GetElementNumber () < HalfLifeEngine::SDK::Constants::MaximumClientNameLength)
			{
				// Set detailed name with prefix,
				detailedName = prefix + ' ' + profile->name;

				// append skill if possible....

				// Whitespace, bracers and skill (Example: "BOT_PREFIX BOT_NAME (BOT_SKILL)")
				if (detailedName.GetElementNumber () + 3u + GetDigitsNumber (profile->skill.GetValue ()) < HalfLifeEngine::SDK::Constants::MaximumClientNameLength)
					detailedName.AppendFormat (" (%u)", profile->skill.GetValue ());
			}
			else if (profile->name.GetElementNumber () + 3u + GetDigitsNumber (profile->skill.GetValue ()) < HalfLifeEngine::SDK::Constants::MaximumClientNameLength)	// Whitespace, bracers and skill (Example: "BOT_NAME (BOT_SKILL)")
				detailedName.AssignFormat ("%s (%u)", profile->name.GetData (), profile->skill.GetValue ());
		}
		else if (prefix.GetElementNumber () + 1u + profile->name.GetElementNumber () < HalfLifeEngine::SDK::Constants::MaximumClientNameLength)	// Prefix, whitespace (Example: "BOT_PREFIX BOT_NAME")
			detailedName = prefix + ' ' + profile->name;
	}

	// Create the fake client entity.
	HalfLifeEngine::SDK::Classes::Edict *const YaPBEntity (HalfLifeEngine::Globals::g_halfLifeEngine->CreateFakeClient (detailedName.IsEmpty () || detailedName.GetElementNumber () >= HalfLifeEngine::SDK::Constants::MaximumClientNameLength ? profile->name : detailedName));

	// Reliability check.
	if (!YaPBEntity->IsValid ())
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_MaximumPlayersReached) + '\n', HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients (), HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ());

		goto Label_RemoveCreationTableSetQuotaToMaximumPlayersAndReturnFailure;
	}

	YaPB *const newYaPB (new YaPB (YaPBEntity, profile));

	#if defined _DEBUG
		// Reliability check.
		if (newYaPB == NULL)
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new bot \"%s\"!", sizeof (YaPB), profile->name.GetData ());

			goto Label_RemoveCreationTableSetQuotaToMaximumPlayersAndReturnFailure;
		}

		// Reliability check.
		if (!newYaPB->IsValid ())
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't create new bot \"%s\" entity!", profile->name.GetData ());

			delete newYaPB;

			goto Label_RemoveCreationTableSetQuotaToMaximumPlayersAndReturnFailure;
		}
	#endif	// if defined _DEBUG

	const unsigned char botIndex (static_cast <const unsigned char> (YaPBEntity->GetIndex ()) - 1u);

	g_server->GetClientManager ()->m_clients[botIndex] = newYaPB;

	g_server->GetClientManager ()->m_clientsArray += newYaPB;

	g_server->GetFakeClientManager ()->ClientConnect (newYaPB);	// YaPB is also fake client :)

	YaPBConnect (newYaPB);

	char rejectReason[128u] = {'\0'/*, ...*/};	// allocate space and reset the reject reason template string

	// let him connect to the server under its own name
	if (!HalfLifeEngine::Globals::g_halfLifeEngine->ClientConnect (YaPBEntity, YaPBEntity->GetNetName (), FormatBuffer ("127.0.0.%u", botIndex + 1u + 100u)/* build it an unique address */, rejectReason))
	{
		AddLogEntry (true, LogLevel_Warning, false, "Can't ClientConnect() for bot \"%s\"!", YaPBEntity->GetNetName ().GetData ());

		Label_KickInvalidBotAndReturnFailure:
		{
			// Kick the bot player if the server refused it.
			newYaPB->Kick ();

			// Kill this bot entity....
			YaPBEntity->variables.flags |= HalfLifeEngine::SDK::Constants::FL_KILLME;

			return false;
		}
	}

	if (!IsEmptyString (rejectReason))
	{
		AddLogEntry (true, LogLevel_Warning, false, "Server refused '%s' connection (%s).", YaPBEntity->GetNetName ().GetData (), rejectReason);

		goto Label_KickInvalidBotAndReturnFailure;
	}

	// print a notification message on the dedicated server console if in developer mode
	if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () && Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
	{
		if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <unsigned char> () == 2u)
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat
			(
				"\nServer requiring authentication.\n"
				"Client '%s' connected.\n"
				"Adress: 127.0.0.%u:27005.\n\n", YaPBEntity->GetNetName ().GetData (), botIndex + 1u + 100u
			);
		}

		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint
		(
			"Verifying and uploading resources...\n"
			"Custom resources total: 0 bytes.\n"
			"\tDecals: 0 bytes.\n"
			"----------------------\n"
			"Resources to request: 0 bytes.\n\n"
		);
	}

	/// @note We not call ours callbacks, cause we already add our bot to common clients array!
	g_server->GetClientManager ()->GetClientPutInServerHooker ().CallOriginalFunction (YaPBEntity);	// Let him actually join the game.

	for (HumanManager::HumansArray_t::IndexType_t index (0u); index < g_server->GetHumanManager ()->GetHumansCount (); ++index)
	{
		Human *const human (g_server->GetHumanManager ()->GetHumansArray ()[index]);

		if (human->GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1)
			g_menusManager.Draw (human);	// Redraw kick YaPB menu....
	}

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ConnectingBot) + '\n', YaPBEntity->GetNetName ().GetData (), profile->skill.GetValue ());

	// bot successfully created, so return...
	return true;
}

void YaPBManager::ReconnectBots (void)
{
	for (ConnectedYaPBsArray_t::IndexType_t index (0u); index < g_connectedYaPBs.GetElementNumber (); ++index)
	{
		
	}
}

void YaPBManager::AddBot (const DynamicString &name, const DynamicString &skill, const DynamicString &personality, const DynamicString &team, const DynamicString &model)
{
	// This function is same as the function above, but accept as parameters string instead of integers.

	const char blank[] = {'*', '_'};
	YaPB::Profile queueID;

	if (!name.IsEmpty () && name != blank[0u] && name != blank[1u])
		queueID.name = name;	// Fill the holder.

	if (!skill.IsEmpty () && skill != blank[0u] && skill != blank[1u])
	{
		const unsigned char skillValue (skill.GetValue <unsigned char> ());

		if (skillValue >= BotSkillValue_Minimum && skillValue <= BotSkillValue_Maximum)
			queueID.skill = skillValue;	// Fill the holder.
		else
			AddLogEntry (true, LogLevel_Error, false, "YaPBManager::AddBot(): Ignoring given skill value %s(%u) since it's out of range!", skill.GetData (), skillValue);
	}

	if (!team.IsEmpty () && team != blank[0u] && team != blank[1u])
		queueID.team = team.GetValue <HalfLifeEngine::SDK::Constants::TeamID_t> ();	// Fill the holder.

	if (!model.IsEmpty () && model != blank[0u] && model != blank[1u])
		queueID.model[queueID.team - 1u] = model.GetValue <unsigned char> ();	// Fill the holder.

	if (!personality.IsEmpty () && personality != blank[0u] && personality != blank[1u])
		queueID.personality = personality.GetValue <Personality_t> ();	// Fill the holder.

	AddBot (queueID);
}

void YaPBManager::FillServer (HalfLifeEngine::SDK::Constants::TeamID_t team, const Personality_t personality, const unsigned char skill, const unsigned char numToAdd)
{
	// This function fill server with bots, with specified team and personality.

	const unsigned char botsCount (GetBotsCount ());
	const unsigned char humansCount (g_server->GetHumanManager ()->GetHumansCount ());

	if (botsCount >= HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients () - humansCount)
		return;

	const char *const teamDescription[HalfLifeEngine::SDK::Constants::TeamID_AutoSelect + 1u] =
	{
		"",
		"Terrorists",
		"CTs",
		"",
		"",
		"Random"
	};
	const unsigned char toAdd (numToAdd == -1 ? HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients () - (humansCount + botsCount) : numToAdd);

	switch (team)
	{
		case HalfLifeEngine::SDK::Constants::TeamID_Terrorist:
		case HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist:
			HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_SetValue ("mp_limitteams",      0u);
			HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_SetValue ("mp_autoteambalance", false);

			break;

		default:
			team = HalfLifeEngine::SDK::Constants::TeamID_AutoSelect;
	}

	for (unsigned char index (0u); index <= toAdd; ++index)
		AddBot
		(
			"",

			// since we got constant skill from menu (since creation process call automatic), we need to manually randomize skill here, on given skill there
			skill >= BotSkillValue_Minimum && skill <= BotSkillLevel_Stupid ?
				g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillValue_Minimum, BotSkillLevel_Stupid) :
			skill >= BotSkillLevel_Stupid && skill <= BotSkillLevel_Newbie ?
				g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Stupid, BotSkillLevel_Newbie) :
			skill >= BotSkillLevel_Newbie && skill <= BotSkillLevel_Average ?
				g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Newbie, BotSkillLevel_Average) :
			skill >= BotSkillLevel_Average && skill <= BotSkillLevel_Advanced ?
				g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Average, BotSkillLevel_Advanced) :
			skill >= BotSkillLevel_Advanced && skill <= BotSkillLevel_Professional ?
				g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Advanced, BotSkillLevel_Professional) :
			skill == BotSkillValue_Maximum ?
				skill :
			BotSkillValue_Minimum,

			personality,
			team,
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModID () == HalfLifeEngine::Engine::Version_Steam ? static_cast <unsigned char> (HalfLifeEngine::SDK::Constants::PlayerModelID_AutoSelect) : static_cast <unsigned char> (HalfLifeEngine::SDK::Constants::PlayerModelID_ConditionZero_AutoSelect)
		);

	Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (toAdd);
	Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_QuotaMode)->SetString ("fill");

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_FillServerWithBots), teamDescription[team]);
}

void YaPBManager::ListBots (const Human *const/* human*/) const
{
	// this function list's bots currently playing on the server.

	if (GetBotsCount () == 0u)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');

		return;
	}

	char tempMessage[1024u];	// display some information
	unsigned short length (static_cast <unsigned short> (sprintf (tempMessage, "%-3.5s | %-9.13s | %-17.18s | %-3.4s | %-3.4s | %-3.4s\n\n", "index", "name", "personality", "team", "skill", "frags")));

	// loop through all bot slots
	for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
	{
		const YaPB *const bot (m_botsArray[index]);

		length += static_cast <unsigned short> (sprintf (tempMessage + length, "[%2u] | %-22s | %17.18s | %-3.4s | %-3.1u | %-3.1i\n", bot->GetEdict ()->GetIndex (), bot->GetName ().GetData (), bot->m_profile->personality == Personality_Normal ? "normal" : bot->m_profile->personality == Personality_Rusher ? "rusher" : "careful", bot->GetRealTeam () == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "T" : "CT", bot->m_profile->skill.GetValue (), static_cast <int> (bot->GetEdict ()->variables.frags)));
	}

	if (!HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
	{
		const Host *const hostClient (g_server->GetHumanManager ()->GetHostClient ());

		if (hostClient->IsValid ())
		{
			hostClient->DisplayMOTD (hostClient->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_MOTD_Title_BotsList), tempMessage);

			return;
		}
	}

	// Else
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (tempMessage);
}

void YaPBManager::SelectLeaderForTerrorists (void) const
{
	if (g_server->GetMapType () & Server::MapType_De)
	{
		// loop through all bot slots
		for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
		{
			YaPB *const bot (m_botsArray[index]);

			if (!bot->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4))
				continue;

			// bot carrying the bomb is the leader
			bot->m_isLeader = true;

			// terrorist carrying a bomb needs to have some company
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 80u)
			{
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						bot->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						bot->ChatterMessage (ChatterManager::ChatterMessage_GoingToPlantBomb);

						break;
				}
			}
			else if (g_server->IsTeamplay () && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 20u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) >= 4u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist) >= 4u && Console::Server::variableManager.GetVariable (Console::Server::VariableID_RoundTime)->GetValue <float> () >= 1.6f)
				bot->ChatterMessage (ChatterManager::ChatterMessage_LetsDoThis, g_randomNumberGenerator.GetValueBetween (bot->GetProfile ()->baseReactTime * 0.2f, bot->GetProfile ()->baseReactTime));

			return;
		}

		return;
	}

	YaPB *const botLeader (GetAliveHighestFragsBot (HalfLifeEngine::SDK::Constants::TeamID_Terrorist));

	if (!botLeader->IsValid ())
		return;

	botLeader->m_isLeader = true;

	if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 30u)
		botLeader->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe);
	else if (g_server->IsTeamplay () && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 20u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) >= 4u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist) >= 4u && Console::Server::variableManager.GetVariable (Console::Server::VariableID_RoundTime)->GetValue <float> () >= 1.6f)
		botLeader->ChatterMessage (ChatterManager::ChatterMessage_LetsDoThis, g_randomNumberGenerator.GetValueBetween (botLeader->GetProfile ()->baseReactTime * 0.2f, botLeader->GetProfile ()->baseReactTime));
}

void YaPBManager::SelectLeaderForCounterTerrorists (void) const
{
	if (g_server->GetMapType () & Server::MapType_As)
	{
		// loop through all bot slots
		for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
		{
			YaPB *const bot (m_botsArray[index]);

			if (!bot->IsVIP ())
				continue;

			// vip bot is the leader
			bot->m_isLeader = true;

			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 50u)
				bot->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe);
			else if (g_server->IsTeamplay () && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 20u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) >= 4u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist) >= 4u && Console::Server::variableManager.GetVariable (Console::Server::VariableID_RoundTime)->GetValue <float> () >= 1.6f)
				bot->ChatterMessage (ChatterManager::ChatterMessage_LetsDoThis, g_randomNumberGenerator.GetValueBetween (bot->GetProfile ()->baseReactTime * 0.2f, bot->GetProfile ()->baseReactTime));

			return;
		}

		return;
	}

	YaPB *const botLeader (GetAliveHighestFragsBot (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));

	if (!botLeader->IsValid ())
		return;

	botLeader->m_isLeader = true;

	if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 30u)
		botLeader->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe);
	else if (g_server->IsTeamplay () && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 20u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) >= 4u && g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist) >= 4u && Console::Server::variableManager.GetVariable (Console::Server::VariableID_RoundTime)->GetValue <float> () >= 1.6f)
		botLeader->ChatterMessage (ChatterManager::ChatterMessage_LetsDoThis, g_randomNumberGenerator.GetValueBetween (botLeader->GetProfile ()->baseReactTime * 0.2f, botLeader->GetProfile ()->baseReactTime));
}