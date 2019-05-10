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
// ServerConsoleVariableManager.cpp
//
// Class: VariableManager
//
// Description: Simple server console variables manager class.
//
// Version: $ID:$
//

#include <Core.hpp>

namespace Console
{
	namespace Server
	{
		DLL_GLOBAL VariableManager variableManager;

		#define INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK(name, nameString)																\
			m_consoleVariables[VariableID_##name] = HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetPointer (nameString);			\
			m_onStringChangeCallbackFunctionPointer[VariableID_##name] = NULL;																	\
																																				\
			/* Reliability check.... */																											\
			InternalAssert (m_consoleVariables[VariableID_##name]->IsValid ());

		#define INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK(name, nameString)														\
			m_consoleVariables[VariableID_##name] = HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetPointer (nameString);	\
			m_onStringChangeCallbackFunctionPointer[VariableID_##name] = OnConsoleVariableStringChange_##name;							\
																																		\
			/* Reliability check.... */																									\
			InternalAssert (m_consoleVariables[VariableID_##name]->IsValid ());

		void VariableManager::RegisterVariables (void)
		{
			// since this function is called after original DLL function called, we can get pointer of console variables here...
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (Gravity,        "sv_gravity");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (Developer,      "developer");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK    (VoiceEnable,    "sv_voiceenable");	// Globally enable or disable voice.
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK    (Voice_codec,    "sv_voicecodec");	// Specifies which voice codec DLL to use in a game. Set to the name of the DLL without the extension.
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK    (Voice_quality,  "sv_voicequality");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK    (SkyColor_Red,   "sv_skycolor_r");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK    (SkyColor_Green, "sv_skycolor_g");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK    (SkyColor_Blue,  "sv_skycolor_b");
		}
		void VariableManager::RegisterGameVariables (void)
		{
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (C4Timer,        "mp_c4timer");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (BuyTime,        "mp_buytime");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (FriendlyFire,   "mp_friendlyfire");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (RoundTime,      "mp_roundtime");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (FreezeTime,     "mp_freezetime");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (FootSteps,      "mp_footsteps");
			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (AllTalk,        "sv_alltalk");
//			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (FreeForAll,     "mp_freeforall");

//			INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK (CSDMActive,     "csdm_active");
		}

		#undef INITIALIZE_CONSOLE_VARIABLE_ITEM_WITHOUT_CALLBACK
		#undef INITIALIZE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACK

		// On value change callbacks....

		/*static */void VariableManager::OnConsoleVariableStringChange_VoiceEnable (const char *const newString)
		{
			if (g_server->IsRunning ())
			{
				if (atof (newString) == 0.0f)
				{
					if (g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized ())
						g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Deinitialize ();
				}
				else if (Bot::variableManager.GetVariable (Bot::VariableID_Communication)->GetValue <Bot::VariableValue_Communication_t> () == Bot::VariableValue_Communication_VoiceNew && !g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized ())
				{
					if (g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialize (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_codec)->GetString ()))
						g_server->GetYaPBManager ()->OnChangeCodec (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_quality)->GetValue <unsigned char> ());
				}
			}

			// Call the original function....
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_VoiceEnable), newString);
		}
		/*static */void VariableManager::OnConsoleVariableStringChange_Voice_codec (const char *const newString)
		{
			if (g_server->IsRunning ())
			{
				if (!variableManager.GetVariable (VariableID_VoiceEnable)->GetValue <bool> () || Bot::variableManager.GetVariable (Bot::VariableID_Communication)->GetValue <Bot::VariableValue_Communication_t> () != Bot::VariableValue_Communication_VoiceNew)
				{
					// Reliability check....
					InternalAssert (!g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized ());
				}
				else if (g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().GetCodecName ().CompareWithoutCaseCheck (newString) != 0)
					g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().ChangeCodec (newString);
			}

			// Call the original function....
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_codec), newString);
		}
		/*static */void VariableManager::OnConsoleVariableStringChange_Voice_quality (const char *const newString)
		{
			if (g_server->IsRunning ())
			{
				if (!variableManager.GetVariable (VariableID_VoiceEnable)->GetValue <bool> () || Bot::variableManager.GetVariable (Bot::VariableID_Communication)->GetValue <Bot::VariableValue_Communication_t> () != Bot::VariableValue_Communication_VoiceNew)
				{
					// Reliability check....
					InternalAssert (!g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized ());

					goto Label_CallOriginalFunctionAndReturn;
				}

				const int newValue (static_cast <int> (atof (newString)));

				if (g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().GetQuality () != static_cast <unsigned char> (newValue))
					g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().ChangeQuality (static_cast <unsigned char> (newValue));
/*
				// Check the quality value range....
				if (newValue < 0)
				{
					AddLogEntry (true, LogLevel_Error, false, "Value of '%s' should be greater or equal zero!", variableManager.GetVariable (VariableID_Voice_quality)->GetName ().GetData ());

					// Fail!
					return;
				}
				else if (newValue > VariableValue_Voice_quality_Total)
				{
					AddLogEntry (true, LogLevel_Error, false, "Value of '%s' should be less or equal %u!", variableManager.GetVariable (VariableID_Voice_quality)->GetName ().GetData (), VariableValue_Voice_quality_Total);

					// Fail!
					return;
				}
	/*! DO WE NEED THIS? (I THINK THAT quality IS UNUSED IN CODEC (voice_miles, other not cheked)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
				if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () && g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().GetQuality () != static_cast <unsigned char> (newValue))
				{
					FOR EACH BOT....
						if (g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().InitializedCodec ())
							g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().DeinitializeCodec ();

						g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().InitializeCodec (static_cast <unsigned char> (newValue));
				}*/
			}

			Label_CallOriginalFunctionAndReturn:
				// Call the original function....
				g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Voice_quality), newString);
		}

		/*static */void VariableManager::OnConsoleVariableStringChange_SkyColor_Red (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Update sky light level value....
			g_skyLightLevel = static_cast <unsigned char> ((Math::GetClampedValueInRange (newValue, 0, 255) + Math::GetClampedValueInRange (variableManager.GetVariable (VariableID_SkyColor_Green)->GetValue <int> (), 0, 255) + Math::GetClampedValueInRange (variableManager.GetVariable (VariableID_SkyColor_Blue)->GetValue <int> (), 0, 255)) / Color <>::ID_Total);

			// Call the original function....
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SkyColor_Red), newString);
		}
		/*static */void VariableManager::OnConsoleVariableStringChange_SkyColor_Green (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Update sky light level value....
			g_skyLightLevel = static_cast <unsigned char> ((Math::GetClampedValueInRange (variableManager.GetVariable (VariableID_SkyColor_Red)->GetValue <int> (), 0, 255) + Math::GetClampedValueInRange (newValue, 0, 255) + Math::GetClampedValueInRange (variableManager.GetVariable (VariableID_SkyColor_Blue)->GetValue <int> (), 0, 255)) / Color <>::ID_Total);

			// Call the original function....
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SkyColor_Green), newString);
		}
		/*static */void VariableManager::OnConsoleVariableStringChange_SkyColor_Blue (const char *const newString)
		{
			const int newValue (static_cast <int> (atof (newString)));

			// Update sky light level value....
			g_skyLightLevel = static_cast <unsigned char> ((Math::GetClampedValueInRange (variableManager.GetVariable (VariableID_SkyColor_Red)->GetValue <int> (), 0, 255) + Math::GetClampedValueInRange (variableManager.GetVariable (VariableID_SkyColor_Green)->GetValue <int> (), 0, 255) + Math::GetClampedValueInRange (newValue, 0, 255)) / Color <>::ID_Total);

			// Call the original function....
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_SkyColor_Blue), newString);
		}
	}
}