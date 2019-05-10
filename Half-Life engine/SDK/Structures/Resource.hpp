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
// Resource.hpp
//
// Structure: Resource_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_RESOURCE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_RESOURCE_INCLUDED

/////////////////
// Customization
// passed to pfnPlayerCustomization
// For automatic downloading.
enum resourcetype_t
{
	t_sound,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript,
	t_world	// Fake type for world, is really t_model
};


struct _resourceinfo_t
{
	int size;
};
struct resourceinfo_t
{
	_resourceinfo_t info[8u];
};

struct Resource_t
{
	DECLARE_TYPED_ENUMERATION (unsigned char, Flag_t)
	{
		RES_FATALIFMISSING = BIT (0u),	// Disconnect if we can't get this file.
		RES_WASMISSING     = BIT (1u),	// Do we have the file locally, did we get it ok?
		RES_CUSTOM         = BIT (2u),	// Is this resource one that corresponds to another player's customization or is it a server startup resource.
		RES_REQUESTED      = BIT (3u),	// Already requested a download of this one
		RES_PRECACHED      = BIT (4u)	// Already precached
	};

/*!off=0c!*/	char              szFileName[Constants::MaximumPathNameLength]; // File name to download/precache.
/*!off=64c!*/	resourcetype_t    type;                // t_sound, t_skin, t_model, t_decal.
/*!off=68c!*/	int               nIndex;              // For t_decals
/*!off=72c!*/	int               nDownloadSize;       // Size in Bytes if this must be downloaded.
/*!off=76c!*/	Flag_t            ucFlags;

// For handling client to client resource propagation
/*!off=77c!*/	unsigned char     rgucMD5_hash[16u];    // To determine if we already have it.
/*!off=93c!*/	unsigned char     playernum;           // Which player index this resource is associated with, if it's a custom resource.

/*!off=94c!*/	unsigned char	  rguc_reserved[32u]; // For future expansion
/*!off=125c!*/	Resource_t *pNext;              // Next in chain.
/*!off=129c!*/	Resource_t *pPrev;
};	// sizeof (Resource_t) == 136 (34)

#define	MAX_RESOURCES	1280u

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_RESOURCE_INCLUDED