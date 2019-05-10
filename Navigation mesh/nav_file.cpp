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
// nav_file.hpp
//
// Description: Reading and writing navigation files.
//
// Version: $ID:$
//

#include <Core.hpp>

//--------------------------------------------------------------------------------------------------------------
/*virtual */void NavigationMesh::BaseNavigationArea::SaveToOBJFile (BaseFile &file, unsigned int &base) const
{
	// Save navigation area as "*.OBJ" file....

	Math::Vector3D vertexes[NUM_CORNERS];

	GetCorners (vertexes);

	// Write area vertexes....
	for (unsigned char vertexIndex (NORTH_WEST); vertexIndex < NUM_CORNERS; ++vertexIndex)
	{
		// Transform HL engine coordinate system to recast demo coordinate system....
		{
			// Swap Y and Z axes....
			vertexes[vertexIndex].Permutate (Math::Vector3D::AxisID_Y);

			// Invert X axis....
			vertexes[vertexIndex].x = -vertexes[vertexIndex].x;
		}

		file.PrintFormat ("v %f %f %f\n", vertexes[vertexIndex].x, vertexes[vertexIndex].y, vertexes[vertexIndex].z);
	}

	#define PRINT_ATTRIBUTE(attribute) HasAttributes (attribute) ? #attribute + GET_STATIC_STRING_LENGTH ("NAV_MESH_") : ""

	// Write area flags....
	file.PrintFormat
	(
/// @warning TEMP!!!		"\n\ng %04uArea%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
		"\n\ng %04uArea%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n",

		GetID (),

		PRINT_ATTRIBUTE (NAV_MESH_CROUCH),
		PRINT_ATTRIBUTE (NAV_MESH_JUMP),
		PRINT_ATTRIBUTE (NAV_MESH_DOUBLE_JUMP),
		PRINT_ATTRIBUTE (NAV_MESH_PRECISE),
		PRINT_ATTRIBUTE (NAV_MESH_NO_JUMP),
		PRINT_ATTRIBUTE (NAV_MESH_STOP),
		PRINT_ATTRIBUTE (NAV_MESH_RUN),
		PRINT_ATTRIBUTE (NAV_MESH_WALK),
		PRINT_ATTRIBUTE (NAV_MESH_AVOID),
		PRINT_ATTRIBUTE (NAV_MESH_TRANSIENT),
		PRINT_ATTRIBUTE (NAV_MESH_DONT_HIDE),
		PRINT_ATTRIBUTE (NAV_MESH_STAND),
		PRINT_ATTRIBUTE (NAV_MESH_NO_HOSTAGES),
		PRINT_ATTRIBUTE (NAV_MESH_STAIRS),
		PRINT_ATTRIBUTE (NAV_MESH_UNMERGEABLE),
		PRINT_ATTRIBUTE (NAV_MESH_OBSTACLE)/*!,
@warning TEMORAILY DISABLED - NEED TO CHANGE 'NavAttributeFlag_t' TO 'unsigned int' 		PRINT_ATTRIBUTE (NAV_MESH_CLIFF)*/
	);

	#undef PRINT_ATTRIBUTE

	// Write area face....
	file.PrintFormat ("f %u %u %u %u\n\n", base, base + 1u, base + 2u, base + 3u);

	base += 4u;
}
const bool NavigationMesh::SaveToOBJFile (void) const
{
	if (IsGenerating ())
	{
		AddLogEntry (true, LogLevel_Default, false, "Please wait until generation will be complete....");

		return false;
	}

	if (m_areas.IsEmpty ())
	{
		AddLogEntry (true, LogLevel_Default, false, "Navigation mesh is empty! Nothing to save.");

		return false;
	}

	WarnIfMeshNeedsAnalysis ();

	DynamicString directory (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Navigation mesh");

	if (!IsDirectoryExists (directory))
	{
		if (!CreatePath (directory))
			return false;
	}
	else if (!IsDirectoryExists (directory += "/OBJs") && !CreatePath (directory))
		return false;

	STDIOFile file (directory + '/' + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".OBJ", "w");

	if (!file.IsValid ())
		return false;

	unsigned int base (1u);

	// store each area....
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->SaveToOBJFile (file, base);

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/*** Save a navigation area to the opened binary stream*/
void NavigationMesh::NavigationArea::Save (BaseFile &file) const
{
	// Call this function in base class....
	BaseNavigationArea::Save (file);

	// save attribute flags
	file.Write (m_attributeFlags);

	// save extent of area
	file.Write (m_extent);

	// save heights of implicit corners
	file.Write (m_neZ);
	file.Write (m_swZ);

	// save connections (IDs) to adjacent areas in the enumeration order: NORTH, EAST, SOUTH, WEST
	SaveConnectedAreas (file);

	//
	// Store hiding spots for this area
	//
	SaveHidingSpots (file);

	//
	// Save the approach areas for this area
	//
	SaveApproachAreas (file);

	//
	// Save encounter spots for this area
	//
	SaveSpotEncounters (file);

	//
	// Save Place data
	//
	SavePlaceData (file);

	//
	// Write out ladder info
	//
	SaveConnectedLadders (file);

	//
	// Save earliest occupy times
	//
	SaveEarliestOccupyTimes (file);
}

//--------------------------------------------------------------------------------------------------------------
/*** Load a navigation area from the file*/
const bool NavigationMesh::NavigationArea::Load (const BaseFile &file)
{
	// Call this function in base class....
	if (!BaseNavigationArea::Load (file))
		return false;

	// update next ID to avoid ID collisions by later areas
	if (m_id > g_server->GetNavigationMeshManager ().GetNextAreaID ())
		g_server->GetNavigationMeshManager ().SetNextAreaID (m_id);

	// load attribute flags
	if (!file.Read (m_attributeFlags))
		return false;

	// load extent of area
	if (!file.Read (m_extent))
		return false;

	m_center = m_extent.mins.GetMidPoint (m_extent.maxs);

	if (IsDegenerate ())
	{
		m_extentInvertedSizeX = 0.0f;
		m_extentInvertedSizeY = 0.0f;

		AddLogEntry (true, LogLevel_Warning, false, "Degenerate Navigation Area #%d at position %s.", GetID (), m_center.ToString ());
	}
	else
	{
		m_extentInvertedSizeX = m_extent.GetInvertedSizeX ();
		m_extentInvertedSizeY = m_extent.GetInvertedSizeY ();
	}

	// load heights of implicit corners
	if (!file.Read (m_neZ) || !file.Read (m_swZ))
		return false;

	CheckWaterLevel ();

	// load connections (IDs) to adjacent areas in the enumeration order: NORTH, EAST, SOUTH, WEST
	if (!LoadConnectedAreas (file))
		return false;

	//
	// Load hiding spots
	//
	if (!LoadHidingSpots (file))
		return false;

	//
	// Load approach areas
	//
	if (!LoadApproachAreas (file))
		return false;

	//
	// Load encounter paths for this area
	//
	if (!LoadSpotEncounters (file))
		return false;

	//
	// Load Place data
	//
	if (!LoadPlaceData (file))
		return false;

	//
	// Load ladder data
	//
	if (!LoadConnectedLadders (file))
		return false;

	//
	// Load earliest occupy times
	//
	if (!LoadEarliestOccupyTimes (file))
		return false;

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/*** Convert loaded IDs to pointers* Make sure all IDs are converted, even if corrupt data is encountered.*/
const NavigationMesh::NavErrorType_t NavigationMesh::NavigationArea::PostLoad (void)
{
	NavErrorType_t error (NAV_OK);

	for (unsigned char direction (NavigationLadder::Direction_Up); direction < NavigationLadder::Direction_Total; ++direction)
		for (NavLadderConnectArray::IndexType_t index (0u); index < m_connectedLadders[direction].GetElementNumber (); ++index)
		{
			NavigationLadder *&connect = m_connectedLadders[direction][index];
			const ID_t id (reinterpret_cast <ID_t> (connect));

			if (!g_server->GetNavigationMeshManager ().GetLadders ().IsMember (connect))
				connect = g_server->GetNavigationMeshManager ().GetLadderByID (id);

			if (id > 0u && connect == NULL)
			{
				AddLogEntry (true, LogLevel_Default, false, "NavigationArea::PostLoad(): Corrupt navigation ladder data. Cannot connect Navigation Areas.");

				error = NAV_CORRUPT_DATA;
			}
		}

	// connect areas together
	for (unsigned char direction (NORTH); direction < NUM_DIRECTIONS; ++direction)
		for (NavAreaConnectArray::IndexType_t index (0u); index < m_connectedAreas[direction].GetElementNumber (); ++index)
		{
			NavigationArea *&connect = m_connectedAreas[direction][index];
			const ID_t id (reinterpret_cast <ID_t> (connect));

			connect = g_server->GetNavigationMeshManager ().GetNavAreaByID (id);

			if (id > 0u && connect == NULL)
			{
				AddLogEntry (true, LogLevel_Default, false, "NavigationArea::PostLoad(): Corrupt navigation data. Cannot connect Navigation Areas.");

				error = NAV_CORRUPT_DATA;
			}
		}

//	BuildPortals ();

	// resolve approach area IDs
	for (ApproachArray::IndexType_t approachIndex (0u); approachIndex < m_approach.GetElementNumber (); ++approachIndex)
		if (!m_approach[approachIndex].PostLoad ())
			error = NAV_CORRUPT_DATA;

	// resolve spot encounter IDs
	for (SpotEncounterArray::IndexType_t index (0u); index < m_spotEncounters.GetElementNumber (); ++index)
		if (!m_spotEncounters[index]->PostLoad (this))
			error = NAV_CORRUPT_DATA;

	// build overlap areas list
	BuildOverlapAreas ();

	return error;
}

//--------------------------------------------------------------------------------------------------------------
/*** Determine the earliest time this hiding spot can be reached by either team*/
void NavigationMesh::NavigationArea::ComputeEarliestOccupyTimes (void)
{
	const float playerSpeed (HalfLifeEngine::SDK::Constants::MaximumDefaultPlayerSpeed/*EDWARD: WAS 240.0*/);	// standart maximum player speed in units/second
	unsigned char teamIndex;

	for (teamIndex = TeamArrayID_Terrorist; teamIndex < TeamArrayID_Total; ++teamIndex)
	{
		// no spot in the map should take longer than this to reach
		m_earliestOccupyTime[teamIndex] = playerSpeed * 0.5f;
	}

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_quicksave)->GetValue <bool> ())
		return;

	ShortestPathCost cost;
	Server::SpawnPointsContainer::SpawnPointsArray::IndexType_t spawnPointIndex;

	// determine the shortest time it will take a team to reach this area
	for (teamIndex = TeamArrayID_Terrorist; teamIndex < TeamArrayID_Total; ++teamIndex)
		for (spawnPointIndex = 0u; spawnPointIndex < g_server->GetSpawnPointsContainer ()[static_cast <TeamArrayID_t> (teamIndex)].GetElementNumber (); ++spawnPointIndex)
		{
			const float travelDistance (NavAreaTravelDistance (g_server->GetSpawnPointsContainer ()[static_cast <TeamArrayID_t> (teamIndex)][spawnPointIndex], GetCenter (), cost));

			if (travelDistance < 0.0f)
				continue;

			const float travelTime (travelDistance / playerSpeed);

			if (travelTime < m_earliestOccupyTime[teamIndex])
				m_earliestOccupyTime[teamIndex] = travelTime;
		}
}

//--------------------------------------------------------------------------------------------------------------
/*** Save a navigation ladder to the opened binary stream*/
void NavigationMesh::NavigationLadder::Save (BaseFile &file) const
{
	// Call this function in base class....
	BaseNavigationArea::Save (file);

	// save extent of ladder
	file.Write (m_width);

	// save top endpoint of ladder
	file.Write (m_top);

	// save bottom endpoint of ladder
	file.Write (m_bottom);

	// save direction
	file.Write (m_direction);

	// save IDs of connecting areas
	BaseNavigationArea::ID_t id;

	for (unsigned char direction (ConnectionType_TopBehind); direction < ConnectionType_Total; ++direction)
	{
		id = m_connectedAreas[direction] != NULL ? m_connectedAreas[direction]->GetID () : 0u;

		file.Write (id);
	}
}

//--------------------------------------------------------------------------------------------------------------
/*** Load a navigation ladder from the opened binary stream*/
const bool NavigationMesh::NavigationLadder::Load (const BaseFile &file)
{
	// Call this function in base class....
	if (!BaseNavigationArea::Load (file))
		return false;

	// update next ID to avoid ID collisions by later ladders
	if (m_id > g_server->GetNavigationMeshManager ().IncreaseAndGetNextLadderID ())
		g_server->GetNavigationMeshManager ().SetNextLadderID (m_id);

	// load extent of ladder
	if (!file.Read (m_width))
		return false;

	// load top endpoint of ladder
	if (!file.Read (m_top))
		return false;

	// load bottom endpoint of ladder
	if (!file.Read (m_bottom))
		return false;

	m_center = m_top.GetMidPoint (m_bottom);	// compute center of ladder
	m_length = m_top.GetDistance (m_bottom);	// compute ladder length

	// load direction
	if (!file.Read (m_direction))
		return false;

	m_edict = FindEdict (m_center);

	// Reliability check.
	InternalAssert (m_edict->IsValid ());

	SetDirection (m_direction);	// regenerate the surface normal
/*
	// load dangling status
	if (version == 6u)
	{
		bool m_isDangling;

		if (!file.Read (m_isDangling))
			return false;
	}
*/
	// load IDs of connecting areas
	BaseNavigationArea::ID_t id;

	for (unsigned char direction (ConnectionType_TopBehind); direction < ConnectionType_Total; ++direction)
	{
		if (!file.Read (id))
			return false;

		m_connectedAreas[direction] = g_server->GetNavigationMeshManager ().GetNavAreaByID (id);
	}

	if (m_connectedAreas[ConnectionType_Bottom] == NULL && Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Unconnected ladder #%u bottom at %s.", m_id, m_bottom.ToString ());

//		DevWarning ("nav_unmark; nav_mark ladder %d; nav_warp_to_mark\n", m_id);
	}

	// can't include behind area, since it is not used when going up a ladder
	if (m_connectedAreas[ConnectionType_TopBehind] == NULL && m_connectedAreas[ConnectionType_TopLeft] == NULL && m_connectedAreas[ConnectionType_TopRight] == NULL && Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Unconnected ladder #%u top at %s.", m_id, m_top.ToString ());

//		DevWarning ("nav_unmark; nav_mark ladder %d; nav_warp_to_mark\n", m_id);
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------
/*** Determine if this area is a "battlefront" area - where two rushing teams first meet.*/
void NavigationMesh::ComputeBattlefrontAreas (void)
{
#if 0
	ShortestPathCost cost;

	for (unsigned char terroristSpawnPointIndex (0u), counterTerroristSpawnPointIndex; terroristSpawnPointIndex < g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_Terrorist].GetElementNumber (); ++terroristSpawnPointIndex)
	{
		NavigationArea *const tArea (GetNavArea (g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_Terrorist][terroristSpawnPointIndex]));

		if (tArea == NULL)
			continue;

		for (counterTerroristSpawnPointIndex = 0u; counterTerroristSpawnPointIndex < g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist].GetElementNumber (); ++counterTerroristSpawnPointIndex)
		{
			NavigationArea *const ctArea (GetNavArea (g_server->GetSpawnPointsContainer ()[HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist][counterTerroristSpawnPointIndex]));

			if (ctArea == NULL)
				continue;

			if (tArea == ctArea)
			{
				tArea->m_isBattlefront = true;

				return;
			}

			// build path between these two spawn points - assume if path fails, it at least got close
			// (ie: imagine spawn points that you jump down from - can't path to)
			NavigationArea *goalArea (NULL);

			NavAreaBuildPath (tArea, ctArea, NULL, cost, &goalArea);

			if (goalArea == NULL)
				continue;
/*** @todo Need to enumerate ALL paths between all pairs of spawn points to find all battlefront areas*/
			// find the area with the earliest overlapping occupy times
			NavigationArea *battlefront (NULL);
			float earliestTime (999999.9f);
			const float epsilon (1.0f);

			for (NavigationArea *area (goalArea); area != NULL; area = area->GetParent ())
				if (Math::AreEqual (area->GetEarliestOccupyTime (HalfLifeEngine::SDK::Constants::TeamID_Terrorist), area->GetEarliestOccupyTime (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist), epsilon))
				{
				}
		}
	}
#endif
}

//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::WarnIfMeshNeedsAnalysis (void) const
{
	// Quick check to warn about needing to analyze: yb_nav_strip, yb_nav_delete, etc set
	// every NavigationArea's m_approach.GetElementNumber() to 0, and delete their m_spotEncounters.
	// So, if no area has either, odds are good we need an analyze.

	bool hasApproachAreas (false), hasSpotEncounters (false);

	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		const NavigationArea *const area (m_areas[index]);

		if (!area->m_approach.IsEmpty ())
			hasApproachAreas = true;

		if (!area->m_spotEncounters.IsEmpty ())
			hasSpotEncounters = true;
	}

	if (!hasApproachAreas || !hasSpotEncounters)
		AddLogEntry (true, LogLevel_Warning, false, "The navigation mesh needs a full '" PRODUCT_CONSOLE_TAG "_nav_analyze'.");
}

/*** Store Navigation Mesh to a file*/
const bool NavigationMesh::Save (void)
{
	if (IsGenerating ())
	{
		AddLogEntry (true, LogLevel_Default, false, "Please wait until generation will be complete....");

		return false;
	}

	if (m_areas.IsEmpty ())
	{
		AddLogEntry (true, LogLevel_Default, false, "Navigation mesh is empty! Nothing to save.");

		return false;
	}

	WarnIfMeshNeedsAnalysis ();

	DynamicString directory (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Navigation mesh");

	if (!IsDirectoryExists (directory))
	{
		if (!CreatePath (directory))
			return false;
	}
	else if (!IsDirectoryExists (directory += "/Maps") && !CreatePath (directory))
		return false;

//	const DynamicString filename (GetFilename ());	// navigation filename is derived from map filename

	// get size of source BSP file for later (before we open the navigation file for writing, in case of failure)
	STDIOFile file (directory + '/' + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + '.' + FileExtention/*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + filename*/, "wb");

	if (!file.IsValid ())
		return false;

	//
	// Store the NAV file
	//

	// store "magic number" to help identify this kind of file
	file.Write (NAV_MAGIC_NUMBER);

	// store version number of file
	// 1 = hiding spots as plain vector array
	// 2 = hiding spots as HidingSpot objects
	// 3 = Encounter spots use HidingSpot ID's instead of storing vector again
	// 4 = Includes size of source BSP file to verify navigation data correlation
	// ---- Beta Release at V4 -----
	// 5 = Added Place info
	// ---- Conversion to Src ------
	// 6 = Added Ladder info
	// 7 = Areas store ladder ID's so ladders can have one-way connections
	// 8 = Added earliest occupy times (2 floats) to each area
	// 9 = Promoted NavigationArea's attribute flags to a 'unsigned short' type
	// 10 = UNKNOWN
	// 11 = UNKNOWN
	// 12 = UNKNOWN
	// 13 = UNKNOWN
	// 14 = UNKNOWN
	// 15 = UNKNOWN
	// 16 = UNKNOWN
	file.Write (NavCurrentVersion);

	const DynamicString bspPathname ("maps/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".BSP");
	const unsigned int bspFileSize (HalfLifeEngine::Globals::g_halfLifeEngine->GetFileSize (bspPathname));

	if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
		AddLogEntry (true, LogLevel_Default, false, "Size of BSP file '%s' is %u bytes.", bspPathname.GetData (), bspFileSize);

	// store the size of source BSP file in the navigation file so we can test if the BSP changed since the navigation file was made
	file.Write (bspFileSize);

	//
	// Build a directory of the Places in this map before storing navigation areas
	//
	{
		m_placeDirectory.Rebuild ();

		m_placeDirectory.Save (file);
	}

	//
	// Store navigation areas
	//
	SaveAreas (file);

	//
	// Store ladders
	//
	SaveLadders (file);

	file.Flush ();

	if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Developer)->GetValue <bool> ())
		AddLogEntry (true, LogLevel_Default, false, "Size of navigation file '%s' is %u bytes.", /*filename*/GetFilename ().GetData (), file.GetSize ());

	return true;
}


//--------------------------------------------------------------------------------------------------------------
static NavigationMesh::NavErrorType_t CheckNavFile (const char *const bspFilename)
{
	if (bspFilename == NULL)
		return NavigationMesh::NAV_CANT_ACCESS_FILE;

	char bspPathname[256u];
	char filename[256u];

	strncpy (bspPathname, "Maps/", sizeof (bspPathname));
	strncat (bspPathname, bspFilename, sizeof (bspPathname));
	strncpy (filename, bspPathname, sizeof (filename));

	// Replace BSP file extension to NAV.
	{
		const unsigned short length (GetStringLength <unsigned short> (filename));

		filename[length - 3u] = NavigationMesh::FileExtention[0u];
		filename[length - 2u] = NavigationMesh::FileExtention[1u];
		filename[length - 1u] = NavigationMesh::FileExtention[2u];
	}

	bool navIsInBsp (false);
	STDIOFile file (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + filename, "rb");	// this ignores .NAV files embedded in the .BSP ...

	if (!file.IsValid ())
	{
		navIsInBsp = true;

//		file.Open (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + filename, "rb");	// ... and this looks for one if it's the only one around.
	}

	if (!file.IsValid ())
		return NavigationMesh::NAV_CANT_ACCESS_FILE;

	// check magic number
	unsigned int magic;

	if (!file.Read (magic) || magic != NavigationMesh::NAV_MAGIC_NUMBER)
		return NavigationMesh::NAV_INVALID_FILE;

	// read file version number
	unsigned char version;

	if (!file.Read (version) || version > NavigationMesh::NavCurrentVersion)
		return NavigationMesh::NAV_BAD_FILE_VERSION;

	unsigned int savedBspSize;

	// get size of source BSP file and verify that the BSP hasn't changed
	file.Read (savedBspSize);

	// verify size
	const unsigned int originalBspSize (HalfLifeEngine::Globals::g_halfLifeEngine->GetFileSize (bspPathname));

	return originalBspSize != savedBspSize && !navIsInBsp ? NavigationMesh::NAV_FILE_OUT_OF_DATE : NavigationMesh::NAV_OK;
}

/*
//--------------------------------------------------------------------------------------------------------------
void CommandNavCheckFileConsistency (void)
{
	if (!UTIL_IsCommandIssuedByServerAdmin ())
		return;

	FileFindHandle_t findHandle;
	const char *bspFilename (filesystem->FindFirstEx ("maps/*.BSP", "MOD", &findHandle));

	while (bspFilename != NULL)
	{
		switch (CheckNavFile (bspFilename))
		{
			case NavigationMesh::NAV_CANT_ACCESS_FILE:
				AddLogEntry (true, LogLevel_Warning, false, "Missing navigation file for %s.", bspFilename);

				break;

			case NavigationMesh::NAV_INVALID_FILE:
				AddLogEntry (true, LogLevel_Warning, false, "Invalid navigation file for %s.", bspFilename);

				break;

			case NavigationMesh::NAV_BAD_FILE_VERSION:
				AddLogEntry (true, LogLevel_Warning, false, "Old navigation file for %s.", bspFilename);

				break;

			case NavigationMesh::NAV_FILE_OUT_OF_DATE:
				AddLogEntry (true, LogLevel_Warning, false, "The navigation file for %s is built from an old version of the map.", bspFilename);

				break;

			case NavigationMesh::NavErrorType_NotEnoughMemory:
				AddLogEntry (true, LogLevel_Warning, false, "Not enough memory for navigation file %s.", bspFilename);

				break;

			default:	// NAV_OK
				AddLogEntry (true, LogLevel_Default, false, "The navigation file for %s is up-to-date.", bspFilename);

				break;
		}

		bspFilename = filesystem->FindNext (findHandle);
	}

	filesystem->FindClose (findHandle);
}
static ConCommand nav_check_file_consistency ("nav_check_file_consistency", CommandNavCheckFileConsistency, "Scans the maps directory and reports any missing/out-of-date navigation files.", FCVAR_GAMEDLL | FCVAR_CHEAT);
*/

//--------------------------------------------------------------------------------------------------------------
/*** Load AI navigation data from a file*/
const NavigationMesh::NavErrorType_t NavigationMesh::Load (void)
{
	// free previous navigation mesh data
	Reset ();
	m_placeDirectory.Reset ();

	m_nextAreaID = 0u;	// Reset the next area ID to 0

	const DynamicString filename (GetFilename ());	// navigation filename is derived from map filename
	bool navIsInBsp (false);
	STDIOFile file (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + filename, "rb"/*, "MOD"*/);	// this ignores .NAV files embedded in the .BSP...

	if (!file.IsValid ())
	{
		navIsInBsp = true;

//		file.Open (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + filename, "rb"/*, "GAME"*/);	// ... and this looks for one if it's the only one around.
	}

	if (!file.IsValid ())
		return NAV_CANT_ACCESS_FILE;

	unsigned int magic;

	// check magic number
	if (!file.Read (magic) || magic != NAV_MAGIC_NUMBER)
		return NAV_INVALID_FILE;

	// read file version number
	unsigned char version;

	if (!file.Read (version) || version > NavCurrentVersion)
		return NAV_BAD_FILE_VERSION;

	{
		unsigned int saveBspSize;

		// get size of source BSP file and verify that the BSP hasn't changed
		if (!file.Read (saveBspSize))
			return NAV_BAD_FILE_VERSION;

		// verify size
		const unsigned int bspFileSize (HalfLifeEngine::Globals::g_halfLifeEngine->GetFileSize ("maps/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".BSP"));

		if (bspFileSize != saveBspSize && !navIsInBsp)
		{
			AddLogEntry (true, LogLevel_Warning, false, "The Navigation Mesh was built using a different version of this map.");

			m_isFromCurrentMap = false;
		}
	}

	// load Place directory
	if (!m_placeDirectory.Load (file))
		return NAV_BAD_FILE_VERSION;

	//
	// Set up all the areas
	//
	{
		const NavErrorType_t loadResult (LoadAreas (file));

		if (loadResult != NAV_OK)
			return loadResult;
	}

	//
	// Set up all the ladders
	//
	{
		const NavErrorType_t loadResult (LoadLadders (file));

		if (loadResult != NAV_OK)
			return loadResult;
	}

	// allow areas to connect to each other, etc
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		m_areas[index]->PostLoad ();

	// allow hiding spots to compute information
	for (HidingSpotArray::IndexType_t index (0u); index < m_hidingSpots.GetElementNumber (); ++index)
		m_hidingSpots[index]->PostLoad ();
/*
	if (version < 8u)
	{
		// Old navigation meshes need to compute earliest occupy times
		for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
			m_areas[index]->ComputeEarliestOccupyTimes ();
	}
*/
	ComputeBattlefrontAreas ();

	// the Navigation Mesh has been successfully loaded
	m_loadState = LoadState_FromFile;

	WarnIfMeshNeedsAnalysis ();

	YaPB::RebuildAreaMapZones ();

	return NAV_OK;
}

const NavigationMesh::NavErrorType_t NavigationMesh::LoadAreas (const BaseFile &file)
{
	NavAreaArray::IndexType_t count;

	// get number of areas
	if (!file.Read (count))
		return NAV_BAD_FILE_VERSION;

	Math::Extent2D totalExtent (m_worldMap.GetBSPFile ().models[0u].boundingBox/*9999999999.9f, -9999999999.9f*/);

	// load the areas and compute total extent
	for (NavAreaArray::IndexType_t areaIndex (0u); areaIndex < count; ++areaIndex)
	{
		NavigationArea *const area (new NavigationArea ());

		#if defined _DEBUG
			// Reliability check.
			if (area == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new navigation area!", sizeof (*area));

				return NavErrorType_NotEnoughMemory;
			}
		#endif	// if defined _DEBUG

		if (!area->Load (file))
		{
			delete area;

			return NAV_BAD_FILE_VERSION;
		}

		m_areas += area;

		const Math::Extent3D &areaExtent (area->GetExtent ());

		// check validity of navigation area
		if (areaExtent.mins.x >= areaExtent.maxs.x || areaExtent.mins.y >= areaExtent.maxs.y)
			AddLogEntry (true, LogLevel_Warning, false, "Degenerate Navigation Area #%d at position %s.", area->GetID (), area->m_center.ToString ());

		totalExtent.mins.SetMinimum (areaExtent.mins);
		totalExtent.maxs.SetMaximum (areaExtent.maxs);
	}

	AllocateGrid (totalExtent);

	// add the areas to the navigation mesh
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
		AddArea (m_areas[index]);

	return NAV_OK;
}

const NavigationMesh::NavErrorType_t NavigationMesh::LoadLadders (const BaseFile &file)
{
	NavLadderArray::IndexType_t count;

	// get number of ladders
	if (!file.Read (count))
		return NAV_BAD_FILE_VERSION;

	// load the ladders
	for (NavLadderArray::IndexType_t ladderIndex (0u); ladderIndex < count; ++ladderIndex)
	{
		NavigationLadder *const ladder (new NavigationLadder ());

		#if defined _DEBUG
			// Reliability check.
			if (ladder == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating navigation ladder!", sizeof (*ladder));

				return NavErrorType_NotEnoughMemory;
			}
		#endif	// if defined _DEBUG

		if (!ladder->Load (file))
		{
			delete ladder;

			return NAV_BAD_FILE_VERSION;
		}

		m_ladders += ladder;
	}

	return NAV_OK;
}

void NavigationMesh::WriteNavigationAreasBitmap (const DynamicString &filename) const
{
	// this function allocates memory and clears the debug bitmap buffer.

	if (m_areas.IsEmpty ())
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("There are no navigation areas, nothing to draw...\n");

		return;
	}

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Drawing global navigation mesh (%u navigation areas) to bitmap file...\n", m_areas.GetElementNumber ());

	BMPFileCreator BMPFile (m_worldMap.GetBSPFile ().models[0u].boundingBox);

	// cycle through all walk faces in the navmesh...
	for (NavAreaArray::IndexType_t index (0u); index < m_areas.GetElementNumber (); ++index)
	{
		// this function necessary so as to draw each of the edges of the walk face pointed to by walk face, using the palette color specified by color.

		const NavigationArea *const navigationArea (m_areas[index]);

		// and draw them in the bitmap....

		// Draw extent....
		BMPFile.DrawLine (navigationArea->GetExtent ().mins, Math::Vector2D (navigationArea->GetExtent ().maxs.x, navigationArea->GetExtent ().mins.y), BMPFileCreator::Color_Yellow);
		BMPFile.DrawLine (Math::Vector2D (navigationArea->GetExtent ().maxs.x, navigationArea->GetExtent ().mins.y), navigationArea->GetExtent ().maxs, BMPFileCreator::Color_Yellow);
		BMPFile.DrawLine (navigationArea->GetExtent ().maxs, Math::Vector2D (navigationArea->GetExtent ().mins.x, navigationArea->GetExtent ().maxs.y), BMPFileCreator::Color_Yellow);
		BMPFile.DrawLine (Math::Vector2D (navigationArea->GetExtent ().mins.x, navigationArea->GetExtent ().maxs.y), navigationArea->GetExtent ().mins, BMPFileCreator::Color_Yellow);

		// Draw hidding spots....
		for (NavigationArea::HidingSpotArray::IndexType_t hidingSpotIndex (0u); hidingSpotIndex < navigationArea->m_hidingSpots.GetElementNumber (); ++hidingSpotIndex)
		{
			const NavigationArea::HidingSpot *const spot (navigationArea->m_hidingSpots[hidingSpotIndex]);

			BMPFile.DrawPoint (spot->GetPosition (), spot->IsIdealSniperSpot () ? BMPFileCreator::Color_Red/*NavIdealSniperColor*/ : spot->IsGoodSniperSpot () ? BMPFileCreator::Color_Pink/*NavGoodSniperColor*/ : spot->HasGoodCover () ? BMPFileCreator::Color_Green/*NavGoodCoverColor*/ : BMPFileCreator::Color_Pink/*NavExposedColor*/);
		}
	}

	// and then draw it
	if (BMPFile.Write (filename))
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("BMP file created: '%s'.\n", filename.GetData ());

	// finished, navigation areas has been printed into the BMP dot matrix.
}

//--------------------------------------------------------------------------------------------------------------
/**
* Load the place names from a file
*/
void NavigationMesh::PlaceContainer::LoadPlaceDatabase (void)
{
	STDIOFile file (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Navigation mesh/Places.db");

	if (!file.IsValid ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Bot navigation place name database file not found (Addons/YaPB/Navigation mesh/Places.db)!");

		return;
	}

	const unsigned char maximumPlaceNameLength (128u);
	DynamicString line;

	// for each line in the file...
	while (file.ReadLine (line, maximumPlaceNameLength))
	{
		// trim all the blanks or linefeeds
		line.Trim ();

		// skip all comment lines or empty lines
		if (IsEmptyOrCommentedFileBufferLine (line))
			continue;

		if (m_places.IsMember (line.MakeLower ()))
		{
			AddLogEntry (true, LogLevel_Warning, false, "Duplicate place name \"%s\" in Places.db file!", line.GetData ());

			continue;
		}

		m_places += line;
	}

	if (m_places.IsEmpty ())
		AddLogEntry (true, LogLevel_Warning, false, "Not one place name has been found in a Places.db file!");
	else
	{
		m_places.Sort (Compare);

		AddLogEntry (true, LogLevel_Default, false, "%u place names loaded.", m_places.GetElementNumber ());
	}
}