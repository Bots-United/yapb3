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
// BotEconomyManager.cpp
//
// Class: EconomyManager
//
// Description: Bot economy manager.
//
// Version: $ID:$
//

#include <Core.hpp>

unsigned short YaPBManager::EconomyManager::ms_botBuyEconomyTable[TableID_Total] =
{
	1900u,	// If bot's money more than the value specified here he can buy the primary weapon.
	2100u,	// If bot's money more than the value specified here he will not buy SMGs  (MP5, MAC10, TMP, P90, UMP45) (only for CTs) (+ 7/8/9).
	2100u,	// If bot's money more than the value specified here he will not buy SMGs (MP5, MAC10, TMP, P90, UMP45, +SCOUT) (only for Ts) (+ 7/8/9).
	4000u,	// If bot's money more than the value specified here he can buy shotguns (M3, XM1014).
	6000u,	// If bot's money less than the value specified here he cannot buy shotguns (M3, XM1014).
	7000u,	// If bot's money more than the value specified here he can buy AWM, SG550, G3SG1, M249.
	16000u,	// If bot's money less than the value specified here he cannot buy AWM, SG550, G3SG1, M249.
	1200u,	// How much money bot leaves a prostock, at purchase of the primary weapon (only for type of behaviour - Defensive).
	800u,	// How much money bot leaves a prostock, at purchase of the primary weapon (only for type of behaviour - Agressive).
	1000u,	// How much money bot leaves a prostock, at purchase of the primary weapon (only for type of behaviour - Normal).
	3000u	// If bot's money less than the value specified here he cannot buy Riot Shield.
};

void YaPBManager::EconomyManager::LoadBotEconomics (void)
{
	// GENERAL DATA INITIALIZATION

	STDIOFile file;

	// opens file readonly
	if (!OpenConfigurationFile (ECONOMY_MANAGER_CONFIG_FILENAME, "Bot economics configuration file not found (configs/" ECONOMY_MANAGER_CONFIG_FILENAME "). Loading defaults.", file))
		return;

	DynamicString line;
	unsigned int lineIndex (0u);

	// for each line in the file...
	while (file.ReadLine (line, 256u))
	{
		++lineIndex;

		line.Trim ();	// trim all the blanks or linefeeds

		// skip all comment lines or empty lines
		if (IsEmptyOrCommentedFileBufferLine (line))
			continue;

		DynamicArray <DynamicString, unsigned char> pairs (line.Split <unsigned char> ('='));

		if (pairs.GetElementNumber () != 2u)
		{
			AddLogEntry (true, LogLevel_Error, false, "Error in general bot configuration file syntax... Please correct all errors.");

			return;
		}

		// trim all the blanks or linefeeds
		pairs.GetFirstElement ().Trim ();

		if (pairs.GetFirstElement () != "Economics")
		{
			AddLogEntry (true, LogLevel_Error, false, "%s entry in general configuration file, on a line: %u is unknown!", pairs.GetFirstElement ().GetData (), lineIndex);

			continue;
		}

		// trim all the blanks or linefeeds
		pairs.GetLastElement ().Trim ();

		const DynamicArray <DynamicString, unsigned char> splitted (pairs.GetLastElement ().Split <unsigned char> (','));

		if (splitted.GetElementNumber () != TableID_Total)
		{
			AddLogEntry (true, LogLevel_Error, false, "%s entry in general configuration file, on a line: %u is not valid.", pairs.GetFirstElement ().GetData (), lineIndex);

			return;
		}

		for (unsigned char index (TableID_IfMoreThanCanBuyPrimaryWeapon); index < TableID_Total; ++index)
		{
			const unsigned short value (splitted[index].GetValue <unsigned short> ());

			if (value <= HalfLifeEngine::SDK::Constants::PlayerMaximumMoneyAmount)
				ms_botBuyEconomyTable[index] = value;
			else
				AddLogEntry (true, LogLevel_Warning, false, "%s entry in " ECONOMY_MANAGER_CONFIG_FILENAME ", on a line: %u is out of range: %u (minimum: 0, maximum: 16000).", pairs.GetFirstElement ().GetData (), lineIndex, value);
		}

		break;
	}

	AddLogEntry (true, LogLevel_Default, false, "Bot configuration file " ECONOMY_MANAGER_CONFIG_FILENAME " loaded successfully.");
}

void YaPBManager::EconomyManager::CheckTeamEconomics (const HalfLifeEngine::SDK::Constants::TeamID_t team)
{
	// This function decides is players on specified team is able to buy primary weapons by calculating players that have
	// not enough money to buy primary (with economics), and if this result higher than 'teammatesPercent' percent, player is can't buy primary weapons.

	m_economicsGood[team - 1u] = true;	// Reset.

	if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Economics)->GetValue <bool> () || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Deathmatch)->GetValue <int> () == 2 || m_lastWinner == team)
		return;	// Don't check economics while economics is disabled, or if deathmatch mode is activated, or if bot is in winner team, he must buy something!

	const unsigned char teammatesPercent (80u);
	unsigned char teammatesCount (0u), poorTeammatesCount (0u);

	// Start calculating....
	for (YaPBManager::YaPBsArray_t::IndexType_t index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
	{
		const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

		// Is valid bot team?
		if (bot->GetRealTeam () != team)
			continue;

		++teammatesCount;	// Update count of bot teammates.

		if (bot->GetMoney () <= ms_botBuyEconomyTable[TableID_IfMoreThanCanBuyPrimaryWeapon])
			++poorTeammatesCount;	// Update count of poor bot teammates.
	}

	// If 'teammatesPercent' percent of bot team have no enough money to purchase primary weapon....
	if (teammatesCount > 1u && poorTeammatesCount >= teammatesCount * teammatesPercent / 100u)
		m_economicsGood[team - 1u] = false;
}