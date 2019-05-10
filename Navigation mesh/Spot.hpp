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
// Spot.hpp
//
// Class: Spot
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_SPOT_INCLUDED
#define NAVIGATION_MESH_SPOT_INCLUDED

class Spot
{
	friend NavigationMesh;

	//
	// Group: Type definitions.
	//
	public:
		typedef unsigned int ID_t;

	//
	// Group: Protected members.
	//
	protected:
		Math::Vector3D       m_pos;					///< world coordinates of the spot
//		ID_t                 m_baseId;				///< this spot's unique ID
		ID_t                 m_id;					///< derived spot's unique ID
//		unsigned int         m_baseMarker;			///< this spot's unique marker
		NavigationArea      *m_area;				///< the navigation area containing this spot

		static ID_t          m_nextBaseID;			///< used when allocating spot ID's
		static unsigned int  m_baseMasterMarker;	///< used to mark spots

	//
	// Group: (Con/De)structors.
	//
	// Note: This group of functions are declared in Server.hpp.
	//
	public:
		///< Construct a Spot. Assign a unique ID which may be overwritten if loaded.
		inline Spot (void);	///< Must use factory to create.
		inline Spot (const Math::Vector3D &pos);	///< Must use factory to create.

		virtual inline ~Spot (void);

	//
	// Group: Functions.
	//
	public:
		virtual inline void Save (BaseFile &file) const
		{
//			file.Write (m_baseId);
			file.Write (m_pos);
		}
		virtual inline const bool Load (const BaseFile &file)
		{
			if (/*!file.Read (m_baseId) || */!file.Read (m_pos))
				return false;
/*
			// update next ID to avoid ID collisions by later spots
			if (m_baseId > m_nextBaseID)
				m_nextBaseID = m_baseId;
*/
			return true;
		}
		virtual const bool PostLoad (void);

		inline const Math::Vector3D        &GetPosition        (void) const { return m_pos; }	///< get the position of the spot
//		inline const ID_t                   GetBaseID          (void) const { return m_baseId; }
		inline const ID_t                   GetID              (void) const { return m_id; }
		inline const NavigationArea *const  GetArea            (void) const { return m_area; }	///< return navigation area this spot is within
/*
		inline void                         Mark               (void)       { m_baseMarker = m_baseMasterMarker; }
		inline const bool                   IsMarked           (void) const { return m_baseMarker == m_baseMasterMarker; }
		static inline void                  ChangeMasterMarker (void)       { ++m_baseMasterMarker; }*/
};

typedef DynamicArray <Spot *, unsigned char> SpotArray_t;

#endif	// ifndef NAVIGATION_MESH_SPOT_INCLUDED