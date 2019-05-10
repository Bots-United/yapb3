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
// WaypointVisibilityTable.cpp
//
// Class: WaypointVisibilityTable
//
// Version: $ID:$
//

#include <Core.hpp>

const DynamicString WaypointVisibilityTable::HeaderFileExtension = "YaPB_WaypointVisibilityTable";

void WaypointVisibilityTable::Save (void) const
{
	// saves the visibility table (after recalculating the visibility - when *.pwf file was changed)

	if (g_server->GetWaypointManager ().GetWaypointsNumber () == 0u)
		return;

	if (m_visibilityTable == NULL || m_createVisibilityTable)
	{
		AddLogEntry (false, LogLevel_Critical, false, "Can't save visiblity table - bad data.");

		return;
	}

	const FileHeader_t &header (g_server->GetWaypointManager ().GetWaypointsNumber ());

	AddLogEntry (true, LogLevel_Default, false, "Compressing and saving visibility table... this may take a while!");

	if (Compressor::Compress (g_server->GetWaypointManager ().GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".vis", static_cast <const unsigned char *> (static_cast <const void *> (&header)), sizeof (FileHeader_t), reinterpret_cast <const unsigned char *> (m_visibilityTable), MaximumWaypoints * (MaximumWaypoints / 4u)/* * sizeof (unsigned char)*/) == -1)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't save visibility table!");

		return;
	}

	AddLogEntry (true, LogLevel_Default, false, "Visibility table successfully saved...");
}

void WaypointVisibilityTable::Load (void)
{
	// initiates the visibility table (at the map start)

	if (g_server->GetWaypointManager ().GetWaypointsNumber () == 0u)
		return;

	STDIOFile file (g_server->GetWaypointManager ().GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".vis", "rb");

	if (!file.IsValid ())
	{
		AddLogEntry (true, LogLevel_Default, false, "Visiblity table, not exists, visiblity table will be rebuilded.");

		m_createVisibilityTable = true;

		return;
	}

	FileHeader_t header;

	// if file exists, read the visibility table from it....
	if (!file.Read (header))
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.vis - has wrong or unsupported format, visiblity table will be rebuilded.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ());

		m_createVisibilityTable = true;

		return;
	}

	if (header.headerExtension != HeaderFileExtension)
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.vis is not a visiblity table file (header found '%s' needed '%s'), visiblity table will be rebuilded.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData (), header.headerExtension.GetData (), HeaderFileExtension.GetData ());

		m_createVisibilityTable = true;

		return;
	}

	if (header.fileVersion != HeaderFileVersion)
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.vis - incorrect visiblity table file version (expected '%u' found '%u'), visiblity table will be rebuilded.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData (), HeaderFileVersion, header.fileVersion);

		m_createVisibilityTable = true;

		return;
	}

	if (header.pointNumber != g_server->GetWaypointManager ().GetWaypointsNumber ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Visiblity table damaged (not for this map), visiblity table will be rebuilded.");

		m_createVisibilityTable = true;

		return;
	}

	AddLogEntry (true, LogLevel_Default, false, "Loading and decompressing visibility table...");

	if (Compressor::Uncompress (g_server->GetWaypointManager ().GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".vis", sizeof (FileHeader_t), reinterpret_cast <unsigned char *> (m_visibilityTable), MaximumWaypoints * (MaximumWaypoints / 4u)/* * sizeof (unsigned char)*/) == -1)
	{
		AddLogEntry (true, LogLevel_Error, false, "Failed to decode visiblity table, visiblity table will be rebuilded.");

		m_createVisibilityTable = true;

		return;
	}

	m_createVisibilityTable = false;

	AddLogEntry (true, LogLevel_Default, false, "Visibility table successfully loaded from file.");
}

void WaypointVisibilityTable::InitializeVisibility (void)
{
	// This function calculates the waypoint visibility table.

	if (!m_createVisibilityTable)
		return;

	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	unsigned char res, shift;
	Math::Vector3D sourceDuck, sourceStand, destination;
	unsigned short standCount, crouchCount;

	// Initialize the visibility table....
	memset (m_visibilityTable, 0u, sizeof (m_visibilityTable));

	for (unsigned short visibilityWaypointIndex = 0u, index; visibilityWaypointIndex < g_server->GetWaypointManager ().GetWaypointsNumber (); ++visibilityWaypointIndex)
	{
		Waypoint *const visibilityWaypoint = g_server->GetWaypointManager ().GetWaypoint (visibilityWaypointIndex);

		sourceDuck = sourceStand = visibilityWaypoint->m_origin;
		standCount = crouchCount = 0u;

		sourceDuck.z += ((visibilityWaypoint->m_flags & Waypoint::Flag_Crouch) ? 12.0f : -18.0f + 12.0f);
		sourceStand.z += ((visibilityWaypoint->m_flags & Waypoint::Flag_Crouch) ? 18.0f + 28.0f : 28.0f);

		for (index = 0u; index < g_server->GetWaypointManager ().GetWaypointsNumber (); ++index)
		{
			// first check ducked visibility
			destination = g_server->GetWaypointManager ().GetWaypoint (index)->m_origin;

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (sourceDuck, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

			// check if line of sight to object is not blocked (i.e. visible)
/*			res = traceResult.fraction < 1.0f || traceResult.isStartSolid ? 1u : 0u;

			res <<= 1u;
*/
			res = (traceResult.fraction < 1.0f || traceResult.isStartSolid) << 1u;

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (sourceStand, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

			// check if line of sight to object is not blocked (i.e. visible)
			if (traceResult.fraction < 1.0f || traceResult.isStartSolid)
				res |= 1u;

			shift = (index % 4u) << 1u;

			m_visibilityTable[visibilityWaypointIndex][index >> 2u] &= ~(3u << shift);
			m_visibilityTable[visibilityWaypointIndex][index >> 2u] |= res << shift;

			if (!(res & 1u))
				++standCount;

			if (!(res & 2u))
				++crouchCount;
		}

		visibilityWaypoint->m_visibility.stand = standCount;
		visibilityWaypoint->m_visibility.crouch = crouchCount;

		AddLogEntry (true, LogLevel_Default, false, "Visibility table %u percent complete.", (visibilityWaypointIndex * 100u) / g_server->GetWaypointManager ().GetWaypointsNumber ());
	}

	m_createVisibilityTable = false;
}