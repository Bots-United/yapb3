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
// Server.hpp
//
// Description: Some constants for 'Structures::Server_t' structure.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_SERVER_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_SERVER_INCLUDED

namespace Server
{
	// Can be found by string "Client %s:%s sent a bogus command packet\n" in function SV_ParseMove().
	const unsigned char signature[] = {"\xA1\x2A\x2A\x2A\x2A\x85\x2A\x0F\x84\x2A\x2A\x2A\x2A\xA1\x2A\x2A\x2A\x2A\x83\x2A\x00\x75\x2A\x8B\x48\x04\x85\xC9\x0F\x84\x2A\x2A\x2A\x2A"};

	enum
	{
		Offset_Pointer = 1u
	};

	namespace Static
	{
		// Can be found by string "Sys_InitializeGameDLL called twice, skipping second call\n" in function Host_InitializeGameDLL().
		const unsigned char signature[] = {"\xA1\x2A\x2A\x2A\x2A\x83\xC4\x04\x85\xC0\x74\x2A\x68\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x83\xC4\x04\xC3"};

		enum
		{
			Offset_Pointer = 1u
		};
	}
}

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_SERVER_INCLUDED