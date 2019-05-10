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
// MenuKey.hpp
//
// Description: Counter-Strike menu keys.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_KEY_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_KEY_INCLUDED

enum Key_t
{
	Key_None,

	Key_1,
	Key_2,
	Key_3,
	Key_4,
	Key_5,
	Key_6,
	Key_7,
	Key_8,
	Key_9,

	Key_0,

	Key_Total
};

DECLARE_TYPED_ENUMERATION (unsigned short, KeyBit_t)
{
	KeyBit_None = Key_None,	// no valid slots means that the menu should be turned off

	KeyBit_1    = BIT (Key_1 - 1u),
	KeyBit_2    = BIT (Key_2 - 1u),
	KeyBit_3    = BIT (Key_3 - 1u),
	KeyBit_4    = BIT (Key_4 - 1u),
	KeyBit_5    = BIT (Key_5 - 1u),
	KeyBit_6    = BIT (Key_6 - 1u),
	KeyBit_7    = BIT (Key_7 - 1u),
	KeyBit_8    = BIT (Key_8 - 1u),
	KeyBit_9    = BIT (Key_9 - 1u),

	KeyBit_0    = BIT (Key_0 - 1u)
};

typedef unsigned char ItemIndex_t, PageIndex_t;

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_KEY_INCLUDED