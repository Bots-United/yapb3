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

const DynamicString Waypoint::HeaderFileExtension = "YaPB_Waypoint";

Waypoint::Waypoint (const Math::Vector3D &origin, const unsigned int flags, const unsigned char illumination) :
	BaseNavigationElement (),

	m_origin (origin),	// store the origin (location) of this waypoint
	m_flags (flags),
	m_radius (0.0f),
	m_illumination (illumination),

	m_displayTime (0.0f)	// set the time that this waypoint was originally displayed...
{
	g_server->GetWaypointManager ().CalculateWayNodesForWaypoint (this);

	CalculateWayZone ();	// Calculate the way zone of this waypoint.
}

void Waypoint::SaveXML (BaseFile &file) const
{
	file.PrintFormat ("\t\t\t<waypoint>%x</waypoint>\n", this);
	file.PrintFormat ("\t\t\t<flags>%u</flags>\n", m_flags);
	file.PrintFormat ("\t\t\t<origin: x=\"%.2f\" y=\"%.2f\" z=\"%.2f\"/>\n", m_origin.x, m_origin.y, m_origin.z);
	file.PrintFormat ("\t\t\t<radius>%.2f</radius>\n", m_radius);
	file.PrintFormat ("\t\t\t<illumination>%u</illumination>\n", m_illumination);
/*
	if (m_flags & Flag_Camp)
	{
		const Waypoint_Camp *const campWaypoint (static_cast <const Waypoint_Camp *> (this));

		// Reliability check.
		InternalAssert (campWaypoint->IsValid ());

		file.PrintFormat ("\t\t\t<camp start: x=\"%.2f\" y=\"%.2f\"/>\n", campWaypoint->GetCampStartPosition ().x, campWaypoint->GetCampStartPosition ().y);
		file.PrintFormat ("\t\t\t<camp end: x=\"%.2f\" y=\"%.2f\"/>\n", campWaypoint->GetCampEndPosition ().x, campWaypoint->GetCampEndPosition ().y);
	}
*/
	file.PrintFormat ("\t\t\t<connections size=\"%u\">\n", m_paths.GetElementNumber ());

	for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
	{
		file.PrintFormat ("\t\t\t\t<connection ID=\"%u\">\n", index + 1u);

		m_paths[index]->SaveXML (file);
	}

	file.Print ("\t\t\t</connections>\n");
	file.Print ("\t\t</waypoint>\n");
}

void Waypoint::AddPath (Waypoint *const other, const WaypointNode::ConnectionFlag_t connectionFlag)
{
	// adds a path between 2 waypoints

	if (!other->IsValid ())
	{
//		AddLogEntry (true, LogLevel_Warning, false, "Denied path creation from %x to %x (one of the chosen waypoints out of range).", this, other);

		return;
	}

	// don't allow paths get connected to the same waypoint
	if (this == other)
	{
		AddLogEntry (true, LogLevel_Warning, false, "Denied path creation from %x to %x (same waypoint).", this, other);

		return;
	}

	if (m_paths.GetElementNumber () == MaximumNodes/* || other->m_paths.GetElementNumber () == MaximumNodes*/)
	{
		AddLogEntry (true, LogLevel_Warning, false, "Denied path creation from %x to %x (maximum paths reached (%u)).", this, other, MaximumNodes);

		return;
	}

	for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
		if (m_paths[index]->m_destinationWaypoint == other)
		{
/*			for (unsigned char index2 (0u); index2 < other->m_paths.GetElementNumber (); ++index2)
				if (other->m_paths[index2]->m_destinationWaypoint == this)
				{
					AddLogEntry (true, LogLevel_Warning, false, "Path from %x to %x already exists, changing old connection flag %s to the new %s.", this, other, m_paths[index]->m_travelFlag == WaypointNode::ConnectionFlag_Duck ? "duck" : m_paths[index]->m_travelFlag == WaypointNode::ConnectionFlag_Jump ? "jump" : m_paths[index]->m_travelFlag == WaypointNode::ConnectionFlag_DoubleJump ? "double jump" : "normal", connectionFlag == WaypointNode::ConnectionFlag_Duck ? "duck" : connectionFlag == WaypointNode::ConnectionFlag_Jump ? "jump" : connectionFlag == WaypointNode::ConnectionFlag_DoubleJump ? "double jump" : "normal");

					m_paths[index]->m_travelFlag = other->m_paths[index2]->m_travelFlag = connectionFlag;
					m_paths[index]->m_type = m_paths[index]->m_type == WaypointNode::ConnectionType_Incoming ? WaypointNode::ConnectionType_Bothways : WaypointNode::ConnectionType_Outgoing;
					other->m_paths[index2]->m_type = other->m_paths[index2]->m_type == WaypointNode::ConnectionType_Outgoing ? WaypointNode::ConnectionType_Bothways : WaypointNode::ConnectionType_Incoming;

					return;
				}

			AddLogEntry (false, LogLevel_Critical, false, "UNKNOWN ERROR!");
*/
			AddLogEntry (true, LogLevel_Warning, false, "Path from %x to %x already exists, changing old connection flag %s to the new %s.", this, other, m_paths[index]->m_travelFlag == WaypointNode::ConnectionFlag_Duck ? "duck" : m_paths[index]->m_travelFlag == WaypointNode::ConnectionFlag_Jump ? "jump" : m_paths[index]->m_travelFlag == WaypointNode::ConnectionFlag_DoubleJump ? "double jump" : "normal", connectionFlag == WaypointNode::ConnectionFlag_Duck ? "duck" : connectionFlag == WaypointNode::ConnectionFlag_Jump ? "jump" : connectionFlag == WaypointNode::ConnectionFlag_DoubleJump ? "double jump" : "normal");

			m_paths[index]->m_travelFlag = connectionFlag;

			return;
		}

	WaypointNode *newNode (new WaypointNode (this, other, connectionFlag/*, WaypointNode::ConnectionType_Outgoing*/));

	// Reliability check.
	if (newNode == NULL)
	{
		// Error allocating memory!
		AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint node!");

		return;
	}

	m_paths += newNode;
/*
	newNode = new WaypointNode (other, this, connectionFlag, WaypointNode::ConnectionType_Incoming);

	// Reliability check.
	if (newNode == NULL)
	{
		// Error allocating memory!
		AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint node!");

		return;
	}

	other->m_paths += newNode;
*/
/*	if (m_paths.IsEmpty ())
	{
		AddLogEntry (true, LogLevel_Default, false, "Waypoint::AddPath(): Creation new path from %x to %x.", this, other);

		WaypointNode *const newNode (new WaypointNode (this, other, connectionFlag, WaypointNode::ConnectionType_Outgoing));

		// Reliability check.
		if (newNode == NULL)
		{
			// Error allocating memory!
			AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint node!");

			return;
		}

		m_paths += newNode;
	}
	else for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
		if (m_paths[index]->m_destinationWaypoint == other)
		{
//			InternalAssert (m_paths[index]->m_type == WaypointNode::ConnectionType_Outgoing);
//			InternalAssert (m_paths[index]->m_travelFlag == connectionFlag);
			InternalAssert (!other->m_paths.IsEmpty ());

			m_paths[index]->m_type = m_paths[index]->m_type == WaypointNode::ConnectionType_Incoming ? WaypointNode::ConnectionType_Bothways : WaypointNode::ConnectionType_Outgoing;
			m_paths[index]->m_travelFlag = connectionFlag;

			AddLogEntry (true, LogLevel_Warning, false, "Path from %x to %x with travel flag '%s' already exists.", this, other, connectionFlag == WaypointNode::ConnectionFlag_Duck ? "duck" : connectionFlag == WaypointNode::ConnectionFlag_Jump ? "jump" : connectionFlag == WaypointNode::ConnectionFlag_DoubleJump ? "double jump" : "normal");

//			return;
		}

	if (other->m_paths.IsEmpty ())
	{
		AddLogEntry (true, LogLevel_Default, false, "Waypoint::AddPath(): Creation new path from %x to %x.", other, this);

		WaypointNode *const newNode (new WaypointNode (other, this, connectionFlag, WaypointNode::ConnectionType_Incoming));

		// Reliability check.
		if (newNode == NULL)
		{
			// Error allocating memory!
			AddLogEntry (true, LogLevel_Error, false, "Can't allocate enough memory for new waypoint node!");

			return;
		}

		other->m_paths += newNode;
	}
	else for (unsigned char index (0u); index < other->m_paths.GetElementNumber (); ++index)
		if (other->m_paths[index]->m_destinationWaypoint == this)
		{
//			InternalAssert (other->m_paths[index]->m_type == WaypointNode::ConnectionType_Incoming);
//			InternalAssert (other->m_paths[index]->m_travelFlag == connectionFlag);
			InternalAssert (!m_paths.IsEmpty ());

			other->m_paths[index]->m_type = other->m_paths[index]->m_type == WaypointNode::ConnectionType_Outgoing ? WaypointNode::ConnectionType_Bothways : WaypointNode::ConnectionType_Incoming;
			other->m_paths[index]->m_travelFlag = connectionFlag;

			AddLogEntry (true, LogLevel_Warning, false, "Path from %x to %x with travel flag '%s' already exists.", other, this, connectionFlag == WaypointNode::ConnectionFlag_Duck ? "duck" : connectionFlag == WaypointNode::ConnectionFlag_Jump ? "jump" : connectionFlag == WaypointNode::ConnectionFlag_DoubleJump ? "double jump" : "normal");
		}*/
}

void Waypoint::CalculateWayZone (void)
{
	// Calculates "way zones" for the nearest waypoint to pentedict (meaning a dynamic distance area to vary waypoint origin).

	if ((m_flags & (Flag_Ladder | Flag_Goal | Flag_Camp | Flag_Rescue | Flag_Crouch | Flag_DoubleJump))/* || m_learnJumpWaypoint*/)
	{
		m_radius = 0.0f;

		return;
	}

	for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
	{
		if (!(m_paths[index]->m_destinationWaypoint->m_flags & Flag_Ladder))
			continue;

		m_radius = 0.0f;

		return;
	}

	Math::Angles2D direction;
	Math::Vector3D radiusStart, radiusEnd, dropStart, dropEnd;
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
	bool wayBlocked = false;

	for (unsigned char scanDistance (16u), circleRadius; scanDistance < 128u; scanDistance += 16u)
	{
		Math::Angles3D::ZeroValue.MakeForwardVector ();

		direction = (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * scanDistance).ToAngles2D ();

		m_radius = scanDistance;

		for (circleRadius = 0u; circleRadius < 180u; circleRadius += 5u)
		{
			direction.MakeForwardVector ();

			radiusStart = m_origin - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * scanDistance;
			radiusEnd = m_origin + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * scanDistance;

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (radiusStart, radiusEnd, true, HalfLifeEngine::SDK::Constants::Hull_Head, NULL, traceResult);

			if (traceResult.fraction < 1.0f)
			{
				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (radiusStart, radiusEnd, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

				if (traceResult.hitEntity->GetClassName ().CompareWithCaseCheck ("func_door", GET_STATIC_STRING_LENGTH ("func_door")) == 0)
				{
					wayBlocked = true;

					m_radius = 0.0f;

					break;
				}

				wayBlocked = true;

				m_radius -= HalfLifeEngine::SDK::Constants::HalfHumanWidth;

				break;
			}

			dropStart = m_origin + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * scanDistance;
			dropEnd = dropStart - Math::Vector3D (0.0f, 0.0f, scanDistance + 60.0f);

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (dropStart, dropEnd, true, HalfLifeEngine::SDK::Constants::Hull_Head, NULL, traceResult);

			if (traceResult.fraction == 1.0f)
			{
				wayBlocked = true;

				m_radius -= HalfLifeEngine::SDK::Constants::HalfHumanWidth;

				break;
			}

			dropStart = m_origin - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * scanDistance;
			dropEnd = dropStart - Math::Vector3D (0.0f, 0.0f, scanDistance + 60.0f);

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (dropStart, dropEnd, true, HalfLifeEngine::SDK::Constants::Hull_Head, NULL, traceResult);

			if (traceResult.fraction == 1.0f)
			{
				wayBlocked = true;

				m_radius -= HalfLifeEngine::SDK::Constants::HalfHumanWidth;

				break;
			}

			radiusEnd.z += 34.0f;

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (radiusStart, radiusEnd, true, HalfLifeEngine::SDK::Constants::Hull_Head, NULL, traceResult);

			if (traceResult.fraction < 1.0f)
			{
				wayBlocked = true;

				m_radius -= HalfLifeEngine::SDK::Constants::HalfHumanWidth;

				break;
			}

			direction.yaw = Math::AngleNormalize (direction.yaw + circleRadius);
		}

		if (wayBlocked)
			break;
	}

	m_radius -= HalfLifeEngine::SDK::Constants::HalfHumanWidth;

	if (m_radius < 0.0f)
		m_radius = 0.0f;
}

const DynamicString Waypoint::GetInfo (void) const
{
	// This function returns path information for this waypoint.

	const bool hasJumpNodes (IsConnectedWithTravelFlag (WaypointNode::ConnectionFlag_Jump));

	if (m_flags == 0u && !hasJumpNodes)
		return " (none)";

	// Else....

	DynamicString info;

	if (m_flags & Flag_Terrorist)
		info += " TERRORIST";

	if (m_flags & Flag_CounterTerrorist)
		info += " CT";

	if (m_flags & Flag_Lift)
		info += " LIFT";

	if (m_flags & Flag_Crouch)
		info += " CROUCH";

	if (m_flags & Flag_Crossing)
		info += " CROSSING";

	if (m_flags & Flag_Camp)
		info += " CAMP";

	if (m_flags & Flag_Sniper)
		info += " SNIPER";

	if (m_flags & Flag_Goal)
		info += " GOAL";

	if (m_flags & Flag_Ladder)
		info += " LADDER";

	if (m_flags & Flag_Rescue)
		info += " RESCUE";

	if (m_flags & Flag_DoubleJump)
		info += " JUMPHELP";

	if (m_flags & Flag_NoHostage)
		info += " NOHOSTAGE";

	if (hasJumpNodes)
		info += " JUMP";

	// return the message buffer.
	return info;
}

void Waypoint::Draw (const bool/* drawPaths*/)
{
	if (m_displayTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		return;

	const Human *const hostClient (g_server->GetHumanManager ()->GetHostClient ());

	// check the node height
	const float nodeHeight ((m_flags & Flag_Crouch) ? 36.0f : 72.0f);
	const float nodeHalfHeight (nodeHeight * 0.5f);
	const Math::Vector3D &origin ((m_flags & Flag_Crouch) ? m_origin : m_origin - Math::Vector3D (0.0f, 0.0f, 18.0f));

	// check the colors

	// colorize all other waypoints
	const Color <> &nodeColor
	(
		(m_flags & Flag_Camp) ?
			Color <> (0u, 255u, 255u) :
		(m_flags & Flag_Goal) ?
			Color <> (128u, 0u, 255u) :
		(m_flags & Flag_Ladder) ?
			Color <> (128u, 64u, 0u) :
		(m_flags & Flag_Rescue) ?
			Color <> (255u, 255u, 255u) :
		Color <> (0u, 255u, 0u)	// all waypoints are by default are green
	);

	// colorize additional flags
	const Color <> &nodeFlagColor
	(
		(m_flags & Flag_Sniper) ?
			Color <> (128u, 64u, 0u) :
		(m_flags & Flag_NoHostage) ?
			Color <> (255u, 255u, 255u) :
		(m_flags & Flag_Terrorist) ?
			Color <> (255u, 0u, 0u) :
		(m_flags & Flag_CounterTerrorist) ?
			Color <> (0u, 0u, 255u) :
		Color <>::ZeroValue	// no additional flags
	);

	// is this waypoint has no additional flags?, If so draw this waypoint without it...
	if (nodeFlagColor == Color <>::ZeroValue)
		hostClient->DrawLine (m_origin - Math::Vector3D (0.0f, 0.0f, nodeHalfHeight), m_origin + Math::Vector3D (0.0f, 0.0f, nodeHalfHeight), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 15u, 0u, Color <> (nodeColor), 250u, 0u, 10u);
	else
	{
		// draw basic path
		hostClient->DrawLine (m_origin - Math::Vector3D (0.0f, 0.0f, nodeHalfHeight), m_origin - Math::Vector3D (0.0f, 0.0f, nodeHalfHeight - nodeHeight * 0.75f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 14u, 0u, Color <> (nodeColor), 250u, 0u, 10u);

		// draw additional path
		hostClient->DrawLine (m_origin - Math::Vector3D (0.0f, 0.0f, nodeHalfHeight - nodeHeight * 0.75f), m_origin + Math::Vector3D (0.0f, 0.0f, nodeHalfHeight), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 14u, 0u, Color <> (nodeFlagColor), 250u, 0u, 10u);
	}

	if (this == hostClient->GetCurrentWaypoint () || this == g_server->GetWaypointManager ().GetFacingAtWaypoint ())
	{
		// draw the connections
/*		if (this == hostClient->GetCurrentWaypoint () && g_server->GetWaypointManager ().GetFacingAtWaypoint () != NULL && IsConnected (g_server->GetWaypointManager ().GetFacingAtWaypoint ()))
		{
			for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
				m_paths[index]->Draw ();
		}
		else if ()
			for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
				m_paths[index]->Draw ();*/
		for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
			m_paths[index]->Draw ();

		// now look for oneway incoming connections
//		g_server->GetWaypointManager ().DrawIncomingNodesForWaypoint (this, hostClient);

		// if radius is nonzero, draw a full circle
		if (m_radius > 0.0f)
		{
			const float squareRoot (Math::sqrtf (MATH_GET_SQUARED (m_radius) * 0.5f));

			// draw the radius circle

			hostClient->DrawLine (origin + Math::Vector2D (m_radius, 0.0f), origin + Math::Vector2D (squareRoot, -squareRoot), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);
			hostClient->DrawLine (origin + Math::Vector2D (squareRoot, -squareRoot), origin + Math::Vector2D (0.0f, -m_radius), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);

			hostClient->DrawLine (origin + Math::Vector2D (0.0f, -m_radius), origin + Math::Vector2D (-squareRoot, -squareRoot), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);
			hostClient->DrawLine (origin - Math::Vector2D (squareRoot, squareRoot), origin + Math::Vector2D (-m_radius, 0.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);

			hostClient->DrawLine (origin + Math::Vector2D (-m_radius, 0.0f), origin + Math::Vector2D (-squareRoot, squareRoot), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);
			hostClient->DrawLine (origin + Math::Vector2D (-squareRoot, squareRoot), origin + Math::Vector2D (0.0f, m_radius), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);

			hostClient->DrawLine (origin + Math::Vector2D (0.0f, m_radius), origin + Math::Vector2D (squareRoot, squareRoot), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);
			hostClient->DrawLine (origin + Math::Vector2D (squareRoot, squareRoot), origin + Math::Vector2D (m_radius, 0.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 0u, 255u), 200u, 0u, 10u);
		}
		else
		{
			const float squareRoot (Math::sqrtf (32.0f));

			hostClient->DrawLine (origin + Math::Vector2D (squareRoot, -squareRoot), origin + Math::Vector2D (-squareRoot, squareRoot), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 0u), 200u, 0u, 10u);
			hostClient->DrawLine (origin - Math::Vector2D (squareRoot, squareRoot), origin + Math::Vector2D (squareRoot, squareRoot), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (255u, 0u, 0u), 200u, 0u, 10u);
		}
	}

	// update timer to one second
	m_displayTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.0f;
}