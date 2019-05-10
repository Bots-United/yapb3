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
// Customization.hpp
//
// Structure: Customization_t
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_CUSTOMIZATION_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_CUSTOMIZATION_INCLUDED

#define FCUST_FROMHPAK		( 1<<0 )
#define FCUST_WIPEDATA		( 1<<1 )
#define FCUST_IGNOREINIT	( 1<<2 )

struct Customization_t
{
	BOOL bInUse;     // Is this customization in use;
	Resource_t resource; // The Resource_t for this customization
	BOOL bTranslated; // Has the raw data been translated into a useable format? (e.g., raw decal .wad make into texture_t *)
	int        nUserData1; // Customization specific data
	int        nUserData2; // Customization specific data
	void *pInfo;          // Buffer that holds the data structure that references the data (e.g., the cachewad_t)
	void *pBuffer;       // Buffer that holds the data for the customization (the raw .wad data)
	Customization_t *pNext; // Next in chain
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_CUSTOMIZATION_INCLUDED