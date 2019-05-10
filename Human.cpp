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
// Human.cpp
//
// Class: Human
//
// Version: $ID:$
//

#include <Core.hpp>

void Human::DisplayMenu (const MenuID_t menuID)
{
	{
		// Just a update?
		if (GetOpenedYaPBMenu ().id == menuID)
			return;

		m_openedYaPBMenu.SetNewID (menuID);

		g_menusManager.Draw (this);

		return;
	}
#if 0
	// Reliability check.
//	InternalAssert (GetOpenedYaPBMenu ().id != menuID || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹1 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹2 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹3 || GetOpenedYaPBMenu ().id == MenuID_BotKickMenu¹4);
//	if (GetOpenedYaPBMenu ().id == menuID)
//	AddLogEntry (true, LogLevel_Default, false, "Human::DisplayMenu(): GetOpenedYaPBMenu().id(%s) == menuID(%s).", GetOpenedYaPBMenu ().id == MenuID_None ? "none" : g_menus[GetOpenedYaPBMenu ().id].menuText.GetData (), menuID == MenuID_None ? "none" : g_menus[menuID].menuText.GetData ());

	switch (menuID)
	{
#if 0
/// @warning UNDONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case MenuID_BotKickMenu¹1:
		{
			const YaPBManager::YaPBsArray_t::IndexType_t botsCount (g_server->GetYaPBManager ()->GetBotsCount ());

			if (botsCount == 0u)
			{
				Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer));

				// Stops others from hearing menu sounds....
				ExecuteCommand ("speak player/geiger1\n");

				return;
			}

			if (botsCount <= 9u)
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText = "\\yYaPB Kick Menu:\n\n";

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == botsCount)
						break;
				}

				g_menus[menuID].menuText += "\n\\r0\\w. Previous...";
			}
			else if (botsCount == 17u)
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 1/2):\n\n";

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == 9u)
						break;
				}

				g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";
			}
			else if (botsCount == 25u)
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 1/3):\n\n";

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == 9u)
						break;
				}

				g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";
			}
			else
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText.AssignFormat ("\\yYaPB Kick Menu (Page 1/%u):\n\n", botsCount / 8u);	// 0.125 == one bot slot

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == 8u)
						break;
				}

				g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";
			}

			break;
		}

		case MenuID_BotKickMenu¹2:
		{
			const YaPBManager::YaPBsArray_t::IndexType_t botsCount (g_server->GetYaPBManager ()->GetBotsCount ());

			if (botsCount == 0u)
			{
				Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer));

				// Stops others from hearing menu sounds....
				ExecuteCommand ("speak player/geiger1\n");

				return;
			}

			if (botsCount <= 9u)
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText = "\\yYaPB Kick Menu:\n\n";

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == botsCount)
						break;
				}

				g_menus[menuID].menuText += "\n\\r0\\w. Previous...";
			}
			else if (botsCount == 17u)
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 1/2):\n\n";

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == 9u)
						break;
				}

				g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";
			}
			else if (botsCount == 25u)
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 1/3):\n\n";

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == 9u)
						break;
				}

				g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";
			}
			else
			{
				unsigned char menuIndex (0u);

				g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
				g_menus[menuID].menuText.AssignFormat ("\\yYaPB Kick Menu (Page 1/%u):\n\n", botsCount / 8u);	// 0.125 == one bot slot

				// loop through all bot slots
				for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < botsCount; ++index)
				{
					const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);
					const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

					g_menus[menuID].validSlots |= BIT (menuIndex);
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", ++menuIndex, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

					if (menuIndex == 8u)
						break;
				}

				g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";
			}

			break;
		}
#endif	// if 0
		case MenuID_BotKickMenu¹1:
			g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
			g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 1/4):\n\n";

			// loop through all menu slots
			for (unsigned char index (0u); index < 8u; ++index)
			{
				const Client *const bot (g_server->GetClientManager ()->GetClient (index));

				// Is this slot used?
				if (!bot->IsValid ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient slot is empty\n", index + 1u);

					continue;
				}

				if (!bot->IsYaPB ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient not a YaPB\n", index + 1u);

					continue;
				}

				// Else....

				const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

				g_menus[menuID].validSlots |= BIT (index);
				g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", index + 1u, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");
			}

			g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";

			break;

		case MenuID_BotKickMenu¹2:
			g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
			g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 2/4):\n\n";

			// loop through all menu slots
			for (unsigned char index (0u); index < 8u; ++index)
			{
				const Client *const bot (g_server->GetClientManager ()->GetClient (index + 8u));

				// Is this slot used?
				if (!bot->IsValid ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient slot is empty\n", index + 1u);

					continue;
				}

				if (!bot->IsYaPB ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient not a YaPB\n", index + 1u);

					continue;
				}

				// Else....

				const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

				g_menus[menuID].validSlots |= BIT (index);
				g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", index + 1u, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");
			}

			g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";

			break;

		case MenuID_BotKickMenu¹3:
			g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
			g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 3/4):\n\n";

			// loop through all menu slots
			for (unsigned char index (0u); index < 8u; ++index)
			{
				const Client *const bot (g_server->GetClientManager ()->GetClient (index + 16u));

				// Is this slot used?
				if (!bot->IsValid ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient slot is empty\n", index + 1u);

					continue;
				}

				if (!bot->IsYaPB ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient not a YaPB\n", index + 1u);

					continue;
				}

				// Else....

				const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

				g_menus[menuID].validSlots |= BIT (index);
				g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", index + 1u, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");
			}

			g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\\r0\\w. Next...";

			break;

		case MenuID_BotKickMenu¹4:
			g_menus[menuID].validSlots = HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0;
			g_menus[menuID].menuText = "\\yYaPB Kick Menu (Page 4/4):\n\n";

			// loop through all menu slots
			for (unsigned char index (0u); index < 8u; ++index)
			{
				const Client *const bot (g_server->GetClientManager ()->GetClient (index + 24u));

				// Is this slot used?
				if (!bot->IsValid ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient slot is empty\n", index + 1u);

					continue;
				}

				if (!bot->IsYaPB ())
				{
					g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. \\dClient not a YaPB\n", index + 1u);

					continue;
				}

				// Else....

				const HalfLifeEngine::SDK::Constants::TeamID_t botTeam (bot->GetRealTeam ());

				g_menus[menuID].validSlots |= BIT (index);
				g_menus[menuID].menuText.AppendFormat ("\\r%u\\w. %s (%s)\n", index + 1u, bot->GetName ().GetData (), botTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : botTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");
			}

			g_menus[menuID].menuText += "\n\\r9\\w. Previous...\n\n\\r0\\w. Exit";

			break;
	}

	DisplayMenu (g_menus[menuID].menuText, g_menus[menuID].validSlots);

	// Just a update?
	if (GetOpenedYaPBMenu ().id == menuID)
		return;

	m_openedYaPBMenu.id = menuID;

	// Stops others from hearing menu sounds....
	ExecuteCommand ("speak player/geiger1\n");
#endif	// if 0
}
#if 0
const BotConsoleCommandReturnType_t Human::BotCommandHandler (const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5, const DynamicString &argument6)
{
	if (argument1.CompareWithoutCaseCheck ("localizer") == 0)
	{
		if (argument2.CompareWithoutCaseCheck ("reload") == 0)
			g_localizer.Reload ();
	}
	else if (argument1.CompareWithoutCaseCheck ("add") == 0 || argument1.CompareWithoutCaseCheck ("addbot") == 0)	// adding one bot with random parameters to random team
		g_server->GetYaPBManager ()->AddBot (argument5, argument2, argument3, argument4, argument6);
	else if (argument1.CompareWithoutCaseCheck ("addbot_hs") == 0 || argument1.CompareWithoutCaseCheck ("addhs") == 0)		// adding one bot with high skill parameters to random team
		g_server->GetYaPBManager ()->AddBot (argument5, "100", "1", argument4, argument6);
	else if (argument1.CompareWithoutCaseCheck ("addbot_t") == 0 || argument1.CompareWithoutCaseCheck ("add_t") == 0)		// adding one bot with random parameters to terrorist team
		g_server->GetYaPBManager ()->AddBot (argument5, argument2, argument3, "1", argument6);
	else if (argument1.CompareWithoutCaseCheck ("addbot_ct") == 0 || argument1.CompareWithoutCaseCheck ("add_ct") == 0)		// adding one bot with random parameters to counter-terrorist team
		g_server->GetYaPBManager ()->AddBot (argument5, argument2, argument3, "2", argument6);
	else if (argument1.CompareWithoutCaseCheck ("kickbot_t") == 0 || argument1.CompareWithoutCaseCheck ("kick_t") == 0)		// kicking off one bot from the terrorist team
		g_server->GetYaPBManager ()->KickFromTeam (HalfLifeEngine::SDK::Constants::TeamID_Terrorist);
	else if (argument1.CompareWithoutCaseCheck ("kickbot_ct") == 0 || argument1.CompareWithoutCaseCheck ("kick_ct") == 0)	// kicking off one bot from the counter-terrorist team
		g_server->GetYaPBManager ()->KickFromTeam (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);
	else if (argument1.CompareWithoutCaseCheck ("killbots_t") == 0 || argument1.CompareWithoutCaseCheck ("kill_t") == 0)	// kills all bots on the terrorist team
		g_server->GetYaPBManager ()->KillAll (HalfLifeEngine::SDK::Constants::TeamID_Terrorist);
	else if (argument1.CompareWithoutCaseCheck ("killbots_ct") == 0 || argument1.CompareWithoutCaseCheck ("kill_ct") == 0)	// kills all bots on the counter-terrorist team
		g_server->GetYaPBManager ()->KillAll (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);
	else if (argument1.CompareWithoutCaseCheck ("listbots") == 0 || argument1.CompareWithoutCaseCheck ("list") == 0)		// list all bots playeing on the server
		g_server->GetYaPBManager ()->ListBots (this);
	else if (argument1.CompareWithoutCaseCheck ("kickbots") == 0 || argument1.CompareWithoutCaseCheck ("kickall") == 0)		// kick off all bots from the played server
		g_server->GetYaPBManager ()->KickAll ();
	else if (argument1.CompareWithoutCaseCheck ("killbots") == 0 || argument1.CompareWithoutCaseCheck ("killall") == 0)		// kill all bots on the played server
		g_server->GetYaPBManager ()->KillAll ();
	else if (argument1.CompareWithoutCaseCheck ("kickone") == 0 || argument1.CompareWithoutCaseCheck ("kickbot") == 0 || argument1.CompareWithoutCaseCheck ("kick") == 0)			// kick off one random bot from the played server
		g_server->GetYaPBManager ()->KickRandom ();
	else if (argument1.CompareWithoutCaseCheck ("fillserver") == 0 || argument1.CompareWithoutCaseCheck ("fill") == 0)		// fill played server with bots
		g_server->GetYaPBManager ()->FillServer (argument2.IsEmpty () ? HalfLifeEngine::SDK::Constants::TeamID_AutoSelect : argument2.GetValue <HalfLifeEngine::SDK::Constants::TeamID_t> (), argument3.IsEmpty () ? Personality_None : argument3.GetValue <Personality_t> (), argument4.IsEmpty () ? -1 : argument4.GetValue <unsigned char> (), argument5.IsEmpty () ? -1 : argument5.GetValue <unsigned char> ());
	else if (argument1.CompareWithoutCaseCheck ("swapteams") == 0 || argument1.CompareWithoutCaseCheck ("swap") == 0)		// swap counter-terrorist and terrorist teams
		g_server->GetClientManager ()->SwapClientsTeams ();
	else if (argument1.CompareWithoutCaseCheck ("swapbotteams") == 0 || argument1.CompareWithoutCaseCheck ("swapbots") == 0)		// swap counter-terrorist and terrorist bot teams
		g_server->GetYaPBManager ()->SwapBotsTeams ();
/*	else if (argument1.CompareWithoutCaseCheck ("weaponmode") == 0 || argument1.CompareWithoutCaseCheck ("wmode") == 0)	// select the weapon mode for bots
	{
		if (argument2.IsEmpty ())
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Please specify weapon mode.\n");

			return BotConsoleCommandReturnType_Accessed;
		}

		const HalfLifeEngine::SDK::Constants::Menu::Key_t selection (argument2.GetValue <HalfLifeEngine::SDK::Constants::Menu::Key_t> ());

		// check is selected range valid
		if (selection < HalfLifeEngine::SDK::Constants::Menu::Key_1 || selection > HalfLifeEngine::SDK::Constants::Menu::Key_7)
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Choose weapon from 1 to 7 range.\n");

			return BotConsoleCommandReturnType_Accessed;
		}

		g_server->GetYaPBManager ()->SetWeaponMode (selection);
	}*/
	else if (argument1.CompareWithoutCaseCheck ("viewprofiles") == 0 || argument1.CompareWithoutCaseCheck ("botprofiles") == 0)	// else do we want to display the profiles database?
		g_server->GetYaPBManager ()->GetProfileManager ().PrintBotProfiles ();
/*	else if (argument1.CompareWithoutCaseCheck ("votemap") == 0)	// force all bots to vote to specified map
	{
		if (argument2.IsEmpty ())
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Please specify nominated map.\n");

			return BotConsoleCommandReturnType_Accessed;
		}

		const int nominatedMap (argument2.GetValue <int> ());

		// loop through all YaPB's
		for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
			g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->m_voteMap = nominatedMap;

		PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "All dead bots will vote for map #%i.\n", nominatedMap);
	}*/
	else if (argument1.CompareWithoutCaseCheck ("sendcmd") == 0 || argument1.CompareWithoutCaseCheck ("order") == 0)	// force bots to execute client command
	{
		if (argument2.IsEmpty ())
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_PleaseSpecifyOrderCommand) + '\n');

			return BotConsoleCommandReturnType_Accessed;
		}

		if (g_server->GetYaPBManager ()->GetBotsCount () == 0u)
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');

			return BotConsoleCommandReturnType_Accessed;
		}

		// Without name?
		if (argument3.IsEmpty ())
		{
			g_server->GetYaPBManager ()->ExecuteCommand (argument2);

			return BotConsoleCommandReturnType_Accessed;
		}

		// Else, search client with given user ID or name

		if (argument3[0u] == '#')
		{
			const unsigned char botUserID (argument3.GetValue <unsigned char> (1u));

			// loop through all bot slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				// remember slot of this bot entity
				const Client *const bot (g_server->GetClientManager ()->GetClientsArray ()[index]);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*bot) != botUserID)
					continue;

				if (!bot->IsYaPB ())
				{
					PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_PlayerIsNotYaPB) + '\n', bot->GetName ().GetData ());

					return BotConsoleCommandReturnType_Accessed;
				}

				bot->ExecuteCommand (argument2);

				PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotHasExecutedCommand) + '\n', bot->GetName ().GetData (), argument2.GetData ());

				return BotConsoleCommandReturnType_Accessed;
			}

			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotWithThatUserIDNotFound) + '\n');

			return BotConsoleCommandReturnType_Accessed;
		}

		// Else, search by bot name....

		const Client *bestNominee (NULL);

		// loop through all client slots
		for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
		{
			// remember slot of this client entity
			const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

			if (client->GetName ().Contains (argument3, false) && (bestNominee == NULL || bestNominee->GetName ().GetElementNumber () > client->GetName ().GetElementNumber ()))
				bestNominee = client;
		}

		if (bestNominee != NULL)
		{
			if (!bestNominee->IsYaPB ())
			{
				PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_PlayerIsNotYaPB) + '\n', bestNominee->GetName ().GetData ());

				return BotConsoleCommandReturnType_Accessed;
			}

			bestNominee->ExecuteCommand (argument2);

			PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotHasExecutedCommand) + '\n', bestNominee->GetName ().GetData (), argument2.GetData ());
		}
		else
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');

		return BotConsoleCommandReturnType_Accessed;
	}
	else if (argument1.CompareWithoutCaseCheck ("ctime") == 0 || argument1.CompareWithoutCaseCheck ("time") == 0)	// display current time on the server
	{
		const tm *const localTime (GetLocalTime ());

		PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtCenter, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_CurrentTimeIs), localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
	}
	else if (argument1.CompareWithoutCaseCheck ("about_bot") == 0 || argument1.CompareWithoutCaseCheck ("about") == 0)	// displays bot about information
	{
		HudMessage
		(
			Math::Vector2D (-1.0f, -1.0f),
			ColorWithAlpha <>
			(
				g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u),
				g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u),
				g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u)
			),
			ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
			HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 5.0f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,
			"+----------------------------------------------------------------------------------------+\n"
			" The YaPB for Counter-Strike Version " PRODUCT_SUPPORT_VERSION "\n"
			" Created by " PRODUCT_AUTHOR ", Using " PRODUCT_INTERNAL_NAME " - Source Code\n"
			" WebSite: " PRODUCT_URL "\n"
			"+----------------------------------------------------------------------------------------+"
		);
	}
	else if (argument1.CompareWithoutCaseCheck ("version") == 0 || argument1.CompareWithoutCaseCheck ("ver") == 0)	// displays version information
	{
		PrintFormat
		(
			HalfLifeEngine::SDK::Constants::PrintType_AtConsole,
			"\n-----------------------------------------------------------------\n"
			"\tName: " PRODUCT_NAME "\n"
			"\tVersion: " PRODUCT_VERSION " (Build: " PRODUCT_BUILD ")\n"
			"\tCreated by: " PRODUCT_AUTHOR "\n"
			"\tCompiled: " PRODUCT_DATE ", " PRODUCT_TIME " GMT+9\n"
			"\tOptimization: " PRODUCT_OPTIMIZATION_TYPE "\n"
			"\tHalf-Life engine interface version: %u\n"
			"\tMeta mod interface version: %s\n"
			"-----------------------------------------------------------------\n", HalfLifeEngine::SDK::Constants::InterfaceVersion, MetaModEngine::SDK::Constants::InterfaceVersion.GetData ()
		);
	}
	else if (argument1 == '\?' || argument1.CompareWithoutCaseCheck ("help") == 0)	// display some sort of help information
	{
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Commands:\n\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb version		- Display version information.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb about			- Show bot about information.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb add			- Create a bot in current game.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb fill			- Fill the server with random bots.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb kickall		- Disconnects all bots from current game.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb killbots		- Kills all bots in current game.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb kick			- Disconnect one random bot from game.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb weaponmode	- Select bot weapon mode.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb viewprofiles	- Display the bot profiles database.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb votemap		- Allows dead bots to vote for specific map.\n");
		Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb cmenu			- Displaying bots command menu.\n");

		if (argument2.CompareWithoutCaseCheck ("full") == 0 || argument2 == '\?')
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb add_t		- Creates one random bot to terrorist team.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb add_ct	- Creates one random bot to ct team.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb kick_t	- Disconnect one random bot from terrorist team.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb kick_ct	- Disconnect one random bot from ct team.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb kill_t	- Kills all bots on terrorist team.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb kill_ct	- Kills all bots on ct team.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb list		- Display list of bots currently playing.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb order		- Execute specific command on specified bot.\n");
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb time		- Displays current time on server.\n");

			if (!HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
			{
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb autowp			- Toggle autowppointing.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp				- Toggle waypoint showing.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp erase			- Remove all waypoint files for current map from hard disk.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp on noclip		- Enable noclip cheat.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp save nocheck	- Save waypoints without checking.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp add			- Open menu for waypoint creation.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp menu			- Open main waypoint menu.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp addbasic		- Creates basic waypoints on map.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp find			- Show direction to specified waypoint.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp load			- Load the waypoint file from hard disk.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp check			- Checks if all waypoints connections are valid.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp cache			- Cache nearest waypoint.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp teleport		- Teleport hostile to specified waypoint.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb wp setradius		- Manually sets the wayzone radius for this waypoint.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb path autodistance	- Opens menu for setting autopath maximum distance.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb path cache		- Remember the nearest to player waypoint.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb path create		- Opens menu for path creation.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb path delete		- Delete path from cached to nearest waypoint.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb path create_in	- Creating incoming path connection.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb path create_out	- Creating outgoing path connection.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb path create_both	- Creating both-ways path connection.\n");
				Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "yapb exp save			- Save the experience data.\n");
			}
		}
	}
	else if (argument1.CompareWithoutCaseCheck ("sethealth") == 0 || argument1.CompareWithoutCaseCheck ("sethp") == 0 || argument1.CompareWithoutCaseCheck ("health") == 0)	// sets health for all available bots
	{
		if (argument2.IsEmpty ())
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_PleaseSpecifyHealth) + '\n');

			return BotConsoleCommandReturnType_Accessed;
		}

		if (g_server->GetYaPBManager ()->GetBotsCount () == 0u)
		{
			Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');

			return BotConsoleCommandReturnType_Accessed;
		}

		// Without name?
		if (argument3.IsEmpty ())
		{
			g_server->GetYaPBManager ()->SetHealth (static_cast <unsigned short> (abs (argument2.GetValue <int> ())));

			return BotConsoleCommandReturnType_Accessed;
		}

		/// @TODO: For specified bot.
	}
	else if (argument1.CompareWithoutCaseCheck ("botmenu") == 0 || argument1.CompareWithoutCaseCheck ("menu") == 0)		// displays main bot menu
		DisplayMenu (MenuID_YaPBMain);
	else if (argument1.CompareWithoutCaseCheck ("cmdmenu") == 0 || argument1.CompareWithoutCaseCheck ("cmenu") == 0)	// display command menu
	{
		if (!IsAlive ())
		{
			// reset menu display
			CloseMenu ();

			Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_YouDeadAndHaveNoAccessToThisMenu));

			return BotConsoleCommandReturnType_Accessed;
		}

		DisplayMenu (MenuID_BotCommand);
	}
	else if (argument1.CompareWithoutCaseCheck ("setcvar") == 0 || argument1.CompareWithoutCaseCheck ("set") == 0)	// sets public server bot console variable
	{
		for (unsigned char index (0u); index < Console::Bot::VariableID_Total; ++index)
		{
			if (Console::Bot::variableManager.GetVariable (static_cast <Console::Bot::VariableID_t> (index))->GetName ().CompareWithoutCaseCheck (3u, argument2) != 0)
				continue;

			if (index == Console::Bot::VariableID_Password || index == Console::Bot::VariableID_PasswordKey)
				return BotConsoleCommandReturnType_RequireAccess;

			Console::Bot::variableManager.GetVariable (static_cast <Console::Bot::VariableID_t> (index))->SetString (argument3);

			break;
		}
	}
	#if defined _DEBUG
		else if (argument1.CompareWithoutCaseCheck ("debug") == 0)	// some debug routines
		{
			// test random number generator
			if (argument2.CompareWithoutCaseCheck ("randgen") == 0)
			{
				for (unsigned short index (0u); index < 500u; ++index)
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Result Range (0 - 100): %u.\n", g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 100u));
			}
			#if defined MEMORY_MANAGER
				else if (argument2.CompareWithoutCaseCheck ("memrep") == 0)	// dump memory information
				{
					m_dumpMemoryReport ();

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("All memory information logged into memreport.log file, placed to hl.exe directory.\n");
				}
			#endif	// if defined MEMORY_MANAGER
		}
	#endif	// if defined _DEBUG
	else if (argument1.CompareWithoutCaseCheck ("navmesh") == 0)
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () || !IsHost ())
			return BotConsoleCommandReturnType_ServerOnly;

		if (argument2.IsEmpty ())
		{
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->GetValue <bool> () == false)
			{
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->SetValue <bool> (true);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing Enabled.\n");
			}
			else
			{
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->SetValue <bool> (false);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing Disabled.\n");
			}
		}
		else if (argument2.CompareWithoutCaseCheck ("on") == 0)
		{
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->GetValue <bool> () == false)
			{
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->SetValue <bool> (true);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing Enabled.\n");
			}
			else
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing already Enabled.\n");
		}
		else if (argument2.CompareWithoutCaseCheck ("off") == 0)
		{
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->GetValue <bool> () == true)
			{
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->SetValue <bool> (false);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing Disabled.\n");
			}
			else
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing already Disabled.\n");
		}
		else if (argument2.CompareWithoutCaseCheck ("analyze") == 0)
			g_server->GetNavigationMeshManager ().BeginAnalysis ();
		else if (argument2.CompareWithoutCaseCheck ("generate") == 0)
			g_server->GetNavigationMeshManager ().BeginGeneration ();
		else if (argument2.CompareWithoutCaseCheck ("generate_incremental") == 0)
			g_server->GetNavigationMeshManager ().BeginGeneration (true);
		else if (argument2.CompareWithoutCaseCheck ("save") == 0)
		{
			if (g_server->GetNavigationMeshManager ().Save ())
				AddLogEntry (true, LogLevel_Default, false, "Navigation map '%s' saved.", g_server->GetNavigationMeshManager ().GetFilename ().GetData ());
			else
				AddLogEntry (true, LogLevel_Error, false, "Cannot save navigation map '%s'.", g_server->GetNavigationMeshManager ().GetFilename ().GetData ());
		}
		else if (argument2.CompareWithoutCaseCheck ("load") == 0)
		{
			if (g_server->GetNavigationMeshManager ().Load () != NavigationMesh::NAV_OK)
				AddLogEntry (true, LogLevel_Error, false, "Navigation Mesh load failed.");
		}
		else if (argument2.CompareWithoutCaseCheck ("print_place_list") == 0)
		{
			DynamicArray <NavigationMesh::Place_t> placeDirectory;

			for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
			{
				const NavigationMesh::Place_t place (g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetPlace ());

				if (place > NavigationMesh::UNDEFINED_PLACE && !placeDirectory.IsMember (place))
					placeDirectory += place;
			}

			AddLogEntry (true, LogLevel_Default, false, "Map uses %u place names:", placeDirectory.GetElementNumber ());

			for (unsigned short index (0u); index < placeDirectory.GetElementNumber (); ++index)
				AddLogEntry (true, LogLevel_Default, false, "\t%s.", g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (placeDirectory[index]));
		}
		else if (argument2.CompareWithoutCaseCheck ("toggle_place_mode") == 0)
			g_server->GetNavigationMeshManager ().CommandNavTogglePlaceMode ();
		else if (argument2.CompareWithoutCaseCheck ("make_sniper_spots") == 0)
			g_server->GetNavigationMeshManager ().CommandNavMakeSniperSpots ();
		else if (argument2.CompareWithoutCaseCheck ("transient") == 0)
			g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NavigationMesh::NAV_MESH_TRANSIENT);
		else if (argument2.CompareWithoutCaseCheck ("delete") == 0)
			g_server->GetNavigationMeshManager ().CommandNavDelete ();
		else if (argument2.CompareWithoutCaseCheck ("mark") == 0)
			g_server->GetNavigationMeshManager ().CommandNavMark ();
		else if (argument2.CompareWithoutCaseCheck ("toggle_place_painting") == 0)
			g_server->GetNavigationMeshManager ().CommandNavTogglePlacePainting ();
		else if (argument2.CompareWithoutCaseCheck ("print_all_places") == 0)
			g_server->GetNavigationMeshManager ().GetPlaceContainer ().PrintAllPlaces ();
		else if (argument2.CompareWithoutCaseCheck ("use_place") == 0)
		{
			if (argument3.IsEmpty ())
			{
				// no arguments = list all available places
				g_server->GetNavigationMeshManager ().GetPlaceContainer ().PrintAllPlaces ();
			}
			else
			{
				// single argument = set current place
				const NavigationMesh::Place_t place (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (argument3));

				if (place == NavigationMesh::UNDEFINED_PLACE)
					AddLogEntry (true, LogLevel_Default, false, "Ambiguous.");
				else
				{
					AddLogEntry (true, LogLevel_Default, false, "Current place set to '%s'.", g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (place).GetData ());

					g_server->GetNavigationMeshManager ().SetNavPlace (place);
				}
			}
		}
		else if (argument2.CompareWithoutCaseCheck ("place_replace") == 0)
		{
			if (argument4.IsEmpty ())
			{
				// no arguments
				AddLogEntry (true, LogLevel_Default, false, "Usage: " PRODUCT_CONSOLE_TAG " navmesh place_replace <OldPlace> <NewPlace>.");
			}
			else
			{
				// two arguments - replace the first place with the second
				const NavigationMesh::Place_t oldPlace (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (argument3));
				const NavigationMesh::Place_t newPlace (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (argument4));

				if (oldPlace == NavigationMesh::UNDEFINED_PLACE || newPlace == NavigationMesh::UNDEFINED_PLACE)
					AddLogEntry (true, LogLevel_Default, false, "Ambiguous.");
				else for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
				{
					NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

					if (area->GetPlace () == oldPlace)
						area->SetPlace (newPlace);
				}
			}
		}
	}
	else if (argument1.CompareWithoutCaseCheck ("waypoint") == 0 || argument1.CompareWithoutCaseCheck ("wp") == 0 || argument1.CompareWithoutCaseCheck ("wpt") == 0)	// waypoint manimupulation (really obsolete, can be edited through menu) (supported only on listen server)
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () || !IsHost ())
			return BotConsoleCommandReturnType_ServerOnly;

		// enables or disable waypoint displaying

		if (argument2.IsEmpty ())
		{
/*NOT SUPPORT YET!!!			g_waypointOn ^= true;	// switch waypoint editing on/off (XOR it)

			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Waypoints are %s.\n", g_waypointOn ? "Enabled" : "Disabled");*/
		}
		else if (argument2.CompareWithoutCaseCheck ("on") == 0)
		{
/*NOT SUPPORT YET!!!			if (!g_waypointOn)
			{
				g_waypointOn = true;

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing Enabled.\n");
			}
			else
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing already Enabled.\n");

			// enables noclip cheat
			if (argument3.CompareWithoutCaseCheck ("noclip") == 0)
			{
				if (g_editNoclip && m_edict->variables.moveType != HalfLifeEngine::SDK::Constants::MOVETYPE_WALK)
				{
					m_edict->variables.moveType = HalfLifeEngine::SDK::Constants::MOVETYPE_WALK;

					g_editNoclip = false;

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Noclip Cheat Disabled.\n");
				}
				else
				{
					m_edict->variables.moveType = HalfLifeEngine::SDK::Constants::MOVETYPE_NOCLIP;

					g_editNoclip = true;

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Noclip Cheat Enabled.\n");
				}
			}
*/
			HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommand ("yapb wp mdl on\n");
		}
		else if (argument2.CompareWithoutCaseCheck ("off") == 0)	// switching waypoint editing off
		{
/*NOT SUPPORT YET!!!			if (g_waypointOn)
			{
				g_waypointOn = false;

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing Disabled.\n");
			}
			else
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing already Disabled.\n");

			g_editNoclip = false;
*/
			m_edict->variables.moveType = HalfLifeEngine::SDK::Constants::MOVETYPE_WALK;

			HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommand ("yapb wp mdl off\n");
		}
		else if (argument2.CompareWithoutCaseCheck ("mdl") == 0 || argument2.CompareWithoutCaseCheck ("models") == 0)	// toggles displaying player models on spawn spots
		{
			if (argument3.CompareWithoutCaseCheck ("on") == 0)
			{
				HalfLifeEngine::SDK::Classes::Edict *spawnEntity (NULL);

				while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_player_deathmatch"))->IsValid ())
					spawnEntity->variables.effects &= ~HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;

				while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_player_start"))->IsValid ())
					spawnEntity->variables.effects &= ~HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;

				while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_vip_start"))->IsValid ())
					spawnEntity->variables.effects &= ~HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;

				HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_SetValue ("mp_roundtime",  9.0f);	// reset round time to maximum
				HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_SetValue ("mp_timelimit",  0.0f);	// disable the time limit
				HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_SetValue ("mp_freezetime", 0.0f);	// disable freezetime
			}
			else if (argument3.CompareWithoutCaseCheck ("off") == 0)
			{
				HalfLifeEngine::SDK::Classes::Edict *spawnEntity (NULL);

				while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_player_deathmatch"))->IsValid ())
					spawnEntity->variables.effects |= HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;

				while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_player_start"))->IsValid ())
					spawnEntity->variables.effects |= HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;

				while ((spawnEntity = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (spawnEntity, "info_vip_start"))->IsValid ())
					spawnEntity->variables.effects |= HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;
			}
		}
/*NOT SUPPORT YET!!!		else if (argument2.CompareWithoutCaseCheck ("find") == 0)	// show direction to specified waypoint
			g_server->GetWaypointManager ().SetFindIndex (argument3.GetValue <int> ());*/
		else if (argument2.CompareWithoutCaseCheck ("add") == 0)	// opens adding waypoint menu
		{
/*NOT SUPPORT YET!!!			if (!g_waypointOn)
			{
				g_waypointOn = true;	// turn waypoints on

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing Enabled.\n");
			}
*/
			DisplayMenu (MenuID_WaypointAdd);
		}
		else if (argument2.CompareWithoutCaseCheck ("addbasic") == 0)	// creates basic waypoints on the map (ladder/spawn points/goals)
		{
//NOT SUPPORT YET!!!			g_server->GetWaypointManager ().CreateBasic ();

			Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, "Basic waypoints was Created");
		}
		else if (argument2.CompareWithoutCaseCheck ("delete") == 0)	// delete nearest to host edict waypoint
		{
/*NOT SUPPORT YET!!!			if (!g_waypointOn)
			{
				g_waypointOn = true;	// turn waypoints on

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing Enabled.\n");
			}

			g_server->GetWaypointManager ().Delete ();*/
		}
		else if (argument2.CompareWithoutCaseCheck ("save") == 0)	// save waypoint data into file on hard disk
		{
			if (argument3.CompareWithoutCaseCheck ("nocheck") == 0 || g_server->GetWaypointManager ().IsNodesValid ())
			{
				g_server->GetWaypointManager ().Save ();

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoints Saved.\n");
			}
		}
/*NOT SUPPORT YET!!!		else if (argument2.CompareWithoutCaseCheck ("erase") == 0 || argument2.CompareWithoutCaseCheck ("deletewp") == 0)	// remove waypoint and all corresponding files from hard disk
			g_server->GetWaypointManager ().EraseFromHardDisk ();*/
		else if (argument2.CompareWithoutCaseCheck ("load") == 0)	// load all waypoints again (overrides all changes, that wasn't saved)
		{
			if (g_server->GetWaypointManager ().Load ())
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoints loaded.\n");
		}
		else if (argument2.CompareWithoutCaseCheck ("check") == 0)	// check all nodes for validation
		{
			if (g_server->GetWaypointManager ().IsNodesValid ())
				Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, "Nodes work Fine");
		}
		else if (argument2.CompareWithoutCaseCheck ("flags") == 0)		// opens menu for setting (removing) waypoint flags
			DisplayMenu (MenuID_WaypointFlagSet);
/*NOT SUPPORT YET!!!		else if (argument2.CompareWithoutCaseCheck ("setradius") == 0)	// setting waypoint radius
			g_server->GetWaypointManager ().SetRadius (argument3.GetValue <float> ());
		else if (argument2.CompareWithoutCaseCheck ("cache") == 0)		// remembers nearest waypoint
			g_server->GetWaypointManager ().CacheWaypoint ();
		else if (argument2.CompareWithoutCaseCheck ("teleport") == 0)	// teleport player to specified waypoint
		{
			if (!IsAlive ())
			{
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("You can not be teleported while is dead.\n");

				return BotConsoleCommandReturnType_Accessed;
			}

			const int teleportPoint (argument3.GetValue <int> ());

			if (teleportPoint < g_server->GetWaypointManager ().GetWaypointsNumber ())
			{
				if (!g_waypointOn)
				{
					g_waypointOn = true;	// turn waypoints on

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing Enabled.\n");
				}

				m_edict->SetOrigin (g_server->GetWaypointManager ().GetPath (teleportPoint)->origin);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("You teleported to waypoint #%u [x=\"%.2f\", y=\"%.2f\", z=\"%.2f\"].\n", teleportPoint, g_server->GetWaypointManager ().GetPath (teleportPoint)->origin.x, g_server->GetWaypointManager ().GetPath (teleportPoint)->origin.y, g_server->GetWaypointManager ().GetPath (teleportPoint)->origin.z);
			}
		}*/
		else if (argument2.CompareWithoutCaseCheck ("menu") == 0)	// displays waypoint menu
			DisplayMenu (MenuID_WaypointMain¹1);
/*NOT SUPPORT YET!!!		else	// otherwise display waypoint current status
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Waypoints are %s.\n", g_waypointOn ? "Enabled" : "Disabled");*/
	}
	else if (argument1.CompareWithoutCaseCheck ("pathWaypoint") == 0 || argument1.CompareWithoutCaseCheck ("path") == 0 || argument1.CompareWithoutCaseCheck ("pwp") == 0)	// path waypoint editing system (supported only on listen server)
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () || !IsHost ())
			return BotConsoleCommandReturnType_ServerOnly;

		// opens path creation menu
		if (argument2.CompareWithoutCaseCheck ("create") == 0)
			DisplayMenu (MenuID_WaypointPathConnectionsSet);
/*NOT SUPPORT YET!!!		else if (argument2.CompareWithoutCaseCheck ("create_in") == 0)		// creates incoming path from the cached waypoint
			g_server->GetWaypointManager ().CreatePath (PathConnection_Incoming);
		else if (argument2.CompareWithoutCaseCheck ("create_out") == 0)		// creates outgoing path from current waypoint
			g_server->GetWaypointManager ().CreatePath (PathConnection_Outgoing);
		else if (argument2.CompareWithoutCaseCheck ("create_both") == 0)	// creates bidirectional path from cahed to current waypoint
			g_server->GetWaypointManager ().CreatePath (PathConnection_Bothways);
		else if (argument2.CompareWithoutCaseCheck ("delete") == 0)			// delete special path
			g_server->GetWaypointManager ().DeletePath ();*/
		else if (argument2.CompareWithoutCaseCheck ("autodistance") == 0)	// sets auto path maximum distance
			DisplayMenu (MenuID_WaypointAutoPathMaximumDistanceSet);
	}
	else if (argument1.CompareWithoutCaseCheck ("autoWaypoint") == 0 || argument1.CompareWithoutCaseCheck ("autowp") == 0)	// automatic waypoint handling (supported only on listen server)
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () || !IsHost ())
			return BotConsoleCommandReturnType_ServerOnly;

		// enable auto waypointing
/*NOT SUPPORT YET!!!
		if (argument2.IsEmpty ())
		{
			if (!g_waypointOn)
				g_waypointOn = true;	// turn this on just in case

			// switch auto waypointing on/off (XOR it)
			g_autoWaypoint ^= true;
		}
		else if (argument2.CompareWithoutCaseCheck ("on") == 0 && !g_autoWaypoint)	// enable auto waypointing
		{
			if (!g_waypointOn)
				g_waypointOn = true;	// turn this on just in case

			// turn waypoints on
			g_autoWaypoint = true;
		}
		else if (argument2.CompareWithoutCaseCheck ("off") == 0 && g_autoWaypoint)	// disable auto waypointing
			g_autoWaypoint = false;

		// display status
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Auto-Waypoint %s.\n", g_autoWaypoint ? "Enabled" : "Disabled");*/
	}
	else if ((argument1.CompareWithoutCaseCheck ("experience") == 0 || argument1.CompareWithoutCaseCheck ("exp") == 0) && argument2.CompareWithoutCaseCheck ("save") == 0)	// experience system handling (supported only on listen server)
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () || !IsHost ())
			return BotConsoleCommandReturnType_ServerOnly;
/*NOT SUPPORT YET!!!
		// write experience and visibility tables to hard disk
		g_server->GetWaypointManager ().SaveExperienceTable ();
		g_server->GetWaypointManager ().SaveVisibilityTable ();
*/
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Experience table saved.\n");
	}
	else
		return BotConsoleCommandReturnType_NotFound;	// command is not handled by bot

	// Command was handled by bot.
	return BotConsoleCommandReturnType_Accessed;
}
#endif	// if 0
const Client::HandleExecutedCommandResult_t Human::HandleExecutedCommand (const DynamicString &command, const DynamicString &argument1)
{
	// this function is called whenever the client whose player entity is 'client' issues a client
	// command. How it works is that clients all have a global string in their client DLL that
	// stores the command string; if ever that string is filled with characters, the client DLL
	// sends it to the engine as a command to be executed. When the engine has executed that
	// command, that string is reset to zero. By the server side, we can access this string
	// by asking the engine with the CmdArgv(), CmdArgs() and CmdArgc() functions that work just
	// like executable files argument processing work in C (argc gets the number of arguments,
	// command included, args returns the whole string, and argv returns the wanted argument
	// only). Here is a good place to set up either bot debug commands the listen server client
	// could type in his game console, or real new client commands, but we wouldn't want to do
	// so as this is just a bot DLL, not a MOD. The purpose is not to add functionality to
	// clients. Hence it can lack of commenting a bit, since this code is very subject to change.

//	PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Human is executed command: \"%s\".\n", command.GetData ());
/*
	#if defined _DEBUG
		if (command == "say" && !argument1.IsEmpty ())
		{
			HudMessageCentered
			(
				Math::Vector2D (0.5f, 0.5f),
				ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u)),
				ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
				HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 5.0f, 0.0078125f, 2.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

				argument1
			);

			return HandleExecutedCommandResult_Handled;
	#endif	// if defined _DEBUG
*/
	// this player has issued a message; handle this message....
	const HandleExecutedCommandResult_t result (Client::HandleExecutedCommand (command, argument1));

	if (result != HandleExecutedCommandResult_Ignored)
		return result;
/*
	// Don't search client commands of other edicts than admins....
	if (!IsAdmin ())
		return HandleExecutedCommandResult_Ignored;
*/
	// is bot command?
	if (g_consoleCommandsManager.IsYaPBCommand (command))
	{
		// Check status for passed command.
		g_consoleCommandsManager.HandleExecutedCommand (this, argument1.IsEmpty () ? '\?' : argument1, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (2u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (3u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (4u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (5u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (6u));
/*	if (command.CompareWithoutCaseCheck (PRODUCT_CONSOLE_TAG_OTHER) == 0 || command.CompareWithoutCaseCheck (PRODUCT_CONSOLE_TAG) == 0)
	{
		const DynamicString argument2 (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (2u));

		switch (BotCommandHandler (argument1.IsEmpty () ? '\?' : argument1, argument2, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (3u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (4u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (5u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (6u)))
		{
			case BotConsoleCommandReturnType_NotFound:
				AddLogEntry (false, LogLevel_Error, false, "Client \"%s\" has executed unknown command: \"%s\".", GetName ().GetData (), argument1.GetData ());
				PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Unknown command: \"%s\".\n", argument1.GetData ());

				break;

			case BotConsoleCommandReturnType_ServerOnly:
				AddLogEntry (false, LogLevel_Error, false, "Client \"%s\" has executed command %s, that can only be executed from server console.", GetName ().GetData (), argument1.GetData ());
				PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Command %s, can only be executed from server console.\n", argument1.GetData ());

				break;

			case BotConsoleCommandReturnType_RequireAccess:
				AddLogEntry (false, LogLevel_Error, false, "Client \"%s\" has executed console variable " PRODUCT_CONSOLE_TAG "_%s, that can be only set via RCON access.", GetName ().GetData (), argument2.GetData ());
				PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Console variable " PRODUCT_CONSOLE_TAG "_%s, can be only set via RCON access.\n", argument2.GetData ());

				break;
		}
*/
		if (MetaModEngine::Globals::g_metaModEngine != NULL)
			MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

		return HandleExecutedCommandResult_Supercede;
	}
	else if (command == "menuselect" && !argument1.IsEmpty () && GetOpenedYaPBMenu ().id != MenuID_None)	// care for menus instead....
	{
		const HalfLifeEngine::SDK::Constants::Menu::Key_t selection (argument1.GetValue <HalfLifeEngine::SDK::Constants::Menu::Key_t> ());

		// Check is selected key valid....
		if (selection > HalfLifeEngine::SDK::Constants::Menu::Key_None && selection < HalfLifeEngine::SDK::Constants::Menu::Key_Total && (/*g_menus[GetOpenedYaPBMenu ().id].validSlots*/GetOpenedYaPBMenu ().keys & BIT (selection - 1u)))
		{
			HandleYaPBMenus (static_cast <const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t> (selection));

			if (MetaModEngine::Globals::g_metaModEngine != NULL)
				MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

			return HandleExecutedCommandResult_Supercede;
		}
	}

	return HandleExecutedCommandResult_Ignored;
}

void Human::HandleYaPBMenus (const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection)
{
	{
		g_menusManager.HandleSelection (this, selection - 1u);

		return;
	}
#if 0
	static unsigned char storeAddBotVars[4u] = {BotSkillValue_Minimum, HalfLifeEngine::SDK::Constants::TeamID_AutoSelect, 5u, static_cast <unsigned char> (Personality_None)};
	static HalfLifeEngine::SDK::Constants::TeamID_t fillServerTeam (HalfLifeEngine::SDK::Constants::TeamID_AutoSelect);
	static bool fillCommand (false);

	switch (GetOpenedYaPBMenu ().id)
	{
		// main YaPB menu
		case MenuID_YaPBMain:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					fillCommand = false;

					DisplayMenu (MenuID_BotControl);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					DisplayMenu (MenuID_BotFeatures);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					fillCommand = true;

					DisplayMenu (MenuID_BotTeamSelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					g_server->GetYaPBManager ()->KillAll ();

					CloseMenu ();	// reset menu display

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					CloseMenu ();	// reset menu display

					break;
			}

			break;

		// bot features menu
		case MenuID_BotFeatures:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					DisplayMenu (MenuID_BotWeaponModeSelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					DisplayMenu (MenuID_WaypointMain¹1);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					DisplayMenu (MenuID_BotPersonalitySelect);

					break;

				#if defined _DEBUG
					case HalfLifeEngine::SDK::Constants::Menu::Key_4:
						Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->SetValue <bool> (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <bool> () ^ true);

						// reset menu display
						CloseMenu ();

						break;
				#endif	// if defined _DEBUG

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
					if (IsAlive ())
						DisplayMenu (MenuID_BotCommand);
					else
					{
						// reset menu display
						CloseMenu ();

						Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_YouDeadAndHaveNoAccessToThisMenu));
					}

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					CloseMenu ();	// reset menu display

					break;
			}

			break;

		// bot control menu
		case MenuID_BotControl:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					g_server->GetYaPBManager ()->AddBot ();

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					DisplayMenu (MenuID_BotSkillSelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					g_server->GetYaPBManager ()->KickRandom ();

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					g_server->GetYaPBManager ()->KickAll ();

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
					DisplayMenu (MenuID_BotKickMenu¹1);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// weapon mode select menu
		case MenuID_BotWeaponModeSelect:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
//NOT SUPPORT YET!!!					g_server->GetYaPBManager ()->SetWeaponMode (selection);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// personality select menu
		case MenuID_BotPersonalitySelect:
			if (fillCommand)
			{
				switch (selection)
				{
					case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					case HalfLifeEngine::SDK::Constants::Menu::Key_4:
						g_server->GetYaPBManager ()->FillServer (fillServerTeam, selection == HalfLifeEngine::SDK::Constants::Menu::Key_4 ? Personality_None : static_cast <const Personality_t> (selection - 1u), storeAddBotVars[0u]);

						// reset menu display
						CloseMenu ();

					case HalfLifeEngine::SDK::Constants::Menu::Key_0:
						// reset menu display
						CloseMenu ();

						break;
				}
			}
			else switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					storeAddBotVars[3u] = selection == HalfLifeEngine::SDK::Constants::Menu::Key_4 ? Personality_None : static_cast <unsigned char> (selection - 1u);

					DisplayMenu (MenuID_BotTeamSelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// skill select menu
		case MenuID_BotSkillSelect:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					storeAddBotVars[0u] = g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillValue_Minimum, BotSkillLevel_Stupid);

					DisplayMenu (MenuID_BotPersonalitySelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					storeAddBotVars[0u] = g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Stupid, BotSkillLevel_Newbie);

					DisplayMenu (MenuID_BotPersonalitySelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					storeAddBotVars[0u] = g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Newbie, BotSkillLevel_Average);

					DisplayMenu (MenuID_BotPersonalitySelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					storeAddBotVars[0u] = g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Average, BotSkillLevel_Advanced);

					DisplayMenu (MenuID_BotPersonalitySelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
					storeAddBotVars[0u] = g_randomNumberGenerator.GetValueBetween <unsigned char> (BotSkillLevel_Advanced, BotSkillLevel_Professional);

					DisplayMenu (MenuID_BotPersonalitySelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
					storeAddBotVars[0u] = BotSkillLevel_Godlike;

					DisplayMenu (MenuID_BotPersonalitySelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// team select menu
		case MenuID_BotTeamSelect:
			if (fillCommand)
			{
				switch (selection)
				{
					case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					case HalfLifeEngine::SDK::Constants::Menu::Key_2:
						// turn off console variables if specified team
						HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_SetValue ("mp_limitteams",      0u);
						HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_SetValue ("mp_autoteambalance", false);

						// reset menu display
						CloseMenu ();

					case HalfLifeEngine::SDK::Constants::Menu::Key_5:
						fillServerTeam = HalfLifeEngine::SDK::Constants::TeamID_AutoSelect;

						DisplayMenu (MenuID_BotSkillSelect);

						break;

					case HalfLifeEngine::SDK::Constants::Menu::Key_0:
						// reset menu display
						CloseMenu ();

						break;
				}
			}
			else switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					storeAddBotVars[1u] = selection;

					DisplayMenu (MenuID_BotTerroristModelSelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					storeAddBotVars[1u] = selection;

					DisplayMenu (MenuID_BotCounterModelSelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
					storeAddBotVars[1u] = selection;

					storeAddBotVars[2u] = 5u;

					g_server->GetYaPBManager ()->AddBot ("", storeAddBotVars[0u], static_cast <Personality_t> (storeAddBotVars[3u]), static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (storeAddBotVars[1u]), storeAddBotVars[2u]);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// terrorist/counter-terrorist model select menu
		case MenuID_BotTerroristModelSelect:
		case MenuID_BotCounterModelSelect:
			if (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModID () == HalfLifeEngine::Engine::Version_Steam)
			{
				switch (selection)
				{
					case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					case HalfLifeEngine::SDK::Constants::PlayerModelID_AutoSelect:
						storeAddBotVars[2u] = selection;

						g_server->GetYaPBManager ()->AddBot ("", storeAddBotVars[0u], static_cast <Personality_t> (storeAddBotVars[3u]), static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (storeAddBotVars[1u]), storeAddBotVars[2u]);

						// reset menu display
						CloseMenu ();

						break;

					case HalfLifeEngine::SDK::Constants::Menu::Key_0:
						// reset menu display
						CloseMenu ();

						break;
				}
			}
			else switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
				case HalfLifeEngine::SDK::Constants::PlayerModelID_ConditionZero_AutoSelect:
					storeAddBotVars[2u] = selection;

					g_server->GetYaPBManager ()->AddBot ("", storeAddBotVars[0u], static_cast <Personality_t> (storeAddBotVars[3u]), static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (storeAddBotVars[1u]), storeAddBotVars[2u]);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// kickmenu #1
		case MenuID_BotKickMenu¹1:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
				{
					YaPB *const bot (g_server->GetYaPBManager ()->GetBot (selection - 1u));

					// In case bot already kicked by some reason....
					if (!bot->IsValid ())
						break;

					bot->Kick ();
/*
					// Must be called after ClientDisconnect() function, but now it not so....
//					DisplayMenu (MenuID_BotKickMenu¹1);	// Here is a need to add some delay and a task to show it...
					// reset menu display
//					CloseMenu ();
*/
					break;
				}

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
					DisplayMenu (MenuID_BotControl);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					DisplayMenu (MenuID_BotKickMenu¹2);

					break;
			}

			break;

		// kickmenu #2
		case MenuID_BotKickMenu¹2:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
				{
					YaPB *const bot (g_server->GetYaPBManager ()->GetBot (selection + 7u));

					// In case bot already kicked by some reason....
					if (!bot->IsValid ())
						break;

					bot->Kick ();
/*
					// Must be called after ClientDisconnect() function, but now it not so....
//					DisplayMenu (MenuID_BotKickMenu¹2);	// Here is a need to add some delay and a task to show it...
					// reset menu display
//					CloseMenu ();
*/
					break;
				}

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
					DisplayMenu (MenuID_BotKickMenu¹1);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					DisplayMenu (MenuID_BotKickMenu¹3);

					break;
			}

			break;

		// kickmenu #3
		case MenuID_BotKickMenu¹3:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
				{
					YaPB *const bot (g_server->GetYaPBManager ()->GetBot (selection + 15u));

					// In case bot already kicked by some reason....
					if (!bot->IsValid ())
						break;

					bot->Kick ();
/*
					// Must be called after ClientDisconnect() function, but now it not so....
//					DisplayMenu (MenuID_BotKickMenu¹3);	// Here is a need to add some delay and a task to show it...
					// reset menu display
//					CloseMenu ();
*/
					break;
				}

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
					DisplayMenu (MenuID_BotKickMenu¹2);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					DisplayMenu (MenuID_BotKickMenu¹4);

					break;
			}

			break;

		// kickmenu #4
		case MenuID_BotKickMenu¹4:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
				{
					YaPB *const bot (g_server->GetYaPBManager ()->GetBot (selection + 23u));

					// In case bot already kicked by some reason....
					if (!bot->IsValid ())
						break;

					bot->Kick ();
/*
					// Must be called after ClientDisconnect() function, but now it not so....
//					DisplayMenu (MenuID_BotKickMenu¹4);	// Here is a need to add some delay and a task to show it...
					// reset menu display
//					CloseMenu ();
*/
					break;
				}

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
					DisplayMenu (MenuID_BotKickMenu¹3);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// command menu
		case MenuID_BotCommand:
			switch (selection)
			{
/*NOT SUPPORT YET!!!				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				{
					YaPB *bot;

					if (!FindNearestPlayer (reinterpret_cast <void **> (&bot), this, 600.0f, true, true, true) || g_server->GetYaPBManager ()->GetPlayersNum (Flag_Bot) == 0)
						break;

					if (selection == HalfLifeEngine::SDK::Constants::Menu::Key_1)
					{
						if (bot->IsPositionVisible (GetOrigin ()) && !(bot->m_edict->variables.weapons & BIT (HalfLifeEngine::SDK::Constants::WeaponID_C4)) && !bot->HasFollowingHostage () && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < (bot->m_profile->personality == Personality_Rusher ? 35u : 90u) && !g_roundEnded && bot->GetOrigin ().GetDistanceSquared (GetOrigin ()) <= MATH_GET_SQUARED (500.0f) && bot->GetCurrentTask ()->taskID != Task_PlantBomb && bot->GetCurrentTask ()->taskID != Task_DefuseBomb && bot->GetCurrentTask ()->taskID != Task_EscapeFromBomb && bot->GetCurrentTask ()->taskID != Task_PerformDoubleJump && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DebugGoal)->GetValue <unsigned short> () == InvalidWaypointIndex && !Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Deathmatch)->GetValue <bool> () && !Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_KnifeMode)->GetValue <bool> ())
						{
							bot->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Affirmative);

							bot->ResetDoubleJumpState ();

							bot->m_doubleJumpHelp.entity = m_edict;
							bot->m_doubleJumpHelp.origin = GetOrigin ();

							bot->StartTask (Task_PerformDoubleJump, TaskPriority_PerformDoubleJump, -1, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime (), true);
						}
						else
							bot->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

						break;
					}

					bot->ResetDoubleJumpState ();

					// reset menu display
					CloseMenu ();

					break;
				}

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					if (FindNearestPlayer (reinterpret_cast <void **> (&bot), this, 300.0f, true, true, true))
						bot->DiscardWeaponForUser (this, selection == HalfLifeEngine::SDK::Constants::Menu::Key_3);

					// reset menu display
					CloseMenu ();

					break;
*/
				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// main waypoint menu
		case MenuID_WaypointMain¹1:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
//NOT SUPPORT YET!!!					HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommandFormat ("yapb waypoint %s\n", g_waypointOn ? "off" : "on");

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
/*NOT SUPPORT YET!!!					g_waypointOn = true;

					g_server->GetWaypointManager ().CacheWaypoint ();
*/
					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
//NOT SUPPORT YET!!!					g_waypointOn = true;

					DisplayMenu (MenuID_WaypointPathConnectionsSet);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
/*NOT SUPPORT YET!!!					g_waypointOn = true;

					g_server->GetWaypointManager ().DeletePath ();
*/
					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
//NOT SUPPORT YET!!!					g_waypointOn = true;

					DisplayMenu (MenuID_WaypointAdd);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
/*NOT SUPPORT YET!!!					g_waypointOn = true;

					g_server->GetWaypointManager ().Delete ();
*/
					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
//NOT SUPPORT YET!!!					g_waypointOn = true;

					DisplayMenu (MenuID_WaypointAutoPathMaximumDistanceSet);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
//NOT SUPPORT YET!!!					g_waypointOn = true;

					DisplayMenu (MenuID_WaypointRadiusSelect);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
					DisplayMenu (MenuID_WaypointMain¹2);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// main waypoint menu (page 2)
		case MenuID_WaypointMain¹2:
			switch (selection)
			{
/*NOT SUPPORT YET!!!				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					static struct Points_t
					{
						unsigned short terrorist;
						unsigned short counterTerrorist;
						unsigned short goal;
						unsigned short rescue;
						unsigned short camp;
						unsigned short sniper;
						unsigned short noHostage;

						inline Points_t (void) :
							terrorist (0u),
							counterTerrorist (0u),
							goal (0u),
							rescue (0u),
							camp (0u),
							sniper (0u),
							noHostage (0u)
						{ /*VOID*//* }
					} points;

					for (unsigned short index (0u); index < g_server->GetWaypointManager ().GetWaypointsNumber (); ++index)
					{
						if (g_server->GetWaypointManager ().GetPath (index)->flags & Waypoint::Flag_Terrorist)
							++points.terrorist;

						if (g_server->GetWaypointManager ().GetPath (index)->flags & Waypoint::Flag_Counter)
							++points.counterTerrorist;

						if (g_server->GetWaypointManager ().GetPath (index)->flags & Waypoint::Flag_Goal)
							++points.goal;
						else if (g_server->GetWaypointManager ().GetPath (index)->flags & Waypoint::Flag_Rescue)
							++points.rescue;

						if (g_server->GetWaypointManager ().GetPath (index)->flags & Waypoint::Flag_Camp)
							++points.camp;

						if (g_server->GetWaypointManager ().GetPath (index)->flags & Waypoint::Flag_Sniper)
							++points.sniper;

						if (g_server->GetWaypointManager ().GetPath (index)->flags & Waypoint::Flag_NoHostage)
							++points.noHostage;
					}

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat
					(
						"\n"
						"YaPB Current Waypoint Stats:\n\n"

						"Waypoints: %i\n"
						"T Points: %i\n"
						"CT Points: %i\n"
						"Goal Points: %i\n"
						"Rescue Points: %i\n"
						"Camp Points: %i\n"
						"Sniper Points: %i\n"
						"Block Hostage Points: %i\n", g_server->GetWaypointManager ().GetWaypointsNumber (), points.terrorist, points.counterTerrorist, points.goal, points.rescue, points.camp, points.sniper, points.noHostage
					);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					g_waypointOn = true;
					g_autoWaypoint &= true;
					g_autoWaypoint ^= true;	// switch auto waypointing on/off (XOR it)

					PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, "Auto-Waypoint %s", g_autoWaypoint ? "Enabled" : "Disabled");

					// reset menu display
					CloseMenu ();

					break;
*/
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
//NOT SUPPORT YET!!!					g_waypointOn = true;

					DisplayMenu (MenuID_WaypointFlagSet);

					break;
/*NOT SUPPORT YET!!!
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					if (g_server->GetWaypointManager ().NodesValid ())
						g_server->GetWaypointManager ().Save ();
					else
						Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, "Waypoint not saved\nThere are errors, see console.");

					// reset menu display
					CloseMenu ();

					break;
*/
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
					g_server->GetWaypointManager ().Save ();

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
					g_server->GetWaypointManager ().Load ();

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
//NOT SUPPORT YET!!!					Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_server->GetWaypointManager ().NodesValid () ? "All Nodes work Fine" : "There are errors, see console for more info...");

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
					HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommand ("yapb wp on noclip\n");

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
					DisplayMenu (MenuID_WaypointMain¹1);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// select waypoint radius menu
		case MenuID_WaypointRadiusSelect:
//NOT SUPPORT YET!!!			g_waypointOn = true;	// turn waypoints on in case

			switch (selection)
			{
/*NOT SUPPORT YET!!!				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					g_server->GetWaypointManager ().SetRadius (0.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					g_server->GetWaypointManager ().SetRadius (8.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					g_server->GetWaypointManager ().SetRadius (16.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					g_server->GetWaypointManager ().SetRadius (32.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
					g_server->GetWaypointManager ().SetRadius (48.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
					g_server->GetWaypointManager ().SetRadius (64.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
					g_server->GetWaypointManager ().SetRadius (80.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
					g_server->GetWaypointManager ().SetRadius (96.0f);

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
					g_server->GetWaypointManager ().SetRadius (128.0f);

					break;
*/
				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// waypoint add menu
		case MenuID_WaypointAdd:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().Add (selection - 1u);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_8:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().Add (100);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_9:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().SetLearnJumpWaypoint ();

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// set waypoint flag menu
		case MenuID_WaypointFlagSet:
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().ToggleFlags (Waypoint::Flag_NoHostage);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().ToggleFlags (Waypoint::Flag_Terrorist);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().ToggleFlags (Waypoint::Flag_Counter);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().ToggleFlags (Waypoint::Flag_Lift);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
//NOT SUPPORT YET!!!					g_server->GetWaypointManager ().ToggleFlags (Waypoint::Flag_Sniper);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;

		// auto-path maximum distance
		case MenuID_WaypointAutoPathMaximumDistanceSet:
/*NOT SUPPORT YET!!!
			switch (selection)
			{
				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					g_autoPathDistance = 0.0f;

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					g_autoPathDistance = 100.0f;

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					g_autoPathDistance = 130.0f;

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_4:
					g_autoPathDistance = 160.0f;

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_5:
					g_autoPathDistance = 190.0f;

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_6:
					g_autoPathDistance = 220.0f;

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_7:
					g_autoPathDistance = 250.0f;

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_autoPathDistance == 0.0f ? "AutoPath disabled" : "AutoPath maximum distance set to %.2f", g_autoPathDistance);
*/
			break;

		// path connections
		case MenuID_WaypointPathConnectionsSet:
			switch (selection)
			{
/*NOT SUPPORT YET!!!				case HalfLifeEngine::SDK::Constants::Menu::Key_1:
					g_server->GetWaypointManager ().CreatePath (WaypointNode::ConnectionType_Outgoing);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_2:
					g_server->GetWaypointManager ().CreatePath (WaypointNode::ConnectionType_Incoming);

					// reset menu display
					CloseMenu ();

					break;

				case HalfLifeEngine::SDK::Constants::Menu::Key_3:
					g_server->GetWaypointManager ().CreatePath (WaypointNode::ConnectionType_Bothways);

					// reset menu display
					CloseMenu ();

					break;
*/
				case HalfLifeEngine::SDK::Constants::Menu::Key_0:
					// reset menu display
					CloseMenu ();

					break;
			}

			break;
	}
#endif	// if 0
}