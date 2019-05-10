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
// Core.hpp
//
// Description: Contains the YaPB ALL Structures and Function Prototypes.
//
// Thanks to:
//	Jeffrey Broome alias botman - Author of HPB Bot.
//	Markus Klinge alias Count Floyd - Author of Ping of Death Bot (POD-Bot).
//	Pierre-Marie Baty - Author of Rational Autonomous Cybernetic Commandos AI (RACC Bot).
//	Wei Mingzhi alias Whistler - Original author of Yet another Ping of Death Bot (YaPB).
//	KWo - Current developer of Meta mod Ping of Death Bot (POD-Bot MM).
//	Jussi Kivilinna alias Ghost of Evilspy - Author of Metamod-P and jk-botti.
//	SamPlay - Author of extended BSPView program, creator of the great site: "http://www.gamebotfactory.com/gdtoolbox".
//	DaTa - My C++ teacher. :{P
//	Dmtiry Zhukov alias jeefo - Developer of Yet another Ping of Death Bot (YaPB).
//	AMXX Mod development team.
//	MeRcyLeZZ - Author of 'Ping Faker' AMXX plugin.
//	Quake engine development team.
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef YAPB_CORE_INCLUDED
#define YAPB_CORE_INCLUDED

// Costom settings:
//{
#define USE_SPEECH_ENGINES	// Comment this line to disable this.
#define MULTIPLE_DRAW_METHODS	// Comment this line to disable this.
//#define MULTIPLE_MSEC_METHODS	// Comment this line to disable this.

#if defined _DEBUG
#	define MEMORY_MANAGER	// Comment this line to disable the memory manager.

	// If DEBUG_NAV_NODES is true, nav_show_nodes controls drawing node positions, and
	// nav_show_node_id allows you to show the IDs of nodes that didn't get used to create areas.
#	define DEBUG_NAV_NODES	// Comment this line to disable navigation nodes debug.
#endif	// if defined _DEBUG
//}

// detects the build platform
#if !defined _WIN32
#	error "Platform unrecognized!"
#endif	// if !defined _WIN32

// detects the compiler
#if defined _MSC_VER
#	define COMPILER_VISUALC	_MSC_VER
#elif defined __BORLANDC__
#	define COMPILER_BORLAND	__BORLANDC__
#elif defined __MINGW32__
#	define COMPILER_MINGW32	__MINGW32__
#endif	// if defined _MSC_VER

// Configure macro for function-exporting from DLL.. (attributes to specify an "exported" function, visible from outside the DLL).
// From HLSDK dlls/cbase.h: "C functions for external declarations that call the appropriate C++ methods".
// Windows uses '__declspec (dllexport)' to mark functions in the DLL that should be visible/callable externally.
// It also apparently requires WINAPI for GiveFnptrsToDll().
#if defined COMPILER_VISUALC || defined COMPILER_MINGW32
#	define EXPORT extern "C" __declspec (dllexport)	// WINAPI should be provided in the windows compiler headers (it's usually defined to something like '__stdcall')
#elif defined COMPILER_BORLAND
#	define EXPORT extern "C"
#else
#	error "Can't configure export macros. Compiler unrecognized!"
#endif	// if defined COMPILER_VISUALC || defined COMPILER_MINGW32

// silence certain warnings
#pragma warning (disable: 4100)	// unreferenced formal parameter
#pragma warning (disable: 4995)	// 'function': name was marked as #pragma deprecated
#pragma warning (disable: 4996)	// function was declared deprecated

// misc C-runtime library headers
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>	// Header File For Standard Input/Output
#include <memory.h>
#include <time.h>
#include <io.h>	// For access() function.

#if defined _DEBUG
#	include <typeinfo>	// Very useful for debugging.
#endif	// if defined _DEBUG

// prevent tons of unused windows definitions
#define WIN32_LEAN_AND_MEAN	// exclude rarely-used stuff from windows headers

// operating system specific macros, functions and type definitions
#include <windows.h>	// Header File For Windows
#include <direct.h>		// Working with directories

#include <new>

#include <sys/stat.h>
#define FileStatistics_t struct _stat	// Ugly but this is the only way....

namespace MemoryUtilities
{
	struct ModuleInformation_t;	// For 'Library' class.
}

// (dz): disable deprecation warnings concerning unsafe CRT functions
#if !defined _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif	// if !defined _CRT_SECURE_NO_WARNINGS

// Makes a 4-byte "packed ID" int out of 4 characters
#define MAKEID(d,c,b,a)					(((int)(a) << 24) | ((int)(b) << 16) | ((int)(c) << 8) | ((int)(d)))

#include <MMSystem.h>	// wave format
//#include <mmreg.h>	// adpcm format
#pragma comment (lib, "WinMM.lib")
/*! @todo MAKE IT COMPILABLE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!??????????????????????????????
#include <dsound.h>
//#pragma comment (lib, "dsound.lib")
//#pragma comment (lib, "dxguid")
//*/
#if defined USE_SPEECH_ENGINES
#	include <Speech.hpp>
#endif	// if defined USE_SPEECH_ENGINES

#include <Resource.hpp>

#if defined _DEBUG && defined MEMORY_MANAGER
#	include <Fluid_Studios_Memory_Manager/mmgr.hpp>
#endif	// if defined _DEBUG && defined MEMORY_MANAGER

// Log levels....
enum LogLevel_t
{
	LogLevel_Default,	// default log message
	LogLevel_Note,		// note log message
	LogLevel_Warning,	// warning log message
	LogLevel_Error,		// error log message
	LogLevel_Critical	// fatal error log message (terminate the game!)
};

extern void AddLogEntry (const bool outputToConsole, const LogLevel_t logLevel, const bool ignoreLogLevel, const char *const format, ...);

#include <Core tool library/CoreToolLibrary.hpp>

using namespace CoreToolLibrary;
/*BARADA!!!
struct Product_t
{
	DynamicString internalName;
	DynamicString name;
	DynamicString version;
	DynamicString author;
	DynamicString url;
	DynamicString email;
	DynamicString logTag;
	DynamicString description;
	DynamicString copyrightYear;
	DynamicString copyright;
	DynamicString legal;
	DynamicString originalName;
	DynamicString supportVersion;
	DynamicString date;
	DynamicString time;

	inline Product_t (void) :
		internalName   ("Ping of Death Bot"),
		name           ("Yet another " + internalName),
		version        ("3.0"),
		author         ("YaPB Dev Team"),
		url            ("http://YaPB.Bots-United.com/"),
		email          ("jeefo@inbox.ru"),
		logTag         ("YAPB"),
		description    (name + " v" + version + " - The Counter-Strike v1.x Bot"),
		copyrightYear  ("2003-2010"),
		copyright      ("Copyright © " + copyrightYear + ", by " + author),
		legal          ("Half-Life, Counter-Strike, Counter-Strike: Condition Zero, Steam, Valve is a trademark of Valve Corporation"),
		originalName   ("YaPB.DLL"),
		supportVersion ("1.6 - CZ"),
		date           (__DATE__),
		time           (__TIME__)
	{ /*VOID*//* }
};
*/
namespace Trampolines
{
	namespace Bytecode
	{
		struct MoveESIToValue32_t;
	}
}

#if defined MULTIPLE_DRAW_METHODS
#	include <DrawTools.hpp>
#endif	// if defined MULTIPLE_DRAW_METHODS

enum BotConsoleCommandReturnType_t
{
	BotConsoleCommandReturnType_NotFound,		// command not found
	BotConsoleCommandReturnType_Accessed,		// okay
	BotConsoleCommandReturnType_ServerOnly,		// command can only be executed from server console
	BotConsoleCommandReturnType_RequireAccess	// dont have access to command

//	BotConsoleCommandReturnType_Error			// accessed but error occurred
};

// Personalities defines.
enum Personality_t
{
	Personality_None = -1,

	Personality_Normal,
	Personality_Rusher,
	Personality_Careful,

	// Total of bot personalities.
	Personality_Total
};
/*
struct GameMod_t
{
	GameModType_t type;
	DynamicString name;
} g_gameMod;
*/
class OwningItem;
class OwningItemsManager;
class CurrentWeapon;
class Client;
class Human;
class Host;
class Player;
class FakeClient;
class FakeClient_NotYaPB;
class ZBot;
class YaPB;
template <typename clientType> class ClientManagerTemplate;
class ClientManager;
class FakeClientManager;
class FakeClientNotYaPBManager;
class HumanManager;
class YaPBManager;
class HostageManager;
class C4Manager;
class Server;

class WaypointLocationsTable;
class WaypointVisibilityTable;
class Waypoint;
class WaypointManager;

#include <GlobalConstant.hpp>

#include <Debug.hpp>

namespace MetaModEngine
{
	class Engine;

	namespace Globals
	{
		extern Engine *g_metaModEngine;
	}
}

/// @note TEMPORAILY HERE!!!
namespace Trampolines
{
	inline const bool VirtualProtect (void *const address, const unsigned int size, const unsigned int newProtectFlags)
	{
		unsigned long dummyOldProtectFlags;

		return ::VirtualProtect (address, size, newProtectFlags, &dummyOldProtectFlags) == TRUE;
	}
}

#include <Half-Life engine/Core.hpp>	// shared engine/DLL constants/structures
#include <Meta mod engine/Core.hpp>

#if defined MULTIPLE_DRAW_METHODS
	/*static */inline const float DrawInterface::TimedObject::GetTime (void) { return HalfLifeEngine::Globals::g_halfLifeEngine->GetTime (); }	// For some customization....
#endif	// if defined MULTIPLE_DRAW_METHODS

#include <Support.hpp>
#include <Trampolines.hpp>

namespace Console
{
	#include <Console/Console_definitions.hpp>
}

namespace CoreToolLibrary
{
	// default log message
	inline void Log::WriteDefaultMessage                (const char *const message)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Default)
			return;

		JustWriteDefaultMessage (message);
	}
	inline void Log::WriteDefaultMessageFormat          (const char *const format, ...)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Default)
			return;

		// Concatenate all the arguments in one string....
		COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, m_message, sizeof (m_message));

		JustWriteDefaultMessage (m_message);
	}

	// note log message
	inline void Log::WriteNoteMessage                (const char *const message)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Note)
			return;

		JustWriteNoteMessage (message);
	}
	inline void Log::WriteNoteMessageFormat          (const char *const format, ...)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Note)
			return;

		// Concatenate all the arguments in one string....
		COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, m_message, sizeof (m_message));

		JustWriteNoteMessage (m_message);
	}

	// warning log message
	inline void Log::WriteWarningMessage             (const char *const message)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Warning)
			return;

		JustWriteWarningMessage (message);
	}
	inline void Log::WriteWarningMessageFormat       (const char *const format, ...)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Warning)
			return;

		// Concatenate all the arguments in one string....
		COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, m_message, sizeof (m_message));

		JustWriteWarningMessage (m_message);
	}

	// error log message
	inline void Log::WriteErrorMessage               (const char *const message)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Error)
			return;

		JustWriteErrorMessage (message);
	}
	inline void Log::WriteErrorMessageFormat         (const char *const format, ...)
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_LogLevel)->GetValue <unsigned char> () < Console::Bot::VariableValue_LogLevel_Error)
			return;

		// Concatenate all the arguments in one string....
		COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, m_message, sizeof (m_message));

		JustWriteErrorMessage (m_message);
	}
}

#include <MegaHAL.hpp>

#include <Localizer.hpp>

#include <Globals/Globals_definitions.hpp>

#include <AStarNode.hpp>

#include <BaseNavigationElement.hpp>

#include <WaypointNode.hpp>
#include <Waypoint.hpp>
#include <WaypointDistancesTable.hpp>
#include <WaypointLocationsTable.hpp>
#include <WaypointVisibilityTable.hpp>
#include <WaypointManager.hpp>

#include <HostageManager.hpp>

#include <C4.hpp>
#include <C4Manager.hpp>

#include <Navigation mesh/NavigationMesh_definitions.hpp>
template </* NavigationMesh::NavigationArea */> inline       NavigationMesh::NavigationArea   *const BaseNavigationElement::GetAreaPointer     (void)
		{
			// Reliability check.
			InternalAssert (IsBaseNavigationArea () && !IsNavigationLadder ());

			return reinterpret_cast <NavigationMesh::NavigationArea *const> (this);
		}
		template </* NavigationMesh::NavigationArea */> inline const NavigationMesh::NavigationArea   *const BaseNavigationElement::GetAreaPointer     (void) const
		{
			// Reliability check.
			InternalAssert (IsBaseNavigationArea () && !IsNavigationLadder ());

			return reinterpret_cast <const NavigationMesh::NavigationArea *const> (this);
		}

		template </* NavigationMesh::NavigationLadder */> inline       NavigationMesh::NavigationLadder *const BaseNavigationElement::GetLadderPointer   (void)
		{
			// Reliability check.
			InternalAssert (IsNavigationLadder ());

			return reinterpret_cast <NavigationMesh::NavigationLadder *const> (this);
		}
		template </* NavigationMesh::NavigationLadder */> inline const NavigationMesh::NavigationLadder *const BaseNavigationElement::GetLadderPointer   (void) const
		{
			// Reliability check.
			InternalAssert (IsNavigationLadder ());

			return reinterpret_cast <const NavigationMesh::NavigationLadder *const> (this);
		}
#include <Server.hpp>

#include <Menus.hpp>

#include <Client.hpp>
#include <Human.hpp>
#include <Host.hpp>
#include <Player.hpp>
#include <FakeClient.hpp>
#include <OtherBot.hpp>
#include <ZBot.hpp>

#include <ChatManager.hpp>
#include <ChatterManager.hpp>

#include <YaPB.hpp>
#include <ClientManagerTemplate.hpp>
#include <ClientManager.hpp>
#include <FakeClientManager.hpp>
#include <FakeClientNotYaPBManager.hpp>
#include <HumanManager.hpp>
#include <YaPBManager.hpp>

inline const bool LocalizerManager::TryRemoveLocalizer (const Localizer *const localizer, const Human *const owner)
{
	if (localizer == &g_localizer)
		return true;	// Do not let to remove the server localizer....

	/// @todo MAYBE MAKE SOME REFERENCES COUNT?
	// Check if this localizer is used by other human....
	for (HumanManager::HumansArray_t::IndexType_t index (0u); index < g_server->GetHumanManager ()->GetHumansCount (); ++index)
	{
		const Human *const human (g_server->GetHumanManager ()->GetHumansArray ()[index]);

		if (human != owner && human->GetLocalizer () == localizer)
			return true;
	}

	return RemoveLocalizer (localizer);
}

namespace HalfLifeEngine
{
	namespace SDK
	{
		namespace Classes
		{
			inline const bool Edict::IsPlayerZBot (void) const
			{
				return (privateData->GetBasePlayerPointer ()->*MemoryUtilities::GetElementsTable <BasePlayer::FunctionIsBot_t> (privateData)[Constants::EntityVirtualTableFunctionID_Player_IsBot]) ();
			}

			inline const Edict *const Edict::GetHostageRescuer (void) const
			{
				// Returns the rescuer of this hostage.
				/// @note here we don't check if hostage valid and alive, due to this function is called after hostage dies....

				const HostageImprov *const hostageImprovPointer (GetPrivateDataReference <HostageImprov *const> (Constants::OFFSET_Hostage_ImprovPointer));

				// Set to not follow anything?
				if (hostageImprovPointer != NULL && MemoryUtilities::GetOffsetAddressReference <unsigned int> (hostageImprovPointer, Constants::OFFSET_HostageImprov_UNKNOWN0) == reinterpret_cast <const int> (&MemoryUtilities::GetOffsetAddressReference <unsigned int> (hostageImprovPointer, Constants::OFFSET_HostageImprov_UNKNOWN1)))
					return MemoryUtilities::GetOffsetAddressReference <Edict *const> (hostageImprovPointer, Constants::OFFSET_HostageImprov_Target);

				return GetPrivateDataReference <bool> (Constants::OFFSET_Hostage_IsStopped) ? NULL : GetPrivateDataReference <Edict *const> (Constants::OFFSET_HOSTAGE_TARGET);
			}
		}
	}

	namespace Utilities
	{
		#include <Half-Life engine/ZBotInGameEnabler.hpp>
	}
}

#include <ConsoleCommand.hpp>

inline Server::~Server (void)
{
	// This function gets called when the server is shut down.

//	UnregisterAllForwardsAllHooks ();	// This function is called in InternalDetach() and ServerDeactivate() functions....

	// free all the memory allocated for all clients (including bots)....
/*!	delete m_fakeClientManager;
	delete m_otherBotManager;
	delete m_humanManager;
	delete m_YaPBManager;*/
	delete m_clientManager;
	delete m_fakeClientManager;
	delete m_otherBotManager;
	delete m_humanManager;
	delete m_YaPBManager;

	delete m_recastWrapper;
}
inline void Server::Think (void)
{
	m_navigationMeshManager.Think ();
/*
	if (m_navigationMeshManager.IsGenerating ())
		return;
*/
//	GetRecastWrapper ()->Think ();

	m_waypointManager.Think ();

	m_fakeClientManager->Think ();	/// @note Call this function before we calling 'ClientManager::Think()', for calculating light style values!
	m_clientManager->Think ();
	m_YaPBManager->Think ();
}

namespace HalfLifeEngine
{
	namespace Export
	{
		namespace ReplacedFunctions
		{
			namespace GameRulesTable
			{
				#include <Half-Life engine/Export/Replaced functions/GameRules.hpp>	// Declare game rules callbacks functions here....
			}

			namespace BotManagerTable
			{
				#include <Half-Life engine/Export/Replaced functions/BotManager.hpp>	// Declare bot manager callbacks functions here....
			}

			namespace EntityTable
			{
				#include <Half-Life engine/Export/Replaced functions/Entity.hpp>	// Declare entity callbacks functions here....
			}

			namespace Other
			{
				#include <Half-Life engine/Export/Replaced functions/Other.hpp>
			}
		}
	}

	inline void Engine::StudioSetupPlayerBones (const SDK::Classes::Edict *const entity, const char boneID/* = -1 means setup all the bones. */) const
	{
		// Reliability check.
		InternalAssert (entity->IsPlayer ());
		InternalAssert (m_boneTransformMatrix != NULL);

		// Code adapted from SV_HullForStudioModel() and R_StudioHull() functions.... (ONLY FOR PLAYER ENTITIES!)

		Math::Angles3D angles (entity->variables.modelAngles);
		const SDK::Structures::StudioHeader_t *const studioHeader (entity->GetModelPointer ());
		const SDK::Structures::mstudioseqdesc_t *const sequenceDescription (reinterpret_cast <const SDK::Structures::mstudioseqdesc_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->seqindex));
		int blend;

		// calculate blending for player
		Utilities::R_StudioPlayerBlend (sequenceDescription[entity->variables.sequence], blend, angles.pitch);

		const unsigned char controller[4u] = {127u, 127u, 127u, 127u};
		const unsigned char blending[2u] = {static_cast <unsigned char> (blend), 0u};

		angles.pitch = -angles.pitch;	// Inverse pitch.

		GetBlendingInterface ().SV_StudioSetupBones
		(
			g_server->GetServer ().models[entity->variables.modelIndex],
			entity->variables.frame,
			entity->variables.sequence,
			angles,
			entity->variables.origin,
			controller,
			blending,
			boneID,
			entity
		);
	}
	inline void Engine::TraceHullThroughSolidEntities (const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::Hull_t hullNumber, Interface_TraceFilter &traceFilter, SDK::Structures::TraceResult_t &traceResult) const
	{
		SDK::Classes::Edict *const worldspawnEntity (GetEdictOfIndex (SDK::Constants::WorldspawnEntitySlotIndex));	// worldspawn
		float worldFraction;

		if (traceFilter.ShouldHitEntity (worldspawnEntity))
		{
			TraceEntity (source, destination, hullNumber, worldspawnEntity, traceResult);

			if (traceResult.fraction == 0.0f)
				return;	// blocked immediately by the world

			worldFraction = traceResult.fraction;	// Save the world collision fraction.
		}
		else
		{
			// Fill in a default trace result
			memset (&traceResult, 0, sizeof (traceResult));

			traceResult.fraction = worldFraction = 1.0f;
			traceResult.endPosition = destination;
		}

		// create the bounding box of the entire move
		// we can limit it to the part of the move not
		// already clipped off by the world, which can be
		// a significant savings for line of sight and shot traces
		Math::Extent3D extent (SDK::Constants::hullSizes[hullNumber].GetWithMovedBounds (source, traceResult.endPosition));

		// clip to other solid entities
		ClipMoveToEntities (g_server->GetEdictAreaNodes (), extent, source, destination, hullNumber, traceFilter, traceResult);

		// Fix up the fraction so it's appropriate given the original unclipped-to-world ray
		traceResult.fraction *= worldFraction;
	}
}

extern const bool IsBorderPlaneVisible (const Math::Vector3D &source, const Math::Vector3D &destination, const Math::Vector3D &mins, const Math::Vector3D &maxs, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore, HalfLifeEngine::SDK::Classes::Edict *const entityToSkip, const float multiplier, Math::Plane::Points_t &points);
inline const bool IsBorderPlaneVisible (const Math::Vector3D &source, const Math::Vector3D &destination, const Math::Vector3D &mins, const Math::Vector3D &maxs, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore = HalfLifeEngine::SDK::Constants::TraceIgnore_None, HalfLifeEngine::SDK::Classes::Edict *const entityToSkip = NULL, const float multiplier = 1.0f)
{
	Math::Plane::Points_t points;

	return IsBorderPlaneVisible (source, destination, mins, maxs, traceIgnore, entityToSkip, multiplier, points);
}
/*
#if defined MULTIPLE_DRAW_METHODS
inline const bool DrawInterface_GL::IsLineVisibleForHost (const Math::Vector3D &source, const Math::Vector3D &destination) const
{
	#define MAX_OVERLAY_DIST_SQR 90000000.0f

	// --------------------------------------------------------------
	// Clip the line before sending so we don't overflow the client message buffer
	// --------------------------------------------------------------
	const Host *const player (g_server->GetHumanManager ()->GetHostClient ());

	if (player == NULL)
		return false;

	const Math::Vector3D &hostOrigin (player->GetOrigin ());

	// ------------------------------------
	// Clip line that is far away
	// ------------------------------------
	if (hostOrigin.GetDistanceSquared (source) > MAX_OVERLAY_DIST_SQR && hostOrigin.GetDistanceSquared (destination) > MAX_OVERLAY_DIST_SQR)
		return false;

	// ------------------------------------
	// Clip line that is behind the client
	// ------------------------------------
	const Math::Vector3D clientForward          (player->GetEyeForwardDirection ());
	const Math::Vector3D directionToSource      (source - player->GetOrigin ());
	const Math::Vector3D directionToDestination (destination - player->GetOrigin ());
	const float          dotOrigin              (clientForward | directionToSource);
	const float          dotTarget              (clientForward | directionToDestination);

	return dotOrigin >= 0.0f && dotTarget >= 0.0f;
}
#endif	// if defined MULTIPLE_DRAW_METHODS
*/
#endif	// ifndef YAPB_CORE_INCLUDED