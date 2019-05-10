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
// ServerConsoleVariableManager.hpp
//
// Class: VariableManager
//
// Description: Simple server console variables container class (defined for more convenient work with console variables).
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SERVER_CONSOLE_VARIABLE_MANAGER_INCLUDED
#define SERVER_CONSOLE_VARIABLE_MANAGER_INCLUDED

class VariableManager : public Console::VariableManager
{
	friend ConsoleVariableValueChangeHooker;

	//
	// Group: Private members.
	//
	private:
		HalfLifeEngine::SDK::Classes::ConsoleVariable              *m_consoleVariables[VariableID_Total];
		ConsoleVariableValueChangeHooker::OnStringChangeCallback_t  m_onStringChangeCallbackFunctionPointer[VariableID_Total];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline VariableManager (void)
		{
			// Initialize the console variables array....
			for (unsigned char index (VariableID_C4Timer); index < VariableID_Total; ++index)
			{
				m_consoleVariables[index] = NULL;
				m_onStringChangeCallbackFunctionPointer[index] = NULL;
			}
		}

	//
	// Group: Private functions.
	//
	private:
		// On value change callbacks....

		static void OnConsoleVariableStringChange_VoiceEnable   (const char *const newString);
		static void OnConsoleVariableStringChange_Voice_codec   (const char *const newString);
		static void OnConsoleVariableStringChange_Voice_quality (const char *const newString);

		static void OnConsoleVariableStringChange_SkyColor_Red   (const char *const newString);
		static void OnConsoleVariableStringChange_SkyColor_Green (const char *const newString);
		static void OnConsoleVariableStringChange_SkyColor_Blue  (const char *const newString);

	//
	// Group: Functions.
	//
	public:
		void RegisterVariables     (void);
		void RegisterGameVariables (void);

		// Console variable access...

		//
		// Function: GetVariable
		//
		// Description: Gets console variable from specified index.
		//
		// Parameters:
		//	index - Console variable index to retrieve.
		//
		// Returns: Console variable object.
		//
		inline HalfLifeEngine::SDK::Classes::ConsoleVariable *const &GetVariable (const VariableID_t index) const
		{
			// Reliability checks.
			InternalAssert (index >= VariableID_C4Timer && index < VariableID_Total);
			InternalAssert (m_consoleVariables[index]->IsValid ());

			return m_consoleVariables[index];
		}

		inline const ConsoleVariableValueChangeHooker::OnStringChangeCallback_t &GetOnStringChangeCallback (const VariableID_t index) const
		{
			// Reliability checks.
			InternalAssert (index >= VariableID_C4Timer && index < VariableID_Total);

			return m_onStringChangeCallbackFunctionPointer[index];
		}
};

extern DLL_GLOBAL VariableManager variableManager;

#endif	// ifndef SERVER_CONSOLE_VARIABLE_MANAGER_INCLUDED