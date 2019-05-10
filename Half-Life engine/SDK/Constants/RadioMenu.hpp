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
// RadioMenu.hpp
//
// Description: Counter-Strike 1.6 radio menus.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef RADIO_MENU_INCLUDED
#define RADIO_MENU_INCLUDED

enum RadioMenu_t
{
	RadioMenu_None,

	RadioMenu_1,	// Radio menu 1. (Radio Commands)
	RadioMenu_2,	// Radio menu 2. (Group Radio Commands)
	RadioMenu_3,	// Radio menu 3. (Radio Responses/Reports)

	RadioMenu_Total = RadioMenu_3	// Total of Counter-Strike 1.6 radio menus.
};

enum RadioSelectID_t
{
	// Radio menu 1. (Radio Commands)
	RadioMenu1SelectID_CoverMe = 1u,
	RadioMenu1SelectID_YouTakePoint,
	RadioMenu1SelectID_HoldPosition,
	RadioMenu1SelectID_RegroupTeam,
	RadioMenu1SelectID_FollowMe,
	RadioMenu1SelectID_TakingFire,

	RadioMenu1SelectID_Total = RadioMenu1SelectID_TakingFire,

	// Radio menu 2. (Group Radio Commands)
	RadioMenu2SelectID_GoGoGo = 11u,
	RadioMenu2SelectID_FallBack,
	RadioMenu2SelectID_StickTogether,
	RadioMenu2SelectID_GetInPosition,
	RadioMenu2SelectID_StormTheFront,
	RadioMenu2SelectID_ReportTeam,

	RadioMenu2SelectID_Total = RadioMenu2SelectID_ReportTeam - RadioMenu2SelectID_GoGoGo + 1u,

	// Radio menu 3. (Radio Responses/Reports)
	RadioMenu3SelectID_Affirmative = 21u,
	RadioMenu3SelectID_EnemySpotted,
	RadioMenu3SelectID_NeedBackup,
	RadioMenu3SelectID_SectorClear,
	RadioMenu3SelectID_InPosition,
	RadioMenu3SelectID_ReportingIn,
	RadioMenu3SelectID_ShesGonnaBlow,
	RadioMenu3SelectID_Negative,
	RadioMenu3SelectID_EnemyDown,

	RadioMenu3SelectID_Total = RadioMenu3SelectID_EnemyDown - RadioMenu3SelectID_Affirmative + 1u
};

#endif	// ifndef RADIO_MENU_INCLUDED