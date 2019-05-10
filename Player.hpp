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
// Player.hpp
//
// Class: Player
//
// Description: Contains the 'Player' class Function Prototypes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

class Player : public Human
{
	friend ClientManager;

	DECLARE_CLASS_WITH_BASE (Human, Player);

	//
	// Group: Private members.
	//
	private:
		unsigned int m_commandAccessLevel;

		bool         m_isAdmin;					// player is YaPB admin
		float        m_updateAdminStatusTime;	// time to update 'm_isAdmin' variable.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Player (HalfLifeEngine::SDK::Classes::Edict *const player) :
			Human (player),

			m_commandAccessLevel (0u),

			m_isAdmin (false),
			m_updateAdminStatusTime (0.0f)
		{ /* VOID */ }
		virtual inline ~Player (void) { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_Player; }

		inline void CheckAdminStatus (void)
		{
			const DynamicString password (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Password)->GetString ());
			const DynamicString key (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PasswordKey)->GetString ());

			if (m_isAdmin)
			{
				if (key.IsEmpty () && password.IsEmpty ())
					m_isAdmin = false;
				else if (const_cast <const char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetInfoKeyValue (m_edict->GetInfoKeyBuffer (), key)) == password)
				{
					m_isAdmin = false;

					AddLogEntry (true, LogLevel_Default, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_PlayerHadLostRemoteAccessToYaPB), GetName ().GetData ());
				}
			}
			else if (key.IsEmpty () && password.IsEmpty () && const_cast <const char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetInfoKeyValue (m_edict->GetInfoKeyBuffer (), key)) == password)
			{
				m_isAdmin = true;

				AddLogEntry (true, LogLevel_Default, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_PlayerHadGainedFullRemoteAccessToYaPB), GetName ().GetData ());
			}
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsHost
		//
		// Description: Checks whether client is host.
		//
		// Returns: True if client is host, false otherwise.
		//
		inline const bool                   IsHost                    (void) const { return false; }

		//
		// Function: IsPlayer
		//
		// Description: Checks whether client is player.
		//
		// Returns: True if client is player, false otherwise.
		//
		inline const bool                   IsPlayer                  (void) const { return true; }

		inline const unsigned int           GetCommandAccessLevel     (void)                       const { return m_commandAccessLevel; }
		inline const bool                   HasCommandAccessLevel     (const unsigned short level) const { return (m_commandAccessLevel & level) > 0u; }

		//
		// Function: IsAdmin
		//
		// Description: Checks whether client is YaPB admin.
		//
		// Returns: True if client is YaPB admin, false otherwise.
		//
		inline const bool                   IsAdmin                   (void) const { return m_isAdmin; }

//		const BotConsoleCommandReturnType_t BotCommandHandler         (const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5, const DynamicString &argument6);

	//
	// Group: Engine callbacks.
	//
	public:
		inline void UserInfoChanged (const DynamicString &infoBuffer)
		{
			// Called on client user info changed.

			// Call this function in base class....
			Human::UserInfoChanged (infoBuffer);

			const DynamicString password (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Password)->GetString ());
			const DynamicString passwordField (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PasswordKey)->GetString ());

			if (password.IsEmpty () || passwordField.IsEmpty ())
				return;

			m_isAdmin = const_cast <const char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetInfoKeyValue (infoBuffer, passwordField)) == password;

			AddLogEntry (true, LogLevel_Default, false, "Player::UserInfoChanged(): called for player \"%s\", 'm_isAdmin' is changed to: %s.", GetName ().GetData (), m_isAdmin ? "true" : "false");
		}
		inline void Think           (void)
		{
			// code below is executed only on dedicated server
			if (m_updateAdminStatusTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
			{
				CheckAdminStatus ();

				// update timer to 1.5 second
				m_updateAdminStatusTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.5f;
			}

			// Call this function in base class....
			Human::Think ();
		}
};

inline       Player *const Client::GetPlayerPointer (void)
{
	// Reliability check.
	InternalAssert (IsPlayer ());

	return static_cast <Player *const> (this);
}
inline const Player *const Client::GetPlayerPointer (void) const
{
	// Reliability check.
	InternalAssert (IsPlayer ());

	return static_cast <const Player *const> (this);
}

#endif	// ifndef PLAYER_INCLUDED