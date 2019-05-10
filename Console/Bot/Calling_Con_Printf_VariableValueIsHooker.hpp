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
// Calling_Con_Printf_VariableValueIsHooker.hpp
//
// Class: Calling_Con_Printf_VariableValueIsHooker
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CALLING_CON_PRINTF_VARIABLE_VALUE_IS_HOOKER_INCLUDED
#define CALLING_CON_PRINTF_VARIABLE_VALUE_IS_HOOKER_INCLUDED

class Calling_Con_Printf_VariableValueIsHooker
{
	public:
		typedef void (*const OnPrintHelpCallback_t) (void);

	private:
		HalfLifeEngine::SDK::TypeDefinitions::Con_PrintfFunction_t m_Con_PrintfFunctionPointer;	/// @note 'HalfLifeEngine::SDK::Structures::EngineFunction_t::pfnServerPrint()' is called this function directly, so there is no difference which function to call, original or ServerPrint()....

		typedef StaticArray <void *, unsigned char, 2u> HookedAddressArray_t;
		HookedAddressArray_t                                       m_hookedAddresses;

	public:
		inline Calling_Con_Printf_VariableValueIsHooker (void) : m_Con_PrintfFunctionPointer (NULL) { /* VOID */ }

	private:
		inline void CallOriginalFunction (const char *const variableName, const char *const variableValue) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat/*(*m_Con_PrintfFunctionPointer)*/ (reinterpret_cast <const char *const> (HalfLifeEngine::SDK::Constants::signature_Calling_Con_Printf_VariableValueIs), variableName, variableValue);
		}

	public:
		inline void DoPatch (void)
		{
			// Reliability check.
			InternalAssert (m_hookedAddresses.IsEmpty ());

			// Find string "\"%s\" is \"%s\"\n" address....
			void *string (MemoryUtilities::FindPatternWithoutWildCard (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), HalfLifeEngine::SDK::Constants::signature_Calling_Con_Printf_VariableValueIs, GET_STATIC_STRING_LENGTH (HalfLifeEngine::SDK::Constants::signature_Calling_Con_Printf_VariableValueIs)));

			if (string == NULL)
				return;

			// For all strings....
			do
			{
				// Find addres where string "\"%s\" is \"%s\"\n" is used....
				void *address (MemoryUtilities::FindMemoryChunkReference32 (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), string, Trampolines::Bytecode::Push32));

				if (address == NULL)
					return;

				// For all addresses where this string used....
				do
				{
					// Skip push instruction and address....
					reinterpret_cast <unsigned char *&> (address) += Trampolines::Bytecode::Push32Size;

					// Skip call instruction....
					reinterpret_cast <unsigned char *&> (address) += sizeof (Trampolines::Bytecode::CallImmediately32);

					if (Trampolines::RedirectFunctionCall32 (address, &Hooker, reinterpret_cast <const void *&> (m_Con_PrintfFunctionPointer)))
						m_hookedAddresses += address;

					// Skip call address....
					reinterpret_cast <unsigned char *&> (address) += Trampolines::Bytecode::Pointer32Size;
				} while ((address = MemoryUtilities::FindMemoryChunkReference32 (address, HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetSize () - (reinterpret_cast <unsigned int> (address) - reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ())), string, Trampolines::Bytecode::Push32)) != NULL);

				// Skip string address....
				reinterpret_cast <unsigned char *&> (string) += sizeof (HalfLifeEngine::SDK::Constants::signature_Calling_Con_Printf_VariableValueIs);
			} while ((string = MemoryUtilities::FindPatternWithoutWildCard (string, HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetSize () - (reinterpret_cast <unsigned int> (string) - reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ())), HalfLifeEngine::SDK::Constants::signature_Calling_Con_Printf_VariableValueIs, GET_STATIC_STRING_LENGTH (HalfLifeEngine::SDK::Constants::signature_Calling_Con_Printf_VariableValueIs))) != NULL);

			// Reliability checks.
			InternalAssert (!IsBadCodePtr (reinterpret_cast <FARPROC> (m_Con_PrintfFunctionPointer)));
			InternalAssert (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().IsContainingAddress (m_Con_PrintfFunctionPointer));

			AddLogEntry (true, LogLevel_Default, false, "Console variable help shower installed successfully for %u addresses!", m_hookedAddresses.GetElementNumber ());
		}
		inline void UndoPatch (void)
		{
			if (m_hookedAddresses.IsEmpty ())
				return;

			// Reliability checks.
			InternalAssert (!IsBadCodePtr (reinterpret_cast <FARPROC> (m_Con_PrintfFunctionPointer)));
			InternalAssert (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().IsContainingAddress (m_Con_PrintfFunctionPointer));

			for (HookedAddressArray_t::IndexType_t index (0u); index < m_hookedAddresses.GetElementNumber (); ++index)
				Trampolines::RedirectFunctionCall32 (m_hookedAddresses[index], m_Con_PrintfFunctionPointer);

			m_hookedAddresses.RemoveAll ();
		}

	public:
		static void Hooker (const char *const format, const char *const variableName, const char *const variableValue);

		inline void CallOriginalFunction (const VariableID_t variableID) const
		{
			CallOriginalFunction (variableManager.GetVariable (variableID)->name, variableManager.GetVariable (variableID)->string);
		}
};

#endif	// ifndef CALLING_CON_PRINTF_VARIABLE_VALUE_IS_HOOKER_INCLUDED