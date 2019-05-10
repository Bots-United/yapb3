//
// Copyright � 2003-2010, by YaPB Development Team. All rights reserved.
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
// TeamID.hpp
//
// Description: Counter-Strike team ID's.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TEAM_ID_INCLUDED
#define TEAM_ID_INCLUDED

// Team Defines
enum TeamID_t
{
	TeamID_Unassigned,	// not assigned to a team

	TeamID_Terrorist,
	TeamID_CounterTerrorist,

	TeamID_Spectator,	// spectator team

	TeamID_Total,

	TeamID_AutoSelect
};

const char *const TeamNames[TeamID_Total] =
{
	"UNASSIGNED",

	"TERRORIST",
	"CT",

	"SPECTATOR"
};

const TeamID_t enemyTeamID[TeamArrayID_Total] =
{
	TeamID_CounterTerrorist,
	TeamID_Terrorist
};

#endif	// ifndef TEAM_ID_INCLUDED