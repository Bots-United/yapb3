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
// WaypointVisibilityTable.hpp
//
// Class: WaypointVisibilityTable
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef WAY_POINT_VISIBILITY_TABLE_INCLUDED
#define WAY_POINT_VISIBILITY_TABLE_INCLUDED

class WaypointVisibilityTable
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

		bool          m_createVisibilityTable;

		DECLARE_TYPED_ENUMERATION (unsigned char, VisibilityType_t)
		{
			VisibilityType_None,	// Waypoint is not visible.

			VisibilityType_Stand,	// Waypoint is visible when standing.
			VisibilityType_Duck,	// Waypoint is visible when crouching.

			VisibilityType_All = VisibilityType_Stand | VisibilityType_Duck	// Waypoint is visible when standing or crouching.
		};
		VisibilityType_t m_visibilityTable[MaximumWaypoints][MaximumWaypoints / 4u];

	//
	// Group: (Con/De)structors.
	//
	public:
		inline WaypointVisibilityTable (void) :
			m_createVisibilityTable (false)
		{
			// Initialize the visibility table....
			memset (m_visibilityTable, VisibilityType_None, sizeof (m_visibilityTable));
		}
		inline ~WaypointVisibilityTable (void)
		{
			Save ();
		}

	//
	// Group: Functions.
	//
	public:
		void InitializeVisibility (void);

		void Save (void) const;
		void Load (void);

		void workVisibilityForWaypoint (int i, int iNumWaypoints, bool bTwoway = false);

		void WorkOutVisibilityTable (void);

		inline const bool needToWorkVisibility (void) { return m_createVisibilityTable; }
		inline void setWorkVisiblity (const bool bSet) { m_createVisibilityTable = bSet; }

		inline const bool IsVisible (const unsigned short sourceIndex, const unsigned short destinationIndex) const
		{
/*			unsigned char res = m_visibilityTable[sourceIndex][destinationIndex >> 2u];

			res >>= (destinationIndex % 4u) << 1u;

			return !((res & 3u) == 3u);
*/
			return !(((m_visibilityTable[sourceIndex][destinationIndex >> 2u] >> ((destinationIndex % 4u) << 1u)) & 3u) == 3u);
		}

		inline const bool IsDuckVisible (const unsigned short sourceIndex, const unsigned short destinationIndex) const
		{
/*			unsigned char res = m_visibilityTable[sourceIndex][destinationIndex >> 2u];

			res >>= (destinationIndex % 4u) << 1u;

			return !((res & 2u) == 2u);
*/
			return !(((m_visibilityTable[sourceIndex][destinationIndex >> 2u] >> ((destinationIndex % 4u) << 1u)) & 2u) == 2u);
		}

		inline const bool IsStandVisible (const unsigned short sourceIndex, const unsigned short destinationIndex) const
		{
/*			unsigned char res = m_visibilityTable[sourceIndex][destinationIndex >> 2u];

			res >>= (destinationIndex % 4u) << 1u;

			return !((res & 1u) == 1u);
*/
			return !(((m_visibilityTable[sourceIndex][destinationIndex >> 2u] >> ((destinationIndex % 4u) << 1u)) & 1u) == 1u);
		}
};

#endif	// ifndef WAY_POINT_VISIBILITY_TABLE_INCLUDED