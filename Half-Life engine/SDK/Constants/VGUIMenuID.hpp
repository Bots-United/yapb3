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
// VGUIMenuID.hpp
//
// Enumeration: VGUIMenuID_t
//
// Description: Counter-Strike VGUI menu ID's. (VGUI menus (since latest steam updates is obsolete, but left for old cs))
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_ID_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_ID_INCLUDED

enum MenuID_t
{
	MenuID_None,

	MenuID_ChooseTeam0       = 1u,	/// When just connected???
	MenuID_ChooseTeam1       = 2u,	/// When change team???
	MenuID_ChooseAppearance  = 3u,

	MenuID_Buy               = 4u,
	MenuID_Buy_Pistol        = 5u,
	MenuID_Buy_Rifle         = 6u,
	MenuID_Buy_MachineGun    = 7u,
	MenuID_Buy_ShotGun       = 8u,
	MenuID_Buy_SubMachineGun = 9u,
	MenuID_Buy_Equipment     = 10u,

	MenuID_Radio_1           = 11u,
	MenuID_Radio_2           = 12u,
	MenuID_Radio_3           = 13u,

	MenuID_ClientBuy         = 14u,
};

enum VGUIMenuID_t
{
	// CS-specific menus.
	VGUIMenuID_TeamSelect = 2u,					// menu select team

	VGUIMenuID_ModelSelect_Terrorist = 26u,		// terrorist select menu
	VGUIMenuID_ModelSelect_CounterTerrorist,	// counter-terorist select menu

	VGUIMenuID_Buy,
	VGUIMenuID_Buy_Pistol,
	VGUIMenuID_Buy_ShotGun,
	VGUIMenuID_Buy_Rifle,
	VGUIMenuID_Buy_SubMachineGun,
	VGUIMenuID_Buy_MachineGun,
	VGUIMenuID_Buy_Equipment,

	VGUIMenuID_Total
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_MENU_ID_INCLUDED