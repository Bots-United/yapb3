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
// utility_API.hpp
//
// Description: Meta utility function table type.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef UTILITY_API_INCLUDED
#define UTILITY_API_INCLUDED

struct MetaModUtilityFunction_t
{
	void              (*pfnLogConsole)           (const Structures::PluginInfo_t *const pluginInformation, const char *const format, ...);	// Log to console; newline added.
	void              (*pfnLogMessage)           (const Structures::PluginInfo_t *const pluginInformation, const char *const format, ...);	// Log regular message to logs; newline added.
	void              (*pfnLogError)             (const Structures::PluginInfo_t *const pluginInformation, const char *const format, ...);	// Log an error message to logs; newline added.
	void              (*pfnLogDeveloper)         (const Structures::PluginInfo_t *const pluginInformation, const char *const format, ...);	// Log a message only if console variable "developer" set; newline added.
	void              (*pfnCenterSay)            (const Structures::PluginInfo_t *const pluginInformation, const char *const format, ...);	// Print message on center of all player's screens. (Uses default text parameters (color green, 10 second fade-in).)
	void              (*pfnCenterSayParms)       (const Structures::PluginInfo_t *const pluginInformation, const Structures::HUDTextParameter_t HUDTextParameters, const char *const format, ...);	// Print a center-message, with given text parameters.
	void              (*pfnCenterSayVarargs)     (const Structures::PluginInfo_t *const pluginInformation, const Structures::HUDTextParameter_t HUDTextParameters, const char *const format, va_list argumentPointer);	// Print a center-message, with text parameters and varargs. (Provides functionality to the above center_say interfaces.)
	const BOOL        (*pfnCallGameEntity)       (const Structures::PluginInfo_t *const pluginInformation, const char *const className, HalfLifeEngine::SDK::Structures::EntityVariables_t *const entityVariables);	// Allow plugins to call the entity functions in the game.DLL. (In particular, calling "player()" as needed by most Bots.)
	const int         (*pfnGetUserMsgID)         (const Structures::PluginInfo_t *const pluginInformation, const char *const name, int *const size);	// Find a user message, registered by the game.DLL, with the corresponding 'name', and return remaining info about it (id, size).
	const char *const (*pfnGetUserMsgName)       (const Structures::PluginInfo_t *const pluginInformation, const int id, int *const size);	// Find a user message, registered by the game.DLL, with the corresponding 'id', and return remaining info about it (name, size).
	const char *const (*pfnGetPluginPath)        (const Structures::PluginInfo_t *const pluginInformation);	// Return the full path of the plugin's loaded DLL/so file.
	const char *const (*pfnGetGameInfo)          (const Structures::PluginInfo_t *const pluginInformation, const Constants::GameInfo_t tag);	// Return various string-based info about the game/MOD/game.DLL.
	const int         (*pfnLoadPlugin)           (const Structures::PluginInfo_t *const pluginInformation, const char *const commandLine, const Constants::PluginLoadTime_t now, HMODULE *const pluginHandle);
	const int         (*pfnUnloadPlugin)         (const Structures::PluginInfo_t *const pluginInformation, const char *const commandLine, const Constants::PluginLoadTime_t now, const Constants::PluginUnloadReason_t reason);
	const int         (*pfnUnloadPluginByHandle) (const Structures::PluginInfo_t *const pluginInformation, HMODULE pluginHandle, const Constants::PluginLoadTime_t now, const Constants::PluginUnloadReason_t reason);
	const char *const (*pfnIsQueryingClientCvar) (const Structures::PluginInfo_t *const pluginInformation, const HalfLifeEngine::SDK::Classes::Edict *player);	// Check if player is being queried for console variable.
	const int         (*pfnMakeRequestID)        (const Structures::PluginInfo_t *const pluginInformation);
	void              (*pfnGetHookTables)        (const Structures::PluginInfo_t *const pluginInformation, HalfLifeEngine::SDK::Structures::EngineFunction_t **const engineFunctions, HalfLifeEngine::SDK::Structures::DLLFunction_t **const DLLFunctions, HalfLifeEngine::SDK::Structures::NewDLLFunction_t **const newDLLFunctions);
};

#endif	// ifndef UTILITY_API_INCLUDED