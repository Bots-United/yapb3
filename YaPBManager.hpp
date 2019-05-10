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
// YaPBManager.hpp
//
// Class: YaPBManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef YAPB_MANAGER_INCLUDED
#define YAPB_MANAGER_INCLUDED

class YaPBManager
{
	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <YaPB *, unsigned char>          YaPBsArray_t;
		typedef DynamicArray <YaPB::Profile/*YaPB **/, unsigned char> YaPBsCreationTableArray_t;

	//
	// Group: Private members.
	//
	private:
		YaPBsArray_t             m_botsArray;			// For quickly YaPB's looping/counting.

		YaPB                    *m_currentThinkingBot;	// bot that's currently thinking

		#if defined _DEBUG
			YaPB                *m_debugBot;			// bot to debug
			float                m_debugBotUpdateTime;
			static const float   ms_debugBotUpdateDelay;
		#endif	// if defined _DEBUG

		#include <BotProfileManager.hpp>

		ProfileManager           m_profileManager;

		#include <BotEconomyManager.hpp>

		EconomyManager           m_economyManager;

		ChatterManager           m_chatterManager;

		static YaPBsCreationTableArray_t ms_creationTable;	// bot creation table (List to store bots, so we can restore them on mapchange, or something else...)
		float                    m_maintainTime;		// time to maintain bot creation quota

		#include <Buy.hpp>

		BuyManager               m_buyManager;
//*! NOT WORK, ORIGINAL CODE DON'T WANT TO INITIALIZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		#include <BotVoiceCodecDLLWrapper.hpp>

		VoiceCodecDLLWrapper     m_voiceCodecDLLWrapper;
//*/
	public:
		struct ConnectedYaPB_t
		{
			YaPB                                      *pointer;
			HalfLifeEngine::SDK::Structures::Client_t *engineClient;
		};
		typedef DynamicArray <ConnectedYaPB_t> ConnectedYaPBsArray_t;
		static ConnectedYaPBsArray_t g_connectedYaPBs;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline YaPBManager (void) :
			m_currentThinkingBot (NULL),

			#if defined _DEBUG
				m_debugBot (NULL),
				m_debugBotUpdateTime (2.0f),
			#endif	// if defined _DEBUG

			m_maintainTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 2.0f)
		{
			// This is a bot manager class constructor.

			YaPB::LoadBotSkillTables ();

			ReconnectBots ();
		}
/*
		inline ~YaPBManager (void)
		{
			// This is a bot manager class destructor.

			MessageBox (NULL, FormatBuffer ("HELLO %u!!!", m_botsArray.GetElementNumber ()), "YaPB Critical Error", MB_ICONASTERISK);

			// Fill bot creation table....
			ChangeLevel (NULL, NULL);
		}
*/
	//
	// Group: Private functions.
	//
	private:
		const bool  CreateBot                        (const DynamicString &name, const unsigned char skill, const Personality_t personality, const HalfLifeEngine::SDK::Constants::TeamID_t team, const unsigned char model);
		const bool  CreateBot                        (YaPB::Profile *const profile);

		void ReconnectBots (void);

		inline void ResetCreationTableAndQuota (void)
		{
			GetCreationTable ().RemoveAll ();	// Reset table of creation.

			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (0u);	// Reset quota console variable.
		}

		inline void CheckConsoleVariables (void) const
		{
			// Check valid range of quota....
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <int> () < 0)
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (0u);

			const unsigned char humanNumber (g_server->GetHumanManager ()->GetHumansCount ());

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_QuotaMode)->GetString ().CompareWithoutCaseCheck ("match") != 0)
			{
				// Quota mode is "fill"....

				// Fill the server....
				const unsigned char avalibleSlots (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutoVacate)->GetValue <bool> () ? HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients () - 1u - (humanNumber + 1u) : HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients () - humanNumber);

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned int> () > avalibleSlots)
					Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (avalibleSlots);

				return;
			}

			// Quota mode is "match"....

			const unsigned char avalibleSlots ((HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients () - humanNumber) / humanNumber);
//			const unsigned int quota (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned int> () * humanNumber);

			// Check valid range of quota....
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned int> () > avalibleSlots)
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (avalibleSlots);

			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutoVacate)->SetValue (false);
		}
		inline void BalanceQuota (void)
		{
			const YaPBsArray_t::IndexType_t botNumber (GetBotsCount ());
			const unsigned char humanNumber (g_server->GetHumanManager ()->GetHumansCount ());

			if (botNumber > Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> ())
				KickRandom ();

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutoVacate)->GetValue <bool> ())
			{
				if (botNumber < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> () && botNumber < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients () - 1u)
					AddBot ();

				if (humanNumber >= HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ())
					KickRandom ();
			}
			else if (botNumber < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> () && botNumber < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ())
				AddBot ();
		}

		void        SelectLeaderForTerrorists        (void) const;
		void        SelectLeaderForCounterTerrorists (void) const;
		inline void SelectLeaderForEachTeam          (void) const
		{
			SelectLeaderForTerrorists ();
			SelectLeaderForCounterTerrorists ();
		}

	//
	// Group: Functions.
	//
	public:
		inline       YaPB *const GetCurrentThinkingBot (void)                                 { return m_currentThinkingBot; }
		inline const YaPB *const GetCurrentThinkingBot (void)                           const { return m_currentThinkingBot; }
		inline void              SetCurrentThinkingBot (YaPB *const currentThinkingBot)       { m_currentThinkingBot = currentThinkingBot; }

		inline       BuyManager          &GetBuyManager          (void)       { return m_buyManager; }
		inline const BuyManager          &GetBuyManager          (void) const { return m_buyManager; }

		inline       ChatterManager &GetChatterManager (void)       { return m_chatterManager; }
		inline const ChatterManager &GetChatterManager (void) const { return m_chatterManager; }

		inline       VoiceCodecDLLWrapper &GetVoiceCodecDLLWrapper (void)       { return m_voiceCodecDLLWrapper; }
		inline const VoiceCodecDLLWrapper &GetVoiceCodecDLLWrapper (void) const { return m_voiceCodecDLLWrapper; }
		inline void ReleaseCodec (void) const
		{
			// Loop through all bots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				if (m_botsArray[index]->GetChatterManager ().IsCodecInitialized ())
					m_botsArray[index]->GetChatterManager ().ReleaseCodec ();
		}
		inline void OnChangeCodec (const unsigned char quality) const
		{
			// Loop through all bots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->GetChatterManager ().OnChangeCodec (quality);
		}

		#if defined _DEBUG
			inline const YaPB *const  GetDebugBot              (void)            const { return m_debugBot; }
			inline  void              SetDebugBot              (YaPB *const bot)       { m_debugBot = bot; }
			inline const float        GetDebugBotUpdateTime    (void)            const { return m_debugBotUpdateTime; }
			static inline const float GetDebugBotUpdateDelay   (void)                  { return ms_debugBotUpdateDelay; }
			inline void               UpdateDebugBotUpdateTime (void)
			{
				// update timer to one second
				m_debugBotUpdateTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + GetDebugBotUpdateDelay ();
			}
		#endif	// if defined _DEBUG

		inline YaPB *const GetBot (const unsigned char index)
		{
			// This function finds a bot specified by index, and then returns pointer to it (using own 'YaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (index)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[index]->GetYaPBPointer ();
		}
		inline const YaPB *const GetBot (const unsigned char index) const
		{
			// This function finds a bot specified by index, and then returns pointer to it (using own 'YaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (index)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[index]->GetYaPBPointer ();
		}
		inline YaPB *const GetBot (const HalfLifeEngine::SDK::Classes::Edict *const bot)
		{
			// This function finds a bot specified by edict, and then returns pointer to it (using own 'YaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (bot)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[bot->GetIndex () - 1u]->GetYaPBPointer ();
		}
		inline const YaPB *const GetBot (const HalfLifeEngine::SDK::Classes::Edict *const bot) const
		{
			// This function finds a bot specified by edict, and then returns pointer to it (using own 'YaPB' class).

			// Reliability check.
			if (!g_server->GetClientManager ()->GetClient (bot)->IsValid ())
				return NULL;	// If no edict, return NULL.

			return g_server->GetClientManager ()->m_clients[bot->GetIndex () - 1u]->GetYaPBPointer ();
		}
		inline YaPB *const GetRandomBot (void) const
		{
			// Reliability check.
			InternalAssert (!m_botsArray.IsEmpty ());

			return m_botsArray.GetRandomElement ();
		}

		inline       YaPBsArray_t &GetYaPBsArray (void)
		{
			// This function returns a reference to non-constant YaPB's array.

			return m_botsArray;
		}
		inline const YaPBsArray_t &GetYaPBsArray (void) const
		{
			// This function returns a reference to constant YaPB's array.

			return m_botsArray;
		}

		inline void MaintainQuota (void)
		{
			// This function gets called each frame and keeps number of bots up to date, and don't allow to maintain bot creation while creation process in process.

			if (m_maintainTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				return;

			if (!GetCreationTable ().IsEmpty ())
			{
				const YaPB::Profile /*&*/last (GetCreationTable ().PopBack ());

				CreateBot (last.name, last.skill, last.personality, last.team, last.model[last.team - 1u]);

				m_maintainTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.2f;
			}
			else	// now keep bot number up to date
			{
				CheckConsoleVariables ();
				BalanceQuota ();

				m_maintainTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.25f;
			}
		}

		static inline YaPBsCreationTableArray_t &GetCreationTable  (void)       { return ms_creationTable; }
		inline        ProfileManager            &GetProfileManager (void)       { return m_profileManager; }
		inline const  ProfileManager            &GetProfileManager (void) const { return m_profileManager; }
		inline        EconomyManager            &GetEconomyManager (void)       { return m_economyManager; }
		inline const  EconomyManager            &GetEconomyManager (void) const { return m_economyManager; }

		inline void AddBot (void)
		{
			AddBot (YaPB::Profile ());
		}
		inline void AddBot (const YaPB::Profile &botProfile)
		{
			// Put to queue.
			GetCreationTable () += botProfile;

			const YaPBsArray_t::IndexType_t botsCount (GetBotsCount () + 1u);

			// keep quota number up to date
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> () < botsCount)
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (botsCount);
		}
		inline void AddBot (const DynamicString &name, const unsigned char skill, const Personality_t personality, const HalfLifeEngine::SDK::Constants::TeamID_t team, const unsigned char model)
		{
			// This function putting bot creation process to queue to prevent engine crashes.

			YaPB::Profile queueID;

			// Fill the holder....
			queueID.name = name;
			queueID.skill = skill;
			queueID.personality = personality;
			queueID.team = team;
			queueID.model[team - 1u] = model;

			AddBot (queueID);
		}
		void AddBot        (const DynamicString &name, const DynamicString &skill, const DynamicString &personality, const DynamicString &team, const DynamicString &model);
		void FillServer    (HalfLifeEngine::SDK::Constants::TeamID_t team, const Personality_t personality = Personality_None, const unsigned char skill = -1, const unsigned char numToAdd = -1);

		inline YaPB *const FindBot (const DynamicString &name)
		{
			// Loop through all bots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (bot->GetName ().CompareWithoutCaseCheck (name) == 0)
					return bot;
			}

			return NULL;
		}
		inline const YaPB *const FindBot (const DynamicString &name) const
		{
			// Loop through all bots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				const YaPB *const bot (m_botsArray[index]);

				if (bot->GetName ().CompareWithoutCaseCheck (name) == 0)
					return bot;
			}

			return NULL;
		}
		inline YaPB *const FindBot (const unsigned char userID)
		{
			// Loop through all bots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*bot) == userID)
					return bot;
			}

			return NULL;
		}
		inline const YaPB *const FindBot (const unsigned char userID) const
		{
			// Loop through all bots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				const YaPB *const bot (m_botsArray[index]);

				if ( HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerUserID (*bot) == userID)
					return bot;
			}

			return NULL;
		}

		inline void ExecuteCommand (const DynamicString &command) const
		{
			// Loop through all alive bots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
					m_botsArray[index]->ExecuteCommand (command);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllBotsHasExecutedCommand) + '\n', command.GetData ());
		}

		inline void SetHealth (const unsigned short value) const
		{
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (bot->IsAlive ())
					bot->GetEdict ()->variables.health = value;
			}

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BotsHealthIsSetTo), value);
		}
		inline void SetMoney (const unsigned short value) const
		{
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (bot->IsAlive ())
					bot->SetMoney (value);
			}

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BotsHealthIsSetTo), value);
		}

		void ListBots (const Human *const human) const;

		inline void CheckAutoVacate (void) const
		{
			// This function sets timer to kick one bot off.

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutoVacate)->GetValue <bool> ())
				KickRandom ();
		}

		inline void KillAll (void) const
		{
			// This function kills all YaPB's on server.

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->Client::Kill ();

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_YaPBs).GetData ());
		}

		inline void KillAll (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function kills all YaPB's on server.

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				const YaPB *const bot (m_botsArray[index]);

				if (bot->GetRealTeam () == team)
					bot->Client::Kill ();
			}

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKilled), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_YaPBs).GetData ());
		}

		inline void KickRandom (void) const
		{
			// This function kicks random YaPB from server.

			if (!m_botsArray.IsEmpty ())
				m_botsArray.GetRandomElement ()->Kick ();
		}
		inline void KickBotNotFromForcedTeam (void)
		{
			const DynamicString forceTeam (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ForceTeam)->GetString ());

			if (forceTeam.IsEmpty ())
			{
				Label_KickRandomBot:
					KickRandom ();

					return;
			}

			// Terrorists team.
			if (forceTeam.CompareWithCaseCheck ('1') == 0 || forceTeam.CompareWithoutCaseCheck ('t') == 0 || forceTeam.CompareWithoutCaseCheck ("terrorist") == 0)
				KickFromTeam (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);
			else if (forceTeam.CompareWithCaseCheck ('2') == 0 || forceTeam.CompareWithoutCaseCheck ("ct") == 0 || forceTeam.CompareWithoutCaseCheck ("counter-terrorist") == 0)	// Counter-Terrorists team.
				KickFromTeam (HalfLifeEngine::SDK::Constants::TeamID_Terrorist);
			else
				goto Label_KickRandomBot;
		}
		inline void KickFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kick random bot from specified team

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (bot->GetRealTeam () != team)
					continue;

				bot->Kick ();

				break;
			}
		}
		inline void KickAllFromTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// this function kicks all bots from specified team

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (bot->GetRealTeam () == team)
					bot->Kick ();
			}
		}

		inline void KickAll (void)
		{
			// This function drops all YaPB's from server.

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->Kick ();	// Delete him/her/it from the list....

			ResetCreationTableAndQuota ();	// Reset table of creation and quota.

			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutoVacate)->SetValue (false);	// Reset auto vacate console variable.

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_AllKickedFromServer), g_localizer.GetLocalizedString (Localizer::LocalizedStringID_YaPBs).GetData ());
		}

		inline const YaPBsArray_t::IndexType_t GetBotsCount (void) const
		{
			// This function returns number of YaPB's playing on the server.

			return m_botsArray.GetElementNumber ();
		}

		inline const YaPBsArray_t::IndexType_t GetBotsCount (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			// This function returns number of YaPB's playing on the given team.

			YaPBsArray_t::IndexType_t botCount (0u);

			// Enumerate all the YaPB's in the server....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				if (m_botsArray[index]->GetRealTeam () == team)
					++botCount;	// This is a YaPB.

			// Return the number of YaPB's.
			return botCount;
		}

		inline YaPB *const GetAliveHighestFragsBot (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			YaPB *aliveHighestFragsBot (NULL);
			float bestScore (-1.0f);

			// Search bots in this team.
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (bot->GetRealTeam () != team || !bot->IsAlive () || bot->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				aliveHighestFragsBot = bot;	// Keep track of the highest frags bot.
				bestScore = bot->GetEdict ()->variables.frags;	// Update best score.
			}

			return aliveHighestFragsBot;
		}
		inline YaPB *const GetHighestFragsBot (const HalfLifeEngine::SDK::Constants::TeamID_t team) const
		{
			YaPB *highestFragsBot (NULL);
			float bestScore (-1.0f);

			// Search bots in this team.
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const bot (m_botsArray[index]);

				if (bot->GetRealTeam () != team || bot->GetEdict ()->variables.frags <= bestScore)
					continue;	// Filter players with given parameters....

				highestFragsBot = bot;	// Keep track of the highest frags bot.
				bestScore = bot->GetEdict ()->variables.frags;	// Update best score.
			}

			return highestFragsBot;
		}

		inline void SwapBotsTeams (void) const
		{
			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				const YaPB *const bot (m_botsArray[index]);

				if (!bot->IsSpectator ())
					bot->ChangeTeam ();
			}
		}

	//
	// Group: Game message handler callbacks.
	//
	public:
		inline void RoundStarted (void)
		{
			// This function is called at the start of each round.

			m_economyManager.RoundStarted ();

			// if deathmatch mode is switched off
			if (g_server->IsTeamplay ())
				SelectLeaderForEachTeam ();	// select a leader bot for this team
		}
		inline void RoundEnded (void)
		{
			// This function is called at the end of each round (i.e. after next messages: Terrorists Win!, Counter-Terrorists Win! or something else....).

			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->RoundEnded ();
		}
		inline void BombPlanted (void)
		{
			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->BombPlanted ();
		}
		inline void BombDetonated (void)
		{
			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->BombDetonated ();
		}
		inline void BombDefused (void)
		{
			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->BombDefused ();
		}
		inline void TerroristsWin (void)
		{
			// This function is called at the end of each round.

			// Update last winner team for economics.
			m_economyManager.SetLastWinnerTeam (HalfLifeEngine::SDK::Constants::TeamID_Terrorist);

			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->TerroristsWin ();

			if (!m_botsArray.IsEmpty () && !(g_server->GetMapType () & (Server::MapType_Ka | Server::MapType_Fy | Server::MapType_Gg | Server::MapType_Aim | Server::MapType_Dm | Server::MapType_Kz | Server::MapType_Awp)) && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 35u && g_server->IsTeamplay () && g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) >= 5u && g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist) >= 5u)
				m_botsArray.GetRandomElement ()->GetYaPBPointer ()->ChatterMessage (g_server->GetRoundInformation ().midTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () ? ::ChatterManager::ChatterMessage_QuicklyWonTheRound : ::ChatterManager::ChatterMessage_WonTheRound);
		}
		inline void CounterTerroristsWin (void)
		{
			// This function is called at the end of each round.

			// Update last winner team for economics.
			m_economyManager.SetLastWinnerTeam (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist);

			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->CounterTerroristsWin ();

			if (!m_botsArray.IsEmpty () && !(g_server->GetMapType () & (Server::MapType_Ka | Server::MapType_Fy | Server::MapType_Gg | Server::MapType_Aim | Server::MapType_Dm | Server::MapType_Kz | Server::MapType_Awp)) && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 35u && g_server->IsTeamplay () && g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_Terrorist) >= 5u && g_server->GetClientManager ()->GetClientsCount (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist) >= 5u)
				m_botsArray.GetRandomElement ()->ChatterMessage (g_server->GetRoundInformation ().midTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () ? ::ChatterManager::ChatterMessage_QuicklyWonTheRound : ::ChatterManager::ChatterMessage_WonTheRound);
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void YaPBConnect (YaPB *const bot)
		{
			m_botsArray += bot;

			const unsigned char botsCount (GetBotsCount ());

			// Balances quota.
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> () < botsCount)
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (botsCount);
		}

		inline void OnClientDisconnect (Client *const client)
		{
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->OnClientDisconnect (client);
		}
		inline void YaPBDisconnect     (YaPB *const bot)
		{
			bot->Disconnect ();	// Tell bot that he is disconnected....

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_YaPBKicked), bot->GetName ().GetData ());

			m_botsArray -= bot;

			// Reliability check.
			InternalAssert (bot != GetCurrentThinkingBot ());
//			if (bot == GetCurrentThinkingBot ())
//				SetCurrentThinkingBot (NULL);
		}

		inline void FreezePeriodExpired (void)
		{
			// Loop through all bot slots....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				m_botsArray[index]->FreezePeriodExpired ();
		}

		inline void ClientKilled (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const killer) const
		{
			// Is called when the one client/bot is kill other client/bot.

			if (killer->IsPlayer () && victim->GetEdict () != killer)
			{
				Client *const killerClient (g_server->GetClientManager ()->GetClient (killer));

				// Loop through all bot slots....
				for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
					if (m_botsArray[index] != victim && m_botsArray[index] != killerClient)
						m_botsArray[index]->ClientKilled (victim, killer);

				if (killerClient->IsYaPB ())
					killerClient->GetYaPBPointer ()->Kill (victim);
			}
			else
			{
				// Loop through all bot slots....
				for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
					if (m_botsArray[index] != victim)
						m_botsArray[index]->ClientKilled (victim, killer);
			}
		}

		inline void PlayerTouched (const HalfLifeEngine::SDK::Classes::Edict *const bot, const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			GetBot (bot)->Touched (entity);
		}

		inline void ReceiveRadioCommand (const Client *const from, const HalfLifeEngine::SDK::Constants::RadioCommand_t radioCommand) const
		{
			// This function gets called when the client 'from' has issued a radio message 'radioCommand'.

			const HalfLifeEngine::SDK::Constants::TeamID_t team (from->GetRealTeam ());

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const to (m_botsArray[index]);

				// if this client is NOT a bot OR this client is dead OR this client is on the different side from who sent this radio message
				if (to->IsAlive () && to->GetRealTeam () == team && to->IsCanHearRadio ())
					to->HearRadioCommand (from, radioCommand);
			}
		}
		inline void ReceiveChatterMessage (const Client *const from, const ::ChatterManager::ChatterMessage_t chatterMessage) const
		{
			// This function gets called when the client 'from' has issued a chatter message 'chatterMessage'.

			const HalfLifeEngine::SDK::Constants::TeamID_t team (from->GetRealTeam ());

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const to (m_botsArray[index]);

				// if this client is NOT a bot OR this client is dead OR this client is on the different side from who sent this chatter message
				if (to->IsAlive () && to->GetRealTeam () == team)
					to->HearChatterMessage (from, chatterMessage);
			}
		}
		inline void ReceiveNoisedSound (const Client *const from, const Client::NoisedSound_t &noisedSound) const
		{
			// This function gets called when the client 'from' has issued a noise sound 'noisedSound'.

			// Reliability check.
			InternalAssert (from->IsAlive ());

			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
			{
				YaPB *const to (m_botsArray[index]);

				// if this client is NOT a bot OR this client is dead OR this client is on the different side from who sent this chatter message
				if (to != from && to->IsAlive () && to->IsCanHearSound (from, noisedSound))
					to->HearNoisedSound (from, noisedSound);
			}
		}

		inline void Think (void)
		{
			MaintainQuota ();	// Keep bot number up to date.
		}

		inline void ChangeLevel (const char *const/* s1*/, const char *const/* s2*/) const
		{
			// Loop through all bots, and save it....
			for (YaPBsArray_t::IndexType_t index (0u); index < m_botsArray.GetElementNumber (); ++index)
				GetCreationTable () += *m_botsArray[index]->GetProfile ();
		}
};

inline void Server::ChangeLevel (const char *const s1, const char *const s2) const
{
	m_YaPBManager->ChangeLevel (s1, s2);
}

inline void Server::FreezePeriodExpired (void)
{
//	HalfLifeEngine::Globals::g_halfLifeEngine->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Server::FreezePeriodExpired(): called!!!");

	m_YaPBManager->FreezePeriodExpired ();
}
inline void Server::BombPlanted (void)
{
	m_c4Manager.BombPlanted ();

	m_YaPBManager->BombPlanted ();
}
inline void Server::BombDetonated (void)
{
	m_YaPBManager->BombDetonated ();
}
inline void Server::BombDefused (void)
{
	m_YaPBManager->BombDefused ();
}

inline void ClientManager::ClientKilled (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const killer)
{
	// This function called after client/bot kill.

	// Reliability checks.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());
	InternalAssert (killer != NULL);

	const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

	// Reliability checks.
	InternalAssert (m_clients[clientIndex]->IsValid ());
	InternalAssert (!m_clients[clientIndex]->IsSpectator ());

	// Update alive clients/bots/humans counts....
	{
		m_teamAliveClients[m_clients[clientIndex]->GetRealArrayTeamID ()] -= m_clients[clientIndex];

		if (m_clients[clientIndex]->IsFakeClient ())
			g_server->GetFakeClientManager ()->ClientKilled (m_clients[clientIndex]->GetFakeClientPointer ());
	}

	// Notice client/bot that has been killed....
	{
		/// @note killer edict - actual killer entity (could be "worldspawn", "trigger_hurt", or something else...)
		m_clients[clientIndex]->Killed (killer);

		g_server->GetYaPBManager ()->ClientKilled (m_clients[clientIndex], killer);
	}
}

inline void ClientManager::UpdateClientData (const HalfLifeEngine::SDK::Classes::Edict *const client) const
{
	// Reliability check.
	InternalAssert (client->IsValid () && client->IsNotWorldspawnPlayer ());

	const unsigned char clientIndex (static_cast <const unsigned char> (client->GetIndex ()) - 1u);

	// Reliability checks.
	InternalAssert (m_clients[clientIndex]->IsValid ());
	InternalAssert (m_clients[clientIndex]->IsHuman ());

	// This function called only for human players....
	m_clients[clientIndex]->GetHumanPointer ()->UpdateData ();
}

inline YaPB::ChatterManager::ChatterManager (void) :
	m_encodeCodec (g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().TryInitializeCodec (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_quality)->GetValue <unsigned char> ())),

	m_bVoiceRecording (false),

	m_pUncompressedFileData (NULL),
	m_nUncompressedDataBytes (0),
	m_pUncompressedDataFilename (NULL),

	m_pDecompressedFileData (NULL),
	m_nDecompressedDataBytes (0),
	m_pDecompressedDataFilename (NULL),

	m_pMicInputFileData (NULL),
	m_nMicInputFileBytes (0u),
	m_CurMicInputFileByte (0),
	m_MicStartTime (0.0),

	m_chatterIconShowTime (0.0f),
	m_chatterIconHideTime (0.0f)
{ /* VOID */ }
inline const bool YaPB::ChatterManager::IsCodecInitialized (void) const { return GetCodec () != NULL && g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized (); }
inline void YaPB::ChatterManager::OnChangeCodec (const unsigned char quality)
{
	// Reliability check....
	InternalAssert (!IsCodecInitialized ());

	m_encodeCodec = g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().TryInitializeCodec (quality);
}
inline void YaPB::ChatterManager::InstantChatterMessage (const YaPB *const bot, ChatterItem_t chatterItem)
{
	const ::ChatterManager::ChatterSound_t &sound (g_server->GetYaPBManager ()->GetChatterManager ().GetChatter (bot->m_profile->chatterBankIndex, chatterItem.index).sounds.GetRandomElement ());

	g_server->GetYaPBManager ()->ReceiveChatterMessage (bot, chatterItem.index);

	bot->StartChatter (g_server->GetYaPBManager ()->GetChatterManager ().GetBank (bot->m_profile->chatterBankIndex).path + sound.name + ".WAV");

	chatterItem.sayTime += sound.duration + g_randomNumberGenerator.GetValueBetween (bot->GetProfile ()->baseReactTime * 0.4f, bot->GetProfile ()->baseReactTime);

	if (!IsChatterIconDisplayed ())
	{
		bot->DisplayChatterIcon ();

		m_chatterIconShowTime = 0.0f;
	}

	// Remember the current chatter icon state.
	m_chatterIconHideTime = chatterItem.sayTime;

//	chatterItem.sayTime += g_server->GetYaPBManager ()->GetChatterManager ().GetChatter (bot->m_profile->chatterBankIndex, chatterItem.index).repeatTime;
}
inline void YaPB::ChatterManager::PushChatterMessage (const ::ChatterManager::BankIndex_t bankIndex, const ::ChatterManager::ChatterMessage_t chatterMessageIndex, const float sayTime)
{
	if (g_server->GetYaPBManager ()->GetChatterManager ().GetChatter (bankIndex, chatterMessageIndex).sounds.IsEmpty ())
		return;

	// Reliability check.
//	InternalAssert (!IsPriorityQueueMember (chatterMessageIndex));	// OCCURS!!!
	#pragma message ("TEMPORAILY UNCOMMENT ME BACK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
//	if (IsPriorityQueueMember (chatterMessageIndex))
		return;

	const ChatterItem_t newItem = {chatterMessageIndex, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + sayTime};

	m_chatterMessagesPriorityQueue += newItem;

	m_chatterIconShowTime = m_chatterMessagesPriorityQueue.GetFirstElement ().sayTime - g_randomNumberGenerator.GetValueBetween (0.1f, 0.9f)/*g_randomNumberGenerator.GetValueBetween (bot->GetProfile ()->baseReactTime * 0.4f, bot->GetProfile ()->baseReactTime)*/;

	if (m_chatterIconShowTime < HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		m_chatterIconShowTime = 0.0f;
}
inline void YaPB::ChatterManager::InstantChatterMessage (const YaPB *const bot, const ::ChatterManager::ChatterMessage_t chatterMessageIndex)
{
	if (g_server->GetYaPBManager ()->GetChatterManager ().GetChatter (bot->m_profile->chatterBankIndex, chatterMessageIndex).sounds.IsEmpty ())
		return;

	// Reliability check.
	InternalAssert (!IsPriorityQueueMember (chatterMessageIndex));

	const ChatterItem_t newItem = {chatterMessageIndex, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()};

	InstantChatterMessage (bot, newItem);
}
inline void YaPB::Kick (void)
{
	// Call this function in base class....
	FakeClient::Kick ();

	const YaPBManager::YaPBsArray_t::IndexType_t botsCount (g_server->GetYaPBManager ()->GetBotsCount () - 1u);

	// Balances quota.
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->GetValue <unsigned char> () > botsCount)
		Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Quota)->SetValue (botsCount);
}
#if defined _DEBUG
	inline const bool YaPB::IsDebuggingBot (void) const
	{
		const Host *const host (g_server->GetHumanManager ()->GetHostClient ());

		// Reliability check.
		InternalAssert (host != NULL);

		if (m_edict->GetIndex () == host->GetObserverTargetEdictIndex ())
		{
			if (g_server->GetYaPBManager ()->GetDebugBot () != NULL && g_server->GetYaPBManager ()->GetDebugBot () != this)
				return false;
		}
		else if (this != g_server->GetYaPBManager ()->GetDebugBot ())
			return false;

		return true;
	}
#endif	// if defined _DEBUG

inline void Client::TryReceiveLastNoisedSound (void)
{
	if (m_lastNoisedSound.timeLasting > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		g_server->GetYaPBManager ()->ReceiveNoisedSound (this, m_lastNoisedSound);
}

#endif	// ifndef YAPB_MANAGER_INCLUDED