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
// Server.hpp
//
// Class: Server
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SERVER_INCLUDED
#define SERVER_INCLUDED

class Server
{
	//
	// Group: Constants.
	//
	public:
		// Defines map type.
		enum MapTypeID_t
		{
			MapTypeID_As,	// Assassination map.
			MapTypeID_Cs,	// Rescue map.
			MapTypeID_De,	// Defusion map.
			MapTypeID_Es,	// Escape map.

			// Other map types without map-specific entities....
			MapTypeID_Ka,
			MapTypeID_Fy,
			MapTypeID_Gg,
			MapTypeID_Aim,
			MapTypeID_Dm,
			MapTypeID_Kz,
			MapTypeID_Zm,
			MapTypeID_Awp,
			MapTypeID_He,

			MapTypeID_Total
		};
		DECLARE_TYPED_ENUMERATION (unsigned int, MapType_t)
		{
			MapType_None,

			// Standart maps....
			MapType_As  = BIT (MapTypeID_As),	// Assassination map.
			MapType_Cs  = BIT (MapTypeID_Cs),	// Rescue map.
			MapType_De  = BIT (MapTypeID_De),	// Defusion map.
			MapType_Es  = BIT (MapTypeID_Es),	// Escape map.

			// Other map types without map-specific entities....
			MapType_Ka  = BIT (MapTypeID_Ka),
			MapType_Fy  = BIT (MapTypeID_Fy),
			MapType_Gg  = BIT (MapTypeID_Gg),
			MapType_Aim = BIT (MapTypeID_Aim),
			MapType_Dm  = BIT (MapTypeID_Dm),
			MapType_Kz  = BIT (MapTypeID_Kz),
			MapType_Zm  = BIT (MapTypeID_Zm),
			MapType_Awp = BIT (MapTypeID_Awp),
			MapType_He  = BIT (MapTypeID_He)
		};

		#include <ThrownGrenadesManager.hpp>
		#include <SpawnPointsContainer.hpp>

	//
	// Group: Private members.
	//
	private:
		static unsigned short                                         ms_startsNumber;

		const HalfLifeEngine::SDK::Structures::Server_t              &m_server;	// local server
		const HalfLifeEngine::SDK::Structures::ServerStatic_t        &m_serverStatic;
		const HalfLifeEngine::SDK::Structures::EdictAreaNode_t *const m_edictAreaNodes/*[HalfLifeEngine::SDK::Constants::EdictAreaNode_Total]*/;
		HalfLifeEngine::SDK::Structures::CLC_ParseFunction_t *const   m_CLC_ParseFunctions;	// client to server parsing functions.
		const HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *const &m_gameRules;
		const HalfLifeEngine::SDK::Classes::CSBotManager *const      &m_ZBotManager;
		const HalfLifeEngine::SDK::Structures::MapCycle_t            &m_mapCycle;

		bool                                                         &m_UseBotArgs;
		const char                                                  **m_BotArgs/*[4u]*/;

		HalfLifeEngine::SDK::Structures::CLC_ParseFunction_t::ParseFunctionPointer_t m_originalSV_ParseVoiceDataFunction;
		HalfLifeEngine::SDK::TypeDefinitions::UTIL_LogPrintf_t                       m_originalUTIL_LogPrintfFunction;

		ClientManager *const                                          m_clientManager;
		FakeClientManager *const                                      m_fakeClientManager;
		FakeClientNotYaPBManager *const                               m_otherBotManager;
		HumanManager *const                                           m_humanManager;
		YaPBManager *const                                            m_YaPBManager;

		#include <GameMessageHandlerManager.hpp>

		GameMessageHandlerManager                                     m_gameMessageHandlerManager;

		HooksManager                                                  m_engineVirtualTableHooksManager;

		C4Manager                                                     m_c4Manager;

		/// @todo MOVE THIS TO 'YaPBManager' CLASS!!!
		///@{
		NavigationMesh                                                m_navigationMeshManager;	// The Navigation Mesh interface.
		RecastWrapper                                                *m_recastWrapper;
		WaypointManager                                               m_waypointManager;
		///@}

		HostageManager                                                m_hostageManager;

		ThrownGrenadesManager                                         m_thrownGrenadesManager;

		struct Round_t
		{
			float startTime;		// Stores the start of the round (in world time).
			float freezeEndTime;	// Stores the start of the round + freeze time (in world time).
			float midTime;			// Stores the halftime of the round (in world time).
			float endTime;			// Stores the end of the round (in world time).

			bool  isEnded;			// Is round ended? (i.e., Terrorists Win!, Counter-Terrorists Win! or something else....)

			inline Round_t (void) : startTime (0.0f), freezeEndTime (0.0f), midTime (0.0f), endTime (0.0f), isEnded (false) { /*VOID*/ }

			inline void Start (void)
			{
				const float roundTimeInSeconds (Console::Server::variableManager.GetVariable (Console::Server::VariableID_RoundTime)->GetValue <float> () * 60.0f);

				// calculate the round start/mid/end in world time
				startTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
				freezeEndTime = startTime + Console::Server::variableManager.GetVariable (Console::Server::VariableID_FreezeTime)->GetValue <float> ();
				midTime = freezeEndTime + roundTimeInSeconds * 0.5f;
				endTime = freezeEndTime + roundTimeInSeconds;
				isEnded = false;
			}
			inline void End (void) { isEnded = true; }
		}                                                             m_round;

		const SpawnPointsContainer                                    m_spawnPointsContainer;	// Contains the origins of all player spawn points on this map.

		MapType_t                                                     m_mapType;	// Type of map - assassination, bomb defuse etc....
		class MapScenario
		{
		};
		class MapScenario_Assassination : public MapScenario
		{
		};
		class MapScenario_Rescue : public MapScenario
		{
			private:
				HostageManager m_hostageManager;
		};
		class MapScenario_Defusion : public MapScenario
		{
		};
		class MapScenario_Escape : public MapScenario
		{
		};
		class MapScenario_RescueAndDefusion : public MapScenario_Rescue, public MapScenario_Defusion
		{
		};

		MapScenario                                                  *m_mapScenario;	// UNUSED!

	//
	// Group: (Con/De)structors.
	//
	public:
		Server (void);
		virtual inline ~Server (void);	// Note: This function declared in Core.hpp

	//
	// Group: Private functions.
	//
	private:
		static void SV_ParseVoiceData (HalfLifeEngine::SDK::Structures::Client_t *const client);

		void RegisterClientToServerMessageForwards   (void);
		void UnregisterClientToServerMessageForwards (void) const;
		void RegisterEngineAPIForwards               (void) const;
		void UnregisterEngineAPIForwards             (void) const;
		void RegisterGameDLLAPIForwards              (void);
		void UnregisterGameDLLAPIForwards            (void);
		void RegisterEngineVirtualTableHooks         (void);
		void UnregisterEngineVirtualTableHooks       (void);

		void DetectMapType                           (void);

	//
	// Group: Functions.
	//
	public:
		static inline const unsigned short GetStartsNumber    (void) { return ms_startsNumber; }
		static inline const bool           IsFirstServerStart (void) { return GetStartsNumber () <= 1u; }

		inline const bool                   IsRunning                 (void) const { return this != NULL; }	// Is server already running?
		inline const DynamicString          GetMapName                (void) const { return HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName (); }
		inline const MapType_t              GetMapType                (void) const { return m_mapType; }
		inline const bool                   IsMapHasTarget            (void) const { return (GetMapType () & (MapType_As | MapType_Cs | MapType_De | MapType_Es)) > 0u; }
		inline const bool                   IsFreezePeriod            (void) const { return m_gameRules->IsFreezePeriod ()/*m_round.freezeEndTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()*/; }
		inline const bool                   IsTeamplay                (void) const { return !Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Deathmatch)->GetValue <bool> () && !HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetValue <bool> ("csdm_active"); }
		inline const bool                   IsGameCommenced           (void) const { return m_gameRules->m_isGameCommenced; }
		inline const Round_t               &GetRoundInformation       (void) const { return m_round; }

		inline const HalfLifeEngine::SDK::Structures::Server_t              &GetServer                    (void) const { return m_server; }
		inline const HalfLifeEngine::SDK::Structures::ServerStatic_t        &GetStatic                    (void) const { return m_serverStatic; }
		inline const HalfLifeEngine::SDK::Structures::EdictAreaNode_t *const GetEdictAreaNodes            (void) const { return m_edictAreaNodes; }
		inline const HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *const &GetGameRules                 (void) const { return m_gameRules; }
		inline const HalfLifeEngine::SDK::Classes::CSBotManager *const      &GetZBotManager               (void) const { return m_ZBotManager; }
		inline const HalfLifeEngine::SDK::Structures::MapCycle_t            &GetMapCycleInformation       (void) const { return m_mapCycle; }
		inline bool                                                         &GetUseBotArgsReference       (void)                      { return m_UseBotArgs; }
		inline const char                                                  *&GetBotArgsReference          (const unsigned char index) { return m_BotArgs[index]; }
		inline void                                                          CallOriginalSV_ParseVoiceDataFunction (HalfLifeEngine::SDK::Structures::Client_t *const client) const { (*m_originalSV_ParseVoiceDataFunction) (client); }
		inline void                                                          CallOriginalUTIL_LogPrintfFunction    (const char *const format/*, ...*/) const { (*m_originalUTIL_LogPrintfFunction) (format); }

		inline       GameMessageHandlerManager                              &GetGameMessageHandlerManager (void)       { return m_gameMessageHandlerManager; }
		inline       HooksManager                                           &GetHooksManager              (void)       { return m_engineVirtualTableHooksManager; }
		inline const HooksManager                                           &GetHooksManager              (void) const { return m_engineVirtualTableHooksManager; }
		inline       ClientManager            *const                         GetClientManager             (void)       { return m_clientManager; }
		inline const ClientManager            *const                         GetClientManager             (void) const { return m_clientManager; }
		inline       FakeClientManager        *const                         GetFakeClientManager         (void)       { return m_fakeClientManager; }
		inline const FakeClientManager        *const                         GetFakeClientManager         (void) const { return m_fakeClientManager; }
		inline       FakeClientNotYaPBManager *const                         GetFakeClientNotYaPBManager  (void)       { return m_otherBotManager; }
		inline const FakeClientNotYaPBManager *const                         GetFakeClientNotYaPBManager  (void) const { return m_otherBotManager; }
		inline       HumanManager             *const                         GetHumanManager              (void)       { return m_humanManager; }
		inline const HumanManager             *const                         GetHumanManager              (void) const { return m_humanManager; }
		inline       YaPBManager              *const                         GetYaPBManager               (void)       { return m_YaPBManager; }
		inline const YaPBManager              *const                         GetYaPBManager               (void) const { return m_YaPBManager; }
		inline const C4Manager                                              &GetC4Manager                 (void) const { return m_c4Manager; }
		inline       NavigationMesh                                         &GetNavigationMeshManager     (void)       { return m_navigationMeshManager; }	// The singleton for accessing the navigation mesh.
		inline const NavigationMesh                                         &GetNavigationMeshManager     (void) const { return m_navigationMeshManager; }	// The singleton for accessing the navigation mesh.
		inline       RecastWrapper            *const                         GetRecastWrapper             (void)       { return m_recastWrapper; }
		inline       WaypointManager                                        &GetWaypointManager           (void)       { return m_waypointManager; }
		inline const WaypointManager                                        &GetWaypointManager           (void) const { return m_waypointManager; }
		inline       HostageManager                                         &GetHostageManager            (void)       { return m_hostageManager; }
		inline const HostageManager                                         &GetHostageManager            (void) const { return m_hostageManager; }
		inline       ThrownGrenadesManager                                  &GetThrownGrenadesManager     (void)       { return m_thrownGrenadesManager; }
		inline const ThrownGrenadesManager                                  &GetThrownGrenadesManager     (void) const { return m_thrownGrenadesManager; }
		inline const SpawnPointsContainer                                   &GetSpawnPointsContainer      (void) const { return m_spawnPointsContainer; }

		inline void                                                          ExecuteMainConfigurationFile (void) const
		{
			// Execute the main configuration file....
			if (STDIOFile::IsExists (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Configs/YaPB.cfg", "rb"))
			{
				AddLogEntry (true, LogLevel_Default, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ExecutingMainConfigurationFile));

				HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommand ("exec Addons/YaPB/Configs/YaPB.cfg\n");
			}
			else
				AddLogEntry (true, LogLevel_Warning, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_NoMainConfigurationFileFound));
		}
		inline void                                                          ExecuteMapSpecificConfigurationFile (void) const
		{
			// Execute the map-specific configuration file....

			const DynamicString configurationFilename ("Addons/YaPB/Configs/Maps/" + GetMapName () + ".cfg");

			if (STDIOFile::IsExists (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + configurationFilename, "rb"))
			{
				AddLogEntry (true, LogLevel_Default, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ExecutingMapSpecificConfigurationFile));

				HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommand ("exec " + configurationFilename + '\n');
			}
			else
				AddLogEntry (true, LogLevel_Warning, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_NoMapSpecificConfigurationFileFound));
		}
		inline void                                                          ExecuteConfigurationFiles    (void) const
		{
			ExecuteMainConfigurationFile ();
			ExecuteMapSpecificConfigurationFile ();
		}
		inline void                                                          TryExecuteConfigurationFilesOnServerStart (void) const
		{
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Config_LoadTime)->GetValue <Console::Bot::VariableValue_Config_LoadTime_t> () == Console::Bot::VariableValue_Config_LoadTime_PerServerActivate || (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Config_LoadTime)->GetValue <Console::Bot::VariableValue_Config_LoadTime_t> () == Console::Bot::VariableValue_Config_LoadTime_Once && IsFirstServerStart ()))
				ExecuteMainConfigurationFile ();

			ExecuteMapSpecificConfigurationFile ();
		}

		inline void RegisterAllForwardsAndHooks   (void)
		{
			RegisterClientToServerMessageForwards ();
			RegisterEngineAPIForwards ();
			RegisterGameDLLAPIForwards ();
			RegisterEngineVirtualTableHooks ();
		}
		inline void UnregisterAllForwardsAllHooks (void)
		{
			UnregisterClientToServerMessageForwards ();
			UnregisterEngineAPIForwards ();
			UnregisterGameDLLAPIForwards ();
			UnregisterEngineVirtualTableHooks ();
		}

	//
	// Group: Game message handler callbacks.
	//
	public:
		void RoundStarted (void);
		void RoundEnded   (void);

		// Note: The following 3 functions are declared in YaPBManager.hpp.
		inline void BombPlanted   (void);
		inline void BombDetonated (void);
		inline void BombDefused   (void);

		inline const unsigned char GetTeamScore (const TeamArrayID_t teamArrayID) const
		{
			// Reliability check.
			InternalAssert (teamArrayID < TeamArrayID_Total);

			// Get score for specified team....
			return static_cast <const unsigned char> (m_gameRules->m_score[!teamArrayID]);
		}
		inline const unsigned char GetTeamScore (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const { return GetTeamScore (static_cast <TeamArrayID_t> (teamID - 1u)); }
		inline const bool IsTeamCanBuy (const TeamArrayID_t teamArrayID) const
		{
			// Reliability check.
			InternalAssert (teamArrayID < TeamArrayID_Total);

			// Get score for specified team....
			return m_gameRules->m_isCantBuy[teamArrayID] == false;
		}
		inline const bool IsTeamCanBuy (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const { return IsTeamCanBuy (static_cast <TeamArrayID_t> (teamID - 1u)); }
		inline const bool IsFlashlightAllowed (void) const { return m_gameRules->IsFlashlightAllowed (); }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Think (void);	// Note: This function declared in Core.hpp.

		inline void FreezePeriodExpired (void);	// Note: This function declared in YaPBManager.hpp.

		inline void ChangeLevel (const char *const s1, const char *const s2) const;	// Note: This function declared in YaPBManager.hpp.

		void OnGameEvent (const HalfLifeEngine::SDK::Constants::GameEventType_t gameEvent, const HalfLifeEngine::SDK::Classes::BaseEntity *const argument1, const void *const argument2);
};
class ListenServer : public Server
{
	public:
		inline ListenServer (void) : Server () { /* VOID */ }

		virtual inline ~ListenServer (void) { /* VOID */ }

	public:
		/// @todo !!!
};
class DedicatedServer : public Server
{
	public:
		inline DedicatedServer (void) : Server () { /* VOID */ }

		virtual inline ~DedicatedServer (void) { /* VOID */ }

	public:
		/// @todo !!!
};

inline const float C4::GetExplosionDamage (void) const { return g_server->GetGameRules ()->m_c4ExplosionRadius; }

inline NavigationMesh::BaseNavigationArea::BaseNavigationArea (const ID_t id) :	///< Constructor used during normal runtime.
	BaseNavigationElement (),

	m_baseId (g_server->GetNavigationMeshManager ().IncreaseAndGetNextBaseAreaID ()),	// set an ID for splitting and other interactive editing - loads will overwrite this
	m_id (id),	// set an ID for splitting and other interactive editing - loads will overwrite this

	m_attributeFlags (NavAttributeFlag_None),

	m_place (g_server->GetNavigationMeshManager ().GetNavPlace ()),
	m_isBlocked (false),
	m_isUnderwater (false),
	m_isBattlefront (false)/*,

	m_basePrevHash (NULL),
	m_baseNextHash (NULL)*/
{
	for (unsigned char index (TeamArrayID_Terrorist); index < TeamArrayID_Total; ++index)
		m_clearedTimestamp[index] = m_earliestOccupyTime[index] = 0.0f;

	g_server->GetNavigationMeshManager ().m_baseAreas += this;
}
inline NavigationMesh::BaseNavigationArea::~BaseNavigationArea (void)
{
	g_server->GetNavigationMeshManager ().m_baseAreas -= this;
}

inline NavigationMesh::NavigationLadder::NavigationLadder (void) :
	BaseNavigationArea (g_server->GetNavigationMeshManager ().IncreaseAndGetNextLadderID ()),	// set an ID for interactive editing - loads will overwrite this

	m_connectedGapAreasNumber (0u),
	m_connectedGapAreas (NULL),

	m_edict (NULL)
{ /*VOID*/ }

inline NavigationMesh::NavigationArea::NavigationArea (void) :	///< Constructor used during normal runtime.
	BaseNavigationArea (g_server->GetNavigationMeshManager ().IncreaseAndGetNextAreaID ())	// set an ID for splitting and other interactive editing - loads will overwrite this
{
	Initialize ();
}

inline void NavigationMesh::NavigationArea::SavePlaceData (BaseFile &file) const	///< save place data to a file
{
	const PlaceDirectory::DirectoryArray::IndexType_t entry (g_server->GetNavigationMeshManager ().GetPlaceDirectory ().GetIndex (GetPlace ()));

	// store place directory entry
	file.Write (entry);
}
inline const bool NavigationMesh::NavigationArea::LoadPlaceData (const BaseFile &file)	///< load place data from a file
{
	PlaceDirectory::DirectoryArray::IndexType_t entry;

	if (!file.Read (entry))
		return false;

	// convert entry to actual Place
	SetPlace (g_server->GetNavigationMeshManager ().GetPlaceDirectory ().IndexToPlace (entry));

	return true;
}

inline NavigationMesh::NavigationArea::Spot::Spot (void) :	///< Must use factory to create.
	m_pos (Math::Vector3D::ZeroValue),
//	m_baseId (++m_nextBaseID),
	m_area (NULL)
{
	///< Construct a Spot. Assign a unique ID which may be overwritten if loaded.

//	g_server->GetNavigationMeshManager ().m_spots += this;
}
inline NavigationMesh::NavigationArea::Spot::Spot (const Math::Vector3D &pos) :	///< Must use factory to create.
	m_pos (pos),
//	m_baseId (++m_nextBaseID),
	m_area (NULL)
{
//	g_server->GetNavigationMeshManager ().m_spots += this;
}
inline NavigationMesh::NavigationArea::Spot::~Spot (void)
{
//	g_server->GetNavigationMeshManager ().m_spots -= this;
}
inline NavigationMesh::NavigationArea::HidingSpot::HidingSpot (void) :	///< Must use factory to create.
/*	Spot (),

	m_id (++m_nextID),
	m_flags (Flag_None)
*/
	m_pos (Math::Vector3D::ZeroValue),
	m_id (++m_nextID),
	m_flags (Flag_None),
	m_area (NULL)
{
	///< Construct a Hiding Spot. Assign a unique ID which may be overwritten if loaded.

	g_server->GetNavigationMeshManager ().m_hidingSpots += this;
}
inline NavigationMesh::NavigationArea::HidingSpot::HidingSpot (const Math::Vector3D &pos, const Flag_t flag) :	///< Must use factory to create.
/*	Spot (pos),

	m_id (++m_nextID),
	m_flags (static_cast <const unsigned char> (flag))
*/
	m_pos (pos),
	m_id (++m_nextID),
	m_flags (static_cast <const unsigned char> (flag)),
	m_area (NULL)
{
	g_server->GetNavigationMeshManager ().m_hidingSpots += this;
}
inline NavigationMesh::NavigationArea::HidingSpot::~HidingSpot (void)
{
	g_server->GetNavigationMeshManager ().m_hidingSpots -= this;
}

inline const bool NavigationMesh::NavigationArea::ApproachInfo_t::PostLoad (void)
{
	bool isCorruptData (false);
	ID_t id (reinterpret_cast <ID_t> (here));

	here = g_server->GetNavigationMeshManager ().GetNavAreaByID (id);

	if (id > 0u && here == NULL)
	{
		AddLogEntry (true, LogLevel_Default, false, "NavigationArea::ApproachInfo_t::PostLoad(): Corrupt navigation data. Missing Approach Area (here).");

		isCorruptData = true;
	}

	id = reinterpret_cast <ID_t> (prev);

	prev = g_server->GetNavigationMeshManager ().GetNavAreaByID (id);

	if (id > 0u && prev == NULL)
	{
		AddLogEntry (true, LogLevel_Default, false, "NavigationArea::ApproachInfo_t::PostLoad(): Corrupt navigation data. Missing Approach Area (prev).");

		isCorruptData = true;
	}

	id = reinterpret_cast <ID_t> (next);

	next = g_server->GetNavigationMeshManager ().GetNavAreaByID (id);

	if (id > 0u && next == NULL)
	{
		AddLogEntry (true, LogLevel_Default, false, "NavigationArea::ApproachInfo_t::PostLoad(): Corrupt navigation data. Missing Approach Area (next).");

		isCorruptData = true;
	}

	return isCorruptData;
}

inline const bool NavigationMesh::NavigationArea::SpotEncounter::PostLoad (NavigationArea *const ownerArea)
{
	bool isCorruptData (false);

	from = g_server->GetNavigationMeshManager ().GetNavAreaByID (reinterpret_cast <ID_t> (from));

	if (from == NULL)
	{
		AddLogEntry (true, LogLevel_Default, false, "NavigationArea::SpotEncounter::PostLoad(): Corrupt navigation data. Missing \"from\" Navigation Area for Encounter Spot.");

		isCorruptData = true;
	}

	to = g_server->GetNavigationMeshManager ().GetNavAreaByID (reinterpret_cast <ID_t> (to));

	if (to == NULL)
	{
		AddLogEntry (true, LogLevel_Default, false, "NavigationArea::SpotEncounter::PostLoad(): Corrupt navigation data. Missing \"to\" Navigation Area for Encounter Spot.");

		isCorruptData = true;
	}

	if (from != NULL && to != NULL)
	{
		// compute path
		ownerArea->ComputePortal (to, toDir, path.to);
		ownerArea->ComputePortal (from, fromDir, path.from);

		const float eyeHeight (HalfLifeEngine::SDK::Constants::HalfHumanHeight);

		path.from.z = from->GetZ (path.from) + eyeHeight;
		path.to.z = to->GetZ (path.to) + eyeHeight;
	}

	// resolve HidingSpot IDs
	for (SpotOrderArray::IndexType_t index (0u); index < spots.GetElementNumber (); ++index)
	{
		if (spots[index].PostLoad ())
			continue;

		AddLogEntry (true, LogLevel_Default, false, "NavigationArea::SpotEncounter::PostLoad(): Corrupt navigation data. Missing Hiding Spot.");

		isCorruptData = true;
	}

	return isCorruptData;
}

inline const bool NavigationMesh::NavigationArea::SpotEncounter::SpotOrder::PostLoad (void)
{
	// set our spot
	return (spot = g_server->GetNavigationMeshManager ().GetHidingSpotByID (reinterpret_cast <unsigned int> (spot))) != NULL;
}

inline void NavigationMesh::PlaceDirectory::Save (BaseFile &file) const
{
	/// store the directory

	// store number of entries in directory
	const DirectoryArray::IndexType_t count (m_directory.GetElementNumber ());

	file.Write (count);

	// store entries
	for (DirectoryArray::IndexType_t index (0u); index < count; ++index)
		file.Write (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (m_directory[index]));
}

inline const bool NavigationMesh::PlaceDirectory::Load (const BaseFile &file)
{
	/// load the directory

	DirectoryArray::IndexType_t count;

	// do some cleanup first
	Reset ();

	// read number of entries
	if (!file.Read (count))
		return false;

	// read each entry
	DynamicString placeName;

	for (DirectoryArray::IndexType_t index (0u); index < count; ++index)
	{
		if (!file.Read (placeName))
			return false;

		const Place_t place (g_server->GetNavigationMeshManager ().GetPlaceContainer ().NameToPlace (placeName));

		if (place == UNDEFINED_PLACE)
			AddLogEntry (true, LogLevel_Warning, false, "NavMesh place \"%s\" is undefined?", placeName.GetData ());
		else
			AddPlace (place);
	}

	return true;
}

inline void NavigationMesh::PlaceDirectory::Build (void)
{
	/// build the directory

	for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
	{
		const Place_t place (g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetPlace ());

		if (place != UNDEFINED_PLACE)
			AddPlace (place);
	}
}

inline void NavigationMesh::PlaceContainer::ReloadPlaceDatabase (void)
{
	///< reload the place names from a file

	// do some cleanup first
	Reset ();

	if (g_server->GetNavigationMeshManager ().GetNavPlace () == UNDEFINED_PLACE && g_server->GetNavigationMeshManager ().GetAreas ().IsEmpty ())
	{
		LoadPlaceDatabase ();

		return;
	}

	// Save old places before loading new from file....
	const PlaceContainer oldPlaces (g_server->GetNavigationMeshManager ().GetPlaceContainer ());

	LoadPlaceDatabase ();

	/// @todo SPEEDUP IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	g_server->GetNavigationMeshManager ().SetNavPlace (NameToPlace (oldPlaces.PlaceToName (g_server->GetNavigationMeshManager ().GetNavPlace ())));

	// Update area places....
	for (NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		g_server->GetNavigationMeshManager ().GetAreas ()[index]->SetPlace (NameToPlace (oldPlaces.PlaceToName (g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetPlace ())));
}

inline void NavigationMesh::NodesGridSize_t::Initialize (void)
{
	x = static_cast <unsigned char> (g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.GetSizeX () / NodesGridCellSize) + 1u;
	y = static_cast <unsigned char> (g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.GetSizeY () / NodesGridCellSize) + 1u;
	z = static_cast <unsigned char> (g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.GetSizeZ () / NodesGridCellSize) + 1u;
}

inline NavigationMesh::Generator::NodesGridSize_t::NodesGridSize_t (void) :
	x (static_cast <unsigned char> (g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.GetSizeX () / GridCellSize) + 1u),
	y (static_cast <unsigned char> (g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.GetSizeY () / GridCellSize) + 1u),
	z (static_cast <unsigned char> (g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.GetSizeZ () / GridCellSize) + 1u)
{ /*VOID*/ }

inline const unsigned char NavigationMesh::Generator::WorldToGridX (const float worldX) const	///< given X component, return grid index
{
	// Return bucket.
	return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldX - g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.mins.x) / GridCellSize), 0, m_nodesGridSize.x - 1));
}
inline const unsigned char NavigationMesh::Generator::WorldToGridY (const float worldY) const	///< given Y component, return grid index
{
	// Return bucket.
	return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldY - g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.mins.y) / GridCellSize), 0, m_nodesGridSize.y - 1));
}
inline const unsigned char NavigationMesh::Generator::WorldToGridZ (const float worldZ) const	///< given Z component, return grid index
{
	// Return bucket.
	return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldZ - g_server->GetNavigationMeshManager ().m_worldMap.GetBSPFile ().models[0u].boundingBox.mins.z) / GridCellSize), 0, m_nodesGridSize.z - 1));
}

#endif	// ifndef SERVER_INCLUDED