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
// EdictAreaNode.hpp
//
// Structure: EdictAreaNode_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_EDICT_AREA_NODE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_EDICT_AREA_NODE_INCLUDED

struct EdictAreaNode_t
{
	enum ChildrenType_t
	{
		ChildrenType_Front,
		ChildrenType_Back,

		ChildrenType_Total
	};

	int              axis;	// -1 = leaf node, other values from 'Math::Vector3D::AxisID_t'
	float            distance;
	EdictAreaNode_t *children[ChildrenType_Total];

	Link_t           triggerEdicts;
	Link_t           solidEdicts;
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_EDICT_AREA_NODE_INCLUDED