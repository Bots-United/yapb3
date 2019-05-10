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
// ChatterManager.hpp
//
// Class: ChatterManager
//
// Description: YaPB chatter manager.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CHATTER_MANAGER_INCLUDED
#define CHATTER_MANAGER_INCLUDED

class ChatterManager
{
	//
	// Group: Constants.
	//
	public:
		#define CHATTER_MANAGER_CONFIG_FILENAME                "chatter"
		#define CHATTER_MANAGER_CONFIG_FILENAME_EXTANTION      "cfg"
		#define CHATTER_MANAGER_CONFIG_FILENAME_WITH_EXTENTION CHATTER_MANAGER_CONFIG_FILENAME "." CHATTER_MANAGER_CONFIG_FILENAME_EXTANTION

		// Voice system. (extending enumeration above, messages 0 - HalfLifeEngine::SDK::Constants::RadioCommand_Total is reserved by radio commands)
		enum ChatterMessage_t
		{
			ChatterMessage_SpotTheBomber = HalfLifeEngine::SDK::Constants::RadioCommand_Total,
			ChatterMessage_FriendlyFire,
			ChatterMessage_Suicide,
			ChatterMessage_DiePain,
			ChatterMessage_GotBlinded,
			ChatterMessage_GoingToPlantBomb,
			ChatterMessage_RescuingHostages,
			ChatterMessage_GoingToCamp,
			ChatterMessage_HearSomething,
			ChatterMessage_TeamKill,
			ChatterMessage_Camp,
			ChatterMessage_PlantingC4,
			ChatterMessage_DefusingC4,
			ChatterMessage_InCombat,
			ChatterMessage_SeeksEnemy,
			ChatterMessage_Nothing,
			ChatterMessage_UseHostage,
			ChatterMessage_FoundC4,
			ChatterMessage_WonTheRound,
			ChatterMessage_ScaredEmotion,
			ChatterMessage_HeardEnemy,
			ChatterMessage_SniperWarning,
			ChatterMessage_SniperKilled,
			ChatterMessage_VIPSpotted,
			ChatterMessage_GuardingVipSafety,
			ChatterMessage_GoingToGuardVIPSafety,
			ChatterMessage_QuicklyWonTheRound,
			ChatterMessage_OneEnemyLeft,
			ChatterMessage_TwoEnemiesLeft,
			ChatterMessage_ThreeEnemiesLeft,
			ChatterMessage_NoEnemiesLeft,
			ChatterMessage_FoundBombPlace,
			ChatterMessage_WhereIsTheBomb,
			ChatterMessage_DefendingBombSite,
			ChatterMessage_BarelyDefused,
			ChatterMessage_NiceShotCommander,
			ChatterMessage_NiceShotPall,
			ChatterMessage_GoingToGuardHostages,
			ChatterMessage_GoingToGuardDoppedBomb,
			ChatterMessage_OnMyWay,
			ChatterMessage_FollowingCommander,
			ChatterMessage_PinnedDown,
			ChatterMessage_BombTickingDown,
			ChatterMessage_CommanderDown,
			ChatterMessage_LetsDoThis,
			ChatterMessage_BombSiteSecure,
			ChatterMessage_CoveringFriend,
			ChatterMessage_AgreeWithPlan,
			ChatterMessage_Agree,
			ChatterMessage_Disagree,
			ChatterMessage_GuardingDoppedBomb,
			ChatterMessage_OnARollBrag,
			ChatterMessage_SpottedTwoEnemies,
			ChatterMessage_SpottedThreeEnemies,
			ChatterMessage_SpottedTooManyEnemies,

			ChatterMessage_Total	// Total of bot chatter messages.
		};

		#include <Chatter.hpp>

		DECLARE_TYPED_ENUMERATION (unsigned char, BankIndex_t)
		{
			BankIndex_Default,

			BankIndex_Total = HalfLifeEngine::SDK::Constants::MaximumClients
		};
		struct Bank_t
		{
			DynamicString path;								// Path to chatter bank directory.
			Chatter_t     chatter[ChatterMessage_Total];	// Array of known bot chatter messages, loaded from file.
		};
		typedef DynamicArray <Bank_t, BankIndex_t> BanksArray_t;

	//
	// Group: Private members.
	//
	private:
		BanksArray_t m_banks;

		float        m_lastChatterTime[TeamArrayID_Total];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ChatterManager (void)
		{
			// Load bot chatter from the disk....

			// Initialize the last chatter time array....
			for (unsigned char teamIndex (0u); teamIndex < TeamArrayID_Total; ++teamIndex)
				m_lastChatterTime[teamIndex] = 0.0f;

			LoadChatterBank ();
		}

	//
	// Group: Private functions.
	//
	private:
		void ParseVoiceEvent (const BankIndex_t bankIndex, const ChatterMessage_t type, const DynamicString &line, const float timeToRepeat);

	//
	// Group: Functions.
	//
	public:
		void                    LoadChatterBank    (void);
		inline void             FreeChatter        (void)
		{
			// Free all the memory allocated for bot chatter.

			m_banks.RemoveAll ();
		}

		void                    PrintChatter       (const BankIndex_t bankIndex) const;

		inline const Bank_t    &GetBank            (const BankIndex_t bankIndex)                                             const { return m_banks[bankIndex]; }
		inline const Chatter_t &GetChatter         (const BankIndex_t bankIndex, const ChatterMessage_t chatterMessageIndex) const { return m_banks[bankIndex].chatter[chatterMessageIndex]; }
		inline const float      GetLastChatterTime (const HalfLifeEngine::SDK::Constants::TeamID_t team)                     const { return m_lastChatterTime[team]; }
};

#endif	// ifndef CHATTER_MANAGER_INCLUDED