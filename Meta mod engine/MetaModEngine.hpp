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
// Engine.hpp
//
// Class: Engine
//
// Description: Class for convenient work with Meta mod plugin.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef META_MOD_ENGINE_INCLUDED
#define META_MOD_ENGINE_INCLUDED

class Engine
{
	friend BOOL                Export::Meta_Query                                            (const char *const interfaceVersion, const SDK::Structures::PluginInfo_t **const pluginInfo, const SDK::Structures::MetaModUtilityFunction_t *const utilityFunctions);
	friend BOOL                Export::Meta_Attach                                           (SDK::Constants::PluginLoadTime_t now, SDK::Structures::MetaModFunction_t *functionTable, SDK::Structures::MetaModGlobal_t *globals, SDK::Structures::GameDLLFunction_t *gameDLLFunctions);
	friend BOOL                Export::Meta_Detach                                           (SDK::Constants::PluginLoadTime_t now, SDK::Constants::PluginUnloadReason_t reason);

	friend void                Export::ReplacedFunctions::DLLTable::GameDLLInit              (void);
	friend int                 Export::ReplacedFunctions::DLLTable::Spawn                    (HalfLifeEngine::SDK::Classes::Edict *entity);
	friend void                Export::ReplacedFunctions::DLLTable::Touch                    (HalfLifeEngine::SDK::Classes::Edict *entityTouched, HalfLifeEngine::SDK::Classes::Edict *entityOther);
	friend BOOL                Export::ReplacedFunctions::DLLTable::ClientConnect            (HalfLifeEngine::SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]);
	friend void                Export::ReplacedFunctions::DLLTable::ClientDisconnect         (HalfLifeEngine::SDK::Classes::Edict *client);
	friend void                Export::ReplacedFunctions::DLLTable::ClientCommand            (HalfLifeEngine::SDK::Classes::Edict *client);
	friend void                Export::ReplacedFunctions::DLLTable::ClientUserInfoChanged    (HalfLifeEngine::SDK::Classes::Edict *client, char *infoBuffer);
	friend void                Export::ReplacedFunctions::DLLTable::ServerActivate           (HalfLifeEngine::SDK::Classes::Edict *pentEdictList, int edictCount, int clientMax);
	friend void                Export::ReplacedFunctions::DLLTable::ServerDeactivate         (void);
	friend void                Export::ReplacedFunctions::DLLTable::StartFrame               (void);
	friend void                Export::ReplacedFunctions::DLLTable::PM_Move                  (HalfLifeEngine::SDK::Structures::PlayerMove_t *playerMove, BOOL server);
	friend void                Export::ReplacedFunctions::DLLTable::PM_Init                  (HalfLifeEngine::SDK::Structures::PlayerMove_t *playerMove);
	friend void                Export::ReplacedFunctions::DLLTable::UpdateClientData         (const HalfLifeEngine::SDK::Classes::Edict *entity, int sendWeapons, HalfLifeEngine::SDK::Structures::ClientData_t *clientData);

	friend int                 Export::ReplacedFunctions::DLLTable::Post::Spawn              (HalfLifeEngine::SDK::Classes::Edict *entity);
	friend void                Export::ReplacedFunctions::DLLTable::Post::ServerActivate     (HalfLifeEngine::SDK::Classes::Edict *pentEdictList, int edictCount, int clientMax);
	friend void                Export::ReplacedFunctions::DLLTable::Post::CmdStart           (const HalfLifeEngine::SDK::Classes::Edict *const player, const HalfLifeEngine::SDK::Structures::UserCommand_t *const command, const unsigned int randomSeed);

	friend void                Export::ReplacedFunctions::EngineTable::pfnEmitSound          (HalfLifeEngine::SDK::Classes::Edict *entity, HalfLifeEngine::SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, HalfLifeEngine::SDK::Constants::SoundFlag_t flags, HalfLifeEngine::SDK::Constants::SoundPitch_t pitch);
	friend void                Export::ReplacedFunctions::EngineTable::pfnClientCommand      (HalfLifeEngine::SDK::Classes::Edict *client, char *format, ...);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnLightStyle   (const int style, char *const value);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnMessageBegin (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const unsigned int type, const Math::Vector3D *const origin, HalfLifeEngine::SDK::Classes::Edict *const to);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnMessageEnd   (void);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteByte    (int value);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteChar    (int value);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteShort   (int value);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteLong    (int value);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteAngle   (float value);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteCoord   (float value);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteString  (const char *const string);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnWriteEntity  (int value);
	friend int                 Export::ReplacedFunctions::EngineTable::pfnRegUserMsg         (const char *name, int size);
	friend void                Export::ReplacedFunctions::EngineTable::pfnClientPrintf       (HalfLifeEngine::SDK::Classes::Edict *client, HalfLifeEngine::SDK::Constants::PrintType_t printType, const char *message);
	friend const char         *Export::ReplacedFunctions::EngineTable::pfnCmd_Args           (void);
	friend const char         *Export::ReplacedFunctions::EngineTable::pfnCmd_Argv           (unsigned int argc);
	friend const unsigned int  Export::ReplacedFunctions::EngineTable::pfnCmd_Argc           (void);
	friend void                Export::ReplacedFunctions::EngineTable::Post::pfnSetView      (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const viewEntity);

	//
	// Group: Private members.
	//
	private:
		static const SDK::Structures::PluginInfo_t         sm_pluginInfo;

		// global variables from meta mod (pointers will be NULL if the game.DLL doesn't support this API)

		// holds the meta mod engine functionality callbacks
		const SDK::Structures::MetaModUtilityFunction_t   *m_utilityFunctions;	// meta mod utility functions

	public:
		SDK::Structures::MetaModGlobal_t                  *m_globalVariables;		// meta mod globals

		LibraryHandleWithSize                              m_library;
		SDK::Structures::engine_t                         *m_engine;
		SDK::Structures::gamedll_t                        *m_gameDLL;

		HalfLifeEngine::SDK::Structures::EngineFunction_t  m_originalPluginEngineFunctions;			// Copy of 'm_engine->pl_funcs' table.
		HalfLifeEngine::SDK::Structures::DLLFunction_t     m_originalPluginDLLFunctionAPITable;		// Copy of 'm_gameDLL->funcs.DLLFunctionAPITable' table.
		HalfLifeEngine::SDK::Structures::NewDLLFunction_t  m_originalPluginNewDLLFunctionAPITable;	// Copy of 'm_gameDLL->funcs.newDLLFunctionAPITable' table.

		// api table list
		struct api_tables_t
		{
			HalfLifeEngine::SDK::Structures::EngineFunction_t **engine;
			HalfLifeEngine::SDK::Structures::DLLFunction_t    **dllapi;
			HalfLifeEngine::SDK::Structures::NewDLLFunction_t **newapi;
		};
		api_tables_t                                      *m_api_tables;
		api_tables_t                                       m_api_tablesOriginal;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Engine (void) :
			m_utilityFunctions (NULL),
			m_globalVariables (NULL),

			m_library (),
			m_engine (NULL),
			m_gameDLL (NULL),

			m_api_tables (NULL)
		{ /* VOID */ }
/*
		inline ~Engine (void)
		{
//			UnregisterAllForwardsAndHooks ();
		}
*/
	//
	// Group: Private operators.
	//
	private:
		inline Engine &operator = (const Engine &/*right*/);	// Avoid "warning C4512: 'Engine' : assignment operator could not be generated".

	//
	// Group: Private functions.
	//
	private:
		void RegisterPluginEngineHooks (void);
		void RegisterPluginDLLAPIHooks (void);
		inline void RegisterPluginHooks (void)
		{
			RegisterPluginEngineHooks ();
			RegisterPluginDLLAPIHooks ();
		}

		void UnregisterPluginEngineHooks (void);
		void UnregisterPluginDLLAPIHooks (void);
		inline void UnregisterPluginHooks (void)
		{
			UnregisterPluginEngineHooks ();
			UnregisterPluginDLLAPIHooks ();
		}
		inline void UnregisterExportRedirections (void)
		{
			if (!GetLibrary ().exportchange ("Server_GetBlendingInterface", HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <DWORD> ("Server_GetBlendingInterface")))
				AddLogEntry (true, LogLevel_Error, false, "Couldn't restore Server_GetBlendingInterface() replacement for meta mod DLL!");
		}

	//
	// Group: Functions.
	//
	public:
		template <typename elementType> inline elementType &GetOriginalResult (void) const { return static_cast <elementType *> (m_globalVariables->originalReturnValue)[0u]; }
		template <typename elementType> inline elementType &GetOverrideResult (void) const { return static_cast <elementType *> (m_globalVariables->overrideReturnValue)[0u]; }

		inline       LibraryHandleWithSize &GetLibrary (void)       { return m_library; }
		inline const LibraryHandleWithSize &GetLibrary (void) const { return m_library; }

		inline void RegisterExportRedirections (void)
		{
			/// @note Don't get original meta mod Server_GetBlendingInterface() function pointer cause this function do nothing in MetaMod without P extention, or in MetaMod-P version just redirected to original mp.DLL function which in meta mod combined export table....
			if (!GetLibrary ().exportchange ("Server_GetBlendingInterface", reinterpret_cast <DWORD> (&HalfLifeEngine::Export::Server_GetBlendingInterface)))
				AddLogEntry (false, LogLevel_Critical, true, "Couldn't load Server_GetBlendingInterface() replacement for meta mod DLL!");
		}

		inline void RegisterAllForwardsAndHooks (void)
		{
			RegisterPluginHooks ();
			RegisterExportRedirections ();
		}
		inline void UnregisterAllForwardsAndHooks (void)
		{
			m_api_tables->dllapi = m_api_tablesOriginal.dllapi;	/// @todo PLACE INTO SOME FUNCTION....

			UnregisterPluginHooks ();
			UnregisterExportRedirections ();
		}

	//
	// Group: Utilities functions.
	//
	public:
		inline void               LogConsole                      (const char *const message) const
		{
			// Reliability checks.
			InternalAssert (!IsNullOrEmptyString (message));
			InternalAssert (GetStringLength <unsigned short> (message) <= MaximumLogMessageLength);

			(*m_utilityFunctions->pfnLogConsole) (&sm_pluginInfo, message);
		}
		inline void               LogConsoleFormat                (const char *const format, ...) const
		{
			char message[MaximumLogMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			LogConsole (message);
		}
		inline void               LogMessage                      (const char *const message) const
		{
			// Reliability checks.
			InternalAssert (!IsNullOrEmptyString (message));
			InternalAssert (GetStringLength <unsigned short> (message) <= MaximumLogMessageLength);

			(*m_utilityFunctions->pfnLogMessage) (&sm_pluginInfo, message);
		}
		inline void               LogMessageFormat                (const char *const format, ...) const
		{
			char message[MaximumLogMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			LogMessage (message);
		}
		inline void               LogError                        (const char *const message) const
		{
			// Reliability checks.
			InternalAssert (!IsNullOrEmptyString (message));
			InternalAssert (GetStringLength <unsigned short> (message) <= MaximumLogMessageLength);

			(*m_utilityFunctions->pfnLogError) (&sm_pluginInfo, message);
		}
		inline void               LogErrorFormat                  (const char *const format, ...) const
		{
			char message[MaximumLogMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			LogError (message);
		}
		inline void               LogDeveloper                    (const char *const message) const
		{
			// Reliability checks.
			InternalAssert (!IsNullOrEmptyString (message));
			InternalAssert (GetStringLength <unsigned short> (message) <= MaximumLogMessageLength);

			(*m_utilityFunctions->pfnLogDeveloper) (&sm_pluginInfo, message);
		}
		inline void               LogDeveloperFormat              (const char *const format, ...) const
		{
			char message[MaximumLogMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			LogDeveloper (message);
		}
		inline void               CenterSay                       (const char *const message) const
		{
			// Reliability checks.
			InternalAssert (!IsNullOrEmptyString (message));
			InternalAssert (GetStringLength <unsigned short> (message) <= MaximumLogMessageLength);

			(*m_utilityFunctions->pfnCenterSay) (&sm_pluginInfo, message);
		}
		inline void               CenterSayFormat                 (const char *const format, ...) const
		{
			char message[MaximumLogMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			CenterSay (message);
		}
		inline void               CenterSayParms                  (const SDK::Structures::HUDTextParameter_t HUDTextParameters, const char *const message) const
		{
			// Reliability checks.
			InternalAssert (!IsNullOrEmptyString (message));
			InternalAssert (GetStringLength <unsigned short> (message) <= MaximumLogMessageLength);

			(*m_utilityFunctions->pfnCenterSayParms) (&sm_pluginInfo, HUDTextParameters, message);
		}
		inline void               CenterSayParmsFormat            (const SDK::Structures::HUDTextParameter_t HUDTextParameters, const char *const format, ...) const
		{
			char message[MaximumLogMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			CenterSayParms (HUDTextParameters, message);
		}
		inline void                 CenterSayVarArgs                (const SDK::Structures::HUDTextParameter_t HUDTextParameters, const char *const format, const va_list argumentPointer)                                                                                                      const { (*m_utilityFunctions->pfnCenterSayVarargs) (&sm_pluginInfo, HUDTextParameters, format, argumentPointer); }
		inline const bool           CallGameEntity                  (const char *const className, HalfLifeEngine::SDK::Structures::EntityVariables_t &entityVariables)                                                                                                                          const { return (*m_utilityFunctions->pfnCallGameEntity) (&sm_pluginInfo, className, &entityVariables) == TRUE; }
		inline const unsigned short GetUserMsgID                    (const DynamicString &name, int *const size)                                                                                                                                                                                const { return static_cast <unsigned short> ((*m_utilityFunctions->pfnGetUserMsgID) (&sm_pluginInfo, name, size)); }
		inline const DynamicString  GetUserMessageName              (const unsigned short id, int *const size)                                                                                                                                                                                  const { return (*m_utilityFunctions->pfnGetUserMsgName) (&sm_pluginInfo, id, size); }
		inline const DynamicString  GetPluginPath                   (void)                                                                                                                                                                                                                      const { return (*m_utilityFunctions->pfnGetPluginPath) (&sm_pluginInfo); }
		inline const DynamicString  GetGameInfo                     (const SDK::Constants::GameInfo_t tag)                                                                                                                                                                                      const { return (*m_utilityFunctions->pfnGetGameInfo) (&sm_pluginInfo, tag); }
		inline const int            LoadPlugin                      (const DynamicString &commandLine, const SDK::Constants::PluginLoadTime_t now, HMODULE *const pluginHandle)                                                                                                                 const { return (*m_utilityFunctions->pfnLoadPlugin) (&sm_pluginInfo, commandLine, now, pluginHandle); }
		inline const int            UnloadPlugin                    (const DynamicString &commandLine, const SDK::Constants::PluginLoadTime_t now, const SDK::Constants::PluginUnloadReason_t reason)                                                                                           const { return (*m_utilityFunctions->pfnUnloadPlugin) (&sm_pluginInfo, commandLine, now, reason); }
		inline const int            UnloadPluginByHandle            (HMODULE pluginHandle, const SDK::Constants::PluginLoadTime_t now, const SDK::Constants::PluginUnloadReason_t reason)                                                                                                       const { return (*m_utilityFunctions->pfnUnloadPluginByHandle) (&sm_pluginInfo, pluginHandle, now, reason); }
		inline const DynamicString  IsQueryingClientConsoleVariable (const HalfLifeEngine::SDK::Classes::Edict *const player)                                                                                                                                                                   const { return (*m_utilityFunctions->pfnIsQueryingClientCvar) (&sm_pluginInfo, player); }
		inline const int            MakeRequestID                   (void)                                                                                                                                                                                                                      const { return (*m_utilityFunctions->pfnMakeRequestID) (&sm_pluginInfo); }
		inline void                 GetHookTables                   (HalfLifeEngine::SDK::Structures::EngineFunction_t **const engineFunctions, HalfLifeEngine::SDK::Structures::DLLFunction_t **const DLLFunctions, HalfLifeEngine::SDK::Structures::NewDLLFunction_t **const newDLLFunctions) const { (*m_utilityFunctions->pfnGetHookTables) (&sm_pluginInfo, engineFunctions, DLLFunctions, newDLLFunctions); }
		inline void                 GetEngineHookTable              (HalfLifeEngine::SDK::Structures::EngineFunction_t *&engineFunctions)                                                                                                                                                       const { GetHookTables (&engineFunctions, NULL, NULL); }
		inline void                 GetDLLAPIHookTable              (HalfLifeEngine::SDK::Structures::DLLFunction_t    *&DLLFunctions)                                                                                                                                                          const { GetHookTables (NULL, &DLLFunctions, NULL); }
		inline void                 GetNewDLLAPIHookTable           (HalfLifeEngine::SDK::Structures::NewDLLFunction_t *&newDLLFunctions)                                                                                                                                                       const { GetHookTables (NULL, NULL, &newDLLFunctions); }
};

#endif	// ifndef META_MOD_ENGINE_INCLUDED