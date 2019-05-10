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
// ClientManagerTemplate.hpp
//
// Class: ClientManagerTemplate
//
// Description: The base class for 'ClientManager', 'HumanManager', 'OtherBotManager' and 'YaPBManager'.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CLIENT_MANAGER_TEMPLATE_INCLUDED
#define CLIENT_MANAGER_TEMPLATE_INCLUDED

template <typename clientType> class ClientManagerTemplate
{
	// It's my best friends! :{P
	friend ClientManager;
	friend FakeClientManager;
	friend FakeClientNotYaPBManager;
	friend HumanManager;
	friend YaPBManager;

	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <clientType *, unsigned char> ClientsArray_t;

	//
	// Group: Protected members.
	//
	protected:
		ClientsArray_t m_clientsArray;							// For quickly clients looping/counting.

		// Real-time clients number statistics....
		ClientsArray_t m_teamClients[TeamArrayID_Total];		// For quickly team clients looping/counting.
		ClientsArray_t m_teamAliveClients[TeamArrayID_Total];	// For quickly team alive clients looping/counting. (@note this can be only modified on player change team, spawn being alive or killed)

	//
	// Group: Private operators.
	//
	private:
		inline ClientManagerTemplate &operator = (const ClientManagerTemplate &/*right*/);	// Avoid "warning C4512: 'ClientManagerTemplate' : assignment operator could not be generated".

	//
	// Group: Private functions.
	//
	private:
		inline void AddClient (clientType *const client) { m_clientsArray += client; }
		inline void RemoveClient (clientType *const client)
		{
//			AddLogEntry (true, LogLevel_Default, true, "FCM::RemoveClient(): Called for client \"%s\".", client->GetName ().GetData ());

			m_clientsArray -= client;

			{
				char clientRealTeam (static_cast <char> (client->GetRealTeam ()));

				if (!HalfLifeEngine::Utilities::IsSpectatorTeam (static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (clientRealTeam)))
				{
					--clientRealTeam;

					m_teamClients[clientRealTeam] -= client;

					if (client->IsAlive ())
					{
InternalAssert (m_teamAliveClients[clientRealTeam].IsMember (client));
//if (!m_teamAliveClients[clientRealTeam].IsMember (client))
//	AddLogEntry (false, LogLevel_Critical, true, "client \"%s\" [IsAlive () && !m_teamAliveClients[clientRealTeam].IsMember (client)].", client->GetName ().GetData ());

						m_teamAliveClients[clientRealTeam] -= client;
					}
					#if defined _DEBUG
						else
							InternalAssert (!m_teamAliveClients[clientRealTeam].IsMember (client));
					#endif	// if defined _DEBUG
				}
			}
		}

	//
	// Group: Functions.
	//
	public:
		inline       ClientsArray_t &GetClientsArray (void)
		{
			// This function returns a reference to non-constant clients array.

			return m_clientsArray;
		}
		inline const ClientsArray_t &GetClientsArray (void) const
		{
			// This function returns a reference to constant clients array.

			return m_clientsArray;
		}

		inline       ClientsArray_t &GetTeamClientsArray (const TeamArrayID_t teamArrayID)
		{
			// This function returns a reference to non-constant team clients array.

			return m_teamClients[teamArrayID];
		}
		inline const ClientsArray_t &GetTeamClientsArray (const TeamArrayID_t teamArrayID) const
		{
			// This function returns a reference to constant team clients array.

			return m_teamClients[teamArrayID];
		}
		inline       ClientsArray_t &GetTeamAliveClientsArray (const TeamArrayID_t teamArrayID)
		{
			// This function returns a reference to non-constant team alive clients array.

			return m_teamAliveClients[teamArrayID];
		}
		inline const ClientsArray_t &GetTeamAliveClientsArray (const TeamArrayID_t teamArrayID) const
		{
			// This function returns a reference to constant team alive clients array.

			return m_teamAliveClients[teamArrayID];
		}

		inline const typename ClientsArray_t::IndexType_t GetAliveClientsNumber     (const TeamArrayID_t realTeamArrayIndex)                  const { return m_teamAliveClients[realTeamArrayIndex].GetElementNumber (); }
		inline const typename ClientsArray_t::IndexType_t GetAliveClientsNumber     (const HalfLifeEngine::SDK::Constants::TeamID_t realTeam) const { return GetAliveClientsNumber (static_cast <TeamArrayID_t> (realTeam - 1u)); }
//		inline const typename ClientsArray_t::IndexType_t GetAliveFakeClientsNumber (const HalfLifeEngine::SDK::Constants::TeamID_t realTeam) const { return m_teamAliveFakeClients[realTeam - 1u].GetElementNumber (); }
//		inline const typename ClientsArray_t::IndexType_t GetAliveHumansNumber      (const HalfLifeEngine::SDK::Constants::TeamID_t realTeam) const { return m_teamAliveHumans[realTeam - 1u].GetElementNumber (); }

		inline const bool IsPointVisibleToTeam (const Math::Vector3D &position, const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return IsPointVisibleToTeam (position, static_cast <TeamArrayID_t> (teamID - 1u));
		}
		inline const bool IsPointVisibleToTeam (const Math::Vector3D &position, const TeamArrayID_t teamArrayID) const
		{
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

			for (ClientsArray_t::IndexType_t index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
			{
				clientType *const client (m_teamAliveClients[teamArrayID][index]);

				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (client->GetEyePosition (), position, HalfLifeEngine::SDK::Constants::TraceIgnore_All, *client, traceResult);

				if (traceResult.fraction == 1.0f)
					return true;
			}

			return false;
		}
		inline clientType *const GetClosestClientToPosition (const Math::Vector3D &position, const HalfLifeEngine::SDK::Constants::TeamID_t teamID, float minimumDistance = HalfLifeEngine::SDK::Constants::MapSize) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return GetClosestClientToPosition (position, static_cast <TeamArrayID_t> (teamID - 1u), minimumDistance);
		}
		inline clientType *const GetClosestClientToPosition (const Math::Vector3D &position, const TeamArrayID_t teamArrayID, float minimumDistance = HalfLifeEngine::SDK::Constants::MapSize) const
		{
			clientType *closestClient (NULL);

			minimumDistance *= minimumDistance;	// Square up the minimum distance.

			for (ClientsArray_t::IndexType_t index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
			{
				clientType *const client (m_teamAliveClients[teamArrayID][index]);
				const float distanceSquared (position.GetDistanceSquared (client->GetOrigin ()));

				if (minimumDistance > distanceSquared)
				{
					minimumDistance = distanceSquared;
					closestClient = client;
				}
			}

			return closestClient;
		}

		inline const unsigned char GetDeadClientsNumber (void) const
		{
			// This function also counts spectator players!

			unsigned char deadClientsCount (0u);

			// Enumerate all the dead clients/bots in the server....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (!m_clientsArray[index]->IsAlive ())
					++deadClientsCount;	// This is a dead client/bot.

			// Return the number of dead clients/bots.
			return deadClientsCount;
		}

		inline clientType *const GetAliveHighestFragsClient (void) const
		{
			clientType *aliveHighestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in both teams.
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (!client->IsAlive () || client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return aliveHighestFragsClient;
		}
		inline clientType *const GetAliveHighestFragsClient (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			clientType *aliveHighestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in this team.
/*			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (client->GetRealTeam () != team || !client->IsAlive () || client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}*/
			for (ClientsArray_t::IndexType_t index (0u); index < m_teamAliveClients[team - 1u].GetElementNumber (); ++index)
			{
				clientType *const client (m_teamAliveClients[team - 1u][index]);

				if (client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return aliveHighestFragsClient;
		}

		inline clientType *const GetHighestFragsClient (void) const
		{
			clientType *highestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in both teams.
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return highestFragsClient;
		}
		inline clientType *const GetHighestFragsClient (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			clientType *highestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in this team.
/*			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (client->GetRealTeam () != team || client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}*/
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (team));

			for (ClientsArray_t::IndexType_t index (0u); index < m_teamClients[team - 1u].GetElementNumber (); ++index)
			{
				clientType *const client (m_teamClients[team - 1u][index]);

				if (client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return highestFragsClient;
		}

		inline const bool IsNameTaken (const DynamicString &name) const
		{
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index]->GetName ().CompareWithoutCaseCheck (name) == 0)	// Don't care about case sensitivity in the name....
					return true;	// Found a match.

			return false;	// No match.
		}

		inline void SwapClientsTeams (void) const
		{
			// Loop through all client slots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (!m_clientsArray[index]->IsSpectator ())
					m_clientsArray[index]->ChangeTeam ();
		}

		inline const typename ClientsArray_t::IndexType_t GetClientsCount (void) const
		{
			// This function returns number of clients/bots playing on the server.

			return m_clientsArray.GetElementNumber ();	// Return the number of clients/bots.
		}

		inline const typename ClientsArray_t::IndexType_t GetClientsCount (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return GetClientsCount (static_cast <TeamArrayID_t> (teamID - 1u));
		}
		inline const typename ClientsArray_t::IndexType_t GetClientsCount (const TeamArrayID_t teamArrayID) const
		{
			// This function returns number of clients/bots playing on the given team.

			// Return the number of clients/bots.
			return m_teamClients[teamArrayID].GetElementNumber ();
		}

		inline const unsigned char GetClientsNumberNearPosition (const Math::Vector3D &origin, const float radius, const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return GetClientsNumberNearPosition (origin, radius, static_cast <TeamArrayID_t> (teamID - 1u));
		}
		inline const unsigned char GetClientsNumberNearPosition (const Math::Vector3D &origin, float radius, const TeamArrayID_t teamArrayID) const
		{
			unsigned char clientsNumber (0u);

			radius *= radius;	// Square up.

			// Loop through all player slots in given team....
			for (ClientsArray_t::IndexType_t index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
				if (m_teamAliveClients[teamArrayID][index]->GetOrigin ().GetDistanceSquared (origin) <= radius)
					++clientsNumber;	// Increment clients number.

			return clientsNumber;
		}
		inline const bool IsGroupOfClientsNearPosition (const unsigned char clientsNumber, const Math::Vector3D &origin, const float radius, const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return IsGroupOfClientsNearPosition (clientsNumber, origin, radius, static_cast <TeamArrayID_t> (teamID - 1u));
		}
		inline const bool IsGroupOfClientsNearPosition (unsigned char clientsNumber, const Math::Vector3D &origin, float radius, const TeamArrayID_t teamArrayID) const
		{
			radius *= radius;	// Square up.

			// Loop through all player slots in given team....
			for (ClientsArray_t::IndexType_t index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
				if (m_teamAliveClients[teamArrayID][index]->GetOrigin ().GetDistanceSquared (origin) <= radius)
					if (--clientsNumber == 0u)	// Decrement clients number.
						return true;

			return false;
		}
		inline const bool IsSomeClientStayNearPosition (const Math::Vector3D &origin, const float radius, const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return IsSomeClientStayNearPosition (origin, radius, static_cast <TeamArrayID_t> (teamID - 1u));
		}
		inline const bool IsSomeClientStayNearPosition (const Math::Vector3D &origin, float radius, const TeamArrayID_t teamArrayID) const
		{
			radius *= radius;	// Square up.

			for (ClientsArray_t::IndexType_t index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
				if (m_teamAliveClients[teamArrayID][index]->GetOrigin ().GetDistanceSquared (origin) <= radius)
					return true;

			return false;
		}

		inline clientType *const FindClient (const DynamicString &name)
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index]->GetName ().CompareWithoutCaseCheck (name) == 0)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline const clientType *const FindClient (const DynamicString &name) const
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index]->GetName ().CompareWithoutCaseCheck (name) == 0)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline clientType *const FindClient (const unsigned char userID)
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*m_clientsArray[index]) == userID)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline const clientType *const FindClient (const unsigned char userID) const
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*m_clientsArray[index]) == userID)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline const bool IsMember (const clientType *const client) const
		{
			// Loop through all client/bot slots (if it's not in the list it's not valid)....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index] == client)
					return true;	// Found.

			return false;
		}

		inline void KillAll (void) const
		{
			// This function kills all clients on server.

			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				m_clientsArray[index]->Kill ();

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_Clients).GetData ());
		}

		inline void KillAll (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function kills all clients on server.
/*
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index]->GetRealTeam () == team)
					m_clientsArray[index]->Kill ();
*/
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (team));

			for (ClientsArray_t::IndexType_t index (0u); index < m_teamClients[team - 1u].GetElementNumber (); ++index)
				m_teamClients[team - 1u][index]->Kill ();

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_Clients).GetData ());
		}

		inline void KickAll (void) const
		{
			// This function drops all clients from server.

			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				m_clientsArray[index]->Kick ();	// Delete him/her/it from the list....

			// Reset some console variables....
			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue <unsigned char> (0u);
			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutoVacate)->SetValue <unsigned char> (0u);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKickedFromServer), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_Clients).GetData ());
		}

		inline const bool IsTeamFull    (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			return g_server->GetGameRules ()->IsTeamFull (teamID);
		}
		inline const bool IsTeamStacked (const HalfLifeEngine::SDK::Constants::TeamID_t currentTeamID, const HalfLifeEngine::SDK::Constants::TeamID_t newTeamID) const
		{
			return g_server->GetGameRules ()->IsTeamStacked (currentTeamID, newTeamID);
		}

		inline clientType *const GetClientByString (const DynamicString &string) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty ());

			// Search client with given user ID or name

			if (string[0u] == '#')
				return GetClientByUserID (string.GetValue <unsigned char> (1u));

			return GetClientByPartialName (string);
		}
		inline clientType *const GetClientByPartialNameWithCaseCheck (const DynamicString &partialName) const
		{
			clientType *bestNominee (NULL);

			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				clientType *const client (m_clientsArray[index]);
				const DynamicString clientName (client->GetName ());

				if (clientName.Contains (partialName) && (bestNominee == NULL || bestNominee->GetName ().GetElementNumber () > clientName.GetElementNumber ()))
					bestNominee = client;
			}

			return bestNominee;
		}
		inline clientType *const GetClientByPartialName (const DynamicString &partialName) const
		{
			clientType *bestNominee (NULL);

			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				clientType *const client (m_clientsArray[index]);
				const DynamicString clientName (client->GetName ());

				if (clientName.Contains (partialName, false) && (bestNominee == NULL || bestNominee->GetName ().GetElementNumber () > clientName.GetElementNumber ()))
					bestNominee = client;
			}

			return bestNominee;
		}
		inline clientType *const GetClientByUserID (const unsigned char userID) const
		{
			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				clientType *const client (m_clientsArray[index]);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*client) == userID)
					return client;
			}

			return NULL;
		}
		inline clientType *const GetClientByAuthID (const DynamicString &authID) const
		{
			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				clientType *const client (m_clientsArray[index]);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerAuthID (*client) == authID)
					return client;
			}

			return NULL;
		}

		inline void SetTeam (const HalfLifeEngine::SDK::Constants::TeamID_t newTeam)
		{
			// loop through all client slots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				m_clientsArray[index]->SetTeam (newTeam);
		}

		inline void ExecuteCommand (const DynamicString &command) const
		{
			// Loop through all alive clients....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
					m_clientsArray[index]->ExecuteCommand (command);
		}

		inline void SetHealth (const unsigned short value) const
		{
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (client->IsAlive ())
					client->GetEdict ()->variables.health = value;
			}
		}
		inline void SetMoney (const unsigned short value) const
		{
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (client->IsAlive ())
					client->SetMoney (value);
			}
		}

		inline void KickRandom (void) const
		{
			// This function kicks random client from server.

			if (!m_clientsArray.IsEmpty ())
				m_clientsArray.GetRandomElement ()->Kick ();
		}
		inline void KickFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kick random client from specified team

			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (client->GetRealTeam () != team)
					continue;

				client->Kick ();

				break;
			}
		}
		inline void KickAllFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kicks all clients from specified team

			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				clientType *const client (m_clientsArray[index]);

				if (client->GetRealTeam () == team)
					client->Kick ();
			}
		}

	//
	// Group: Game message handler callbacks.
	//
	public:
		inline void ClientTeamSwitchedToTerrorists (clientType *const client)
		{
			// This function called after client/bot switch team to terrorists.

			// Reliability check. (Same team?)
			if (client->m_currentTeamID == HalfLifeEngine::SDK::Constants::TeamID_Terrorist)
			{
				#if defined _DEBUG
					Label_Check:
					{
//			AddLogEntry (true, LogLevel_Default, true, "FCM::ClientTeamSwitchedToTerrorists(): Called for client \"%s\", m_currentTeamID=%i, BasePlayer::m_team=%i, Edict::IsAlive()=%i, IsAlive()=%i.", client->GetName ().GetData (), client->m_currentTeamID, client->GetRealTeam (), client->GetEdict ()->IsAlive (), client->IsAlive ());

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

/// @note ALREADY CALLED BY 'ClientManager' CLASS			client->TeamChanged (HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

			#if defined _DEBUG
				goto Label_Check;
			#endif	// if defined _DEBUG
		}
		inline void ClientTeamSwitchedToCounterTerrorists (clientType *const client)
		{
			// This function called after client/bot switch team to counter-terrorists.

			// Reliability check. (Same team?)
			if (client->m_currentTeamID == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
			{
				#if defined _DEBUG
					Label_Check:
					{
//			AddLogEntry (true, LogLevel_Default, true, "FCM::ClientTeamSwitchedToCounterTerrorists(): Called for client \"%s\", m_currentTeamID=%i, BasePlayer::m_team=%i, Edict::IsAlive()=%i, IsAlive()=%i.", client->GetName ().GetData (), client->m_currentTeamID, client->GetRealTeam (), client->GetEdict ()->IsAlive (), client->IsAlive ());

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

/// @note ALREADY CALLED BY 'ClientManager' CLASS			client->TeamChanged (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);

			#if defined _DEBUG
				goto Label_Check;
			#endif	// if defined _DEBUG
		}
		inline void ClientTeamSwitchedToSpectators (clientType *const client, const bool isNewTeamUnassigned)
		{
			// This function called after client/bot switch team to spectators.

			const HalfLifeEngine::SDK::Constants::TeamID_t newTeamID (isNewTeamUnassigned ? HalfLifeEngine::SDK::Constants::TeamID_Unassigned : HalfLifeEngine::SDK::Constants::TeamID_Spectator);

			// Reliability check. (Same team?)
			if (client->m_currentTeamID == newTeamID)
			{
				#if defined _DEBUG
					Label_Check:
					{
//			AddLogEntry (true, LogLevel_Default, true, "FCM::ClientTeamSwitchedToSpectators(): Called for client \"%s\", m_currentTeamID=%i, BasePlayer::m_team=%i, Edict::IsAlive()=%i, IsAlive()=%i.", client->GetName ().GetData (), client->m_currentTeamID, client->GetRealTeam (), client->GetEdict ()->IsAlive (), client->IsAlive ());

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

/// @note ALREADY CALLED BY 'ClientManager' CLASS			client->TeamChanged (newTeamID);

			#if defined _DEBUG
				goto Label_Check;
			#endif	// if defined _DEBUG
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void ClientConnect    (clientType *const client) { AddClient (client); }
		inline void ClientDisconnect (clientType *const client) { RemoveClient (client); }

		inline void ClientAliveSpawnPost (clientType *const client)
		{
			const TeamArrayID_t clientRealTeam (client->GetRealArrayTeamID ());

			// Client can spawn twice being alive!
			if (!m_teamAliveClients[clientRealTeam].IsMember (client))
				m_teamAliveClients[clientRealTeam] += client;
		}
		inline void ClientKilled (clientType *const client)
		{
			m_teamAliveClients[client->GetRealArrayTeamID ()] -= client;
		}
};

#endif	// ifndef CLIENT_MANAGER_TEMPLATE_INCLUDED