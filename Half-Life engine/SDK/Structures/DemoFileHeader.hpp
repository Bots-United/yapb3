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
// DemoFileHeader.hpp
//
// Structure: DemoFileHeader_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DEMO_FILE_HEADER_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_DEMO_FILE_HEADER_INCLUDED

const char DemoFileHeaderName[] = {"HLDEMO"};
const unsigned char DEMO_PROTOCOL (5u);	// NOTE: Change this any time the file version of the demo changes!

#define	MAX_OSPATH 260	// max length of a filesystem pathname

struct DemoFileHeader_t
{
	char fileStamp[sizeof (DemoFileHeaderName)];	// Should be "HLDEMO"
	int  protocol;									// Should be DEMO_PROTOCOL (Demo version)
	int  networkProtocol;							// Should be PROTOCOL_VERSION (Network protocol version)
	char mapName[MAX_OSPATH];						// Name of map
	char gameDirectory[MAX_OSPATH];					// Name of game directory (com_gamedir)
	int  serverCRC;									// To determine if client is playing hacked .map. (entities lump is skipped)
	int  directoryOffset;							// Offset of Entry Directory.
};

struct DemoEntry_t
{
	// Demo lump types
	enum EntryType_t
	{
		DEMO_STARTUP,	// This lump contains startup info needed to spawn into the server
		DEMO_NORMAL		// This lump contains playback info of messages, etc., needed during playback.
	};

	DECLARE_TYPED_ENUMERATION (unsigned int, Flag_t)
	{
		Flag_None,

		Flag_HasTitle    = BIT (0u),
		Flag_UNKNOWN     = BIT (1u),	// Never used!
		Flag_PlayCDTrack = BIT (2u),
		Flag_FadeInSlow  = BIT (3u),
		Flag_FadeInFast  = BIT (4u),
		Flag_FadeOutSlow = BIT (5u),
		Flag_FadeOutFast = BIT (6u)
	};

	EntryType_t entryType;		// DEMO_STARTUP or DEMO_NORMAL
	char        title[64u];		// Example: "LOADING"
	Flag_t      flags;
	int         CDTrack;		// By default == -1
	float       playbackTime;	// Time of track
	int         playbackFrames;	// # of frames in track
	int         offset;			// File offset of track data
	int         length;			// Length of track
};

struct DemoDirectory_t
{
	unsigned int  numEntries;	// Number of tracks ("LOADING" & "Playback") (@note: maximum value is 1024!)
	DemoEntry_t  *entries;		// Track entry info buffer
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_DEMO_FILE_HEADER_INCLUDED