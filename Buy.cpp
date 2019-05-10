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
// Buy.cpp
//
// Class: BuyManager
//
// Version: $ID:$
//

#include <Core.hpp>

const bool YaPBManager::BuyManager::IsWeaponRestricted (const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID) const
{
	// this function checks for weapon restrictions.

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_RestrictedWeapons)->GetString ().IsEmpty ())
		return false;	// no banned weapons

	const DynamicArray <DynamicString, unsigned char> bannedWeapons (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_RestrictedWeapons)->GetString ().Split <unsigned char> (';'));

	for (unsigned char index (0u); index < bannedWeapons.GetElementNumber (); ++index)
	{
		// check is this weapon is banned
		if (bannedWeapons[index].CompareWithoutCaseCheck (HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].buyAlias1) == 0)
			return true;
	}

	// this function checks restriction set by AMX Mod, this function code is courtesy of KWo.

	// check for weapon restrictions
	if (BIT (weaponID) & (HalfLifeEngine::SDK::Constants::WeaponBits_Primary | HalfLifeEngine::SDK::Constants::WeaponBits_Secondary | HalfLifeEngine::SDK::Constants::WeaponID_Shield))
	{
		const DynamicString restrictedWeapons (HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetString ("amx_restrweapons"));

		if (restrictedWeapons.IsEmpty ())
			return false;

		const char indices[30u] = {4, 25, 20, -1, 8, -1, 12, 19, -1, 5, 6, 13, 23, 17, 18, 1, 2, 21, 9, 24, 7, 16, 10, 22, -1, 3, 15, 14, 0, 11};
		const char index (indices[weaponID - 1u]);	// find the weapon index

		// validate index range
		if (index < 0 || index >= static_cast <const char> (restrictedWeapons.GetElementNumber ()))
			return false;

		return restrictedWeapons[static_cast <const unsigned short> (index)] != '0';
	}

	// else check for equipment restrictions
	const DynamicString restrictedEquipment (HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetString ("amx_restrequipammo"));

	if (restrictedEquipment.IsEmpty ())
		return false;

	const char indices[33u] = {-1, -1, -1, 3, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, -1, -1, -1, -1, 0, 1, 5};
	const char index (indices[weaponID - 1u]);	// find the weapon index

	// validate index range
	if (index < 0 || index >= static_cast <const char> (restrictedEquipment.GetElementNumber ()))
		return false;

	return restrictedEquipment[static_cast <const unsigned short> (index)] != '0';
}

const bool YaPBManager::BuyManager::IsItemRestricted (const HalfLifeEngine::SDK::Constants::RestrictableItem_t &item) const
{
	// this function checks for weapon restrictions.

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_RestrictedWeapons)->GetString ().IsEmpty ())
		return false;	// no banned weapons

	const DynamicArray <DynamicString, unsigned char> bannedWeapons (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_RestrictedWeapons)->GetString ().Split <unsigned char> (';'));

	for (unsigned char index (0u); index < bannedWeapons.GetElementNumber (); ++index)
	{
		// check is this weapon is banned
		if (bannedWeapons[index].CompareWithoutCaseCheck (item.buyAlias) == 0)
			return true;
	}

	// this function checks restriction set by AMX Mod, this function code is courtesy of KWo.

	// else check for equipment restrictions
	const DynamicString restrictedEquipment (HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetString ("amx_restrequipammo"));

	if (restrictedEquipment.IsEmpty ())
		return false;

	return restrictedEquipment[static_cast <const unsigned short> (item.restrictIndex)] != '0';
}

// choose a weapon to buy and returns the weapon ID
// emotion: 0 (careful) - 100 (rusher)
const HalfLifeEngine::SDK::Constants::WeaponID_t YaPBManager::BuyManager::ChoosePrimaryWeapon (const YaPB *const bot)
{
	const unsigned char emotion (g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 100u)/*FIXME*/);
	const unsigned short money (bot->GetMoney ());
	const TeamArrayID_t botRealTeamArrayID (static_cast <const TeamArrayID_t> (bot->GetRealTeam () - 1u));

	const char random (g_randomNumberGenerator.GetValueBetween <char> (0, 100));	// choose a random value
	const char delta (random - emotion);
	const Personality_t personality (abs (delta) > 33 ? delta > 0 ? Personality_Careful : Personality_Rusher : Personality_Normal);	// personality: 0 - normal, 1 - rusher, 2 - careful
	const BuyPriorityArray_t &buyPriority (m_buyPriority[botRealTeamArrayID][personality]);
	BuyPriorityArray_t chosen;	// weapons which has been chosen

	for (BuyPriorityArray_t::IndexType_t index (0u); index < buyPriority.GetElementNumber (); ++index)
	{
		const HalfLifeEngine::SDK::Constants::WeaponID_t id (buyPriority[index]);

		// see if this weapon can be bought

		// are we on an assasssination map?
		if (g_server->GetMapType () & Server::MapType_As)
		{
			// skip certain weapons which is not available on as_* maps
/*			if (id == HalfLifeEngine::SDK::Constants::WeaponID_Scout)
				continue;	// skip this weapon
			else */if (botRealTeamArrayID == TeamArrayID_Terrorist)
			{
				if (id == HalfLifeEngine::SDK::Constants::WeaponID_M3 || id == HalfLifeEngine::SDK::Constants::WeaponID_XM1014 || id == HalfLifeEngine::SDK::Constants::WeaponID_MP5 || id == HalfLifeEngine::SDK::Constants::WeaponID_P90/* || id == HalfLifeEngine::SDK::Constants::WeaponID_SG552 || id == HalfLifeEngine::SDK::Constants::WeaponID_G3SG1*/ || id == HalfLifeEngine::SDK::Constants::WeaponID_M249)
					continue;	// skip this weapon
			}
			else if (id == HalfLifeEngine::SDK::Constants::WeaponID_AWP)
				continue;	// skip this weapon
		}

		// ignore weapon if this weapon is restricted
		if (IsWeaponRestricted (id))
			continue;	// skip this weapon

		// check if this weapon is in our list & if we have enough money (also add some money for ammo)
		if (money < HalfLifeEngine::SDK::Constants::weaponProperties[id].price + 150u || HalfLifeEngine::Utilities::WeaponIsPistol (id))
			continue;	// skip this weapon if not

		// this is a good weapon, add it to our chosen list
		chosen += id;
	}

	if (chosen.IsEmpty ())
		return HalfLifeEngine::SDK::Constants::WeaponID_None;	// no weapon is chosen, can't buy weapon

	float factor (static_cast <float> ((money - 3000.0f) / (HalfLifeEngine::SDK::Constants::PlayerMaximumMoneyAmount - 3000) * 3));

	if (factor < 1.0f)
		factor = 1.0f;

	// select the weapon to buy in the table
	return chosen[static_cast <unsigned char> (static_cast <float> (chosen.GetElementNumber () - 1u) * Math::log10f (g_randomNumberGenerator.GetValueBetween (1.0f, Math::powf (10.0f, factor))) / factor + 0.5f)];
}

// choose a weapon to buy and returns the weapon ID
// emotion: 0 (careful) - 100 (rusher)
const HalfLifeEngine::SDK::Constants::WeaponID_t YaPBManager::BuyManager::ChooseSecondaryWeapon (const YaPB *const bot)
{
	const unsigned char emotion (g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 100u)/*FIXME*/);
	const unsigned short money (bot->GetMoney ());
	const TeamArrayID_t botRealTeamArrayID (static_cast <const TeamArrayID_t> (bot->GetRealTeam () - 1u));

	const char random (g_randomNumberGenerator.GetValueBetween <char> (0, 100));	// choose a random value
	const char delta (random - emotion);
	const Personality_t personality (abs (delta) > 33 ? delta > 0 ? Personality_Careful : Personality_Rusher : Personality_Normal);	// personality: 0 - normal, 1 - rusher, 2 - careful
	const BuyPriorityArray_t &buyPriority (m_buyPriority[botRealTeamArrayID][personality]);
	BuyPriorityArray_t chosen;	// weapons which has been chosen

	for (BuyPriorityArray_t::IndexType_t index (0u); index < buyPriority.GetElementNumber (); ++index)
	{
		const HalfLifeEngine::SDK::Constants::WeaponID_t id (buyPriority[index]);

		// see if this weapon can be bought

		// are we on an assasssination map?
		if (g_server->GetMapType () & Server::MapType_As)
		{
			// skip certain weapons which is not available on as_* maps
/*			if (id == HalfLifeEngine::SDK::Constants::WeaponID_Scout)
				continue;	// skip this weapon
			else */if (botRealTeamArrayID == TeamArrayID_Terrorist)
			{
				if (id == HalfLifeEngine::SDK::Constants::WeaponID_M3 || id == HalfLifeEngine::SDK::Constants::WeaponID_XM1014 || id == HalfLifeEngine::SDK::Constants::WeaponID_MP5 || id == HalfLifeEngine::SDK::Constants::WeaponID_P90/* || id == HalfLifeEngine::SDK::Constants::WeaponID_SG552 || id == HalfLifeEngine::SDK::Constants::WeaponID_G3SG1*/ || id == HalfLifeEngine::SDK::Constants::WeaponID_M249)
					continue;	// skip this weapon
			}
			else if (id == HalfLifeEngine::SDK::Constants::WeaponID_AWP)
				continue;	// skip this weapon
		}

		if (bot->HasShield () && id == HalfLifeEngine::SDK::Constants::WeaponID_Elite)
			continue;	// if we have a shield, we can't buy the dual elites

		// ignore weapon if this weapon is restricted
		if (IsWeaponRestricted (id))
			continue;	// skip this weapon

		// check if this weapon is in our list & if we have enough money (also add some money for ammo)
		if (money < HalfLifeEngine::SDK::Constants::weaponProperties[id].price + 150u || HalfLifeEngine::Utilities::WeaponIsPrimary (id) || id == HalfLifeEngine::SDK::Constants::WeaponID_Shield)
			continue;	// skip this weapon if not

		// this is a good weapon, add it to our chosen list
		chosen += id;
	}

	if (chosen.IsEmpty ())
		return HalfLifeEngine::SDK::Constants::WeaponID_None;	// no weapon is chosen, can't buy weapon

	float factor (static_cast <float> ((money - 3000.0f) / (HalfLifeEngine::SDK::Constants::PlayerMaximumMoneyAmount - 3000) * 3));

	if (factor < 1.0f)
		factor = 1.0f;

	// select the weapon to buy in the table
	return chosen[static_cast <unsigned char> (static_cast <float> (chosen.GetElementNumber () - 1u) * Math::log10f (g_randomNumberGenerator.GetValueBetween (1.0f, Math::powf (10.0f, factor))) / factor + 0.5f)];
}

void YaPBManager::BuyManager::LoadBuyPriority (void)
{
	// load the buy priority configuration file

	STDIOFile file;

	// BUY TABLES INITIALIZATION....

	// try to load the map specific buy configuration file, if the map specific buy configuration file doesn't exist, load the general buy configuration file.
	if (!OpenConfigurationFile (FormatBuffer ("buy_tables(%s)", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ()), FormatBuffer ("Bot buy configuration file for current map not found (configs/buy_tables(%s).cfg). Trying to loading default configuration file....", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ()), file) && !OpenConfigurationFile ("buy_tables", "Bot buy configuration file not found (configs/buy_tables.cfg). Loading defaults....", file))
		return;

	DynamicString line;
	unsigned int lineIndex (0u);
	unsigned char weaponsNumber (0u);

	// for each line in the file...
	while (file.ReadLine (line, 256u))
	{
		++lineIndex;

		line.Trim ();	// trim all the blanks or linefeeds

		// skip all comment lines or empty lines
		if (IsEmptyOrCommentedFileBufferLine (line))
			continue;

		if (line == "[T]")
			weaponsNumber = 16u;
		else if (line == "[CT]")
			weaponsNumber = 17u;	// + shield
		else if (weaponsNumber > 0u)
		{
			DynamicArray <DynamicString, unsigned char> pairs (line.Split <unsigned char> ('='));

			if (pairs.GetElementNumber () != 2u)
			{
				AddLogEntry (true, LogLevel_Error, false, "Weapons buy tables configuration file is not valid on a line: %u.... Please correct all errors.", lineIndex);

				return;
			}

			// trim all the blanks or linefeeds
			pairs.GetFirstElement ().Trim ();

			if (pairs.GetFirstElement () == "PersonalityNormal")
			{
				// trim all the blanks or linefeeds
				pairs.GetLastElement ().Trim ();

				// just to be more unique :)
				pairs.GetLastElement ().TrimLeft ("{(");
				pairs.GetLastElement ().TrimRight (';');
				pairs.GetLastElement ().TrimRight ("})");

				DynamicArray <DynamicString, unsigned char> splitted (pairs.GetLastElement ().Split <unsigned char> (','));
				BuyPriorityArray_t buyPriority;

				for (unsigned char index (0u); index < splitted.GetElementNumber (); ++index)
				{
					// trim all the blanks or linefeeds
					splitted[index].Trim ();

					const HalfLifeEngine::SDK::Constants::WeaponID_t weapon (HalfLifeEngine::Utilities::GetWeaponIDFromAlias (splitted[index]));
					const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID (weapon == HalfLifeEngine::SDK::Constants::WeaponID_None ? splitted[index].GetValue <HalfLifeEngine::SDK::Constants::WeaponID_t> () : weapon);

					// Reliability check.
					if (weaponID < HalfLifeEngine::SDK::Constants::WeaponID_P228 || weaponID > HalfLifeEngine::SDK::Constants::WeaponID_P90 || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_ExplosiveGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_C4 || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_SmokeGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_FlashBangGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_Knife)
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapon \"%s\" in buy_tables.cfg, on a line: %u, is unknown or not needed to put this weapon into this array!", splitted[index].GetData (), lineIndex);

						continue;
					}

					// Reliability check.
					if (buyPriority.IsMember (weaponID))
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapon \"%s\" in buy_tables.cfg, on a line: %u, is already places in the array!", splitted[index].GetData (), lineIndex);

						continue;
					}

					buyPriority += weaponID;
				}

				if (buyPriority.GetElementNumber () != weaponsNumber)
				{
					AddLogEntry (true, LogLevel_Error, false, "Error in general bot configuration file syntax... Please correct all errors.");

					continue;
				}

				for (unsigned char index (0u); index < weaponsNumber; ++index)
					m_buyPriority[weaponsNumber - 16u][Personality_Normal][index] = buyPriority[index];	// set personality weapon pointers here
			}
			else if (pairs.GetFirstElement () == "PersonalityRusher")
			{
				// trim all the blanks or linefeeds
				pairs.GetLastElement ().Trim ();

				// just to be more unique :)
				pairs.GetLastElement ().TrimLeft ("{(");
				pairs.GetLastElement ().TrimRight (';');
				pairs.GetLastElement ().TrimRight ("})");

				DynamicArray <DynamicString, unsigned char> splitted (pairs.GetLastElement ().Split <unsigned char> (','));
				BuyPriorityArray_t buyPriority;

				for (unsigned char index (0u); index < splitted.GetElementNumber (); ++index)
				{
					// trim all the blanks or linefeeds
					splitted[index].Trim ();

					const HalfLifeEngine::SDK::Constants::WeaponID_t weapon (HalfLifeEngine::Utilities::GetWeaponIDFromAlias (splitted[index]));
					const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID (weapon == HalfLifeEngine::SDK::Constants::WeaponID_None ? splitted[index].GetValue <HalfLifeEngine::SDK::Constants::WeaponID_t> () : weapon);

					// Reliability check.
					if (weaponID < HalfLifeEngine::SDK::Constants::WeaponID_P228 || weaponID > HalfLifeEngine::SDK::Constants::WeaponID_P90 || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_ExplosiveGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_C4 || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_SmokeGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_FlashBangGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_Knife)
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapon \"%s\" in buy_tables.cfg, on a line: %u, is unknown or not needed to put this weapon into this array!", splitted[index].GetData (), lineIndex);

						continue;
					}

					// Reliability check.
					if (buyPriority.IsMember (weaponID))
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapon \"%s\" in buy_tables.cfg, on a line: %u, is already places in the array!", splitted[index].GetData (), lineIndex);

						continue;
					}

					buyPriority += weaponID;
				}

				if (buyPriority.GetElementNumber () != weaponsNumber)
				{
					AddLogEntry (true, LogLevel_Error, false, "Error in general bot configuration file syntax... Please correct all errors.");

					continue;
				}

				for (unsigned char index (0u); index < weaponsNumber; ++index)
					m_buyPriority[weaponsNumber - 16u][Personality_Rusher][index] = buyPriority[index];	// set personality weapon pointers here
			}
			else if (pairs.GetFirstElement () == "PersonalityCareful")
			{
				// trim all the blanks or linefeeds
				pairs.GetLastElement ().Trim ();

				// just to be more unique :)
				pairs.GetLastElement ().TrimLeft ("{(");
				pairs.GetLastElement ().TrimRight (';');
				pairs.GetLastElement ().TrimRight ("})");

				DynamicArray <DynamicString, unsigned char> splitted (pairs.GetLastElement ().Split <unsigned char> (','));
				BuyPriorityArray_t buyPriority;

				for (unsigned char index (0u); index < splitted.GetElementNumber (); ++index)
				{
					// trim all the blanks or linefeeds
					splitted[index].Trim ();

					const HalfLifeEngine::SDK::Constants::WeaponID_t weapon (HalfLifeEngine::Utilities::GetWeaponIDFromAlias (splitted[index]));
					const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID (weapon == HalfLifeEngine::SDK::Constants::WeaponID_None ? splitted[index].GetValue <HalfLifeEngine::SDK::Constants::WeaponID_t> () : weapon);

					// Reliability check.
					if (weaponID < HalfLifeEngine::SDK::Constants::WeaponID_P228 || weaponID > HalfLifeEngine::SDK::Constants::WeaponID_P90 || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_ExplosiveGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_C4 || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_SmokeGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_FlashBangGrenade || weaponID == HalfLifeEngine::SDK::Constants::WeaponID_Knife)
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapon \"%s\" in buy_tables.cfg, on a line: %u, is unknown or not needed to put this weapon into this array!", splitted[index].GetData (), lineIndex);

						continue;
					}

					// Reliability check.
					if (buyPriority.IsMember (weaponID))
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapon \"%s\" in buy_tables.cfg, on a line: %u, is already places in the array!", splitted[index].GetData (), lineIndex);

						continue;
					}

					buyPriority += weaponID;
				}

				if (buyPriority.GetElementNumber () != weaponsNumber)
				{
					AddLogEntry (true, LogLevel_Error, false, "Error in general bot configuration file syntax... Please correct all errors.");

					continue;
				}

				for (unsigned char index (0u); index < weaponsNumber; ++index)
					m_buyPriority[weaponsNumber - 16u][Personality_Careful][index] = buyPriority[index];	// set personality weapon pointers here
			}
			else
				AddLogEntry (true, LogLevel_Error, false, "%s entry in buy_tables.cfg, on a line: %u is unknown!", pairs.GetFirstElement ().GetData (), lineIndex);
		}
		else
			AddLogEntry (true, LogLevel_Error, false, "%s entry in buy_tables.cfg, on a line: %u is unknown!", line.GetData (), lineIndex);
	}

	AddLogEntry (true, LogLevel_Default, false, "Bot configuration file buy_tables.cfg loaded successfully.");
}