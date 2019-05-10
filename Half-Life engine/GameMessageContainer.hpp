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
// GameMessageContainer.hpp
//
// Class: GameMessageContainer
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef GAME_MESSAGE_CONTAINER_INCLUDED
#define GAME_MESSAGE_CONTAINER_INCLUDED

class GameMessageContainer
{
	//
	// Group: Constants.
	//
	public:
		// Network message functions.
		enum Type_t
		{
			Type_Undefined = -1,	// network message terminator

			// bot messages (which we hooking)...
			Type_AmmoPickup,
			Type_AmmoX,
			Type_CurrentWeapon,
			Type_Damage,
			Type_ScreenShake,
			Type_ScreenFade,
			Type_ShowMenu,
			Type_StatusIcon,
			Type_ReloadSound,
			Type_TeamInfo,
			Type_Text,
			Type_VGUIMenu,

			Type_BotTotal,	// total of bot network messages

			// ...and client messages too (which we NOT hooking)...
			Type_BotProgress = Type_BotTotal,
			Type_BotVoice,
			Type_MOTD,
			Type_SendAudio,
			Type_ServerName,

			Type_Total	// total of all network messages
		};

	//
	// Group: Private members.
	//
	private:
		// Messages created in RegUserMsg() which will be "caught".
		unsigned char m_registeredGameMessages[Type_Total];
		Type_t        m_registeredGameMessageTypes[SDK::Constants::MaximumUserMessages];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageContainer (void)
		{
			unsigned short index;

			for (index = Type_AmmoPickup; index < Type_Total; ++index)
				m_registeredGameMessages[index] = SDK::Constants::SVC_BAD;

			for (index = SDK::Constants::SVC_BAD; index < SDK::Constants::MaximumUserMessages; ++index)
				m_registeredGameMessageTypes[index] = Type_Undefined;
		}

	//
	// Group: Functions.
	//
	public:
		inline void Register (const DynamicString &name, const unsigned char id)
		{
			// Reliability check.
			InternalAssert (id >= SDK::Constants::GameMessageID_Start);

			#define CHECK_GAME_MESSAGE(checkName, type)									\
				if (name == #checkName)													\
				{																		\
					/* Reliability check. */											\
					InternalAssert (id == SDK::Constants::GameMessageID_##checkName);	\
																						\
					/* Record this message's index.... */								\
					m_registeredGameMessages[type] = id;								\
					m_registeredGameMessageTypes[id] = type;							\
				}

			// now keep track (or update if necessary) this user message in our own array...
				 CHECK_GAME_MESSAGE (AmmoPickup,  Type_AmmoPickup)
			else CHECK_GAME_MESSAGE (AmmoX,       Type_AmmoX)
			else CHECK_GAME_MESSAGE (CurWeapon,   Type_CurrentWeapon)
			else CHECK_GAME_MESSAGE (Damage,      Type_Damage)
			else CHECK_GAME_MESSAGE (ScreenShake, Type_ScreenShake)
			else CHECK_GAME_MESSAGE (ScreenFade,  Type_ScreenFade)
			else CHECK_GAME_MESSAGE (ShowMenu,    Type_ShowMenu)
			else CHECK_GAME_MESSAGE (StatusIcon,  Type_StatusIcon)
			else CHECK_GAME_MESSAGE (ReloadSound, Type_ReloadSound)
			else CHECK_GAME_MESSAGE (TeamInfo,    Type_TeamInfo)
			else CHECK_GAME_MESSAGE (TextMsg,     Type_Text)
			else CHECK_GAME_MESSAGE (VGUIMenu,    Type_VGUIMenu)
			// ...and not handleable client messages too... (we only needed id's of this messages)
			else CHECK_GAME_MESSAGE (BotProgress, Type_BotProgress)
			else CHECK_GAME_MESSAGE (BotVoice,    Type_BotVoice)
			else CHECK_GAME_MESSAGE (MOTD,        Type_MOTD)
			else CHECK_GAME_MESSAGE (SendAudio,   Type_SendAudio)
			else CHECK_GAME_MESSAGE (ServerName,  Type_ServerName)

			#undef CHECK_GAME_MESSAGE
		}

		inline const Type_t GetBotGameMessageTypeFromIndex (const unsigned char id) const
		{
/*			// Loop through all needed for handling game message types....
			for (unsigned char index (Type_AmmoPickup); index < Type_BotTotal; ++index)
				if (id == m_registeredGameMessages[index])
					return static_cast <Type_t> (index);	// If sought client message type is found, so return it.... :{P

			// Not found.
			return Type_Undefined;*/
			return m_registeredGameMessageTypes[id];
		}

		inline const unsigned char GetRegisteredGameMessageIndexByType (const Type_t messageType) const
		{
			// Reliability check.
			InternalAssert (messageType > Type_Undefined && messageType < Type_Total);
			InternalAssert (m_registeredGameMessages[messageType] > SDK::Constants::SVC_BAD);

			return m_registeredGameMessages[messageType];
		}
};

#endif	// ifndef GAME_MESSAGE_CONTAINER_INCLUDED