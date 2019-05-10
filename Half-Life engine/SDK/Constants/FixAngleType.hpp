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
// FixAngleType.hpp
//
// Enumeration: FixAngleType_t
//
// Description: View angle update types for 'Structures::EntityVariables_t::fixAngle'.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_FIX_ANGLE_TYPE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_FIX_ANGLE_TYPE_INCLUDED

enum FixAngleType_t
{
	FixAngleType_None,		// nothing

	FixAngleType_Absolute,	// force model angles ('Structures::EntityVariables_t::modelAngles')
	FixAngleType_Relative	// add 'Structures::EntityVariables_t::angleVelocity' (When set to 1 it forces the client to update its angles to the ones set in 'Structures::EntityVariables_t::modelAngles' during the next player think)
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_FIX_ANGLE_TYPE_INCLUDED