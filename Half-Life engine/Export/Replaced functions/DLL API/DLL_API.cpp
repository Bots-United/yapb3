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
// DLL_API.cpp
//
// Description: Implementation of Half-Life DLL routines.
//
// Version: $ID:$
//

#include <Core.hpp>

namespace HalfLifeEngine
{
	namespace Export
	{
		namespace Callbacks
		{
			namespace DLLTable
			{
				void GameDLLInit (void)
				{
					// this function is a one-time call, and appears to be the second function called in the
					// DLL after FunctionPointer_t() has been called. Its purpose is to tell the MOD DLL to
					// initialize the game before the engine actually hooks into it with its video frames and
					// clients connecting. Note that it is a different step than the *server* initialization.
					// This one is called once, and only once, when the game process boots up before the first
					// server is enabled. Here is a good place to do our own game session initialization, and
					// to register by the engine side the server commands we need to administrate our bots.

					/// @remind DO WE NEED THIS?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?

//	MessageBox (NULL, "GameDLLInit(): Called!!!", "YaPB", MB_ICONASTERISK | MB_TOPMOST);
				}

				void EntitySpawn (SDK::Classes::Edict *const entity)
				{
					// this function asks the game.DLL to spawn (i.e, give a physical existence in the virtual world, in other words to 'display') the entity pointed to by 'entity' in the game.
					// The Spawn() function is one of the functions any entity is supposed to have in the game.DLL, and any MOD is supposed to implement one for each of its entities.

//					DEBUG_FUNCTION_TRACING_START ();

					// Reliability check.
					InternalAssert (entity != NULL);

					{
						static bool isFirstCall (true);

						if (isFirstCall)
						{
							isFirstCall = false;

							if (Globals::g_halfLifeEngine->IsUnderMetaMod ())
								Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->InitializePrecachedStrings ();
						}
					}

					const DynamicString entityClassName (entity->GetClassName ());

					if (entityClassName == "worldspawn")
						Globals::g_halfLifeEngine->PrecacheStuff ();	// Precache resources used in bot code.
/*DO WE NEED THIS?!?!?!?!?!?!?!?!?!?!?!?!					else if (entityClassName == "player_weaponstrip" && entity->GetTarget ().IsEmpty ())
					{
						// thanks to strelomet for fixing crash on scoutzknives maps
						entity->variables.target = entity->variables.targetName = Globals::g_halfLifeEngine->GetOffsetFromString ("fake");	// I think it's a bug.
						// I think it's a bug.
//						entity->variables.target = Globals::g_halfLifeEngine->GetOffsetFromString ("fake");
//						entity->variables.targetName = Globals::g_halfLifeEngine->GetOffsetFromString ("fake");
					}
					else if (entityClassName == "info_player_deathmatch")
					{
						entity->SetModel ("models/player/terror/terror.mdl");

						entity->variables.renderMode = SDK::Constants::kRenderTransAlpha;	// set its render mode to transparency
						entity->variables.renderAmount = 127.0f;	// set its transparency amount
						entity->variables.effects |= SDK::Constants::EntityEffect_NoDraw;
					}
					else if (entityClassName == "info_player_start")
					{
						entity->SetModel ("models/player/urban/urban.mdl");

						entity->variables.renderMode = SDK::Constants::kRenderTransAlpha;	// set its render mode to transparency
						entity->variables.renderAmount = 127.0f;	// set its transparency amount
						entity->variables.effects |= SDK::Constants::EntityEffect_NoDraw;
					}
					else if (entityClassName == "info_vip_start")
					{
						entity->SetModel ("models/player/vip/vip.mdl");

						entity->variables.renderMode = SDK::Constants::kRenderTransAlpha;	// set its render mode to transparency
						entity->variables.renderAmount = 127.0f;	// set its transparency amount
						entity->variables.effects |= SDK::Constants::EntityEffect_NoDraw;
					}
*/
//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ClientConnect (SDK::Classes::Edict *const/* client*/, const char *const/* name*/, const char *const/* address*/, const char/* rejectReason*/[128u])
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

//					DEBUG_FUNCTION_TRACING_START ();

					// check if this client is the listen server client
//					if (strcmp (address, "loopback") == 0/* || strcmp (address, "127.0.0.1") == 0*/)
//						g_hostEntity = new Host (client);	// save the edict of the listen server client...
/*
					#if defined _DEBUG
						AddLogEntry (true, LogLevel_Default, true, "HalfLifeEngine::Export::Callbacks::DLLTable::ClientConnect(client: \"%s\", address: \"%s\", rejectReason: \"%s\") called!", name, address, rejectReason);
					#endif	// if defined _DEBUG
*/
//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ClientDisconnect (SDK::Classes::Edict *const client)
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

//					DEBUG_FUNCTION_TRACING_START ();

					// Check in case the server deactivate, ServerDeactivate() (where there is a deleting 'g_server' pointer) called before ClientDisconnect().
					InternalAssert (g_server->IsRunning ());

					g_server->GetClientManager ()->ClientDisconnect (client);

//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ClientPutInServer (SDK::Classes::Edict *const client)
				{
					// this function is called once a just connected client actually enters the game, after
					// having downloaded and synchronized its resources with the of the server's. It's the
					// perfect place to hook for client connecting, since a client can always try to connect
					// passing the ClientConnect() step, and not be allowed by the server later (because of a
					// latency timeout or whatever reason). We can here keep track of both bots and players
					// counts on occurence, since bots connect the server just like the way normal client do,
					// and their third party bot flag is already supposed to be set then. If it's a bot which
					// is connecting, we also have to awake its brain(s) by reading them from the disk.

//					DEBUG_FUNCTION_TRACING_START ();

					// Reliability checks.
					InternalAssert (g_server->IsRunning ());
					InternalAssert (client->IsValid () && client->IsPlayer ());

					g_server->GetClientManager ()->ClientPutInServer (client);

//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ClientCommand (SDK::Classes::Edict *const/* clientEdict*/)
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

//					DEBUG_FUNCTION_TRACING_START ();
/*! @note TEMPORAILY DISABLED, HandleExecutedCommand() FUNCTION IS CALLED FROM Detour_ClientCommand() - Server.cpp
					// Reliability checks.
					InternalAssert (g_server->IsRunning ());
					InternalAssert (clientEdict->IsValid () && clientEdict->IsPlayer ());

					Client *const client (g_server->GetClientManager ()->GetClient (clientEdict));

					// Occurs on changelevel.
					if (client->IsValid ())
						client->HandleExecutedCommand (Globals::g_halfLifeEngine->GetCommandArgument (0u), Globals::g_halfLifeEngine->GetCommandArgument (1u));
*/
//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ClientUserInfoChanged (SDK::Classes::Edict *const client, char *const infoBuffer)
				{
					// this function is called when a player changes model, or changes team. Occasionally it
					// enforces rules on these changes (for example, some MODs don't want to allow players to
					// change their player model). But most commonly, this function is in charge of handling
					// team changes, recounting the teams population, etc...

//					DEBUG_FUNCTION_TRACING_START ();

					// Reliability check.
					InternalAssert (g_server->IsRunning ());

					g_server->GetClientManager ()->ClientUserInfoChanged (client, infoBuffer);

//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ServerActivate (void)
				{
					// this function is called when the server has fully loaded and is about to manifest itself
					// on the network as such. Since a mapchange is actually a server shutdown followed by a
					// restart, this function is also called when a new map is being loaded. Hence it's the
					// perfect place for doing initialization stuff for our bots, such as reading the BSP data,
					// loading the bot profiles, and drawing the world map (i.e., filling the navigation hash table).
					// Once this function has been called, the server can be considered as "running".

//					DEBUG_FUNCTION_TRACING_START ();

					// Just to make sure....
					InternalAssert (!g_server->IsRunning ());

					if (Server::IsFirstServerStart ())
					{
						// Register the server console variables, engine virtual tables hooks only on first server start....

						// FIRST - Initialize localizer. (Load only configuration file)
						g_localizer.LoadConfigurationFile ();

						Console::Server::variableManager.RegisterGameVariables ();	// Register "mp_" console variables....

						#if defined MULTIPLE_DRAW_METHODS
							if (Globals::g_halfLifeEngine->IsSoftwareRenderingMode ())
								Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DrawMethod)->SetValue (Console::Bot::VariableValue_DrawMethod_Default);
							else
							{
								g_drawInterface = new DrawInterface_GL ();

								if (g_drawInterface == NULL)
									AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating hardware drawing interface! Hardware rendering disabled.", sizeof (DrawInterface_GL));
								else if (!g_drawInterface->Initialize ())
								{
									AddLogEntry (true, LogLevel_Error, false, "Couldn't initialize hardware drawing interface! Hardware rendering disabled.");

									delete g_drawInterface;

									g_drawInterface = NULL;
								}
							}
						#endif	// if defined MULTIPLE_DRAW_METHODS
					}
					else
						g_localizer.Reload ();	// Load original strings and replacements or translated strings (depending on the given language) from the configuration file.

					// Allocate memory for the server....
					if (Globals::g_halfLifeEngine->IsDedicatedServer ())
						g_server = new DedicatedServer ();
					else
						g_server = new ListenServer ();

					#if defined _DEBUG
						// Reliability check.
						if (g_server == NULL)
							TerminateOnMalloc ();
					#endif	// if defined _DEBUG
/*
					STDIOFile file;
					char line[256u];
					int index;

					// initialize all configuration files...

					// GENERAL DATA INITIALIZATION
					if (OpenConfigurationFile ("general", "General configuration file not found (configs/general.cfg). Loading defaults.", file))
					{
						DynamicArray <DynamicString> pairs, splitted;

						// for each line in the file...
						while (!file.IsEndOfFile () && file.ReadLine (line, sizeof (line)) != NULL)
						{
							// ignore line if void OR commented
							if (IsEmptyOrCommentedFileBufferLine (line))
								continue;

							pairs = DynamicString (line).Split ('=');

							if (pairs.GetElementNumber () != 2)
							{
								file.Close ();

								AddLogEntry (false, LogLevel_Critical, false, "Error in general bot configuration file syntax... Please correct all errors.");

								return;
							}

							for (index = 0u; index < pairs.GetElementNumber (); index++)
								pairs[index].Trim ().Trim ();	// double trim

							splitted = pairs[1].Split (',');

							if (pairs[0] == "MapStandard")
							{
								if (splitted.GetElementNumber () != Const_NumWeapons)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								for (index = 0u; index < Const_NumWeapons; index++)
									g_weaponSelect[index].teamStandard = splitted[index].ToInt ();
							}
							else if (pairs[0] == "MapAS")
							{
								if (splitted.GetElementNumber () != Const_NumWeapons)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								for (index = 0u; index < Const_NumWeapons; index++)
									g_weaponSelect[index].teamAS = splitted[index].ToInt ();
							}
							else if (pairs[0] == "GrenadePercent")
							{
								if (splitted.GetElementNumber () != 3)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								for (index = 0u; index < 3u; ++index)
									g_grenadeBuyPrecent[index] = splitted[index].ToInt ();
							}
							else if (pairs[0] == "Economics")
							{
								if (splitted.GetElementNumber () != 11)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								for (index = 0u; index < 11u; ++index)
									g_buyEconomyTable[index] = splitted[index].ToInt ();
							}
							else if (pairs[0] == "PersonalityNormal")
							{
								if (splitted.GetElementNumber () != Const_NumWeapons)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								for (index = 0u; index < Const_NumWeapons; ++index)
									g_weaponPreferences[Personality_Normal][index] = splitted[index].ToInt ();	// set personality weapon pointers here
							}
							else if (pairs[0] == "PersonalityRusher")
							{
								if (splitted.GetElementNumber () != Const_NumWeapons)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								for (index = 0u; index < Const_NumWeapons; ++index)
									g_weaponPreferences[Personality_Rusher][index] = splitted[index].ToInt ();	// set personality weapon pointers here
							}
							else if (pairs[0] == "PersonalityCareful")
							{
								if (splitted.GetElementNumber () != Const_NumWeapons)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								for (index = 0u; index < Const_NumWeapons; ++index)
									g_weaponPreferences[Personality_Careful][index] = splitted[index].ToInt ();	// set personality weapon pointers here
							}
							else if (pairs[0] == "Skill")
							{
								if (splitted.GetElementNumber () != 8)
								{
									file.Close ();

									AddLogEntry (false, LogLevel_Critical, false, "%s entry in general configuration file is not valid.", pairs[0].GetData ());

									return;
								}

								const int parserState = (pairs[0] & "Newbie") ? 1 : (pairs[0] & "Average") ? 2 : (pairs[0] & "Advanced") ? 3 : (pairs[0] & "Professional") ? 4 : (pairs[0] & "Godlike") ? 5 : 0;

								for (index = 0u; index < 8u; ++index)
								{
									switch (index)
									{
										case 0u:
											YaPB::s_skillTables[parserState].minSurpriseTime = splitted[index].ToFloat ();

											break;

										case 1u:
											YaPB::s_skillTables[parserState].maxSurpriseTime = splitted[index].ToFloat ();

											break;

										case 2u:
											YaPB::s_skillTables[parserState].minTurnSpeed = splitted[index].ToFloat ();

											break;

										case 3u:
											YaPB::s_skillTables[parserState].maxTurnSpeed = splitted[index].ToFloat ();

											break;

										case 4u:
											YaPB::s_skillTables[parserState].headshotFrequency = splitted[index].ToInt ();

											break;

										case 5u:
											YaPB::s_skillTables[parserState].heardShootThruProb = splitted[index].ToInt ();

											break;

										case 6u:
											YaPB::s_skillTables[parserState].seenShootThruProb = splitted[index].ToInt ();

											break;

										case 7u:
											YaPB::s_skillTables[parserState].recoilAmount = splitted[index].ToInt ();

											break;
									}
								}
							}
						}
					}
*/
					// do level initialization stuff here...
//					g_waypoint->Initialize ();
//					g_waypoint->Load ();

//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ServerDeactivate (void)
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

//					DEBUG_FUNCTION_TRACING_START ();

					// Just to make sure.
					InternalAssert (g_server->IsRunning ());

					g_server->UnregisterAllForwardsAllHooks ();

					// Deactivate the server and free the all used memory.
					delete g_server;

					g_server = NULL;

					// save collected experience on shutdown
//					g_waypoint->SaveExperienceTable ();
//					g_waypoint->SaveVisibilityTable ();

//					DEBUG_FUNCTION_TRACING_END ();
				}

				void StartFrame (void)
				{
					// this function starts a video frame. It is called once per video frame by the engine. If
					// you run Half-Life at 90 fps, this function will then be called 90 times per second. By
					// placing a hook on it, we have a good place to do things that should be done continuously
					// during the game, for example making the bots think (yes, because no Think() function exists
					// for the bots by the MOD side, remember). Also here we have control on the bot population,
					// for example if a new player joins the server, we should disconnect a bot, and if the
					// player population decreases, we should fill the server with other bots.

//					DEBUG_FUNCTION_TRACING_START ();

					InternalAssert (g_server->IsRunning ());

					g_server->Think ();	// Make the server 'think'.
/*
					static int index;
					static float secondThinkTimer (0.0f);

					// check is it time to execute second think
					if (secondThinkTimer <= Globals::g_halfLifeEngine->GetTime ())
					{
						// check console variable restrictions...
						if (Globals::consoleVariables[ConsoleVariableID_DangerFactor]->GetValue <float> () < 0.0f)
							Globals::consoleVariables[ConsoleVariableID_DangerFactor]->SetValue (0.0f);
						else if (Globals::consoleVariables[ConsoleVariableID_DangerFactor]->GetValue <float> () > 4096.0f)
							Globals::consoleVariables[ConsoleVariableID_DangerFactor]->SetValue (4096.0f);

						if (g_bombPlanted)
							g_waypoint->SetBombPosition ();

						// update timer to 1.5 second
						secondThinkTimer = Globals::g_halfLifeEngine->GetTime () + 1.5f;
					}

					// AI EXECUTION STARTS
					g_botManager->Think ();
					// AI EXECUTION FINISH

					// the following functions is only valid on listen server
					if (!IsDedicatedServer () && g_server->GetClientManager ()->IsClientSlotUsed (g_hostEntity))
					{
						// is waypointing enabled?
						if (g_waypointOn)
							g_waypoint->Think ();	// WAYPOINT EXECUTION STARTS

						// receive welcome message to listen server entity, if it's time to do this...
						CheckWelcomeMessage ();
					}

					// keep bot number up to date
					g_botManager->MaintainBotQuota ();
*/
//					DEBUG_FUNCTION_TRACING_END ();
				}

				void UpdateClientData (const SDK::Classes::Edict *const entity)
				{
					// This function is a synchronization tool that is used periodically by the engine to tell
					// the game DLL to send player info over the network to one of its clients when it suspects
					// that this client is desynchronizing. Early bots were using it to ask the game DLL for the
					// weapon list of players (by setting 'sendWeapons' to TRUE), but most of the time having a
					// look around the entity->variables.weapons bitmask is enough, since that's the place commonly used for
					// MODs to store weapon information. If it can't be read from there, catching a few network
					// messages (like in DMC) do the job better than this function anyway.

//					DEBUG_FUNCTION_TRACING_START ();

					// Just to make sure.
					InternalAssert (g_server->IsRunning ());

					g_server->GetClientManager ()->UpdateClientData (entity);

//					DEBUG_FUNCTION_TRACING_END ();
				}

				inline void PM_PlaySound (SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, SDK::Constants::SoundFlag_t flags, SDK::Constants::SoundPitch_t pitch)
				{
					// Reliability checks.
					InternalAssert (g_server->IsRunning ());

					const unsigned char index (static_cast <unsigned char> (Globals::g_halfLifeEngine->GetPlayerMovePointer ()->playerIndex));

					// Reliability check.
//					InternalAssert (index < g_server->GetClientManager ()->GetClientsCount ());	// OCCURS!!!!!
					if (index < g_server->GetClientManager ()->GetClientsCount ())	// OCCURS!!!!!
					{
						Client *const client (g_server->GetClientManager ()->GetClient (index));

						// Reliability check.
						InternalAssert (client->IsValid ());
/*
							int ivolume = (int)(1000*((volume+1)/2));
							SaveSound (client, client->GetOrigin (), ivolume, channel, 5.0f);
*/
						g_server->GetClientManager ()->EmitSound (*client, sample, volume, attenuation, flags, pitch);
					}

					(*Globals::g_halfLifeEngine->GetPlayerMove ().PM_PlaySound) (channel, sample, volume, attenuation, flags, pitch);
				}

				void PM_Move (SDK::Structures::PlayerMove_t *const playerMove, const bool server)
				{
					// this is the player movement code clients run to predict things when the server can't update
					// them often enough (or doesn't want to). The server runs exactly the same function for
					// moving players. There is normally no distinction between them, else client-side prediction
					// wouldn't work properly (and it doesn't work that well, already...)

					// Reliability checks.
					InternalAssert (playerMove != NULL);
					InternalAssert (Globals::g_halfLifeEngine->GetPlayerMovePointer () == playerMove);
					InternalAssert (server == true);

					if (playerMove->PM_PlaySound != &PM_PlaySound)
					{
						Globals::g_halfLifeEngine->m_playerMove.PM_PlaySound = playerMove->PM_PlaySound;

						playerMove->PM_PlaySound = &PM_PlaySound;	// hook footstep sound function
					}
				}

				void PM_Init (SDK::Structures::PlayerMove_t *const playerMove)
				{
					// I sure as hell don't know what this one is up to. I suspect it's one function that must be
					// called before using PM_Move() on a PlayerMove_t structure. Probably for initializing data.
					// If anyone has info...

					// Reliability check.
					InternalAssert (playerMove != NULL);

					Globals::g_halfLifeEngine->SetPlayerMove (playerMove);
				}

				namespace Post
				{
					void EntitySpawn (SDK::Classes::Edict *const entity)
					{
						// this function asks the game.DLL to spawn (i.e, give a physical existence in the virtual
						// world, in other words to 'display') the entity pointed to by 'entity' in the game.
						// The Spawn() function is one of the functions any entity is supposed to have in the game DLL,
						// and any MOD is supposed to implement one for each of its entities.

//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (entity != NULL);

						// solves the bots unable to see through certain types of glass bug. (engine bug)
						// MAPPERS: NEVER EVER ALLOW A TRANSPARENT ENTITY TO WEAR THE FL_WORLDBRUSH FLAG !!!
						// FL_WORLDBRUSH defines unbreakable glasses... - it doesn't work at all (tested on de_frosty)
						if (entity->variables.renderMode == SDK::Constants::kRenderTransTexture)
							entity->variables.flags &= ~SDK::Constants::FL_WORLDBRUSH;	// clear the 'FL_WORLDBRUSH' flag out of transparent entities

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void ServerActivate (void)
					{
						// this function is called when the server has fully loaded and is about to manifest itself
						// on the network as such. Since a mapchange is actually a server shutdown followed by a
						// restart, this function is also called when a new map is being loaded. Hence it's the
						// perfect place for doing initialization stuff for our bots, such as reading the BSP data,
						// loading the bot profiles, and drawing the world map (ie, filling the navigation hashtable).
						// Once this function has been called, the server can be considered as "running".

//						DEBUG_FUNCTION_TRACING_START ();

//						g_waypoint->InitializeVisibility ();

//						DEBUG_FUNCTION_TRACING_END ();
					}
				}
			}
		}

		namespace ReplacedFunctions
		{
			namespace DLLTable
			{
				void GameDLLInit (void)
				{
					// this function is a one-time call, and appears to be the second function called in the
					// DLL after FunctionPointer_t() has been called. Its purpose is to tell the MOD DLL to
					// initialize the game before the engine actually hooks into it with its video frames and
					// clients connecting. Note that it is a different step than the *server* initialization.
					// This one is called once, and only once, when the game process boots up before the first
					// server is enabled. Here is a good place to do our own game session initialization, and
					// to register by the engine side the server commands we need to administrate our bots.

					Callbacks::DLLTable::GameDLLInit ();

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnGameInit) ();	// Call the original engine function...
				}

				int Spawn (SDK::Classes::Edict *entity)
				{
					// this function asks the game.DLL to spawn (i.e, give a physical existence in the virtual world, in other words to 'display') the entity pointed to by 'entity' in the game.
					// The Spawn() function is one of the functions any entity is supposed to have in the game.DLL, and any MOD is supposed to implement one for each of its entities.

					Callbacks::DLLTable::EntitySpawn (entity);

					// Pre call original function and get result....
					const int result ((*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnSpawn) (entity));

					Callbacks::DLLTable::Post::EntitySpawn (entity);

					return result;
				}

				void Touch (SDK::Classes::Edict *entityTouched, SDK::Classes::Edict *entityOther)
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
/*
					// Reliability check.
					InternalAssert (g_server->IsRunning ());

					if (g_server->GetClientManager ()->GetClient (SDK::Constants::ListenServerHostClientSlotIndex)->IsValid ()/* && entityTouched->GetClassName () == "player"*//*)
						Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Entity %s touched entity %s.\n", entityTouched->IsValid () ? entityTouched->GetClassName ().GetData () : "NULL", entityOther->IsValid () ? entityOther->GetClassName ().GetData () : "NULL");
*/
					// bot code wants to evade engine seeing entities being touched
//					if (IsValidBot (entityTouched, true))
//						g_botManager->GetBot (entityTouched)->Touch (entityTouched);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnTouch) (entityTouched, entityOther);
				}

				BOOL ClientConnect (SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u])
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

					Callbacks::DLLTable::ClientConnect (client, name, address, rejectReason);

					return (*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnClientConnect) (client, name, address, rejectReason);
				}

				void ClientDisconnect (SDK::Classes::Edict *client)
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

					Callbacks::DLLTable::ClientDisconnect (client);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnClientDisconnect) (client);
				}

				void ClientPutInServer (SDK::Classes::Edict *const client)
				{
					// this function is called once a just connected client actually enters the game, after
					// having downloaded and synchronized its resources with the of the server's. It's the
					// perfect place to hook for client connecting, since a client can always try to connect
					// passing the ClientConnect() step, and not be allowed by the server later (because of a
					// latency timeout or whatever reason). We can here keep track of both bots and players
					// counts on occurence, since bots connect the server just like the way normal client do,
					// and their third party bot flag is already supposed to be set then. If it's a bot which
					// is connecting, we also have to awake its brain(s) by reading them from the disk.

					Callbacks::DLLTable::ClientPutInServer (client);

					g_server->GetClientManager ()->GetClientPutInServerHooker ().CallOriginalFunction (client);	// Call the original game DLL function....
				}

				void ClientCommand (SDK::Classes::Edict *clientEdict)
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

					Callbacks::DLLTable::ClientCommand (clientEdict);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnClientCommand) (clientEdict);
				}

				void ClientUserInfoChanged (SDK::Classes::Edict *client, char *infoBuffer)
				{
					// this function is called when a player changes model, or changes team. Occasionally it
					// enforces rules on these changes (for example, some MODs don't want to allow players to
					// change their player model). But most commonly, this function is in charge of handling
					// team changes, recounting the teams population, etc...

					Callbacks::DLLTable::ClientUserInfoChanged (client, infoBuffer);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnClientUserInfoChanged) (client, infoBuffer);
				}

				void ServerActivate (SDK::Classes::Edict *entityEdictList, int edictCount, int clientMax)
				{
					// this function is called when the server has fully loaded and is about to manifest itself
					// on the network as such. Since a mapchange is actually a server shutdown followed by a
					// restart, this function is also called when a new map is being loaded. Hence it's the
					// perfect place for doing initialization stuff for our bots, such as reading the BSP data,
					// loading the bot profiles, and drawing the world map (i.e., filling the navigation hash table).
					// Once this function has been called, the server can be considered as "running".

					Callbacks::DLLTable::ServerActivate ();

					// Pre call original function....
					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnServerActivate) (entityEdictList, edictCount, clientMax);

					Callbacks::DLLTable::Post::ServerActivate ();
				}

				void ServerDeactivate (void)
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

					Callbacks::DLLTable::ServerDeactivate ();

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnServerDeactivate) ();
				}

				void StartFrame (void)
				{
					// this function starts a video frame. It is called once per video frame by the engine. If
					// you run Half-Life at 90 fps, this function will then be called 90 times per second. By
					// placing a hook on it, we have a good place to do things that should be done continuously
					// during the game, for example making the bots think (yes, because no Think() function exists
					// for the bots by the MOD side, remember). Also here we have control on the bot population,
					// for example if a new player joins the server, we should disconnect a bot, and if the
					// player population decreases, we should fill the server with other bots.

					Callbacks::DLLTable::StartFrame ();

//					if (!g_server->IsRunning () || !g_server->GetNavigationMeshManager ().IsGenerating ())
					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnStartFrame) ();
				}

				void PM_Move (SDK::Structures::PlayerMove_t *playerMove, BOOL server)
				{
					// this is the player movement code clients run to predict things when the server can't update
					// them often enough (or doesn't want to). The server runs exactly the same function for
					// moving players. There is normally no distinction between them, else client-side prediction
					// wouldn't work properly (and it doesn't work that well, already...)

					Callbacks::DLLTable::PM_Move (playerMove, server == TRUE);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnPM_Move) (playerMove, server);
				}

				void PM_Init (SDK::Structures::PlayerMove_t *playerMove)
				{
					// I sure as hell don't know what this one is up to. I suspect it's one function that must be
					// called before using PM_Move() on a PlayerMove_t structure. Probably for initializing data.
					// If anyone has info...

					Callbacks::DLLTable::PM_Init (playerMove);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnPM_Init) (playerMove);
				}

				void UpdateClientData (const SDK::Classes::Edict *entity, int sendWeapons, SDK::Structures::ClientData_t *clientData)
				{
					// This function is a synchronization tool that is used periodically by the engine to tell
					// the game DLL to send player info over the network to one of its clients when it suspects
					// that this client is desynchronizing. Early bots were using it to ask the game DLL for the
					// weapon list of players (by setting 'sendWeapons' to TRUE), but most of the time having a
					// look around the entity->variables.weapons bitmask is enough, since that's the place commonly used for
					// MODs to store weapon information. If it can't be read from there, catching a few network
					// messages (like in DMC) do the job better than this function anyway.

					Callbacks::DLLTable::UpdateClientData (entity);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnUpdateClientData) (entity, sendWeapons, clientData);
				}

				void CmdStart (const SDK::Classes::Edict *const player, const SDK::Structures::UserCommand_t *const command, unsigned int randomSeed)
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
						InternalAssert (randomSeed == 0u);

						randomSeed = player->privateData->GetBasePlayerPointer ()->random_seed;
					}
/*					else
					{
						static unsigned int s_maximumRandomSeed (0u);

						if (s_maximumRandomSeed < randomSeed)
							s_maximumRandomSeed = randomSeed;

						Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "CmdStart(): randomSeed=%u, s_maximumRandomSeed=%u.", randomSeed, s_maximumRandomSeed);
					}
*/
//					Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "CmdStart(): Called for client \"%s\", msec=%u.", player->GetNetName ().GetData (), command->msecValue);

					(*Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnCmdStart) (player, command, randomSeed);
				}
			}
		}
	}
}