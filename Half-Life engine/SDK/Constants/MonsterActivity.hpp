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
// MonsterActivity.hpp
//
// Enumeration: MonsterActivity_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MONSTER_ACTIVITY_INCLUDED
#define MONSTER_ACTIVITY_INCLUDED

enum MonsterActivity_t
{
	MonsterActivity_INVALID = -1,		// So we have something more succint to check for than '-1'

	MonsterActivity_RESET,				// Set m_activity to this invalid value to force a reset to m_idealActivity
	MonsterActivity_IDLE,
	MonsterActivity_GUARD,
	MonsterActivity_WALK,
	MonsterActivity_RUN,
	MonsterActivity_FLY,				// Fly (and flap if appropriate)
	MonsterActivity_SWIM,
	MonsterActivity_HOP,				// vertical jump
	MonsterActivity_LEAP,				// long forward jump
	MonsterActivity_FALL,
	MonsterActivity_LAND,
	MonsterActivity_STRAFE_LEFT,
	MonsterActivity_STRAFE_RIGHT,
	MonsterActivity_ROLL_LEFT,			// tuck and roll, left
	MonsterActivity_ROLL_RIGHT,			// tuck and roll, right
	MonsterActivity_TURN_LEFT,			// turn quickly left (stationary)
	MonsterActivity_TURN_RIGHT,			// turn quickly right (stationary)
	MonsterActivity_CROUCH,				// the act of crouching down from a standing position
	MonsterActivity_CROUCHIDLE,			// holding body in crouched position (loops)
	MonsterActivity_STAND,				// the act of standing from a crouched position
	MonsterActivity_USE,
	MonsterActivity_SIGNAL1,
	MonsterActivity_SIGNAL2,
	MonsterActivity_SIGNAL3,
	MonsterActivity_TWITCH,
	MonsterActivity_COWER,
	MonsterActivity_SMALL_FLINCH,
	MonsterActivity_BIG_FLINCH,
	MonsterActivity_RANGE_ATTACK1,
	MonsterActivity_RANGE_ATTACK2,
	MonsterActivity_MELEE_ATTACK1,
	MonsterActivity_MELEE_ATTACK2,
	MonsterActivity_RELOAD,
	MonsterActivity_ARM,				// pull out gun, for instance
	MonsterActivity_DISARM,				// reholster gun
	MonsterActivity_EAT,				// monster chowing on a large food item (loop)
	MonsterActivity_DIESIMPLE,
	MonsterActivity_DIEBACKWARD,
	MonsterActivity_DIEFORWARD,
	MonsterActivity_DIEVIOLENT,
	MonsterActivity_BARNACLE_HIT,		// barnacle tongue hits a monster
	MonsterActivity_BARNACLE_PULL,		// barnacle is lifting the monster (loop)
	MonsterActivity_BARNACLE_CHOMP,		// barnacle latches on to the monster
	MonsterActivity_BARNACLE_CHEW,		// barnacle is holding the monster in its mouth (loop)
	MonsterActivity_SLEEP,
	MonsterActivity_INSPECT_FLOOR,		// for active idles, look at something on or near the floor
	MonsterActivity_INSPECT_WALL,		// for active idles, look at something directly ahead of you (doesn't HAVE to be a wall or on a wall)
	MonsterActivity_IDLE_ANGRY,			// alternate idle animation in which the monster is clearly agitated. (loop)
	MonsterActivity_WALK_HURT,			// limp (loop)
	MonsterActivity_RUN_HURT,			// limp (loop)
	MonsterActivity_HOVER,				// Idle while in flight
	MonsterActivity_GLIDE,				// Fly (don't flap)
	MonsterActivity_FLY_LEFT,			// Turn left in flight
	MonsterActivity_FLY_RIGHT,			// Turn right in flight
	MonsterActivity_DETECT_SCENT,		// this means the monster smells a scent carried by the air
	MonsterActivity_SNIFF,				// this is the act of actually sniffing an item in front of the monster
	MonsterActivity_BITE,				// some large monsters can eat small things in one bite. This plays one time, EAT loops.
	MonsterActivity_THREAT_DISPLAY,		// without attacking, monster demonstrates that it is angry. (Yell, stick out chest, etc)
	MonsterActivity_FEAR_DISPLAY,		// monster just saw something that it is afraid of
	MonsterActivity_EXCITED,			// for some reason, monster is excited. Sees something he really likes to eat, or whatever.
	MonsterActivity_SPECIAL_ATTACK1,	// very monster specific special attacks.
	MonsterActivity_SPECIAL_ATTACK2,
	MonsterActivity_COMBAT_IDLE,		// agitated idle.
	MonsterActivity_WALK_SCARED,
	MonsterActivity_RUN_SCARED,
	MonsterActivity_VICTORY_DANCE,		// killed a player, do a victory dance.
	MonsterActivity_DIE_HEADSHOT,		// die, hit in head.
	MonsterActivity_DIE_CHESTSHOT,		// die, hit in chest
	MonsterActivity_DIE_GUTSHOT,		// die, hit in gut
	MonsterActivity_DIE_BACKSHOT,		// die, hit in back
	MonsterActivity_FLINCH_HEAD,
	MonsterActivity_FLINCH_CHEST,
	MonsterActivity_FLINCH_STOMACH,
	MonsterActivity_FLINCH_LEFTARM,
	MonsterActivity_FLINCH_RIGHTARM,
	MonsterActivity_FLINCH_LEFTLEG,
	MonsterActivity_FLINCH_RIGHTLEG
};

#endif	// ifndef MONSTER_ACTIVITY_INCLUDED