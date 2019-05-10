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
// DLL_function.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef DLL_FUNCTION_INCLUDED
#define DLL_FUNCTION_INCLUDED

struct DLLFunction_t
{
	void        (*pfnGameInit)                 (void);	// Initialize/shutdown the game. (one-time call after loading of game.DLL)
	int         (*pfnSpawn)                    (Classes::Edict *const entity);
	void        (*pfnThink)                    (Classes::Edict *const entity);
	void        (*pfnUse)                      (Classes::Edict *const entityUsed, Classes::Edict *const entityOther);
	void        (*pfnTouch)                    (Classes::Edict *const entityTouched, Classes::Edict *const entityOther);
	void        (*pfnBlocked)                  (Classes::Edict *const entityBlocked, Classes::Edict *const entityOther);
	void        (*pfnKeyValue)                 (Classes::Edict *const entityKeyValue, KeyValueData_t &keyValueData);
	void        (*pfnSave)                     (Classes::Edict *const entity, SaveRestoreData_t *const saveData);
	int         (*pfnRestore)                  (Classes::Edict *const entity, SaveRestoreData_t *const saveData, int globalEntity);
	void        (*pfnSetAbsBox)                (Classes::Edict *const entity);

	void        (*pfnSaveWriteFields)          (SaveRestoreData_t *const, const char *const, void *const, TypeDescription_t *const, int);
	void        (*pfnSaveReadFields)           (SaveRestoreData_t *const, const char *const, void *const, TypeDescription_t *const, int);

	void        (*pfnSaveGlobalState)          (SaveRestoreData_t *const);
	void        (*pfnRestoreGlobalState)       (SaveRestoreData_t *const);
	void        (*pfnResetGlobalState)         (void);

	BOOL        (*pfnClientConnect)            (Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]);	// Client has connected.
	void        (*pfnClientDisconnect)         (Classes::Edict *const client);	// Player has left the game.
	void        (*pfnClientKill)               (Classes::Edict *const client);	// Player has typed "kill".
	void        (*pfnClientPutInServer)        (Classes::Edict *const client);	// Client is entering the game.
	void        (*pfnClientCommand)            (Classes::Edict *const client);	// Player has sent a command (typed, or from a bind).
	void        (*pfnClientUserInfoChanged)    (Classes::Edict *const client, char *const infoBuffer);	// Client has updated their setinfo structure.

	void        (*pfnServerActivate)           (Classes::Edict *const entityEdictList, int edictCount, int clientMax);	// Server is starting a new map.
	void        (*pfnServerDeactivate)         (void);	// Server is leaving the map (shutdown, or changelevel).

	void        (*pfnPlayerPreThink)           (Classes::Edict *const player);
	void        (*pfnPlayerPostThink)          (Classes::Edict *const player);

	void        (*pfnStartFrame)               (void);
	void        (*pfnParmsNewLevel)            (void);
	void        (*pfnParmsChangeLevel)         (void);

	const char *(*pfnGetGameDescription)       (void);	// Returns string describing current DLL. E.g., "Team Fortress 2", "Half-Life".

	void        (*pfnPlayerCustomization)      (Classes::Edict *const player, Customization_t *const customization);	// Notifies game.DLL about a player customization.

	// spectator functions
	void        (*pfnSpectatorConnect)         (Classes::Edict *const spectator);	// Called when spectator joins server.
	void        (*pfnSpectatorDisconnect)      (Classes::Edict *const spectator);	// Called when spectator leaves the server.
	void        (*pfnSpectatorThink)           (Classes::Edict *const spectator);	// Called when spectator sends a command packet. (UserCommand_t)

	void        (*pfnSys_Error)                (const char *const errorString);	// Notify game.DLL that engine is going to shut down. Allows mod authors to set a breakpoint.

	void        (*pfnPM_Move)                  (PlayerMove_t *const playerMove, BOOL server);
	void        (*pfnPM_Init)                  (PlayerMove_t *const playerMove);	// Server version of player movement initialization.
	char        (*pfnPM_FindTextureType)       (const char *const textureName);
	void        (*pfnSetupVisibility)          (Classes::Edict *const viewEntity, Classes::Edict *const client, unsigned char **PVS, unsigned char **PAS);	// Set up PVS and PAS for networking for this client.
	void        (*pfnUpdateClientData)         (const Classes::Edict *const entity, int sendWeapons, ClientData_t *clientData);	// Set up data sent only to specific client.
	BOOL        (*pfnAddToFullPack)            (EntityState_t *const state, int entityIndex, Classes::Edict *const entity, Classes::Edict *const host, int hostFlags, int player, unsigned char *set);
	void        (*pfnCreateBaseline)           (int player, int entityIndex, EntityState_t *const baseline, Classes::Edict *const entity, int playerModelIndex, Math::Vector3D playerMins, Math::Vector3D playerMaxs);	// Tweak entity baseline for network encoding, allows setup of player baselines, too.
	void        (*pfnRegisterEncoders)         (void);	// Callbacks for network encoding.
	int         (*pfnGetWeaponData)            (Classes::Edict *const player, WeaponData_t *const info);

	void        (*pfnCmdStart)                 (const Classes::Edict *const player, const UserCommand_t *const command, const unsigned int randomSeed);
	void        (*pfnCmdEnd)                   (const Classes::Edict *const player);

	// Return 1 if the packet is valid. Set 'responseBufferSize' if you want to send a response packet. Incoming, it holds the max size of the 'responseBuffer', so you must zero it out if you choose not to respond.
	BOOL        (*pfnConnectionlessPacket)     (const netadr_t *const netFrom, const char *const args, char *responseBuffer, int *responseBufferSize);

	// Enumerates player hulls. Returns 0 if the hull number doesn't exist, 1 otherwise.
	BOOL        (*pfnGetHullBounds)            (PlayerMove_t::UseHullType_t hullNumber, Math::Vector3D &mins, Math::Vector3D &maxs);

	// Create baselines for certain "unplaced" items.
	void        (*pfnCreateInstancedBaselines) (void);

	// One of the pfnForceUnmodified() files failed the consistency check for the specified player return 0 to allow the client to continue, 1 to force immediate disconnection (with an optional disconnect message of up to 256 characters).
	BOOL        (*pfnInconsistentFile)         (const Classes::Edict *const player, const char *const filename, char *disconnectMessage);

	// The game.DLL should return 1 if lag compensation should be allowed (could also just set the 'sv_unlag' console variable.
	// Most games right now should return 0, until client-side weapon prediction code is written and tested for them.
	BOOL        (*pfnAllowLagCompensation)     (void);
};

#endif	// ifndef DLL_FUNCTION_INCLUDED