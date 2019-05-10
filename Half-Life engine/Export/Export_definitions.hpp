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

#ifndef EXPORT_DEFINITIONS_INCLUDED
#define EXPORT_DEFINITIONS_INCLUDED

namespace Callbacks
{
	namespace DLLTable
	{
		void GameDLLInit           (void);
		void EntitySpawn           (SDK::Classes::Edict *const entity);
		void ClientConnect         (SDK::Classes::Edict *const client, const char *const name, const char *const address, const char rejectReason[128u]);
		void ClientDisconnect      (SDK::Classes::Edict *const client);
		void ClientPutInServer     (SDK::Classes::Edict *const client);
		void ClientCommand         (SDK::Classes::Edict *const clientEdict);
		void ClientUserInfoChanged (SDK::Classes::Edict *const client, char *const infoBuffer);
		void ServerActivate        (void);
		void ServerDeactivate      (void);
		void StartFrame            (void);
		void PM_Move               (SDK::Structures::PlayerMove_t *const playerMove, const bool server);
		void PM_Init               (SDK::Structures::PlayerMove_t *const playerMove);
		void UpdateClientData      (const SDK::Classes::Edict *const entity);

		namespace Post
		{
			void EntitySpawn    (SDK::Classes::Edict *const entity);
			void ServerActivate (void);
		}
	}

	namespace EngineTable
	{
		void                 ChangeLevel         (const char *const s1, const char *const s2);
		void                 EmitSound           (const SDK::Classes::Edict *const entity, const SDK::Constants::SoundChannel_t channel, const char *const sample, const float volume, const float attenuation, const SDK::Constants::SoundFlag_t flags, const SDK::Constants::SoundPitch_t pitch);
		void                 ClientCommand       (SDK::Classes::Edict *const client, const char *const command);
		const unsigned char  RegisterUserMessage (const char *const name, const short size);
		void                 ClientPrintf        (SDK::Classes::Edict *const client, const SDK::Constants::PrintType_t printType, const char *const message);
		const char *const    Cmd_Args            (void);
		const char *const    Cmd_Argv            (const unsigned char argc);
		const unsigned char  Cmd_Argc            (void);

		namespace Post
		{
			void SetEntityModel (SDK::Classes::Edict *const entity, const char *const modelName);
			void LightStyle     (const int style, char *const value);
			void MessageBegin   (const SDK::Constants::MessageDestination_t destination, const unsigned int id, const Math::Vector3D *const origin, SDK::Classes::Edict *const to);
			void MessageEnd     (void);
			void WriteByte      (const int value);
			void WriteChar      (const int value);
			void WriteShort     (const int value);
			void WriteLong      (const int value);
			void WriteAngle     (const float value);
			void WriteCoord     (const float value);
			void WriteString    (const char *const string);
			void WriteEntity    (const int value);
			void SetView        (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity);
		}
	}
}

namespace ReplacedFunctions
{
	#include <Half-Life engine/Export/Replaced functions/Replaced_functions_definitions.hpp>
}

#include <Half-Life engine/Export/Functions.hpp>

#endif	// ifndef EXPORT_DEFINITIONS_INCLUDED