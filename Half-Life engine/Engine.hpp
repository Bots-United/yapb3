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
// Description: Class for convenient work with Half-Life engine.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_INCLUDED
#define HALF_LIFE_ENGINE_INCLUDED

class Engine_WithMetaMod;
class Engine_WithoutMetaMod;

class Engine
{
	friend BOOL                Export::GetEntityAPI2                                      (SDK::Structures::DLLFunction_t *DLLFunctionTable, int *interfaceVersion);
	friend BOOL                Export::Server_GetBlendingInterface                        (int version, SDK::Structures::ServerBlendingInterface_t **blendInterface, SDK::Structures::ServerStudioAPI_t *studio, SDK::TypeDefinitions::RotationMatrix_t *const rotationMatrix, SDK::TypeDefinitions::BoneTransformMatrix_t *const boneTransform);

	friend void                Export::ReplacedFunctions::DLLTable::GameDLLInit           (void);
	friend int                 Export::ReplacedFunctions::DLLTable::Spawn                 (SDK::Classes::Edict *entity);
	friend void                Export::ReplacedFunctions::DLLTable::Touch                 (SDK::Classes::Edict *entityTouched, SDK::Classes::Edict *entityOther);
	friend BOOL                Export::ReplacedFunctions::DLLTable::ClientConnect         (SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]);
	friend void                Export::ReplacedFunctions::DLLTable::ClientDisconnect      (SDK::Classes::Edict *client);
	friend void                Export::ReplacedFunctions::DLLTable::ClientCommand         (SDK::Classes::Edict *client);
	friend void                Export::ReplacedFunctions::DLLTable::ClientUserInfoChanged (SDK::Classes::Edict *client, char *infoBuffer);
	friend void                Export::ReplacedFunctions::DLLTable::ServerActivate        (SDK::Classes::Edict *pentEdictList, int edictCount, int clientMax);
	friend void                Export::ReplacedFunctions::DLLTable::ServerDeactivate      (void);
	friend void                Export::ReplacedFunctions::DLLTable::StartFrame            (void);
	friend void                Export::ReplacedFunctions::DLLTable::PM_Move               (SDK::Structures::PlayerMove_t *playerMove, BOOL server);
	friend void                Export::Callbacks::DLLTable::PM_Move                       (SDK::Structures::PlayerMove_t *const playerMove, const bool server);
	friend void                Export::ReplacedFunctions::DLLTable::PM_Init               (SDK::Structures::PlayerMove_t *playerMove);
	friend void                Export::ReplacedFunctions::DLLTable::UpdateClientData      (const SDK::Classes::Edict *entity, int sendWeapons, SDK::Structures::ClientData_t *clientData);
	friend void                Export::ReplacedFunctions::DLLTable::CmdStart              (const SDK::Classes::Edict *const player, const SDK::Structures::UserCommand_t *const command, const unsigned int randomSeed);

	friend void                Export::ReplacedFunctions::EngineTable::pfnSetModel        (SDK::Classes::Edict *entity, const char *modelName);
	friend void                Export::ReplacedFunctions::EngineTable::pfnChangeLevel     (const char *const s1, const char *const s2);
	friend void                Export::ReplacedFunctions::EngineTable::pfnEmitSound       (SDK::Classes::Edict *entity, SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, SDK::Constants::SoundFlag_t flags, SDK::Constants::SoundPitch_t pitch);
	friend void                Export::ReplacedFunctions::EngineTable::pfnClientCommand   (SDK::Classes::Edict *client, char *format, ...);
	friend void                Export::ReplacedFunctions::EngineTable::pfnLightStyle      (const int style, char *const value);
	friend void                Export::ReplacedFunctions::EngineTable::pfnMessageBegin    (const SDK::Constants::MessageDestination_t destination, const unsigned int type, const Math::Vector3D *const origin, SDK::Classes::Edict *const to);
	friend void                Export::ReplacedFunctions::EngineTable::pfnMessageEnd      (void);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteByte       (int value);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteChar       (int value);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteShort      (int value);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteLong       (int value);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteAngle      (float value);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteCoord      (float value);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteString     (const char *const string);
	friend void                Export::ReplacedFunctions::EngineTable::pfnWriteEntity     (int value);
	friend int                 Export::ReplacedFunctions::EngineTable::pfnRegUserMsg      (const char *name, int size);
	friend void                Export::ReplacedFunctions::EngineTable::pfnClientPrintf    (SDK::Classes::Edict *client, SDK::Constants::PrintType_t printType, const char *message);
	friend const char         *Export::ReplacedFunctions::EngineTable::pfnCmd_Args        (void);
	friend const char         *Export::ReplacedFunctions::EngineTable::pfnCmd_Argv        (unsigned int argc);
	friend const unsigned int  Export::ReplacedFunctions::EngineTable::pfnCmd_Argc        (void);
	friend void                Export::ReplacedFunctions::EngineTable::pfnSetView         (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity);

	friend void                Math::Vector3D::MakeRightVector                            (void) const;
	friend void                Math::Vector3D::MakeUpwardVector                           (void) const;
	friend void                Math::Vector3D::MakeVectors                                (void) const;

	friend void                Math::Angles3D::MakeVectors                                (void) const;
	friend void                Math::Angles3D::MakeTransposedVectors                      (void) const;

	friend void                Math::Angles2D::MakeForwardVector                          (void) const;

	friend void                Math::Angles3D::MakeForwardVector                          (void) const;
	friend void                Math::Angles3D::MakeRightVector                            (void) const;
	friend void                Math::Angles3D::MakeUpwardVector                           (void) const;

	//
	// Group: Constants.
	//
	public:
		enum RenderMode_t
		{
			RenderMode_Software,

			RenderMode_Hardware,
				RenderMode_OpenGL = RenderMode_Hardware,
				RenderMode_Direct3D,
		};

		// supported game mod's
		enum GameModType_t
		{
			Version_Unknown = -1,	// mod type terminator

			Version_Steam,			// Counter-Strike 1.6 and Above
			Version_ConditionZero,	// Counter-Strike: Condition Zero

			Version_Total			// total of supported by this bot game mods
		};
		class GameMod	// Class for conversions between CS 1.6 and CS: CZ.
		{
			private:
				

			public:
				
		};	/// @todo !!!
		class GameMod_Original : public GameMod
		{
			private:
				

			public:
				
		};	/// @todo !!!
		class GameMod_ConditionZero : public GameMod
		{
			private:
				

			public:
				
		};	/// @todo !!!

		enum PrecachedStringID_t
		{
			// Sounds:
			PrecachedStringID_weapons_slash_xbow_hit1_dot_wav,		// "weapons/xbow_hit1.wav"
			PrecachedStringID_weapons_slash_mine_activate_dot_wav,	// "weapons/mine_activate.wav"
			PrecachedStringID_debris_slash_bustglass1_dot_wav,		// "debris/bustglass1.wav"
			PrecachedStringID_items_slash_medshot4_dot_wav,			// "items/medshot4.wav"

			// Models:
//			PrecachedStringID_models_slash_mechgibs_dot_mdl,		// "models/mechgibs.mdl"

			PrecachedStringID_sprites_slash_laserbeam_dot_spr,		// "sprites/laserbeam.spr"
			PrecachedStringID_sprites_slash_arrow1_dot_spr,			// "sprites/arrow1.spr"

			PrecachedStringID_Total
		};
		enum PrecachedSpriteIndex_t
		{
			PrecachedSpriteIndex_Laser,	// used for waypoint images
			PrecachedSpriteIndex_Arrow,	// used for way up to the current purpose image

			PrecachedSpriteIndex_Total
		};

		#include <Half-Life engine/GameMessageContainer.hpp>

	//
	// Group: Private members.
	//
	private:
		const RenderMode_t                                 m_renderingMode;			// Current rendering/drawing mode.

	//
	// Group: Protected members.
	//
	protected:
		static const char                                 *sm_precachedStrings[PrecachedStringID_Total];

		LibraryHandleWithSize                              m_engineModule;			// Must be first of all members!!
		LibraryWithSize                                   *m_gameDLL;

		GameModType_t                                      m_gameModID;				// mod ID
		DynamicString                                      m_gameModName;			// mod name (the game directory)

		GameMessageContainer                               m_gameMessageContainer;

		struct BuildInformation_t
		{
			DynamicString time;
			DynamicString date;
			unsigned int  number;
			unsigned char protocol;
		} m_buildInformation;

/*		// holds engine functionality callbacks
		SDK::Structures::DLLFunction_t    *m_gameDLLFunctionAPITablePointer;	// game.DLL function tables
		SDK::Structures::DLLFunction_t     m_gameDLLFunctionAPITable;			// game.DLL function tables
		SDK::Structures::NewDLLFunction_t *m_newGameDLLFunctionAPITablePointer;	// game.DLL new function tables
		SDK::Structures::NewDLLFunction_t  m_newGameDLLFunctionAPITable;		// game.DLL new function tables
*/		SDK::Structures::EngineFunction_t           *m_engineFunctionTablePointer;	// must be provided by user of this code
		SDK::Structures::EngineFunction_t            m_engineFunctionTable;	// must be provided by user of this code

		/// @todo : MOVE THIS TO 'Engine_Listen' CLASS!!!
		/// @note : Only listen server stuff!!!
		///@{
		SDK::Structures::ClientEngineFunction_t *const m_clientEngineFunctionTablePointer;
		SDK::Structures::ClientEngineFunction_t m_clientEngineFunctionTable;
		SDK::Structures::ClientDLLExportTable_t *const m_clientDLLExportTablePointer;
		SDK::Structures::ClientDLLExportTable_t m_clientDLLExportTable;
		SDK::Structures::StudioModelRenderer_t *const m_clientStudioModelRendererPointer;
		SDK::Structures::StudioModelRenderer_t m_clientStudioModelRenderer;
		SDK::Structures::EngineStudioAPI_t *const m_clientEngineStudioAPIPointer;
		SDK::Structures::EngineStudioAPI_t m_clientEngineStudioAPI;
		///@}

		SDK::Structures::PlayerMove_t *m_playerMovePointer;
		SDK::Structures::PlayerMove_t  m_playerMove;

		SDK::Structures::ServerStudioAPI_t                 m_engineStudioFunctionTable;
		SDK::Structures::ServerBlendingInterface_t m_blendInterface;
		// Matrices
		// Model to world transformation
//		const SDK::TypeDefinitions::RotationMatrix_t                 *m_rotationMatrix;	// rotation matrix
		// Concatenated bone transform
		const SDK::TypeDefinitions::BoneTransformMatrix_t            *m_boneTransformMatrix;	// bone transformation matrix

		SDK::Structures::GlobalVariables_t *const          m_globalVariables;		// half-life engine globals

		SDK::Classes::Interface::FileSystem *const m_fileSystem;
		const SDK::Classes::Interface::CreateFunction_t m_engineFactory;
		SDK::Classes::Interface::Engine *const m_engine;
		SDK::Classes::Interface::CDAudio *const m_cdaudio;
		SDK::VGUI::ISurface *const m_surface;

		const double                                      &m_realTime;				// not bounded in any way, changed at start of every frame, never reset
		const SDK::TypeDefinitions::SV_InactivateClientsFunction_t               m_SV_InactivateClientsFunction;	// Used for hooking level change or server restart.
		const SDK::TypeDefinitions::SV_CalcPingFunction_t                        m_SV_CalcPing;
		const SDK::TypeDefinitions::GL_DisableMultitextureFunction_t             m_GL_DisableMultitextureFunction;

		short                                              m_precachedSpritesIndexes[PrecachedSpriteIndex_Total];	// definitions of indexes precached sprites

	//
	// Group: Members.
	//
	public:
		// holds engine functionality callbacks
		SDK::Structures::DLLFunction_t    *m_gameDLLFunctionAPITablePointer;	// game.DLL function tables
		SDK::Structures::DLLFunction_t     m_gameDLLFunctionAPITable;			// game.DLL function tables
		SDK::Structures::NewDLLFunction_t *m_newGameDLLFunctionAPITablePointer;	// game.DLL new function tables
		SDK::Structures::NewDLLFunction_t  m_newGameDLLFunctionAPITable;		// game.DLL new function tables

	//
	// Group: (Con/De)structors.
	//
	public:
		Engine (SDK::Structures::EngineFunction_t *const engineFunctionTable, SDK::Structures::GlobalVariables_t *const globalVariables);
		virtual inline ~Engine (void)
		{
			// if dynamic link library of game mod is load...
			delete m_gameDLL;	// free the game.DLL space
		}

		const bool GetEntityAPI2 (SDK::Structures::DLLFunction_t *const DLLFunctionTable);

	//
	// Group: Private operators.
	//
	private:
		inline Engine &operator = (const Engine &/*right*/);	// Avoid "warning C4512: 'Engine' : assignment operator could not be generated".

	//
	// Group: Private functions.
	//
	private:
		void       GetVersion        (void);
		void       ComputeBuild      (void);
#if 0
// Info about the game dll/mod.
struct gamedll_t
{
	DynamicString name;			// ie "cstrike" (from gamedir)
	const char *desc;				// ie "Counter-Strike"
	DynamicString gamedir;			// ie "/home/willday/half-life/cstrike"
	DynamicString pathname;		// ie "/home/willday/half-life/cstrike/dlls/mp.DLL"
	const char *file;				// ie "mp.DLL"
	DynamicString real_pathname;	// in case pathname overridden by bot, etc
	LibraryWithSize *handle;
//	MetaModEngine::SDK::Structures::GameDLLFunction_t funcs;	// dllapi_table, newapi_table
	inline void Clear (void)
	{
		name.MakeEmpty ();
		desc = NULL;
		gamedir.MakeEmpty ();
		pathname.MakeEmpty ();
		file = NULL;
		real_pathname.MakeEmpty ();
		handle = NULL;
//		funcs.DLLFunctionAPITable = NULL;
//		funcs.newDLLFunctionAPITable = NULL;
	}
};
gamedll_t GameDLL;
		const bool meta_init_gamedll (void);
		const bool setup_gamedll (void);
#endif	// if 0
	//
	// Group: Functions.
	//
	public:
		inline const RenderMode_t                                 GetRenderingMode               (void)                            const { return m_renderingMode; }

		inline const bool                                         IsSoftwareRenderingMode        (void)                            const { return GetRenderingMode () == RenderMode_Software; }
		inline const bool                                         IsHardwareRenderingMode        (void)                            const { return GetRenderingMode () >= RenderMode_Hardware; }
		inline const bool                                         IsOpenGLRenderingMode          (void)                            const { return GetRenderingMode () == RenderMode_OpenGL; }
		inline const bool                                         IsDirect3DRenderingMode        (void)                            const { return GetRenderingMode () == RenderMode_Direct3D; }

		static inline const char *const                           GetPrecachedString             (const PrecachedStringID_t id)          { return sm_precachedStrings[id]; }

		void RestoreAllRedirectionsAndHooks (void);

		inline const GameModType_t                                GetGameModID                   (void)                            const { return m_gameModID; }
		inline const DynamicString                               &GetGameModName                 (void)                            const { return m_gameModName; }
		inline const GameMessageContainer                        &GetGameMessageContainer        (void)                            const { return m_gameMessageContainer; }

		inline       SDK::Structures::EngineFunction_t *const     GetEngineFunctionTablePointer  (void)                            const { return m_engineFunctionTablePointer; }
		inline       SDK::Structures::EngineFunction_t           &GetEngineFunctionTable         (void)                                  { return m_engineFunctionTable; }
		inline const SDK::Structures::EngineFunction_t           &GetEngineFunctionTable         (void)                            const { return m_engineFunctionTable; }
		inline       SDK::Structures::ServerStudioAPI_t          &GetEngineStudioFunctionTable   (void)                                  { return m_engineStudioFunctionTable; }
		inline const SDK::Structures::ServerStudioAPI_t          &GetEngineStudioFunctionTable   (void)                            const { return m_engineStudioFunctionTable; }
		inline const SDK::Structures::ServerBlendingInterface_t  &GetBlendingInterface           (void)                            const { return m_blendInterface; }
//		inline const SDK::TypeDefinitions::RotationMatrix_t      &GetRotationMatrix              (void)                            const { return *m_rotationMatrix; }
		inline const SDK::TypeDefinitions::BoneTransformMatrix_t &GetBoneTransformMatrix         (void)                            const { return *m_boneTransformMatrix; }
		inline       SDK::Structures::DLLFunction_t *const        GetGameDLLFunctionTablePointer (void)                                  { return m_gameDLLFunctionAPITablePointer; }
		inline       SDK::Structures::DLLFunction_t              &GetGameDLLFunctionTable        (void)                                  { return m_gameDLLFunctionAPITable; }
		inline       SDK::Structures::NewDLLFunction_t           &GetNewGameDLLFunctionTable     (void)                                  { return m_newGameDLLFunctionAPITable; }

		/// @todo : MOVE THIS TO 'Engine_Listen' CLASS!!!
		/// @note : Only listen server stuff!!!
		///@{
		inline const SDK::Structures::ClientEngineFunction_t &GetClientEngineFunctionTable (void)                            const { return m_clientEngineFunctionTable; }
		inline const SDK::Structures::ClientEngineFunction_t *const GetClientEngineFunctionTablePointer (void)                            const { return m_clientEngineFunctionTablePointer; }
		inline const SDK::Structures::ClientDLLExportTable_t &GetClientDLLExportTable (void) const { return m_clientDLLExportTable; }
		inline const SDK::Structures::StudioModelRenderer_t &GetClientStudioModelRenderer (void) const { return m_clientStudioModelRenderer; }
		inline const SDK::Structures::EngineStudioAPI_t &GetClientEngineStudioAPI (void) const { return m_clientEngineStudioAPI; }
		///@}

		inline void SetPlayerMove (SDK::Structures::PlayerMove_t *const playerMove)
		{
			m_playerMovePointer = playerMove;
			m_playerMove = *playerMove;
		}
		inline const SDK::Structures::PlayerMove_t *const GetPlayerMovePointer (void) const { return m_playerMovePointer; }
		inline const SDK::Structures::PlayerMove_t       &GetPlayerMove        (void) const { return m_playerMove; }

		inline       SDK::Structures::GlobalVariables_t *const  GetGlobalVariables      (void)                                  { return m_globalVariables; }
		inline const SDK::Structures::GlobalVariables_t *const  GetGlobalVariables      (void)                            const { return m_globalVariables; }
		inline const float                                      GetTime                 (void)                            const { return m_globalVariables->time; }	// This function returns engine current time on this server.
		inline SDK::Classes::Interface::FileSystem *const GetFileSystem (void) const { return m_fileSystem; }
		inline SDK::Classes::Interface::Engine *const GetEngine (void) const { return m_engine; }
		inline SDK::Classes::Interface::CDAudio *const GetCDAudio (void) const { return m_cdaudio; }
		inline SDK::VGUI::ISurface *const GetSurface (void) const { return m_surface; }
		inline const double                                     GetRealTime             (void)                            const { return m_realTime; }
		inline const SDK::TypeDefinitions::SV_InactivateClientsFunction_t GetSV_InactivateClientsFunction (void) const { return m_SV_InactivateClientsFunction; }
		inline const SDK::TypeDefinitions::SV_CalcPingFunction_t GetSV_CalcPingFunction (void) const { return m_SV_CalcPing; }
		inline void CallGL_DisableMultitextureFunction (void) const { (*m_GL_DisableMultitextureFunction) (); }
		inline const DynamicString                              GetMapName              (void)                            const { return GetStringFromOffset (m_globalVariables->mapName); }	// This function gets the map name and store it in the 'map_name' global string variable (maximum map name is 32 characters).
		inline const unsigned char                              GetMaximumClients       (void)                            const { return static_cast <unsigned char> (m_globalVariables->maximumClients); }	// This function returns current players on server.

		const bool MakeLocalFileCopy (const DynamicString &filename) const;

		inline const short                                      GetPrecachedSpriteIndex (const PrecachedSpriteIndex_t id) const { return m_precachedSpritesIndexes[id]; }
/*
		inline const bool RedirectEngineInterfaceFunction (const unsigned char index)
		{
			VoidBaseHookedClassWithVirtualFunctionTable *const eng (reinterpret_cast <VoidBaseHookedClassWithVirtualFunctionTable *> (m_engine));

			if (!Trampolines::VirtualProtect (&eng->GetPseudoVirtualFunctionTable ()[index], sizeof (eng->GetPseudoVirtualFunctionTable ()[index]), PAGE_READWRITE))
			{
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect(Engine::TrapKey_Event) failed!\nError code: %i.", GetLastError ());

				return false;
			}

			CHEAT::g_originalEngine__TrapKey_Event = reinterpret_cast <CHEAT::Engine__TrapKey_Event_t &> (eng->GetPseudoVirtualFunctionTable ()[index]);
			eng->GetPseudoVirtualFunctionTable ()[index] = &CHEAT::Engine__TrapKey_Event;

			return true;
		}
*/
		inline void DirectorHudMessage (const Math::Vector2D &coordinates, const Color <> &color, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const char *const message) const
		{
			const unsigned char messageLength (GetStringLength <unsigned char> (message));

			// Reliability check.
			InternalAssert (messageLength <= SDK::Constants::MAX_DIRECTOR_CMD_STRING);

			MessageBegin (SDK::Constants::MSG_BROADCAST, SDK::Constants::SVC_DIRECTOR, NULL, NULL);
				WriteByte (sizeof (unsigned char) + sizeof (unsigned char) + sizeof (unsigned int) + sizeof (Math::Vector2D) + sizeof (float) + sizeof (float) + sizeof (float) + sizeof (float) + messageLength + sizeof ('\0'));	// Total director message length
				WriteByte (SDK::Constants::DRC_CMD_MESSAGE);
				WriteByte (static_cast <const unsigned char> (effect));	// effect (fade in/out)
				WritePackedColor (color);	// Color
				WriteFloat (coordinates.x);	// x coordinates
				WriteFloat (coordinates.y);	// y coordinates
				WriteFloat (fadeInTime);	// fade-in time in seconds
				WriteFloat (fadeOutTime);	// fade-out time in seconds
				WriteFloat (holdTime);	// hold time in seconds
				WriteFloat (fxTime);	// fx time in seconds
				WriteString (message);	// write message
			MessageEnd ();
		}
		inline void DirectorHudMessageFormat (const Math::Vector2D &coordinates, const Color <> &color, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const char *const format, ...) const
		{
			char message[SDK::Constants::MAX_DIRECTOR_CMD_STRING];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			DirectorHudMessage (coordinates, color, effect, fxTime, holdTime, fadeInTime, fadeOutTime, message);
		}
		inline void DirectorHudMessage (SDK::Classes::Edict *const client, const Math::Vector2D &coordinates, const Color <> &color, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const char *const message) const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.
		inline void DirectorHudMessageFormat (SDK::Classes::Edict *const client, const Math::Vector2D &coordinates, const Color <> &color, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const char *const format, ...) const
		{
			char message[SDK::Constants::MAX_DIRECTOR_CMD_STRING];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			DirectorHudMessage (client, coordinates, color, effect, fxTime, holdTime, fadeInTime, fadeOutTime, message);
		}
		inline void HudMessage (const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const SDK::Constants::HUDMessageChannel_t channel, const char *const message) const
		{
			// Reliability check.
			InternalAssert (GetStringLength <unsigned short> (message) <= SDK::Constants::MaximumClientHudMessageLength);

			MessageBegin (SDK::Constants::MSG_BROADCAST, SDK::Constants::SVC_TEMPENTITY, NULL, NULL);
				WriteByte (SDK::Constants::TempEntityEvent_TextMessage);
				WriteByte (static_cast <const char> (channel));	// channel
				WriteShort (Utilities::GetFixedSigned16 (coordinates.x, 13u));	// x coordinates
				WriteShort (Utilities::GetFixedSigned16 (coordinates.y, 13u));	// y coordinates
				WriteByte (static_cast <const unsigned char> (effect));			// effect (fade in/out)
				WriteColor (color);	// Color
				WriteColor (effectColor);	// Effect color
				WriteShort (Utilities::GetFixedUnsigned16 (fadeInTime, 8u));	// fade-in time in seconds
				WriteShort (Utilities::GetFixedUnsigned16 (fadeOutTime, 8u));	// fade-out time in seconds
				WriteShort (Utilities::GetFixedUnsigned16 (holdTime, 8u));		// hold time in seconds

				if (effect == SDK::Constants::HUDMessageEffect_2)
					WriteShort (Utilities::GetFixedUnsigned16 (fxTime, 8u));	// fx time in seconds

				WriteString (message);	// write message
			MessageEnd ();
		}
		inline void HudMessageFormat (const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const SDK::Constants::HUDMessageChannel_t channel, const char *const format, ...) const
		{
			char message[SDK::Constants::MaximumClientHudMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			HudMessage (coordinates, color, effectColor, effect, fxTime, holdTime, fadeInTime, fadeOutTime, channel, message);
		}
		inline void HudMessage (SDK::Classes::Edict *const client, const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const SDK::Constants::HUDMessageChannel_t channel, const char *const message) const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.
		inline void HudMessageFormat (SDK::Classes::Edict *const client, const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const SDK::Constants::HUDMessageChannel_t channel, const char *const format, ...) const
		{
			char message[SDK::Constants::MaximumClientHudMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			HudMessage (client, coordinates, color, effectColor, effect, fxTime, holdTime, fadeInTime, fadeOutTime, channel, message);
		}
		inline void Print (SDK::Classes::Edict *const client, const SDK::Constants::PrintType_t type, const char *const message) const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.
		inline void PrintFormat (SDK::Classes::Edict *const client, const SDK::Constants::PrintType_t type, const char *const format, ...) const
		{
			// Print a text message to this client.

			char message[SDK::Constants::MaximumClientMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			Print (client, type, message);
		}
		inline void Print (const SDK::Constants::HUDPrintType_t type, const char *const message) const
		{
			// Reliability check.
			InternalAssert (GetStringLength <unsigned short> (message) <= SDK::Constants::MaximumTextMessageLength);
/*
			if (IsDedicatedServer ())
			{
				(*m_engineFunctionTable.pfnServerPrint) (message);

				return;
			}
*/
			MessageBegin (SDK::Constants::MSG_BROADCAST, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_Text), NULL, NULL);
				WriteByte (static_cast <unsigned char> (type));
				WriteString (message);
			MessageEnd ();
		}
		inline void PrintFormat (const SDK::Constants::HUDPrintType_t type, const char *const format, ...) const
		{
			char message[SDK::Constants::MaximumTextMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			Print (type, message);
		}
		inline void Print (SDK::Classes::Edict *const client, const SDK::Constants::HUDPrintType_t type, const char *const message) const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.
		inline void PrintFormat (SDK::Classes::Edict *const client, const SDK::Constants::HUDPrintType_t type, const char *const format, ...) const
		{
			char message[SDK::Constants::MaximumTextMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			Print (client, type, message);
		}
		inline void DrawLine (const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const unsigned char width, const unsigned char noise, const Color <> &color, const unsigned char brightness, const unsigned char speed, const unsigned char duration) const	// Purpose: Draws a line from one position to another.
		{
			// This function draws a line visible from the client side of the player whose player entity
			// is pointed to by 'client', from the vector location 'source' to the vector location 'destination',
			// which is supposed to last life tenths seconds, and having the color defined by RGB.

			// send this client a packet telling him to draw a beam using the specified parameters
			MessageBegin (SDK::Constants::MSG_BROADCAST, SDK::Constants::SVC_TEMPENTITY, NULL, NULL);
				WriteByte (SDK::Constants::TempEntityEvent_BeamPoints);
				WriteCoordinates (destination);	// source position
				WriteCoordinates (source);	// destination position
				WriteShort (spriteIndex);	// sprite index
				WriteByte (0u);		// starting frame
				WriteByte (10u);	// frame rate in 0.1's
				WriteByte (duration);	// life in 0.1's
				WriteByte (width);	// line width in 0.1's
				WriteByte (noise);	// noise amplitude in 0.01's
				WriteColor (color);	// Color
				WriteByte (brightness);	// brightness
				WriteByte (speed);	// scroll speed in 0.1's
			MessageEnd ();
		}
		inline void DrawLine (SDK::Classes::Edict *const client, const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const unsigned char width, const unsigned char noise, const Color <> &color, const unsigned char brightness, const unsigned char speed, const unsigned char duration) const;	// Purpose: Draws a line from one position to another. (Note: This function declared in 'Half-Life engine/Core.hpp')
		inline void                         DrawCross2D               (const Math::Vector3D &position, const float length, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			DrawLine (position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawCross2D               (SDK::Classes::Edict *const client, const Math::Vector3D &position, const float length, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			DrawLine (client, position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawCross3D               (const Math::Vector3D &position, const Math::Vector3D &mins, const Math::Vector3D &maxs, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw a colored 3D cross of the given hull size at the given position.

			Math::Vector3D start (position + mins);
			Math::Vector3D end (position + maxs);

			DrawLine (start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			start.x += maxs.x - mins.x;
			end.x -= maxs.x - mins.x;

			DrawLine (start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			start.y += maxs.y - mins.y;
			end.y -= maxs.y - mins.y;

			DrawLine (start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			start.x -= maxs.x - mins.x;
			end.x += maxs.x - mins.x;

			DrawLine (start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawCross3D               (SDK::Classes::Edict *const client, const Math::Vector3D &position, const Math::Vector3D &mins, const Math::Vector3D &maxs, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw a colored 3D cross of the given hull size at the given position.

			Math::Vector3D start (position + mins);
			Math::Vector3D end (position + maxs);

			DrawLine (client, start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			start.x += maxs.x - mins.x;
			end.x -= maxs.x - mins.x;

			DrawLine (client, start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			start.y += maxs.y - mins.y;
			end.y -= maxs.y - mins.y;

			DrawLine (client, start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			start.x -= maxs.x - mins.x;
			end.x += maxs.x - mins.x;

			DrawLine (client, start, end, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawCross3DWithoutNegativeZ (const Math::Vector3D &position, const float length, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			DrawLine (position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (position, position + Math::Vector3D (0.0f, 0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawCross3D               (const Math::Vector3D &position, const float length, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			DrawLine (position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (position - Math::Vector3D (0.0f, 0.0f, length), position + Math::Vector3D (0.0f, 0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawCross3D               (SDK::Classes::Edict *const client, const Math::Vector3D &position, const float length, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			DrawLine (client, position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, position - Math::Vector3D (0.0f, 0.0f, length), position + Math::Vector3D (0.0f, 0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawCross3DWithoutNegativeZ (SDK::Classes::Edict *const client, const Math::Vector3D &position, const float length, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw a colored 3D cross of the given size at the given position.

			DrawLine (client, position - Math::Vector2D (length, 0.0f), position + Math::Vector2D (length, 0.0f), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, position - Math::Vector2D (0.0f, length), position + Math::Vector2D (0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, position, position + Math::Vector3D (0.0f, 0.0f, length), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawTriangle              (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw triangle.

			DrawLine (point1, point2, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (point2, point3, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (point1, point3, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawTriangle              (SDK::Classes::Edict *const client, const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw triangle.

			DrawLine (client, point1, point2, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, point2, point3, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, point1, point3, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawQuadrangle            (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const Math::Vector3D &point4, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw quadrangle.

			DrawLine (point1, point2, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (point2, point3, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (point3, point4, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (point1, point4, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawQuadrangle            (SDK::Classes::Edict *const client, const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const Math::Vector3D &point4, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// Purpose: Draw quadrangle.

			DrawLine (client, point1, point2, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, point2, point3, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, point3, point4, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, point1, point4, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawHorizontalArrow       (const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const unsigned char width, const unsigned char noise, const Color <> &color, const unsigned char brightness, const unsigned char speed, const unsigned char duration) const
		{
			// Purpose: Draw a horizontal arrow pointing in the specified direction.
/*
			const Math::Vector3D offset (0.0f, 0.0f, 1.0f);

			DrawLine (source + offset, destination + offset, spriteIndex, width, noise, color, brightness, speed, duration);
*/
			const Math::Vector3D lineDir ((destination - source).Normalize ());
			const Math::Vector3D upVec (Math::Vector3D::UnitZ);
			const Math::Vector3D sideDir (lineDir ^ upVec);
			const float radius (width * 0.5f);
			const Math::Vector3D p1 (source - sideDir * radius);
			const Math::Vector3D p2 (destination - lineDir * width - sideDir * radius);
			const Math::Vector3D p3 (destination - lineDir * width - sideDir * width);
			const Math::Vector3D p4 (destination);
			const Math::Vector3D p5 (destination - lineDir * width + sideDir * width);
			const Math::Vector3D p6 (destination - lineDir * width + sideDir * radius);
			const Math::Vector3D p7 (source + sideDir * radius);

			// Outline the arrow
			DrawLine (p1, p2, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (p2, p3, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (p3, p4, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (p4, p5, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (p5, p6, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (p6, p7, spriteIndex, 1u, noise, color, brightness, speed, duration);

			// Fill us in with triangles
			DrawTriangle (p5, p4, p3, spriteIndex, color, duration);	// Tip
			DrawTriangle (p1, p7, p6, spriteIndex, color, duration);	// Shaft
			DrawTriangle (p6, p2, p1, spriteIndex, color, duration);

			// And backfaces
			DrawTriangle (p3, p4, p5, spriteIndex, color, duration);	// Tip
			DrawTriangle (p6, p7, p1, spriteIndex, color, duration);	// Shaft
			DrawTriangle (p1, p2, p6, spriteIndex, color, duration);
		}
		inline void                         DrawHorizontalArrow       (SDK::Classes::Edict *const client, const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const unsigned char width, const unsigned char noise, const Color <> &color, const unsigned char brightness, const unsigned char speed, const unsigned char duration) const
		{
			// Purpose: Draw a horizontal arrow pointing in the specified direction.
/*
			const Math::Vector3D offset (0.0f, 0.0f, 1.0f);

			DrawLine (client, source + offset, destination + offset, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
*/
			const Math::Vector3D lineDir ((destination - source).Normalize ());
			const Math::Vector3D upVec (Math::Vector3D::UnitZ);
			const Math::Vector3D sideDir (lineDir ^ upVec);
			const float radius (width * 0.5f);
			const Math::Vector3D p1 (source - sideDir * radius);
			const Math::Vector3D p2 (destination - lineDir * width - sideDir * radius);
			const Math::Vector3D p3 (destination - lineDir * width - sideDir * width);
			const Math::Vector3D p4 (destination);
			const Math::Vector3D p5 (destination - lineDir * width + sideDir * width);
			const Math::Vector3D p6 (destination - lineDir * width + sideDir * radius);
			const Math::Vector3D p7 (source + sideDir * radius);

			// Outline the arrow
			DrawLine (client, p1, p2, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (client, p2, p3, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (client, p3, p4, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (client, p4, p5, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (client, p5, p6, spriteIndex, 1u, noise, color, brightness, speed, duration);
			DrawLine (client, p6, p7, spriteIndex, 1u, noise, color, brightness, speed, duration);

			// Fill us in with triangles
			DrawTriangle (client, p5, p4, p3, spriteIndex, color, duration);	// Tip
			DrawTriangle (client, p1, p7, p6, spriteIndex, color, duration);	// Shaft
			DrawTriangle (client, p6, p2, p1, spriteIndex, color, duration);

			// And backfaces
			DrawTriangle (client, p3, p4, p5, spriteIndex, color, duration);	// Tip
			DrawTriangle (client, p6, p7, p1, spriteIndex, color, duration);	// Shaft
			DrawTriangle (client, p1, p2, p6, spriteIndex, color, duration);
		}
		inline void                         DrawDashedLine            (const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			const Math::Vector3D offset (0.0f, 0.0f, 1.0f);
			const float solidLength (7.0f);
			const float gapLength (3.0f);
			Math::Vector3D direction (destination - source);
			const float totalDistance (direction.GetLength ());
			float distance (0.0f);

			// Reliability check.
			InternalAssert (totalDistance > 0.0f);

			direction /= totalDistance;

			do
			{
				distance += solidLength + gapLength;

				DrawLine (source + direction * distance + offset, source + direction * Math::GetMinimumValueBetween (distance + solidLength, totalDistance) + offset, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			} while (distance < totalDistance);
		}
		inline void                         DrawDashedLine            (SDK::Classes::Edict *const client, const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			const Math::Vector3D offset (0.0f, 0.0f, 1.0f);
			const float solidLength (7.0f);
			const float gapLength (3.0f);
			Math::Vector3D direction (destination - source);
			const float totalDistance (direction.GetLength ());
			float distance (0.0f);

			// Reliability check.
			InternalAssert (totalDistance > 0.0f);

			direction /= totalDistance;

			do
			{
				distance += solidLength + gapLength;

				DrawLine (client, source + direction * distance + offset, source + direction * Math::GetMinimumValueBetween (distance + solidLength, totalDistance) + offset, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			} while (distance < totalDistance);
		}
		inline void                         DrawBox                   (const Math::Vector3D &absoluteMins, const Math::Vector3D &absoluteMaxs, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// This function draws an axial box (i.e, a box whose faces are parallel to the map's axises)
			// visible from the client side of the player whose player entity is pointed to by 'client',
			// encompassing the bounding box defined by the two vector locations 'absoluteMins' and 'absoluteMaxs', for a
			// duration of life tenths seconds, and having the color defined by RGB.

			// bottom square
			DrawLine (absoluteMins, Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), absoluteMins, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			// verticals
			DrawLine (absoluteMins, Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), absoluteMaxs, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			// top square
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), absoluteMaxs, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (absoluteMaxs, Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawBox                   (SDK::Classes::Edict *const client, const Math::Vector3D &absoluteMins, const Math::Vector3D &absoluteMaxs, const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			// This function draws an axial box (i.e, a box whose faces are parallel to the map's axises)
			// visible from the client side of the player whose player entity is pointed to by 'client',
			// encompassing the bounding box defined by the two vector locations 'absoluteMins' and 'absoluteMaxs', for a
			// duration of life tenths seconds, and having the color defined by RGB.

			// bottom square
			DrawLine (client, absoluteMins, Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), absoluteMins, spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			// verticals
			DrawLine (client, absoluteMins, Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMins.z), Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMaxs.x, absoluteMaxs.y, absoluteMins.z), absoluteMaxs, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMins.z), Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			// top square
			DrawLine (client, Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMaxs.x, absoluteMins.y, absoluteMaxs.z), absoluteMaxs, spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), Math::Vector3D (absoluteMins.x, absoluteMins.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, absoluteMaxs, Math::Vector3D (absoluteMins.x, absoluteMaxs.y, absoluteMaxs.z), spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawBox                   (const Math::Vector3D points[8u], const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			DrawLine (points[0u], points[1u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[2u], points[3u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[4u], points[5u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[6u], points[7u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			DrawLine (points[0u], points[2u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[2u], points[4u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[4u], points[6u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[6u], points[0u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			DrawLine (points[1u], points[3u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[3u], points[5u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[5u], points[7u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (points[7u], points[1u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}
		inline void                         DrawBox                   (SDK::Classes::Edict *const client, const Math::Vector3D points[8u], const short spriteIndex, const Color <> &color, const unsigned char duration) const
		{
			DrawLine (client, points[0u], points[1u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[2u], points[3u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[4u], points[5u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[6u], points[7u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			DrawLine (client, points[0u], points[2u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[2u], points[4u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[4u], points[6u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[6u], points[0u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);

			DrawLine (client, points[1u], points[3u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[3u], points[5u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[5u], points[7u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
			DrawLine (client, points[7u], points[1u], spriteIndex, 5u, 0u, color, 250u, 5u, duration);
		}

		inline void DisplayMenu (const char *text, const SDK::Constants::Menu::KeyBit_t validSlots) const
		{
			// Reliability check.
			InternalAssert (!IsNullOrEmptyString (text));

			unsigned short textLength (GetStringLength <unsigned short> (text));

			// Reliability check.
			InternalAssert (textLength <= SDK::Constants::MaximumMenuFullStringLength);

			while (textLength >= SDK::Constants::MaximumMenuStringLength)
			{
				MessageBegin (SDK::Constants::MSG_BROADCAST, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_ShowMenu), NULL, NULL);
					WriteShort (validSlots);	// used key slots
					WriteCharacter (-1);	// lifetime seconds (forever)
					WriteByte (true);	// needmore? (yes)
/*
					const char tempCharacter (text[SDK::Constants::MaximumMenuStringLength]);	// Save last character....

					text[SDK::Constants::MaximumMenuStringLength] = '\0';	// Terminate the string for WriteString()....

						WriteString (text);	// Menu text to display (character to character)

					text[SDK::Constants::MaximumMenuStringLength] = tempCharacter;	// Save last character....
*/
					for (unsigned char index (0u); index < SDK::Constants::MaximumMenuStringLength; ++index)
						WriteCharacter (text[index]);	// Menu text to display (character to character)

				MessageEnd ();

				text += SDK::Constants::MaximumMenuStringLength;
				textLength -= SDK::Constants::MaximumMenuStringLength;
			}

			// And send the final message....
			MessageBegin (SDK::Constants::MSG_BROADCAST, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_ShowMenu), NULL, NULL);
				WriteShort (validSlots);	// used key slots
				WriteCharacter (-1);	// lifetime seconds (forever)
				WriteByte (false);	// needmore? (no)
				WriteString (text);	// Menu text to display
			MessageEnd ();
		}
		inline void DisplayMenu (SDK::Classes::Edict *const client, const char *text, const SDK::Constants::Menu::KeyBit_t validSlots) const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.
		inline void CloseMenu   (void) const
		{
			// Reset the currently displaying menu.
			MessageBegin (SDK::Constants::MSG_BROADCAST, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_ShowMenu), NULL, NULL);
				WriteShort (SDK::Constants::Menu::KeyBit_None);	// used key slots
				WriteCharacter (0);	// lifetime seconds (none)
				WriteByte (false);	// needmore? (no)
				WriteString (NULL);	// Menu text to display
			MessageEnd ();
		}
		inline void CloseMenu   (SDK::Classes::Edict *const client) const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.

		inline void DisplayMOTD (SDK::Classes::Edict *const client, const DynamicString &name, const char *text) const;

		inline SDK::Classes::Edict *const FIND_ENTITY_BY_CLASSNAME (SDK::Classes::Edict *const entityStart, const char *const className) const
		{
			return (*m_engineFunctionTable.pfnFindEntityByString) (entityStart, "classname", className);
		}
		inline SDK::Classes::Edict *const FIND_ENTITY_BY_TARGETNAME (SDK::Classes::Edict *const entityStart, const char *const targetName) const
		{
			return (*m_engineFunctionTable.pfnFindEntityByString) (entityStart, "targetname", targetName);
		}
		inline SDK::Classes::Edict *const FIND_ENTITY_BY_TARGET (SDK::Classes::Edict *const entityStart, const char *const target) const
		{
			// For doing a reverse lookup. Say you have a door, and want to find it's button.

			return (*m_engineFunctionTable.pfnFindEntityByString) (entityStart, "target", target);
		}

		inline const char *const                          GetStringPointerFromOffset (const SDK::TypeDefinitions::StringOffset_t offset) const { return m_globalVariables->stringBase + offset; }
		inline const DynamicString                        GetStringFromOffset        (const SDK::TypeDefinitions::StringOffset_t offset) const { return GetStringPointerFromOffset (offset); }
		inline const SDK::TypeDefinitions::StringOffset_t GetOffsetFromString (const char *const string) const
		{
			// Reliability check.
			InternalAssert (!IsNullString (string));

			return string - m_globalVariables->stringBase;
		}

		inline void GiveNamedItem (SDK::Classes::Edict *const player, const char *const className) const;

	//
	// Group: Callbacks.
	//
	// Description: The actual engine callbacks.
	//
	public:
		inline const bool                     IsDedicatedServer        (void) const { return (*m_engineFunctionTable.pfnIsDedicatedServer) () == TRUE; }	// This function returns true if server is dedicated server, false otherwise.
		inline const bool                     IsListenServer           (void) const { return IsDedicatedServer () == false; }	// This function returns true if server is listen server, false otherwise.

		inline const unsigned char            GetPlayerUserID          (const SDK::Classes::Edict *const player) const { return static_cast <const unsigned char> ((*m_engineFunctionTable.pfnGetPlayerUserId) (player)); }
		inline const short                    PrecacheModel            (const char *const modelName) const { return static_cast <short> ((*m_engineFunctionTable.pfnPrecacheModel) (modelName)); }
		inline const short                    PrecacheModelSafe        (const PrecachedStringID_t modelNameID) const { return PrecacheModel (GetPrecachedString (modelNameID)); }
		inline const int                      PrecacheSound            (const char *const soundName) const { return (*m_engineFunctionTable.pfnPrecacheSound) (soundName); }
		inline const int                      PrecacheSoundSafe        (const PrecachedStringID_t soundNameID) const { return PrecacheSound (GetPrecachedString (soundNameID)); }
		inline const int                      PrecacheGeneric          (const char *const s)         const { return (*m_engineFunctionTable.pfnPrecacheGeneric) (s); }
		inline const int                      PrecacheGenericSafe      (const PrecachedStringID_t sID)         const { return PrecacheGeneric (GetPrecachedString (sID)); }
		inline void                           SetEntityModel           (SDK::Classes::Edict *const entity, const char *const modelName) const { (*m_engineFunctionTable.pfnSetModel) (entity, modelName); }
		inline const int                      GetModelIndex            (const char *const modelName)                                                                                                                                                                 const { return (*m_engineFunctionTable.pfnModelIndex) (modelName); }
//		inline const int                      MODEL_FRAMES             (const int modelIndex)                                                                                                                                                                        const { return (*m_engineFunctionTable.pfnModelFrames) (modelIndex); }
//		inline void                           SET_SIZE                 (SDK::Classes::Edict *const entity, const Math::Vector3D &min, const Math::Vector3D &max)                                                                                                     const { (*m_engineFunctionTable.pfnSetSize) (entity, min, max); }
		inline void                           ChangeLevel              (const char *const s1, const char *const s2)                                                                                                                                                  const { (*m_engineFunctionTable.pfnChangeLevel) (s1, s2); }
//		inline void                           GET_SPAWN_PARMS          (SDK::Classes::Edict *const entity)                                                                                                                                                           const { (*m_engineFunctionTable.pfnGetSpawnParms) (entity); }
//		inline void                           SAVE_SPAWN_PARMS         (SDK::Classes::Edict *const entity)                                                                                                                                                           const { (*m_engineFunctionTable.pfnSaveSpawnParms) (entity); }
//		inline const float                    VEC_TO_YAW               (const Math::Vector3D &vector)                                                                                                                                                                const { return (*m_engineFunctionTable.pfnVecToYaw) (vector); }
		inline void                           VectorToAngles           (const Math::Vector3D &vectorIn, Math::Angles3D &anglesOut)                                                                                                                                     const { (*m_engineFunctionTable.pfnVecToAngles) (vectorIn, anglesOut); }
//		inline void                           MOVE_TO_ORIGIN           (SDK::Classes::Edict *const entity, const Math::Vector3D &goal, const float dist, const int moveType)                                                                                         const { (*m_engineFunctionTable.pfnMoveToOrigin) (entity, goal, dist, moveType); }
//		inline void                           oldCHANGE_YAW            (SDK::Classes::Edict *const entity)                                                                                                                                                           const { (*m_engineFunctionTable.pfnChangeYaw) (entity); }
//		inline void                           CHANGE_PITCH             (SDK::Classes::Edict *const entity)                                                                                                                                                           const { (*m_engineFunctionTable.pfnChangePitch) (entity); }
		inline void                           MakeVectors              (const Math::Angles3D &angles)                                                                                                                                                                  const { (*m_engineFunctionTable.pfnMakeVectors) (angles); }
		inline SDK::Classes::Edict *const     CreateEntity             (void)                                                                                                                                                                                        const { return (*m_engineFunctionTable.pfnCreateEntity) (); }
		template <class classType> inline classType *const AllocateEntity (void) const
		{
			SDK::Classes::Edict *const newEntity (CreateEntity ());

			// Reliability checks.
			InternalAssert (newEntity->IsValid ());
			InternalAssert (newEntity->privateData == NULL);

			SDK::Structures::EntityVariables_t &newEntityVariables (newEntity->variables);

			// allocate private data
//			classType *const classPointer (new (&newEntityVariables) classType);
			classType *const classPointer (AllocateEntityPrivateData <classType> (newEntityVariables.containingEntity));

			// Reliability check.
			InternalAssert (classPointer != NULL);

			classPointer->m_variables = &newEntityVariables;

			return classPointer;
		}
		inline void                           RemoveEntity             (SDK::Classes::Edict *const entity)                                                                                                                                                           const { (*m_engineFunctionTable.pfnRemoveEntity) (entity); }
		inline SDK::Classes::Edict *const     CreateNamedEntity        (const char *const className)                                                                                                                                                                 const { return (*m_engineFunctionTable.pfnCreateNamedEntity) (GetOffsetFromString (className)); }
//		inline void                           MAKE_STATIC              (SDK::Classes::Edict *const entity)                                                                                                                                                           const { (*m_engineFunctionTable.pfnMakeStatic) (entity); }
		inline const bool                     IsEntityOnFloor          (const SDK::Classes::Edict *const entity)                                                                                                                                                     const { return (*m_engineFunctionTable.pfnEntIsOnFloor) (const_cast <SDK::Classes::Edict *> (entity)) == TRUE; }
//		inline const int                      DROP_TO_FLOOR            (SDK::Classes::Edict *const entity)                                                                                                                                                           const { return (*m_engineFunctionTable.pfnDropToFloor) (entity); }
		inline void                           SetEntityOrigin          (SDK::Classes::Edict *const entity, const Math::Vector3D &newOrigin)                                                                                                                          const { (*m_engineFunctionTable.pfnSetOrigin) (entity, newOrigin); }
		inline void                           EmitSound                (const SDK::Classes::Edict *const entity, const SDK::Constants::SoundChannel_t channel, const char *const sample, const float volume = SDK::Constants::SoundVolume_Normal, const float attenuation = SDK::Constants::SoundAttenuation_Normal, const SDK::Constants::SoundFlag_t flags = SDK::Constants::SoundFlag_None, const SDK::Constants::SoundPitch_t pitch = SDK::Constants::SoundPitch_Normal)                                         const { (*m_engineFunctionTable.pfnEmitSound) (const_cast <SDK::Classes::Edict *> (entity), channel, sample, volume, attenuation, flags, pitch); }
//		inline void                           BUILD_SOUND_MSG          (SDK::Classes::Edict *const entity, const SDK::Constants::SoundChannel_t channel, const char *const sample, const float volume, const float attenuation, const SDK::Constants::SoundFlag_t flags, const SDK::Constants::SoundPitch_t pitch, const int destination, const int type, const Math::Vector3D &origin, SDK::Classes::Edict *to) const { (*m_engineFunctionTable.pfnBuildSoundMsg) (entity, channel, sample, volume, attenuation, flags, pitch, destination, type, origin, to); }

		inline void                           TraceLine                (const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::TraceIgnore_t traceIgnore, SDK::Classes::Edict *const entityToSkip, SDK::Structures::TraceResult_t &traceResult)                                  const
		{
			// this function traces a line dot by dot, starting from 'source' vector in the direction of 'destination' vector,
			// ignoring or not monsters (depending on the value of 'TraceIgnore_Monsters', true or false), and stops
			// at the first obstacle encountered, returning the results of the trace in the 'TraceResult_t' structure pointer.
			// Such results are (amongst others) the distance traced, the hit surface, the hit plane
			// vector normal, etc. See the 'TraceResult_t' structure for details. This function allows to specify
			// whether the trace starts "inside" an entity's polygonal model, and if so, to specify that entity
			// in 'ignoreEntity' in order to ignore it as a possible obstacle.
			// this is an overloaded prototype to add 'TraceIgnore_Glass' in the same way as 'TraceIgnore_Monsters' work.

			(*m_engineFunctionTable.pfnTraceLine) (source, destination, traceIgnore, entityToSkip, traceResult);
		}
		inline void                           TraceToss                 (SDK::Classes::Edict *const entity, SDK::Classes::Edict *const entityToSkip, SDK::Structures::TraceResult_t &traceResult)                                                                                const { (*m_engineFunctionTable.pfnTraceToss) (entity, entityToSkip, traceResult); }
//		inline const int                      TRACE_MONSTER_HULL        (SDK::Classes::Edict *const entityEdict, const Math::Vector3D &source, const Math::Vector3D &destination, const int noMonsters, SDK::Classes::Edict *const entityToSkip, SDK::Structures::TraceResult_t &traceResult) const { return (*m_engineFunctionTable.pfnTraceMonsterHull) (entityEdict, source, destination, noMonsters, entityToSkip, traceResult); }

		#include <Half-Life engine/TraceFilters.hpp>

		inline void                           TraceHullThroughAllEntities   (const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::Hull_t hullNumber, Interface_TraceFilter &traceFilter, SDK::Structures::TraceResult_t &traceResult) const;
		inline void                           ClipMoveToEntities            (const SDK::Structures::EdictAreaNode_t *const node, Math::Extent3D &extent, const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::Hull_t hullNumber, Interface_TraceFilter &traceFilter, SDK::Structures::TraceResult_t &traceResult) const;
		inline void                           TraceHullThroughSolidEntities (const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::Hull_t hullNumber, Interface_TraceFilter &traceFilter, SDK::Structures::TraceResult_t &traceResult) const;
		inline void                           TraceHull                 (const Math::Vector3D &source, const Math::Vector3D &destination, const bool ignoreMonsters, const SDK::Constants::Hull_t hullNumber, SDK::Classes::Edict *const entityToSkip, SDK::Structures::TraceResult_t &traceResult)             const
		{
			// this function traces a hull dot by dot, starting from 'source' vector in the direction of 'destination' vector,
			// ignoring or not monsters (depending on the value of 'IGNORE_MONSTERS', true or
			// false), and stops at the first obstacle encountered, returning the results
			// of the trace in the 'TraceResult_t' structure pointer, just like TraceLine(). Hulls that can be traced
			// (by parameter 'hull_type') are 'point_hull' (a line), 'Hull_Head' (size of a crouching player),
			// 'human_hull' (a normal body size) and 'large_hull' (for monsters?). Not all the hulls in the
			// game can be traced here, this function is just useful to give a relative idea of spatial
			// reachability (i.e. can a hostage pass through that tiny hole?) Also like TraceLine(), this
			// function allows to specify whether the trace starts "inside" an entity's polygonal model,
			// and if so, to specify that entity in 'ignoreEntity' in order to ignore it as an obstacle.

			(*m_engineFunctionTable.pfnTraceHull) (source, destination, ignoreMonsters, hullNumber, entityToSkip, traceResult);
		}
		inline void                           TraceEntity                (const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::Hull_t hullNumber, SDK::Classes::Edict *const entity, SDK::Structures::TraceResult_t &traceResult)    const
		{
/*			{	/// @warning ONLY FOR hw.dll (build 4554)
				typedef SDK::Structures::EngineTraceResult_t (*const SV_ClipMoveToEntityFunctionPointer_t) (SDK::Classes::Edict *entity, const Math::Vector3D &source, const Math::Vector3D &mins, const Math::Vector3D &maxs, const Math::Vector3D &destination);
				const SV_ClipMoveToEntityFunctionPointer_t SV_ClipMoveToEntityFunctionPointer (reinterpret_cast <SV_ClipMoveToEntityFunctionPointer_t> (reinterpret_cast <unsigned int> (Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()) + 0xDE020u));

				InternalAssert (SV_ClipMoveToEntityFunctionPointer != NULL);

				const SDK::Structures::EngineTraceResult_t engineTraceResult ((*SV_ClipMoveToEntityFunctionPointer) (entity, source, SDK::Constants::hullSizes[hullNumber].mins, SDK::Constants::hullSizes[hullNumber].maxs, destination));

				traceResult.isAllSolid = engineTraceResult.isAllSolid;
				traceResult.isStartSolid = engineTraceResult.isStartSolid;
				traceResult.isInOpen = engineTraceResult.isInOpen;
				traceResult.isInWater = engineTraceResult.isInWater;
				traceResult.fraction = engineTraceResult.fraction;
				traceResult.endPosition = engineTraceResult.endPosition;
				traceResult.planeDistance = engineTraceResult.plane.distance;
				traceResult.planeNormal = engineTraceResult.plane.normal;
				traceResult.hitEntity = engineTraceResult.hitEntity;
				traceResult.hitGroup = engineTraceResult.hitGroup;
			}
*/
			(*m_engineFunctionTable.pfnTraceEntity) (source, destination, hullNumber, entity, traceResult);
		}
//		inline void                           GET_AIM_VECTOR           (SDK::Classes::Edict *const entity, const float speed, Math::Vector3D &result)                                                                                                                            const { (*m_engineFunctionTable.pfnGetAimVector) (entity, speed, result); }
		inline void                           ExecuteServerCommand       (const DynamicString &command)                                                                                                                                                                       const
		{
			// This function asks the engine to execute a server command.

			// Reliability check.
			InternalAssert (!command.IsEmpty () && command.GetElementNumber () <= SDK::Constants::MaximumServerCommandLength);

			// Execute command.
			(*m_engineFunctionTable.pfnServerCommand) (command);
		}
		inline void                           ExecuteServerCommandFormat (const char *const format, ...)                                                                                                                                                               const
		{
			// This function asks the engine to execute a server command.

			char command[SDK::Constants::MaximumServerCommandLength];

			// Concatenate all the arguments in one command....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, command, sizeof (command));

			// Execute command.
			ExecuteServerCommand (command);
		}
		inline void                           ExecuteServerCommandNow  (const DynamicString &command)                                                                                                                                                                       const
		{
			// This function asks the engine to execute a server command, passed by pfnServerCommand() function.

			// Execute command.
			ExecuteServerCommand (command);
			ExecuteServerCommandNow ();
		}
		inline void                           ExecuteServerCommandFormatNow (const char *const format, ...)                                                                                                                                                                       const
		{
			// This function asks the engine to execute a server command, passed by pfnServerCommand() function.

			char command[SDK::Constants::MaximumServerCommandLength];

			// Concatenate all the arguments in one command....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, command, sizeof (command));

			// Execute command.
			ExecuteServerCommand (command);
			ExecuteServerCommandNow ();
		}
		inline void                           ExecuteServerCommandNow  (void)                                                                                                                                                                       const
		{
			// This function asks the engine to execute a server command, passed by pfnServerCommand() function.

			// Execute command.
			(*m_engineFunctionTable.pfnServerExecute) ();
		}
		inline void                           ExecuteClientCommand     (SDK::Classes::Edict *const client, char *const command)                                                                                                                                      const { (*m_engineFunctionTable.pfnClientCommand) (client, command); }
		inline void                           ParticleEffect           (const Math::Vector3D &origin, const Math::Vector3D &direction, const unsigned char color, const unsigned char count)                                                                                                           const { (*m_engineFunctionTable.pfnParticleEffect) (origin, direction, color, count); }
//		inline void                           LIGHT_STYLE              (const int style, /*const */char *const value)                                                                                                                                                        const { (*m_engineFunctionTable.pfnLightStyle) (style, value); }
		inline const short                    GetDecalIndex            (const char *const name)                                                                                                                                                                          const { return static_cast <short> ((*m_engineFunctionTable.pfnDecalIndex) (name)); }
		inline const SDK::Constants::PointContentType_t GetPointContents            (const Math::Vector3D &vector)                                                                                                                                                            const { return static_cast <SDK::Constants::PointContentType_t> ((*m_engineFunctionTable.pfnPointContents) (vector)); }
//		inline void                           CRC32_INIT               (TypeDefinitions::CRC32_t *pulCRC)                                                                                                                                                                       const { (*m_engineFunctionTable.pfnCRC32_Init) (pulCRC); }
//		inline void                           CRC32_PROCESS_BUFFER     (TypeDefinitions::CRC32_t *pulCRC, void *p, const int length)                                                                                                                                            const { (*m_engineFunctionTable.pfnCRC32_ProcessBuffer) (pulCRC, p, length); }
//		inline void                           CRC32_PROCESS_BYTE       (TypeDefinitions::CRC32_t *pulCRC, const unsigned char ch)                                                                                                                                               const { (*m_engineFunctionTable.pfnCRC32_ProcessByte) (pulCRC, ch); }
//		inline const TypeDefinitions::CRC32_t CRC32_FINAL              (const TypeDefinitions::CRC32_t pulCRC)                                                                                                                                                                  const { return (*m_engineFunctionTable.pfnCRC32_Final) (pulCRC); }

		inline const char *const              GetPlayerAuthID          (SDK::Classes::Edict *const player)                                                                                                                                                           const { return (*m_engineFunctionTable.pfnGetPlayerAuthId) (player); }

		inline const float                    GetWaveSoundPlayDuration (const char *const filePath)                                                                                                                                                                  const { return (*m_engineFunctionTable.pfnGetApproxWavePlayLen) (filePath) * 0.001f; }	// This function returns the wave sound play duration (in minutes).

		inline void                           MessageBegin             (const SDK::Constants::MessageDestination_t destination, const unsigned char type, const Math::Vector3D *const origin, SDK::Classes::Edict *const to)                                         const { (*m_engineFunctionTable.pfnMessageBegin) (destination, type, origin, to); }
		inline void                           MessageEnd               (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnMessageEnd) (); }
		inline void                           WriteByte                (const unsigned char value)                                                                                                                                                                   const { (*m_engineFunctionTable.pfnWriteByte) (value); }
		inline void                           WriteColor               (const Color <> &color)                                                                                                                                                                          const
		{
			WriteByte (color.red);		// red component of color
			WriteByte (color.green);	// green component of color
			WriteByte (color.blue);		// blue component of color
		}
		inline void                           WritePackedColor         (const Color <> &color)                                                                                                                                                                          const
		{
			WriteLong (color.GetRawColor ());	// red, green, blue components of color
		}
		inline void                           WriteColor               (const ColorWithAlpha <> &color)                                                                                                                                                                 const
		{
			WriteByte (color.red);		// initial RED
			WriteByte (color.green);	// initial GREEN
			WriteByte (color.blue);		// initial BLUE
			WriteByte (color.alpha);	// initial ALPHA
		}
		inline void                           WritePackedColor         (const ColorWithAlpha <> &color)                                                                                                                                                                 const
		{
			WriteLong (color.GetRawColor ());	// initial RED, GREEN, BLUE, ALPHA
		}
		inline void                           WriteCharacter           (const char value)                                                                                                                                                                            const { (*m_engineFunctionTable.pfnWriteChar) (value); }
		inline void                           WriteShort               (const short value)                                                                                                                                                                           const { (*m_engineFunctionTable.pfnWriteShort) (value); }
		inline void                           WriteLong                (const int value)                                                                                                                                                                             const { (*m_engineFunctionTable.pfnWriteLong) (value); }
		inline void                           WriteFloat               (const float value)                                                                                                                                                                           const { WriteLong (*reinterpret_cast <const unsigned int *const> (&value)); }
		inline void                           WriteAngle               (const float value)                                                                                                                                                                           const { (*m_engineFunctionTable.pfnWriteAngle) (value); }
		inline void                           WriteCoordinate          (const float value)                                                                                                                                                                           const { (*m_engineFunctionTable.pfnWriteCoord) (value); }
		inline void                           WriteCoordinates         (const Math::Vector2D &coordinates)                                                                                                                                                           const { WriteCoordinate (coordinates.x); WriteCoordinate (coordinates.y); }
		inline void                           WriteCoordinates         (const Math::Vector3D &coordinates)                                                                                                                                                           const { WriteCoordinate (coordinates.x); WriteCoordinate (coordinates.y); WriteCoordinate (coordinates.z); }
		inline void                           WriteString              (const char *const string)                                                                                                                                                                    const { (*m_engineFunctionTable.pfnWriteString) (string); }
		inline void                           WriteEntity              (const unsigned int entityIndex)                                                                                                                                                              const { (*m_engineFunctionTable.pfnWriteEntity) (entityIndex); }
		inline void                           WriteEntity              (const SDK::Classes::Edict *const entity)                                                                                                                                                     const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.

		inline void                                              ConsoleVariable_Register   (SDK::Classes::ConsoleVariable &consoleVariable)                                                                                                                         const { (*m_engineFunctionTable.pfnCVarRegister) (consoleVariable); }
		template <typename elementType> inline const elementType ConsoleVariable_GetValue   (const char *const variableName)                                                                                                                                         const { return static_cast <const elementType> ((*m_engineFunctionTable.pfnCVarGetFloat) (variableName)); }
		template </* bool */>           inline const bool        ConsoleVariable_GetValue   (const char *const variableName)                                                                                                                                         const { return (*m_engineFunctionTable.pfnCVarGetFloat) (variableName) > 0.0f; }
		template </* float */>          inline const float       ConsoleVariable_GetValue   (const char *const variableName)                                                                                                                                         const { return (*m_engineFunctionTable.pfnCVarGetFloat) (variableName); }
		inline const DynamicString                               ConsoleVariable_GetString  (const char *const variableName)                                                                                                                                         const { return (*m_engineFunctionTable.pfnCVarGetString) (variableName); }
		template <typename elementType> inline void              ConsoleVariable_SetValue   (const char *const variableName, const elementType value)                                                                                                                const { (*m_engineFunctionTable.pfnCVarSetFloat) (variableName, static_cast <const float> (value)); }
		template </* float */>          inline void              ConsoleVariable_SetValue   (const char *const variableName, const float value)                                                                                                                      const { (*m_engineFunctionTable.pfnCVarSetFloat) (variableName, value); }
		inline void                                              ConsoleVariable_SetString  (const char *const variableName, const char *const value)                                                                                                                const { (*m_engineFunctionTable.pfnCVarSetString) (variableName, value); }
		inline SDK::Classes::ConsoleVariable *const              ConsoleVariable_GetPointer (const char *const variableName)                                                                                                                                         const { return (*m_engineFunctionTable.pfnCVarGetPointer) (variableName); }

		inline void                           AlertMessage               (const SDK::Constants::AlertType_t alertType, const char *const message)                                                                                                                    const { (*m_engineFunctionTable.pfnAlertMessage) (alertType, message); }
		inline void                           AlertMessageFormat         (const SDK::Constants::AlertType_t alertType, const char *const format, ...)                                                                                                                const
		{
			char message[SDK::Constants::MaximumAlertMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			AlertMessage (alertType, message);
		}
//		inline                                ENGINE_FPRINTF            (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnEngineFprintf) (); }
		inline void *const                    AllocateEntityPrivateData (SDK::Classes::Edict *const entity, const size_t allocateBlockSize)                                                                                                                           const { return (*m_engineFunctionTable.pfnPvAllocEntPrivateData) (entity, allocateBlockSize); }
		template <class classType> inline classType *const                    AllocateEntityPrivateData (SDK::Classes::Edict *const entity)                                                                                                                           const { return static_cast <classType *const> (AllocateEntityPrivateData (entity, sizeof (classType))); }
		inline void                           FreeEntityPrivateData     (SDK::Classes::Edict *const entity)                                                                                                                                                           const { (*m_engineFunctionTable.pfnFreeEntPrivateData) (entity); }
		inline const char *const                          GetEngineStringPointerFromOffset       (const SDK::TypeDefinitions::StringOffset_t offset)                                                                                                                               const { return (*m_engineFunctionTable.pfnSzFromIndex) (offset); }
		inline const SDK::TypeDefinitions::StringOffset_t AllocateEngineString                   (const char *const string)                                                                                                                                                        const { return (*m_engineFunctionTable.pfnAllostring) (string); }
		inline const char *const                          AllocateEngineStringAndGetPointer      (const char *const string)                                                                                                                                                        const { return GetStringPointerFromOffset (AllocateEngineString (string)); }
		inline SDK::Classes::Edict *const     FindEntityByString        (SDK::Classes::Edict *const pentEdictStartSearchAfter, const char *const field, const char *const value)                                                                                      const { return (*m_engineFunctionTable.pfnFindEntityByString) (pentEdictStartSearchAfter, field, value); }
		inline const unsigned char            GetEntityIllumination     (const SDK::Classes::Edict *const entity)                                                                                                                                                     const { return static_cast <unsigned char> ((*m_engineFunctionTable.pfnGetEntityIllum) (entity)); }
		inline SDK::Classes::Edict *const     FindEntityInSphere        (SDK::Classes::Edict *const pentEdictStartSearchAfter, const Math::Vector3D &origin, const float radius)                                                                                      const { return (*m_engineFunctionTable.pfnFindEntityInSphere) (pentEdictStartSearchAfter, origin, radius); }
//		inline                                FIND_CLIENT_IN_PVS        (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnFindClientInPVS) (); }
//		inline                                EMIT_AMBIENT_SOUND        (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnEmitAmbientSound) (); }
		inline SDK::Classes::Edict *const                 GetEntityOfEdictOffset    (const SDK::TypeDefinitions::EntityOffset_t edictOffset)                                                                                                                                           const { return (*m_engineFunctionTable.pfnPEntityOfEntOffset) (edictOffset); }
		inline const SDK::TypeDefinitions::EntityOffset_t GetEntityOffset           (const SDK::Classes::Edict *const entity)                                                                                                                                                     const { return (*m_engineFunctionTable.pfnEntOffsetOfPEntity) (entity); }
		inline const unsigned short           GetEdictIndex             (const SDK::Classes::Edict *const entity)                                                                                                                                                     const { return static_cast <unsigned short> ((*m_engineFunctionTable.pfnIndexOfEdict) (entity)); }
		inline SDK::Classes::Edict *const     GetEdictOfIndex           (const unsigned short/*int*/ entityIndex)                                                                                                                                                              const { return (*m_engineFunctionTable.pfnPEntityOfEntIndex) (entityIndex); }
		inline SDK::Classes::Edict *const     GetWorldSpawnEdict        (void)                                                                                                                                                                                        const { return GetEdictOfIndex (SDK::Constants::WorldspawnEntitySlotIndex); }
		inline SDK::Structures::StudioHeader_t *const GetEdictModelPointer     (const SDK::Classes::Edict *const entity)                                                                                                                                        const { return (*m_engineFunctionTable.pfnGetModelPtr) (entity); }
		inline const unsigned char            RegisterUserMessage       (const DynamicString &name, const short size)
		{
			const unsigned char message (static_cast <unsigned char> ((*m_engineFunctionTable.pfnRegUserMsg) (name, size)));	// register the message for the engine

			if (message > SDK::Constants::SVC_BAD)
				m_gameMessageContainer.Register (name, message);	// Register this message for a bot....

			// ...and return the registered message ID number the engine gave us back.
			return message;
		}
		inline void                           StudioSetupPlayerBones    (const SDK::Classes::Edict *const entity, const char boneID = -1/* means setup all the bones. */)                                                                                                                             const;	// Note: Theis function declared in Core.hpp.
		inline void                           GetBonePosition           (const SDK::Classes::Edict *const entity, const unsigned char boneID, Math::Vector3D &boneOrigin, Math::Angles3D *const boneAngles = NULL)                                                    const { (*m_engineFunctionTable.pfnGetBonePosition) (entity, boneID, boneOrigin, boneAngles); }
//		inline                                FUNCTION_FROM_NAME        (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnFunctionFromName) (); }
//		inline                                NAME_FOR_FUNCTION         (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnNameForFunction) (); }
		inline const char *const              TraceTexture              (SDK::Classes::Edict *const textureEntity, const Math::Vector3D &source, const Math::Vector3D &destination)                                                                                   const { return (*m_engineFunctionTable.pfnTraceTexture) (textureEntity, source, destination); }
		inline const char                     GetTextureType            (SDK::Classes::Edict *const entity, const Math::Vector3D &source, const Math::Vector3D &destination)                                                                                          const;	// Note: This function declared in 'Half-Life engine/Core.hpp'.
		inline void                           ClientPrint               (SDK::Classes::Edict *const client, const SDK::Constants::PrintType_t printType, const char *const message)                                                                                   const { /*IsDedicatedServer () ? (*m_engineFunctionTable.pfnServerPrint) (message) : */(*m_engineFunctionTable.pfnClientPrintf) (client, printType, message); }
		inline const DynamicString            GetCommandArgumentString  (void)                                                                                                                                                                                        const
		{
			const char *const commandArgumentsBufferPointer ((*m_engineFunctionTable.pfnCmd_Args) ());

			// Reliability check.
//			InternalAssert (!IsNullString (commandArgumentsBufferPointer));	// OCCURS!!!

			return IsNullString (commandArgumentsBufferPointer) ? "" : commandArgumentsBufferPointer;
		}
		virtual inline const DynamicString    GetCommandArgumentString_FromAll  (void)                                                                                                                                                                                        const = 0;
		inline const DynamicString            GetCommandArgument        (const unsigned char index)                                                                                                                                                                   const { return (*m_engineFunctionTable.pfnCmd_Argv) (index); }
		virtual inline const DynamicString    GetCommandArgument_FromAll (const unsigned char index)                                                                                                                                                                   const = 0;
		inline const unsigned char            GetCommandArgumentsCount  (void)                                                                                                                                                                                        const { return static_cast <unsigned char> ((*m_engineFunctionTable.pfnCmd_Argc) ()); }
		virtual inline const unsigned char    GetCommandArgumentsCount_FromAll (void)                                                                                                                                                                                 const = 0;
		inline void                           GetEdictAttachment        (const SDK::Classes::Edict *const entity, const unsigned char attachment, Math::Vector3D &attachmentOrigin, Math::Angles3D *const attachmentAngles = NULL)                                    const { (*m_engineFunctionTable.pfnGetAttachment) (entity, attachment, attachmentOrigin, attachmentAngles); }
		inline void                           SetClientView             (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity)                                                                                                        const { (*m_engineFunctionTable.pfnSetView) (client, viewEntity); }
		inline const float                    GetSystemTime             (void)                                                                                                                                                                                        const { return (*m_engineFunctionTable.pfnTime) (); }
		inline void                           SetClientCrosshairAngles  (const SDK::Classes::Edict *const client, const Math::Angles2D &angles)                                                                                                                       const { (*m_engineFunctionTable.pfnCrosshairAngle) (client, angles.pitch, angles.yaw); }

		inline unsigned char *const           LoadFile                  (const char *const filename, unsigned int *const length = NULL)                                                                                                                               const { return (*m_engineFunctionTable.pfnLoadFileForMe) (filename, length); }
		inline void                           FreeFile                  (unsigned char *const fileBuffer)                                                                                                                                                             const { (*m_engineFunctionTable.pfnFreeFile) (fileBuffer); }
		inline const int                      GetFileSize               (const char *const filename)                                                                                                                                                                  const { return (*m_engineFunctionTable.pfnGetFileSize) (filename); }	// LH: Give access to filesize via filesystem

//		inline                                COMPARE_FILE_TIME         (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnCompareFileTime) (); }
		inline void                           GetGameDirectory          (char *const getGameDirectory)                                                                                                                                                                const { (*m_engineFunctionTable.pfnGetGameDir) (getGameDirectory); }
//		inline                                IS_MAP_VALID              (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnIsMapValid) (); }
		inline SDK::Classes::Edict *const     CreateFakeClient          (const char *const netName)                                                                                                                                                                   const { return (*m_engineFunctionTable.pfnCreateFakeClient) (netName); }
		inline void                           RunPlayerMove             (SDK::Classes::Edict *const fakeClient, const Math::Angles3D &moveAngles, const float forwardMoveSpeed, const float sideMoveSpeed, const float upMoveSpeed, const unsigned short buttons, const SDK::Constants::PlayerImpulseID_t impulse, const unsigned char msecValue) const { (*m_engineFunctionTable.pfnRunPlayerMove) (fakeClient, moveAngles, forwardMoveSpeed, sideMoveSpeed, upMoveSpeed, buttons, static_cast <const unsigned char> (impulse), msecValue); }
		inline const unsigned short           GetNumberOfEntities       (void)                                                                                                                                                                                        const { return static_cast <unsigned short> ((*m_engineFunctionTable.pfnNumberOfEntities) ()); }
//		inline                                PRECACHE_EVENT            (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnPrecacheEvent) (); }
//		inline                                PLAYBACK_EVENT_FULL       (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnPlaybackEvent) (); }
		inline const unsigned char *const     SetPotentiallyVisible     (const Math::Vector3D &origin)                                                                                                                                                                const { return (*m_engineFunctionTable.pfnSetFatPVS) (origin); }
//		inline const unsigned char *const     ENGINE_SET_PAS            (const Math::Vector3D &origin)                                                                                                                                                                const { return (*m_engineFunctionTable.pfnSetFatPAS) (origin); }
		inline const bool                     CheckVisibility           (const SDK::Classes::Edict *const entity, const unsigned char *const pset)                                                                                                                    const { return (*m_engineFunctionTable.pfnCheckVisibility) (entity, pset) > 0; }
//		inline                                DELTA_SET                 (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnDeltaSetField) (); }
//		inline                                DELTA_UNSET               (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnDeltaUnsetField) (); }
//		inline                                DELTA_ADDENCODER          (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnDeltaAddEncoder) (); }
//		inline                                ENGINE_CURRENT_PLAYER     (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnGetCurrentPlayer) (); }
//		inline                                ENGINE_CANSKIP            (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnCanSkipPlayer) (); }
//		inline                                DELTA_FINDFIELD           (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnDeltaFindField) (); }
//		inline                                DELTA_SETBYINDEX          (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnDeltaSetFieldByIndex) (); }
//		inline                                DELTA_UNSETBYINDEX        (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnDeltaUnsetFieldByIndex) (); }
//		inline                                ENGINE_GETPHYSINFO        (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnGetPhysicsInfoString) (); }
//		inline                                ENGINE_SETGROUPMASK       (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnSetGroupMask) (); }
//		inline                                ENGINE_INSTANCE_BASELINE  (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnCreateInstancedBaseline) (); }
//		inline                                ENGINE_FORCE_UNMODIFIED   (void)                                                                                                                                                                                        const { (*m_engineFunctionTable.pfnForceUnmodified) (); }
		inline void                           GetNetInformation         (const SDK::Classes::Edict *const client, int &ping, int &packetLoss)                                                                                                                         const { (*m_engineFunctionTable.pfnGetPlayerStats) (client, ping, packetLoss); }
		inline char *const                    GetEntityInfoKeyBuffer    (const SDK::Classes::Edict *const entity)                                                                                                                                                     const { return (*m_engineFunctionTable.pfnGetInfoKeyBuffer) (entity); }
		inline char *const                    GetInfoKeyValue           (/*const */char *const infoBuffer, const char *const key)                                                                                                                                                 const { return (*m_engineFunctionTable.pfnInfoKeyValue) (infoBuffer, key); }
		inline void                           SetClientKeyValue         (const unsigned char clientIndex, /*const */char *const infoBuffer, const char *const key, const char *const value)                                                                                             const { (*m_engineFunctionTable.pfnSetClientKeyValue) (clientIndex, infoBuffer, key, value); }
		inline void                           RegisterServerCommand     (const char *const commandName, SDK::TypeDefinitions::ConsoleCommandCallbackFunction_t callbackFunction)                                                                                      const { (*m_engineFunctionTable.pfnAddServerCommand) (commandName, callbackFunction); }
		inline void                           ServerPrint               (const char *const message) const
		{
			// Reliability check.
			InternalAssert (!IsNullOrEmptyString (message));
			InternalAssert (GetStringLength <unsigned short> (message) <= SDK::Constants::MaximumServerMessageLength);

			(*m_engineFunctionTable.pfnServerPrint) (message/*FormatBuffer ("[%s] %s", PRODUCT_LOG_TAG, message)*/);
		}
		inline void                           ServerPrintFormat         (const char *const format, ...) const
		{
			char message[SDK::Constants::MaximumServerMessageLength];

			// Concatenate all the arguments in one message....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, message, sizeof (message));

			ServerPrint (message);
		}
		inline void                           SetServerKeyValue         (char *const infoBuffer, const char *const key, const char *const value)                                                                                                                      const { (*m_engineFunctionTable.pfnSetKeyValue) (infoBuffer, key, value); }

		// for voice communications, set which clients hear eachother.
		// NOTE: these functions take player entity indices (starting at 1).
		inline const bool                     GetClientVoiceListening   (const unsigned char receiver, const unsigned char sender)                    const { return (*m_engineFunctionTable.pfnVoice_GetClientListening) (receiver, sender) == TRUE; }
		inline const bool                     SetClientVoiceListening   (const unsigned char receiver, const unsigned char sender, const bool listen) const { return (*m_engineFunctionTable.pfnVoice_SetClientListening) (receiver, sender, listen) == TRUE; }

		// Also, create some nice inlines for engine callback combos.
		inline char *const GetEntityKeyValue    (const SDK::Classes::Edict *const entity, const char *const key)                          const { return GetInfoKeyValue (GetEntityInfoKeyBuffer (entity), key); }	// Get a setinfo value from a player entity.
		inline void        SetEntityKeyValue    (const SDK::Classes::Edict *const entity, const char *const key, const char *const value) const { SetClientKeyValue (static_cast <const unsigned char> (GetEdictIndex (entity)), GetEntityInfoKeyBuffer (entity), key, value); }	// Set a setinfo value for a player entity.
		inline char *const GetServerInformation (const char *const key)                                                                   const { return GetEntityKeyValue (GetEdictOfIndex (SDK::Constants::WorldspawnEntitySlotIndex), key); }	// Get a "serverinfo" value.
		inline void        SetServerInformation (const char *const key, const char *const value)                                          const { SetServerKeyValue (GetEntityInfoKeyBuffer (GetEdictOfIndex (SDK::Constants::WorldspawnEntitySlotIndex)), key, value); }	// Set a "serverinfo" value.
		inline char *const GetLocalInformation  (const char *const key)                                                                   const { return GetEntityKeyValue (NULL, key); }	// Get a "localinfo" value.
		inline void        SetLocalInformation  (const char *const key, const char *const value)                                          const { SetServerKeyValue (GetEntityInfoKeyBuffer (NULL), key, value); }	// Set a "localinfo" value.

	//
	// Group: GAME DLL API functions.
	//
	// Note: This group are declared in 'Meta mod engine/Core.hpp'.
	//
	public:
		inline void              GameInit                 (void)                                                                                                          const { (*m_gameDLLFunctionAPITable.pfnGameInit) (); }	// Initialize/shutdown the game. (one-time call after loading of game.DLL)
		inline const int         Spawn                    (SDK::Classes::Edict *const entity)                                                                             const { return (*m_gameDLLFunctionAPITable.pfnSpawn) (entity); }
		inline void              Think                    (SDK::Classes::Edict *const entity)                                                                             const { (*m_gameDLLFunctionAPITable.pfnThink) (entity); }
		inline void              Use                      (SDK::Classes::Edict *const entityUsed, SDK::Classes::Edict *const entityOther)                                 const { (*m_gameDLLFunctionAPITable.pfnUse) (entityUsed, entityOther); }
		inline void              Touch                    (SDK::Classes::Edict *const entityTouched, SDK::Classes::Edict *const entityOther)                              const { (*m_gameDLLFunctionAPITable.pfnTouch) (entityTouched, entityOther); }
		inline void              Blocked                  (SDK::Classes::Edict *const entityBlocked, SDK::Classes::Edict *const entityOther)                              const { (*m_gameDLLFunctionAPITable.pfnBlocked) (entityBlocked, entityOther); }
		inline void              KeyValue                 (SDK::Classes::Edict *const entityKeyValue, SDK::Structures::KeyValueData_t &keyValueData)                      const { (*m_gameDLLFunctionAPITable.pfnKeyValue) (entityKeyValue, keyValueData); }
		inline void              Save                     (SDK::Classes::Edict *const entity, SDK::Structures::SaveRestoreData_t *const saveData)                         const { (*m_gameDLLFunctionAPITable.pfnSave) (entity, saveData); }
		inline const int         Restore                  (SDK::Classes::Edict *const entity, SDK::Structures::SaveRestoreData_t *const saveData, const int globalEntity) const { return (*m_gameDLLFunctionAPITable.pfnRestore) (entity, saveData, globalEntity); }
		inline void              SetAbsoluteBox           (SDK::Classes::Edict *const entity)                                                                             const { (*m_gameDLLFunctionAPITable.pfnSetAbsBox) (entity); }

		inline void              SaveWriteFields          (SDK::Structures::SaveRestoreData_t *const, const char *const, void *const, SDK::Structures::TypeDescription_t *const, const int) const;
		inline void              SaveReadFields           (SDK::Structures::SaveRestoreData_t *const, const char *const, void *const, SDK::Structures::TypeDescription_t *const, const int) const;

		inline const char        SaveGlobalState          (SDK::Structures::SaveRestoreData_t *const) const;
		inline const char        RestoreGlobalState       (SDK::Structures::SaveRestoreData_t *const) const;
		inline void              ResetGlobalState         (void)                                      const { (*m_gameDLLFunctionAPITable.pfnResetGlobalState) (); }

		inline const bool        ClientConnect            (SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]) const { return (*m_gameDLLFunctionAPITable.pfnClientConnect) (client, name, address, rejectReason) == TRUE; }	// Client has connected.
		inline void              ClientDisconnect         (SDK::Classes::Edict *const client)                                                                             const { (*m_gameDLLFunctionAPITable.pfnClientDisconnect) (client); }	// Player has left the game.
		inline void              ClientKill               (SDK::Classes::Edict *const client)                                                                             const { (*m_gameDLLFunctionAPITable.pfnClientKill) (client); }	// Player has typed "kill".
		inline void              ClientPutInServer        (SDK::Classes::Edict *const client)                                                                             const { (*m_gameDLLFunctionAPITable.pfnClientPutInServer) (client); }	// Client is entering the game.
		inline void              ClientCommand            (SDK::Classes::Edict *const client)                                                                             const { (*m_gameDLLFunctionAPITable.pfnClientCommand) (client); }	// Player has sent a command (typed, or from a bind).
		inline void              ClientUserInfoChanged    (SDK::Classes::Edict *const client, char *const infoBuffer)                                                     const { (*m_gameDLLFunctionAPITable.pfnClientUserInfoChanged) (client, infoBuffer); }	// Client has updated their setinfo structure.

		inline void              ServerActivate           (SDK::Classes::Edict *const entityEdictList, int edictCount, int clientMax) const { (*m_gameDLLFunctionAPITable.pfnServerActivate) (entityEdictList, edictCount, clientMax); }	// Server is starting a new map.
		inline void              ServerDeactivate         (void)                                                                      const { (*m_gameDLLFunctionAPITable.pfnServerDeactivate) (); }	// Server is leaving the map (shutdown, or changelevel).

		inline void              PlayerPreThink           (SDK::Classes::Edict *const player) const { (*m_gameDLLFunctionAPITable.pfnPlayerPreThink) (player); }
		inline void              PlayerPostThink          (SDK::Classes::Edict *const player) const { (*m_gameDLLFunctionAPITable.pfnPlayerPostThink) (player); }

		inline void              StartFrame               (void) const { (*m_gameDLLFunctionAPITable.pfnStartFrame) (); }
		inline void              ParmsNewLevel            (void) const { (*m_gameDLLFunctionAPITable.pfnParmsNewLevel) (); }
		inline void              ParmsChangeLevel         (void) const { (*m_gameDLLFunctionAPITable.pfnParmsChangeLevel) (); }

		inline const char *const GetGameDescription       (void) const { return (*m_gameDLLFunctionAPITable.pfnGetGameDescription) (); }	// Returns string describing current DLL. E.g., "Team Fortress 2", "Half-Life".

		inline void              PlayerCustomization      (SDK::Classes::Edict *const player, SDK::Structures::Customization_t *const customization) const { (*m_gameDLLFunctionAPITable.pfnPlayerCustomization) (player, customization); }	// Notifies game.DLL about a player customization.

		// spectator functions
		inline void              SpectatorConnect         (SDK::Classes::Edict *const spectator) const { (*m_gameDLLFunctionAPITable.pfnSpectatorConnect) (spectator); }	// Called when spectator joins server.
		inline void              SpectatorDisconnect      (SDK::Classes::Edict *const spectator) const { (*m_gameDLLFunctionAPITable.pfnSpectatorDisconnect) (spectator); }	// Called when spectator leaves the server.
		inline void              SpectatorThink           (SDK::Classes::Edict *const spectator) const { (*m_gameDLLFunctionAPITable.pfnSpectatorThink) (spectator); }	// Called when spectator sends a command packet. (UserCommand_t)

		inline void              Sys_Error                (const char *const errorString) const { (*m_gameDLLFunctionAPITable.pfnSys_Error) (errorString); }	// Notify game.DLL that engine is going to shut down. Allows mod authors to set a breakpoint.

		inline void              PM_Move                  (SDK::Structures::PlayerMove_t *const playerMove, const bool server)                                                                                                                                                    const { (*m_gameDLLFunctionAPITable.pfnPM_Move) (playerMove, server); }
		inline void              PM_Init                  (SDK::Structures::PlayerMove_t *const playerMove)                                                                                                                                                                       const { (*m_gameDLLFunctionAPITable.pfnPM_Init) (playerMove); }	// Server version of player movement initialization.
		inline const char        FindTextureType          (const char *const textureName)                                                                                                                                                                                         const { return (*m_gameDLLFunctionAPITable.pfnPM_FindTextureType) (const_cast <char *> (textureName)); }
		inline void              SetupVisibility          (SDK::Classes::Edict *const viewEntity, SDK::Classes::Edict *const client, unsigned char **PVS, unsigned char **PAS)                                                                                                    const { (*m_gameDLLFunctionAPITable.pfnSetupVisibility) (viewEntity, client, PVS, PAS); }	// Set up PVS and PAS for networking for this client.
		inline void              UpdateClientData         (const SDK::Classes::Edict *const entity, const int sendWeapons, SDK::Structures::ClientData_t *const clientData)                                                                                                       const { (*m_gameDLLFunctionAPITable.pfnUpdateClientData) (entity, sendWeapons, clientData); }	// Set up data sent only to specific client.
		inline const int         AddToFullPack            (SDK::Structures::EntityState_t *const state, const int entityIndex, SDK::Classes::Edict *const entity, SDK::Classes::Edict *const host, const int hostFlags, const int player, unsigned char *const set)                    const { return (*m_gameDLLFunctionAPITable.pfnAddToFullPack) (state, entityIndex, entity, host, hostFlags, player, set); }
		inline void              CreateBaseline           (const int player, const int entityIndex, SDK::Structures::EntityState_t *const baseline, SDK::Classes::Edict *const entity, const int playerModelIndex, const Math::Vector3D &playerMins, const Math::Vector3D &playerMaxs) const { (*m_gameDLLFunctionAPITable.pfnCreateBaseline) (player, entityIndex, baseline, entity, playerModelIndex, playerMins, playerMaxs); }	// Tweak entity baseline for network encoding, allows setup of player baselines, too.
		inline void              RegisterEncoders         (void)                                                                                                                                                                                                                  const { (*m_gameDLLFunctionAPITable.pfnRegisterEncoders) (); }	// Callbacks for network encoding.
		inline const int         GetWeaponData            (SDK::Classes::Edict *const player, SDK::Structures::WeaponData_t *const info)                                                                                                                                          const { return (*m_gameDLLFunctionAPITable.pfnGetWeaponData) (player, info); }

		inline void              CmdStart                 (const SDK::Classes::Edict *const player, const SDK::Structures::UserCommand_t *const command, const unsigned int randomSeed) const { (*m_gameDLLFunctionAPITable.pfnCmdStart) (player, command, randomSeed); }
		inline void              CmdEnd                   (const SDK::Classes::Edict *const player)                                                                                     const { (*m_gameDLLFunctionAPITable.pfnCmdEnd) (player); }

		// Return 1 if the packet is valid. Set 'responseBufferSize' if you want to send a response packet. Incoming, it holds the max size of the 'responseBuffer', so you must zero it out if you choose not to respond.
		inline const int         ConnectionlessPacket     (const SDK::Structures::netadr_t *const netFrom, const char *const args, char *const responseBuffer, int *const responseBufferSize) const { return (*m_gameDLLFunctionAPITable.pfnConnectionlessPacket) (netFrom, args, responseBuffer, responseBufferSize); }

		// Enumerates player hulls. Returns 0 if the hull number doesn't exist, 1 otherwise.
		inline const bool        GetHullBounds            (const SDK::Structures::PlayerMove_t::UseHullType_t hullNumber, Math::Vector3D &mins, Math::Vector3D &maxs) const { return (*m_gameDLLFunctionAPITable.pfnGetHullBounds) (hullNumber, mins, maxs) == TRUE; }

		// Create baselines for certain "unplaced" items.
		inline void              CreateInstancedBaselines (void) const { (*m_gameDLLFunctionAPITable.pfnCreateInstancedBaselines) (); }

		// One of the pfnForceUnmodified() files failed the consistency check for the specified player return 0 to allow the client to continue, 1 to force immediate disconnection (with an optional disconnect message of up to 256 characters).
		inline const int         InconsistentFile         (const SDK::Classes::Edict *const player, const char *const filename, char *const disconnectMessage) const { return (*m_gameDLLFunctionAPITable.pfnInconsistentFile) (player, filename, disconnectMessage); }

		// the game.DLL should return 1 if lag compensation should be allowed (could also just set the 'sv_unlag' console variable.
		// Most games right now should return 0, until client-side weapon prediction code is written and tested for them.
		inline const bool        AllowLagCompensation     (void) const { return (*m_gameDLLFunctionAPITable.pfnAllowLagCompensation) () == TRUE; }

	//
	// Group: NEW GAME DLL API functions.
	//
	// Note: This group are declared in 'Meta mod engine/Core.hpp'.
	//
	// Warning: For now in CS all new mod DLL functions is NULL!
	//
	public:
		inline void      OnFreeEntityPrivateData (SDK::Classes::Edict *const entity)                                                                                            const
		{
			// Called right before the object's memory is freed. Calls its destructor.

			// Reliability check.
			InternalAssert (m_newGameDLLFunctionAPITable.pfnOnFreeEntPrivateData != NULL);	/// @warning For now it's occurs!!!

			(*m_newGameDLLFunctionAPITable.pfnOnFreeEntPrivateData) (entity);
		}
		inline void      GameShutdown            (void)                                                                                                                         const
		{
			// Reliability check.
			InternalAssert (m_newGameDLLFunctionAPITable.pfnGameShutdown != NULL);	/// @warning For now it's occurs!!!

			(*m_newGameDLLFunctionAPITable.pfnGameShutdown) ();
		}
		inline const int ShouldCollide           (SDK::Classes::Edict *const entityTouched, SDK::Classes::Edict *const entityOther)                                             const
		{
			// Reliability check.
			InternalAssert (m_newGameDLLFunctionAPITable.pfnShouldCollide != NULL);	/// @warning For now it's occurs!!!

			return (*m_newGameDLLFunctionAPITable.pfnShouldCollide) (entityTouched, entityOther);
		}

		inline void      CvarValue               (const SDK::Classes::Edict *const entity, const char *const value)                                                             const
		{
			// Reliability check.
			InternalAssert (m_newGameDLLFunctionAPITable.pfnCvarValue != NULL);	/// @warning For now it's occurs!!!

			(*m_newGameDLLFunctionAPITable.pfnCvarValue) (entity, value);
		}
		inline void      CvarValue2              (const SDK::Classes::Edict *const entity, const int requestID, const char *const consoleVariableName, const char *const value) const
		{
			// Reliability check.
			InternalAssert (m_newGameDLLFunctionAPITable.pfnCvarValue2 != NULL);	/// @warning For now it's occurs!!!

			(*m_newGameDLLFunctionAPITable.pfnCvarValue2) (entity, requestID, consoleVariableName, value);
		}

	//
	// Group: Other functions.
	//
	public:
		inline void PrecacheStuff (void)
		{
			// This is the function that precaches the stuff we need by the server side, such as the
			// entity model for the entities used in the fake client illumination bug fix, and the sprites
			// used for displaying the waypoints.

			PrecacheSoundSafe (PrecachedStringID_weapons_slash_xbow_hit1_dot_wav);		// waypoint add
			PrecacheSoundSafe (PrecachedStringID_weapons_slash_mine_activate_dot_wav);	// waypoint delete
			PrecacheSoundSafe (PrecachedStringID_debris_slash_bustglass1_dot_wav);		// waypoint error found
			PrecacheSoundSafe (PrecachedStringID_items_slash_medshot4_dot_wav);			// waypoint no error found

//			PrecacheModelSafe (PrecachedStringID_models_slash_mechgibs_dot_mdl);	// invisible model for bot illumination entity (used to create fake client illumination entities)

			m_precachedSpritesIndexes[PrecachedSpriteIndex_Laser] = PrecacheModelSafe (PrecachedStringID_sprites_slash_laserbeam_dot_spr);
			m_precachedSpritesIndexes[PrecachedSpriteIndex_Arrow] = PrecacheModelSafe (PrecachedStringID_sprites_slash_arrow1_dot_spr);
		}

		inline       LibraryHandleWithSize &GetEngineModule (void)       { return m_engineModule; }
		inline const LibraryHandleWithSize &GetEngineModule (void) const { return m_engineModule; }

		inline       LibraryWithSize *const               GetGameLibrary                 (void)       { return m_gameDLL; }
		inline const LibraryWithSize *const               GetGameLibrary                 (void) const { return m_gameDLL; }

		virtual inline const bool                         IsUnderMetaMod                 (void) const =/*>*/ 0;
		inline       Engine_WithMetaMod *const            GetEngineWithMetaModPointer    (void);
		inline const Engine_WithMetaMod *const            GetEngineWithMetaModPointer    (void) const;
		inline       Engine_WithoutMetaMod *const         GetEngineWithoutMetaModPointer (void);
		inline const Engine_WithoutMetaMod *const         GetEngineWithoutMetaModPointer (void) const;

		// Note: This function declared in 'Meta mod engine/Core.hpp'.
		virtual inline void                               CallGameEntity                 (const char *const entityClassName, SDK::Structures::EntityVariables_t &entityVariables) const;

		#define DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION(FunctionName)					\
			virtual inline void HookEngineAPI##FunctionName##Function   (void) =/*>*/ 0;	\
			virtual inline void UnhookEngineAPI##FunctionName##Function (void) =/*>*/ 0;	\

		#define DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION(FunctionName)				\
			virtual inline void HookGameDLLAPI##FunctionName##Function   (void) =/*>*/ 0;	\
			virtual inline void UnhookGameDLLAPI##FunctionName##Function (void) =/*>*/ 0;	\

		// Engine API table....
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (SetModel);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (EmitSound);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (ClientCommand);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (LightStyle);

		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (MessageBegin);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (MessageEnd);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteByte);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteChar);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteShort);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteLong);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteAngle);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteCoord);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteString);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (WriteEntity);

		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (ClientPrintf);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (Cmd_Args);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (Cmd_Argv);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (Cmd_Argc);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION (SetView);

		// Game DLL API table....
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (Touch);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (ClientConnect);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (ClientDisconnect);
///		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (ClientCommand);	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (ClientUserInfoChanged);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (ServerDeactivate);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (StartFrame);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION (UpdateClientData);

		#undef DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION
		#undef DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION

		inline void                                       HookEngineAPIMessageSendingFunctions    (void)
		{
			HookEngineAPIMessageBeginFunction ();
			HookEngineAPIMessageEndFunction   ();
			HookEngineAPIWriteByteFunction    ();
			HookEngineAPIWriteCharFunction    ();
			HookEngineAPIWriteShortFunction   ();
			HookEngineAPIWriteLongFunction    ();
			HookEngineAPIWriteAngleFunction   ();
			HookEngineAPIWriteCoordFunction   ();
			HookEngineAPIWriteStringFunction  ();
			HookEngineAPIWriteEntityFunction  ();
		}
		inline void                                       UnhookEngineAPIMessageSendingFunctions  (void)
		{
			UnhookEngineAPIMessageBeginFunction ();
			UnhookEngineAPIMessageEndFunction   ();
			UnhookEngineAPIWriteByteFunction    ();
			UnhookEngineAPIWriteCharFunction    ();
			UnhookEngineAPIWriteShortFunction   ();
			UnhookEngineAPIWriteLongFunction    ();
			UnhookEngineAPIWriteAngleFunction   ();
			UnhookEngineAPIWriteCoordFunction   ();
			UnhookEngineAPIWriteStringFunction  ();
			UnhookEngineAPIWriteEntityFunction  ();
		}

		inline void                                       HookEngineAPICmdFunctions   (void)
		{
			HookEngineAPICmd_ArgsFunction ();
			HookEngineAPICmd_ArgvFunction ();
			HookEngineAPICmd_ArgcFunction ();
		}
		inline void                                       UnhookEngineAPICmdFunctions (void)
		{
			UnhookEngineAPICmd_ArgsFunction ();
			UnhookEngineAPICmd_ArgvFunction ();
			UnhookEngineAPICmd_ArgcFunction ();
		}
};
/*! @todo !!!
class Engine_Dedicated : public Engine
{
	//
	// Group: (Con/De)structors.
	//
	public:
		Engine_Dedicated (SDK::Structures::EngineFunction_t *const engineFunctionTable, SDK::Structures::GlobalVariables_t *const globalVariables);
};
class Engine_Listen : public Engine
{
	//
	// Group: (Con/De)structors.
	//
	public:
		Engine_Listen (SDK::Structures::EngineFunction_t *const engineFunctionTable, SDK::Structures::GlobalVariables_t *const globalVariables);
};
*/
class Engine_WithMetaMod : public Engine
{
	//
	// Group: Private members.
	//
	private:
		SDK::Structures::EngineFunction_t *m_enginePOSTFunctionTablePointer;		// must be provided by user of this code
		SDK::Structures::DLLFunction_t    *m_gameDLLPOSTFunctionAPITablePointer;	// game.DLL function tables
		SDK::Structures::NewDLLFunction_t *m_newGameDLLPOSTFunctionAPITablePointer;	// new game.DLL function tables

	public:
		SDK::Structures::EngineFunction_t *m_hookedEngineFunctionsPointer;			// Pointer to table of functions which called from original 'mp.DLL', but provided by meta mod. (This pointer we get by 'MetaModEngine::SDK::Structures::MetaModUtilityFunction_t::pfnGetHookTables' function)

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Engine_WithMetaMod (SDK::Structures::EngineFunction_t *const engineFunctionTable, SDK::Structures::GlobalVariables_t *const globalVariables) :
			Engine (engineFunctionTable, globalVariables),

			m_enginePOSTFunctionTablePointer (NULL),
			m_gameDLLPOSTFunctionAPITablePointer (NULL),
			m_newGameDLLPOSTFunctionAPITablePointer (NULL),

			m_hookedEngineFunctionsPointer (NULL)
		{
			// Get engine functions from meta mod....
			m_engineFunctionTablePointer = engineFunctionTable;
		}

	//
	// Group: Callbacks.
	//
	// Description: The actual engine callbacks.
	//
	public:
		inline const DynamicString GetCommandArgumentString_FromAll  (void) const
		{
			const char *const commandArgumentsBufferPointer ((*m_hookedEngineFunctionsPointer->pfnCmd_Args) ());

			// Reliability check.
//			InternalAssert (!IsNullString (commandArgumentsBufferPointer));	// OCCURS!!!

			return IsNullString (commandArgumentsBufferPointer) ? "" : commandArgumentsBufferPointer;
		}
		inline const DynamicString GetCommandArgument_FromAll       (const unsigned char index) const { return (*m_hookedEngineFunctionsPointer->pfnCmd_Argv) (index); }
		inline const unsigned char GetCommandArgumentsCount_FromAll (void)                      const { return static_cast <unsigned char> ((*m_hookedEngineFunctionsPointer->pfnCmd_Argc) ()); }

	//
	// Group: Functions.
	//
	public:
		inline void InitializePrecachedStrings (void)
		{
			// Actual only if under meta mod.

			for (unsigned char precachedStringIndex (0u); precachedStringIndex < PrecachedStringID_Total; ++precachedStringIndex)
				sm_precachedStrings[precachedStringIndex] = AllocateEngineStringAndGetPointer (GetPrecachedString (static_cast <PrecachedStringID_t> (precachedStringIndex)));
		}

		inline const bool IsUnderMetaMod (void) const { return true; }

		inline void SetEnginePREFunctionTablePointer         (SDK::Structures::EngineFunction_t *const enginePREFunctionTablePointer)         { m_engineFunctionTablePointer = enginePREFunctionTablePointer; }
		inline void SetEnginePOSTFunctionTablePointer        (SDK::Structures::EngineFunction_t *const enginePOSTFunctionTablePointer)        { m_enginePOSTFunctionTablePointer = enginePOSTFunctionTablePointer; }
		inline void SetGameDLLPREFunctionAPITablePointer     (SDK::Structures::DLLFunction_t    *const gameDLLPREFunctionAPITablePointer)     { m_gameDLLFunctionAPITablePointer = gameDLLPREFunctionAPITablePointer; }
		inline void SetGameDLLPOSTFunctionAPITablePointer    (SDK::Structures::DLLFunction_t    *const gameDLLPOSTFunctionAPITablePointer)    { m_gameDLLPOSTFunctionAPITablePointer = gameDLLPOSTFunctionAPITablePointer; }
		inline void SetGameDLLPREFunctionNewAPITablePointer  (SDK::Structures::NewDLLFunction_t *const gameDLLPREFunctionNewAPITablePointer)  { m_newGameDLLFunctionAPITablePointer = gameDLLPREFunctionNewAPITablePointer; }
		inline void SetGameDLLPOSTFunctionNewAPITablePointer (SDK::Structures::NewDLLFunction_t *const gameDLLPOSTFunctionNewAPITablePointer) { m_newGameDLLPOSTFunctionAPITablePointer = gameDLLPOSTFunctionNewAPITablePointer; }

		#define DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS(FunctionName, returnType, argumentsDefinitions)									\
			private:																																									\
				returnType (*m_EngineAPI##FunctionName##FunctionPointer) argumentsDefinitions;	/* This pointer will be called as original function from meta mod plugin */				\
																																														\
			public:																																										\
				static inline returnType EngineAPI##FunctionName##FunctionCallbackFromPlugins argumentsDefinitions;	/* Note: This function declared in 'Half-Life engine/Core.hpp'. */	\
																																														\
				inline void HookEngineAPI##FunctionName##Function   (void);	/* Note: This function declared in 'Meta mod engine/Core.hpp'. */											\
				inline void UnhookEngineAPI##FunctionName##Function (void)																												\
				{																																										\
					m_engineFunctionTablePointer->pfn##FunctionName = m_enginePOSTFunctionTablePointer->pfn##FunctionName = NULL;														\
					m_EngineAPI##FunctionName##FunctionPointer = GetEngineFunctionTable ().pfn##FunctionName;																			\
				}																																										\

		#define DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS(FunctionName, returnType, argumentsDefinitions)										\
			private:																																										\
				returnType (*m_GameDLLAPI##FunctionName##FunctionPointer) argumentsDefinitions;																								\
																																															\
			public:																																											\
				static inline returnType GameDLLAPI##FunctionName##FunctionCallbackFromPlugins argumentsDefinitions;	/* Note: This function declared in 'Half-Life engine/Core.hpp'. */	\
																																															\
				inline void HookGameDLLAPI##FunctionName##Function   (void);	/* Note: This function declared in 'Meta mod engine/Core.hpp'. */											\
				inline void UnhookGameDLLAPI##FunctionName##Function (void)																													\
				{																																											\
					m_gameDLLFunctionAPITablePointer->pfn##FunctionName = m_gameDLLPOSTFunctionAPITablePointer->pfn##FunctionName = NULL;													\
					m_GameDLLAPI##FunctionName##FunctionPointer = GetGameDLLFunctionTable ().pfn##FunctionName;																				\
				}																																											\

		// Engine API table....
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (SetModel, void, (SDK::Classes::Edict *entity, const char *modelName));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (EmitSound, void, (SDK::Classes::Edict *entity, SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, SDK::Constants::SoundFlag_t flags, SDK::Constants::SoundPitch_t pitch));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientCommand, void, (SDK::Classes::Edict *client, char *format, ...));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (LightStyle, void, (const int style, char *const value));

		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (MessageBegin, void, (const SDK::Constants::MessageDestination_t destination, const unsigned int type, const Math::Vector3D *const origin, SDK::Classes::Edict *const to));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (MessageEnd, void, (void));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteByte, void, (int value));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteChar, void, (int value));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteShort, void, (int value));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteLong, void, (int value));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteAngle, void, (float value));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteCoord, void, (float value));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteString, void, (const char *const string));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteEntity, void, (int value));

		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientPrintf, void, (SDK::Classes::Edict *client, SDK::Constants::PrintType_t printType, const char *message));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Cmd_Args, const char *, (void));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Cmd_Argv, const char *, (unsigned int argc));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Cmd_Argc, const unsigned int, (void));
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (SetView, void, (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity));

		// Game DLL API table....
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Touch, void, (SDK::Classes::Edict *entityTouched, SDK::Classes::Edict *entityOther));
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientConnect, BOOL, (SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]));
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientDisconnect, void, (SDK::Classes::Edict *client));
///		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientCommand, void, (SDK::Classes::Edict *client));	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientUserInfoChanged, void, (SDK::Classes::Edict *client, char *infoBuffer));
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ServerDeactivate, void, (void));
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (StartFrame, void, (void));
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (UpdateClientData, void, (const SDK::Classes::Edict *entity, int sendWeapons, SDK::Structures::ClientData_t *clientData));

		#undef DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS
		#undef DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS

		inline void SetOriginalEngineFunctionPointer (void)
		{
			// Engine API table....
			m_EngineAPISetModelFunctionPointer = GetEngineFunctionTable ().pfnSetModel;
			m_EngineAPIEmitSoundFunctionPointer = GetEngineFunctionTable ().pfnEmitSound;
			m_EngineAPIClientCommandFunctionPointer = GetEngineFunctionTable ().pfnClientCommand;
			m_EngineAPILightStyleFunctionPointer = GetEngineFunctionTable ().pfnLightStyle;
			m_EngineAPIMessageBeginFunctionPointer = GetEngineFunctionTable ().pfnMessageBegin;
			m_EngineAPIMessageEndFunctionPointer = GetEngineFunctionTable ().pfnMessageEnd;
			m_EngineAPIWriteByteFunctionPointer = GetEngineFunctionTable ().pfnWriteByte;
			m_EngineAPIWriteCharFunctionPointer = GetEngineFunctionTable ().pfnWriteChar;
			m_EngineAPIWriteShortFunctionPointer = GetEngineFunctionTable ().pfnWriteShort;
			m_EngineAPIWriteLongFunctionPointer = GetEngineFunctionTable ().pfnWriteLong;
			m_EngineAPIWriteAngleFunctionPointer = GetEngineFunctionTable ().pfnWriteAngle;
			m_EngineAPIWriteCoordFunctionPointer = GetEngineFunctionTable ().pfnWriteCoord;
			m_EngineAPIWriteStringFunctionPointer = GetEngineFunctionTable ().pfnWriteString;
			m_EngineAPIWriteEntityFunctionPointer = GetEngineFunctionTable ().pfnWriteEntity;
			m_EngineAPIClientPrintfFunctionPointer = GetEngineFunctionTable ().pfnClientPrintf;
			m_EngineAPICmd_ArgsFunctionPointer = GetEngineFunctionTable ().pfnCmd_Args;
			m_EngineAPICmd_ArgvFunctionPointer = GetEngineFunctionTable ().pfnCmd_Argv;
			m_EngineAPICmd_ArgcFunctionPointer = GetEngineFunctionTable ().pfnCmd_Argc;
			m_EngineAPISetViewFunctionPointer = GetEngineFunctionTable ().pfnSetView;

		}
		inline void SetOriginalDLLAPIFunctionPointers (void)
		{
			// Game DLL API table....
			m_GameDLLAPITouchFunctionPointer = GetGameDLLFunctionTable ().pfnTouch;
			m_GameDLLAPIClientConnectFunctionPointer = GetGameDLLFunctionTable ().pfnClientConnect;
			m_GameDLLAPIClientDisconnectFunctionPointer = GetGameDLLFunctionTable ().pfnClientDisconnect;
///			m_GameDLLAPIClientCommandFunctionPointer = GetGameDLLFunctionTable ().pfnClientCommand;	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
			m_GameDLLAPIClientUserInfoChangedFunctionPointer = GetGameDLLFunctionTable ().pfnClientUserInfoChanged;
			m_GameDLLAPIServerDeactivateFunctionPointer = GetGameDLLFunctionTable ().pfnServerDeactivate;
			m_GameDLLAPIStartFrameFunctionPointer = GetGameDLLFunctionTable ().pfnStartFrame;
			m_GameDLLAPIUpdateClientDataFunctionPointer = GetGameDLLFunctionTable ().pfnUpdateClientData;
		}
		inline void SetOriginalFunctionPointers (void)
		{
			SetOriginalEngineFunctionPointer ();
			SetOriginalDLLAPIFunctionPointers ();
		}
};

inline       Engine_WithMetaMod *const Engine::GetEngineWithMetaModPointer (void)
{
	// Reliability check.
	InternalAssert (IsUnderMetaMod ());

	return static_cast <Engine_WithMetaMod *const> (this);
}
inline const Engine_WithMetaMod *const Engine::GetEngineWithMetaModPointer (void) const
{
	// Reliability check.
	InternalAssert (IsUnderMetaMod ());

	return static_cast <const Engine_WithMetaMod *const> (this);
}

class Engine_WithoutMetaMod : public Engine
{
	//
	// Group: (Con/De)structors.
	//
	public:
		Engine_WithoutMetaMod (SDK::Structures::EngineFunction_t *const engineFunctionTable, SDK::Structures::GlobalVariables_t *const globalVariables);

	//
	// Group: Callbacks.
	//
	// Description: The actual engine callbacks.
	//
	public:
		inline const DynamicString GetCommandArgumentString_FromAll  (void) const
		{
			const char *const commandArgumentsBufferPointer ((*GetEngineFunctionTablePointer ()->pfnCmd_Args) ());

			// Reliability check.
//			InternalAssert (!IsNullString (commandArgumentsBufferPointer));	// OCCURS!!!

			return IsNullString (commandArgumentsBufferPointer) ? "" : commandArgumentsBufferPointer;
		}
		inline const DynamicString GetCommandArgument_FromAll       (const unsigned char index) const { return (*GetEngineFunctionTablePointer ()->pfnCmd_Argv) (index); }
		inline const unsigned char GetCommandArgumentsCount_FromAll (void)                      const { return static_cast <unsigned char> ((*GetEngineFunctionTablePointer ()->pfnCmd_Argc) ()); }

	//
	// Group: Functions.
	//
	public:
		inline const bool                         IsUnderMetaMod                 (void) const { return false; }

		inline void                               CallGameEntity                 (const char *const entityClassName, SDK::Structures::EntityVariables_t &entityVariables) const
		{
/*! NEED TO INVOLVE THIS! AS MACRO?			static const SDK::TypeDefinitions::EntityFunctionPointer_t &entityFunction (m_gameDLL->GetFunctionAddress <SDK::TypeDefinitions::EntityFunctionPointer_t> (entityClassName));

			// Call the entity function....
			(*entityFunction) (&entityVariables);*/
			(*m_gameDLL->GetFunctionAddress <SDK::TypeDefinitions::EntityFunctionPointer_t> (entityClassName)) (&entityVariables);
		}

		#define DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD(FunctionName)										\
			inline void HookEngineAPI##FunctionName##Function   (void)															\
			{																													\
				m_engineFunctionTablePointer->pfn##FunctionName = Export::ReplacedFunctions::EngineTable::pfn##FunctionName;	\
			}																													\
			inline void UnhookEngineAPI##FunctionName##Function (void)															\
			{																													\
				m_engineFunctionTablePointer->pfn##FunctionName = m_engineFunctionTable.pfn##FunctionName;						\
			}																													\

		#define DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD(FunctionName)								\
			inline void HookGameDLLAPI##FunctionName##Function   (void)														\
			{																												\
				m_gameDLLFunctionAPITablePointer->pfn##FunctionName = Export::ReplacedFunctions::DLLTable::FunctionName;	\
			}																												\
			inline void UnhookGameDLLAPI##FunctionName##Function (void)														\
			{																												\
				m_gameDLLFunctionAPITablePointer->pfn##FunctionName = m_gameDLLFunctionAPITable.pfn##FunctionName;			\
			}																												\

		// Engine API table....
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (SetModel);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (EmitSound);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (ClientCommand);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (LightStyle);

		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (MessageBegin);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (MessageEnd);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteByte);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteChar);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteShort);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteLong);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteAngle);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteCoord);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteString);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (WriteEntity);

		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (ClientPrintf);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (Cmd_Args);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (Cmd_Argv);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (Cmd_Argc);
		DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD (SetView);

		// Game DLL API table....
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (Touch);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (ClientConnect);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (ClientDisconnect);
///		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (ClientCommand);	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (ClientUserInfoChanged);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (ServerDeactivate);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (StartFrame);
		DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD (UpdateClientData);

		#undef DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITHOUT_METAMOD
		#undef DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITHOUT_METAMOD
};

inline       Engine_WithoutMetaMod *const Engine::GetEngineWithoutMetaModPointer (void)
{
	// Reliability check.
	InternalAssert (!IsUnderMetaMod ());

	return static_cast <Engine_WithoutMetaMod *const> (this);
}
inline const Engine_WithoutMetaMod *const Engine::GetEngineWithoutMetaModPointer (void) const
{
	// Reliability check.
	InternalAssert (!IsUnderMetaMod ());

	return static_cast <const Engine_WithoutMetaMod *const> (this);
}

#endif	// ifndef HALF_LIFE_ENGINE_INCLUDED