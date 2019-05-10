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

#ifndef META_MOD_ENGINE_EXPORT_REPLACED_FUNCTIONS_DLL_API_DLL_API_INCLUDED
#define META_MOD_ENGINE_EXPORT_REPLACED_FUNCTIONS_DLL_API_DLL_API_INCLUDED

inline void GameDLLInit           (void)
{
	// this function is a one-time call, and appears to be the second function called in the
	// DLL after FunctionPointer_t() has been called. Its purpose is to tell the MOD DLL to
	// initialize the game before the engine actually hooks into it with its video frames and
	// clients connecting. Note that it is a different step than the *server* initialization.
	// This one is called once, and only once, when the game process boots up before the first
	// server is enabled. Here is a good place to do our own game session initialization, and
	// to register by the engine side the server commands we need to administrate our bots.

	HalfLifeEngine::Export::Callbacks::DLLTable::GameDLLInit ();

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline int  Spawn                 (HalfLifeEngine::SDK::Classes::Edict *entity)
{
	// this function asks the game.DLL to spawn (i.e, give a physical existence in the virtual world, in other words to 'display') the entity pointed to by 'entity' in the game.
	// The Spawn() function is one of the functions any entity is supposed to have in the game.DLL, and any MOD is supposed to implement one for each of its entities.

	HalfLifeEngine::Export::Callbacks::DLLTable::EntitySpawn (entity);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;

	return 0;
}
inline void Touch                 (HalfLifeEngine::SDK::Classes::Edict * /*entityTouched*/, HalfLifeEngine::SDK::Classes::Edict * /*entityOther*/)
{
	// this function is called when two entities' bounding boxes enter in collision. For example,
	// when a player walks upon a gun, the player entity bounding box collides to the gun entity
	// bounding box, and the result is that this function is called. It is used by the game for
	// taking the appropriate action when such an event occurs (in our example, the player who
	// is walking upon the gun will "pick it up"). Entities that "touch" others are usually
	// entities having a velocity, as it is assumed that static entities (entities that don't
	// move) will never touch anything. Hence, in our example, the 'entityTouched' will be the gun
	// (static entity), whereas the 'entityOther' will be the player (as it is the one moving). When
	// the two entities both have velocities, for example two players colliding, this function
	// is called twice, once for each entity moving.

//	HalfLifeEngine::Export::Callbacks::DLLTable::Touch (entityTouched, entityOther);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline BOOL ClientConnect         (HalfLifeEngine::SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u])
{
	// this function is called in order to tell the MOD DLL that a client attempts to connect the game.
	// The entity pointer of this client is 'client', the name under which he connects is
	// pointed to by the 'name' pointer, and it's IP address string is pointed by the 'address'
	// one. Note that this does not mean this client will actually join the game; he could as
	// well be refused connection by the server later, because of latency timeout, unavailable
	// game resources, or whatever reason. In which case the reason why the game.DLL (read well,
	// the game.DLL, *NOT* the engine) refuses this player to connect will be printed in the
	// 'rejectReason' string in all letters. Understand that a client connecting process is done
	// in three steps. First, the client requests a connection from the server. This is engine internals.
	// When there are already too many players, the engine will refuse this client to
	// connect, and the game.DLL won't even notice. Second, if the engine sees no problem, the
	// game.DLL is asked. This is where we are. Once the game.DLL acknowledges the connection,
	// the client downloads the resources it needs and synchronizes its local engine with the one
	// of the server. And then, the third step, which comes *AFTER* ClientConnect(), is when the
	// client officially enters the game, through the ClientPutInServer() function, later below.
	// Here we hook this function in order to keep track of the listen server client entity,
	// because a listen server client always connects with a "loopback" address string. Also we
	// tell the bot manager to check the bot population, in order to always have one free slot on
	// the server for incoming clients.

	HalfLifeEngine::Export::Callbacks::DLLTable::ClientConnect (client, name, address, rejectReason);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;

	return FALSE;
}
inline void ClientDisconnect      (HalfLifeEngine::SDK::Classes::Edict *client)
{
	// this function is called whenever a client is VOLUNTARILY disconnected from the server,
	// either because the client dropped the connection, or because the server dropped him from
	// the game (latency timeout). The effect is the freeing of a client slot on the server.
	// Note that clients and bots disconnected because of a level change NOT NECESSARILY call this
	// function, because in case of a level change, it's a server shutdown, and not a normal disconnection.
	// I find that completely stupid, but that's it. Anyway it's time to update
	// the bots and players counts, and in case the client disconnecting is a bot, to back its
	// brain(s) up to disk. We also try to notice when a listen server client disconnects, so as
	// to reset his entity pointer for safety. There are still a few server frames to go once a
	// listen server client disconnects, and we don't want to send him any sort of message then.

	HalfLifeEngine::Export::Callbacks::DLLTable::ClientDisconnect (client);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void ClientCommand         (HalfLifeEngine::SDK::Classes::Edict *client)
{
	// this function is called whenever the client whose player entity is 'client' issues a client
	// command. How it works is that clients all have a global string in their client DLL that
	// stores the command string; if ever that string is filled with characters, the client DLL
	// sends it to the engine as a command to be executed. When the engine has executed that
	// command, that string is reset to zero. By the server side, we can access this string
	// by asking the engine with the CmdArgv(), CmdArgs() and CmdArgc() functions that work just
	// like executable files argument processing work in C (argc gets the number of arguments,
	// command included, args returns the whole string, and argv returns the wanted argument
	// only). Here is a good place to set up either bot debug commands the listen server client
	// could type in his game console, or real new client commands, but we wouldn't want to do
	// so as this is just a bot DLL, not a MOD. The purpose is not to add functionality to
	// clients. Hence it can lack of commenting a bit, since this code is very subject to change.

	HalfLifeEngine::Export::Callbacks::DLLTable::ClientCommand (client);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void ClientUserInfoChanged (HalfLifeEngine::SDK::Classes::Edict *client, char *infoBuffer)
{
	// this function is called when a player changes model, or changes team. Occasionally it
	// enforces rules on these changes (for example, some MODs don't want to allow players to
	// change their player model). But most commonly, this function is in charge of handling
	// team changes, recounting the teams population, etc...

	HalfLifeEngine::Export::Callbacks::DLLTable::ClientUserInfoChanged (client, infoBuffer);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void ServerActivate        (HalfLifeEngine::SDK::Classes::Edict * /*entityEdictList*/, int/* edictCount*/, int/* clientMax*/)
{
	// this function is called when the server has fully loaded and is about to manifest itself
	// on the network as such. Since a mapchange is actually a server shutdown followed by a
	// restart, this function is also called when a new map is being loaded. Hence it's the
	// perfect place for doing initialization stuff for our bots, such as reading the BSP data,
	// loading the bot profiles, and drawing the world map (i.e., filling the navigation hash table).
	// Once this function has been called, the server can be considered as "running".

	HalfLifeEngine::Export::Callbacks::DLLTable::ServerActivate ();

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void ServerDeactivate      (void)
{
	// this function is called when the server is shutting down. A particular note about map
	// changes: changing the map means shutting down the server and starting a new one. Of course
	// this process is transparent to the user, but either in single player when the hero reaches
	// a new level and in multiplayer when it's time for a map change, be aware that what happens
	// is that the server actually shuts down and restarts with a new map. Hence we can use this
	// function to free and deinit anything which is map-specific, for example we free the memory
	// space we m'allocated for our BSP data, since a new map means new BSP data to interpret. In
	// any case, when the new map will be booting, ServerActivate() will be called, so we'll do
	// the loading of new bots and the new BSP data parsing there.

	HalfLifeEngine::Export::Callbacks::DLLTable::ServerDeactivate ();

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void StartFrame            (void)
{
	// this function starts a video frame. It is called once per video frame by the engine. If
	// you run Half-Life at 90 fps, this function will then be called 90 times per second. By
	// placing a hook on it, we have a good place to do things that should be done continuously
	// during the game, for example making the bots think (yes, because no Think() function exists
	// for the bots by the MOD side, remember). Also here we have control on the bot population,
	// for example if a new player joins the server, we should disconnect a bot, and if the
	// player population decreases, we should fill the server with other bots.

	HalfLifeEngine::Export::Callbacks::DLLTable::StartFrame ();

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void PM_Move (HalfLifeEngine::SDK::Structures::PlayerMove_t *playerMove, BOOL server)
{
	// this is the player movement code clients run to predict things when the server can't update
	// them often enough (or doesn't want to). The server runs exactly the same function for
	// moving players. There is normally no distinction between them, else client-side prediction
	// wouldn't work properly (and it doesn't work that well, already...)

	/// @note This is ugly & fast crash fix. When we reloaded our module,
	/// original PM_Init() function is not called for us, so we use NULL pointer to 'PlayerMove_t' structure!
	/// This means when we use HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerMovePointer() we produce a segfault.
	if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerMovePointer () == NULL)
		HalfLifeEngine::Export::Callbacks::DLLTable::PM_Init (playerMove);

	HalfLifeEngine::Export::Callbacks::DLLTable::PM_Move (playerMove, server == TRUE);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void PM_Init (HalfLifeEngine::SDK::Structures::PlayerMove_t *playerMove)
{
	// I sure as hell don't know what this one is up to. I suspect it's one function that must be
	// called before using PM_Move() on a PlayerMove_t structure. Probably for initializing data.
	// If anyone has info...

	HalfLifeEngine::Export::Callbacks::DLLTable::PM_Init (playerMove);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}
inline void UpdateClientData      (const HalfLifeEngine::SDK::Classes::Edict *entity, int/* sendWeapons*/, HalfLifeEngine::SDK::Structures::ClientData_t * /*clientData*/)
{
	// This function is a synchronization tool that is used periodically by the engine to tell
	// the game DLL to send player info over the network to one of its clients when it suspects
	// that this client is desynchronizing. Early bots were using it to ask the game DLL for the
	// weapon list of players (by setting 'sendWeapons' to TRUE), but most of the time having a
	// look around the entity->variables.weapons bitmask is enough, since that's the place commonly used for
	// MODs to store weapon information. If it can't be read from there, catching a few network
	// messages (like in DMC) do the job better than this function anyway.

	HalfLifeEngine::Export::Callbacks::DLLTable::UpdateClientData (entity);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}

namespace Post
{
	inline int  Spawn          (HalfLifeEngine::SDK::Classes::Edict *entity)
	{
		// this function asks the game.DLL to spawn (i.e, give a physical existence in the virtual
		// world, in other words to 'display') the entity pointed to by 'entity' in the game.
		// The Spawn() function is one of the functions any entity is supposed to have in the game DLL,
		// and any MOD is supposed to implement one for each of its entities.
		// Post version called only by metamod.

		HalfLifeEngine::Export::Callbacks::DLLTable::Post::EntitySpawn (entity);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;

		return 0;
	}

	inline void ServerActivate (HalfLifeEngine::SDK::Classes::Edict * /*entityEdictList*/, int/* edictCount*/, int/* clientMax*/)
	{
		// this function is called when the server has fully loaded and is about to manifest itself
		// on the network as such. Since a mapchange is actually a server shutdown followed by a
		// restart, this function is also called when a new map is being loaded. Hence it's the
		// perfect place for doing initialization stuff for our bots, such as reading the BSP data,
		// loading the bot profiles, and drawing the world map (ie, filling the navigation hashtable).
		// Once this function has been called, the server can be considered as "running".
		// Post version called only by metamod.

		HalfLifeEngine::Export::Callbacks::DLLTable::Post::ServerActivate ();

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void CmdStart (const HalfLifeEngine::SDK::Classes::Edict *const player, const HalfLifeEngine::SDK::Structures::UserCommand_t *const command, const unsigned int randomSeed)
	{
		// some MODs don't feel like doing like everybody else. It's the case in DMC, where players
		// don't select their weapons using a simple client command, but have to use an horrible
		// datagram like this. CmdStart() marks the start of a network packet clients send to the
		// server that holds a limited set of requests (see the UserCommand_t structure for details).
		// It has been adapted for usage to HLTV spectators, who don't send ClientCommands, but send
		// all their update information to the server using usercmd's instead, it seems.
		// Post version called only by metamod.

		if (player->IsPlayerFakeClient ())
		{
			// Reliability check.
			InternalAssert (randomSeed == 0u && player->privateData->GetBasePlayerPointer ()->random_seed == 0u);

			/// @note Already done in FakeClient::Think(), but original function sets 'HalfLifeEngine::SDK::Classes::BasePlayer::random_seed' value back to 0, so we need to reset it again....
			player->privateData->GetBasePlayerPointer ()->random_seed = g_randomNumberGenerator.GetValueBetween (0u, 0x7FFFFFFFu);	// full range
		}
/*		else
		{
			static unsigned int s_maximumRandomSeed (0u);

			if (s_maximumRandomSeed < randomSeed)
				s_maximumRandomSeed = randomSeed;

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "CmdStart(): randomSeed=%u, s_maximumRandomSeed=%u.", randomSeed, s_maximumRandomSeed);
		}
*/
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "CmdStart(): Called for client \"%s\", msec=%u.", player->GetNetName ().GetData (), command->msecValue);

		MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_IGNORED;
	}
}

#endif	// ifndef META_MOD_ENGINE_EXPORT_REPLACED_FUNCTIONS_DLL_API_DLL_API_INCLUDED