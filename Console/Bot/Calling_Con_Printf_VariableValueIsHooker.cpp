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
// Calling_Con_Printf_VariableValueIsHooker.cpp
//
// Class: Calling_Con_Printf_VariableValueIsHooker
//
// Version: $ID:$
//

#include <Core.hpp>

namespace Console
{
	namespace Bot
	{
		/*static */void VariableManager::Calling_Con_Printf_VariableValueIsHooker::Hooker (const char *const/* format = "\"%s\" is \"%s\"\n"*/, const char *const variableName, const char *const variableValue)
		{
			// Arguments[3] = {"\"%s\" is \"%s\"\n", variable->name, variable->string}.

			// Reliability check.
			InternalAssert (!IsNullString (variableName));

			// Quick check....
			if (strncmp (variableName, PRODUCT_CONSOLE_TAG "_", GET_STATIC_STRING_LENGTH (PRODUCT_CONSOLE_TAG "_")) != 0)
				goto Label_PrintOriginalStringAndReturn;

			// Is YaPB variable?
			for (unsigned char variableIndex (VariableID_AutoVacate); variableIndex < VariableID_Total; ++variableIndex)
			{
				// Compare name pointers.... (Much faster then comparing name strings....)
				if (variableManager.GetVariable (static_cast <VariableID_t> (variableIndex))->name != variableName)
					continue;

//				AddLogEntry (true, LogLevel_Default, true, "Calling_Con_Printf_VariableValueIsHooker::Hooker(): Called for " PRODUCT_CONSOLE_TAG " variable \"%s\"!", variableName);

				sm_variableList[variableIndex].m_onPrintHelpCallbackFunctionPointer ();

				// Don't call original function from here - give chance to do it from 'm_onPrintHelpCallbackFunctionPointer' if needed....
				return;
			}

			Label_PrintOriginalStringAndReturn:
				// Print original string to server console (call the original function....)
				variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().CallOriginalFunction (variableName, variableValue);
		}
	}
}