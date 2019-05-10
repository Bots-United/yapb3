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
// Color.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_COLOR_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_COLOR_INCLUDED

enum ColorID_t
{
	ColorID_None = -1,

	ColorID_Grey,	// (Dampened/Disabled)
	ColorID_Red,
	ColorID_White,
	ColorID_Yelow,

	ColorID_Total
};

const char *const colors[ColorID_Total] =
{
	{"\\d"},	// ColorID_Grey
	{"\\r"},	// ColorID_Red
	{"\\w"},	// ColorID_White
	{"\\w"}		// ColorID_Yelow
};

const char tabulate[] = {"\\R"};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_COLOR_INCLUDED