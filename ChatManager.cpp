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
// ChatManager.cpp
//
// Class: ChatManager
//
// Description: YaPB chat manager.
//
// Version: $ID:$
//

#include <Core.hpp>
#if 0
void ChatManager::LoadChat (void)
{
	// This function is called each time a new server starts, typically in ServerActivate().
	// It fills one by one the bot personality slots with the info read from the
	// "chat.cfg" file that holds the bots names, skin, logo and skill of the bots.

	STDIOFile file;

	// do some cleanup first
	FreeChat ();

	// opens file readonly
	if (!OpenConfigurationFile (CHAT_MANAGER_CONFIG_FILENAME, "Chat configuration file not found.", file, true))
		return;

	DynamicString line;
	unsigned int lineIndex (0u);
	enum SectionType_t
	{
		SectionType_None,

		SectionType_WeaponPreferenceTypes,
		SectionType_DefaultProfile,
		SectionType_Profile
	} currentSectionType (SectionType_None);

	// Reads line per line
	while (file.ReadLine (line, 256u))
	{
		++lineIndex;

		line.Trim ();	// trim all the blanks or linefeeds

		// skip all comment lines or empty lines
		if (IsEmptyOrCommentedFileBufferLine (line))
			continue;

		if (line.CompareWithoutCaseCheck ("[DEFAULT PROFILE]") == 0)
		{
			// A default profile item has started....
			if (currentSectionType == SectionType_DefaultProfile)
				AddLogEntry (true, LogLevel_Error, false, "The default profile should be defined only once.");
			else
				currentSectionType = SectionType_DefaultProfile;
		}
		else if (line.CompareWithoutCaseCheck ("[PROFILE]") == 0)
		{
			// A new profile item has started....
			m_chat += m_defaultProfile;

			currentSectionType = SectionType_Profile;
		}
		else if (currentSectionType != SectionType_None)
		{
			DynamicArray <DynamicString, unsigned char> pairs (line.Split <unsigned char> ('='));

			if (pairs.GetElementNumber () != 2u)
			{
				AddLogEntry (true, LogLevel_Error, false, "Profiles configuration file is not valid on a line: %u.", lineIndex);

				continue;
			}

			Chat_t *const profile (currentSectionType == SectionType_DefaultProfile ? &m_defaultProfile : &m_chat.GetLastElement ());

			// trim all the blanks or linefeeds
			pairs.GetFirstElement ().Trim ();
			pairs.GetLastElement ().Trim ();
/*
			if (currentSectionType == SectionType_None)
			{
				AddLogEntry (true, LogLevel_Error, false, "[END] section on a line: %u has been found without any start section.", lineIndex);

				continue;
			}
			else */if (currentSectionType == SectionType_WeaponPreferenceTypes)
			{
				if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("RifleT") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("RifleCT") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("PunchT") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("PunchCT") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("Sniper") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("Power") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("Shotgun") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("Shield") == 0 || pairs.GetFirstElement ().CompareWithoutCaseCheck ("Spray") == 0)
				{
					// just to be more unique :)
					pairs.GetLastElement ().TrimLeft ("{(");
					pairs.GetLastElement ().TrimRight (';');
					pairs.GetLastElement ().TrimRight ("})");

					DynamicArray <DynamicString, unsigned char> weaponsPreferencesNames (pairs.GetLastElement ().Split <unsigned char> (",;"));

					if (weaponsPreferencesNames.IsEmpty ())
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapons preferences array \"%s\" in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u, has zero length.", pairs.GetFirstElement ().GetData (), lineIndex);

						continue;
					}

					if (weaponsPreferencesNames.GetElementNumber () > 18u)
					{
						AddLogEntry (true, LogLevel_Error, false, "Weapons preferences array \"%s\" in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u, has too big length: %u (maximum length is: 18).", pairs.GetFirstElement ().GetData (), lineIndex, weaponsPreferencesNames.GetElementNumber ());

						continue;
					}

					YaPB::Profile::WeaponsPreferences_t::Array_t weaponsPreferences;

					for (YaPB::Profile::WeaponsPreferences_t::Array_t::IndexType_t index (0u); index < weaponsPreferencesNames.GetElementNumber (); ++index)
					{
						// trim all the blanks or linefeeds
						weaponsPreferencesNames[index].Trim ();

						weaponsPreferences += HalfLifeEngine::Utilities::GetWeaponIDFromAlias (weaponsPreferencesNames[index]);
					}

					// Reliability check.
					InternalAssert (!weaponsPreferences.IsEmpty ());

					profile->weaponsPreferences = weaponsPreferences;
				}
				else
					AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", pairs.GetFirstElement ().GetData (), lineIndex);
			}
			else if (currentSectionType == SectionType_DefaultProfile || currentSectionType == SectionType_Profile)
			{
				if (m_chat.IsEmpty ())
				{
					AddLogEntry (true, LogLevel_Error, false, "[END] section on a line: %u has been found with empty profiles array.", lineIndex);

					continue;
				}

				if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("name") == 0)
				{
					const DynamicString &name (pairs.GetLastElement ());

					if (name.GetElementNumber () >= HalfLifeEngine::SDK::Constants::MaximumClientNameLength)
					{
						AddLogEntry (true, LogLevel_Error, false, "Bot name: \"%s\" in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u, has too big length: %u (maximum length is: %u).", name.GetData (), lineIndex, name.GetElementNumber (), HalfLifeEngine::SDK::Constants::MaximumClientNameLength - 1u);

						profile->name.Assign (name, 0u, HalfLifeEngine::SDK::Constants::MaximumClientNameLength);
					}
					else if (IsBotNameExists (name))	// check if this name already exists...
						AddLogEntry (true, LogLevel_Error, false, "Duplicate name in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION " - \"%s\", on a line: %u.", name.GetData (), lineIndex);
					else
						profile->name = name;
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("team") == 0)
				{
					const HalfLifeEngine::SDK::Constants::TeamID_t team (pairs.GetLastElement ().GetValue <HalfLifeEngine::SDK::Constants::TeamID_t> ());

					if (team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist || team == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
						profile->team = team;
					else if (team != HalfLifeEngine::SDK::Constants::TeamID_AutoSelect)
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, team);
				}
				else
					AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", pairs.GetFirstElement ().GetData (), lineIndex);
			}
		}
		else if (line.CompareWithoutCaseCheck ("[END]") == 0)
			currentSectionType = SectionType_None;
		else
			AddLogEntry (true, LogLevel_Error, false, "%s section in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", line.GetData (), lineIndex);
	}
#if 0
					// fixes for crashing if configuration file couldn't be accessed
					g_chatFactory.SetSize (ChatType_Total);

					// CHAT SYSTEM CONFIGURATION FILE INITIALIZATION
					if (OpenConfigurationFile (CHAT_MANAGER_CONFIG_FILENAME, "Chat file not found.", file, true))
					{
						char command[80u];
						ChatType_t chatType (ChatType_None);
						Reply_t replyKey;

						// for each line in the file...
						while (!file.IsEndOfFile () && file.ReadLine (line, sizeof (line)) != NULL)
						{
							// ignore line if void OR commented
							if (IsEmptyOrCommentedFileBufferLine (line))
								continue;

							// this line looks like a valid data line, figure out which the chat type it is talking about
							strcpy (command, GetField (line, 0, true));

							if (strcmp (command, "[KILLED]") == 0)
							{
								chatType = ChatType_Killing;

								continue;
							}
							else if (strcmp (command, "[DEADCHAT]") == 0)
							{
								chatType = ChatType_Dead;

								continue;
							}
							else if (strcmp (command, "[BOMBPLANT]") == 0)
							{
								chatType = ChatType_BombPlant;

								continue;
							}
							else if (strcmp (command, "[TEAMATTACK]") == 0)
							{
								chatType = ChatType_TeamAttack;

								continue;
							}
							else if (strcmp (command, "[TEAMKILL]") == 0)
							{
								chatType = ChatType_TeamKill;

								continue;
							}
							else if (strcmp (command, "[WELCOME]") == 0)
							{
								chatType = ChatType_Welcome;

								continue;
							}
							else if (strcmp (command, "[UNKNOWN]") == 0)
							{
								chatType = ChatType_NoKeyword;

								continue;
							}
							else if (strcmp (command, "[REPLIES]") == 0)
							{
								chatType = ChatType_Reply;

								continue;
							}

							if (chatType != ChatType_Reply)
								line[79u] = 0;	// terminate the string

							strtrim (line);

							switch (chatType)
							{
								case ChatType_Killing:
									g_chatFactory[ChatType_Killing] += line;

									break;

								case ChatType_Dead:
									g_chatFactory[ChatType_Dead] += line;

									break;

								case ChatType_BombPlant:
									g_chatFactory[ChatType_BombPlant] += line;

									break;

								case ChatType_TeamAttack:
									g_chatFactory[ChatType_TeamAttack] += line;

									break;

								case ChatType_TeamKill:
									g_chatFactory[ChatType_TeamKill] += line;

									break;

								case ChatType_Welcome:
									g_chatFactory[ChatType_Welcome] += line;

									break;

								case ChatType_NoKeyword:
									g_chatFactory[ChatType_NoKeyword] += line;

									break;

								case ChatType_Reply:
									if (strstr (line, "@KEY") != NULL)
									{
										if (!replyKey.keywords.IsEmpty () && !replyKey.replies.IsEmpty ())
										{
											g_replyFactory += replyKey;

											replyKey.replies.RemoveAll ();
										}

										replyKey.keywords.RemoveAll ();

										replyKey.keywords = DynamicString (&line[4]).Split (',');

										for (index = 0; index < replyKey.keywords.GetElementNumber (); index++)
											replyKey.keywords[index].Trim ().TrimQuotes ();
									}
									else if (!replyKey.keywords.IsEmpty ())
										replyKey.replies += line;

									break;
/*
								default:
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "Undefined chat type!");

									return;*/
							}
						}
					}
					else
						Globals::consoleVariables[ConsoleVariableID_Chat]->SetValue <bool> (false);
#endif	// if 0
	AddLogEntry (true, LogLevel_Default, false, "%u bot profiles loaded.", m_chat.GetElementNumber ());
}

Chat_t *const ChatManager::PickChat (const unsigned char skill/* = 0u*/)
{
	// Pick a bot profile which is same as the specified skill.

	// Reliability check.
	if (m_chat.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, can't pick any profile....");

		return NULL;	// Profiles array is empty, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, using default profile....");

		return &m_defaultProfile;	// Profiles array is empty, so return default profile pointer.
	}

	DynamicArray <unsigned char, unsigned char> validProfileIndexes;

	// Loop through all profiles....
	for (unsigned char index (0u); index < m_chat.GetElementNumber (); ++index)
	{
		// check if this profile is NOT used AND skill is NOT specified OR specified skill is the same as this profile
		if (!m_chat[index].isUsed && (skill == 0u || m_chat[index].skill == skill))
			validProfileIndexes += index;	// This is a good one.
	}

	if (validProfileIndexes.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, can't pick any profile....");

		return NULL;	// No profile is good to use, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, using default profile....");

		return &m_defaultProfile;	// No profile is good to use, so return default profile pointer.
	}

	return m_chat + validProfileIndexes.GetRandomElement ();
}

Chat_t *const ChatManager::PickChat (const DynamicString &botName)
{
	// Pick a bot profile which is same as the specified name.

	// Reliability check.
	if (m_chat.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, can't pick any profile....");

		return NULL;	// Profiles array is empty, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, using default profile....");

		return &m_defaultProfile;	// Profiles array is empty, so return default profile pointer.
	}

	DynamicArray <unsigned char, unsigned char> validProfileIndexes;

	// Loop through all profiles....
	for (unsigned char index (0u); index < m_chat.GetElementNumber (); ++index)
	{
		// check if this profile is NOT used AND specified name is the same as this profile
		if (!m_chat[index].isUsed && m_chat[index].name == botName)
			validProfileIndexes += index;	// This is a good one.
	}

	if (validProfileIndexes.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, can't pick any profile....");

		return NULL;	// No profile is good to use, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, using default profile....");

		return &m_defaultProfile;	// No profile is good to use, so return default profile pointer.
	}

	return m_chat + validProfileIndexes.GetRandomElement ();
}

void ChatManager::PrintChat (void) const
{
	// Reliability check.
	if (m_chat.IsEmpty ())
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("No loaded chat!\n");

		return;
	}

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Bot chat:\n");	// tell what we're about to do

	// Cycle through all chat and display them....
	for (unsigned char index (0u); index < m_chat.GetElementNumber (); ++index)
	{
		const Chat_t *const chat (&m_chat[index]);

		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Index == %u, is used: %s, name: \"%s\", team: %s, model == %u, skill == %u, reaction time == %.2f, attack delay == %.2f, teamplay percent == %u, voice pitch == %u, personality: %s, use bot tag: %s, logo: %s, is used chatter: %s.\n", index + 1u, profile->isUsed ? "true" : "false", profile->name.GetData (), profile->team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "terrorist" : profile->team == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "counter-terrorist" : "auto-assign", profile->model, profile->skill.GetValue (), profile->reactionTime, profile->attackDelay, profile->teamplayPercent, profile->voicePitch, profile->personality == Personality_Normal ? "normal" : profile->personality == Personality_Rusher ? "rusher" : "careful", profile->ping ? "true" : "false", profile->logo.GetData (), profile->communicationType == YaPB::CommunicationType_Chatter ? "true" : "false");
	}

	// Display the count.
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\tTotal is %u chats.\n", m_chat.GetElementNumber ());
}
#endif	// if 0