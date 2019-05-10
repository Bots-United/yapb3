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
// Console_definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CONSOLE_DEFINITIONS_INCLUDED
#define CONSOLE_DEFINITIONS_INCLUDED

#include <Console/ConsoleVariableValueChangeHooker.hpp>

class VariableManager
{/*
	//
	// Group: Type definitions.
	//
	public:
		typedef unsigned char VariableID_t;
*/
	//
	// Group: (Con/De)structors.
	//
	public:
		virtual inline ~VariableManager (void) { /* VOID */ }

	//
	// Group: Private operators.
	//
	private:
		inline VariableManager &operator = (const VariableManager &/*right*/);	// Avoid "warning C4512: 'VariableManager' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		virtual void RegisterVariables (void) = 0;
/*
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
		virtual inline HalfLifeEngine::SDK::Classes::ConsoleVariable *const &GetVariable (const VariableID_t index) const = 0;*/
};

namespace Bot
{
	#include <Console/Bot/BotVariableID.hpp>

	#include <Console/Bot/BotConsoleVariableManager.hpp>
}

namespace Server
{
	#include <Console/Server/ServerVariableID.hpp>

	#include <Console/Server/ServerConsoleVariableManager.hpp>
}

//#include <Console/ConsoleCommands.hpp>

#endif	// ifndef CONSOLE_DEFINITIONS_INCLUDED