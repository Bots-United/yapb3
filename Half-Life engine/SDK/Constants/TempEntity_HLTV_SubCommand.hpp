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
// TempEntity_HLTV_SubCommand.hpp
//
// Enumeration: TempEntity_HLTV_SubCommand_t
//
// Description: From hltv.h from the HLSDK, these are used in conjunction with 'SVC_HLTV'. (Sub commands)
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_TEMP_ENTITY_HLTV_SUB_COMMAND_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_TEMP_ENTITY_HLTV_SUB_COMMAND_INCLUDED
/*! UNKNOWN
enum TempEntity_HLTV_Type_t
{
	TYPE_CLIENT,			// client is a normal HL client (default)
	TYPE_PROXY,				// client is another proxy
	TYPE_COMMENTATOR = 3u,	// client is a commentator
	TYPE_DEMO				// client is a demo file
};
*/
enum TempEntity_HLTV_SubCommand_t
{
	HLTV_ACTIVE,	// tells client that he's an spectator and will get director commands
	HLTV_STATUS,	// send status infos about proxy
	HLTV_LISTEN		// tell client to listen to a multicast stream
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_TEMP_ENTITY_HLTV_SUB_COMMAND_INCLUDED