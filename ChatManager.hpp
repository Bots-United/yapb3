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
// ChatManager.hpp
//
// Class: ChatManager
//
// Description: YaPB chat manager.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CHAT_MANAGER_INCLUDED
#define CHAT_MANAGER_INCLUDED

class ChatManager
{
	//
	// Group: Constants.
	//
	public:
		#define CHAT_MANAGER_CONFIG_FILENAME                "chat"
		#define CHAT_MANAGER_CONFIG_FILENAME_EXTANTION      "cfg"
		#define CHAT_MANAGER_CONFIG_FILENAME_WITH_EXTENTION CHAT_MANAGER_CONFIG_FILENAME "." CHAT_MANAGER_CONFIG_FILENAME_EXTANTION

		// Chat types ID's
		enum ChatType_t
		{
			ChatType_None = -1,		// ID to undefined chat array

			ChatType_Killing,		// ID to kill chat array
			ChatType_Dead,			// ID to dead chat array
			ChatType_BombPlant,		// ID to bomb chat array
			ChatType_TeamAttack,	// ID to team-attack chat array
			ChatType_TeamKill,		// ID to team-kill chat array
			ChatType_Welcome,		// ID to welcome chat array
			ChatType_NoKeyword,		// ID to no keyword chat array

			// total of chat types array
			ChatType_Total
		};

		#include <Chat.hpp>

	//
	// Group: Private members.
	//
	private:
		DynamicArray <Chat_t, unsigned char> m_chat[ChatType_Total];	// Array of known bot chat, loaded from configuration file.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ChatManager (void)
		{
			// Load bot chat from the disk....

			LoadChat ();
		}

	//
	// Group: Private functions.
	//
	private:
/*		inline const bool IsBotNameExists (const DynamicString &name) const
		{
			// Reliability check.
			if (m_chat.IsEmpty ())
				return false;	// No match.

			for (unsigned char index (0u); index < m_chat.GetElementNumber (); ++index)
				if (m_chat[index].string.CompareWithoutCaseCheck (name) == 0)
					return true;	// Found a match.

			return false;	// No match.
		}
*/
	//
	// Group: Functions.
	//
	public:
		void        LoadChat (void);
		inline void FreeChat (void)
		{
			// Free all the memory allocated for bot chat.

			for (unsigned char index (0u); index < ChatType_Total; ++index)
				m_chat[index].RemoveAll ();
		}

		Chat_t *const PickChat (const unsigned char skill = 0u);
		Chat_t *const PickChat (const DynamicString &botName);

		void PrintChat (void) const;
};

#endif	// ifndef CHAT_MANAGER_INCLUDED