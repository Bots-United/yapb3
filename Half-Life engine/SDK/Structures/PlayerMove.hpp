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
// PlayerMove.hpp
//
// Structure: PlayerMove_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLAYER_MOVE_INCLUDED
#define PLAYER_MOVE_INCLUDED

struct PlayerMove_t
{
	enum UseHullType_t
	{
		UseHullType_Regular,	// 0 = regular player hull (Normal player)
		UseHullType_Ducked,		// 1 = ducked player hull (Crouched player)
		UseHullType_Point,		// 2 = point hull (Point based hull)

		UseHullType_Large,		// Note: for this hull function DLLFunction_t::pfnGetHullBounds() just returns false!

		UseHullType_Total
	};

	// Physics variables.
	struct Variables_t
	{
		float gravity;				// Gravity for map (Console variable "sv_gravity" value)
		float stopspeed;			// Deceleration when not moving (Console variable "sv_stopspeed" value)
		float maxspeed;				// Max allowed speed (Console variable "sv_maxspeed" value)
		float spectatormaxspeed;	// (Console variable "sv_spectatormaxspeed" value)
		float accelerate;			// Acceleration factor (Console variable "sv_accelerate" value)
		float airaccelerate;		// Same for when in open air (Console variable "sv_airaccelerate" value)
		float wateraccelerate;		// Same for when in water (Console variable "sv_wateraccelerate" value)
		float friction;				// (Console variable "sv_friction" value)
		float edgefriction;			// Extra friction near dropofs (Console variable "sv_edgefriction" value)
		float waterfriction;		// Less in water (Console variable "sv_waterfriction" value)
		float entgravity;			// 1.0 (Has no associated console variable)
		float bounce;				// Wall bounce value. 1.0 (Console variable "sv_bounce" value)
		float stepsize;				// sv_stepsize; (Console variable "sv_stepsize" value)
		float maxvelocity;			// maximum server velocity. (Console variable "sv_maxvelocity" value)
		float zmax;					// Max z-buffer range (for GL) (Console variable "sv_zmax" value)
		float waveHeight;			// Water wave height (for GL) (Console variable "sv_wateramp" value)
		BOOL  footsteps;			// Play footstep sounds (Console variable "sv_footsteps" value)
		char  skyName[32u];			// Name of the sky map (Console variable "sv_skyname" value)
		float rollangle;			// (Console variable "sv_rollangle" value)
		float rollspeed;			// (Console variable "sv_rollspeed" value)
		float skycolor_r;			// Sky color (Console variable "sv_skycolor_r" value)
		float skycolor_g;			// (Console variable "sv_skycolor_g" value)
		float skycolor_b;			// (Console variable "sv_skycolor_b" value)
		float skyvec_x;				// Sky vector (Console variable "sv_skyvec_x" value)
		float skyvec_y;				// (Console variable "sv_skyvec_y" value)
		float skyvec_z;				// (Console variable "sv_skyvec_z" value)
	};

	typedef BOOL (*const IgnoreFunction_t) (PhysEntity_t *const entity);

/*! Off=0(0)*/	int                            playerIndex;											// So we don't try to run the PM_CheckStuck() nudging too quickly.
/*! Off=4(1)*/	BOOL                           isServer;											// For debugging, are we running physics code on server side?

/*! Off=8(2)*/	BOOL                           isMultiplayer;										// TRUE == multiplayer server
/*! Off=12(3)*/	float                          time;												// realtime on host, for reckoning duck timing
/*! Off=16(4)*/	float                          frameTime;											// Duration of this frame

	// Vectors for angles
/*! Off=20(5)*/	Math::Vector3D                 forward;
/*! Off=32(8)*/	Math::Vector3D                 right;
/*! Off=44(11)*/	Math::Vector3D                 upward;

	// player state
/*! Off=56(14)*/	Math::Vector3D                 origin;												// Movement origin.
/*! Off=68(17)*/	Math::Angles3D                 angles;												// Movement view angles.
/*! Off=80(20)*/	Math::Angles3D                 oldAngles;											// Angles before movement view angles were looked at.
/*! Off=92(23)*/	Math::Vector3D                 velocity;											// Current movement direction.
/*! Off=104(26)*/	Math::Vector3D                 moveDirection;										// For waterjumping, a forced forward velocity so we can fly over lip of ledge.
/*! Off=116(29)*/	Math::Vector3D                 baseVelocity;										// Velocity of the conveyor we are standing, e.g.

	// For ducking/dead
/*! Off=128(32)*/	Math::Vector3D                 viewOffset;											// Our eye position.
/*! Off=140(35)*/	float                          duckTime;											// Time we started duck
/*! Off=144(36)*/	BOOL                           isInDuck;											// In process of ducking or ducked already?

	// For walking/falling
/*! Off=148(37)*/	int                            timeStepSound;										// Next time we can play a step sound
/*! Off=152(38)*/	int                            stepLeft;

/*! Off=156(39)*/	float                          fallVelocity;										// how fast we are falling
/*! Off=160(40)*/	Math::Angles3D                 punchAngles;

/*! Off=172(43)*/	float                          swimTime;

/*! Off=176(44)*/	float                          nextPrimaryAttack;

/*! Off=180(45)*/	Constants::EntityEffectFlag_t  effects;												// MUZZLE_FLASH, e.g.

/*! Off=184(46)*/	Constants::EntityFlag_t        flags;												// FL_ONGROUND, FL_DUCKING, etc.
/*! Off=188(47)*/	UseHullType_t                  useHull;												// Used in 'playerMins'/'playerMaxs'.
/*! Off=192(48)*/	float                          gravity;												// Our current gravity and friction.
/*! Off=196(49)*/	float                          friction;
/*! Off=200(50)*/	Constants::ButtonType_t        oldButtons;											// Buttons last user command
/*! Off=204(51)*/	float                          waterJumpTime;										// Amount of time left in jumping out of water cycle.
/*! Off=208(52)*/	BOOL                           isDead;												// Are we a dead player?
/*! Off=212(53)*/	Constants::DeadState_t         deadFlag;
/*! Off=216(54)*/	BOOL                           spectator;											// Should we use spectator physics model?
/*! Off=220(55)*/	Constants::EntityMoveType_t    moveType;											// Our movement type, NOCLIP, WALK, FLY

/*! Off=224(56)*/	int                            onGround;
/*! Off=228(57)*/	Constants::EntityWaterLevel_t  waterLevel;
/*! Off=232(58)*/	Constants::PointContentType_t  waterType;
/*! Off=236(59)*/	Constants::EntityWaterLevel_t  oldWaterLevel;

/*! Off=240(60)*/	char                           textureName[256u];
/*! Off=496(124)*/	Constants::TextureType_t       textureType;

/*! Off=500(125)*/	float                          maximumSpeed;
/*! Off=504(126)*/	float                          clientMaximumSpeed;									// Player specific maximum speed

	// For mods....

/*! Off=508(127)*/	int                            iuser1;
/*! Off=512(128)*/	int                            iuser2;
/*! Off=516(129)*/	int                            iuser3;
/*! Off=520(130)*/	int                            iuser4;

/*! Off=524(131)*/	float                          fuser1;
/*! Off=528(132)*/	float                          fuser2;
/*! Off=532(133)*/	float                          fuser3;
/*! Off=536(134)*/	float                          fuser4;

/*! Off=540(135)*/	Math::Vector3D                 vuser1;
/*! Off=552(138)*/	Math::Vector3D                 vuser2;
/*! Off=564(141)*/	Math::Vector3D                 vuser3;
/*! Off=576(144)*/	Math::Vector3D                 vuser4;

	// world state....

/*! Off=588(147)*/	int                            numberPhysEntities;									// Number of entities to clip against.
/*! Off=592(148)*/	PhysEntity_t                   physEntities[Constants::MaximumPhysEntities];

/*! Off=134992(33748)*/	int                            numberMoveEntities;									// Number of momvement entities (ladders)
/*! Off=134996(33749)*/	PhysEntity_t                   moveEntities[Constants::MaximumMoveEntities];		// just a list of ladders

	// All things being rendered, for tracing against things you don't actually collide with
/*! Off=149332(37333)*/	int                            numberVisibilityEntities;
/*! Off=149336(37334)*/	PhysEntity_t                   visibilityEntities[Constants::MaximumPhysEntities];

/*! Off=283736(70934)*/	UserCommand_t                  command;												// input to run through physics.

	// Trace results for objects we collided with.
/*! Off=283788(70947)*/	int                            numberTouch;
/*! Off=283792(70948)*/	PMTraceResult_t                touchIndex[Constants::MaximumPhysEntities];

/*! Off=324592(81148)*/	char                           physInfo[Constants::MaximumPhysInfoStringLength];	// Physics info string

/*! Off=324848(81212)*/	Variables_t                   *moveVariables;
/*! Off=324852(81213)*/	Math::Vector3D                 playerMins[UseHullType_Total];
/*! Off=324900(81225)*/	Math::Vector3D                 playerMaxs[UseHullType_Total];

	// Common functions....
/*! Off=324948(81237)*/	const char                    *(*PM_Info_ValueForKey)     (const char *s, const char *key);
/*! Off=324952(81238)*/	void                           (*PM_Particle)             (const Math::Vector3D &origin, int color, float life, int zPosition, int zVelocity);
/*! Off=324956(81239)*/	int                            (*PM_TestPlayerPosition)   (const Math::Vector3D &position, PMTraceResult_t &traceResult);
/*! Off=324960(81240)*/	void                           (*Con_NPrintf)             (int index, const char *format, ...);
/*! Off=324964(81241)*/	void                           (*Con_DPrintf)             (const char *format, ...);
/*! Off=324968(81242)*/	void                           (*Con_Printf)              (const char *format, ...);
/*! Off=324972(81243)*/	double                         (*Sys_FloatTime)           (void);
/*! Off=324976(81244)*/	void                           (*PM_StuckTouch)           (int hitEntity, const PMTraceResult_t &traceResult);
/*! Off=324980(81245)*/	Constants::PointContentType_t  (*PM_PointContents)        (const Math::Vector3D &point, Constants::PointContentType_t *const trueContents/* filled in if this is non-null */);
/*! Off=324984(81246)*/	Constants::PointContentType_t  (*PM_TruePointContents)    (const Math::Vector3D &point);
/*! Off=324988(81247)*/	Constants::PointContentType_t  (*PM_HullPointContents)    (const Hull_t *hull, int num, const Math::Vector3D &point);
/*! Off=324992(81248)*/	PMTraceResult_t                (*PM_PlayerTrace)          (const Math::Vector3D &start, const Math::Vector3D &end, Constants::PMPlayerTraceFlag_t traceFlags, int entityToSkip);
/*! Off=324996(81249)*/	PMTraceResult_t               *(*PM_TraceLine)            (const Math::Vector3D &start, const Math::Vector3D &end, Constants::PMTraceLineFlag_t flags, UseHullType_t useHull, int entityToSkip);
/*! Off=325000(81250)*/	int                            (*RandomLong)              (int low, int high);
/*! Off=325004(81251)*/	float                          (*RandomFloat)             (float low, float high);
/*! Off=325008(81252)*/	Constants::ModelType_t         (*PM_GetModelType)         (Model_t *model);
/*! Off=325012(81253)*/	void                           (*PM_GetModelBounds)       (Model_t *model, Math::Vector3D &mins, Math::Vector3D &maxs);
/*! Off=325016(81254)*/	Hull_t                        *(*PM_HullForBsp)           (const PhysEntity_t *entity, Math::Vector3D &offset);
/*! Off=325020(81255)*/	const float                    (*PM_TraceModel)           (const PhysEntity_t *const model, const Math::Vector3D &start, const Math::Vector3D &end, ModelTraceResult_t &traceResult);
/*! Off=325024(81256)*/	int                            (*COM_FileSize)            (const char *filename);
/*! Off=325028(81257)*/	unsigned char                 *(*COM_LoadFile)            (const char *path, int useHunk, int *length);
/*! Off=325032(81258)*/	void                           (*COM_FreeFile)            (void *buffer);
/*! Off=325036(81259)*/	char                          *(*memfgets)                (const unsigned char *memFile, int fileSize, int *filePosition, char *buffer, int bufferSize);

	// Functions
/*! Off=325040(81260)*/	BOOL                           runFunctions;										// Run functions for this frame?
/*! Off=325044(81261)*/	void                           (*PM_PlaySound)            (Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, SDK::Constants::SoundFlag_t flags, Constants::SoundPitch_t pitch);
/*! Off=325048(81262)*/	const char                    *(*PM_TraceTexture)         (int ground, const Math::Vector3D &start, const Math::Vector3D &end);
/*! Off=325052(81263)*/	void                           (*PM_PlaybackEventFull)    (event_info_t::Flag_t flags, int clientIndex, unsigned short eventIndex, float delay, const Math::Vector3D &origin, const Math::Angles3D &angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);

/*! Off=325056(81264)*/	PMTraceResult_t                (*PM_PlayerTraceEx)        (const Math::Vector3D &start, const Math::Vector3D &end, Constants::PMPlayerTraceFlag_t traceFlags, IgnoreFunction_t pfnIgnore);
/*! Off=325060(81265)*/	int                            (*PM_TestPlayerPositionEx) (const Math::Vector3D &position, PMTraceResult_t &traceResult, IgnoreFunction_t pfnIgnore);
/*! Off=325064(81266)*/	PMTraceResult_t               *(*PM_TraceLineEx)          (const Math::Vector3D &start, const Math::Vector3D &end, Constants::PMTraceLineFlag_t flags, UseHullType_t useHull, IgnoreFunction_t pfnIgnore);
};	// sizeof (PlayerMove_t) == 325068 (81267)

#endif	// ifndef PLAYER_MOVE_INCLUDED