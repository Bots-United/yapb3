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
// GameMessageHandler.hpp
//
// Class: GameMessageHandler
//
// Description: Handles Messages sent from the Server to a Clients.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef GAME_MESSAGE_HANDLER_INCLUDED
#define GAME_MESSAGE_HANDLER_INCLUDED

class GameMessageHandler
{
	friend GameMessageHandlerManager;

	DECLARE_CLASS_WITHOUT_BASE (GameMessageHandler);

	//
	// Group: Protected members.
	//
	protected:
		static unsigned char m_state;	// Message state, with which the execution will be begun. (1 - based)
#if 0
		/// @todo WORK WITH MESSAGES AFTER STORE IT'S BUFFER!!!
		union /* Unnamed */
		{
			HalfLifeEngine::SDK::TypeDefinitions::GameMessageBuffer_t    buffer;	// Raw message buffer.

			HalfLifeEngine::SDK::Structures::UserMessages::AmmoPickup_t  ammoPickup;
			HalfLifeEngine::SDK::Structures::UserMessages::AmmoX_t       ammoX;
			HalfLifeEngine::SDK::Structures::UserMessages::CurWeapon_t   currentWeapon;
			HalfLifeEngine::SDK::Structures::UserMessages::Damage_t      damage;
			HalfLifeEngine::SDK::Structures::UserMessages::ScreenShake_t screenShake;
			HalfLifeEngine::SDK::Structures::UserMessages::ScreenFade_t  screenFade;	// Error: C2621
			HalfLifeEngine::SDK::Structures::UserMessages::ShowMenu_t    showMenu;
			HalfLifeEngine::SDK::Structures::UserMessages::StatusIcon_t  statusIcon;
			HalfLifeEngine::SDK::Structures::UserMessages::ReloadSound_t reloadSound;
			HalfLifeEngine::SDK::Structures::UserMessages::TeamInfo_t    teamInfo;
			HalfLifeEngine::SDK::Structures::UserMessages::TextMsg_t     text;
			HalfLifeEngine::SDK::Structures::UserMessages::VGUIMenu_t    VGUIMenu;
		} m_message;
#endif	// if 0
	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler (const unsigned char state = 1u) { m_state = state; }
		virtual inline ~GameMessageHandler (void) { /* VOID */ }

	//
	// Group: Private operators.
	//
	private:
		inline GameMessageHandler &operator = (const GameMessageHandler &/*right*/);	// Avoid "warning C4512: 'GameMessageHandler' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		virtual void Execute (const void *const pointer) =/*>*/ 0;

		virtual inline void Begin           (const HalfLifeEngine::SDK::Constants::MessageDestination_t/* destination*/, const Math::Vector3D *const/* origin*/, HalfLifeEngine::SDK::Classes::Edict *const/* to*/)
		{
			m_state = 1u;	// Reset network message state.

			EnableForwards ();	// Tell DLL to forward required 'Write*' functions....
		}
		virtual inline void End             (void)
		{
			DisableForwards ();	// Stop DLL forwarding....
		}
		virtual inline void WriteByte       (const unsigned char value)                                                                                                                                { Execute (&value); }//=/*>*/ 0;
		virtual inline void WriteCharacter  (const char character)                                                                                                                                     { Execute (&character); }//=/*>*/ 0;
		virtual inline void WriteShort      (const short value)                                                                                                                                        { Execute (&value); }//=/*>*/ 0;
		virtual inline void WriteLong       (const int value)                                                                                                                                          { Execute (&value); }//=/*>*/ 0;
		virtual inline void WriteAngle      (const float value)                                                                                                                                        { Execute (&value); }//=/*>*/ 0;
		virtual inline void WriteCoord      (const float value)                                                                                                                                        { Execute (&value); }//=/*>*/ 0;
		virtual inline void WriteString     (const char *const string)                                                                                                                                 { Execute (IsNullString (string) ? "" : string); }//=/*>*/ 0;
		virtual inline void WriteEntity     (const unsigned short entityIndex)                                                                                                                         { Execute (&entityIndex); }//=/*>*/ 0;

		virtual inline void EnableForwards  (void)
		{
			// Tell DLL to forward required 'Write*' functions....
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteByteFunction   ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteCharFunction   ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteShortFunction  ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteLongFunction   ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteAngleFunction  ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteCoordFunction  ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteStringFunction ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIWriteEntityFunction ();
			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIMessageEndFunction  ();
		}
		virtual inline void DisableForwards (void)
		{
			// Stop DLL forwarding....
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteByteFunction   ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteCharFunction   ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteShortFunction  ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteLongFunction   ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteAngleFunction  ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteCoordFunction  ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteStringFunction ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIWriteEntityFunction ();
			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIMessageEndFunction  ();
		}
};
template <typename clientType> class GameMessageHandler_For : public GameMessageHandler
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler, GameMessageHandler_For);

	//
	// Group: Type definitions.
	//
	public:
		typedef clientType ClientType_t;

	//
	// Group: Protected members.
	//
	protected:
		/*static - @todo !!! */clientType *const m_client;	// Message owner (client to which this message is sent).

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_For (clientType *const to, const unsigned char state = 1u) :
			GameMessageHandler (state),

			m_client (to)
		{
			// Reliability check.
			InternalAssert (to->IsValid ());	// OCCURS ON KICKING BOT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
};
typedef GameMessageHandler_For <Client> GameMessageHandler_ForClient;
typedef GameMessageHandler_For <Human>  GameMessageHandler_ForHuman;
typedef GameMessageHandler_For <YaPB>   GameMessageHandler_ForYaPB;

class GameMessageHandler_ForClient_AmmoPickup : public GameMessageHandler_ForClient
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForClient, GameMessageHandler_ForClient_AmmoPickup);

	//
	// Group: Private members.
	//
	private:
		HalfLifeEngine::SDK::Constants::WeaponID_t m_ammoID;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForClient_AmmoPickup (Client *const to) : GameMessageHandler_ForClient (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForClient_AmmoX : public GameMessageHandler_ForClient
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForClient, GameMessageHandler_ForClient_AmmoX);

	//
	// Group: Private members.
	//
	private:
		HalfLifeEngine::SDK::Constants::WeaponID_t m_ammoID;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForClient_AmmoX (Client *const to) : GameMessageHandler_ForClient (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForClient_CurrentWeapon : public GameMessageHandler_ForClient
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForClient, GameMessageHandler_ForClient_CurrentWeapon);

	//
	// Group: Private members.
	//
	private:
		bool                                       m_weaponState;
		HalfLifeEngine::SDK::Constants::WeaponID_t m_weaponID;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForClient_CurrentWeapon (Client *const to) : GameMessageHandler_ForClient (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForYaPB_Damage : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_Damage);

	//
	// Group: Private members.
	//
	private:
		unsigned char  m_damageArmor;
		unsigned short m_damageTaken;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_Damage (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForYaPB_ScreenShake : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_ScreenShake);

	//
	// Group: Private members.
	//
	private:
		unsigned short m_amplitude, m_duration;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_ScreenShake (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForYaPB_ScreenFade : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_ScreenFade);

	//
	// Group: Private members.
	//
	private:
		ColorWithAlpha <> m_fadeColor;
		bool              m_isStayOut;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_ScreenFade (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

struct ShowMenu_t
{
	HalfLifeEngine::SDK::Constants::Menu::KeyBit_t validSlots;
	char                                           lifeTime;
	bool                                           isNeedMore;

/// @todo !!!	typedef StaticString <HalfLifeEngine::SDK::Constants::MaximumMenuFullStringLength> StringType_t;
	typedef DynamicString StringType_t;
	static StringType_t                            fullString;
};
class GameMessageHandler_ShowMenu : public GameMessageHandler
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler, GameMessageHandler_ShowMenu);

	//
	// Group: Private members.
	//
	private:
		ShowMenu_t m_information;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ShowMenu (void) : GameMessageHandler () { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);

		inline void WriteCharacter (const char character)
		{
			// Not a string?
			if (m_state != 4u)
			{
				// Call this function in base class....
				BaseClass::WriteCharacter (character);

				return;
			}

			// Reliability check.
			InternalAssert (m_information.isNeedMore == true);

			if (character != '\0')
				m_information.fullString += character;
		}
};
class GameMessageHandler_ForHuman_ShowMenu : public GameMessageHandler_ForHuman
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForHuman, GameMessageHandler_ForHuman_ShowMenu);

	//
	// Group: Private members.
	//
	private:
		ShowMenu_t m_information;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForHuman_ShowMenu (Human *const to) : GameMessageHandler_ForHuman (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);

		inline void WriteCharacter (const char character)
		{
			// Not a string?
			if (m_state != 4u)
			{
				// Call this function in base class....
				BaseClass::WriteCharacter (character);

				return;
			}

			// Reliability check.
			InternalAssert (m_information.isNeedMore == true);

			if (character != '\0')
				m_information.fullString += character;
		}
};
class GameMessageHandler_ForYaPB_ShowMenu : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_ShowMenu);

	//
	// Group: Private members.
	//
	private:
		ShowMenu_t m_information;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_ShowMenu (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);

		inline void WriteCharacter (const char character)
		{
			// Not a string?
			if (m_state != 4u)
			{
				// Call this function in base class....
				BaseClass::WriteCharacter (character);

				return;
			}

			// Reliability check.
			InternalAssert (m_information.isNeedMore == true);

			if (character != '\0')
				m_information.fullString += character;
		}
};

class GameMessageHandler_ForYaPB_StatusIcon : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_StatusIcon);

	//
	// Group: Private members.
	//
	private:
		HalfLifeEngine::SDK::Constants::StatusIcon_StatusType_t m_status;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_StatusIcon (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForYaPB_ReloadSound : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_ReloadSound);

	//
	// Group: Private members.
	//
	private:
		unsigned char m_volume;
		bool          m_isNotShotgun;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_ReloadSound (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_TeamInfo : public GameMessageHandler
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler, GameMessageHandler_TeamInfo);

	//
	// Group: Private members.
	//
	private:
		unsigned char m_playerEdictIndex;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_TeamInfo (void) : GameMessageHandler () { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_Text : public GameMessageHandler
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler, GameMessageHandler_Text);

	//
	// Group: Private members.
	//
	private:
		DynamicString m_message;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_Text (void) : GameMessageHandler () { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForYaPB_Text : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_Text);

	//
	// Group: Private members.
	//
	private:
		DynamicString m_message;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_Text (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

class GameMessageHandler_ForYaPB_VGUIMenu : public GameMessageHandler_ForYaPB
{
	DECLARE_CLASS_WITH_BASE (GameMessageHandler_ForYaPB, GameMessageHandler_ForYaPB_VGUIMenu);

	//
	// Group: (Con/De)structors.
	//
	public:
		inline GameMessageHandler_ForYaPB_VGUIMenu (YaPB *const to) : GameMessageHandler_ForYaPB (to) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		void Execute (const void *const pointer);
};

#endif	// ifndef GAME_MESSAGE_HANDLER_INCLUDED