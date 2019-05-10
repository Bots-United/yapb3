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
// PlayerInfo.hpp
//
// Structure: PlayerInfo_t (Past name "player_info_t")
//
// Description: Engine player info, no game related infos here.
//
// Purpose: This data structure is filled in by the engine when the client .DLL requests information about
//	other players that the engine knows about.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_PLAYER_INFO_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_PLAYER_INFO_INCLUDED

// ------------------  Player Model Animation Info ----------------
struct PlayerInfo_t
{
/*off=0*/	int  userid;	// User id on server (local server user ID, unique while server is running)
/*off=1*/	char userinfo[Constants::MaximumInfoStringLength];	// User info string (Settings for player)

	// scoreboard information
/*off=65*/	char name[Constants::MaximumClientNameLength];	// Name

/*off=73*/	BOOL spectator;	// Spectator or not, unused (true if player is the HLTV proxy (ishltv))

/*off=74*/	int             ping;
/*off=75*/	int             packetLoss;

	// skin information
/*off=76*/	char model[Constants::MaximumPathNameLength];
/*off=92*/	int topcolor;
/*off=93*/	int bottomcolor;

/*off=94*/	int             renderFrame;	// last frame rendered

	// Gait frame estimation
/*off=95*/	int             gaitSequence;
/*off=96*/	float           gaitFrame;
/*off=97*/	float           gaitYaw;
/*off=98*/	Math::Vector3D  prevGaitOrigin;

/*off=101*/	Customization_t customData;
/*off=142*/	char hashedcdkey[16];	// hashed cd key

/*off=146*/	Classes::Steam::ID steamID;	// friends identification number
};	// sizeof (PlayerInfo_t) == 592 (148)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_PLAYER_INFO_INCLUDED