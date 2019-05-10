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
// BotManager.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_MANAGER_INCLUDED
#define BOT_MANAGER_INCLUDED

enum Offset_t
{
	Offset_Pointer                         = GameRules::Offset_Pointer - 1u + sizeof (Classes::GameRules *) + 26u,	// Offset to 'TheBots' pointer from Counter-Strike.

	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	Offset_RadioMessageTimestamps           = 1u,	// (as float * ?) (Using: reinterpret_cast <float *> (botManager)[teamID + 2u * 'GameEventType_RADIO_*' + Offset_RadioMessageTimestamps])

	Offset_MapType                          = 4u,	// Map type (as MapType_t)
	Offset_MapZones                         = 5u,	// ? (as Zone_t[4])
	Offset_MapZonesNumber                   = 121u,	// (as unsigned int)
	Offset_C4_IsPlanted                     = 488u,	// (as bool)
	Offset_C4_PlantedTime                   = 123u,	// (as float)

	Offset_C4_Defuser                       = 125u,	// (as Classes::BasePlayer *)
	Offset_LoosedC4                         = 126u,	// (as Classes::Edict *, but Classes::EntityHandle)
	Offset_LoosedC4NavigationArea           = 128u,	// (as Classes::NavigationArea *)
	Offset_IsRoundEnded                     = 516u,	// (as bool)

	Offset_LastEnemySpottedRadioCommandTime = 178u	// (as float)
};

enum VirtualTableFunctionID_t
{
	// void Classes::BotManager::OnEvent (GameEventType_t, Classes::BaseEntity *, void *)
	VirtualTableFunctionID_OnEvent = 9u
};

#endif	// ifndef BOT_MANAGER_INCLUDED