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
// WaypointNode.hpp
//
// Class: WaypointNode
//
// Description: A link between waypoints.
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef WAY_POINT_NODE_INCLUDED
#define WAY_POINT_NODE_INCLUDED

class WaypointNode
{
	friend Waypoint;
	friend WaypointManager;

	//
	// Group: Members.
	//
	public:
		// defines waypoint connection types
		enum ConnectionType_t
		{
			ConnectionType_Outgoing,
			ConnectionType_Incoming,
			ConnectionType_Bothways
		};

		// defines for waypoint connection flags field (16 bits are available)
		enum ConnectionFlag_t
		{
			ConnectionFlag_Normal,		// No actions needed for travel thru this connection.
			ConnectionFlag_Duck,		// Must duck for this connection.
			ConnectionFlag_Jump,		// Must jump for this connection.
			ConnectionFlag_DoubleJump,	// Must use friend for this connection.
			ConnectionFlag_Teleport,

			ConnectionFlag_NoHostage	// Hostages can't pass thru this connection.
		};

	//
	// Group: Protected members.
	//
	protected:
		Waypoint         *m_sourceWaypoint;
		Waypoint         *m_destinationWaypoint;

		ConnectionFlag_t  m_travelFlag;				// travel flag of this link
		unsigned short    m_distance;				// length of the link
//		ConnectionType_t  m_type;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline WaypointNode (Waypoint *const sourceWaypoint, Waypoint *const destinationWaypoint, const ConnectionFlag_t travelFlag/*, const ConnectionType_t type*/);	// Note: This function declared in Waypoint.hpp

		virtual inline ~WaypointNode (void) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsValid
		//
		// Description: Checks whether path is valid.
		//
		// Returns: True if path is valid, false otherwise.
		//
		inline const bool             IsValid                   (void) const { return this != NULL; }

		inline const Waypoint *const  GetSourceWaypoint         (void) const { return m_sourceWaypoint; }
		inline const Waypoint *const  GetDestinationWaypoint    (void) const { return m_destinationWaypoint; }
		inline const ConnectionFlag_t GetTravelFlag             (void) const { return m_travelFlag; }
		inline const float            GetDistance               (void) const { return m_distance; }
//		inline const ConnectionType_t GetConnectionType         (void) const { return m_type; }

//		inline const unsigned short   GetVisibilityStand        (void) const { return m_visibility.stand; }
//		inline const unsigned short   GetVisibilityCrouch       (void) const { return m_visibility.crouch; }

		inline const bool             IsConnected               (const unsigned short index) const;

		virtual void Save           (BaseFile &file) const;
		virtual inline void SaveXML (BaseFile &file) const;
		virtual void Load           (const BaseFile &file);

//		void AddPath (const ConnectionType_t connectionType);

		virtual void Draw (void) const;
};

class WaypointNode_Jump : public WaypointNode
{
	//
	// Group: Private members.
	//
	private:
		Math::Vector3D m_travelVelocity;	// Speed for overcoming of this path.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline WaypointNode_Jump (Waypoint *const sourceWaypoint, Waypoint *const destinationWaypoint/*, const ConnectionType_t type*/, const Math::Vector2D &travelVelocity) :
			WaypointNode (sourceWaypoint, destinationWaypoint, ConnectionFlag_Jump/*, type*/),

			m_travelVelocity (travelVelocity)
		{ /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		inline const Math::Vector3D GetTravelVelocity (void) const { return m_travelVelocity; }
};

inline void WaypointNode::SaveXML (BaseFile &file) const
{
//		file.PrintFormat ("\t\t\t\t\t<index>%u</index>\n", m_sourceWaypoint->GetIndex ());
		file.PrintFormat ("\t\t\t\t\t<index>%x</index>\n", m_sourceWaypoint);
//		file.PrintFormat ("\t\t\t\t\t<index>%u</index>\n", m_destinationWaypoint->GetIndex ());
		file.PrintFormat ("\t\t\t\t\t<index>%x</index>\n", m_destinationWaypoint);
		file.PrintFormat ("\t\t\t\t\t<travel flag>%s</travel flag>\n", m_travelFlag == ConnectionFlag_Duck ? "duck" : m_travelFlag == ConnectionFlag_Jump ? "jump" : m_travelFlag == ConnectionFlag_DoubleJump ? "double jump" : "none");
		file.PrintFormat ("\t\t\t\t\t<distance>%u</distance>\n", m_distance);
//		file.PrintFormat ("\t\t\t\t\t<type>%s</type>\n", m_type == ConnectionType_Outgoing ? "outgoing" : m_type == ConnectionType_Incoming ? "incoming" : "bothways");
/*
		if (m_travelFlag == ConnectionFlag_Jump)
		{
			const WaypointNode_Jump *const node (static_cast <const WaypointNode_Jump *const> (this));

			// Reliability check.
			InternalAssert (node->IsValid ());

			file.PrintFormat ("\t\t\t\t\t<velocity: [x=\"%.2f\" y=\"%.2f\" z=\"%.2f\"/]>\n", node->GetTravelVelocity ().x, node->GetTravelVelocity ().y, node->GetTravelVelocity ().z);
		}
*/
		file.Print ("\t\t\t\t</connection>\n");
	}

#endif	// ifndef WAY_POINT_NODE_INCLUDED