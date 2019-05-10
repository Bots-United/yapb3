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
// GameMessageHandlerManager.hpp
//
// Class: GameMessageHandlerManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef GAME_MESSAGE_HANDLER_MANAGER_INCLUDED
#define GAME_MESSAGE_HANDLER_MANAGER_INCLUDED

class GameMessageHandlerManager
{
	//
	// Group: Private members.
	//
	private:
		#include <GameMessageHandler.hpp>
/*
		static GameMessageHandler_ForClient_AmmoPickup        sm_gameMessageHandler_ForClient_AmmoPickup;
		static GameMessageHandler_ForClient_AmmoX             sm_gameMessageHandler_ForClient_AmmoX;
		static GameMessageHandler_ForClient_CurrentWeapon     sm_gameMessageHandler_ForClient_CurrentWeapon;
		static GameMessageHandler_ForYaPB_Damage              sm_gameMessageHandler_ForYaPB_Damage;
		static GameMessageHandler_ForYaPB_ScreenShake         sm_gameMessageHandler_ForYaPB_ScreenShake;
		static GameMessageHandler_ForYaPB_ScreenFade          sm_gameMessageHandler_ForYaPB_ScreenFade;
		static GameMessageHandler_ForHuman_ShowMenu           sm_gameMessageHandler_ForHuman_ShowMenu;
		static GameMessageHandler_ForYaPB_ShowMenu            sm_gameMessageHandler_ForYaPB_ShowMenu;
		static GameMessageHandler_ForYaPB_StatusIcon          sm_gameMessageHandler_ForYaPB_StatusIcon;
		static GameMessageHandler_ForYaPB_ReloadSound         sm_gameMessageHandler_ForYaPB_ReloadSound;
		static GameMessageHandler_TeamInfo                    sm_gameMessageHandler_TeamInfo;
		static GameMessageHandler_Text                        sm_gameMessageHandler_Text;
		static GameMessageHandler_ForYaPB_Text                sm_gameMessageHandler_ForYaPB_Text;
		static GameMessageHandler_ForYaPB_VGUIMenu            sm_gameMessageHandler_ForYaPB_VGUIMenu;
*/
		typedef GameMessageHandler *const (*const GameMessageHandlerConstructorFunction_t) (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);

		static const GameMessageHandlerConstructorFunction_t  sm_handlerConstructorFunctions[HalfLifeEngine::Engine::GameMessageContainer::Type_BotTotal/*HalfLifeEngine::SDK::Constants::MaximumUserMessages*/];

		GameMessageHandler                                   *m_currentGameMessage;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline  GameMessageHandlerManager (void) : m_currentGameMessage (NULL)
		{
			// Start hooking message sending functions....
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIMessageBeginFunction ();
		}
		inline ~GameMessageHandlerManager (void)
		{
			delete m_currentGameMessage;

			// Stop hooking message sending functions....
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIMessageSendingFunctions ();
		}

	//
	// Group: Private functions.
	//
	private:
//		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_NULL          (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to) { return NULL; }

		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_AmmoPickup    (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_AmmoX         (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_CurrentWeapon (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_Damage        (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_ScreenShake   (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_ScreenFade    (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_ShowMenu      (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_StatusIcon    (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_ReloadSound   (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_TeamInfo      (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, const HalfLifeEngine::SDK::Classes::Edict *const/* to*/) { return new GameMessageHandler_TeamInfo (); }
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_Text          (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.
		static inline GameMessageHandler *const GameMessageHandlerConstructorFunction_VGUIMenu      (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const Math::Vector3D *const origin, const HalfLifeEngine::SDK::Classes::Edict *const to);	// Note: This function declared in ClientManager.hpp.

	//
	// Group: Engine callbacks.
	//
	public:
		inline void MessageBegin (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const unsigned char id, const Math::Vector3D *const origin, HalfLifeEngine::SDK::Classes::Edict *const to)
		{
			// Reliability check.
			InternalAssert (m_currentGameMessage == NULL);
//			if (m_currentGameMessage != NULL)
//				AddLogEntry (false, LogLevel_Critical, true, "m_currentGameMessage(%s) != NULL, new message: %s (id=%u), type=%i, to=%s.", typeid (*m_currentGameMessage).name () + GET_STATIC_STRING_LENGTH ("Server::GameMessageHandlerManager::GameMessageHandler_"), HalfLifeEngine::SDK::Constants::g_serverToClientUserMessageNames[id], id, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetBotGameMessageTypeFromIndex (id), to == NULL ? "NULL" : to->IsPlayer () ? to->GetNetName ().GetData () : to->GetClassName ().GetData ());

			const HalfLifeEngine::Engine::GameMessageContainer::Type_t messageType (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetBotGameMessageTypeFromIndex (id));

			// Reliability checks.
			CompileTimeAssert (HalfLifeEngine::Engine::GameMessageContainer::Type_Undefined < HalfLifeEngine::Engine::GameMessageContainer::Type_BotTotal);
			CompileTimeAssert (static_cast <unsigned char> (HalfLifeEngine::Engine::GameMessageContainer::Type_Undefined) >= HalfLifeEngine::Engine::GameMessageContainer::Type_BotTotal);

			// Is this message does not require handling by bot?
			if (static_cast <unsigned char> (messageType) >= HalfLifeEngine::Engine::GameMessageContainer::Type_BotTotal)
				return;
/*
			Client *client (NULL);

			switch (messageType)
			{
				case HalfLifeEngine::Engine::GameMessageContainer::Type_AmmoPickup:
					client = g_server->GetClientManager ()->GetClient (to);

					m_currentGameMessage = &sm_gameMessageHandler_ForClient_AmmoPickup;

					m_currentGameMessage->Begin (destination, origin, to);

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_AmmoX:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					if (client == NULL)	// OCCURS ON CientDisconnect().
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForClient_AmmoX;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_CurrentWeapon:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					if (client == NULL)	// OCCURS ON CientDisconnect().
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForClient_CurrentWeapon;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_Damage:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					InternalAssert (client->IsValid ());	// OCCURS ON CientDisconnect().

					// Reliability check.
					if (!client->IsYaPB ())
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_Damage;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_ScreenShake:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					InternalAssert (client->IsValid ());

					// Reliability check.
					if (!client->IsYaPB ())
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_ScreenShake;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_ScreenFade:
					if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AimMethod)->GetValue <Console::Bot::VariableValue_AimMethod_t> () == Console::Bot::VariableValue_AimMethod_1)
						return;

					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					InternalAssert (client->IsValid ());

					// Reliability check.
					if (!client->IsYaPB ())
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_ScreenFade;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_ShowMenu:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					InternalAssert (client->IsValid ());

					// Reliability check.
					if (!client->IsYaPB ())
					{
						if (client->IsHuman ())
						{
							m_currentGameMessage = &sm_gameMessageHandler_ForHuman_ShowMenu;

							break;
						}

						return;
					}

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_ShowMenu;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_StatusIcon:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					if (client == NULL || !client->IsYaPB ())	// OCCURS ON CientDisconnect().
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_StatusIcon;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_ReloadSound:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					InternalAssert (client->IsValid ());

					// Reliability check.
					if (!client->IsYaPB () || !client->IsAlive ())
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_ReloadSound;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_TeamInfo:
					m_currentGameMessage = &sm_gameMessageHandler_TeamInfo;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_Text:
					// Is this message for client?
					if (to == NULL)
					{
						m_currentGameMessage = &sm_gameMessageHandler_Text;

						break;
					}

					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					if (client == NULL || !client->IsYaPB ())
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_Text;

					break;

				case HalfLifeEngine::Engine::GameMessageContainer::Type_VGUIMenu:
					client = g_server->GetClientManager ()->GetClient (to);

					// Reliability check.
					InternalAssert (client->IsValid ());

					// Reliability check.
					if (!client->IsYaPB ())
						return;

					m_currentGameMessage = &sm_gameMessageHandler_ForYaPB_VGUIMenu;

					break;

				default:
					return;
			}

			// Reliability check.
			InternalAssert (client->IsValid ());
*/
			// Else, begin handling the new game message.... :{P
			m_currentGameMessage = (*sm_handlerConstructorFunctions[messageType]) (destination, origin, to);	// Is this message for a client/bot? (if not so, ClientManager::GetClient() will returned 'NULL').

			if (m_currentGameMessage != NULL)
				m_currentGameMessage->Begin (destination, origin, to);
		}
		inline void MessageEnd   (void)
		{
			// If this message is for a bot, delete it....
			InternalAssert (m_currentGameMessage != NULL);

			m_currentGameMessage->End ();

			delete m_currentGameMessage;

			m_currentGameMessage = NULL;
		}
#if 0
		template <typename elementType> inline void Write (const elementType value)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->Execute (&value);
		}
		template </* char *const */> inline void Write (const char *const string)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->Execute (IsNullString (string) ? "" : string);
		}
#endif	// if 0
		inline void WriteByte (const unsigned char value)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteByte (value);
		}
		inline void WriteCharacter (const char character)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteCharacter (character);
		}
		inline void WriteShort (const short value)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteShort (value);
		}
		inline void WriteLong (const int value)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteLong (value);
		}
		inline void WriteAngle (const float value)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteAngle (value);
		}
		inline void WriteCoord (const float value)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteCoord (value);
		}
		inline void WriteString (const char *const string)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteString (string);
		}
		inline void WriteEntity (const unsigned short entityIndex)
		{
			// Check if current message is unneeded for us.
			InternalAssert (m_currentGameMessage != NULL);

			// If this message is for a bot, call the client message function....
			m_currentGameMessage->WriteEntity (entityIndex);
		}
};

#endif	// ifndef GAME_MESSAGE_HANDLER_MANAGER_INCLUDED