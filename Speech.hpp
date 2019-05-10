//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// Speech.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SPEECH_INCLUDED
#define SPEECH_INCLUDED

#include <atlbase.h>
#include <atlcom.h>

#include <SAPI.h>
#include <SPHelper.h>
#include <SPDDKHLP.h>
#include <speventq.h>

#include <C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/winui/speech/simpleaudio/simpleaudio.h>
//#include <C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/winui/speech/simpleaudio/simpleaudio_i.c>	// Included in YaPB.cpp
#include <C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/winui/speech/simpleaudio/SpAudioPlug.h>

//#include <TTSStream.h>
#if 0
abstract_class SynthesisManager
{
	public:
class Voice
{
};
class SynthesisRequest
{
};

	protected:
		enum Type_t
		{
			Type_SAPI
		};

	private:
		virtual inline const Type_t        GetType      (void) const = 0;					// get the type of synthesis manager

	// Synthesis Manager routines
	public:
		virtual inline const bool          Initialize   (void) = 0;
		virtual inline const bool          Deinitialize (void) = 0;

		virtual inline const char *const   GetName (void) const = 0;	// Used for menus, etc

		inline const bool                  IsSAPI       (void) const { return GetType () == Type_SAPI; }

		virtual inline const bool          SetVoice     (Voice *const voice) = 0;			// set the voice
		virtual inline Voice *const        GetVoice     (void) const = 0;					// get the voice
		virtual inline const bool          SetPitch     (const unsigned char pitch) = 0;	// set the speaking rate
		virtual inline const unsigned char GetPitch     (void) const = 0;					// get the speaking rate
		virtual inline const bool          SetVolume    (const unsigned char volume) = 0;	// set the audio output volume for the voice. SAPI specifies that this should be an integer between 0 and 100. 100 being the maximum volume for the selected voice.
		virtual inline const unsigned char GetVolume    (void) const = 0;					// get the audio output volume for the voice.
		virtual inline const bool          GetData      (unsigned char *data, unsigned short &size) const = 0;
		virtual inline const bool          Synthesize   (SynthesisRequest &request) = 0;	// the function synthesizes a request
};
class SAPISynthesisManager
{
	// SAPI variables
	private:
//		CComPtr <ISpAudioPlug> pVoice;
		ISpVoice *pVoice;

		int iSpeechRate;
		int iVolume;

//		CComPtr <ISpAudioPlug> pSpStream;
		ISpAudioPlug *pSpStream;

		HRESULT hr;	// generic variable to receive return status

	private:
		inline const Type_t GetType (void) const { return Type_SAPI; }

		static DWORD WINAPI Thread (LPVOID pParams)
		{
			// the main thread of the synthesis manager

			return 1u;
		}

	public:
		inline const bool Initialize (void)
		{
			// Creates SAPI instance from COM
			hr = CoCreateInstance (CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
			if(FAILED (hr))
			{
				AddLogEntry (true, LogLevel_Error, true, "Failed to create SAPI voice instance. (hr=0x%X)", hr);
				return false;
			}
			hr = CoCreateInstance (CLSID_SpAudioPlug, NULL, CLSCTX_ALL, IID_ISpAudioPlug, (void **)&pSpStream);
			if (FAILED (hr))
			{
				AddLogEntry (true, LogLevel_Error, true, "Unable to create SAPI SpAudioPlug instance. (hr=0x%X)", hr);
				return false;
			}
			pSpStream->Init (VARIANT_TRUE, SAFT8kHz16BitMono);
			pVoice->SetOutput (pSpStream, FALSE);
			if (FAILED (hr))
			{
				AddLogEntry (true, LogLevel_Error, true, "Unable to set SAPI voice output. (hr=0x%X)", hr);
				return false;
			}

			return true;
		}
		inline const bool Deinitialize (void)
		{
			// Releases SAPI voice
			pVoice->Release ();

			return true;
		}

		inline const char *const GetName (void) const { return "MS SAPI 5.1"; }

		inline const bool SetVoice (Voice *const voice)
		{
			// set the voice (sets the synthesis voice to whatever is specified in
			// sVoiceName). If sVoiceName is *, it loads the first available voice

			return false;
		}
		inline Voice *const GetVoice (void) const
		{
			return NULL;
		}
		inline const bool SetPitch (const unsigned char pitch)
		{
			if (pitch > 0)
			{
				hr = pVoice->SetRate(pitch);
				if (SUCCEEDED(hr))
				{
					iSpeechRate = pitch;	/// @todo MODIFY OURS 'pitch' TO SAPI RATE!!!

					AddLogEntry (true, LogLevel_Default, false, "Set speaking rate: %d", pitch);

					return true;
				}

				AddLogEntry (true, LogLevel_Error, false, "Failed to set speaking rate: %d (0x%X).", pitch, hr);

				return false;
			}
			else
			{
				// disable speech rate adjustment
				AddLogEntry (true, LogLevel_Error, false, "No speech rate given, using voice default!");

				return false;
			}
		}
		inline const unsigned char GetPitch (void) const { return iSpeechRate; }
		inline const bool SetVolume (const unsigned char volume)
		{
			iVolume = volume;	/// @todo MODIFY OURS 'volume' TO SAPI VOLUME!!!
			pVoice->SetVolume((USHORT)iVolume);
			AddLogEntry (true, LogLevel_Default, false, "Set volume: %d%%.", iVolume);

			return true;
		}
		inline const unsigned char GetVolume (void) const { return iVolume; }
		inline const bool GetData (unsigned char *data, unsigned short &size) const
		{
			return false;
		}
		inline const bool Synthesize (SynthesisRequest &request)
		{
			return false;
		}
};
#endif	// if 0
#endif	// ifndef SPEECH_INCLUDED