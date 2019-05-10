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
// Interface_CareerUI.hpp
//
// Class: Interface_CareerUI
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_CAREER_UI_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_CAREER_UI_INCLUDED

//-----------------------------------------------------------------------------
// Purpose: contains all the functions that the ICareerUI dll exports
//			CareerUI_GetInterface() is exported via dll export table to get this table
//-----------------------------------------------------------------------------
abstract_class ICareerUI : public VoidBaseHookedClassWithVirtualFunctionTable//Base
{
	// the interface version is the number to call CareerUI_GetInterface(int interfaceNumber) with
	#define CAREERUI_INTERFACE_VERSION "CareerUI001"

	public:
/*! off=4(1) */		unsigned int m_UNKNOWN0;
};	// sizeof (ICareerUI) == 280 (70)

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_INTERFACES_INTERFACE_CAREER_UI_INCLUDED