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
// WaypointManager.hpp
//
// Class: WaypointManager
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef WAY_POINT_MANAGER_INCLUDED
#define WAY_POINT_MANAGER_INCLUDED

class WaypointManager
{
//	friend inline const unsigned short Waypoint::GetIndex (void) const;

	//
	// Group: Type definitions.
	//
	public:
		typedef StaticArray <Waypoint *, unsigned short, MaximumWaypoints> WaypointArray_t;

	//
	// Group: Private members.
	//
	private:
		WaypointArray_t          m_waypoints;

		WaypointDistancesTable   m_distancesTable;
		WaypointLocationsTable   m_locationsTable;
		WaypointVisibilityTable  m_visibilityTable;

		bool                     m_isEditModeActivated;
		bool                     m_isWaypointsChanged;
		float                    m_autoPathDistance;

		float                    m_showImportaintWaypointsTime;
		float                    m_pathDisplayTime;
		Waypoint                *m_facingAtWaypoint;
		Waypoint                *m_cachedWaypoint;
		Waypoint                *m_findWaypoint;

		DynamicString            m_information;

	//
	// Group: Members.
	//
	public:
		static const DynamicString FileExtention;
//		static const unsigned short InvalidWaypointIndex = static_cast <unsigned short> (-1);
/*		enum Constant_t
		{
			MaximumWaypoints  = 1024u,
			MaximumPathPoints = 8u
		};
*/
	//
	// Group: (Con/De)structors.
	//
	public:
		inline WaypointManager (void) :
			m_isEditModeActivated (false),
			m_isWaypointsChanged (true),
			m_autoPathDistance (250.0f),

			m_showImportaintWaypointsTime (0.0f),
			m_pathDisplayTime (0.0f),
			m_facingAtWaypoint (NULL),
			m_cachedWaypoint (NULL),
			m_findWaypoint (NULL)
		{
			Load ();
		}

		inline ~WaypointManager (void)
		{
			// This function frees all waypoints slots (used on server shutdown), do not use 'GetWaypointsNumber()' here!!

			// Free all the memory allocated for all waypoints....
			for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)
				delete m_waypoints[index];	// Delete it from the array.
		}

	//
	// Group: Private functions.
	//
	private:
		Waypoint *const GetHostClientFacingWaypoint (void) const;

	//
	// Group: Engine callbacks.
	//
	public:
		void Think (void);

	//
	// Group: Functions.
	//
	public:
		inline Waypoint *const GetWaypoint (const unsigned short index)
		{
			// This function finds a waypoint specified by index, and then returns pointer to it (using own Waypoint class).

			// Reliability check.
			InternalAssert (index < GetWaypointsNumber ());

			return m_waypoints[index];	// If no waypoint --> NULL.
		}
		inline const Waypoint *const GetWaypoint (const unsigned short index) const
		{
			// This function finds a waypoint specified by index, and then returns pointer to it (using own Waypoint class).

			// Reliability check.
			InternalAssert (index < GetWaypointsNumber ());

			return m_waypoints[index];	// If no waypoint --> NULL.
		}

		inline const bool IsConnected (const Waypoint *const waypoint) const
		{
			// This function checks if the waypoint is connected somewhere.

			// Reliability check.
			InternalAssert (waypoint->IsValid ());

			for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)
				if (m_waypoints[index]->IsValid () && m_waypoints[index] != waypoint)
					for (unsigned char j (0u); j < m_waypoints[index]->m_paths.GetElementNumber (); ++j)
						if (m_waypoints[index]->m_paths[j]->m_destinationWaypoint == waypoint)
							return true;

			return false;
		}

		const bool IsNodeReachable (const Math::Vector3D &source, const Math::Vector3D &destination) const;
		const bool IsNodesValid    (void) const;

		void CalculateWayNodesForWaypoint (Waypoint *const waypoint);
//		/*inline */void DrawIncomingNodesForWaypoint (const Waypoint *const waypoint, const Client *const client) const;
/*		{
			// Reliability check.
			InternalAssert (waypoint->IsValid ());

			for (unsigned short index (0u); index < GetWaypointsNumber (); ++index)
				if (m_waypoints[index]->IsValid () && m_waypoints[index]->IsConnected (waypoint) && !waypoint->IsConnected (m_waypoints[index]))
					client->DrawLine (m_waypoints[index]->m_origin, waypoint->m_origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, Color <> (0u, 192u, 96u), 200u, 0u, 10u);
		}
*/
		inline const DynamicString GetWaypointsDirectory (void) const
		{
			return HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/addons/YaPB/Waypoints/";
		}

		inline const unsigned short           GetWaypointsNumber  (void) const { return m_waypoints.GetElementNumber (); }
		inline const float                    GetAutoPathDistance (void) const { return m_autoPathDistance; }
		inline       WaypointDistancesTable  &GetDistancesTable   (void)       { return m_distancesTable; }
		inline const WaypointDistancesTable  &GetDistancesTable   (void) const { return m_distancesTable; }
		inline       WaypointLocationsTable  &GetLocationsTable   (void)       { return m_locationsTable; }
		inline const WaypointLocationsTable  &GetLocationsTable   (void) const { return m_locationsTable; }
		inline       WaypointVisibilityTable &GetVisibilityTable  (void)       { return m_visibilityTable; }
		inline const WaypointVisibilityTable &GetVisibilityTable  (void) const { return m_visibilityTable; }
		inline const Waypoint *const          GetFacingAtWaypoint (void) const { return m_facingAtWaypoint; }

		const bool Save    (void) const;
		void       SaveXML (void) const;
		const bool Load    (void);

		void AddWaypoint    (const Math::Vector3D &origin, const Waypoint::Flag_t flag, const unsigned char illumination);
		void DeleteWaypoint (const unsigned short index);

		inline const bool IsEditModeActivated (void) const       { return m_isEditModeActivated; }
		inline void       ToggleDraw          (void)             { m_isEditModeActivated ^= true; }
		inline void       ToggleDraw          (const bool state) { m_isEditModeActivated = state; }
};

#endif	// ifndef WAY_POINT_MANAGER_INCLUDED