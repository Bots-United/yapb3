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
// Definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef META_MOD_STRUCTURES_DEFINITIONS_INCLUDED
#define META_MOD_STRUCTURES_DEFINITIONS_INCLUDED

#include <Meta mod engine/SDK/structures/function.hpp>
#include <Meta mod engine/SDK/structures/game_DLL_function.hpp>
#include <Meta mod engine/SDK/structures/global.hpp>
#include <Meta mod engine/SDK/structures/HUD_text_parameter.hpp>
#include <Meta mod engine/SDK/structures/plugin_info.hpp>
#include <Meta mod engine/SDK/structures/utility_API.hpp>

// Our structure for storing engine references.
struct engine_t
{
//	engine_t (void);
//	engine_t (const engine_t &);
//	engine_t &operator = (const engine_t &);

	HalfLifeEngine::SDK::Structures::EngineFunction_t  *funcs;		// engine funcs
	HalfLifeEngine::SDK::Structures::GlobalVariables_t *globals;	// engine globals
	HalfLifeEngine::SDK::Structures::EngineFunction_t  *pl_funcs;	// "modified" engine funcs we give to plugins
//	EngineInfo                                          info;		// some special info elements
};

// Info about the game dll/mod.
struct gamedll_t
{
	char               name[_MAX_FNAME];		// ie "cstrike" (from gamedir)
	const char        *desc;					// ie "Counter-Strike"
	char               gamedir[MAX_PATH];		// ie "/home/willday/half-life/cstrike"
	char               pathname[MAX_PATH];		// ie "/home/willday/half-life/cstrike/dlls/cs_i386.so"
	char const        *file;					// ie "cs_i386.so"
	char               real_pathname[MAX_PATH];	// in case pathname overridden by bot, etc
	HINSTANCE          handle;
	GameDLLFunction_t  funcs;					// 'DLLFunctionAPITable', 'newDLLFunctionAPITable'
};

#endif	// ifndef META_MOD_STRUCTURES_DEFINITIONS_INCLUDED