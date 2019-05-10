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
// HumanManager.hpp
//
// Class: HumanManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HUMAN_MANAGER_INCLUDED
#define HUMAN_MANAGER_INCLUDED

class HumanManager
{
	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <Human *, unsigned char> HumansArray_t;

	//
	// Group: Private members.
	//
	private:
		HumansArray_t m_humansArray;	// For quickly humans looping/counting.

	//
	// Group: Functions.
	//
	public:
		inline Host *const GetHostClient (void)
		{
			// Reliability check.
			if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
			{
				#if defined _DEBUG
					// Loop through all humans....
					for (HumansArray_t::IndexType_t index (0u); index < m_humansArray.GetElementNumber (); ++index)
					{
						Human *const human (m_humansArray[index]);

						// Never send to local client, since the ConVar is directly linked there
						if (human->IsHost ())
							return human->GetHostPointer ();
					}
				#endif	// if defined _DEBUG

				return NULL;	// If no host edict, return NULL.
			}

			// Reliability checks.
			InternalAssert (g_server->GetClientManager ()->GetClient (HalfLifeEngine::SDK::Constants::ListenServerHostClientSlotIndex)->IsValid ());
			InternalAssert (g_server->GetClientManager ()->GetClient (HalfLifeEngine::SDK::Constants::ListenServerHostClientSlotIndex)->IsHost ());

			// Host client is always the first one in listen server.
			return g_server->GetClientManager ()->GetClient (HalfLifeEngine::SDK::Constants::ListenServerHostClientSlotIndex)->GetHostPointer ();
		}
		inline const Host *const GetHostClient (void) const
		{
			// Reliability check.
			if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
			{
				#if defined _DEBUG
					// Loop through all humans....
					for (HumansArray_t::IndexType_t index (0u); index < m_humansArray.GetElementNumber (); ++index)
					{
						Human *const human (m_humansArray[index]);

						// Never send to local client, since the ConVar is directly linked there
						if (human->IsHost ())
							return human->GetHostPointer ();
					}
				#endif	// if defined _DEBUG

				return NULL;	// If no host edict, return NULL.
			}

			// Reliability checks.
			InternalAssert (g_server->GetClientManager ()->GetClient (HalfLifeEngine::SDK::Constants::ListenServerHostClientSlotIndex)->IsValid ());
			InternalAssert (g_server->GetClientManager ()->GetClient (HalfLifeEngine::SDK::Constants::ListenServerHostClientSlotIndex)->IsHost ());

			// Host client is always the first one in listen server.
			return g_server->GetClientManager ()->GetClient (HalfLifeEngine::SDK::Constants::ListenServerHostClientSlotIndex)->GetHostPointer ();
		}
		inline const bool IsHostClientInServer (void) const
		{
			return !HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () && g_server->GetClientManager ()->GetClient (HalfLifeEngine::SDK::Constants::ListenServerHostClientSlotIndex) != NULL;
		}

		inline Human *const GetHuman (const unsigned char index)
		{
			// This function finds a human specified by index, and then returns pointer to it (using own 'Human' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (index)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[index]->GetHumanPointer ();
		}
		inline const Human *const GetHuman (const unsigned char index) const
		{
			// This function finds a human specified by index, and then returns pointer to it (using own 'Human' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (index)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[index]->GetHumanPointer ();
		}
		inline Human *const GetHuman (const HalfLifeEngine::SDK::Classes::Edict *const human)
		{
			// This function finds a human specified by edict, and then returns pointer to it (using own 'Human' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (human)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[human->GetIndex () - 1u]->GetHumanPointer ();
		}
		inline const Human *const GetHuman (const HalfLifeEngine::SDK::Classes::Edict *const human) const
		{
			// This function finds a human specified by edict, and then returns pointer to it (using own 'Human' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (human)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[human->GetIndex () - 1u]->GetHumanPointer ();
		}

		inline       HumansArray_t &GetHumansArray (void)
		{
			// This function returns a reference to non-constant humans array.

			return m_humansArray;
		}
		inline const HumansArray_t &GetHumansArray (void) const
		{
			// This function returns a reference to constant humans array.

			return m_humansArray;
		}

		inline void KickRandom (void) const
		{
			// This function kicks random human from server.

			if (!m_humansArray.IsEmpty ())
				m_humansArray.GetRandomElement ()->Kick ();
		}

		inline Human *const FindHuman (const DynamicString &name)
		{
			// Loop through all humans....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				Human *const human (m_humansArray[index]);

				if (human->GetName ().CompareWithoutCaseCheck (name) == 0)
					return human;
			}

			return NULL;
		}
		inline const Human *const FindHuman (const DynamicString &name) const
		{
			// Loop through all humans....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				const Human *const human (m_humansArray[index]);

				if (human->GetName ().CompareWithoutCaseCheck (name) == 0)
					return human;
			}

			return NULL;
		}
		inline Human *const FindHuman (const unsigned char userID)
		{
			// Loop through all humans....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				Human *const human (m_humansArray[index]);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*human) == userID)
					return human;
			}

			return NULL;
		}
		inline const Human *const FindHuman (const unsigned char userID) const
		{
			// Loop through all humans....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				const Human *const human (m_humansArray[index]);

				if ( HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*human) == userID)
					return human;
			}

			return NULL;
		}

		inline void ExecuteCommand (const DynamicString &command) const
		{
			// Loop through all alive humans....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
					m_humansArray[index]->ExecuteCommand (command);

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllHumansHasExecutedCommand) + '\n', command.GetData ());
		}

		inline void SetHealth (const unsigned short value) const
		{
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				Human *const human (m_humansArray[index]);

				if (human->IsAlive ())
					human->GetEdict ()->variables.health = value;
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_HumansHealthIsSetTo), value);
		}

		void ListHumans (const Human *const human) const;

		inline void KillAll (void) const
		{
			// This function kills all humans on server.

			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
				m_humansArray[index]->Kill ();

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_Humans).GetData ());
		}

		inline void KillAll (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function kills all humans on server.

			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				const Human *const human (m_humansArray[index]);

				if (human->GetRealTeam () == team)
					human->Kill ();
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_Humans).GetData ());
		}

		inline void KickFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kick random human from specified team

			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				Human *const human (m_humansArray[index]);

				if (human->GetRealTeam () != team)
					continue;

				human->Kick ();

				break;
			}
		}
		inline void KickAllFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kicks all humans from specified team

			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				Human *const human (m_humansArray[index]);

				if (human->GetRealTeam () == team)
					human->Kick ();
			}
		}

		void KickAll (void)
		{
			// This function drops all humans from server.

			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
				m_humansArray[index]->Kick ();	// Delete him/her/it from the list....

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKickedFromServer), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_Humans).GetData ());
		}

		inline const unsigned char GetHumansCount (void) const
		{
			// This function returns number of humans playing on the server.

			return m_humansArray.GetElementNumber ();	// Return the number of humans.
		}
		inline const unsigned char GetHumansCount (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function returns number of humans playing on the given team.

			unsigned char humanCount (0u);

			// Enumerate all the humans in the server....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
				if (m_humansArray[index]->GetRealTeam () == team)
					++humanCount;	// This is a human.

			// Return the number of humans.
			return humanCount;
		}
		inline const unsigned char GetAliveHumansCount (void) const
		{
			// This function returns number of alive humans playing on the server.

			unsigned char aliveHumanCount (0u);

			// Enumerate all the humans in the server....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
				if (m_humansArray[index]->IsAlive ())
					++aliveHumanCount;	// This is a alive human.

			// Return the number of alive humans.
			return aliveHumanCount;
		}
		inline const unsigned char GetAliveHumansCount (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function returns number of alive humans playing on the given team.

			unsigned char aliveHumanCount (0u);

			// Enumerate all the humans in the server....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
				if (m_humansArray[index]->IsAlive () && m_humansArray[index]->GetRealTeam () == team)
					++aliveHumanCount;	// This is a alive human.

			// Return the number of alive humans.
			return aliveHumanCount;
		}

		inline Human *const GetAliveHighestFragsHuman (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			Human *aliveHighestFragsHuman (NULL);
			float bestScore (-1.0f);

			// Search humans in this team.
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				Human *const human (m_humansArray[index]);

				if (human->GetRealTeam () != team || !human->IsAlive () || human->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsHuman = human;	// Keep track of the highest frags human.
				bestScore = human->GetEdict ()->variables.frags;	// Update best score.
			}

			return aliveHighestFragsHuman;
		}

		inline Human *const GetHighestFragsHuman (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			Human *highestFragsHuman (NULL);
			float bestScore (-1.0f);

			// Search humans in this team.
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				Human *const human (m_humansArray[index]);

				if (human->GetRealTeam () != team || human->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsHuman = human;	// Keep track of the highest frags human.
				bestScore = human->GetEdict ()->variables.frags;	// Update best score.
			}

			return highestFragsHuman;
		}

		inline void SwapHumansTeams (void) const
		{
			// Loop through all human slots....
			for (unsigned char index (0u); index < m_humansArray.GetElementNumber (); ++index)
			{
				const Human *const human (m_humansArray[index]);

				if (!human->IsSpectator ())
					human->ChangeTeam ();
			}
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void HumanConnect    (Human *const human) { m_humansArray += human; }
		inline void HumanDisconnect (Human *const human)
		{
			m_humansArray -= human;

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutoVacate)->GetValue <bool> () && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> () < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients () - 1u)
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue <unsigned char> (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> () + 1u);
		}
};

#if defined _DEBUG
void YaPB::DrawApproachPoints (void) const
{
	for (ApproachPointArray_t::IndexType_t index (0u); index < m_approachPoints.GetElementNumber (); ++index)
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_approachPoints[index], m_approachPoints[index] + Math::Vector3D (0.0f, 0.0f, 50.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 255u), 250u, 5u, 1u, false);
}
#endif	// if defined _DEBUG

inline const bool YaPB::CanDoingScenario (void) const
{
	return !Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DeferToHuman)->GetValue <bool> () || g_server->GetHumanManager ()->GetAliveHumansCount (GetRealTeam ()) == 0u;
}
/*
inline void YaPB::ChatMessage (const ChatManager::ChatMessage_t type, const bool isTeamSay)
{
	if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <bool> () || g_chatFactory[type].IsEmpty ())
		return;

	PrepareChatMessage (g_chatFactory[type].GetRandomElement ());

	PushMessageQueue (isTeamSay ? GameAction_TeamSay : GameAction_SayMsg);
}
*/
inline const bool YaPB::IsCanUseRadio (void) const
{
	return IsAlive () && g_server->GetClientManager ()->GetAliveClientsNumber (GetRealTeam ()) > 0u && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_Radio && g_server->IsTeamplay () && g_server->GetHumanManager ()->GetHumansCount () > 0u;
}

inline void YaPB::RadioMessage (const HalfLifeEngine::SDK::Constants::RadioCommand_t message)
{
	// this function inserts the radio message into the message queue.

	if (!IsCanUseRadio ())
		return;
/*
	// use radio instead voice
	if (g_chatterFactory[message].IsEmpty () || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_Radio)
		m_radioInsteadVoice = true;

	m_radioSelect = message;

	PushMessageQueue (GameAction_RadioMessage);*/
	ExecuteCommand (HalfLifeEngine::SDK::Constants::RadioCommandAliasNames[message]);
}

inline void YaPB::RadioMessage (const HalfLifeEngine::SDK::Constants::RadioCommand_t/* message*/, const float sayTime)
{
	// this function inserts the radio message into the message queue.

	// Reliability check.
	InternalAssert (sayTime > 0.0f);

	if (!IsCanUseRadio ())
		return;
/*
	// use radio instead voice
	if (g_chatterFactory[message].IsEmpty () || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_Radio)
		m_radioInsteadVoice = true;

	m_radioSelect = message;

	PushMessageQueue (GameAction_RadioMessage);*/
//	ExecuteCommand (HalfLifeEngine::SDK::Constants::RadioCommandAliasNames[message]);
}

inline const bool YaPB::IsCanUseChatter (void) const
{
	return (IsAlive () ? g_server->GetClientManager ()->GetAliveClientsNumber (GetRealTeam ()) > 0u : g_server->GetClientManager ()->GetDeadClientsNumber () > 0u) && m_profile->communicationType == CommunicationType_Chatter && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_Voice && g_server->IsTeamplay () && g_server->GetHumanManager ()->GetHumansCount () > 0u;
}

inline void YaPB::ChatterMessage (const ::ChatterManager::ChatterMessage_t chatterMessageIndex)
{
	// This function inserts the voice message into the message queue (mostly same as above).

	if (!IsCanUseChatter ())
		return;
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_Radio)
		switch (chatterMessageIndex)
		{
			case ::ChatterManager::ChatterMessage_SpotTheBomber:
			case ::ChatterManager::ChatterMessage_HearSomething:
			case ::ChatterManager::ChatterMessage_HeardEnemy:
			case ::ChatterManager::ChatterMessage_VIPSpotted:
			case ::ChatterManager::ChatterMessage_SpottedTwoEnemies:
			case ::ChatterManager::ChatterMessage_SpottedThreeEnemies:
			case ::ChatterManager::ChatterMessage_SpottedTooManyEnemies:
				m_chatterManager.InstantChatterMessage (this, static_cast <::ChatterManager::ChatterMessage_t> (HalfLifeEngine::SDK::Constants::RadioCommand_EnemySpotted));

				return;

			case ::ChatterManager::ChatterMessage_SniperKilled:
				m_chatterManager.InstantChatterMessage (this, static_cast <::ChatterManager::ChatterMessage_t> (HalfLifeEngine::SDK::Constants::RadioCommand_EnemyDown));

				return;

			case ::ChatterManager::ChatterMessage_BombSiteSecure:
				m_chatterManager.InstantChatterMessage (this, static_cast <::ChatterManager::ChatterMessage_t> (HalfLifeEngine::SDK::Constants::RadioCommand_RegroupTeam));

				return;

			default:
				return;
		}
*/
	m_chatterManager.InstantChatterMessage (this, chatterMessageIndex);
}

inline void YaPB::ChatterMessage (const ::ChatterManager::ChatterMessage_t chatterMessageIndex, const float sayTime)
{
	// This function inserts the voice message into the message queue (mostly same as above).

	// Reliability check.
	InternalAssert (sayTime > 0.0f);

	if (!IsCanUseChatter ())
		return;
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_Radio)
		switch (chatterMessageIndex)
		{
			case ::ChatterManager::ChatterMessage_SpotTheBomber:
			case ::ChatterManager::ChatterMessage_HearSomething:
			case ::ChatterManager::ChatterMessage_HeardEnemy:
			case ::ChatterManager::ChatterMessage_VIPSpotted:
			case ::ChatterManager::ChatterMessage_SpottedTwoEnemies:
			case ::ChatterManager::ChatterMessage_SpottedThreeEnemies:
			case ::ChatterManager::ChatterMessage_SpottedTooManyEnemies:
				m_chatterManager.PushChatterMessage (m_profile->chatterBankIndex, static_cast <::ChatterManager::ChatterMessage_t> (HalfLifeEngine::SDK::Constants::RadioCommand_EnemySpotted), sayTime);

				return;

			case ::ChatterManager::ChatterMessage_SniperKilled:
				m_chatterManager.PushChatterMessage (m_profile->chatterBankIndex, static_cast <::ChatterManager::ChatterMessage_t> (HalfLifeEngine::SDK::Constants::RadioCommand_EnemyDown), sayTime);

				return;

			case ::ChatterManager::ChatterMessage_BombSiteSecure:
				m_chatterManager.PushChatterMessage (m_profile->chatterBankIndex, static_cast <::ChatterManager::ChatterMessage_t> (HalfLifeEngine::SDK::Constants::RadioCommand_RegroupTeam), sayTime);

				return;

			default:
				return;
		}
*/
	m_chatterManager.PushChatterMessage (m_profile->chatterBankIndex, chatterMessageIndex, sayTime);
}

inline const bool NavigationMesh::GetEditVectors (Math::Vector3D &eyePosition, Math::Vector3D &viewDirection)
{
	///< Gets the eye position and view direction of the editing player.

	const Client *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (!player->IsValid ())
		return false;

	eyePosition = player->GetEdict ()->GetEyePosition ();
	viewDirection = player->GetEyeForwardDirection ();

	return true;
}

#endif	// ifndef HUMAN_MANAGER_INCLUDED