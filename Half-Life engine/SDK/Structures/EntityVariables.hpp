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
// EntityVariables.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_VARIABLES_INCLUDED
#define ENTITY_VARIABLES_INCLUDED

struct EntityVariables_t
{
/*! off=0(0)/128(32) */	TypeDefinitions::StringOffset_t  className;			// Type of entity (Value of this controls what code handles the entity's think at nextThinkTime).
/*! off=4(1)/132(33) */	TypeDefinitions::StringOffset_t  globalName;		// This is the name of the global variable (set by an "env_global" entity) that can be used to control the state of the entity.

/*! off=8(2)/136(34) */	Math::Vector3D                   origin;			// Model's origin in world coordinates. (Location of the entity in the map)
	Math::Vector3D                   oldOrigin;			// Doesn't seem to be used anymore.
	Math::Vector3D                   velocity;			// current movement direction (Vector speed of entity)
	Math::Vector3D                   baseVelocity;		// if standing on conveyor, e.g. (Velocity of the conveyor we are standing)
	Math::Vector3D                   clBaseVelocity;	// base velocity that was passed in to server physics so client can predict conveyors correctly. Server zeroes it, so we need to store here, too.
	Math::Vector3D                   moveDirection;		// for water jumping, a forced forward velocity so we can fly over lip of ledge. (For waterjumping, a forced forward velocity so we can fly over lip of ledge)

	Math::Angles3D                   modelAngles;		// model angles (Angles for model rendering)
	Math::Angles3D                   angleVelocity;		// angle velocity (degrees per second) (Rotating Velocity)
	Math::Angles3D                   punchAngles;		// auto-decaying view angle adjustment (Offset of 'viewAngles' usually due to recoil or being shot. 'punchAngles' decreases to 0.0 gradually/automatically)
	Math::Angles3D                   viewAngles;		// viewing angles (player only) (Angles for player view rendering. Same numbers as angles.)

	// for parametric entities
	Math::Vector3D                   endPosition;
	Math::Vector3D                   startPosition;
	float                            impactTime;
	float                            startTime;

	Constants::FixAngleType_t        fixAngle;			// View angle update type.
	float                            idealPitch;
	float                            pitchSpeed;
	float                            idealYaw;
	float                            yawSpeed;

	int                              modelIndex;		// Model index returned by pfnPrecacheModel().
	TypeDefinitions::StringOffset_t  model;				// The model of the entity.

	TypeDefinitions::StringOffset_t  viewModelName;		// player's view model (The weapon model displayed to the character in first person (v_ models))
	TypeDefinitions::StringOffset_t  weaponModelName;	// what other players see (The weapon model other players see as the weapon being carried by the player (p_ models))

	Math::Extent3D                   absoluteBoundingBox;	// bounding box min/max translated to world coord (These 2 variables are used by the engine for calculations and are set automatically for all entities)

	// send bounding box down to client for use during prediction
	Math::Extent3D                   boundingBox;		// local bounding box mins/maxs (Negative/Positive distance away from the origin on each axis the entity is touchable / clippable)

	Math::Vector3D                   size;				// boundingBox.maxs - boundingBox.mins

	float                            localTime;			// local time
	float                            nextThinkTime;		// Time next call Classes::BaseEntity::Think() function. (The time (in relevance to Engine::GetTime()) of the next time this entity's think will be called)

	Constants::EntityMoveType_t      moveType;			// entity move type
	Constants::EntitySolidType_t     solid;				// entity solid type

	int                              skin;				// Selects the skin the model displays. Most models only have 1 skin (skin 0).
	int                              body;				// sub-model selection for studiomodels (Part of a model. 1 shows backpack in)
	Constants::EntityEffectFlag_t    effects;			// Bitsum of preset draw effects for an entity.

	float                            gravity;			// % of "normal" gravity multiplier (0.0 = No gravity, 1.0 = Normal Gravity. Values are not limited to 0.0 through 1.0)
	float                            friction;			// inverse elasticity of 'MOVETYPE_BOUNCE' (0.0 = No Friction (0 Acceleration), 1.0 = Normal Friction, >1.0 = Faster Acceleration + Faster Halt. Negative value = Constant Acceleration in backwards direction)

	int                              lightLevel;		// equal pfnGetEntityIllum() (also has fake client illumination bug) (Note: this only for "player" entities!) (0..255) (Amount of light shining on the entity (Used for model rendering) (0=No light 255=Fully Light))

	// model information
	int                              sequence;			// animation sequence index (Primary sequence the entity runs. If player or monster this is the sequence of the lower body parts (IE: run / sequence 4))
	Constants::GaitSequence_t        gaitSequence;		// movement animation sequence for player (0 for none) (Secondary sequence for monsters and players that shows upper body animation (IE: ref_aim_mp5 / sequence 36))
	float                            frame;				// % playback position in animation sequences (0..255) (The frame of the current sequence of the current model (Starts at frame 0.0))
	float                            animationTime;		// world time when frame was set (The time to start the animation of the sequence (if sequence is running this is set to Engine::GetTime()))
	float                            frameRate;			// animation playback rate (-8x to 8x) (The frames per second (FPS) the current sequence is playing at)
	unsigned char                    controller[4u];	// bone controller setting (0..255)
	unsigned char                    blending[2u];		// animation blending amount between sub-sequences (0..255)

	// sprite information
	float                            scale;				// sprite rendering scale (0..255) (Doesn't affect models. Used for such things as "env_sprites" (stores sprite scale) or "env_shooter" (stores gib size))

	// render information
	Constants::RenderMode_t          renderMode;		// entity render mode values (The type of rendering for the model / how the model is rendered)
	float                            renderAmount;		// entity transparency amount (The intensity of the render effects (0 = no effect 255 = full effect). Don't think the value of this matters when the render mode is Constants::kRenderNormal (0))
	Math::Vector3D                   renderColor;		// Color to use with 'renderMode'
	Constants::RenderFx_t            renderFx;			// entity render FX values (The effects added to the model rendering)

	float                            health;			// Health of the entity. When it reaches 0 it is destroyed.
	float                            frags;				// # of frags. Scoreboard changes when the "ScoreInfo" message is called.
	Constants::WeaponBit_t           weapons;			// bit mask for available weapons (Bitsum of all currently carried weapons for a player)
	Constants::TakeDamageType_t      takeDamageType;	// entity take damage values (Damage multiplyer)

	Constants::DeadState_t           deadValue;			// entity dead value
	Math::Vector3D                   viewOffset;		// eye position (Permanent version of 'punchAngles')

	Constants::ButtonType_t          buttons;			// Current pressed buttons. (Bit flag for what buttons are currently being held by the player)
	Constants::PlayerImpulseID_t     impulse;			// Impulse command issued. (When set it activates impulse commands during think (Resets to 0.0 after impulse occurs))

	Classes::Edict                  *chain;				// entity pointer when linked into a linked list
	Classes::Edict                  *damageInflictor;	// The entity that dealt damage to this entity. (If a gun deals the damage the gun's 'owner' is the entity who dealt the damage).
	Classes::Edict                  *enemy;
	Classes::Edict                  *aimEntity;			// entity pointer when 'MOVETYPE_FOLLOW' (Contains the entity's edict that this entity is following. This is only in effect if this entity's move type is 'MOVETYPE_FOLLOW')
	Classes::Edict                  *owner;				// if owned by a player, the pointer to that player (Parent of this entity. For weapons this entity is the player)
	Classes::Edict                  *groundEntity;		// if standing on a entity, the pointer to that entity (The edict of the entity we're standing on)

	Constants::EntitySpawnFlag_t     spawnFlags;		// spawn flags (Bit flag of entity's spawn flags)
	Constants::EntityFlag_t          flags;				// flags (Bit flag of entity flags)

	int                              colorMap;			// lowbyte topcolor, highbyte bottomcolor
	int                              team;				// Team of the entity (Use mod specific team constants). I think it can only be extracted as an int which may be why it is displaying 0.0 on my reading even though I was on the T team.

	float                            maximumHealth;		// Always set to 100 but doesn't effect entities. I think it is just for reference. Players always respawn at 100.0 but I don't know if non-player entities spawn with this much HP when pfnSpawn() is called. Will have to test.
	float                            teleportTime;		// Counts back from 2000.0 whenever I get out of the water (when the game moves your player upwards to represent climbing out).
	float                            armorType;			// Type of armor the entity has. (from 'Constants::PlayerArmorType_t' enumeration)
	float                            armorValue;		// Amount of armor the entity has.
	Constants::EntityWaterLevel_t    waterLevel;		// Water level for entity.
	Constants::PointContentType_t    waterType;

	TypeDefinitions::StringOffset_t  target;			// Entity that this entity is handling.
	TypeDefinitions::StringOffset_t  targetName;		// The name given to this entity that another entity searches for to handle it.
	TypeDefinitions::StringOffset_t  netName;			// Player or NPC name.
	TypeDefinitions::StringOffset_t  message;

	float                            damageTake;		// Amount of damage entity was hit for.
	float                            damageSave;
	float                            damage;			// Damage type that this entity got hit by (from 'Constants::DamageType_t' enumeration).
	float                            damageTime;		// On grenades, the time (in relevance to Engine::GetTime()) at which the explosion should take place.

	TypeDefinitions::StringOffset_t  noise;				// Noise variables do different things for different entities.
	TypeDefinitions::StringOffset_t  noise1;			// This is the move sound for doors.
	TypeDefinitions::StringOffset_t  noise2;			// This is the stop sound for doors.
	TypeDefinitions::StringOffset_t  noise3;			// This is for blocking "game_player_equip" and "player_weaponstrip"

	float                            speed;				// Not for players. Effects change from entity to entity (IE: used for door swing speed).
	float                            airFinished;		// Number is set according to the previous equation every think unless a player is submerged under water.
	float                            painFinished;		// Guess would be that this is set to a game time when you are set on fire or something of the sort.
	float                            radsuitFinished;	// Guess would be that this is set to a game time for how long the radiation suit lasts for maps like rock in TFC.

	Classes::Edict                  *containingEntity;	// pointer to the 'Classes::Edict' structure from it's entity variables

	int                              playerClass;
	float                            maximumSpeed;		// Think uses this to determine the maximum speed a player or monster can move at when fully accelerated.

	float                            fov;				// Determines how many degrees of view fit on the screen. (Only represents screen width, i.e. x, yaw!)
	int                              weaponAnimation;	// Sequence of the v_ model.

	int                              pushMsec;

	BOOL                             isInDuck;			// in process of ducking or ducked already?
	int                              timeStepSound;		// next time we can play a step sound (Starts at 400.0 and decreases to 0.0 then resets to 400.0 and repeats constantly)
	int                              swimTime;
	int                              duckTime;			// time we started duck (When crouch button is pressed this variable is set to 1000.0 and decreaments to 0.0. If crouch button is released variable is instantly reset to 0.0)
	int                              stepLeft;			// Seems to control which footstep sound is played. Toggles between 1.0 and 0.0 every time a footstep sound occurs for the player.
	float                            fallVelocity;		// how fast we are falling (Opposite of Z 'velocity' (Falling Speed))

	int                              gameState;			// 1.0 = No Shield, 0.0 = Shield

	Constants::ButtonType_t          oldButtons;		// latched buttons last time state updated (Bit flag of the buttons that the player was pressing during the last think)

	unsigned int                     groupInfo;

	// for mods...
	int                              iuser1;			// For "player" entity this is observer mode (from Constants::SpectatorObserverMode_t enumeration)
/*! off=584(146)/712(178) */	int                              iuser2;			// For "player" spectator entity this is first spectator target (in 'Free Look' mode always == 0)
/*! off=588(147)/716(179) */	int                              iuser3;			// For "player" spectator entity this is second spectator target
/*! off=592(148)/720(180) */	int                              iuser4;			// For "player" spectator entity this is fire prevention for spectator

/*! off=596(149)/724(181) */	float                            fuser1;
/*! off=600(150)/728(182) */	float                            fuser2;			// For "player" entity this is factor(multiplier), slowing the move speed. (When jump button is pressed this variable is set to 1320.0 and decreaments to 0.0. With default friction when this variable reads 0.0 the player should have the ability to accelerate like normal after landing from a jump. This variable does not affect this occurrence but monitors it)
/*! off=604(151)/732(183) */	float                            fuser3;
/*! off=608(152)/736(184) */	float                            fuser4;

/*! off=612(153)/740(185) */	Math::Vector3D                   vuser1;
/*! off=624(156)/752(188) */	Math::Vector3D                   vuser2;
/*! off=636(159)/764(191) */	Math::Vector3D                   vuser3;
/*! off=648(162)/776(194) */	Math::Vector3D                   vuser4;

/*! off=660(165)/788(197) */	Classes::Edict                  *euser1;
/*! off=664(166)/792(198) */	Classes::Edict                  *euser2;
/*! off=668(167)/796(199) */	Classes::Edict                  *euser3;
/*! off=672(168)/800(200) */	Classes::Edict                  *euser4;
};	// sizeof (EntityVariables_t) == 676 (169)

#endif	// ifndef ENTITY_VARIABLES_INCLUDED