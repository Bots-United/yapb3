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
// TypeDefinitions.hpp
//
// Description: Header file containing Meta mod definitions. (Operating system specific macros, functions and type definitions)
//				Types definitions for these are provided in HLSDK engine/eiface.h, but I didn't like the names (APIFUNCTION, APIFUNCTION2, NEW_DLL_FUNCTIONS_FN).
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef META_MOD_SDK_TYPE_DEFINITIONS_INCLUDED
#define META_MOD_SDK_TYPE_DEFINITIONS_INCLUDED

typedef BOOL (*GetEntityAPI_FN)       (HalfLifeEngine::SDK::Structures::DLLFunction_t    *DLLFunctionTable,    int  interfaceVersion);
typedef BOOL (*GetEntityAPI2_FN)      (HalfLifeEngine::SDK::Structures::DLLFunction_t    *DLLFunctionTable,    int *interfaceVersion);
typedef BOOL (*GetNewDLLFunctions_FN) (HalfLifeEngine::SDK::Structures::NewDLLFunction_t *newDLLFunctionTable, int *interfaceVersion);
typedef BOOL (*GetEngineFunctions_FN) (HalfLifeEngine::SDK::Structures::EngineFunction_t *engineFunctionTable, int *interfaceVersion);

#endif	// ifndef META_MOD_SDK_TYPE_DEFINITIONS_INCLUDED