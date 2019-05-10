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
// Description: Header file containing Half-Life engine definitions. (Operating system specific macros, functions and type definitions)
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TYPE_DEFINITIONS_INCLUDED
#define TYPE_DEFINITIONS_INCLUDED

typedef int          EntityOffset_t;	// More explicit than 'int'.
typedef int          StringOffset_t;
typedef unsigned int CRC32_t;
typedef void (*ConsoleCommandCallbackFunction_t) (void);
typedef void (*SV_InactivateClientsFunction_t) (void);
typedef void (*GL_DisableMultitextureFunction_t) (void);
typedef void (*const Cmd_TokenizeStringFunctionPointer_t) (const char *text);
typedef const unsigned int (*SV_CalcPingFunction_t) (const Structures::Client_t *const client);
typedef void (*Cvar_DirectSetFunction_t) (Classes::ConsoleVariable *const variable, const char *const value);
typedef void (*Con_PrintfFunction_t) (const char *const format, ...);
typedef void (*UTIL_LogPrintf_t) (const char *const format, ...);
typedef unsigned char * GCFFileBuffer_t;

typedef unsigned short DMarkSurface_t;
typedef int            DSurfEdge_t;

#endif	// ifndef TYPE_DEFINITIONS_INCLUDED