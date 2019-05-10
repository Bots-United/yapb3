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
// vector_definitions.hpp
//
// Description: Some vector constants.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef VECTOR_DEFINITIONS_INCLUDED
#define VECTOR_DEFINITIONS_INCLUDED

const Math::Vector3D VEC_POINT_HULL_MIN         (0.0f, 0.0f, 0.0f);											// Hull_Point
const Math::Vector3D VEC_POINT_HULL_MAX         (0.0f, 0.0f, 0.0f);											// Hull_Point
const Math::Vector3D VEC_HULL_MIN               (-HalfHumanWidth, -HalfHumanWidth, -HalfHumanHeight);		// Hull_Human
const Math::Vector3D VEC_HULL_MAX               ( HalfHumanWidth,  HalfHumanWidth,  HalfHumanHeight);		// Hull_Human
const Math::Vector3D VEC_LARGE_HULL_MIN         (-32.0f, -32.0f, -32.0f);									// Hull_Large
const Math::Vector3D VEC_LARGE_HULL_MAX         ( 32.0f,  32.0f,  32.0f);									// Hull_Large
const Math::Vector3D VEC_HUMAN_HULL_MIN         (-HalfHumanWidth, -HalfHumanWidth,  0.0f);
const Math::Vector3D VEC_HUMAN_HULL_MAX         ( HalfHumanWidth,  HalfHumanWidth,  HumanHeight);
const Math::Vector3D VEC_HUMAN_HULL_DUCK        ( HalfHumanWidth,  HalfHumanWidth,  HalfHumanHeight);
const Math::Vector3D VectorHumanStandViewOffset (           0.0f,            0.0f,  HumanStandViewOffset);
const Math::Vector3D VEC_DUCK_HULL_MIN          (-HalfHumanWidth, -HalfHumanWidth, -HalfDuckHumanHeight);	// Hull_Head
const Math::Vector3D VEC_DUCK_HULL_MAX          ( HalfHumanWidth,  HalfHumanWidth,  HalfDuckHumanHeight);	// Hull_Head
const Math::Vector3D VectorHumanDuckViewOffset  (           0.0f,            0.0f,  HumanDuckViewOffset);

const Math::Extent3D hullSizes[Hull_Total] =
{
//	Math::Extent3D (Mins,               Maxs),
	Math::Extent3D (VEC_POINT_HULL_MIN, VEC_POINT_HULL_MAX),	// Hull_Point (0x0x0)
	Math::Extent3D (VEC_HULL_MIN,       VEC_HULL_MAX),			// Hull_Human (32x32x72)
	Math::Extent3D (VEC_LARGE_HULL_MIN, VEC_LARGE_HULL_MAX),	// Hull_Large (64x64x64)
	Math::Extent3D (VEC_DUCK_HULL_MIN,  VEC_DUCK_HULL_MAX)		// Hull_Head  (32x32x36)
};

#endif	// ifndef VECTOR_DEFINITIONS_INCLUDED