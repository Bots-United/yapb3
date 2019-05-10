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
// EventInformation.hpp
//
// Structure: event_info_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_EVENT_INFORMATION_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_EVENT_INFORMATION_INCLUDED

struct event_info_t
{
	DECLARE_TYPED_ENUMERATION (unsigned int, Flag_t)
	{
		Flag_None,

		FEV_NOTHOST  = BIT (0u),	// Skip local host for event send.

		// Send the event reliably. You must specify the origin and angles and use
		// PLAYBACK_EVENT_FULL for this to work correctly on the server for anything
		// that depends on the event origin/angles. I.e., the origin/angles are not
		// taken from the invoking edict for reliable events.
		FEV_RELIABLE = BIT (1u),

		// Don't restrict to PAS/PVS, send this event to _everybody_ on the server (useful for stopping CHAN_STATIC
		//  sounds started by client event when client is not in PVS anymore (hwguy in TFC e.g.)).
		FEV_GLOBAL   = BIT (2u),

		FEV_UPDATE   = BIT (3u),	// If this client already has one of these events in its queue, just update the event instead of sending it as a duplicate
		FEV_HOSTONLY = BIT (4u),	// Only send to entity specified as the invoker
		FEV_SERVER   = BIT (5u),	// Only send if the event was created on the server.
		FEV_CLIENT   = BIT (6u)		// Only issue event client side (from shared code)
	};

	struct event_args_t
	{
		DECLARE_TYPED_ENUMERATION (unsigned int, Flag_t)
		{
			FEVENT_ORIGIN = BIT (0u),	// Event was invoked with stated origin
			FEVENT_ANGLES = BIT (1u)	// Event was invoked with stated angles
		};

		Flag_t	flags;

		// Transmitted
		int		entindex;

		Math::Vector3D	origin;
		Math::Angles3D	angles;
		Math::Vector3D	velocity;

		int		ducking;

		float	fparam1;
		float	fparam2;

		int		iparam1;
		int		iparam2;

		int		bparam1;
		int		bparam2;
	};	// sizeof (event_args_t) == 72 (18)

/*!Off=0(0)*/	unsigned short index;	// 0 implies not in use

/*!Off=2(0.5)*/	short packet_index;		// Use data from state info for entity in delta_packet. -1 implies separate info based on event parameter signature
/*!Off=4(1)*/	short entity_index;		// The edict this event is associated with

/*!Off=8(2)*/	float fire_time;		// if non-zero, the time when the event should be fired (fixed up on the client)

/*!Off=12(3)*/	event_args_t args;

// CLIENT ONLY
/*!Off=84(21)*/	Flag_t	  flags;		// Reliable or not, etc.
};	// sizeof (event_info_t) == 88 (22)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_EVENT_INFORMATION_INCLUDED