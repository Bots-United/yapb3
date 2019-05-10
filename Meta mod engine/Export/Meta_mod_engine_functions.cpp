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
// Meta_mod_engine_functions.cpp
//
// Description: Minimal implementation of metamod's plugin interface.
//				This is intended to illustrate the (more or less) bare minimum code
//				required for a valid metamod plugin, and is targeted at those who want
//				to port existing HL/SDK DLL code to run as a metamod plugin.
//
// Call queue:
//	1) Meta_Init() - if present in plugin.DLL.
//	2) GiveFnptrsToDll()
//	3) Meta_Query()
//	4) Meta_PExtGiveFnptrs() - if running under MetaMod-P version and function present in plugin.DLL.
//	5) Meta_Attach()
//
//	6) GetNewDLLFunctions() - if present in plugin.DLL.
//	7) GetNewDLLFunctions_Post() - if present in plugin.DLL.
//	8) GetEntityAPI2() - if present in plugin.DLL.
//	9) GetEntityAPI2_Post() - if present in plugin.DLL.
//	10) GetEntityAPI() - if present in plugin.DLL.
//	11) GetEntityAPI_Post() - if present in plugin.DLL.
//	12) GetEngineFunctions() - if present in plugin.DLL.
//	13) GetEngineFunctions_Post() - if present in plugin.DLL.
//
// Version: $ID:$
//

#include <Core.hpp>	// Of course

namespace MetaModEngine
{
	namespace Export
	{
		EXPORT void Meta_Init (void)
		{
			// This function is called by metamod, before any other interface functions.
			// Purpose of this function to give plugin a chance to determine is plugin running under metamod or not.

			Globals::g_metaModEngine = new Engine ();

			// Reliability check.
			if (Globals::g_metaModEngine == NULL)
			{
				g_YaPBNotLoaded = true;

				TerminateOnMalloc ();
			}
		}

		EXPORT BOOL Meta_Query (const char *const interfaceVersion, const SDK::Structures::PluginInfo_t **const pluginInfo, const SDK::Structures::MetaModUtilityFunction_t *const utilityFunctions)
		{
			// this function is the first function ever called by metamod in the plugin.DLL. Its purpose
			// is for metamod to retrieve basic information about the plugin, such as its meta-interface
			// version, for ensuring compatibility with the current version of the running metamod.

			// meta mod requesting info about this plugin:
			//	'interfaceVersion' - 'INTERFACE_VERSION' metamod is using (given)
			//	'pluginInfo'       - struct with info about plugin (requested)
			//	'utilityFunctions' - table of utility functions provided by metamod (given)

			if (g_YaPBNotLoaded)
				return FALSE;

			#if defined _DEBUG
				// check for valid 'utilityFunctions' before we continue
				if (utilityFunctions == NULL)
				{
					AddLogEntry (false, LogLevel_Critical, false, "Function Meta_Query() called with null 'utilityFunctions' pointer!");

					return FALSE;
				}
			#endif	// if defined _DEBUG

			*pluginInfo = &Engine::sm_pluginInfo;	// Give metamod our 'PluginInfo_t' structure.

			// Keep track of the pointers to metamod function tables metamod gives us....
			Globals::g_metaModEngine->m_utilityFunctions = utilityFunctions;	// Get metamod utility function table.

			// check for interface version compatibility
			if (interfaceVersion != SDK::Constants::InterfaceVersion)
			{
				int metaMajor (0), metaMinor (0), pluginMajor (0), pluginMinor (0);

				AddLogEntry (true, LogLevel_Warning, false, "Meta mod interface version mismatch (meta mod: %s, %s: %s).", interfaceVersion, Engine::sm_pluginInfo.name, SDK::Constants::InterfaceVersion.GetData ());

				// if plugin has later interface version, it's incompatible (update metamod)
				sscanf (interfaceVersion, "%i:%i", &metaMajor, &metaMinor);
				sscanf (SDK::Constants::InterfaceVersion, "%i:%i", &pluginMajor, &pluginMinor);

				if (pluginMajor > metaMajor || (pluginMajor == metaMajor && pluginMinor > metaMinor))
				{
					AddLogEntry (true, LogLevel_Error, false, "Metamod version is too old for this plugin; update metamod.");

					return FALSE;
				}
				else if (pluginMajor < metaMajor)	// if plugin has older major interface version, it's incompatible (update plugin)
				{
					AddLogEntry (true, LogLevel_Error, false, "Metamod version is incompatible with this plugin; please find a newer version of this plugin.");

					return FALSE;
				}
				else if (pluginMajor == metaMajor && pluginMinor < metaMinor)	// minor interface is older, but this is guaranteed to be backwards compatible, so we warn, but we still accept it
					AddLogEntry (true, LogLevel_Warning, false, "Metamod version is newer than expected; consider finding a newer version of this module.");
				else
					AddLogEntry (true, LogLevel_Error, false, "Unexpected version comparison; meta: interface version=%s, major=%i, minor=%i; plugin: version=%s, major=%i, minor=%i.", interfaceVersion, metaMajor, metaMinor, SDK::Constants::InterfaceVersion.GetData (), pluginMajor, pluginMinor);
			}

			// tell metamod this plugin looks safe
			return TRUE;
		}

		EXPORT BOOL Meta_Attach (SDK::Constants::PluginLoadTime_t now, SDK::Structures::MetaModFunction_t *functionTable, SDK::Structures::MetaModGlobal_t *globals, SDK::Structures::GameDLLFunction_t *gameDLLFunctions)
		{
			// this function is called when metamod attempts to load the plugin. Since it's the place
			// where we can tell if the plugin will be allowed to run or not, we wait until here to make
			// our initialization stuff, like registering console variables and dedicated server commands.

			// metamod attaching plugin to the server.
			//	'now'              - current phase, ie during map, during change level, or at startup (given)
			//	'functionTable'    - table of function tables this plugin catches (requested)
			//	'globals'          - global vars from metamod (given)
			//	'gameDLLFunctions' - copy of function tables from game.DLL (given)

			if (g_YaPBNotLoaded)
				return FALSE;

			// are we allowed to load this plugin now?
			if (now > Engine::sm_pluginInfo.loadable)
			{
				AddLogEntry (true, LogLevel_Error, false, "Plugin NOT attaching (can't load plugin right now).");

				// returning false prevents metamod from attaching this plugin
				return FALSE;
			}

			#if defined _DEBUG
				// Reliability check.
				if (functionTable == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Plugin NOT attaching (function Meta_Attach() called with null 'functionTable' pointer).");

					// returning false prevents metamod from attaching this plugin
					return FALSE;
				}

				// Reliability check.
				if (globals == NULL)
				{
					AddLogEntry (true, LogLevel_Warning, false, "Plugin NOT attaching (function Meta_Attach() called with null 'globals' pointer).");

					// returning false prevents metamod from attaching this plugin
					return FALSE;
				}
			#endif	// if defined _DEBUG

			functionTable->pfnGetEntityAPI2           = HalfLifeEngine::Export::GetEntityAPI2;				// pfnEntityAPI2() (HL SDK2; called before game.DLL)
			functionTable->pfnGetEntityAPI2_Post      = HalfLifeEngine::Export::GetEntityAPI2_Post;			// pfnEntityAPI2_Post() (META; called after game.DLL)
			functionTable->pfnGetNewDLLFunctions      = HalfLifeEngine::Export::GetNewDLLFunctions;			// pfnGetNewDLLFunctions() (HL SDK2; called before game.DLL)
			functionTable->pfnGetEngineFunctions      = HalfLifeEngine::Export::GetEngineFunctions;			// pfnGetEngineFunctions() (META; called before HL engine)
			functionTable->pfnGetEngineFunctions_Post = HalfLifeEngine::Export::GetEngineFunctions_Post;	// pfnGetEngineFunctions_Post() (META; called after HL engine)

			// Keep track of the pointers to engine function tables metamod gives us....
			Globals::g_metaModEngine->m_globalVariables = globals;
			HalfLifeEngine::Globals::g_halfLifeEngine->m_gameDLLFunctionAPITablePointer = gameDLLFunctions->DLLFunctionAPITable;
			HalfLifeEngine::Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable = *gameDLLFunctions->DLLFunctionAPITable;
			HalfLifeEngine::Globals::g_halfLifeEngine->m_newGameDLLFunctionAPITablePointer = gameDLLFunctions->newDLLFunctionAPITable;
			HalfLifeEngine::Globals::g_halfLifeEngine->m_newGameDLLFunctionAPITable = *gameDLLFunctions->newDLLFunctionAPITable;

			// Do external hooks to other plugins....
			{
			// Initialize all....
			HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->SetOriginalFunctionPointers ();

			Globals::g_metaModEngine->GetEngineHookTable (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->m_hookedEngineFunctionsPointer);

			// Reliability check.
			InternalAssert (Globals::g_metaModEngine->GetLibrary ().IsValid ());

			void *const GiveFnptrsToDllFunctionPointer (Globals::g_metaModEngine->GetLibrary ().GetFunctionAddress <void *> ("GiveFnptrsToDll"));
			void *const assignAddress (MemoryUtilities::FindPatternWithoutWildCard (GiveFnptrsToDllFunctionPointer, GET_STATIC_STRING_LENGTH ("\x55\x8B\xEC\x83\x3D\xC4\xBC\x01\x10\x00\x8B\x45\x0C\x56\x57\x8B\x7D\x08\xA3\x78\xB5\x01\x10\xC7\x05"), (unsigned char *)"\xC7\x05", GET_STATIC_STRING_LENGTH ("\xC7\x05")));

			// Reliability check.
			InternalAssert (assignAddress != NULL);

			// Get 'engine_t' pointer from metamod.dll....
			Globals::g_metaModEngine->m_engine = &MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <SDK::Structures::engine_t> (assignAddress, GET_STATIC_STRING_LENGTH ("\xC7\x05"));

			// Reliability check.
			InternalAssert (Globals::g_metaModEngine->m_engine->pl_funcs->pfnMessageBegin == HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineFunctionTable ().pfnMessageBegin);

			// Copy original engine function table, before we hook it....
			Globals::g_metaModEngine->m_originalPluginEngineFunctions = *Globals::g_metaModEngine->m_engine->pl_funcs;

			const unsigned char string[] = {"dll: Unrecognized game: %s"};	// From meta_load_gamedll() function.
			unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (Globals::g_metaModEngine->GetLibrary (), string, GET_STATIC_STRING_LENGTH (string))));

			// Reliability check.
			InternalAssert (address != NULL);

			unsigned char *const tempAddress (static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (Globals::g_metaModEngine->GetLibrary (), address, Trampolines::Bytecode::Push32)));

			if (tempAddress == NULL)
			{
				address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (Globals::g_metaModEngine->GetLibrary (), address, (unsigned char *)"\xC7\x04\x24", GET_STATIC_STRING_LENGTH ("\xC7\x04\x24")));

				// Reliability check.
				InternalAssert (address != NULL);
			}
			else
				address = tempAddress;

			Globals::g_metaModEngine->m_gameDLL = &MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <SDK::Structures::gamedll_t> (address, -Trampolines::Bytecode::Pointer32Size/* Step back to previous argument - in assembler, or step forward to next argument - in C++ */);

			// Reliability checks.
			InternalAssert (Globals::g_metaModEngine->m_gameDLL->funcs.DLLFunctionAPITable != NULL);
			InternalAssert (Globals::g_metaModEngine->m_gameDLL->funcs.newDLLFunctionAPITable != NULL);
			InternalAssert (Globals::g_metaModEngine->m_gameDLL->funcs.DLLFunctionAPITable->pfnGameInit == HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnGameInit);

			// Copy original game.DLL function tables, before we hook them....
			Globals::g_metaModEngine->m_originalPluginDLLFunctionAPITable = *Globals::g_metaModEngine->m_gameDLL->funcs.DLLFunctionAPITable;
			Globals::g_metaModEngine->m_originalPluginNewDLLFunctionAPITable = *Globals::g_metaModEngine->m_gameDLL->funcs.newDLLFunctionAPITable;

			// Make callbacks from meta mod plugins.... (Redirect original functions to our functions for meta mod plugins)
			Globals::g_metaModEngine->RegisterPluginHooks ();

			{
			Globals::g_metaModEngine->m_api_tablesOriginal.engine = &Globals::g_metaModEngine->m_engine->funcs;
			Globals::g_metaModEngine->m_api_tablesOriginal.dllapi = &Globals::g_metaModEngine->m_gameDLL->funcs.DLLFunctionAPITable;
			Globals::g_metaModEngine->m_api_tablesOriginal.newapi = &Globals::g_metaModEngine->m_gameDLL->funcs.newDLLFunctionAPITable;

			void *const addrLoc (MemoryUtilities::FindPatternWithoutWildCard (Globals::g_metaModEngine->GetLibrary (), reinterpret_cast <const unsigned char *> (&Globals::g_metaModEngine->m_api_tablesOriginal), sizeof (Globals::g_metaModEngine->m_api_tablesOriginal)));

			// Reliability check.
			InternalAssert (addrLoc != NULL);

			Globals::g_metaModEngine->m_api_tables = reinterpret_cast <Engine::api_tables_t *> (addrLoc);

			InternalAssert (Globals::g_metaModEngine->m_api_tables->engine == &Globals::g_metaModEngine->m_engine->funcs);
			InternalAssert (Globals::g_metaModEngine->m_api_tables->dllapi == &Globals::g_metaModEngine->m_gameDLL->funcs.DLLFunctionAPITable);
			InternalAssert (Globals::g_metaModEngine->m_api_tables->newapi == &Globals::g_metaModEngine->m_gameDLL->funcs.newDLLFunctionAPITable);

			// Replace pointers to game DLL functions API which meta mod call as original functions....
			// This can be needed to provide non-meta mod original game DLL call bahaviour, but for now because we hook 'm_gameDLL->funcs' function tables, we need to restore them only for 'api_tables[e_api_dllapi]' and 'api_tables[e_api_newapi]' arrays in metaMod.DLL, which meta mod uses as original API provider.
			// See 'api_tables' pointer in "metaMod/api_hook.cpp" file.
			{
				// DLL API:
				{
					static HalfLifeEngine::SDK::Structures::DLLFunction_t s_MY_gameDLLFunctionTable (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ());
					static HalfLifeEngine::SDK::Structures::DLLFunction_t *sp_MY_gameDLLFunctionTable (&s_MY_gameDLLFunctionTable);

					// Reliability checks.
					InternalAssert ((*Globals::g_metaModEngine->m_api_tables->dllapi)->pfnClientConnect == HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIClientConnectFunctionCallbackFromPlugins);
					InternalAssert (Globals::g_metaModEngine->m_gameDLL->funcs.DLLFunctionAPITable->pfnClientConnect == HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIClientConnectFunctionCallbackFromPlugins);

//					InternalAssert (Trampolines::VirtualProtect (&Globals::g_metaModEngine->m_api_tables->dllapi, sizeof (Globals::g_metaModEngine->m_api_tables->dllapi), PAGE_EXECUTE_READWRITE));

					Globals::g_metaModEngine->m_api_tables->dllapi = &sp_MY_gameDLLFunctionTable;	// Provide original DLL API functions table to meta mod.

					// Reliability checks.
					InternalAssert ((*Globals::g_metaModEngine->m_api_tables->dllapi)->pfnClientConnect == HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientConnect);
					InternalAssert (Globals::g_metaModEngine->m_gameDLL->funcs.DLLFunctionAPITable->pfnClientConnect == HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIClientConnectFunctionCallbackFromPlugins);
				}
/*! NOT NEEDED. CUZ WE DO NOT HOOK ANY NEW DLL API FUNCTION. YET?
				// New DLL API:
				{
					static HalfLifeEngine::SDK::Structures::NewDLLFunction_t s_MY_newGameDLLFunctionTable (HalfLifeEngine::Globals::g_halfLifeEngine->GetNewGameDLLFunctionTable ());
					static HalfLifeEngine::SDK::Structures::NewDLLFunction_t *sp_MY_newGameDLLFunctionTable (&s_MY_newGameDLLFunctionTable);

					// Reliability check.
//					InternalAssert (Trampolines::VirtualProtect (&Globals::g_metaModEngine->m_api_tables->newapi, sizeof (Globals::g_metaModEngine->m_api_tables->newapi), PAGE_EXECUTE_READWRITE));

					Globals::g_metaModEngine->m_api_tables->newapi = &sp_MY_newGameDLLFunctionTable;	// Provide original NEW DLL API functions table to meta mod.
				}*/
			}
			}
			}

			// returning true enables metamod to attach this plugin
			return TRUE;
		}

		EXPORT BOOL Meta_Detach (SDK::Constants::PluginLoadTime_t now, SDK::Constants::PluginUnloadReason_t reason)
		{
			// this function is called when metamod unloads the plugin. A basic check is made in order
			// to prevent unloading the plugin if its processing should not be interrupted.

			// metamod detaching plugin from the server.
			//	'now'    - current phase, ie during map, etc... (given)
			//	'reason' - why detaching (refresh, console unload, forced unload, etc...) (given)

			// is metamod allowed to unload the plugin?
			if (now > Engine::sm_pluginInfo.unloadable && reason != SDK::Constants::PNL_CMD_FORCED)
			{
				AddLogEntry (true, LogLevel_Error, false, "Plugin not detaching (can't unload plugin right now).");

				// returning false prevents metamod from unloading this plugin
				return FALSE;
			}

			// Restore all hooked functions and free all used memory....
			InternalDetach ();

			// returning true enables metamod to unload this plugin
			return TRUE;
		}
	}
}