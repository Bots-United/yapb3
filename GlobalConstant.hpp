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
// GlobalConstant.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef GLOBAL_CONSTANT_INCLUDED
#define GLOBAL_CONSTANT_INCLUDED

enum BotPingValue_t
{
	BotPingValue_BotTag  = -1,
	BotPingValue_Minimum = 0u,		// minimum bot ping value
	BotPingValue_Maximum = 4095u	// maximum bot ping value
};

enum BotSkillValue_t
{
	BotSkillValue_Minimum = 1u,		// minimum bot skill value
	BotSkillValue_Maximum = 100u	// maximum bot skill value
};

enum TeamArrayID_t
{
	TeamArrayID_Terrorist,			// (HalfLifeEngine::SDK::Constants::TeamID_Terrorist - 1u)
	TeamArrayID_CounterTerrorist,	// (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist - 1u)

	TeamArrayID_Total				// (HalfLifeEngine::SDK::Constants::MaximumPlayableTeamsNumber)
};
const TeamArrayID_t enemyTeamArrayID[TeamArrayID_Total] =
{
	TeamArrayID_CounterTerrorist,
	TeamArrayID_Terrorist
};

enum BotSkillLevel_t
{
	BotSkillLevel_Stupid       = 20u,	// (BotSkillValue_Minimum,  BotSkillLevel_Stupid)
	BotSkillLevel_Newbie       = 40u,	// (BotSkillLevel_Stupid,   BotSkillLevel_Newbie)
	BotSkillLevel_Average      = 60u,	// (BotSkillLevel_Newbie,   BotSkillLevel_Average)
	BotSkillLevel_Advanced     = 80u,	// (BotSkillLevel_Average,  BotSkillLevel_Advanced)
	BotSkillLevel_Professional = 99u,	// (BotSkillLevel_Advanced, BotSkillLevel_Professional)
	BotSkillLevel_Godlike      = BotSkillValue_Maximum
};

enum GlobalConstant_t
{
	MaximumNodes              = 8u,
	MaximumKillHistory        = 16u,
	NumberWeapons             = 26u,	// it's the number of YaPB weapons - used for buying and restriction (g_weaponSelect)
	NumberWeaponsToBuy        = 30u/*24u*/,	// it's the number of weapons - used for buying (g_weaponBuyTable)
	MaximumWaypoints          = 1024u,	// maximum waypoints number on the map
	MaximumLogMessageLength   = 1024u,	// maximum buffer size for printed messages
	MaximumStringBufferLength = 1024u,	// For various character string buffers.
	MaximumDamageValue        = 2040u,
	MaximumGoalValue          = 2040u
};

static const unsigned short InvalidWaypointIndex (static_cast <unsigned short> (-1));

#endif	// ifndef GLOBAL_CONSTANT_INCLUDED