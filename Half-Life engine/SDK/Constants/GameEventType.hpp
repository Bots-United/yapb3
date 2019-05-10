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
// GameEventType.hpp
//
// Enumeration: GameEventType_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef GAME_EVENT_TYPE_INCLUDED
#define GAME_EVENT_TYPE_INCLUDED

enum GameEventType_t
{
	// Note: 'GameEventType_*' == 'EVENT_*' in CS game mod DLL.
	GameEventType_INVALID,

	GameEventType_WEAPON_FIRED,						// tell bots the player is attack (argumens: 1 = attacker, 2 = NULL)
	GameEventType_WEAPON_FIRED_ON_EMPTY,			// tell bots the player is attack without clip ammo (argumens: 1 = attacker, 2 = NULL)
	GameEventType_WEAPON_RELOADED,					// tell bots the player is reloading his weapon (argumens: 1 = reloader, 2 = NULL)

	GameEventType_HE_GRENADE_EXPLODED,				// tell bots the HE grenade is exploded (argumens: 1 = grenade thrower, 2 = NULL)
	GameEventType_FLASHBANG_GRENADE_EXPLODED,		// tell bots the flashbang grenade is exploded (argumens: 1 = grenade thrower, 2 = explosion origin)
	GameEventType_SMOKE_GRENADE_EXPLODED,			// tell bots the smoke grenade is exploded (argumens: 1 = grenade thrower, 2 = NULL)
	GameEventType_GRENADE_BOUNCED,

	GameEventType_BEING_SHOT_AT,

	GameEventType_PLAYER_BLINDED_BY_FLASHBANG,		// tell bots the player is flashed (argumens: 1 = flashed player, 2 = NULL)
	GameEventType_PLAYER_FOOTSTEP,					// tell bots the player is running (argumens: 1 = runner, 2 = NULL)
	GameEventType_PLAYER_JUMPED,					// tell bots the player is jumped (argumens: 1 = jumper, 2 = NULL)
	GameEventType_PLAYER_DIED,						// tell bots the player is killed (argumens: 1 = victim, 2 = killer)
	GameEventType_PLAYER_LANDED_FROM_HEIGHT,		// tell bots the player is fell with some damage (argumens: 1 = felled player, 2 = NULL)
	GameEventType_PLAYER_TOOK_DAMAGE,				// tell bots the player is take damage (argumens: 1 = victim, 2 = attacker)

	GameEventType_HOSTAGE_DAMAGED,					// tell bots the player has injured a hostage (argumens: 1 = hostage, 2 = injurer)
	GameEventType_HOSTAGE_KILLED,					// tell bots the player has killed a hostage (argumens: 1 = hostage, 2 = killer)

	GameEventType_DOOR,								// tell bots the door is moving (argumens: 1 = door, 2 = NULL)

	GameEventType_BREAK_GLASS,						// tell bots the glass has break (argumens: 1 = glass, 2 = NULL)
	GameEventType_BREAK_WOOD,						// tell bots the wood has break (argumens: 1 = wood, 2 = NULL)
	GameEventType_BREAK_METAL,						// tell bots the metal/computer has break (argumens: 1 = metal/computer, 2 = NULL)
	GameEventType_BREAK_FLESH,						// tell bots the flesh has break (argumens: 1 = flesh, 2 = NULL)
	GameEventType_BREAK_CONCRETE,					// tell bots the concrete has break (argumens: 1 = concrete, 2 = NULL)

	GameEventType_BOMB_PLANTED,						// tell bots the bomb has been planted (argumens: 1 = planter, 2 = NULL)
	GameEventType_BOMB_DROPPED,						// tell bots the bomb has been dropped (argumens: 1 = NULL, 2 = NULL)
	GameEventType_BOMB_PICKED_UP,					// let the bots hear the bomb pickup (argumens: 1 = player that pickup c4, 2 = NULL)
	GameEventType_BOMB_BEEP,						// let the bots hear the bomb beeping (argumens: 1 = c4, 2 = NULL)
	GameEventType_BOMB_DEFUSING,					// tell the bots someone has started defusing (argumens: 1 = defuser, 2 = NULL)
	GameEventType_BOMB_DEFUSE_ABORTED,				// tell the bots someone has aborted defusing (argumens: 1 = NULL, 2 = NULL)
	GameEventType_BOMB_DEFUSED,						// tell the bots the bomb is defused (argumens: 1 = defuser, 2 = NULL)
	GameEventType_BOMB_EXPLODED,					// let the bots hear the bomb exploding (argumens: 1 = NULL, 2 = NULL)

	GameEventType_HOSTAGE_USED,						// tell bots the hostage is used (argumens: 1 = user, 2 = NULL)
	GameEventType_HOSTAGE_RESCUED,					// tell bots the hostage is rescued (argumens: 1 = rescuer (Classes::BasePlayer *) (Note: for CS: CZ may be NULL!), 2 = hostage (Classes::Hostage *))
	GameEventType_ALL_HOSTAGES_RESCUED,				// tell bots the all hostages are rescued (argumens: 1 = NULL, 2 = NULL)

	GameEventType_VIP_ESCAPED,						// tell bots the VIP is escaped (argumens: 1 = NULL, 2 = NULL)
	GameEventType_VIP_ASSASSINATED,					// tell bots the VIP is assassinated (argumens: 1 = NULL, 2 = NULL)

	GameEventType_TERRORISTS_WIN,					// tell bots the terrorists won the round (argumens: 1 = NULL, 2 = NULL)
	GameEventType_CTS_WIN,							// tell bots the CTs won the round (argumens: 1 = NULL, 2 = NULL)

	GameEventType_ROUND_DRAW,						// tell bots the round was a draw (argumens: 1 = NULL, 2 = NULL)
	GameEventType_ROUND_WIN,
	GameEventType_ROUND_LOSS,
	GameEventType_ROUND_START,						// tell bots the round was started (when freeze period is expired) (argumens: 1 = NULL, 2 = NULL)
	GameEventType_PLAYER_SPAWNED,					// tell bots the player is spawned (argumens: 1 = spawned player, 2 = NULL)
	GameEventType_CLIENT_CORPSE_SPAWNED,
	GameEventType_BUY_TIME_START,					// tell bots the round was restarted (argumens: 1 = NULL, 2 = NULL)
	GameEventType_PLAYER_LEFT_BUY_ZONE,
	GameEventType_DEATH_CAMERA_START,				// tell bots the player is sart observer (argumens: 1 = observer player, 2 = NULL)
	GameEventType_KILL_ALL,
	GameEventType_ROUND_TIME,
	GameEventType_DIE,
	GameEventType_KILL,
	GameEventType_HEADSHOT,
	GameEventType_KILL_FLASHBANGED,
	GameEventType_TUTOR_BUY_MENU_OPENNED,			// tell bots the buy menu is opened (argumens: 1 = NULL, 2 = NULL)
	GameEventType_TUTOR_AUTOBUY,
	GameEventType_PLAYER_BOUGHT_SOMETHING,
	GameEventType_TUTOR_NOT_BUYING_ANYTHING,
	GameEventType_TUTOR_NEED_TO_BUY_PRIMARY_WEAPON,
	GameEventType_TUTOR_NEED_TO_BUY_PRIMARY_AMMO,
	GameEventType_TUTOR_NEED_TO_BUY_SECONDARY_AMMO,
	GameEventType_TUTOR_NEED_TO_BUY_ARMOR,
	GameEventType_TUTOR_NEED_TO_BUY_DEFUSE_KIT,
	GameEventType_TUTOR_NEED_TO_BUY_GRENADE,
	GameEventType_CAREER_TASK_DONE,

	// Radio commands.... (argumens: 1 = radio command sender, 2 = NULL)
	GameEventType_RADIO,

		GameEventType_START_RADIO_1 = GameEventType_RADIO,
		GameEventType_RADIO_1,
			GameEventType_RADIO_1_Cover_me = GameEventType_RADIO_1,
			GameEventType_RADIO_1_You_take_the_point,
			GameEventType_RADIO_1_Hold_this_position,
			GameEventType_RADIO_1_Regroup_team,
			GameEventType_RADIO_1_Follow_me,
			GameEventType_RADIO_1_Taking_fire,
		GameEventType_RADIO_1_End,

		GameEventType_START_RADIO_2 = GameEventType_RADIO_1_End,
		GameEventType_RADIO_2,
			GameEventType_RADIO_2_Go_go_go = GameEventType_RADIO_2,
			GameEventType_RADIO_2_Team_fall_back,
			GameEventType_RADIO_2_Stick_together_team,
			GameEventType_RADIO_2_Get_in_position_and_wait,
			GameEventType_RADIO_2_Storm_the_front,
			GameEventType_RADIO_2_Report_in_team,
		GameEventType_RADIO_2_End,

		GameEventType_START_RADIO_3 = GameEventType_RADIO_2_End,
		GameEventType_RADIO_3,
			GameEventType_RADIO_3_Affirmative = GameEventType_RADIO_3,
			GameEventType_RADIO_3_Enemy_spotted,
			GameEventType_RADIO_3_Need_backup,
			GameEventType_RADIO_3_Sector_clear,
			GameEventType_RADIO_3_In_position,
			GameEventType_RADIO_3_Reporting_in,
			GameEventType_RADIO_3_Get_out_of_there,
			GameEventType_RADIO_3_Negative,
			GameEventType_RADIO_3_Enemy_down,
		GameEventType_RADIO_3_End,

	GameEventType_END_RADIO = GameEventType_RADIO_3_End,

	GameEventType_NEW_MATCH,						// tell bots the game is reset (argumens: 1 = NULL, 2 = NULL)
	GameEventType_PLAYER_CHANGED_TEAM,				// tell bots the player is switch his team (also called from ClientPutInServer()) (argumens: 1 = switcher, 2 = NULL)
	GameEventType_BULLET_IMPACT,					// tell bots the player is shoot at wall (argumens: 1 = shooter, 2 = shoot trace end position)
	GameEventType_GAME_COMMENCE,					// tell bots the game is commencing (argumens: 1 = NULL, 2 = NULL)
	GameEventType_WEAPON_ZOOMED,					// tell bots the player is switch weapon zoom (argumens: 1 = zoom switcher, 2 = NULL)
	GameEventType_HOSTAGE_CALLED_FOR_HELP,			// tell bots the hostage is talking (argumens: 1 = listener, 2 = NULL)

	GameEventType_Total
};

enum
{
	GameEventType_RADIO_1_Total = GameEventType_RADIO_1_End - GameEventType_RADIO_1,
	GameEventType_RADIO_2_Total = GameEventType_RADIO_2_End - GameEventType_RADIO_2,
	GameEventType_RADIO_3_Total = GameEventType_RADIO_3_End - GameEventType_RADIO_3,

	GameEventType_RADIO_Total   = GameEventType_END_RADIO - GameEventType_RADIO
};

#endif	// ifndef GAME_EVENT_TYPE_INCLUDED