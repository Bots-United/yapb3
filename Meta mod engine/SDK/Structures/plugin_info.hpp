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
// plugin_info.hpp
//
// Description: Information plugin provides about itself.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLUGIN_INFO_INCLUDED
#define PLUGIN_INFO_INCLUDED

struct PluginInfo_t
{
	const char *const                 interfaceVersion;	// meta_interface version
	const char *const                 name;				// full name of plugin
	const char *const                 version;			// version
	const char *const                 date;				// date
	const char *const                 author;			// author name/email
	const char *const                 URL;				// URL
	const char *const                 logTag;			// log message prefix (unused right now)
	const Constants::PluginLoadTime_t loadable;			// when loadable
	const Constants::PluginLoadTime_t unloadable;		// when unloadable

	inline PluginInfo_t
	(
		const char *const inputInterfaceVersion,
		const char *const inputName,
		const char *const inputVersion,
		const char *const inputDate,
		const char *const inputAuthor,
		const char *const inputURL,
		const char *const inputLogTag,
		const Constants::PluginLoadTime_t inputLoadable,
		const Constants::PluginLoadTime_t inputUnloadable
	) :
		interfaceVersion (inputInterfaceVersion),
		name             (inputName),
		version          (inputVersion),
		date             (inputDate),
		author           (inputAuthor),
		URL              (inputURL),
		logTag           (inputLogTag),
		loadable         (inputLoadable),
		unloadable       (inputUnloadable)
	{ /* VOID */ }

	//
	// Group: Private operators.
	//
	private:
		inline PluginInfo_t &operator = (const PluginInfo_t &/*right*/);	// Avoid "warning C4512: 'PluginInfo_t' : assignment operator could not be generated".
};

#endif	// ifndef PLUGIN_INFO_INCLUDED