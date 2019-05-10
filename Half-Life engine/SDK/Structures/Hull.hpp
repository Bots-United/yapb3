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
// Hull.hpp
//
// Structure: Hull_t
//
// 06/23/2002 MAH
// Note: this structure is exactly the same in QW software and hardware renderers 'model.h' and 'gl_model.h'.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_HULL_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_HULL_INCLUDED

struct Hull_t
{
/*! Off=0(0)*/	DClipNode_t    *clipNodes;
/*! Off=4(1)*/	MPlane_t       *planes;

/*! Off=8(2)*/	int             firstClipNodeIndex;
/*! Off=12(3)*/	int             lastClipNodeIndex;

/*! Off=16(4)*/	Math::Vector3D  clipMins;
/*! Off=28(7)*/	Math::Vector3D  clipMaxs;
};	// sizeof (Hull_t) == 40 (10)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_HULL_INCLUDED