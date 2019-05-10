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
// WaypointLocationsTable.cpp
//
// Class: WaypointLocationsTable
//
// Description:
//	Hash table of waypoint indexes accross certian
//	buckets in the map on X/Y Co-ords for quicker
//	nearest waypoint finding and waypoint displaying.
//
// Version: $ID:$
//

#include <Core.hpp>

unsigned char WaypointLocationsTable::g_failedWaypoints[MaximumWaypoints];

unsigned char *const WaypointLocationsTable::ResetFailedWaypoints (const DynamicArray <unsigned short> *const iIgnoreWpts)
{
	unsigned short index;

	for (index = 0u; index < MaximumWaypoints; ++index)
		g_failedWaypoints[index] = 0u;

	if (iIgnoreWpts != NULL)
	{
		unsigned short waypointIndex;

		for (index = 0u; index < iIgnoreWpts->GetElementNumber (); ++index)
			if ((waypointIndex = (*iIgnoreWpts)[index]) != InvalidWaypointIndex)
				g_failedWaypoints[waypointIndex] = 1u;
	}

	return g_failedWaypoints;
}

void WaypointLocationsTable::AutoPath (HalfLifeEngine::SDK::Classes::Edict *const pPlayer, const unsigned short iWpt)
{
	// returns nearest waypoint that can be used to cover from 'vCoverFrom' vector

	const Math::Vector3D &origin (g_server->GetWaypointManager ().GetWaypoint (iWpt)->GetOrigin ());
	unsigned char minLocationX, maxLocationX, minLocationY, maxLocationY, minLocationZ, maxLocationZ;

	GetMinsMaxs (origin, minLocationX, minLocationY, minLocationZ, maxLocationX, maxLocationY, maxLocationZ);

	for (unsigned char locationY, locationZ; minLocationX <= maxLocationX; ++minLocationX)
		for (locationY = minLocationY; locationY <= maxLocationY; ++locationY)
			for (locationZ = minLocationZ; locationZ <= maxLocationZ; ++locationZ)
				AutoPathInBucket (pPlayer, minLocationX, locationY, locationZ, iWpt);	// check each area around the current area for closer waypoints
}

void WaypointLocationsTable::GetAllVisible (const Math::Vector3D &vVisibleFrom, const Math::Vector3D &origin, DynamicArray <unsigned short> &iVisible)
{
	// Note: In this function we don't erase the 'iVisible' array!

	const unsigned short iFrom (GetNearestWaypoint (vVisibleFrom, MaximumReachableRange));

	if (iFrom == InvalidWaypointIndex)
		return;

	unsigned char minLocationX, maxLocationX, minLocationY, maxLocationY, minLocationZ, maxLocationZ;
	const WaypointVisibilityTable &visiblityTable (g_server->GetWaypointManager ().GetVisibilityTable ());

	GetMinsMaxs (origin, minLocationX, minLocationY, minLocationZ, maxLocationX, maxLocationY, maxLocationZ);

	for (unsigned char locationY, locationZ, index; minLocationX <= maxLocationX; ++minLocationX)
		for (locationY = minLocationY; locationY <= maxLocationY; ++locationY)
			for (locationZ = minLocationZ; locationZ <= maxLocationZ; ++locationZ)
				for (index = 0u; index < m_locations[minLocationX][locationY][locationZ].GetElementNumber (); ++index)
				{
					const unsigned short iWpt (m_locations[minLocationX][locationY][locationZ][index]);

					if (visiblityTable.IsVisible (iFrom, iWpt))
						iVisible += iWpt;
				}
}

void WaypointLocationsTable::AutoPathInBucket (HalfLifeEngine::SDK::Classes::Edict *const/* pPlayer*/, const unsigned char/* locationX*/, const unsigned char/* locationY*/, const unsigned char/* locationZ*/, const unsigned short/* iWptFrom*/)
{
/*	Waypoint *pOtherWpt;
	Waypoint *const pWpt (g_server->GetWaypointManager ().GetWaypoint (iWptFrom));
	Math::Vector3D vWptOrigin (pWpt->GetOrigin ());
	Math::Vector3D vOtherWptOrigin;
	const WaypointVisibilityTable &visiblityTable (g_server->GetWaypointManager ().GetVisibilityTable ());

	for (unsigned char index (0u); index < m_locations[locationX][locationY][locationZ].GetElementNumber (); ++index)
	{
		pOtherWpt = g_server->GetWaypointManager ().GetWaypoint (m_locations[locationX][locationY][locationZ][index]);

		if (!pOtherWpt->IsValid () || pOtherWpt == pWpt)
			continue;

		vOtherWptOrigin = pOtherWpt->GetOrigin ();

//		if (!Math::AreEqual2 (vOtherWptOrigin.z, vWptOrigin.z, 128.0f))
//			continue;

		if (vWptOrigin.GetDistanceSquared (vOtherWptOrigin) <= MATH_GET_SQUARED (bot_waypointpathdist.GetFloat ()) && visiblityTable.IsVisible (iWptFrom, m_locations[locationX][locationY][locationZ][index]))//CBotGlobals::isVisible (vWptOrigin, vOtherWptOrigin))
		{
			if (CBotGlobals::walkableFromTo (pPlayer, vWptOrigin, vOtherWptOrigin))
				pWpt->addPathTo (iWpt);

			if ( CBotGlobals::walkableFromTo(pPlayer,vOtherWptOrigin,vWptOrigin))
				pOtherWpt->addPathTo(iWptFrom);
		}
	}*/
}

const unsigned short WaypointLocationsTable::GetCoverWaypoint (const Math::Vector3D &vPlayerOrigin, const Math::Vector3D &vCoverFrom, const DynamicArray <unsigned short> *const iIgnoreWpts, Math::Vector3D *const vGoalOrigin, const HalfLifeEngine::SDK::Constants::TeamID_t team)
{
	// returns nearest waypoint that can be used to cover from 'vCoverFrom' vector

	const unsigned short waypoint (GetNearestWaypoint (vCoverFrom, MaximumReachableRange, InvalidWaypointIndex, false));

	if (waypoint == InvalidWaypointIndex)
		return InvalidWaypointIndex;

	float nearestDistance (HalfLifeEngine::SDK::Constants::HalfMapSize);
	unsigned short nearestIndex (InvalidWaypointIndex);
	unsigned char minLocationX, maxLocationX, minLocationY, maxLocationY, minLocationZ, maxLocationZ;

	GetMinsMaxs (vPlayerOrigin, minLocationX, minLocationY, minLocationZ, maxLocationX, maxLocationY, maxLocationZ);

	ResetFailedWaypoints (iIgnoreWpts);

	for (unsigned char locationY, locationZ; minLocationX <= maxLocationX; ++minLocationX)
		for (locationY = minLocationY; locationY <= maxLocationY; ++locationY)
			for (locationZ = minLocationZ; locationZ <= maxLocationZ; ++locationZ)
			{
				// check each area around the current area for closer waypoints
				FindNearestCoverWaypointInBucket (minLocationX, locationY, locationZ, vPlayerOrigin, nearestDistance, nearestIndex, waypoint, vGoalOrigin, team);
			}

	return nearestIndex;
}

void WaypointLocationsTable::FindNearestCoverWaypointInBucket (const unsigned char locationX, const unsigned char locationY, const unsigned char locationZ, const Math::Vector3D &origin, float &minDist, unsigned short &index, const unsigned short iCoverFromWpt, Math::Vector3D *const vGoalOrigin, const HalfLifeEngine::SDK::Constants::TeamID_t team)
{
	// Search for the nearest waypoint : I.e.
	// Find the waypoint that is closest to 'origin' from the distance 'minDist'
	// And set the 'index' to the waypoint index if closer.

	Waypoint *curr_wpt;
	unsigned short iSelectedIndex;
	float distance;

	for (unsigned char l (0u); l < m_locations[locationX][locationY][locationZ].GetElementNumber (); ++l)
	{
		iSelectedIndex = m_locations[locationX][locationY][locationZ][l];

		if (iCoverFromWpt == iSelectedIndex || g_failedWaypoints[iSelectedIndex] == 1u)
			continue;

		curr_wpt = g_server->GetWaypointManager ().GetWaypoint (iSelectedIndex);

		if (!curr_wpt->IsValid () || !curr_wpt->IsForTeam (team) || g_server->GetWaypointManager ().GetVisibilityTable ().IsVisible (iCoverFromWpt, iSelectedIndex))
			continue;

		distance = curr_wpt->GetOrigin ().GetDistance (origin);

		if (vGoalOrigin != NULL)
			distance += curr_wpt->GetOrigin ().GetDistance (*vGoalOrigin);

		if (distance < minDist)
		{
			index = iSelectedIndex;
			minDist = distance;
		}
	}
}

void WaypointLocationsTable::FindNearestInBucket (const unsigned char locationX, const unsigned char locationY, const unsigned char locationZ, const Math::Vector3D &origin, float &minDist, unsigned short &index, const unsigned short iIgnoreWpt, const bool bGetVisible, const bool bIsBot, const HalfLifeEngine::SDK::Constants::TeamID_t team)
{
	// Search for the nearest waypoint : I.e.
	// Find the waypoint that is closest to 'origin' from the distance 'minDist'
	// And set the 'index' to the waypoint index if closer.

	Waypoint *curr_wpt;
	float distance;
	unsigned short iSelectedIndex;

	for (unsigned char l (0u); l < m_locations[locationX][locationY][locationZ].GetElementNumber (); ++l)
	{
		iSelectedIndex = m_locations[locationX][locationY][locationZ][l];

		if (iSelectedIndex == iIgnoreWpt)
			continue;

		if (g_failedWaypoints[iSelectedIndex] == 1u)
		{
			g_failedWaypoints[iSelectedIndex] = 2u;	// flag this

			continue;
		}

		curr_wpt = g_server->GetWaypointManager ().GetWaypoint (iSelectedIndex);

		if (!curr_wpt->IsValid ())
			continue;

		if (!curr_wpt->IsForTeam (team))
			continue;

//		if (bIsBot && (curr_wpt->GetFlags () & CWaypointTypes::W_FL_JUMP))
		if (bIsBot && curr_wpt->IsConnectedWithTravelFlag (WaypointNode::ConnectionFlag_Jump))
			continue;

		distance = curr_wpt->GetOrigin ().GetDistance (origin);

		if (distance >= minDist)
			continue;

		if (bGetVisible)
		{
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (origin, curr_wpt->GetOrigin (), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

			if (traceResult.fraction < 1.0f)
				continue;
		}

		index = iSelectedIndex;
		minDist = distance;
	}
}

const unsigned short WaypointLocationsTable::GetNearestWaypoint (const Math::Vector3D &origin, float nearestDistance, const unsigned short iIgnoreWpt, const bool bGetVisible, const bool bIsBot, DynamicArray <unsigned short> *const iFailedWpts, const bool bNearestAimingOnly, const HalfLifeEngine::SDK::Constants::TeamID_t team)
{
	// gets the nearest waypoint INDEX from an origin

	unsigned short nearestIndex (InvalidWaypointIndex);
	unsigned char minLocationX, maxLocationX, minLocationY, maxLocationY, minLocationZ, maxLocationZ;

	GetMinsMaxs (origin, minLocationX, minLocationY, minLocationZ, maxLocationX, maxLocationY, maxLocationZ);

	if (!bNearestAimingOnly)
		ResetFailedWaypoints (iFailedWpts);

	for (unsigned char locationY, locationZ; minLocationX <= maxLocationX; ++minLocationX)
		for (locationY = minLocationY; locationY <= maxLocationY; ++locationY)
			for (locationZ = minLocationZ; locationZ <= maxLocationZ; ++locationZ)
				FindNearestInBucket (minLocationX, locationY, locationZ, origin, nearestDistance, nearestIndex, iIgnoreWpt, bGetVisible, bIsBot, team);

	if (iFailedWpts != NULL)
		for (unsigned short index (0u), waypoint; index < iFailedWpts->GetElementNumber (); ++index)
			if ((waypoint = (*iFailedWpts)[index]) != InvalidWaypointIndex && g_failedWaypoints[waypoint] == 2u)
				iFailedWpts->Pop (waypoint);

	return nearestIndex;
}

void WaypointLocationsTable::DrawWaypoints (HalfLifeEngine::SDK::Classes::Edict *const player, const bool drawPaths)
{
	// Draw waypoints around a player

	unsigned char minLocationX, maxLocationX, minLocationY, maxLocationY, minLocationZ, maxLocationZ;
	Math::Vector3D waypointOrigin;
	unsigned char *PVS (NULL), *PAS (NULL);

	// Setup visibility, for quick visibility checking.
	HalfLifeEngine::Globals::g_halfLifeEngine->SetupVisibility (NULL, player, &PVS, &PAS);

	GetMinsMaxs (player->variables.origin, minLocationX, minLocationY, minLocationZ, maxLocationX, maxLocationY, maxLocationZ);

	for (unsigned char locationY, locationZ, index; minLocationX <= maxLocationX; ++minLocationX)
		for (locationY = minLocationY; locationY <= maxLocationY; ++locationY)
			for (locationZ = minLocationZ; locationZ <= maxLocationZ; ++locationZ)
				for (index = 0u; index < m_locations[minLocationX][locationY][locationZ].GetElementNumber (); ++index)
				{
					// Draw each waypoint in distance

					Waypoint *const waypoint (g_server->GetWaypointManager ().GetWaypoint (m_locations[minLocationX][locationY][locationZ][index]));

					// deleted?
					if (!waypoint->IsValid ())
						continue;	// Skip it....

					waypointOrigin = waypoint->GetOrigin ();

					// also in z range
					if (/*Math::AreEqual2 (waypointOrigin.z, player->variables.origin.z, MAX_BUCKET_SPACING) && */HalfLifeEngine::Globals::g_halfLifeEngine->CheckVisibility (player, HalfLifeEngine::Globals::g_halfLifeEngine->SetPotentiallyVisible (waypointOrigin)))
						waypoint->Draw (drawPaths);
				}
}