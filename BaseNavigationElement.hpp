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
// BaseNavigationElement.hpp
//
// Class: BaseNavigationElement
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef BASE_NAVIGATION_ELEMENT_INCLUDED
#define BASE_NAVIGATION_ELEMENT_INCLUDED
/*! WE CAN'T!!!!!!!!!!!!!!
class NavigationMesh
class NavigationMesh::NavigationArea;
class NavigationMesh::NavigationLadder;
*/
class BaseNavigationElement
{
	//
	// Group: Protected members.
	//
	protected:
		/// @todo PUT HERE SOME COMMON VARIABLES!!!!!!!!!!!!!!!!!!!!!!

	//
	// Group: (Con/De)structors.
	//
	public:
		virtual inline ~BaseNavigationElement (void) { /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		virtual inline const bool IsWaypoint           (void) const = 0;	// Is 'Waypoint' or some derived class?
		virtual inline const bool IsBaseNavigationArea (void) const = 0;	// Is 'NavigationMesh::BaseNavigationArea' or some derived class?
		virtual inline const bool IsNavigationLadder   (void) const = 0;	// Is 'NavigationMesh::NavigationLadder' or some derived class?

		inline       Waypoint   *const                       GetWaypointPointer (void)
		{
			// Reliability check.
			InternalAssert (IsWaypoint ());

			return reinterpret_cast <Waypoint *const> (this);
		}
		inline const Waypoint   *const                       GetWaypointPointer (void) const
		{
			// Reliability check.
			InternalAssert (IsWaypoint ());

			return reinterpret_cast <const Waypoint *const> (this);
		}

		template <typename elementType> inline       elementType *const GetAreaPointer   (void);
		template <typename elementType> inline const elementType *const GetAreaPointer   (void) const;

		template <typename elementType> inline       elementType *const GetLadderPointer (void);
		template <typename elementType> inline const elementType *const GetLadderPointer (void) const;
/*! WE CAN'T FORWARD DECLARE NESTED 'NavigationMesh::Navigation*' CLASSES!!!!!!!!!!!!!!!! D}:
		inline       NavigationMesh::NavigationArea   *const GetAreaPointer     (void)
		{
			// Reliability check.
			InternalAssert (IsBaseNavigationArea () && !IsNavigationLadder ());

			return reinterpret_cast <NavigationMesh::NavigationArea *const> (this);
		}
		inline const NavigationMesh::NavigationArea   *const GetAreaPointer     (void) const
		{
			// Reliability check.
			InternalAssert (IsBaseNavigationArea () && !IsNavigationLadder ());

			return reinterpret_cast <const NavigationMesh::NavigationArea *const> (this);
		}

		inline       NavigationMesh::NavigationLadder *const GetLadderPointer   (void)
		{
			// Reliability check.
			InternalAssert (IsNavigationLadder ());

			return reinterpret_cast <NavigationMesh::NavigationLadder *const> (this);
		}
		inline const NavigationMesh::NavigationLadder *const GetLadderPointer   (void) const
		{
			// Reliability check.
			InternalAssert (IsNavigationLadder ());

			return reinterpret_cast <const NavigationMesh::NavigationLadder *const> (this);
		}*/
};

#endif	// ifndef BASE_NAVIGATION_ELEMENT_INCLUDED