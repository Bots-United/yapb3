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
// FakeClientManager.hpp
//
// Class: FakeClientManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef FAKE_CLIENT_MANAGER_INCLUDED
#define FAKE_CLIENT_MANAGER_INCLUDED

class FakeClientManager : public ClientManagerTemplate <FakeClient>
{
	//
	// Group: Private members.
	//
	private:
		HalfLifeEngine::SDK::Structures::ClientLightStyle_t m_lightStyle[MAX_LIGHTSTYLES];
		int                                                 m_lightStyleValue[HalfLifeEngine::SDK::Constants::MaximumLightStyleValue];	// 8.8 fraction of base light value

	//
	// Group: Private functions.
	//
	private:
		inline void R_AnimateLight (void)
		{
			// light animations
			// 'm' is normal light, 'a' is no light, 'z' is double bright
			const int i (static_cast <int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () * 10.0f));
			int k;

			for (unsigned char j (0u); j < MAX_LIGHTSTYLES; ++j)
			{
				if (m_lightStyle[j].length == 0u)
				{
					m_lightStyleValue[j] = 256;

					continue;
				}

				k = m_lightStyle[j].map[i % m_lightStyle[j].length] - 'a';
				k *= 22;

				m_lightStyleValue[j] = k;
			}	
		}

	public:
		inline void SetupLightStyles (void)
		{
			// Setup lighting information....

			// store all current light styles
			for (unsigned char index (0u); index < MAX_LIGHTSTYLES; ++index)
				LightStyle (index, g_server->GetServer ().lightstyles[index]);

			for (unsigned short index (0u); index < HalfLifeEngine::SDK::Constants::MaximumLightStyleValue; ++index)
				m_lightStyleValue[index] = 264;	// normal light value
		}

		inline const int *const GetLightStyleValue (void) const { return m_lightStyleValue; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void LightStyle (const unsigned char style, char *const value)
		{
/*			for (ClientsArray_t::IndexType_t index (0u); index < m_clientsArray.GetElementNumber (); ++index)
				m_clientsArray[index]->CL_Parse_LightStyle (style, value);
*/			if (style >= MAX_LIGHTSTYLES)
			{
				AddLogEntry (true, LogLevel_Critical, true, "SVC_LIGHTSTYLE > MAX_LIGHTSTYLES");

				return;
			}

			// OCCURS!
			if (IsNullString (value))
			{
				m_lightStyle[style].length = 0u;
				m_lightStyle[style].map[0u] = '\0';

				return;
			}

			const unsigned short maximumCopyAmount (GET_STATIC_STRING_LENGTH (m_lightStyle[style].map));

			strncpy (m_lightStyle[style].map, value, maximumCopyAmount);

			m_lightStyle[style].map[maximumCopyAmount] = '\0';	// Terminate the string....

			m_lightStyle[style].length = GetStringLength <int> (m_lightStyle[style].map);
		}

		inline void Think (void)
		{
			R_AnimateLight ();
		}
};

#endif	// ifndef FAKE_CLIENT_MANAGER_INCLUDED