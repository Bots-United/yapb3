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
// GlobalVariables.hpp
//
// Structure: GlobalVariables_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef GLOBAL_VARIABLES_INCLUDED
#define GLOBAL_VARIABLES_INCLUDED

struct GlobalVariables_t
{
	DECLARE_TYPED_ENUMERATION (int, TraceFlag_t)
	{
		FTRACE_SIMPLEBOX = BIT (0u)	// Traceline with a simple box.
	};

/*!off=0(0)*/	float                            time;				// Current time (usually tickcount * tick_interval, except during client rendering)
/*!off=4(1)*/	float                            frameTime;			// Time spent on last server or client frame (has nothing to do with think intervals)
/*!off=8(2)*/	float                            forceRetouch;		// (Boolean)
/*!off=12(3)*/	TypeDefinitions::StringOffset_t  mapName;
/*!off=16(4)*/	TypeDefinitions::StringOffset_t  startSpot;
/*!off=20(5)*/	float                            deathmatch;		// (Boolean)
/*!off=24(6)*/	float                            coop;				// (Boolean)
/*!off=28(7)*/	float                            teamPlay;			// (Boolean) (Unused by engine!)
/*!off=32(8)*/	float                            serverFlags;		// (unsigned integer) (Unused by engine!)
/*!off=36(9)*/	float                            foundSecrets;		// (unsigned integer) (Unused by engine!)

/*!off=40(10)*/	Math::Vector3D                   forward;
/*!off=52(13)*/	Math::Vector3D                   upward;
/*!off=64(16)*/	Math::Vector3D                   right;

	// Tracing results after impacting of two entities, for example.
/*!off=76(19)*/	float                            traceIsAllSolid;	// (Boolean)
/*!off=80(20)*/	float                            traceIsStartSolid;	// (Boolean)
/*!off=84(21)*/	float                            traceFraction;
/*!off=88(22)*/	Math::Vector3D                   traceEndPosition;
/*!off=100(25)*/	Math::Plane                      tracePlane;
/*!off=116(29)*/	Classes::Edict                  *traceHitEntity;
/*!off=120(30)*/	float                            traceIsInOpen;		// (Boolean)
/*!off=124(31)*/	float                            traceIsInWater;	// (Boolean)
/*!off=128(32)*/	Constants::PlayerBodyPart_t      traceHitGroup;
/*!off=132(33)*/	TraceFlag_t                      traceFlags;

/*!off=136(34)*/	int                              messageEntity;		// (Unused by engine!)
/*!off=140(35)*/	int                              cdAudioTrack;

/*!off=144(36)*/	int                              maximumClients;	// current "maxplayers" console variable setting
/*!off=148(37)*/	int                              maximumEntities;

/*!off=152(38)*/	const char                      *stringBase;
/*!off=156(39)*/	SaveRestoreData_t               *saveData;			// current saverestore data
/*!off=160(40)*/	Math::Vector3D                   landmarkOffset;	// (Unused by engine!)
};	// sizeof (GlobalVariables_t) == 164 (41)

#endif	// ifndef GLOBAL_VARIABLES_INCLUDED