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
// SV_CalcPingIfFakeClientInjector.hpp
//
// Class: SV_CalcPingIfFakeClientInjector
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SV_CALC_PING_IF_FAKE_CLIENT_INJECTOR_INCLUDED
#define SV_CALC_PING_IF_FAKE_CLIENT_INJECTOR_INCLUDED

class SV_CalcPingIfFakeClientInjector
{
	private:
		static const unsigned char s_patchSize = Trampolines::Bytecode::JumpImmediately32Size;

		unsigned char        m_originalBytes[s_patchSize / sizeof (unsigned char)];
		unsigned char *const m_addressIfFakeClient;
		const unsigned char  m_epilogueSize;
		unsigned char *const m_detour;

	private:
		static inline unsigned char *const GetTargetAddress (void)
		{
			const union /* Unnamed */
			{
				unsigned int  integer;
				unsigned char byte[sizeof (unsigned int)];
			} integerToByte = {GET_OFFSET_OF_MEMBER_VARIABLE (HalfLifeEngine::SDK::Structures::Client_t, fakeclient)/*, Do not initialize me! */};
			const unsigned char signatureIfFakeClient[] = {0x39, Trampolines::Bytecode::WildCard, integerToByte.byte[0u], integerToByte.byte[1u], integerToByte.byte[2u], integerToByte.byte[3u]};	// cmp [ecx+2548h], (ebp/esi)	// Compare Two Operands (2548h - offset to HalfLifeEngine::SDK::Structures::Client_t::fakeclient member)
			unsigned char *addressIfFakeClient (static_cast <unsigned char *> (MemoryUtilities::FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_CalcPingFunction (), GET_STATIC_STRING_LENGTH ("\x55\x8B\xEC\x51\x8B\x4D\x08\x56\x33\xF6\x39\xB1\x48\x25\x00\x00\x74\x07\x33\xC0\x5E\x8B\xE5\x5D\xC3")/* Signature from swds.dll (has more opcodes than in hw.dll) */, signatureIfFakeClient, sizeof (signatureIfFakeClient))));

			// Reliability check.
			InternalAssert (addressIfFakeClient != NULL);

			// Skip "cmp [ecx+2548h], (ebp/esi)" and "jz short loc_1DB27F5" instructions....
			addressIfFakeClient += sizeof (signatureIfFakeClient) + GET_STATIC_STRING_LENGTH ("\x74\x05")/* jz short loc_1DB27F5 */;

			return addressIfFakeClient;
		}
		inline const unsigned char CalculateAndGetEpilogueSize (void) const
		{
			// Reliability check.
			InternalAssert (m_addressIfFakeClient != NULL);

			unsigned char index (0u);
			const unsigned char *const addressPrologueStart (m_addressIfFakeClient + GET_STATIC_STRING_LENGTH ("\x33\xC0")/* skip "xor eax, eax" instruction.... */);

			while (addressPrologueStart[++index] != Trampolines::Bytecode::Return);

			return index + sizeof (Trampolines::Bytecode::Return);
		}

		inline void DoInjection (void)
		{
			// Reliability checks.
			InternalAssert (m_addressIfFakeClient[0u] == 0x33 && m_addressIfFakeClient[1u] == 0xC0);	// xor eax, eax;	// Logical Exclusive OR

			// Save the original signature....
			memcpy (m_originalBytes, m_addressIfFakeClient, sizeof (m_originalBytes));

			if (!Trampolines::WriteFunctionJump32 (m_addressIfFakeClient, m_detour))
			{
				AddLogEntry (false, LogLevel_Critical, true, "DoInjection(): Trampolines::WriteFunctionJump32() filed!");

				return;
			}

			AddLogEntry (false, LogLevel_Default, false, "DoInjection(): Injection into SV_CalcPing() function is complete!");
		}

		inline void UndoInjection (void) const
		{
			// Reliability check.
			InternalAssert (m_addressIfFakeClient[0u] == Trampolines::Bytecode::JumpImmediately32);

			memcpy (m_addressIfFakeClient, m_originalBytes, s_patchSize);
		}

	public:
		inline  SV_CalcPingIfFakeClientInjector (void) :
			m_addressIfFakeClient (GetTargetAddress ()),
			m_epilogueSize (CalculateAndGetEpilogueSize ()),
			m_detour (Detours::g_codeAllocator.Allocate <unsigned char> (sizeof (Trampolines::Bytecode::PushECX) + Trampolines::Bytecode::CallImmediately32Size + Trampolines::Bytecode::AddESP8Size + m_epilogueSize))
		{
			// Reliability check.
			InternalAssert (m_detour != NULL);

			const unsigned char detourOpcodes[] =
			{
				Trampolines::Bytecode::PushECX,																																										// push ecx		// push the engine fake client pointer
				Trampolines::Bytecode::CallImmediately32, Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],	// call <gate>	// call our function
				Trampolines::Bytecode::AddESP8[0u], Trampolines::Bytecode::AddESP8[1u], 0x04u																														// add  esp, 4	// correct stack
/*
				// Continue....
				// For hw.DLL
				{
					Trampolines::Bytecode::PopEBP,	// pop ebp
					Trampolines::Bytecode::PopECX,	// pop ecx

					Trampolines::Bytecode::Return	// retn	// Return Near from Procedure
				}
				// OR for swds.DLL
				{
					Trampolines::Bytecode::PopESI,	// pop esi
					0x8B, 0xE5,						// mov esp, ebp
					Trampolines::Bytecode::PopEBP,	// pop ebp

					Trampolines::Bytecode::Return	// retn	// Return Near from Procedure
				}
*/			};
			const unsigned char detourCallPosition (GET_STATIC_STRING_LENGTH ("\x51\xE8"));
			const unsigned char detourOriginalProloguePosition (detourCallPosition + GET_STATIC_STRING_LENGTH ("\xDE\xFA\xAD\xDE\x83\xC4\x04"));

			// Copy the main trampoline function
			memcpy (m_detour, detourOpcodes, sizeof (detourOpcodes));

			// Copy our detour call into the trampoline
//			Trampolines::RedirectFunctionCall32 (m_detour + detourCallPosition, &HalfLifeEngine::Export::ReplacedFunctions::Other::SV_CalcPing_IfFakeClient);
			unsigned char *tempAddress (m_detour + detourCallPosition);
			reinterpret_cast <unsigned int *> (tempAddress)[0u] = reinterpret_cast <unsigned int> (&HalfLifeEngine::Export::ReplacedFunctions::Other::SV_CalcPing_IfFakeClient) - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);

			// Copy original bytes onto the end of the trampoline function
			memcpy (m_detour + detourOriginalProloguePosition, m_addressIfFakeClient + GET_STATIC_STRING_LENGTH ("\x33\xC0")/* skip "xor eax, eax" instruction.... */, m_epilogueSize);

			DoInjection ();
		}
		inline ~SV_CalcPingIfFakeClientInjector (void)
		{
			UndoInjection ();

			// Reliability check.
			InternalAssert (m_detour != NULL);

			// Free memory used
			Detours::g_codeAllocator.Free (m_detour);
		}

	//
	// Group: Private operators.
	//
	private:
		inline SV_CalcPingIfFakeClientInjector &operator = (const SV_CalcPingIfFakeClientInjector &/*right*/);	// Avoid "warning C4512: 'SV_CalcPingIfFakeClientInjector' : assignment operator could not be generated".

	public:
		inline void CallOriginalFunction (const HalfLifeEngine::SDK::Structures::Client_t *const client)
		{
			UndoInjection ();

				(*HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_CalcPingFunction ()) (client);	// Call original engine function.

			DoInjection ();
		}
};

#endif	// ifndef SV_CALC_PING_IF_FAKE_CLIENT_INJECTOR_INCLUDED