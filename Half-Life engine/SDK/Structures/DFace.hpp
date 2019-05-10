//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// DFace.hpp
//
// Structure: DFace_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DFACE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_DFACE_INCLUDED

struct DFace_t
{
	unsigned short planeNum;							// The plane in which the face lies (Index into planes lump. Must be in [0, m_bspFile.planes.GetElementNumber()])
	unsigned short side;								// index of the side(s) that are textured (or not) on this face (0 if in front of the plane, 1 if behind the plane) (If nonzero, must flip the normal of the given plane to match face orientation)
	unsigned int   firstSurfEdgeIndex;					// index number of the first surf edge of this face (Index into surf edges lump)
	unsigned short surfEdgesNum;						// number of surf edges this face has (must be in [0, m_bspFile.surfEdges.GetElementNumber()])
	unsigned short textureInfo;							// index of the texture info lump the face is part of (must be in [0, m_bspFile.texturesInformation.GetElementNumber()])

	// lighting info
	unsigned char  styles[Constants::MAX_MAP_LIGHTS];	// lightmap styles for this face (0 - type of lighting, for the face, 2 - indicates the kind of lighting that should be applied to the face (from 0xFF (dark) to 0 (bright) - 0 - is the normal value, to be used with a light map, 0xFF - is to be used when there is no light map, 1 - produces a fast pulsating light, 2 - produces a slow pulsating light, 3 to 10 - produce various other lighting effects, as defined in the code lump), 2-3 - two additional light models)
	int            lightOffset;							// Pointer inside the general light map, or -1, this define the start of the face light map (start of [numStyles * surfSize] samples)
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DFACE_INCLUDED