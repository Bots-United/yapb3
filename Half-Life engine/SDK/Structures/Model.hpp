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
// Model.hpp
//
// Structure: Model_t
//
//  06/25/2002 MAH
//  This header file has been modified to now include the proper BSP model
//  structure definitions for each of the two Quakeworld client renderers:
//  software mode and GL mode. Originally, Valve only supplied it with
//  the software mode definitions, which caused General Protection Fault's
//  when accessing members of the structures that are different between
//  the two versions.  These are: 'mnode_t', 'mleaf_t', 'msurface_t' and
//  'texture_t'. To select the GL hardware rendering versions of these
//  structures, define 'HARDWARE_MODE' as a preprocessor symbol, otherwise
//  it will default to software mode as supplied.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_MODEL_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_MODEL_INCLUDED

struct mleaf_t;
struct efrag_t;
struct Model_t;
struct mnode_t;
struct msurface_t;

struct PositionHistory_t
{
	// Time stamp for this movement
/*! Off=0(0)/1032(258)*/	float					animtime;

/*! Off=4(1)/1036(259)*/	Math::Vector3D					origin;
/*! Off=16(4)/1048(262)*/	Math::Vector3D					angles;
};	// sizeof (PositionHistory_t) == 28 (7)

struct Mouth_t
{
	unsigned char					mouthopen;		// 0 = mouth closed, 255 = mouth agape
	unsigned char					sndcount;		// counter for running average
	int						sndavg;			// running average
};

struct LatchedVariables_t
{
/*! Off=0(0)/2832(708)*/	float					prevanimtime;
/*! Off=4(1)/2836(709)*/	float					sequencetime;
/*! Off=8(2)/2840(710)*/	unsigned char					prevseqblending[2];
/*! Off=12(3)/2844(711)*/	Math::Vector3D					prevorigin;
/*! Off=24(6)/2856(714)*/	Math::Vector3D					prevangles;

/*! Off=36(9)/2868(717)*/	int						prevsequence;
/*! Off=40(10)/2872(718)*/	float					prevframe;

/*! Off=44(11)/2876(719)*/	unsigned char					prevcontroller[4];
/*! Off=48(12)/2880(720)*/	unsigned char					prevblending[2];
};	// sizeof (LatchedVariables_t) == 52 (13)

struct ClientEntity_t
{
#define HISTORY_MAX		64u  // Must be power of 2
#define HISTORY_MASK	(HISTORY_MAX - 1u)	/// 0x3F (HISTORY_MAX=0x40)

/*! Off=0(0)*/	int						index;	// Index into cl_entities (should match actual slot, but not necessarily)

/*! Off=4(1)*/	BOOL				player;	// True if this entity is a "player"

/*! Off=8(2)*/	EntityState_t			baseline;	// The original state from which to delta during an uncompressed message
/*! Off=348(87)*/	EntityState_t			prevstate;	// The state information from the penultimate message received from the server
/*! Off=688(172)*/	EntityState_t			curstate;	// The state information from the last message received from server

/*! Off=1028(257)*/	int						current_position;	// Last received history update index
/*! Off=1032(258)*/	PositionHistory_t		ph[HISTORY_MAX];	// History of position and angle updates for this player

/*! Off=2824(706)*/	Mouth_t					mouth;		// For synchronizing mouth movements.

/*! Off=2832(708)*/	LatchedVariables_t			latched;	// Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
/*! Off=2884(721)*/	float					lastmove;

	// Actual render position and angles
/*! Off=2888(722)*/	Math::Vector3D					origin;
/*! Off=2900(725)*/	Math::Vector3D					angles;

	// Attachment points
/*! Off=2912(728)*/	Math::Vector3D					attachment[4];

	// Other entity local information
/*! Off=2960(740)*/	BOOL						trivial_accept;

/*! Off=2964(741)*/	Model_t			*model;			// cl.model_precache[curstate.modelindex]; all visible entities have a model
/*! Off=2968(742)*/	efrag_t			*efrag;			// linked list of efrags
/*! Off=2972(743)*/	mnode_t			*topnode;		// for bmodels, first world node that splits bmodel, or NULL if not split

/*! Off=2976(744)*/	float					syncbase;		// for client-side animations -- used by obsolete alias animation system, remove?
/*! Off=2980(745)*/	int						visframe;		// last frame this entity was found in an active leaf
/*! Off=2984(746)*/	ColorWithAlpha <unsigned int>				cvFloorColor;
};	// sizeof (ClientEntity_t) == 3000 (750)

struct efrag_t
{
	mleaf_t		*leaf;
	efrag_t		*leafnext;
	ClientEntity_t	*entity;
	efrag_t		*entnext;
};

// JAY: Compress this as much as possible
struct decal_t
{
	decal_t		*pnext;			// linked list for each surface
	msurface_t	*psurface;		// Surface id for persistence / unlinking
	short		dx;				// Offsets into surface texture (in texture coordinates, so we don't need floats)
	short		dy;
	short		texture;		// Decal texture
	unsigned char		scale;			// Pixel scale
	unsigned char		flags;			// Decal flags

	short		entityIndex;	// Entity this is attached to
};

// 06/23/2002 MAH
// This structure is different between hardware and software mode.
//
// software mode - QW 'model.h'
struct texture_t
{
/*! Off=0(0)*/	char		name[16];
/*! Off=16(4)*/	unsigned int	width, height;
/*! Off=24(6)*/	int			anim_total;				// total tenths in sequence (0 = no)
/*! Off=28(7)*/	int			anim_min, anim_max;		// time for this frame min <=time< max
/*! Off=36(9)*/	texture_t *anim_next;		// in the animation sequence
/*! Off=40(10)*/	texture_t *alternate_anims;	// bmodels in frame 1 use these
/*! Off=44(11)*/	unsigned int	offsets[MIPLEVELS];		// four mip maps stored
/*! Off=60(15)*/	unsigned int	paloffset;
};	// sizeof (texture_t) == 64 (16) - SHOULD BE 66 (16.5) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  hardware mode - QW 'gl_model.h'
struct GL_texture_t
{
/*! Off=0(0)*/	char		name[16];
/*! Off=16(4)*/	unsigned int	width, height;
/*! Off=24(6)*/	int			gl_texturenum;
/*! Off=28(7)*/	msurface_t	*texturechain;	// for gl_texsort drawing
/*! Off=32(8)*/	int			anim_total;				// total tenths in sequence (0 = no)
/*! Off=36(9)*/	int			anim_min, anim_max;		// time for this frame min <=time< max
/*! Off=44(11)*/	texture_t *anim_next;		// in the animation sequence
/*! Off=48(12)*/	texture_t *alternate_anims;	// bmodels in frame 1 use these
/*! Off=52(13)*/	unsigned int	offsets[MIPLEVELS];		// four mip maps stored
/*! Off=68(17)*/	unsigned int	paloffset;
};	// sizeof (GL_texture_t) == 72 (18)

// 06/23/2002 MAH
// This structure is the same in QW source files 'model.h' and 'gl_model.h'.
struct mtexinfo_t
{
/*! Off=0(0)*/	float		vecs[2][4];		// [s/t] unit vectors in world space.
								// [i][3] is the s/t offset relative to the origin.
								// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
/*! Off=32(8)*/	float		mipadjust;		// ?? mipmap limits for very small surfaces
/*! Off=36(9)*/	texture_t	*texture;
/*! Off=40(10)*/	int			flags;			// sky or slime, no lightmap or 256 subdivision
};	// sizeof (texture_t) == 44 (11)

struct surfcache_t
{
	surfcache_t	*next;
	surfcache_t 	**owner;		// NULL is an empty chunk of memory
	int					lightadj[MAXLIGHTMAPS]; // checked for strobe flush
	int					dlight;
	int					size;		// including header
	unsigned int			width;
	unsigned int			height;		// DEBUG only needed for debug
	float				mipscale;
	texture_t	*texture;	// checked for animating textures
	unsigned char				data[4];	// width*height elements
};

#define	SURF_PLANEBACK		2
#define	SURF_DRAWSKY		4
#define SURF_DRAWSPRITE		8
#define SURF_DRAWTURB		0x10
#define SURF_DRAWTILED		0x20
#define SURF_DRAWBACKGROUND	0x40

// software renderer - QW 'model.h'.
struct msurface_t
{
/*off=0(0)*/	int			visframe;		// should be drawn when node is crossed

/*off=4(1)*/	int			dlightframe;	// last frame the surface was checked by an animated light
/*off=8(2)*/	int			dlightbits;		// dynamically generated. Indicates if the surface illumination is modified by an animated light.

/*off=12(3)*/	MPlane_t	*plane;			// pointer to shared plane
/*off=16(4)*/	int			flags;			// see SURF_* #defines

/*off=20(5)*/	int			firstedge;	// look up in model->surfedges[], negative numbers are backwards edges
/*off=24(6)*/	int			numedges;

// surface generation data
// 06/23/2002 MAH
// WARNING: this structure is not defined anywhere in the HL SDK 2.2 headers. See Quakeworld client software mode source for this.
/*off=28(7)*/	surfcache_t	*cachespots[MIPLEVELS];

/*off=44(11)*/	short		texturemins[2]; // smallest s/t position on the surface.
/*off=48(12)*/	short		extents[2];		// ?? s/t texture size, 1..256 for all non-sky surfaces

/*off=52(13)*/	mtexinfo_t	*texinfo;

// lighting info
/*off=56(14)*/	unsigned char		styles[MAXLIGHTMAPS]; // index into d_lightstylevalue[] for animated lights
									  // no one surface can be effected by more than 4
									  // animated lights.
/*off=60(15)*/	Color <>		*samples;	// [numstyles*surfsize]

/*off=64(16)*/	decal_t		*pdecals;
};	// sizeof (msurface_t) == 68 (17)
// 06/23/2002 MAH
// This structure is only need for hardware rendering.
struct glpoly_t
{
#define	VERTEXSIZE	7

	glpoly_t	*next;
	glpoly_t	*chain;
	int		numverts;
	int		flags;			// for SURF_UNDERWATER
	// 07/24/2003 R9 - not actually four! AHA! AHAHAHAHAHAHA! hack.
	float	verts[4][VERTEXSIZE];	// variable sized (xyz s1t1 s2t2)
};
//  hardware renderer - QW 'gl_model.h'
//  06/23/2002 2230 MAH
//  WARNING - the above indicates this structure was modified
//      for Half-Life this structure needs VERIFICATION
//  06/23/2002 2300 MAH - the below version for hardware agrees
//      with a hexadecimal data dump of these structures taken
//      from a running game.
struct GL_msurface_t
{
#define SURF_UNDERWATER		0x80	// ONLY FOR OpenGL!!!
//#define SURF_DONTWARP		0x100	// ONLY FOR OpenGL!!! (EXISTS?!?!?!?!?!??!?!?!?!?!?!??!)

/*off=0(0)*/	int			visframe;		// should be drawn when node is crossed

/*off=4(1)*/	MPlane_t	*plane;			// pointer to shared plane
/*off=8(2)*/	int			flags;			// see SURF_* #defines

/*off=12(3)*/	int			firstedge;	// look up in model->surfedges[], negative numbers are backwards edges
/*off=16(4)*/	int			numedges;

/*off=20(5)*/	short		texturemins[2]; // smallest s/t position on the surface.
/*off=24(6)*/	short		extents[2];		// ?? s/t texture size, 1..256 for all non-sky surfaces

/*off=28(7)*/	int			light_s, light_t;	// gl lightmap coordinates

/*off=36(9)*/	glpoly_t	*polys;				// multiple if warped
/*off=40(10)*/	msurface_t	*texturechain;

/*off=44(11)*/	mtexinfo_t	*texinfo;

// lighting info
/*off=48(12)*/	int			dlightframe;	// last frame the surface was checked by an animated light
/*off=52(13)*/	int			dlightbits;		// dynamically generated. Indicates if the surface illumination
								// is modified by an animated light.
/*off=56(14)*/	int			lightmaptexturenum;
/*off=60(15)*/	unsigned char		styles[MAXLIGHTMAPS]; // index into d_lightstylevalue[] for animated lights
									  // no one surface can be effected by more than 4
									  // animated lights.
/*off=64(16)*/	int			cached_light[MAXLIGHTMAPS];	// values currently used in lightmap
/*off=80(20)*/	BOOL	cached_dlight;				// true if dynamic light in cache
/*off=84(21)*/	Color <>		*samples;	// [numstyles*surfsize]

/*off=88(22)*/	decal_t		*pdecals;
};	// sizeof (GL_msurface_t) == 92 (23)

// 06/23/2002 MAH
// This structure is different between hardware and software mode.
//
// Software Mode - QW 'model.h'.
struct mleaf_t
{
// common with node
/*! Off=0(0)*/	int			contents;		// wil be a negative contents number
/*! Off=4(1)*/	int			visframe;		// node needs to be traversed if current

/*! Off=8(2)*/	short		minmaxs[6u];		// for bounding box culling

/*! Off=20(5)*/	mnode_t	*parent;

// leaf specific
/*! Off=24(6)*/	unsigned char		*compressed_vis;
/*! Off=28(7)*/	efrag_t	*efrags;

/*! Off=32(8)*/	msurface_t	**firstmarksurface;
/*! Off=36(9)*/	int			nummarksurfaces;
/*! Off=40(10)*/	int			key;			// BSP sequence number for leaf's contents
/*! Off=44(11)*/	unsigned char		ambient_sound_level[Constants::NUM_AMBIENTS];
};	// sizeof (mleaf_t) == 48 (12)
// hardware renderer - QW 'gl_model.h'.
struct GL_mleaf_t
{
// common with node
/*! Off=0(0)*/	int			contents;		// wil be a negative contents number
/*! Off=4(1)*/	int			visframe;		// node needs to be traversed if current

/*! Off=8(2)*/	Math::Extent3D		minmaxs;		// for bounding box culling

/*! Off=32(8)*/	mnode_t	*parent;

// leaf specific
/*! Off=36(9)*/	unsigned char		*compressed_vis;
/*! Off=40(10)*/	efrag_t	*efrags;

/*! Off=44(11)*/	msurface_t	**firstmarksurface;
/*! Off=48(12)*/	int			nummarksurfaces;
/*! Off=52(13)*/	int			key;			// BSP sequence number for leaf's contents
/*! Off=56(14)*/	unsigned char		ambient_sound_level[Constants::NUM_AMBIENTS];
};	// sizeof (GL_mleaf_t) == 60 (15)

// 06/23/2002 MAH
// This structure is the same in QW source files 'model.h' and 'gl_model.h'.
struct medge_t
{
/*! Off=0(0)*/	unsigned short	v[2u];
/*! Off=4(1)*/	unsigned int	cachededgeoffset;
};	// sizeof (mleaf_t) == 8 (2)

// 06/23/2002 MAH
// This structure is different between hardware and software mode.
//
// Software Mode - QW 'model.h'.
struct mnode_t
{
	enum ChildrenType_t
	{
		ChildrenType_Front,
		ChildrenType_Back,

		ChildrenType_Total
	};

// common with leaf
/*! Off=0(0)*/	int			contents;		// 0, to differentiate from leafs
/*! Off=4(1)*/	int			visframe;		// node needs to be traversed if current

/*! Off=8(2)*/	short		minmaxs[6u];		// for bounding box culling

/*! Off=20(5)*/	mnode_t	*parent;

// node specific
/*! Off=24(6)*/	MPlane_t	*plane;
/*! Off=28(7)*/	mnode_t	*children[ChildrenType_Total];

/*! Off=36(9)*/	unsigned short		firstsurface;
/*! Off=38(9.5)*/	unsigned short		numsurfaces;
};	// sizeof (mnode_t) == 40 (10)
// hardware mode - QW 'gl_model.h'.
struct GL_mnode_t
{
	enum ChildrenType_t
	{
		ChildrenType_Front,
		ChildrenType_Back,

		ChildrenType_Total
	};

// common with leaf
/*! Off=0(0)*/	int			contents;		// 0, to differentiate from leafs
/*! Off=4(1)*/	int			visframe;		// node needs to be traversed if current

/*! Off=8(2)*/	Math::Extent3D		minmaxs;		// for bounding box culling

/*! Off=32(8)*/	mnode_t	*parent;

// node specific
/*! Off=36(9)*/	MPlane_t	*plane;
/*! Off=40(10)*/	mnode_t	*children[ChildrenType_Total];

/*! Off=48(12)*/	unsigned short		firstsurface;
/*! Off=50(12.5)*/	unsigned short		numsurfaces;
};	// sizeof (GL_mnode_t) == 52 (13)

struct CacheUser_t
{
	void *data;
};

struct Model_t
{
#define MAX_MODEL_NAME		64

/*! Off=0(0)*/	char		name[MAX_MODEL_NAME];
/*! Off=64(16)*/	BOOL	needload;		// bmodels and sprites don't cache normally

/*! Off=68(17)*/	Constants::ModelType_t	type;
/*! Off=72(18)*/	int			numframes;
/*! Off=76(19)*/	Constants::synctype_t	synctype;

/*! Off=80(20)*/	int			flags;

//
// volume occupied by the model
//
/*! Off=84(21)*/	Math::Extent3D		boundingBox;
/*! Off=108(27)*/	float		radius;

//
// brush model
//
/*! Off=112(28)*/	int			firstmodelsurface, nummodelsurfaces;

/*! Off=120(30)*/	int			numsubmodels;
/*! Off=124(31)*/	DModel_t	*submodels;

/*! Off=128(32)*/	int			numplanes;
/*! Off=132(33)*/	MPlane_t	*planes;

/*! Off=136(34)*/	int			numleafs;		// number of visible leafs, not counting 0
/*! Off=140(35)*/	mleaf_t		*leafs;

/*! Off=144(36)*/	int			numvertexes;
/*! Off=148(37)*/	DVertex_t	*vertexes;

/*! Off=152(38)*/	int			numedges;
/*! Off=156(39)*/	medge_t		*edges;

/*! Off=160(40)*/	int			numnodes;
/*! Off=164(41)*/	mnode_t		*nodes;

/*! Off=168(42)*/	int			numtexinfo;
/*! Off=172(43)*/	mtexinfo_t	*texinfo;

/*! Off=176(44)*/	int			numsurfaces;
/*! Off=180(45)*/	msurface_t	*surfaces;

/*! Off=184(46)*/	int			numsurfedges;
/*! Off=188(47)*/	int			*surfedges;

/*! Off=192(48)*/	int			numclipnodes;
/*! Off=196(49)*/	DClipNode_t	*clipnodes;

/*! Off=200(50)*/	int			nummarksurfaces;
/*! Off=204(51)*/	msurface_t	**marksurfaces;

/*! Off=208(52)*/	Hull_t		hulls[Constants::Hull_Total];

/*! Off=368(92)*/	int			numtextures;
/*! Off=372(93)*/	texture_t	**textures;

/*! Off=376(94)*/	unsigned char		*visdata;

/*! Off=380(95)*/	Color <>		*lightdata;

/*! Off=384(96)*/	char		*entities;

//
// additional model data
//
/*! Off=388(97)*/	CacheUser_t	cache;		// only access through Mod_Extradata
};	// sizeof (Model_t) == 392 (98)

struct alight_t
{
	int			ambientlight;	// clip at 128
	int			shadelight;		// clip at 192 - ambientlight
	Math::Vector3D		color;
	float		*plightvec;
};

struct auxvert_t
{
	float fv[3];	// viewspace x, y
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_MODEL_INCLUDED