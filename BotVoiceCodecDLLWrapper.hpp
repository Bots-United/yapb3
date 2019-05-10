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
// BotVoiceCodecDLLWrapper.hpp
//
// Class: VoiceCodecDLLWrapper
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_VOICE_CODEC_DLL_WRAPPER_INCLUDED
#define BOT_VOICE_CODEC_DLL_WRAPPER_INCLUDED

class VoiceCodecDLLWrapper
{
	//
	// Group: Private members.
	//
	private:
		#include <VoiceCodecLibrary.hpp>

		VoiceCodecLibrary m_voiceCodecDLL;
		DynamicString     m_codecName;
		unsigned char     m_quality;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline VoiceCodecDLLWrapper (void) :
			m_quality (0u)
		{
			if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_VoiceEnable)->GetValue <bool> () && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_VoiceNew)
				Initialize (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_codec)->GetString ());
		}

	//
	// Group: Functions.
	//
	public:
		inline const DynamicString      &GetCodecName     (void) const { return m_codecName; }
		inline const VoiceCodecLibrary  &GetVoiceCodecDLL (void) const { return m_voiceCodecDLL; }
		inline const unsigned char       GetQuality       (void) const { return m_quality; }

		inline const bool Initialized (void) const { return m_voiceCodecDLL.IsValid () && m_voiceCodecDLL.GetCreateCodecFunction () != NULL; }
		inline const bool Initialize (const DynamicString &codecName)
		{
			// Reliability checks.
			InternalAssert (!Initialized ());

			if (codecName.IsEmpty ())
			{
				AddLogEntry (true, LogLevel_Error, false, "Voice codec name is empty. Bot voice disabled.", codecName.GetData ());

				return false;
			}

			// Get the codec name with extention.
			const DynamicString codecDLLName (codecName + ".DLL");

			HalfLifeEngine::Globals::g_halfLifeEngine->MakeLocalFileCopy ("MSS32.DLL");	// voice_miles.DLL will use this...
			HalfLifeEngine::Globals::g_halfLifeEngine->MakeLocalFileCopy (codecDLLName);

			if (!m_voiceCodecDLL.Load (codecDLLName))
			{
				AddLogEntry (true, LogLevel_Error, false, "Unable to load voice codec DLL '%s'. Bot voice disabled.", codecName.GetData ());

				return false;
			}

			if (!m_voiceCodecDLL.InitializeCreateCodecFunction ())
			{
				AddLogEntry (true, LogLevel_Error, false, "Unable to get a factory for voice codec '%s'. Bot voice disabled.", codecName.GetData ());

				return false;
			}

			m_codecName = codecName;
			m_quality = Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_quality)->GetValue <unsigned char> ();

			AddLogEntry (true, LogLevel_Default, false, "Voice codec '%s' successfully loaded. (Base address = 0x%X)", codecDLLName.GetData (), m_voiceCodecDLL.GetPointer ());

			return true;
		}
		inline void Deinitialize (void)
		{
			// Reliability check.
			InternalAssert (Initialized ());

			g_server->GetYaPBManager ()->ReleaseCodec ();

			m_voiceCodecDLL.Free ();
		}

		inline const bool ChangeCodec (const DynamicString &newCodecName)
		{
			// Reliability check.
			InternalAssert (GetCodecName ().CompareWithoutCaseCheck (newCodecName) != 0);

			if (Initialized ())
				Deinitialize ();

			if (!Initialize (newCodecName))
				return false;

			g_server->GetYaPBManager ()->OnChangeCodec (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_quality)->GetValue <unsigned char> ());

			return true;
		}
		inline void ChangeQuality (const unsigned char newQuality)
		{
			if (!Initialized ())
				return;

			m_quality = newQuality;	// Update quality value....

			g_server->GetYaPBManager ()->ReleaseCodec ();

			// Reliability check.
			InternalAssert (Console::Server::variableManager.GetVariable (Console::Server::VariableID_VoiceEnable)->GetValue <bool> () && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_VoiceNew);

			g_server->GetYaPBManager ()->OnChangeCodec (newQuality);
		}

		inline HalfLifeEngine::SDK::Classes::Interface::Voice::IVoiceCodec *const InitializeCodec (const unsigned char quality) const
		{
			// Reliability check.
			InternalAssert (Initialized ());

			HalfLifeEngine::SDK::Classes::Interface::Voice::IVoiceCodec *const newCodec (static_cast <HalfLifeEngine::SDK::Classes::Interface::Voice::IVoiceCodec *> ((*m_voiceCodecDLL.GetCreateCodecFunction ()) (GetCodecName (), NULL)));

			if (newCodec == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Unable to create interface for voice codec '%s'. Bot voice disabled.", GetCodecName ().GetData ());

				return NULL;
			}

			if (!newCodec->Init (quality))
			{
				AddLogEntry (true, LogLevel_Error, false, "Unable to initialize voice codec '%s'. Bot voice disabled.", GetCodecName ().GetData ());

				if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
					newCodec->Release ();
				else
				{
					HalfLifeEngine::SDK::Classes::Interface::CDAudio *const cdaudio (HalfLifeEngine::Globals::g_halfLifeEngine->GetCDAudio ());

					cdaudio->UNKNOWN2 ();

						newCodec->Release ();

					cdaudio->UNKNOWN1 ();
				}

				return NULL;
			}

			AddLogEntry (true, LogLevel_Default, false, "Voice codec '%s' successfully initialized. Bot voice enabled.", GetCodecName ().GetData ());

			return newCodec;
		}
		inline HalfLifeEngine::SDK::Classes::Interface::Voice::IVoiceCodec *const TryInitializeCodec (const unsigned char quality) const
		{
			if (!Console::Server::variableManager.GetVariable (Console::Server::VariableID_VoiceEnable)->GetValue <bool> () || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () != Console::Bot::VariableValue_Communication_VoiceNew)
				return NULL;

			return InitializeCodec (quality);
		}
};

#endif	// ifndef BOT_VOICE_CODEC_DLL_WRAPPER_INCLUDED