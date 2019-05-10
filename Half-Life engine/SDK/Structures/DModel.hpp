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
// DModel.hpp
//
// Structure: DModel_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DMODEL_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_DMODEL_INCLUDED

struct DModel_t
{
	Math::Extent3D boundingBox;						// vector location of the lower/upper corner of the model's bounding box
	Math::Vector3D origin;							// vector local origin of the model, usually (0.0, 0.0, 0.0)
	unsigned int   headNode[Constants::Hull_Total];	// indexes of the BSP nodes for each hull (Drawing hull: [0: Hull_Point - index of the first BSP node], Collision hulls: [1: Hull_Human - index of the first clip node, 2: Hull_Large - index of the second clip node, 3: Hull_Head - index of the third clip node])
	unsigned int   visLeafs;						// number of BSP leaves, not including the solid leaf 0
	unsigned int   firstFaceIndex;					// index of the model's first face in faces lump
	unsigned int   facesNum;						// number of faces the model has
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DMODEL_INCLUDED