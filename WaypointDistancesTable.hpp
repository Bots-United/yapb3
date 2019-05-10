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
// WaypointDistancesTable.hpp
//
// Class: WaypointDistancesTable
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef WAY_POINT_DISTANCES_TABLE_INCLUDED
#define WAY_POINT_DISTANCES_TABLE_INCLUDED

class WaypointDistancesTable
{
	//
	// Group: Private members.
	//
	private:
		// bot known file headers
		static const DynamicString HeaderFileExtension;
		static const unsigned char HeaderFileVersion = 1u;

		// General visibility table header information structure....
		struct FileHeader_t
		{
			DynamicString  headerExtension;	// should be 'HeaderFileExtension'.
			unsigned char  fileVersion;		// should be 'HeaderFileVersion'.
			DynamicString  mapName;			// This header map name.
			unsigned short pointNumber;		// number of waypoints

			inline FileHeader_t (void) { /*VOID*/ }
			inline FileHeader_t (const unsigned short inputPointNumber) :
				headerExtension (HeaderFileExtension),
				fileVersion (HeaderFileVersion),
				pointNumber (inputPointNumber)
			{ /*VOID*/ }
		};

		float m_distancesMatrix[MaximumWaypoints][MaximumWaypoints];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline WaypointDistancesTable (void)
		{
			// Initialize the distances table....
			memset (m_distancesMatrix, 0, sizeof (m_distancesMatrix));
		}

	//
	// Group: Private functions.
	//
	private:
		

	//
	// Group: Functions.
	//
	public:
		const bool Load (void);
		const bool Save (void) const;

		inline const float GetPathDistance (const unsigned short sourceIndex, const unsigned short destinationIndex) const
		{
			// This function returns the "standerd" distance between 2 waypoints.

			// Reliability check.
//			InternalAssert (sourceIndex < g_server->GetWaypointManager ().GetWaypointsNumber () && destinationIndex < g_server->GetWaypointManager ().GetWaypointsNumber ());

			return m_distancesMatrix[sourceIndex][destinationIndex];
		}
};

#endif	// ifndef WAY_POINT_DISTANCES_TABLE_INCLUDED