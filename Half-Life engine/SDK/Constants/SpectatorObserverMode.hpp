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
// SpectatorObserverMode.hpp
//
// Enumeration: SpectatorObserverMode_t
//
// Description: For "player" entity this is observer mode, stored in pev->iuser1.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_SPECTATOR_OBSERVER_MODE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_SPECTATOR_OBSERVER_MODE_INCLUDED

enum SpectatorObserverMode_t
{
	SpectatorObserverMode_None,					// When just connected and not spawned yet OR alive.

	SpectatorObserverMode_LockedChaseCamera,	// 'Locked Chase Camera'
	SpectatorObserverMode_FreeChaseCamera,		// 'Free Chase Camera'
	SpectatorObserverMode_FreeLook,				// 'Free Look'
	SpectatorObserverMode_FirstPerson,			// 'First Person'
	SpectatorObserverMode_FreeMapOverview,		// 'Free Map Overview'
	SpectatorObserverMode_ChaseMapOverview		// 'Chase Map Overview'
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_SPECTATOR_OBSERVER_MODE_INCLUDED