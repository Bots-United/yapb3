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
// PlaceDirectory.hpp
//
// Class: PlaceDirectory
//
// Description: The 'place directory' is used to save and load places from
//				navigation files in a size-efficient manner that also allows for the
//				order of the place ID's to change without invalidating the
//				navigation files.
//
//				The place directory is stored in the navigation file as a list of
//				place name strings. Each navigation area then contains an index
//				into that directory, or zero if no place has been assigned to
//				that area.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLACE_DIRECTORY_INCLUDED
#define PLACE_DIRECTORY_INCLUDED

class PlaceDirectory
{
	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <Place_t> DirectoryArray;

	//
	// Group: Constants.
	//
	public:
		enum { MaximumPlaces = 1000u };

	//
	// Group: Private members.
	//
	private:
		DirectoryArray m_directory;

	//
	// Group: Functions.
	//
	public:
		inline void Reset (void)
		{
			m_directory.RemoveAll ();
		}

		inline const bool IsKnown (const Place_t place) const
		{
			/// return true if this place is already in the directory

			return m_directory.IsMember (place);
		}

		inline const DirectoryArray::IndexType_t GetIndex (const Place_t place) const
		{
			/// return the directory index corresponding to this Place (UNDEFINED_PLACE = no entry)

			if (place == UNDEFINED_PLACE)
				return UNDEFINED_PLACE;

			const DirectoryArray::IndexType_t index (m_directory.Find (place));

			// Reliability check.
			InternalAssert (index < m_directory.BadPosition && "PlaceDirectory::GetIndex() failure!");

			return static_cast <DirectoryArray::IndexType_t> (index + 1u);
		}

		inline void AddPlace (const Place_t place)
		{
			/// add the place to the directory if not already known

			// Reliability checks.
			InternalAssert (place != UNDEFINED_PLACE);
			InternalAssert (place < MaximumPlaces);

			if (!IsKnown (place))
				m_directory += place;
		}

		inline const Place_t IndexToPlace (const DirectoryArray::IndexType_t entry) const
		{
			/// given an index, return the Place

			if (entry == UNDEFINED_PLACE)
				return UNDEFINED_PLACE;

			const DirectoryArray::IndexType_t index (entry - 1u);

			// Reliability check.
			InternalAssert (index < m_directory.GetElementNumber ());

			return m_directory[index];
		}

		// Note: Below functions are declared in Server.hpp.
		inline void       Save (BaseFile &file) const;	/// store the directory
		inline const bool Load (const BaseFile &file);		/// load the directory

		inline void       Build   (void);				/// build the directory
		inline void       Rebuild (void)
		{
			/// rebuild the directory

			// do some cleanup first
			Reset ();

			Build ();
		}
};

#endif	// ifndef PLACE_DIRECTORY_INCLUDED