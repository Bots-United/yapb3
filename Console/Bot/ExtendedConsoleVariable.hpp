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
// ExtendedConsoleVariable.hpp
//
// Class: ExtendedConsoleVariable
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef EXTENDED_CONSOLE_VARIABLE_INCLUDED
#define EXTENDED_CONSOLE_VARIABLE_INCLUDED

class ExtendedConsoleVariable : public HalfLifeEngine::SDK::Classes::ConsoleVariable
{
	//
	// Group: Members.
	//
	public:
		const char *const                                                     m_defaultString;

		const ConsoleVariableValueChangeHooker::OnStringChangeCallback_t      m_onStringChangeCallbackFunctionPointer;	// Called when a 'ConsoleVariable' changes string or value
		const Calling_Con_Printf_VariableValueIsHooker::OnPrintHelpCallback_t m_onPrintHelpCallbackFunctionPointer;		// Called when a 'ConsoleVariable' should print help about itself.

	public:
		inline ExtendedConsoleVariable (const char *const name, const char *const string, const HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_t flags, const char *const defaultString, const ConsoleVariableValueChangeHooker::OnStringChangeCallback_t onStringChangeCallbackFunctionPointer, const Calling_Con_Printf_VariableValueIsHooker::OnPrintHelpCallback_t onPrintHelpCallbackFunctionPointer) :
			HalfLifeEngine::SDK::Classes::ConsoleVariable (name, string, flags),

			m_defaultString (defaultString),

			m_onStringChangeCallbackFunctionPointer (onStringChangeCallbackFunctionPointer),
			m_onPrintHelpCallbackFunctionPointer (onPrintHelpCallbackFunctionPointer)
		{ /* VOID */ }

	//
	// Group: Private operators.
	//
	private:
		inline ExtendedConsoleVariable &operator = (const ExtendedConsoleVariable &/*right*/);	// Avoid "warning C4512: 'ExtendedConsoleVariable' : assignment operator could not be generated".
};

#endif	// ifndef EXTENDED_CONSOLE_VARIABLE_INCLUDED