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
// DLL_API.hpp
//
// Description: Prototypes for Half-Life DLL API routines.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef DLL_API_INCLUDED
#define DLL_API_INCLUDED

void GameDLLInit           (void);
int  Spawn                 (SDK::Classes::Edict *entity);
void Touch                 (SDK::Classes::Edict *entityTouched, SDK::Classes::Edict *entityOther);
BOOL ClientConnect         (SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]);
void ClientDisconnect      (SDK::Classes::Edict *client);
void ClientPutInServer     (SDK::Classes::Edict *const client);
void ClientCommand         (SDK::Classes::Edict *client);
void ClientUserInfoChanged (SDK::Classes::Edict *client, char *infoBuffer);
void ServerActivate        (SDK::Classes::Edict *pentEdictList, int edictCount, int clientMax);
void ServerDeactivate      (void);
void StartFrame            (void);
void PM_Move               (SDK::Structures::PlayerMove_t *playerMove, BOOL server);
void PM_Init               (SDK::Structures::PlayerMove_t *playerMove);
void UpdateClientData      (const SDK::Classes::Edict *entity, int sendWeapons, SDK::Structures::ClientData_t *clientData);
void CmdStart              (const SDK::Classes::Edict *const player, const SDK::Structures::UserCommand_t *const command, unsigned int randomSeed);

#endif	// ifndef DLL_API_INCLUDED