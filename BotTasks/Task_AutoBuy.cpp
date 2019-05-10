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
// Task_AutoBuy.cpp
//
// Class: Task_AutoBuy
//
// Version: $ID:$
//

#include <Core.hpp>

const bool YaPB::Task_AutoBuy::IsMorePowerfulWeaponCanBeBought (void) const
{
	// this function determines currently owned primary weapon, and checks if bot has enough money to buy more powerful weapon.

	// if bot is not rich enough OR non-standard weapon mode enabled return false
	if (/*g_weaponSelect[25].teamStandard != 1 || */m_owner->GetMoney () < 4000u || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_RestrictedWeapons)->GetString ().IsEmpty ())
		return false;

	// also check if bot has really bad weapon, maybe it's time to change it
	if (m_owner->m_currentWeapon->IsBadPrimary ())
		return true;

	const DynamicArray <DynamicString, unsigned char> bannedWeapons (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_RestrictedWeapons)->GetString ().Split <unsigned char> (';'));

	for (unsigned char index (0u); index < bannedWeapons.GetElementNumber (); ++index)
	{
		// check if it's banned
		if (m_owner->m_currentWeapon->GetID () == HalfLifeEngine::Utilities::GetWeaponIDFromAlias (bannedWeapons[index]))
			return true;
	}

	return (m_owner->m_currentWeapon->GetID () == HalfLifeEngine::SDK::Constants::WeaponID_Scout && m_owner->GetMoney () > 5000u) || (m_owner->m_currentWeapon->GetID () == HalfLifeEngine::SDK::Constants::WeaponID_MP5 && m_owner->GetMoney () > 6000u) || (m_owner->m_currentWeapon->IsShotgun () && m_owner->GetMoney () > 4000u);
}

void YaPB::Task_AutoBuy::Execute (void)
{
	if (m_owner->m_profile->skill >= BotSkillLevel_Advanced)
		m_owner->m_navigator->Think ();	// Do move....

	// Bot left buy zone? (Reliability check)
	if (!m_owner->IsInBuyZone ())
	{
		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Is leave buy zone - buy task complete!", m_owner->GetName ().GetData ());

		Complete ();

		return;
	}

	// fight the enemy if we found one
	if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
	{
		// Reliability check.
		InternalAssert (m_owner->m_enemy->IsValid ());

		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Find enemy \"%s\", buying fast!", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

		if (m_owner->m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Enemy)
			m_owner->m_aim->SetLookTask (new Aim::Task_Enemy ());

		m_owner->m_aim->Think ();	// Do look!

		m_nextBuyTime = 0.0f;	// Do fast buying - "bot pressed 'F1' button" :)
	}

	// do all the jobs of buying weapons
/*
	if (m_delayTime == 0.0f)
		m_delayTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (m_owner->m_profile->baseReactTime + 1.0f, m_owner->m_profile->baseReactTime + 4.0f);
	else if (m_delayTime < HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		m_owner->ExecuteCommand ("m4a1");
//		engine->SetFakeClientConVarValue (m_owner->m_edict, "cl_autobuy", "m4a1 ak47 famas galil p90 mp5 primammo secammo defuser vesthelm vest");
//		helpers->ClientCommand (m_owner->m_edict, "autobuy\n");

		Complete ();
	}
*/
	// if we haven't reached the time to buy, don't proceed
	if (m_nextBuyTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		return;	// run again at next think cycle

	const HalfLifeEngine::SDK::Constants::TeamID_t botRealTeam (m_owner->GetRealTeam ());

	switch (m_buyState)
	{
		// if bot doesn't have a primary weapon, buy one
		case BuyState_PrimaryWeapon:
		{
			if ((m_owner->HasPrimaryWeapon () || m_owner->HasShield ()) || (!g_server->GetYaPBManager ()->GetEconomyManager ().IsEconomicsValid (botRealTeam) && !IsMorePowerfulWeaponCanBeBought ()))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Already have a primary weapon; exiting.", m_owner->GetName ().GetData ());

				break;	// don't proceed if we already have one
			}

			// TODO: emotion
			// select a primary weapon
			const HalfLifeEngine::SDK::Constants::WeaponID_t id (g_server->GetYaPBManager ()->GetBuyManager ().ChoosePrimaryWeapon (m_owner));

			if (id != HalfLifeEngine::SDK::Constants::WeaponID_None)
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying weapon: %s.", m_owner->GetName ().GetData (), HalfLifeEngine::SDK::Constants::weaponProperties[id].buyAlias1.GetData ());

				m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::weaponProperties[id].buyAlias1);
			}
			else
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): No primary weapon is selected.", m_owner->GetName ().GetData ());

			break;
		}

		// if bot is rich, buy armor + helmet; else buy armor
		case BuyState_Armor:
			if (!g_server->GetYaPBManager ()->GetEconomyManager ().IsEconomicsValid (botRealTeam) || !m_owner->HasPrimaryWeapon ())
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Don't have a primary weapon or economics invalid; exiting.", m_owner->GetName ().GetData ());

				break;	// don't proceed if economics invalid
			}

			if (m_owner->GetMoney () >= HalfLifeEngine::SDK::Constants::armorHelm.price + g_randomNumberGenerator.GetValueBetween <unsigned short> (2000u, 4000u) && m_owner->m_edict->variables.armorValue < 40.0f)
			{
				if (!g_server->GetYaPBManager ()->GetBuyManager ().IsItemRestricted (HalfLifeEngine::SDK::Constants::armorHelm))
				{
					AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying armor + helmet.", m_owner->GetName ().GetData ());

					m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::armorHelm.buyAlias);
				}
			}
			else if (m_owner->GetMoney () > HalfLifeEngine::SDK::Constants::armor.price && !g_server->GetYaPBManager ()->GetBuyManager ().IsItemRestricted (HalfLifeEngine::SDK::Constants::armor))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying armor.", m_owner->GetName ().GetData ());

				m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::armor.buyAlias);
			}

			break;

		// if bot doesn't have a secondary weapon, buy one
		case BuyState_SecondaryWeapon:
		{
			if (m_owner->HasWeapons (HalfLifeEngine::SDK::Constants::WeaponBit_P228 | HalfLifeEngine::SDK::Constants::WeaponBit_Deagle | HalfLifeEngine::SDK::Constants::WeaponBit_FiveSeven | HalfLifeEngine::SDK::Constants::WeaponBit_Elite) || m_owner->GetMoney () < g_randomNumberGenerator.GetValueBetween <unsigned int> (7500u, 9000u))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Already have a secondary weapon; exiting.", m_owner->GetName ().GetData ());

				break;	// don't proceed if we already have a good secondary weapon
			}

			// TODO: emotion
			// select a primary weapon
			const HalfLifeEngine::SDK::Constants::WeaponID_t id (g_server->GetYaPBManager ()->GetBuyManager ().ChooseSecondaryWeapon (m_owner));

			if (id != HalfLifeEngine::SDK::Constants::WeaponID_None)
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying weapon: %s.", m_owner->GetName ().GetData (), HalfLifeEngine::SDK::Constants::weaponProperties[id].buyAlias1.GetData ());

				m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::weaponProperties[id].buyAlias1);
			}
			else
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): No secondary weapon is selected.", m_owner->GetName ().GetData ());

			break;
		}

		// buy grenades.
		case BuyState_Grenades:
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ExplosiveGrenadeUsePercent)->GetValue <unsigned char> () && m_owner->GetMoney () >= HalfLifeEngine::SDK::Constants::weaponProperties[HalfLifeEngine::SDK::Constants::WeaponID_ExplosiveGrenade].price && !m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_ExplosiveGrenade))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying HE Grenade.", m_owner->GetName ().GetData ());

				// Buy a HE Grenade
				m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::weaponProperties[HalfLifeEngine::SDK::Constants::WeaponID_ExplosiveGrenade].buyAlias1);	// use alias in CS 1.6
			}

			for (unsigned char number (0u); number < HalfLifeEngine::SDK::Constants::weaponProperties[HalfLifeEngine::SDK::Constants::WeaponID_FlashBangGrenade].ammoMaximumAmount; ++number)
				if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_FlashBangGrenadeUsePercent)->GetValue <unsigned char> () && m_owner->GetMoney () >= HalfLifeEngine::SDK::Constants::weaponProperties[HalfLifeEngine::SDK::Constants::WeaponID_FlashBangGrenade].price)
				{
					AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying Concussion Grenade.", m_owner->GetName ().GetData ());

					// Buy a Concussion Grenade, i.e., 'FlashBang'
					m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::weaponProperties[HalfLifeEngine::SDK::Constants::WeaponID_FlashBangGrenade].buyAlias1);	// use alias in CS 1.6
				}

			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SmokeGrenadeUsePercent)->GetValue <unsigned char> () && m_owner->GetMoney () >= HalfLifeEngine::SDK::Constants::weaponProperties[HalfLifeEngine::SDK::Constants::WeaponID_SmokeGrenade].price && !m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_SmokeGrenade))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying Smoke Grenade.", m_owner->GetName ().GetData ());

				// Buy a Smoke Grenade
				m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::weaponProperties[HalfLifeEngine::SDK::Constants::WeaponID_SmokeGrenade].buyAlias1);	// use alias in CS 1.6
			}

			break;

		// if bot is CT and we're on a bomb map, randomly buy the defuse kit
		case BuyState_Defuser:
			if ((g_server->GetMapType () & Server::MapType_De) && botRealTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 80u && m_owner->GetMoney () > HalfLifeEngine::SDK::Constants::defuseKit.price && !g_server->GetYaPBManager ()->GetBuyManager ().IsItemRestricted (HalfLifeEngine::SDK::Constants::defuseKit))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying defuser.", m_owner->GetName ().GetData ());

				m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::defuseKit.buyAlias);	// use alias in CS 1.6
			}

			break;

		// buy enough primary & secondary ammo (do not check for money here)
		case BuyState_Ammo:
			AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying ammo.", m_owner->GetName ().GetData ());

			if (!g_server->GetYaPBManager ()->GetBuyManager ().IsItemRestricted (HalfLifeEngine::SDK::Constants::primaryAmmo))
			{
				if (!g_server->GetYaPBManager ()->GetBuyManager ().IsItemRestricted (HalfLifeEngine::SDK::Constants::secondaryAmmo))
				{
					// buy enough primary secondary and ammo, simulate human
					const unsigned char numbers (g_randomNumberGenerator.GetValueBetween <unsigned char> (2u, 4u));

					for (unsigned char number (0u); number < numbers; ++number)
						m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::primaryAmmo.buyAlias);	// prim. ammo
						m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::secondaryAmmo.buyAlias);	// sec. ammo

					// Try to reload primary weapon
					m_owner->m_reloadState = ReloadState_Primary;

					m_owner->TryCheckReload ();
				}
				else
				{
					// buy enough primary ammo, simulate human
					const unsigned char numbers (g_randomNumberGenerator.GetValueBetween <unsigned char> (2u, 4u));

					for (unsigned char number (0u); number < numbers; ++number)
						m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::primaryAmmo.buyAlias);	// prim. ammo

					// Try to reload primary weapon
					m_owner->m_reloadState = ReloadState_Primary;

					m_owner->TryCheckReload ();
				}
			}
			else if (!g_server->GetYaPBManager ()->GetBuyManager ().IsItemRestricted (HalfLifeEngine::SDK::Constants::secondaryAmmo))
			{
				// buy enough secondary ammo, simulate human
				const unsigned char numbers (g_randomNumberGenerator.GetValueBetween <unsigned char> (2u, 4u));

				for (unsigned char number (0u); number < numbers; ++number)
					m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::secondaryAmmo.buyAlias);	// sec. ammo

				// Try to reload secondary weapon
				m_owner->m_reloadState = ReloadState_Secondary;

				m_owner->TryCheckReload ();
			}

			break;

		// if the map is dark, the bot needs a night vision goggles...
		case BuyState_NightVisionGoggles:
			if (m_owner->m_profile->skill < BotSkillLevel_Godlike && g_skyLightLevel < 50u && m_owner->GetMoney () > HalfLifeEngine::SDK::Constants::nightVision.price + 5000u && !g_server->GetYaPBManager ()->GetBuyManager ().IsItemRestricted (HalfLifeEngine::SDK::Constants::nightVision))
			{
				AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying night vision goggles.", m_owner->GetName ().GetData ());

				m_owner->ExecuteCommand (HalfLifeEngine::SDK::Constants::nightVision.buyAlias);	// use alias in CS 1.6
			}

			m_owner->m_buyingFinished = true;

			// buying finished
			AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AutoBuy::Execute(): Buying complete.", m_owner->GetName ().GetData ());

			Complete ();	// no more buying

			return;

		#if defined _DEBUG
			// Error!
			default:
				AddLogEntry (false, LogLevel_Critical, false, "\"%s\"::Task_AutoBuy::Execute(): Value of 'm_buyState' is out of range! (%u, max=%u).", m_owner->GetName ().GetData (), m_buyState, BuyState_NightVisionGoggles);
		#endif	// if defined _DEBUG
	}

	++m_buyState;	// add the buying counter

	m_nextBuyTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_owner->m_profile->baseReactTime * 0.2f;	// add delay
}