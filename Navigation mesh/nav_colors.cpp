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
// Description: Central point for defining colors and drawing routines for Navigation Mesh edit mode.
//
// Version: $ID:$
//

#include <Core.hpp>

//--------------------------------------------------------------------------------------------------------------
/*** This MUST be kept in sync with the NavEditColor definition*/
const Color <> NavigationMesh::NavColors[NavEditColor_Total] =
{
	// Degenerate area colors
	Color <> (255u, 255u, 255u),	// NavDegenerateFirstColor
	Color <> (255u, 0u, 255u),		// NavDegenerateSecondColor

	// Place painting color
	Color <> (0u, 255u, 0u),		// NavSamePlaceColor
	Color <> (0u, 0u, 255u),		// NavDifferentPlaceColor
	Color <> (255u, 0u, 0u),		// NavNoPlaceColor

	// Normal colors
	Color <> (255u, 255u, 0u),		// NavSelectedColor
	Color <> (0u, 255u, 255u),		// NavMarkedColor
	Color <> (255u, 0u, 0u),		// NavNormalColor
	Color <> (0u, 0u, 255u),		// NavCornerColor
	Color <> (0u, 0u, 255u),		// NavBlockedColor

	Color <> (0u, 255u, 255u),		// NavUNKNOWN1Color

	// Hiding spot colors
	Color <> (255u, 0u, 0u),		// NavIdealSniperColor
	Color <> (255u, 0u, 255u),		// NavGoodSniperColor
	Color <> (0u, 255u, 0u),		// NavGoodCoverColor
	Color <> (255u, 0u, 255u),		// NavExposedColor
	Color <> (255u, 100u, 0u),		// NavApproachPointColor

	// Connector colors
	Color <> (0u, 255u, 255u),		// NavConnectedTwoWaysColor
	Color <> (0u, 0u, 255u),		// NavConnectedOneWayColor

	Color <> (0u, 255u, 0u),		// NavUNKNOWN2Color
	Color <> (255u, 0u, 0u),		// NavUNKNOWN3Color

	// Editing colors
	Color <> (255u, 255u, 255u),	// NavCursorColor
	Color <> (255u, 255u, 255u),	// NavSplitLineColor
	Color <> (0u, 255u, 255u),		// NavCreationColor
	Color <> (255u, 0u, 0u),		// NavUNKNOWN4Color
	Color <> (0u, 64u, 64u),		// NavGridColor

	Color <> (255u, 255u, 255u),	// NavUNKNOWN5Color

	// Navigation attribute colors
	Color <> (0u, 0u, 255u),		// NavAttributeCrouchColor
	Color <> (0u, 255u, 0u),		// NavAttributeJumpColor
	Color <> (0u, 255u, 0u),		// NavAttributePreciseColor
	Color <> (255u, 0u, 0u),		// NavAttributeNoJumpColor
	Color <> (255u, 0u, 0u),		// NavAttributeStopColor
	Color <> (0u, 0u, 255u),		// NavAttributeRunColor
	Color <> (0u, 255u, 0u),		// NavAttributeWalkColor
	Color <> (255u, 0u, 0u),		// NavAttributeAvoidColor
	Color <> (0u, 200u, 0u)			// NavAttributeStairsColor
};


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavDrawLine (const Math::Vector3D &from, const Math::Vector3D &to, const NavEditColor navColor)
{
	const Math::Vector3D offset (0.0f, 0.0f, 1.0f);
	const Color <> &color (NavColors[navColor]);
	const Human *const host (g_server->GetHumanManager ()->GetHostClient ());

	// Reliability check.
	InternalAssert (host != NULL);

//	NDebugOverlay::Line (from + offset, to + offset, color.red / 2u, color.green / 2u, color.blue / 2u, true, 0.1f);
//	NDebugOverlay::Line (from + offset, to + offset, color.red, color.green, color.blue, false, 0.15f);
//	host->DrawLine (from + offset, to + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color / 2u, 255u, 0u, 1u);
//	host->DrawLine (from + offset, to + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 0u, 15u);
	host->DrawLine (from + offset, to + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u/*5u*/, 0u, color, 255u, 0u, 1u, false);
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavDrawTriangle (const Math::Vector3D &point1, const Math::Vector3D &point2, const Math::Vector3D &point3, const NavEditColor navColor)
{
	NavDrawLine (point1, point2, navColor);
	NavDrawLine (point2, point3, navColor);
	NavDrawLine (point1, point3, navColor);
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavDrawHorizontalArrow (const Math::Vector3D &from, const Math::Vector3D &to, const /*float*/unsigned char width, const NavEditColor navColor)
{
	const Math::Vector3D offset (0.0f, 0.0f, 1.0f);
	const Color <> &color (NavColors[navColor]);
	const Human *const host (g_server->GetHumanManager ()->GetHostClient ());

	// Reliability check.
	InternalAssert (host != NULL);

//	NDebugOverlay::HorzArrow (from + offset, to + offset, width, color.red / 2u, color.green / 2u, color.blue / 2u, 255, true, 0.1f);
//	NDebugOverlay::HorzArrow (from + offset, to + offset, width, color.red, color.green, color.blue, 255, false, 0.15f);
//	host->DrawLine (from + offset, to + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), width, 0u, color / 2u, 255u, 0u, 1u);
//	host->DrawLine (from + offset, to + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), width, 0u, color, 255u, 0u, 15u);
	host->DrawLine (from + offset, to + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), width, 0u, color, 255u, 0u, 1u, false);
}


//--------------------------------------------------------------------------------------------------------------
void NavigationMesh::NavDrawDashedLine (const Math::Vector3D &from, const Math::Vector3D &to, const NavEditColor navColor)
{
	const Math::Vector3D offset (0.0f, 0.0f, 1.0f);
	const Color <> &color (NavColors[navColor]);
	const float solidLength (7.0f);
	const float gapLength (3.0f);
	Math::Vector3D direction (to - from);
	const float totalDistance (direction.GetLength ());
	float distance (0.0f);
	const Human *const host (g_server->GetHumanManager ()->GetHostClient ());

	// Reliability check.
	InternalAssert (host != NULL);
	InternalAssert (totalDistance > 0.0f);

	direction /= totalDistance;

	do
	{
		const Math::Vector3D start (from + direction * distance);
		const float endDistance (Math::GetMinimumValueBetween (distance + solidLength, totalDistance));
		const Math::Vector3D end (from + direction * endDistance);

		distance += solidLength + gapLength;

//		NDebugOverlay::Line (start + offset, end + offset, color.red / 2u, color.green / 2u, color.blue / 2u, true, 0.1f);
//		NDebugOverlay::Line (start + offset, end + offset, color.red, color.green, color.blue, false, 0.15f);
//		host->DrawLine (start + offset, end + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color / 2u, 255u, 0u, 1u);
//		host->DrawLine (start + offset, end + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 0u, 15u);
		host->DrawLine (start + offset, end + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 5u, 0u, color, 255u, 0u, 1u, false);
	} while (distance < totalDistance);
}
//--------------------------------------------------------------------------------------------------------------