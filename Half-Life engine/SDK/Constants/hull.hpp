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
// hull.hpp
//
// Enumeration: Hull_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HULL_INCLUDED
#define HULL_INCLUDED

enum Hull_t
{
	Hull_Point,	// Point based hull (Math::Vector3D (0.0, 0.0, 0.0), Math::Vector3D (0.0, 0.0, 0.0))
	Hull_Human,	// Normal player (VEC_HULL_MIN, VEC_HULL_MAX)
	Hull_Large,
	Hull_Head,	// Crouched player (VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX)

	Hull_Total	// hard limit
};

enum DrawingHull_t
{
	DrawingHull_Point,	// Point based hull (Math::Vector3D (0.0, 0.0, 0.0), Math::Vector3D (0.0, 0.0, 0.0))

	DrawingHull_Total	// hard limit
};
enum CollisionHull_t
{
	CollisionHull_Human,	// Normal player (VEC_HULL_MIN, VEC_HULL_MAX)
	CollisionHull_Large,
	CollisionHull_Head,		// Crouched player (VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX)

	CollisionHull_Total		// hard limit
};

#endif	// ifndef HULL_INCLUDED