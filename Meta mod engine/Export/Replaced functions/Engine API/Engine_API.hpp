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
// Engine_API.hpp
//
// Description: Prototypes for Half-Life engine functions.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef META_MOD_ENGINE_EXPORT_REPLACED_FUNCTIONS_ENGINE_API_ENGINE_API_INCLUDED
#define META_MOD_ENGINE_EXPORT_REPLACED_FUNCTIONS_ENGINE_API_ENGINE_API_INCLUDED

inline void pfnEmitSound (HalfLifeEngine::SDK::Classes::Edict *entity, HalfLifeEngine::SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, HalfLifeEngine::SDK::Constants::SoundFlag_t flags, HalfLifeEngine::SDK::Constants::SoundPitch_t pitch)
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

	HalfLifeEngine::Export::Callbacks::EngineTable::EmitSound (entity, channel, sample, volume, attenuation, flags, pitch);

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}

inline void pfnClientCommand (HalfLifeEngine::SDK::Classes::Edict * /*client*/, char * /*format*/, ...)
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

	// DO WE NEED THIS???????????????????
/*
	char buffer[1024u];

	// Concatenate all the arguments in one string....
	COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, buffer, sizeof (buffer));
/*
	// is the target entity an official bot, or a third party bot?
	if (client->IsPlayerFakeClient () || (client->variables.flags & Constants::FL_DORMANT))
	{
		// prevent bots to be forced to issue client commands

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_SUPERCEDE;

		return;
	}
*/
	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}

inline int pfnRegUserMsg (const char *name, int size)
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

	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_SUPERCEDE;

	// register the message and return the registered message ID number the engine gave us back.
	return HalfLifeEngine::Export::Callbacks::EngineTable::RegisterUserMessage (name, static_cast <short> (size));
}

inline void pfnClientPrintf (HalfLifeEngine::SDK::Classes::Edict * /*client*/, HalfLifeEngine::SDK::Constants::PrintType_t/* printType*/, const char * /*message*/)
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

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_SUPERCEDE;

		return;
	}
*/
	Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
}

inline const char *pfnCmd_Args (void)
{
	// this function returns a pointer to the whole current client command string. Since bots
	// have no client DLL and we may want a bot to execute a client command, we had to implement
	// a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bot commands, and also keep track of the
	// argument count. Hence this hook not to let the engine ask an unexistent client DLL for a
	// command we are holding here. Of course, real clients commands are still retrieved the
	// normal way, by asking the engine.

	MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

	return HalfLifeEngine::Export::Callbacks::EngineTable::Cmd_Args ();
}

inline const char *pfnCmd_Argv (unsigned int argc)
{
	// this function returns a pointer to a certain argument of the current client command. Since
	// bots have no client DLL and we may want a bot to execute a client command, we had to
	// implement a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bot commands, and also keep
	// track of the argument count. Hence this hook not to let the engine ask an unexistent client
	// DLL for a command we are holding here. Of course, real clients commands are still retrieved
	// the normal way, by asking the engine.

	MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

	return HalfLifeEngine::Export::Callbacks::EngineTable::Cmd_Argv (static_cast <unsigned char> (argc));
}

inline const unsigned int pfnCmd_Argc (void)
{
	// this function returns the number of arguments the current client command string has. Since
	// bots have no client DLL and we may want a bot to execute a client command, we had to
	// implement a 'g_server->GetClientManager ()->GetFakeClientCommand()' string in the bot DLL for holding the bots' commands, and also keep
	// track of the argument count. Hence this hook not to let the engine ask an unexistent client
	// DLL for a command we are holding here. Of course, real clients commands are still retrieved
	// the normal way, by asking the engine.

	MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_SUPERCEDE;

	return HalfLifeEngine::Export::Callbacks::EngineTable::Cmd_Argc ();
}

namespace Post
{
	inline void pfnSetModel (HalfLifeEngine::SDK::Classes::Edict *entity, const char *modelName)
	{
		// The purpose of this function is to ask the engine to set a render model 'modelName' (like "models/
		// mechgibs.mdl") to entity 'entity'. The engine itself will provide the necessary changes in the
		// entity's entvars structure for it, and clients will be updated, provided they get this
		// entity in their baselines, so that they can display it properly. If you don't know what
		// a "baseline" is all about, have a look around the CreateBaseline() functions in dll.cpp

		HalfLifeEngine::Export::Callbacks::EngineTable::Post::SetEntityModel (entity, modelName);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnLightStyle (const int style, char *const value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::LightStyle (style, value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnMessageBegin (const HalfLifeEngine::SDK::Constants::MessageDestination_t destination, const unsigned int id, const Math::Vector3D *const origin, HalfLifeEngine::SDK::Classes::Edict *const to)
	{
		// this function called each time a message is about to sent.

		HalfLifeEngine::Export::Callbacks::EngineTable::Post::MessageBegin (destination, id, origin, to);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnMessageEnd (void)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::MessageEnd ();

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteByte (int value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteByte (value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteChar (int value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteChar (value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteShort (int value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteShort (value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteLong (int value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteLong (value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteAngle (float value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteAngle (value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteCoord (float value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteCoord (value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteString (const char *const string)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteString (string);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnWriteEntity (int value)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::WriteEntity (value);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}

	inline void pfnSetView (const HalfLifeEngine::SDK::Classes::Edict *const client, const HalfLifeEngine::SDK::Classes::Edict *const viewEntity)
	{
		HalfLifeEngine::Export::Callbacks::EngineTable::Post::SetView (client, viewEntity);

		Globals::g_metaModEngine->m_globalVariables->returnFlag = SDK::Constants::MRES_IGNORED;
	}
}

#endif	// ifndef META_MOD_ENGINE_EXPORT_REPLACED_FUNCTIONS_ENGINE_API_ENGINE_API_INCLUDED