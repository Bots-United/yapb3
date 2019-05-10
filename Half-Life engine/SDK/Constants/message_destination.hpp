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
// message_destination.hpp
//
// Description: For pfnMessageBegin() function.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MESSAGE_DESTINATION_INCLUDED
#define MESSAGE_DESTINATION_INCLUDED

enum MessageDestination_t
{
	MSG_BROADCAST,		// unreliable to all
	MSG_ONE,			// reliable to one (messageEntity)
	MSG_ALL,			// reliable to all
	MSG_INIT,			// write to the init string
	MSG_PVS,			// ents in PVS of origin (send to clients potentially visible from vector)
	MSG_PAS,			// ents in PAS of origin
	MSG_PVS_R,			// reliable to PVS
	MSG_PAS_R,			// reliable to PAS
	MSG_ONE_UNRELIABLE,	// send to one client, but don't put in reliable stream, put in unreliable datagram (could be dropped)
	MSG_SPEC			// sends to all spectator proxies
};

#endif	// ifndef MESSAGE_DESTINATION_INCLUDED