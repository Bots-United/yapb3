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
// Voice definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef VOICE_DEFINITIONS_INCLUDED
#define VOICE_DEFINITIONS_INCLUDED

#include <Half-Life engine/SDK/Voice/circularbuffer.hpp>

#include <Half-Life engine/SDK/Voice/Voice_sound_engine_interface.hpp>
#include <Half-Life engine/SDK/Voice/voice_gain.hpp>
#include <Half-Life engine/SDK/Voice/voice_mixer_controls.hpp>
#include <Half-Life engine/SDK/Voice/Voice_wavefile.hpp>
#include <Half-Life engine/SDK/Voice/riff.hpp>
#include <Half-Life engine/SDK/Voice/snd_audio_source.hpp>
#include <Half-Life engine/SDK/Voice/snd_wave_source.hpp>
#include <Half-Life engine/SDK/Voice/snd_wave_data.hpp>
#include <Half-Life engine/SDK/Voice/snd_fixedint.hpp>
#include <Half-Life engine/SDK/Voice/snd_mix_buf.hpp>
#include <Half-Life engine/SDK/Voice/snd_device.hpp>
#include <Half-Life engine/SDK/Voice/snd_wave_mixer_private.hpp>
#include <Half-Life engine/SDK/Voice/snd_wave_mixer.hpp>

#include <Half-Life engine/SDK/Voice/Voice.hpp>

#endif // ifndef VOICE_DEFINITIONS_INCLUDED