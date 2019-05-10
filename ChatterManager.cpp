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
// ChatterManager.cpp
//
// Class: ChatterManager
//
// Description: YaPB chatter manager.
//
// Version: $ID:$
//

#include <Core.hpp>

void ChatterManager::ParseVoiceEvent (const BankIndex_t bankIndex, const ChatterMessage_t type, const DynamicString &line, const float timeToRepeat)
{
	// this function does common work of parsing single line of voice chatter.

	DynamicArray <DynamicString, unsigned char> temp (line.Split <unsigned char> (','));
	ChatterSound_t chatterSound;
	const DynamicString path (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ChatterPath)->GetString () + '/');

	m_banks[bankIndex].chatter[type].repeatTime = timeToRepeat;

	for (unsigned char index (0u); index < temp.GetElementNumber (); ++index)
	{
		temp[index].Trim ().TrimQuotes ();

		chatterSound.duration = GetWaveSoundPlayDuration (path + temp[index] + ".WAV");

		// Reliability check.
		if (chatterSound.duration == 0.0f)
		{
			AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - has zero play duration, is skipped....", temp[index].GetData ());

			continue;
		}

		// fill the holder
		chatterSound.name = temp[index];

		// put to chatter factory
		m_banks[bankIndex].chatter[type].sounds += chatterSound;
	}
}

void ChatterManager::LoadChatterBank (void)
{
	// This function is called each time a new server starts, typically in ServerActivate().
	// It fills one by one the bot personality slots with the info read from the
	// "chatter.cfg" file that holds the bots names, skin, logo and skill of the bots.

	// RADIO/VOICE SYSTEM INITIALIZATION

	STDIOFile file;

	// do some cleanup first
	FreeChatter ();

	Bank_t newBank;

	newBank.path = Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ChatterPath)->GetString () + '/';

	m_banks += newBank;

	// opens file readonly
	if (!OpenConfigurationFile (CHATTER_MANAGER_CONFIG_FILENAME, "Chatter configuration file not found.", file))
		return;

	DynamicString line;
	unsigned int lineIndex (0u);
	unsigned char loadedChatterNumber (0u);
	bool isParsed[ChatterMessage_Total];

	for (/* Empty */; loadedChatterNumber < ChatterMessage_Total; ++loadedChatterNumber)
		isParsed[loadedChatterNumber] = false;

	loadedChatterNumber = 0u;

	// Reads line per line
	while (file.ReadLine (line, 512u))
	{
		++lineIndex;

		line.Trim ();	// trim all the blanks or linefeeds

		// skip all comment lines or empty lines
		if (IsEmptyOrCommentedFileBufferLine (line))
			continue;

		DynamicArray <DynamicString, unsigned char> pairs (line.Split <unsigned char> ('='));

		if (pairs.GetElementNumber () != 2u)
		{
			AddLogEntry (true, LogLevel_Error, false, "Chatter configuration file is not valid on a line: %u.", lineIndex);

			continue;
		}

		// trim all the blanks or linefeeds
		pairs.GetFirstElement ().Trim ();

		#define PARSE_CHATTER_RADIO_COMMAND_ID(radioCommandID)																																																																														\
			if (pairs.GetFirstElement ().CompareWithoutCaseCheck (#radioCommandID + GET_STATIC_STRING_LENGTH ("RadioCommand_")) == 0)																																																												\
			{																																																																																										\
				if (isParsed[radioCommandID])																																																																																		\
				{																																																																																									\
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" CHATTER_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has been already parsed!", #radioCommandID + GET_STATIC_STRING_LENGTH ("RadioCommand_"), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData (), lineIndex);					\
																																																																																													\
					continue;																																																																																						\
				}																																																																																									\
																																																																																													\
				/* Trim all the blanks or linefeeds and brackets. */																																																																												\
				pairs.GetLastElement ().Trim ();																																																																																	\
				pairs.GetLastElement ().Trim ().TrimLeft ("[{(").TrimRight (';').TrimRight ("]})");																																																																					\
																																																																																													\
				if (pairs.GetLastElement ().IsEmpty ())																																																																																\
				{																																																																																									\
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" CHATTER_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has no translation, using default string!", #radioCommandID + GET_STATIC_STRING_LENGTH ("RadioCommand_"), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData (), lineIndex);	\
																																																																																													\
					continue;																																																																																						\
				}																																																																																									\
																																																																																													\
				ParseVoiceEvent (BankIndex_Default, static_cast <ChatterMessage_t> (radioCommandID), pairs.GetLastElement (), 1.0f/* TEMP!!!!!!!!!!!!! */);																																																							\
																																																																																													\
				isParsed[radioCommandID] = true;																																																																																	\
																																																																																													\
				++loadedChatterNumber;																																																																																				\
																																																																																													\
				continue;																																																																																							\
			}																																																																																										\

		#define PARSE_CHATTER_MESSAGE_ID(chatterMessageID)																																																																																\
			if (pairs.GetFirstElement ().CompareWithoutCaseCheck (#chatterMessageID + GET_STATIC_STRING_LENGTH ("ChatterMessage_")) == 0)																																																												\
			{																																																																																											\
				if (isParsed[chatterMessageID])																																																																																			\
				{																																																																																										\
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" CHATTER_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has been already parsed!", #chatterMessageID + GET_STATIC_STRING_LENGTH ("ChatterMessage_"), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData (), lineIndex);					\
																																																																																														\
					continue;																																																																																							\
				}																																																																																										\
																																																																																														\
				/* Trim all the blanks or linefeeds and brackets. */																																																																													\
				pairs.GetLastElement ().Trim ();																																																																																		\
				pairs.GetLastElement ().Trim ().TrimLeft ("[{(").TrimRight (';').TrimRight ("]})");																																																																						\
																																																																																														\
				if (pairs.GetLastElement ().IsEmpty ())																																																																																	\
				{																																																																																										\
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" CHATTER_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has no translation, using default string!", #chatterMessageID + GET_STATIC_STRING_LENGTH ("ChatterMessage_"), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData (), lineIndex);	\
																																																																																														\
					continue;																																																																																							\
				}																																																																																										\
																																																																																														\
				ParseVoiceEvent (BankIndex_Default, chatterMessageID, pairs.GetLastElement (), 1.0f/* TEMP!!!!!!!!!!!!! */);																																																															\
																																																																																														\
				isParsed[chatterMessageID] = true;																																																																																		\
																																																																																														\
				++loadedChatterNumber;																																																																																					\
																																																																																														\
				continue;																																																																																								\
			}																																																																																											\

		using namespace HalfLifeEngine::SDK::Constants;

		// Radio system, menu 1.
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_CoverMe);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_YouTakePoint);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_HoldPosition);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_RegroupTeam);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_FollowMe);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_TakingFire);

		// Radio menu 2.
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_GoGoGo);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_FallBack);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_StickTogether);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_GetInPosition);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_StormTheFront);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_ReportTeam);

		// Radio menu 3.
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_Affirmative);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_EnemySpotted);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_NeedBackup);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_SectorClear);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_InPosition);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_ReportingIn);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_ShesGonnaBlow);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_Negative);
		PARSE_CHATTER_RADIO_COMMAND_ID (RadioCommand_EnemyDown);

		// Voice system
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_SpotTheBomber);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_FriendlyFire);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_Suicide);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_DiePain);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GotBlinded);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GoingToPlantBomb);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_RescuingHostages);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GoingToCamp);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_HearSomething);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_TeamKill);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_Camp);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_PlantingC4);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_DefusingC4);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_InCombat);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_SeeksEnemy);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_Nothing);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_UseHostage);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_FoundC4);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_WonTheRound);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_ScaredEmotion);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_HeardEnemy);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_SniperWarning);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_SniperKilled);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_VIPSpotted);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GuardingVipSafety);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GoingToGuardVIPSafety);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_QuicklyWonTheRound);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_OneEnemyLeft);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_TwoEnemiesLeft);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_ThreeEnemiesLeft);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_NoEnemiesLeft);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_FoundBombPlace);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_WhereIsTheBomb);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_DefendingBombSite);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_BarelyDefused);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_NiceShotCommander);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_NiceShotPall);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GoingToGuardHostages);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GoingToGuardDoppedBomb);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_OnMyWay);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_FollowingCommander);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_PinnedDown);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_BombTickingDown);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_CommanderDown);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_LetsDoThis);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_BombSiteSecure);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_CoveringFriend);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_AgreeWithPlan);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_Agree);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_Disagree);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_GuardingDoppedBomb);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_OnARollBrag);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_SpottedTwoEnemies);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_SpottedThreeEnemies);
		PARSE_CHATTER_MESSAGE_ID (ChatterMessage_SpottedTooManyEnemies)

		else
			AddLogEntry (true, LogLevel_Error, false, "Entry in %s_" CHATTER_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData (), lineIndex);
	}
/*
					// fixes for crashing if configuration file couldn't be accessed
					g_chatterFactory.SetSize (ChatterMessage_Total);

					// RADIO/VOICE SYSTEM INITIALIZATION
					if (OpenConfigurationFile (CHATTER_MANAGER_CONFIG_FILENAME, "Chatter system configuration file not found (configs/" CHATTER_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ").", file) && g_gameVersion != HalfLifeEngine::Engine::Version_Old && Globals::consoleVariables[ConsoleVariableID_Communication]->GetValue <int> () == 2)
					{
						DynamicArray <DynamicString> array;

						// for each line in the file...
						while (!file.IsEndOfFile () && file.ReadLine (line, sizeof (line)) != NULL)
						{
							// ignore line if void OR commented OR line which is not beginning from a word "Event" (match case)
							if (IsEmptyOrCommentedFileBufferLine (line) || strncmp (line, "Event", 5) != 0)
								continue;

							array = DynamicString (&line[6]).Split ('=');

							if (array.GetElementNumber () != 2)
							{
								file.Close ();

								AddLogEntry (false, LogLevel_Critical, false, "Error in chatter configuration file file syntax... Please correct all errors.");

								return;
							}

							for (index = 0; index < array.GetElementNumber (); index++)
								array[index].Trim ().Trim ();	// double trim

							// just to be more unique :)
							array[1].TrimLeft ('(');
							array[1].TrimRight (';');
							array[1].TrimRight (')');

							#define PARSE_VOICE_EVENT(event, timeToRepeatAgain)	if (strcmp (array[0], #event) == 0) ParseVoiceEvent (array, event, timeToRepeatAgain);

							// radio system, menu 1
							PARSE_VOICE_EVENT (RadioCommand_CoverMe,                  -1.0);
							PARSE_VOICE_EVENT (RadioCommand_YouTakePoint,             -1.0);
							PARSE_VOICE_EVENT (RadioCommand_HoldPosition,             -1.0);
							PARSE_VOICE_EVENT (RadioCommand_RegroupTeam,              -1.0);
							PARSE_VOICE_EVENT (RadioCommand_FollowMe,                 -1.0);
							PARSE_VOICE_EVENT (RadioCommand_TakingFire,               -1.0);

							// menu 2
							PARSE_VOICE_EVENT (RadioCommand_GoGoGo,                   -1.0);
							PARSE_VOICE_EVENT (RadioCommand_Fallback,                 -1.0);
							PARSE_VOICE_EVENT (RadioCommand_StickTogether,            -1.0);
							PARSE_VOICE_EVENT (RadioCommand_GetInPosition,            -1.0);
							PARSE_VOICE_EVENT (RadioCommand_StormTheFront,            -1.0);
							PARSE_VOICE_EVENT (RadioCommand_ReportTeam,               -1.0);

							// menu 3
							PARSE_VOICE_EVENT (RadioCommand_Affirmative,              -1.0);
							PARSE_VOICE_EVENT (RadioCommand_EnemySpotted,             -1.0);
							PARSE_VOICE_EVENT (RadioCommand_NeedBackup,               -1.0);
							PARSE_VOICE_EVENT (RadioCommand_SectorClear,              -1.0);
							PARSE_VOICE_EVENT (RadioCommand_InPosition,               -1.0);
							PARSE_VOICE_EVENT (RadioCommand_ReportingIn,              -1.0);
							PARSE_VOICE_EVENT (RadioCommand_ShesGonnaBlow,            -1.0);
							PARSE_VOICE_EVENT (RadioCommand_Negative,                 -1.0);
							PARSE_VOICE_EVENT (RadioCommand_EnemyDown,                -1.0);

							// voice system
							PARSE_VOICE_EVENT (ChatterMessage_FriendlyFire,            2.1);
							PARSE_VOICE_EVENT (ChatterMessage_DiePain,                -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_GotBlinded,              5.0);
							PARSE_VOICE_EVENT (ChatterMessage_GoingToPlantBomb,       -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_GoingToGuardVIPSafety,  -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_RescuingHostages,       -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_GoingToCamp,            -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_TeamKill,               -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_Camp,                   -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_GuardingVipSafety,      -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_PlantingC4,             -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_DefusingC4,              3.0);
							PARSE_VOICE_EVENT (ChatterMessage_InCombat,               -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_SeeksEnemy,             -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_Nothing,                -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_UseHostage,             -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_FoundC4,                 5.5);
							PARSE_VOICE_EVENT (ChatterMessage_WonTheRound,            -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_ScaredEmotion,           6.1);
							PARSE_VOICE_EVENT (ChatterMessage_HeardEnemy,             12.2);
							PARSE_VOICE_EVENT (ChatterMessage_SniperWarning,           4.3);
							PARSE_VOICE_EVENT (ChatterMessage_SniperKilled,            2.1);
							PARSE_VOICE_EVENT (ChatterMessage_QuicklyWonTheRound,     -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_OneEnemyLeft,            2.5);
							PARSE_VOICE_EVENT (ChatterMessage_TwoEnemiesLeft,          2.5);
							PARSE_VOICE_EVENT (ChatterMessage_ThreeEnemiesLeft,        2.5);
							PARSE_VOICE_EVENT (ChatterMessage_NoEnemiesLeft,          -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_FoundBombPlace,         -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_WhereIsTheBomb,         -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_DefendingBombSite,      -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_BarelyDefused,          -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_NiceShotCommander,      -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_NiceShotPall,           -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_GoingToGuardHostages,    3.0);
							PARSE_VOICE_EVENT (ChatterMessage_GoingToGuardDoppedBomb,  3.0);
							PARSE_VOICE_EVENT (ChatterMessage_OnMyWay,                 1.5);
							PARSE_VOICE_EVENT (ChatterMessage_FollowingCommander,      5.0);
							PARSE_VOICE_EVENT (ChatterMessage_PinnedDown,              6.1);
							PARSE_VOICE_EVENT (ChatterMessage_BombTickingDown,         5.0);
							PARSE_VOICE_EVENT (ChatterMessage_CommanderDown,          -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_LetsDoThis,             -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_BombSiteSecure,          3.5);
							PARSE_VOICE_EVENT (ChatterMessage_CoveringFriend,         -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_AgreeWithPlan,          -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_Agree,                  -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_Disagree,               -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_GuardingDoppedBomb,     -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_OnARollBrag,            -1.0);
							PARSE_VOICE_EVENT (ChatterMessage_SpotTheBomber,           5.5);
							PARSE_VOICE_EVENT (ChatterMessage_VIPSpotted,              5.5);
							PARSE_VOICE_EVENT (ChatterMessage_SpottedTwoEnemies,       5.5);
							PARSE_VOICE_EVENT (ChatterMessage_SpottedThreeEnemies,     5.5);
							PARSE_VOICE_EVENT (ChatterMessage_SpottedTooManyEnemies,   5.5);

							AddLogEntry (false, LogLevel_Default, false, "Parsing '%s' line: %s.", array[0].GetData (), array[1].GetData ());
						}
					}
					else
					{
						Globals::consoleVariables[ConsoleVariableID_Communication]->SetValue <unsigned char> (1u);

						AddLogEntry (true, LogLevel_Default, false, "Voice communication disabled.");
					}
*/
	AddLogEntry (true, LogLevel_Default, false, "%u of %u bot chatter loaded.", loadedChatterNumber, ChatterMessage_Total);
}

void ChatterManager::PrintChatter (const BankIndex_t/* bankIndex*/) const
{
/*	// Reliability check.
	if (m_banks[bankIndex].chatter.IsEmpty ())
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("No loaded chatter!\n");

		return;
	}

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Bot chatter:\n");	// tell what we're about to do

	// Cycle through all chatter and display them....
	for (unsigned char index (0u); index < m_banks[bankIndex].chatter.GetElementNumber (); ++index)
	{
		const Chatter_t *const chatter (&m_banks[bankIndex].chatter[index]);

		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Index == %u, is used: %s, name: \"%s\", team: %s, model == %u, skill == %u, reaction time == %.2f, attack delay == %.2f, teamplay percent == %u, voice pitch == %u, personality: %s, use bot tag: %s, logo: %s, is used chatter: %s.\n", index + 1u, profile->isUsed ? "true" : "false", profile->name.GetData (), profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "terrorist" : profile->team == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "counter-terrorist" : "auto-assign", profile->model, profile->skill.GetValue (), profile->reactionTime, profile->attackDelay, profile->teamplayPercent, profile->voicePitch, profile->personality == Personality_Normal ? "normal" : profile->personality == Personality_Rusher ? "rusher" : "careful", profile->ping ? "true" : "false", profile->logo.GetData (), profile->communicationType == YaPB::CommunicationType_Chatter ? "true" : "false");
	}

	// Display the count.
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\tTotal is %u chatters.\n", m_banks[bankIndex].chatter.GetElementNumber ());*/
}