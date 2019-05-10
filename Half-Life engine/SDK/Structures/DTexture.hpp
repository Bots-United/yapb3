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
// DTexture.hpp
//
// Structure: DTexture_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DTEXTURE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_DTEXTURE_INCLUDED

struct DTexture_t	// Followed by 32-bit offsets (within texture lump) to (m_bspFile.texturesData.GetElementNumber()) miptex structures.
{
	enum { MIP_LEVELS = 4u };

	char         name[16u];				// Name of the texture, for reference from external WAD3 file.
	unsigned int width, height;			// width/height of picture, must be a multiple of 8
	unsigned int offsets[MIP_LEVELS];	// four mip maps stored (0 - Pix[width * height], 1 - Pix[width/2 * height/2], 2 - Pix[width/4 * height/4], 3 - Pix[width/8 * height/8]) (Offsets to texture data, apparently - 0 if texture data not included)
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DTEXTURE_INCLUDED