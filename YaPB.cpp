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
// YaPB.cpp
//
// Class: YaPB
//
// Description: Features the core AI of the YaPB.
//
// Version: $ID:$
//

#include <Core.hpp>

#if defined USE_SPEECH_ENGINES
#	include <C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/winui/speech/simpleaudio/simpleaudio_i.c>
#endif	// if defined USE_SPEECH_ENGINES

#if defined _DEBUG
	YaPB::DebugLevel_t YaPB::s_debugLevel;
#endif	// if defined _DEBUG

const float YaPB::s_minimumBurstDistance (256.0f);
float YaPB::sm_maximumViewDistance (HalfLifeEngine::SDK::Constants::HalfMapSize);	// just some crazy value

// weapon firing delay based on skill (min and max delay for each weapon)
const YaPB::FireDelay_t YaPB::s_fireDelays[NumberWeapons + 1u] =
{
	// knife
	{HalfLifeEngine::SDK::Constants::WeaponID_Knife,     255u, s_minimumBurstDistance,         0.10f, {0.0f, 0.2f, 0.3f, 0.4f, 0.6f, 0.8f}, {0.1f, 0.3f, 0.5f, 0.7f, 1.0f, 1.2f}/*, 0.0f, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}*/},

	// pistols
	{HalfLifeEngine::SDK::Constants::WeaponID_USP,         3u, s_minimumBurstDistance / 0.3f,  0.15f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_Glock18,     5u, s_minimumBurstDistance / 0.3f,  0.15f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_Deagle,      2u, s_minimumBurstDistance / 0.4f,  0.20f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_P228,        4u, s_minimumBurstDistance / 0.3f,  0.14f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_Elite,       3u, s_minimumBurstDistance / 0.4f,  0.20f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_FiveSeven,   4u, s_minimumBurstDistance / 0.35f, 0.14f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},

	// shotguns
	{HalfLifeEngine::SDK::Constants::WeaponID_M3,          8u, s_minimumBurstDistance / 0.7f,  0.86f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_XM1014,      7u, s_minimumBurstDistance / 0.5f,  0.15f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},

	// sub machine guns
	{HalfLifeEngine::SDK::Constants::WeaponID_MP5,         4u, s_minimumBurstDistance / 0.35f, 0.10f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_TMP,         3u, s_minimumBurstDistance / 0.35f, 0.05f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_P90,         4u, s_minimumBurstDistance / 0.35f, 0.10f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_MAC10,       3u, s_minimumBurstDistance / 0.35f, 0.06f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_UMP45,       4u, s_minimumBurstDistance / 0.35f, 0.15f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},

	// rifles
	{HalfLifeEngine::SDK::Constants::WeaponID_AK47,        2u, s_minimumBurstDistance / 0.5f,  0.09f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_SG552,       3u, s_minimumBurstDistance / 0.5f,  0.11f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_M4A1,        3u, s_minimumBurstDistance / 0.5f,  0.08f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_Galil,       4u, s_minimumBurstDistance / 0.5f,  0.09f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_Famas,       4u, s_minimumBurstDistance / 0.5f,  0.10f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_Aug,         3u, s_minimumBurstDistance / 0.5f,  0.11f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},

	// sniper rifles
	{HalfLifeEngine::SDK::Constants::WeaponID_Scout,      10u, s_minimumBurstDistance,         0.18f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_AWP,        10u, s_minimumBurstDistance / 1.5f,  0.22f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_G3SG1,       4u, s_minimumBurstDistance,         0.25f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},
	{HalfLifeEngine::SDK::Constants::WeaponID_SG550,       4u, s_minimumBurstDistance,         0.25f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},

	// machine guns
	{HalfLifeEngine::SDK::Constants::WeaponID_M249,        3u, s_minimumBurstDistance / 0.4f,  0.10f, {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.6f}, {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.7f}/*, 0.2f, {0.0f, 0.0f, 0.1f, 0.1f, 0.2f}, {0.1f, 0.1f, 0.2f, 0.2f, 0.4f}*/},

	// shield
	{HalfLifeEngine::SDK::Constants::WeaponID_Shield,      0u, s_minimumBurstDistance,          0.0f, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}/*, 0.0f, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}*/},

	// terminator
	{HalfLifeEngine::SDK::Constants::WeaponID_None,        0u, s_minimumBurstDistance,          0.0f, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}/*, 0.0f, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}*/}
};

// this array stores the skill based delays for an enemy surprise delay and the pause/camping delays (weak bots are longer surprised and do pause/camp longer as well), aiming offsets,
// headshot frequency and the ShootThruWalls probability (worst to best skill) overridden by general.cfg
YaPB::SkillDefinition_t YaPB::s_skillTables[] =
{
	{0.8f, 1.0f, 45.0f, 65.0f, 2.0f,  3.0f, Math::Vector3D (40.0f, 40.0f, 50.0f),   0u,   0u,   0u, 50u},
	{0.6f, 0.8f, 40.0f, 60.0f, 3.0f,  4.0f, Math::Vector3D (30.0f, 30.0f, 42.0f),  10u,   0u,   0u, 40u},
	{0.4f, 0.6f, 35.0f, 55.0f, 4.0f,  6.0f, Math::Vector3D (20.0f, 20.0f, 32.0f),  30u,   0u,  50u, 35u},
	{0.2f, 0.3f, 30.0f, 50.0f, 6.0f,  8.0f, Math::Vector3D (10.0f, 10.0f, 18.0f),   0u,  30u,  80u, 30u},
	{0.1f, 0.2f, 25.0f, 40.0f, 8.0f, 10.0f, Math::Vector3D ( 5.0f,  5.0f, 10.0f),  80u,  50u, 100u, 23u},
	{0.0f, 0.1f, 20.0f, 30.0f, 9.0f, 12.0f, Math::Vector3D ( 0.0f,  5.0f,  0.0f), 100u, 100u, 100u, 20u}
};

void YaPB::LoadBotSkillTables (void)
{
	// GENERAL DATA INITIALIZATION

	STDIOFile file;

	// opens file readonly
	if (!OpenConfigurationFile ("Skill tables", "Bot skill tables configuration file not found (configs/Skill tables.cfg). Loading defaults.", file))
		return;

	DynamicString line;
	unsigned int lineIndex (0u);
	unsigned char skillTableIndex;

	// for each line in the file...
	while (file.ReadLine (line, 256u))
	{
		++lineIndex;

		line.Trim ();	// trim all the blanks or linefeeds

		// skip all comment lines or empty lines
		if (IsEmptyOrCommentedFileBufferLine (line))
			continue;

		DynamicArray <DynamicString, unsigned char> pairs (line.Split <unsigned char> ('='));

		if (pairs.GetElementNumber () != 2u)
		{
			AddLogEntry (true, LogLevel_Error, false, "Error in general bot configuration file syntax... Please correct all errors.");

			return;
		}

		// trim all the blanks or linefeeds
		pairs.GetFirstElement ().Trim ();
		pairs.GetLastElement ().Trim ();

		const DynamicArray <DynamicString, unsigned char> &splitted (pairs.GetLastElement ().Split <unsigned char> (','));

		if (splitted.GetElementNumber () != 8u)
		{
			AddLogEntry (true, LogLevel_Error, false, "%s entry in skill tables configuration file, on a line: %u is not valid.", pairs.GetFirstElement ().GetData (), lineIndex);

			return;
		}

		if (pairs.GetFirstElement () == "Stupid")
			skillTableIndex = 0u;
		else if (pairs.GetFirstElement () == "Newbie")
			skillTableIndex = 1u;
		else if (pairs.GetFirstElement () == "Average")
			skillTableIndex = 2u;
		else if (pairs.GetFirstElement () == "Advanced")
			skillTableIndex = 3u;
		else if (pairs.GetFirstElement () == "Professional")
			skillTableIndex = 4u;
		else if (pairs.GetFirstElement () == "Godlike")
			skillTableIndex = 5u;
		else
		{
			AddLogEntry (true, LogLevel_Error, false, "%s entry in skill tables configuration file, on a line: %u is unknown!", pairs.GetFirstElement ().GetData (), lineIndex);

			continue;
		}

		s_skillTables[skillTableIndex].minSurpriseTime    = splitted[0u].GetValue <float> ();
		s_skillTables[skillTableIndex].maxSurpriseTime    = splitted[1u].GetValue <float> ();
		s_skillTables[skillTableIndex].minTurnSpeed       = splitted[2u].GetValue <float> ();
		s_skillTables[skillTableIndex].maxTurnSpeed       = splitted[3u].GetValue <float> ();
		s_skillTables[skillTableIndex].headshotFrequency  = splitted[4u].GetValue <unsigned char> ();
		s_skillTables[skillTableIndex].heardShootThruProb = splitted[5u].GetValue <unsigned char> ();
		s_skillTables[skillTableIndex].seenShootThruProb  = splitted[6u].GetValue <unsigned char> ();
		s_skillTables[skillTableIndex].recoilAmount       = splitted[7u].GetValue <unsigned char> ();
	}

	AddLogEntry (true, LogLevel_Default, false, "Bot skill tables configuration file Skill tables.cfg loaded successfully.");
}

YaPB::WaypointMapZonesArray YaPB::s_bombPlaceWaypointMapZones;
YaPB::WaypointMapZonesArray YaPB::s_hostageWaypointMapZones;
YaPB::WaypointMapZonesArray YaPB::s_hostageRescueWaypointMapZones;
YaPB::WaypointMapZonesArray YaPB::s_VIPSafetyWaypointMapZones;

void YaPB::BuildWaypointMapZones (void)
{
	for (HalfLifeEngine::SDK::Classes::Edict *bombPlace (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_bomb_target")); bombPlace->IsValid (); bombPlace = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (bombPlace, "func_bomb_target"))
	{
		/// @todo !!!
	}

	if (!s_bombPlaceWaypointMapZones.IsEmpty ())
		for (HalfLifeEngine::SDK::Classes::Edict *bombPlace (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_bomb_target")); bombPlace->IsValid (); bombPlace = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (bombPlace, "info_bomb_target"))
		{
			/// @todo !!!
		}
}

YaPB::AreaMapZonesArray YaPB::s_bombPlaceAreaMapZones;
YaPB::AreaMapZonesArray YaPB::s_hostageAreaMapZones;
YaPB::AreaMapZonesArray YaPB::s_hostageRescueAreaMapZones;
YaPB::AreaMapZonesArray YaPB::s_VIPSafetyAreaMapZones;

void YaPB::BuildAreaMapZones (void)
{
	if (!g_server->IsMapHasTarget ())
		return;

	AreaMapZone_t newZone;

	for (HalfLifeEngine::SDK::Classes::Edict *bombPlace (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_bomb_target")); bombPlace->IsValid (); bombPlace = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (bombPlace, "func_bomb_target"))
	{
		newZone.edict                    = bombPlace;
		newZone.origin                   = bombPlace->GetCenter ();
		newZone.absoluteBoundingBox.mins = bombPlace->variables.absoluteBoundingBox.mins;
		newZone.absoluteBoundingBox.maxs = bombPlace->variables.absoluteBoundingBox.maxs;

		newZone.absoluteBoundingBox.mins.z -= 50.0f;
		newZone.absoluteBoundingBox.maxs.z += 50.0f;

		for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			if (Math::IsBoundingBoxesTouching (area->GetExtent (), newZone.absoluteBoundingBox.mins, newZone.absoluteBoundingBox.maxs))
				newZone.containingNavigationUnits += area;
		}

		if (!newZone.containingNavigationUnits.IsEmpty ())
		{
			s_bombPlaceAreaMapZones += newZone;

			newZone.containingNavigationUnits.RemoveAll ();
		}
	}

	for (HalfLifeEngine::SDK::Classes::Edict *bombPlace (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_bomb_target")); bombPlace->IsValid (); bombPlace = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (bombPlace, "info_bomb_target"))
	{
		newZone.edict        = bombPlace;
		newZone.origin       = bombPlace->variables.origin;
		newZone.absoluteBoundingBox.mins = newZone.origin;
		newZone.absoluteBoundingBox.maxs = newZone.origin;

		const float fakeSize (256.0f);

		newZone.absoluteBoundingBox.mins.x -= fakeSize;
		newZone.absoluteBoundingBox.mins.y -= fakeSize;
		newZone.absoluteBoundingBox.mins.z -= fakeSize;

		newZone.absoluteBoundingBox.mins.z -= 50.0f;

		newZone.absoluteBoundingBox.maxs.x += fakeSize;
		newZone.absoluteBoundingBox.maxs.y += fakeSize;
		newZone.absoluteBoundingBox.maxs.z += fakeSize;

		newZone.absoluteBoundingBox.maxs.z += 50.0f;

		for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			if (Math::IsBoundingBoxesTouching (area->GetExtent (), newZone.absoluteBoundingBox.mins, newZone.absoluteBoundingBox.maxs))
				newZone.containingNavigationUnits += area;
		}

		if (!newZone.containingNavigationUnits.IsEmpty ())
		{
			s_bombPlaceAreaMapZones += newZone;

			newZone.containingNavigationUnits.RemoveAll ();
		}
	}

	for (::HostageManager::HostagesArray::IndexType_t hostageIndex (0u); hostageIndex < g_server->GetHostageManager ().GetHostagesArray ().GetElementNumber (); ++hostageIndex)
	{
		const HalfLifeEngine::SDK::Classes::Edict *const hostage (g_server->GetHostageManager ().GetHostagesArray ()[hostageIndex]);

		newZone.edict        = hostage;
//		newZone.origin       = hostage->variables.origin;	//! RIGHT?!?!?!?!?!?!?!
//		newZone.absoluteBoundingBox.mins = newZone.origin;	//! RIGHT?!?!?!?!?!?!?!
//		newZone.absoluteBoundingBox.maxs = newZone.origin;	//! RIGHT?!?!?!?!?!?!?!
		newZone.origin       = hostage->GetHostageOrigin ();	//! RIGHT?!?!?!?!?!?!?!
		newZone.absoluteBoundingBox.mins = hostage->variables.absoluteBoundingBox.mins;	//! RIGHT?!?!?!?!?!?!?!
		newZone.absoluteBoundingBox.maxs = hostage->variables.absoluteBoundingBox.maxs;	//! RIGHT?!?!?!?!?!?!?!
/*
		const float fakeSize (256.0f);	//! RIGHT?!?!?!?!?!?!?!

		newZone.absoluteBoundingBox.mins.x -= fakeSize;	//! RIGHT?!?!?!?!?!?!?!
		newZone.absoluteBoundingBox.mins.y -= fakeSize;	//! RIGHT?!?!?!?!?!?!?!
		newZone.absoluteBoundingBox.mins.z -= fakeSize;	//! RIGHT?!?!?!?!?!?!?!
*/
		newZone.absoluteBoundingBox.mins.z -= 50.0f;
/*
		newZone.absoluteBoundingBox.maxs.x += fakeSize;	//! RIGHT?!?!?!?!?!?!?!
		newZone.absoluteBoundingBox.maxs.y += fakeSize;	//! RIGHT?!?!?!?!?!?!?!
		newZone.absoluteBoundingBox.maxs.z += fakeSize;	//! RIGHT?!?!?!?!?!?!?!
*/
		newZone.absoluteBoundingBox.maxs.z += 50.0f;

		for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			if (Math::IsBoundingBoxesTouching (area->GetExtent (), newZone.absoluteBoundingBox.mins, newZone.absoluteBoundingBox.maxs))
				newZone.containingNavigationUnits += area;
		}

		if (!newZone.containingNavigationUnits.IsEmpty ())
		{
			s_hostageAreaMapZones += newZone;

			newZone.containingNavigationUnits.RemoveAll ();
		}
	}

	for (HalfLifeEngine::SDK::Classes::Edict *hostageRescue (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_hostage_rescue")); hostageRescue->IsValid (); hostageRescue = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (hostageRescue, "func_hostage_rescue"))
	{
		newZone.edict        = hostageRescue;
		newZone.origin       = hostageRescue->GetCenter ();
		newZone.absoluteBoundingBox.mins = hostageRescue->variables.absoluteBoundingBox.mins;
		newZone.absoluteBoundingBox.maxs = hostageRescue->variables.absoluteBoundingBox.maxs;

		newZone.absoluteBoundingBox.mins.z -= 50.0f;
		newZone.absoluteBoundingBox.maxs.z += 50.0f;

		for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			if (Math::IsBoundingBoxesTouching (area->GetExtent (), newZone.absoluteBoundingBox.mins, newZone.absoluteBoundingBox.maxs))
				newZone.containingNavigationUnits += area;
		}

		if (!newZone.containingNavigationUnits.IsEmpty ())
		{
			s_hostageRescueAreaMapZones += newZone;

			newZone.containingNavigationUnits.RemoveAll ();
		}
	}

	for (HalfLifeEngine::SDK::Classes::Edict *hostageRescue (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_hostage_rescue")); hostageRescue->IsValid (); hostageRescue = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (hostageRescue, "info_hostage_rescue"))
	{
		newZone.edict        = hostageRescue;
		newZone.origin       = hostageRescue->variables.origin;
		newZone.absoluteBoundingBox.mins = newZone.origin;
		newZone.absoluteBoundingBox.maxs = newZone.origin;

		const float fakeSize (256.0f);

		newZone.absoluteBoundingBox.mins.x -= fakeSize;
		newZone.absoluteBoundingBox.mins.y -= fakeSize;
		newZone.absoluteBoundingBox.mins.z -= fakeSize;

		newZone.absoluteBoundingBox.mins.z -= 50.0f;

		newZone.absoluteBoundingBox.maxs.x += fakeSize;
		newZone.absoluteBoundingBox.maxs.y += fakeSize;
		newZone.absoluteBoundingBox.maxs.z += fakeSize;

		newZone.absoluteBoundingBox.maxs.z += 50.0f;

		for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			if (Math::IsBoundingBoxesTouching (area->GetExtent (), newZone.absoluteBoundingBox.mins, newZone.absoluteBoundingBox.maxs))
				newZone.containingNavigationUnits += area;
		}

		if (!newZone.containingNavigationUnits.IsEmpty ())
		{
			s_hostageRescueAreaMapZones += newZone;

			newZone.containingNavigationUnits.RemoveAll ();
		}
	}

	if (s_hostageRescueAreaMapZones.IsEmpty () && (g_server->GetMapType () & Server::MapType_Cs))
		for (HalfLifeEngine::SDK::Classes::Edict *hostageRescue (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_player_start")); hostageRescue->IsValid (); hostageRescue = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (hostageRescue, "info_player_start"))
		{
			newZone.edict        = hostageRescue;
			newZone.origin       = hostageRescue->variables.origin;
			newZone.absoluteBoundingBox.mins = newZone.origin;
			newZone.absoluteBoundingBox.maxs = newZone.origin;

			const float fakeSize (256.0f);

			newZone.absoluteBoundingBox.mins.x -= fakeSize;
			newZone.absoluteBoundingBox.mins.y -= fakeSize;
			newZone.absoluteBoundingBox.mins.z -= fakeSize;

			newZone.absoluteBoundingBox.mins.z -= 50.0f;

			newZone.absoluteBoundingBox.maxs.x += fakeSize;
			newZone.absoluteBoundingBox.maxs.y += fakeSize;
			newZone.absoluteBoundingBox.maxs.z += fakeSize;

			newZone.absoluteBoundingBox.maxs.z += 50.0f;

			for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
			{
				NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

				if (Math::IsBoundingBoxesTouching (area->GetExtent (), newZone.absoluteBoundingBox.mins, newZone.absoluteBoundingBox.maxs))
					newZone.containingNavigationUnits += area;
			}

			if (!newZone.containingNavigationUnits.IsEmpty ())
			{
				s_hostageRescueAreaMapZones += newZone;

				newZone.containingNavigationUnits.RemoveAll ();
			}
		}

	for (HalfLifeEngine::SDK::Classes::Edict *VIPSafetyZone (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_vip_safetyzone")); VIPSafetyZone->IsValid (); VIPSafetyZone = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (VIPSafetyZone, "func_vip_safetyzone"))
	{
		newZone.edict        = VIPSafetyZone;
		newZone.origin       = VIPSafetyZone->GetCenter ();
		newZone.absoluteBoundingBox.mins = VIPSafetyZone->variables.absoluteBoundingBox.mins;
		newZone.absoluteBoundingBox.maxs = VIPSafetyZone->variables.absoluteBoundingBox.maxs;

		newZone.absoluteBoundingBox.mins.z -= 50.0f;
		newZone.absoluteBoundingBox.maxs.z += 50.0f;

		for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
		{
			NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

			if (Math::IsBoundingBoxesTouching (area->GetExtent (), newZone.absoluteBoundingBox.mins, newZone.absoluteBoundingBox.maxs))
				newZone.containingNavigationUnits += area;
		}

		if (!newZone.containingNavigationUnits.IsEmpty ())
		{
			s_VIPSafetyAreaMapZones += newZone;

			newZone.containingNavigationUnits.RemoveAll ();
		}
	}

	#if defined _DEBUG
		AddLogEntry (true, LogLevel_Default, false, "YaPB::BuildAreaMapZones():\n\ts_bombPlaceAreaMapZones == %u,\n\ts_hostageAreaMapZones == %u,\n\ts_hostageRescueAreaMapZones == %u,\n\ts_VIPSafetyAreaMapZones == %u.", s_bombPlaceAreaMapZones.GetElementNumber (), s_hostageAreaMapZones.GetElementNumber (), s_hostageRescueAreaMapZones.GetElementNumber (), s_VIPSafetyAreaMapZones.GetElementNumber ());
	#endif	// if defined _DEBUG
}

const Math::Vector3D YaPB::GetRandomPositionInAreaZone (const AreaMapZone_t &zone)
{
	// Reliability check.
	InternalAssert (!zone.containingNavigationUnits.IsEmpty ());

	NavigationMesh::NavigationArea *const area (zone.containingNavigationUnits.GetRandomElement ());
/*
	if (zone.isInfoBombTargetZone)
		return area->GetClosestPointOnArea (zone.center);
*/
	Math::Vector3D closestPosition;
	Math::Extent2D extent;

	if (zone.absoluteBoundingBox.mins.x >= area->GetExtent ().mins.x)
		extent.mins.x = zone.absoluteBoundingBox.mins.x;
	else
		extent.mins.x = area->GetExtent ().mins.x;

	if (zone.absoluteBoundingBox.mins.y >= area->GetExtent ().mins.y)
		extent.mins.y = zone.absoluteBoundingBox.mins.y;
	else
		extent.mins.y = area->GetExtent ().mins.y;

	if (zone.absoluteBoundingBox.maxs.x <= area->GetExtent ().maxs.x)
		extent.maxs.x = zone.absoluteBoundingBox.maxs.x;
	else
		extent.maxs.x = area->GetExtent ().maxs.x;

	if (zone.absoluteBoundingBox.maxs.y <= area->GetExtent ().maxs.y)
		extent.maxs.y = zone.absoluteBoundingBox.maxs.y;
	else
		extent.maxs.y = area->GetExtent ().maxs.y;

	const float minimumAreaToRandomizePosition (MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::HumanWidth));

	if (extent.GetArea2D () > minimumAreaToRandomizePosition)
	{
		closestPosition.x = g_randomNumberGenerator.GetValueBetween (extent.mins.x, extent.maxs.x);
		closestPosition.y = g_randomNumberGenerator.GetValueBetween (extent.mins.y, extent.maxs.y);
	}
	else	// Just get center.
	{
		closestPosition.x = extent.GetMidX ();
		closestPosition.y = extent.GetMidY ();
	}

	closestPosition.z = area->GetZ (closestPosition);

	return closestPosition;
}

YaPB::YaPB (HalfLifeEngine::SDK::Classes::Edict *const bot, Profile *const profile) :
	FakeClient (bot),	// Set bot entity.

	m_frameIntervalTime (0.0f),
	m_lastThinkTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()),

	m_spawnTime (0.0f),
	m_fallDownTime (0.0f),

	m_nextCheckFollowMeRadioCommandTime (0.0f),

	m_joinServerTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()),

	m_boredom (g_randomNumberGenerator.GetValueBetween <unsigned char> (20u, Boredom_Maximum)),	//! @TODO: RANDOMIZE IT DEPENDING ON PROFILE!
	m_wantToLeaveCheckTime (0.0f),

	m_buysNumberInCurrentRound (0u),

	m_profile (profile),

	m_gameAction (GameAction_Idle),	// Not selecting team yet.

	m_maximumViewDistance (sm_maximumViewDistance),

	m_timeLogoSpray (0.0f),
	m_illuminationCheckTime (0.0f),

	m_buyingFinished (false),

	m_isLeader (false),

	m_firePause (0.0f),
	m_shootTime (0.0f),
	m_wantsToFire (false),

	m_agressionLevel (profile->baseAggressionLevel),
	m_fearLevel (profile->baseFearLevel),

	m_nextEmotionUpdate (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f),

	m_reloadState (ReloadState_None),
	m_reloadCheckTime (0.0f),

	m_zoomCheckTime (0.0f),
	m_shieldCheckTime (0.0f),
	m_grenadeCheckTime (0.0f),
	m_silencerCheckTime (0.0f),

	m_currentMapZone (HalfLifeEngine::SDK::Constants::MapZone_None),

	m_enemy (NULL),
	m_lastEnemy (NULL),

	m_button (NULL),

	m_avoidPlayersTime (0.0f)
{
	// This function does core operation of creating bot, it's called by YaPBManager::CreateBot(), when bot setup completed. (This is a YaPB class constructor)

	// Erase content of entity, because sometimes entities are reused
	bot->FreePrivateData ();	// free our predecessor's private data (null out the private data pointer)

	// create the player entity by calling MOD's player() function (from LINK_ENTITY_TO_CLASS() for player object)
//	HalfLifeEngine::Export::Entities::player (&bot->variables);	/// @remind DO WE NEED THIS?!?!?!?!??!?!?!? ClientPutInServer() also do this!!

	const unsigned char botIndex (static_cast <const unsigned char> (bot->GetIndex ()));	// link his entity to an useful pointer
	char *const infoKeyBuffer (bot->GetInfoKeyBuffer ());	// set all info buffer keys for this bot

	// set him some parameters in the info buffer
//	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "rate",                  "3500");	// (UNUSED?!?!?!?!?!?!?!??!?!?!?!?!?!??!)
//	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "cl_updaterate",         "30");		// (UNUSED?!?!?!?!?!?!?!??!?!?!?!?!?!??!)
//	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "cl_dlmax",              "128");		// (UNUSED?!?!?!?!?!?!?!??!?!?!?!?!?!??!)
//	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "cl_righthand"/*"lefthand"*/, profile->leftHand == true ? "1" : "0");	// Use left hand. (UNUSED?!?!?!?!?!?!?!??!?!?!?!?!?!??!)

	if (profile->leftHand == true)
		HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "cl_righthand"/*"lefthand"*/,          "1");	// Use left hand. (UNUSED?!?!?!?!?!?!?!??!?!?!?!?!?!??!)

	if (profile->autoWeponSwitch == false)
		HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "_cl_autowepswitch", "0");	// Autochange to a more powerful weapon, when buy, or pickup.

	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "tracker",               "0");
	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "friends",               "0");	// (UNUSED?!?!?!?!?!?!?!??!?!?!?!?!?!??!)
	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "dm",                    "0");	// Disable map information in the round beginning. (UNUSED?!?!?!?!?!?!?!??!?!?!?!?!?!??!)
	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "_ah",                   "0");	// Turn auto-help off.
	HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "_vgui_menus",           "0");	// Turn VGUI menus off.

	if (profile->ping == BotPingValue_BotTag)
		HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "*bot",              "1");	// Enable "BOT" tag in scoreboard instead of a ping.

	// Set Steam ID.... (For bot avatar in scoreboard :P)
	{
		GetEngineClient ().UNKNOWN7_connectionInformation.steamID = Immortal_BLGsteamID;
//		GetEngineClient ().UNKNOWN7_connectionInformation.steamID.InstancedSet (g_randomNumberGenerator.GetValueBetween (0u, SteamAccountIDMaximum), HalfLifeEngine::SDK::Classes::Steam::k_unSteamUserDesktopInstance, HalfLifeEngine::SDK::Constants::Steam::k_EUniversePublic, HalfLifeEngine::SDK::Constants::Steam::k_EAccountTypeIndividual);

		HalfLifeEngine::Globals::g_halfLifeEngine->SetClientKeyValue (botIndex, infoKeyBuffer, "*sid", FormatBuffer ("%I64u", GetEngineClient ().UNKNOWN7_connectionInformation.steamID.ConvertToUint64 ()));
	}

	// Initialize all the variables for this bot....

	{
		profile->isUsed = true;	// Mark this bot profile as used.
/*
		// TRUE when the bot needs to read pretraining file for megahal
		const bool bPreTrain (profile->PrepareHALBrainForPersonality ());	// check the bot HAL brain

		profile->LoadHALBrainForPersonality (bPreTrain);	// wake the bot's HAL brain up
*/	}

	bot->variables.frags = 0.0f;	// reset his frag count
	bot->variables.idealPitch = GetViewAngles ().pitch;
	bot->variables.idealYaw = GetViewAngles ().yaw;
	bot->variables.yawSpeed = g_randomNumberGenerator.GetValueBetween (s_skillTables[profile->skill / 20u].minTurnSpeed, s_skillTables[profile->skill / 20u].maxTurnSpeed);
	bot->variables.pitchSpeed = g_randomNumberGenerator.GetValueBetween (s_skillTables[profile->skill / 20u].minTurnSpeed, s_skillTables[profile->skill / 20u].maxTurnSpeed);
	bot->variables.lightLevel = 0u;
	bot->variables.colorMap = botIndex;

	// Initialize subsystems....
	m_aim = new Aim (this);
	m_navigator = new Navigator_NavigationMesh (this);

	// Reliability checks.
	if (m_aim == NULL || m_navigator == NULL)
		TerminateOnMalloc ();
/// @todo THIS SHOULD BE ASSINGED ONLY ONCE WHEN BOT CONNECTED TO SERVER AT FIRST TIME, BUT NOT PER CHANGELEVEL, OR RESTART, OR SOMETHING ELSE!!!!
	// Bump connection time to now so we don't resend a connection request.
	g_server->GetStatic ().clients[botIndex - 1u].netchan.connect_time = static_cast <const float> (HalfLifeEngine::Globals::g_halfLifeEngine->GetRealTime ());
//	(*reinterpret_cast <HalfLifeEngine::SDK::Structures::Client_t *> (reinterpret_cast <unsigned int> (g_server->GetStatic ().clients) + (botIndex - 1u) * 22552)).netchan.connect_time = static_cast <const float> (HalfLifeEngine::Globals::g_halfLifeEngine->GetRealTime ());	// NOSTEAM

//	m_aim->SetLookTask (new Aim::Task_Edict (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()));
}

// The format we sample voice in.
static const WAVEFORMATEX g_VoiceSampleFormat =
{
	WAVE_FORMAT_PCM,		// wFormatTag
	1u,						// nChannels
	8000u,					// nSamplesPerSec
	16000u,					// nAvgBytesPerSec
	2u,						// nBlockAlign
	16u,					// wBitsPerSample
	sizeof (WAVEFORMATEX)	// cbSize
};
const bool YaPB::ChatterManager::Voice_RecordStart
(
	const char *const pUncompressedFile,
	const char *const pDecompressedFile,
	const char *const pMicInputFile
)
{
	if (!IsCodecInitialized ())
	{
		// Reliability checks.
		InternalAssert (m_pMicInputFileData == NULL);
		InternalAssert (m_pUncompressedFileData == NULL);
		InternalAssert (m_pDecompressedFileData == NULL);
		InternalAssert (m_bVoiceRecording == false);

		return false;
	}

	Voice_RecordStop ();

	GetCodec ()->ResetState ();
//#if 0
	#if defined USE_SPEECH_ENGINES
	if (pMicInputFile == NULL)
	{
		static HalfLifeEngine::SDK::Classes::ConsoleVariable *sp_TTS (NULL);
		if (sp_TTS == NULL)
		{
			static HalfLifeEngine::SDK::Classes::ConsoleVariable s_TTS (PRODUCT_CONSOLE_TAG "_TTS", "Hello world!", HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server);

			HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_Register (s_TTS);
			sp_TTS = HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetPointer (s_TTS.name);
		}

		Timer getOutputTimer;
		Timer speakTimer;
		Timer totalTimer;
		const unsigned int startTimeTotal (totalTimer.GetMilliseconds ());
		unsigned int startTimeSpeak;
		unsigned int endTimeSpeak;
		unsigned int startTimeGetOutput;
		unsigned int endTimeGetOutput;

	    // Initialize COM library on the current thread and identify the concurrency model as
		// single-thread apartment (STA). Applications must initialize the COM library before they
		// can call COM library functions other than CoGetMalloc and memory allocation functions.
		// New application developers may choose to use CoInitializeEx rather than CoInitialize, allowing
		// them to set the concurrency model to apartment or multi-threaded.
		HRESULT hr (CoInitialize (NULL));
		if (FAILED (hr))
		{
			AddLogEntry (true, LogLevel_Error, true, "Failed to initialize COM. (hr=0x%X)", hr);
			return false;
		}

//		CComPtr <ISpAudioPlug> pVoice;
		ISpVoice *pVoice;
		// Creates SAPI instance from COM
		hr = CoCreateInstance (CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
		if(FAILED (hr))
		{
			CoUninitialize ();	// Uninitializes COM
			AddLogEntry (true, LogLevel_Error, true, "Failed to create SAPI voice instance. (hr=0x%X)", hr);
			return false;
		}
//		CComPtr <ISpAudioPlug> pSpStream;
		ISpAudioPlug *pSpStream;
		// Create basic SAPI stream object
		hr = CoCreateInstance (CLSID_SpAudioPlug, NULL, CLSCTX_ALL, IID_ISpAudioPlug, (void **)&pSpStream);
		if (FAILED (hr))
		{
			CoUninitialize ();	// Uninitializes COM
			AddLogEntry (true, LogLevel_Error, true, "Unable to create SAPI SpAudioPlug instance. (hr=0x%X)", hr);
			return false;
		}
		pSpStream->Init (VARIANT_TRUE, SAFT8kHz16BitMono/* Default HL engine voice format */);
		pVoice->SetOutput (pSpStream, FALSE);
		if (FAILED (hr))
		{
			CoUninitialize ();	// Uninitializes COM
			AddLogEntry (true, LogLevel_Error, true, "Unable to set SAPI voice output. (hr=0x%X)", hr);
			return false;
		}

		m_nMicInputFileBytes = 0u;
		m_pMicInputFileData = new char[MAX_WAVEFILEDATA_LEN];

		{
			struct TSynthesisRequest
			{
				DynamicString sPrompt;
				DynamicString sId;
			};
			TSynthesisRequest srRequest;
			const unsigned char iNumBytesPerSample (static_cast <unsigned char> (g_VoiceSampleFormat.wBitsPerSample / 8u));
			CRITICAL_SECTION csPlaybackQueue;
			InitializeCriticalSection (&csPlaybackQueue);

			srRequest.sPrompt = srRequest.sId = sp_TTS->string;

			// Convert the prompt to WCHAR* (that's what SAPI expects)
			size_t iLen = srRequest.sPrompt.GetElementNumber () + 1;
			wchar_t *wsPrompt = new wchar_t[iLen];
			mbstowcs (wsPrompt, srRequest.sPrompt.GetData (), iLen);

		startTimeSpeak = speakTimer.GetMilliseconds ();

			// Start the synthesis
			pVoice->Speak (wsPrompt, /*SPF_PURGEBEFORESPEAK | POSSIBLE???*/SPF_ASYNC, NULL);

		endTimeSpeak = speakTimer.GetMilliseconds ();

			delete [] wsPrompt;

		startTimeGetOutput = getOutputTimer.GetMilliseconds ();

			// Now loop until finished synthesizing
			SPVOICESTATUS svsStatus;
			VARIANT vData;
			short *pAudio;
			for (;;)
			{
				hr = pVoice->GetStatus (&svsStatus, NULL);
				if (FAILED (hr))
				{
					AddLogEntry (true, LogLevel_Error, true, "Failed to get status of ISpVoice! (hr=0x%X)", hr);
					break;
				}

				// check if there is some synthesized audio to send to the playback manager
				hr = pSpStream->GetData (&vData);
				if (hr == S_OK)	// NB: don't use SUCCEEDED here since GetData returns S_FALSE when no data is available, which is a success code
				{
					hr = SafeArrayAccessData (vData.parray, (void **)&pAudio);
					if (SUCCEEDED (hr))
					{
						int iBytes = vData.parray->rgsabound[0].cElements*vData.parray->cbElements;

						EnterCriticalSection (&csPlaybackQueue);
						{
//						AcquireSynthesizedAudio (sCurrentRequestId, pAudio, iBytes/iNumBytesPerSample, iNumBytesPerSample, iSampleRate, iNumChannels);

HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Synthesis [%s]: Appending %u bytes to sample...\n", srRequest.sId.GetData(), iBytes);
		MemoryCopy (pAudio, (short*)(m_pMicInputFileData + m_nMicInputFileBytes), iBytes/iNumBytesPerSample);
		m_nMicInputFileBytes += iBytes;
						}
						LeaveCriticalSection (&csPlaybackQueue);

						hr = SafeArrayUnaccessData (vData.parray);
					}
				}

				if (svsStatus.dwRunningState & SPRS_DONE)
					break;

				Sleep (10);
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Synthesis [%s] IS SLEEPING!!!\n", srRequest.sId.GetData());
			}

			// Process potential left-over chunk of data
			hr = pSpStream->GetData (&vData);
			if (hr == S_OK)
			{
				hr = SafeArrayAccessData (vData.parray, (void **)&pAudio);
				if (SUCCEEDED (hr))
				{
					int iBytes = vData.parray->rgsabound[0].cElements*vData.parray->cbElements;

					EnterCriticalSection (&csPlaybackQueue);
					{
//					AcquireSynthesizedAudio (sCurrentRequestId, pAudio, iBytes/iNumBytesPerSample, iNumBytesPerSample, iSampleRate, iNumChannels);

HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Synthesis [%s]: Appending %u bytes to sample... (POST)\n", srRequest.sId.GetData(), iBytes);
		MemoryCopy (pAudio, (short*)(m_pMicInputFileData + m_nMicInputFileBytes), iBytes/iNumBytesPerSample);
		m_nMicInputFileBytes += iBytes;
					}
					LeaveCriticalSection (&csPlaybackQueue);

					hr = SafeArrayUnaccessData (vData.parray);
				}
			}

			DeleteCriticalSection (&csPlaybackQueue);

			// Display the synthesis request that is currently being processed
			AddLogEntry (true, LogLevel_Default, false, "Synthesis finished for prompt [%s]", srRequest.sId.GetData());
		}

		endTimeGetOutput = getOutputTimer.GetMilliseconds ();

		// Releases SAPI voice
		pVoice->Release ();

		CoUninitialize ();	// Uninitializes COM

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Speak() time: %u ms\nGet output time: %u ms\nTotal time: %u ms\n", endTimeSpeak - startTimeSpeak, endTimeGetOutput - startTimeGetOutput, totalTimer.GetMilliseconds () - startTimeTotal);
	}
	#endif	// if defined USE_SPEECH_ENGINES
//#endif	// if 0
	if (pMicInputFile != NULL)
	{
		STDIOFile fp (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + pMicInputFile, "rb");

		if (!fp.IsValid ())
			return false;

		WaveSoundData wfp (fp);

		if (!wfp.IsValid ())
			return false;

		/// @todo 'm_pitch' VALUE IN THIS CLASS!!!!!!!!!!!!
		/// @todo INVERT PITCH VALUE!!!!!!!!!!!!
		const float pitch (100u/*/Console::Server::variableManager.GetVariable (Console::Server::VariableID_AllTalk)->GetValue <int> ()*/ * 0.01f);	// get playback pitch
		const unsigned short newSamplesPerSec (static_cast <unsigned short> (g_VoiceSampleFormat.nSamplesPerSec * pitch));

		// Reliability check.
		InternalAssert (newSamplesPerSec > 0u);

		// Check if we should convert input data....
		if ((wfp.GetHeader ().format.bitsPerSample != g_VoiceSampleFormat.wBitsPerSample || wfp.GetHeader ().format.sampleRate != newSamplesPerSec) && !wfp.ResampleInternal (newSamplesPerSec, static_cast <unsigned char> (g_VoiceSampleFormat.wBitsPerSample)))
			return false;

		/// @note TEST OF MERGING TWO SAMPLES:
		{
			const char *const pMicInputFile2 ("voice_input_my_org.wav");

			STDIOFile fp2 (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + pMicInputFile2, "rb");

			if (!fp2.IsValid ())
				return false;

			WaveSoundData wfp2 (fp2);

			if (!wfp2.IsValid ())
				return false;

			// Check if we should convert input data....
			if ((wfp2.GetHeader ().format.bitsPerSample != g_VoiceSampleFormat.wBitsPerSample || wfp2.GetHeader ().format.sampleRate != newSamplesPerSec) && !wfp2.ResampleInternal (newSamplesPerSec, static_cast <unsigned char> (g_VoiceSampleFormat.wBitsPerSample)))
				return false;

			wfp += wfp2;
		}

		// Apply volume to samples....
		/// @todo 'm_volume' VALUE IN THIS CLASS!!!!!!!!!!!!
		wfp.SetVolume (255u/*/Console::Server::variableManager.GetVariable (Console::Server::VariableID_AllTalk)->GetValue <unsigned short> ()*/ / 255.0f);

		m_nMicInputFileBytes = wfp.GetSamplesSize ();
		m_pMicInputFileData = new char[m_nMicInputFileBytes];
		MemoryCopy ((char*)wfp.GetSamples (), m_pMicInputFileData, m_nMicInputFileBytes);

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Voice_RecordStart(): File \"%s\", duration=%.2f.", pMicInputFile, wfp.GetDuration ());
#if 0
		unsigned short a, b;
		unsigned int c;

		if (!HalfLifeEngine::SDK::Voice::ReadWaveFile (pMicInputFile, m_pMicInputFileData, m_nMicInputFileBytes, a, b, c))
			return false;

		/// @todo 'm_pitch' VALUE IN THIS CLASS!!!!!!!!!!!!
		/// @todo INVERT PITCH VALUE!!!!!!!!!!!!
		const float pitch (/*100u/*/Console::Server::variableManager.GetVariable (Console::Server::VariableID_AllTalk)->GetValue <int> () * 0.01f);	// get playback pitch
		const unsigned short newSamplesPerSec (static_cast <unsigned short> (g_VoiceSampleFormat.nSamplesPerSec * pitch));

		// Reliability check.
		InternalAssert (newSamplesPerSec > 0u);

		// Check if we should convert input data....
		if (a != g_VoiceSampleFormat.wBitsPerSample || c != newSamplesPerSec)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Voice_RecordStart(): Input file is other than need (wBitsPerSample=%i, nSamplesPerSec=%i)", a, c);

			/// @todo SPEEDUP THIS SHIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			const DynamicString path (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + pMicInputFile);

			XASH::CONVERT_SOUND2 (path, (unsigned char *&)m_pMicInputFileData, m_nMicInputFileBytes, newSamplesPerSec, static_cast <unsigned char> (g_VoiceSampleFormat.wBitsPerSample));
		}
#if 0
		/// @note TEST OF MERGING TWO SAMPLES:
		{
			const char *const pMicInputFile2 ("voice_input_my_org.wav");

			char *m_pMicInputFileData2;
			unsigned int m_nMicInputFileBytes2;

			unsigned short a, b;
			unsigned int c;

			if (HalfLifeEngine::SDK::Voice::ReadWaveFile (pMicInputFile2, m_pMicInputFileData2, m_nMicInputFileBytes2, a, b, c))
			{
				if (a != g_VoiceSampleFormat.wBitsPerSample || c != newSamplesPerSec)
				{
					const DynamicString path (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + '/' + pMicInputFile2);

					XASH::CONVERT_SOUND2 (path, (unsigned char *&)m_pMicInputFileData2, m_nMicInputFileBytes2, newSamplesPerSec, static_cast <unsigned char> (g_VoiceSampleFormat.wBitsPerSample));
				}

				char *m_pMicInputFileDataMixed = new char[m_nMicInputFileBytes + m_nMicInputFileBytes2];
				MemoryCopy (m_pMicInputFileData, m_pMicInputFileDataMixed, m_nMicInputFileBytes);
				delete [] m_pMicInputFileData;
				MemoryCopy (m_pMicInputFileData2, m_pMicInputFileDataMixed + m_nMicInputFileBytes, m_nMicInputFileBytes2);
				delete [] m_pMicInputFileData2;
				m_pMicInputFileData = m_pMicInputFileDataMixed;
				m_nMicInputFileBytes += m_nMicInputFileBytes2;
			}
		}
#endif	// if 0
#endif	// if 0
	}

	// Reliability check.
	InternalAssert (m_pMicInputFileData != NULL);
#if 0
	// Apply volume to samples....
	{
		short *const tempShortData (reinterpret_cast <short *> (m_pMicInputFileData));
		/// @todo 'm_volume' VALUE IN THIS CLASS!!!!!!!!!!!!
		const unsigned short volume (static_cast <unsigned short> (255u/*/Console::Server::variableManager.GetVariable (Console::Server::VariableID_AllTalk)->GetValue <unsigned short> ()*/ / 255.0f * 256u));

		for (unsigned short index (0u); index < m_nMicInputFileBytes / BYTES_PER_SAMPLE; ++index)
			tempShortData[index] = static_cast <short> (Math::GetClampedValueInRange <int> ((tempShortData[index] * (volume/* + g_randomNumberGenerator.GetValueBetween <char> (-10, 10)*/)) >> 8u, -32768, 32767));
	}
#endif	// if 0
	if (pUncompressedFile != NULL)
	{
		m_pUncompressedFileData = new char[MAX_WAVEFILEDATA_LEN];
		m_nUncompressedDataBytes = 0;
		m_pUncompressedDataFilename = pUncompressedFile;
	}

	if (pDecompressedFile != NULL)
	{
		m_pDecompressedFileData = new char[MAX_WAVEFILEDATA_LEN];
		m_nDecompressedDataBytes = 0;
		m_pDecompressedDataFilename = pDecompressedFile;
	}

	m_CurMicInputFileByte = 0;
	m_MicStartTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetSystemTime ();
	m_bVoiceRecording = true;

	return true;
}
void YaPB::ChatterManager::Voice_RecordStop (void)
{
	// Write the files out for debugging.
	if (m_pMicInputFileData != NULL)
	{
		delete [] m_pMicInputFileData;
		m_pMicInputFileData = NULL;
	}

	if (m_pUncompressedFileData != NULL)
	{
		HalfLifeEngine::SDK::Voice::WriteWaveFile (m_pUncompressedDataFilename, m_pUncompressedFileData, m_nUncompressedDataBytes, g_VoiceSampleFormat.wBitsPerSample, g_VoiceSampleFormat.nChannels, g_VoiceSampleFormat.nSamplesPerSec);
		delete [] m_pUncompressedFileData;
		m_pUncompressedFileData = NULL;
	}

	if (m_pDecompressedFileData != NULL)
	{
		HalfLifeEngine::SDK::Voice::WriteWaveFile (m_pDecompressedDataFilename, m_pDecompressedFileData, m_nDecompressedDataBytes, g_VoiceSampleFormat.wBitsPerSample, g_VoiceSampleFormat.nChannels, g_VoiceSampleFormat.nSamplesPerSec);
		delete [] m_pDecompressedFileData;
		m_pDecompressedFileData = NULL;
	}

	m_bVoiceRecording = false;
}
const unsigned short YaPB::ChatterManager::Voice_GetCompressedData (char *const pchDest, const int nCount, const bool bFinal)
{
	if (m_pMicInputFileData == NULL)	/// @note ADDED BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return 0u;

	if (!IsCodecInitialized ())
		return 0u;

	short tempData[8192u];
	int gotten (0);
	int samplesNumber (0);

	// If they want to get the data from a file instead of the mic, use that.
	if (m_pMicInputFileData != NULL)
	{
		const double curtime (HalfLifeEngine::Globals::g_halfLifeEngine->GetSystemTime ());
		const int nShouldGet (static_cast <int> ((curtime - m_MicStartTime) * g_VoiceSampleFormat.nSamplesPerSec));

		gotten = Math::GetMinimumValueBetween <int> (sizeof (tempData) / BYTES_PER_SAMPLE, Math::GetMinimumValueBetween <int> (nShouldGet, (m_nMicInputFileBytes - m_CurMicInputFileByte) / BYTES_PER_SAMPLE));
		samplesNumber = gotten * BYTES_PER_SAMPLE;

		memcpy (tempData, m_pMicInputFileData + m_CurMicInputFileByte, samplesNumber);

		m_CurMicInputFileByte += samplesNumber;
		m_MicStartTime = curtime;
	}

	const unsigned short nCompressedBytes (static_cast <unsigned short> (GetCodec ()->Compress (reinterpret_cast <char *> (tempData), gotten, pchDest, nCount, bFinal)));

	// Write to our file buffers..
	if (m_pUncompressedFileData != NULL)
	{
		const int nToWrite (Math::GetMinimumValueBetween <int> (samplesNumber, MAX_WAVEFILEDATA_LEN - m_nUncompressedDataBytes));

		memcpy (m_pUncompressedFileData + m_nUncompressedDataBytes, tempData, nToWrite);

		m_nUncompressedDataBytes += nToWrite;
	}

	return nCompressedBytes;
}

void YaPB::StartChatter (const char *const chatterMessageSoundPath) const
{
	// This function sends voice message to all valid clients.

	const unsigned char botIndex (static_cast <const unsigned char> (m_edict->GetIndex ()));

	if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_AllTalk)->GetValue <bool> ())
	{
		if (IsAlive ())
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				if (!HalfLifeEngine::Globals::g_halfLifeEngine->GetClientVoiceListening (botIndex, static_cast <const unsigned char> (client->GetEdict ()->GetIndex ())))
					continue;

				// skip invalid players and skip self (i.e. this bot)
				if (client->IsFakeClient ())
					continue;

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_SendAudio), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteString (chatterMessageSoundPath);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (m_profile->voicePitch);
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
		else
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->IsAlive ())
					continue;

				if (!HalfLifeEngine::Globals::g_halfLifeEngine->GetClientVoiceListening (botIndex, static_cast <const unsigned char> (client->GetEdict ()->GetIndex ())))
					continue;

				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->IsFakeClient ())
					continue;
/*
				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->IsFakeClient () || client->IsAlive ())
					continue;
*/
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_SendAudio), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteString (chatterMessageSoundPath);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (m_profile->voicePitch);
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
	}
	else
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t realTeam (GetRealTeam ());

		if (IsAlive ())
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				// skip invalid players and skip self (i.e. this bot)
				if (client->GetRealTeam () != realTeam)
					continue;

				if (!HalfLifeEngine::Globals::g_halfLifeEngine->GetClientVoiceListening (botIndex, static_cast <const unsigned char> (client->GetEdict ()->GetIndex ())))
					continue;

				// skip invalid players and skip self (i.e. this bot)
				if (client->IsFakeClient ())
					continue;
/*
				// skip invalid players and skip self (i.e. this bot)
				if (client->IsFakeClient () || client->GetRealTeam () != realTeam)
					continue;
*/
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_SendAudio), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteString (chatterMessageSoundPath);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (m_profile->voicePitch);
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
		else
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->GetRealTeam () != realTeam || client->IsAlive ())
					continue;

				if (!HalfLifeEngine::Globals::g_halfLifeEngine->GetClientVoiceListening (botIndex, static_cast <const unsigned char> (client->GetEdict ()->GetIndex ())))
					continue;

				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->IsFakeClient ())
					continue;
/*
				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->IsFakeClient () || client->GetRealTeam () != realTeam || client->IsAlive ())
					continue;
*/
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_SendAudio), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteString (chatterMessageSoundPath);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (m_profile->voicePitch);
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
	}
}

void YaPB::DisplayChatterIcon (void) const
{
	// This function shows chatter icon, on the head of bot.

	const unsigned char botIndex (static_cast <const unsigned char> (m_edict->GetIndex ()));

	if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_AllTalk)->GetValue <bool> ())
	{
		if (IsAlive ())
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				// skip invalid players and skip self (i.e. this bot)
				if (client->IsFakeClient ())
					continue;

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotVoice), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (true);	// switch ON/OFF
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);	// byte (index of client/bot entity)
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
		else
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->IsFakeClient () || client->IsAlive ())
					continue;

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotVoice), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (true);	// switch ON/OFF
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);	// byte (index of client/bot entity)
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
	}
	else
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t realTeam (GetRealTeam ());

		if (IsAlive ())
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				// skip invalid players and skip self (i.e. this bot)
				if (client->IsFakeClient () || client->GetRealTeam () != realTeam)
					continue;

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotVoice), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (true);	// switch ON/OFF
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);	// byte (index of client/bot entity)
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
		else
		{
			// cycle through all client slots
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

				// skip invalid players, skip self (i.e. this bot) and skip alive players
				if (client->IsFakeClient () || client->IsAlive () || client->GetRealTeam () != realTeam)
					continue;

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotVoice), NULL, *client);	// begin message
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (true);	// switch ON/OFF
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);	// byte (index of client/bot entity)
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
			}
		}
	}
}

void YaPB::HideChatterIcon (void) const
{
	// This function removes chatter icon, on the head of bot.

	const unsigned char botIndex (static_cast <const unsigned char> (m_edict->GetIndex ()));

	// cycle through all client slots
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		// skip invalid players, skip self (i.e. this bot)
		if (client->IsFakeClient ())
			continue;

		// Hide voice icon....
		HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (HalfLifeEngine::Engine::GameMessageContainer::Type_BotVoice), NULL, *client);	// begin message
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (false);	// switch ON/OFF
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (botIndex);	// byte (index of client/bot entity)
		HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
	}
}

const Math::Vector3D &YaPB::GetEnemyAimPosition (void)
{
	// the purpose of this function, is to make bot aiming not so ideal. It's mutate 'm_enemy->origin' enemy vector returned from visibility check function.

	static float hardcodedZ;
	static Math::Vector3D targetOrigin;
	const Math::Vector3D velocityDifference (m_enemy->pointer->GetVelocity () - GetVelocity ());	// Compensate both the enemy's and the bot's own velocity

	// do not aim at head, at long distance (only if not using sniper weapon)
	if ((m_enemy->visibility & Visibility_Body) && !m_currentWeapon->IsSniperRifle () && !m_currentWeapon->IsPistol () && targetOrigin.GetDistanceSquared (GetOrigin ()) > (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> () ? MATH_GET_SQUARED (2400.0f) : MATH_GET_SQUARED (1800.0f)))
		m_enemy->visibility &= ~Visibility_Head;

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_TargetingMethod)->GetValue <Console::Bot::VariableValue_TargetingMethod_t> () == Console::Bot::VariableValue_TargetingMethod_HitBox)
	{
		const HalfLifeEngine::SDK::Structures::StudioHeader_t *const studioHeader (m_enemy->pointer->GetEdict ()->GetModelPointer ());
		const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const pbboxes (reinterpret_cast <const HalfLifeEngine::SDK::Structures::mstudiobbox_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->hitboxindex));
		const HalfLifeEngine::SDK::Structures::mstudiobbox_t *bbox (NULL);

		if (m_enemy->visibility & Visibility_Head)
		{
			for (unsigned char hitBoxIndex (0u); hitBoxIndex < studioHeader->numhitboxes; ++hitBoxIndex)
			{
				bbox = pbboxes + hitBoxIndex;

				if (bbox->group == HalfLifeEngine::SDK::Constants::PlayerBodyPart_Head)
					break;
			}
		}
		else if (m_enemy->visibility & Visibility_Body)
		{
			for (unsigned char hitBoxIndex (0u); hitBoxIndex < studioHeader->numhitboxes; ++hitBoxIndex)
			{
				bbox = pbboxes + hitBoxIndex;

				if (bbox->group == HalfLifeEngine::SDK::Constants::PlayerBodyPart_Chest || bbox->group == HalfLifeEngine::SDK::Constants::PlayerBodyPart_Stomach)
					break;
			}
		}
		else for (unsigned char hitBoxIndex (0u); hitBoxIndex < studioHeader->numhitboxes; ++hitBoxIndex)
		{
			bbox = pbboxes + hitBoxIndex;

			if (bbox->group != HalfLifeEngine::SDK::Constants::PlayerBodyPart_Head && bbox->group != HalfLifeEngine::SDK::Constants::PlayerBodyPart_Chest && bbox->group != HalfLifeEngine::SDK::Constants::PlayerBodyPart_Stomach)
				break;
		}

		// Reliability check.
		InternalAssert (bbox != NULL);

		const Math::Vector3D hitBoxCenter (m_enemy->pointer->GetHitBoxPosition (bbox));

		{
		const Host *const host (g_server->GetHumanManager ()->GetHostClient ());

		if (host != NULL)
			host->DrawCross3D (hitBoxCenter, 5.0f, Color <> (255u, 0u, 0u), 1u, 255u, 1u, false);
		}

		targetOrigin = hitBoxCenter;
	}
	else
	{
	// get enemy position initially
	targetOrigin = m_enemy->pointer->GetOrigin ();
/*
	// if we only suspect an enemy behind a wall take the worst skill
	if ((m_states & State_SuspectEnemy) && !(m_states & State_SeeingEnemy))
		targetOrigin += Math::Vector3D (g_randomNumberGenerator.GetValueBetween (m_enemy->pointer->GetEdict ()->variables.boundingBox.mins.x, m_enemy->pointer->GetEdict ()->variables.boundingBox.maxs.x), g_randomNumberGenerator.GetValueBetween (m_enemy->pointer->GetEdict ()->variables.boundingBox.mins.y, m_enemy->pointer->GetEdict ()->variables.boundingBox.maxs.y), g_randomNumberGenerator.GetValueBetween (m_enemy->pointer->GetEdict ()->variables.boundingBox.mins.z, m_enemy->pointer->GetEdict ()->variables.boundingBox.maxs.z));
	else*/
	{
		hardcodedZ = m_currentWeapon->IsSniperRifle () ? 15.0f : 17.0f;

		// this is cheating, stolen from official CSBot :P (if target player is a chicken reset Z axis)
		hardcodedZ = m_enemy->pointer->GetEdict ()->GetPlayerModel () == "chicken" ? 0.0f : Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> () ? (m_currentWeapon->IsSniperRifle () ? 2.32f : m_currentWeapon->IsPistol () ? 5.9f : m_enemy->pointer->GetOrigin ().GetDistanceSquared (GetOrigin ()) < MATH_GET_SQUARED (300.0f) ? 4.68f : 6.98f) + hardcodedZ : hardcodedZ - (0.6f * (BotSkillValue_Maximum - m_profile->skill)) + g_randomNumberGenerator.GetValueBetween (5.0f, 6.0f);

		// now take in account different parts of enemy body
		targetOrigin = (m_enemy->visibility & (Visibility_Head | Visibility_Body)) ?	// visible head & body?
				g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < s_skillTables[m_profile->skill / 20u].headshotFrequency || m_currentWeapon->IsPistol () ? targetOrigin + Math::Vector3D (0.0f, 0.0f, hardcodedZ) : targetOrigin :	// now check is our skill match to aim at head, else aim at enemy body
			(m_enemy->visibility & Visibility_Head) || (m_enemy->visibility & Visibility_Body) ?	// visible only head OR body?
				targetOrigin + Math::Vector3D (0.0f, 0.0f, hardcodedZ) :
//			(m_enemy->visibility & Visibility_Other) ?	// random part of body is visible?
				m_enemy->origin;/* :
			m_lastEnemyOrigin;	// something goes wrong, use last enemy origin

		m_lastEnemyOrigin = targetOrigin;*/
	}
	}

	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> ())
		m_enemy->origin = m_currentWeapon->IsSniperRifle () ? targetOrigin : targetOrigin + velocityDifference * GetFrameIntervalTime ()/* * 1.5f*/;	// if uses sniper do not predict enemy position
	else
	{
		const unsigned char skillTableIndex (m_profile->skill / 20u);
		const float divOffset (m_enemy->origin.GetDistance (GetOrigin ()) / (GetFieldOfView () < 40u ? 2000.0f : GetFieldOfView () < HalfLifeEngine::SDK::Constants::DefaultPlayerFieldOfView ? 1000.0f : 500.0f));

		targetOrigin.x += divOffset * g_randomNumberGenerator.GetValueBetween (-s_skillTables[skillTableIndex].aimOffset.x, s_skillTables[skillTableIndex].aimOffset.x);
		targetOrigin.y += divOffset * g_randomNumberGenerator.GetValueBetween (-s_skillTables[skillTableIndex].aimOffset.y, s_skillTables[skillTableIndex].aimOffset.y);
		targetOrigin.z += divOffset * g_randomNumberGenerator.GetValueBetween (-s_skillTables[skillTableIndex].aimOffset.z, s_skillTables[skillTableIndex].aimOffset.z);

		// randomize the target position
		m_enemy->origin = targetOrigin + velocityDifference * GetFrameIntervalTime () * 1.2f;
	}

	return m_enemy->origin;
}

void YaPB::AliveThink (void)
{/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <bool> ())
	{
//		SelectBestWeapon ();

		m_edict->variables.buttons = g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons;

		static float t (0.0f);
		if (t <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
		t = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;
		Label_Repeat:
		const HalfLifeEngine::SDK::Constants::WeaponSlotID_t slot ((HalfLifeEngine::SDK::Constants::WeaponSlotID_t)g_randomNumberGenerator.GetValueBetween <unsigned char> (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons, HalfLifeEngine::SDK::Constants::WeaponSlotID_C4));
		const OwningItemsManager::ItemsArray_t &items (m_owningItemsManager->GetItems (slot));
		if (!items.IsEmpty ())
			SelectWeapon (items.GetRandomElement ()->GetID ());
		else
			goto Label_Repeat;
		}
	}
*/
	// Call this function in base class....
	FakeClient::AliveThink ();

	/// @todo WE NEED RESTORE MAXIMUM DISTANCE ONLY ONCE AFTER FLASH DISAPPEARED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (m_lastScreenFadeInformation.duration <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		m_maximumViewDistance = sm_maximumViewDistance;	// Restore maximum view distance....

	if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Stop)->GetValue <bool> ())
	{
		UpdateApproachPoints ();
		CheckForObstacles ();	// identify any obstacle in front of us
		AvoidPlayers ();	// avoid players while we're at it

		AI ();

		m_scheduleManager.Think (/*this*/);
	}
/*
	if (m_edict->variables.groundEntity != NULL)
	{
		if (m_edict->variables.groundEntity->variables.flags & HalfLifeEngine::SDK::Constants::FL_CONVEYOR)
		{
			m_navigator->MoveForward ();
			m_navigator->m_destinationOrigin = m_edict->variables.groundEntity->GetOrigin ();
			m_navigator->Think ();
		}
	}

	const float inv (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <float> ());
	if (inv != 0.0f)
	{
	if (m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Vector)
		m_aim->SetLookTask (new Aim::Task_Vector (GetOrigin () - Math::Vector3D (0.0f, 0.0f, 8192.0f * inv)));
	else
		static_cast <Aim::Task_Vector *const> (m_aim->GetLookTask ())->SetVector (GetOrigin () - Math::Vector3D (0.0f, 0.0f, 8192.0f * inv));
	}
	else
	{
	if (m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Vector)
		m_aim->SetLookTask (new Aim::Task_Vector (g_server->GetHumanManager ()->GetHostClient ()->GetOrigin ()));
	else
		static_cast <Aim::Task_Vector *const> (m_aim->GetLookTask ())->SetVector (g_server->GetHumanManager ()->GetHostClient ()->GetOrigin ());
	}
	m_aim->Think ();
*/
	#if defined _DEBUG
/*		// draw position history:
		{
			HalfLifeEngine::SDK::Structures::ClientEntity_t* ent = HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetEntityByIndex (m_edict->GetIndex ());

			if(ent && ent->player && ent->curstate.solid)
			{
			// MAXIMUM ORIGIN DIFF IS ~30.0f
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\" origins DIFF=[%f, %f, %f], velocities DIFF=[%f, %f, %f]", GetName ().GetData (), Math::fabsf (GetOrigin ().x - ent->origin.x), Math::fabsf (GetOrigin ().y - ent->origin.y), Math::fabsf (GetOrigin ().z - ent->origin.z), Math::fabsf (GetVelocity ().x - ent->curstate.velocity.x), Math::fabsf (GetVelocity ().y - ent->curstate.velocity.y), Math::fabsf (GetVelocity ().z - ent->curstate.velocity.z));

			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (GetOrigin () - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), GetOrigin () + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight + 10.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (ent->origin - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), ent->origin + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight + 10.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u, false);
/*
			int historyIndex = ent->current_position;

			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (ent->ph[historyIndex].origin - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), ent->ph[historyIndex].origin + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight + 10.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);

				// step back
				if (--historyIndex < 0)
					historyIndex = HISTORY_MAX-1;

			for(int i=1;i<HISTORY_MAX;++i)
			{
//				g_server->GetHumanManager ()->GetHostClient ()->DrawLine(ent->ph[historyIndex].origin, ent->ph[historyIndex].origin, 0, 255, 0, 255, 1);
				g_server->GetHumanManager ()->GetHostClient ()->DrawLine (ent->ph[historyIndex].origin - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), ent->ph[historyIndex].origin + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight + 10.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

				// step back
				if (--historyIndex < 0)
					historyIndex = HISTORY_MAX-1;
			}
/*			for(int i=0;i<HISTORY_MAX;++i)
			{
//				g_server->GetHumanManager ()->GetHostClient ()->DrawLine(ent->ph[historyIndex].origin, ent->ph[historyIndex].origin, 0, 255, 0, 255, 1);
				g_server->GetHumanManager ()->GetHostClient ()->DrawLine (ent->ph[historyIndex].origin - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), ent->ph[historyIndex].origin + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight + 10.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

				// step back
				if (--historyIndex < 0)
					historyIndex = HISTORY_MAX-1;
			}*//*
			}
		}
*/
		if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <bool> ())
			return;

		const Host *const host (g_server->GetHumanManager ()->GetHostClient ());

		if (host == NULL)
			return;

		// if host edict observes for a bot, OR bot is debug bot, which has some actions...
		if (!IsDebuggingBot ())
			return;

//		s_debugLevel.body = 2u;

		// white = velocity
		host->DrawLine (GetOrigin (), GetOrigin () * GetVelocity (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 255u, 255u), 250u, 5u, 1u);

		// green = destination origin
		host->DrawLine (GetEyePosition (), m_navigator->m_destinationOrigin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

		// pink = ideal move angles
		host->DrawLine (GetOrigin (), GetOrigin () + m_navigator->m_moveAngles.BuildForwardVector () * 300.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 255u), 250u, 5u, 1u);
/*
		// blue = ideal view angles
		host->DrawLine (GetEyePosition (), GetEyePosition () + m_aim->m_idealAngles.BuildForwardVector () * 300.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 0u, 255u), 250u, 5u, 1u);
*/
		// red = current view angles
		host->DrawLine (GetEyePosition (), GetEyeForwardPositionWithPunchAngles (300.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u);

		m_navigator->DrawCurrentRoute ();

		DrawApproachPoints ();

		if (g_server->GetYaPBManager ()->GetDebugBotUpdateTime () > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			return;

		const HalfLifeEngine::SDK::Constants::TeamID_t team (GetRealTeam ());
		const ColorWithAlpha <> color (team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? ColorWithAlpha <> (255u, 70u, 0u) : ColorWithAlpha <> (0u, 70u, 255u));
		DynamicString currentArea, currentPlaceName;
		const char *const armorType[HalfLifeEngine::SDK::Constants::PlayerArmorType_Total] =
		{
			"no armor",
			"vest",
			"vest + helmet"
		};
		DynamicString itemsString;
		DynamicString lookTaskInformation;

		if (m_aim->GetLookTask ()->GetID () == Aim::Task::ID_None)
			lookTaskInformation = "NONE";
		else
		{
			lookTaskInformation = m_aim->GetLookTask ()->GetName ();

			const DynamicString lookTaskExtraInformation (m_aim->GetLookTask ()->GetInformationString (this).GetData ());

			if (!lookTaskExtraInformation.IsEmpty ())
			{
				lookTaskInformation += " (";
				lookTaskInformation += lookTaskExtraInformation;
				lookTaskInformation += ')';
			}
		}

		for (unsigned char itemSlotIndex (HalfLifeEngine::SDK::Constants::WeaponSlotID_PrimaryWeapons); itemSlotIndex < HalfLifeEngine::SDK::Constants::WeaponSlotID_Total; ++itemSlotIndex)
		{
			const OwningItemsManager::ItemsArray_t &items (m_owningItemsManager->GetItems (static_cast <HalfLifeEngine::SDK::Constants::WeaponSlotID_t> (itemSlotIndex)));

			if (items.IsEmpty ())
				continue;

			if (itemsString.IsEmpty ())
			{
				itemsString.AppendFormat ("\"%s\"(c=%i, a=%u)", items.GetFirstElement ()->GetEdict () == m_currentWeapon->GetEdict () ? DynamicString (HalfLifeEngine::SDK::Constants::weaponProperties[items.GetFirstElement ()->GetID ()].className.GetData () + GET_STATIC_STRING_LENGTH ("weapon_")).MakeUpper ().GetData () : HalfLifeEngine::SDK::Constants::weaponProperties[items.GetFirstElement ()->GetID ()].className.GetData () + GET_STATIC_STRING_LENGTH ("weapon_"), items.GetFirstElement ()->GetAmmoInClip (), items.GetFirstElement ()->GetAmmo ());

				for (OwningItemsManager::ItemsArray_t::IndexType_t itemIndex (1u); itemIndex < items.GetElementNumber (); ++itemIndex)
					itemsString.AppendFormat (", \"%s\"(c=%i, a=%u)", items[itemIndex]->GetEdict () == m_currentWeapon->GetEdict () ? DynamicString (HalfLifeEngine::SDK::Constants::weaponProperties[items[itemIndex]->GetID ()].className.GetData () + GET_STATIC_STRING_LENGTH ("weapon_")).MakeUpper ().GetData () : HalfLifeEngine::SDK::Constants::weaponProperties[items[itemIndex]->GetID ()].className.GetData () + GET_STATIC_STRING_LENGTH ("weapon_"), items[itemIndex]->GetAmmoInClip (), items[itemIndex]->GetAmmo ());
			}
			else for (OwningItemsManager::ItemsArray_t::IndexType_t itemIndex (0u); itemIndex < items.GetElementNumber (); ++itemIndex)
				itemsString.AppendFormat (", \"%s\"(c=%i, a=%u)", items[itemIndex]->GetEdict () == m_currentWeapon->GetEdict () ? DynamicString (HalfLifeEngine::SDK::Constants::weaponProperties[items[itemIndex]->GetID ()].className.GetData () + GET_STATIC_STRING_LENGTH ("weapon_")).MakeUpper ().GetData () : HalfLifeEngine::SDK::Constants::weaponProperties[items[itemIndex]->GetID ()].className.GetData () + GET_STATIC_STRING_LENGTH ("weapon_"), items[itemIndex]->GetAmmoInClip (), items[itemIndex]->GetAmmo ());
		}

		if (m_lastNavigationArea == NULL)
			currentArea = "NULL";
		else
		{
			DynamicString attributes (m_lastNavigationArea->PrintAttributes ());

			if (attributes.IsEmpty ())
			{
				if (m_lastNavigationArea->IsBlocked ())
				{
					attributes += "BLOCKED";

					if (m_lastNavigationArea->IsUnderwater ())
						attributes += " + UNDERWATER";
				}
				else if (m_lastNavigationArea->IsUnderwater ())
					attributes += "UNDERWATER";
			}
			else
			{
				if (m_lastNavigationArea->IsBlocked ())
					attributes += " + BLOCKED";

				if (m_lastNavigationArea->IsUnderwater ())
					attributes += " + UNDERWATER";
			}

			if (attributes.IsEmpty ())
				currentArea.AssignFormat ("%u", m_lastNavigationArea->GetID ());
			else
				currentArea.AssignFormat ("%u (%s)", m_lastNavigationArea->GetID (), attributes.GetData ());

			currentPlaceName = g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (m_lastNavigationArea->GetPlace ());
		}

		host->HudMessageFormat
		(
			Math::Vector2D (-1.0f, 0.14f),
			color,
			ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
			HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, g_server->GetYaPBManager ()->GetDebugBotUpdateDelay (), 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

			"AP=%i (%s),\n"
			"Current action: %s,\n"
			"Current look task: %s,\n"
			"Current area=%s, place: %s,\n"
			"Money=%u,\n"
			"Speed: [move=%.1f, strafe=%.1f],\n"
			"Stuck amount=%.2f,\n"
			"Agression=%.2f, fear=%.2f,\n"
			"Boredom=%u%%,\n"
			"Is leader: %s,\n"
			"Weapons: [%s]\n"
			"\n"
			"    [%s] [%s] [%s]\n"
			"[%s] [%s] [%s]\n"
			"[%s]  [%s]",

			m_edict->GetArmor (), armorType[GetArmorType ()],
			m_scheduleManager.IsEmpty () ? "NULL" : m_scheduleManager.GetCurrentAction ()->IsSchedule () ? FormatBuffer ("%s, sub action: %s", m_scheduleManager.GetCurrentAction ()->GetName (), m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->IsEmpty () ? "EMPTY" : m_scheduleManager.GetCurrentTask ()->GetName ()) : m_scheduleManager.GetCurrentTask ()->GetName (),
			lookTaskInformation.GetData (),
			currentArea.GetData (), currentPlaceName.IsEmpty () ? "NONE" : currentPlaceName.GetData (),
			GetMoney (),
			m_navigator->GetSpeed ().move, m_navigator->GetSpeed ().strafe,
			m_navigator->GetStuckAmount (),
			m_agressionLevel, m_fearLevel,
			static_cast <unsigned char> (GetBoredomIncreasingOffset () * 100.0f),
			m_isLeader ? "true" : "false",

			itemsString.IsEmpty () ? "NONE" : itemsString.GetData (),

			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_FORWARD)   ? "W"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)       ? "E"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_RELOAD)    ? "R"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVELEFT)  ? "A"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_BACK)      ? "S"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVERIGHT) ? "D"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_DUCK)      ? "DUCK" : "----",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_JUMP)      ? "JUMP" : "----"
		);

		g_server->GetYaPBManager ()->UpdateDebugBotUpdateTime ();
	#endif	// if defined _DEBUG
}

void YaPB::DeathThink (void)
{
	// Call this function in base class....
	FakeClient::DeathThink ();

	// If bot chooses team or model, run think to do it....
	if (m_scheduleManager.IsCurrentActions (Task::Bit_SelectTeam | Task::Bit_SelectModel))
		m_scheduleManager.Think (/*this*/);
}

void YaPB::Think (void)
{
	// This function gets called each frame and is the core of all bot AI. From here all other subroutines are called.
	// Without this function bots will do nothing! :)
/* FROM CSBot
	if (m_edict->variables.flags & HalfLifeEngine::SDK::Constants::FL_DORMANT)
		return;
*/
	g_server->GetYaPBManager ()->SetCurrentThinkingBot (this);

	// Not the best place, but....
	GetEngineClient ().latency = GetCalculatedPing () * 0.001f;

	// reset all buttons pressed...
	m_navigator->ResetMovement ();

	// Call this function in base class....
	FakeClient::Think ();

	#if defined _DEBUG
	if (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
	if (IsDebuggingBot ())
	{
		m_edict->variables.buttons = g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons;

		if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_FORWARD)
		{
			if (!(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_BACK))
				m_navigator->m_speed.move = 250.0f;
		}
		else if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_BACK)
			m_navigator->m_speed.move = -250.0f;

		if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVELEFT)
		{
			if (!(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVERIGHT))
				m_navigator->m_speed.strafe = 250.0f;
		}
		else if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVERIGHT)
			m_navigator->m_speed.strafe = -250.0f;

		m_navigator->m_moveAngles = (g_server->GetHumanManager ()->GetHostClient ()->GetOrigin () - GetOrigin ()).ToAngles3D ();

		m_navigator->m_moveAngles.pitch = -m_navigator->m_moveAngles.pitch;	// invert for engine

//		m_navigator->Think ();

		g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons &= ~HalfLifeEngine::SDK::Constants::IN_USE;
	}
	#endif	// if defined _DEBUG
/*
	{
		m_edict->variables.buttons = HalfLifeEngine::SDK::Constants::ButtonType_None;
		if (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK)
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK;
		if (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK2)
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;
//		m_edict->variables.buttons = g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.buttons;
	}
*/
	RunMove ();

	g_server->GetYaPBManager ()->SetCurrentThinkingBot (NULL);
}

void YaPB::AI (void)
{
	// This function gets called each frame and is the core of all bot AI. From here all other subroutines are called.

	UpdateFallDownTime ();

	// do all sensing, calculate/filter all actions here
	SetConditions ();
/*
//	if (m_checkWeaponSwitch)
	{
		// check if we already switched weapon mode
		if (m_buyingFinished && m_spawnTime + g_randomNumberGenerator.GetValueBetween (0.3f, 1.5f) <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
			if (IsShieldDrawn ())
				m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;
//			else
//				m_silencerCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}

		// switch to knife if time to do this
		if (m_buyingFinished && m_currentWeapon->GetReloadEndTime () + g_randomNumberGenerator.GetValueBetween (1.0f, 2.5f) <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && m_profile->skill >= 75u && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < (m_profile->personality == Personality_Rusher ? 99u : 80u) && (g_server->GetMapType () & (Server::MapType_Cs | Server::MapType_De | Server::MapType_Es | Server::MapType_As)))
			SelectWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Knife);
/*
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Spray)->GetValue <bool> () && m_spawnTime + g_randomNumberGenerator.GetValueBetween (0.3f, 5.5f) <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 85u)
			StartTask (Task_SprayLogo, TaskPriority_SprayLogo, -1, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.0f, false);

		if (m_buyingFinished && g_server->GetRoundInformation ().freezeEndTime + g_randomNumberGenerator.GetValueBetween (0.3f, 2.5f) <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AutofollowPercent)->GetValue <unsigned char> () && !m_targetEntity->IsValid () && !m_isLeader && !HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4) && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DebugGoal)->GetValue <short> () == -1)
			AttachToUser ();

		m_checkWeaponSwitch = false;*/
//	}

	if (!m_radioCommandsManager.GetRadioCommands ().IsEmpty ())
	{
		const RadioCommandsManager::RadioCommand *const currentRadioMessage (m_radioCommandsManager.GetRadioCommands ().GetFirstElement ());

		if (currentRadioMessage->GetReactTime () <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
			ReactOnRadio (currentRadioMessage);

			// radio command has been handled, delete
			m_radioCommandsManager.RemoveTop ();
		}
	}

	// high skill bots don't needed this, cause they are fathers :{P
	if (m_profile->skill < BotSkillLevel_Godlike && m_illuminationCheckTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		CheckIllumination ();

		// update check time
		m_illuminationCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (0.5f, 2.5f);
	}
}

void YaPB::UpdateEmotions (void)
{
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> ())
	{
		m_agressionLevel *= 2.0f;

		if (m_agressionLevel > 1.0f)
			m_agressionLevel = 1.0f;

		m_fearLevel *= 0.5f;
	}

	if (m_agressionLevel > m_profile->baseAggressionLevel)
	{
		m_agressionLevel -= 0.05f;

		if (m_agressionLevel < 0.0f)
			m_agressionLevel = 0.0f;
	}
	else
	{
		m_agressionLevel += 0.05f;

		if (m_agressionLevel > 1.0f)
			m_agressionLevel = 1.0f;
	}

	if (m_fearLevel > m_profile->baseFearLevel)
	{
		m_fearLevel -= 0.05f;

		if (m_fearLevel < 0.0f)
			m_fearLevel = 0.0f;
	}
	else
	{
		m_fearLevel += 0.05f;

		if (m_fearLevel > 1.0f)
			m_fearLevel = 1.0f;
	}
}

void YaPB::SetConditions (void)
{
	// this function carried out each frame. Does all of the sensing, calculates emotions and finally sets the desired action after applying all of the Filters.

	CheckUpdateEmotions ();
}

void YaPB::CheckIllumination (void)
{
	if (IsHasNightVisionGoggles ())
	{
		// is flashlight on?
		if (IsUsesFlashlight ())
			FlashlightTurnOff ();	// switch off the flashlight
		else
		{
			const unsigned char lightLevel (GetIllumination ());

			if (!IsUsingNightVisionGoggles ())
			{
				if ((g_skyLightLevel > 50u && lightLevel < 15u) || (g_skyLightLevel <= 50u && lightLevel < 40u))
					ExecuteCommand ("nightvision");	// switch on the nightvision
			}
			else if ((lightLevel > 20u && g_skyLightLevel > 50u) || (lightLevel > 60u && g_skyLightLevel <= 50u))
				ExecuteCommand ("nightvision");	// switch off the nightvision
		}
	}
	else
	{
		const unsigned char lightLevel (GetIllumination ());

		if (!IsUsesFlashlight ())
		{
			if (((g_skyLightLevel > 50u && lightLevel <= 10u) || (g_skyLightLevel <= 50u && lightLevel < 20u))/* && GetCurrentTask ()->taskID != Task_Camp && m_heardSoundTime + 3.0f <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()*/ && GetFlashlightBattery () >= 30u)
				FlashlightTurnOn ();	// switch on the flashlight
		}
		else if (((lightLevel > 15u && g_skyLightLevel > 50u) || (lightLevel > 25u && g_skyLightLevel <= 50u))/* || GetCurrentTask ()->taskID == Task::ID_Camp || GetCurrentTask ()->taskID == Task::ID_AttackEnemy || m_heardSoundTime + 3.0f > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()*/)
			FlashlightTurnOff ();	// switch off the flashlight
	}
}

void YaPB::ReactOnRadio (const RadioCommandsManager::RadioCommand *const radioMessage)
{
	// This function handling radio and reactings to it.

	if (!g_server->IsTeamplay ())
		return;

	m_scheduleManager.OnReactOnRadio (radioMessage);

	const Task *const currentTask (m_scheduleManager.IsEmpty () ? NULL : m_scheduleManager.GetCurrentTask ());

	// don't allow bot listen you if bot is busy
	if (currentTask != NULL && (currentTask->GetID () == Task::ID_DefuseBomb || currentTask->GetID () == Task::ID_PlantBomb || HasFollowingHostage () || g_server->GetRoundInformation ().isEnded || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DebugGoal)->GetValue <int> () > -1))
		return;

	switch (radioMessage->GetCommand ())
	{
		case HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup:
			if (m_scheduleManager.HasAction (Task::ID_PerformDoubleJump) || m_scheduleManager.HasAction (Schedule::ID_PerformDoubleJump))
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

			m_scheduleManager.AddFront (new Schedule_PerformDoubleJump (this, radioMessage->GetCaller (), radioMessage->GetCaller ()->GetOrigin ()));

			RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Affirmative);

			break;

		case HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe:
		case HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe:
		case HalfLifeEngine::SDK::Constants::RadioCommand_StickTogether:
//		case ::ChatterManager::ChatterMessage_GoingToPlantBomb:
//		case ::ChatterManager::ChatterMessage_DefusingC4:
			if (m_nextCheckFollowMeRadioCommandTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

//			m_nextCheckFollowMeRadioCommandTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 5.0f;

			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) > (m_profile->personality == Personality_Careful ? 80u : 50u))
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

			// check if line of sight to object is not blocked (i.e. visible)
			if (!IsPositionVisible (radioMessage->GetCaller ()->GetOrigin ()))
				break;

			// If Bot isn't already 'used' then follow him about half of the time
			if (m_scheduleManager.HasAction (Task::ID_FollowUser))
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

			unsigned char numFollowersBots (0u);
			const unsigned char allowedFollowers (/*radioMessage->GetCaller ()->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_BombC4) ? 1u : */Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_FollowUser)->GetValue <unsigned char> ());

			// check if no more followers are allowed
			for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
			{
				const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

				if (!bot->IsAlive ())
					continue;

				const Task_FollowUser *const followUserTask (static_cast <const Task_FollowUser *const> (bot->m_scheduleManager.GetAction (Task::ID_FollowUser)));

				if (followUserTask != NULL && followUserTask->GetClient () == radioMessage->GetCaller ())
					++numFollowersBots;
			}

			if (numFollowersBots >= allowedFollowers)
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

//			if (currentTask != NULL && (currentTask->GetID () == Task::ID_Idle || currentTask->GetID () == Task::ID_Camp)
//				m_tasks->time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();	// don't pause/camp/follow anymore

			m_scheduleManager.AddFront (new Task_FollowUser (this, radioMessage->GetCaller (), g_randomNumberGenerator.GetValueBetween (5.0f, 15.0f)));

			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 85u)
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Affirmative);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						switch (radioMessage->GetCommand ())
						{
							case HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe:
//							case ::ChatterManager::ChatterMessage_DefusingC4:
								ChatterMessage (::ChatterManager::ChatterMessage_CoveringFriend);

								break;

							case HalfLifeEngine::SDK::Constants::RadioCommand_FollowMe:
							case HalfLifeEngine::SDK::Constants::RadioCommand_StickTogether:
//							case ::ChatterManager::ChatterMessage_GoingToPlantBomb:
								ChatterMessage (::ChatterManager::ChatterMessage_AgreeWithPlan);

								break;
						}

						break;
				}

			break;
	}
}
#if 0
void YaPB::ReactOnChatter (const ChatterCommandsManager::ChatterCommand *const chatterMessage)
{
	// This function handling radio and reactings to it.

	if (!g_server->IsTeamplay ())
		return;

	const Task *const currentTask (m_scheduleManager.IsEmpty () ? NULL : m_scheduleManager.GetCurrentTask ());

	// don't allow bot listen you if bot is busy
	if (currentTask != NULL && (currentTask->GetID () == Task::ID_DefuseBomb || currentTask->GetID () == Task::ID_PlantBomb || HasFollowingHostage () || g_server->GetRoundInformation ().isEnded || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DebugGoal)->GetValue <int> () > -1))
		return;

	switch (chatterMessage->GetCommand ())
	{
		case ::ChatterManager::ChatterMessage_GoingToPlantBomb:
		case ::ChatterManager::ChatterMessage_DefusingC4:
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) > (m_profile->personality == Personality_Careful ? 80u : 50u))
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

			// check if line of sight to object is not blocked (i.e. visible)
			if (!IsPositionVisible (chatterMessage->GetCaller ()->GetOrigin ()))
				break;

			// If Bot isn't already 'used' then follow him about half of the time
			if (m_scheduleManager.HasAction (Task::ID_FollowUser))
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

			unsigned char numFollowersBots (0u);
			const unsigned char allowedFollowers (/*chatterMessage->GetCaller ()->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_BombC4) ? 1u : */Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_FollowUser)->GetValue <unsigned char> ());

			// check if no more followers are allowed
			for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
			{
				const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

				if (!bot->IsAlive ())
					continue;

				const Task_FollowUser *const followUserTask (static_cast <const Task_FollowUser *const> (bot->m_scheduleManager.GetAction (Task::ID_FollowUser)));

				if (followUserTask != NULL && followUserTask->GetClient () == chatterMessage->GetCaller ())
					++numFollowersBots;
			}

			if (numFollowersBots >= allowedFollowers)
			{
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Negative);

				break;
			}

//			if (currentTask != NULL && (currentTask->GetID () == Task::ID_Idle || currentTask->GetID () == Task::ID_Camp)
//				m_tasks->time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();	// don't pause/camp/follow anymore

			m_scheduleManager.AddFront (new Task_FollowUser (this, chatterMessage->GetCaller (), g_randomNumberGenerator.GetValueBetween (5.0f, 15.0f)));

			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 85u)
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_Affirmative);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						switch (chatterMessage->GetCommand ())
						{
							case ::ChatterManager::ChatterMessage_DefusingC4:
								ChatterMessage (::ChatterManager::ChatterMessage_CoveringFriend);

								break;

							case ::ChatterManager::ChatterMessage_GoingToPlantBomb:
								ChatterMessage (::ChatterManager::ChatterMessage_AgreeWithPlan);

								break;
						}

						break;
				}

			break;
	}
}
#endif	// if 0
void YaPB::RoundStarted (void)
{
	// This function called after bot creation & at the start of each round.

	// Call original function in base class....
	Client::RoundStarted ();

	// Reset some bot variables....

	m_navigator->ClearRoute ();
	m_navigator->m_destinationOrigin = Math::Vector3D::ZeroValue;

	m_chatterManager.ClearChatterMessagesPriorityQueue ();

	m_buysNumberInCurrentRound = 0u;
	m_buyingFinished = m_isLeader = false;

	m_nextCheckFollowMeRadioCommandTime = 0.0f;

	m_areaGameState.Reset ();

	m_scheduleManager.OnRoundStarted ();
}

void YaPB::TakeDamage (const HalfLifeEngine::SDK::Classes::Edict *const inflictor, const unsigned short damage, const unsigned char armor, const HalfLifeEngine::SDK::Constants::DamageType_t bits)
{
	// This function gets called from the network message handler, when bot's gets hurt from any other player.

	// Reliability check.
	InternalAssert (inflictor != NULL);

	// Remember current attacker....
	if (inflictor->IsPlayer ())
		m_lastAttackerInformation.pointer = g_server->GetClientManager ()->GetClient (inflictor);
	else if (inflictor->IsGrenade ())
	{
		const Server::ThrownGrenadesManager::ThrownGrenade_t *const explosiveGrenade (g_server->GetThrownGrenadesManager ().FindThrownExplosiveGrenade (inflictor));

		// Reliability check.
		InternalAssert (explosiveGrenade != NULL);

		m_lastAttackerInformation.pointer = explosiveGrenade->owner;
	}
	else
		m_lastAttackerInformation.pointer = NULL;

	m_lastAttackerInformation.damage.time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	m_lastAttackerInformation.damage.amount = damage;
	m_lastAttackerInformation.damage.origin = inflictor->GetOrigin ();
	m_lastAttackerInformation.damage.flags = bits;	// remember damage type

	m_scheduleManager.OnTakeDamage (inflictor, damage, armor, bits);

	// remember bot team
	const unsigned char team (GetTeam ());

//	CollectGoalExperience (damage, team);

	if (m_lastAttackerInformation.pointer != NULL)
	{
		if (m_lastAttackerInformation.pointer->GetTeam () == team)
		{
			// attacked by a teammate
			if (m_edict->GetHealth () < (m_profile->personality == Personality_Rusher ? 98 : 90) && !m_enemy->IsValid ())
			{
				// alright, die you teamkiller!!! ]:->
				m_enemy = new Enemy_t (/*m_lastEnemy = */m_lastAttackerInformation.pointer, /*m_lastEnemyOrigin = */inflictor->GetEyePosition (), Visibility_Head, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.7f);

				#if defined _DEBUG
					// Reliability check.
					if (m_enemy == NULL)
					{
						AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new bot \"%s\" enemy \"%s\"!", sizeof (*m_enemy), GetName ().GetData (), inflictor->GetNetName ().GetData ());

						return;
					}

//					AddLogEntry (true, LogLevel_Default, false, "YaPB \"%s\" found new enemy(teamkiller): \"%s\".", GetName ().GetData (), inflictor->GetNetName ().GetData ());
				#endif	// if defined _DEBUG
/*
				m_states |= State_SeeingEnemy;

				m_actualReactionTime = 0.0f;

				ChatMessage (ChatType_TeamAttack);*/
			}

			ChatterMessage (::ChatterManager::ChatterMessage_FriendlyFire, 0.3f);
		}
		else
		{
			// attacked by an enemy
			if (m_edict->GetHealth () >= 70)
			{
				m_agressionLevel += 0.1f;

				if (m_agressionLevel > 1.0f)
					m_agressionLevel = 1.0f;
			}
			else
			{
				m_fearLevel += 0.05f;

				if (m_fearLevel > 1.0f)
					m_fearLevel = 1.0f;
			}
/*
			// stop bot from hiding
			RemoveCertainTask (Task_Hide);

			if (!m_enemy->IsValid ())
			{
				m_lastEnemy = m_lastAttackerInformation.pointer;
				m_lastEnemyOrigin = inflictor->variables.origin;

				/// @FIXME - Bot doesn't necessary sees this enemy
				m_enemy->seeTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
			}

			CollectExperienceData (inflictor, armor + damage);*/
		}
	}
/*	else if (!g_waypoint->Reachable (this, g_waypoint->FindNearest (m_navigator->m_destinationOrigin)))	// hurt by unusual damage like drowning or gas, leave the camping/hiding position
	{
		// delete all path finding nodes
		DeleteSearchNodes ();

		FindWaypoint ();
	}*/
/*
	#if defined _DEBUG
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB::TakeDamage() called for bot: \"%s\", current health == %i, inflictor: \"%s\", damage == %i, armor == %i, bits == %i.\n", GetName ().GetData (), m_edict->GetHealth (), inflictor->IsPlayer () ? inflictor->GetNetName ().GetData () : inflictor->GetClassName ().GetData (), damage, armor, bits);
	#endif	// if defined _DEBUG*/
}

void YaPB::TakeBlinded (const float duration, const Client *const flasher)
{
	// This function gets called by network message handler, when screenfade message get's send it's used to make bot blind froumd the grenade.

	{
		const float newMaximumViewDistance (g_randomNumberGenerator.GetValueBetween (10.0f, 20.0f));

		if (newMaximumViewDistance < m_maximumViewDistance)
			m_maximumViewDistance = newMaximumViewDistance;
	}

	// Initialize screen fade information:
	{
		m_lastScreenFadeInformation.flasher = flasher;
		m_lastScreenFadeInformation.duration = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + duration + m_profile->baseReactTime * m_profile->skill.GetReducingOffset ();
	}
/*
	#if defined _DEBUG
		AddLogEntry (true, LogLevel_Default, false, "YaPB::TakeBlinded(): Bot \"%s\" is blinded. Flasher: \"%s\", duration=%.2f.", GetName ().GetData (), m_lastScreenFadeInformation.flasher == NULL ? "NULL" : m_lastScreenFadeInformation.flasher->GetName ().GetData (), m_lastScreenFadeInformation.duration - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
	#endif	// if defined _DEBUG
*/
	m_scheduleManager.OnTakeBlinded (duration, flasher);
/*
	if (m_profile->skill <= BotSkillLevel_Advanced)
	{
		m_blindMoveSpeed = m_blindSidemoveSpeed = 0.0f;
		m_blindButton = HalfLifeEngine::SDK::Constants::IN_DUCK;

		return;
	}

	const float walkSpeed (GetWalkSpeed ());

	m_blindSidemoveSpeed = g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 50u ? walkSpeed : -walkSpeed;

	if (m_edict->GetHealth () < 85)
		m_blindMoveSpeed = -walkSpeed;
	else if (m_profile->personality == Personality_Careful)
	{
		m_blindMoveSpeed = 0.0f;
		m_blindButton = HalfLifeEngine::SDK::Constants::IN_DUCK;
	}
	else
		m_blindMoveSpeed = walkSpeed;*/
}

void YaPB::TakeShakes (const float amplitude, const float duration, const float frequency)
{
	m_maximumViewDistance *= 0.6f;	// reduce maximum view distance

	// Initialize screen shake information:
	{
		m_lastScreenShakeInformation.amplitude = amplitude;
		m_lastScreenShakeInformation.duration = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + duration + m_profile->baseReactTime * m_profile->skill.GetReducingOffset ();
		m_lastScreenShakeInformation.frequency = frequency;
	}
/*
	#if defined _DEBUG
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB \"%s\" take shakes, amplitude == %.2f, duration == %.2f, frequency == %.2f.\n", GetName ().GetData (), amplitude, duration, frequency);
	#endif	// if defined _DEBUG
*/
//	m_scheduleManager.OnTakeShakes (amplitude, duration, frequency);
}

const bool YaPB::IsBombAudible (void) const
{
	// this function checks if bomb is can be heard by the bot, calculations done by manual testing.

	// Reliability check.
	if (!g_server->GetC4Manager ().IsÑ4Planted ())
		return false;

	// we hear bomb if length greater than desired radius (start the manual calculations)
	return GetOrigin ().GetDistanceSquared2D (g_server->GetC4Manager ().GetC4 ()->GetOrigin ()) > MATH_GET_SQUARED (g_server->GetC4Manager ().GetC4 ()->GetAudibleRadius ());
}

void YaPB::Killed (const HalfLifeEngine::SDK::Classes::Edict *const killer)
{
	// Is called when the bot is killed by 'killer'.

//	const HalfLifeEngine::SDK::Constants::PlayerBodyPart_t lastHitGroup (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::PlayerBodyPart_t> (HalfLifeEngine::SDK::Constants::Offset_BaseMonster_LastHitGroup));
	const bool isSuicide (m_edict == killer);

	if (!isSuicide)
		g_server->GetNavigationMeshManager ().IncreaseDangerNearby (static_cast <TeamArrayID_t> (GetRealTeam () - 1u), 1.0f, m_lastNavigationArea, GetOrigin (), 500.0f);

	// Call original function in base class....
	FakeClient::Killed (killer);

	m_scheduleManager.OnKilled (killer);

	GettingBored ();

	// If this is not fall from some height or something else....
	if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 20u)
		ChatterMessage (isSuicide ? ::ChatterManager::ChatterMessage_Suicide : ::ChatterManager::ChatterMessage_DiePain);

	if (m_chatterManager.IsChatterIconDisplayed ())
		HideChatterIcon ();	// Hide icon and stop voice communication.

	m_isLeader = false;	// Dead bot is not a leader any more!
}

void YaPB::Kill (const Client *const victim)
{
	// Is called when a bot has a kill somebody.

	// Reliability check.
	InternalAssert (victim != NULL);

	m_scheduleManager.OnKill (victim);

	const HalfLifeEngine::SDK::Constants::TeamID_t realTeam (GetRealTeam ());
	const HalfLifeEngine::SDK::Constants::TeamID_t victimRealTeam (victim->GetRealTeam ());
	const HalfLifeEngine::SDK::Constants::PlayerBodyPart_t victimLastHitGroup (victim->GetEdict ()->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::PlayerBodyPart_t> (HalfLifeEngine::SDK::Constants::Offset_BaseMonster_LastHitGroup));
	const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID (m_currentWeapon->GetID ());
	const bool isHeadshot (victimLastHitGroup == HalfLifeEngine::SDK::Constants::PlayerBodyPart_Head);	// Was headshot or not?

	HavingFun ();

	{
		{
			// add some aggression because we just killed somebody MUWHAHA !!
			m_agressionLevel += 0.1f;

			if (m_agressionLevel > 1.0f)
				m_agressionLevel = 1.0f;
		}

		{
			m_fearLevel -= 0.1f;

			if (m_fearLevel < 0.0f)
				m_fearLevel = 0.0f;
		}
	}

	// Did bot just kill an enemy?
	if (victimRealTeam == realTeam)
	{
//		ChatMessage (ChatType_TeamKill, true);
		ChatterMessage (::ChatterManager::ChatterMessage_TeamKill, 0.6f);

		return;
	}

	if (weaponID == HalfLifeEngine::SDK::Constants::WeaponID_Knife && isHeadshot)
		ExecuteCommandFormat ("say HAHAHAH!!! %s DIES LIKE FUCKING PIG - LOL!", victim->GetName ().GetData ());
/*
	if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 50u)
		ChatMessage (ChatType_Killing);
*/
	// if this enemy was close to us, spray a logo on his dead body :P
	if (GetOrigin ().GetDistanceSquared (victim->GetOrigin ()) < MATH_GET_SQUARED (200.0f) && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 33u)
		m_timeLogoSpray = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (0.7f, 2.0f);

	if (victim->HasWeapons (HalfLifeEngine::SDK::Constants::WeaponBits_SniperRifles) && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 65u)
	{
		switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
		{
			case Console::Bot::VariableValue_Communication_Radio:
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_EnemyDown, 0.6f);

				break;

			case Console::Bot::VariableValue_Communication_Voice:
				ChatterMessage (::ChatterManager::ChatterMessage_SniperKilled, 0.6f);

				break;
		}
	}
	else switch (g_server->GetClientManager ()->GetAliveClientsNumber (victimRealTeam))
	{
		case 0u:
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 65u)
				ChatterMessage (::ChatterManager::ChatterMessage_NoEnemiesLeft, 0.6f);
			else
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_EnemyDown, 0.6f);

			break;

		case 1u:
			ChatterMessage (::ChatterManager::ChatterMessage_OneEnemyLeft, 0.6f);

			break;

		case 2u:
			ChatterMessage (::ChatterManager::ChatterMessage_TwoEnemiesLeft, 0.6f);

			break;

		case 3u:
			ChatterMessage (::ChatterManager::ChatterMessage_ThreeEnemiesLeft, 0.6f);

			break;

		// More than three enemies....
		default:
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 65u)
				RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_EnemyDown, 0.6f);

			break;
	}

	// if no more enemies found AND bomb planted, switch to knife to get to bombplace faster
	if (realTeam == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist && g_server->GetClientManager ()->GetAliveClientsNumber (victimRealTeam) == 0u && g_server->GetC4Manager ().IsÑ4Planted ())
	{
		if (IsShieldDrawn ())
			m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;

		SelectBestWeaponWithHighestSpeed ();

		const Math::Vector3D &C4Origin (g_server->GetC4Manager ().GetC4 ()->GetEdict ()->GetOrigin ());

		// order team to regroup
		if (GetOrigin ().GetDistanceSquared (C4Origin) < MATH_GET_SQUARED (500.0f) && g_server->GetClientManager ()->GetClientsNumberNearPosition (C4Origin, 600.0f, HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist) == 0u)
			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
			{
				case Console::Bot::VariableValue_Communication_Radio:
					RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_RegroupTeam, 0.6f);

					break;

				case Console::Bot::VariableValue_Communication_Voice:
					ChatterMessage (::ChatterManager::ChatterMessage_BombSiteSecure, 0.6f);

					break;
			}
	}

	if (g_server->IsTeamplay () && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () == Console::Bot::VariableValue_Communication_Voice && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) >= 80u)
		for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
		{
			YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

			if (!bot->IsAlive ())
				continue;

			// need to send congrats on well placed shot
			if (isHeadshot && this != bot && bot->IsPositionVisible (victim->GetOrigin ()) && realTeam == bot->GetRealTeam ())
				bot->ChatterMessage (::ChatterManager::ChatterMessage_NiceShotPall, 0.6f);
			else if (victim == g_server->GetHumanManager ()->GetHostClient () && victimRealTeam == bot->GetRealTeam () && bot->GetNearbyTeammatesNearPosition (bot->GetOrigin (), 500.0f) == 0u && g_server->GetClientManager ()->GetAliveClientsNumber (bot->GetRealTeam ()) >= 3u)	// inform about death of the commander...
				bot->ChatterMessage (::ChatterManager::ChatterMessage_CommanderDown, 0.6f);

			break;
		}
/*
	for (index = 0u; index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
	{
		bot = g_server->GetYaPBManager ()->GetYaPBsArray ()[index];

		if (!bot->IsAlive () || bot->GetTeam () != victimRealTeam || realTeam == victimRealTeam || bot->m_enemy->IsValid () || !IsVisible (GetOrigin (), bot->m_edict))
			continue;

		// notice nearby to victim teammates, that attacker is near
		bot->m_enemy = bot->m_lastEnemy = this;
		bot->m_lastEnemyOrigin = GetOrigin ();

		bot->m_actualReactionTime = 0.0f;
		bot->m_enemy->seeTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	}

	// is this message about a bot who killed somebody?
	if (realTeam == victimRealTeam)	// did a human kill a bot on his team?
	{
		// no movement allowed in
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Votes)->GetValue <bool> ())	// we got a teamkiller? Vote him away...
		{
			ExecuteCommandFormat ("vote %u", killerIndex);

			// if bot tk punishment is enabled slay the tk
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_TKPunish)->GetValue <int> () == 2 && !HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (killerIndex)->IsFakeClient ())
			{
				// remember tk entity
				client = g_server->GetClientManager ()->GetClient (killerIndex);

				InternalAssert (g_clientManager->IsClientSlotUsed (client));

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_PAS, HalfLifeEngine::SDK::Constants::SVC_TEMPENTITY, client->GetOrigin ());
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (HalfLifeEngine::SDK::Constants::TempEntityEvent_TarExplosion);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteCoordinates (client->GetOrigin ());
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_PVS, HalfLifeEngine::SDK::Constants::SVC_TEMPENTITY, client->GetOrigin ());
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (HalfLifeEngine::SDK::Constants::TempEntityEvent_LavaSplash);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteCoordinates (client->GetOrigin ());
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();

				++client->GetEdict ()->variables.frags;

				// kill the teamkiller
				HalfLifeEngine::Globals::g_halfLifeEngine->ClientKill (*client);

				HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::/*MSG_ONE*//*MSG_ONE_UNRELIABLE, g_networkMessage.GetID (Type_ScreenFade), NULL, *client);
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (BIT (15u));	// duration
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (BIT (10u));	// hold time
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (BIT (1u));	// flags
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (100);	// color RED
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (0);	// color GREEN
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (0);	// color BLUE
					HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// alpha
				HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();

				client->HudMessage (Math::Vector2D (-1.0, -1.0), ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (35u, 255u)), ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u), HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 0.1f, 5, 0.0078125, 2, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose, "You was slayed, because of teamkilling a player.\nPlease be careful in the future...");

				// very fun thing ={P
				client->ExecuteCommand ("cd eject\n");
			}
		}
		else if (m_voteMap > 0)	// host wants the bots to vote for a map?
		{
			ExecuteCommandFormat ("votemap %u", m_voteMap);

			// and reset the vote map ID
			m_voteMap = 0u;
		}
	}*/
}

void YaPB::ClientKilled (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const killer)
{
	// Is called when the one client/bot (not me) is kill other client/bot (not me).

	// Reliability check.
	InternalAssert (victim != NULL);

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::ClientKilled(victim: \"%s\", killer: \"%s\"): Called!\n", GetName ().GetData (), victim->GetName ().GetData (), killer->IsPlayer () ? g_server->GetClientManager ()->GetClient (killer)->GetName ().GetData () : "NULL");

	m_scheduleManager.OnClientKilled (victim, killer);

	if (victim->GetEdict () != killer)
	{
		if (killer->IsPlayer ())
		{
			const HalfLifeEngine::SDK::Constants::PlayerBodyPart_t victimLastHitGroup (victim->GetEdict ()->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::PlayerBodyPart_t> (HalfLifeEngine::SDK::Constants::Offset_BaseMonster_LastHitGroup));
			const HalfLifeEngine::SDK::Constants::WeaponID_t killerWeaponID (g_server->GetClientManager ()->GetClient (killer)->GetCurrentWeapon ()->GetID ());
			const bool isHeadshot (victimLastHitGroup == HalfLifeEngine::SDK::Constants::PlayerBodyPart_Head);	// Was headshot or not?

			if (killerWeaponID == HalfLifeEngine::SDK::Constants::WeaponID_Knife && isHeadshot)
				ExecuteCommandFormat ("say HAHAHAH!!! %s DIES LIKE FUCKING PIG - LOL!", victim->GetName ().GetData ());
		}
	}
	else
		ExecuteCommandFormat ("say HAHAHAH!!! %s LOL!", victim->GetName ().GetData ());
}

void YaPB::AliveSpawnPost (void)
{
	// Is called when the bot is spawned.

	m_lastThinkTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	m_spawnTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	m_fallDownTime = 0.0f;

	m_maximumViewDistance = sm_maximumViewDistance;

	m_shootTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
	m_firePause = 0.0f;

	m_reloadState = ReloadState_None;
	m_reloadCheckTime = 0.0f;

	m_shieldCheckTime = 0.0f;
	m_zoomCheckTime = 0.0f;
	m_grenadeCheckTime = 0.0f;
	m_silencerCheckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (0.7f, 3.5f);

	ResetApproachPoints ();

	m_lastAttackerInformation.Reset ();
	m_scheduleManager.FreeMemory ();	// Clear tasks.

	m_navigator->AliveSpawnPost ();

	// Call original function in base class....
	FakeClient::AliveSpawnPost ();

	m_scheduleManager.AddAction (new Task_Normal (this));	// enter the normal AI node

	m_scheduleManager.OnOwnerSpawn ();
}

void YaPB::OnEmitSound (const HalfLifeEngine::SDK::Classes::Edict *const/* edict*/, const NoisedSound_t &/*sound*/)
{
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::OnEmitSound(): sample: \"%s\", vol=%.1f, attn=%.2f, radius=%.2f, dur=%.2f.", GetName ().GetData (), sample.GetData (), volume, attenuation, hearingDistance, maximumLastingTime);
}

void YaPB::Touched (const HalfLifeEngine::SDK::Classes::Edict *const entity)
{
	m_scheduleManager.OnTouched (entity);

/*	const HalfLifeEngine::SDK::Structures::TraceResult_t traceResult =
	{
		static_cast <BOOL> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceIsAllSolid),
		static_cast <BOOL> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceIsStartSolid),
		static_cast <BOOL> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceIsInOpen),
		static_cast <BOOL> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceIsInWater),
		HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceFraction,
		HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceEndPosition,
		HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->tracePlane.distance,
		HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->tracePlane.normal,
		HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceHitEntity,
		HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->traceHitGroup
	};

	if (!entity->IsPlayer ())
		return;

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "YaPB::Touched(): Player \"%s\" touched YaPB \"%s\".\n", entity->GetNetName ().GetData (), GetName ().GetData ());
*/
}

const bool YaPB::IsSimpleDeadlyDrop (const Math::Vector3D &destination)
{
	// This function returns true if given location would hurt bot with falling damage, false otherwise.

	const float traceCheckOffset (1000.0f);
	Math::Vector3D down (destination);	// straight down 1000 units

	down.z -= traceCheckOffset;	// straight down 1000 units

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (destination, down, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

	// wall blocking?
	if (m_traceResult.isStartSolid)
		return false;

	const float height (m_traceResult.fraction * traceCheckOffset);	// height from ground at goal position

	// drops more than 100 units?
	return height > 100.0f;
}

const bool YaPB::IsDeadlyDrop (const Math::Vector3D &destination)
{
	// This function returns true if given location would hurt bot with falling damage, false otherwise.

	/// THIS FUNCTION SHOULD BE REWRITED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	Math::Vector3D check (GetOrigin ()), down (GetOrigin ());
	const float maximumTraceDistanceMove (16.0f);
	const Math::Vector3D direction ((destination - GetOrigin ()).Normalize () * maximumTraceDistanceMove);	// 16 units long
	const float traceCheckOffset (1000.0f);
	const float onGroundOffset (HalfLifeEngine::SDK::Constants::HalfHumanHeight / traceCheckOffset);

	down.z -= traceCheckOffset;	// straight down 1000 units

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (check, down, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

	// we're not on ground anymore?
	if (m_traceResult.fraction > onGroundOffset)
		m_traceResult.fraction = onGroundOffset;

	// height from ground, distance from goal
	for (float height, lastHeight (m_traceResult.fraction * traceCheckOffset), distanceSquared (destination.GetDistanceSquared (check)); distanceSquared > MATH_GET_SQUARED (maximumTraceDistanceMove); lastHeight = height, distanceSquared = destination.GetDistanceSquared (check))
	{
		// move 16 units closer to the goal....
		check += direction;

		down.x = check.x;
		down.y = check.y;
		down.z = check.z - traceCheckOffset;	// straight down 1000 units

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (check, down, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

		// wall blocking?
		if (m_traceResult.isStartSolid)
			return false;

		// height from ground
		height = m_traceResult.fraction * traceCheckOffset;	// Convert fraction to distance

		// drops more than 100 units?
		if (lastHeight < height - 100.0f)
		{
			#if defined _DEBUG
				DebugMessageFormat ("can fall down from tested location 'check' == [%.2f, %.2f, %.2f].", check.x, check.y, check.z);
			#endif	// if defined _DEBUG

			return true;
		}
	}

	return false;
}

void YaPB::SelectBestWeapon (void)
{
	// This function chooses best weapon, from weapons that bot currently own, and change current weapon to best one.

	if
	(
		Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_KnifeMode)->GetValue <bool> () ||
		m_edict->GetPlayerModel () == "chicken" ||
		m_edict->GetPlayerModel ().CompareWithCaseCheck ("zombie", GET_STATIC_STRING_LENGTH ("zombie")) == 0
	)
	{
		// if knife mode activated, force bot to use knife
		if (m_currentWeapon->GetID () != HalfLifeEngine::SDK::Constants::WeaponID_Knife)
			SelectWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Knife);	// select this weapon if it isn't already selected

		return;
	}

	// Loop through all weapons until best is found....
	for (Profile::WeaponsPreferences_t::Array_t::IndexType_t index (0u); index < m_profile->weaponsPreferences.common.GetElementNumber (); ++index)
	{
		const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID (m_profile->weaponsPreferences.common[index]);

		// is the bot NOT carrying this weapon OR there is still ammo in clip?
		if (!HasWeapon (weaponID) || (HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].ammoID > 0u && m_ammo[weaponID].clip + m_ammo[HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].ammoID].total == 0u/*HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].ammoMinimumAmount*/))
			continue;

		if (m_currentWeapon->GetID () != weaponID)
			SelectWeapon (weaponID);	// select this weapon if it isn't already selected

		return;
	}

	if (m_currentWeapon->GetID () != HalfLifeEngine::SDK::Constants::WeaponID_Knife)
		SelectWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Knife);	// select this weapon if it isn't already selected
/*
	HalfLifeEngine::SDK::Constants::WeaponID_t chosenWeaponIndex (HalfLifeEngine::SDK::Constants::WeaponID_None);

	// Loop through all weapons until best is found....
	for (unsigned char index (0u); index < m_profile->weaponsPreferences.common.GetElementNumber (); ++index)
	{
		const HalfLifeEngine::SDK::Constants::WeaponID_t weaponID (m_profile->weaponsPreferences.common[index]);

		// is the bot NOT carrying this weapon?
		if (!HasWeapon (weaponID))
			continue;

		if
		(
			(m_currentWeapon->GetID () == weaponID && (m_currentWeapon->GetAmmoInClip () == -1 || m_currentWeapon->GetAmmoInClip () >= HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].ammoMinimumAmount)) ||	// is the bot already holding this weapon and there is still ammo in clip?
			(HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].ammoID == 0u || m_ammo[HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].ammoID].total >= HalfLifeEngine::SDK::Constants::weaponProperties[weaponID].ammoMinimumAmount)	// is no ammo required for this weapon OR enough ammo available to fire
		)
			chosenWeaponIndex = weaponID;
	}

	#if defined _DEBUG
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("YaPB(%s)::SelectBestWeapon(): wants to select weapon: %s(%u) (curwpn == %u, ammoInClip == %i, ammo == %u).", GetName ().GetData (), HalfLifeEngine::SDK::Constants::weaponProperties[chosenWeaponIndex].className.GetData (), chosenWeaponIndex, m_currentWeapon->GetID (), m_currentWeapon->GetAmmoInClip (), m_currentWeapon->GetAmmo ());
	#endif	// if defined _DEBUG

	if (chosenWeaponIndex == HalfLifeEngine::SDK::Constants::WeaponID_None || chosenWeaponIndex == m_currentWeapon->GetID ())
		return;

	// select this weapon if it isn't already selected
	SelectWeapon (chosenWeaponIndex);

	m_reloadState = ReloadState_None;*/
}

const bool YaPB::CheckVisibility (const HalfLifeEngine::SDK::Classes::Edict *const targetEntity, Math::Vector3D &origin, Visibility_t &bodyPart)
{
	// this function checks visibility of a bot target.
/*
	bool semiTransparent (false);

	if (m_profile->skill < BotSkillLevel_Godlike)
	{
		// don't look through water
		if (m_edict->IsInWater () != targetEntity->IsInWater ())
			semiTransparent = true;

		if (targetEntity->variables.speed <= 100.0f)
		{
			const int renderFx (targetEntity->variables.renderFx);

			// 'kRenderFxExplode' is always invisible even for mode 'kRenderNormal'
			if (renderFx == HalfLifeEngine::SDK::Constants::kRenderFxExplode || (targetEntity->variables.effects & HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw))
			{
				if (!(targetEntity->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
					return false;

				semiTransparent = true;
			}
			else if (renderFx != HalfLifeEngine::SDK::Constants::kRenderFxHologram && renderFx != HalfLifeEngine::SDK::Constants::kRenderFxExplode && targetEntity->variables.renderMode != HalfLifeEngine::SDK::Constants::kRenderNormal)	// 'kRenderFxHologram' is always visible no matter what is the mode
			{
				const float renderAmount (targetEntity->variables.renderAmount);

				if (renderFx == HalfLifeEngine::SDK::Constants::kRenderFxGlowShell)
				{
					const Math::Vector3D renderColor (targetEntity->variables.rendercolor);

					if (renderAmount <= 20.0f && renderColor <= Math::Vector3D (20.0f, 20.0f, 20.0f))
					{
						if (!(targetEntity->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
							return false;

						semiTransparent = true;
					}
					else if (renderAmount <= 60.0f && renderColor <= Math::Vector3D (60.0f, 60.0f, 60.0f))
						semiTransparent = true;
				}
				else
				{
					if (renderAmount <= 20.0f)
					{
						if (!(targetEntity->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
							return false;

						semiTransparent = true;
					}
					else if (renderAmount <= 60.0f)
						semiTransparent = true;
				}
			}

			if (!IsUsingNightVisionGoggles () && !IsUsesFlashlight ())
			{
				const unsigned char lightLevel (targetEntity->GetIllumination ());

				if (((lightLevel <= 3u && g_skyLightLevel > 50u) || (lightLevel <= 25u && g_skyLightLevel <= 50u)) && !(targetEntity->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
					return false;
				else if (((lightLevel <= 10u && g_skyLightLevel > 50u) || (lightLevel <= 30u && g_skyLightLevel <= 50u)) && (targetEntity->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK))	// in this case we can notice the enemy, but not so good...
					semiTransparent = true;
			}
		}
	}
*/
	const Math::Vector3D botHead (m_edict->GetEyePosition ());

	bodyPart = Visibility_None;

	// Check direct Line to waist
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (botHead, targetEntity->variables.origin, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, m_traceResult);

	if (m_traceResult.fraction == 1.0f/* && !semiTransparent*/)
	{
		origin = m_traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, 6.0f);
		bodyPart |= Visibility_Body;
	}

	// Check direct Line to head
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (botHead, targetEntity->GetEyePosition (), HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, m_traceResult);

	if (m_traceResult.fraction == 1.0f/* && !semiTransparent*/)
	{
		origin = m_traceResult.endPosition;
		bodyPart |= Visibility_Head;
	}

	if (bodyPart > Visibility_None)
		return true;

	Math::Vector3D targetOrigin;
#if 0
	// dimension table
	const Math::Vector3D dimensionTable[8u] =
	{
		Math::Vector3D ( 1.0f,  1.0f,  1.0f),
		Math::Vector3D ( 1.0f,  1.0f, -1.0f),
		Math::Vector3D ( 1.0f, -1.0f,  1.0f),
		Math::Vector3D (-1.0f,  1.0f,  1.0f),
		Math::Vector3D ( 1.0f, -1.0f, -1.0f),
		Math::Vector3D (-1.0f, -1.0f,  1.0f),
		Math::Vector3D (-1.0f,  1.0f, -1.0f),
		Math::Vector3D (-1.0f, -1.0f, -1.0f)
	};

	// check the box borders
	for (unsigned char index (7u); index <= 7u; --index)
	{
		targetOrigin = targetEntity->variables.origin + Math::Vector3D (dimensionTable[index].x * 24.0f, dimensionTable[index].y * 24.0f, dimensionTable[index].z * 24.0f);

		// check direct line to random part of the player body
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (botHead, targetOrigin, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, m_traceResult);

		// check if we hit something
		if (m_traceResult.fraction == 1.0f/* && !semiTransparent*/)
		{
			origin = m_traceResult.endPosition;
			bodyPart |= Visibility_Other;

			return true;
		}
	}
#else
	// worst case, choose random position in enemy body
	for (unsigned char index (0u); index < 5u; ++index)
	{
		// get the player origin, find the vector beetwen mins and maxs of the player body, check direct line to random part of the player body
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (botHead, targetEntity->variables.origin + Math::Vector3D (g_randomNumberGenerator.GetValueBetween (targetEntity->variables.boundingBox.mins.x, targetEntity->variables.boundingBox.maxs.x), g_randomNumberGenerator.GetValueBetween (targetEntity->variables.boundingBox.mins.y, targetEntity->variables.boundingBox.maxs.y), g_randomNumberGenerator.GetValueBetween (targetEntity->variables.boundingBox.mins.z, targetEntity->variables.boundingBox.maxs.z)), HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, m_traceResult);

		// check if we hit something
		if (m_traceResult.fraction == 1.0f/* && !semiTransparent*/)
		{
			origin = m_traceResult.endPosition;
			bodyPart |= Visibility_Other;

			return true;
		}
	}
#endif	// if 0
	return false;
}

const bool YaPB::IsBehindSmokeClouds (const Math::Vector3D &origin)
{
	// find all visible grenades on the map, which laying on the floor
	for (HalfLifeEngine::SDK::Classes::Edict *smokeGrenade (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "grenade")); smokeGrenade->IsValid (); smokeGrenade = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (smokeGrenade, "grenade"))
	{
		if (!smokeGrenade->IsGrenadeIsSmokeGrenade () || (smokeGrenade->variables.effects & HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw) || !smokeGrenade->IsNonPlayerOnFloor ()/* || GetAbsoluteFieldOfView (origin - m_edict->GetEyePosition ()) > GetFieldOfView () / 3.0f*/)
			continue;	// Skip....

		// trace a line from bot's eyes to origin...
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), origin, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, m_traceResult);

		// check if line of sight to object is not blocked (i.e. visible)
		if (m_traceResult.fraction < 1.0f)
			continue;	// Skip....

		const Math::Vector3D &grenadeOrigin (smokeGrenade->variables.origin/*GetOrigin ()*/);
		const Math::Vector3D betweenNade ((grenadeOrigin - GetOrigin ()).Normalize ());

		if ((betweenNade | (origin - GetOrigin ()).Normalize ()) > (betweenNade | ((grenadeOrigin + Math::Vector2D (betweenNade.y, betweenNade.x) * 150.0f) - GetOrigin ()).Normalize ()))
			return true;
	}

	return false;
}

const bool YaPB::IsShootableThruObstacle (Math::Vector3D source, const Math::Vector3D &destination)
{
	// This function returns true if enemy can be shoot through some obstacle, false otherwise.

	unsigned char currentWeaponPenetrationPower (static_cast <unsigned char> (HalfLifeEngine::SDK::Constants::weaponProperties[m_currentWeapon->GetID ()].penetrationPower1));
/*
	if (currentWeaponPenetrationPower == HalfLifeEngine::SDK::Constants::WeaponObstaclePenetrationPower_0)
		return false;
*/
	// set conditions....
	const Math::Vector3D direction ((destination - source).Normalize () * 8.0f);	// 8 units long

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		// trace from the bot's eyes to destination...
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters/*TraceIgnore_None*/, m_edict, m_traceResult);

		if (m_traceResult.isStartSolid)
		{
			if (m_traceResult.isAllSolid)
				return false;

			// move 8 units closer to the destination....
			source += direction;
		}
		else
		{
			// check if line hit anything
			if (m_traceResult.fraction == 1.0f)
				return true;

			if (--currentWeaponPenetrationPower == 0u)
				return false;

			// move 8 units closer to the destination....
			source = m_traceResult.endPosition + direction;
		}
	}
}

const unsigned char YaPB::GetApproximateBaseDamage (const Math::Vector3D &source, const Math::Vector3D &destination)
{
	const HalfLifeEngine::SDK::Constants::WeaponID_t currentWeaponID (/*g_server->GetHumanManager ()->GetHostClient ()->*/GetCurrentWeapon ()->GetID ());
	unsigned char currentWeaponPenetrationPower (static_cast <unsigned char> (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].penetrationPower2));
/*
	if (currentWeaponPenetrationPower == HalfLifeEngine::SDK::Constants::WeaponObstaclePenetrationPower_0)
		return 0u;
*/
	Math::Vector3D start (source), end, direction (destination - source);
	float distanceFraction, damageMult (0.5f/* default modification of bullets power after they go through a wall. */), distanceMaximum (0.0f);
	const float length (direction.GetLength ());
	unsigned char bulletVelocity (0u);
	float distance (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].maximumShootDistance1);
	unsigned char damage (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].damage1);
	float obstaclePierce (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].obstaclePierce1);

	if (m_currentWeapon->IsValid () && m_currentWeapon->IsSilenced ())
	{
		distance = HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].maximumShootDistance2;
		damage = HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].damage2;
		obstaclePierce = HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].obstaclePierce2;
	}

	// Reliability check.
	InternalAssert (length > 0.0f);

	direction /= length;	// Normalize.

	end = direction * distance + start;

	++currentWeaponPenetrationPower;

	// damage gets a randomlong added to it in first switch
	switch (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].bulletType)
	{
		case HalfLifeEngine::SDK::Constants::WeaponBulletType_1:
			bulletVelocity = 21u;
			distanceMaximum = 800.0f;

			break;

		case HalfLifeEngine::SDK::Constants::WeaponBulletType_9:
			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_WallSensitivity)->GetValue <unsigned char> ())
			{
				case 0u:
					damage -= 2u;

					break;

				case 2u:
					damage += 2u;

					break;
			}

			bulletVelocity = 15u;
			distanceMaximum = 500.0f;

			break;

		case HalfLifeEngine::SDK::Constants::WeaponBulletType_13:
			bulletVelocity = 30u;
			distanceMaximum = 1000.0f;

			break;

		case HalfLifeEngine::SDK::Constants::WeaponBulletType_11:
			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_WallSensitivity)->GetValue <unsigned char> ())
			{
				case 0u:
					damage -= 2u;

					break;

				case 2u:
					damage += 2u;

					break;
			}

			bulletVelocity = 39u;
			distanceMaximum = 5000.0f;

			break;

		case HalfLifeEngine::SDK::Constants::WeaponBulletType_12:
			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_WallSensitivity)->GetValue <unsigned char> ())
			{
				case 0u:
					damage -= 3u;

					break;

				case 2u:
					damage += 3u;

					break;
			}

			bulletVelocity = 35u;
			distanceMaximum = 4000.0f;

			break;

		case HalfLifeEngine::SDK::Constants::WeaponBulletType_10:
			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_WallSensitivity)->GetValue <unsigned char> ())
			{
				case 0u:
					damage -= 4u;

					break;

				case 2u:
					damage += 4u;

					break;
			}

			bulletVelocity = 45u;
			distanceMaximum = 8000.0f;

			break;

		case HalfLifeEngine::SDK::Constants::WeaponBulletType_14:
			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_WallSensitivity)->GetValue <unsigned char> ())
			{
				case 0u:
					damage -= 4u;

					break;

				case 2u:
					damage += 6u;

					break;

				default:
					++damage;

					break;
			}

			bulletVelocity = 30u;
			distanceMaximum = 2000.0f;

			break;

		case HalfLifeEngine::SDK::Constants::WeaponBulletType_15:
			switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_WallSensitivity)->GetValue <unsigned char> ())
			{
				case 0u:
					damage -= 4u;

					break;

				case 2u:
					damage += 6u;

					break;

				default:
					++damage;

					break;
			}

			bulletVelocity = 25u;
			distanceMaximum = 800.0f;

			break;

		default:
			break;
	}

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (start, end, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters/*TraceIgnore_None*/, m_edict, m_traceResult);

		if (m_traceResult.fraction == 1.0f)
			return 0u;

		// Hit a shield, AND IT'S MIGHTY STRONG!. Don't penetrate.
		if (m_traceResult.hitGroup == HalfLifeEngine::SDK::Constants::PlayerBodyPart_Shield)
			return 0u;

		/************* MATERIAL DETECTION ***********/
		switch (HalfLifeEngine::Globals::g_halfLifeEngine->GetTextureType (m_traceResult.hitEntity, start, end))
		{
			case HalfLifeEngine::SDK::Constants::TextureType_Concrete:
				bulletVelocity = static_cast <unsigned char> (bulletVelocity * 0.25f);

				break;

			case HalfLifeEngine::SDK::Constants::TextureType_Grate:
				bulletVelocity = static_cast <unsigned char> (bulletVelocity * 0.5f);
				damageMult = 0.4f;

				break;

			case HalfLifeEngine::SDK::Constants::TextureType_Metal:
				bulletVelocity = static_cast <unsigned char> (bulletVelocity * 0.15f);	// If we hit metal, reduce the thickness of the brush we can't penetrate
				damageMult = 0.2f;

				break;

			case HalfLifeEngine::SDK::Constants::TextureType_Computer:
				bulletVelocity = static_cast <unsigned char> (bulletVelocity * 0.4f);
				damageMult = 0.45f;

				break;

			case HalfLifeEngine::SDK::Constants::TextureType_Tile:
				bulletVelocity = static_cast <unsigned char> (bulletVelocity * 0.65f);
				damageMult = 0.3f;

				break;

			case HalfLifeEngine::SDK::Constants::TextureType_Ventillation:
				bulletVelocity = static_cast <unsigned char> (bulletVelocity * 0.5f);
				damageMult = 0.45f;

				break;

			case HalfLifeEngine::SDK::Constants::TextureType_Wood:
				damageMult = 0.6f;

				break;

			default:
				break;
		}

		if (m_traceResult.endPosition.GetDistance (source) >= length)
		{
			m_traceResult.fraction = destination.GetDistance (start) / end.GetDistance (start);

			distanceFraction = distance * m_traceResult.fraction;

			// calculate the damage based on the distance the bullet travelled.
			return static_cast <unsigned char> (damage * Math::powf (obstaclePierce, distanceFraction * (1.0f / 500.0f)));
		}

		if ((distanceFraction = distance * m_traceResult.fraction) > distanceMaximum)
			return 0u;

		// calculate the damage based on the distance the bullet travelled.
		damage = static_cast <unsigned char> (damage * Math::powf (obstaclePierce, distanceFraction * (1.0f / 500.0f)));

		if (--currentWeaponPenetrationPower > 0u && m_traceResult.hitEntity->variables.solid == HalfLifeEngine::SDK::Constants::SOLID_BSP)
		{
			start = m_traceResult.endPosition + direction * bulletVelocity;
			distance = Math::GetExtent (distanceFraction, distance);
			end = start + direction * distance;
			damage = static_cast <unsigned char> (damage * damageMult);
		}
		else
		{
			start = m_traceResult.endPosition + direction * 42.0f;
			distance = (distance - distanceFraction) * 0.75f;
			end = start + direction * distance;
			damage = static_cast <unsigned char> (damage * 0.75f);
		}
	}
}

const bool YaPB::FindEnemy (void)
{
	// This function tries to find the best suitable enemy for the bot.

	if (m_enemy->IsValid ())
		m_enemy->visibility = Visibility_None;

	// do not search for enemies while we're blinded, OR shooting disabled by user
	if (m_lastScreenFadeInformation.duration > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()/* || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DontShoot)->GetValue <bool> ()*/)
		return false;

	float currentDistance;
	Math::Vector3D enemyOrigin;
	Visibility_t enemyVisibility;

	// see if we can still see the current enemy...
	if (m_enemy->IsValid ())
	{
		if (CheckVisibility (*m_enemy->pointer, enemyOrigin, enemyVisibility))
		{
			m_enemy->origin = enemyOrigin;
			m_enemy->visibility = enemyVisibility;
		}
		else
			DeleteEnemy ();	// we can no longer see this enemy
	}

	// if we already have an enemy...
	if (m_enemy->IsValid ())
	{
		// don't discard important enemies (bomb/flag/hostage carrier, VIP, etc)
		if (m_enemy->pointer->IsImportant ())
			return false;

		// calculate the distance to the enemy
		currentDistance = m_enemy->pointer->GetOrigin ().GetDistance (GetOrigin ());
	}
	else
		currentDistance = m_maximumViewDistance;

	// Setup potentially visible set for this bot....
	const Math::Vector3D eyePosition (GetEyePosition ());
	const Math::Vector3D potentialVisibility (IsDucking () ? eyePosition + (HalfLifeEngine::SDK::Constants::VEC_HULL_MIN - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN) : eyePosition);	// Temp vector.
	const unsigned char *const potentiallyVisible (HalfLifeEngine::Globals::g_halfLifeEngine->SetPotentiallyVisible (potentialVisibility));
	const unsigned char team (GetTeam ());
	bool isSeeEnemyGunThruWallAndEnemyHeadIsPenetrable;

	// Loop through all the clients/bots...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (!client->IsAlive () || client->GetTeam () == team || client->GetEdict ()->IsImmortal ()/* || client->GetEdict ()->GetHealth () / 2 > client->GetEdict ()->GetMaximumHealth ()*/ || (m_enemy->IsValid () && client == m_enemy->pointer))
			continue;	// skip myself, our teammates and immortals/current enemy....

		isSeeEnemyGunThruWallAndEnemyHeadIsPenetrable = false;

		// let the engine check if this player is potentially visible
		if (!HalfLifeEngine::Globals::g_halfLifeEngine->CheckVisibility (*client, potentiallyVisible))
		{
			const float enemyCurrentWeaponEdgePointOrigin (HalfLifeEngine::SDK::Constants::weaponProperties[client->GetCurrentWeapon ()->GetID ()].edgePointOrigin);

			// Check weapon point, first we need to check if it is no equal to 0
			if (enemyCurrentWeaponEdgePointOrigin == 0.0f)
				continue;

			// We get the view angles and turn them in directional vectors
			// We multiply it to obtain the weapon headpoint
			// We add it to the end origin to obtain the weapon headpoint
			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (eyePosition, client->GetEyeForwardPositionWithPunchAngles (enemyCurrentWeaponEdgePointOrigin), HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, m_traceResult);

			// If weapon head is visible, and enemy head is shootable?
			if (m_traceResult.fraction < 1.0f || !IsShootableThruObstacle (eyePosition, client->GetEdict ()->GetEyePosition ()))
				continue;

			isSeeEnemyGunThruWallAndEnemyHeadIsPenetrable = true;
		}
/*! @TODO: REMOVE THIS DUE IsImmortal() CHECK!?
		// skip glowed players, in free for all mode, we can't hit them
		if ((client->GetEdict ()->variables.renderFx == HalfLifeEngine::SDK::Constants::kRenderFxGlowShell && Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Deathmatch)->GetValue <bool> ()))
			continue;
*/
		const float distance (client->GetOrigin ().GetDistance (GetOrigin ()));

		// if this enemy is further away than the current one...
		if (distance > currentDistance && !client->IsImportant ())
			continue;	// skip it

		if (distance > 4000.0f * m_profile->skill.GetIncreasingOffset () + 900.0f)
			continue;	// enemy is too far

		// if the bot's health decreased or the enemy is shooting
		const float fov (m_enemy->IsValid () && (/*m_previousHealth > m_edict->GetHealth () || */client->IsAttacking ()) ? 360.0f : GetFieldOfView () * 2.0f - (GetFieldOfView () - (distance > GetFieldOfView () * 9.0f ? GetFieldOfView () * 9.0f : distance) / 9.0f));

		// check if enemy is in the view cone
		if (!IsInViewCone (client->GetOrigin (), fov))
			continue;	// enemy isn't in bot's view cone

		// check if enemy is visible
		if (isSeeEnemyGunThruWallAndEnemyHeadIsPenetrable)
		{
			enemyOrigin = client->GetEdict ()->GetEyePosition ();
			enemyVisibility = Visibility_Head;
		}
		else if (!CheckVisibility (*client, enemyOrigin, enemyVisibility))
			continue;	// skip this enemy

		// if the enemy is quite far away, not shooting and the bot is not damaged
		if (!m_enemy->IsValid () && ((distance > 200.0f/* && m_previousHealth <= m_edict->GetHealth ()*/ && !client->IsAttacking () && !client->IsInViewCone (GetOrigin (), 120.0f)) || isSeeEnemyGunThruWallAndEnemyHeadIsPenetrable))
		{
			// if the bot isn't in the fov of the enemy and the bot doesn't really want to fight
			if (WantsToRetreat ())
				continue;	// skip this enemy
		}

		// found a new enemy

		DeleteEnemy ();	// Delete old one....

		m_enemy = new Enemy_t (client, enemyOrigin, enemyVisibility, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());

		#if defined _DEBUG
			// Reliability check.
			if (m_enemy == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating new bot \"%s\" enemy \"%s\"!", sizeof (*m_enemy), GetName ().GetData (), client->GetName ().GetData ());

				return false;
			}

//			AddLogEntry (true, LogLevel_Default, false, "\"%s\"::FindEnemy(): Found new enemy: \"%s\".", GetName ().GetData (), m_enemy->pointer->GetName ().GetData ());
		#endif	// if defined _DEBUG

		return true;
	}

	return false;	// no new enemy is found
}

void YaPB::CheckSilencer (void)
{
	// no checking when shiled is active
	if (HasShield () || !m_currentWeapon->IsValid () || m_currentWeapon->GetNextAttackDelay () > 0.0f || (GetEdict ()->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK2))
		return;

	// aggressive bots don't like the silencer, but careful bots - like the silencer and normal bot 50/50%...
	const unsigned char percent (GetProfile ()->personality == Personality_Rusher ? 30u : GetProfile ()->personality == Personality_Careful ? 70u : 50u);

	switch (m_currentWeapon->GetID ())
	{
		case HalfLifeEngine::SDK::Constants::WeaponID_USP:
			// is the silencer attached...
			if (m_currentWeapon->GetEdict ()->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::WeaponState_t> (HalfLifeEngine::SDK::Constants::Offset_Weapon_State) & HalfLifeEngine::SDK::Constants::WeaponState_USP_SILENCED)
			{
				if (GetProfile ()->skill >= 80u || g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < percent)
					GetEdict ()->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;	// detach the silencer
			}
			else if (GetProfile ()->skill <= 50u && g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < percent)	// is the silencer detached...
				GetEdict ()->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;	// attach the silencer

			break;

		case HalfLifeEngine::SDK::Constants::WeaponID_M4A1:
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < percent)
				GetEdict ()->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_ATTACK2;	// detach the silencer

			break;
	}
}

void YaPB::CheckReload (void)
{
	// This function checks the reload state.

	static HalfLifeEngine::SDK::Constants::WeaponBit_t weapons;
	static HalfLifeEngine::SDK::Constants::WeaponID_t weaponIndex;
	static unsigned char index;
/*
	// if we have enemy or something else don't reload next weapon
	if (m_isUsingGrenade || GetCurrentTask ()->taskID == Task_PlantBomb || GetCurrentTask ()->taskID == Task_DefuseBomb || GetCurrentTask ()->taskID == Task_PickupItem)
	{
		m_reloadState = ReloadState_None;

		return;
	}
*/
	if (m_reloadState == ReloadState_None)
		return;
/*
	weapons = m_edict->variables.weapons;

	weapons &= m_reloadState == ReloadState_Primary ? HalfLifeEngine::SDK::Constants::WeaponBits_Primary : HalfLifeEngine::SDK::Constants::WeaponBits_Secondary;
	weapons &= HalfLifeEngine::SDK::Constants::WeaponBits_Grenades;

	if (weapons == HalfLifeEngine::SDK::Constants::WeaponBit_None)
	{
		if (++m_reloadState >= ReloadState_Total)
			m_reloadState = ReloadState_None;

		return;
	}
*/
	const Profile::WeaponsPreferences_t::Array_t &weaponsPreferences ((&GetProfile ()->weaponsPreferences.primary)[m_reloadState]);

	for (Profile::WeaponsPreferences_t::Array_t::IndexType_t index (0u); index < weaponsPreferences.GetElementNumber (); ++index)
		if (HasWeapon (weaponsPreferences[index]) && m_ammo[weaponsPreferences[index]].clip < HalfLifeEngine::SDK::Constants::weaponProperties[weaponsPreferences[index]].ammoMaximumClip * 0.8f && m_ammo[HalfLifeEngine::SDK::Constants::weaponProperties[weaponsPreferences[index]].ammoID].total > 0u)
		{
			if (m_currentWeapon->GetID () != weaponsPreferences[index])
				SelectWeapon (weaponsPreferences[index]);	// select this weapon if it isn't already selected

			PressReload ();	// press reload button

			return;
		}

	if (++m_reloadState >= ReloadState_Total)
		m_reloadState = ReloadState_None;
}

void YaPB::UpdateEnvironmentWeapons (void)
{
	static HalfLifeEngine::SDK::Classes::Edict *edict;
	static Math::Vector3D entityOrigin;
	static DynamicString entityClassName, entityModelName;

	m_environmentEntities.weapons.RemoveAll ();
	m_environmentEntities.weaponsSearchRadius = 350.0f;

	while ((edict = HalfLifeEngine::Globals::g_halfLifeEngine->FindEntityInSphere (edict, GetOrigin (), m_environmentEntities.weaponsSearchRadius))->IsValid ())
	{
		// someone owns this weapon, OR it hasn't respawned yet
		if (edict->variables.effects & HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw)
			continue;

		// save valid entity origin
		entityOrigin = edict->GetOrigin ();
		entityClassName = edict->GetClassName ();
		entityModelName = edict->GetNonPlayerModel ();

		if (entityClassName != "weaponbox" && entityClassName != "armoury_entity" && entityClassName.CompareWithCaseCheck ("csdmw_", GET_STATIC_STRING_LENGTH ("csdmw_")) != 0)
			continue;

		// check if line of sight to object is not blocked (i.e. visible)
		if (!IsInViewCone (entityOrigin) || !ItemIsVisible (entityOrigin, edict, HalfLifeEngine::SDK::Constants::TraceIgnore_All))
			continue;

		m_environmentEntities.weapons += edict;

//		g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (entityOrigin, 5.0f, Color <> (0u, 255u, 0u), 1u, 255u, 1u, false);
	}
}
void YaPB::CheckItemsToPickup (void)
{
	// this function finds items to collect or use in the near of a bot.
#if 0
	static HalfLifeEngine::SDK::Classes::Edict *currentPickupItem, *entity;
	static float minimumDistance, distance, lightLevel;
	static bool allowPickup;
	static PickupType_t currentPickupType;
	static Math::Vector3D currentPickupOrigin, entityOrigin;
	static DynamicString entityClassName, entityModelName;
	static int primaryWeaponCarried, secondaryWeaponCarried, index;

	// don't try to pickup anything while on ladder or trying to escape from bomb...
	if (IsOnLadder ()/* || currentTask == Task_EscapeFromBomb || currentTask == Task_PlantBomb || currentTask == Task_DefuseBomb*/)
	{
		m_pickupItem = NULL;
		m_pickupType = PickupType_None;

		return;
	}

	minimumDistance = 350.0f;	// set the minimum search item distance

	if (m_pickupItem->IsValid ())
	{
		currentPickupItem = m_pickupItem;

		// find all respawned weapons on the map which nobody owns
		while ((entity = HalfLifeEngine::Globals::g_halfLifeEngine->FindEntityInSphere (entity, GetOrigin (), minimumDistance))->IsValid () && !(entity->variables.effects & HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw) && (!entity->variables.owner->IsValid () || !entity->variables.owner->IsNotWorldspawnPlayer ()) && entity == currentPickupItem)
		{
			if (ItemIsVisible (entity->GetOrigin (), entity))
				return;

			break;
		}

		m_pickupItem = NULL;
		m_pickupType = PickupType_None;
	}

	entity = currentPickupItem = m_pickupItem = NULL;
	m_pickupType = currentPickupType = PickupType_None;

	while ((entity = HalfLifeEngine::Globals::g_halfLifeEngine->FindEntityInSphere (entity, GetOrigin (), minimumDistance))->IsValid ())
	{
		// assume can't use it until known otherwise
		allowPickup = false;

		// someone owns this weapon, OR it hasn't respawned yet
		if ((entity->variables.effects & HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw) || entity == m_itemIgnore)
			continue;

		// save valid entity origin
		entityOrigin = entity->GetOrigin ();
		entityClassName = entity->GetClassName ();
		entityModelName = entity->GetNonPlayerModel ();

		// check if line of sight to object is not blocked (i.e. visible) AND if bot is not using grenade
		if (ItemIsVisible (entityOrigin, entity) && !m_isUsingGrenade)
		{
			if (entityClassName == "hostage_entity")
			{
				allowPickup = true;

				currentPickupType = PickupType_Hostage;

				DebugMessage ("found the hostage and can pickup it.");
			}
			else if (entityClassName == "weaponbox" && entityModelName == "models/w_backpack.mdl" && entity->IsNonPlayerOnFloor ())
			{
				allowPickup = true;

				currentPickupType = PickupType_DroppedC4;
			}
			else if (entityClassName == "weaponbox" || entityClassName == "armoury_entity" || entityClassName.Compare ("csdmw_", GET_STATIC_STRING_LENGTH ("csdmw_")) == 0)
			{
				allowPickup = true;

				currentPickupType = PickupType_Weapon;
			}
			else if
			(
				(entityClassName == "grenade" || entityClassName == "real_nade") &&
				(
					(entityModelName == "models/w_hegrenade.mdl" && !HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Explosive)) ||	// explosive grenade
					(entityModelName == "models/w_flashbang.mdl" && !HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Flashbang)) ||	// concussion grenade
					(entityModelName == "models/w_smokegrenade.mdl" && !HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Smoke))		// smoke grenade
				)
			)
			{
				allowPickup = true;

				currentPickupType = PickupType_Grenade;
			}
			else if (entityClassName == "weapon_shield" && !HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_Elite)) && !HasShield () && (!HasPrimaryWeapon () && RateGroundWeapon (entity)))	// found a shield on ground?
			{
				allowPickup = true;

				currentPickupType = PickupType_ShieldGun;
			}
			else if (entityClassName == "item_thighpack" && GetRealTeam () == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist && !IsHasDefuseKit ())	// defuse kit
			{
				allowPickup = true;

				currentPickupType = PickupType_DefuseKit;
			}
			else if (entity->IsC4 ())
			{
				allowPickup = true;

				currentPickupType = PickupType_PlantedC4;
			}
			else if (entityClassName == "func_button" && m_buttonUsageTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				allowPickup = true;

				currentPickupType = PickupType_Button;
			}
			else if
			(
				// check if the bot can use this item AND the recharger is ready to use AND has no enemies
				entity->variables.frame == 0.0f && m_buttonUsageTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && !(m_states & (State_SeeingEnemy | State_SuspectEnemy | State_HearingEnemy)) &&
				(
					(entityClassName == "func_healthcharger" && m_edict->variables.health < 100.0f) ||	// health charger
					(entityClassName == "func_recharge" && m_edict->variables.armorValue < 100.0f)	// HEV charger
				)
			)
			{
				allowPickup = true;

				currentPickupType = PickupType_Button;
			}
		}

		// if the bot found something it can pickup, see if it's the closest item so far...
		if (!allowPickup || (distance = entityOrigin.GetDistance (GetOrigin ())) >= minimumDistance)
			continue;

		// if condition valid
		if (allowPickup)
		{
			currentPickupItem = entity;			// remember this entity
			minimumDistance = distance;			// update the minimum distance
			currentPickupOrigin = entityOrigin;	// remember location of entity
			m_pickupType = currentPickupType;	// remember type of pickup

			continue;
		}

		// else...
		currentPickupItem = NULL;
		currentPickupType = PickupType_None;
	}	// end of the while loop

	if (currentPickupItem == NULL)
		return;

	const TeamArrayID_t realArrayTeamID (static_cast <TeamArrayID_t> (GetRealTeam () - 1u));

	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetTeamAliveClientsArray (realArrayTeamID).GetElementNumber (); ++index)
	{
		const Client *const client (g_server->GetClientManager ()->GetTeamAliveClientsArray (realArrayTeamID)[index]);

		if (minimumDistance < currentPickupOrigin.GetDistance (client->GetOrigin ()))
			continue;

		if (client->IsYaPB ())
		{
			if (client->GetYaPBPointer ()->m_pickupItem != currentPickupItem)
				continue;
		}
		else
		{
			if (m_pickupType == PickupType_Weapon)
			{
				/// @warning THIS IS WRONG currentPickupItem CAN BE weaponbox OR armoury_entity OR SOMETHING ELSE - OFFSETS ARE DIFFERENT!!!
				if (client->m_currentWeapon->GetID () == currentPickupItem->GetWeaponID () && m_currentWeapon->GetAmmo () >= currentPickupItem->GetAmmo ())
					continue;
			}
		}

		m_pickupItem = NULL;
		m_pickupType = PickupType_None;

		return;
	}

	// check if item is too high to reach, check if getting the item would hurt bot
	if (currentPickupOrigin.z > GetEyePosition ().z + 3.0f || IsDeadlyDrop (currentPickupOrigin))
	{
		m_pickupItem = NULL;
		m_pickupType = PickupType_None;

		return;
	}

	// save pointer of picking up entity
	m_pickupItem = currentPickupItem;
#endif	// if 0
}
#if 0
const Math::Vector3D YaPB::CheckToss (const Math::Vector3D &start, Math::Vector3D end) const
{
	// this function returns the velocity at which an object should looped from 'start' to land near 'end'. Returns null vector if toss is not feasible.

	end -= GetVelocity ();
	end.z -= 15.0f;

	if (!Math::AreEqual2 (end.z, start.z, 500.0f))
		return Math::Vector3D::ZeroValue;	// to high, fail

	// calculate the midpoint and apex of the 'triangle'
	// UNDONE: normalize any Z position differences between 'start' and 'end' so that triangle is always RIGHT

	Math::Vector3D midPoint (start.GetMidPoint (end));	// halfway point between 'start' and 'end'
	Math::Vector3D temp (midPoint);

	temp.z += 500.0f/*384.0f*/;
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <int> () >= 2 && g_clientManager->IsClientSlotUsed (g_hostEntity))
		if (g_hostEntity->variables.origin.GetDistanceSquared (GetOrigin ()) < MATH_GET_SQUARED (20.0f))
			g_hostEntity->DrawLine (midPoint, temp, 50u, 0u, Color <> (0u, 255u, 0u), 255u, 0u, 10u);
*/
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (midPoint, temp, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

	if (m_traceResult.fraction < 1.0f)
	{
		midPoint.x = m_traceResult.endPosition.x;
		midPoint.y = m_traceResult.endPosition.y;
		midPoint.z = m_traceResult.hitEntity->variables.absoluteBoundingBox.mins.z - 1.0f;
	}

	if (/*commented*/midPoint.z < start.z || midPoint.z < end.z)
		return Math::Vector3D::ZeroValue;	// to not enough space, fail

	const float gravity (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Gravity)->GetValue <float> () * 0.55f/*0.5f*/);
	const float timeOne (Math::sqrtf ((midPoint.z - start.z) / (0.5f * gravity)));	// calculate how high should the grenade travel to reach the apex and how long will it take for the grenade to travel this distance

	if (timeOne < Math::RawEpsilon)
		return Math::Vector3D::ZeroValue;	// too close

	const float timeTwo (Math::sqrtf ((midPoint.z - end.z) / (0.5f * gravity)));	// calculate how high should the grenade travel to reach the apex and how long will it take for the grenade to travel this distance
/*
	if (timeOne + timeTwo > 3.8f)
	{
//		AddLogEntry (true, LogLevel_Default, true, "[DEBUG] Bot \"%s\" - to far away to throw a nade - distance = %.2f.", GetName ().GetData (), GetOrigin ().GetDistance (end));

		return Math::Vector3D::ZeroValue;	// grenade is likely to explode in the sky.
	}
*/
	// how hard to throw sideways to get there in time
	Math::Vector3D grenadeVelocity ((end - start) / (timeOne + timeTwo));

	// how hard upwards to reach the apex at the right time
	grenadeVelocity.z = gravity * timeOne;

	// highest point
	Math::Vector3D apex (start + grenadeVelocity * timeOne);	// find the apex

	apex.z = midPoint.z;
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <int> () >= 2 && g_clientManager->IsClientSlotUsed (g_hostEntity))
		if (g_hostEntity->variables.origin.GetDistanceSquared (GetOrigin ()) < MATH_GET_SQUARED (20.0f))
			g_hostEntity->DrawLine (start, apex, 50u, 0u, Color <> (0u, 255u, 0u), 255u, 0u, 10u);
*/
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (start, apex, false, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

	if (m_traceResult.fraction < 1.0f || m_traceResult.isAllSolid)
		return Math::Vector3D::ZeroValue;	// fail!
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <int> () >= 2 && g_clientManager->IsClientSlotUsed (g_hostEntity))
		if (g_hostEntity->variables.origin.GetDistanceSquared (GetOrigin ()) < MATH_GET_SQUARED (20.0f))
			g_hostEntity->DrawLine (apex, end, 50u, 0u, Color <> (0u, 255u, 0u), 255u, 0u, 10u);
*/
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (apex, end, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

	// 60 degrees
	if (m_traceResult.fraction < 1.0f && (-(m_traceResult.planeNormal | (apex - end).GetNormalized ()) > 0.7f || m_traceResult.fraction < 0.8f))
		return Math::Vector3D::ZeroValue;

	return grenadeVelocity * 0.777f/*not exists*/;
}

const Math::Vector3D YaPB::CheckThrow (const Math::Vector3D &start, const Math::Vector3D end) const
{
	// this function returns the velocity vector at which an object should be thrown from start to hit end. Returns null vector if throw is not feasible.

	Math::Vector3D grenadeVelocity (end - start);
	float time (grenadeVelocity.GetLength () / 195.0f/*400.0f*/);	// throw at a constant time

	if (time < 0.01f/* || time > 4.0f*/)
		return Math::Vector3D::ZeroValue;	// too close - fail
	else if (time > 2.0f)	// not exists
		time = 1.2f;	// not exists

	const float gravity (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Gravity)->GetValue <float> () * 0.55f/*0.4f*/);
	Math::Vector3D apex (start.GetMidPoint (end));

	apex.z += 0.5f * gravity * (time * 0.5f) * (time * 0.5f)/*" * (time * 0.5f)" - commented*/;
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <int> () >= 2 && g_clientManager->IsClientSlotUsed (g_hostEntity))
		if (g_hostEntity->variables.origin.GetDistanceSquared (GetOrigin ()) < MATH_GET_SQUARED (20.0f))
			g_hostEntity->DrawLine (start, apex, 50u, 0u, Color <> (0u, 0u, 255u), 255u, 0u, 10u);
*/
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull/*TraceLine*/ (start, apex, false, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

	if (m_traceResult.fraction < 1.0f/* || m_traceResult.isAllSolid*/)
		return Math::Vector3D::ZeroValue;	// fail!
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <int> () >= 2 && g_clientManager->IsClientSlotUsed (g_hostEntity))
		if (g_hostEntity->variables.origin.GetDistanceSquared (GetOrigin ()) < MATH_GET_SQUARED (20.0f))
			g_hostEntity->DrawLine (apex, end, 50u, 0u, Color <> (0u, 0u, 255u), 255u, 0u, 10u);
*/
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull/*TraceLine*/ (apex, end, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, m_traceResult);

	// 60 degrees
	if ((m_traceResult.fraction < 1.0f || m_traceResult.isAllSolid) && (-(m_traceResult.planeNormal | (apex - end).GetNormalized ()) > 0.7f/*0.75f*/ || m_traceResult.fraction < 0.8f))
		return Math::Vector3D::ZeroValue;

	grenadeVelocity *= 1.0f / time;
	grenadeVelocity.z += gravity * time * 0.5f;	// adjust upward toss to compensate for gravity loss

	return grenadeVelocity * 0.7793f/*not exists*/;
}
#endif	// if 0
const bool YaPB::IsTemmateInLineOfFire (const float distance)
{
	static unsigned short clientIndex;

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (GetEyePosition (), GetEyeForwardPositionWithPunchAngles (distance), HalfLifeEngine::SDK::Constants::TraceIgnore_None, m_edict, m_traceResult);

	// check if we hit something
	if (m_traceResult.hitEntity->IsValid ())
	{
		// temporarily keep this slot of trace hit pointer
		clientIndex = m_traceResult.hitEntity->GetIndex () - 1u;

		// check valid range
		if (HalfLifeEngine::Utilities::IsNotWorldspawnPlayerEdictIndex (clientIndex))
		{
			const Client *const client (g_server->GetClientManager ()->GetClient (static_cast <unsigned char> (clientIndex)));

			if (client->IsAlive () && client->GetRealTeam () == GetRealTeam ())
				return true;
		}
	}
/*! @note NOT NEEDED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	static float friendDistance, squareDistance, coneMinimum;

	const TeamArrayID_t realArrayTeamID (GetRealArrayTeamID ());

	// search the world for players
	for (clientIndex = 0u; clientIndex < g_server->GetClientManager ()->GetTeamAliveClientsArray (realArrayTeamID).GetElementNumber (); ++clientIndex)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetTeamAliveClientsArray (realArrayTeamID)[static_cast <unsigned char> (clientIndex)]);

		if (client == this)
			continue;

		friendDistance = (client->GetOrigin ().GetDistance (GetOrigin ()));

		if (friendDistance > distance)
			continue;

		squareDistance = Math::sqrtf (MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::HumanWidth + 1.0f) + MATH_GET_SQUARED (friendDistance));
		coneMinimum = MATH_GET_SQUARED (friendDistance) / MATH_GET_SQUARED (squareDistance);

		if (GetShootingConeDeviation (client->GetOrigin ()) > coneMinimum)
			return true;
	}
*/
	return false;
}

const unsigned char YaPB::GetVisibleEnemiesNumber (void)
{
	// this function returns number of visible enemies by selected bot.

	unsigned char count (0u);

	// loop through all player slots
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (!client->IsAlive () || client->GetTeam () == GetTeam () || !IsPositionVisible (client->GetOrigin ()))
			continue;

		// increment count number
		++count;
	}

	return count;
}

const unsigned char YaPB::GetEnemiesNumber (void) const
{
	// this function returns number of enemies by selected bot.

	unsigned char count (0u);

	// loop through all player slots
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (!client->IsAlive () || client->GetTeam () == GetTeam ())
			continue;

		// increment count number
		++count;
	}

	return count;
}

const unsigned char YaPB::GetNearbyTeammatesNearPosition (const Math::Vector3D &origin, float radius) const
{
	if (!g_server->IsTeamplay ())
		return 0u;

	unsigned char count (0u);

	radius *= radius;

	// loop through all player slots
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (!client->IsAlive () || client == this || client->GetRealTeam () != GetRealTeam () || origin.GetDistanceSquared (client->GetOrigin ()) > radius)
			continue;

		// increment count number
		++count;
	}

	return count;
}

const unsigned char YaPB::GetNearbyEnemiesNearPosition (const Math::Vector3D &origin, float radius) const
{
	unsigned char count (0u);

	radius *= radius;

	// loop through all player slots
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (!client->IsAlive () || client->GetTeam () == GetTeam () || origin.GetDistanceSquared (client->GetOrigin ()) > radius)
			continue;

		// increment count number
		++count;
	}

	return count;
}

const bool YaPB::IsGroupOfEnemies (const Math::Vector3D &location, const unsigned char numberOfEnemies, float radius) const
{
	/// @todo Rewrite this function!!!

	unsigned char numberOfPlayers (0u);

	radius *= radius;

	// search the world for enemy players...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		if (!client->IsAlive () || client == this || client->GetOrigin ().GetDistanceSquared (location) > radius)
			continue;

		// don't target our teammates...
		if (client->GetTeam () == GetTeam ())
			return false;

		if (numberOfPlayers++ > numberOfEnemies)
			return true;
	}

	return false;
}

const bool YaPB::IsGroupOfEnemies (unsigned char numberOfEnemies) const
{
	// Reliability check.
	InternalAssert (numberOfEnemies > 0u);

	// search the world for enemy players...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		// don't target our teammates...
		if (!client->IsAlive () || client->GetTeam () == GetTeam ())
			continue;

		if (--numberOfEnemies == 0u)
			return true;
	}

	return false;
}

const bool YaPB::IsHasEnemies (void) const
{
	// search the world for enemy players...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

		// don't target our teammates...
		if (client->IsAlive () && client->GetTeam () != GetTeam ())
			return true;
	}

	return false;
}

Waypoint *const YaPB::FindGoalWaypoint (void)
{
	return NULL;
}

NavigationMesh::NavigationArea *const YaPB::FindGoalArea (void)
{
	if (g_server->GetNavigationMeshManager ().GetAreas ().IsEmpty ())
		return NULL;

	return g_server->GetNavigationMeshManager ().GetAreas ().GetRandomElement ();	/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

#if defined _DEBUG
	void YaPB::DebugMessage (const char *const message) const
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <unsigned char> () < 2u)
			return;

		const Host *const host (g_server->GetHumanManager ()->GetHostClient ());

		if (host == NULL || host->GetOrigin ().GetDistanceSquared (GetOrigin ()) > MATH_GET_SQUARED (30.0f))
			return;

		const DynamicString fullMessage (FormatBuffer ("[" PRODUCT_LOG_TAG " DEBUG] %s: %s", GetName ().GetData (), message));

		// print this message to the console
		host->Print (HalfLifeEngine::SDK::Constants::PrintType_AtChat, fullMessage);
		host->Print (HalfLifeEngine::SDK::Constants::PrintType_AtChat, "\n");

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <unsigned char> () > 2u)
			AddLogEntry (false, LogLevel_Default, false, fullMessage);	// also store it to the log file
	}
	void YaPB::DebugMessageFormat (const char *const format, ...) const
	{
		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <unsigned char> () < 2u)
			return;

		const Host *const host (g_server->GetHumanManager ()->GetHostClient ());

		if (host == NULL || host->GetOrigin ().GetDistanceSquared (GetOrigin ()) > MATH_GET_SQUARED (30.0f))
			return;

		char message[1024u];

		// Concatenate all the arguments in one message....
		{
			va_list argumentPointer;

			va_start (argumentPointer, format);
				vsnprintf (message, sizeof (message), FormatBuffer ("[" PRODUCT_LOG_TAG " DEBUG] %s: %s", GetName ().GetData (), format), argumentPointer);
			va_end (argumentPointer);
		}

		// print this message to the console
		host->Print (HalfLifeEngine::SDK::Constants::PrintType_AtChat, message);
		host->Print (HalfLifeEngine::SDK::Constants::PrintType_AtChat, "\n");

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Debug)->GetValue <unsigned char> () > 2u)
			AddLogEntry (false, LogLevel_Default, false, message);	// also store it to the log file
	}
#endif	// if defined _DEBUG
void YaPB::CheckForObstacles (void)
{
	// this function fills an integer bitmap describing the presence and quality of any
	// obstacle right around the bot. Useful for low walls over which the bot has to jump,
	// or for determining if the bot should duck to pass a low ceiling. This function is
	// called every frame, systematically, in BotSense(), so that the bot knows, when
	// it starts thinking, the quality of the terrain in front of it. First it checks if
	// it is about to hit something when walking forward, and if so, it checks if the bot's
	// look hits a wall when looking straight horizontally. If so, then the bot might be
	// able to duck over something to pass by ; if not, then the bot might be able to
	// jump over the obstacle ; we do the appropriate checks.

	// NOTE: player's origin is 37 units above the ground (standing height)
	//		 player is 36 units high when ducking
	//		 max stair height is 18 units
	//		 max jump height is 45 units
	//		 max jump height is 63 units when doing a duck-jump

	// NOTE(bis): TraceHull() fails when too close to obstacle but TraceLine() doesn't.
	//			  TraceLine() fails on func_illusionaries but TraceHull() doesn't. (If non-point tracing!)
	//			  Because of this strange behaviour we need to combine both:
	//			  IF either TraceLine() OR TraceHull() fails, there is an obstacle.

	HalfLifeEngine::SDK::Structures::TraceResult_t trl1, trl2, trl3, trl4, trl5;

	m_body.previousHitState = m_body.currentHitState;	// rotate the hit state
	m_body.currentHitState = Body_t::OBSTACLE_NONE;	// first off, reset the hit state bitmap...
	m_body.fallPlaneNormal = Math::Vector3D::ZeroValue;	// ...and the fall edge plane

	// given the bot's velocity, bot will check closer or further forward (range 32-120 units)
	const float checkDistance (0.4f * m_edict->GetSpeed2D ());
	const float angleDifferenceInRadians (Math::DegreeToRadian (GetVelocity ().ToYaw () - GetViewAngles ().yaw));
	float cosine, sine;

	Math::GetCosineSine (angleDifferenceInRadians, cosine, sine);

	// we gotta transpose this distance to both sides of the bot (and also forward)

	// left distance
	float leftCheckDistance (checkDistance * sine);

	if (leftCheckDistance < HalfLifeEngine::SDK::Constants::HumanWidth)
		leftCheckDistance = HalfLifeEngine::SDK::Constants::HumanWidth;	// bound it to 32 units minimum
	else if (leftCheckDistance > 120.0f)
		leftCheckDistance = 120.0f;	// and 120 units maximum

	// forward distance
	float frontCheckDistance (checkDistance * cosine);

	if (frontCheckDistance < HalfLifeEngine::SDK::Constants::HumanWidth/* depth */)
		frontCheckDistance = HalfLifeEngine::SDK::Constants::HumanWidth/* depth */;	// bound it to 32 units minimum
	else if (frontCheckDistance > 120.0f)
		frontCheckDistance = 120.0f;	// and 120 units maximum

	// right distance
	float rightCheckDistance (checkDistance * -sine);

	if (rightCheckDistance < HalfLifeEngine::SDK::Constants::HumanWidth)
		rightCheckDistance = HalfLifeEngine::SDK::Constants::HumanWidth;	// bound it to 32 units minimum
	else if (rightCheckDistance > 120.0f)
		rightCheckDistance = 120.0f;	// and 120 units maximum

	// and save them away
	m_body.leftCheckDistance = leftCheckDistance;
	m_body.frontCheckDistance = frontCheckDistance;
	m_body.rightCheckDistance = rightCheckDistance;

	GetViewAngles ().MakeVectors ();

	// now build the unary vectors
	Math::Vector3D left (-HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right.x, -HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->right.y, 0.0f);

	left.Normalize ();

	const Math::Vector3D right (-left);

	Math::Vector3D front (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.x, HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward.y, 0.0f);

	front.Normalize ();

	// get the bot's feet position
	const Math::Vector3D feet (GetOrigin ().x, GetOrigin ().y, m_edict->variables.absoluteBoundingBox.mins.z);	// take the origin... and lower it at the bottom of the bounding box

	#if defined _DEBUG
		const Host *const host (g_server->GetHumanManager ()->GetHostClient ());
		const bool isDebuggingBot (IsDebuggingBot ());

		// if body debug level is high, display this bot's bounding box as well as the trace rays
		if (isDebuggingBot && s_debugLevel.body > 2u)
			host->DrawBox (m_edict->variables.absoluteBoundingBox.mins, m_edict->variables.absoluteBoundingBox.maxs, Color <> (0u, 255u, 0u), 1u);
	#endif	// if defined _DEBUG

	///////////////////////
	// check on the left //
	///////////////////////

	const float multiplier (200.0f);
	Math::Vector3D source (feet + Math::Vector3D (0.0f, 0.0f, 18.0f + 17.0f)), destination (source + left * multiplier);

	// do a trace 17 units above max stair height left...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl1);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + left * leftCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + left * leftCheckDistance);

			if (source.GetDistanceSquared (trl1.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl1.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl1.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	source = feet + Math::Vector3D (0.0f, 0.0f, 64.0f);
	destination = source + left * multiplier;

	// do a trace one unit above max jump height left...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl2);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + left * leftCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + left * leftCheckDistance);

			if (source.GetDistanceSquared (trl2.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl2.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl2.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	source = feet + Math::Vector3D (0.0f, 0.0f, 17.0f);
	destination = source + left * multiplier;

	// do a trace one unit lower than max stair height left...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, source + left * multiplier, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl3);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + left * leftCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + left * leftCheckDistance);

			if (source.GetDistanceSquared (trl3.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl3.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl3.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	// is there something in the way at feet level that is not a slope
	// AND nothing in the way at eye level?
	if (multiplier * trl3.fraction < leftCheckDistance && trl3.planeNormal.z < 0.5f && multiplier * trl2.fraction >= leftCheckDistance)
	{
		// if bot is standing on the ground, OR obstacle WAS jumpable when bot was last on ground
		if (IsOnFloor ())
			m_body.currentHitState |= Body_t::OBSTACLE_LEFT_LOWWALL;	// then it's a low wall
		else if (m_body.previousHitState & Body_t::OBSTACLE_LEFT_LOWWALL)
			m_body.currentHitState |= Body_t::OBSTACLE_LEFT_LOWWALL;	// then it's a low wall
		else
			m_body.currentHitState |= Body_t::OBSTACLE_LEFT_WALL;	// else it's still a wall
	}

	// is there something in the way at eye level AND nothing in the way at knee level
	// AND the bot is standing on the ground?
	if (multiplier * trl1.fraction < leftCheckDistance && multiplier * trl3.fraction >= leftCheckDistance && IsOnFloor ())
		m_body.currentHitState |= Body_t::OBSTACLE_LEFT_LOWCEILING;	// bot can duck under this obstacle

	// is there something in the way at eye level AND something in the way at knee level
	// OR something in the way at eye level that is an unclimbable slope?
	if ((multiplier * trl2.fraction < leftCheckDistance && multiplier * trl1.fraction < leftCheckDistance) || (multiplier * trl2.fraction >= 60.0f && trl2.planeNormal.z > 0.0f && trl2.planeNormal.z < 0.5f))
		m_body.currentHitState |= Body_t::OBSTACLE_LEFT_WALL;	// bot will definitely hit something

	// if the area is clear on the left at head level, trace down to check for a possible fall
	if (multiplier * trl2.fraction >= leftCheckDistance)
	{
		source = feet + Math::Vector3D (0.0f, 0.0f, 64.0f) + left * leftCheckDistance;
		destination = source - Math::Vector3D (0.0f, 0.0f, 250.0f);

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, trl4);

		#if defined _DEBUG
			if (isDebuggingBot && s_debugLevel.body > 1u)
			{
//				host->DrawLine (source, source + Math::Vector3D (0.0f, 0.0f, -250.0f) * trl4.fraction, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

				const Math::Vector3D endPosition (source + Math::Vector3D (0.0f, 0.0f, -250.0f) * trl4.fraction);

				if (source.GetDistanceSquared (trl4.endPosition) >= source.GetDistanceSquared (endPosition))
					host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
				else
				{
					host->DrawLine (source, trl4.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
					host->DrawLine (trl4.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
				}
			}
		#endif	// if defined _DEBUG

		// did the trace hit nothing OR some water?
		if (trl4.fraction == 1.0f || HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (trl4.endPosition) == HalfLifeEngine::SDK::Constants::Content_Water)
			m_body.currentHitState |= Body_t::OBSTACLE_LEFT_FALL;	// bot can fall on the left
	}

	////////////////////
	// check in front //
	////////////////////

	source = feet + Math::Vector3D (0.0f, 0.0f, 18.0f + 17.0f);
	destination = source + front * multiplier;

	// do a trace 17 units above max stair height forward...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl1);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + front * frontCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + front * frontCheckDistance);

			if (source.GetDistanceSquared (trl1.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl1.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl1.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	source = feet + Math::Vector3D (0.0f, 0.0f, 64.0f);
	destination = source + front * multiplier;

	// do a trace one unit above max jump height forward...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl2);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + front * frontCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + front * frontCheckDistance);

			if (source.GetDistanceSquared (trl2.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl2.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl2.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	source = feet + Math::Vector3D (0.0f, 0.0f, 17.0f);
	destination = source + front * multiplier;

	// do a trace one unit lower than max stair height forward...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl3);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + front * frontCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + front * frontCheckDistance);

			if (source.GetDistanceSquared (trl3.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl3.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl3.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	// is there something in the way at feet level that is not a slope
	// AND nothing in the way at eye level?
	if (multiplier * trl3.fraction < frontCheckDistance && trl3.planeNormal.z < 0.5f && multiplier * trl2.fraction >= frontCheckDistance)
	{
		// if bot is standing on the ground, OR obstacle WAS jumpable when bot was last on ground
		if (IsOnFloor ())
			m_body.currentHitState |= Body_t::OBSTACLE_FRONT_LOWWALL;	// then it's a low wall
		else if (m_body.previousHitState & Body_t::OBSTACLE_FRONT_LOWWALL)
			m_body.currentHitState |= Body_t::OBSTACLE_FRONT_LOWWALL;	// then it's a low wall
		else
			m_body.currentHitState |= Body_t::OBSTACLE_FRONT_WALL;	// else it's still a wall
	}

	// is there something in the way at eye level AND nothing in the way at knee level
	// AND the bot is standing on the ground?
	if (multiplier * trl1.fraction < frontCheckDistance && multiplier * trl3.fraction >= frontCheckDistance && (IsOnFloor ()))
		m_body.currentHitState |= Body_t::OBSTACLE_FRONT_LOWCEILING;	// bot can duck under this obstacle

	// is there something in the way at eye level AND something in the way at knee level?
	// OR something in the way at eye level that is an unclimbable slope?
	if ((multiplier * trl2.fraction < frontCheckDistance && trl1.fraction * multiplier < frontCheckDistance) || (multiplier * trl2.fraction >= 60.0f && trl2.planeNormal.z > 0.0f && trl2.planeNormal.z < 0.5f))
		m_body.currentHitState |= Body_t::OBSTACLE_FRONT_WALL;	// bot will definitely hit something

	// if the area is clear in front at head level, trace down to check for a possible fall
	if (multiplier * trl2.fraction >= frontCheckDistance)
	{
		source = feet + Math::Vector3D (0.0f, 0.0f, 64.0f) + front * frontCheckDistance;
		destination = source - Math::Vector3D (0.0f, 0.0f, 250.0f);

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, trl4);

		#if defined _DEBUG
			if (isDebuggingBot && s_debugLevel.body > 1u)
			{
//				host->DrawLine (source, source + Math::Vector3D (0.0f, 0.0f, -250.0f) * trl4.fraction, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

				const Math::Vector3D endPosition (source + Math::Vector3D (0.0f, 0.0f, -250.0f) * trl4.fraction);

				if (source.GetDistanceSquared (trl4.endPosition) >= source.GetDistanceSquared (endPosition))
					host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
				else
				{
					host->DrawLine (source, trl4.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
					host->DrawLine (trl4.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
				}
			}
		#endif	// if defined _DEBUG

		// did the trace hit nothing OR some water?
		if (trl4.fraction == 1.0f || HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (trl4.endPosition) == HalfLifeEngine::SDK::Constants::Content_Water)
		{
			m_body.currentHitState |= Body_t::OBSTACLE_FRONT_FALL;	// bot can fall in front
//			pPlayer->Bot.reach_time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.5f;	// reflex: don't reach point for 1.5 second

			// trace backwards in front of the bot 17 units down to find the edge plane
			HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (feet - Math::Vector3D (0.0f, 0.0f, 10.0f) + front * frontCheckDistance, feet - Math::Vector3D (0.0f, 0.0f, 10.0f) + front * frontCheckDistance + -front * frontCheckDistance * 2.0f, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl5);

			// did the trace hit something?
			if (trl5.fraction < 1.0f)
				m_body.fallPlaneNormal = trl5.planeNormal;	// if so, then we found the edge plane
/*			else
			{
				// Houston, we have a problem. The bot is about to fall but we did NOT found the
				// edge plane. Make it jump as a reflex to reach the opposite side (if any)

				pPlayer->Bot.BotLegs.forward_time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;	// run forward
				pPlayer->Bot.BotLegs.jump_time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.3f;	// banzaiii...
			}*/
		}
	}

	////////////////////////
	// check on the right //
	////////////////////////

	source = feet + Math::Vector3D (0.0f, 0.0f, 18.0f + 17.0f);
	destination = source + right * multiplier;

	// do a trace 17 units above max stair height right...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl1);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + right * rightCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + right * rightCheckDistance);

			if (source.GetDistanceSquared (trl1.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl1.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl1.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	source = feet + Math::Vector3D (0.0f, 0.0f, 64.0f);
	destination = source + right * multiplier;

	// do a trace one unit above max jump height right...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl2);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + right * rightCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + right * rightCheckDistance);

			if (source.GetDistanceSquared (trl2.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl2.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl2.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	source = feet + Math::Vector3D (0.0f, 0.0f, 17.0f);
	destination = source + right * multiplier;

	// do a trace one unit lower than max stair height right...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, trl3);

	#if defined _DEBUG
		if (isDebuggingBot && s_debugLevel.body > 1u)
		{
//			host->DrawLine (source, source + right * rightCheckDistance, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

			const Math::Vector3D endPosition (source + right * rightCheckDistance);

			if (source.GetDistanceSquared (trl3.endPosition) >= source.GetDistanceSquared (endPosition))
				host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			else
			{
				host->DrawLine (source, trl3.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
				host->DrawLine (trl3.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
			}
		}
	#endif	// if defined _DEBUG

	// is there something in the way at feet level that is not a slope
	// AND nothing in the way at eye level?
	if (multiplier * trl3.fraction < rightCheckDistance && trl3.planeNormal.z < 0.5f && multiplier * trl2.fraction >= rightCheckDistance)
	{
		// if bot is standing on the ground, OR obstacle WAS jumpable when bot was last on ground
		if (IsOnFloor ())
			m_body.currentHitState |= Body_t::OBSTACLE_RIGHT_LOWWALL;	// then it's a low wall
		else if (m_body.previousHitState & Body_t::OBSTACLE_RIGHT_LOWWALL)
			m_body.currentHitState |= Body_t::OBSTACLE_RIGHT_LOWWALL;	// then it's a low wall
		else
			m_body.currentHitState |= Body_t::OBSTACLE_RIGHT_WALL;	// else it's still a wall
	}

	// is there something in the way at eye level AND nothing in the way at knee level
	// AND the bot is standing on the ground?
	if (multiplier * trl1.fraction < rightCheckDistance && multiplier * trl3.fraction >= rightCheckDistance && IsOnFloor ())
		m_body.currentHitState |= Body_t::OBSTACLE_RIGHT_LOWCEILING;	// bot can duck under this obstacle

	// is there something in the way at eye level AND something in the way at knee level?
	// OR something in the way at eye level that is an unclimbable slope?
	if ((multiplier * trl2.fraction < rightCheckDistance && multiplier * trl1.fraction < rightCheckDistance) || (multiplier * trl2.fraction >= 60.0f && trl2.planeNormal.z > 0.0f && trl2.planeNormal.z < 0.5f))
		m_body.currentHitState |= Body_t::OBSTACLE_RIGHT_WALL;	// bot will definitely hit something

	// if the area is clear on the right at head level, trace down to check for a possible fall
	if (multiplier * trl2.fraction >= rightCheckDistance)
	{
		source = feet + Math::Vector3D (0.0f, 0.0f, 64.0f) + right * rightCheckDistance;
		destination = source - Math::Vector3D (0.0f, 0.0f, 250.0f);

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, trl4);

		#if defined _DEBUG
			if (isDebuggingBot && s_debugLevel.body > 1u)
			{
//				host->DrawLine (source, source + Math::Vector3D (0.0f, 0.0f, -250.0f) * trl4.fraction, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

				const Math::Vector3D endPosition (source + Math::Vector3D (0.0f, 0.0f, -250.0f) * trl4.fraction);

				if (source.GetDistanceSquared (trl4.endPosition) >= source.GetDistanceSquared (endPosition))
					host->DrawLine (source, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
				else
				{
					host->DrawLine (source, trl4.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 255u, 0u), 250u, 5u, 1u);
					host->DrawLine (trl4.endPosition, endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
				}
			}
		#endif	// if defined _DEBUG

		// did the trace hit nothing OR some water?
		if (trl4.fraction == 1.0f || HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (trl4.endPosition) == HalfLifeEngine::SDK::Constants::Content_Water)
			m_body.currentHitState |= Body_t::OBSTACLE_RIGHT_FALL;	// bot can fall on the right
	}

	// and finally, see if we are falling or not
	if (GetVelocity ().z > -HalfLifeEngine::SDK::Constants::MaximumPlayerSafeFallSpeed)
		m_body.fallTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();	// save bot fall time

	// finished, surrounding obstacles bitmap is filled

	#if defined _DEBUG
		if (!isDebuggingBot || s_debugLevel.body <= 1u)
			return;

		DynamicString senses;

		if (m_body.currentHitState == Body_t::OBSTACLE_NONE)
			senses = "NONE";
		else
		{
			const DynamicString types[] =
			{
				"FRONT_LOWWALL",
				"FRONT_LOWCEILING",
				"FRONT_WALL",
				"FRONT_FALL",

				"LEFT_LOWWALL",
				"LEFT_LOWCEILING",
				"LEFT_WALL",
				"LEFT_FALL",

				"RIGHT_LOWWALL",
				"RIGHT_LOWCEILING",
				"RIGHT_WALL",
				"RIGHT_FALL"
			};

			STORE_FLAGS_TO_STRING (m_body.currentHitState, senses, 0u, GET_STATIC_ARRAY_SIZE (types), types, "+");
		}

		host->HudMessageFormat
		(
			Math::Vector2D (0.0f, 0.14f),
			ColorWithAlpha <> (255u, 0u, 0u),
			ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
			HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, g_server->GetYaPBManager ()->GetDebugBotUpdateDelay (), 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

			"Senses: %s",

			senses.GetData ()
		);
	#endif	// if defined _DEBUG
}

void YaPB::AvoidPlayers (void)
{
	// this function is mostly a reflex action; its purpose is to make the bot react motionally
	// by strafing to any teammate around. The bot cycles through all players and determines if a
	// player is visible and nearby, and if so, it performs the necessary movement adjustments in
	// order to avoid colliding into him.

	if (m_avoidPlayersTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		return;	// cancel if not time to

	float playerAngle;
	float playerDistanceSquared;

	m_avoidPlayersTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.2f;	// think about it again in 200 milliseconds

	// search the world for players...
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		// temporarily keep this slot
		const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);	// quick access to player

		if (client == this || !client->IsAlive ())
			continue;	// skip invalid players and skip self (i.e. this bot)
/*
		if (DebugLevel.is_observer && !client->is_racc_bot)
			continue;	// skip real players in observer mode

		if (client->GetEdict () == pPlayer->Bot.BotEnemy.pEdict)
			continue;	// don't mind about enemies...
*/
		// see if bot can see the teammate...
		if (!IsInViewCone (client->GetOrigin ()) || !IsPositionVisible (client->GetOrigin ()))
			continue;

		// bot found a visible teammate

		playerAngle = client->GetVelocity ().AsVector2D ().GetAngleBetween (GetVelocity ());	// Ignore Z component.
		playerDistanceSquared = GetOrigin ().GetDistanceSquared (client->GetOrigin ());

		// is that teammate near us OR coming in front of us and within a certain distance?
		if (playerDistanceSquared < MATH_GET_SQUARED (100.0f) || (playerDistanceSquared < MATH_GET_SQUARED (300.0f) && Math::fabsf (playerAngle) > 160.0f && Math::fabsf (Math::AngleNormalize (client->GetViewAngles ().yaw - GetViewAngles ().yaw)) > 165.0f))
		{
			// if we are moving full speed AND there's room forward OR teammate is very close...
			if (playerDistanceSquared < MATH_GET_SQUARED (70.0f) || (!(m_body.currentHitState & Body_t::OBSTACLE_FRONT_WALL) && m_edict->GetSpeed2D () > 50.0f))
			{
				// is the player coming on the left AND is it safe for the bot to strafe right?
				if (playerAngle > 0.0f)
				{
					if (!(m_body.currentHitState & Body_t::OBSTACLE_RIGHT_FALL))
						m_navigator->StrafeRight ();	// strafe right to avoid him
				}
				// else is the player coming on the right AND is it safe for the bot to strafe left?
				else if (playerAngle < 0.0f && !(m_body.currentHitState & Body_t::OBSTACLE_LEFT_FALL))
					m_navigator->StrafeLeft ();	// strafe left to avoid him
			}
		}
	}

	// enough avoiding teammates
}

const bool YaPB::BendLineOfSight (const Math::Vector3D &destination, Math::Vector3D &bendDestination, const float maximumAngle/* = 135.0f*/)
{
	const Math::Vector3D &source (GetOrigin ());
	const Math::Vector3D modifiedDestination (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight));

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, modifiedDestination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, m_traceResult);

	if (m_traceResult.fraction == 1.0f && !m_traceResult.isStartSolid)
	{
		bendDestination = destination;

		return true;
	}

	if (maximumAngle < 5.0f)
	{
		Label_AssignSimpleDestinationAndReturnFalse:
			bendDestination = destination;

			return false;
	}

	const Math::Vector2D direction (destination.AsVector2D () - source.AsVector2D ());
	const float directionLength (direction.GetLength ());
	const float directionYaw (direction.ToYaw ());
	const float increaseAngleOffset (10.0f);
	float angleOffset (increaseAngleOffset);
	unsigned char state;
	Math::Vector2D offsetDirection;
	Math::Vector3D newDestination;

	do
	{
		state = 0u;

		do
		{
			const float degrees (state > 0u ? angleOffset + directionYaw : directionYaw - angleOffset);
			const float radians (Math::DegreeToRadian (degrees));

			Math::GetCosineSine (radians, offsetDirection.x, offsetDirection.y);

			newDestination = source.AsVector2D () + offsetDirection * directionLength;
			newDestination.z = destination.z;

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, newDestination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, m_traceResult);

			if (m_traceResult.isStartSolid)
				continue;

			const Math::Vector3D delta (newDestination - source);
			const float deltaLength (delta.GetLength ());
			const Math::Vector3D deltaNormalized (delta / deltaLength);
			const float maximumLength (deltaLength * m_traceResult.fraction);
			bool done (false);
			const float appendLength (50.0f);

			if (maximumLength < appendLength)
				continue;

			float currentLength (appendLength);

			for (;;)
			{
				newDestination = source + deltaNormalized * currentLength;

				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (newDestination, modifiedDestination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, m_traceResult);

				if (m_traceResult.fraction == 1.0f && !m_traceResult.isStartSolid)
					break;

				if ((currentLength += appendLength) > maximumLength)
				{
					done = true;

					break;
				}
			}

			if (done)
				continue;

			if (!NavigationMesh::GetGroundHeight (newDestination, &newDestination.z))
				newDestination.z = destination.z;

			bendDestination = newDestination;

			return true;
		} while (++state < 2u);
	} while ((angleOffset += increaseAngleOffset) <= maximumAngle);

	goto Label_AssignSimpleDestinationAndReturnFalse;
}

void YaPB::ComputeApproachPoints (void)
{
	RemoveApproachPoints ();

	if (m_lastNavigationArea == NULL)
		return;

	Math::Vector3D point, blendPoint;

	for (NavigationMesh::NavigationArea::ApproachArray::IndexType_t index (0u); index < m_lastNavigationArea->GetApproachInfoCount (); ++index)
	{
		const NavigationMesh::NavigationArea::ApproachInfo_t &info (m_lastNavigationArea->GetApproachInfo (index));

		if (info.here == NULL || info.prev == NULL)
			continue;

		if (info.prevToHereHow > NavigationMesh::GO_WEST)
			point = info.here->GetCenter ();
		else
		{
			info.prev->ComputePortal (info.here, static_cast <NavigationMesh::NavDirType_t> (info.prevToHereHow), point);

			point.z = info.here->GetZ (point);
		}

		// DELETE ME!!!!
//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (point, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 0u, 255u), 1u);

		if (!BendLineOfSight (point, blendPoint))
//		void *const address (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ()) + 0x324B0u));
//		typedef bool (HalfLifeEngine::SDK::Classes::CSBot::*BendLineOfSightFunction_t) (const Math::Vector3D &source, const Math::Vector3D &destination, Math::Vector3D &result);
//		if (!(m_edict->privateData->GetCSBotPointer ()->*reinterpret_cast <BendLineOfSightFunction_t &> (address)) (GetOrigin (), point, blendPoint))
			continue;

		// DELETE ME!!!!
//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (blendPoint, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

		m_approachPoints += blendPoint;
	}
}

void YaPB::MoveTowardsPosition (const Math::Vector3D &position)
{
	// Look at next path point
	{
		if (m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Vector)
			m_aim->SetLookTask (new Aim::Task_Vector (position));
		else
			static_cast <Aim::Task_Vector *const> (m_aim->GetLookTask ())->SetVector (position);
	}

	m_navigator->MoveForward ();

	/// @todo ................
#if 0
	/// @note FROM ZBOT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if ((m_lastNavigationArea == NULL || m_lastNavigationArea->HasAttributes (NAV_MESH_NO_JUMP)) && !IsOnLadder ()/* && !m_isUNKNOWN0*/)
	{
		// ....
	}

	const float yawAngle (GetViewAngles ().yaw);
	const float radians (Math::DegreeToRadian (yawAngle));	// degrees to radians
	float cosine, sine;
	const Math::Vector2D direction (position.AsVector2D () - GetOrigin ().AsVector2D ());
	const Math::Vector2D directionNormalized (direction.Normalize ());

	Math::GetCosineSine (radians, cosine, sine);

	const BaseNavigator::Speed_t speed
	(
		cosine * directionNormalized.x + sine * directionNormalized.y,
		cosine * directionNormalized.y + -sine * directionNormalized.x
	);

	if (speed.move > 0.25f)
		MoveForward ();
	else if (speed.move < -0.25f)
		MoveBackward ();

	if (!m_touchPlayerEntity)
	{
		if (speed.strafe >= 0.25f)
			StrafeLeft ();
		else if (speed.strafe <= -0.25f)
			StrafeRight ();
	}
#endif	// if 0
}