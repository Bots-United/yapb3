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
// Studio.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_STUDIO_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_STUDIO_INCLUDED

enum StudioDrawModelFlag_t
{
	StudioDrawModelFlag_None,

	STUDIO_RENDER,
	STUDIO_EVENTS
};

#define	MIPLEVELS			4
#define	MAXLIGHTMAPS		4

#define ALIAS_Z_CLIP_PLANE	5

// flags in finalvert_t.flags
#define ALIAS_LEFT_CLIP				0x0001
#define ALIAS_TOP_CLIP				0x0002
#define ALIAS_RIGHT_CLIP			0x0004
#define ALIAS_BOTTOM_CLIP			0x0008
#define ALIAS_Z_CLIP				0x0010
#define ALIAS_ONSEAM				0x0020
#define ALIAS_XY_CLIP_MASK			0x000F

#define	ZISCALE	((float)0x8000)

#define CACHE_SIZE	32		// used to align key data structures

enum synctype_t
{
	ST_SYNC,
	ST_RAND
};

// lighting options
enum STUDIO_NF_t
{
	STUDIO_NF_None       = 0u,
	STUDIO_NF_FLATSHADE  = 0x0001,
	STUDIO_NF_CHROME     = 0x0002,
	STUDIO_NF_FULLBRIGHT = 0x0004
};

enum Common_t
{
	MAXSTUDIOTRIANGLES   = 20000u,	// TODO: tune this
	MAXSTUDIOVERTS       = 2048u,	// TODO: tune this
	MAXSTUDIOSEQUENCES   = 256u,	// total animation sequences
	MAXSTUDIOSKINS       = 100u,	// total textures
	MAXSTUDIOSRCBONES    = 512u,	// bones allowed at source movement
	MAXSTUDIOMODELS      = 32u,		// sub-models per model
	MaximumStudioBones   = 128u,	// total bones actually used
	MAXSTUDIOBODYPARTS   = 32u,
	MAXSTUDIOGROUPS      = 16u,
	MAXSTUDIOANIMATIONS  = 512u,	// per sequence
	MAXSTUDIOMESHES      = 256u,
	MAXSTUDIOEVENTS      = 1024u,
	MAXSTUDIOPIVOTS      = 256u,
	MAXSTUDIOCONTROLLERS = 8u
};

// motion flags
#define STUDIO_X		0x0001
#define STUDIO_Y		0x0002
#define STUDIO_Z		0x0004
#define STUDIO_XR		0x0008
#define STUDIO_YR		0x0010
#define STUDIO_ZR		0x0020
#define STUDIO_LX		0x0040
#define STUDIO_LY		0x0080
#define STUDIO_LZ		0x0100
#define STUDIO_AX		0x0200
#define STUDIO_AY		0x0400
#define STUDIO_AZ		0x0800
#define STUDIO_AXR		0x1000
#define STUDIO_AYR		0x2000
#define STUDIO_AZR		0x4000
#define STUDIO_TYPES	0x7FFF
#define STUDIO_RLOOP	0x8000	// controller that wraps shortest distance

// sequence flags
#define STUDIO_LOOPING	0x0001

// bone flags
#define STUDIO_HAS_NORMALS	0x0001
#define STUDIO_HAS_VERTICES 0x0002
#define STUDIO_HAS_BBOX		0x0004
#define STUDIO_HAS_CHROME	0x0008	// if any of the textures have chrome on them

#define RAD_TO_STUDIO		(32768.0f / Math::Pi)
#define STUDIO_TO_RAD		(Math::Pi / 32768.0f)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_STUDIO_INCLUDED