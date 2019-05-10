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
// ClientPutInServerHooker.hpp
//
// Class: ClientPutInServerHooker
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CLIENT_PUT_IN_SERVER_HOOKER_INCLUDED
#define CLIENT_PUT_IN_SERVER_HOOKER_INCLUDED

class ClientPutInServerHooker
{
	private:
		static const unsigned char s_patchSize = sizeof (unsigned char) + sizeof (long);

		unsigned char m_originalBytes[s_patchSize / sizeof (unsigned char)];
		unsigned char m_patchedBytes[s_patchSize / sizeof (unsigned char)];

	private:
		inline void CreatePatch (void)
		{
			memcpy (m_originalBytes, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientPutInServer, s_patchSize);

			m_patchedBytes[0u] = Trampolines::Bytecode::JumpImmediately32;

			reinterpret_cast <long *> (m_patchedBytes + 1u)[0u] = reinterpret_cast <char *> (HalfLifeEngine::Export::ReplacedFunctions::DLLTable::ClientPutInServer) - reinterpret_cast <char *> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientPutInServer) - s_patchSize;
		}

		inline void DoPatch     (void) const
		{
			if (Trampolines::VirtualProtect (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientPutInServer, s_patchSize, PAGE_EXECUTE_READWRITE))
			{
				memcpy (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientPutInServer, m_patchedBytes, s_patchSize);

				return;
			}

			AddLogEntry (true, LogLevel_Critical, true, "pfnClientPutInServer() PATCHING FAILED!!!\n");
		}
		inline void UndoPatch   (void) const
		{
			memcpy (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientPutInServer, m_originalBytes, s_patchSize);
		}

	public:
		inline  ClientPutInServerHooker (void) { CreateAndDoPatch (); }
		inline ~ClientPutInServerHooker (void) { UndoPatch (); }

	public:
		inline void CreateAndDoPatch (void)
		{
			CreatePatch ();
			DoPatch ();
		}

		inline void CallOriginalFunction (HalfLifeEngine::SDK::Classes::Edict *const client) const
		{
			UndoPatch ();

				HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientPutInServer (client);	// Call original engine function.

			DoPatch ();
		}
};

#endif	// ifndef CLIENT_PUT_IN_SERVER_HOOKER_INCLUDED