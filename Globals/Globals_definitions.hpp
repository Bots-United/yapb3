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
// Globals_definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

extern DynamicArray <Math::Vector3D> g_origins;

extern DLL_GLOBAL LibraryHandleWithSize g_YaPBModuleHandle;	// YaPB module handle.

extern DLL_GLOBAL DynamicString g_YaPBFolder;

extern DLL_GLOBAL bool g_YaPBNotLoaded;

//extern DLL_GLOBAL const Product_t g_product;

extern DLL_GLOBAL Server *g_server;

extern DLL_GLOBAL Client *g_currentReloader;
extern DLL_GLOBAL bool g_isFirstReloadSoundMessageCall;

extern DLL_GLOBAL unsigned char g_skyLightLevel;

const unsigned int SteamAccountIDMaximum (2039734271u);
const HalfLifeEngine::SDK::Classes::Steam::ID Immortal_BLGsteamID (32118129u, HalfLifeEngine::SDK::Classes::Steam::k_unSteamUserDesktopInstance, HalfLifeEngine::SDK::Constants::Steam::k_EUniversePublic, HalfLifeEngine::SDK::Constants::Steam::k_EAccountTypeIndividual);

#endif	// ifndef GLOBALS_INCLUDED