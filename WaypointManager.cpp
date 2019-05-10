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
// Waypoint.cpp
//
// Class: Waypoint
//
// Description: Waypoint operation class.
//
// Version: $ID:$
//

#include <Core.hpp>

const DynamicString WaypointManager::FileExtention ("WPT");

const bool WaypointManager::Load (void)
{
	// loads a waypoint file
/*
	STDIOFile file (CheckSubfolderFile (), "rb");

	// we're got valid handle?
	if (!file.IsValid ())
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.pwf does not exist.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ());

		return false;
	}

	WaypointHeader_t header;

	// if file exists, read the waypoint structure from it
	if (file.Read (header) == 0)
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.pwf - has wrong or unsupported format.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ());

		return false;
	}

	if (strncmp (header.headerName, FileHeader_Waypoint, sizeof (header.headerName)) != 0)
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.pwf is not a YaPB waypoint file (header found '%s' needed '%s'.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData (), header.headerName, FileHeader_Waypoint);

		return false;
	}

	if (header.fileVersion != FileHalfLifeEngine::Engine::Version_Waypoint)
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.pwf - incorrect waypoint file version (expected '%i' found '%i').", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData (), FileHalfLifeEngine::Engine::Version_Waypoint, header.fileVersion);

		return false;
	}

	if (header.mapName != HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ())
	{
		AddLogEntry (true, LogLevel_Error, false, "%s.pwf - hacked waypoint file, filename doesn't match waypoint header information (map name: '%s', header name: '%s').", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData (), header.mapName);

		return false;
	}

	Initialize ();

	// read and add waypoint paths...
	for (unsigned short index (0u); index < header.pointNumber; ++index)
	{
		Waypoint *const newWaypoint (new Path_t ());

		if (newWaypoint == NULL)
		{
			TerminateOnMalloc ();

			return false;
		}

		// read the number of paths from this node...
		file.Read (*newWaypoint);

		m_waypointDisplayTime[index] = 0.0f;

//		newWaypoint->next = m_waypoints[index + 1u];

		m_waypoints += newWaypoint;
	}

//	m_waypoints[index - 1u]->next = NULL;

	m_waypointPaths = true;

	InitPathMatrix ();
	InitTypes ();

	g_waypointsChanged = false;
	g_killHistory = 0;

	InitVisibilityTable ();
	InitExperienceTable ();

	g_botManager->InitQuota ();

	Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DebugGoal)->SetValue <short> (InvalidWaypointIndex);

	_snprintf (m_infoBuffer, sizeof (m_infoBuffer), strnicmp (header.authorName, "official", 8) == 0 ? "Using Official Waypoint File." : "Using Waypoint File By: %s.", header.authorName);
*/
	return true;
}

const bool WaypointManager::Save (void) const
{
	// saves the waypoint file
/*
	WaypointHeader_t header;

	memset (header.headerName, 0, sizeof (header.headerName));
	memset (header.mapName, 0, sizeof (header.mapName));
	memset (header.authorName, 0, sizeof (header.authorName));

	strcpy (header.headerName, FileHeader_Waypoint);
	strncpy (header.mapName, HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName (), sizeof (header.mapName));
	strncpy (header.authorName, g_hostEntity->GetName (), sizeof (header.authorName));

	header.fileVersion = FileHalfLifeEngine::Engine::Version_Waypoint;
	header.pointNumber = GetWaypointsNumber ();

	STDIOFile file (GetWaypointsDirectory () + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".pwf", "wb");

	// we're got valid handle?
	if (!file.IsValid ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Error writing '%s.pwf' waypoint file.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ());

		return false;
	}

	// write the waypoint header to the file...
	file.Write (header);

	// save the waypoint paths...
	for (int i (0); i < GetWaypointsNumber (); ++i)
		file.Write (m_waypoints[i], sizeof (m_waypoints[i], 1u));

	// save XML version
	SaveXML ();
*/
	return true;
}

void WaypointManager::SaveXML (void) const
{
	STDIOFile file (GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".xml", "w");

	// we're got valid handle?
	if (!file.IsValid ())
	{
		AddLogEntry (true, LogLevel_Error, false, "Error writing '%s.xml' waypoint file.", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ());

		return;
	}

	file.Print ("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	file.PrintFormat ("<!-- Generated by %s v%s. You may not edit this file! -->\n", PRODUCT_NAME, PRODUCT_VERSION);
	file.Print ("<WaypointData>\n");
	file.Print ("\t<header>\n");
	file.PrintFormat ("\t\t<author>%s</author>\n", HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () == false ? g_server->GetHumanManager ()->GetHostClient ()->GetName ().GetData () : "NULL!!!!!!!!!");	// Need do something here!
	file.PrintFormat ("\t\t<map>%s</map>\n", HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ());
	file.PrintFormat ("\t\t<version>%u</version>\n", Waypoint::HeaderFileVersion);
	file.PrintFormat ("\t\t<number>%u</number>\n", GetWaypointsNumber ());
	file.Print ("\t</header>\n");
	file.Print ("\t<map>\n");

	// save the waypoint paths...
	for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)
	{
		file.PrintFormat ("\t\t<waypoint ID=\"%u\">\n", index + 1u);

		m_waypoints[index]->SaveXML (file);
	}

	file.Print ("\t</map>\n");
	file.Print ("</WaypointData>");
}

const bool WaypointManager::IsNodesValid (void) const
{
	// checks if all nodes are valid - everything connected
/*
	int index, k;
	unsigned j;
	bool hasGoalPoints = false, hasRescuePoints = false, hasTerroristPoints = false, hasCtPoints = false, hasConnections = false;

	for (index = 0; index < GetWaypointsNumber (); ++index)
	{
		for (j = 0; j < Const_MaxPathIndex; j++)
		{
			if (m_paths[index]->connections[j].index == -1)
				continue;

			hasConnections = true;

			break;
		}

		if (!hasConnections && !IsConnected (index))
		{
			AddLogEntry (true, LogLevel_Warning, false, "Waypoint %u isn't connected with any other Waypoint!", index);

			// play "failed" sound...
			EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

			// teleport the client on a place of a mistake...
			SET_ORIGIN (g_hostEntity->m_entity, m_paths[index]->origin);

			// activate waypoint editing
			g_waypointOn = true;

			return false;
		}

		if (m_paths[index]->pathNumber != index)
		{
			AddLogEntry (true, LogLevel_Warning, false, "Waypoint %u pathnumber differs from index!", index);

			// play "failed" sound...
			EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

			// teleport the client on a place of a mistake...
			SET_ORIGIN (g_hostEntity->m_entity, m_paths[index]->origin);

			// activate waypoint editing
			g_waypointOn = true;

			return false;
		}

		if (m_paths[index]->flags & WaypointFlag_Goal)
			hasGoalPoints = true;
		else if (m_paths[index]->flags & WaypointFlag_Rescue)
			hasRescuePoints = true;

		if (m_paths[index]->flags & WaypointFlag_Camp)
		{
			if (m_paths[index]->campEnd == Vector2D::GetNull ())
			{
				AddLogEntry (true, LogLevel_Warning, false, "Waypoint %u Camp-End position not set!", index);

				// play "failed" sound...
				EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

				// teleport the client on a place of a mistake...
				SET_ORIGIN (g_hostEntity->m_entity, m_paths[index]->origin);

				// activate waypoint editing
				g_waypointOn = true;

				return false;
			}
		}

		if (m_paths[index]->flags & WaypointFlag_Terrorist)
			hasTerroristPoints = true;

		if (m_paths[index]->flags & WaypointFlag_Counter)
			hasCtPoints = true;

		for (k = 0; k < Const_MaxPathIndex; ++k)
		{
			if (m_paths[index]->connections[k].index == -1)
				continue;

			if (m_paths[index]->connections[k].index < -1 || m_paths[index]->connections[k].index >= GetWaypointsNumber ())
			{
				AddLogEntry (true, LogLevel_Warning, false, "Waypoint %u - Pathindex %u out of Range!", index, k);

				// play "failed" sound...
				EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

				// teleport the client on a place of a mistake...
				SET_ORIGIN (g_hostEntity->m_entity, m_paths[index]->origin);

				// activate waypoint editing
				g_waypointOn = true;

				return false;
			}
			else if (m_paths[index]->connections[k].index == index)
			{
				AddLogEntry (true, LogLevel_Warning, false, "Waypoint %u - Pathindex %u points to itself!", index, k);

				// play "failed" sound...
				EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

				// teleport the client on a place of a mistake...
				SET_ORIGIN (g_hostEntity->m_entity, m_paths[index]->origin);

				// activate waypoint editing
				g_waypointOn = true;

				return false;
			}
		}
	}

	if (!hasGoalPoints && !(g_server->GetMapType () & (Server::MapType_Ka | Server::MapType_Fy | Server::MapType_Gg | Server::MapType_Aim | Server::MapType_Dm | Server::MapType_Kz)))
	{
		AddLogEntry (true, LogLevel_Warning, false, "You didn't set any Goal Point!");

		// play "failed" sound...
		EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

		return false;
	}
	else if (!hasRescuePoints && (g_server->GetMapType () & Server::MapType_Cs))
	{
		AddLogEntry (true, LogLevel_Warning, false, "You didn't set a Rescue Point!");

		// play "failed" sound...
		EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

		return false;
	}
	else if (!hasTerroristPoints)
	{
		AddLogEntry (true, LogLevel_Warning, false, "You didn't set any Terrorist Important Point!");

		// play "failed" sound...
		EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

		return false;
	}
	else if (!hasCtPoints)
	{
		AddLogEntry (true, LogLevel_Warning, false, "You didn't set any CT Important Point!");

		// play "failed" sound...
		EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

		return false;
	}

	// perform DFS instead of Floyd-Warshall, this shit speedup this process in a bit
	PathNode_t *stack, *current, *newNode;
	bool visited[Const_MaxWaypoints];

	// first check incoming connectivity, initialize the "visited" table
	for (index = 0; index < GetWaypointsNumber (); ++index)
		visited[index] = false;

	// check from waypoint nr. 0
	stack = new PathNode_t;
	stack->next = NULL;
	stack->index = 0;

	while (stack != NULL)
	{
		// pop a node from the stack
		current = stack;
		stack = stack->next;

		visited[current->index] = true;

		for (j = 0; j < Const_MaxPathIndex; j++)
		{
			index = m_paths[current->index]->connections[j].index;

			if (index < 0 || index >= GetWaypointsNumber () || visited[index])
				continue;	// skip this waypoint as it's index not valid or already visited

			newNode = new PathNode_t;

			newNode->next = stack;
			newNode->index = index;

			stack = newNode;
		}

		delete current;
	}

	for (index = 0; index < GetWaypointsNumber (); ++index)
	{
		if (visited[index])
			continue;

		AddLogEntry (true, LogLevel_Warning, false, "Path broken from Waypoint #0 to Waypoint #%u!", index);

		// play "failed" sound...
		EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

		// teleport the client on a place of a mistake...
		SET_ORIGIN (g_hostEntity->m_entity, m_paths[index]->origin);

		// activate waypoint editing
		g_waypointOn = true;

		return false;
	}

	// then check outgoing connectivity
	DynamicArray <int> outgoingPaths[Const_MaxWaypoints];	// store incoming paths for speedup

	for (index = 0; index < GetWaypointsNumber (); ++index)
		for (j = 0; j < Const_MaxPathIndex; ++j)
			if (m_paths[index]->connections[j].index >= 0 && m_paths[index]->connections[j].index < GetWaypointsNumber ())
				outgoingPaths[m_paths[index]->connections[j].index].Push (index);

	// reinitialize the "visited" table
	for (index = 0; index < GetWaypointsNumber (); ++index)
		visited[index] = false;

	// check from waypoint nr. 0
	stack = new PathNode_t;
	stack->next = NULL;
	stack->index = 0;

	while (stack != NULL)
	{
		// pop a node from the stack
		current = stack;
		stack = stack->next;

		visited[current->index] = true;

		for (j = 0; j < outgoingPaths[current->index].GetElementNumber (); ++j)
		{
			if (visited[outgoingPaths[current->index][j]])
				continue;	// skip this waypoint as it's already visited

			newNode = new PathNode_t;

			newNode->next = stack;
			newNode->index = outgoingPaths[current->index][j];

			stack = newNode;
		}

		delete current;
	}

	for (index = 0; index < GetWaypointsNumber (); ++index)
	{
		if (visited[index])
			continue;

		AddLogEntry (true, LogLevel_Warning, false, "Path broken from Waypoint #%u to Waypoint #0!", index);

		// play "failed" sound...
		EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "debris/bustglass1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);

		// teleport the client on a place of a mistake...
		SET_ORIGIN (g_hostEntity->m_entity, m_paths[index]->origin);

		// activate waypoint editing
		g_waypointOn = true;

		return false;
	}

	// play "done" sound...
	EMIT_SOUND_DYN2 (g_hostEntity->m_entity, CHAN_BODY, "items/medshot4.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);
*/
	return true;
}

void WaypointManager::AddWaypoint (const Math::Vector3D &origin, const Waypoint::Flag_t flag, const unsigned char illumination)
{
	// adds a new waypoint

	if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
		return;

	Client *const hostClient (g_server->GetHumanManager ()->GetHostClient ());

	// Is maximum waypoint number already used?
	if (GetWaypointsNumber () == MaximumWaypoints)
	{
		AddLogEntry (true, LogLevel_Warning, false, "Maximal number of waypoints reached! Can't add new waypoint!");

		// Play "failed" sound....
		hostClient->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "debris/bustglass1.wav");

		return;
	}

	const unsigned short freeWaypointIndex (GetWaypointsNumber ());

	#if defined _DEBUG
		// Find the first free waypoint index from the ours waypoints array....
		for (unsigned short index (0u); index < freeWaypointIndex; ++index)
		{
			// Is this slot already used?
			if (!m_waypoints[index]->IsValid ())
				InternalAssert (0);
		}
	#endif	// if defined _DEBUG
/*
	switch (flags)
	{
		// camp end
		case 6:
			if (!hostClient->GetCurrentWaypoint ()->IsValid ())
				return;

			path = m_waypoints[index];

			if (!(path->flags & WaypointFlag_Camp))
			{
				CenterPrint ("This is not camping waypoint!");

				return;
			}

			forward = GetPlayerAimPosition (g_hostEntity->m_edict, 640.0f);

			path->campEnd = Math::Vector2D (forward.x, forward.y);

			// play "done" sound...
			hostClient->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "common/wpn_hudon.wav");

			return;

		// jump start
		case 9:
			if (!hostClient->GetCurrentWaypoint ()->IsValid ())
			{
				newOrigin = m_learnPosition;

				break;
			}

			distanceSquared = m_waypoints[index]->origin.GetDistanceSquared (g_hostEntity->variables.origin);

			if (distanceSquared < 50.0f)
			{
				placeNew = false;
				path = m_waypoints[index];

				path->origin = path->origin.GetMidPoint (m_learnPosition);
			}

			break;

		// jump end
		case 10:
			if (!hostClient->GetCurrentWaypoint ()->IsValid ())
				break;

			distanceSquared = m_waypoints[index]->origin.GetDistanceSquared (g_hostEntity->variables.origin);

			if (distanceSquared < MATH_GET_SQUARED (50.0f))
			{
				bool hasConnectionFlags = false;

				placeNew = false;
				path = m_waypoints[index];

				for (i = 0; i < Const_MaxPathIndex; ++i)
				{
					if (path->connections[i].flags == 0)
						continue;

					hasConnectionFlags = true;

					break;
				}

				if (!hasConnectionFlags)
					path->origin = path->origin.GetMidPoint (g_hostEntity->variables.origin);
			}

			break;
	}
*/
	// Create new waypoint (if creator is ducking add crouch flag)....
	Waypoint *const newWaypoint (new Waypoint (origin, hostClient->IsOnLadder () ? hostClient->GetEdict ()->IsDucking () ? flag | Waypoint::Flag_Ladder | Waypoint::Flag_Crouch : flag | Waypoint::Flag_Ladder : hostClient->GetEdict ()->IsDucking () ? flag | Waypoint::Flag_Crouch : flag, illumination));

	// Reliability check.
	if (newWaypoint == NULL)
	{
		// Error allocating memory!
		AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint!");

		return;
	}

	// Reliability check.
	InternalAssert (freeWaypointIndex == GetWaypointsNumber ());

	m_waypoints += newWaypoint;	// Increment total number of waypoints used....

	m_locationsTable.AddWptLocation (freeWaypointIndex, origin);

	m_isWaypointsChanged = true;

	// play "done" sound...
	hostClient->GetEdict ()->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Body, "weapons/xbow_hit1.wav");
}

void WaypointManager::Think (void)
{
	// This function executes frame of waypoint operation code.

	if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () || !m_isEditModeActivated || GetWaypointsNumber () == 0u)
		return;	// this function is only valid on listen server, and in waypoint enabled mode.

	Human *const hostClient (g_server->GetHumanManager ()->GetHostClient ());

	// Reliability check.
	InternalAssert (hostClient->IsValid ());

	m_facingAtWaypoint = GetHostClientFacingWaypoint ();
/*
	// now iterate through all waypoints in a map, and draw required ones
	for (unsigned short index = 0u; index < GetWaypointsNumber (); ++index)
		if (m_waypoints[index]->IsValid ())
			m_waypoints[index]->Draw (true);*/
	m_locationsTable.DrawWaypoints (*hostClient, true);

	// draw arrow to a some importaint waypoints
	if (m_showImportaintWaypointsTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		// waypoint user facing at - white arrow
		if (m_facingAtWaypoint != NULL)
			hostClient->DrawLine (hostClient->GetOrigin (), m_facingAtWaypoint->m_origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 255u, 255u), 200u, 0u, 5u);

		// cached waypoint - yellow arrow
		if (m_cachedWaypoint != NULL)
			hostClient->DrawLine (hostClient->GetOrigin (), m_cachedWaypoint->m_origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 255u, 0u), 200u, 0u, 5u);

		// finding waypoint - pink arrow
		if (m_findWaypoint != NULL)
			hostClient->DrawLine (hostClient->GetOrigin (), m_findWaypoint->m_origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (128u, 0u, 128u), 200u, 0u, 5u);

		// next display update in 500 milliseconds
		m_showImportaintWaypointsTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;
	}
#if 0
	// draw the danger directions
	if (!m_isWaypointsChanged)
	{
		if ((g_experienceData + (nearestIndex * GetWaypointsNumber ()) + nearestIndex)->dangerIndex[HalfLifeEngine::SDK::Constants::TeamID_Terrorist] != -1 && hostClient->GetTeam () == HalfLifeEngine::SDK::Constants::TeamID_Terrorist)
			hostClient->DrawLine (path->origin, m_paths[(g_experienceData + (nearestIndex * GetWaypointsNumber ()) + nearestIndex)->dangerIndex[HalfLifeEngine::SDK::Constants::TeamID_Terrorist]]->origin, g_precachedSpriteIndexes.arrow, 15, 0, RGB (255, 0, 0), 200, 0, 10);	// draw a red arrow to this index's danger point

		if ((g_experienceData + (nearestIndex * GetWaypointsNumber ()) + nearestIndex)->dangerIndex[HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist] != -1 && hostClient->GetTeam () == HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist)
			hostClient->DrawLine (path->origin, m_paths[(g_experienceData + (nearestIndex * GetWaypointsNumber ()) + nearestIndex)->dangerIndex[HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist]]->origin, g_precachedSpriteIndexes.arrow, 15, 0, RGB (0, 0, 255), 200, 0, 10);	// draw a blue arrow to this index's danger point
	}

	// draw a paths, camplines and danger directions for nearest waypoint
	if (m_pathDisplayTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		return;

	Waypoint *const hostClientCurrentWaypoint (hostClient->GetCurrentWaypoint ());

	if (!hostClientCurrentWaypoint->IsValid ())
		return;

	// display some waypoint information

	char tempMessage[491u];

	// show the information about that point
	unsigned int length
	(
		sprintf
		(
			tempMessage,

			"Current WP Info:\n"
			"\t\tIndex: %u of %u\n"
			"\t\tRadius: %.1f\n"
			"\t\tOrigin: x=\"%.2f\", y=\"%.2f\", z=\"%.2f\"\n"
			"\t\tFlags: %s", nearestIndex, GetWaypointsNumber (), path->radius, path->origin.x, path->origin.y, path->origin.z, hostClientCurrentWaypoint->GetInfo ().GetData ()
		)
	);

	// check if we need to show the facing point index
	if (m_facingAtWaypoint != NULL)
	{
		length += sprintf
		(
			tempMessage + length,

			"\n\n"
			"Facing WP Info:\n"
			"\t\tIndex: %u\n"
			"\t\tRadius: %.1f\n"
			"\t\tOrigin: x=\"%.2f\", y=\"%.2f\", z=\"%.2f\"\n"
			"\t\tFlags: %s", m_facingAtWaypoint, m_facingAtWaypoint->m_radius, m_facingAtWaypoint->m_origin.x, m_facingAtWaypoint->m_origin.y, m_facingAtWaypoint->m_origin.z, m_facingAtWaypoint->GetInfo ().GetData ()
		);
	}

	// check if we need to show the cached point index
	if (m_cachedWaypoint != NULL)
	{
		length += sprintf
		(
			tempMessage + length,

			"\n\n"
			"Cached WP Info:\n"
			"\t\tIndex: %u\n"
			"\t\tRadius: %.1f\n"
			"\t\tOrigin: x=\"%.2f\", y=\"%.2f\", z=\"%.2f\"\n"
			"\t\tFlags: %s", m_cachedWaypoint, m_cachedWaypoint->m_radius, m_cachedWaypoint->m_origin.x, m_cachedWaypoint->m_origin.y, m_cachedWaypoint->m_origin.z, m_cachedWaypoint->GetInfo ().GetData ()
		);
	}

	// if waypoint is not changed display experience also
	if (!m_isWaypointsChanged)
	{
		dangerIndex[TeamArrayID_Terrorist] = (g_experienceData + nearestIndex * GetWaypointsNumber () + nearestIndex)->dangerIndex[TeamArrayID_Terrorist];
		dangerIndex[TeamArrayID_CounterTerrorist] = (g_experienceData + nearestIndex * GetWaypointsNumber () + nearestIndex)->dangerIndex[TeamArrayID_CounterTerrorist];

		length += sprintf
		(
			tempMessage + length,

			"\n\n"
			"Experience Info:\n"
			"\t\tT: %u / %u\n"
			"\t\tCT: %u / %u", dangerIndex[TeamArrayID_Terrorist], dangerIndex[TeamArrayID_Terrorist] != -1 ? (g_experienceData + nearestIndex * GetWaypointsNumber () + dangerIndex[TeamArrayID_Terrorist])->damage[TeamArrayID_Terrorist] : 0, dangerIndex[TeamArrayID_CounterTerrorist], dangerIndex[TeamArrayID_CounterTerrorist] != -1 ? (g_experienceData + nearestIndex * GetWaypointsNumber () + dangerIndex[TeamArrayID_CounterTerrorist])->damage[TeamArrayID_CounterTerrorist] : 0
		);
	}

	// just to be sure...
	tempMessage[sizeof (tempMessage) - 1u] = '\0';

	const float delay (1.0f);

	// draw entire message
	hostClient->HudMessage (Math::Vector2D (0.01f, 0.14f), ColorWithAlpha <> (255u, 255u, 255u, 1u), ColorWithAlpha <> (255u), HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, delay, 0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose, tempMessage);
/*	HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::SDK::Constants::SVC_TEMPENTITY, NULL, hostClient->GetEdict ());
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (HalfLifeEngine::SDK::Constants::TempEntityEvent_TextMessage);
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (4);	// channel
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (HalfLifeEngine::Utilities::GetFixedSigned16 (0.01f, 13u));	// x coordinates
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (HalfLifeEngine::Utilities::GetFixedSigned16 (0.14f, 13u));	// y coordinates
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (0);	// effect (fade in/out)
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// initial RED
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// initial GREEN
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// initial BLUE
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (1);	// initial ALPHA
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// effect RED
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// effect GREEN
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// effect BLUE
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (255);	// effect ALPHA
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (0);	// fade-in time in seconds * 256
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (0);	// fade-out time in seconds * 256
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (HalfLifeEngine::Utilities::GetFixedUnsigned16 (1.0f, 8u));	// hold time in seconds
		HalfLifeEngine::Globals::g_halfLifeEngine->WriteString (tempMessage);	// write message
	HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end
*/
	// update timer to one second
	m_pathDisplayTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + delay;
#endif	// if 0
}

Waypoint *const WaypointManager::GetHostClientFacingWaypoint (void) const
{
	// This function finds waypoint the user is pointing at.

	float distance, nearestDistance (MATH_GET_SQUARED (500.0f));
	Waypoint *facingAtWaypoint (NULL);
	const Client *const hostClient (g_server->GetHumanManager ()->GetHostClient ());

	// find the waypoint the user is pointing at
	for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)
	{
		// get the current view cones, check if we can see it on X, Y and Z axes AND check if we got more close waypoint
		if (!m_waypoints[index]->IsValid () || (distance = m_waypoints[index]->m_origin.GetDistanceSquared (hostClient->GetOrigin ())) >= nearestDistance || Math::Vector3D (hostClient->GetShootingConeDeviation (m_waypoints[index]->m_origin), hostClient->GetShootingConeDeviation (m_waypoints[index]->m_origin - Math::Vector3D (0.0f, 0.0f, (m_waypoints[index]->m_flags & Waypoint::Flag_Crouch) ? 8.0f : 15.0f)), hostClient->GetShootingConeDeviation (m_waypoints[index]->m_origin + Math::Vector3D (0.0f, 0.0f, (m_waypoints[index]->m_flags & Waypoint::Flag_Crouch) ? 8.0f : 15.0f))) < 0.998f)
			continue;

		nearestDistance = distance;	// update nearest distance
		facingAtWaypoint = m_waypoints[index];	// keep track of the closest found waypoint
	}

	return facingAtWaypoint;
}

const bool WaypointManager::IsNodeReachable (const Math::Vector3D &source, const Math::Vector3D &destination) const
{
	// Checks if the node is reachable.

	Math::Vector3D direction (destination - source);
	float distance (direction.GetLength ());	// distance from goal

	// is the destination not close enough?
	if (distance > m_autoPathDistance)
		return false;

	HalfLifeEngine::SDK::Classes::Edict *const hostClientEdict (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ());
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	// check if we go through a "func_illusionary", in which case return false
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, true, HalfLifeEngine::SDK::Constants::Hull_Head, hostClientEdict, traceResult);

	// don't add path waypoints through func_illusionaries
	if (traceResult.hitEntity->IsValid () && traceResult.hitEntity->GetClassName () == "func_illusionary")
		return false;

	// check if this waypoint is "visible"...
	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, hostClientEdict, traceResult);

	// if waypoint is visible from current position (even behind head)...
	while (traceResult.fraction < 1.0f)
	{
		// if it's a door check if nothing blocks behind
		if (traceResult.hitEntity->GetClassName ().CompareWithCaseCheck ("func_door", GET_STATIC_STRING_LENGTH ("func_door")) != 0)
			return false;

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (traceResult.endPosition, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult.hitEntity, traceResult);
	}

	// check for special case of both waypoints being in water...
	if (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (source) == HalfLifeEngine::SDK::Constants::Content_Water && HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (destination) == HalfLifeEngine::SDK::Constants::Content_Water)
		return true;	// then they're reachable each other

	// is destination waypoint higher than source? (45.0 is max jump height)
	if (destination.z > source.z + HalfLifeEngine::SDK::Constants::JumpHeight/*JumpCrouchHeight*/ + 1.0f)
	{
		const Math::Vector3D destinationNew (destination.x, destination.y, destination.z - 50.0f);	// straight down 50 units

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (destination, destinationNew, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, hostClientEdict, traceResult);

		// check if we didn't hit anything, if not then it's in mid-air
		if (traceResult.fraction == 1.0f)
			return false;	// can't reach this one
	}

	if (distance <= 10.0f)
		return true;

	// check if distance to ground drops more than step height at points between source and destination...
	Math::Vector3D check (source), down (source);

	down.z -= 1000.0f;	// straight down 1000 units

	direction /= distance;	// 1 unit long
	direction *= 10.0f;		// 10 units long

	HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (check, down, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, hostClientEdict, traceResult);

	float height;
	float lastHeight (traceResult.fraction/* * 1000.0f*/);	// height from ground

	do
	{
		// move 10 units closer to the goal...
		check += direction;

		down = check;

		down.z -= 1000.0f;	// straight down 1000 units

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (check, down, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, hostClientEdict, traceResult);

		// height from ground
		height = traceResult.fraction/* * 1000.0f*/;

		// is the current height greater than the step height?
		if (height < lastHeight - HalfLifeEngine::SDK::Constants::StepHeight * 0.001f)
			return false;	// can't get there without jumping...

		lastHeight = height;
		distance = destination.GetDistanceSquared (check);	// distance from goal
	} while (distance > MATH_GET_SQUARED (10.0f));

	return true;
}

void WaypointManager::CalculateWayNodesForWaypoint (Waypoint *const waypoint)
{
	// ladder waypoints need careful connections
	if (waypoint->m_flags & Waypoint::Flag_Ladder)
	{
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		float distanceSquared, minDistanceSquared (MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::MapSize));
		Waypoint *destinationWaypoint (NULL);

		// calculate all the paths to this new waypoint
		for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)
		{
			// Reliability check.
			InternalAssert (m_waypoints[index]->IsValid ());

			if (m_waypoints[index] == waypoint)
				continue;	// skip the waypoint that was just added

			// other ladder waypoints should connect to this
			if (m_waypoints[index]->m_flags & Waypoint::Flag_Ladder)
			{
				// check if the waypoint is reachable from the new one
				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (waypoint->m_origin, m_waypoints[index]->m_origin, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, *g_server->GetHumanManager ()->GetHostClient (), traceResult);

				if (traceResult.fraction == 1.0f && Math::AreEqual (waypoint->m_origin.x, m_waypoints[index]->m_origin.x, 64.0f) && Math::AreEqual (waypoint->m_origin.y, m_waypoints[index]->m_origin.y, 64.0f) && Math::AreEqual (waypoint->m_origin.z, m_waypoints[index]->m_origin.z, m_autoPathDistance))
				{
					// Bothways.
					waypoint->AddPath (m_waypoints[index], WaypointNode::ConnectionFlag_Normal);
					m_waypoints[index]->AddPath (waypoint, WaypointNode::ConnectionFlag_Normal);
				}
			}
			else if ((IsNodeReachable (waypoint->m_origin, m_waypoints[index]->m_origin) || IsNodeReachable (m_waypoints[index]->m_origin, waypoint->m_origin))/* && Math::AreEqual (waypoint->m_origin.z + 16.0f, m_waypoints[index]->m_origin.z, 64.0f)*/ && (distanceSquared = m_waypoints[index]->m_origin.GetDistanceSquared (waypoint->m_origin)) < minDistanceSquared)	// check if the waypoint is reachable from the new one
			{
				destinationWaypoint = m_waypoints[index];
				minDistanceSquared = distanceSquared;
			}
		}

		if (destinationWaypoint != NULL)
		{
			// check if the waypoint is reachable from the new one (one-way)
			if (IsNodeReachable (waypoint->m_origin, destinationWaypoint->m_origin))
				waypoint->AddPath (destinationWaypoint, WaypointNode::ConnectionFlag_Normal);

			// check if the new one is reachable from the waypoint (other way)
			if (IsNodeReachable (destinationWaypoint->m_origin, waypoint->m_origin))
				destinationWaypoint->AddPath (waypoint, WaypointNode::ConnectionFlag_Normal);
		}
	}
	else for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)	// calculate all the paths to this new waypoint
	{
		// Reliability check.
		InternalAssert (m_waypoints[index]->IsValid ());

		// skip the waypoint that was just added
		if (m_waypoints[index] == waypoint)
			continue;

		// check if the waypoint is reachable from the new one (one-way)
		if (IsNodeReachable (waypoint->m_origin, m_waypoints[index]->m_origin))
		{
			AddLogEntry (true, LogLevel_Default, false, "WaypointManager::CalculateWayNodesForWaypoint(%x): Add path from %x to %x.", waypoint, waypoint, m_waypoints[index]);

			waypoint->AddPath (m_waypoints[index], WaypointNode::ConnectionFlag_Normal);
		}

		// check if the new one is reachable from the waypoint (other way)
		if (IsNodeReachable (m_waypoints[index]->m_origin, waypoint->m_origin))
		{
			AddLogEntry (true, LogLevel_Default, false, "WaypointManager::CalculateWayNodesForWaypoint(%x): Add path from %x to %x.", waypoint, m_waypoints[index], waypoint);

			m_waypoints[index]->AddPath (waypoint, WaypointNode::ConnectionFlag_Normal);
		}
	}
}
#if 0
/*inline */void WaypointManager::DrawIncomingNodesForWaypoint (const Waypoint *const waypoint, const Client *const client) const
{
	// Reliability check.
	InternalAssert (waypoint->IsValid ());

	for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)
		if (m_waypoints[index]->IsValid () && m_waypoints[index]->IsConnected (waypoint) && !waypoint->IsConnected (m_waypoints[index]))
			client->DrawLine (m_waypoints[index]->m_origin, waypoint->m_origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 192u, 96u), 200u, 0u, 10u);
}
#endif	// if 0