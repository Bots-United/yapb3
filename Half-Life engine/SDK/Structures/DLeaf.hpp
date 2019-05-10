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
// DLeaf.hpp
//
// Structure: DLeaf_t
//
// Note: Leaf 0 is the generic 'Constants::Content_Solid' leaf, used for all solid areas all other leafs need visibility info.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DLEAF_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_DLEAF_INCLUDED

struct DLeaf_t
{
	Constants::PointContentType_t contents;									// Special type of leaf
	int                           visOffset;								// Beginning of visibility lists (must be -1 = no visibility info or in [0, numvislist])

	DBoundingBox_t                boundingBox;								// Bounding box of the leaf (for sphere culling (frustum culling))

	unsigned short                firstMarkSurface;							// First item of the list of mark surfaces lump (must be in [0, m_bspFile.markSurfaces.GetElementNumber()])
	unsigned short                markSurfacesNum;							// Number of mark surfaces in the leaf

	unsigned char                 ambientLevel[Constants::NUM_AMBIENTS];	// level of the four ambient sounds (0 - is no sound, 255 - is maximum volume) (indexed by ambient enumeration)
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DLEAF_INCLUDED