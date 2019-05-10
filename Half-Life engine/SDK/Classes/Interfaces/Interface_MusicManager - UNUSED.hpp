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
// Interface_MusicManager.hpp
//
// Class: Interface_MusicManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_MUSIC_MANAGER_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_MUSIC_MANAGER_INCLUDED

//-----------------------------------------------------------------------------
// Purpose: ???
//-----------------------------------------------------------------------------
abstract_class IMusicManager : public Base
{
public:
	// param1 must be one of [2, 3, 4, 5, 6, 7]
	// param2 is "looping" if non-zero
	#define MUSICMANAGER_NOT_LOOPING 0
	#define MUSICMANAGER_LOOPING 1
	virtual void	StartTrack( int track, char looping ) = 0;

	virtual void	StopTrack() = 0;

	virtual void	FadeoutTrack() = 0;

	virtual bool	IsPlayingTrack( int track ) = 0;

	// Returns whether current track is number 1 ???
	virtual bool	Unknown1() = 0;
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_MUSIC_MANAGER_INCLUDED