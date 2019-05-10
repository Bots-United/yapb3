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
// Editor.hpp
//
// Class: Editor
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_MESH_EDITOR_INCLUDED
#define NAVIGATION_MESH_EDITOR_INCLUDED

class Editor
{
	//
	// Group: Private members.
	//
	private:
		Place_t m_navPlace;											///< current navigation place for editing
		bool m_isEditing;											///< true if in edit mode
		bool m_isNavigationMeshChanged;								///< true if navigation mesh has been changed by editor
		Math::Vector3D m_editCursorPos;								///< current position of the cursor
		NavigationArea *m_markedArea;								///< currently marked area for edit operations
		NavigationArea *m_selectedArea;								///< area that is selected this frame
		NavigationArea *m_lastSelectedArea;							///< area that was selected last frame
		NavCornerType_t m_markedCorner;								///< currently marked corner for edit operations
		Math::Vector3D m_anchor;									///< first corner of an area being created
		bool m_isPlacePainting;										///< if true, we set an area's place by pointing at it
		bool m_splitAlongX;											///< direction the selected navigation area would be split
		float m_splitEdge;											///< location of the possible split

		enum NavEditMode
		{
			NAV_EDIT_NORMAL,
			NAV_EDIT_PLACE,											///< place-editing mode
			NAV_EDIT_CREATE_AREA,									///< manually creating a new navigation area
			NAV_EDIT_CREATE_LADDER,									///< manually creating a new navigation ladder
			NAV_EDIT_RESIZE_HORIZONTAL,
			NAV_EDIT_RESIZE_VERTICAL
		} m_navEditMode;

		bool m_climbableSurface;									///< if true, the cursor is pointing at a climable surface
		Math::Vector3D m_surfaceNormal;								///< Normal of the surface the cursor is pointing at
		Math::Vector3D m_ladderAnchor;								///< first corner of a ladder being created
		Math::Vector3D m_ladderNormal;								///< Normal of the surface of the ladder being created
		NavigationLadder *m_selectedLadder;							///< ladder that is selected this frame
		NavigationLadder *m_lastSelectedLadder;						///< ladder that was selected last frame
		NavigationLadder *m_markedLadder;							///< currently marked ladder for edit operations

		HalfLifeEngine::Utilities::CountdownTimer m_showAreaInfoTimer;							///< Timer that controls how long area info is displayed

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Editor (void) :
			m_navPlace (UNDEFINED_PLACE),
			m_isEditing (false),
			m_isNavigationMeshChanged (false),
			m_markedArea (NULL),
			m_selectedArea (NULL),
			m_lastSelectedArea (NULL),
			m_isPlacePainting (false),
			m_navEditMode (NAV_EDIT_NORMAL),
			m_climbableSurface (false),
			m_selectedLadder (NULL),
			m_markedLadder (NULL)
		{ /*VOID*/ }
		inline ~Editor (void)
		{
			//
		}

	//
	// Group: Private functions.
	//
	public:
		void OnEditModeStart (void);								///< called when edit mode has just been enabled
		void DrawEditMode    (void);								///< draw navigation areas
		void OnEditModeEnd   (void);								///< called when edit mode has just been disabled

		const bool FindActiveNavArea (void);								///< Finds the area or ladder the local player is currently pointing at. Returns true if a surface was hit by the traceline.

		inline void SetEditMode (const NavEditMode mode = NAV_EDIT_NORMAL)						///< sets/clears place mode
		{
			m_markedLadder = NULL;
			m_markedArea = NULL;
			m_markedCorner = NUM_CORNERS;
			m_navEditMode = mode;
/*			m_isSelecting = false;
			m_isDeselecting = false;
			m_isUNKNOWN = false;*/
		}
		void SetPlacePaintingMode (const bool painting);				///< Sets place-painting, if we're in place mode
		void SetMarkedLadder (NavigationLadder *const ladder);			///< mark ladder for further edit operations
		void SetMarkedArea   (NavigationArea *const area);				///< mark area for further edit operations

	//
	// Group: Functions.
	//
	public:
		inline const Place_t GetNavPlace (void)              const { return m_navPlace; }
		inline void        SetNavPlace (const Place_t place)       { m_navPlace = place; }
		inline const Math::Vector3D &GetEditCursorPosition (void) const { return m_editCursorPos; }	///< return position of edit cursor

		// Edit callbacks from 'ConsoleCommand's
		void CommandNavDelete                       (void);									///< delete current area
		void CommandNavDeleteMarked                 (void);									///< delete current marked area
		void CommandNavSplit                        (void);									///< split current area
		void CommandNavMerge                        (void);									///< merge adjacent areas
		void CommandNavMark                         (void);									///< mark an area for further operations
		void CommandNavUnmark                       (void);									///< removes the mark
		void CommandNavBeginArea                    (void);									///< begin creating a new navigation area
		void CommandNavEndArea                      (void);									///< end creation of the new navigation area
		void CommandNavConnect                      (void);									///< connect marked area to selected area
		void CommandNavDisconnect                   (void);									///< disconnect marked area from selected area
		void CommandNavSplice                       (void);									///< create new area in between marked and selected areas
		void CommandNavCrouch                       (void);									///< toggle crouch attribute on current area
		void CommandNavTogglePlaceMode              (void);									///< switch between normal and place editing
		void CommandNavSetPlaceMode                 (void);									///< switch between normal and place editing
		void CommandNavPlaceFloodFill               (void);									///< floodfill areas out from current area
		void CommandNavPlacePick                    (void);									///< "pick up" the place at the current area
		void CommandNavTogglePlacePainting          (void);									///< switch between "painting" places onto areas
		void CommandNavMarkUnnamed                  (void);									///< mark an unnamed area for further operations
		void CommandNavCornerSelect                 (void);									///< select a corner on the current area
		void CommandNavCornerRaise                  (void);									///< raise a corner on the current area
		void CommandNavCornerLower                  (void);									///< lower a corner on the current area
		void CommandNavCornerPlaceOnGround          (void);									///< position a corner on the current area at ground height
		void CommandNavWarpToMark                   (void) const;							///< warp a spectating local player to the selected mark
		void CommandNavLadderFlip                   (void);									///< Flips the direction a ladder faces
		void CommandNavToggleAttribute              (const NavAttributeFlag_t attribute);	///< toggle an attribute on current area
		void CommandNavMakeSniperSpots              (void);									///< cuts up the marked area into individual areas suitable for sniper spots
		void CommandNavBuildLadder                  (void);									///< builds a navigation ladder on the climbable surface under the cursor
		inline void CommandNavRemoveUnusedJumpAreas (void) { /*g_server->GetNavigationMeshManager ().RemoveUnusedJumpAreas ();*//* - TEMPORARY!!!!!!!!!*/ }	///< removes jump areas with at most 1 connection to a ladder or non-jump area

		// IN-PROGRESS COMMANDS FOR MANIPULATING EXISTING AREAS
		void CommandNavPickArea (void);
		void CommandNavResizeHorizontal (void);
		void CommandNavResizeVertical (void);
		void CommandNavResizeEnd (void);

		inline const bool IsPlaceMode (void) const { return m_navEditMode == NAV_EDIT_PLACE; }

		inline void GetEditVectors (Math::Vector3D &eyePosition, Math::Vector3D &viewDirection) const;	///< Gets the eye position and view direction of the editing player. (Note: This function declared in HumanManager.hpp)

		inline NavigationArea *const   GetMarkedArea     (void) const { return m_markedArea; }		///< return area marked by user in edit mode
		inline NavigationLadder *const GetMarkedLadder   (void) const { return m_markedLadder; }		///< return ladder marked by user in edit mode

		inline NavigationArea *const   GetSelectedArea   (void) const { return m_selectedArea; }		///< return area selected by user in edit mode
		inline NavigationLadder *const GetSelectedLadder (void) const { return m_selectedLadder; }	///< return ladder selected by user in edit mode

		void CreateLadder (const Math::Vector3D &absoluteMins, const Math::Vector3D &absoluteMaxs);
};

#endif	// ifndef NAVIGATION_MESH_EDITOR_INCLUDED