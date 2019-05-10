//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// Engine.cpp
//
// Class: Engine
//
// Description: Class for convenient work with Meta mod plugin.
//
// Version: $ID:$
//

#include <Core.hpp>

namespace MetaModEngine
{
	// Assign meta mod plugin information from Resource.hpp....
	/*static */const SDK::Structures::PluginInfo_t Engine::sm_pluginInfo
	(
		SDK::Constants::InterfaceVersion,	// meta mod interface version (version consists of "major:minor", two separate integer numbers)
		PRODUCT_NAME,						// plugin name
		PRODUCT_VERSION,					// plugin version
		PRODUCT_DATE,						// date of creation
		PRODUCT_AUTHOR,						// plugin author
		PRODUCT_URL,						// plugin URL
		PRODUCT_LOG_TAG,					// plugin log tag
		SDK::Constants::PT_ANYTIME,			// when loadable
		SDK::Constants::PT_ANYTIME			// when unloadable
	);

	void Engine::RegisterPluginEngineHooks (void)
	{
		// Make engine callbacks from meta mod plugins.... (Redirect original functions to our functions for meta mod plugins)
		m_engine->pl_funcs->pfnSetModel     = HalfLifeEngine::Engine_WithMetaMod::EngineAPISetModelFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnEmitSound    = HalfLifeEngine::Engine_WithMetaMod::EngineAPIEmitSoundFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnClientCommand = HalfLifeEngine::Engine_WithMetaMod::EngineAPIClientCommandFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnMessageBegin = HalfLifeEngine::Engine_WithMetaMod::EngineAPIMessageBeginFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnMessageEnd   = HalfLifeEngine::Engine_WithMetaMod::EngineAPIMessageEndFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteByte    = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteByteFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteChar    = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteCharFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteShort   = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteShortFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteLong    = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteLongFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteAngle   = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteAngleFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteCoord   = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteCoordFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteString  = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteStringFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnWriteEntity  = HalfLifeEngine::Engine_WithMetaMod::EngineAPIWriteEntityFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnClientPrintf = HalfLifeEngine::Engine_WithMetaMod::EngineAPIClientPrintfFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnCmd_Args     = HalfLifeEngine::Engine_WithMetaMod::EngineAPICmd_ArgsFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnCmd_Argv     = HalfLifeEngine::Engine_WithMetaMod::EngineAPICmd_ArgvFunctionCallbackFromPlugins;
		m_engine->pl_funcs->pfnCmd_Argc     = HalfLifeEngine::Engine_WithMetaMod::EngineAPICmd_ArgcFunctionCallbackFromPlugins;
	}
	void Engine::UnregisterPluginEngineHooks (void)
	{
		// Restore engine callbacks for meta mod plugins.... (Redirect our functions to original functions for meta mod plugins)
		m_engine->pl_funcs->pfnSetModel     = m_originalPluginEngineFunctions.pfnSetModel;
		m_engine->pl_funcs->pfnEmitSound    = m_originalPluginEngineFunctions.pfnEmitSound;
		m_engine->pl_funcs->pfnClientCommand = m_originalPluginEngineFunctions.pfnClientCommand;
		m_engine->pl_funcs->pfnMessageBegin = m_originalPluginEngineFunctions.pfnMessageBegin;
		m_engine->pl_funcs->pfnMessageEnd   = m_originalPluginEngineFunctions.pfnMessageEnd;
		m_engine->pl_funcs->pfnWriteByte    = m_originalPluginEngineFunctions.pfnWriteByte;
		m_engine->pl_funcs->pfnWriteChar    = m_originalPluginEngineFunctions.pfnWriteChar;
		m_engine->pl_funcs->pfnWriteShort   = m_originalPluginEngineFunctions.pfnWriteShort;
		m_engine->pl_funcs->pfnWriteLong    = m_originalPluginEngineFunctions.pfnWriteLong;
		m_engine->pl_funcs->pfnWriteAngle   = m_originalPluginEngineFunctions.pfnWriteAngle;
		m_engine->pl_funcs->pfnWriteCoord   = m_originalPluginEngineFunctions.pfnWriteCoord;
		m_engine->pl_funcs->pfnWriteString  = m_originalPluginEngineFunctions.pfnWriteString;
		m_engine->pl_funcs->pfnWriteEntity  = m_originalPluginEngineFunctions.pfnWriteEntity;
		m_engine->pl_funcs->pfnClientPrintf = m_originalPluginEngineFunctions.pfnClientPrintf;
		m_engine->pl_funcs->pfnCmd_Args     = m_originalPluginEngineFunctions.pfnCmd_Args;
		m_engine->pl_funcs->pfnCmd_Argv     = m_originalPluginEngineFunctions.pfnCmd_Argv;
		m_engine->pl_funcs->pfnCmd_Argc     = m_originalPluginEngineFunctions.pfnCmd_Argc;
	}

	void Engine::RegisterPluginDLLAPIHooks (void)
	{
		// Make DLL API callbacks from meta mod plugins.... (Redirect original functions to our functions for meta mod plugins)
		m_gameDLL->funcs.DLLFunctionAPITable->pfnTouch                 = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPITouchFunctionCallbackFromPlugins;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientConnect         = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIClientConnectFunctionCallbackFromPlugins;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientDisconnect      = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIClientDisconnectFunctionCallbackFromPlugins;
///		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientCommand         = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIClientCommandFunctionCallbackFromPlugins;	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientUserInfoChanged = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIClientUserInfoChangedFunctionCallbackFromPlugins;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnServerDeactivate      = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIServerDeactivateFunctionCallbackFromPlugins;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnStartFrame            = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIStartFrameFunctionCallbackFromPlugins;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnUpdateClientData      = HalfLifeEngine::Engine_WithMetaMod::GameDLLAPIUpdateClientDataFunctionCallbackFromPlugins;
	}
	void Engine::UnregisterPluginDLLAPIHooks (void)
	{
		// Restore DLL API callbacks for meta mod plugins.... (Redirect our functions to original functions for meta mod plugins)
		m_gameDLL->funcs.DLLFunctionAPITable->pfnTouch                 = m_originalPluginDLLFunctionAPITable.pfnTouch;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientConnect         = m_originalPluginDLLFunctionAPITable.pfnClientConnect;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientDisconnect      = m_originalPluginDLLFunctionAPITable.pfnClientDisconnect;
///		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientCommand         = m_originalPluginDLLFunctionAPITable.pfnClientCommand;	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
		m_gameDLL->funcs.DLLFunctionAPITable->pfnClientUserInfoChanged = m_originalPluginDLLFunctionAPITable.pfnClientUserInfoChanged;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnServerDeactivate      = m_originalPluginDLLFunctionAPITable.pfnServerDeactivate;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnStartFrame            = m_originalPluginDLLFunctionAPITable.pfnStartFrame;
		m_gameDLL->funcs.DLLFunctionAPITable->pfnUpdateClientData      = m_originalPluginDLLFunctionAPITable.pfnUpdateClientData;
	}
}