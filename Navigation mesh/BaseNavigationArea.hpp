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
// BaseNavigationArea.hpp
//
// Class: BaseNavigationArea
//
// Description: AI Navigation areas.
//				A BaseNavigationArea is a rectangular region defining a walkable area in the environment.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BASE_NAVIGATION_AREA_INCLUDED
#define BASE_NAVIGATION_AREA_INCLUDED

class BaseNavigationArea : public BaseNavigationElement
{
	friend NavigationMesh;

	//
	// Group: Type definitions.
	//
	public:
		typedef unsigned short ID_t;
		typedef DynamicArray <Client *, unsigned char> PlayersArray;

		#include <Navigation mesh/NavigationAreaDanger.hpp>
		#include <Navigation mesh/NavigationAreaConnection.hpp>

	//
	// Group: Protected members.
	//
	protected:
/*!
	'm_extent.mins'				'Math::Vector3D (m_extent.maxs.x, m_extent.mins.y, m_neZ)'
		NORTH-WEST	   NORTH (-)  NORTH-EAST
			 •—————————————————————————•
			 |                         |
			 |                         |
			 |                         |
			 |                         |
			 |       'm_center'        |
	WEST (-) |            +———> X      | EAST (+)
			 |            |            |
			 |            V            |
			 |            Y            |
			 |                         |
			 |                         |
			 •—————————————————————————•
		SOUTH-WEST	   SOUTH (+)  SOUTH-EAST
	'Math::Vector3D
	(
		m_extent.mins.x,
		m_extent.maxs.y,
		m_swZ
	)'			  				  'm_extent.maxs'
*/
		ID_t                       m_baseId;																	///< unique area ID
		ID_t                       m_id;																		///< unique derived area ID
		Math::Extent3D             m_extent;																	///< extents of area in world coords (NOTE: mins.z is not necessarily the minimum Z, but corresponds to Z at point (mins.x, mins.y), etc
		float                      m_extentInvertedSizeX;														///< Inverted extent size X. (1.0f / (m_extent.maxs.x - m_extent.mins.x))
		float                      m_extentInvertedSizeY;														///< Inverted extent size Y. (1.0f / (m_extent.maxs.y - m_extent.mins.y))
		Math::Vector3D             m_center;																	///< centroid of area

		///< height of the implicit corners
		float                      m_neZ;
		float                      m_swZ;

		NavAttributeFlag_t         m_attributeFlags;															///< set of attribute bit flags

		Place_t                    m_place;																		///< place descriptor
		bool                       m_isBlocked;																	///< if true, some part of the world is preventing movement through this navigation area
		bool                       m_isUnderwater;																///< true if the center of the area is underwater
#if 0
		//- connections to adjacent areas -------------------------------------------------------------------
		template <typename elementType> struct Connection_t
		{
			Math::Vector3D      position;		///< portal position between areas
			float               halfWidth;		///< radius of the link
			float               distance;		///< length of the link
			const elementType  *to;				///< area to connected with

			NavAttributeFlag_t  attributeFlags;	///< set of attribute bit flags
		};
		typedef Connection_t <NavigationArea>   AreaConnection_t;
		typedef Connection_t <NavigationLadder> LadderConnection_t;
		typedef DynamicArray <AreaConnection_t,   unsigned char> AreaConnectionArray;
		typedef DynamicArray <LadderConnection_t, unsigned char> LadderConnectionArray;
#endif	// if 0
		//- for hunting -------------------------------------------------------------------------------------
		float                      m_clearedTimestamp[TeamArrayID_Total];										///< time this area was last "cleared" of enemies

		//- "danger" ----------------------------------------------------------------------------------------
		Danger                     m_danger[TeamArrayID_Total];													///< the information about danger of this area, high level is allowing bots to avoid areas where they died in the past.

		//- occupy time -------------------------------------------------------------------------------------
		float                      m_earliestOccupyTime[TeamArrayID_Total];										///< min time to reach this spot from spawn

		bool                       m_isBattlefront;

		//- player counting ---------------------------------------------------------------------------------
		PlayersArray               m_players[TeamArrayID_Total];												///< the number of players currently in this area

//		BaseNavigationArea        *m_basePrevHash, *m_baseNextHash;												///< for hash table in 'NavigationMesh'

	//
	// Group: (Con/De)structors.
	//
	public:
		inline BaseNavigationArea (const ID_t id);	///< Constructor used during normal runtime. (Note: This function declared in Server.hpp)

		virtual inline ~BaseNavigationArea (void);	// Note: This function declared in Server.hpp.

	//
	// Group: Private operators.
	//
	private:
		inline BaseNavigationArea &operator = (const BaseNavigationArea &/*right*/);	// Avoid "warning C4512: 'BaseNavigationArea' : assignment operator could not be generated".

	//
	// Group: Private functions.
	//
	private:
#if 0
		//- "danger" ----------------------------------------------------------------------------------------
		/**
		* Decay the danger values.
		*/
		inline void DecayDangers (const float time = 2.0f * 60.0f)
		{
			for (unsigned char index (TeamArrayID_Terrorist); index < TeamArrayID_Total; ++index)
				m_danger[index].Decay (time);
		}
#endif	// if 0
	//
	// Group: Functions.
	//
	public:
		inline const bool                            IsWaypoint           (void) const { return false; }
		inline const bool                            IsBaseNavigationArea (void) const { return true; }

		virtual inline void       Save (BaseFile &file) const
		{
			// save ID
			file.Write (m_id);
		}
		virtual void              SaveToOBJFile (BaseFile &file, unsigned int &base) const;	///< store Navigation Area to a "*.OBJ" file.
		virtual inline const bool Load (const BaseFile &file)
		{
			// load ID
			if (!file.Read (m_id))
				return false;

			return true;
		}

		inline const ID_t               GetBaseID        (void) const { return m_baseId; }		///< return this area's unique ID
		inline const ID_t               GetID            (void) const { return m_id; }			///< return derived area's unique ID

		inline void                     SetAttributes    (const NavAttributeFlag_t bits)       { m_attributeFlags = bits; }
		inline void                     AddAttributes    (const NavAttributeFlag_t bits)       { m_attributeFlags |= bits; }
		inline void                     RemoveAttributes (const NavAttributeFlag_t bits)       { m_attributeFlags &= ~bits; }
		inline const NavAttributeFlag_t GetAttributes    (void)                          const { return m_attributeFlags; }
		inline const bool               HasAttributes    (const NavAttributeFlag_t bits) const { return (m_attributeFlags & bits) > 0u; }
		inline const DynamicString      PrintAttributes  (void)                          const
		{
			DynamicString attributes;

			STORE_FLAGS_TO_STRING (m_attributeFlags, attributes, NavAttributeType_CROUCH, NavAttributeType_Total, NavAttributeStrings, "+");

			return attributes;
		}

		inline const bool IsOverlapping (const Math::Vector2D &pos) const	///< Return true if 'pos' is within 2D extents of area.
		{
			return m_extent.Contains (pos);
		}
		inline const bool IsOverlapping (const Math::Vector2D &pos, const float tolerance) const	///< Return true if 'pos' is within 2D extents of area.
		{
			return m_extent.Contains (pos, tolerance);
		}
		inline const bool IsOverlappingX (const float positionX) const			///< Return true if 'positionX' overlaps our X extent.
		{
			return positionX < m_extent.maxs.x && positionX > m_extent.mins.x;
		}
		inline const bool IsOverlappingX (const BaseNavigationArea *const area) const			///< Return true if 'area' overlaps our X extent.
		{
			return area->m_extent.mins.x < m_extent.maxs.x && area->m_extent.maxs.x > m_extent.mins.x;
		}
		inline const bool IsOverlappingY (const float positionY) const			///< Return true if 'positionY' overlaps our Y extent.
		{
			return positionY < m_extent.maxs.y && positionY > m_extent.mins.y;
		}
		inline const bool IsOverlappingY (const BaseNavigationArea *const area) const			///< Return true if 'area' overlaps our Y extent.
		{
			return area->m_extent.mins.y < m_extent.maxs.y && area->m_extent.maxs.y > m_extent.mins.y;
		}
		inline const bool IsOverlapping (const BaseNavigationArea *const area) const			///< Return true if 'area' overlaps our 2D extents.
		{
			return area->m_extent.IsIntersects2D (m_extent);
		}
		inline const bool IsTouching (const BaseNavigationArea *const area) const			///< Return true if 'area' touching our 2D extents.
		{
			return m_extent.IsTouching2D (area->m_extent);
		}

		inline const bool IsBlocked (void) const { return m_isBlocked; }
		virtual void CheckWaterLevel (void) =/*>*/ 0;
		inline const bool IsUnderwater (void) const { return m_isUnderwater; }

		//- for hunting algorithm ---------------------------------------------------------------------------
		inline void        SetClearedTimestamp (const TeamArrayID_t teamArrayID)       { m_clearedTimestamp[teamArrayID] = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime (); }						///< set this area's "clear" timestamp to now
		inline void        SetClearedTimestamp (const HalfLifeEngine::SDK::Constants::TeamID_t teamID)       { SetClearedTimestamp (static_cast <const TeamArrayID_t> (teamID - 1u)); }						///< set this area's "clear" timestamp to now
		inline const float GetClearedTimestamp (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const { return m_clearedTimestamp[teamID - 1u]; }				///< get time this area was marked "clear"

		//- "danger" ----------------------------------------------------------------------------------------
		inline void IncreaseDanger (const TeamArrayID_t teamArrayID, const float amount) { m_danger[teamArrayID].Increase (amount); }	///< increase the danger of this area for the given team
		inline void IncreaseDanger (const HalfLifeEngine::SDK::Constants::TeamID_t teamID, const float amount) { IncreaseDanger (static_cast <const TeamArrayID_t> (teamID - 1u), amount); }	///< increase the danger of this area for the given team
		inline const float GetDanger (const TeamArrayID_t teamArrayID) { return m_danger[teamArrayID]; }	///< return the danger of this area (decays over time)
		inline const float GetDanger (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) { return GetDanger (static_cast <const TeamArrayID_t> (teamID - 1u)); }	///< return the danger of this area (decays over time)

		//- extents -----------------------------------------------------------------------------------------
		inline const Math::Extent3D &GetExtent (void) const { return m_extent; }
		inline const Math::Vector3D &GetCenter (void) const { return m_center; }

		inline void SetPlace (const Place_t place) { m_place = place; }	///< set place descriptor
		inline const Place_t GetPlace (void) const { return m_place; }	///< get place descriptor

		const Math::Vector3D GetCorner (const NavCornerType_t corner) const;
		inline void GetCorners (Math::Vector3D corners[NUM_CORNERS]) const
		{
			corners[NORTH_WEST] = GetCorner (NORTH_WEST);
			corners[NORTH_EAST] = GetCorner (NORTH_EAST);
			corners[SOUTH_EAST] = GetCorner (SOUTH_EAST);
			corners[SOUTH_WEST] = GetCorner (SOUTH_WEST);
		}
		void SetCorner (const NavCornerType_t corner, const Math::Vector3D &newPosition);
		inline const Math::Vector3D GetDirectionVector (const NavDirType_t direction) const
		{
			switch (direction)
			{
				case NORTH:
					return GetCorner (NORTH_WEST).GetMidPoint (GetCorner (NORTH_EAST));

				case EAST:
					return GetCorner (NORTH_EAST).GetMidPoint (GetCorner (SOUTH_EAST));

				case SOUTH:
					return GetCorner (SOUTH_EAST).GetMidPoint (GetCorner (SOUTH_WEST));

				#if defined _DEBUG
					case WEST:
				#else	// if defined _DEBUG
					default:	// WEST
				#endif	// if !defined _DEBUG
					return GetCorner (SOUTH_WEST).GetMidPoint (GetCorner (NORTH_WEST));

				#if defined _DEBUG
					default:
						AddLogEntry (false, LogLevel_Critical, false, "BaseNavigationArea::GetDirectionVector(): Invalid type %u!", direction);

						return Math::Vector3D::ZeroValue;
				#endif	// if !defined _DEBUG
			}
		}
		inline void GetDirections (Math::Vector3D directions[NUM_DIRECTIONS]) const
		{
			directions[NORTH] = GetDirectionVector (NORTH);
			directions[EAST]  = GetDirectionVector (EAST);
			directions[SOUTH] = GetDirectionVector (SOUTH);
			directions[WEST]  = GetDirectionVector (WEST);
		}
		inline const Math::Vector3D ComputeNormal (void) const	///< Computes the area's normal based on m_extent.mins.
		{
			const Math::Vector3D u (m_extent.GetSizeX (), 0.0f, m_neZ - m_extent.mins.z);
			const Math::Vector3D v (0.0f, m_extent.GetSizeY (), m_swZ - m_extent.mins.z);

			return u.GetUnitCrossProduct (v);
		}
		inline const Math::Vector3D ComputeAlternateNormal (void) const	///< Computes the area's normal based on m_extent.maxs.
		{
			const Math::Vector3D u (m_extent.mins.x - m_extent.maxs.x, 0.0f, m_swZ - m_extent.maxs.z);
			const Math::Vector3D v (0.0f, m_extent.mins.y - m_extent.maxs.y, m_neZ - m_extent.maxs.z);

			return u.GetUnitCrossProduct (v);
		}

		virtual const bool Contains (const Math::Vector3D &pos) const;					///< return true if given point is on or above this area, but no others
		const bool IsCoplanar (const BaseNavigationArea *const area) const;				///< return true if this area and given area are approximately co-planar
		const float GetDistanceSquaredToPoint (const Math::Vector3D &pos) const;	///< return shortest distance between point and this area
		inline const bool IsDegenerate (void) const { return m_extent.mins.x >= m_extent.maxs.x || m_extent.mins.y >= m_extent.maxs.y; }							///< return true if this area is badly formed
		inline const bool IsRoughlySquare (void) const							///< Return true if area is more or less square. This is used when merging to prevent long, thin, areas being created.
		{
			const float aspect (m_extent.GetSizeX () / m_extent.GetSizeY ());
			const float maxAspect (3.01f);
			const float minAspect (1.0f / maxAspect);

			return aspect >= minAspect && aspect <= maxAspect;
		}
		inline const bool IsFlat (void) const									///< Return true if the area is approximately flat, using normals computed from opposite corners.
		{
			const Math::Vector3D normal (ComputeNormal ()), alternateNormal (ComputeAlternateNormal ());

			return (normal | alternateNormal) > Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_coplanar_slope_limit)->GetValue <float> ();
		}

		//- occupy time -------------------------------------------------------------------------------------
		inline const float GetEarliestOccupyTime (const TeamArrayID_t teamArrayIndex) const { return m_earliestOccupyTime[teamArrayIndex]; }			///< returns the minimum time for someone of the given team to reach this spot from their spawn
		inline const float GetEarliestOccupyTime (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const { return GetEarliestOccupyTime (static_cast <const TeamArrayID_t> (teamID - 1u)); }			///< returns the minimum time for someone of the given team to reach this spot from their spawn

		inline const bool IsBattlefront (void) const { return m_isBattlefront; }	///< true if this area is a "battlefront" - where rushing teams initially meet

		//- player counting ---------------------------------------------------------------------------------
		inline const PlayersArray &GetPlayers (const TeamArrayID_t teamArrayIndex)                    const { return m_players[teamArrayIndex]; }
		inline const PlayersArray &GetPlayers (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const { return GetPlayers (static_cast <const TeamArrayID_t> (teamID - 1u)); }
		inline void AddPlayer     (Client *const client);				///< add one player to this area's count
		inline void RemovePlayer  (Client *const client);				///< subtract one player from this area's count
		inline void RemovePlayers (void)								///< set the player count to zero
		{
			for (unsigned char index (TeamArrayID_Terrorist); index < TeamArrayID_Total; ++index)
				m_players[index].RemoveAll ();
		}
		inline const unsigned char GetPlayerCount (void) const		///< return number of players currently within this area
		{
			// sum all players
			return GetPlayerCount (TeamArrayID_Terrorist) + GetPlayerCount (TeamArrayID_CounterTerrorist);
		}
		inline const PlayersArray::IndexType_t GetPlayerCount (const TeamArrayID_t teamArrayIndex) const		///< return number of players of given team currently within this area
		{
			return m_players[teamArrayIndex].GetElementNumber ();
		}
		inline const PlayersArray::IndexType_t GetPlayerCount (const HalfLifeEngine::SDK::Constants::TeamID_t teamID) const		///< return number of players of given team currently within this area
		{
			return GetPlayerCount (static_cast <const TeamArrayID_t> (teamID - 1u));
		}

		const float GetZ (const Math::Vector2D &pos) const;						///< return Z of area at (x, y) of 'pos'
		const Math::Vector3D GetClosestPointOnArea (const Math::Vector2D &pos) const;	///< return closest point to 'pos' on this area - returned point in 'close'

		virtual const bool IsVisible (const Math::Vector3D &eye, Math::Vector3D *const visSpot = NULL) const =/*>*/ 0;	///< return true if area is visible from the given eyepoint, return visible spot

		virtual void UpdateBlocked (void) =/*>*/ 0;									///< Updates the (un)blocked status of the navigation area

		inline void Shift (const Math::Vector2D &offset)
		{
			m_extent.Shift (offset);

			m_center += offset;
		}
		inline void Shift (const Math::Vector3D &offset)
		{
			m_extent.Shift (offset);

			m_center += offset;
/*
			m_neZ += offset.z;
			m_swZ += offset.z;
*/
		}

		inline const Math::Vector3D GetRandomPoint (void) const
		{
			Math::Vector3D randomPosition
			(
				g_randomNumberGenerator.GetValueBetween (m_extent.mins.x, m_extent.maxs.x),
				g_randomNumberGenerator.GetValueBetween (m_extent.mins.y, m_extent.maxs.y),

				0.0f	// Not initialized yet.
			);

			randomPosition.z = GetZ (randomPosition);

			return randomPosition;
		}

		//- editing -----------------------------------------------------------------------------------------
		virtual void Draw               (void) const =/*>*/ 0;	///< Draws area and connections
		virtual void DrawConnectedAreas (void) const =/*>*/ 0;	///< Draws connected areas
		void         DrawPlayerCounts   (void) const;	///< Show current player counts on this area for debugging. NOTE: Assumes two teams.
		void         DrawOccupyTime     (void) const;
		void         DrawBattlefront    (void) const;
};

typedef DynamicArray <BaseNavigationArea *> NavBaseAreaArray;

#endif	// ifndef BASE_NAVIGATION_AREA_INCLUDED