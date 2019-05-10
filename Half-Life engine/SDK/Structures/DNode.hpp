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
// DNode.hpp
//
// Structure: DNode_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DNODE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_DNODE_INCLUDED

struct DNode_t : public DClipNode_t
{
/*	enum ChildrenType_t
	{
		ChildrenType_Front,
		ChildrenType_Back,

		ChildrenType_Total
	};

	unsigned int   planeNum;						// The plane that splits the node (Index into planes lump. Must be in [0, m_bspFile.planes.GetElementNumber()])
	short          children[ChildrenType_Total];	// If positive, id of front/back child node, negative numbers are child leafs: -(leafs + 1)
*/
	DBoundingBox_t boundingBox;						// Bounding box of node and all childs (for sphere culling (frustum culling))

	unsigned short firstFace;						// Index of first Polygons in the node.
	unsigned short facesNum;						// Number of faces in the node (counting both sides)
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DNODE_INCLUDED