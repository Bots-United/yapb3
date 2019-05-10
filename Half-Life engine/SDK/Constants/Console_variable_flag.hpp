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
// Console_variable_flag.hpp
//
// Description: Used for 'Classes::ConsoleVariable' class.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CONSOLE_VARIABLE_FLAG_INCLUDED
#define CONSOLE_VARIABLE_FLAG_INCLUDED

DECLARE_TYPED_ENUMERATION (int, ConsoleVariableFlag_t)
{
	ConsoleVariableFlag_None,						// The default, no flags at all.

	ConsoleVariableFlag_Archive        = BIT (0u),	// set to cause it to be saved to vars.rc
	ConsoleVariableFlag_UserInfo       = BIT (1u),	// changes the client's info string
	ConsoleVariableFlag_Server         = BIT (2u),	// notifies players when changed
	ConsoleVariableFlag_ExternalDLL    = BIT (3u),	// defined by external DLL (Note: this flag is automaticly added by pfnCVarRegister() function!)
	ConsoleVariableFlag_ClientDLL      = BIT (4u),	// defined by the client.DLL
	ConsoleVariableFlag_Protected      = BIT (5u),	// it's a server console variable, but we don't send the data since it's a password, etc. Sends 1 if it's not bland/zero, 0 otherwise as value
	ConsoleVariableFlag_ServerPartOnly = BIT (6u),	// this console variable cannot be changed by clients connected to a multiplayer server
	ConsoleVariableFlag_PrintableOnly  = BIT (7u),	// this console variable's string cannot contain unprintable characters (e.g., used for player name etc).
	ConsoleVariableFlag_Unlogged       = BIT (8u)	// if this is a 'ConsoleVariableFlag_Server', don't log changes to the log file/console if we are creating a log
};

#endif	// ifndef CONSOLE_VARIABLE_FLAG_INCLUDED