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
// FakeClientCommand.hpp
//
// Class: FakeClientCommand.
//
// Description: Simple fake client command class.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef FAKE_CLIENT_COMMAND_INCLUDED
#define FAKE_CLIENT_COMMAND_INCLUDED

class FakeClientCommand
{
	//
	// Group: Private members.
	//
	private:
//		const char **const  m_arguments;
//		bool               &m_useArguments;

		typedef StaticArray <DynamicString, unsigned char, HalfLifeEngine::SDK::Constants::MaximumCommandArgumens> ArgumentsArray_t;

		const char       *m_command;	// full command string
		ArgumentsArray_t  m_arguments;	// current command arguments

	//
	// Group: Operators.
	//
	public:
		inline operator const bool        (void) const { return m_arguments.IsEmpty () == false; }	// is command issued by fake client?
		inline operator const char *const (void) const { return m_command; }

	//
	// Group: Functions.
	//
	public:
		inline const ArgumentsArray_t::IndexType_t GetArgumentsNumber  (void)                        const { return m_arguments.GetElementNumber (); }

		// This function gets and returns a particuliar field in a 'string' where several fields are
		// concatenated. Fields can be words, or groups of words between quotes; separators may be
		// white space or tabs. A purpose of this function is to provide bots with the same Cmd_Argv()
		// convenience the engine provides to real clients. This way the handling of real client
		// commands and bot client commands is exactly the same, just have a look in engine.cpp
		// for the hooking of pfnCmd_Argc(), pfnCmd_Args() and pfnCmd_Argv(), which redirects the call
		// either to the actual engine functions (when the caller is a real client), either on
		// our function here, which does the same thing, when the caller is a bot.
		inline const char *const   GetArgument         (const ArgumentsArray_t::IndexType_t fieldID) const { return m_arguments[fieldID]; }

		void                       Execute             (HalfLifeEngine::SDK::Classes::Edict *const fakeClient, const DynamicString &command);
		inline void                ExecuteWithSeparatedArguments (HalfLifeEngine::SDK::Classes::Edict *const fakeClient, const char *const command, const char *const argument1 = NULL, const char *const argument2 = NULL, const char *const argument3 = NULL) const
		{
			// Reliability checks. (if nothing in the command buffer, return)
			InternalAssert (fakeClient->IsValid ());
			InternalAssert (fakeClient->IsFakeClient ());
			InternalAssert (!IsNullOrEmptyString (command));

			g_server->GetBotArgsReference (0u) = command;
			g_server->GetBotArgsReference (1u) = argument1;
			g_server->GetBotArgsReference (2u) = argument2;
			g_server->GetBotArgsReference (3u) = argument3;

			g_server->GetUseBotArgsReference () = true;	// set the "fakeclient command" flag

			HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIClientCommandFunction ();
			g_server->GetClientManager ()->GetClientCommandHooker ().UndoPatch ();

			// tell now the MOD DLL to execute this client command...
			HalfLifeEngine::Globals::g_halfLifeEngine->ClientCommand (fakeClient);

			HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIClientCommandFunction ();
			g_server->GetClientManager ()->GetClientCommandHooker ().DoPatch ();

			g_server->GetUseBotArgsReference () = false;	// reset the "fakeclient command" flag
		}
};

#endif	// ifndef FAKE_CLIENT_COMMAND_INCLUDED