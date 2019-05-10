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
// ClientManager.cpp
//
// Class: ClientManager
//
// Version: $ID:$
//

#include <Core.hpp>

void ClientManager::ClientTeamSwitchedToTerrorists (const unsigned char clientEdictIndex)
{
	// This function called after client/bot switch team to terrorists.

	// Reliability check.
	InternalAssert (HalfLifeEngine::Utilities::IsPlayerEdictIndex (clientEdictIndex));

	Client *const client (m_clients[clientEdictIndex - 1u]);

	// Reliability check.
//	InternalAssert (client->IsValid ());	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (!client->IsValid ())
		return;

	// Reliability check. (Same team?)
	if (client->m_currentTeamID == HalfLifeEngine::SDK::Constants::TeamID_Terrorist)
	{
		#if defined _DEBUG
			Label_Check:
			{
//	AddLogEntry (true, LogLevel_Default, true, "CM::ClientTeamSwitchedToTerrorists(): Called for client \"%s\", m_currentTeamID=%i, BasePlayer::m_team=%i, Edict::IsAlive()=%i, Client::IsAlive()=%i.", client->GetName ().GetData (), client->m_currentTeamID, client->GetRealTeam (), client->GetEdict ()->IsAlive (), client->IsAlive ());

				// Reliability checks.
				InternalAssert (m_teamClients[TeamArrayID_Terrorist].IsMember (client));
				InternalAssert (!m_teamClients[TeamArrayID_CounterTerrorist].IsMember (client));
				InternalAssert (client->IsAlive () == m_teamAliveClients[TeamArrayID_Terrorist].IsMember (client));
				InternalAssert (!m_teamAliveClients[TeamArrayID_CounterTerrorist].IsMember (client));
			}
		#endif	// if defined _DEBUG

		return;
	}

	if (m_teamClients[TeamArrayID_CounterTerrorist].Pop (client))
		m_teamAliveClients[TeamArrayID_CounterTerrorist] -= client;

	if (!m_teamClients[TeamArrayID_Terrorist].IsMember (client))
		m_teamClients[TeamArrayID_Terrorist] += client;

	if (client->IsAlive () && !m_teamAliveClients[TeamArrayID_Terrorist].IsMember (client))
		m_teamAliveClients[TeamArrayID_Terrorist] += client;

	if (client->IsFakeClient ())
	{
		g_server->GetFakeClientManager ()->ClientTeamSwitchedToTerrorists (client->GetFakeClientPointer ());

/*! @warning "New message started when msg '86' has not been sent yet" - @todo IMPROVE THIS
		if (client->IsYaPB ())
			for (HumanManager::HumansArray_t::IndexType_t index (0u); index < g_server->GetHumanManager ()->GetHumansCount (); ++index)
			{
				Human *const human (g_server->GetHumanManager ()->GetHumansArray ()[index]);

				if (human->GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1)
					g_menusManager.Draw (human);	// Redraw kick YaPB menu....
			}*/
	}

	client->TeamChanged (HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

	#if defined _DEBUG
		goto Label_Check;
	#endif	// if defined _DEBUG
}
void ClientManager::ClientTeamSwitchedToCounterTerrorists (const unsigned char clientEdictIndex)
{
	// This function called after client/bot switch team to counter-terrorists.

	// Reliability check.
	InternalAssert (HalfLifeEngine::Utilities::IsPlayerEdictIndex (clientEdictIndex));

	Client *const client (m_clients[clientEdictIndex - 1u]);

	// Reliability check.
//	InternalAssert (client->IsValid ());	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (!client->IsValid ())
		return;

	// Reliability check. (Same team?)
	if (client->m_currentTeamID == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
	{
		#if defined _DEBUG
			Label_Check:
			{
//	AddLogEntry (true, LogLevel_Default, true, "CM::ClientTeamSwitchedToCounterTerrorists(): Called for client \"%s\", m_currentTeamID=%i, BasePlayer::m_team=%i, Edict::IsAlive()=%i, IsAlive()=%i.", client->GetName ().GetData (), client->m_currentTeamID, client->GetRealTeam (), client->GetEdict ()->IsAlive (), client->IsAlive ());

				// Reliability checks.
				InternalAssert (!m_teamClients[TeamArrayID_Terrorist].IsMember (client));
				InternalAssert (m_teamClients[TeamArrayID_CounterTerrorist].IsMember (client));
				InternalAssert (!m_teamAliveClients[TeamArrayID_Terrorist].IsMember (client));
				InternalAssert (client->IsAlive () == m_teamAliveClients[TeamArrayID_CounterTerrorist].IsMember (client));
			}
		#endif	// if defined _DEBUG

		return;
	}

	if (m_teamClients[TeamArrayID_Terrorist].Pop (client))
		m_teamAliveClients[TeamArrayID_Terrorist] -= client;

	if (!m_teamClients[TeamArrayID_CounterTerrorist].IsMember (client))
		m_teamClients[TeamArrayID_CounterTerrorist] += client;

	if (client->IsAlive () && !m_teamAliveClients[TeamArrayID_CounterTerrorist].IsMember (client))
		m_teamAliveClients[TeamArrayID_CounterTerrorist] += client;

	if (client->IsFakeClient ())
	{
		g_server->GetFakeClientManager ()->ClientTeamSwitchedToCounterTerrorists (client->GetFakeClientPointer ());

/*! @warning "New message started when msg '86' has not been sent yet" - @todo IMPROVE THIS
		if (client->IsYaPB ())
			for (HumanManager::HumansArray_t::IndexType_t index (0u); index < g_server->GetHumanManager ()->GetHumansCount (); ++index)
			{
				Human *const human (g_server->GetHumanManager ()->GetHumansArray ()[index]);

				if (human->GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1)
					g_menusManager.Draw (human);	// Redraw kick YaPB menu....
			}*/
	}

	client->TeamChanged (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);

	#if defined _DEBUG
		goto Label_Check;
	#endif	// if defined _DEBUG
}
void ClientManager::ClientTeamSwitchedToSpectators (const unsigned char clientEdictIndex, const bool isNewTeamUnassigned)
{
	// This function called after client/bot switch team to spectators.

	// Reliability check.
	InternalAssert (HalfLifeEngine::Utilities::IsPlayerEdictIndex (clientEdictIndex));

	Client *const client (m_clients[clientEdictIndex - 1u]);

	// Reliability check.
//	InternalAssert (client->IsValid ());	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (!client->IsValid ())
		return;

	const HalfLifeEngine::SDK::Constants::TeamID_t newTeamID (isNewTeamUnassigned ? HalfLifeEngine::SDK::Constants::TeamID_Unassigned : HalfLifeEngine::SDK::Constants::TeamID_Spectator);

	// Reliability check. (Same team?)
	if (client->m_currentTeamID == newTeamID)
	{
		#if defined _DEBUG
			Label_Check:
			{
//	AddLogEntry (true, LogLevel_Default, true, "CM::ClientTeamSwitchedToSpectators(): Called for client \"%s\", m_currentTeamID=%i, BasePlayer::m_team=%i, Edict::IsAlive()=%i, IsAlive()=%i.", client->GetName ().GetData (), client->m_currentTeamID, client->GetRealTeam (), client->GetEdict ()->IsAlive (), client->IsAlive ());

				// Reliability checks.
				InternalAssert (!m_teamClients[TeamArrayID_Terrorist].IsMember (client));
				InternalAssert (!m_teamAliveClients[TeamArrayID_Terrorist].IsMember (client));
				InternalAssert (!m_teamClients[TeamArrayID_CounterTerrorist].IsMember (client));
				InternalAssert (!m_teamAliveClients[TeamArrayID_CounterTerrorist].IsMember (client));
			}
		#endif	// if defined _DEBUG

		return;
	}

	// Just remove this client....
	if (m_teamClients[TeamArrayID_Terrorist].Pop (client))
		m_teamAliveClients[TeamArrayID_Terrorist] -= client;
	else if (m_teamClients[TeamArrayID_CounterTerrorist].Pop (client))
		m_teamAliveClients[TeamArrayID_CounterTerrorist] -= client;

	if (client->IsFakeClient ())
	{
		g_server->GetFakeClientManager ()->ClientTeamSwitchedToSpectators (client->GetFakeClientPointer (), isNewTeamUnassigned);

/*! @warning "New message started when msg '86' has not been sent yet" - @todo IMPROVE THIS
		if (client->IsYaPB ())
			for (HumanManager::HumansArray_t::IndexType_t index (0u); index < g_server->GetHumanManager ()->GetHumansCount (); ++index)
			{
				Human *const human (g_server->GetHumanManager ()->GetHumansArray ()[index]);

				if (human->GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1)
					g_menusManager.Draw (human);	// Redraw kick YaPB menu....
			}*/
	}

	client->TeamChanged (newTeamID);

	#if defined _DEBUG
		goto Label_Check;
	#endif	// if defined _DEBUG
}

void ClientManager::ClientSpawnPost (const HalfLifeEngine::SDK::Classes::Edict *const client)
{
	// This function called after each client/bot spawn, also after bot creation.
	/// @warning IN THIS FUNCTION WE SHOULD USE ONLY 'HalfLifeEngine::SDK::Classes::Edict::IsAlive()' FUNCTION!

	// Reliability checks.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());

	const ClientsArray_t::IndexType_t clientIndex (static_cast <const ClientsArray_t::IndexType_t> (client->GetIndex ()) - 1u);

	// Reliability check.
	InternalAssert (m_clients[clientIndex]->IsValid ());

	m_clients[clientIndex]->SpawnPost ();

	// Not alive?
	if (!m_clients[clientIndex]->GetEdict ()->IsAlive ())
		return;

	char clientRealTeam (static_cast <char> (m_clients[clientIndex]->GetRealTeam ()));

	// Didn't join a team yet?
	if (clientRealTeam == HalfLifeEngine::SDK::Constants::TeamID_Unassigned)
		return;

	// Player spawned being alive.

	--clientRealTeam;

	// Client can spawn twice being alive!
	if (!m_teamAliveClients[clientRealTeam].IsMember (m_clients[clientIndex]))
	{
		m_teamAliveClients[clientRealTeam] += m_clients[clientIndex];
//	AddLogEntry (true, LogLevel_Default, true, "CM::ClientSpawnPost(): Called for client \"%s\", m_currentTeamID=%i, BasePlayer::m_team=%i.", m_clients[clientIndex]->GetName ().GetData (), m_clients[clientIndex]->m_currentTeamID, m_clients[clientIndex]->GetRealTeam ());
	}

	if (m_clients[clientIndex]->IsFakeClient ())
		g_server->GetFakeClientManager ()->ClientAliveSpawnPost (m_clients[clientIndex]->GetFakeClientPointer ());

	m_clients[clientIndex]->AliveSpawnPost ();
}

void ClientManager::ClientRoundRespawnPost (const HalfLifeEngine::SDK::Classes::Edict *const client)
{
	// This function called after each client/bot spawn, also after bot creation.

	// Reliability checks.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());

	const ClientsArray_t::IndexType_t clientIndex (static_cast <const ClientsArray_t::IndexType_t> (client->GetIndex ()) - 1u);

	// Reliability check.
	InternalAssert (m_clients[clientIndex]->IsValid ());

	m_clients[clientIndex]->RoundRespawnPost ();
}

void ClientManager::ClientUse (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const entity, const HalfLifeEngine::SDK::Classes::Edict *const caller, const HalfLifeEngine::SDK::Constants::UseType_t type, const float value) const
{
	// Reliability check.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
	InternalAssert (caller->IsValid ());

	const ClientsArray_t::IndexType_t clientIndex (static_cast <const ClientsArray_t::IndexType_t> (client->GetIndex ()) - 1u);

	// Reliability check.
	InternalAssert (m_clients[clientIndex]->IsValid ());
/*
	if (!client->IsYaPB ())
	{
		// loop through all client/bot slots
		for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
		{
			// if this client slot not used, or client isn't this bot (YaPB), or bot has no target button
			if (!m_clientsArray[index]->IsYaPB () || !m_clientsArray[index]->GetYaPBPointer ()->m_button->IsValid () || m_clientsArray[index]->GetYaPBPointer ()->m_button != entity)
				continue;

			// button was pressed by another client/bot, so reset button target
			m_clientsArray[index]->GetYaPBPointer ()->m_button = NULL;
		}
	}
*/
	m_clients[clientIndex]->Use (entity, caller, type, value);
}

void ClientManager::ClientDisconnect (const HalfLifeEngine::SDK::Classes::Edict *const client)
{
	// This function frees one client selected by index (used on client disconnect).

	// Reliability check.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
/*
	#if defined _DEBUG
		AddLogEntry (true, LogLevel_Default, false, "ClientManager::ClientDisconnect(): called for client: \"%s\".", client->GetNetName ().GetData ());
	#endif	// if defined _DEBUG
*/
	const ClientsArray_t::IndexType_t clientIndex (static_cast <const ClientsArray_t::IndexType_t> (client->GetIndex ()) - 1u);

	// Reliability check.
	InternalAssert (m_clients[clientIndex]->IsValid ());

	// Record some Stats of all Players on the Server
	{
		m_clientsArray -= m_clients[clientIndex];

		if (m_clients[clientIndex]->IsFakeClient ())
		{
			g_server->GetFakeClientManager ()->ClientDisconnect (m_clients[clientIndex]->GetFakeClientPointer ());

			if (m_clients[clientIndex]->IsOtherBot ())
				g_server->GetFakeClientNotYaPBManager ()->FakeClientNotYaPBDisconnect (m_clients[clientIndex]->GetFakeClientNotYaPBPointer ());
			else if (m_clients[clientIndex]->IsYaPB ())
			{
				g_server->GetYaPBManager ()->YaPBDisconnect (m_clients[clientIndex]->GetYaPBPointer ());

				for (HumanManager::HumansArray_t::IndexType_t index (0u); index < g_server->GetHumanManager ()->GetHumansCount (); ++index)
				{
					Human *const human (g_server->GetHumanManager ()->GetHumansArray ()[index]);

					if (human->GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1)
						g_menusManager.Draw (human);	// Redraw kick YaPB menu....
				}
			}
		}
		else
		{
			// Reliability check.
			InternalAssert (m_clients[clientIndex]->IsHuman ());

			g_server->GetHumanManager ()->HumanDisconnect (m_clients[clientIndex]->GetHumanPointer ());
		}

		{
			char clientRealTeam (static_cast <char> (m_clients[clientIndex]->GetRealTeam ()));

			if (!HalfLifeEngine::Utilities::IsSpectatorTeam (static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (clientRealTeam)))
			{
				--clientRealTeam;

				m_teamClients[clientRealTeam] -= m_clients[clientIndex];

				if (m_clients[clientIndex]->IsAlive ())
				{
InternalAssert (m_teamAliveClients[clientRealTeam].IsMember (m_clients[clientIndex]));
//if (!m_teamAliveClients[clientRealTeam].IsMember (m_clients[clientIndex]))
//	AddLogEntry (false, LogLevel_Critical, true, "client \"%s\" [IsAlive () && !m_teamAliveClients[clientRealTeam].IsMember (client)].", m_clients[clientIndex]->GetName ().GetData ());

					m_teamAliveClients[clientRealTeam] -= m_clients[clientIndex];
				}
				#if defined _DEBUG
					else
						InternalAssert (!m_teamAliveClients[clientRealTeam].IsMember (m_clients[clientIndex]));
				#endif	// if defined _DEBUG
			}
		}

		if (m_clients[clientIndex]->m_lastNavigationArea != NULL)
			m_clients[clientIndex]->m_lastNavigationArea->RemovePlayer (m_clients[clientIndex]);

		g_server->GetYaPBManager ()->OnClientDisconnect (m_clients[clientIndex]);
	}

	// Delete him/her/it from the list....
	delete m_clients[clientIndex];	// Call destructor for this client,

	m_clients[clientIndex] = NULL;	// and reset pointer.
}

void ClientManager::ClientPutInServer (HalfLifeEngine::SDK::Classes::Edict *const client)
{
	/// @note This function is not called for YaPB's!

	// Reliability check.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
/*
	#if defined _DEBUG
		AddLogEntry (true, LogLevel_Default, false, "ClientManager::ClientPutInServer(): called for client: \"%s\".", client->GetNetName ().GetData ());
	#endif	// if defined _DEBUG
*/
	const ClientsArray_t::IndexType_t clientIndex (static_cast <const ClientsArray_t::IndexType_t> (client->GetIndex ()) - 1u);

	// Reliability check.
	InternalAssert (m_clients[clientIndex] == NULL);

	if (client->IsPlayerHuman ())
	{
		g_server->GetYaPBManager ()->CheckAutoVacate ();

		#if defined _DEBUG
		if ((GetClientsCount () == 0u && !HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ()) || HalfLifeEngine::SDK::Network::NET_IsReservedAdr (g_server->GetStatic ().clients[clientIndex].netchan.remote_address))
		#else
		// If first human connect and listen server - create the host. (Host is always first :))
		if (GetClientsCount () == 0u && !HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
		#endif	// if !defined _DEBUG
		{
			Host *const newHost (new Host (client));

			#if defined _DEBUG
				// Reliability check.
				if (newHost == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating host \"%s\"!", sizeof (*newHost), client->GetNetName ().GetData ());

					return;
				}
			#endif	// if defined _DEBUG

			// add him/her to the lists
			m_clients[clientIndex] = newHost;

			g_server->GetHumanManager ()->HumanConnect (newHost);
/*
			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Default, false, "ClientManager::ClientPutInServer(): host \"%s\" created succesfully! (0x%u)", client->GetNetName ().GetData (), sizeof (*newHost));
			#endif	// if defined _DEBUG
*/		}
		else	// Else - create player.
		{
			Player *const newPlayer (new Player (client));

			#if defined _DEBUG
				// Reliability check.
				if (newPlayer == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating player \"%s\"!", sizeof (*newPlayer), client->GetNetName ().GetData ());

					return;
				}
			#endif	// if defined _DEBUG

			// add him/her to the lists
			m_clients[clientIndex] = newPlayer;

			g_server->GetHumanManager ()->HumanConnect (newPlayer);
/*
			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Default, false, "ClientManager::ClientPutInServer(): player \"%s\" created succesfully! (0x%u)", client->GetNetName ().GetData (), sizeof (*newPlayer));
			#endif	// if defined _DEBUG
*/		}
	}
	else if (client->IsPlayerZBot ())
	{
		ZBot *const newZBot (new ZBot (client));

		#if defined _DEBUG
			// Reliability check.
			if (newZBot == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating Z bot \"%s\"!", sizeof (*newZBot), client->GetNetName ().GetData ());

				return;
			}
		#endif	// if defined _DEBUG

		// add him/her to the lists
		m_clients[clientIndex] = newZBot;

		g_server->GetFakeClientManager ()->ClientConnect (newZBot);
//		g_server->GetZBotManager ()->ZBotConnect (newZBot);
/*
		#if defined _DEBUG
			AddLogEntry (true, LogLevel_Default, false, "ClientManager::ClientPutInServer(): Z bot \"%s\" created succesfully! (0x%u)", client->GetNetName ().GetData (), sizeof (*newZBot));
		#endif	// if defined _DEBUG
*/	}
	else
	{
		FakeClient_NotYaPB *const newFakeClientNotYaPB (new FakeClient_NotYaPB (client));

		#if defined _DEBUG
			// Reliability check.
			if (newFakeClientNotYaPB == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating other bot \"%s\"!", sizeof (*newFakeClientNotYaPB), client->GetNetName ().GetData ());

				return;
			}
		#endif	// if defined _DEBUG

		// add him/her to the lists
		m_clients[clientIndex] = newFakeClientNotYaPB;

		g_server->GetFakeClientManager ()->ClientConnect (newFakeClientNotYaPB);
		g_server->GetFakeClientNotYaPBManager ()->FakeClientNotYaPBConnect (newFakeClientNotYaPB);
/*
		#if defined _DEBUG
			AddLogEntry (true, LogLevel_Default, false, "ClientManager::ClientPutInServer(): other bot \"%s\" created succesfully! (0x%u)", client->GetNetName ().GetData (), sizeof (*newFakeClientNotYaPB));
		#endif	// if defined _DEBUG
*/	}

	m_clientsArray += m_clients[clientIndex];
}

void ClientManager::ClientUserInfoChanged (HalfLifeEngine::SDK::Classes::Edict *const client, const DynamicString &infoBuffer)
{
	// Reliability check.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
/*
	#if defined _DEBUG
		AddLogEntry (true, LogLevel_Default, false, "ClientManager::ClientUserInfoChanged(): called for client: \"%s\", infoBuffer: \"%s\".", client->GetNetName ().GetData (), infoBuffer.GetData ());
	#endif	// if defined _DEBUG
*/
	const ClientsArray_t::IndexType_t clientIndex (static_cast <const ClientsArray_t::IndexType_t> (client->GetIndex ()) - 1u);

	// Client::IsHuman() check is added since if client entity is bot, flag 'HalfLifeEngine::SDK::Constants::FL_FAKECLIENT' is not added, yet.
	if (m_clients[clientIndex]->IsValid () && m_clients[clientIndex]->IsHuman ())
		m_clients[clientIndex]->GetHumanPointer ()->UserInfoChanged (infoBuffer);
}

void ClientManager::Think (void) const
{
	// This function calls Think() function for all available at call moment clients, and try to catch internal error if such shit occurs.

	// Loop through all client/bot slots....
	for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
	{
		// if think time > world time
		if (m_clientsArray[index]->GetThinkIntervalTime () > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			continue;

			// Use these try-catch blocks to prevent server crashes when error occurs
		#if !defined NDEBUG && !defined _DEBUG
			try
			{
				m_clientsArray[index]->Think ();
			}
			catch (...)
			{
				// Error occurred. Kick off all bots and then print a warning message.
				KickAll ();

				AddLogEntry (true, LogLevel_Error, true, "**** INTERNAL " PRODUCT_SHORT_NAME " ERROR! PLEASE SHUTDOWN AND RESTART YOUR SERVER! ****");
			}
		#else
			m_clientsArray[index]->Think ();
		#endif	// if !defined NDEBUG && !defined _DEBUG

		// randomness prevents all bots to run on same frame -> less laggy server
		m_clientsArray[index]->m_thinkIntervalTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1u / Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ThinkFPS)->GetValue <unsigned char> () * g_randomNumberGenerator.GetValueBetween (0.97f, 1.05f);
	}

	for (unsigned short index (0u); index < g_origins.GetElementNumber (); ++index)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (g_origins[index], 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (g_server->GetHumanManager ()->GetHostClient ()->GetOrigin (), g_origins[index], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
	}
/*/
	for (unsigned short index (1u); index < g_origins.GetElementNumber (); ++index)
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (g_origins[index - 1u], g_origins[index], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
*/
	// Find the grenade entity....
	for (HalfLifeEngine::SDK::Classes::Edict *grenade (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "grenade")); grenade->IsValid (); grenade = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (grenade, "grenade"))
	{
		if (grenade->variables.gravity == 0.0f || grenade->variables.velocity == Math::Vector3D::ZeroValue || grenade->variables.owner == NULL || grenade->variables.damageTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			continue;

		InternalAssert (Math::AreEqual (Math::GetMidPoint (grenade->variables.absoluteBoundingBox.maxs.x, grenade->variables.absoluteBoundingBox.mins.x), grenade->variables.origin.x));
		InternalAssert (Math::AreEqual (Math::GetMidPoint (grenade->variables.absoluteBoundingBox.maxs.y, grenade->variables.absoluteBoundingBox.mins.y), grenade->variables.origin.y));
		InternalAssert (Math::AreEqual (grenade->variables.absoluteBoundingBox.mins.z + 1.0f, grenade->variables.origin.z));
		InternalAssert (grenade->variables.viewOffset == Math::Vector3D::ZeroValue);
		InternalAssert (grenade->variables.angleVelocity == Math::Angles3D::ZeroValue);

//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Grenade: type=%i(& 3 = %i), owner team=%i.\n", grenade->GetPrivateDataReference <int> (HalfLifeEngine::SDK::Constants::OFFSET_Grenade_TypeID), grenade->GetPrivateDataReference <int> (HalfLifeEngine::SDK::Constants::OFFSET_Grenade_TypeID) & 3, grenade->GetPrivateDataReference <int> (HalfLifeEngine::SDK::Constants::OFFSET_Grenade_HE_OwnerTeam));
	}
#if 0
	const unsigned char MAX_POINTS (32u);	// the total amount of points to store
	static Math::Vector3D vOrigins[MAX_POINTS];
	static unsigned char numO (0u);
	static const unsigned char maxNumO (255u);
	static HalfLifeEngine::SDK::Classes::Edict *lastGrenade (NULL);

//	if (vOrigins[0u].x != 0.0f)
//		for (unsigned char index (0u); index < MAX_POINTS; ++index)
//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (vOrigins[index], 3.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
	if (numO > 0u)
		for (unsigned char index (0u); index < numO; ++index)
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (vOrigins[index], 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "frameTime=%lf.", HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime);

	if (numO >= maxNumO)
		return;

	HalfLifeEngine::Utilities::PseudoGrenadeEntityVariables_t pseudoGrenadeEntityVariables;

	// Find the grenade entity....
	for (HalfLifeEngine::SDK::Classes::Edict *grenade (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "grenade")); grenade->IsValid (); grenade = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (grenade, "grenade"))
	{
		if (grenade->variables.gravity == 0.0f || grenade->variables.velocity == Math::Vector3D::ZeroValue || grenade->variables.owner == NULL || grenade->variables.damageTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () || grenade == lastGrenade)
			continue;

		HalfLifeEngine::Utilities::TraceThrownGrenadeToss (grenade, pseudoGrenadeEntityVariables);

//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (grenade->variables.origin, pseudoGrenadeEntityVariables.origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (pseudoGrenadeEntityVariables.origin, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

		vOrigins[numO] = pseudoGrenadeEntityVariables.origin;

		++numO;

		lastGrenade = grenade;
/*
//		const Math::Vector3D s (grenade->variables.absoluteBoundingBox.maxs - grenade->variables.absoluteBoundingBox.mins);
		const Math::Vector3D s (grenade->variables.absoluteBoundingBox.maxs.GetMidPoint (grenade->variables.absoluteBoundingBox.mins));
		const Math::Vector3D s1 (grenade->variables.origin);

//		AddLogEntry (false, LogLevel_Critical, true, "s=%s.", s.ToString ());
		AddLogEntry (false, LogLevel_Critical, true, "s=[%lf, %lf, %lf], s1=[%lf, %lf, %lf].", s.x, s.y, s.z, s1.x, s1.y, s1.z);
/*		AddLogEntry (false, LogLevel_Critical, true, "viewOffset=%s.", grenade->variables.viewOffset.ToString ());
*/
		InternalAssert (Math::AreEqual (Math::GetMidPoint (grenade->variables.absoluteBoundingBox.maxs.x, grenade->variables.absoluteBoundingBox.mins.x), grenade->variables.origin.x));
		InternalAssert (Math::AreEqual (Math::GetMidPoint (grenade->variables.absoluteBoundingBox.maxs.y, grenade->variables.absoluteBoundingBox.mins.y), grenade->variables.origin.y));
		InternalAssert (Math::AreEqual (grenade->variables.absoluteBoundingBox.mins.z + 1.0f, grenade->variables.origin.z));
		InternalAssert (grenade->variables.viewOffset == Math::Vector3D::ZeroValue);
		InternalAssert (grenade->variables.angleVelocity == Math::Angles3D::ZeroValue);
/*
		static unsigned int c (0u);

		grenade->Print (FormatBuffer ("C:/Users/Ýäóàðä/Desktop/Grenade/%s_%u.log", grenade->GetModel ().GetData () + 9u, c));

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "waterLevel=%i, on grnd=%u, part grnd=%u, grnd ent=%s.", grenade->variables.waterLevel, (grenade->variables.flags & HalfLifeEngine::SDK::Constants::FL_ONGROUND), (grenade->variables.flags & HalfLifeEngine::SDK::Constants::FL_PARTIALGROUND), grenade->variables.groundEntity != NULL ? grenade->variables.groundEntity->GetClassName ().GetData () : "NULL");
/*
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

		for ()
		{
		}
*/#if 0
	// ============= VERSION 2 ===============
	// Uses maximum points and memory arrays
	// =======================================

	// ...
	// grenade = entity to find trajectory

//	Math::Vector3D vOrigins[MAX_POINTS];
//	float fTimes[MAX_POINTS];
	const Math::Vector3D vStartOrigin (grenade->variables.origin);
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceToss (grenade, /*grenade*/grenade->variables.owner, traceResult);

	const Math::Vector3D vStopOrigin (traceResult.endPosition);
	const Math::Vector3D vVelocity (grenade->variables.velocity);
	const float fGravity (grenade->variables.gravity * Console::Server::variableManager.GetVariable (Console::Server::VariableID_Gravity)->GetValue <float> ());
	const float fTotalTime ((vVelocity.z + Math::sqrtf ((Math::powf (vVelocity.z, 2.0f) + (2.0f * fGravity * (vStartOrigin.z - vStopOrigin.z))))) / fGravity);
	float fPercent;
	float fTime;

	for (unsigned char index (0u); index < MAX_POINTS; ++index)
	{
		fPercent = static_cast <float> (index) / MAX_POINTS;
		fTime = fTotalTime * fPercent;

		vOrigins[index].x = vStartOrigin.x + (fPercent * (vStopOrigin.x - vStartOrigin.x));
		vOrigins[index].y = vStartOrigin.y + (fPercent * (vStopOrigin.y - vStartOrigin.y));
		vOrigins[index].z = vStartOrigin.z - (Math::powf (fTime, 2.0f) * fGravity / 2.0f) + (fTime * vVelocity.z);
//		fTimes[index] = fTime;
	}
#endif	// if 0
	}
#endif	// if 0
}

void ClientManager::EmitSound (const HalfLifeEngine::SDK::Classes::Edict *const entity, const DynamicString &sample, const float volume, const float attenuation, const HalfLifeEngine::SDK::Constants::SoundFlag_t flags, const HalfLifeEngine::SDK::Constants::SoundPitch_t pitch) const
{
	// This function called by the sound hooking code (in pfnEmitSound()) enters the played sound into the array associated with the entity.

	const HalfLifeEngine::SDK::Classes::Edict *clientEdict (entity);

	if (clientEdict->IsPlayer ())
	{
		Label_CallEmitSoundCallbackAndReturn:
		{
			const ClientsArray_t::IndexType_t clientIndex (static_cast <const ClientsArray_t::IndexType_t> (clientEdict->GetIndex ()) - 1u);

			// Reliability check.
			InternalAssert (m_clients[clientIndex]->IsValid ());

			if (m_clients[clientIndex]->IsAlive ())
				m_clients[clientIndex]->EmitSound (sample, volume, attenuation, flags, pitch);

			return;
		}
	}

	// is owner of object? (For spraylogo sound....)
	if (entity->variables.owner != NULL && entity->variables.owner->IsPlayer ())
	{
		clientEdict = entity->variables.owner;

		goto Label_CallEmitSoundCallbackAndReturn;
	}
/*
	{
	const DynamicString fullFilename ("/sound/" + sample);
	const DynamicString CSfullFilename (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + fullFilename);
	const float maximumLastingTime (STDIOFile::IsExists (CSfullFilename, "rb") ? GetWaveSoundPlayDuration (CSfullFilename) : GetWaveSoundPlayDuration ("valve" + fullFilename));

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::EmitSound(): sample: \"%s\", vol=%.1f, attn=%.2f, radius=%.2f, dur=%.2f.", entity->GetClassName ().GetData (), sample.GetData (), volume, attenuation, SOUND_ATTENUATION_TO_RADIUS (attenuation), maximumLastingTime);
	}*/
#if 0
	// Else....

	const Math::Vector3D origin (entity->GetOrigin ());
	float distanceSquared, nearestClientDistanceSquared (MATH_GET_SQUARED (512.0f));
	char nearestClientIndex (-1);

	// Loop through all players....
	for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
	{
		// now find nearest player
		if (!m_clientsArray[index]->IsAlive () || (distanceSquared = m_clientsArray[index]->GetOrigin ().GetDistanceSquared (origin)) >= nearestClientDistanceSquared)
			continue;

		nearestClientIndex = index;	// keep track of the closest found player
		nearestClientDistanceSquared = distanceSquared;	// update nearest distance
	}

	if (nearestClientIndex != -1)
		m_clientsArray[static_cast <ClientsArray_t::IndexType_t> (nearestClientIndex)]->EmitSound (sample, volume, attenuation, flags, pitch);
#endif	// if 0
}