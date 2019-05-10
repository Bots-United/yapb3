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
// BSPMapFileHeader.hpp
//
// Structure: BSPMapFileHeader_t
//
// Description: BSP files begin with a header, followed by a directory giving the location and length of each data lump.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_BSP_MAP_FILE_HEADER_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_BSP_MAP_FILE_HEADER_INCLUDED

struct BSPMapFileHeader_t
{
	enum
	{
		CurrentVersion = 30u	// Map version constant for the Half-Life 1.
	};

	unsigned int version;				// BSP header version number (This must be 30 for a BSP-30 file)

	struct Lump_t
	{
		enum ID_t
		{
			ID_Entities,		// Entities lump is just a long, null-terminated string
			ID_Planes,			// Planes lump contains plane structures.
			ID_Textures,		// Textures lump begins with a header, followed by offsets to miptex structures, then miptex structures.
			ID_Vertexes,		// Vertices lump contains vertex structures.
			ID_Visibility,		// Visibility lump is a huge bitfield of unknown format.
			ID_Nodes,			// Nodes lump contains node structures.
			ID_TextureInfo,		// Texinfo lump contains texinfo structures.
			ID_Faces,			// Faces lump contains face structures
			ID_Lighting,		// Lighting lump is of unknown format
			ID_ClipNodes,		// Clipnodes lump contains clip node structures
			ID_Leafs,			// Leaves lump contains leaf structures
			ID_MarkSurfaces,	// Marksurfaces lump is array of unsigned short indices into faces lump. Leaves index into marksurfaces, which index into faces; but nodes index into faces directly.
			ID_Edges,			// Edges lump contains edge structures - note, edge 0 is never used, because 0 can't be negated
			ID_SurfEdges,		// Surfedges lump is array of signed int indices into edge lump, where a negative index indicates using the referenced edge in the opposite direction.  Faces index into surfedges, which index into edges, which finally index into vertices.
			ID_Models,			// Models lump contains model structures - these are entity-tied BSP models like func_doors and such

			ID_Total			// This must be the last enum in the list
		};

		unsigned int fileStartOffset;	// Offset to lump entry, in bytes, from start of BSP file
		unsigned int length;			// Size/Length of lump entry in BSP file, in bytes
	}            lumps[Lump_t::ID_Total];	// number of lumps which the BSP file has
};	// general BSP map file header information structure

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_BSP_MAP_FILE_HEADER_INCLUDED