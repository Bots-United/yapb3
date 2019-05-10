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
// DPlane.hpp
//
// Structure: DPlane_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DPLANE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_DPLANE_INCLUDED

struct DPlane_t : public Math::Plane
{
	// BSP file related stuff
	enum Type_t
	{
		// 'Type_X', 'Type_Y' and 'Type_Z' are axial planes (Plane is perpendicular to given axis)
		Type_X,		// Axial plane, in X. (normal == [±1.0,  0.0,  0.0])
		Type_Y,		// Axial plane, in Y. (normal == [ 0.0, ±1.0,  0.0])
		Type_Z,		// Axial plane, in Z. (normal == [ 0.0,  0.0, ±1.0])

		// 'Type_AnyX', 'Type_AnyY' and 'Type_AnyZ' are non-axial planes snapped to the nearest (Dominant axis (axis along which projection of normal has greatest magnitude))
		Type_AnyX,	// Non axial plane, roughly toward X.
		Type_AnyY,	// Non axial plane, roughly toward Y.
		Type_AnyZ	// Non axial plane, roughly toward Z.
	} type;	// Type of plane, depending on normal vector. (for fast side tests)
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DPLANE_INCLUDED