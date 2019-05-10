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
// trace_result.hpp
//
// Description: For pfnTraceLine()/pfnTraceToss()/pfnTraceMonsterHull()/pfnTraceHull()/pfnTraceEntity()/pfnTraceSphere() functions.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TRACE_RESULT_INCLUDED
#define TRACE_RESULT_INCLUDED

struct TraceResult_t
{
/* Off=0  (0) */	BOOL                         isAllSolid;	// if true, plane is not valid
/* Off=4  (1) */	BOOL                         isStartSolid;	// if true, the initial point was in a solid area
/* Off=8  (2) */	BOOL                         isInOpen;		// end point is in empty space
/* Off=12 (3) */	BOOL                         isInWater;		// end point is in water
/* Off=16 (4) */	float                        fraction;		// time completed, 1.0 = didn't hit anything
/* Off=20 (5) */	Math::Vector3D               endPosition;	// final position
/* Off=32 (8) */	float                        planeDistance;
/* Off=36 (9) */	Math::Vector3D               planeNormal;	// surface normal at impact
/* Off=48 (12) */	Classes::Edict              *hitEntity;		// entity the surface is on
/* Off=52 (13) */	Constants::PlayerBodyPart_t  hitGroup;		// 0 == generic, non zero is specific body part
};

// Note: original name was 'trace_t'.
struct EngineTraceResult_t
{
/* Off=0  (0) */	BOOL                         isAllSolid;	// if true, plane is not valid
/* Off=4  (1) */	BOOL                         isStartSolid;	// if true, the initial point was in a solid area
/* Off=8  (2) */	BOOL                         isInOpen;		// end point is in empty space
/* Off=12 (3) */	BOOL                         isInWater;		// end point is in water
/* Off=16 (4) */	float                        fraction;		// time completed, 1.0 = didn't hit anything
/* Off=20 (5) */	Math::Vector3D               endPosition;	// final position
/* Off=32 (8) */	Math::Plane                  plane;			// surface normal at impact
/* Off=48 (12) */	Classes::Edict              *hitEntity;		// entity the surface is on
/* Off=52 (13) */	Constants::PlayerBodyPart_t  hitGroup;		// 0 == generic, non zero is specific body part
};	// sizeof (EngineTraceResult_t) == 56 (14)

#endif	// ifndef TRACE_RESULT_INCLUDED