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
// SoundFlag.hpp
//
// Enumeration: SoundFlag_t
//
// Description: Sound flags for pfnEmitSound()/pfnBuildSoundMsg() functions.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_SOUND_FLAG_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_SOUND_FLAG_INCLUDED

DECLARE_TYPED_ENUMERATION (unsigned int, SoundFlag_t)
{
	SoundFlag_None,

	SND_VOLUME       = BIT (0u),	// a scaled byte
	SND_ATTENUATION  = BIT (1u),	// a byte
	SND_LARGE_INDEX  = BIT (2u),	// a send sound as short
	SND_PITCH        = BIT (3u),	// a byte
	SND_SENTENCE     = BIT (4u),	// set if sound num is actually a sentence num

	SND_STOP         = BIT (5u),	// stop the sound
	SND_CHANGE_VOL   = BIT (6u),	// change sound volume
	SND_CHANGE_PITCH = BIT (7u),	// change sound pitch
	SND_SPAWNING     = BIT (8u),	// we're spawing, used in some cases for ambients

	// local flags (never sending acorss the net)
	SND_LOCALSOUND   = BIT (9u),	// not paused, not looped, for internal use
	SND_STOP_LOOPING = BIT (10u)	// stop all looping sounds on the entity.
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_SOUND_FLAG_INCLUDED