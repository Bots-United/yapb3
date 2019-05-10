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
// WaypointNode.cpp
//
// Class: WaypointNode
//
// Version: $ID:$
//

#include <Core.hpp>

void WaypointNode::Save (BaseFile &/*file*/) const
{
	// saves the visibility table (after recalculating the visibility - when *.pwf file was changed)

	if (g_server->GetWaypointManager ().GetWaypointsNumber () == 0u)
		return;
/*
	if (m_visibilityTable == NULL || m_createVisibilityTable)
	{
		AddLogEntry (false, LogLevel_Critical, false, "Can't save visiblity table - bad data.");

		return;
	}

	const FileHeader_t &header (g_server->GetWaypointManager ().GetWaypointsNumber ());

	AddLogEntry (true, LogLevel_Default, false, "Compressing and saving visibility table... this may take a while!");

	if (Compressor::Compress (g_server->GetWaypointManager ().GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".vis", static_cast <const unsigned char *> (static_cast <const void *> (&header)), sizeof (FileHeader_t), reinterpret_cast <const unsigned char *> (m_visibilityTable), MaximumWaypoints * (MaximumWaypoints / 4u)/* * sizeof (unsigned char)*//*) == -1)
	{
		AddLogEntry (true, LogLevel_Error, false, "Couldn't save visibility table!");

		return;
	}

	AddLogEntry (true, LogLevel_Default, false, "Visibility table successfully saved...");*/
}

void WaypointNode::Load (const BaseFile &/*file*/)
{
	// initiates the visibility table (at the map start)

	if (g_server->GetWaypointManager ().GetWaypointsNumber () == 0u)
		return;
/*
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

	if (Compressor::Uncompress (g_server->GetWaypointManager ().GetWaypointsDirectory () + "data/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".vis", sizeof (FileHeader_t), reinterpret_cast <unsigned char *> (m_visibilityTable), MaximumWaypoints * (MaximumWaypoints / 4u)/* * sizeof (unsigned char)*//*) == -1)
	{
		AddLogEntry (true, LogLevel_Error, false, "Failed to decode visiblity table, visiblity table will be rebuilded.");

		m_createVisibilityTable = true;

		return;
	}

	m_createVisibilityTable = false;

	AddLogEntry (true, LogLevel_Default, false, "Visibility table successfully loaded from file.");*/
}

void WaypointNode::Draw (void) const
{
	// jump connection (crimson color), OR twoway connection (yellow color), OR oneway connection (white color)
	if (/*m_type == ConnectionType_Outgoing*/m_sourceWaypoint->IsOnlyOutgoingConnected (m_destinationWaypoint))
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_sourceWaypoint->GetOrigin (), m_destinationWaypoint->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, m_travelFlag == ConnectionFlag_Duck ? Color <> (255u, 255u, 0u) : m_travelFlag == ConnectionFlag_Jump ? Color <> (255u, 0u, 128u) : m_travelFlag == ConnectionFlag_DoubleJump ? Color <> (0u, 0u, 128u) : Color <> (255u, 255u, 255u), 200u, 0u, 10u);
	else if (/*m_type == ConnectionType_Incoming*/m_sourceWaypoint->IsOnlyIncomingConnected (m_destinationWaypoint))
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_destinationWaypoint->GetOrigin (), m_sourceWaypoint->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, m_travelFlag == ConnectionFlag_Duck ? Color <> (255u, 255u, 0u) : m_travelFlag == ConnectionFlag_Jump ? Color <> (255u, 0u, 128u) : m_travelFlag == ConnectionFlag_DoubleJump ? Color <> (0u, 0u, 128u) : Color <> (0u, 192u, 96u), 200u, 0u, 10u);
	else	// Bothways
	{
		const Math::Vector3D offset (0.0f, 0.0f, 2.5f);

		if (m_travelFlag == ConnectionFlag_Duck)
		{
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_sourceWaypoint->GetOrigin () + offset, m_destinationWaypoint->GetOrigin () + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 255u, 0u), 200u, 0u, 10u);
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_destinationWaypoint->GetOrigin () - offset, m_sourceWaypoint->GetOrigin () - offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 255u, 0u), 200u, 0u, 10u);
		}
		else if (m_travelFlag == ConnectionFlag_Jump)
		{
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_sourceWaypoint->GetOrigin () + offset, m_destinationWaypoint->GetOrigin () + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 128u), 200u, 0u, 10u);
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_destinationWaypoint->GetOrigin () - offset, m_sourceWaypoint->GetOrigin () - offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 128u), 200u, 0u, 10u);
		}
		else if (m_travelFlag == ConnectionFlag_DoubleJump)
		{
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_sourceWaypoint->GetOrigin () + offset, m_destinationWaypoint->GetOrigin () + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 0u, 128u), 200u, 0u, 10u);
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_destinationWaypoint->GetOrigin () - offset, m_sourceWaypoint->GetOrigin () - offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 0u, 128u), 200u, 0u, 10u);
		}
		else
		{
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_sourceWaypoint->GetOrigin () + offset, m_destinationWaypoint->GetOrigin () + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 200u, 0u, 10u);
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (m_destinationWaypoint->GetOrigin () - offset, m_sourceWaypoint->GetOrigin () - offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 200u, 0u, 10u);
		}
	}
}
#if 0
void WaypointNode::AddPath (const ConnectionType_t connectionType)
{
	// don't allow paths get connected twice
/*	if (m_type == connectionType)
	{
		AddLogEntry (true, LogLevel_Warning, false, "Denied path creation from %x to %x (path with connection type %s already exists).", m_sourceWaypoint, m_destinationWaypoint, m_type == ConnectionType_Outgoing ? "outgoing" : m_type == ConnectionType_Incoming ? "incoming" : "bothways");

		return;
	}
*/
	switch (connectionType)
	{
		case ConnectionType_Outgoing:
			if (g_server->GetWaypointManager ().IsNodeReachable (m_sourceWaypoint->m_origin, m_destinationWaypoint->m_origin))
			{
				m_type = m_type == ConnectionType_Incoming ? ConnectionType_Bothways : ConnectionType_Outgoing;

				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): path from %x to %x created with type %s.", m_sourceWaypoint, m_destinationWaypoint, m_type == ConnectionType_Outgoing ? "outgoing" : m_type == ConnectionType_Incoming ? "incoming" : "bothways");

				for (unsigned char index (0u); index < m_destinationWaypoint->m_paths.GetElementNumber (); ++index)
					if (m_destinationWaypoint->m_paths[index]->m_sourceWaypoint == m_destinationWaypoint && m_destinationWaypoint->m_paths[index]->m_destinationWaypoint == m_sourceWaypoint)
					{
						m_destinationWaypoint->m_paths[index]->m_type = m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Outgoing ? ConnectionType_Bothways : ConnectionType_Incoming;

						AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): path from %x to %x created with type %s.", m_destinationWaypoint, m_sourceWaypoint, m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Outgoing ? "outgoing" : m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Incoming ? "incoming" : "bothways");

						return;
					}

				// In case if destination waypoint has no paths....

				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): Creation path from %x to %x with type %s, as m_destinationWaypoint has no m_paths.", m_destinationWaypoint, m_sourceWaypoint, connectionType == WaypointNode::ConnectionType_Outgoing ? "outgoing" : connectionType == WaypointNode::ConnectionType_Incoming ? "incoming" : "bothways");
/*
				WaypointNode *const node = new WaypointNode (m_destinationWaypoint, m_sourceWaypoint, m_travelFlag, ConnectionType_Outgoing);

				// Reliability check.
				if (node == NULL)
				{
					// Error allocating memory!
					AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint node!");

					return;
				}

				m_destinationWaypoint->m_paths += node;*/
			}
			else
				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): node from %x to %x is unreachable, requested connection type: outgoing.", m_sourceWaypoint, m_destinationWaypoint);

			return;

		case ConnectionType_Incoming:
			if (g_server->GetWaypointManager ().IsNodeReachable (m_destinationWaypoint->m_origin, m_sourceWaypoint->m_origin))
			{
				m_type = m_type == ConnectionType_Outgoing ? ConnectionType_Bothways : ConnectionType_Incoming;

				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): path from %x to %x created with type %s.", m_sourceWaypoint, m_destinationWaypoint, m_type == ConnectionType_Outgoing ? "outgoing" : m_type == ConnectionType_Incoming ? "incoming" : "bothways");

				for (unsigned char index (0u); index < m_destinationWaypoint->m_paths.GetElementNumber (); ++index)
					if (m_destinationWaypoint->m_paths[index]->m_sourceWaypoint == m_destinationWaypoint && m_destinationWaypoint->m_paths[index]->m_destinationWaypoint == m_sourceWaypoint)
					{
						m_destinationWaypoint->m_paths[index]->m_type = m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Incoming ? ConnectionType_Bothways : ConnectionType_Outgoing;

						AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): path from %x to %x created with type %s.", m_destinationWaypoint, m_sourceWaypoint, m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Outgoing ? "outgoing" : m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Incoming ? "incoming" : "bothways");

						return;
					}

				// In case if destination waypoint has no paths....

				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): Creation path from %x to %x with type %s, as m_destinationWaypoint has no m_paths.", m_destinationWaypoint, m_sourceWaypoint, connectionType == WaypointNode::ConnectionType_Outgoing ? "outgoing" : connectionType == WaypointNode::ConnectionType_Incoming ? "incoming" : "bothways");
/*
				WaypointNode *const node = new WaypointNode (m_destinationWaypoint, m_sourceWaypoint, m_travelFlag, ConnectionType_Incoming);

				// Reliability check.
				if (node == NULL)
				{
					// Error allocating memory!
					AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint node!");

					return;
				}

				m_destinationWaypoint->m_paths += node;*/
			}
			else
				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): node from %x to %x is unreachable, requested connection type: incoming.", m_destinationWaypoint, m_sourceWaypoint);

			return;

		case ConnectionType_Bothways:
			if (g_server->GetWaypointManager ().IsNodeReachable (m_sourceWaypoint->m_origin, m_destinationWaypoint->m_origin) && g_server->GetWaypointManager ().IsNodeReachable (m_destinationWaypoint->m_origin, m_sourceWaypoint->m_origin))
			{
				m_type = ConnectionType_Bothways;

				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): path from %x to %x created with type %s.", m_sourceWaypoint, m_destinationWaypoint, m_type == ConnectionType_Outgoing ? "outgoing" : m_type == ConnectionType_Incoming ? "incoming" : "bothways");

				for (unsigned char index (0u); index < m_destinationWaypoint->m_paths.GetElementNumber (); ++index)
					if (m_destinationWaypoint->m_paths[index]->m_sourceWaypoint == m_destinationWaypoint && m_destinationWaypoint->m_paths[index]->m_destinationWaypoint == m_sourceWaypoint)
					{
						m_destinationWaypoint->m_paths[index]->m_type = ConnectionType_Bothways;

						AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): path from %x to %x and from %x to %x created with type %s.", m_destinationWaypoint, m_sourceWaypoint, m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Outgoing ? "outgoing" : m_destinationWaypoint->m_paths[index]->m_type == ConnectionType_Incoming ? "incoming" : "bothways");

						return;
					}

				// In case if destination waypoint has no paths....

				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): Creation path from %x to %x with type %s, as m_destinationWaypoint has no m_paths.", m_destinationWaypoint, m_sourceWaypoint, connectionType == WaypointNode::ConnectionType_Outgoing ? "outgoing" : connectionType == WaypointNode::ConnectionType_Incoming ? "incoming" : "bothways");
/*
				WaypointNode *const node (new WaypointNode (m_destinationWaypoint, m_sourceWaypoint, m_travelFlag, ConnectionType_Bothways));

				// Reliability check.
				if (node == NULL)
				{
					// Error allocating memory!
					AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint node!");

					return;
				}

				m_destinationWaypoint->m_paths += node;*/
			}
			else
				AddLogEntry (true, LogLevel_Default, false, "WaypointNode::AddPath(): nodes from %x to %x are unreachable, requested connection type: bothways.", m_sourceWaypoint, m_destinationWaypoint, m_destinationWaypoint, m_sourceWaypoint);

			return;
	}
}
#endif	// if 0