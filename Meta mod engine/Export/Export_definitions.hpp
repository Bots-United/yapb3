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
// Export_definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef META_MOD_ENGINE_EXPORT_EXPORT_DEFINITIONS_INCLUDED
#define META_MOD_ENGINE_EXPORT_EXPORT_DEFINITIONS_INCLUDED

namespace ReplacedFunctions
{
	namespace DLLTable
	{
		inline void GameDLLInit           (void);
		inline int  Spawn                 (HalfLifeEngine::SDK::Classes::Edict *entity);
		inline void Touch                 (HalfLifeEngine::SDK::Classes::Edict *entityTouched, HalfLifeEngine::SDK::Classes::Edict *entityOther);
		inline BOOL ClientConnect         (HalfLifeEngine::SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]);
		inline void ClientDisconnect      (HalfLifeEngine::SDK::Classes::Edict *client);
		inline void ClientCommand         (HalfLifeEngine::SDK::Classes::Edict *client);
		inline void ClientUserInfoChanged (HalfLifeEngine::SDK::Classes::Edict *client, char *infoBuffer);
		inline void ServerActivate        (HalfLifeEngine::SDK::Classes::Edict *pentEdictList, int edictCount, int clientMax);
		inline void ServerDeactivate      (void);
		inline void StartFrame            (void);
		inline void PM_Move               (HalfLifeEngine::SDK::Structures::PlayerMove_t *playerMove, BOOL server);
		inline void PM_Init               (HalfLifeEngine::SDK::Structures::PlayerMove_t *playerMove);
		inline void UpdateClientData      (const HalfLifeEngine::SDK::Classes::Edict *entity, int sendWeapons, HalfLifeEngine::SDK::Structures::ClientData_t *clientData);

		namespace Post
		{
			inline int  Spawn          (HalfLifeEngine::SDK::Classes::Edict *entity);
			inline void ServerActivate (HalfLifeEngine::SDK::Classes::Edict *pentEdictList, int edictCount, int clientMax);
			inline void CmdStart       (const HalfLifeEngine::SDK::Classes::Edict *const player, const HalfLifeEngine::SDK::Structures::UserCommand_t *const command, const unsigned int randomSeed);
		}
	}

	namespace EngineTable
	{
		inline void                pfnEmitSound     (HalfLifeEngine::SDK::Classes::Edict *entity, HalfLifeEngine::SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, HalfLifeEngine::SDK::Constants::SoundFlag_t flags, HalfLifeEngine::SDK::Constants::SoundPitch_t pitch);
		inline void                pfnClientCommand (HalfLifeEngine::SDK::Classes::Edict *client, char *format, ...);
		inline int                 pfnRegUserMsg    (const char *name, int size);
		inline void                pfnClientPrintf  (HalfLifeEngine::SDK::Classes::Edict *client, HalfLifeEngine::SDK::Constants::PrintType_t printType, const char *message);
		inline const char         *pfnCmd_Args      (void);
		inline const char         *pfnCmd_Argv      (unsigned int argc);
		inline const unsigned int  pfnCmd_Argc      (void);

		namespace Post
		{
			inline void pfnSetModel     (HalfLifeEngine::SDK::Classes::Edict *entity, const char *modelName);
			inline void pfnLightStyle   (const int style, char *const value);
			inline void pfnMessageBegin (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const unsigned int type, const Math::Vector3D *const origin, HalfLifeEngine::SDK::Classes::Edict *const to);
			inline void pfnMessageEnd   (void);
			inline void pfnWriteByte    (int value);
			inline void pfnWriteChar    (int value);
			inline void pfnWriteShort   (int value);
			inline void pfnWriteLong    (int value);
			inline void pfnWriteAngle   (float value);
			inline void pfnWriteCoord   (float value);
			inline void pfnWriteString  (const char *const string);
			inline void pfnWriteEntity  (int value);
			inline void pfnSetView      (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const viewEntity);
		}
	}
}

#include <Meta mod engine/Export/Meta_mod_engine_functions.hpp>

#endif	// ifndef META_MOD_ENGINE_EXPORT_EXPORT_DEFINITIONS_INCLUDED