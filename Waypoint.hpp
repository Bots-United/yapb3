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
// Waypoint.hpp
//
// Class: Waypoint
//
// Description: Waypoint operation class.
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef WAY_POINT_INCLUDED
#define WAY_POINT_INCLUDED

class Waypoint : public BaseNavigationElement
{
	friend WaypointNode;
	friend WaypointManager;
	friend WaypointLocationsTable;
	friend WaypointVisibilityTable;

	//
	// Group: Members.
	//
	public:
		// defines for waypoint flags field (32 bits are available)
		enum Flag_t
		{
			Flag_None,

			Flag_Lift             = BIT (0u),	// wait for lift to be down before approaching this waypoint
			Flag_Crossing         = BIT (1u),	// a target waypoint
			Flag_Goal             = BIT (2u),	// mission goal point (bomb, hostage etc.)
			Flag_Ladder           = BIT (3u),	// waypoint is on ladder
			Flag_Rescue           = BIT (4u),	// waypoint is a hostage rescue point
			Flag_Camp             = BIT (5u),	// waypoint is a camping point
			Flag_NoHostage        = BIT (6u),	// only use this waypoint if no hostage
			Flag_DoubleJump       = BIT (7u),	// bot help's another player (requster) to get somewhere (using djump)
			Flag_Sniper           = BIT (8u),	// it's a specific sniper point
			Flag_Terrorist        = BIT (9u),	// it's a specific terrorist point
			Flag_CounterTerrorist = BIT (10u),	// it's a specific counter-terrorist point
			Flag_Crouch           = BIT (11u)	// addional flag where bot must crouch to reach this waypoint
		};

		static const unsigned char MaximumNodes = 8u;

	//
	// Group: Protected members.
	//
	protected:
		// bot known file headers
		static const DynamicString HeaderFileExtension;
		static const unsigned char HeaderFileVersion = 1u;

		Math::Vector3D                               m_origin;			// location
		unsigned int                                 m_flags;			// button, lift, flag, health, ammo, etc.
		float                                        m_radius;			// maximum distance waypoint origin can be varied
		unsigned char                                m_illumination;	// illumination level

		DynamicArray <WaypointNode *, unsigned char> m_paths;			// paths to other waypoints.

		struct Visibility_t
		{
			unsigned short stand;	// how many stand visibilities this waypoint has
			unsigned short crouch;	// how many crouch visibilities this waypoint has

			inline Visibility_t (void) : stand (0u), crouch (0u) { /*VOID*/ }
		}                                            m_visibility;

		float                                        m_displayTime;	// Last time this waypoint has been displayed.

	//
	// Group: (Con/De)structors.
	//
	public:
		Waypoint (const Math::Vector3D &origin, const unsigned int flags, const unsigned char illumination);

		virtual inline ~Waypoint (void)
		{
			// This function frees all waypoints paths.

			// Free all the memory allocated for all waypoints paths....
			m_paths.DeleteAll ();	// Delete it from the array.
		}

	//
	// Group: Private functions.
	//
	private:
		void CalculateWayZone (void);

	//
	// Group: Functions.
	//
	public:
		inline const bool IsWaypoint           (void) const { return true; }
		inline const bool IsBaseNavigationArea (void) const { return false; }
		inline const bool IsNavigationLadder   (void) const { return false; }

		//
		// Function: IsValid
		//
		// Description: Checks whether waypoint is valid.
		//
		// Returns: True if waypoint is valid, false otherwise.
		//
		inline const bool            IsValid                   (void) const { return this != NULL; }

		inline const unsigned int    GetFlags                  (void) const { return m_flags; }
		inline const Math::Vector3D &GetOrigin                 (void) const { return m_origin; }
		inline const float           GetRadius                 (void) const { return m_radius; }
		inline const unsigned char   GetIllumination           (void) const { return m_illumination; }

		inline const unsigned short  GetVisibilityStand        (void) const { return m_visibility.stand; }
		inline const unsigned short  GetVisibilityCrouch       (void) const { return m_visibility.crouch; }
/*
		inline const bool IsConnected (const Waypoint *const waypoint, const WaypointNode::ConnectionType_t type = WaypointNode::ConnectionType_Outgoing) const
		{
			// This function checks if this waypoint is connected with other waypoint.

			for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
				if (m_paths[index]->m_destinationWaypoint == waypoint && m_paths[index]->m_type == type)
					return true;

			return false;
		}*/
		inline const bool IsOutgoingConnected (const Waypoint *const waypoint) const
		{
			// This function checks if this waypoint is connected with other waypoint with outgoing connection type.

			for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
				if (m_paths[index]->m_destinationWaypoint == waypoint)
					return true;

			return false;
		}
		inline const bool IsOnlyOutgoingConnected (const Waypoint *const waypoint) const
		{
			// This function checks if this waypoint is connected with other waypoint with outgoing connection type.

			return IsOutgoingConnected (waypoint) && !IsIncomingConnected (waypoint);
		}
		inline const bool IsIncomingConnected (const Waypoint *const waypoint) const
		{
			// This function checks if this waypoint is connected with other waypoint with incoming connection type.

			for (unsigned char index (0u); index < waypoint->m_paths.GetElementNumber (); ++index)
				if (waypoint->m_paths[index]->m_destinationWaypoint == this)
					return true;

			return false;
		}
		inline const bool IsOnlyIncomingConnected (const Waypoint *const waypoint) const
		{
			// This function checks if this waypoint is connected with other waypoint with incoming connection type.

			return IsIncomingConnected (waypoint) && !IsOutgoingConnected (waypoint);
		}
		inline const bool IsBothwaysConnected (const Waypoint *const waypoint) const
		{
			// This function checks if this waypoint is connected with other waypoint with bothways connection type.

			return IsOutgoingConnected (waypoint) && IsIncomingConnected (waypoint);
		}

		inline const bool IsConnectedWithTravelFlag (const Waypoint *const waypoint, const WaypointNode::ConnectionFlag_t travelFlag = WaypointNode::ConnectionFlag_Normal) const
		{
			// This function checks if this waypoint is connected with other waypoint, with specified travel flag.

			for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
				if (m_paths[index]->m_destinationWaypoint == waypoint && m_paths[index]->m_travelFlag == travelFlag)
					return true;

			return false;
		}

		inline const bool IsConnectedWithTravelFlag (const WaypointNode::ConnectionFlag_t travelFlag = WaypointNode::ConnectionFlag_Normal) const
		{
			// This function checks if this waypoint is connected with any other waypoint, with specified travel flag.

			for (unsigned char index (0u); index < m_paths.GetElementNumber (); ++index)
				if (m_paths[index]->m_travelFlag == travelFlag)
					return true;

			return false;
		}

		inline const bool IsForTeam (const HalfLifeEngine::SDK::Constants::TeamID_t team)
		{
			switch (team)
			{
				case HalfLifeEngine::SDK::Constants::TeamID_Terrorist:
					return (m_flags & Flag_Terrorist) != 0;

				case HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist:
					return (m_flags & Flag_CounterTerrorist) != 0;

				default:
					return true;
			}
		}

		const DynamicString GetInfo (void) const;

		void AddPath (Waypoint *const other, const WaypointNode::ConnectionFlag_t connectionFlag);

		virtual const bool Save    (BaseFile &/*file*/) const { return false; }	/// @todo !!!
		virtual void       SaveXML (BaseFile &file) const;
		virtual const bool Load    (const BaseFile &/*file*/) { return false; }	/// @todo !!!

		virtual void Draw (const bool drawPaths);
/*	DO WE NEED THIS???????????????????????????????????????????????????
		inline const unsigned short GetIndex (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return m_waypoints.GetElementIndex (this);
		}*/
};

inline WaypointNode::WaypointNode (Waypoint *const sourceWaypoint, Waypoint *const destinationWaypoint, const ConnectionFlag_t travelFlag/*, const ConnectionType_t type*/) :
	m_sourceWaypoint (sourceWaypoint),
	m_destinationWaypoint (destinationWaypoint),
	m_travelFlag (travelFlag),
	m_distance (static_cast <const unsigned short> (m_sourceWaypoint->m_origin.GetDistance (m_destinationWaypoint->m_origin)))/*,
	m_type (type)*/
{ /*VOID*/ }

class Waypoint_Camp : public Waypoint
{
	//
	// Group: Private members.
	//
	private:
		Math::Vector2D m_campStart;
		Math::Vector2D m_campEnd;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Waypoint_Camp (const Math::Vector3D &origin, const unsigned int flags, const unsigned char illumination, const Math::Vector2D &campStart, const Math::Vector2D &campEnd) :
			Waypoint (origin, flags, illumination),

			m_campStart (campStart),
			m_campEnd (campEnd)
		{ /*VOID*/ }

		virtual inline ~Waypoint_Camp (void) { /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		inline const Math::Vector2D &GetCampStartPosition (void) const { return m_campStart; }
		inline const Math::Vector2D &GetCampEndPosition   (void) const { return m_campEnd; }
};

#endif	// ifndef WAY_POINT_INCLUDED