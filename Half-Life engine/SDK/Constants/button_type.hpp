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
// button_type.hpp
//
// Description: Buttons types for pfnRunPlayerMove() function.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BUTTON_TYPE_INCLUDED
#define BUTTON_TYPE_INCLUDED

DECLARE_TYPED_ENUMERATION (/*unsigned */int, ButtonType_t)
{
	ButtonType_None,

	IN_ATTACK    = BIT (0u),
	IN_JUMP      = BIT (1u),
	IN_DUCK      = BIT (2u),
	IN_FORWARD   = BIT (3u),
	IN_BACK      = BIT (4u),
	IN_USE       = BIT (5u),
	IN_CANCEL    = BIT (6u),
	IN_LEFT      = BIT (7u),
	IN_RIGHT     = BIT (8u),
	IN_MOVELEFT  = BIT (9u),
	IN_MOVERIGHT = BIT (10u),
	IN_ATTACK2   = BIT (11u),
	IN_RUN       = BIT (12u),
	IN_RELOAD    = BIT (13u),
	IN_ALT1      = BIT (14u),
	IN_SCORE     = BIT (15u)	// used by client.DLL for when scoreboard is held down
};

enum ButtonBits_t
{
	ButtonBits_InMove = IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT
};

#endif	// ifndef BUTTON_TYPE_INCLUDED