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
// Debug.hpp
//
// Class: Debug
//
// Description: YaPB debugger.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef DEBUG_INCLUDED
#define DEBUG_INCLUDED

class Debug
{
	//
	// Group: Constants.
	//
	public:
		// Debug flags.
		enum FlagID_t
		{
			DebugID_General,
			DebugID_EngineAPI,
			DebugID_DLLAPI,
			DebugID_Error,
			DebugID_GameMessage,

			DebugID_BotAI,
			DebugID_BotNavigation,
			DebugID_BotMove,
			DebugID_BotCombat,
			DebugID_BotTeam,
			DebugID_BotTask,
			DebugID_BotAim,

			DebugID_Total
		};
		DECLARE_TYPED_ENUMERATION (unsigned int, FlagBit_t)
		{
			DebugBit_None,

			DebugBit_General       = BIT (DebugID_General),
			DebugBit_EngineAPI     = BIT (DebugID_EngineAPI),
			DebugBit_DLLAPI        = BIT (DebugID_DLLAPI),
			DebugBit_Error         = BIT (DebugID_Error),
			DebugBit_GameMessage   = BIT (DebugID_GameMessage),

			DebugBit_BotAI         = BIT (DebugID_BotAI),
			DebugBit_BotNavigation = BIT (DebugID_BotNavigation),
			DebugBit_BotMove       = BIT (DebugID_BotMove),
			DebugBit_BotCombat     = BIT (DebugID_BotCombat),
			DebugBit_BotTeam       = BIT (DebugID_BotTeam),
			DebugBit_BotTask       = BIT (DebugID_BotTask),
			DebugBit_BotAim        = BIT (DebugID_BotAim)
		};

	//
	// Group: Private members.
	//
	private:
		FlagBit_t m_flags;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Debug (void) : m_flags (DebugBit_None) { /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		inline void       DebugOn        (const FlagBit_t flag) { m_flags |= flag; }
		inline void       DebugOff       (const FlagBit_t flag) { m_flags &= ~flag; }

		inline const bool IsDebugOn      (const FlagBit_t flag) const { return (m_flags & flag) > DebugBit_None; }

		void              DebugLog       (const FlagID_t flag, const char *const message);
		void              DebugLogFormat (const FlagID_t flag, const char *const format, ...)
		{
			// log a message in the file
			char logBuffer[256u];
			va_list va_alist;

			// concatenate all the arguments in one string
			va_start (va_alist, format);
			_vsnprintf (logBuffer, sizeof (logBuffer), format, va_alist);
			va_end (va_alist);

			DebugLog (flag, logBuffer);
		}
};

extern Debug g_debug;

#endif	// ifndef DEBUG_INCLUDED