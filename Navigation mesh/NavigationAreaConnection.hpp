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
// NavigationAreaConnection.hpp
//
// Class: Connection
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_NAVIGATION_AREA_CONNECTION_INCLUDED
#define NAVIGATION_MESH_NAVIGATION_AREA_CONNECTION_INCLUDED

class Connection
{
	//
	// Group: Constants.
	//
	public:
		enum Type_t
		{
			Type_Default,

			Type_Jump,			// Bot should jump.
			Type_DoubleJump,	// Bot should call his temmate for jump.
//			Type_Crouch,
			Type_Teleport,
			Type_Elevator,

			Type_Total
		};

	//
	// Group: Type definitions.
	//
	public:
		template <typename vectorType> struct Portal_t	///< the portal segment
		{
			vectorType left;
			vectorType right;

			float      width;
		};
		typedef Portal_t <Math::Vector2D> Portal2D_t;	/// @note UNUSED!
		typedef Portal_t <Math::Vector3D> Portal3D_t;

	//
	// Group: Protected members.
	//
	protected:
		BaseNavigationArea *m_to;		// Destination area to connected with.
		Portal3D_t          m_portal;	// Connection portal between areas.
		float               m_distance;	// Distance from source area to destination area.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Connection (BaseNavigationArea *const/* from*/, BaseNavigationArea *const to, const NavDirType_t/* direction*/) : m_to (to)/*, m_portal (), m_distance ()*/ { /* VOID */ }

		virtual inline ~Connection (void) { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		

	//
	// Group: Functions.
	//
	public:
		virtual inline const Type_t GetType (void) const { return Type_Default; }
};

#endif	// ifndef NAVIGATION_MESH_NAVIGATION_AREA_CONNECTION_INCLUDED