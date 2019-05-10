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
// SpotEncounter.hpp
//
// Class: SpotEncounter
//
// Description: This structure stores possible path segments thru a NavigationArea, and the dangerous spots to look at as we traverse that path segment.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SPOT_ENCOUNTER_INCLUDED
#define SPOT_ENCOUNTER_INCLUDED

struct SpotEncounter
{
	NavigationArea *from;
	NavDirType_t    fromDir;
	NavigationArea *to;
	NavDirType_t    toDir;
	struct Ray_t
	{
		Math::Vector3D from;
		Math::Vector3D to;
	}               path;	///< the path segment

	#include <Navigation mesh/SpotOrder.hpp>	// Stores a pointer to an interesting "spot", and a parametric distance along a path.

	SpotOrderArray  spots;	///< list of spots to look at, in order of occurrence

	inline void       Save (BaseFile &file) const;
	inline const bool Load (const BaseFile &file)
	{
		unsigned short id;

		if (!file.Read (id))
			return false;

		from = reinterpret_cast <NavigationArea *> (id);

		if (!file.Read (fromDir))
			return false;

		if (!file.Read (id))
			return false;

		to = reinterpret_cast <NavigationArea *> (id);

		if (!file.Read (toDir))
			return false;

		SpotOrderArray::IndexType_t spotCount;

		// read list of spots along this path
		if (!file.Read (spotCount))
			return false;

		SpotOrder order;

		for (SpotOrderArray::IndexType_t index (0u); index < spotCount; ++index)
		{
			if (!order.Load (file))
				return false;

			spots += order;
		}

		return true;
	}
	inline const bool PostLoad (NavigationArea *const ownerArea);	// Note: This function declared in Server.hpp.
};

typedef DynamicArray <SpotEncounter *> SpotEncounterArray;

#endif	// ifndef SPOT_ENCOUNTER_INCLUDED