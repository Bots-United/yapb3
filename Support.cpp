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
// Support.cpp
//
// Version: $ID:$
//

#include <Core.hpp>

const bool GetWaveSoundInformation (const DynamicString &filename, DynamicString &output)
{
	// This function returns the length of given WAVE filename.

	// open the wave file with binary read mode
	STDIOFile wave (filename, "rb");

	output.MakeEmpty ();

	// check if got valid handle
	if (!wave.IsValid ())
	{
		output.AssignFormat ("Wave File '%s' not found!", filename.GetData ());

		// if not found, then give up
		return false;
	}

	// WAVE file related structure definitions
	WaveFileHeader_t waveFileHeader;

	// try to read the file
	if (!wave.Read (waveFileHeader))
	{
		output.AssignFormat ("Wave File '%s' - has wrong or unsupported format!", filename.GetData ());

		// avoid buffer overflows
		return false;
	}

	// check the RIFF chunk ID
	if (strncmp (waveFileHeader.riffChunkID, "RIFF", GET_STATIC_STRING_LENGTH ("RIFF")) != 0)
	{
		output.AssignFormat ("Wave File '%s' - \"RIFF\" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return false;
	}

	// check the WAVE chunk ID
	if (strncmp (waveFileHeader.waveChunkID, "WAVE", GET_STATIC_STRING_LENGTH ("WAVE")) != 0)
	{
		output.AssignFormat ("Wave File '%s' - \"WAVE\" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return false;
	}

	// check the format chunk ID
	if (strncmp (waveFileHeader.formatChunkID, "fmt ", GET_STATIC_STRING_LENGTH ("fmt ")) != 0)
	{
		output.AssignFormat ("Wave File '%s' - \"fmt \" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return false;
	}

	// check the format chunk ID
	if (waveFileHeader.format.tag != WAVE_FORMAT_PCM)
	{
		output.AssignFormat ("Wave File '%s' - format tag should be Microsoft PCM format only!", filename.GetData ());

		// avoid buffer overflows
		return false;
	}

	// check the data chunk ID
	if (strncmp (waveFileHeader.dataChunkID, "data", GET_STATIC_STRING_LENGTH ("data")) != 0)
	{
		output.AssignFormat ("Wave File '%s' - \"data\" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return false;
	}

	// check the bytes per second value
	if (waveFileHeader.format.bytesPerSecond == 0u)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - invalid bytes per second value (%u)!", filename.GetData (), waveFileHeader.format.bytesPerSecond);

		// avoid buffer overflows
		return 0.0f;
	}

	// is that file of an unknown format?
	if (waveFileHeader.format.bytesPerSample != 1u && waveFileHeader.format.bytesPerSample != 2u)
	{
		output.AssignFormat ("Wave File '%s' - invalid bytes per sample value (%u)!", filename.GetData (), waveFileHeader.format.bytesPerSample);

		// avoid buffer overflows
		return false;
	}

	if (waveFileHeader.dataChunkLength == 0u)
	{
		output.AssignFormat ("Wave File '%s' - has zero data chunk length!", filename.GetData ());

		// avoid buffer overflows
		return false;
	}

	float secondLength (static_cast <float> (waveFileHeader.dataChunkLength) / static_cast <float> (waveFileHeader.format.bytesPerSecond));

	if (Math::IsZero (secondLength))
	{
		char ch[32u];

		_snprintf (ch, sizeof (ch), "0.%u", waveFileHeader.dataChunkLength);

		// else return milli second length.
		secondLength = static_cast <float> (atof (ch));
	}

	output.AssignFormat
	(
		"Wave file \"%s\" header:\n"
		"\triffChunkID: \"%s\"\n"
		"\tpackageSize = %u\n\n"

		"\twaveChunkID: \"%s\"\n"
		"\tformatChunkID: \"%s\"\n"
		"\tformatChunkLength = %u\n\n"

		"\tformatTag = %u\n"
		"\tchannels = %u\n"
		"\tsampleRate = %u\n"
		"\tbytesPerSecond = %u\n"
		"\tbytesPerSample = %u\n"
		"\tbitsPerSample = %u\n\n"

		"\tdataChunkID: \"%s\"\n"
		"\tdataChunkLength = %u\n"

		"Other information:\n"
		"\tDuration = %.2f\n",

		filename.GetData (),

		waveFileHeader.riffChunkID,
		waveFileHeader.packageSize,

		waveFileHeader.waveChunkID,
		waveFileHeader.formatChunkID,
		waveFileHeader.formatChunkLength,

		waveFileHeader.format.tag,
		waveFileHeader.format.channels,
		waveFileHeader.format.sampleRate,
		waveFileHeader.format.bytesPerSecond,
		waveFileHeader.format.bytesPerSample,
		waveFileHeader.format.bitsPerSample,

		waveFileHeader.dataChunkID,
		waveFileHeader.dataChunkLength,

		secondLength
	);

	return true;
}
const float GetWaveSoundPlayDuration (const DynamicString &filename)
{
	// This function returns the length of given WAVE filename.

	// open the wave file with binary read mode
	STDIOFile wave (filename, "rb");

	// check if got valid handle
	if (!wave.IsValid ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' not found!", filename.GetData ());

		// if not found, then give up
		return 0.0f;
	}

	// WAVE file related structure definitions
	WaveFileHeader_t waveFileHeader;

	// try to read the file
	if (!wave.Read (waveFileHeader))
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - has wrong or unsupported format!", filename.GetData ());

		// avoid buffer overflows
		return 0.0f;
	}

	// check the RIFF chunk ID
	if (strncmp (waveFileHeader.riffChunkID, "RIFF", GET_STATIC_STRING_LENGTH ("RIFF")) != 0)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - \"RIFF\" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return 0.0f;
	}

	// check the WAVE chunk ID
	if (strncmp (waveFileHeader.waveChunkID, "WAVE", GET_STATIC_STRING_LENGTH ("WAVE")) != 0)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - \"WAVE\" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return 0.0f;
	}

	// check the format chunk ID
	if (strncmp (waveFileHeader.formatChunkID, "fmt ", GET_STATIC_STRING_LENGTH ("fmt ")) != 0)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - \"fmt \" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return 0.0f;
	}

	// check the format chunk ID
	if (waveFileHeader.format.tag != WAVE_FORMAT_PCM)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - format tag should be Microsoft PCM format only!", filename.GetData ());

		// avoid buffer overflows
		return 0.0f;
	}

	// check the data chunk ID
	if (strncmp (waveFileHeader.dataChunkID, "data", GET_STATIC_STRING_LENGTH ("data")) != 0)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - \"data\" chunk ID not found!", filename.GetData ());

		// avoid buffer overflows
		return 0.0f;
	}

	// check the bytes per second value
	if (waveFileHeader.format.bytesPerSecond == 0u)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - invalid bytes per second value (%u)!", filename.GetData (), waveFileHeader.format.bytesPerSecond);

		// avoid buffer overflows
		return 0.0f;
	}

	// is that file of an unknown format?
	if (waveFileHeader.format.bytesPerSample != 1u && waveFileHeader.format.bytesPerSample != 2u)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - invalid bytes per sample value (%u)!", filename.GetData (), waveFileHeader.format.bytesPerSample);

		// avoid buffer overflows
		return 0.0f;
	}

	if (waveFileHeader.dataChunkLength == 0u)
	{
		AddLogEntry (true, LogLevel_Error, false, "Wave File '%s' - has zero data chunk length!", filename.GetData ());

		// avoid buffer overflows
		return 0.0f;
	}

	// now return the duration
	return static_cast <float> (waveFileHeader.dataChunkLength) / static_cast <float> (waveFileHeader.format.bytesPerSecond);
/*
	const float secondLength (static_cast <float> (waveFileHeader.dataChunkLength) / static_cast <float> (waveFileHeader.format.bytesPerSecond));

	if (!Math::IsZero (secondLength))
		return secondLength;

	char ch[32u];

	_snprintf (ch, sizeof (ch), "0.%u", waveFileHeader.dataChunkLength);

	// else return milli second length.
	return static_cast <float> (atof (ch));*/
}

void AddLogEntry (const bool outputToConsole, const LogLevel_t logLevel, const bool/* ignoreLogLevel*/, const char *const format, ...)
{
	// This function logs a message to the message log file root directory.

	char logBuffer[1024u], logLine[1024u];

	// Concatenate all the arguments in one string....
	COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, logBuffer, sizeof (logBuffer));

	switch (logLevel)
	{
		// default log message
		case LogLevel_Default:
			g_log.WriteDefaultMessage (logBuffer);

			break;

		// note log message
		case LogLevel_Note:
			g_log.WriteNoteMessage (logBuffer);

			break;

		// warning log message
		case LogLevel_Warning:
			g_log.WriteWarningMessage (logBuffer);

			break;

		// error log message
		case LogLevel_Error:
			g_log.WriteErrorMessage (logBuffer);

			break;

		// critical error log message
		case LogLevel_Critical:
			g_log.WriteCriticalErrorMessage (logBuffer);

			break;

		// error in the code!!!
		default:
			strcpy (logLine, "AddLogEntry(): Error - unknown log level!\n");

			break;
	}
//#if 0
	switch (logLevel)
	{
		// default log message
		case LogLevel_Default:
			strcpy (logLine, "Log: ");
			strncat (logLine, logBuffer, sizeof (logLine));
			strncat (logLine, "\n", sizeof (logLine));

			break;

		// note log message
		case LogLevel_Note:
			strcpy (logLine, "Note: ");
			strncat (logLine, logBuffer, sizeof (logLine));
			strncat (logLine, "\n", sizeof (logLine));

			break;

		// warning log message
		case LogLevel_Warning:
			strcpy (logLine, "Warning: ");
			strncat (logLine, logBuffer, sizeof (logLine));
			strncat (logLine, "\n", sizeof (logLine));

			break;

		// error log message
		case LogLevel_Error:
			strcpy (logLine, "Error: ");
			strncat (logLine, logBuffer, sizeof (logLine));
			strncat (logLine, "\n", sizeof (logLine));

			break;

		// critical error log message
		case LogLevel_Critical:
			strcpy (logLine, "Critical: ");
			strncat (logLine, logBuffer, sizeof (logLine));
			strncat (logLine, "\n", sizeof (logLine));

			break;

		// error in the code!!!
		default:
			strcpy (logLine, "AddLogEntry(): Error - unknown log level!\n");

			break;
	}

	if (outputToConsole && HalfLifeEngine::Globals::g_halfLifeEngine != NULL)
	{
		// print to console
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (PRODUCT_LOG_TAG " ");
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (logLine);
	}
#if 0
	// now check if logging disabled
	if
	(
		!ignoreLogLevel &&
		(
			(logLevel == LogLevel_Default && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < 3u) ||	// no log, default logging is disabled
			(logLevel == LogLevel_Warning && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < 2u) ||	// no log, warning logging is disabled
			(logLevel == LogLevel_Error && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < 1u)		// no log, error logging is disabled
		)
	)
		return;

	const DynamicString logDirectory (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Logs");

	if (!IsDirectoryExists (logDirectory) && !CreatePath (logDirectory))
	{
		strcpy (logBuffer, "Unable to create the logs directory: \"%s\"!", logDirectory.GetData ());

		goto Label_TerminateGame;
	}

	// try to open log file in a standard stream
	STDIOFile file (logDirectory + "/Log.log", "at");

	// check if we got a valid handle
	if (file.IsValid ())
	{
		const tm *const localTime (GetLocalTime ());

		// dump the string into the file
		file.PrintFormat ("[%02u:%02u:%02u] %s", localTime->tm_hour, localTime->tm_min, localTime->tm_sec, logLine);
	}
	else
	{
		strcpy (logBuffer, "Unable to open the log file for writing!");

		goto Label_TerminateGame;
	}

	if (logLevel != LogLevel_Critical)
		return;

	Label_TerminateGame:
	{
		// Free everything that's freeable
//		FreeAllMemory ();	// AUTOMATICLY CALLED BY DllMain()

		DestroyWindow (GetForegroundWindow ());																																					\

		// once everything is freed, just exit and print to message box
		MessageBox (NULL, logBuffer, "YaPB Critical Error", MB_ICONERROR | MB_TOPMOST | MB_SYSTEMMODAL);

		ExitProcess (1u);
	}
#endif	// if 0
}

const bool OpenConfigurationFile (const DynamicString &filename, const DynamicString &errorIfNotExists, BaseFile &outFile)
{
	// Reliability check.
	if (outFile.IsValid ())
		outFile.Close ();
/*
	if (filename.CompareWithoutCaseCheck (LOCALIZER_CONFIG_FILENAME) == 0)
	{
		const DynamicString fullFilename (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString () + '_' + filename + ".cfg");

		AddLogEntry (true, LogLevel_Default, false, "Trying to open configuration file %s for current language \"%s\"....", fullFilename.GetData (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData ());

		// check is file is exists for this language
		outFile.Open (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Configs/Languages/" + fullFilename);
	}
	else */if (filename.CompareWithoutCaseCheck (PROFILE_MANAGER_CONFIG_FILENAME) == 0 || filename.CompareWithoutCaseCheck ("chatter") == 0)
	{
		const DynamicString fullFilename (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString () + '_' + filename + ".cfg");

		AddLogEntry (true, LogLevel_Default, false, "Trying to open configuration file %s for current language \"%s\"....", fullFilename.GetData (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData ());

		const DynamicString languageDependantConfigurationFile (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Configs/Languages/" + fullFilename);

		// check is file is exists for this language
		if (STDIOFile::IsExists (languageDependantConfigurationFile, "rb"))
			outFile.Open (languageDependantConfigurationFile);
		else if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().CompareWithoutCaseCheck (g_localizer.GetDefaultLanguageName ()) != 0)
		{
			AddLogEntry (true, LogLevel_Error, false, "Configuration file %s not found for current language \"%s\", trying to open this file for basic language (\"%s\")....", fullFilename.GetData (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Language)->GetString ().GetData (), g_localizer.GetDefaultLanguageName ().GetData ());

			outFile.Open (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Configs/Languages/" + g_localizer.GetDefaultLanguageName () + '_' + filename + ".cfg");
		}
	}
	else
	{
		const DynamicString fullFilename (filename + ".cfg");

		AddLogEntry (true, LogLevel_Default, false, "Trying to open configuration file %s....", fullFilename.GetData ());

		outFile.Open (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Configs/" + fullFilename);
	}

	if (!outFile.IsValid ())
	{
		AddLogEntry (true, LogLevel_Error, false, errorIfNotExists);

		return false;
	}

	return true;
}

void FreeAllMemory (void)
{
	// this function is in charge of freeing all the memory space we allocated, because the DLL
	// is going to shutdown. Of course a check is made upon the space pointer we want to free not
	// to free it twice in case it would have already been done (malloc and free implementations
	// are usually so crappy they hardly ever give error messages, rather crash without warning).
	// For safety reasons, we also reset the pointer to NULL, in order not to try to access it
	// later. Yeah, this should never happen, but who knows. Note, it is always safest to free
	// things in the reverse order they have been allocated, because of interdependency reasons
	// (for example, the map topology hashtable relies on the map walkfaces array). Hence we
	// start by freeing the bots data, then we move on the global stuff.

	// Deactivate the server and free the all used memory.
	{
		delete g_server;

		g_server = NULL;
	}

	#if defined MULTIPLE_DRAW_METHODS
		// Free draw interface....
		{
			delete g_drawInterface;

			g_drawInterface = NULL;
		}
	#endif	// if defined MULTIPLE_DRAW_METHODS

	{
		delete MetaModEngine::Globals::g_metaModEngine;

		MetaModEngine::Globals::g_metaModEngine = NULL;
	}

	{
		delete HalfLifeEngine::Globals::g_halfLifeEngine;

		HalfLifeEngine::Globals::g_halfLifeEngine = NULL;
	}

//	MetaMod::FreeNewExportFunctions (g_YaPBModuleHandle);

//	HalfLifeEngine::SDK::Voice::Voice_Deinit ();
}

void RestoreAllHookedFunctions (void)
{
	// Restore all hooked functions....

	// Reliability checks.
	InternalAssert (GetModuleHandle ("hw.dll") != NULL);
	InternalAssert (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer () == GetModuleHandle ("hw.dll"));

	Console::Bot::variableManager.GetCalling_Con_Printf_VariableValueIsHooker ().UndoPatch ();
	Console::g_consoleVariableValueChangeHooker.UndoPatch ();

	if (g_server->IsRunning ())
		g_server->UnregisterAllForwardsAllHooks ();

	MetaModEngine::Globals::g_metaModEngine->UnregisterAllForwardsAndHooks ();
	HalfLifeEngine::Globals::g_halfLifeEngine->RestoreAllRedirectionsAndHooks ();
}

void InternalAttach (void)
{
	// Get blending interface, studio API, rotation and bone transformation matrixes:
	{
		const LibraryHandleWithSize &engineLibrary (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ());
		const unsigned char string[] = {"Server_GetBlendingInterface"};	// From SV_CheckBlendingInterface() function.
		unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (engineLibrary, string, GET_STATIC_STRING_LENGTH (string))));

		if (address != NULL)
		{
			address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (engineLibrary, address, Trampolines::Bytecode::Push32));

			if (address != NULL)
			{
				address += GET_STATIC_STRING_LENGTH
				(
					"\x68\xEC\xBC\xE7\x01"	// push    offset aServer_getblen ; "Server_GetBlendingInterface"
					"\x50"					// push    eax             ; hModule
					"\xFF\xD7"				// call    edi ; GetProcAddress ; Indirect Call Near Procedure
					"\x85\xC0"				// test    eax, eax        ; Logical Compare
					"\x74\x31"				// jz      short loc_1D68C8D ; Jump if Zero (ZF=1)
					"\x68"					// push
				);

				HalfLifeEngine::SDK::TypeDefinitions::BoneTransformMatrix_t *const boneTransform (*reinterpret_cast <HalfLifeEngine::SDK::TypeDefinitions::BoneTransformMatrix_t **> (address));

				address += Trampolines::Bytecode::Pointer32Size + sizeof (Trampolines::Bytecode::Push32);

				HalfLifeEngine::SDK::TypeDefinitions::RotationMatrix_t *const rotationMatrix (*reinterpret_cast <HalfLifeEngine::SDK::TypeDefinitions::RotationMatrix_t **> (address));

				address += Trampolines::Bytecode::Pointer32Size + sizeof (Trampolines::Bytecode::Push32);

				HalfLifeEngine::SDK::Structures::ServerStudioAPI_t *const studio (*reinterpret_cast <HalfLifeEngine::SDK::Structures::ServerStudioAPI_t **> (address));

				address += Trampolines::Bytecode::Pointer32Size + sizeof (Trampolines::Bytecode::Push32);

				HalfLifeEngine::SDK::Structures::ServerBlendingInterface_t **const blendInterface = *reinterpret_cast <HalfLifeEngine::SDK::Structures::ServerBlendingInterface_t ***> (address);

				address += Trampolines::Bytecode::Pointer32Size + sizeof (Trampolines::Bytecode::Push8);

				const int version (*address);

				HalfLifeEngine::Export::Server_GetBlendingInterface (version, blendInterface, studio, rotationMatrix, boneTransform);
			}
		}
	}

//	HalfLifeEngine::Export::Callbacks::DLLTable::GameDLLInit ();	// UNUSED! AND BTW METAMOD CALLED IT HIMSELF!!!

	// Register user messages:
	/// @todo OPTIMIZE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("AmmoPickup", 2);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("AmmoX", 2);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("CurWeapon", 3);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("Damage", 12);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("ScreenShake", 6);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("ScreenFade", 10);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("ShowMenu", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("StatusIcon", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("ReloadSound", 2);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("TeamInfo", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("TextMsg", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("VGUIMenu", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("BotProgress", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("BotVoice", 2);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("MOTD", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("SendAudio", -1);
		HalfLifeEngine::Globals::g_halfLifeEngine->RegisterUserMessage ("ServerName", -1);
	}

	HalfLifeEngine::SDK::Structures::Server_t &sv (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <HalfLifeEngine::SDK::Structures::Server_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), HalfLifeEngine::SDK::Constants::Server::signature, GET_STATIC_STRING_LENGTH (HalfLifeEngine::SDK::Constants::Server::signature)), HalfLifeEngine::SDK::Constants::Server::Offset_Pointer));
	const HalfLifeEngine::SDK::Structures::server_state_t sv_state_ORIGINAL (sv.state);
	sv.state = HalfLifeEngine::SDK::Structures::ss_loading;	// HACK!, or otherwise we got Error: "PF_precache_*_I: '%s' Precache can only be done in spawn functions"!!!

	// Initialize "worldspawn":
	{
		HalfLifeEngine::SDK::Classes::Edict *const worldspawnEdict (HalfLifeEngine::Globals::g_halfLifeEngine->GetWorldSpawnEdict ());

		if (worldspawnEdict != NULL && !worldspawnEdict->isFree && worldspawnEdict->privateData != NULL)
		{
			HalfLifeEngine::Export::Callbacks::DLLTable::EntitySpawn (worldspawnEdict);
			HalfLifeEngine::Export::Callbacks::DLLTable::Post::EntitySpawn (worldspawnEdict);
		}
	}

	sv.state = sv_state_ORIGINAL;	// Restore original value back....

	// Initialize other entities:
	for (unsigned short edictIndex (static_cast <unsigned short> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->maximumClients) + 1u); edictIndex < HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->maximumEntities; ++edictIndex)
	{
		HalfLifeEngine::SDK::Classes::Edict *const edict (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (edictIndex));

		if (edict == NULL || edict->isFree || edict->privateData == NULL)
			continue;

		HalfLifeEngine::Export::Callbacks::DLLTable::EntitySpawn (edict);
		HalfLifeEngine::Export::Callbacks::DLLTable::Post::EntitySpawn (edict);
	}

	HalfLifeEngine::Export::Callbacks::DLLTable::ServerActivate ();
	HalfLifeEngine::Export::Callbacks::DLLTable::Post::ServerActivate ();

	// Initialize clients: (After 'g_server->IsRunning()')
	{
		const char rejectReason[128u] = {"Connection rejected by game\n"};

		for (unsigned char clientEdictIndex (1u); clientEdictIndex <= HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->maximumClients; ++clientEdictIndex)
		{
			HalfLifeEngine::SDK::Classes::Edict *const clientEdict (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (clientEdictIndex));

			if (clientEdict == NULL || clientEdict->isFree || clientEdict->privateData == NULL || clientEdict->GetNetName ().IsEmpty ())
				continue;

			HalfLifeEngine::Export::Callbacks::DLLTable::ClientConnect (clientEdict, clientEdict->GetNetName (), "127.0.0.1"/*! @todo !!! */, rejectReason);
			HalfLifeEngine::Export::Callbacks::DLLTable::ClientPutInServer (clientEdict);
		}
	}
}

void InternalDetach (void)
{
	if (g_server->IsRunning () && g_server->GetYaPBManager ()->GetBotsCount () > 0u)
		g_server->GetYaPBManager ()->KickAll ();

	RestoreAllHookedFunctions ();
	FreeAllMemory ();
}

const bool IsBorderPlaneVisible (const Math::Vector3D &source, const Math::Vector3D &destination, const Math::Vector3D &mins, const Math::Vector3D &maxs, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore, HalfLifeEngine::SDK::Classes::Edict *const entityToSkip, const float multiplier, Math::Plane::Points_t &points)
{
	// Function checks 4 points on a plane made on an entity based on its bounding box (it is like 4 trace lines made to 4 different points calculated by the bounding box!)
	// Returns true if one point from the border-plane is visible.
	// Paramaters:
	// source - start point (this is the point where the trace starts!)
	// destination - end origin, from this origin the plane will be created in right, left, up and down
	// mins and maxs - are the bounding box of the target entity, use customs if you like
	// traceIgnore - ignore property (see trace line tutorial)
	// entityToSkip - ignore entity
	// multiplier - this is a multiplication constant, normally the plane has the size of the cube that surronds the entity. If this constant for example 0.5 then the plane is 1/2 times smaller

	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	Math::Vector3D ZnegOffset (0.0f, 0.0f, mins.z), ZpozOffset (0.0f, 0.0f, maxs.z);
	float wideOffset ((mins.x - maxs.x + mins.y - maxs.y) / Math::Plane::PointIndex_Total);

	if (ZpozOffset.z == 0.0f && ZnegOffset.z == 0.0f && wideOffset == 0.0f)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, traceIgnore, entityToSkip, traceResult);

		return traceResult.fraction < 1.0f;
	}
/*
	if (multiplier < 0.0f)
		multiplier = Math::fabsf (multiplier);
*/
	wideOffset *= multiplier;
	ZpozOffset.z *= multiplier;
	ZnegOffset.z *= multiplier;

	const Math::Angles3D angles ((source - destination).Normalize ().ToAngles3D ());

	angles.MakeVectors ();

	if (Math::fabsf (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.z) <= 0.7071f)
	{
		points[Math::Plane::PointIndex_0] = destination + ZpozOffset + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset;
		points[Math::Plane::PointIndex_1] = destination + ZpozOffset - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset;
		points[Math::Plane::PointIndex_2] = destination + ZnegOffset + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset;
		points[Math::Plane::PointIndex_3] = destination + ZnegOffset - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset;
	}
	else
	{
		if (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.z > 0.0f)
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.z = 0.0f;

			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.Normalize ();

			points[Math::Plane::PointIndex_0] = destination + ZpozOffset + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset - multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
			points[Math::Plane::PointIndex_1] = destination + ZpozOffset - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset - multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
			points[Math::Plane::PointIndex_2] = destination + ZnegOffset + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset + multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
			points[Math::Plane::PointIndex_3] = destination + ZnegOffset - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset + multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
		}
		else
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.z = 0.0f;

			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.Normalize ();

			points[Math::Plane::PointIndex_0] = destination + ZpozOffset + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset + multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
			points[Math::Plane::PointIndex_1] = destination + ZpozOffset - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset + multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
			points[Math::Plane::PointIndex_2] = destination + ZnegOffset + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset - multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
			points[Math::Plane::PointIndex_3] = destination + ZnegOffset - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right * wideOffset - multiplier * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * wideOffset;
		}
	}

	for (unsigned char index (Math::Plane::PointIndex_0); index < Math::Plane::PointIndex_Total; ++index)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, points[index], traceIgnore, entityToSkip, traceResult);

		if (traceResult.fraction == 1.0f)
			return true;
	}

	return false;
}
/*
inline const void *const HooksManager::GetFunction (const void *const privateData, const unsigned char virtualTableID, bool &isTrampoline)
{
	isTrampoline = false;
	const void *const function (MemoryUtilities::GetElementsTable <void *const> (privateData)[virtualTableID]);	// Get the virtual table entry....

	// Check to see if it's a trampoline
	// Iterate through the hooks for the id, see if the function is found
	for (unsigned char index (0u); index < m_virtualTableHooks.GetElementNumber (); ++index)
	{
		// If the function points to a trampoline, then return the original function.
		if (function != m_virtualTableHooks[index]->m_trampoline)
			continue;

		isTrampoline = true;

		return function;
	}

	// this is an original function
	return function;
}

inline void Call_Void_Void (const unsigned char virtualTableID, const HalfLifeEngine::SDK::Classes::Edict *const entity)
{
	const void *const privateData (entity->privateData);
	bool isTrampoline;
	const void *const function (GetFunction (privateData, virtualTableID, isTrampoline));

//	if (!isTrampoline && !gDoForwards)
//		gDoForwards = true;

	static_cast <void (__fastcall *) (void *, int)> (function) (privateData, 0);
}
*/
void HooksManager::RegisterEngineHookFromAddress (const unsigned int address, const bool/* isVoid*/, const unsigned char parametersNumber, const void *const callbackFunction)
{
	/*const */void * /*&*/originalFunctionAddress = HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetMemory <void *> (address);
//	/*const */void *originalFunctionAddress (HalfLifeEngine::Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnSpawn);
//	const void *&originalFunctionAddress = reinterpret_cast <const void *&> (HalfLifeEngine::Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable.pfnSpawn);

	// Reliability check.
	InternalAssert (originalFunctionAddress != NULL);

	// Check the list of this function's hooks, see if the function we have is a hook.
	InternalAssert (!IsMember (originalFunctionAddress));	// Yes, this function is hooked.

	// We've passed all tests...
	set_hook (/***static_cast <void ***> (*/originalFunctionAddress, callbackFunction, parametersNumber);
}

void HooksManager::RegisterEngineHook (const unsigned char *const pattern, const unsigned char length, const bool/* isVoid*/, const unsigned char/* parametersNumber*/, const void *const/* callbackFunction*/)
{
	// First, preprocess the signature
	const void *originalFunctionAddress (MemoryUtilities::FindPattern (*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary (), pattern, length));

	// Reliability check.
	InternalAssert (originalFunctionAddress != NULL);

	AddLogEntry (true, LogLevel_Default, true, "originalFunctionAddress=%x", originalFunctionAddress);
/*
	{
//find mp_autoteambalance
//subtract the return from the start
#define CSPLAYER_RESTARTROUND			"\x51\x53\x56\x8B\xF1\x8B\x0D\x2A\x2A\x2A\x2A\x8B\x01\xFF\x50\x1C\x8B\x0D\x2A\x2A\x2A\x2A\x33\xDB\x3B\xCB\x74\x05\xE8\x2A\x2A\x2A"
#define CSPLAYER_RESTARTROUND_BYTES		32
//there are 0x9DE bytes in this function.
//the last ones are a mov, 3 pops, and a retn
//that gives us, from the end...
#define CSPLAYER_RESTARTROUND_END		0x9DE
#define CSPLAYER_RESTARTROUND_PATCH_BYTES	10

struct RoundRestartPatch_t
{
	unsigned char *new_func;
	unsigned char  unPatch[CSPLAYER_RESTARTROUND_PATCH_BYTES];
} g_restartRoundPatch;

	unsigned char patch[6] = {'\xFF', '\x25', 0, 0, 0, 0}; //JMP *(0x0)
	const int newfunc_size = 7;
	unsigned char new_func[newfunc_size] =
				{  '\x60',		//PUSHAD
				   '\xE8', 0, 0, 0, 0,	//CALL
				   '\x61'		//POPAD
				};
	const int callgate_patch = 2;

	//allocate new func
	unsigned char *faddr = Detours::g_codeAllocator.Allocate <unsigned char> (newfunc_size + CSPLAYER_RESTARTROUND_PATCH_BYTES);
	//get src func ptr
	unsigned char *src_addr = (unsigned char *)originalFunctionAddress + (CSPLAYER_RESTARTROUND_END - CSPLAYER_RESTARTROUND_PATCH_BYTES + 1);

	//protect it
	Trampolines::VirtualProtect (src_addr, CSPLAYER_RESTARTROUND_PATCH_BYTES, PAGE_EXECUTE_READWRITE);

	//copy in the bytes needed
	g_restartRoundPatch.new_func = faddr;
	memcpy(faddr, new_func, newfunc_size);
	faddr += newfunc_size;
	memcpy(faddr, src_addr, CSPLAYER_RESTARTROUND_PATCH_BYTES);
	memcpy(g_restartRoundPatch.unPatch, src_addr, CSPLAYER_RESTARTROUND_PATCH_BYTES);
	//assemble the gate
	faddr = g_restartRoundPatch.new_func;
	faddr += callgate_patch;
	//eip is faddr + 4
	//target function is callbackFunction
	//:., we want:

	*(unsigned long *)faddr = (unsigned long)callbackFunction - (unsigned long)(faddr + 4);

	//gate is assembled, now patch it in
	faddr = patch;
	faddr += 2;

	*(unsigned char **)faddr = (unsigned char *)&g_restartRoundPatch.new_func;

	memcpy(src_addr, patch, sizeof(patch));
	}
*/
	// Check the list of this function's hooks, see if the function we have is a hook.
	InternalAssert (!IsMember (originalFunctionAddress));	// Yes, this function is hooked.

	// We've passed all tests...

//	memcpy((void*)originalBytes, originalFunctionAddress, patchSize);

//	static const int patchSize = sizeof (unsigned char) + sizeof (void */*long*/);
/*	unsigned char originalBytes[patchSize / sizeof(unsigned char)];
	unsigned char patchedBytes[patchSize / sizeof(unsigned char)];

	patchedBytes[0] = 0xE9;
	*((long*)(&patchedBytes[1])) = (char*)callbackFunction - (char*)originalFunctionAddress - patchSize;

	if (Trampolines::VirtualProtect (originalFunctionAddress, sizeof (patchSize), PAGE_EXECUTE_READWRITE))
		memcpy (originalFunctionAddress, (void*)patchedBytes, patchSize);*/
}