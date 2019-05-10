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
// PlaceContainer.hpp
//
// Class: PlaceContainer
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_PLACE_CONTAINER_INCLUDED
#define NAVIGATION_MESH_PLACE_CONTAINER_INCLUDED

class PlaceContainer
{
	//
	// Group: Private members.
	//
	private:
		typedef DynamicArray <DynamicString> PlacesArray;

		PlacesArray m_places;	///< master directory of place names (ie: "places")

	//
	// Group: (Con/De)structors.
	//
	public:
		inline PlaceContainer (void)
		{
			LoadPlaceDatabase ();
		}

	//
	// Group: Private functions.
	//
	private:
		static inline const int Compare (const DynamicString *const string1, const DynamicString *const string2)	///< compare function
		{
			// Compare all of both strings:
			return string1->CompareWithCaseCheck (*string2);
		}

		inline void Reset (void)
		{
			m_places.RemoveAll ();
		}

		void LoadPlaceDatabase (void);	///< load the place names from a file

	//
	// Group: Functions.
	//
	public:
		inline const bool IsEmpty (void) const { return m_places.IsEmpty (); }
		inline void ReloadPlaceDatabase (void);						///< reload the place names from a file. (Note: This function declared in Server.hpp)
		inline const DynamicString PlaceToName (const Place_t place) const					///< Given a place, return its name. Reserve zero as invalid.
		{
			if (place == UNDEFINED_PLACE || place > m_places.GetElementNumber ())
				return "";

			return m_places[place - 1u];
		}
		inline const Place_t NameToPlace (const DynamicString &name) const							///< Given a place name, return a place ID or zero if no place is defined. Reserve zero as invalid.
		{
			if (name.IsEmpty ())
				return UNDEFINED_PLACE;

			for (PlacesArray::IndexType_t index (0u); index < m_places.GetElementNumber (); ++index)
				if (m_places[index] == name)
					return index + 1u;

			return UNDEFINED_PLACE;
		}
		const Place_t PartialNameToPlace (const DynamicString &name) const;																	///< given the first part of a place name, return a place ID or zero if no place is defined, or the partial match is ambiguous
		void PrintAllPlaces (void) const;																								///< output a list of names to the console
		int PlaceNameAutocomplete (const char *partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);	///< Given a partial place name, fill in possible place names for 'ConCommand' autocomplete
};

#endif	// ifndef NAVIGATION_MESH_PLACE_CONTAINER_INCLUDED