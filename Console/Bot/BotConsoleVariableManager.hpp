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
// BotConsoleVariableManager.hpp
//
// Class: VariableManager
//
// Description: Simple bot console variables container class (defined for more convenient work with console variables).
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_CONSOLE_VARIABLE_MANAGER_INCLUDED
#define BOT_CONSOLE_VARIABLE_MANAGER_INCLUDED

class VariableManager;

extern DLL_GLOBAL VariableManager variableManager;

class VariableManager : public Console::VariableManager
{
	//
	// Group: Private members.
	//
	private:
		#include <Console/Bot/Calling_Con_Printf_VariableValueIsHooker.hpp>
		#include <Console/Bot/ExtendedConsoleVariable.hpp>

		static ExtendedConsoleVariable                 sm_variableList[VariableID_Total];

		HalfLifeEngine::SDK::Classes::ConsoleVariable *m_consoleVariables[VariableID_Total];

		Calling_Con_Printf_VariableValueIsHooker       m_calling_Con_Printf_VariableValueIsHooker;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline VariableManager (void)
		{
			// Initialize the console variables array....
			for (unsigned char index (VariableID_AutoVacate); index < VariableID_Total; ++index)
				m_consoleVariables[index] = sm_variableList + index;
		}

	//
	// Group: Private functions.
	//
	private:
		// On value change callbacks....

		// Bot console variables callbacks....
		static inline void OnConsoleVariableStringChange_AutoVacate (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_AutoVacate), newString);
		}
		static inline void OnConsoleVariableStringChange_Quota (const char *const newString)
		{
/*			const int newValue (static_cast <int> (atof (newString)));

			// Check valid range of quota....
			if (newValue < 0u)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater or equal zero!", variableManager.GetVariable (VariableID_Quota)->GetName ().GetData ());

//				newString = "0";	/// @remind NEED?!?!?!?!?!?!?!??!?!?!?!?!??!!

				// Fail!
				return;
			}
			else if (newValue > HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ())
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be less or equal %u!", variableManager.GetVariable (VariableID_Quota)->GetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ());

				// Fail!
				return;
			}
*/
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Quota), newString);
		}
		static inline void OnConsoleVariableStringChange_QuotaMode (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_QuotaMode), newString);
		}
		static inline void OnConsoleVariableStringChange_MinSkill (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check the minimum skills value range....
			if (newValue < BotSkillValue_Minimum)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater or equal %u!", variableManager.GetVariable (VariableID_MinSkill)->GetName ().GetData (), BotSkillValue_Minimum);

				// Fail!
				return;
			}
			else if (newValue > BotSkillValue_Maximum)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be less or equal %u!", variableManager.GetVariable (VariableID_MinSkill)->GetName ().GetData (), BotSkillValue_Maximum);

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_MinSkill), newString);
		}
		static inline void OnConsoleVariableStringChange_MaxSkill (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check the maximum skills value range....
			if (newValue < BotSkillValue_Minimum)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater or equal %u!", variableManager.GetVariable (VariableID_MaxSkill)->GetName ().GetData (), BotSkillValue_Minimum);

				// Fail!
				return;
			}
			else if (newValue > BotSkillValue_Maximum)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be less or equal %u!", variableManager.GetVariable (VariableID_MaxSkill)->GetName ().GetData (), BotSkillValue_Maximum);

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_MaxSkill), newString);
		}
		static inline void OnConsoleVariableStringChange_FollowUser (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_FollowUser), newString);
		}
		static inline void OnConsoleVariableStringChange_TimerSound (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_TimerSound), newString);
		}
		static inline void OnConsoleVariableStringChange_TimerPickup (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_TimerPickup), newString);
		}
		static inline void OnConsoleVariableStringChange_TimerGrenade (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_TimerGrenade), newString);
		}
		static inline void OnConsoleVariableStringChange_DebugGoal (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_DebugGoal), newString);
		}
		static inline void OnConsoleVariableStringChange_Chat (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Chat), newString);
		}
		static inline void OnConsoleVariableStringChange_Synth (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Synth), newString);
		}
		static inline void OnConsoleVariableStringChange_KnifeMode (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_KnifeMode), newString);
		}
		static inline void OnConsoleVariableStringChange_SkillTags (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SkillTags), newString);
		}
		static inline void OnConsoleVariableStringChange_Stop (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Stop), newString);
		}
		static inline void OnConsoleVariableStringChange_ThruWalls (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_ThruWalls), newString);
		}
		static inline void OnConsoleVariableStringChange_WallSensitivity (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_WallSensitivity), newString);
		}
		static inline void OnConsoleVariableStringChange_Votes (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Votes), newString);
		}
		static inline void OnConsoleVariableStringChange_Spray (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Spray), newString);
		}
		static inline void OnConsoleVariableStringChange_BotBuy (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_BotBuy), newString);
		}

		#if defined _DEBUG
			static inline void OnConsoleVariableStringChange_Debug (const char *const newString)
			{
				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Debug), newString);
			}
		#endif	// if defined _DEBUG

		static inline void OnConsoleVariableStringChange_Prefix (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Prefix), newString);
		}
		static inline void OnConsoleVariableStringChange_Language (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Language), newString);
		}
		static inline void OnConsoleVariableStringChange_EnableClientLanguages (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_EnableClientLanguages), newString);
		}
		static inline void OnConsoleVariableStringChange_Version (const char *const/* newString*/)
		{
			// Shouldn't happen!!!
			InternalAssert (false);
		}
		static inline void OnConsoleVariableStringChange_NavPointLookMethod (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check ranges....
			if (newValue <= VariableValue_NavPointLookMethod_None || newValue >= VariableValue_NavPointLookMethod_Total)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be in range %u...%u!", variableManager.GetVariable (VariableID_NavPointLookMethod)->GetName ().GetData (), VariableValue_NavPointLookMethod_None + 1u, VariableValue_NavPointLookMethod_Total - 1u);

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_NavPointLookMethod), newString);
		}
		static inline void OnConsoleVariableStringChange_AimMethod (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check ranges....
			if (newValue <= VariableValue_AimMethod_None || newValue >= VariableValue_AimMethod_Total)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be in range %u...%u!", variableManager.GetVariable (VariableID_AimMethod)->GetName ().GetData (), VariableValue_AimMethod_None + 1u, VariableValue_AimMethod_Total - 1u);

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_AimMethod), newString);
		}
		static inline void OnConsoleVariableStringChange_TargetingMethod (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check ranges....
			if (newValue <= VariableValue_TargetingMethod_None || newValue >= VariableValue_TargetingMethod_Total)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be in range %u...%u!", variableManager.GetVariable (VariableID_TargetingMethod)->GetName ().GetData (), VariableValue_TargetingMethod_None + 1u, VariableValue_TargetingMethod_Total - 1u);

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_TargetingMethod), newString);
		}
		static inline void OnConsoleVariableStringChange_DamperCoeff_X (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_DamperCoeff_X), newString);
		}
		static inline void OnConsoleVariableStringChange_DamperCoeff_Y (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_DamperCoeff_Y), newString);
		}
		static inline void OnConsoleVariableStringChange_Deviation_X (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Deviation_X), newString);
		}
		static inline void OnConsoleVariableStringChange_Deviation_Y (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Deviation_Y), newString);
		}
		static inline void OnConsoleVariableStringChange_Influence_XonY (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Influence_XonY), newString);
		}
		static inline void OnConsoleVariableStringChange_Influence_YonX (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Influence_YonX), newString);
		}
		static inline void OnConsoleVariableStringChange_SlowdownRatio (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SlowdownRatio), newString);
		}
		static inline void OnConsoleVariableStringChange_OffsetDelay (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_OffsetDelay), newString);
		}
		static inline void OnConsoleVariableStringChange_SpringStiff_X (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SpringStiff_X), newString);
		}
		static inline void OnConsoleVariableStringChange_SpringStiff_Y (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SpringStiff_Y), newString);
		}
		static inline void OnConsoleVariableStringChange_AnticipRatio (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_AnticipRatio), newString);
		}
		static inline void OnConsoleVariableStringChange_Password (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Password), newString);
		}
		static inline void OnConsoleVariableStringChange_PasswordKey (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_PasswordKey), newString);
		}
		static inline void OnConsoleVariableStringChange_ChatterPath (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_ChatterPath), newString);
		}
		static inline void OnConsoleVariableStringChange_TKPunish (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_TKPunish), newString);
		}
		static void OnConsoleVariableStringChange_Communication (const char *const newString);
		static inline void OnConsoleVariableStringChange_Economics (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Economics), newString);
		}
		static inline void OnConsoleVariableStringChange_ForceTeam (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_ForceTeam), newString);
		}
		static inline void OnConsoleVariableStringChange_RestrictedWeapons (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_RestrictedWeapons), newString);
		}
		static inline void OnConsoleVariableStringChange_DangerFactor (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_DangerFactor), newString);
		}
		static inline void OnConsoleVariableStringChange_DontShoot (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_DontShoot), newString);
		}
		static inline void OnConsoleVariableStringChange_HardcoreMode (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_HardcoreMode), newString);
		}
		static inline void OnConsoleVariableStringChange_AutofollowPercent (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_AutofollowPercent), newString);
		}
		static inline void OnConsoleVariableStringChange_ExplosiveGrenadeUsePercent (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_ExplosiveGrenadeUsePercent), newString);
		}
		static inline void OnConsoleVariableStringChange_FlashBangGrenadeUsePercent (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_FlashBangGrenadeUsePercent), newString);
		}
		static inline void OnConsoleVariableStringChange_SmokeGrenadeUsePercent (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SmokeGrenadeUsePercent), newString);
		}
		static inline void OnConsoleVariableStringChange_LogoTypes (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_LogoTypes), newString);
		}

		#if defined MULTIPLE_MSEC_METHODS
			static inline void OnConsoleVariableStringChange_MsecMethod (const char *const newString)
			{
				const int newValue (static_cast <int> (atof (newString)));

				// Check ranges....
				if (newValue <= VariableValue_MsecMethod_None || newValue >= VariableValue_MsecMethod_Total)
				{
					/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
					AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be in range %u...%u!", variableManager.GetVariable (VariableID_MsecMethod)->GetName ().GetData (), VariableValue_MsecMethod_None + 1u, VariableValue_MsecMethod_Total - 1u);

					// Fail!
					return;
				}

				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_MsecMethod), newString);
			}
		#endif	// if defined MULTIPLE_MSEC_METHODS

		static inline void OnConsoleVariableStringChange_LogLevel (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_LogLevel), newString);
		}
		static inline void OnConsoleVariableStringChange_Config_LoadTime (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Config_LoadTime), newString);
		}
		static inline void OnConsoleVariableStringChange_WalkAllowed (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_WalkAllowed), newString);
		}
		static inline void OnConsoleVariableStringChange_Deathmatch (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Deathmatch), newString);
		}
		static inline void OnConsoleVariableStringChange_ThinkFPS (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check ranges....
			if (newValue <= 0)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater than zero!", variableManager.GetVariable (VariableID_ThinkFPS)->GetName ().GetData ());

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_ThinkFPS), newString);
		}
		static inline void OnConsoleVariableStringChange_Ping (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check the ping value range....
			if (newValue < BotPingValue_BotTag)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater or equal %i!", variableManager.GetVariable (VariableID_Ping)->GetName ().GetData (), BotPingValue_BotTag);

				// Fail!
				return;
			}
			else if (newValue > BotPingValue_Maximum)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be less or equal %u!", variableManager.GetVariable (VariableID_Ping)->GetName ().GetData (), BotPingValue_Maximum);

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Ping), newString);
		}
		static inline void OnConsoleVariableStringChange_PingFlux (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check the ping flux value range....
			if (newValue < 0)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater or equal zero!", variableManager.GetVariable (VariableID_PingFlux)->GetName ().GetData ());

				// Fail!
				return;
			}
			else if (newValue > BotPingValue_Maximum)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be less or equal %u!", variableManager.GetVariable (VariableID_PingFlux)->GetName ().GetData (), BotPingValue_Maximum);

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_PingFlux), newString);
		}
		static inline void OnConsoleVariableStringChange_DeferToHuman (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_DeferToHuman), newString);
		}

		// Voice system....
		static inline void OnConsoleVariableStringChange_Voice_inputfromfile (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_inputfromfile), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_recordtofile (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_recordtofile), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_avggain (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_avggain), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_maxgain (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_maxgain), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_scale (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_scale), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_loopback (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_loopback), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_fadeouttime (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_fadeouttime), newString);
		}

		// Debugging cvars.
		static inline void OnConsoleVariableStringChange_Voice_profile (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_profile), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_showchannels (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_showchannels), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_showincoming (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_showincoming), newString);
		}

		static inline void OnConsoleVariableStringChange_Voice_forcemicrecord (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_forcemicrecord), newString);
		}

		static inline void OnConsoleVariableStringChange_Voice_overdrive (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_overdrive), newString);
		}
		static inline void OnConsoleVariableStringChange_Voice_overdrivefadetime (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_overdrivefadetime), newString);
		}

		static inline void OnConsoleVariableStringChange_MaximumAFKTime (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Check the ping flux value range....
			if (newValue < 10)
			{
				/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater or equal 10!", variableManager.GetVariable (VariableID_MaximumAFKTime)->GetName ().GetData ());

				// Fail!
				return;
			}

			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_MaximumAFKTime), newString);
		}

		#if defined MULTIPLE_DRAW_METHODS
			static inline void OnConsoleVariableStringChange_DrawMethod (const char *const newString)
			{
				const int newValue (static_cast <int> (atof (newString)));

				if (newValue != VariableValue_DrawMethod_Default && HalfLifeEngine::Globals::g_halfLifeEngine->IsSoftwareRenderingMode ())
				{
					/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
					AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' can't be applied in dedicated server!", variableManager.GetVariable (VariableID_DrawMethod)->GetName ().GetData ());

					// Fail!
					return;
				}

				// Check ranges....
				if (newValue <= VariableValue_DrawMethod_None || newValue >= VariableValue_DrawMethod_Total)
				{
					/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
					AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be in range %u...%u!", variableManager.GetVariable (VariableID_DrawMethod)->GetName ().GetData (), VariableValue_DrawMethod_None + 1, VariableValue_DrawMethod_Total - 1u);

					// Fail!
					return;
				}

				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_DrawMethod), newString);
			}
		#endif	// if defined MULTIPLE_DRAW_METHODS

		// Navigation mesh console variables callbacks....
		static inline void OnConsoleVariableStringChange_nav_edit (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_edit), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_quicksave (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_quicksave), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_hiding_spots (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_hiding_spots), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_approach_points (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_approach_points), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_danger (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_danger), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_player_counts (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_player_counts), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_occupy_time (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_occupy_time), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_battlefront (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_battlefront), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_area_info (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_area_info), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_snap_to_grid (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_snap_to_grid), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_create_place_on_ground (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_create_place_on_ground), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_create_area_at_feet (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_create_area_at_feet), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_nodes (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_nodes), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_show_compass (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_compass), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_slope_limit (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_slope_limit), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_slope_tolerance (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_slope_tolerance), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_restart_after_analysis (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_restart_after_analysis), newString);
		}

		#if defined DEBUG_NAV_NODES
			static inline void OnConsoleVariableStringChange_nav_show_node_id (const char *const newString)
			{
				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_show_node_id), newString);
			}
			static inline void OnConsoleVariableStringChange_nav_test_node (const char *const newString)
			{
				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_test_node), newString);
			}
			static inline void OnConsoleVariableStringChange_nav_test_node_crouch (const char *const newString)
			{
				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_test_node_crouch), newString);
			}
			static inline void OnConsoleVariableStringChange_nav_test_node_crouch_dir (const char *const newString)
			{
				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_test_node_crouch_dir), newString);
			}
		#endif	// if defined DEBUG_NAV_NODES

		static inline void OnConsoleVariableStringChange_nav_coplanar_slope_limit (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_coplanar_slope_limit), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_split_place_on_ground (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_split_place_on_ground), newString);
		}
		#if defined MULTIPLE_DRAW_METHODS
			static inline void OnConsoleVariableStringChange_nav_area_bgcolor (const char *const newString)
			{
				const int newValue (static_cast <int> (atof (newString)));

				// Check the area bgcolor value range....
				if (newValue < 0)
				{
					/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
					AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be greater or equal 0!", variableManager.GetVariable (VariableID_nav_area_bgcolor)->GetName ().GetData ());

					// Fail!
					return;
				}
				else if (newValue > 255)
				{
					/// @todo INVOLVE 'g_localizer' INSEAD OF IT!!!!!
					AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be less or equal 255!", variableManager.GetVariable (VariableID_nav_area_bgcolor)->GetName ().GetData ());

					// Fail!
					return;
				}

				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_area_bgcolor), newString);
			}
		#endif	// if defined MULTIPLE_DRAW_METHODS
		static inline void OnConsoleVariableStringChange_nav_area_max_size (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_area_max_size), newString);
		}
		static inline void OnConsoleVariableStringChange_nav_corner_adjust_adjacent (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_nav_corner_adjust_adjacent), newString);
		}

		static inline void OnConsoleVariableStringChange_NavigationMesh_MaximumOffPlaneTolerance (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_NavigationMesh_MaximumOffPlaneTolerance), newString);
		}
		static inline void OnConsoleVariableStringChange_NavigationMesh_MaximumSampleStepTime (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_NavigationMesh_MaximumSampleStepTime), newString);
		}

		static inline void OnConsoleVariableStringChange_NavigationMesh_GenerateFenceTopsAreas (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_NavigationMesh_GenerateFenceTopsAreas), newString);
		}
		static inline void OnConsoleVariableStringChange_NavigationMesh_SquareUpAreas (const char *const newString)
		{
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_NavigationMesh_SquareUpAreas), newString);
		}

		// On print help callbacks.... (Note: The below functions are declared in Localizer.hpp)

		// Bot console variables callbacks....
		static inline void OnConsoleVariablePrintHelp_AutoVacate                              (void);
		static inline void OnConsoleVariablePrintHelp_Quota                                   (void);
		static inline void OnConsoleVariablePrintHelp_QuotaMode                               (void);
		static inline void OnConsoleVariablePrintHelp_MinSkill                                (void);
		static inline void OnConsoleVariablePrintHelp_MaxSkill                                (void);
		static inline void OnConsoleVariablePrintHelp_FollowUser                              (void);
		static inline void OnConsoleVariablePrintHelp_TimerSound                              (void);
		static inline void OnConsoleVariablePrintHelp_TimerPickup                             (void);
		static inline void OnConsoleVariablePrintHelp_TimerGrenade                            (void);
		static inline void OnConsoleVariablePrintHelp_DebugGoal                               (void);
		static inline void OnConsoleVariablePrintHelp_Chat                                    (void);
		static inline void OnConsoleVariablePrintHelp_Synth                                   (void);
		static inline void OnConsoleVariablePrintHelp_KnifeMode                               (void);
		static inline void OnConsoleVariablePrintHelp_SkillTags                               (void);
		static inline void OnConsoleVariablePrintHelp_Stop                                    (void);
		static inline void OnConsoleVariablePrintHelp_ThruWalls                               (void);
		static inline void OnConsoleVariablePrintHelp_WallSensitivity                         (void);
		static inline void OnConsoleVariablePrintHelp_Votes                                   (void);
		static inline void OnConsoleVariablePrintHelp_Spray                                   (void);
		static inline void OnConsoleVariablePrintHelp_BotBuy                                  (void);

		#if defined _DEBUG
			static inline void OnConsoleVariablePrintHelp_Debug                               (void);
		#endif	// if defined _DEBUG

		static inline void OnConsoleVariablePrintHelp_Prefix                                  (void);
		static inline void OnConsoleVariablePrintHelp_Language                                (void);
		static inline void OnConsoleVariablePrintHelp_EnableClientLanguages                   (void);
		static inline void OnConsoleVariablePrintHelp_Version                                 (void);
		static inline void OnConsoleVariablePrintHelp_NavPointLookMethod                      (void);
		static inline void OnConsoleVariablePrintHelp_AimMethod                               (void);
		static inline void OnConsoleVariablePrintHelp_TargetingMethod                         (void);
		static inline void OnConsoleVariablePrintHelp_DamperCoeff_X                           (void);
		static inline void OnConsoleVariablePrintHelp_DamperCoeff_Y                           (void);
		static inline void OnConsoleVariablePrintHelp_Deviation_X                             (void);
		static inline void OnConsoleVariablePrintHelp_Deviation_Y                             (void);
		static inline void OnConsoleVariablePrintHelp_Influence_XonY                          (void);
		static inline void OnConsoleVariablePrintHelp_Influence_YonX                          (void);
		static inline void OnConsoleVariablePrintHelp_SlowdownRatio                           (void);
		static inline void OnConsoleVariablePrintHelp_OffsetDelay                             (void);
		static inline void OnConsoleVariablePrintHelp_SpringStiff_X                           (void);
		static inline void OnConsoleVariablePrintHelp_SpringStiff_Y                           (void);
		static inline void OnConsoleVariablePrintHelp_AnticipRatio                            (void);
		static inline void OnConsoleVariablePrintHelp_Password                                (void);
		static inline void OnConsoleVariablePrintHelp_PasswordKey                             (void);
		static inline void OnConsoleVariablePrintHelp_ChatterPath                             (void);
		static inline void OnConsoleVariablePrintHelp_TKPunish                                (void);
		static inline void OnConsoleVariablePrintHelp_Communication                           (void);
		static inline void OnConsoleVariablePrintHelp_Economics                               (void);
		static inline void OnConsoleVariablePrintHelp_ForceTeam                               (void);
		static inline void OnConsoleVariablePrintHelp_RestrictedWeapons                       (void);
		static inline void OnConsoleVariablePrintHelp_DangerFactor                            (void);
		static inline void OnConsoleVariablePrintHelp_DontShoot                               (void);
		static inline void OnConsoleVariablePrintHelp_HardcoreMode                            (void);
		static inline void OnConsoleVariablePrintHelp_AutofollowPercent                       (void);
		static inline void OnConsoleVariablePrintHelp_ExplosiveGrenadeUsePercent              (void);
		static inline void OnConsoleVariablePrintHelp_FlashBangGrenadeUsePercent              (void);
		static inline void OnConsoleVariablePrintHelp_SmokeGrenadeUsePercent                  (void);
		static inline void OnConsoleVariablePrintHelp_LogoTypes                               (void);

		#if defined MULTIPLE_MSEC_METHODS
			static inline void OnConsoleVariablePrintHelp_MsecMethod                          (void);
		#endif	// if defined MULTIPLE_MSEC_METHODS

		static inline void OnConsoleVariablePrintHelp_LogLevel                                (void);
		static inline void OnConsoleVariablePrintHelp_Config_LoadTime                         (void);
		static inline void OnConsoleVariablePrintHelp_WalkAllowed                             (void);
		static inline void OnConsoleVariablePrintHelp_Deathmatch                              (void);
		static inline void OnConsoleVariablePrintHelp_ThinkFPS                                (void);
		static inline void OnConsoleVariablePrintHelp_Ping                                    (void);
		static inline void OnConsoleVariablePrintHelp_PingFlux                                (void);
		static inline void OnConsoleVariablePrintHelp_DeferToHuman                            (void);

		// Voice system....
		static inline void OnConsoleVariablePrintHelp_Voice_inputfromfile                     (void);
		static inline void OnConsoleVariablePrintHelp_Voice_recordtofile                      (void);
		static inline void OnConsoleVariablePrintHelp_Voice_avggain                           (void);
		static inline void OnConsoleVariablePrintHelp_Voice_maxgain                           (void);
		static inline void OnConsoleVariablePrintHelp_Voice_scale                             (void);
		static inline void OnConsoleVariablePrintHelp_Voice_loopback                          (void);
		static inline void OnConsoleVariablePrintHelp_Voice_fadeouttime                       (void);

		// Debugging cvars.
		static inline void OnConsoleVariablePrintHelp_Voice_profile                           (void);
		static inline void OnConsoleVariablePrintHelp_Voice_showchannels                      (void);
		static inline void OnConsoleVariablePrintHelp_Voice_showincoming                      (void);

		static inline void OnConsoleVariablePrintHelp_Voice_forcemicrecord                    (void);

		static inline void OnConsoleVariablePrintHelp_Voice_overdrive                         (void);
		static inline void OnConsoleVariablePrintHelp_Voice_overdrivefadetime                 (void);

		static inline void OnConsoleVariablePrintHelp_MaximumAFKTime                          (void);

		#if defined MULTIPLE_DRAW_METHODS
			static inline void OnConsoleVariablePrintHelp_DrawMethod                          (void);
		#endif	// if defined MULTIPLE_DRAW_METHODS

		// Navigation mesh console variables callbacks....
		static inline void OnConsoleVariablePrintHelp_nav_edit                                (void);
		static inline void OnConsoleVariablePrintHelp_nav_quicksave                           (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_hiding_spots                   (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_approach_points                (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_danger                         (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_player_counts                  (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_occupy_time                    (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_battlefront                    (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_area_info                      (void);
		static inline void OnConsoleVariablePrintHelp_nav_snap_to_grid                        (void);
		static inline void OnConsoleVariablePrintHelp_nav_create_place_on_ground              (void);
		static inline void OnConsoleVariablePrintHelp_nav_create_area_at_feet                 (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_nodes                          (void);
		static inline void OnConsoleVariablePrintHelp_nav_show_compass                        (void);
		static inline void OnConsoleVariablePrintHelp_nav_slope_limit                         (void);
		static inline void OnConsoleVariablePrintHelp_nav_slope_tolerance                     (void);
		static inline void OnConsoleVariablePrintHelp_nav_restart_after_analysis              (void);

		#if defined DEBUG_NAV_NODES
			static inline void OnConsoleVariablePrintHelp_nav_show_node_id                    (void);
			static inline void OnConsoleVariablePrintHelp_nav_test_node                       (void);
			static inline void OnConsoleVariablePrintHelp_nav_test_node_crouch                (void);
			static inline void OnConsoleVariablePrintHelp_nav_test_node_crouch_dir            (void);
		#endif	// if defined DEBUG_NAV_NODES

		static inline void OnConsoleVariablePrintHelp_nav_coplanar_slope_limit                (void);
		static inline void OnConsoleVariablePrintHelp_nav_split_place_on_ground               (void);
		#if defined MULTIPLE_DRAW_METHODS
			static inline void OnConsoleVariablePrintHelp_nav_area_bgcolor                    (void);
		#endif	// if defined MULTIPLE_DRAW_METHODS
		static inline void OnConsoleVariablePrintHelp_nav_area_max_size                       (void);
		static inline void OnConsoleVariablePrintHelp_nav_corner_adjust_adjacent              (void);

		static inline void OnConsoleVariablePrintHelp_NavigationMesh_MaximumOffPlaneTolerance (void);
		static inline void OnConsoleVariablePrintHelp_NavigationMesh_MaximumSampleStepTime    (void);

		static inline void OnConsoleVariablePrintHelp_NavigationMesh_GenerateFenceTopsAreas   (void);
		static inline void OnConsoleVariablePrintHelp_NavigationMesh_SquareUpAreas            (void);

	//
	// Group: Functions.
	//
	public:
		inline       Calling_Con_Printf_VariableValueIsHooker &GetCalling_Con_Printf_VariableValueIsHooker (void)       { return m_calling_Con_Printf_VariableValueIsHooker; }
		inline const Calling_Con_Printf_VariableValueIsHooker &GetCalling_Con_Printf_VariableValueIsHooker (void) const { return m_calling_Con_Printf_VariableValueIsHooker; }

		void RegisterVariables (void);

		// Console variable access...

		//
		// Function: GetVariable
		//
		// Description: Gets console variable from specified index.
		//
		// Parameters:
		//	index - Console variable index to retrieve.
		//
		// Returns: Console variable object.
		//
		inline HalfLifeEngine::SDK::Classes::ConsoleVariable *const &GetVariable (const VariableID_t index) const
		{
			// Reliability checks.
			InternalAssert (index >= VariableID_AutoVacate && index < VariableID_Total);
			InternalAssert (m_consoleVariables[index]->IsValid ());

			return m_consoleVariables[index];
		}

		static inline ExtendedConsoleVariable &GetExtendedVariable (const VariableID_t index)
		{
			// Reliability checks.
			InternalAssert (index >= VariableID_AutoVacate && index < VariableID_Total);

			return sm_variableList[index];
		}
};

#endif	// ifndef BOT_CONSOLE_VARIABLE_MANAGER_INCLUDED