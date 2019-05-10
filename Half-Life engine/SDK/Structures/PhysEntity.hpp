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
// PhysEntity.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PHYS_ENTITY_INCLUDED
#define PHYS_ENTITY_INCLUDED

struct PhysEntity_t
{
/*! Off=0(0)*/	char            name[32u];		// Name of model, or "player" or "world".
/*! Off=32(8)*/	int             player;
/*! Off=36(9)*/	Math::Vector3D  origin;			// Model's origin in world coordinates.
/*! Off=48(12)*/	Model_t        *model;			// only for bsp models
/*! Off=52(13)*/	Model_t        *studioModel;	// SOLID_BBOX, but studio clip intersections.

/*! Off=56(14)*/	Math::Extent3D  boundingBox;	// only for non-bsp models

/*! Off=80(20)*/	int             info;			// For client or server to use to identify (index into sv.edicts or cl_entities)
/*! Off=84(21)*/	Math::Angles3D  angles;			// rotated entities need this info for hull testing to work.

/*! Off=96(24)*/	int             solid;			// Triggers and "func_door" type WATER brushes are SOLID_NOT
/*! Off=100(25)*/	int             skin;			// BSP Contents for such things like "func_door" water brushes.
/*! Off=104(26)*/	int             renderMode;		// So we can ignore glass

	// Complex collision detection.
/*! Off=108(27)*/	float           frame;
/*! Off=112(28)*/	int             sequence;
/*! Off=116(29)*/	unsigned char   controller[4u];
/*! Off=120(30)*/	unsigned char   blending[2u];

/*! Off=124(31)*/	int             moveType;
/*! Off=128(32)*/	int             takeDamage;
/*! Off=132(33)*/	int             bloodDecal;
/*! Off=136(34)*/	int             team;
/*! Off=140(35)*/	int             classNumber;

	// For mods....

/*! Off=144(36)*/	int             iuser1;
/*! Off=148(37)*/	int             iuser2;
/*! Off=152(38)*/	int             iuser3;
/*! Off=156(39)*/	int             iuser4;

/*! Off=160(40)*/	float           fuser1;
/*! Off=164(41)*/	float           fuser2;
/*! Off=168(42)*/	float           fuser3;
/*! Off=172(43)*/	float           fuser4;

/*! Off=176(44)*/	Math::Vector3D  vuser1;
/*! Off=188(47)*/	Math::Vector3D  vuser2;
/*! Off=200(50)*/	Math::Vector3D  vuser3;
/*! Off=212(53)*/	Math::Vector3D  vuser4;
};	// sizeof (PhysEntity_t) == 224 (56)

#endif	// ifndef PHYS_ENTITY_INCLUDED