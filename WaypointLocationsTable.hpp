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
// WaypointLocationsTable.hpp
//
// Class: WaypointLocationsTable
//
// Description:
//	Hash table of waypoint indexes accross certian
//	buckets in the map on X/Y/Z co-ords for quicker
//	nearest waypoint finding and displaying.
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef WAY_POINT_LOCATIONS_TABLE_INCLUDED
#define WAY_POINT_LOCATIONS_TABLE_INCLUDED

class WaypointLocationsTable
{
	//
	// Group: Members.
	//
	public:
		static const unsigned short MaximumReachableRange = 512u;

	//
	// Group: Private members.
	//
	private:
		enum Constants_t
		{
			MAX_WPT_BUCKETS    = 32u,	// want bucket spacing of 'MAX_BUCKET_SPACING' units
			MAX_BUCKET_SPACING = HalfLifeEngine::SDK::Constants::MapSize / MAX_WPT_BUCKETS	// 256 units
		};

		static unsigned char                         g_failedWaypoints[MaximumWaypoints];	// Global check for checking waypoints to ignore for current bot.

		DynamicArray <unsigned short, unsigned char> m_locations[MAX_WPT_BUCKETS][MAX_WPT_BUCKETS][MAX_WPT_BUCKETS];
#if 0
		DynamicArray <unsigned short, unsigned char> *m_grid;
		struct GridSize_t
		{
			unsigned short x, y, z;

			inline GridSize_t (void) : x (MAX_BUCKET_SPACING), y (MAX_BUCKET_SPACING), z (MAX_BUCKET_SPACING) { /*VOID*/ }
		}                                      m_gridSize;

	//
	// Group: Private functions.
	//
	private:
		inline  WaypointLocationsTable (void) : m_grid (NULL) { /*VOID*/ }
		inline ~WaypointLocationsTable (void) { delete [] m_grid; }
#endif	// if 0
	//
	// Group: Private functions.
	//
	private:
		static inline const unsigned char ReadLocation (const float location)
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((HalfLifeEngine::SDK::Constants::HalfMapSize + location) / MAX_BUCKET_SPACING), 0, MAX_BUCKET_SPACING - 1));
		}

		static inline void GetMinsMaxs (const Math::Vector3D &location, unsigned char &minLocationX, unsigned char &minLocationY, unsigned char &minLocationZ, unsigned char &maxLocationX, unsigned char &maxLocationY, unsigned char &maxLocationZ)
		{
			// get current area

			minLocationX = maxLocationX = ReadLocation (location.x);

			if (--minLocationX >= MAX_WPT_BUCKETS)
				minLocationX = 0u;

			if (++maxLocationX >= MAX_WPT_BUCKETS)
				maxLocationX = MAX_WPT_BUCKETS - 1u;

			minLocationY = maxLocationY = ReadLocation (location.y);

			if (--minLocationY >= MAX_WPT_BUCKETS)
				minLocationY = 0u;

			if (++maxLocationY >= MAX_WPT_BUCKETS)
				maxLocationY = MAX_WPT_BUCKETS - 1u;

			minLocationZ = maxLocationZ = ReadLocation (location.z);

			if (--minLocationZ >= MAX_WPT_BUCKETS)
				minLocationZ = 0u;

			if (++maxLocationZ >= MAX_WPT_BUCKETS)
				maxLocationZ = MAX_WPT_BUCKETS - 1u;
		}

	//
	// Group: Functions.
	//
	public:
		unsigned char *const ResetFailedWaypoints (const DynamicArray <unsigned short> *const iIgnoreWpts);
/*
		inline void Clear (void)
		{
			for (unsigned char locationX (0u), locationY, locationZ; locationX < MAX_WPT_BUCKETS; ++locationX)
				for (locationY = 0u; locationY < MAX_WPT_BUCKETS; ++locationY)
					for (locationZ = 0u; locationZ < MAX_WPT_BUCKETS; ++locationZ)
						m_locations[locationX][locationY][locationZ].RemoveAll ();
		}
*/
		const unsigned short GetCoverWaypoint (const Math::Vector3D &vPlayerOrigin, const Math::Vector3D &vCoverFrom, const DynamicArray <unsigned short> *const iIgnoreWpts, Math::Vector3D *const vGoalOrigin = NULL, const HalfLifeEngine::SDK::Constants::TeamID_t team = HalfLifeEngine::SDK::Constants::TeamID_Unassigned);

		void FindNearestCoverWaypointInBucket (const unsigned char locationX, const unsigned char locationY, const unsigned char locationZ, const Math::Vector3D &origin, float &minDist, unsigned short &index, const unsigned short iCoverFromWpt, Math::Vector3D *const vGoalOrigin = NULL, const HalfLifeEngine::SDK::Constants::TeamID_t team = HalfLifeEngine::SDK::Constants::TeamID_Unassigned);

		inline void AddWptLocation (const unsigned short index, const Math::Vector3D &origin)
		{
			// Add a waypoint with index and at origin (for quick insertion in the list)

			m_locations[ReadLocation (origin.x)][ReadLocation (origin.y)][ReadLocation (origin.z)] += index;
		}

		inline void DeleteWptLocation (const unsigned short index, const Math::Vector3D &origin)
		{
			// Delete the waypoint index at the origin (for finding it quickly in the list)

			m_locations[ReadLocation (origin.x)][ReadLocation (origin.y)][ReadLocation (origin.z)] -= index;
		}

		void FindNearestInBucket (const unsigned char locationX, const unsigned char locationY, const unsigned char locationZ, const Math::Vector3D &origin, float &minDist, unsigned short &index, const unsigned short iIgnoreWpt = InvalidWaypointIndex, const bool bGetVisible = true, const bool bIsBot = false, const HalfLifeEngine::SDK::Constants::TeamID_t team = HalfLifeEngine::SDK::Constants::TeamID_Unassigned);

		void DrawWaypoints (HalfLifeEngine::SDK::Classes::Edict *const player, const bool drawPaths);

		const unsigned short GetNearestWaypoint (const Math::Vector3D &origin, float fDist = MaximumReachableRange, const unsigned short iIgnoreWpt = InvalidWaypointIndex, const bool bGetVisible = true, const bool bIsBot = false, DynamicArray <unsigned short> *const iFailedWpts = NULL, const bool bNearestAimingOnly = false, const HalfLifeEngine::SDK::Constants::TeamID_t team = HalfLifeEngine::SDK::Constants::TeamID_Unassigned);

		void GetAllVisible (const Math::Vector3D &vVisibleFrom, const Math::Vector3D &origin, DynamicArray <unsigned short> &iVisible);

		///////////

		void AutoPath (HalfLifeEngine::SDK::Classes::Edict *const pPlayer, const unsigned short iWpt);

		void AutoPathInBucket (HalfLifeEngine::SDK::Classes::Edict *const pPlayer, const unsigned char locationX, const unsigned char locationY, const unsigned char locationZ, const unsigned short iWpt);
};

#endif	// ifndef WAY_POINT_LOCATIONS_TABLE_INCLUDED