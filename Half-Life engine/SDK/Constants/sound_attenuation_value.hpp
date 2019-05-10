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
// sound_attenuation_value.hpp
//
// Description: Common sound attenuation values for pfnEmitSound()/pfnEmitAmbientSound()/pfnBuildSoundMsg() functions.
//				An attenuation of 0 will play full volume everywhere in the level.
//				Larger attenuations will drop off. (max 4 attenuation)
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SOUND_ATTENUATION_VALUE_INCLUDED
#define SOUND_ATTENUATION_VALUE_INCLUDED

static const float SoundAttenuation_None   (0.0f);	// Sound everywhere. (MapSize)
static const float SoundAttenuation_Normal (0.8f);	// Large radius. (1250)
static const float SoundAttenuation_Static (1.25f);	// Medium radius. (800)
static const float SoundAttenuation_Idle   (2.0f);	// Small radius. (500)

static const float SoundNominalClipDistance (1000.0f);

#define SOUND_ATTENUATION_TO_RADIUS(attenuation) (HalfLifeEngine::SDK::Constants::SoundNominalClipDistance / attenuation)

enum soundlevel_t
{
	SNDLVL_NONE    = 0,

	SNDLVL_20dB    = 20,	// rustling leaves
	SNDLVL_25dB    = 25,	// whispering
	SNDLVL_30dB    = 30,	// library
	SNDLVL_35dB    = 35,
	SNDLVL_40dB    = 40,
	SNDLVL_45dB    = 45,	// refrigerator

	SNDLVL_50dB    = 50,	// average home (3.9)
	SNDLVL_55dB    = 55,	// (3.0)

	SNDLVL_60dB    = 60,	// normal conversation, clothes dryer (2.0)
	SNDLVL_IDLE    = SNDLVL_60dB,

	SNDLVL_65dB    = 65,	// washing machine, dishwasher (1.5)
	SNDLVL_STATIC  = SNDLVL_65dB + 1,	// (1.25)

	SNDLVL_70dB    = 70,	// car, vacuum cleaner, mixer, electric sewing machine (1.0)

	SNDLVL_75dB    = 75,	// busy traffic (0.8)
	SNDLVL_NORM    = SNDLVL_75dB,

	SNDLVL_80dB    = 80,	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile (0.7)
	SNDLVL_TALKING = SNDLVL_80dB,
	SNDLVL_85dB    = 85,	// average factory, electric shaver (0.6)
	SNDLVL_90dB    = 90,	// screaming child, passing motorcycle, convertible ride on frw (0.5)
	SNDLVL_95dB    = 95,
	SNDLVL_100dB   = 100,	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer (0.4)
	SNDLVL_105dB   = 105,	// helicopter, power mower
	SNDLVL_110dB   = 110,	// snowmobile drvrs seat, inboard motorboat, sandblasting
	SNDLVL_120dB   = 120,	// auto horn, propeller aircraft
	SNDLVL_130dB   = 130,	// air raid siren

	SNDLVL_140dB   = 140,	// THRESHOLD OF PAIN, gunshot, jet engine (0.(2))
	SNDLVL_GUNFIRE = SNDLVL_140dB,

	SNDLVL_150dB   = 150,	// (0.2)
	SNDLVL_180dB   = 180	// rocket launching (0.154)
};

#define ATTN_TO_SNDLVL(attenuation) (soundlevel_t)(int)((attenuation) ? (50 + 20 / ((float)attenuation)) : 0)
#define SNDLVL_TO_ATTN(soundLevel) ((soundLevel > 50) ? (20.0f / (float)(soundLevel - 50)) : 4.0)

// This is a limit due to network encoding.
// It encodes attenuation * 64 in 8 bits, so the maximum is (255 / 64)
static const float MAX_ATTENUATION (3.98f);

#endif	// ifndef SOUND_ATTENUATION_VALUE_INCLUDED