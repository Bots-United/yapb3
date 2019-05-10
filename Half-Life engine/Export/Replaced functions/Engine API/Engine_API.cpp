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
// Engine_API.cpp
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
			namespace EngineTable
			{
				void ChangeLevel (const char *const s1, const char *const s2)
				{
					// the purpose of this function is to ask the engine to shutdown the server and restart a
					// new one running the map whose name is s1. It is used ONLY IN SINGLE PLAYER MODE and is
					// transparent to the user, because it saves the player state and equipment and restores it
					// back in the new level. The "changelevel trigger point" in the old level is linked to the
					// new level's spawn point using the s2 string, which is formatted as follows: "trigger_name
					// to spawnpoint_name", without spaces (for example, "tr_1atotr_2lm" would tell the engine
					// the player has reached the trigger point "tr_1a" and has to spawn in the next level on the
					// spawn point named "tr_2lm".

//					DEBUG_FUNCTION_TRACING_START ();

					// Reliability check.
					InternalAssert (g_server->IsRunning ());

					g_server->ChangeLevel (s1, s2);

//					DEBUG_FUNCTION_TRACING_END ();
				}

				void EmitSound (const SDK::Classes::Edict *const entity, const SDK::Constants::SoundChannel_t/* channel*/, const char *const sample, const float volume, const float attenuation, const SDK::Constants::SoundFlag_t flags, const SDK::Constants::SoundPitch_t pitch)
				{
					// this function tells the engine that the entity pointed to by 'entity', is emitting a sound
					// which filename is 'sample', at level 'channel' ('CHAN_VOICE', etc...), with 'volume' as
					// loudness multiplicator (normal volume 'VOL_NORM' is 1.0), with a pitch of 'pitch' (normal
					// pitch 'PITCH_NORM' is 100.0), and that this sound has to be attenuated by distance in air
					// according to the value of 'attenuation' (normal attenuation 'ATTN_NORM' is 0.8; 'ATTN_NONE'
					// means no attenuation with distance). Optionally flags 'flags' can be passed, which I don't
					// know the heck of the purpose. After we tell the engine to emit the sound, we have to call
					// SoundAttachToThreat() to bring the sound to the ears of the bots. Since bots have no client.DLL
					// to handle this for them, such a job has to be done manually.

//					DEBUG_FUNCTION_TRACING_START ();

					// Reliability checks.
					InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!
					InternalAssert (entity->IsValid ());

					g_server->GetClientManager ()->EmitSound (entity, sample, volume, attenuation, flags, pitch);

//					DEBUG_FUNCTION_TRACING_END ();
				}

				void ClientCommand (SDK::Classes::Edict *const/* client*/, const char *const/* command*/)
				{
					// this function forces the client whose player entity is 'client' to issue a client command.
					// How it works is that clients all have a 'g_server->GetClientManager ()->GetFakeClientCommand()' global string in their client.DLL that
					// stores the command string; if ever that string is filled with characters, the client.DLL
					// sends it to the engine as a command to be executed. When the engine has executed that
					// command, this 'g_server->GetClientManager ()->GetFakeClientCommand()' string is reset to zero. Here is somehow a curious implementation of
					// ClientCommand: the engine sets the command it wants the client to issue in his 'g_server->GetClientManager ()->GetFakeClientCommand()', then
					// the client.DLL sends it back to the engine, the engine receives it then executes the
					// command therein. Don't ask me why we need all this complicated crap. Anyhow since bots have
					// no client.DLL, be certain never to call this function upon a bot entity, else it will just
					// make the server crash. Since hordes of uncautious, not to say stupid, programmers don't
					// even imagine some players on their servers could be bots, this check is performed less than
					// sometimes actually by their side, that's why we strongly recommend to check it here too. In
					// case it's a bot asking for a client command, we handle it like we do for bot commands, i.e. using FakeClientCommand().

					// DO WE NEED THIS??????????????????? - ANSWER: "NO" - ENGINE CHECKS WHERE 'client' IS BOT!
/*
					// is the target entity an official bot, or a third party bot?
					if (client->IsPlayerFakeClient () || (client->variables.flags & Constants::FL_DORMANT))
					{
						// prevent bots to be forced to issue client commands

						if (MetaModEngine::Globals::g_metaModEngine != NULL)
							MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

						return;
					}*/
				}

				const unsigned char RegisterUserMessage (const char *const name, const short size)
				{
					// this function registers a "user message" by the engine side. User messages are network
					// messages the game.DLL asks the engine to send to clients. Since many MODs have completely
					// different client features (Counter-Strike has a radar and a timer, for example), network
					// messages just can't be the same for every MOD. Hence here the MOD DLL tells the engine,
					// "Hey, you have to know that I use a network message whose name is 'name' and it is 'size'
					// packets long". The engine books it, and returns the ID number under which he recorded that
					// custom message. Thus every time the MOD DLL will be wanting to send a message named 'name'
					// using pfnMessageBegin(), it will know what message ID number to send, and the engine will
					// know what to do.

					// register the message and return the registered message ID number the engine gave us back.
					return Globals::g_halfLifeEngine->RegisterUserMessage (name, size);
				}

				void ClientPrintf (SDK::Classes::Edict *const/* client*/, const SDK::Constants::PrintType_t/* printType*/, const char *const/* message*/)
				{
					// this function prints the text message string pointed to by 'message' by the client side of
					// the client entity pointed to by 'client', in a manner depending of 'printType' ('PrintType_AtConsole',
					// 'PrintType_AtCenter' or 'PrintType_AtChat'). Be certain never to try to feed a bot with this function,
					// as it will crash your server. Why would you, anyway? bots have no client DLL as far as
					// we know, right? But since stupidity rules this world, we do a preventive check :)

					// DO WE NEED THIS???????????????????
/*
					// is this message for a bot?
					if (client->IsPlayerFakeClient ())
					{
						// disallow client printings for bots

						if (MetaModEngine::Globals::g_metaModEngine != NULL)
							MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

						return;
					}
*/
				}

				const char *const Cmd_Args (void)
				{
					// this function returns a pointer to the whole current client command string. Since bots
					// have no client DLL and we may want a bot to execute a client command, we had to implement
					// a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bot commands, and also keep track of the
					// argument count. Hence this hook not to let the engine ask an unexistent client DLL for a
					// command we are holding here. Of course, real clients commands are still retrieved the
					// normal way, by asking the engine.

					// This is a bot issuing that client command....
					// Return the whole bot client command string we know....

					return g_server->GetClientManager ()->GetFakeClientCommand ();
				}

				const char *const Cmd_Argv (const unsigned char argc)
				{
					// this function returns a pointer to a certain argument of the current client command. Since
					// bots have no client DLL and we may want a bot to execute a client command, we had to
					// implement a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bot commands, and also keep
					// track of the argument count. Hence this hook not to let the engine ask an unexistent client
					// DLL for a command we are holding here. Of course, real clients commands are still retrieved
					// the normal way, by asking the engine.

					// This is a bot issuing that client command....
					// If so, then return the wanted argument we know....

					return g_server->GetClientManager ()->GetFakeClientCommand ().GetArgument (static_cast <unsigned char> (argc));
				}

				const unsigned char Cmd_Argc (void)
				{
					// this function returns the number of arguments the current client command string has. Since
					// bots have no client DLL and we may want a bot to execute a client command, we had to
					// implement a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bots' commands, and also keep
					// track of the argument count. Hence this hook not to let the engine ask an unexistent client
					// DLL for a command we are holding here. Of course, real clients commands are still retrieved
					// the normal way, by asking the engine.

					// This is a bot issuing that client command....
					// If so, then return the argument number we know....

					return g_server->GetClientManager ()->GetFakeClientCommand ().GetArgumentsNumber ();
				}

				namespace Post
				{
					void SetEntityModel (SDK::Classes::Edict *const entity, const char *const modelName)
					{
						// The purpose of this function is to ask the engine to set a render model 'modelName' (like "models/
						// mechgibs.mdl") to entity 'entity'. The engine itself will provide the necessary changes in the
						// entity's entvars structure for it, and clients will be updated, provided they get this
						// entity in their baselines, so that they can display it properly. If you don't know what
						// a "baseline" is all about, have a look around the CreateBaseline() functions in dll.cpp

//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (entity->IsValid ());

						// Not yet thrown?
						if (entity->variables.gravity == 0.0f || entity->variables.owner == NULL || modelName[7u] != 'w' || modelName[8u] != '_')
							return;

						// Grenade has been	thrown.
						// Only	here we	may	find out who is	an owner.

						// The moment the grenade is thrown....
						switch (modelName[9u])
						{
							// High Explosive grenade....
							case 'h':
								// Reliability check.
								InternalAssert (g_server->IsRunning ());
								InternalAssert (entity->variables.owner->IsValid () && entity->variables.owner->IsNotWorldspawnPlayer ());

								g_server->GetThrownGrenadesManager ().PutThrownExplosiveGrenade (entity);

//								Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "HE grenade has been thrown! Owner \"%s\", damage=%.2f, damageTime=%.2f.\n", entity->variables.owner->GetNetName ().GetData (), entity->variables.damage, entity->variables.damageTime - Globals::g_halfLifeEngine->GetTime ());

								{
									Utilities::PseudoGrenadeEntityVariables_t pseudoGrenadeEntityVariables;

		Utilities::TraceThrownGrenadeToss (entity, pseudoGrenadeEntityVariables);

//		Globals::g_halfLifeEngine->DrawLine (grenade->variables.origin, pseudoGrenadeEntityVariables.origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
//		Globals::g_halfLifeEngine->DrawCross3D (pseudoGrenadeEntityVariables.origin, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

									g_origins += pseudoGrenadeEntityVariables.origin;

									Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Pseudo HE grenade waterLevel=%u, flags=%u.\n", pseudoGrenadeEntityVariables.waterLevel, pseudoGrenadeEntityVariables.flags);

//		entity->Print ("C:/Users/Эдуард/Desktop/Grenades/HE.log");

									Globals::g_halfLifeEngine->GiveNamedItem (entity->variables.owner, "weapon_hegrenade");
								}

								break;

							// Flashbang grenade....
							case 'f':
								// Reliability check.
								InternalAssert (g_server->IsRunning ());
								InternalAssert (entity->variables.owner->IsValid () && entity->variables.owner->IsNotWorldspawnPlayer ());

								if (modelName[10u] == 'l')
									g_server->GetThrownGrenadesManager ().PutThrownFlashBangGrenade (entity);

								{
									Utilities::PseudoGrenadeEntityVariables_t pseudoGrenadeEntityVariables;

		Utilities::TraceThrownGrenadeToss (entity, pseudoGrenadeEntityVariables);

//		Globals::g_halfLifeEngine->DrawLine (grenade->variables.origin, pseudoGrenadeEntityVariables.origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
//		Globals::g_halfLifeEngine->DrawCross3D (pseudoGrenadeEntityVariables.origin, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

									g_origins += pseudoGrenadeEntityVariables.origin;

									Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Pseudo FLASH grenade waterLevel=%u, flags=%u.\n", pseudoGrenadeEntityVariables.waterLevel, pseudoGrenadeEntityVariables.flags);

//		entity->Print ("C:/Users/Эдуард/Desktop/Grenades/FLASH.log");

									Globals::g_halfLifeEngine->GiveNamedItem (entity->variables.owner, "weapon_flashbang");
								}

								break;

							// Smoke grenade....
							case 's':
								// Reliability check.
								InternalAssert (g_server->IsRunning ());
								InternalAssert (entity->variables.owner->IsValid () && entity->variables.owner->IsNotWorldspawnPlayer ());

								if (modelName[10u] == 'm')
									g_server->GetThrownGrenadesManager ().PutThrownSmokeGrenade (entity);

								{
									Utilities::PseudoGrenadeEntityVariables_t pseudoGrenadeEntityVariables;

		Utilities::TraceThrownGrenadeToss (entity, pseudoGrenadeEntityVariables);

//		Globals::g_halfLifeEngine->DrawLine (grenade->variables.origin, pseudoGrenadeEntityVariables.origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
//		Globals::g_halfLifeEngine->DrawCross3D (pseudoGrenadeEntityVariables.origin, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

									g_origins += pseudoGrenadeEntityVariables.origin;

									Globals::g_halfLifeEngine->PrintFormat (SDK::Constants::HUDPrint_Talk, "Pseudo SMOKE grenade waterLevel=%u, flags=%u.\n", pseudoGrenadeEntityVariables.waterLevel, pseudoGrenadeEntityVariables.flags);

//		entity->Print ("C:/Users/Эдуард/Desktop/Grenades/SMOKE.log");

									Globals::g_halfLifeEngine->GiveNamedItem (entity->variables.owner, "weapon_smokegrenade");
								}

								break;
/*
							// C4 grenade....
							case 'c':
								// Reliability check.
								InternalAssert (g_server->IsRunning ());
								InternalAssert (entity->variables.owner->IsValid () && entity->variables.owner->IsNotWorldspawnPlayer ());

								if (modelName[10u] == '4')
									g_server->BombPlanted (entity);

								break;*/
						}

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void LightStyle (const int style, char *const value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());

						// Update light style for fake clients....
						g_server->GetFakeClientManager ()->LightStyle (static_cast <const unsigned char> (style), value);

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void MessageBegin (const SDK::Constants::MessageDestination_t destination, const unsigned int id, const Math::Vector3D *const origin, SDK::Classes::Edict *const to)
					{
						// this function called each time a message is about to sent.

//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability checks.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!
						InternalAssert (id < SDK::Constants::MaximumUserMessages);

						g_server->GetGameMessageHandlerManager ().MessageBegin (destination, static_cast <unsigned char> (id), origin, to);

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void MessageEnd (void)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().MessageEnd ();

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteByte (const int value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteByte (static_cast <const unsigned char> (value));

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteChar (const int value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteCharacter (static_cast <const char> (value));

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteShort (const int value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteShort (static_cast <const short> (value));

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteLong (const int value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteLong (value);

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteAngle (const float value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteAngle (value);

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteCoord (const float value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteCoord (value);

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteString (const char *const string)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteString (string);
/*
						static YaPB *bot;

						bot = g_botManager->FindOneValidAliveBot ();

						if (bot != NULL)
							bot->HandleChatterMessage (string);
*/
//						DEBUG_FUNCTION_TRACING_END ();
					}

					void WriteEntity (const int value)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability check.
						InternalAssert (g_server->IsRunning ());	// OCCURS!!!!!!!!!!!!!!!!!!!

						g_server->GetGameMessageHandlerManager ().WriteEntity (static_cast <const unsigned short> (value));

//						DEBUG_FUNCTION_TRACING_END ();
					}

					void SetView (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity)
					{
//						DEBUG_FUNCTION_TRACING_START ();

						// Reliability checks.
						InternalAssert (g_server->IsRunning ());
						InternalAssert (client->IsValid ());
						InternalAssert (viewEntity->IsValid ());

						Client *const clientPointer (g_server->GetClientManager ()->GetClient (client));

						// Reliability check.
						InternalAssert (clientPointer->IsValid ());

						if (clientPointer->IsFakeClient () && clientPointer->GetViewEdict () != viewEntity)
						{
							// Update view entity for fake client....
							clientPointer->GetFakeClientPointer ()->SetView (viewEntity);
						}

//						DEBUG_FUNCTION_TRACING_END ();
					}
				}
			}
		}

		namespace ReplacedFunctions
		{
			namespace EngineTable
			{
				void pfnSetModel (SDK::Classes::Edict *entity, const char *modelName)
				{
					// The purpose of this function is to ask the engine to set a render model 'modelName' (like "models/
					// mechgibs.mdl") to entity 'entity'. The engine itself will provide the necessary changes in the
					// entity's entvars structure for it, and clients will be updated, provided they get this
					// entity in their baselines, so that they can display it properly. If you don't know what
					// a "baseline" is all about, have a look around the CreateBaseline() functions in dll.cpp

					// Pre call original function....
					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnSetModel) (entity, modelName);

					Callbacks::EngineTable::Post::SetEntityModel (entity, modelName);
				}

				void pfnChangeLevel (const char *const s1, const char *const s2)
				{
					// the purpose of this function is to ask the engine to shutdown the server and restart a
					// new one running the map whose name is s1. It is used ONLY IN SINGLE PLAYER MODE and is
					// transparent to the user, because it saves the player state and equipment and restores it
					// back in the new level. The "changelevel trigger point" in the old level is linked to the
					// new level's spawn point using the s2 string, which is formatted as follows: "trigger_name
					// to spawnpoint_name", without spaces (for example, "tr_1atotr_2lm" would tell the engine
					// the player has reached the trigger point "tr_1a" and has to spawn in the next level on the
					// spawn point named "tr_2lm".

					Callbacks::EngineTable::ChangeLevel (s1, s2);

					// Post call original function....
//					(*Globals::g_halfLifeEngine->ChangeLevelOriginal) (s1, s2);
				}

				void pfnEmitSound (SDK::Classes::Edict *entity, SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, SDK::Constants::SoundFlag_t flags, SDK::Constants::SoundPitch_t pitch)
				{
					// this function tells the engine that the entity pointed to by 'entity', is emitting a sound
					// which filename is 'sample', at level 'channel' ('CHAN_VOICE', etc...), with 'volume' as
					// loudness multiplicator (normal volume 'VOL_NORM' is 1.0), with a pitch of 'pitch' (normal
					// pitch 'PITCH_NORM' is 100.0), and that this sound has to be attenuated by distance in air
					// according to the value of 'attenuation' (normal attenuation 'ATTN_NORM' is 0.8; 'ATTN_NONE'
					// means no attenuation with distance). Optionally flags 'flags' can be passed, which I don't
					// know the heck of the purpose. After we tell the engine to emit the sound, we have to call
					// SoundAttachToThreat() to bring the sound to the ears of the bots. Since bots have no client.DLL
					// to handle this for them, such a job has to be done manually.

					Callbacks::EngineTable::EmitSound (entity, channel, sample, volume, attenuation, flags, pitch);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnEmitSound) (entity, channel, sample, volume, attenuation, flags, pitch);
				}

				void pfnClientCommand (SDK::Classes::Edict *client, char *format, ...)
				{
					// this function forces the client whose player entity is 'client' to issue a client command.
					// How it works is that clients all have a 'g_server->GetClientManager ()->GetFakeClientCommand()' global string in their client.DLL that
					// stores the command string; if ever that string is filled with characters, the client.DLL
					// sends it to the engine as a command to be executed. When the engine has executed that
					// command, this 'g_server->GetClientManager ()->GetFakeClientCommand()' string is reset to zero. Here is somehow a curious implementation of
					// ClientCommand: the engine sets the command it wants the client to issue in his 'g_server->GetClientManager ()->GetFakeClientCommand()', then
					// the client.DLL sends it back to the engine, the engine receives it then executes the
					// command therein. Don't ask me why we need all this complicated crap. Anyhow since bots have
					// no client.DLL, be certain never to call this function upon a bot entity, else it will just
					// make the server crash. Since hordes of uncautious, not to say stupid, programmers don't
					// even imagine some players on their servers could be bots, this check is performed less than
					// sometimes actually by their side, that's why we strongly recommend to check it here too. In
					// case it's a bot asking for a client command, we handle it like we do for bot commands, i.e. using FakeClientCommand().

					// DO WE NEED THIS??????????????????? - ANSWER: "NO" - ENGINE CHECKS WHERE 'client' IS BOT!

					char buffer[1024u];

					// Concatenate all the arguments in one string....
					COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, buffer, sizeof (buffer));
/*
					// is the target entity an official bot, or a third party bot?
					if (client->IsPlayerFakeClient () || (client->variables.flags & Constants::FL_DORMANT))
					{
						// prevent bots to be forced to issue client commands

						if (MetaModEngine::Globals::g_metaModEngine != NULL)
							MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

						return;
					}
*/
					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnClientCommand) (client, buffer);
				}

				void pfnLightStyle (const int style, char *const value)
				{
					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnLightStyle) (style, value);

					// POST call our callback....
					Callbacks::EngineTable::Post::LightStyle (style, value);
				}

				void pfnMessageBegin (const SDK::Constants::MessageDestination_t destination, const unsigned int id, const Math::Vector3D *const origin, SDK::Classes::Edict *const to)
				{
					// this function called each time a message is about to sent.

					Callbacks::EngineTable::Post::MessageBegin (destination, id, origin, to);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnMessageBegin) (destination, id, origin, to);
				}

				void pfnMessageEnd (void)
				{
					Callbacks::EngineTable::Post::MessageEnd ();

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnMessageEnd) ();
				}

				void pfnWriteByte (int value)
				{
					Callbacks::EngineTable::Post::WriteByte (value);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteByte) (value);
				}

				void pfnWriteChar (int value)
				{
					Callbacks::EngineTable::Post::WriteChar (value);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteChar) (value);
				}

				void pfnWriteShort (int value)
				{
					Callbacks::EngineTable::Post::WriteShort (value);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteShort) (value);
				}

				void pfnWriteLong (int value)
				{
					Callbacks::EngineTable::Post::WriteLong (value);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteLong) (value);
				}

				void pfnWriteAngle (float value)
				{
					Callbacks::EngineTable::Post::WriteAngle (value);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteAngle) (value);
				}

				void pfnWriteCoord (float value)
				{
					Callbacks::EngineTable::Post::WriteCoord (value);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteCoord) (value);
				}

				void pfnWriteString (const char *const string)
				{
					Callbacks::EngineTable::Post::WriteString (string);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteString) (string);
				}

				void pfnWriteEntity (int value)
				{
					Callbacks::EngineTable::Post::WriteEntity (value);

					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnWriteEntity) (value);
				}

				int pfnRegUserMsg (const char *name, int size)
				{
					// this function registers a "user message" by the engine side. User messages are network
					// messages the game.DLL asks the engine to send to clients. Since many MODs have completely
					// different client features (Counter-Strike has a radar and a timer, for example), network
					// messages just can't be the same for every MOD. Hence here the MOD DLL tells the engine,
					// "Hey, you have to know that I use a network message whose name is 'name' and it is 'size'
					// packets long". The engine books it, and returns the ID number under which he recorded that
					// custom message. Thus every time the MOD DLL will be wanting to send a message named 'name'
					// using pfnMessageBegin(), it will know what message ID number to send, and the engine will
					// know what to do.

					// register the message and return the registered message ID number the engine gave us back.
					return Callbacks::EngineTable::RegisterUserMessage (name, static_cast <short> (size));
				}

				void pfnClientPrintf (SDK::Classes::Edict *client, SDK::Constants::PrintType_t printType, const char *message)
				{
					// this function prints the text message string pointed to by 'message' by the client side of
					// the client entity pointed to by 'client', in a manner depending of 'printType' ('PrintType_AtConsole',
					// 'PrintType_AtCenter' or 'PrintType_AtChat'). Be certain never to try to feed a bot with this function,
					// as it will crash your server. Why would you, anyway? bots have no client DLL as far as
					// we know, right? But since stupidity rules this world, we do a preventive check :)

					// DO WE NEED THIS???????????????????
/*
					// is this message for a bot?
					if (client->IsPlayerFakeClient ())
					{
						// disallow client printings for bots

						if (MetaModEngine::Globals::g_metaModEngine != NULL)
							MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

						return;
					}
*/
					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnClientPrintf) (client, printType, message);
				}

				const char *pfnCmd_Args (void)
				{
					// this function returns a pointer to the whole current client command string. Since bots
					// have no client DLL and we may want a bot to execute a client command, we had to implement
					// a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bot commands, and also keep track of the
					// argument count. Hence this hook not to let the engine ask an unexistent client DLL for a
					// command we are holding here. Of course, real clients commands are still retrieved the
					// normal way, by asking the engine.

					return Callbacks::EngineTable::Cmd_Args ();
				}

				const char *pfnCmd_Argv (unsigned int argc)
				{
					// this function returns a pointer to a certain argument of the current client command. Since
					// bots have no client DLL and we may want a bot to execute a client command, we had to
					// implement a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bot commands, and also keep
					// track of the argument count. Hence this hook not to let the engine ask an unexistent client
					// DLL for a command we are holding here. Of course, real clients commands are still retrieved
					// the normal way, by asking the engine.

					return Callbacks::EngineTable::Cmd_Argv (static_cast <unsigned char> (argc));
				}

				const unsigned int pfnCmd_Argc (void)
				{
					// this function returns the number of arguments the current client command string has. Since
					// bots have no client DLL and we may want a bot to execute a client command, we had to
					// implement a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bots' commands, and also keep
					// track of the argument count. Hence this hook not to let the engine ask an unexistent client
					// DLL for a command we are holding here. Of course, real clients commands are still retrieved
					// the normal way, by asking the engine.

					return Callbacks::EngineTable::Cmd_Argc ();
				}

				void pfnSetView (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity)
				{
					(*Globals::g_halfLifeEngine->m_engineFunctionTable.pfnSetView) (client, viewEntity);

					Callbacks::EngineTable::Post::SetView (client, viewEntity);
				}
			}
		}
	}
}