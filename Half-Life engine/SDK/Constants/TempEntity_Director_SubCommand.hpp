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
// TempEntity_Director_SubCommand.hpp
//
// Enumeration: TempEntity_Director_SubCommand_t
//
// Description: From hltv.h from the HLSDK, these are used in conjunction with 'SVC_DIRECTOR'. (Sub commands)
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_TEMP_ENTITY_DIRECTOR_SUB_COMMAND_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_TEMP_ENTITY_DIRECTOR_SUB_COMMAND_INCLUDED

enum TempEntity_Director_SubCommand_t
{
	DRC_CMD_NONE,		// NULL director command (no operation)

	DRC_CMD_START,		// start director mode
	DRC_CMD_EVENT,		// informs about director command
	DRC_CMD_MODE,		// switches camera modes
	DRC_CMD_CAMERA,		// sets camera registers
	DRC_CMD_TIMESCALE,	// sets time scale
	DRC_CMD_MESSAGE,	// send HUD centerprint (You can show only 8 messages at once)
	DRC_CMD_SOUND,		// plays a particular sound
	DRC_CMD_STATUS,		// status info about broadcast

	// Notice:
	//	The tga file only support 24b color format.
	//	The zip file contain tga only fit 800*600 display resolution of client.
	//	Change the size if most client used difference resolution.
	// Example send show tga command to client:
	//	MessageBegin(MSG_ONE, SVC_DIRECTOR, NULL, client);
	//		WriteByte(sizeof ("gfx/friends.tga") + sizeof (unsigned char));	// command length in bytes
	//		WriteByte(DRC_CMD_BANNER);
	//		WriteString("gfx/friends.tga");	// banner file
	//	MessageEnd();
	DRC_CMD_BANNER,		// banner filename for HLTV gui

	DRC_CMD_FADE,		// send screen fade command (In the client.dll decompiled source code this is force to client command execution)
	DRC_CMD_SHAKE,		// send screen shake command
	DRC_CMD_STUFFTEXT,	// like the normal 'SVC_STUFFTEXT' but as director command

	// New commands....
	DRC_CMD_UNKNOWN1,
	DRC_CMD_UNKNOWN2,
	DRC_CMD_UNKNOWN3
};

enum TempEntity_Director_HLTV_Flag_t
{
	// HLTV_EVENT event flags
	DRC_FLAG_PRIO_MASK  = 0x0F,		// priorities between 0 and 15 (15 most important)
	DRC_FLAG_SIDE       = BIT (4u),
	DRC_FLAG_DRAMATIC   = BIT (5u),	// is a dramatic scene
	DRC_FLAG_SLOWMOTION = BIT (6u),	// would look good in SloMo
	DRC_FLAG_FACEPLAYER = BIT (7u),	// player is doning something (reload/defuse bomb etc)
	DRC_FLAG_INTRO      = BIT (8u),	// is a introduction scene
	DRC_FLAG_FINAL      = BIT (9u),	// is a final scene
	DRC_FLAG_NO_RANDOM  = BIT (10u)	// don't randomize event data
};

enum TempEntity_Director_Constats_t
{
	MAX_DIRECTOR_CMD_PARAMETERS = 4u,
	MAX_DIRECTOR_CMD_STRING     = 128u
};

// commands of the director API function CallDirectorProc(...)
enum DirectorAPI_Command_t
{
	DRCAPI_NOP,					// no operation
	DRCAPI_ACTIVE,				// de/acivates director mode in engine
	DRCAPI_STATUS,				// request proxy information
	DRCAPI_SETCAM,				// set camera n to given position and angle
	DRCAPI_GETCAM,				// request camera n position and angle
	DRCAPI_DIRPLAY,				// set director time and play with normal speed
	DRCAPI_DIRFREEZE,			// freeze directo at this time
	DRCAPI_SETVIEWMODE,			// overview or 4 cameras
	DRCAPI_SETOVERVIEWPARAMS,	// sets parameter for overview mode
	DRCAPI_SETFOCUS,			// set the camera which has the input focus
	DRCAPI_GETTARGETS,			// queries engine for player list
	DRCAPI_SETVIEWPOINTS		// gives engine all waypoints
};
/*! UNKNOWN
enum TempEntity_Director_UNKNOWN_t
{
	DRC_ACTIVE,	// tells client that he's an spectator and will get director command
	DRC_STATUS,	// send status infos about proxy
	DRC_CAMERA,	// set the actual director camera position
	DRC_EVENT	// informs the dircetor about ann important game event
};
*/
#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_TEMP_ENTITY_DIRECTOR_SUB_COMMAND_INCLUDED