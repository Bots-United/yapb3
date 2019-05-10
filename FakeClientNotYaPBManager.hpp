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
// FakeClientNotYaPBManager.hpp
//
// Class: FakeClientNotYaPBManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef OTHER_BOT_MANAGER_INCLUDED
#define OTHER_BOT_MANAGER_INCLUDED

class FakeClientNotYaPBManager
{
	friend YaPBManager;

	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <FakeClient_NotYaPB *, unsigned char> FakeClientNotYaPBArray;

	//
	// Group: Private members.
	//
	private:
		FakeClientNotYaPBArray m_otherBotsArray;	// For quickly other bots looping/counting.

	//
	// Group: Private functions.
	//
	private:
		inline void AddOtherBot    (FakeClient_NotYaPB *const otherBot) { m_otherBotsArray += otherBot; }
		inline void RemoveOtherBot (FakeClient_NotYaPB *const otherBot) { m_otherBotsArray -= otherBot; }

	//
	// Group: Functions.
	//
	public:
		inline FakeClient_NotYaPB *const GetFakeClientNotYaPB (const unsigned char index)
		{
			// This function finds a other bot specified by index, and then returns pointer to it (using own 'FakeClient_NotYaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (index)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[index]->GetFakeClientNotYaPBPointer ();
		}
		inline const FakeClient_NotYaPB *const GetFakeClientNotYaPB (const unsigned char index) const
		{
			// This function finds a other bot specified by index, and then returns pointer to it (using own 'FakeClient_NotYaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (index)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[index]->GetFakeClientNotYaPBPointer ();
		}
		inline FakeClient_NotYaPB *const GetFakeClientNotYaPB (const HalfLifeEngine::SDK::Classes::Edict *const otherBot)
		{
			// This function finds a other bot specified by edict, and then returns pointer to it (using own 'FakeClient_NotYaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (otherBot)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[otherBot->GetIndex () - 1u]->GetFakeClientNotYaPBPointer ();
		}
		inline const FakeClient_NotYaPB *const GetFakeClientNotYaPB (const HalfLifeEngine::SDK::Classes::Edict *const otherBot) const
		{
			// This function finds a other bot specified by edict, and then returns pointer to it (using own 'FakeClient_NotYaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (otherBot)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[otherBot->GetIndex () - 1u]->GetFakeClientNotYaPBPointer ();
		}

		inline       FakeClientNotYaPBArray &GetFakeClientNotYaPBArray (void)
		{
			// This function returns a reference to non-constant other bots array.

			return m_otherBotsArray;
		}
		inline const FakeClientNotYaPBArray &GetFakeClientNotYaPBArray (void) const
		{
			// This function returns a reference to constant other bots array.

			return m_otherBotsArray;
		}

		inline void KickRandom (void) const
		{
			// This function kicks random other bot from server.

			if (!m_otherBotsArray.IsEmpty ())
				m_otherBotsArray.GetRandomElement ()->Kick ();
		}

		inline FakeClient_NotYaPB *const FindFakeClientNotYaPB (const DynamicString &name)
		{
			// Loop through all other bots....
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->GetName ().CompareWithoutCaseCheck (name) == 0)
					return otherBot;
			}

			return NULL;
		}
		inline const FakeClient_NotYaPB *const FindFakeClientNotYaPB (const DynamicString &name) const
		{
			// Loop through all other bots....
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				const FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->GetName ().CompareWithoutCaseCheck (name) == 0)
					return otherBot;
			}

			return NULL;
		}
		inline FakeClient_NotYaPB *const FindFakeClientNotYaPB (const unsigned char userID)
		{
			// Loop through all other bots....
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*otherBot) == userID)
					return otherBot;
			}

			return NULL;
		}
		inline const FakeClient_NotYaPB *const FindFakeClientNotYaPB (const unsigned char userID) const
		{
			// Loop through all other bots....
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				const FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if ( HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*otherBot) == userID)
					return otherBot;
			}

			return NULL;
		}

		inline void ExecuteCommand (const DynamicString &command) const
		{
			// Loop through all alive other bots....
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
					m_otherBotsArray[index]->ExecuteCommand (command);

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllFakeClientsHasExecutedCommand) + '\n', command.GetData ());
		}

		inline void SetHealth (const unsigned short value) const
		{
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->IsAlive ())
					otherBot->GetEdict ()->variables.health = value;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_FakeClientsHealthIsSetTo), value);
		}

		void ListFakeClientNotYaPB (const Human *const human) const;

		inline void KillAll (void) const
		{
			// This function kills all other bots on server.

			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
				m_otherBotsArray[index]->Kill ();

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_FakeClients).GetData ());
		}

		inline void KillAll (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function kills all other bots on server.

			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				const FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->GetRealTeam () == team)
					otherBot->Kill ();
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_FakeClients).GetData ());
		}

		inline void KickFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kick random other bot from specified team

			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->GetRealTeam () != team)
					continue;

				otherBot->Kick ();

				break;
			}
		}
		inline void KickAllFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kicks all other bots from specified team

			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->GetRealTeam () == team)
					otherBot->Kick ();
			}
		}

		void KickAll (void)
		{
			// This function drops all other bots from server.

			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
				m_otherBotsArray[index]->Kick ();	// Delete him/her/it from the list....

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKickedFromServer), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_FakeClients).GetData ());
		}

		inline const unsigned char GetFakeClientNotYaPBCount (void) const
		{
			// This function returns number of other bots playing on the server.

			return m_otherBotsArray.GetElementNumber ();	// Return the number of other bots.
		}

		inline const unsigned char GetFakeClientNotYaPBCount (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function returns number of other bots playing on the given team.

			unsigned char otherBotCount (0u);

			// Enumerate all the other bots in the server....
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
				if (m_otherBotsArray[index]->GetRealTeam () == team)
					++otherBotCount;	// This is a other bot.

			// Return the number of other bots.
			return otherBotCount;
		}

		inline FakeClient_NotYaPB *const GetAliveHighestFragsFakeClientNotYaPB (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			FakeClient_NotYaPB *aliveHighestFragsFakeClient (NULL);
			float bestScore (-1.0f);

			// Search other bots in this team.
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->GetRealTeam () != team || !otherBot->IsAlive () || otherBot->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsFakeClient = otherBot;	// Keep track of the highest frags other bot.
				bestScore = otherBot->GetEdict ()->variables.frags;	// Update best score.
			}

			return aliveHighestFragsFakeClient;
		}

		inline FakeClient_NotYaPB *const GetHighestFragsFakeClientNotYaPB (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			FakeClient_NotYaPB *highestFragsFakeClient (NULL);
			float bestScore (-1.0f);

			// Search other bots in this team.
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (otherBot->GetRealTeam () != team || otherBot->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsFakeClient = otherBot;	// Keep track of the highest frags other bot.
				bestScore = otherBot->GetEdict ()->variables.frags;	// Update best score.
			}

			return highestFragsFakeClient;
		}

		inline void SwapFakeClientNotYaPBTeams (void) const
		{
			// Loop through all other bot slots....
			for (unsigned char index (0u); index < m_otherBotsArray.GetElementNumber (); ++index)
			{
				const FakeClient_NotYaPB *const otherBot (m_otherBotsArray[index]);

				if (!otherBot->IsSpectator ())
					otherBot->ChangeTeam ();
			}
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void FakeClientNotYaPBConnect    (FakeClient_NotYaPB *const otherBot) { AddOtherBot (otherBot); }
		inline void FakeClientNotYaPBDisconnect (FakeClient_NotYaPB *const otherBot) { RemoveOtherBot (otherBot); }
};

#endif	// ifndef OTHER_BOT_MANAGER_INCLUDED