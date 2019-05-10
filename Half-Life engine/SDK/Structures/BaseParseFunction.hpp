//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// BaseParseFunction.hpp
//
// Structures: BaseParseFunction_t, SVC_ParseFunction_t, CLC_ParseFunction_t.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_BASE_PARSE_FUNCTION_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_BASE_PARSE_FUNCTION_INCLUDED

struct BaseParseFunction_t
{
	unsigned char  opcode;	// Opcode (From 'Constants::ServerMessageType_t' for 'SVC_ParseFunction_t' structure or 'Constants::ClientMessageType_t' for 'CLC_ParseFunction_t' structure)
	const char    *name;	// Display Name

	private:
		inline BaseParseFunction_t (void);

		inline BaseParseFunction_t &operator = (const BaseParseFunction_t &/*right*/);	// Avoid "warning C4512: 'BaseParseFunction_t' : assignment operator could not be generated".
};

struct SVC_ParseFunction_t : BaseParseFunction_t
{
	typedef void (*ParseFunctionPointer_t) (void);	// Parse function type definition.

	ParseFunctionPointer_t pfnParse;	// Parse function.
};
struct CLC_ParseFunction_t : BaseParseFunction_t
{
	typedef void (*ParseFunctionPointer_t) (Client_t *const client);	// Parse function type definition.

	ParseFunctionPointer_t pfnParse;	// Parse function.
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_BASE_PARSE_FUNCTION_INCLUDED