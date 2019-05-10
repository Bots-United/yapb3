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
// Menus.cpp
//
// Version: $ID:$
//

#include <Core.hpp>

MenuManager g_menusManager;
#if 0
// Text and key flags for bot menues ('\\d' & '\\r' & '\\w' & '\\y' are special CS colour tags).
Menu_t g_menus[MenuID_Total] =
{
	// main YaPB menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yYaPB Main Menu:\n\n"

		"\\r1\\w. YaPB Control\n"
		"\\r2\\w. Features\n\n"

		"\\r3\\w. Fill Server\n"
		"\\r4\\w. End Round\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_YaPBMain

	// bot features menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yYaPB Features:\n\n"

		"\\r1\\w. Weapon Mode Menu\n"
		"\\r2\\w. Waypoint Menu\n"
		"\\r3\\w. Select Personality\n\n"

		"\\r4\\w. Toggle Debug Mode\n"
		"\\r5\\w. Command Menu\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotFeatures

	// bot control menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yYaPB Control Menu:\n\n"

		"\\r1\\w. Add a Bot, Quick\n"
		"\\r2\\w. Add a Bot, Specified\n\n"

		"\\r3\\w. Kick Random Bot\n"
		"\\r4\\w. Kick All Bots\n\n"

		"\\r5\\w. Kick Bot Menu\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotControl

	// bot weapon mode select menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_6 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_7 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yYaPB Weapon Mode:\n\n"

		"\\r1\\w. Knives only\n"
		"\\r2\\w. Pistols only\n"
		"\\r3\\w. Shotguns only\n"
		"\\r4\\w. Machine Guns only\n"
		"\\r5\\w. Rifles only\n"
		"\\r6\\w. Sniper Weapons only\n"
		"\\r7\\w. All Weapons\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotWeaponModeSelect

	// bot personality select menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yYaPB Personality:\n\n"

		"\\r1\\w. Normal\n"
		"\\r2\\w. Aggressive\n"
		"\\r3\\w. Careful\n\n"

		"\\r4\\w. Random\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotPersonalitySelect

	// bot skill select menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_6 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yYaPB Skill Level:\n\n"

		"\\r1\\w. Stupid (1-20)\n"
		"\\r2\\w. Newbie (20-40)\n"
		"\\r3\\w. Average (40-60)\n"
		"\\r4\\w. Advanced (60-80)\n"
		"\\r5\\w. Professional (80-99)\n"
		"\\r6\\w. Godlike (100)\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotSkillSelect

	// bot team select menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\ySelect a team:\n\n"

		"\\r1\\w. Terrorist Force\n"
		"\\r2\\w. Counter-Terrorist Force\n\n"

		"\\r5\\w. Auto-select\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotTeamSelect

	// bot terrorist model select menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\ySelect an appearance:\n\n"

		"\\r1\\w. Phoenix Connexion\n"
		"\\r2\\w. L337 Krew\n"
		"\\r3\\w. Arctic Avengers\n"
		"\\r4\\w. Guerilla Warfare\n\n"

		"\\r5\\w. Auto-select\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotTerroristModelSelect

	// bot counter-terrorist model select menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\ySelect an appearance:\n\n"

		"\\r1\\w. Seal Team 6 (DEVGRU)\n"
		"\\r2\\w. German GSG-9\n"
		"\\r3\\w. UK SAS\n"
		"\\r4\\w. French GIGN\n\n"

		"\\r5\\w. Auto-select\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotCounterModelSelect

	// bot kick menu (page 1)
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		""
	},	// MenuID_BotKickMenu¹1

	// bot kick menu (page 2)
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		""
	},	// MenuID_BotKickMenu¹2

	// bot kick menu (page 3)
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		""
	},	// MenuID_BotKickMenu¹3

	// bot kick menu (page 4)
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		""
	},	// MenuID_BotKickMenu¹4

	// bot command menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yBot Command Menu:\n\n"

		"\\r1\\w. Make Double Jump\n"
		"\\r2\\w. Finish Double Jump\n\n"

		"\\r3\\w. Drop the C4 Bomb\n"
		"\\r4\\w. Drop the Weapon\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_BotCommand

	// waypoint main menu (page 1)
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_6 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_7 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_8 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yWaypoint Operations (Page 1/2):\n\n"

		"\\r1\\w. Show/Hide waypoints\n"
		"\\r2\\w. Cache waypoint\n"
		"\\r3\\w. Create path\n"
		"\\r4\\w. Delete path\n"
		"\\r5\\w. Add waypoint\n"
		"\\r6\\w. Delete waypoint\n"
		"\\r7\\w. Set Autopath Distance\n"
		"\\r8\\w. Set Radius\n\n"

		"\\r9\\w. Next...\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_WaypointMain¹1

	// waypoint main menu (page 2)
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_6 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_7 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_8 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yWaypoint Operations (Page 2/2):\n\n"

		"\\r1\\w. Waypoint stats\n"
		"\\r2\\w. Auto-waypoint on/off\n"
		"\\r3\\w. Set flags\n"
		"\\r4\\w. Save waypoints\n"
		"\\r5\\w. Save without checking\n"
		"\\r6\\w. Load waypoints\n"
		"\\r7\\w. Check waypoints\n"
		"\\r8\\w. Noclip cheat on/off\n\n"

		"\\r9\\w. Previous...\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_WaypointMain¹2

	// waypoint radius select menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_6 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_7 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_8 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yWaypoint Set Radius:\n\n"

		"\\r1\\w. Radius 0.0\n"
		"\\r2\\w. Radius 8.0\n"
		"\\r3\\w. Radius 16.0\n"
		"\\r4\\w. Radius 32.0\n"
		"\\r5\\w. Radius 48.0\n"
		"\\r6\\w. Radius 64.0\n"
		"\\r7\\w. Radius 80.0\n"
		"\\r8\\w. Radius 96.0\n"
		"\\r9\\w. Radius 128.0\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_WaypointRadiusSelect

	// waypoint add menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_6 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_7 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_8 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_9 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yWaypoint Set Type:\n\n"

		"\\r1\\w. Normal\n"
		"\\r2\\w. \\rTerrorist Important\n"
		"3\\w. \\rCounter-Terrorist Important\n"
		"4\\w. Block with hostage / Ladder\n"
		"\\r5\\w. \\yRescue Zone\n"
		"\\r6\\w. Camping\n"
		"\\r7\\w. Camp End\n"
		"\\r8\\w. \\rMap Goal\n"
		"9\\w. Jump\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_WaypointAdd

	// waypoint flag set menu
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yToggle waypoint Flags:\n\n"

		"\\r1\\w. Block with Hostage\n"
		"\\r2\\w. Terrorists Specific\n"
		"\\r3\\w. CTs Specific\n"
		"\\r4\\w. Use Elevator\n"
		"\\r5\\w. Sniper Point (\\yFor Camp Points Only!\\w)\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_WaypointFlagSet

	// waypoint auto-path maximum distance set
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_4 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_5 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_6 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_7 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yAuto Path Distance:\n\n"

		"\\r1\\w. Distance 0.0\n"
		"\\r2\\w. Distance 100.0\n"
		"\\r3\\w. Distance 130.0\n"
		"\\r4\\w. Distance 160.0\n"
		"\\r5\\w. Distance 190.0\n"
		"\\r6\\w. Distance 220.0\n"
		"\\r7\\w. Distance 250.0 (Default)\n\n"

		"\\r0\\w. Exit"
	},	// MenuID_WaypointAutoPathMaximumDistanceSet

	// waypoint path connections set
	{
		HalfLifeEngine::SDK::Constants::Menu::KeyBit_1 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_2 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_3 | HalfLifeEngine::SDK::Constants::Menu::KeyBit_0,

		"\\yCreate Path (Choose Direction):\n\n"

		"\\r1\\w. Outgoing Path\n"
		"\\r2\\w. Incoming Path\n"
		"\\r3\\w. Bidirectional (Both Ways)\n\n"

		"\\r0\\w. Exit"
	}	// MenuID_WaypointPathConnectionsSet
};
#if 0
MenuManager::MenuManager (void)
{
	// Bot -------------------------------------------------------------------------------------------------------------------------
	{
	m_menus[MenuID_YaPBMain] = new Menu (Localizer::LocalizedStringID_Menu_Bot_Main_Title);

	// Reliability check.
	if (m_menus[MenuID_YaPBMain] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating main YaPB menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_YaPBMain]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Main_Item_Control,    "YaPB menu control");
	m_menus[MenuID_YaPBMain]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Main_Item_Features,   "YaPB menu features");
	m_menus[MenuID_YaPBMain]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Main_Item_FillServer, "YaPB fillServer");
	m_menus[MenuID_YaPBMain]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Main_Item_EndRound,   "YaPB killAll");
	}

	{
	m_menus[MenuID_BotFeatures] = new Menu (Localizer::LocalizedStringID_Menu_Bot_Features_Title);

	// Reliability check.
	if (m_menus[MenuID_BotFeatures] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB features menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotFeatures]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Features_Item_WeaponModeMenu,    "YaPB menu weaponMode");
	m_menus[MenuID_BotFeatures]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Features_Item_WaypointMenu,      "YaPB menu waypoint");
	m_menus[MenuID_BotFeatures]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Features_Item_SelectPersonality, "YaPB menu selectPersonality");

	#if defined _DEBUG
		m_menus[MenuID_BotFeatures]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Features_Item_ToggleDebugMode,   Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <bool> () ? "YaPB debugMode on" : "YaPB debugMode off");
	#endif	// if defined _DEBUG

	m_menus[MenuID_BotFeatures]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Features_Item_CommandMenu,       "YaPB menu command");
	}

	{
	m_menus[MenuID_BotControl] = new Menu (Localizer::LocalizedStringID_Menu_Bot_Control_Title);

	// Reliability check.
	if (m_menus[MenuID_BotControl] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB control menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotControl]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Control_Item_AddBotQuick,     "YaPB menu control");
	m_menus[MenuID_BotControl]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Control_Item_AddBotSpecified, "YaPB menu features");
	m_menus[MenuID_BotControl]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Control_Item_KickRandomBot,   "YaPB fillServer");
	m_menus[MenuID_BotControl]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Control_Item_KickAllBots,     "YaPB killAll");
	m_menus[MenuID_BotControl]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Control_Item_KickBotMenu,     "YaPB killAll");
	}

	{
	m_menus[MenuID_BotWeaponModeSelect] = new Menu (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Title);

	// Reliability check.
	if (m_menus[MenuID_BotWeaponModeSelect] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB weapon mode select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotWeaponModeSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_KnivesOnly,        "YaPB menu weaponMode");
	m_menus[MenuID_BotWeaponModeSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_PistolsOnly,       "YaPB menu waypoint");
	m_menus[MenuID_BotWeaponModeSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_ShotgunsOnly,      "YaPB menu selectPersonality");
	m_menus[MenuID_BotWeaponModeSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_MachineGunsOnly,   "YaPB debugMode on");
	m_menus[MenuID_BotWeaponModeSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_RiflesOnly,        "YaPB menu command");
	m_menus[MenuID_BotWeaponModeSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_SniperWeaponsOnly, "YaPB menu command");
	m_menus[MenuID_BotWeaponModeSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_AllWeapons,        "YaPB menu command");
	}

	{
	m_menus[MenuID_BotPersonalitySelect] = new Menu (Localizer::LocalizedStringID_Menu_Bot_PersonalitySelect_Title);

	// Reliability check.
	if (m_menus[MenuID_BotPersonalitySelect] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB personality select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotPersonalitySelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Normal,     "YaPB menu control");
	m_menus[MenuID_BotPersonalitySelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Aggressive, "YaPB menu features");
	m_menus[MenuID_BotPersonalitySelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Careful,    "YaPB fillServer");
	m_menus[MenuID_BotPersonalitySelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Random,     "YaPB killAll");
	}

	{
	m_menus[MenuID_BotSkillSelect] = new Menu (Localizer::LocalizedStringID_Menu_Bot_SkillSelect_Title);

	// Reliability check.
	if (m_menus[MenuID_BotSkillSelect] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB skill select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotSkillSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_SkillSelect_Item_Stupid,       "YaPB menu weaponMode");
	m_menus[MenuID_BotSkillSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_SkillSelect_Item_Newbie,       "YaPB menu waypoint");
	m_menus[MenuID_BotSkillSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_SkillSelect_Item_Average,      "YaPB menu selectPersonality");
	m_menus[MenuID_BotSkillSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_SkillSelect_Item_Advanced,     "YaPB debugMode on");
	m_menus[MenuID_BotSkillSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_SkillSelect_Item_Professional, "YaPB menu command");
	m_menus[MenuID_BotSkillSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_SkillSelect_Item_Godlike,      "YaPB menu command");
	}

	{
	m_menus[MenuID_BotTeamSelect] = new Menu (Localizer::LocalizedStringID_Menu_Bot_TeamSelect_Title);

	// Reliability check.
	if (m_menus[MenuID_BotTeamSelect] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB team select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotTeamSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_TeamSelect_Item_Terrorist,        "YaPB menu control");
	m_menus[MenuID_BotTeamSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_TeamSelect_Item_CounterTerrorist, "YaPB menu features");
	m_menus[MenuID_BotTeamSelect]->AddItem (Localizer::LocalizedStringID_AutoSelect,                                "YaPB menu features");
	}

	{
	m_menus[MenuID_BotTerroristModelSelect] = new Menu (Localizer::LocalizedStringID_Menu_Bot_ModelSelect_Title);

	// Reliability check.
	if (m_menus[MenuID_BotTerroristModelSelect] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB model select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotTerroristModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Phoenix,  "YaPB menu weaponMode");
	m_menus[MenuID_BotTerroristModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Leet,     "YaPB menu waypoint");
	m_menus[MenuID_BotTerroristModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Arctic,   "YaPB menu selectPersonality");
	m_menus[MenuID_BotTerroristModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Guerilla, "YaPB debugMode on");
	m_menus[MenuID_BotTerroristModelSelect]->AddItem (Localizer::LocalizedStringID_AutoSelect,                                  "YaPB menu command");
	}

	{
	m_menus[MenuID_BotCounterModelSelect] = new Menu (Localizer::LocalizedStringID_Menu_Bot_ModelSelect_Title);

	// Reliability check.
	if (m_menus[MenuID_BotCounterModelSelect] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB model select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotCounterModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_Urban, "YaPB menu control");
	m_menus[MenuID_BotCounterModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GSG9,  "YaPB menu features");
	m_menus[MenuID_BotCounterModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_SAS,   "YaPB fillServer");
	m_menus[MenuID_BotCounterModelSelect]->AddItem (Localizer::LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GIGN,  "YaPB killAll");
	m_menus[MenuID_BotCounterModelSelect]->AddItem (Localizer::LocalizedStringID_AutoSelect,                                      "YaPB killAll");
	}

	{
	m_menus[MenuID_BotKickMenu¹1] = new Menu (Localizer::LocalizedStringID_Menu_Bot_Kick_Title);

	// Reliability check.
	if (m_menus[MenuID_BotKickMenu¹1] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB kick menu!", sizeof (Menu));

		return;
	}
	}

	{
	m_menus[MenuID_BotCommand] = new Menu (Localizer::LocalizedStringID_Menu_Bot_Command);

	// Reliability check.
	if (m_menus[MenuID_BotCommand] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating YaPB command menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_BotCommand]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Command_Item_MakeDoubleJump,   "YaPB menu control");
	m_menus[MenuID_BotCommand]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Command_Item_FinishDoubleJump, "YaPB menu features");
	m_menus[MenuID_BotCommand]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Command_Item_DropC4Bomb,       "YaPB fillServer");
	m_menus[MenuID_BotCommand]->AddItem (Localizer::LocalizedStringID_Menu_Bot_Command_Item_DropWeapon,       "YaPB killAll");
	}

	// Waypoints ------------------------------------------------------------------------------------------------------------------
	{
	m_menus[MenuID_WaypointMain¹1] = new Menu (Localizer::LocalizedStringID_Menu_Waypoint_Main_Title);

	// Reliability check.
	if (m_menus[MenuID_WaypointMain¹1] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating main waypoint menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_ToggleWaypoints,     "YaPB menu weaponMode");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_CacheWaypoint,       "YaPB menu waypoint");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_CreatePath,          "YaPB menu selectPersonality");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_DeletePath,          "YaPB debugMode on");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_AddWaypoint,         "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_DeleteWaypoint,      "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_SetAutopathDistance, "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_SetRadius,           "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_WaypointStats,       "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_ToggleAutoWaypoint,  "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_SetFlags,            "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_SaveWaypoints,       "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_SaveWithoutChecking, "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_LoadWaypoints,       "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_CheckWaypoints,      "YaPB menu command");
	m_menus[MenuID_WaypointMain¹1]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_Main_Item_ToggleNoclipCheat,   "YaPB menu command");
	}

	{
	m_menus[MenuID_WaypointRadiusSelect] = new Menu (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Title);

	// Reliability check.
	if (m_menus[MenuID_WaypointRadiusSelect] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating waypoint radius select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_0,   "YaPB menu control");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_8,   "YaPB menu features");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_16,  "YaPB fillServer");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_32,  "YaPB killAll");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_48,  "YaPB killAll");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_64,  "YaPB killAll");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_80,  "YaPB killAll");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_96,  "YaPB killAll");
	m_menus[MenuID_WaypointRadiusSelect]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_128, "YaPB killAll");
	}

	{
	m_menus[MenuID_WaypointAdd] = new Menu (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Title);

	// Reliability check.
	if (m_menus[MenuID_WaypointAdd] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating waypoint type select menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Normal,                    "YaPB menu weaponMode");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_TerroristImportant,        "YaPB menu waypoint");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CounterTerroristImportant, "YaPB menu selectPersonality");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_BlockWithHostageOrLadder,  "YaPB debugMode on");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_RescueZone,                "YaPB menu command");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Camping,                   "YaPB menu command");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CampEnd,                   "YaPB menu command");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_MapGoal,                   "YaPB menu command");
	m_menus[MenuID_WaypointAdd]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Jump,                      "YaPB menu command");
	}

	{
	m_menus[MenuID_WaypointFlagSet] = new Menu (Localizer::LocalizedStringID_Menu_Waypoint_FlagSet_Title);

	// Reliability check.
	if (m_menus[MenuID_WaypointFlagSet] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating waypoint flag set menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_WaypointFlagSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_FlagSet_Item_BlockWithHostageOrLadder, "YaPB menu control");
	m_menus[MenuID_WaypointFlagSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_FlagSet_Item_TerroristSpecific,        "YaPB menu features");
	m_menus[MenuID_WaypointFlagSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_FlagSet_Item_CounterTerroristSpecific, "YaPB fillServer");
	m_menus[MenuID_WaypointFlagSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_FlagSet_Item_UseElevator,              "YaPB killAll");
	m_menus[MenuID_WaypointFlagSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_FlagSet_Item_SniperPoint,              "YaPB killAll");
	}

	{
	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet] = new Menu (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Title);

	// Reliability check.
	if (m_menus[MenuID_WaypointAutoPathMaximumDistanceSet] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating waypoint auto-path maximum distance set menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_0,   "YaPB menu weaponMode");
	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_100, "YaPB menu waypoint");
	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_130, "YaPB menu selectPersonality");
	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_160, "YaPB debugMode on");
	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_190, "YaPB menu command");
	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_220, "YaPB menu command");
	m_menus[MenuID_WaypointAutoPathMaximumDistanceSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_250, "YaPB menu command");
	}

	{
	m_menus[MenuID_WaypointPathConnectionsSet] = new Menu (Localizer::LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Title);

	// Reliability check.
	if (m_menus[MenuID_WaypointPathConnectionsSet] == NULL)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating waypoint path connections set menu!", sizeof (Menu));

		return;
	}

	m_menus[MenuID_WaypointPathConnectionsSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Outgoing,      "YaPB menu weaponMode");
	m_menus[MenuID_WaypointPathConnectionsSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Incoming,      "YaPB menu waypoint");
	m_menus[MenuID_WaypointPathConnectionsSet]->AddItem (Localizer::LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Bidirectional, "YaPB menu selectPersonality");
	}
}

const char *const Menu::PrepareText (const MenuID_t id, Human *const human, const HalfLifeEngine::SDK::Constants::Menu::PageIndex_t page, unsigned short &keys)
{
	const unsigned char pagesNumber (GetPagesNumber ());

	InternalAssert (page < pagesNumber);

	const unsigned char itemsNumber (GetItemsNumber ());
	DynamicString text;

	keys = 0u;

	if (m_maximumItemsPerPage > 0u && pagesNumber > 1u)
		text.AssignFormat ("%s%s (%u/%u):\n\n", HalfLifeEngine::SDK::Constants::Menu::colors[m_titleColorID], human->GetLocalizer ()->GetLocalizedString (m_titleID).GetData (), page + 1u, pagesNumber);
	else
		text.AssignFormat ("%s%s\n\n", HalfLifeEngine::SDK::Constants::Menu::colors[m_titleColorID], human->GetLocalizer ()->GetLocalizedString (m_titleID).GetData ());

	enum
	{
		Display_Back = BIT (0u),
		Display_Next = BIT (1u)
	};

	unsigned char flags (Display_Back | Display_Next);
	const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t start (page * m_maximumItemsPerPage);
	HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t end (0u);

	if (m_maximumItemsPerPage > 0u)
	{
		if (start + m_maximumItemsPerPage >= itemsNumber)
		{
			end = itemsNumber - 1u;

			flags &= ~Display_Next;
		}
		else
			end = start + m_maximumItemsPerPage - 1u;
	}
	else if ((end = itemsNumber - 1u) > 10u)
		end = 10u;

	if (page == 0u)
		flags &= ~Display_Back;

	Item_t *item (NULL);
	unsigned char option (0u);
	bool enabled (true);
	unsigned char slots (0u);
	unsigned char option_display (0u);

	for (HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t index (start); index <= end; ++index)
	{
		// reset enabled
		enabled = true;
		item = m_items[index];

		if (item->accessLevel > 0u && !human->IsHost () && !human->GetPlayerPointer ()->HasCommandAccessLevel (item->accessLevel))
			enabled = false;

		if (item->handlerFunction != NULL)
		{
			const Item_t::State_t ret ((item->handlerFunction) (human, id, index));

			if (ret != Item_t::State_Ignored)
				enabled = ret == Item_t::State_Enabled;
		}

		if (enabled)
			keys |= BIT (option);

		if ((option_display = ++option) == 10u)
			option_display = 0u;

		if (enabled)
			text.AppendFormat ("%s%u%s. %s\n", HalfLifeEngine::SDK::Constants::Menu::colors[m_itemColorID], option_display, HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_White], human->GetLocalizer ()->GetLocalizedString (item->textID).GetData ());
		else
			text.AppendFormat ("%s%u. %s\n", HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_Grey], option_display, human->GetLocalizer ()->GetLocalizedString (item->textID).GetData ());

		++slots;

		// attach blanks
		if (!item->blanks.IsEmpty ())
			for (unsigned char j (0u); j < item->blanks.GetElementNumber (); ++j)
			{
				if (item->blanks[j].EatNumber ())
					++option;

				text += item->blanks[j].GetDisplay ();
				text += '\n';

				++slots;
			}
	}

	if (m_maximumItemsPerPage > 0u)
	{
		// Pad spaces until we reach the end of the max possible items
		for (/* Empty */; slots < m_maximumItemsPerPage; ++slots)
		{
			text += '\n';

			++option;
		}

		// Make sure there is at least one visual pad
		text += '\n';

		// Don't bother if there is only one page
		if (pagesNumber > 1u)
		{
			if (flags & Display_Back)
			{
				keys |= BIT (option++);

				text.AppendFormat ("%s%u%s. %s\n", HalfLifeEngine::SDK::Constants::Menu::colors[m_itemColorID], option == 10u ? 0u : option, HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_White], human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Item_Previous).GetData ());
			}
			else
			{
				++option;

				text.AppendFormat ("%s%u. %s\n%s", HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_Grey], option == 10u ? 0u : option, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Item_Previous).GetData (), HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_White]);
			}

			if (flags & Display_Next)
			{
				keys |= BIT (option++);

				text.AppendFormat ("%s%u%s. %s\n", HalfLifeEngine::SDK::Constants::Menu::colors[m_itemColorID], option == 10u ? 0u : option, HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_White], human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Item_Next).GetData ());
			}
			else
			{
				++option;

				text.AppendFormat ("%s%u. %s\n%s", HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_Grey], option == 10u ? 0u : option, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Item_Next).GetData (), HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_White]);
			}
		}
		else
			option += 2u;	// Keep padding

		keys |= BIT (option++);

		text.AppendFormat ("%s%u%s. %s\n", HalfLifeEngine::SDK::Constants::Menu::colors[m_itemColorID], option == 10u ? 0u : option, HalfLifeEngine::SDK::Constants::Menu::colors[HalfLifeEngine::SDK::Constants::Menu::ColorID_White], human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_Menu_Item_Exit).GetData ());
	}

	return text;
}

void Menu::Display (const MenuID_t/* id*/, Human *const/* human*/, const HalfLifeEngine::SDK::Constants::Menu::PageIndex_t/* page*/)
{
/*	HalfLifeEngine::SDK::Constants::Menu::KeyBit_t keys;

	human->DisplayMenu (PrepareText (id, human, page, keys), keys);

	// Stops others from hearing menu sounds....
	human->ExecuteCommand ("speak " + m_soundName + '\n');*/
}
#endif	// if 0
#endif	// if 0
/*
void MenuManager::Menu_YaPBMain::HandleSelection (Human *const human, const HalfLifeEngine::SDK::Constants::Menu::ItemIndex_t selection)
{
	switch (selection)
	{
		case HalfLifeEngine::SDK::Constants::Menu::Key_1:
//			fillCommand = false;

			human->DisplayMenu (MenuID_BotControl);

			break;

		case HalfLifeEngine::SDK::Constants::Menu::Key_2:
			human->DisplayMenu (MenuID_BotFeatures);

			break;

		case HalfLifeEngine::SDK::Constants::Menu::Key_3:
//			fillCommand = true;

			human->DisplayMenu (MenuID_BotTeamSelect);

			break;

		case HalfLifeEngine::SDK::Constants::Menu::Key_4:
			g_server->GetYaPBManager ()->KillAll ();

			human->CloseMenu ();	// reset menu display

			break;

		case HalfLifeEngine::SDK::Constants::Menu::Key_0:
			human->CloseMenu ();	// reset menu display

			break;
	}
}*/
void MenuManager::Menu_YaPBMain::Menu_YaPBControl::Item_AddBotQuick::HandleSelection (Human *const human)
{
	// Call this function in base class....
	Item::HandleSelection (human);

	g_server->GetYaPBManager ()->AddBot ();
}

template <typename clientType> void MenuManager::Menu_YaPBMain::Menu_YaPBControl::Menu_YaPBKick::Item_Kick <clientType>::AppendToMenu (Human *const human, DynamicString &menuText, const ItemID_t index) const
{
	// Reliability checks.
	InternalAssert (HasAccess (human));
	InternalAssert (m_client->IsValid ());

	const HalfLifeEngine::SDK::Constants::TeamID_t clientTeam (m_client->GetRealTeam ());

	menuText.AppendFormat ("\n%s%u\\w. %s (%s)", HalfLifeEngine::SDK::Constants::Menu::colors[GetColorID ()], (index + 1u) % ItemID_Total, m_client->GetName ().GetData (), clientTeam == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "\\rT\\w" : clientTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "\\yCT\\w" : clientTeam == HalfLifeEngine::SDK::Constants::TeamID_Spectator ? "SP" : "UN");

	human->m_openedYaPBMenu.keys |= BIT (index);
}
template <typename clientType> void MenuManager::Menu_YaPBMain::Menu_YaPBControl::Menu_YaPBKick::Item_Kick <clientType>::HandleSelection (Human *const human)
{
	// Reliability checks.
	InternalAssert (m_client->IsValid ());

	m_client->Kick ();

	human->m_openedYaPBMenu.keys = HalfLifeEngine::SDK::Constants::Menu::KeyBit_None;	// Reset only keys, menu will automaticly displayed after client will fully distonnected from server....
}

void MenuManager::Menu_YaPBMain::Menu_YaPBControl::Menu_YaPBKick::Draw (Human *const human)
{
	m_pages.RemoveAll ();

	const YaPBManager::YaPBsArray_t::IndexType_t botsCount (g_server->GetYaPBManager ()->GetBotsCount ());

	if (botsCount == 0u)
	{
		human->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Center, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer));

		m_itemPrevious.HandleSelection (human);	// HACK!

		return;
	}

	PageID_t pageIndex (0u);
	ItemID_t itemIndex (0u);

	m_pages.PushBackDefaultElement ();	// Add new page....

	for (YaPBManager::YaPBsArray_t::IndexType_t botIndex (0u); botIndex < botsCount; ++botIndex)
	{
		const bool isLastBot (botIndex + 1u >= botsCount);

		if (itemIndex < ItemID_Total - 3u/* Don't include next, previous and exit items */)
		{
			YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[botIndex]);

			m_itemKickBot[botIndex].SetClient (bot);

			m_pages[pageIndex] += m_itemKickBot + botIndex;

			// Last bot slot?
			if (isLastBot)
			{
				++itemIndex;

				do
				{
					m_pages[pageIndex] += &m_itemEmpty;
				} while (++itemIndex < ItemID_Total - 2u/* Don't include previous and exit items */);

				goto Label_AppendPreviousAndExitItems;
			}
		}
		else
		{
			// Last bot slot?
			if (isLastBot)
			{
				YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[botIndex]);

				m_itemKickBot[botIndex].SetClient (bot);

				m_pages[pageIndex] += m_itemKickBot + botIndex;
			}
			else
				m_pages[pageIndex] += &m_itemNext;

			Label_AppendPreviousAndExitItems:
			{
				m_pages[pageIndex] += &m_itemPrevious;
				m_pages[pageIndex] += &m_itemExit;

				// Reliability check.
				InternalAssert (m_pages[pageIndex].GetElementNumber () == ItemID_Total);

				// Not last bot slot?
				if (!isLastBot)
				{
					++pageIndex;

					// Reliability check.
					InternalAssert (pageIndex == m_pages.GetElementNumber ());

					m_pages.PushBackDefaultElement ();	// Add new page....

					itemIndex = 0u;	// Reset 'itemIndex' value, because new page has been started.
				}

				continue;
			}
		}

		++itemIndex;
	}

	// POST call this function in base class....
	Menu::Draw (human);
}

void MenuManager::Menu_YaPBMain::Menu_YaPBControl::Item_OpenKickBotMenu::HandleSelection (Human *const human)
{
	human->DisplayMenu (MenuID_BotKickMenu¹1);
}