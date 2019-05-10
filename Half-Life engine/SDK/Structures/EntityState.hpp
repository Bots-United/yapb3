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
// EntityState.hpp
//
// Structure: EntityState_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_ENTITY_STATE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_ENTITY_STATE_INCLUDED

// For entityType below
DECLARE_TYPED_ENUMERATION (int, EntityType_t)
{
	ENTITY_NORMAL = BIT (0u),
	ENTITY_BEAM   = BIT (1u)
};

// Entity state is used for the baseline and for delta compression of a packet of entities that is sent to a client.
struct EntityState_t
{
// Fields which are filled in by routines outside of delta compression
/*! Off=0(0)*/	EntityType_t entityType;
	// Index into 'cl_entities' array for this entity.
/*! Off=4(1)*/	int			number;
/*! Off=8(2)*/	float		msg_time;

	// Message number last time the player/entity state was updated.
/*! Off=12(3)*/	int			messagenum;

	// Fields which can be transitted and reconstructed over the network stream
/*! Off=16(4)*/	Math::Vector3D origin;
/*! Off=28(7)*/	Math::Angles3D angles;

/*! Off=40(10)*/	int			modelindex;
/*! Off=44(11)*/	int			sequence;
/*! Off=48(12)*/	float		frame;
/*! Off=52(13)*/	int			colormap;
/*! Off=56(14)*/	short		skin;
/*! Off=58(14.5)*/	short		solid;	// (From 'Constants::EntitySolidType_t' enumeration)
/*! Off=60(15)*/	Constants::EntityEffectFlag_t			effects;
/*! Off=64(16)*/	float		scale;

/*! Off=68(17)*/	unsigned char		eflags;	// (Only EFLAG_SLERP flag from 'Constants::EntitySolidType_t' enumeration)

	// Render information
/*! Off=72(18)*/	Constants::RenderMode_t			renderMode;
/*! Off=76(19)*/	int			renderamt;
/*! Off=80(20)*/	Color <>		rendercolor;
/*! Off=84(21)*/	Constants::RenderFx_t			renderfx;

/*! Off=88(22)*/	Constants::EntityMoveType_t			movetype;
/*! Off=92(23)*/	float		animtime;
/*! Off=96(24)*/	float		framerate;
/*! Off=100(25)*/	int			body;
/*! Off=104(26)*/	unsigned char		controller[4u];
/*! Off=108(27)*/	unsigned char		blending[4u];
/*! Off=112(28)*/	Math::Vector3D		velocity;


/*! Off=124(31)*/	Math::Extent3D		boundingBox;	// Send bbox down to client for use during prediction.

/*! Off=148(37)*/	int			aiment;
	// If owned by a player, the index of that player ( for projectiles ).
/*! Off=152(38)*/	int			owner;

	// Friction, for prediction.
/*! Off=156(39)*/	float		friction;
	// Gravity multiplier
/*! Off=160(40)*/	float		gravity;

// PLAYER SPECIFIC
/*! Off=164(41)*/	int			team;
/*! Off=168(42)*/	int			playerclass;
/*! Off=172(43)*/	int			health;
/*! Off=176(44)*/	BOOL	spectator;
/*! Off=180(45)*/	int         weaponmodel;
/*! Off=184(46)*/	Constants::GaitSequence_t			gaitsequence;
	// If standing on conveyor, e.g.
/*! Off=188(47)*/	Math::Vector3D		basevelocity;
	// Use the crouched hull, or the regular player hull.
/*! Off=200(50)*/	BOOL			usehull;
	// Latched buttons last time state updated.
/*! Off=204(51)*/	Constants::ButtonType_t			oldbuttons;
	// -1 = in air, else pmove entity number
/*! Off=208(52)*/	int			onground;
/*! Off=212(53)*/	int			iStepLeft;
	// How fast we are falling
/*! Off=216(54)*/	float		flFallVelocity;

/*! Off=220(55)*/	float		fov;
/*! Off=224(56)*/	int			weaponanim;

	// Parametric movement overrides
/*! Off=228(57)*/	Math::Vector3D				startpos;
/*! Off=240(60)*/	Math::Vector3D				endpos;
/*! Off=252(63)*/	float				impacttime;
/*! Off=256(64)*/	float				starttime;

	// For mods
/*! Off=260(65)*/	int			iuser1;
/*! Off=264(66)*/	int			iuser2;
/*! Off=268(67)*/	int			iuser3;
/*! Off=272(68)*/	int			iuser4;
/*! Off=276(69)*/	float		fuser1;
/*! Off=280(70)*/	float		fuser2;
/*! Off=284(71)*/	float		fuser3;
/*! Off=288(72)*/	float		fuser4;
/*! Off=292(73)*/	Math::Vector3D		vuser1;
/*! Off=304(76)*/	Math::Vector3D		vuser2;
/*! Off=316(79)*/	Math::Vector3D		vuser3;
/*! Off=328(82)*/	Math::Vector3D		vuser4;
};	// sizeof (EntityState_t) == 340 (85)

struct ClientData_t
{
	Math::Vector3D				origin;
	Math::Vector3D				velocity;

	int					viewmodel;
	Math::Vector3D				punchangle;
	int					flags;
	int					waterlevel;
	int					watertype;
	Math::Vector3D				view_ofs;
	float				health;

	int					bInDuck;

	int					weapons; // remove?
	
	int					flTimeStepSound;
	int					flDuckTime;
	int					flSwimTime;
	int					waterjumptime;

	float				maxspeed;

	float				fov;
	int					weaponanim;

	int					m_iId;
	int					ammo_shells;
	int					ammo_nails;
	int					ammo_cells;
	int					ammo_rockets;
	float				m_flNextAttack;
	
	int					tfstate;

	int					pushmsec;

	int					deadflag;

	char				physinfo[Constants::MaximumPhysInfoStringLength];

	// For mods
	int					iuser1;
	int					iuser2;
	int					iuser3;
	int					iuser4;
	float				fuser1;
	float				fuser2;
	float				fuser3;
	float				fuser4;
	Math::Vector3D				vuser1;
	Math::Vector3D				vuser2;
	Math::Vector3D				vuser3;
	Math::Vector3D				vuser4;
};	// sizeof (ClientData_t) == 476 (119)

// Info about weapons player might have in his/her possession
struct WeaponData_t
{
	int			m_iId;
	int			m_iClip;

	float		m_flNextPrimaryAttack;
	float		m_flNextSecondaryAttack;
	float		m_flTimeWeaponIdle;

	int			m_fInReload;
	int			m_fInSpecialReload;
	float		m_flNextReload;
	float		m_flPumpTime;
	float		m_fReloadTime;

	float		m_fAimedDamage;
	float		m_fNextAimBonus;
	int			m_fInZoom;
	int			m_iWeaponState;

	int			iuser1;
	int			iuser2;
	int			iuser3;
	int			iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
};	// sizeof (WeaponData_t) == 88 (22)

struct local_state_t
{
	EntityState_t playerstate;
	ClientData_t  client;
	WeaponData_t  weapondata[Constants::MaximumWeapons];
};	// sizeof (local_state_t) == 3632 (908)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_ENTITY_STATE_INCLUDED