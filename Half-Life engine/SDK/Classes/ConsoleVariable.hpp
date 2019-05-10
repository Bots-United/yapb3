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
// ConsoleVariable.hpp
//
// Class: ConsoleVariable
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CONSOLE_VARIABLE_INCLUDED
#define CONSOLE_VARIABLE_INCLUDED

class ConsoleVariable
{
	//
	// Group: Members.
	//
	public:
		const char                       *name;		// console variable name
		const char                       *string;	// string value
		Constants::ConsoleVariableFlag_t  flags;	// console variable flags
		float                             value;	// console variable value

		ConsoleVariable                  *next;		// link to next structure

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ConsoleVariable (const char *const inputName, const char *const inputString, const Constants::ConsoleVariableFlag_t inputFlags) :
			name (inputName),
			string (inputString),
			flags (inputFlags),

			value (0.0f),
			next (NULL)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsValid
		//
		// Description: Checks whether console variable is valid.
		//
		// Returns: True if console variable valid, false otherwise.
		//
		inline const bool                                        IsValid   (void)                    const
		{
			#if defined _DEBUG
				return this != NULL && this == HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetPointer (name);
			#else	// if defined _DEBUG
				return this != NULL;
			#endif	// if !defined _DEBUG
		}

		inline const DynamicString                               GetName   (void)                    const { return name; }

		inline const DynamicString                               GetString (void)                    const { return string; }
		inline void                                              SetString (const char *const value)       { Globals::g_halfLifeEngine->ConsoleVariable_SetString (name, value); }

		template <typename elementType> inline const elementType GetValue  (void)                    const { return static_cast <const elementType> (static_cast <const int> (value)); }
		template </* bool */>           inline const bool        GetValue  (void)                    const { return value != 0.0f; }
		template </* float */>          inline const float       GetValue  (void)                    const { return value; }

		template <typename elementType> inline void              SetValue  (const elementType value)       { Globals::g_halfLifeEngine->ConsoleVariable_SetValue (name, value); }
};

#endif	// ifndef CONSOLE_VARIABLE_INCLUDED