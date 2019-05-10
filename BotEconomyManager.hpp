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
// BotEconomyManager.hpp
//
// Class: EconomyManager
//
// Description: YaPB economy manager.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_ECONOMY_MANAGER_INCLUDED
#define BOT_ECONOMY_MANAGER_INCLUDED

class EconomyManager
{
	//
	// Group: Private members.
	//
	private:
		#define ECONOMY_MANAGER_CONFIG_FILENAME                "economics_table"
		#define ECONOMY_MANAGER_CONFIG_FILENAME_EXTANTION      "cfg"
		#define ECONOMY_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ECONOMY_MANAGER_CONFIG_FILENAME "." ECONOMY_MANAGER_CONFIG_FILENAME_EXTANTION

		enum TableID_t
		{
			TableID_IfMoreThanCanBuyPrimaryWeapon,									// If bot's money more than the value specified here he can buy the primary weapon.
			TableID_IfMoreThanCantBuySubMachineGun_CounterTerrorist,				// If bot's money more than the value specified here he will not buy SMGs  (MP5, MAC10, TMP, P90, UMP45) (only for CTs) (+ 7/8/9).
			TableID_IfMoreThanCantBuySubMachineGunAndScout_Terrorist,				// If bot's money more than the value specified here he will not buy SMGs (MP5, MAC10, TMP, P90, UMP45, +SCOUT) (only for Ts) (+ 7/8/9).
			TableID_IfMoreThanCanBuyShotgun,										// If bot's money more than the value specified here he can buy shotguns (M3, XM1014).
			TableID_IfLessThanCantBuyShotgun,										// If bot's money less than the value specified here he cannot buy shotguns (M3, XM1014).
			TableID_IfMoreThanCanBuyAWPorSG550orG3SG1orM249,						// If bot's money more than the value specified here he can buy AWM, SG550, G3SG1, M249.
			TableID_IfLessThanCantBuyAWPorSG550orG3SG1orM249,						// If bot's money less than the value specified here he cannot buy AWM, SG550, G3SG1, M249.
			TableID_MoneyAmountBotKeepProstockAtPurchaseOfPrimaryWeapon_Defensive,	// How much money bot leaves a prostock, at purchase of the primary weapon (only for type of behaviour - Defensive).
			TableID_MoneyAmountBotKeepProstockAtPurchaseOfPrimaryWeapon_Agressive,	// How much money bot leaves a prostock, at purchase of the primary weapon (only for type of behaviour - Agressive).
			TableID_MoneyAmountBotKeepProstockAtPurchaseOfPrimaryWeapon_Normal,		// How much money bot leaves a prostock, at purchase of the primary weapon (only for type of behaviour - Normal).
			TableID_IfLessThanCantBuyShield,										// If bot's money less than the value specified here he cannot buy Riot Shield.

			TableID_Total
		};
		static unsigned short                    ms_botBuyEconomyTable[TableID_Total];

		HalfLifeEngine::SDK::Constants::TeamID_t m_lastWinner;							// the team who won previous round
		bool                                     m_economicsGood[TeamArrayID_Total];	// is team able to buy anything

//		unsigned int                             m_roundCount;							// rounds passed

	//
	// Group: (Con/De)structors.
	//
	public:
		inline EconomyManager (void) : m_lastWinner (HalfLifeEngine::SDK::Constants::TeamID_Unassigned)
		{
			// This is a bot economy manager class constructor.

			for (unsigned char teamIndex (TeamArrayID_Terrorist); teamIndex < TeamArrayID_Total; ++teamIndex)
				m_economicsGood[teamIndex] = true;

			LoadBotEconomics ();
		}

	//
	// Group: Private functions.
	//
	private:
		void        LoadBotEconomics    (void);
		void        CheckTeamEconomics  (const HalfLifeEngine::SDK::Constants::TeamID_t team);
		inline void CheckTeamsEconomics (void)
		{
			// Check team economics.
			CheckTeamEconomics (HalfLifeEngine::SDK::Constants::TeamID_Terrorist);
			CheckTeamEconomics (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);
		}

	//
	// Group: Game message handler callbacks.
	//
	public:
		inline void RoundStarted (void)
		{
			// This function is called at the start of each round.

			// Check teams economics.
			CheckTeamsEconomics ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool                                     IsEconomicsValid  (const HalfLifeEngine::SDK::Constants::TeamID_t team)   const { return m_economicsGood[team - 1u]; }
		inline const HalfLifeEngine::SDK::Constants::TeamID_t GetLastWinnerTeam (void)                                                  const { return m_lastWinner; }
		inline void                                           SetLastWinnerTeam (const HalfLifeEngine::SDK::Constants::TeamID_t winner)       { m_lastWinner = winner; }
};

#endif	// ifndef BOT_ECONOMY_MANAGER_INCLUDED