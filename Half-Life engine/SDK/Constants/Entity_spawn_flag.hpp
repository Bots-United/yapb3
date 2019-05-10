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
// Entity_spawn_flag.hpp
//
// Enumeration: EntitySpawnFlag_t
//
// Description: Edict->spawnFlags flags.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_ENTITY_SPAWN_FLAG_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CONSTANTS_ENTITY_SPAWN_FLAG_INCLUDED

DECLARE_TYPED_ENUMERATION (/*unsigned */int, EntitySpawnFlag_t)
{
	EntitySpawnFlag_NoRespawn = BIT (30u),	// !!!set this bit on guns and stuff that should never respawn.

	SF_ITEM_USE_ONLY = 256u,				// ITEM_USE_ONLY = BUTTON_USE_ONLY = DOOR_USE_ONLY!!!

	SF_TRIGGER_PUSH_START_OFF       = 2u,	// spawn flag that makes trigger_push spawn turned OFF

	SF_TRIGGER_HURT_TARGETONCE      = 1u,	// Only fire hurt target once
	SF_TRIGGER_HURT_START_OFF       = 2u,	// spawn flag that makes trigger_hurt spawn turned OFF
	SF_TRIGGER_HURT_NO_CLIENTS      = 8u,	// spawn flag that makes trigger_hurt hurt only no client entities
	SF_TRIGGER_HURT_CLIENTONLYFIRE  = 16u,	// trigger hurt will only fire its target if it is hurting a client
	SF_TRIGGER_HURT_CLIENTONLYTOUCH = 32u,	// only clients may touch this trigger.

	SF_BRUSH_ACCDCC                 = 16u,	// brush should accelerate and decelerate when toggled
	SF_BRUSH_HURT                   = 32u,	// rotating brush that inflicts pain based on rotation speed
	SF_ROTATING_NOT_SOLID           = 64u,	// some special rotating objects are not solid.

	SF_PENDULUM_SWING               = 2u,	// spawn flag that makes a pendulum a rope swing.

	SF_WALL_START_OFF               = 1u,

	SF_CONVEYOR_VISUAL              = 1u,
	SF_CONVEYOR_NOTSOLID            = 2u,

	SF_BUTTON_DONTMOVE     = BIT (0u),	// is this a non-moving button?
	SF_ROTBUTTON_NOTSOLID  = BIT (0u),	// is not solid button? (pev->solid == SOLID_NOT)
	SF_BUTTON_TOGGLE       = BIT (5u),	// button stays pushed until reactivated (toggle buttons fire when they get back to their "home" position)
	SF_BUTTON_SPARK_IF_OFF = BIT (6u),	// button sparks in OFF state
	SF_BUTTON_TOUCH_ONLY   = BIT (8u),	// button only fires as a result of USE key

	SF_GLOBAL_SET          = BIT (0u),	// set global state to initial state on spawn

	SF_MULTI_INIT = 1u,

	// Make this button behave like a door (HACKHACK)
	// This will disable use and make the button solid
	// rotating buttons were made SOLID_NOT by default since their were some
	// collision problems with them...
	SF_MOMENTARY_DOOR = 0x0001u,

	SF_BTARGET_USE = 0x0001u,
	SF_BTARGET_ON = 0x0002u,

	// doors
	SF_DOOR_ROTATE_Y = 0u,
	SF_DOOR_START_OPEN = 1u,
	SF_DOOR_ROTATE_BACKWARDS = 2u,
	SF_DOOR_PASSABLE = 8u,
	SF_DOOR_ONEWAY = 16u,
	SF_DOOR_NO_AUTO_RETURN = 32u,
	SF_DOOR_ROTATE_Z = 64u,
	SF_DOOR_ROTATE_X = 128u,
	SF_DOOR_USE_ONLY = 256u,	// door must be opened by player's use button.
	SF_DOOR_NOMONSTERS = 512u,	// Monster can't open
	SF_DOOR_SILENT = 0x80000000u,

	SF_GIBSHOOTER_REPEATABLE = 1,	// allows a gibshooter to be refired

	SF_FUNNEL_REVERSE = 1u,	// funnel effect repels particles instead of attracting them.

	SF_BUBBLES_STARTOFF = 0x0001u,

	SF_BLOOD_RANDOM = 0x0001u,
	SF_BLOOD_STREAM = 0x0002u,
	SF_BLOOD_PLAYER = 0x0004u,
	SF_BLOOD_DECAL = 0x0008u,

	// pev->scale is amplitude
	// pev->dmg_save is frequency
	// pev->dmg_take is duration
	// pev->dmg is radius
	// radius of 0 means all players
	// NOTE: UTIL_ScreenShake() will only shake players who are on the ground
	SF_SHAKE_EVERYONE = 0x0001u,		// Don't check radius
	// UNDONE: These don't work yet
	SF_SHAKE_DISRUPT = 0x0002u,		// Disrupt controls
	SF_SHAKE_INAIR = 0x0004u,		// Shake players in air

	// pev->dmg_take is duration
	// pev->dmg_save is hold duration
	SF_FADE_IN = 0x0001u,		// Fade in, not out
	SF_FADE_MODULATE = 0x0002u,		// Modulate, don't blend
	SF_FADE_ONLYONE = 0x0004u,

	SF_BEAM_STARTON = 0x0001u,
	SF_BEAM_TOGGLE = 0x0002u,
	SF_BEAM_RANDOM = 0x0004u,
	SF_BEAM_RING = 0x0008u,
	SF_BEAM_SPARKSTART = 0x0010u,
	SF_BEAM_SPARKEND = 0x0020u,
	SF_BEAM_DECALS = 0x0040u,
	SF_BEAM_SHADEIN = 0x0080u,
	SF_BEAM_SHADEOUT = 0x0100u,
	SF_BEAM_TEMPORARY = 0x8000u,

	SF_SPRITE_STARTON = 0x0001u,
	SF_SPRITE_ONCE = 0x0002u,
	SF_SPRITE_TEMPORARY = 0x8000u,

	SF_MESSAGE_ONCE = 0x0001u,		// Fade in, not out
	SF_MESSAGE_ALL = 0x0002u,	// Send to all clients

	SF_ENVEXPLOSION_NODAMAGE = BIT (0u),	// when set, ENV_EXPLOSION will not actually inflict damage
	SF_ENVEXPLOSION_REPEATABLE = BIT (1u),	// can this entity be refired?
	SF_ENVEXPLOSION_NOFIREBALL = BIT (2u),	// don't draw the fireball
	SF_ENVEXPLOSION_NOSMOKE = BIT (3u),	// don't draw the smoke
	SF_ENVEXPLOSION_NODECAL = BIT (4u),	// don't make a scorch mark
	SF_ENVEXPLOSION_NOSPARKS = BIT (5u),	// don't make a scorch mark

	SF_TANK_ACTIVE = 0x0001u,
	SF_TANK_PLAYER = 0x0002u,
	SF_TANK_HUMANS = 0x0004u,
	SF_TANK_ALIENS = 0x0008u,
	SF_TANK_LINEOFSIGHT = 0x0010u,
	SF_TANK_CANCONTROL = 0x0020u,
	SF_TANK_SOUNDON = 0x8000u,

	// Grenades flagged with this will be triggered when the owner calls detonateSatchelCharges
	SF_DETONATE = 0x0001u,

	SF_SUIT_SHORTLOGON = 0x0001u,

	// CGameScore / game_score -- award points to player / team
	//	Points ±total
	//	Flag: Allow negative scores					SF_SCORE_NEGATIVE
	//	Flag: Award points to team in teamplay		SF_SCORE_TEAM
	SF_SCORE_NEGATIVE = 0x0001u,
	SF_SCORE_TEAM = 0x0002u,

	//
	// CGameText / game_text	-- NON-Localized HUD Message (use env_message to display a titles.txt message)
	//	Flag: All players					SF_ENVTEXT_ALLPLAYERS
	//
	SF_ENVTEXT_ALLPLAYERS = 0x0001u,

	//
	// CGameTeamMaster / game_team_master	-- "Masters" like multisource, but based on the team of the activator
	// Only allows mastered entity to fire if the team matches my team
	//
	// team index (pulled from server team list "mp_teamlist"
	// Flag: Remove on Fire
	// Flag: Any team until set?		-- Any team can use this until the team is set (otherwise no teams can use it)
	//
	SF_TEAMMASTER_FIREONCE = 0x0001u,
	SF_TEAMMASTER_ANYTEAM = 0x0002u,

	//
	// CGameTeamSet / game_team_set	-- Changes the team of the entity it targets to the activator's team
	// Flag: Fire once
	// Flag: Clear team				-- Sets the team to "NONE" instead of activator
	SF_TEAMSET_FIREONCE = 0x0001u,
	SF_TEAMSET_CLEARTEAM = 0x0002u,

	//
	// CGamePlayerHurt / game_player_hurt	-- Damages the player who fires it
	// Flag: Fire once
	SF_PKILL_FIREONCE = 0x0001u,

	//
	// CGameCounter / game_counter	-- Counts events and fires target
	// Flag: Fire once
	// Flag: Reset on Fire
	SF_GAMECOUNT_FIREONCE = 0x0001u,
	SF_GAMECOUNT_RESET = 0x0002u,

	//
	// CGameCounterSet / game_counter_set	-- Sets the counter's value
	// Flag: Fire once
	SF_GAMECOUNTSET_FIREONCE = 0x0001u,

	//
	// CGamePlayerEquip / game_playerequip	-- Sets the default player equipment
	// Flag: USE Only
	SF_PLAYEREQUIP_USEONLY = 0x0001u,
	MAX_EQUIP = 32u,

	//
	// CGamePlayerTeam / game_player_team	-- Changes the team of the player who fired it
	// Flag: Fire once
	// Flag: Kill Player
	// Flag: Gib Player
	SF_PTEAM_FIREONCE = 0x0001u,
	SF_PTEAM_KILL = 0x0002u,
	SF_PTEAM_GIB = 0x0004u,

	SF_PLAT_TOGGLE = 0x0001u,

	// ----------------------------------------------------------------------------
	//
	// Track changer / Train elevator
	//
	// ----------------------------------------------------------------------------
	SF_TRACK_ACTIVATETRAIN = 0x00000001u,
	SF_TRACK_RELINK = 0x00000002u,
	SF_TRACK_ROTMOVE = 0x00000004u,
	SF_TRACK_STARTBOTTOM = 0x00000008u,
	SF_TRACK_DONT_MOVE = 0x00000010u,

	// Tracktrain spawn flags
	SF_TRACKTRAIN_NOPITCH = 0x0001u,
	SF_TRACKTRAIN_NOCONTROL = 0x0002u,
	SF_TRACKTRAIN_FORWARDONLY = 0x0004u,
	SF_TRACKTRAIN_PASSABLE = 0x0008u,

	// Spawnflag for CPathTrack
	SF_PATH_DISABLED = 0x00000001u,
	SF_PATH_FIREONCE = 0x00000002u,
	SF_PATH_ALTREVERSE = 0x00000004u,
	SF_PATH_DISABLE_TRAIN = 0x00000008u,
	SF_PATH_ALTERNATE = 0x00008000u,

	// Spawnflags of CPathCorner
	SF_CORNER_WAITFORTRIG = 0x001u,
	SF_CORNER_TELEPORT = 0x002u,
	SF_CORNER_FIREONCE = 0x004u,

	// This trigger will fire when the level spawns (or respawns if not fire once)
	// It will check a global state before firing.  It supports delay and killtargets
	SF_AUTO_FIREONCE = 0x0001u,

	SF_RELAY_FIREONCE = 0x0001u,

	//**********************************************************
	// The Multimanager Entity - when fired, will fire up to 16 targets
	// at specified times.
	// FLAG:		THREAD (create clones when triggered)
	// FLAG:		CLONE (this is a clone for a threaded execution)
	SF_MULTIMAN_CLONE = 0x80000000u,
	SF_MULTIMAN_THREAD = 0x00000001u,

	//
	// Render parameters trigger
	//
	// This entity will copy its render parameters (renderfx, rendermode, rendercolor, renderamt)
	// to its targets when triggered.
	//
	// Flags to indicate masking off various render parameters that are normally copied to the targets
	SF_RENDER_MASKFX = BIT (0u),
	SF_RENDER_MASKAMT = BIT (1u),
	SF_RENDER_MASKMODE = BIT (2u),
	SF_RENDER_MASKCOLOR = BIT (3u),

	SF_CHANGELEVEL_USEONLY = 0x0002u,

	SF_ENDSECTION_USEONLY = 0x0001u,

	SF_CAMERA_PLAYER_POSITION = 1u,
	SF_CAMERA_PLAYER_TARGET = 2u,
	SF_CAMERA_PLAYER_TAKECONTROL = 4u,

	// func_rotating
	SF_BRUSH_ROTATE_Y_AXIS = 0u,
	SF_BRUSH_ROTATE_INSTANT = 1u,
	SF_BRUSH_ROTATE_BACKWARDS = 2u,
	SF_BRUSH_ROTATE_Z_AXIS = 4u,
	SF_BRUSH_ROTATE_X_AXIS = 8u,
	SF_PENDULUM_AUTO_RETURN = 16u,
	SF_PENDULUM_PASSABLE = 32u,

	SF_BRUSH_ROTATE_SMALLRADIUS = 128u,
	SF_BRUSH_ROTATE_MEDIUMRADIUS = 256u,
	SF_BRUSH_ROTATE_LARGERADIUS = 512u,

	// triggers
	SF_TRIGGER_ALLOWMONSTERS = 1u,	// monsters allowed to fire this trigger
	SF_TRIGGER_NOCLIENTS = 2u,	// players not allowed to fire this trigger
	SF_TRIGGER_PUSHABLES = 4u,	// only pushables can fire this trigger

	// func breakable
	SF_BREAK_TRIGGER_ONLY = 1u,	// may only be broken by trigger
	SF_BREAK_TOUCH = 2u,	// can be 'crashed through' by running player (plate glass)
	SF_BREAK_PRESSURE = 4u,	// can be broken by a player standing on it
	SF_BREAK_CROWBAR = 256u,	// instant break if hit with crowbar

	// func_pushable (it's also "func_breakable", so don't collide with those flags)
	SF_TRIG_PUSH_ONCE = 1u,
	SF_PUSH_BREAKABLE = 128u,

	SF_LIGHT_START_OFF = 1u,

	SPAWNFLAG_NOMESSAGE = 1u,
	SPAWNFLAG_NOTOUCH = 1u,
	SPAWNFLAG_DROIDONLY = 4u,

	SPAWNFLAG_USEONLY = 1u,		// can't be touched, must be used (buttons)

	SF_DECAL_NOTINDEATHMATCH = 2048u,

	SF_WORLD_DARK = 0x0001u,		// Fade from black at startup
	SF_WORLD_TITLE = 0x0002u,		// Display game title at startup
	SF_WORLD_FORCETEAM = 0x0004u,		// Force teams

	SF_HAIR_SYNC = 0x0001u
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CONSTANTS_ENTITY_SPAWN_FLAG_INCLUDED