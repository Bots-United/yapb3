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
// FakeClientCommand.cpp
//
// Class: FakeClientCommand.
//
// Description: Simple fake client command class.
//
// Version: $ID:$
//

#include <Core.hpp>

void ClientManager::FakeClientCommand::Execute (HalfLifeEngine::SDK::Classes::Edict *const fakeClient, const DynamicString &command)
{
	// the purpose of this function is to provide fake clients (bots) with the same client command-scripting advantages (putting multiple commands in one line between semicolons) as real players.
	// it is an improved version of botman's FakeClientCommand(), in which you supply directly the whole string as if you were typing it in the bot's "console".
	// it is supposed to work exactly like the pfnClientCommand() (server-sided client command).

	// Reliability checks. (if nothing in the command buffer, return)
	InternalAssert (fakeClient->IsValid ());
//	InternalAssert (fakeClient->IsFakeClient ());	// OCCURS!!!!
	InternalAssert (!command.IsEmpty ());
	InternalAssert (command.GetElementNumber () <= HalfLifeEngine::SDK::Constants::MaximumClientCommandLength);

	unsigned short start, stop, index, stringIndex (0u);
	unsigned char quotesNumber;
	static const CharacterSet characterBreakSetIncludingColons ("{}()':,");	// wordbreak parsing set

	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPICmdFunctions ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIClientCommandFunction ();
	g_server->GetClientManager ()->GetClientCommandHooker ().UndoPatch ();	// Reset our callback....

	// process all individual commands (separated by a semicolon) one each a time
	do
	{
		m_command = NULL;
		m_arguments.RemoveAll ();	// let's now parse that command and count the different arguments

		// save field start position (first character)
		start = stringIndex;

		// find a '\n' or ';' line break
		for (quotesNumber = 0u; stringIndex < command.GetElementNumber (); ++stringIndex)
		{
			if (command[stringIndex] == '\n')
				break;	// reach end of field

			if (command[stringIndex] == '"')
				++quotesNumber;

			if ((quotesNumber & 1u) == 0u && command[stringIndex] == ';')
				break;	// don't break if inside a quoted string
		}

		stop = stringIndex;	// discard any trailing '\n' if needed and save field stop position (last character before semicolon or end)

		// Reliability check.
		InternalAssert (start < stop);

		// count the number of arguments
		do
		{
			while (start < stop && command[start] <= ' ')
				++start;	// ignore spaces (skip whitespaces up to a '!')

			// Command is just a whitespace?
			if (start == stop)
				break;

			if (m_arguments.GetElementNumber () == 1u)
				m_command = command.GetData () + start;	// Store the full command string....

			// skip // comments
			if (command[start] == '/' && command[start + sizeof ('/')] == '/')
				break;

			// is this field a group of words between quotes or a single word? (handle quoted strings specially)
			if (command[start] == '"')
			{
				// move one step further to bypass the quote
				index = ++start;

				while (index < stop && command[index] != '"')
					++index;	// reach end of field
			}
			else
			{
				// parse single characters
				if (characterBreakSetIncludingColons[command[start]])
					index = start + sizeof (command[start]);
				else
				{
					// parse a regular word

					// move one step further to bypass the regular character
					index = start + sizeof (command[start]);

					while (index < stop && !characterBreakSetIncludingColons[command[index]] && command[index] > ' ')
						++index;	// this is a single word, so reach the end of field
				}
			}

			// Reliability check.
			InternalAssert (start < index);	/// @warning OCCURS WHEN COMMAND FOR EXAMPLE IS: "say ""string" string""!!!
/*			if (start >= index)
				break;//continue;
*/
			const unsigned short argumentLength (index - start);

			// Reliability check.
			InternalAssert (argumentLength <= HalfLifeEngine::SDK::Constants::MaximumCommandArgumenLength);

			m_arguments += command.GetSubstring (start, argumentLength);

			// we have processed one argument more....
		} while ((start = ++index) < stop);

		// tell now the MOD DLL to execute this client command...
		HalfLifeEngine::Globals::g_halfLifeEngine->ClientCommand (fakeClient);
	} while (++stringIndex < command.GetElementNumber ());	// move the overall string index one step further to bypass the semicolon

	// reset the argument count
	m_arguments.RemoveAll ();

	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPICmdFunctions ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIClientCommandFunction ();
	g_server->GetClientManager ()->GetClientCommandHooker ().DoPatch ();	// Restore our callback....
}