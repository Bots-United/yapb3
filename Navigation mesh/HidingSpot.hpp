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
// HidingSpot.hpp
//
// Class: HidingSpot
//
// Description: A HidingSpot is a good place for a bot to crouch and wait for enemies.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HIDING_SPOT_INCLUDED
#define HIDING_SPOT_INCLUDED

class HidingSpot// : public Spot
{
	friend NavigationMesh;

	//
	// Group: Type definitions.
	//
	public:
		typedef unsigned int ID_t;

		DECLARE_TYPED_ENUMERATION (unsigned char, Flag_t)
		{
			Flag_None,

			IN_COVER          = BIT (0u),	///< in a corner with good hard cover nearby
			GOOD_SNIPER_SPOT  = BIT (1u),	///< had at least one decent sniping corridor
			IDEAL_SNIPER_SPOT = BIT (2u),	///< can see either very far, or a large area, or both
			EXPOSED           = BIT (3u)	///< spot in the open, usually on a ledge or cliff
		};

	//
	// Group: Private members.
	//
	private:
		Math::Vector3D       m_pos;				///< world coordinates of the spot
		unsigned int         m_id;				///< this spot's unique ID
		unsigned int         m_marker;			///< this spot's unique marker
		NavigationArea      *m_area;			///< the navigation area containing this hiding spot
		unsigned char        m_flags;			///< bit flags

		static unsigned int  m_nextID;			///< used when allocating spot ID's
		static unsigned int  m_masterMarker;	///< used to mark spots

	//
	// Group: (Con/De)structors.
	//
	// Note: This group of functions are declared in Server.hpp.
	//
	public:
		///< Construct a Hiding Spot. Assign a unique ID which may be overwritten if loaded.
		inline HidingSpot (void);	///< Must use factory to create.
		inline HidingSpot (const Math::Vector3D &pos, const Flag_t flag);	///< Must use factory to create.

		virtual inline ~HidingSpot (void);

	//
	// Group: Functions.
	//
	public:
		inline const bool HasGoodCover      (void) const { return (m_flags & IN_COVER) > 0u; }	///< return true if hiding spot in in cover
		inline const bool IsGoodSniperSpot  (void) const { return (m_flags & GOOD_SNIPER_SPOT) > 0u; }
		inline const bool IsIdealSniperSpot (void) const { return (m_flags & IDEAL_SNIPER_SPOT) > 0u; }
		inline const bool IsExposed         (void) const { return (m_flags & EXPOSED) > 0u; }

		inline const Flag_t GetFlags (void) const { return m_flags; }

		inline void Save (BaseFile &file) const
		{
//			Spot::Save (file);

			file.Write (m_id);
			file.Write (m_pos);
			file.Write (m_flags);
		}
		inline const bool Load (const BaseFile &file)
		{
/*			if (!Spot::Load (file))
				return false;
*/
			if (!file.Read (m_id) || !file.Read (m_pos) || !file.Read (m_flags))
				return false;

			// update next ID to avoid ID collisions by later spots
			if (m_id > m_nextID)
				m_nextID = m_id;

			return true;
		}
		const bool PostLoad (void);

		inline const Math::Vector3D        &GetPosition        (void) const { return m_pos; }	///< get the position of the hiding spot
		inline const ID_t                   GetID              (void) const { return m_id; }
		inline const NavigationArea *const  GetArea            (void) const { return m_area; }	///< return navigation area this hiding spot is within

		inline void                         Mark               (void)       { m_marker = m_masterMarker; }
		inline const bool                   IsMarked           (void) const { return m_marker == m_masterMarker; }
		static inline void                  ChangeMasterMarker (void)       { ++m_masterMarker; }

		void ClassifySniperSpot (void);
};

typedef DynamicArray <HidingSpot *, unsigned char> HidingSpotArray;

#endif	// ifndef HIDING_SPOT_INCLUDED