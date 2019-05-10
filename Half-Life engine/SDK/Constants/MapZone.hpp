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
// MapZone.hpp
//
// Enumeration: MapZone_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MAP_ZONE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_MAP_ZONE_INCLUDED

DECLARE_TYPED_ENUMERATION (unsigned int, MapZone_t)
{
	MapZone_None,

	IN_BUY_ZONE             = BIT (0u),	// Is client inside HIS buy zone? (for 'OFFSET_MAP_ZONE')
	IN_BOMB_PLACE           = BIT (1u),	// Is TERRORIST client inside bomb place?
	IN_HOSTAGES_RESCUE_ZONE = BIT (2u),	// Is client inside hostages rescue zone?
	IN_ESCAPE_ZONE          = BIT (3u),	// Is COUNTER-TERRORIST client inside terrorist escape zone?
	IN_VIP_RESCUE_ZONE      = BIT (4u)	// Is client inside vip rescue zone?
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MAP_ZONE_INCLUDED