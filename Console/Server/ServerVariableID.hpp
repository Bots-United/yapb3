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
// ServerVariableID.hpp
//
// Description: Bot/server console variables indexes used for 'ServerConsoleVariableManager' class.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SERVER_CONSOLE_VARIABLE_ID_INCLUDED
#define SERVER_CONSOLE_VARIABLE_ID_INCLUDED

enum VariableID_t
{
	// Server console variables...,
	VariableID_C4Timer,
	VariableID_BuyTime,
	VariableID_FriendlyFire,
	VariableID_RoundTime,
	VariableID_Gravity,	/// @note WE ALSO CAN GET VALUE FROM 'g_halfLifeEngine->GetPlayerMove ().moveVariables->gravity'.
	VariableID_Developer,
	VariableID_FreezeTime,
	VariableID_FootSteps,	/// @note WE ALSO CAN GET VALUE FROM 'g_halfLifeEngine->GetPlayerMove ().moveVariables->footsteps'.
	VariableID_VoiceEnable,	// Globally enable or disable voice.
	VariableID_Voice_codec,	// Specifies which voice codec DLL to use in a game. Set to the name of the DLL without the extension.
	VariableID_Voice_quality,
	VariableID_AllTalk,
	VariableID_FreeForAll,
	VariableID_SkyColor_Red,	/// @note WE ALSO CAN GET VALUE FROM 'g_halfLifeEngine->GetPlayerMove ().moveVariables->skycolor_r'.
	VariableID_SkyColor_Green,	/// @note WE ALSO CAN GET VALUE FROM 'g_halfLifeEngine->GetPlayerMove ().moveVariables->skycolor_g'.
	VariableID_SkyColor_Blue,	/// @note WE ALSO CAN GET VALUE FROM 'g_halfLifeEngine->GetPlayerMove ().moveVariables->skycolor_b'.

	// and AMXX modules console variables too..., :{P
	VariableID_CSDMActive,

	// total of all server console variables.
	VariableID_Total
};

enum VariableValue_Voice_quality_t
{
	VariableValue_Voice_quality_Total = static_cast <unsigned char> (-1)
};

#endif	// ifndef SERVER_CONSOLE_VARIABLE_ID_INCLUDED