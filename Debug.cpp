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
// Debug.cpp
//
// Class: Debug
//
// Description: YaPB debugger.
//
// Version: $ID:$
//

#include <Core.hpp>

Debug g_debug;

void Debug::DebugLog (const FlagID_t flag, const char *const message)
{
	if (!IsDebugOn (flag))
		return;

	// log a message in the file
	DynamicString description;
	const tm *const localTime (GetLocalTime ());
	static const char *const debugTypeString[DebugID_Total] =
	{
		"General",
		"Engine",
		"DLL",
		"Error",
		"GameMessage",

		"BotAI",
		"BotNavigation",
		"BotMove",
		"BotCombat",
		"BotTeam",
		"BotTask",
		"BotAim"
	};

	STDIOFile file (/*g_General.BuildFilename (debug_filename)*/"Debug.log", "at");

	// Reliability check.
	if (!file.IsValid ())
		return;

	file.PrintFormat ("[%s %02i:%02i:%02i] %s\n", static_cast <const unsigned char> (flag) >= DebugID_Total ? "UNKNOWN" : debugTypeString[flag], localTime->tm_hour, localTime->tm_min, localTime->tm_sec, message);
}