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
// SpotOrder.hpp
//
// Class: SpotOrder
//
// Description: Stores a pointer to an interesting "spot", and a parametric distance along a path.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SPOT_ORDER_INCLUDED
#define SPOT_ORDER_INCLUDED

struct SpotOrder
{
	float       t;		///< parametric distance along ray where this spot first has LOS to our path
	HidingSpot *spot;	///< the spot to look at or spot ID for save/load

	inline void       Save (BaseFile &file) const
	{
		// spot may be NULL if we've loaded a navigation mesh that has been edited but not re-analyzed
		const unsigned int id (spot != NULL ? spot->GetID () : 0u);

		file.Write (id);

		const unsigned char newT (static_cast <unsigned char> (255u * t));

		file.Write (newT);
	}
	inline const bool Load (const BaseFile &file)
	{
		unsigned int id;

		if (!file.Read (id))
			return false;

		spot = reinterpret_cast <HidingSpot *> (id);

		unsigned char newT;

		if (!file.Read (newT))
			return false;

		t = newT / 255.0f;

		return true;
	}
	inline const bool PostLoad (void);	// Note: This function declared in Server.hpp.
};

typedef DynamicArray <SpotOrder, unsigned char> SpotOrderArray;

#endif	// ifndef SPOT_ORDER_INCLUDED