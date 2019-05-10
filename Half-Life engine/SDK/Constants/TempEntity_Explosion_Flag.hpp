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
// TempEntity_Explosion_Flag.hpp
//
// Description: The some explosion effect flags to control performance/aesthetic features.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TEMP_ENTITY_EXPLOSION_FLAG_INCLUDED
#define TEMP_ENTITY_EXPLOSION_FLAG_INCLUDED

enum TempEntity_Explosion_Flag_t
{
	TempEntity_Explosion_Flag_None,							// all flags clear makes default Half-Life explosion
	TempEntity_Explosion_Flag_NoAdditive       = BIT (0u),	// sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
	TempEntity_Explosion_Flag_NoDdynamicLights = BIT (1u),	// do not render dynamic lights
	TempEntity_Explosion_Flag_NoSound          = BIT (2u),	// do not play client explosion sound
	TempEntity_Explosion_Flag_NoParticles      = BIT (3u)	// do not draw particles
};

#endif	// ifndef TEMP_ENTITY_EXPLOSION_FLAG_INCLUDED