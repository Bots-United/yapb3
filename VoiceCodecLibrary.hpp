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
// VoiceCodecLibrary.hpp
//
// Class: VoiceCodecLibrary
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef VOICE_CODEC_LIBRARY_INCLUDED
#define VOICE_CODEC_LIBRARY_INCLUDED

class VoiceCodecLibrary : public LibraryHandle
{
	//
	// Group: Private members.
	//
	private:
		DynamicString                                             m_pathName;				// path to library
		HalfLifeEngine::SDK::Classes::Interface::CreateFunction_t m_createCodecFunction;	// Create factory.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline VoiceCodecLibrary (void) : LibraryHandle (), m_createCodecFunction (NULL) { /* VOID */ }

		inline ~VoiceCodecLibrary (void)
		{
			if (IsValid ())
				FreeLibrary (m_module);
		}

	//
	// Group: Functions.
	//
	public:
		inline const DynamicString                                             &GetPathName            (void) const { return m_pathName; }	// Get the path name of the library.
		inline const HalfLifeEngine::SDK::Classes::Interface::CreateFunction_t  GetCreateCodecFunction (void) const { return m_createCodecFunction; }

		inline const bool Load (const DynamicString &pathName)
		{
			// Reliability check.
			InternalAssert (!IsValid ());

			m_pathName = pathName;
			m_module = LoadLibrary (pathName);

			return IsValid ();
		}
		inline const bool InitializeCreateCodecFunction (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			m_createCodecFunction = GetFunctionAddress <HalfLifeEngine::SDK::Classes::Interface::CreateFunction_t> (CREATEINTERFACE_PROCNAME);

			return m_createCodecFunction != NULL;
		}
		inline const bool Free (void)
		{
			// Reliability check.
			if (!IsValid ())
				return true;

			m_pathName.MakeEmpty ();

			bool result;

			if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
				result = FreeLibrary (m_module) == TRUE;
			else
			{
				HalfLifeEngine::SDK::Classes::Interface::CDAudio *const cdaudio (HalfLifeEngine::Globals::g_halfLifeEngine->GetCDAudio ());

				cdaudio->UNKNOWN2 ();

					result = FreeLibrary (m_module) == TRUE;

				cdaudio->UNKNOWN1 ();
			}

			m_module.handle = NULL;
			m_createCodecFunction = NULL;

			return result;
		}
};

#endif	// ifndef VOICE_CODEC_LIBRARY_INCLUDED