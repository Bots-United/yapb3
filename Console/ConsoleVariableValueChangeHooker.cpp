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
// ConsoleVariableValueChangeHooker.cpp
//
// Class: ConsoleVariableValueChangeHooker
//
// Version: $ID:$
//

#include <Core.hpp>

namespace Console
{
	DLL_GLOBAL ConsoleVariableValueChangeHooker g_consoleVariableValueChangeHooker;
#if 0
	/*static */const unsigned char ConsoleVariableValueChangeHooker::ms_detourOpcodes[] =
	{
		0x8D, 0x84, 0x24, 0x10, 0x04, 0x00, 0x00,																																																								// lea eax, [esp+408h+arg_4]	; push the pointer to 'value' pointer to 'eax'
		Trampolines::Bytecode::PushEAX,																																																											// push eax						; push the pointer to 'value' pointer
//		Trampolines::Bytecode::PushECX,																																																											// push ecx						; push the pointer to 'value' pointer
		Trampolines::Bytecode::PushESI,																																																											// push esi						; push the 'variable' pointer
		Trampolines::Bytecode::CallImmediately32, Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],																		// call <gate>					; call our Hooker() function
		Trampolines::Bytecode::PopESI,																																																											// pop esi						; restore the 'esi' address and correct stack
		Trampolines::Bytecode::PopECX,																																																											// pop ecx						; ecx = eax(&value) and correct stack
		0x8B, 0x09,																																																																// mov ecx, [ecx]				; value = *value
		0x8B, 0xF9,																																																																// mov edi, ecx					; edi = ecx (update 'edi' register)
		0x84, 0xC0,																																																																// test al, al					; do != 0 test
		Trampolines::Bytecode::JumpIfNotZeroImmediately32[0u], Trampolines::Bytecode::JumpIfNotZeroImmediately32[1u], Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],	// jnz <cont>					; if != 0, return to 'loc_1D3B765' (epilogue of Cvar_DirectSet())
/*
		// Continue....
		// execute original instruction and jump to where old function is saved otherwise
		0xF6, 0x46, 0x08, 0x80,																																																													// test byte ptr [esi+8], 80h	; Logical Compare (part of original bytes (if (variable->flags & FCVAR_PRINTABLEONLY)))
		Trampolines::Bytecode::JumpIfZeroImmediately32[0u], Trampolines::Bytecode::JumpIfZeroImmediately32[1u], Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],			// jz loc_1D3B5F9				; Jump if Zero (ZF=1)

		Trampolines::Bytecode::JumpImmediately32, Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u]																		// jmp <Cvar_DirectSet>			; jump to original function (Address: 0x1D3B5AB)
*/	};
	/*static */const bool ConsoleVariableValueChangeHooker::Hooker2 (HalfLifeEngine::SDK::Classes::ConsoleVariable *const variable, const char *&value)
	{
		value = "BARADA";

		AddLogEntry (true, LogLevel_Default, false, "Cvar_DirectSet(): Hooker2(): Called for variable \"%s\", value \"%s\".", variable->name, value);

		return false;
	}
#endif	// if 0
	/*static */void ConsoleVariableValueChangeHooker::Hooker (HalfLifeEngine::SDK::Classes::ConsoleVariable *const variable, const char *const value)
	{
		// Reliability checks.
		InternalAssert (variable->IsValid ());
		InternalAssert (!IsNullString (value));

		// Note: The engine compares only string.... (BTW with case check)
		if (strcmp (variable->string, value) == 0 && variable->value == atof (value))
		{
//			AddLogEntry (true, LogLevel_Default, false, "Cvar_DirectSet(): Called for variable \"%s\" with same value \"%s\".", variable->name, value);

			return;	// Same value, just return....
		}

		// Is server variable change?
		for (unsigned char variableIndex (Server::VariableID_C4Timer); variableIndex < Server::VariableID_Total; ++variableIndex)
		{
			// Compare pointers....
			if (Server::variableManager.m_consoleVariables[variableIndex] != variable)	/// @note We not use Server::VariableManager::GetVariable() function because some variables may not registered yet, so it can be NULL....
				continue;

//			AddLogEntry (true, LogLevel_Default, false, "Cvar_DirectSet(): Called for Server variable \"%s\", new string: \"%s\"!", variable->name, value);

			const OnStringChangeCallback_t callbackFunction (Server::variableManager.GetOnStringChangeCallback (static_cast <Server::VariableID_t> (variableIndex)));

			// Check if callback function is exists....
			if (callbackFunction == NULL)
				goto Label_CallOriginalFunctionAndReturn;	// Nothing, just exit from loop and call the original function....

			(*callbackFunction) (value);

			// If we have callback function, this function should call original function by itself, if needed....
			return;
		}

		// Quick check....
		if (strncmp (variable->name, PRODUCT_CONSOLE_TAG "_", GET_STATIC_STRING_LENGTH (PRODUCT_CONSOLE_TAG "_")) != 0)
			goto Label_CallOriginalFunctionAndReturn;

		// Is YaPB variable change?
		for (unsigned char variableIndex (Bot::VariableID_AutoVacate); variableIndex < Bot::VariableID_Total; ++variableIndex)
		{
			// Compare pointers....
			if (Bot::variableManager.GetVariable (static_cast <Bot::VariableID_t> (variableIndex)) != variable)
				continue;

//			AddLogEntry (true, LogLevel_Default, false, "Cvar_DirectSet(): Called for " PRODUCT_CONSOLE_TAG " variable \"%s\", new string: \"%s\"!", variable->name, value);

			const OnStringChangeCallback_t callbackFunction (Bot::variableManager.GetExtendedVariable (static_cast <Bot::VariableID_t> (variableIndex)).m_onStringChangeCallbackFunctionPointer);

			// Check if callback function is exists....
			if (callbackFunction == NULL)
				goto Label_CallOriginalFunctionAndReturn;	// Nothing, just exit from loop and call the original function....

			(*callbackFunction) (value);

			// If we have callback function, this function should call original function by itself, if needed....
			return;
		}

		Label_CallOriginalFunctionAndReturn:
			// Call the original function....
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variable, value);
	}
}