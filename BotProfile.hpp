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
// BotProfile.hpp
//
// Class: Profile
//
// Description: YaPB profile item structure.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_PROFILE_INCLUDED
#define BOT_PROFILE_INCLUDED

#include <BotSkill.hpp>

enum WeaponPreferenceType_t
{
	WeaponPreferenceType_RifleT,
	WeaponPreferenceType_RifleCT,

	WeaponPreferenceType_PunchT,
	WeaponPreferenceType_PunchCT,

	WeaponPreferenceType_Sniper,

	WeaponPreferenceType_Power,

	WeaponPreferenceType_Shotgun,

	WeaponPreferenceType_Shield,

	WeaponPreferenceType_Spray
};

class Ping
{
	//
	// Group: Private members.
	//
	private:
		short m_originalValue;	// the displayed bot ping (min: 0, max: 4095), 'BotPingValue_BotTag' - print "BOT" in scoreboard.
		short m_value;			// the displayed bot ping (min: 0, max: 4095), 'BotPingValue_BotTag' - print "BOT" in scoreboard.

//		float m_updateTime;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Ping (const short value) :
			m_originalValue (value),
			m_value (value == BotPingValue_BotTag ? BotPingValue_BotTag : Math::GetClampedValueInRange <short> (value + g_randomNumberGenerator.GetValueBetween <short> (-Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PingFlux)->GetValue <unsigned short> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PingFlux)->GetValue <unsigned short> ()), 0u, BotPingValue_Maximum))
		{ /* VOID */ }

		inline Ping (const Ping &right) :
			m_originalValue (right.m_originalValue),
			m_value (right.m_value)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const short GetValue (void) const { return m_value; }

		inline void Calculate (void)
		{
			// Reliability check.
			InternalAssert (m_originalValue != BotPingValue_BotTag);
/*
			if (m_updateTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				return;

			m_updateTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 2.0f;
*/
			m_value = Math::GetClampedValueInRange <short> (m_originalValue + g_randomNumberGenerator.GetValueBetween <short> (-Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PingFlux)->GetValue <unsigned short> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PingFlux)->GetValue <unsigned short> ()), 0u, BotPingValue_Maximum);

		}

	//
	// Group: Operators.
	//
	public:
		//
		// Function: operator =
		//
		// Description: Assigns the value to ping.
		//
		// Parameters:
		//	value - Value that should be assigned.
		//
		// Returns: Reassigned ping.
		//
		inline Ping &operator = (const short value)
		{
			m_originalValue = value;
			m_value = value == BotPingValue_BotTag ? BotPingValue_BotTag : Math::GetClampedValueInRange <short> (value + g_randomNumberGenerator.GetValueBetween <short> (-Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PingFlux)->GetValue <unsigned short> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_PingFlux)->GetValue <unsigned short> ()), 0u, BotPingValue_Maximum);

			return *this;
		}
		inline Ping &operator = (const Ping &right)
		{
			m_originalValue = right.m_originalValue;
			m_value = right.m_value;

			return *this;
		}

		// casting operators
		inline operator const short (void) const { return m_value; }	// 'Ping' will now automatically convert to 'const short' when needed.
};

class Profile
{
	public:
	#define DEFAULT_YAPB_NAME "Fake client"

	bool                                        isUsed;						// whether this profile is used or not

	DynamicString                               name;						// name of this bot
	HalfLifeEngine::SDK::Constants::TeamID_t    team;						// preferred team for this bot
	unsigned char                               model[TeamArrayID_Total];	// preferred model for this bot
	Skill                                       skill;						// reference to bot 'Skill' class
	unsigned char                               aggression;					// Bot aggression (1 - careful, 50 - normal, 100 - rusher)
	float                                       reactionTime;				// reaction time
	float                                       attackDelay;				// delay after fire
	unsigned char                               teamplayPercent;			// chance to do teamwork
	unsigned char                               voiceVolume;				// bot voice volume (0-255 master volume)
	unsigned char                               voicePitch;					// bot voice pitch (base pitch percent (100% is normal pitch playback))
	float                                       soundVolume;				// console variable "volume" value for this bot (0-1 master volume)
	bool                                        hasStereo;					// bot has stereo speakers :)
	Personality_t                               personality;				// bot personality	/// @TODO: SHOULD BE REMOVED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Ping                                        ping;						// reference to bot 'Ping' class
	bool                                        leftHand;					// console variable "lefthand" value for this bot
	bool                                        autoWeponSwitch;			// console variable "_cl_autowepswitch" value for this bot
	DynamicString                               logo;						// the bot's preferred logo
	CommunicationType_t                         communicationType;			// bot communication type
	::ChatterManager::BanksArray_t::IndexType_t chatterBankIndex;			// Bot chatter bank index. ('BadPosition' if chatter disabled)
	float                                       baseReactTime;				// time to react on sent radio commands, chat messages, game events....
	float                                       baseAggressionLevel;		// base aggression level (on initializing)
	float                                       baseFearLevel;				// base fear level (on initializing)
	unsigned char                               explosiveGrenadeUsePercent;	// chance to using a explosive grenade
	unsigned char                               flashBangGrenadeUsePercent;	// chance to using a flash bang grenade
	unsigned char                               smokeGrenadeUsePercent;		// chance to using a smoke grenade
	struct WeaponsPreferences_t
	{
		typedef DynamicArray <HalfLifeEngine::SDK::Constants::WeaponID_t, unsigned char> Array_t;

		Array_t common;		// Bot common weapons preferences (sorted by priority (from best to worst))

		Array_t primary;	// Bot primary weapons preferences (sorted by priority (from best to worst))
		Array_t secondary;	// Bot secondary weapons preferences (sorted by priority (from best to worst))

		inline WeaponsPreferences_t (const Personality_t personality)
		{
			if (personality == Personality_Normal)
			{
				common += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
				common += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
				common += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M249;
				common += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
				common += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
				common += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
				common += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
				common += HalfLifeEngine::SDK::Constants::WeaponID_P90;
				common += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
				common += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M3;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
				common += HalfLifeEngine::SDK::Constants::WeaponID_USP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
				common += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
				common += HalfLifeEngine::SDK::Constants::WeaponID_P228;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Elite;
			}
			else if (personality == Personality_Rusher)
			{
				/// @todo MAKE WEAPONS PREFERENCES FOR RUSHER PERSONALITY!
				common += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
				common += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
				common += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M249;
				common += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
				common += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
				common += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
				common += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
				common += HalfLifeEngine::SDK::Constants::WeaponID_P90;
				common += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
				common += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M3;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
				common += HalfLifeEngine::SDK::Constants::WeaponID_USP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
				common += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
				common += HalfLifeEngine::SDK::Constants::WeaponID_P228;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Elite;
			}
			else
			{
				/// @todo MAKE WEAPONS PREFERENCES FOR CAREFUL PERSONALITY!
				common += HalfLifeEngine::SDK::Constants::WeaponID_AK47;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M4A1;
				common += HalfLifeEngine::SDK::Constants::WeaponID_SG552;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Aug;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Famas;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Galil;
				common += HalfLifeEngine::SDK::Constants::WeaponID_AWP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M249;
				common += HalfLifeEngine::SDK::Constants::WeaponID_G3SG1;
				common += HalfLifeEngine::SDK::Constants::WeaponID_SG550;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Scout;
				common += HalfLifeEngine::SDK::Constants::WeaponID_MP5;
				common += HalfLifeEngine::SDK::Constants::WeaponID_UMP45;
				common += HalfLifeEngine::SDK::Constants::WeaponID_P90;
				common += HalfLifeEngine::SDK::Constants::WeaponID_MAC10;
				common += HalfLifeEngine::SDK::Constants::WeaponID_TMP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_XM1014;
				common += HalfLifeEngine::SDK::Constants::WeaponID_M3;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Deagle;
				common += HalfLifeEngine::SDK::Constants::WeaponID_USP;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Shield;
				common += HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven;
				common += HalfLifeEngine::SDK::Constants::WeaponID_P228;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Glock18;
				common += HalfLifeEngine::SDK::Constants::WeaponID_Elite;
			}

			CreatePrimaryAndSecondaryArrays ();
		}
		inline WeaponsPreferences_t (const Array_t &inputCommon)
		{
			common = inputCommon;

			CreatePrimaryAndSecondaryArrays ();
		}
		inline WeaponsPreferences_t &operator = (const Array_t &inputCommon)
		{
			common = inputCommon;

			return *this;
		}

		inline void RecreatePrimaryAndSecondaryArrays (void)
		{
			primary.RemoveAll ();
			secondary.RemoveAll ();

			CreatePrimaryAndSecondaryArrays ();
		}

		private:
			inline void CreatePrimaryAndSecondaryArrays (void)
			{
				for (Array_t::IndexType_t index (0u); index < common.GetElementNumber (); ++index)
				{
					if (HalfLifeEngine::Utilities::WeaponIsPrimary (common[index]))
						primary += common[index];
					else if (HalfLifeEngine::Utilities::WeaponIsSecondary (common[index]))
						secondary += common[index];
				}
			}
	}                                           weaponsPreferences;			// Bot weapons preferences (sorted by priority (from best to worst))

	/************ FROM RCBot - START ************/
#define BOT_PROFILES_FOLDER "Profiles"

	int              m_iProfileId;

	/************ MegaHAL - START ************/
	DynamicString    m_szHAL_PreTrainFile;
	DynamicString    m_szHAL_AuxFile;
	DynamicString    m_szHAL_BanFile;
	DynamicString    m_szHAL_SwapFile;

	HAL_bot_t       *m_HAL;

void FreeHALBrain (void);
bool PrepareHALBrainForPersonality (void);
bool LoadHALBrainForPersonality (const bool bPreTrain);
void SaveHALBrainForPersonality (void);

inline void DeleteHALBrain (void)
{
	if (m_HAL == NULL)
		return;

	FreeHALBrain ();

	delete m_HAL;

	m_HAL = NULL;
}
	/************ MegaHAL - END ************/

//	CBotReputations  m_Rep;
	/************ FROM RCBot - END ************/

	inline Profile (void) :
		isUsed (false),

		name (DEFAULT_YAPB_NAME),
		team (static_cast <HalfLifeEngine::SDK::Constants::TeamID_t> (g_randomNumberGenerator.GetValueBetween <unsigned char> (HalfLifeEngine::SDK::Constants::TeamID_Terrorist, HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist))),
		skill (g_randomNumberGenerator.GetValueBetween (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_MinSkill)->GetValue <unsigned char> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_MaxSkill)->GetValue <unsigned char> ())),
		aggression (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u)),
		reactionTime (g_randomNumberGenerator.GetValueBetween (0.0f, 5.0f)),
		attackDelay (g_randomNumberGenerator.GetValueBetween (0.0f, 5.0f)),
		teamplayPercent (g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, 100u)),
		voiceVolume (g_randomNumberGenerator.GetValueBetween <unsigned char> (20u, 255u)),
		voicePitch (g_randomNumberGenerator.GetValueBetween <unsigned char> (83u, 125u)),
		soundVolume (g_randomNumberGenerator.GetValueBetween (0.0f, 1.0f)),
		hasStereo (g_randomNumberGenerator.GetValueBetween <bool> ()),
		personality (static_cast <const Personality_t> (g_randomNumberGenerator.GetValueBetween <unsigned char> (Personality_Normal, Personality_Careful))),
		ping (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Ping)->GetValue <short> ()),
		leftHand (g_randomNumberGenerator.GetValueBetween <bool> ()),
		autoWeponSwitch (g_randomNumberGenerator.GetValueBetween <bool> ()),
		logo ("{lambda06"),
		communicationType (static_cast <CommunicationType_t> (g_randomNumberGenerator.GetValueBetween <unsigned char> (CommunicationType_None, CommunicationType_Chatter))),
		chatterBankIndex (::ChatterManager::BankIndex_Default),
		baseReactTime (g_randomNumberGenerator.GetValueBetween (0.2f, 2.5f)),
		baseAggressionLevel (personality == Personality_Normal ? g_randomNumberGenerator.GetValueBetween (0.4f, 0.7f) : personality == Personality_Rusher ? g_randomNumberGenerator.GetValueBetween (0.7f, 1.0f) : g_randomNumberGenerator.GetValueBetween (0.0f, 0.4f)),
		baseFearLevel (personality == Personality_Normal ? g_randomNumberGenerator.GetValueBetween (0.4f, 0.7f) : personality == Personality_Rusher ? g_randomNumberGenerator.GetValueBetween (0.0f, 0.4f) : g_randomNumberGenerator.GetValueBetween (0.7f, 1.0f)),
		explosiveGrenadeUsePercent (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_ExplosiveGrenadeUsePercent)->GetValue <unsigned char> ()),
		flashBangGrenadeUsePercent (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_FlashBangGrenadeUsePercent)->GetValue <unsigned char> ()),
		smokeGrenadeUsePercent (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SmokeGrenadeUsePercent)->GetValue <unsigned char> ()),
		weaponsPreferences (personality),

		/************ FROM RCBot - START ************/
		m_iProfileId (0u),

		m_szHAL_PreTrainFile ("common_hal.trn"),
		m_szHAL_AuxFile ("common_hal.aux"),
		m_szHAL_BanFile ("common_hal.ban"),
		m_szHAL_SwapFile ("common_hal.swp"),

		m_HAL (NULL)
		/************ FROM RCBot - END ************/
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModID () == HalfLifeEngine::Engine::Version_Steam)
		{
			model[TeamArrayID_Terrorist]        = g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, HalfLifeEngine::SDK::Constants::MaximumPlayerModels);
			model[TeamArrayID_CounterTerrorist] = g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, HalfLifeEngine::SDK::Constants::MaximumPlayerModels);
		}
		else
		{
			// Condition Zero has spetsnaz and militia skins....
			model[TeamArrayID_Terrorist]        = g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, HalfLifeEngine::SDK::Constants::MaximumPlayerModels_ConditionZero);
			model[TeamArrayID_CounterTerrorist] = g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, HalfLifeEngine::SDK::Constants::MaximumPlayerModels_ConditionZero);
		}
	}
};

#endif	// ifndef BOT_PROFILE_INCLUDED