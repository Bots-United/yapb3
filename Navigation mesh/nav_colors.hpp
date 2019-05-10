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
// NavigationMeshColors.hpp
//
// Description: Colors used for navigation mesh editing.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_COLORS_INCLUDED
#define NAVIGATION_MESH_COLORS_INCLUDED

//--------------------------------------------------------------------------------------------------------------
enum NavEditColor
{
	// Degenerate area colors
	NavDegenerateFirstColor,
	NavDegenerateSecondColor,

	// Place painting color
	NavSamePlaceColor,
	NavDifferentPlaceColor,
	NavNoPlaceColor,

	// Normal colors
	NavSelectedColor,
	NavMarkedColor,
	NavNormalColor,
	NavCornerColor,
	NavBlockedColor,

	NavUNKNOWN1Color,

	// Hiding spot colors
	NavIdealSniperColor,
	NavGoodSniperColor,
	NavGoodCoverColor,
	NavExposedColor,
	NavApproachPointColor,

	// Connector colors
	NavConnectedTwoWaysColor,
	NavConnectedOneWayColor,

	NavUNKNOWN2Color,
	NavUNKNOWN3Color,

	// Editing colors
	NavCursorColor,
	NavSplitLineColor,
	NavCreationColor,
	NavUNKNOWN4Color,
	NavGridColor,

	NavUNKNOWN5Color,

	// Navigation attribute colors
	NavAttributeCrouchColor,
	NavAttributeJumpColor,
	NavAttributePreciseColor,
	NavAttributeNoJumpColor,
	NavAttributeStopColor,
	NavAttributeRunColor,
	NavAttributeWalkColor,
	NavAttributeAvoidColor,
	NavAttributeStairsColor,

	NavEditColor_Total
};

static const Color <> NavColors[NavEditColor_Total];

//--------------------------------------------------------------------------------------------------------------

static void NavDrawLine (const Math::Vector3D &from, const Math::Vector3D &to, const NavEditColor navColor);
static void NavDrawTriangle (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const NavEditColor navColor);
static void NavDrawHorizontalArrow (const Math::Vector3D &from, const Math::Vector3D &to, const /*float*/unsigned char width, const NavEditColor navColor);
static void NavDrawDashedLine (const Math::Vector3D &from, const Math::Vector3D &to, const NavEditColor navColor);

//--------------------------------------------------------------------------------------------------------------

#endif	// ifndef NAVIGATION_MESH_COLORS_INCLUDED