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
// ConsoleVariableValueChangeHooker.hpp
//
// Class: ConsoleVariableValueChangeHooker
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CONSOLE_VARIABLE_VALUE_CHANGE_HOOKER_INCLUDED
#define CONSOLE_VARIABLE_VALUE_CHANGE_HOOKER_INCLUDED

class ConsoleVariableValueChangeHooker;

extern DLL_GLOBAL ConsoleVariableValueChangeHooker g_consoleVariableValueChangeHooker;

class ConsoleVariableValueChangeHooker
{
	public:
		typedef void (*OnStringChangeCallback_t) (const char *const newString);	// Returns true when variable value shouldn't be changed by some reason....

	private:
		static const unsigned char s_patchSize = sizeof (unsigned char) + sizeof (long);

		unsigned char                                                  m_originalBytes[s_patchSize / sizeof (unsigned char)];
		unsigned char                                                  m_patchedBytes[s_patchSize / sizeof (unsigned char)];

		HalfLifeEngine::SDK::TypeDefinitions::Cvar_DirectSetFunction_t m_Cvar_DirectSetFunctionPointer;
#if 0
/*
		#pragma pack (push, 1)	// Structures must be packed (byte-aligned) (Very important!)
		struct Trampoline_t
		{
			unsigned char                              leaESPPlusOffset32ToEAX[7u];
			unsigned char                              pushEAX;
			unsigned char                              pushESI;
			Trampolines::Bytecode::CallImmediately32_t callback;
			unsigned char                              popESI;
			unsigned char                              popEAX;
			unsigned char                              movECXReferenceToECX;
			unsigned char                              movECXToEDI;
			unsigned char                              testALAndAL;
			unsigned char                              JumpIfNotZeroImmediately32[Trampolines::Bytecode::JumpIfNotZeroImmediately32Size];
		};
		#pragma pack (pop)	// Reset default packing.
*/		enum Offset_t
		{
			Offset_DetourCallPosition = GET_STATIC_STRING_LENGTH (/**"\xCC"/**/ "\x8D\x84\x24\x10\x04\x00\x00\x50\x56\xE8"),
			Offset_DetourJumpToEpiloguePosition = Offset_DetourCallPosition + GET_STATIC_STRING_LENGTH ("\xDE\xFA\xAD\xDE\x5E\x59\x8B\x09\x8B\xF9\x84\xC0\x0F\x85"),
			DetourCobyBytes_Cvar_DirectSet = GET_STATIC_STRING_LENGTH ("\xF6\x46\x08\x80"),

			Offset_Total = Offset_DetourJumpToEpiloguePosition + sizeof (Trampolines::Bytecode::RawNumber),

			Offset_DetourTestIfPrintableOnlyPosition = Offset_Total,
			Offset_DetourJumpIfPrintablePosition = Offset_Total + DetourCobyBytes_Cvar_DirectSet,
			Offset_DetourJumpPosition = Offset_DetourJumpIfPrintablePosition + Trampolines::Bytecode::JumpIfZeroImmediately32Size,
			Offset_FullDetourSize = Offset_DetourJumpPosition + Trampolines::Bytecode::JumpImmediately32Size
		};
		void          *m_targetAddress;
		unsigned char *m_fullDetour;

		static const unsigned char ms_detourOpcodes[Offset_Total];
#endif	// if 0
	private:
		inline void InitializeCvar_DirectSetFunctionPointer (void)
		{
			// Get real engine Cvar_DirectSet() function pointer....

			// Assign address of wrapper function....
			m_Cvar_DirectSetFunctionPointer = HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineFunctionTable ().pfnCvar_DirectSet;

			// Find call of real engine Cvar_DirectSet() function (pfnCvar_DirectSet() function it's just a function which calls real engine Cvar_DirectSet() function (wrapper))
			while (*++reinterpret_cast <unsigned char *&> (m_Cvar_DirectSetFunctionPointer) != Trampolines::Bytecode::CallImmediately32);

			// Skip call instruction....
			reinterpret_cast <unsigned char *&> (m_Cvar_DirectSetFunctionPointer) += sizeof (Trampolines::Bytecode::CallImmediately32);

			// Convert relative address to real....
			m_Cvar_DirectSetFunctionPointer = reinterpret_cast <HalfLifeEngine::SDK::TypeDefinitions::Cvar_DirectSetFunction_t> (Trampolines::GetRealAddressOfRelativeAddress32 (m_Cvar_DirectSetFunctionPointer));

			// Reliability checks.
			InternalAssert (!IsBadCodePtr (reinterpret_cast <FARPROC> (m_Cvar_DirectSetFunctionPointer)));
			InternalAssert (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().IsContainingAddress (m_Cvar_DirectSetFunctionPointer));
		}
		inline void CreatePatch (void)
		{
			MemoryCopy (reinterpret_cast <unsigned char *> (m_Cvar_DirectSetFunctionPointer), m_originalBytes, s_patchSize);

			m_patchedBytes[0u] = Trampolines::Bytecode::JumpImmediately32;

			reinterpret_cast <long *> (m_patchedBytes + sizeof (Trampolines::Bytecode::JumpImmediately32))[0u] = reinterpret_cast <char *> (&Hooker) - reinterpret_cast <char *> (m_Cvar_DirectSetFunctionPointer) - s_patchSize;
		}
		inline void DoPatch (void)// const
		{
#if 0
			m_targetAddress = (void *)m_Cvar_DirectSetFunctionPointer;	// (Address: 0x1D3B580)
			reinterpret_cast <unsigned char *&> (m_targetAddress) += GET_STATIC_STRING_LENGTH
			(
				"\x8B\x4C\x24\x08"				// mov     ecx, [esp+arg_4]
				"\x81\xEC\x00\x04\x00\x00"		// sub     esp, 400h       ; Integer Subtraction
				"\x56"							// push    esi
				"\x8B\xB4\x24\x08\x04\x00\x00"	// mov     esi, [esp+404h+arg_0]
				"\x57"							// push    edi
				"\x8B\xF9"						// mov     edi, ecx
				"\x85\xF6"						// test    esi, esi        ; Logical Compare
				"\x0F\x84\xC8\x01\x00\x00"		// jz      loc_1D3B765     ; Jump if Zero (ZF=1)
				"\x85\xC9"						// test    ecx, ecx        ; Logical Compare
				"\x0F\x84\xC0\x01\x00\x00"		// jz      loc_1D3B765     ; Jump if Zero (ZF=1)
			);	// (Address: 0x1D3B5A5)

			// Reliability checks.
			InternalAssert (m_fullDetour == NULL);
			InternalAssert (static_cast <unsigned short *> (m_targetAddress)[0u] == MakeID (0xF6, 0x46));

			m_fullDetour = Detours::g_codeAllocator.Allocate <unsigned char> (Offset_FullDetourSize);

			// Reliability check.
			InternalAssert (m_fullDetour != NULL);

			// Copy the main trampoline function
			MemoryCopy (ms_detourOpcodes, m_fullDetour, sizeof (ms_detourOpcodes));

			// Copy our detour call into the trampoline
//			Trampolines::RedirectFunctionCall32 (m_fullDetour + Offset_DetourCallPosition, &Hooker2);
			unsigned char *tempAddress (m_fullDetour + Offset_DetourCallPosition);
			reinterpret_cast <unsigned int *> (tempAddress)[0u] = reinterpret_cast <unsigned int> (&Hooker2) - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);

			// Copy epilogue of target function into our trampoline
			tempAddress = m_fullDetour + Offset_DetourJumpToEpiloguePosition;
			reinterpret_cast <unsigned int *> (tempAddress)[0u] = reinterpret_cast <unsigned int> (Trampolines::GetRealAddressOfRelativeAddress32 (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (m_targetAddress) - Trampolines::Bytecode::Pointer32Size))) - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);

			// Copy original bytes onto the end of the trampoline function
			MemoryCopy (static_cast <unsigned char *> (m_targetAddress), m_fullDetour + Offset_Total, DetourCobyBytes_Cvar_DirectSet);

			// Set if printable variable address
			tempAddress = static_cast <unsigned char *> (m_targetAddress) + DetourCobyBytes_Cvar_DirectSet + sizeof ('\x74');
			const unsigned int ifPrintableVariableAddress (reinterpret_cast <unsigned int> (Trampolines::GetRealAddressOfRelativeAddress8 (tempAddress)));
			(tempAddress = m_fullDetour + Offset_DetourJumpIfPrintablePosition)[0u] = Trampolines::Bytecode::JumpIfZeroImmediately32[0u];
			(++tempAddress)[0u] = Trampolines::Bytecode::JumpIfZeroImmediately32[1u];
			reinterpret_cast <unsigned int *> (++tempAddress)[0u] = ifPrintableVariableAddress - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);

			// Patch and copy the final jmp
//			Trampolines::WriteFunctionJump32 (m_fullDetour + Offset_DetourJumpPosition, reinterpret_cast <unsigned char *> (m_targetAddress) + DetourCobyBytes_Cvar_DirectSet);
			(tempAddress = m_fullDetour + Offset_DetourJumpPosition)[0u] = Trampolines::Bytecode::JumpImmediately32;
			reinterpret_cast <unsigned int *> (++tempAddress)[0u] = reinterpret_cast <unsigned int> (m_targetAddress) + DetourCobyBytes_Cvar_DirectSet + GET_STATIC_STRING_LENGTH ("\x74\x4E") - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);
#endif	// if 0
			if (Trampolines::VirtualProtect (m_Cvar_DirectSetFunctionPointer, s_patchSize, PAGE_EXECUTE_READWRITE))
//			if (Trampolines::VirtualProtect (m_targetAddress, Trampolines::Bytecode::JumpImmediately32Size, PAGE_EXECUTE_READWRITE))
			{
				MemoryCopy (m_patchedBytes, reinterpret_cast <unsigned char *> (m_Cvar_DirectSetFunctionPointer), s_patchSize);
#if 0
			// Now overwrite the target function with our trampoline
//			Trampolines::WriteFunctionJump32 (m_targetAddress, m_fullDetour);
			unsigned char *tempAddress (static_cast <unsigned char *> (m_targetAddress));
			tempAddress[0u] = Trampolines::Bytecode::JumpImmediately32;
			reinterpret_cast <unsigned int *> (++tempAddress)[0u] = reinterpret_cast <unsigned int> (m_fullDetour) - reinterpret_cast <unsigned int> (tempAddress + Trampolines::Bytecode::Pointer32Size);
#endif	// if 0
				return;
			}

			AddLogEntry (true, LogLevel_Error, false, "Cvar_DirectSet() PATCHING FAILED!!!\n");
		}

	public:
		inline void CreateAndDoPatch (void)
		{
			// Get real engine Cvar_DirectSet() function pointer....
			InitializeCvar_DirectSetFunctionPointer ();

			CreatePatch ();
			DoPatch ();

			AddLogEntry (true, LogLevel_Default, false, "Console variable change hooker installed successfully!");
		}
		inline void UndoPatch (void) const
		{
			// Reliability checks.
			InternalAssert (!IsBadCodePtr (reinterpret_cast <FARPROC> (m_Cvar_DirectSetFunctionPointer)));
			InternalAssert (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().IsContainingAddress (m_Cvar_DirectSetFunctionPointer));

			MemoryCopy (m_originalBytes, reinterpret_cast <unsigned char *> (m_Cvar_DirectSetFunctionPointer), s_patchSize);
		}

		inline void CallOriginalFunction (HalfLifeEngine::SDK::Classes::ConsoleVariable *const variable, const char *const value) const
		{
			g_consoleVariableValueChangeHooker.UndoPatch ();

				(*m_Cvar_DirectSetFunctionPointer) (variable, value);	// Call original engine function.

			g_consoleVariableValueChangeHooker.DoPatch ();
		}

		static void Hooker (HalfLifeEngine::SDK::Classes::ConsoleVariable *const variable, const char *const value);
//		static const bool Hooker2 (HalfLifeEngine::SDK::Classes::ConsoleVariable *const variable, const char *&value);
};

#endif	// ifndef CONSOLE_VARIABLE_VALUE_CHANGE_HOOKER_INCLUDED