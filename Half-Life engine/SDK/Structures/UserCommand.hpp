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
// UserCommand.hpp
//
// Structure: UserCommand_t
//
// Description: User command is sent to the server each client frame.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef USER_COMMAND_INCLUDED
#define USER_COMMAND_INCLUDED

struct UserCommand_t
{
/*! off=0(0) */	short          lerpMsec;		// Interpolation time on client.
/*! off=2(0.5) */	unsigned char  msecValue;		// Duration in ms of command.
/*! off=4(1) */	Math::Angles3D viewAngles;		// Command view angles. (Player instantaneous view angles)

	// intended velocities
/*! off=16(4) */	float          forwardMove;		// Forward velocity.
/*! off=20(5) */	float          sideMove;		// Sideways velocity.
/*! off=24(6) */	float          upMove;			// Upward velocity.

/*! off=28(7) */	unsigned char  lightLevel;		// Light level at spot where we are standing.
/*! off=30(7.5) */	unsigned short buttons;			// Attack button states.
/*! off=32(8) */	unsigned char  impulse;			// Impulse command issued.
/*! off=33(8.25) */	unsigned char  weaponSelect;	// Current weapon id.

	// Experimental player impact stuff.
/*! off=36(9) */	int            impactIndex;
/*! off=40(10) */	Math::Vector3D impactPosition;
};	// sizeof (UserCommand_t) == 52 (13)

#endif	// ifndef USER_COMMAND_INCLUDED