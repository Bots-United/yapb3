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
// sound_channel.hpp
//
// Description: Sound channels for pfnEmitSound()/pfnBuildSoundMsg() functions.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SOUND_CHANNEL_INCLUDED
#define SOUND_CHANNEL_INCLUDED

enum SoundChannel_t
{
	SoundChannel_Auto,

	SoundChannel_Weapon,
	SoundChannel_Voice,
	SoundChannel_Item,
	SoundChannel_Body,
	SoundChannel_Stream,					// allocate stream channel from the static or dynamic area
	SoundChannel_Static,					// allocate channel from the static area

	SoundChannel_NetworkVoice_Base,			// allocate channel for network voice data (voice data coming across the network)
	SoundChannel_NetworkVoice_End = 500u	// network voice data reserves slots ('SoundChannel_NetworkVoice_Base' through 'SoundChannel_NetworkVoice_End')
};

#endif	// ifndef SOUND_CHANNEL_INCLUDED