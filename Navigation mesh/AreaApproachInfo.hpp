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
// AreaApproachInfo.hpp
//
// Structure: ApproachInfo_t
//
// Version: $ID:$
//
/*
#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_AREA_APPROACH_INFO_INCLUDED
#define NAVIGATION_AREA_APPROACH_INFO_INCLUDED
*/
struct ApproachInfo_t
{
	NavigationArea    *here;			///< the approach area
	NavigationArea    *prev;			///< the area just before the approach area on the path
	NavTraverseType_t  prevToHereHow;
	NavigationArea    *next;			///< the area just after the approach area on the path
	NavTraverseType_t  hereToNextHow;

	inline void Save (BaseFile &file) const
	{
		unsigned short id (here != NULL ? here->GetID () : 0u);

		file.Write (id);

		id = prev != NULL ? prev->GetID () : 0u;

		file.Write (id);
		file.Write (prevToHereHow);

		id = next != NULL ? next->GetID () : 0u;

		file.Write (id);
		file.Write (hereToNextHow);
	}
	inline const bool Load (const BaseFile &file)
	{
		unsigned short id;

		if (!file.Read (id))
			return false;

		here = reinterpret_cast <NavigationArea *> (id);

		if (!file.Read (id))
			return false;

		prev = reinterpret_cast <NavigationArea *> (id);

		if (!file.Read (prevToHereHow))
			return false;

		if (!file.Read (id))
			return false;

		next = reinterpret_cast <NavigationArea *> (id);

		return file.Read (hereToNextHow);
	}
	inline const bool PostLoad (void);	// Note: This function declared in Server.hpp.
};

enum { MAX_APPROACH_AREAS = 16u };

//typedef StaticArray <ApproachInfo_t, unsigned char, MAX_APPROACH_AREAS> ApproachArray;
typedef DynamicArray <ApproachInfo_t, unsigned char> ApproachArray;

//#endif	// ifndef NAVIGATION_AREA_APPROACH_INFO_INCLUDED