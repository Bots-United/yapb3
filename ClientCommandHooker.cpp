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
// ClientCommandHooker.cpp
//
// Class: ClientCommandHooker
//
// Version: $ID:$
//

#include <Core.hpp>

/*static */const unsigned char ClientManager::ClientCommandHooker::ms_detourOpcodes[] =
{
	Trampolines::Bytecode::PushEBP,																																																									// push ebp			; push the 'clientEdict' pointer
	Trampolines::Bytecode::CallImmediately32, Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],																// call <gate>		; call our Detour_ClientCommand() function
	Trampolines::Bytecode::PopEBP,																																																									// pop ebp			; store back (in case) 'clientEdict' pointer to ebp and correct stack
	0x84, 0xC0,																																																														// test al, al		; do != 0 test
	Trampolines::Bytecode::JumpIfZeroImmediately32[0u], Trampolines::Bytecode::JumpIfZeroImmediately32[1u], Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],	// jz <cont>		; if == 0, return to 'loc_10070C20' (epilogue of ClientCommand())
/*
	// Continue....
	0x8A, 0x15, Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],																								// mov dl, 'UseBotArgs' (Restore dl(edx) register to point at 'UseBotArgs' value in case dl changed by above call....)

	// execute original instruction and jump to where old function is saved otherwise
	0x8D, 0xB5, Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u],																								// lea esi, [ebp+80h]	; part of original bytes (esi = &clientEdict->variables)

	Trampolines::Bytecode::JumpImmediately32, Trampolines::Bytecode::RawNumber[0u], Trampolines::Bytecode::RawNumber[1u], Trampolines::Bytecode::RawNumber[2u], Trampolines::Bytecode::RawNumber[3u]																// jmp <ClientCommand>	; jump to original function
*/};

/*static */const unsigned char ClientManager::ClientCommandHooker::Detour_ClientCommand (const HalfLifeEngine::SDK::Classes::Edict *const clientEdict)
{
	// Reliability checks.
	InternalAssert (g_server->IsRunning ());
	InternalAssert (clientEdict->IsValid () && clientEdict->IsPlayer ());

	Client *const client (g_server->GetClientManager ()->GetClient (clientEdict));

	// Reliability check.
	InternalAssert (client->IsValid ());

	if (g_server->GetUseBotArgsReference ())
	{
//		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Server::ClientCommandHooker::Detour_ClientCommand(%s): UseBotArgs=%u, BotArgs: [0: \"%s\", 1: \"%s\", 2: \"%s\", 3: \"%s\"].\n", clientEdict->GetNetName ().GetData (), g_server->GetUseBotArgsReference (), g_server->GetBotArgsReference (0u), g_server->GetBotArgsReference (1u), g_server->GetBotArgsReference (2u), g_server->GetBotArgsReference (3u));

		return static_cast <const unsigned char> (client->HandleExecutedCommand (g_server->GetBotArgsReference (0u), g_server->GetBotArgsReference (1u) == NULL ? "" : g_server->GetBotArgsReference (1u)));
	}

//	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Server::ClientCommandHooker::Detour_ClientCommand(%s): command: \"%s\", arguments[1]: \"%s\".\n", clientEdict->GetNetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (0u).GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount_FromAll () == 1u ? "NONE" : HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (1u).GetData ());

	return static_cast <const unsigned char> (client->HandleExecutedCommand (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (0u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount_FromAll () == 1u ? "" : HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (1u)));
}