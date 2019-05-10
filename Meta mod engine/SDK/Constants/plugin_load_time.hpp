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
// plugin_load_time.hpp
//
// Description: Flags for plugin to indicate when it can be be loaded/unloaded.
//
// NOTE: Order is crucial, as greater/less comparisons are made.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLUGIN_LOAD_TIME_INCLUDED
#define PLUGIN_LOAD_TIME_INCLUDED

enum PluginLoadTime_t
{
	PT_NEVER,
	PT_STARTUP,		// should only be loaded/unloaded at initial hlds execution
	PT_CHANGELEVEL,	// can be loaded/unloaded between maps
	PT_ANYTIME,		// can be loaded/unloaded at any time
	PT_ANYPAUSE		// can be loaded/unloaded at any time, and can be "paused" during a map
};

#endif	// ifndef PLUGIN_LOAD_TIME_INCLUDED