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
// VirtualTableFunctionID.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_GAME_RULES_VIRTUAL_TABLE_FUNCTION_ID_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_GAME_RULES_VIRTUAL_TABLE_FUNCTION_ID_INCLUDED

/**
* A few notes about all of the following functions:
*   - Not all functions will do as you expect on all mods.
*     If a function does not do what you would believe it should
*     DO NOT file a bug report, you will be ignored.
*
*   - Passing invalid parameters has potential to crash the server
*     So be careful, and adequately test!
*
*   - All functions take (and pass) a "this" index as the first param.
*     This is the entity from which the function is being executed on.
*
*   - All functions and forwards (eg: {Register,Execute}VirtualTableFunctionID_t[B]) require
*     the mod to have the pev and base keys in addition to the function
*     keys for the corresponding mod/operating system in hamdata.ini
*
*   - Some functions that return booleans may need to be logically ANDed
*     to get the results desired. E.g: if (ExecuteFunction (VirtualTableFunctionID_TS_IsObjective, this) & 0x0000FFFF != 0) { // true.. }
*     because the module will return the full integer value.
*/
enum VirtualTableFunctionID_t
{
	VirtualTableFunctionID_FAllowFlashlight = 3u,	// BOOL HalfLifeMultiplay::FAllowFlashlight(void)
	VirtualTableFunctionID_RestartRound = 64u,	// void HalfLifeMultiplay::RestartRound(void)

	/**
	* DONT USE ME LOL
	*/
	VirtualTableFunctionID_Total
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_GAME_RULES_VIRTUAL_TABLE_FUNCTION_ID_INCLUDED