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
// NavigationLadder.hpp
//
// Class: NavigationLadder
//
// Description: AI Navigation ladders.
//				The NavigationLadder represents ladders in the Navigation Mesh, and their connections to adjacent Navigation Areas.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_LADDER_INCLUDED
#define NAVIGATION_LADDER_INCLUDED

//--------------------------------------------------------------------------------------------------------------
/*** @todo Deal with ladders that allow jumping off to areas in the middle*/
class NavigationLadder : public BaseNavigationArea
{
	friend NavigationMesh;
	friend NavigationArea;

	//
	// Group: Constants.
	//
	public:
		enum Direction_t
		{
			Direction_Up,
			Direction_Down,

			Direction_Both,

			Direction_Total
		};

		enum ConnectionType_t			///< Ladder connection directions, to facilitate iterating over connections
		{
			///< the areas at the top of the ladder
			ConnectionType_TopBehind,	/// @note For gap areas this is forward connection type!
			ConnectionType_TopLeft,
			ConnectionType_TopRight,

			ConnectionType_GapTotal,

			ConnectionType_TopForward = ConnectionType_GapTotal,	///< area at top of ladder "ahead" it - only useful for descending

			ConnectionType_Bottom,		///< the area at the bottom of the ladder

			ConnectionType_Total
		};

	//
	// Group: Private members.
	//
	private:
		NavigationArea *m_connectedAreas[ConnectionType_Total];
//		NavAreaConnectArray m_connectedGapAreas;	///< areas connected between bottom and top of ladder
		unsigned char m_connectedGapAreasNumber;	///< the number of areas connected between bottom and top of ladder
		typedef NavigationArea *ConnectedAreas_t[ConnectionType_GapTotal];
		ConnectedAreas_t *m_connectedGapAreas;	///< areas connected between bottom and top of ladder

		Math::Vector3D  m_top;			///< world coords of the top of the ladder
		Math::Vector3D  m_bottom;		///< world coords of the bottom of the ladder
		float           m_length;		///< the length of the ladder
		float           m_width;		///< the width of the ladder

		NavDirType_t    m_direction;	///< which way the ladder faces (i.e.: surface normal of climbable side)
		Math::Vector3D  m_normal;		///< surface normal of the ladder surface (or Math::Vector3D-ized m_direction, if the TraceLine() fails)

		HalfLifeEngine::SDK::Classes::Edict *m_edict;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline NavigationLadder (void);	// Note: This function declared in Server.hpp.
		NavigationLadder (HalfLifeEngine::SDK::Classes::Edict *const ladderEdict);
		NavigationLadder (const Math::Vector3D &topEdge, const Math::Vector3D &bottomEdge, const Math::Vector3D &leftEdge, const Math::Vector3D &rightEdge, const Math::Vector2D &direction);

		virtual inline ~NavigationLadder (void) { delete [] m_connectedGapAreas; }

	//
	// Group: Functions.
	//
	public:
		static inline HalfLifeEngine::SDK::Classes::Edict *const FindEdict (const Math::Vector3D &center)
		{
			float minimumDistanceSquared (MATH_GET_SQUARED (HalfLifeEngine::SDK::Constants::MapSize));
			HalfLifeEngine::SDK::Classes::Edict *closestLadderEdict (NULL);

			for (HalfLifeEngine::SDK::Classes::Edict *ladderEdict (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_ladder")); ladderEdict->IsValid (); ladderEdict = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ladderEdict, "func_ladder"))
			{
				const float distanceSquared (ladderEdict->GetCenter ().GetDistanceSquared (center));

				if (distanceSquared >= minimumDistanceSquared)
					continue;

				minimumDistanceSquared = distanceSquared;
				closestLadderEdict = ladderEdict;
			}

			return closestLadderEdict;
		}

		inline const bool IsNavigationLadder (void) const { return true; }

		void Save (BaseFile &file) const;
		const bool Load (const BaseFile &file);

		const Math::Vector3D GetPosAtHeight (const float height) const;	///< Compute x, y coordinate of the ladder at a given height

		inline const bool IsConnected (const NavigationArea *const area) const	///< returns true if given area is connected with ladder
		{
			for (unsigned char direction (ConnectionType_TopBehind); direction < ConnectionType_Total; ++direction)
				if (m_connectedAreas[direction] == area)
					return true;
/*
			for (unsigned char index (0u); index < m_connectedGapAreasNumber; ++index)
				for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_GapTotal; ++connectionType)
					if (m_connectedGapAreas[index][connectionType] == area)
						return true;
*/
			return false;
		}
		inline const bool IsConnected2 (NavigationArea *&iarea) const	///< returns true if given area is connected with ladder
		{
			NavigationArea *const area (iarea);

			iarea = NULL;

			for (unsigned char index (0u); index < m_connectedGapAreasNumber; ++index)
				for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_GapTotal; ++connectionType)
					if (m_connectedGapAreas[index][connectionType] == area)
					{
						iarea = area;

						return true;
					}

			iarea = area;

			return false;
		}
		inline const bool IsConnected (const NavigationArea *const area, const Direction_t direction) const	///< returns true if given area is connected in given direction
		{
			// Reliability check.
			InternalAssert (direction < Direction_Total);

			switch (direction)
			{
				case Direction_Up:
					for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_Bottom; ++connectionType)
						if (m_connectedAreas[connectionType] == area)
							return true;
/*
					for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_GapTotal; ++connectionType)
						if (m_connectedGapAreas[m_connectedGapAreasNumber - 1u][connectionType] == area)
							return true;
*/
					return false;

				case Direction_Down:
					return m_connectedAreas[ConnectionType_Bottom] == area;
/*
					for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_GapTotal; ++connectionType)
						if (m_connectedGapAreas[0u][connectionType] == area)
							return true;

					return false;
*/
				default:	// Direction_Both
					for (unsigned char index (1u); index < m_connectedGapAreasNumber; ++index)
						for (unsigned char connectionType (ConnectionType_TopBehind); connectionType < ConnectionType_GapTotal; ++connectionType)
							if (m_connectedGapAreas[index][connectionType] == area)
								return true;

					return false;
			}
		}

		void ConnectGeneratedLadder (void);			///< Connect a generated ladder to navigation areas at the end of navigation generation

		void ConnectTo (NavigationArea *const area);				///< connect this ladder to given area
		void Disconnect (const NavigationArea *const area);				///< disconnect this ladder from given area

		void OnSplit (const NavigationArea *const original, NavigationArea *const alpha, NavigationArea *const beta);	///< when original is split into alpha and beta, update our connections
		inline void OnDestroyNotify (const NavigationArea *const dead) { Disconnect (dead); }			///< Invoked when given area is going away

		void Draw               (void) const;				///< Draws ladder and connections
		void DrawConnectedAreas (void) const;				///< Draws connected areas

		void UpdateDangling (void);					///< Checks if the ladder is dangling (bots cannot go up)

		const bool IsInUse (void) const;						///< return true if someone is on this ladder
		const bool IsInUse (const Client *const ignore) const;	///< return true if someone is on this ladder (other than 'ignore')

		inline void CheckWaterLevel (void) { m_isUnderwater = HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (m_center) == HalfLifeEngine::SDK::Constants::Content_Water; }
		inline void UpdateBlocked (void) { m_isBlocked = false; }
		inline const bool IsVisible (const Math::Vector3D &/*eye*/, Math::Vector3D *const/* visSpot*/ = NULL) const { return false; }	/// @todo !!!!!

		void SetDirection (const NavDirType_t direction);

		inline void ReverseTopConnectedAreas (void)
		{
			SwapElements (m_connectedAreas[ConnectionType_TopBehind], m_connectedAreas[ConnectionType_TopForward]);
			SwapElements (m_connectedAreas[ConnectionType_TopLeft], m_connectedAreas[ConnectionType_TopRight]);
		}

		inline void                   SetConnectedArea (const ConnectionType_t connectionType, NavigationArea *const area) { m_connectedAreas[connectionType] = area; }
		inline NavigationArea *const  GetConnectedArea (const ConnectionType_t connectionType) const { return m_connectedAreas[connectionType]; }
		inline const Math::Vector3D  &GetTop           (void)                                  const { return m_top; }
		inline const Math::Vector3D  &GetBottom        (void)                                  const { return m_bottom; }
		inline const float            GetLength        (void)                                  const { return m_length; }
		inline const float            GetWidth         (void)                                  const { return m_width; }
		inline const NavDirType_t     GetDirection     (void)                                  const { return m_direction; }
		inline const Math::Vector3D  &GetNormal        (void)                                  const { return m_normal; }
		inline const Math::Vector3D  &GetCenter        (void)                                  const { return m_center; }
};

typedef DynamicArray <NavigationLadder *> NavLadderArray;

class NavigationLadder_Thinkable : public NavigationLadder
{
	//
};

#endif	// ifndef NAVIGATION_LADDER_INCLUDED