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
// meta_return_flag.hpp
//
// Enumeration: ReturnFlag_t
//
// Description: Flags returned by a plugin's API function.
//
// NOTE: Order is crucial, as greater/less comparisons are made.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef META_RETURN_TYPE_INCLUDED
#define META_RETURN_TYPE_INCLUDED

enum ReturnFlag_t
{
	MRES_UNSET,

	MRES_IGNORED,	// plugin didn't take any action (Calls target function, returns normal value)
	MRES_HANDLED,	// plugin did something, but real function should still be called (Tells the module you did something, still calls target function and returns normal value)
	MRES_OVERRIDE,	// call real function, but use my return value (Still calls the target function, but returns whatever is set by you)
	MRES_SUPERCEDE	// skip real function; use my return value (Block the target call, and use your return value (if applicable))
};

#endif	// ifndef META_RETURN_TYPE_INCLUDED