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
// Core.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef META_MOD_ENGINE_CORE_INCLUDED
#define META_MOD_ENGINE_CORE_INCLUDED

namespace MetaModEngine
{
	namespace SDK
	{
		#include <Meta mod engine/SDK/Definitions.hpp>
	}

	namespace Export
	{
		#include <Meta mod engine/Export/Export_definitions.hpp>
	}

	#include <Meta mod engine/MetaModEngine.hpp>

	namespace Globals
	{
		#include <Meta mod engine/Globals/Meta_mod_engine_globals.hpp>
	}

	namespace Export
	{
		namespace ReplacedFunctions
		{
			#include <Meta mod engine/Export/Replaced functions/Replaced_functions_definitions.hpp>
		}
	}
}

namespace HalfLifeEngine
{
	inline void Engine::CallGameEntity (const char *const entityClassName, SDK::Structures::EntityVariables_t &entityVariables) const
	{
		// Call the entity function....
		MetaModEngine::Globals::g_metaModEngine->CallGameEntity (entityClassName, entityVariables);
	}

	// Engine API table....
	inline void Engine_WithMetaMod::HookEngineAPISetModelFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnSetModel = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnSetModel;
		m_EngineAPISetModelFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnSetModel;
	}
	inline void Engine_WithMetaMod::HookEngineAPIEmitSoundFunction (void)
	{
		m_engineFunctionTablePointer->pfnEmitSound = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnEmitSound;
		m_EngineAPIEmitSoundFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnEmitSound;
	}
	inline void Engine_WithMetaMod::HookEngineAPIClientCommandFunction (void)
	{
		m_engineFunctionTablePointer->pfnClientCommand = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnClientCommand;
		m_EngineAPIClientCommandFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnClientCommand;
	}
	inline void Engine_WithMetaMod::HookEngineAPILightStyleFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnLightStyle = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnLightStyle;
		m_EngineAPILightStyleFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnLightStyle;
	}

	inline void Engine_WithMetaMod::HookEngineAPIMessageBeginFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnMessageBegin = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnMessageBegin;
		m_EngineAPIMessageBeginFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnMessageBegin;
	}
	inline void Engine_WithMetaMod::HookEngineAPIMessageEndFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnMessageEnd = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnMessageEnd;
		m_EngineAPIMessageEndFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnMessageEnd;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteByteFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteByte = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteByte;
		m_EngineAPIWriteByteFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteByte;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteCharFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteChar = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteChar;
		m_EngineAPIWriteCharFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteChar;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteShortFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteShort = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteShort;
		m_EngineAPIWriteShortFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteShort;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteLongFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteLong = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteLong;
		m_EngineAPIWriteLongFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteLong;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteAngleFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteAngle = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteAngle;
		m_EngineAPIWriteAngleFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteAngle;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteCoordFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteCoord = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteCoord;
		m_EngineAPIWriteCoordFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteCoord;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteStringFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteString = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteString;
		m_EngineAPIWriteStringFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteString;
	}
	inline void Engine_WithMetaMod::HookEngineAPIWriteEntityFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnWriteEntity = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnWriteEntity;
		m_EngineAPIWriteEntityFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnWriteEntity;
	}

	inline void Engine_WithMetaMod::HookEngineAPIClientPrintfFunction (void)
	{
		m_engineFunctionTablePointer->pfnClientPrintf = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnClientPrintf;
		m_EngineAPIClientPrintfFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnClientPrintf;
	}
	inline void Engine_WithMetaMod::HookEngineAPICmd_ArgsFunction (void)
	{
		m_engineFunctionTablePointer->pfnCmd_Args = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnCmd_Args;
		m_EngineAPICmd_ArgsFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnCmd_Args;
	}
	inline void Engine_WithMetaMod::HookEngineAPICmd_ArgvFunction (void)
	{
		m_engineFunctionTablePointer->pfnCmd_Argv = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnCmd_Argv;
		m_EngineAPICmd_ArgvFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnCmd_Argv;
	}
	inline void Engine_WithMetaMod::HookEngineAPICmd_ArgcFunction (void)
	{
		m_engineFunctionTablePointer->pfnCmd_Argc = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnCmd_Argc;
		m_EngineAPICmd_ArgcFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnCmd_Argc;
	}
	inline void Engine_WithMetaMod::HookEngineAPISetViewFunction (void)
	{
		m_enginePOSTFunctionTablePointer->pfnSetView = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnSetView;
		m_EngineAPISetViewFunctionPointer = Export::ReplacedFunctions::EngineTable::pfnSetView;
	}

	// Game DLL API table....
	inline void Engine_WithMetaMod::HookGameDLLAPITouchFunction (void)
	{
		m_gameDLLFunctionAPITablePointer->pfnTouch = MetaModEngine::Export::ReplacedFunctions::DLLTable::Touch;
		m_GameDLLAPITouchFunctionPointer = Export::ReplacedFunctions::DLLTable::Touch;
	}
	inline void Engine_WithMetaMod::HookGameDLLAPIClientConnectFunction (void)
	{
		m_gameDLLFunctionAPITablePointer->pfnClientConnect = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientConnect;
		m_GameDLLAPIClientConnectFunctionPointer = Export::ReplacedFunctions::DLLTable::ClientConnect;
	}
	inline void Engine_WithMetaMod::HookGameDLLAPIClientDisconnectFunction (void)
	{
		m_gameDLLFunctionAPITablePointer->pfnClientDisconnect = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientDisconnect;
		m_GameDLLAPIClientDisconnectFunctionPointer = Export::ReplacedFunctions::DLLTable::ClientDisconnect;
	}
/*!	inline void Engine_WithMetaMod::HookGameDLLAPIClientCommandFunction (void)	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
	{
		m_gameDLLFunctionAPITablePointer->pfnClientCommand = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientCommand;
		m_GameDLLAPIClientCommandFunctionPointer = Export::ReplacedFunctions::DLLTable::ClientCommand;
	}*/
	inline void Engine_WithMetaMod::HookGameDLLAPIClientUserInfoChangedFunction (void)
	{
		m_gameDLLFunctionAPITablePointer->pfnClientUserInfoChanged = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientUserInfoChanged;
		m_GameDLLAPIClientUserInfoChangedFunctionPointer = Export::ReplacedFunctions::DLLTable::ClientUserInfoChanged;
	}
	inline void Engine_WithMetaMod::HookGameDLLAPIServerDeactivateFunction (void)
	{
		m_gameDLLFunctionAPITablePointer->pfnServerDeactivate = MetaModEngine::Export::ReplacedFunctions::DLLTable::ServerDeactivate;
		m_GameDLLAPIServerDeactivateFunctionPointer = Export::ReplacedFunctions::DLLTable::ServerDeactivate;
	}
	inline void Engine_WithMetaMod::HookGameDLLAPIStartFrameFunction (void)
	{
		m_gameDLLFunctionAPITablePointer->pfnStartFrame = MetaModEngine::Export::ReplacedFunctions::DLLTable::StartFrame;
		m_GameDLLAPIStartFrameFunctionPointer = Export::ReplacedFunctions::DLLTable::StartFrame;
	}
	inline void Engine_WithMetaMod::HookGameDLLAPIUpdateClientDataFunction (void)
	{
		m_gameDLLFunctionAPITablePointer->pfnUpdateClientData = MetaModEngine::Export::ReplacedFunctions::DLLTable::UpdateClientData;
		m_GameDLLAPIUpdateClientDataFunctionPointer = Export::ReplacedFunctions::DLLTable::UpdateClientData;
	}
}

#endif	// ifndef META_MOD_ENGINE_CORE_INCLUDED