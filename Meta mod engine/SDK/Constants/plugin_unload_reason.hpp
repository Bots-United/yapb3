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
// plugin_unload_reason.hpp
//
// Description: Flags to indicate why the plugin is being unloaded.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLUGIN_UNLOAD_REASON_INCLUDED
#define PLUGIN_UNLOAD_REASON_INCLUDED

enum PluginUnloadReason_t
{
	PNL_NULL,
	PNL_INI_DELETED,	// was deleted from plugins.ini
	PNL_FILE_NEWER,		// file on disk is newer than last load
	PNL_COMMAND,		// requested by server/console command
	PNL_CMD_FORCED,		// forced by server/console command
	PNL_DELAYED,		// delayed from previous request; can't tell origin

	//only used for 'real_reason' on MPlugin::unload()
	PNL_PLUGIN,			// requested by plugin function call
	PNL_PLG_FORCED,		// forced by plugin function call
	PNL_RELOAD			// forced unload by reload()
};

#endif	// ifndef PLUGIN_UNLOAD_REASON_INCLUDED