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
// point_content_type.hpp
//
// Enumaration: PointContentType_t
//
// Description: Contents of a spot in the world (used for pfnPointContents()/pev->waterType functions).
//				Possible contents of leaves and clip nodes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef POINT_CONTENT_TYPE_INCLUDED
#define POINT_CONTENT_TYPE_INCLUDED

enum PointContentType_t
{
	PointContentType_None,			// Added manually!

	Content_Empty           = -1,	// Open space.
	Content_Solid           = -2,	// Entirely inside a solid.
	Content_Water           = -3,	// Water, the vision is troubled.
	Content_Slime           = -4,	// Slime, green acid that hurts the player.
	Content_Lava            = -5,	// Lava, vision turns red and the player is badly hurt.
	Content_Sky             = -6,	// Behaves like water, but is used for sky.

	// 2 placed below contents are removed or changed in the process of map compilation! (You will not find them in the BSP file)
	Content_Origin          = -7,	// Removed at csg time
	Content_Clip            = -8,	// Changed to 'Content_Solid' (invisible, penetrable, but players can't walk through)

	// I don't know what these and the following are for.
	Content_Current_0       = -9,
	Content_Current_90      = -10,
	Content_Current_180     = -11,
	Content_Current_270     = -12,
	Content_Current_Up      = -13,
	Content_Current_Down    = -14,

	Content_Translucent     = -15,
	Content_Ladder          = -16,	// For "func_ladder" entities. (In the compiler of maps it is named as 'Content_Hint'. Filters down to 'Content_Empty' by bsp, ENGINE SHOULD NEVER SEE THIS)
	Content_FlyField        = -17,	// In the compiler of maps it is named as 'Content_Null'. Removed in csg and bsp, VIS or RAD shouldnt have to deal with this, only clip planes!
	Content_GravityFlyField = -18,	// In the compiler of maps it is named as 'Content_Detail'.
	Content_Fog             = -19
};

#endif	// ifndef POINT_CONTENT_TYPE_INCLUDED