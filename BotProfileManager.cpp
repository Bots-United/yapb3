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
// ProfileManager.cpp
//
// Class: ProfileManager
//
// Description: YaPB profile manager.
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPBManager::ProfileManager::LoadBotProfiles (void)
{
	// This function is called each time a new server starts, typically in ServerActivate().
	// It fills one by one the bot personality slots with the info read from the
	// PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION file that holds the bots names, skin, logo and skill of the bots.

	/// @todo ADD TEMPLATES FOR BOT PROFILES FILE LIKE CSBOT HAS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	STDIOFile file;
	struct WeaponTemplates_t
	{
		DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> rifle;
		DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> punch;
		DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> sniper;
		DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> power;
		DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> shotgun;
		DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> shield;
		DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> spray;

		inline WeaponTemplates_t (void)
		{
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_USP;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
			rifle += HalfLifeEngine::SDK::Constants::WeaponID_Elite;

			punch += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_USP;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
			punch += HalfLifeEngine::SDK::Constants::WeaponID_Elite;

			sniper += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_USP;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
			sniper += HalfLifeEngine::SDK::Constants::WeaponID_Elite;

			power += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			power += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			power += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			power += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			power += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			power += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			power += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			power += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			power += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			power += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			power += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			power += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			power += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			power += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			power += HalfLifeEngine::SDK::Constants::WeaponID_USP;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			power += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			power += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
			power += HalfLifeEngine::SDK::Constants::WeaponID_Elite;

			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_USP;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
			shotgun += HalfLifeEngine::SDK::Constants::WeaponID_Elite;

			shield += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_USP;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
			shield += HalfLifeEngine::SDK::Constants::WeaponID_Elite;

			spray += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_M249;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_P90;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_M3;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_USP;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_P228;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
			spray += HalfLifeEngine::SDK::Constants::WeaponID_Elite;
		}
	} weaponTemplates;

	// do some cleanup first
	FreeBotProfiles ();

	// opens file readonly
	if (!OpenConfigurationFile (PROFILE_MANAGER_CONFIG_FILENAME, "Profiles configuration file not found.", file))
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
				AddLogEntry (true, LogLevel_Error, false, "The default profile should be defined only once! Line: %u.", lineIndex);
			else if (!m_profiles.IsEmpty ())
				AddLogEntry (true, LogLevel_Error, false, "The default profile should be defined only first! Line: %u.", lineIndex);
			else
				currentSectionType = SectionType_DefaultProfile;
		}
		else if (line.CompareWithoutCaseCheck ("[PROFILE]") == 0)
		{
			// A new profile item has started....
			AppendDefaultProfile ();

			currentSectionType = SectionType_Profile;
		}
		else if (line.CompareWithoutCaseCheck ("[END]") == 0)
		{
			if (currentSectionType == SectionType_None)
			{
				AddLogEntry (true, LogLevel_Warning, false, "\"[END]\" section on a line: %u has been found without any start section.", lineIndex);

				continue;
			}

			currentSectionType = SectionType_None;
		}
		else if (currentSectionType != SectionType_None)
		{
			// read bot profile with bots name etc on it.
/*
			/// NEW!!!!!!!!!!!!!!!!!!!!!!!
			{
			StaticArray <DynamicString, unsigned char, 2u> pairs;
			DynamicString::IndexType_t separatorIndex (line.Find ('='));

			if (separatorIndex == DynamicString::BadPosition)
			{
				AddLogEntry (true, LogLevel_Warning, false, "Profiles configuration file has no separator character '=' on a line: %u.", lineIndex);

				continue;
			}

			pairs += line.GetLeftSubstring (separatorIndex);
			pairs += line.GetRightSubstring (line.GetElementNumber () - (separatorIndex + 1u/* Skip '=' character. *//*));
			}*/
			DynamicArray <DynamicString, unsigned char> pairs (line.Split <unsigned char> ('='));

			if (pairs.GetElementNumber () != 2u)
			{
				AddLogEntry (true, LogLevel_Warning, false, "Profiles configuration file is not valid on a line: %u.", lineIndex);

				continue;
			}

			// trim all the blanks or linefeeds
			pairs.GetFirstElement ().TrimRight ();
			pairs.GetLastElement ().TrimLeft ();

			pairs.GetFirstElement ().TrimQuotes ();
			pairs.GetLastElement ().TrimQuotes ();

			if (pairs.GetFirstElement ().IsEmpty () || pairs.GetLastElement ().IsEmpty ())
			{
				AddLogEntry (true, LogLevel_Warning, false, "Profiles configuration file is not valid on a line: %u.", lineIndex);

				continue;
			}

			YaPB::Profile &profile (currentSectionType == SectionType_DefaultProfile ? m_defaultProfile : m_profiles.GetLastElement ());

			if (currentSectionType == SectionType_WeaponPreferenceTypes)
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

					profile.weaponsPreferences = weaponsPreferences;
				}
				else
					AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", pairs.GetFirstElement ().GetData (), lineIndex);
			}
			else if (currentSectionType == SectionType_DefaultProfile || currentSectionType == SectionType_Profile)
			{
				if (m_profiles.IsEmpty () && currentSectionType != SectionType_DefaultProfile)
				{
					AddLogEntry (true, LogLevel_Error, false, "[END] section on a line: %u has been found with empty profiles array.", lineIndex);

					continue;
				}

				if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("name") == 0)
				{
					DynamicString &name (pairs.GetLastElement ());

					// check name length
					if (name.GetElementNumber () >= HalfLifeEngine::SDK::Constants::MaximumClientNameLength)
					{
						AddLogEntry (true, LogLevel_Warning, false, "Bot name: \"%s\" in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u, has too big length: %u (maximum length is: %u). The name will be truncated.", name.GetData (), lineIndex, name.GetElementNumber (), HalfLifeEngine::SDK::Constants::MaximumClientNameLength - 1u);

						// Truncate name....
						name.ReleaseBuffer (HalfLifeEngine::SDK::Constants::MaximumClientNameLength);
					}

					// check if this name already exists....
					if (IsBotNameExists (name))
						AddLogEntry (true, LogLevel_Warning, false, "Duplicate name in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION " - \"%s\", on a line: %u.", name.GetData (), lineIndex);

					// Whatever assign it....
					profile.name = name;
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("team") == 0)
				{
					const HalfLifeEngine::SDK::Constants::TeamID_t team (pairs.GetLastElement ().GetValue <HalfLifeEngine::SDK::Constants::TeamID_t> ());

					if (team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist || team == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
						profile.team = team;
					else if (team != HalfLifeEngine::SDK::Constants::TeamID_AutoSelect)
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, team);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("model ", GET_STATIC_STRING_LENGTH ("model ")) == 0)	//! @todo MAKE READING FOR BOTH TEAMS!
				{
					const unsigned char model (pairs.GetLastElement ().GetValue <unsigned char> ());

					if (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModID () == HalfLifeEngine::Engine::Version_Steam)
					{
						if (model >= 1u && model <= HalfLifeEngine::SDK::Constants::MaximumPlayerModels)
						{
							const DynamicString team (pairs.GetFirstElement ().GetData () + GET_STATIC_STRING_LENGTH ("model "));

							if (!team.IsEmpty ())
							{
								// Terrorists team.
								if (team.CompareWithCaseCheck ('1') == 0 || team.CompareWithoutCaseCheck ('t') == 0)
									profile.model[TeamArrayID_Terrorist] = model;
								else if (team.CompareWithCaseCheck ('2') == 0 || team.CompareWithoutCaseCheck ("ct") == 0)	// Counter-Terrorists team.
									profile.model[TeamArrayID_CounterTerrorist] = model;
								else
									AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is for unknown team: \"%s\"!", pairs.GetFirstElement ().GetData (), lineIndex, team.GetData ());
							}
							else
								AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is for unknown (empty) team!", pairs.GetFirstElement ().GetData (), lineIndex);
						}
						else if (model != HalfLifeEngine::SDK::Constants::PlayerModelID_AutoSelect)
							AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, model);
					}
					else	// Condition Zero
					{
						// Condition Zero has spetsnaz and militia skins
						if (model >= 1u && model <= HalfLifeEngine::SDK::Constants::MaximumPlayerModels_ConditionZero)
						{
							const DynamicString team (pairs.GetFirstElement ().GetData () + GET_STATIC_STRING_LENGTH ("model "));

							if (!team.IsEmpty ())
							{
								// Terrorists team.
								if (team.CompareWithCaseCheck ('1') == 0 || team.CompareWithoutCaseCheck ('t') == 0)
									profile.model[TeamArrayID_Terrorist] = model;
								else if (team.CompareWithCaseCheck ('2') == 0 || team.CompareWithoutCaseCheck ("ct") == 0)	// Counter-Terrorists team.
									profile.model[TeamArrayID_CounterTerrorist] = model;
								else
									AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is for unknown team: \"%s\"!", pairs.GetFirstElement ().GetData (), lineIndex, team.GetData ());
							}
							else
								AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is for unknown (empty) team!", pairs.GetFirstElement ().GetData (), lineIndex);
						}
						else if (model != HalfLifeEngine::SDK::Constants::PlayerModelID_ConditionZero_AutoSelect)
							AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, model);
					}
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("aggression") == 0)
				{
					const unsigned char aggression (pairs.GetLastElement ().GetValue <unsigned char> ());

					if (aggression >= 1u && aggression <= 100u)
						profile.aggression = aggression;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, aggression);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("skill") == 0)
				{
					const unsigned char skill (pairs.GetLastElement ().GetValue <unsigned char> ());

					if (skill >= BotSkillValue_Minimum && skill <= BotSkillValue_Maximum)
						profile.skill = skill;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, skill);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("reactionTime") == 0)
				{
					const float reactionTime (pairs.GetLastElement ().GetValue <float> ());

					if (reactionTime >= 0.0f && reactionTime <= 5.0f)
						profile.reactionTime = reactionTime;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %.2f.", pairs.GetFirstElement ().GetData (), lineIndex, reactionTime);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("attackDelay") == 0)
				{
					const float attackDelay (pairs.GetLastElement ().GetValue <float> ());

					if (attackDelay >= 0.0f && attackDelay <= 5.0f)
						profile.attackDelay = attackDelay;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %.2f.", pairs.GetFirstElement ().GetData (), lineIndex, attackDelay);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("teamplayPercent") == 0)
				{
					const unsigned char teamplayPercent (pairs.GetLastElement ().GetValue <unsigned char> ());

					if (teamplayPercent <= 100u)
						profile.teamplayPercent = teamplayPercent;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, teamplayPercent);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("voiceVolume") == 0)
				{
					const unsigned char voiceVolume (pairs.GetLastElement ().GetValue <unsigned char> ());

					if (voiceVolume >= 20u)
						profile.voiceVolume = voiceVolume;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, voiceVolume);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("voicePitch") == 0)
				{
					const unsigned char voicePitch (pairs.GetLastElement ().GetValue <unsigned char> ());

					if (voicePitch >= 83u && voicePitch <= 125u)
						profile.voicePitch = voicePitch;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, voicePitch);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("soundVolume") == 0)
				{
					const float soundVolume (pairs.GetLastElement ().GetValue <float> ());

					if (soundVolume >= 0.0f && soundVolume <= 1.0f)
						profile.soundVolume = soundVolume;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, soundVolume);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("hasStereo") == 0)
					profile.hasStereo = pairs.GetLastElement ().GetValue <bool> ();
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("personality") == 0)
				{
					const Personality_t personality (pairs.GetLastElement ().GetValue <Personality_t> ());

					if (personality >= Personality_Normal && personality <= Personality_Careful)
						profile.personality = personality;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, personality);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("ping") == 0)
				{
					const short ping (pairs.GetLastElement ().GetValue <short> ());

					if (ping >= BotPingValue_BotTag && ping <= BotPingValue_Maximum)
						profile.ping = ping;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %i.", pairs.GetFirstElement ().GetData (), lineIndex, ping);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("leftHand") == 0)
					profile.leftHand = pairs.GetLastElement ().GetValue <bool> ();
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("autoWeponSwitch") == 0)
					profile.autoWeponSwitch = pairs.GetLastElement ().GetValue <bool> ();
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("logo") == 0)
				{
					const DynamicString &logo (pairs.GetLastElement ());
					const short decalIndex (HalfLifeEngine::Globals::g_halfLifeEngine->GetDecalIndex (logo));

					if (logo.GetElementNumber () >= HalfLifeEngine::SDK::Constants::MaximumLogoNameLength)
						AddLogEntry (true, LogLevel_Error, false, "Bot logo name: \"%s\" in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u, has too big length: %u (maximum length is: %u).", logo.GetData (), lineIndex, logo.GetElementNumber (), HalfLifeEngine::SDK::Constants::MaximumLogoNameLength);
					else if (decalIndex < 0)
						AddLogEntry (true, LogLevel_Error, false, "Couldn't get index of decal %s, using default decal (lambda).", logo.GetData ());
					else
						profile.logo = logo;
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("communicationType") == 0)
				{
					const YaPB::CommunicationType_t communicationType (pairs.GetLastElement ().GetValue <YaPB::CommunicationType_t> ());

					if (communicationType >= YaPB::CommunicationType_None && communicationType <= YaPB::CommunicationType_Chatter)
						profile.communicationType = communicationType;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %u.", pairs.GetFirstElement ().GetData (), lineIndex, communicationType);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("chatterBank") == 0)
				{
					const DynamicString &chatterBankPath (pairs.GetLastElement ());

					/// @todo !!!
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("baseReactTime") == 0)
				{
					const float baseReactTime (pairs.GetLastElement ().GetValue <float> ());

					if (baseReactTime >= 0.2f && baseReactTime <= 2.5f)
						profile.baseReactTime = baseReactTime;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %.2f.", pairs.GetFirstElement ().GetData (), lineIndex, baseReactTime);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("baseAggressionLevel") == 0)
				{
					const float baseAggressionLevel (pairs.GetLastElement ().GetValue <float> ());

					if (baseAggressionLevel >= 0.0f && baseAggressionLevel <= 1.0f)
						profile.baseAggressionLevel = baseAggressionLevel;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %.2f.", pairs.GetFirstElement ().GetData (), lineIndex, baseAggressionLevel);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("baseFearLevel") == 0)
				{
					const float baseFearLevel (pairs.GetLastElement ().GetValue <float> ());

					if (baseFearLevel >= 0.0f && baseFearLevel <= 1.0f)
						profile.baseFearLevel = baseFearLevel;
					else
						AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is out of range: %.2f.", pairs.GetFirstElement ().GetData (), lineIndex, baseFearLevel);
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("hal_pretrain_file") == 0)
				{
					const DynamicString &m_szHAL_PreTrainFile (pairs.GetLastElement ());

					profile.m_szHAL_PreTrainFile = m_szHAL_PreTrainFile;
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("hal_aux_file") == 0)
				{
					const DynamicString &m_szHAL_AuxFile (pairs.GetLastElement ());

					profile.m_szHAL_AuxFile = m_szHAL_AuxFile;
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("hal_ban_file") == 0)
				{
					const DynamicString &m_szHAL_BanFile (pairs.GetLastElement ());

					profile.m_szHAL_BanFile = m_szHAL_BanFile;
				}
				else if (pairs.GetFirstElement ().CompareWithoutCaseCheck ("hal_swap_file") == 0)
				{
					const DynamicString &m_szHAL_SwapFile (pairs.GetLastElement ());

					profile.m_szHAL_SwapFile = m_szHAL_SwapFile;
				}
				else
					AddLogEntry (true, LogLevel_Error, false, "%s entry in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", pairs.GetFirstElement ().GetData (), lineIndex);
			}
		}
		else
			AddLogEntry (true, LogLevel_Error, false, "%s section in " PROFILE_MANAGER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", line.GetData (), lineIndex);
	}
#if 0
					// NAMING SYSTEM INITIALIZATION
					if (OpenConfigurationFile ("names", "Name configuration file not found (configs/names.cfg).", file, true))
					{
						BotName_t item;
						bool isThisNameAlreadyExists;

						// for each line in the file...
						for (int linePosition = 1; !file.IsEndOfFile () && file.ReadLine (line, sizeof (line)) != NULL; linePosition++)
						{
							// ignore line if void OR commented
							if (IsEmptyOrCommentedFileBufferLine (line))
								continue;

							strtrim (line);

							// check name length
							if (GetStringLength <unsigned short> (line) >= Const_MaximumClientNameLength)
							{
								AddLogEntry (true, LogLevel_Warning, false, "Bot name (%s) length in a 'names.cfg' on line %i is more than 31 characters!", line, linePosition);

								continue;
							}

							isThisNameAlreadyExists = false;

							// Check for a duplicate - an existing entry with this pathname.
							for (index = 0; index < g_botNames.GetElementNumber (); index++)
							{
								if (g_botNames[index].name != const_cast <const char *const> (line))
									continue;

								isThisNameAlreadyExists = true;

								break;
							}

							if (isThisNameAlreadyExists)
							{
								AddLogEntry (false, LogLevel_Default, false, "Skipping duplicate bot name: '%s' in a 'names.cfg', on line %i.", line, linePosition);

								continue;
							}

							// fill the holder
							item.name   = line;
							item.isUsed = false;

							// put to queue
							g_botNames += item;
						}
					}
#endif	// if 0
	AddLogEntry (true, LogLevel_Default, false, "%u bot profiles loaded.", m_profiles.GetElementNumber ());
}

YaPB::Profile *const YaPBManager::ProfileManager::PickBotProfile (const unsigned char skill/* = 0u*/)
{
	// Pick a bot profile which is same as the specified skill.

	// Reliability check.
	if (m_profiles.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, can't pick any profile....");

		return NULL;	// Profiles array is empty, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, using default profile....");

		AppendDefaultProfile ();

		return &m_profiles.GetLastElement ();	// Profiles array is empty, so return default profile pointer.
	}

	DynamicArray <ProfilesArray_t::IndexType_t, ProfilesArray_t::IndexType_t> validProfileIndexes;

	// Loop through all profiles....
	for (ProfilesArray_t::IndexType_t index (0u); index < m_profiles.GetElementNumber (); ++index)
	{
		// check if this profile is NOT used AND skill is NOT specified OR specified skill is the same as this profile
		if (!m_profiles[index].isUsed && (skill == 0u || m_profiles[index].skill == skill))
			validProfileIndexes += index;	// This is a good one.
	}

	if (validProfileIndexes.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, can't pick any profile....");

		return NULL;	// No profile is good to use, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, using default profile....");

		AppendDefaultProfile ();

		return &m_profiles.GetLastElement ();	// No profile is good to use, so return default profile pointer.
	}

	return m_profiles + validProfileIndexes.GetRandomElement ();
}

YaPB::Profile *const YaPBManager::ProfileManager::PickBotProfile (const DynamicString &botName)
{
	// Pick a bot profile which is same as the specified name.

	// Reliability check.
	if (m_profiles.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, can't pick any profile....");

		return NULL;	// Profiles array is empty, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "Bot profiles array is empty, using default profile....");

		AppendDefaultProfile ();

		return &m_profiles.GetLastElement ();	// Profiles array is empty, so return default profile pointer.
	}

	DynamicArray <ProfilesArray_t::IndexType_t, ProfilesArray_t::IndexType_t> validProfileIndexes;
	const bool isEmptyOrDefaultBotName (botName.IsEmpty () || botName == DEFAULT_YAPB_NAME);

	// Loop through all profiles....
	for (ProfilesArray_t::IndexType_t index (0u); index < m_profiles.GetElementNumber (); ++index)
	{
		// check if this profile is NOT used AND specified name is the same as this profile
		if (!m_profiles[index].isUsed && (isEmptyOrDefaultBotName || m_profiles[index].name == botName))
			validProfileIndexes += index;	// This is a good one.
	}

	if (validProfileIndexes.IsEmpty ())
	{
/*		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, can't pick any profile....");

		return NULL;	// No profile is good to use, so return NULL pointer.
*/
		AddLogEntry (true, LogLevel_Error, false, "All bot profiles are used, using default profile....");

		AppendDefaultProfile ();

		return &m_profiles.GetLastElement ();	// No profile is good to use, so return default profile pointer.
	}

	return m_profiles + validProfileIndexes.GetRandomElement ();
}

void YaPBManager::ProfileManager::PrintBotProfiles (void) const
{
	// Reliability check.
	if (m_profiles.IsEmpty ())
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("No loaded profiles!\n");

		return;
	}

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Bot profiles:\n");	// tell what we're about to do

	// Cycle through all profiles and display them....
	for (ProfilesArray_t::IndexType_t index (0u); index < m_profiles.GetElementNumber (); ++index)
	{
		const YaPB::Profile &profile (m_profiles[index]);

		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%u: is used: %s, name: \"%s\", team: %s, model: [T=%u, CT=%u], skill=%u, aggression=%u, reaction time=%.2f, attack delay=%.2f, teamplay percent=%u, voice volume=%u, voice pitch=%u, personality: %s, ping: %i, left hand: %s, auto wepon switch: %s, logo: \"%s\", communication type=%u, HAL: [m_iProfileId=%u, m_szHAL_PreTrainFile: \"%s\", m_szHAL_AuxFile: \"%s\", m_szHAL_BanFile: \"%s\", m_szHAL_SwapFile: \"%s\"].\n", index + 1u, profile.isUsed ? "true" : "false", profile.name.GetData (), profile.team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? "T" : profile.team == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist ? "CT" : "random", profile.model[TeamArrayID_Terrorist], profile.model[TeamArrayID_CounterTerrorist], profile.skill.GetValue (), profile.aggression, profile.reactionTime, profile.attackDelay, profile.teamplayPercent, profile.voiceVolume, profile.voicePitch, profile.personality == Personality_Normal ? "normal" : profile.personality == Personality_Rusher ? "rusher" : "careful", profile.ping.GetValue (), profile.leftHand ? "true" : "false", profile.autoWeponSwitch ? "true" : "false", profile.logo.GetData (), profile.communicationType, profile.m_iProfileId, profile.m_szHAL_PreTrainFile.GetData (), profile.m_szHAL_AuxFile.GetData (), profile.m_szHAL_BanFile.GetData (), profile.m_szHAL_SwapFile.GetData ());
	}

	// Display the count.
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\tTotal is %u profiles.\n", m_profiles.GetElementNumber ());
}