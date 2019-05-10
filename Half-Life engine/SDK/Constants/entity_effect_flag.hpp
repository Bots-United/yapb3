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
// entity_effect_flag.hpp
//
// Description: Edict->effects flags.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_EFFECT_FLAG_INCLUDED
#define ENTITY_EFFECT_FLAG_INCLUDED

DECLARE_TYPED_ENUMERATION (/*unsigned */int, EntityEffectFlag_t)
{
	EntityEffectFlag_None,

	EF_BRIGHTFIELD        = BIT (0u),	// swirling cloud of particles
	EFLAG_SLERP           = BIT (0u),	// do studio interpolation of this entity
	EF_MUZZLEFLASH        = BIT (1u),	// single frame ELIGHT on entity attachment 0
	EF_BRIGHTLIGHT        = BIT (2u),	// DLIGHT centered at entity origin
	EntityEffect_DimLight = BIT (3u),	// player flashlight
	EF_INVLIGHT           = BIT (4u),	// get lighting from ceiling
	EF_NOINTERP           = BIT (5u),	// don't interpolate the next frame
	EF_LIGHT              = BIT (6u),	// rocket flare glow sprite
	EntityEffect_NoDraw   = BIT (7u)	// don't draw entity
};

#endif	// ifndef ENTITY_EFFECT_FLAG_INCLUDED