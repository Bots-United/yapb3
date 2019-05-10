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
// GameMessageHandler.cpp
//
// Class: GameMessageHandler
//
// Description: Handles Messages sent from the Server to a Clients.
//
// Version: $ID:$
//

#include <Core.hpp>

/*static */unsigned char Server::GameMessageHandlerManager::GameMessageHandler::m_state;

/*static */Server::GameMessageHandlerManager::ShowMenu_t::StringType_t Server::GameMessageHandlerManager::ShowMenu_t::fullString;

void Server::GameMessageHandlerManager::GameMessageHandler_ForClient_AmmoPickup::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// This message tells a game client that his player picked up some ammo.
	// The only difference with the AmmoX message is that this message enables the client to display a nice little icon on the player's HUD to notify him that he just picked up some ammo.

	// Reliability check.
	InternalAssert (m_client->IsValid ());	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!

	switch (m_state)
	{
		// ammo ID
		case 1u:
			m_ammoID = static_cast <const HalfLifeEngine::SDK::Constants::WeaponID_t *const> (pointer)[0u];

			break;

		// amount
		case 2u:
			// Reliability check.
			InternalAssert (m_ammoID < HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u);

			m_client->AmmoAmountChanged (m_ammoID, static_cast <const unsigned char *const> (pointer)[0u]);	// update amount of this type of ammo
#if 0
			for (i = 1; i < HalfLifeEngine::SDK::Constants::MaximumWeapons; ++i)
				if (HalfLifeEngine::SDK::Constants::weaponProperties[i].primaryAmmoID == m_ammoID)
					m_client->m_ammo[m_ammoID].total = static_cast <const int *const> (pointer)[0u];	// update amount of this type of ammo
#endif	// if 0
			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForClient_AmmoX::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message tells a game client to raise or lower a certain type of ammo by a certain value (for example, a decrease in bullets when the player loads a new clip in his weapon).
	// It is also sent when players spawn, to set all their ammo back to starting values.

	// Reliability check.
	InternalAssert (m_client->IsValid ());	// OCCURS ON CientDisconnect().

	switch (m_state)
	{
		// ammo ID
		case 1u:
			m_ammoID = static_cast <const HalfLifeEngine::SDK::Constants::WeaponID_t *const> (pointer)[0u];

			break;

		// amount
		case 2u:
			// Reliability check.
			InternalAssert (m_ammoID < HalfLifeEngine::SDK::Constants::MaximumWeapons);

			m_client->AmmoAmountChanged (m_ammoID, static_cast <const unsigned char *const> (pointer)[0u]);	// update amount of this type of ammo
#if 0
			for (i = 1; i < HalfLifeEngine::SDK::Constants::MaximumWeapons; ++i)
				if (HalfLifeEngine::SDK::Constants::weaponProperties[i].primaryAmmoID == m_ammoID)
					m_client->m_ammo[m_ammoID].total = static_cast <const int *const> (pointer)[0u];	// update amount of this type of ammo
#endif	// if 0
			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForClient_CurrentWeapon::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message is sent when a game client just selected one weapon.
	// The server updates him with the amount of ammo it knows it has and the state of the weapon, to ensure the client will use the same ammo amount that the server knows he has.
	// It is also sent to clients when the server auto assigns them a weapon.
	// It seems also to be sent when the weapon is fired, causing the amount of ammo currently in clip to decrease.

	// Reliability check.
	InternalAssert (m_client->IsValid ());	// OCCURS ON CientDisconnect().

	switch (m_state)
	{
		// Is this weapon is fixed in hands?
		case 1u:
			m_weaponState = static_cast <const bool *const> (pointer)[0u];	// State.

			break;

		// Weapon ID.
		case 2u:
			m_weaponID = static_cast <const HalfLifeEngine::SDK::Constants::WeaponID_t *const> (pointer)[0u];	// Weapon ID.

			break;

		// Ammo currently in the clip for this weapon.
		case 3u:
			// is that new weapon's ID valid and is the weapon in a usable state?
			InternalAssert (m_weaponID >= HalfLifeEngine::SDK::Constants::WeaponID_None);

			if (m_weaponID < HalfLifeEngine::SDK::Constants::MaximumWeapons - 1u)
			{
				// update the pointers to the new current weapon in the bot weapons array
				m_client->CurrentWeaponChanged (m_weaponState, m_weaponID, static_cast <const char *const> (pointer)[0u]);
			}

			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_Damage::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message tells a game client that his player is taking damage by some other entity (likely, an enemy, but it can also be the world itself).
	// It is used to display all the damage-related signaletics on the player's HUD.

	// Reliability check.
	InternalAssert (m_client->IsValid ());	// OCCURS ON CientDisconnect().
	InternalAssert (m_client->IsYaPB ());

	switch (m_state)
	{
		// damage save
		case 1u:
			m_damageArmor = static_cast <const unsigned char *const> (pointer)[0u];

			// Reliability check.
			InternalAssert (static_cast <const int *const> (pointer)[0u] == m_damageArmor);
			InternalAssert (m_damageArmor == 0u);

			break;

		// damage take
		case 2u:
			m_damageTaken = static_cast <const unsigned short *const> (pointer)[0u];

			// Reliability check.
			InternalAssert (static_cast <const int *const> (pointer)[0u] == m_damageTaken);

			break;

		// damage type
		case 3u:
			if (m_damageArmor > 0u || m_damageTaken > 0u)
				m_client->TakeDamage (m_client->GetEdict ()->variables.damageInflictor, m_damageTaken, m_damageArmor, static_cast <const HalfLifeEngine::SDK::Constants::DamageType_t *const> (pointer)[0u]);

			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ScreenShake::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// Shakes the screen.
	// This message is sent over the net to describe a screen shake event.

	// Reliability checks.
	InternalAssert (m_client->IsValid ());
	InternalAssert (m_client->IsYaPB ());

	switch (m_state)
	{
		// Amplitude (FIXED 4.12 amount of shake)
		case 1u:
			m_amplitude = static_cast <const unsigned short *const> (pointer)[0u];

			break;

		// Duration (FIXED 4.12 seconds duration)
		case 2u:
			m_duration = static_cast <const unsigned short *const> (pointer)[0u];

			break;

		// Frequency (FIXED 8.8 noise frequency (low frequency is a jerk, high frequency is a rumble))
		case 3u:
			m_client->TakeShakes (static_cast <float> (m_amplitude >> 12u), static_cast <float> (m_duration >> 12u), static_cast <float> (static_cast <const unsigned short *const> (pointer)[0u] >> 8u));

			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ScreenFade::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message tells a game client to fade in/out the screen of his player to a certain color, and for a certain duration.
	// It can happen, for example, when players are affected by a flash grenade.

	// Reliability checks.
	InternalAssert (m_client->IsValid ());
	InternalAssert (m_client->IsYaPB ());

	switch (m_state)
	{
/*		// Flags
		case 3u:
			m_isStayOut = (static_cast <const unsigned char *const> (pointer)[0u] & HalfLifeEngine::SDK::Constants::ScreenFadeFlag_StayOut) != 0;

			break;
*/
		// color red
		case 4u:
			m_fadeColor.red = static_cast <const unsigned char *const> (pointer)[0u];

			break;

		// color green
		case 5u:
			m_fadeColor.green = static_cast <const unsigned char *const> (pointer)[0u];

			break;

		// color blue
		case 6u:
			m_fadeColor.blue = static_cast <const unsigned char *const> (pointer)[0u];

			break;

		// alpha
		case 7u:
			m_fadeColor.alpha = static_cast <const unsigned char *const> (pointer)[0u];

			if (m_fadeColor.red == 255u && m_fadeColor.green == 255u && m_fadeColor.blue == 255u && m_fadeColor.alpha > 200u)
			{
				const float duration (/*m_isStayOut ? 3.4e+38f : */static_cast <float> (m_fadeColor.alpha - 200u) / 16.0f);
				const Server::ThrownGrenadesManager::ThrownGrenade_t *const flashBangGrenade (g_server->GetThrownGrenadesManager ().FindLatestThrownFlashBangGrenade ());
				const Client *const flasher (flashBangGrenade == NULL ? NULL : flashBangGrenade->owner);

				m_client->TakeBlinded (duration, flasher);
/*
				#if defined _DEBUG
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Bot \"%s\" take blinded, fadeColor == [red=%u, green=%u, blue=%u, alpha=%u], duration == %.2f.\n, flasher: \"%s\".\n", m_client->GetName ().GetData (), m_fadeColor.red, m_fadeColor.green, m_fadeColor.blue, m_fadeColor.alpha, duration, flasher == NULL ? "NULL" : flasher->GetName ().GetData ());
				#endif	// if defined _DEBUG
*/			}

			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ShowMenu::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// This message tells a game client to display a HUD text menu.

	switch (m_state)
	{
		// Valid slots (unsigned short)
		// a bitfield of keys that are valid input.
		case 1u:
			m_information.validSlots = static_cast <const HalfLifeEngine::SDK::Constants::Menu::KeyBit_t *const> (pointer)[0u];

			break;

		// Duration? (char)
		// the duration, in seconds, the menu should stay up. -1 means is stays until something is chosen.
		case 2u:
			m_information.lifeTime = static_cast <const char *const> (pointer)[0u];

			break;

		// Need more? (boolean)
		// TRUE if there is more string yet to be received before displaying the menu, FALSE if it's the last string.
		case 3u:
			m_information.isNeedMore = static_cast <const bool *const> (pointer)[0u];	// Is full menu string or some part?

			break;

		// only take the third packet (only want menu string)
		// menu string to display.
		case 4u:
		{
			if (m_information.validSlots == HalfLifeEngine::SDK::Constants::Menu::KeyBit_None)
			{
				// no valid slots means that the menu should be turned off

				// Reliability check.
				InternalAssert (m_information.isNeedMore == false);

				for (HumanManager::HumansArray_t::IndexType_t humanIndex (0u); humanIndex < g_server->GetHumanManager ()->GetHumansCount (); ++humanIndex)
					g_server->GetHumanManager ()->GetHumansArray ()[humanIndex]->OnCloseMenu ();

				for (YaPBManager::YaPBsArray_t::IndexType_t botIndex (0u); botIndex < g_server->GetYaPBManager ()->GetBotsCount (); ++botIndex)
					g_server->GetYaPBManager ()->GetYaPBsArray ()[botIndex]->OnCloseMenu ();

				break;
			}

			const ShowMenu_t::StringType_t::IndexType_t stringLength (GetStringLength <ShowMenu_t::StringType_t::IndexType_t> (static_cast <const char *const> (pointer)));

			// Reliability check.
			InternalAssert (HalfLifeEngine::SDK::Constants::MaximumMenuFullStringLength >= stringLength);

			const ShowMenu_t::StringType_t::IndexType_t length (Math::GetMinimumValueBetween <ShowMenu_t::StringType_t::IndexType_t> (stringLength, HalfLifeEngine::SDK::Constants::MaximumMenuFullStringLength - m_information.fullString.GetElementNumber ()));

			if (length > 0u)
			{
				// append to the current menu string....
				m_information.fullString.Append (static_cast <const char *const> (pointer), length);
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("All clients take menu, m_information: [validSlots=%u, lifeTime=%d, isNeedMore=%u, string=\"%s\"].\n", m_information.validSlots, m_information.lifeTime, m_information.isNeedMore, static_cast <const char *const> (pointer));

			// check, if we have the whole string
			if (m_information.isNeedMore == false)
			{
				for (HumanManager::HumansArray_t::IndexType_t humanIndex (0u); humanIndex < g_server->GetHumanManager ()->GetHumansCount (); ++humanIndex)
					g_server->GetHumanManager ()->GetHumansArray ()[humanIndex]->OnDisplayMenu (m_information.validSlots, m_information.lifeTime, m_information.fullString);

				for (YaPBManager::YaPBsArray_t::IndexType_t botIndex (0u); botIndex < g_server->GetYaPBManager ()->GetBotsCount (); ++botIndex)
					g_server->GetYaPBManager ()->GetYaPBsArray ()[botIndex]->ShowMenu (m_information.fullString);

				m_information.fullString.MakeEmpty ();	// Reset the string....
			}

			break;
		}
	}

	// And finally update network message state.
	++m_state;
}
void Server::GameMessageHandlerManager::GameMessageHandler_ForHuman_ShowMenu::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// This message tells a game client to display a HUD text menu.

	// Reliability checks.
	InternalAssert (m_client->IsValid ());
	InternalAssert (m_client->IsHuman ());

	switch (m_state)
	{
		// Valid slots (unsigned short)
		// a bitfield of keys that are valid input.
		case 1u:
			m_information.validSlots = static_cast <const HalfLifeEngine::SDK::Constants::Menu::KeyBit_t *const> (pointer)[0u];

			break;

		// Duration? (char)
		// the duration, in seconds, the menu should stay up. -1 means is stays until something is chosen.
		case 2u:
			m_information.lifeTime = static_cast <const char *const> (pointer)[0u];

			break;

		// Need more? (boolean)
		// TRUE if there is more string yet to be received before displaying the menu, FALSE if it's the last string.
		case 3u:
			m_information.isNeedMore = static_cast <const bool *const> (pointer)[0u];	// Is full menu string or some part?

			break;

		// only take the third packet (only want menu string)
		// menu string to display.
		case 4u:
		{
			if (m_information.validSlots == HalfLifeEngine::SDK::Constants::Menu::KeyBit_None)
			{
				// Reliability check.
				InternalAssert (m_information.isNeedMore == false);

				m_client->OnCloseMenu ();	// no valid slots means that the menu should be turned off

				break;
			}

			const ShowMenu_t::StringType_t::IndexType_t stringLength (GetStringLength <ShowMenu_t::StringType_t::IndexType_t> (static_cast <const char *const> (pointer)));

			// Reliability check.
			InternalAssert (HalfLifeEngine::SDK::Constants::MaximumMenuFullStringLength >= stringLength);

			const ShowMenu_t::StringType_t::IndexType_t length (Math::GetMinimumValueBetween <ShowMenu_t::StringType_t::IndexType_t> (stringLength, HalfLifeEngine::SDK::Constants::MaximumMenuFullStringLength - m_information.fullString.GetElementNumber ()));

			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\"%s\"::OnDisplayMenu(): take menu, m_information: [validSlots=%u, lifeTime=%d, string=\"%s\", fullString=\"%s\", stringLength=%u, length=%u].\n", m_client->GetName ().GetData (), m_information.validSlots, m_information.lifeTime, static_cast <const char *const> (pointer), m_information.fullString.GetData (), stringLength, length);

			if (length > 0u)
			{
				// append to the current menu string....
				m_information.fullString.Append (static_cast <const char *const> (pointer), length);
			}

//			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Human \"%s\" take menu, m_information: [validSlots=%u, lifeTime=%d, isNeedMore=%u, string=\"%s\"].\n", m_client->GetName ().GetData (), m_information.validSlots, m_information.lifeTime, m_information.isNeedMore, static_cast <const char *const> (pointer));

			// check, if we have the whole string
			if (m_information.isNeedMore == false)
			{
				m_client->OnDisplayMenu (m_information.validSlots, m_information.lifeTime, m_information.fullString);

				m_information.fullString.MakeEmpty ();	// Reset the string....
			}

			break;
		}
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ShowMenu::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// This message tells a game client to display a HUD text menu.

	// Reliability checks.
	InternalAssert (m_client->IsValid ());
	InternalAssert (m_client->IsYaPB ());

	switch (m_state)
	{
		// Need more? (boolean)
		// TRUE if there is more string yet to be received before displaying the menu, FALSE if it's the last string.
		case 3u:
			m_information.isNeedMore = static_cast <const bool *const> (pointer)[0u];	// Is full menu string or some part?

			break;

		// only take the third packet (only want menu string)
		// menu string to display.
		case 4u:
		{
			if (m_information.validSlots == HalfLifeEngine::SDK::Constants::Menu::KeyBit_None)
			{
				// Reliability check.
				InternalAssert (m_information.isNeedMore == false);

				m_client->OnCloseMenu ();	// no valid slots means that the menu should be turned off

				break;
			}

			const ShowMenu_t::StringType_t::IndexType_t stringLength (GetStringLength <ShowMenu_t::StringType_t::IndexType_t> (static_cast <const char *const> (pointer)));

			// Reliability check.
			InternalAssert (HalfLifeEngine::SDK::Constants::MaximumMenuFullStringLength >= stringLength);

			const ShowMenu_t::StringType_t::IndexType_t length (Math::GetMinimumValueBetween <ShowMenu_t::StringType_t::IndexType_t> (stringLength, HalfLifeEngine::SDK::Constants::MaximumMenuFullStringLength - m_information.fullString.GetElementNumber ()));

			if (length > 0u)
			{
				// append to the current menu string....
				m_information.fullString.Append (static_cast <const char *const> (pointer), length);
			}

			// check, if we have the whole string
			if (m_information.isNeedMore == false)
			{
//				m_client->ShowMenu (DynamicString (static_cast <const char *const> (pointer), Math::GetMinimumValueBetween <unsigned short> (GetStringLength <unsigned short> (static_cast <const char *const> (pointer)), 25u)));	// copy menu title name (only 25 characters - maximum menu title name (which we use) length).
				m_client->ShowMenu (m_information.fullString);

				m_information.fullString.MakeEmpty ();	// Reset the string....
			}

			break;
		}
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_StatusIcon::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message tells a game client to display (or stop displaying) a certain status icon on his player's HUD.

	// Reliability checks.
	InternalAssert (m_client->IsValid ());
	InternalAssert (m_client->IsYaPB ());

	switch (m_state)
	{
		// status
		case 1u:
			m_status = static_cast <const HalfLifeEngine::SDK::Constants::StatusIcon_StatusType_t *const> (pointer)[0u];

			break;

		// sprite name
		case 2u:
			// copy sprite name (only 9 characters - maximum sprite name (which we use) length).
			m_client->StatusIconChanged (m_status, DynamicString (static_cast <const char *const> (pointer), Math::GetMinimumValueBetween <unsigned short> (GetStringLength <unsigned short> (static_cast <const char *const> (pointer)), 9u)));

			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_ReloadSound::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message tells a game client that a reload sound is being played in the neighbourhood of its player character.

	// Reliability checks.
	InternalAssert (m_client->IsValid ());
	InternalAssert (m_client->IsYaPB ());
	InternalAssert (g_currentReloader->IsValid ());
	InternalAssert (g_currentReloader != m_client);

	switch (m_state)
	{
		// volume
		case 1u:
			m_volume = static_cast <const unsigned char *const> (pointer)[0u];

			break;

		// is not shotgun
		case 2u:
			m_isNotShotgun = static_cast <const bool *const> (pointer)[0u];

			// Ugly method to prevent calling of 'Client::NoisedSound_t::Initialize' function several times....
			if (g_isFirstReloadSoundMessageCall)
			{
				g_isFirstReloadSoundMessageCall = false;

				if (m_isNotShotgun)
					g_currentReloader->SetCurrentNoisedSound (/*HalfLifeEngine::SDK::Constants::SoundChannel_Item, */"weapons/generic_reload.wav", m_volume * (1.0f / 255.0f), 1.95312f/*1.0f*/, HalfLifeEngine::SDK::Constants::SoundFlag_None, HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
				else
					g_currentReloader->SetCurrentNoisedSound (/*HalfLifeEngine::SDK::Constants::SoundChannel_Item, */"weapons/generic_shot_reload.wav", m_volume * (1.0f / 255.0f), 1.95312f/*1.0f*/, HalfLifeEngine::SDK::Constants::SoundFlag_None, HalfLifeEngine::SDK::Constants::SoundPitch_Normal);

				g_currentReloader->UpdateLastNoisedSound ();
			}

			m_client->HearNoisedSound (g_currentReloader, g_currentReloader->GetLastNoisedSound ());

			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_TeamInfo::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// This message sets the team information for the given player.

	switch (m_state)
	{
		// Player ID
		case 1u:
			m_playerEdictIndex = static_cast <const unsigned char *const> (pointer)[0u];

			break;

		// Team name. (either "UNASSIGNED", "TERRORIST", "CT" or "SPECTATOR")
		case 2u:
			switch (static_cast <const char *const> (pointer)[0u])
			{
				// "TERRORIST"
				case 'T':
					g_server->GetClientManager ()->ClientTeamSwitchedToTerrorists (m_playerEdictIndex);

					break;

				// "CT"
				case 'C':
					g_server->GetClientManager ()->ClientTeamSwitchedToCounterTerrorists (m_playerEdictIndex);

					break;

				// "UNASSIGNED" or "SPECTATOR"
				default:
					g_server->GetClientManager ()->ClientTeamSwitchedToSpectators (m_playerEdictIndex, static_cast <const char *const> (pointer)[0u] == 'U');

					break;
			}

			break;
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_Text::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message gets sent when a text message is displayed on the HUD of all players

	if (m_state == 2u)
	{
		// copy game message (only 32 characters - maximum message (which we use) length)
		m_message.Assign (static_cast <const char *const> (pointer), Math::GetMinimumValueBetween <unsigned short> (GetStringLength <unsigned short> (static_cast <const char *const> (pointer)), 32u));

		// message
		if
		(
			m_message == "#Game_Commencing" ||
			m_message == "#Game_will_restart_in" ||
			m_message == "#Round_Draw" ||
			m_message == "#Terrorists_Win" ||
			m_message == "#CTs_Win" ||
			m_message == "#Bomb_Defused" ||
			m_message == "#Target_Saved" ||
			m_message == "#Target_Bombed" ||
			m_message == "#All_Hostages_Rescued" ||
			m_message == "#Hostages_Not_Rescued" ||
			m_message == "#VIP_Escaped" ||
			m_message == "#VIP_Not_Escaped" ||
			m_message == "#Terrorists_Escaped" ||
			m_message == "#Terrorists_Not_Escaped" ||
			m_message == "#Escaping_Terrorists_Neutralized" ||
			m_message == "#VIP_Assassinated" ||
			m_message == "#CTs_PreventEscape"
		)
		{
			g_server->RoundEnded ();

			if (m_message == "#Bomb_Defused")
				g_server->BombDefused ();
			else if (m_message == "#Target_Bombed")
				g_server->BombDetonated ();
			else if (m_message == "#Terrorists_Win")
				g_server->GetYaPBManager ()->TerroristsWin ();
			else if (m_message == "#CTs_Win")
				g_server->GetYaPBManager ()->CounterTerroristsWin ();
		}
		else if (m_message == "#Bomb_Planted")	// Catch the "bomb planted" message.
			g_server->BombPlanted ();
	}

	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_Text::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message gets sent when a text message is displayed on the HUD of all players

	// Reliability checks.
	InternalAssert (m_client->IsValid ());	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	InternalAssert (m_client->IsYaPB ());
/*
	static DynamicString stringBuffer[3u];
	static Client *radioCommandCaller (NULL);

	switch (m_state)
	{
		// Substring.
		case 3u:
			// copy substring message (only 11 characters - maximum message (which we use) length)
			stringBuffer[0u].Assign (static_cast <const char *const> (pointer), Math::GetMinimumValueBetween <unsigned short> (GetStringLength <unsigned short> (static_cast <const char *const> (pointer)), 11u));

			break;

		// Substring.
		case 4u:
			// copy substring message (only HalfLifeEngine::SDK::Constants::MaximumClientNameLength characters - maximum message (which we use) length)
			stringBuffer[1u].Assign (static_cast <const char *const> (pointer), Math::GetMinimumValueBetween <unsigned short> (GetStringLength <unsigned short> (static_cast <const char *const> (pointer)), HalfLifeEngine::SDK::Constants::MaximumClientNameLength));

			break;

		// Substring.
		case 5u:
			// Only check the rest of the message if we are certain it's a radio message.
			if (stringBuffer[0u] != "#Game_radio")
				break;

			if (radioCommandCaller == NULL || radioCommandCaller->GetName () != stringBuffer[1u])
				for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
					if ((radioCommandCaller = g_server->GetClientManager ()->GetClientsArray ()[index])->GetName () == stringBuffer[1u])
						break;

			InternalAssert (radioCommandCaller->IsValid () && radioCommandCaller->GetName () == stringBuffer[1u]);
/*			if (!radioCommandCaller->IsValid ())
				AddLogEntry (false, LogLevel_Critical, false, "radioCommandCaller INVALID!!!!!!!");
			else if (radioCommandCaller->GetName () != stringBuffer[1u])
				AddLogEntry (false, LogLevel_Critical, false, "radioCommandCaller->GetEdict()->GetNetName()(%s) != stringBuffer[1](%s)", radioCommandCaller->GetName ().GetData (), stringBuffer[1u].GetData ());
*//*
			// copy substring message (only 25 characters - maximum message (which we use) length)
			stringBuffer[2u].Assign (static_cast <const char *const> (pointer), Math::GetMinimumValueBetween <unsigned short> (GetStringLength <unsigned short> (static_cast <const char *const> (pointer)), 25u));

			// Radio menu 1.
			if (stringBuffer[2u] == "#Cover_me")	// 'Cover me' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_CoverMe);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#You_take_the_point")	// 'You take the point' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_YouTakePoint);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Hold_this_position")	// 'Hold this position' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_HoldPosition);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Regroup_team")	// 'Regroup team' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_RegroupTeam);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Follow_me")	// 'Follow me' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_FollowMe);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Taking_fire")	// 'Taking fire, need backup' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_TakingFire);	// Handle this message actually....
			}
			// Radio menu 2.
			else if (stringBuffer[2u] == "#Go_go_go")	// 'Go Go Go' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_GoGoGo);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Team_fall_back")	// 'Team fall back' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_FallBack);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Stick_together_team")	// 'Stick together team' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_StickTogether);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Get_in_position_and_wait")	// 'Stay in position and wait for my go' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_GetInPosition);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Storm_the_front")	// 'Storm The Front' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_StormTheFront);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Report_in_team")	// 'Report In' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_ReportTeam);	// Handle this message actually....
			}
			// Radio menu 3.
			else if (stringBuffer[2u] == "#Affirmative" || stringBuffer[2u] == "#Roger_that")	// 'Affirmative' or 'Roger that' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_Affirmative);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Enemy_spotted")	// 'Enemy spotted' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_EnemySpotted);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Need_backup")	// 'Need backup' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_NeedBackup);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Sector_clear")	// 'Sector clear' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_SectorClear);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#In_position")	// 'I'm in position' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_InPosition);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Reporting_in")	// 'Reporting in' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_ReportingIn);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Get_out_of_there")	// 'Get outta here' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_ShesGonnaBlow);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Negative")	// 'Negative' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_Negative);	// Handle this message actually....
			}
			else if (stringBuffer[2u] == "#Enemy_down")	// 'Enemy down' radio message.
			{
				radioCommandCaller->UpdateRadioDelayTime ();

				m_client->HearRadio (radioCommandCaller, RadioCommandID_EnemyDown);	// Handle this message actually....
			}

			break;
	}
*/
	// And finally update network message state.
	++m_state;
}

void Server::GameMessageHandlerManager::GameMessageHandler_ForYaPB_VGUIMenu::Execute (const void *const pointer)
{
	// Reliability check.
	InternalAssert (pointer != NULL);

	// Now starts of network message execution....

	// this message tells a game client to display a VGUI menu

	// Reliability checks.
	InternalAssert (m_client->IsValid ());
	InternalAssert (m_client->IsYaPB ());

	// only take the first packet (only want the menu code)
	if (m_state == 1u)
		m_client->VGUIMenuShow (static_cast <const HalfLifeEngine::SDK::Constants::VGUIMenuID_t *const> (pointer)[0u]);	// menu ID (since latest steam updates is obsolete, but left for old CS)

	// And finally update network message state.
	++m_state;
}