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
// Globals_definitions.cpp
//
// Version: $ID:$
//

#include <Core.hpp>

DynamicArray <Math::Vector3D> g_origins;

const float Server::ThrownGrenadesManager::ThrownGrenade_t::s_explosionOverTime (0.2f/*! original delay before explosion ?? */ + 0.1f);

DLL_GLOBAL LibraryHandleWithSize g_YaPBModuleHandle;	// YaPB module handle.

DLL_GLOBAL DynamicString g_YaPBFolder ("Addons/YaPB");

DLL_GLOBAL bool g_YaPBNotLoaded (false);

//DLL_GLOBAL const Product_t g_product;

DLL_GLOBAL Server *g_server (NULL);

DLL_GLOBAL Client *g_currentReloader (NULL);
DLL_GLOBAL bool g_isFirstReloadSoundMessageCall (false);

DLL_GLOBAL unsigned char g_skyLightLevel (255u);