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
// PMPlayerTraceFlag.hpp
//
// Description: Values for 'traceFlags' parameter of PM_PlayerTrace().
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PM_PLAYER_TRACE_FLAG_INCLUDED
#define PM_PLAYER_TRACE_FLAG_INCLUDED

enum PMPlayerTraceFlag_t
{
	PMPlayerTraceFlag_Normal,

	PMPlayerTraceFlag_StudioIgnore = BIT (0u),	// Skip studio models
	PMPlayerTraceFlag_StudioBox    = BIT (1u),	// Use boxes for non-complex studio models (even in trace line)
	PMPlayerTraceFlag_GlassIgnore  = BIT (2u),	// Ignore entities with non-normal rendermode
	PMPlayerTraceFlag_WorldOnly    = BIT (3u)	// Only trace against the world
};

#endif	// ifndef PM_PLAYER_TRACE_FLAG_INCLUDED