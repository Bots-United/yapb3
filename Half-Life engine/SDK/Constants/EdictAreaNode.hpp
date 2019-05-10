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
// EdictAreaNode.hpp
//
// Description: Some constants for 'Structures::EdictAreaNode_t' structure.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_EDICT_AREA_NODE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_EDICT_AREA_NODE_INCLUDED

namespace EdictAreaNode
{
	// Can be found by string "Inserted %s with no model\n" in function SV_LinkEdict().
	const unsigned char signature[] = {"\x52\x68\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x83\xC4\x08\x5F\x5E\x5D\xC3\xB9\x2A\x2A\x2A\x2A"};

	enum
	{
		Offset_Pointer = 19u
	};
}

enum EdictAreaNode_t
{
	EdictAreaNode_Depth = 4u,
	EdictAreaNode_Total = 32u
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_EDICT_AREA_NODE_INCLUDED