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
// WaypointDistancesTable.cpp
//
// Class: WaypointDistancesTable
//
// Version: $ID:$
//

#include <Core.hpp>

const DynamicString WaypointDistancesTable::HeaderFileExtension = "YaPB_WaypointDistancesTable";
/*
void WaypointDistancesTable::Save (void) const
{
	STDIOFile file (g_server->GetWaypointManager ().GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".pmt", "wb");

	// unable to open file
	if (!file.IsValid ())
	{
		AddLogEntry (false, LogLevel_Critical, false, "Failed to open file for writing.");

		return;
	}

	const unsigned short waypointNumber (g_server->GetWaypointManager ().GetWaypointsNumber ());

	// write number of waypoints
	file.Write (waypointNumber);

	// write path & distance matrix
	file.Write (m_distancesMatrix, sizeof (m_distancesMatrix), MATH_GET_SQUARED (waypointNumber));
}

const bool WaypointDistancesTable::Load (void)
{
	STDIOFile file (g_server->GetWaypointManager ().GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".pmt", "rb");

	// file doesn't exists return false
	if (!file.IsValid ())
		return false;

	unsigned short num (0u);

	// read number of waypoints
	file.Read (num);

	const unsigned short waypointNumber (g_server->GetWaypointManager ().GetWaypointsNumber ());

	if (num != waypointNumber)
	{
		AddLogEntry (true, LogLevel_Default, false, "Wrong number of points (pmt: %i/cur: %i). Matrix will be rebuilded.", num, g_server->GetWaypointManager ().GetWaypointsNumber ());

		return false;
	}

	// read path & distance matrixes
	file.Read (m_distancesMatrix, sizeof (m_distancesMatrix), MATH_GET_SQUARED (waypointNumber));

	return true;
}*/