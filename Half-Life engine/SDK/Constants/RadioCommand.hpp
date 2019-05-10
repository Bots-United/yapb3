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
// RadioCommand.hpp
//
// Description: Counter-Strike 1.6 radio messages.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef RADIO_COMMAND_INCLUDED
#define RADIO_COMMAND_INCLUDED

enum RadioCommand_t
{
	// Radio menu 1. (Radio Commands)
	RadioCommand_CoverMe,
	RadioCommand_YouTakePoint,
	RadioCommand_HoldPosition,
	RadioCommand_RegroupTeam,
	RadioCommand_FollowMe,
	RadioCommand_TakingFire,

	// Radio menu 2. (Group Radio Commands)
	RadioCommand_GoGoGo,
	RadioCommand_FallBack,
	RadioCommand_StickTogether,
	RadioCommand_GetInPosition,
	RadioCommand_StormTheFront,
	RadioCommand_ReportTeam,

	// Radio menu 3. (Radio Responses/Reports)
	RadioCommand_Affirmative,
	RadioCommand_EnemySpotted,
	RadioCommand_NeedBackup,
	RadioCommand_SectorClear,
	RadioCommand_InPosition,
	RadioCommand_ReportingIn,
	RadioCommand_ShesGonnaBlow,
	RadioCommand_Negative,
	RadioCommand_EnemyDown,

	RadioCommand_Total	// Total of Counter-Strike 1.6 radio messages.
};

const char *const RadioCommandAliasNames[RadioCommand_Total] =
{
	// Radio menu 1. (Radio Commands)
	"coverme",
	"takepoint",
	"holdpos",
	"regroup",
	"followme",
	"takingfire",

	// Radio menu 2. (Group Radio Commands)
	"go",
	"fallback",
	"sticktog",
	"getinpos",
	"stormfront",
	"report",

	// Radio menu 3. (Radio Responses/Reports)
	"roger",
	"enemyspot",
	"needbackup",
	"sectorclear",
	"inposition",
	"reportingin",
	"getout",
	"negative",
	"enemydown"
};

#endif	// ifndef RADIO_COMMAND_INCLUDED