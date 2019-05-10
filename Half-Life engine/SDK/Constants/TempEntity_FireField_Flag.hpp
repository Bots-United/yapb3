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
// TempEntity_FireField_Flag.hpp
//
// Description: To keep network traffic low, this message has associated flags that fit into a byte.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TEMP_ENTITY_FIRE_FIELD_FLAG_INCLUDED
#define TEMP_ENTITY_FIRE_FIELD_FLAG_INCLUDED

enum TempEntity_FireField_Flag_t
{
	TEFIRE_FLAG_ALLFLOAT  = BIT (0u),	// all sprites will drift upwards as they animate
	TEFIRE_FLAG_SOMEFLOAT = BIT (1u),	// some of the sprites will drift upwards. (50% chance)
	TEFIRE_FLAG_LOOP      = BIT (2u),	// if set, sprite plays at 15 fps, otherwise plays at whatever rate stretches the animation over the sprite's duration
	TEFIRE_FLAG_ALPHA     = BIT (3u),	// if set, sprite is rendered alpha blended at 50% else, opaque
	TEFIRE_FLAG_PLANAR    = BIT (4u)	// if set, all fire sprites have same initial Z instead of randomly filling a cube
};

#endif	// ifndef TEMP_ENTITY_FIRE_FIELD_FLAG_INCLUDED