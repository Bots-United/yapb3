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
// ClientCommandHooker.hpp
//
// Class: ClientCommandHooker
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CLIENT_COMMAND_HOOKER_INCLUDED
#define CLIENT_COMMAND_HOOKER_INCLUDED

class ClientCommandHooker
{
	private:
		enum ClientCommandOffset_t
		{
			ClientCommandOffset_DetourCallPosition = GET_STATIC_STRING_LENGTH ("\x55\xE8"),
			ClientCommandOffset_DetourJumpToEpiloguePosition = ClientCommandOffset_DetourCallPosition + GET_STATIC_STRING_LENGTH ("\xDE\xFA\xAD\xDE\x5D\x84\xC0\x0F\x84"),
			ClientCommandOffset_MovUseBotArgsToDl = GET_STATIC_STRING_LENGTH ("\x8A\x15\xDE\xFA\xAD\xDE"),
			CS_DETOURCOPYBYTES_CLIENTCOMMAND = GET_STATIC_STRING_LENGTH ("\x8D\xB5\x80\x00\x00\x00"),

			ClientCommandOffset_Total = ClientCommandOffset_DetourJumpToEpiloguePosition + sizeof (Trampolines::Bytecode::RawNumber),

			ClientCommandOffset_DetourMovUseBotArgsToDlPosition = ClientCommandOffset_Total,
			ClientCommandOffset_DetourCS_DETOURCOPYBYTES_CLIENTCOMMANDPosition = ClientCommandOffset_DetourMovUseBotArgsToDlPosition + ClientCommandOffset_MovUseBotArgsToDl,
			ClientCommandOffset_DetourJumpPosition = ClientCommandOffset_Total + ClientCommandOffset_MovUseBotArgsToDl + CS_DETOURCOPYBYTES_CLIENTCOMMAND,
			ClientCommandOffset_FullDetourSize = ClientCommandOffset_DetourJumpPosition + Trampolines::Bytecode::JumpImmediately32Size
		};
		void          *m_targetAddress;
		unsigned char *m_fullDetour;

		static const unsigned char ms_detourOpcodes[ClientCommandOffset_Total];

	private:
		static inline void *const GetTargetAddress (void)
		{
			const union /* Unnamed */
			{
				const void    *pointer;
				unsigned char  byte[Trampolines::Bytecode::Pointer32Size];
			} pointerToByte = {&g_server->GetGameRules ()/*, Do not initialize me! */};
			const unsigned char pattern[] =
			{
				0x8B, 0x3D, pointerToByte.byte[0u], pointerToByte.byte[1u], pointerToByte.byte[2u], pointerToByte.byte[3u],										// mov  edi, 'g_pGameRules'
				0x89, 0x7C, 0x24, 0x18,																															// mov  [esp+0A4h+var_8C], edi
				0x8B, 0x45, 0x7C,																																// mov  eax, [ebp+7Ch]
				0x85, 0xC0,																																		// test eax, eax
				0x0F, 0x84, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard	// jz   loc_10070C20
			};	// (Address: 0x1006DC5E)
			void *address (MemoryUtilities::FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientCommand/* Start searching from beginning of ClientCommand() function. (Address: 0x1006DC20)*/, reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientCommand) - reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ()), pattern, sizeof (pattern)));

			// Reliability check.
			InternalAssert (address != NULL);

			reinterpret_cast <unsigned char *&> (address) += sizeof (pattern);	// (Address: 0x1006DC73)

			// Reliability check.
			InternalAssert (static_cast <unsigned short *> (address)[0u] == MakeID (0x8D, 0xB5));

			return address;
		}

		/**
		* Called when CS internally fires a command to a player. It does this for a few
		* functions, most notably rebuy/autobuy functionality. This is also used to pass
		* commands to CZ bots internally.
		*
		* @param id			Client index.
		* @param cmd		Command string.
		* @return			PLUGIN_HANDLED (true) to block, PLUGIN_CONTINUE (false) for normal operation.
		*/
		static const unsigned char Detour_ClientCommand (const HalfLifeEngine::SDK::Classes::Edict *const clientEdict);

		inline void CreatePatch (void)
		{
			m_targetAddress = GetTargetAddress ();	// (Address: 0x1006DC73)

			// Reliability checks.
			InternalAssert (m_fullDetour == NULL);
			InternalAssert (static_cast <unsigned short *> (m_targetAddress)[0u] == MakeID (0x8D, 0xB5));

			m_fullDetour = Detours::g_codeAllocator.Allocate <unsigned char> (ClientCommandOffset_FullDetourSize);

			// Reliability check.
			InternalAssert (m_fullDetour != NULL);

			// Copy the main trampoline function
			MemoryCopy (ms_detourOpcodes, m_fullDetour, sizeof (ms_detourOpcodes));

			// Copy our detour call into the trampoline
//			Trampolines::RedirectFunctionCall32 (m_fullDetour + ClientCommandOffset_DetourCallPosition, &Detour_ClientCommand);
			unsigned char *tempAddress (m_fullDetour + ClientCommandOffset_DetourCallPosition);
			reinterpret_cast <unsigned int *> (tempAddress)[0u] = reinterpret_cast <unsigned int> (&Detour_ClientCommand) - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);

			// Copy epilogue of target function into our trampoline
			tempAddress = m_fullDetour + ClientCommandOffset_DetourJumpToEpiloguePosition;
			reinterpret_cast <unsigned int *> (tempAddress)[0u] = reinterpret_cast <unsigned int> (Trampolines::GetRealAddressOfRelativeAddress32 (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (m_targetAddress) - Trampolines::Bytecode::Pointer32Size))) - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);

			// Copy original bytes onto the end of the trampoline function
			MemoryCopy (static_cast <unsigned char *> (m_targetAddress) + CS_DETOURCOPYBYTES_CLIENTCOMMAND + GET_STATIC_STRING_LENGTH ("\x85\xF6\x89\x74\x24\x1C\x75\x16\xFF\x15\x94\x23\x16\x10\x85\xC0\x74\x06\x8D\xB0\x80\x00\x00\x00")/* (Address: 0x1006DC91) */, m_fullDetour + ClientCommandOffset_DetourMovUseBotArgsToDlPosition, ClientCommandOffset_MovUseBotArgsToDl);
			MemoryCopy (static_cast <unsigned char *> (m_targetAddress), m_fullDetour + ClientCommandOffset_DetourCS_DETOURCOPYBYTES_CLIENTCOMMANDPosition, CS_DETOURCOPYBYTES_CLIENTCOMMAND);

			// Patch and copy the final jmp
//			Trampolines::WriteFunctionJump32 (m_fullDetour + ClientCommandOffset_DetourJumpPosition, reinterpret_cast <unsigned char *> (m_targetAddress) + CS_DETOURCOPYBYTES_CLIENTCOMMAND);
			(tempAddress = m_fullDetour + ClientCommandOffset_DetourJumpPosition)[0u] = Trampolines::Bytecode::JumpImmediately32;
			reinterpret_cast <unsigned int *> (++tempAddress)[0u] = reinterpret_cast <unsigned int> (m_targetAddress) + CS_DETOURCOPYBYTES_CLIENTCOMMAND - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);
		}
		inline void DestroyPatch (void)
		{
			// Reliability check.
			InternalAssert (m_fullDetour != NULL);

			// Free memory used
			Detours::g_codeAllocator.Free (m_fullDetour);

			m_fullDetour = NULL;
		}

	public:
		inline ClientCommandHooker (void) : m_targetAddress (NULL), m_fullDetour (NULL) { /* VOID */ }

	public:
		inline void DoPatch (void)
		{
			if (!Trampolines::VirtualProtect (m_targetAddress, Trampolines::Bytecode::JumpImmediately32Size, PAGE_EXECUTE_READWRITE))
			{
				DestroyPatch ();

				AddLogEntry (true, LogLevel_Critical, true, "ClientCommand() PATCHING FAILED!!!\n");

				return;
			}

			// Now overwrite the target function with our trampoline
//			Trampolines::WriteFunctionJump32 (m_targetAddress, m_fullDetour);
			unsigned char *tempAddress (static_cast <unsigned char *> (m_targetAddress));
			tempAddress[0u] = Trampolines::Bytecode::JumpImmediately32;
			reinterpret_cast <unsigned int *> (++tempAddress)[0u] = reinterpret_cast <unsigned int> (m_fullDetour) - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);
		}
		inline void UndoPatch (void)
		{
			// Copy back the original function bytes
			MemoryCopy (m_fullDetour + ClientCommandOffset_DetourCS_DETOURCOPYBYTES_CLIENTCOMMANDPosition, static_cast <unsigned char *> (m_targetAddress), Trampolines::Bytecode::JumpImmediately32Size);
		}

		inline void CreateAndDoPatch (void)
		{
			CreatePatch ();
			DoPatch ();
		}
		inline void UndoAndDestroyPatch (void)
		{
			UndoPatch ();
			DestroyPatch ();
		}

		inline void CallOriginalFunction (HalfLifeEngine::SDK::Classes::Edict *const clientEdict)
		{
			UndoPatch ();

				HalfLifeEngine::Globals::g_halfLifeEngine->ClientCommand (clientEdict);	// Call original engine function.

			DoPatch ();
		}
};

#endif	// ifndef CLIENT_COMMAND_HOOKER_INCLUDED