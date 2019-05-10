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
// ClientManager.hpp
//
// Class: ClientManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CLIENT_MANAGER_INCLUDED
#define CLIENT_MANAGER_INCLUDED

class ClientManager
{
	// It's my best friends! :{P
	friend FakeClientNotYaPBManager;
	friend HumanManager;
	friend YaPBManager;

	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <Client *, unsigned char> ClientsArray_t;

	//
	// Group: Private members.
	//
	private:
		Client **const m_clients;								// For quickly getting the client pointer, by it's entity index. (Example: client->GetIndex () - 1u)
		ClientsArray_t m_clientsArray;							// For quickly clients looping/counting.

		// Real-time clients number statistics....
		ClientsArray_t m_teamClients[TeamArrayID_Total];		// For quickly team clients looping/counting.
		ClientsArray_t m_teamAliveClients[TeamArrayID_Total];	// For quickly team alive clients looping/counting. (@note this can be only modified on player change team, spawn being alive or killed)
#if 0
		class AdminsManager
		{
			class Admin
			{
				private:
					
			};

			public:
				typedef DynamicArray <Admin *, unsigned char> AdminsArray_t;

			private:
				AdminsArray_t m_admins;

			public:
				inline ~AdminsManager (void) { m_admins.DeleteAll (); }
		};
#endif	// if 0
		#include <ClientCommandHooker.hpp>

		ClientCommandHooker             m_clientCommandHooker;

		#include <ClientPutInServerHooker.hpp>

		ClientPutInServerHooker         m_clientPutInServerHooker;

		#include <SV_CalcPingIfFakeClientInjector.hpp>

		SV_CalcPingIfFakeClientInjector m_SV_CalcPingIfFakeClientInjector;

		#include <FakeClientCommand.hpp>

		FakeClientCommand               m_fakeClientCommand;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ClientManager (void) : m_clients (new Client *[HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ()])
		{
			// This function builds all client slots. (Used on server activation)

			#if defined _DEBUG
				// Reliability check.
				if (m_clients == NULL)
					TerminateOnMalloc ();
			#endif	// if defined _DEBUG

			// Initialize the clients array....
			for (unsigned char index (0u); index < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients (); ++index)
				m_clients[index] = NULL;	// Null out the client pointer.
		}
		inline ~ClientManager (void)
		{
			// This function frees all client slots. (Used on server shutdown)

			// Free all the memory allocated for all clients (including bots)....
			for (unsigned char index (0u); index < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients (); ++index)
				delete m_clients[index];	// Delete him/her/it from the list.
/*/
			// Free all the memory allocated for all clients (including bots)....
			for (unsigned char index (0u); index < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients (); ++index)
			{
/*				if (m_clients[index]->IsYaPB ())
					g_connectedYaPBs += ConnectedYaPB_t (m_clients[index]->GetYaPBPointer (), &m_clients[index]->GetEngineClient ());
				else
*//*				if (!m_clients[index]->IsYaPB ())
					delete m_clients[index];	// Delete him/her/it from the list.
			}
//*/
			delete [] m_clients;
		}

	//
	// Group: Private operators.
	//
	private:
		inline ClientManager &operator = (const ClientManager &/*right*/);	// Avoid "warning C4512: 'ClientManager' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		inline void RegisterGameDLLAPIForwards   (void) { GetClientCommandHooker ().CreateAndDoPatch (); }
		inline void UnregisterGameDLLAPIForwards (void) { GetClientCommandHooker ().UndoAndDestroyPatch (); }

		inline       ClientCommandHooker     &GetClientCommandHooker     (void)       { return m_clientCommandHooker; }
		inline       ClientPutInServerHooker &GetClientPutInServerHooker (void)       { return m_clientPutInServerHooker; }
		inline       FakeClientCommand       &GetFakeClientCommand       (void)       { return m_fakeClientCommand; }
		inline const FakeClientCommand       &GetFakeClientCommand       (void) const { return m_fakeClientCommand; }

		inline Client *const &GetClient (const unsigned char index)
		{
			// this function finds a client specified by index, and then returns pointer to it (using own 'Client' class).

			// Reliability check. (Check bot slot index)
			InternalAssert (index < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ());

			// If no edict, return NULL.
			return m_clients[index];
		}
		inline const Client *const &GetClient (const unsigned char index) const
		{
			// this function finds a client specified by index, and then returns pointer to it (using own 'Client' class).

			// Reliability check. (Check bot slot index)
			InternalAssert (index < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ());

			// If no edict, return NULL.
			return m_clients[index];
		}
		inline Client *const &GetClient (const HalfLifeEngine::SDK::Classes::Edict *const client)
		{
			// this function finds a client specified by edict, and then returns pointer to it (using own 'Client' class).

			// Reliability checks.
			InternalAssert (client->IsValid ());
			InternalAssert (client->IsNotWorldspawnPlayer ());

			// If no edict, return NULL.
			return m_clients[client->GetIndex () - 1u];
		}
		inline const Client *const &GetClient (const HalfLifeEngine::SDK::Classes::Edict *const client) const
		{
			// this function finds a client specified by edict, and then returns pointer to it (using own 'Client' class).

			// Reliability checks.
			InternalAssert (client->IsValid ());
			InternalAssert (client->IsNotWorldspawnPlayer ());

			// If no edict, return NULL.
			return m_clients[client->GetIndex () - 1u];
		}

		inline       Client **const       GetClients (void)       { return m_clients; }
		inline const Client *const *const GetClients (void) const { return m_clients; }

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

		inline const unsigned char GetAliveClientsNumber     (const TeamArrayID_t realTeamArrayIndex)                  const { return m_teamAliveClients[realTeamArrayIndex].GetElementNumber (); }
		inline const unsigned char GetAliveClientsNumber     (const HalfLifeEngine::SDK::Constants::TeamID_t realTeam) const { return GetAliveClientsNumber (static_cast <TeamArrayID_t> (realTeam - 1u)); }
//		inline const unsigned char GetAliveFakeClientsNumber (const HalfLifeEngine::SDK::Constants::TeamID_t realTeam) const { return m_teamAliveFakeClients[realTeam - 1u].GetElementNumber (); }
//		inline const unsigned char GetAliveHumansNumber      (const HalfLifeEngine::SDK::Constants::TeamID_t realTeam) const { return m_teamAliveHumans[realTeam - 1u].GetElementNumber (); }

		inline const bool IsPointVisibleToTeam (const Math::Vector3D &position, const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return IsPointVisibleToTeam (position, static_cast <TeamArrayID_t> (teamID - 1u));
		}
		inline const bool IsPointVisibleToTeam (const Math::Vector3D &position, const TeamArrayID_t teamArrayID) const
		{
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

			for (unsigned char index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
			{
				Client *const client (m_teamAliveClients[teamArrayID][index]);

				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (client->GetEyePosition (), position, HalfLifeEngine::SDK::Constants::TraceIgnore_All, *client, traceResult);

				if (traceResult.fraction == 1.0f)
					return true;
			}

			return false;
		}
		inline Client *const GetClosestClientToPosition (const Math::Vector3D &position, const HalfLifeEngine::SDK::Constants::TeamID_t teamID, float minimumDistance = HalfLifeEngine::SDK::Constants::MapSize) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return GetClosestClientToPosition (position, static_cast <TeamArrayID_t> (teamID - 1u), minimumDistance);
		}
		inline Client *const GetClosestClientToPosition (const Math::Vector3D &position, const TeamArrayID_t teamArrayID, float minimumDistance = HalfLifeEngine::SDK::Constants::MapSize) const
		{
			Client *closestClient (NULL);

			minimumDistance *= minimumDistance;	// Square up the minimum distance.

			for (unsigned char index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
			{
				Client *const client (m_teamAliveClients[teamArrayID][index]);
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

		inline Client *const GetAliveHighestFragsClient (void) const
		{
			Client *aliveHighestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in both teams.
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				Client *const client (m_clientsArray[index]);

				if (!client->IsAlive () || client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return aliveHighestFragsClient;
		}
		inline Client *const GetAliveHighestFragsClient (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			Client *aliveHighestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in this team.
/*			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				Client *const client (m_clientsArray[index]);

				if (client->GetRealTeam () != team || !client->IsAlive () || client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}*/
			for (unsigned char index (0u); index < m_teamAliveClients[team - 1u].GetElementNumber (); ++index)
			{
				Client *const client (m_teamAliveClients[team - 1u][index]);

				if (client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return aliveHighestFragsClient;
		}

		inline Client *const GetHighestFragsClient (void) const
		{
			Client *highestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in both teams.
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				Client *const client (m_clientsArray[index]);

				if (client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return highestFragsClient;
		}
		inline Client *const GetHighestFragsClient (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			Client *highestFragsClient (NULL);
			float bestScore (-1.0f);

			// Search clients in this team.
/*			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				Client *const client (m_clientsArray[index]);

				if (client->GetRealTeam () != team || client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}*/
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (team));

			for (unsigned char index (0u); index < m_teamClients[team - 1u].GetElementNumber (); ++index)
			{
				Client *const client (m_teamClients[team - 1u][index]);

				if (client->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsClient = client;	// Keep track of the highest frags client.
				bestScore = client->GetEdict ()->variables.frags;	// Update best score.
			}

			return highestFragsClient;
		}

		inline const Client *const GetC4Defuser (void) const
		{
			// This function returns client pointer of defuser entity if finds somebody currently defusing the bomb, NULL otherwise.

			const HalfLifeEngine::SDK::Classes::BasePlayer *const C4Defuser (g_server->GetZBotManager ()->m_bombDefuser);

			return C4Defuser == NULL ? NULL : GetClient (C4Defuser->GetEdict ());
#if 0
			// Reliability check.
			if (!g_server->GetC4Manager ().Is—4Planted ())
				return NULL;
/*
			// Loop through all client/bot slots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index]->IsDefusing ())
					return m_clientsArray[index];
*/
			for (unsigned char index (0u); index < m_teamAliveClients[TeamArrayID_CounterTerrorist].GetElementNumber (); ++index)
				if (m_teamAliveClients[TeamArrayID_CounterTerrorist][index]->IsDefusing ())
					return m_teamAliveClients[TeamArrayID_CounterTerrorist][index];

			return NULL;
#endif	// if 0
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

		inline const ClientsArray_t::IndexType_t GetClientsCount (void) const
		{
			// This function returns number of clients/bots playing on the server.

			return m_clientsArray.GetElementNumber ();	// Return the number of clients/bots.
		}

		inline const unsigned char GetClientsCount (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const
		{
			// Reliability check.
			InternalAssert (!HalfLifeEngine::Utilities::IsSpectatorTeam (teamID));

			return GetClientsCount (static_cast <TeamArrayID_t> (teamID - 1u));
		}
		inline const unsigned char GetClientsCount (const TeamArrayID_t teamArrayID) const
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

			radius *= radius;

			// Loop through all player slots in given team....
			for (unsigned char index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
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
			radius *= radius;

			// Loop through all player slots in given team....
			for (unsigned char index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
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
			radius *= radius;

			for (unsigned char index (0u); index < m_teamAliveClients[teamArrayID].GetElementNumber (); ++index)
				if (m_teamAliveClients[teamArrayID][index]->GetOrigin ().GetDistanceSquared (origin) <= radius)
					return true;

			return false;
		}

		inline Client *const FindClient (const DynamicString &name)
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index]->GetName ().CompareWithoutCaseCheck (name) == 0)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline const Client *const FindClient (const DynamicString &name) const
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (m_clientsArray[index]->GetName ().CompareWithoutCaseCheck (name) == 0)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline Client *const FindClient (const unsigned char userID)
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*m_clientsArray[index]) == userID)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline const Client *const FindClient (const unsigned char userID) const
		{
			// Loop through all bots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*m_clientsArray[index]) == userID)
					return m_clientsArray[index];	// Found.

			return NULL;
		}
		inline const bool IsMember (const Client *const client) const
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

			for (unsigned char index (0u); index < m_teamClients[team - 1u].GetElementNumber (); ++index)
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

		inline Client *const GetClientByString (const DynamicString &string) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty ());

			// Search client with given user ID or name

			if (string[0u] == '#')
				return GetClientByUserID (string.GetValue <unsigned char> (1u));

			return GetClientByPartialName (string);
		}
		inline Client *const GetClientByPartialNameWithCaseCheck (const DynamicString &partialName) const
		{
			Client *bestNominee (NULL);

			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				Client *const client (m_clientsArray[index]);
				const DynamicString clientName (client->GetName ());

				if (clientName.Contains (partialName) && (bestNominee == NULL || bestNominee->GetName ().GetElementNumber () > clientName.GetElementNumber ()))
					bestNominee = client;
			}

			return bestNominee;
		}
		inline Client *const GetClientByPartialName (const DynamicString &partialName) const
		{
			Client *bestNominee (NULL);

			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				Client *const client (m_clientsArray[index]);
				const DynamicString clientName (client->GetName ());

				if (clientName.Contains (partialName, false) && (bestNominee == NULL || bestNominee->GetName ().GetElementNumber () > clientName.GetElementNumber ()))
					bestNominee = client;
			}

			return bestNominee;
		}
		inline Client *const GetClientByUserID (const unsigned char userID) const
		{
			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				Client *const client (m_clientsArray[index]);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*client) == userID)
					return client;
			}

			return NULL;
		}
		inline Client *const GetClientByAuthID (const DynamicString &authID) const
		{
			// loop through all client slots
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
			{
				// remember slot of this client entity
				Client *const client (m_clientsArray[index]);

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

	//
	// Group: Game message handler callbacks.
	//
	public:
		inline void RoundStarted (void) const
		{
			// This function is called at the start of each round.

			// Loop through all client slots....
			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				m_clientsArray[index]->RoundStarted ();
		}
		void ClientTeamSwitchedToTerrorists        (const unsigned char clientEdictIndex);
		void ClientTeamSwitchedToCounterTerrorists (const unsigned char clientEdictIndex);
		void ClientTeamSwitchedToSpectators        (const unsigned char clientEdictIndex, const bool isNewTeamUnassigned);

	//
	// Group: Engine callbacks.
	//
	public:
		void        ClientSpawnPost        (const HalfLifeEngine::SDK::Classes::Edict *const client);
		void        ClientRoundRespawnPost (const HalfLifeEngine::SDK::Classes::Edict *const client);
		void        ClientUse              (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const entity, const HalfLifeEngine::SDK::Classes::Edict *const caller, const HalfLifeEngine::SDK::Constants::UseType_t type, const float value) const;
		inline void ClientTouchPost        (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			GetClient (client)->TouchPost (entity);
		}
		inline void ClientItemDeployPost   (const HalfLifeEngine::SDK::Classes::Edict *const client, HalfLifeEngine::SDK::Classes::Edict *const weapon) const
		{
			// Reliability checks.
			InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());	// OCCURS!

			const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

			// Reliability check.
			if (m_clients[clientIndex]->IsValid ())	// OCCURS ON changing level.
				m_clients[clientIndex]->ItemDeployPost (weapon);
		}
		inline void ClientWeaponReloadPost (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const weapon) const
		{
			// Reliability checks.
			InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());

			const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

			// Reliability check.
			InternalAssert (m_clients[clientIndex]->IsValid ());

			m_clients[clientIndex]->WeaponReloadPost (weapon);
		}
		inline void ClientKilled (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const killer);	// Note: This function declared in YaPBManager.hpp
		inline void ClientAddWeapon (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const weapon) const
		{
			// This function called after client/bot weapon add.

			// Reliability checks.
			InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
			InternalAssert (weapon->IsValid ());

			const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

			// Reliability check.
			InternalAssert (m_clients[clientIndex]->IsValid ());

			m_clients[clientIndex]->AddWeapon (weapon);
		}
		inline void ClientRemoveWeapon (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const weapon) const
		{
			// This function called after client/bot weapon remove.

			// Reliability checks.
			InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
			InternalAssert (weapon->IsValid ());

			const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

			// Reliability checks.
			if (m_clients[clientIndex] == NULL)	// OCCURS!!!
				return;

			InternalAssert (m_clients[clientIndex]->IsValid ());

			m_clients[clientIndex]->RemoveWeapon (weapon);
		}
		inline void ClientOnTouchingWeapon (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			// Reliability checks.
			InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
			InternalAssert (weapon->IsValid ());

			const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

			// Reliability check.
			InternalAssert (m_clients[clientIndex]->IsValid ());

			m_clients[clientIndex]->OnTouchingWeapon (weapon);
		}
		inline void UpdateClientData (const HalfLifeEngine::SDK::Classes::Edict *const client) const;	// Note: This function declared in YaPBManager.hpp
		inline void ClientImpulseCommand (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t impulseCommand)
		{
			// Reliability checks.
			InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());

			const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

			// Reliability check.
			InternalAssert (m_clients[clientIndex]->IsValid ());

			m_clients[clientIndex]->ImpulseCommand (impulseCommand);
		}

		void ClientDisconnect              (const HalfLifeEngine::SDK::Classes::Edict *const client);
		void ClientPutInServer             (HalfLifeEngine::SDK::Classes::Edict *const client);
		void ClientUserInfoChanged         (HalfLifeEngine::SDK::Classes::Edict *const client, const DynamicString &infoBuffer);
/*
		inline void ClientStudioSetupBones (const HalfLifeEngine::SDK::Classes::Edict *const client, HalfLifeEngine::SDK::Structures::Model_t *const model, const float frame, const int sequence, const Math::Angles3D &angles, const Math::Vector3D &origin, const unsigned char *const controller, const unsigned char *const blending, const int boneID)
		{
			// Reliability checks.
			InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());

			const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

			// Reliability check.
			InternalAssert (m_clients[clientIndex]->IsValid ());

			m_clients[clientIndex]->OnStudioSetupBones (model, frame, sequence, angles, origin, controller, blending, boneID);
		}
*/
		void EmitSound                     (const HalfLifeEngine::SDK::Classes::Edict *const entity, const DynamicString &sample, const float volume, const float attenuation, const HalfLifeEngine::SDK::Constants::SoundFlag_t flags, const HalfLifeEngine::SDK::Constants::SoundPitch_t pitch) const;

		void Think                         (void) const;

	//
	// Group: Other callbacks.
	//
	public:
		inline void OnDestroyNavigationArea (NavigationMesh::NavigationArea *const area)
		{
			for (unsigned char teamIndex (TeamArrayID_Terrorist); teamIndex < TeamArrayID_Total; ++teamIndex)
				for (ClientsArray_t::IndexType_t clientIndex (0u); clientIndex < m_teamAliveClients[teamIndex].GetElementNumber (); ++clientIndex)
					if (m_teamAliveClients[teamIndex][clientIndex]->m_lastNavigationArea == area)
						m_teamAliveClients[teamIndex][clientIndex]->m_lastNavigationArea = NULL;
		}
		inline void OnDestroyNavigationMesh (void)
		{
			for (unsigned char teamIndex (TeamArrayID_Terrorist); teamIndex < TeamArrayID_Total; ++teamIndex)
				for (ClientsArray_t::IndexType_t clientIndex (0u); clientIndex < m_teamAliveClients[teamIndex].GetElementNumber (); ++clientIndex)
					m_teamAliveClients[teamIndex][clientIndex]->m_lastNavigationArea = NULL;
		}
};

inline const bool Client::IsHasMostFrags (void) const
{
	return g_server->IsTeamplay () ? g_server->GetClientManager ()->GetHighestFragsClient (GetRealTeam ()) == this : g_server->GetClientManager ()->GetHighestFragsClient () == this;
}

inline const Client *const Client::GetObserverTarget (void) const { return g_server->GetClientManager ()->GetClient (GetObserverTargetEdictIndex () - 1u); }

inline void FakeClient::ExecuteCommand (const DynamicString &command) const { g_server->GetClientManager ()->GetFakeClientCommand ().Execute (m_edict, command); }

inline Server::ThrownGrenadesManager::ThrownGrenade_t::ThrownGrenade_t (const HalfLifeEngine::SDK::Classes::Edict *const inputGrenade, const Type_t inputType) :
	grenade (inputGrenade),
	type (inputType),
	pseudoEntityVariables (HalfLifeEngine::Utilities::TraceThrownGrenadeToss (inputGrenade)),
	owner (/*!inputGrenade->variables.owner->IsValid () || !inputGrenade->variables.owner->IsNotWorldspawnPlayer () ? NULL : */g_server->GetClientManager ()->GetClient (inputGrenade->variables.owner))
{ /* VOID */ }

inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_AmmoPickup (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForClient_AmmoPickup (g_server->GetClientManager ()->GetClient (to)));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing client \"%s\" game message 'AmmoPickup' (id == %u)!", sizeof (GameMessageHandler_ForClient_AmmoPickup), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_AmmoPickup));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_AmmoX (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	if (client == NULL)	// OCCURS ON CientDisconnect().
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForClient_AmmoX (client));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing client \"%s\" game message 'AmmoX' (id == %u)!", sizeof (GameMessageHandler_ForClient_AmmoX), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_AmmoX));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_CurrentWeapon (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	if (client == NULL)	// OCCURS ON CientDisconnect().
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForClient_CurrentWeapon (client));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing client \"%s\" game message 'CurrentWeapon' (id == %u)!", sizeof (GameMessageHandler_ForClient_CurrentWeapon), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_CurrentWeapon));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_Damage (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	InternalAssert (client->IsValid ());	// OCCURS ON CientDisconnect().

	// Reliability check.
	if (!client->IsYaPB ())
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_Damage (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'Damage' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_Damage), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_Damage));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ScreenShake (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	InternalAssert (client->IsValid ());

	// Reliability check.
	if (!client->IsYaPB ())
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_ScreenShake (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'ScreenShake' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_ScreenShake), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_ScreenShake));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ScreenFade (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AimMethod)->GetValue <Console::Bot::VariableValue_AimMethod_t> () == Console::Bot::VariableValue_AimMethod_1)
		return NULL;

	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	InternalAssert (client->IsValid ());

	// Reliability check.
	if (!client->IsYaPB ())
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_ScreenFade (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'ScreenFade' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_ScreenFade), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_ScreenFade));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ShowMenu (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	// Is this message for client?
	if (to == NULL)
	{
		GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ShowMenu ());

		#if defined _DEBUG
			// Reliability check.
			if (newCurrentGameMessage == NULL)
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing global game message 'ShowMenu' (id == %u)!", sizeof (GameMessageHandler_ShowMenu), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_ShowMenu));
		#endif	// if defined _DEBUG

		return newCurrentGameMessage;
	}

	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	InternalAssert (client->IsValid ());

	// Reliability check.
	if (!client->IsYaPB ())
	{
		if (client->IsHuman ())
			return new GameMessageHandler_ForHuman_ShowMenu (client->GetHumanPointer ());

		return NULL;
	}

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_ShowMenu (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'ShowMenu' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_ShowMenu), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_ShowMenu));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_StatusIcon (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	if (client == NULL || !client->IsYaPB ())	// OCCURS ON CientDisconnect().
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_StatusIcon (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'StatusIcon' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_StatusIcon), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_StatusIcon));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_ReloadSound (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	InternalAssert (client->IsValid ());

	// Reliability check.
	if (!client->IsYaPB () || !client->IsAlive ())
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_ReloadSound (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'ReloadSound' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_ReloadSound), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_ReloadSound));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_Text (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	// Is this message for client?
	if (to == NULL)
	{
		GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_Text ());

		#if defined _DEBUG
			// Reliability check.
			if (newCurrentGameMessage == NULL)
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing global game message 'Text' (id == %u)!", sizeof (GameMessageHandler_Text), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_Text));
		#endif	// if defined _DEBUG

		return newCurrentGameMessage;
	}

	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	if (client == NULL || !client->IsYaPB ())
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_Text (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'Text' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_Text), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_Text));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}
inline Server::GameMessageHandlerManager::GameMessageHandler *const Server::GameMessageHandlerManager::GameMessageHandlerConstructorFunction_VGUIMenu (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const to)
{
	Client *const client (g_server->GetClientManager ()->GetClient (to));

	// Reliability check.
	InternalAssert (client->IsValid ());

	// Reliability check.
	if (!client->IsYaPB ())
		return NULL;

	GameMessageHandler *const newCurrentGameMessage (new GameMessageHandler_ForYaPB_VGUIMenu (client->GetYaPBPointer ()));

	#if defined _DEBUG
		// Reliability check.
		if (newCurrentGameMessage == NULL)
			AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for processing YaPB \"%s\" game message 'VGUIMenu' (id == %u)!", sizeof (GameMessageHandler_ForYaPB_VGUIMenu), to->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_VGUIMenu));
	#endif	// if defined _DEBUG

	return newCurrentGameMessage;
}

inline void HostageManager::HostageKilled (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity, const Client *const killer)
{
	const HalfLifeEngine::SDK::Classes::Edict *const hostageRescuerEdict (hostageEntity->GetHostageRescuer ());

	if (hostageRescuerEdict != NULL)
		g_server->GetClientManager ()->GetClient (hostageRescuerEdict)->HostageKilled (hostageEntity, killer);
	else
		m_unusedAliveHostages -= hostageEntity;
}
inline void HostageManager::HostageRescued (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity, Client *const rescuer)
{
	// Reliability check.
	InternalAssert (rescuer != NULL);

	m_unusedAliveHostages -= hostageEntity;

	rescuer->HostageRescued (hostageEntity);
}
inline void HostageManager::HostageUsePost (HalfLifeEngine::SDK::Classes::Edict *const hostageEntity, Client *const previousRescuer, Client *const currentRescuer)
{
	if (previousRescuer != NULL)
		previousRescuer->HostageUnusePost (hostageEntity, currentRescuer);

	if (currentRescuer != NULL)
	{
		m_unusedAliveHostages -= hostageEntity;

		currentRescuer->HostageUsePost (hostageEntity, previousRescuer);
	}
}
inline void HostageManager::AllHostagesRescued (void)
{
	m_unusedAliveHostages.RemoveAll ();

	// Loop through all the clients/bots...
	for (unsigned char clientIndex (0u); clientIndex < g_server->GetClientManager ()->GetAliveClientsNumber (TeamArrayID_CounterTerrorist); ++clientIndex)
	{
		// temporarily keep this slot
		Client *const client (g_server->GetClientManager ()->GetTeamAliveClientsArray (TeamArrayID_CounterTerrorist)[clientIndex]);

		client->AllHostagesRescued ();
	}
}

inline const Client *const C4::GetDefuser (void) const { return g_server->GetClientManager ()->GetC4Defuser (); }

inline const float NavigationMesh::ShortestPathCostAlsoThruDoubleJumpAreas::operator () (const NavigationArea *const area, const NavigationArea *const fromArea, const NavigationLadder *const ladder) const
{
	if (fromArea == NULL)
		return 0.0f;	// first area in path, no cost

	// compute distance travelled along path so far
	const float dist (ladder != NULL ? ladder->GetLength () : area->GetCenter ().GetDistance (fromArea->GetCenter ()));
	float cost (dist + fromArea->GetCostSoFar ());

	// if this is a "crouch" area, add penalty
	if (area->HasAttributes (NAV_MESH_CROUCH))
	{
		const float crouchPenalty (20.0f);	// 10

		cost += crouchPenalty * dist;
	}

	// if this is a "jump" area, add penalty
	if (area->HasAttributes (NAV_MESH_JUMP))
	{
		const float jumpPenalty (5.0f);

		cost += jumpPenalty * dist;
	}
	else if (area->HasAttributes (NAV_MESH_DOUBLE_JUMP))	// if this is a "double jump" area, add penalty
	{
		if (!g_server->IsTeamplay () || g_server->GetClientManager ()->GetAliveClientsNumber (m_bot->GetRealTeam ()) < 2u || g_server->GetClientManager ()->GetAliveClientsNumber (m_bot->GetEnemyTeam ()) < 2u)
			return 999999.9f;	/// @todo MOVE THESE CHECKS OUT OF HERE!!!

		if (!g_server->GetClientManager ()->IsSomeClientStayNearPosition (/*area->GetCenter ()/*/m_bot->GetOrigin (), 200.0f, m_bot->GetRealTeam ()))
			return 999999.9f;

		const float doubleJumpPenalty (25.0f);

		cost += doubleJumpPenalty * dist;
	}

	return cost;
}

#endif	// ifndef CLIENT_MANAGER_INCLUDED