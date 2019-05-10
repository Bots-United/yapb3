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
// Studio.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef STUDIO_INCLUDED
#define STUDIO_INCLUDED

/*
==============================================================================

STUDIO MODELS

Studio models are position independent, so the cache manager can move them.
==============================================================================
*/


// header for demand loaded sequence group data
struct studioseqhdr_t
{
	int  id;
	int  version;

	char name[64u];
	int  length;
};

// bones
struct mstudiobone_t
{
	char  name[32u];			// bone name for symbolic links
	int   parent;				// parent bone
	int   flags;				// ??
	int   bonecontroller[6u];	// bone controller index, -1 == none
	float value[6u];			// default DoF values
	float scale[6u];			// scale for delta DoF values
};

// bone controllers
struct mstudiobonecontroller_t
{
	int   bone;		// -1 == 0
	int   type;		// X, Y, Z, XR, YR, ZR, M (STUDIO_* constants)
	float start;
	float end;
	int   rest;		// unsigned char index value at rest
	int   index;	// 0-3 user set controller, 4 mouth
};

// intersection boxes
struct mstudiobbox_t
{
	int            bone;
	int            group;		// intersection group (for players from Constants::PlayerBodyPart_t enumeration)
	Math::Extent3D boundingBox;	// bounding box
};

// demand loaded sequence groups
struct mstudioseqgroup_t
{
	char        label[32u];	// textual name
	char        name[64u];	// file name
	CacheUser_t cache;		// cache index pointer
	int         data;		// hack for group 0
};

// events
struct mstudioevent_t
{
	int  frame;
	int  event;
	int  type;
	char options[64u];
};

// pivots
struct mstudiopivot_t
{
	Math::Vector3D org;	// pivot point
	int            start;
	int            end;
};

// sequence descriptions
struct mstudioseqdesc_t
{
	char           label[32u];	// sequence label

	float          fps;			// frames per second
	int            flags;		// looping/non-looping flags

	int            activity;	// initialized at loadtime to game DLL values
	int            actweight;

	// 'mstudioevent_t' structure information....
	int            numevents;
	int            eventindex;
	inline const mstudioevent_t *const pEvent (const unsigned int i) const { InternalAssert (i < static_cast <unsigned int> (numevents)); return reinterpret_cast <const mstudioevent_t *> (reinterpret_cast <const unsigned char *> (this) + eventindex) + i; }

	int            numframes;	// number of frames per sequence

	// 'mstudiopivot_t' structure information....
	int            numpivots;	// number of foot pivots
	int            pivotindex;

	int            motiontype;
	int            motionbone;
	Math::Vector3D linearmovement;
	int            automoveposindex;
	int            automoveangleindex;

	Math::Extent3D boundingBox;		// per sequence bounding box

	int            numblends;
	int            animindex;		// 'mstudioanim_t' pointer relative to start of sequence group data [blend][bone][X, Y, Z, XR, YR, ZR]

	int            blendtype[2u];	// X, Y, Z, XR, YR, ZR
	float          blendstart[2u];	// starting value
	float          blendend[2u];	// ending value
	int            blendparent;

	int            seqgroup;		// sequence group for demand loading

	int            entrynode;		// transition node at entry
	int            exitnode;		// transition node at exit
	int            nodeflags;		// transition rules

	int            nextseq;			// auto advancing sequences
};

// attachment
struct mstudioattachment_t
{
	char           name[32u];
	int            type;
	int            bone;
	Math::Vector3D org;	// attachment point
	Math::Vector3D vectors[3u];
};

struct mstudioanim_t
{
	unsigned short offset[6u];
};

// animation frames
union mstudioanimvalue_t
{
	struct
	{
		unsigned char valid;
		unsigned char total;
	} num;
	short value;
};

// skin info
struct mstudiotexture_t
{
	char name[64u];
	int  flags;
	int  width;
	int  height;
	int  index;
};


// skin families
// short	index[skinfamilies][skinref]

// meshes
struct mstudiomesh_t
{
	int numtris;
	int triindex;
	int skinref;
	int numnorms;	// per mesh normals
	int normindex;	// normal Math::Vector3D
};

// studio models
struct mstudiomodel_t
{
	char  name[64u];

	int   type;

	float boundingradius;

	int   nummesh;
	int   meshindex;
	inline const mstudiomesh_t *const pMesh (const unsigned int i) const { return reinterpret_cast <const mstudiomesh_t *> (reinterpret_cast <const unsigned char *> (this) + meshindex) + i; }

	// cache purposes
	int   numverts;			// number of unique vertices/normals/texcoords
	int   vertinfoindex;	// vertex bone info
	int   vertindex;		// vertex Math::Vector3D
	int   numnorms;			// number of unique surface normals
	int   norminfoindex;	// normal bone info
	int   normindex;		// normal Math::Vector3D

	int   numgroups;		// deformation groups
	int   groupindex;
};

// body part index
struct mstudiobodyparts_t
{
	char name[64u];
	int  nummodels;
	int  base;
	int  modelindex;	// index into models array
	inline const mstudiomodel_t *const pModel (const unsigned int i) const { return reinterpret_cast <const mstudiomodel_t *> (reinterpret_cast <const unsigned char *> (this) + modelindex) + i; }
};

// Math::Vector3D boundingbox[model][bone][2u];	// complex intersection info

// triangles
#if 0
struct mstudiotrivert_t
{
	short vertindex;	// index into vertex array
	short normindex;	// index into normal array
	short s, t;			// s, t position on skin
};
#endif

struct StudioHeader_t
{
	int            id;
	int            version;

	char           name[64u];
	int            length;

	Math::Vector3D eyeposition;			// ideal eye position
	Math::Vector3D min;					// ideal movement hull size
	Math::Vector3D max;

	Math::Extent3D boundingBox;			// clipping bounding box

	int            flags;

	// 'mstudiobone_t' structure information....
	int            numbones;			// bones
	int            boneindex;
	inline const mstudiobone_t *const pBone (const unsigned int i) const { InternalAssert (i < static_cast <unsigned int> (numbones)); return reinterpret_cast <const mstudiobone_t *> (reinterpret_cast <const unsigned char *> (this) + boneindex) + i; }

	// 'mstudiobonecontroller_t' structure information....
	int            numbonecontrollers;	// bone controllers
	int            bonecontrollerindex;
	inline const mstudiobonecontroller_t *const pBonecontroller (const unsigned int i) const { InternalAssert (i < static_cast <unsigned int> (numbonecontrollers)); return reinterpret_cast <const mstudiobonecontroller_t *> (reinterpret_cast <const unsigned char *> (this) + bonecontrollerindex) + i; }

	// 'mstudiobbox_t' structure information....
	int            numhitboxes;			// complex bounding boxes
	int            hitboxindex;

	// 'mstudioseqdesc_t' structure information....
	int            numseq;				// animation sequences
	int            seqindex;

	// 'mstudioseqgroup_t' structure information....
	int            numseqgroups;		// demand loaded sequences
	int            seqgroupindex;

	// 'mstudiotexture_t' structure information....
	int            numtextures;			// raw textures
	int            textureindex;
	int            texturedataindex;
	inline const mstudiotexture_t *const pTexture (const unsigned int i) const { InternalAssert (i < static_cast <unsigned int> (numtextures)); return reinterpret_cast <const mstudiotexture_t *> (reinterpret_cast <const unsigned char *> (this) + textureindex) + i; }

	int            numskinref;			// replaceable textures tables
	int            numskinfamilies;
	int            skinindex;
	inline const short *const pSkinref (const unsigned int i) const { return reinterpret_cast <const short *> (reinterpret_cast <const unsigned char *> (this) + skinindex) + i; }

	// 'mstudiobodyparts_t' structure information....
	int            numbodyparts;
	int            bodypartindex;
	inline const mstudiobodyparts_t *const pBodypart (const unsigned int i) const { return reinterpret_cast <const mstudiobodyparts_t *> (reinterpret_cast <const unsigned char *> (this) + bodypartindex) + i; }

	// 'mstudioattachment_t' structure information....
	int            numattachments;		// queryable attachable points
	int            attachmentindex;

	int            soundtable;
	int            soundindex;
	int            soundgroups;
	int            soundgroupindex;

	int            numtransitions;		// animation node to animation node transition graph
	int            transitionindex;
};

#endif	// ifndef STUDIO_INCLUDED