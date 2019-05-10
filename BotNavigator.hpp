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
// BotNavigator.hpp
//
// Class: BaseNavigator
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef BOT_NAVIGATOR_INCLUDED
#define BOT_NAVIGATOR_INCLUDED

template <typename elementType> class ArrayIterator : public NoCopy
{
	protected:
		elementType *const m_array;

	public:
		inline const ArrayIterator (elementType *const array) : m_array (array) { /* VOID */ }

	public:
		inline       elementType *const &GetData   (void)       { return m_array; }
		inline const elementType *const &GetData   (void) const { return m_array; }

	public:
		inline       elementType &operator [] (const unsigned short index)       { return m_array[index]; }
		inline const elementType &operator [] (const unsigned short index) const { return m_array[index]; }
};
class SimplePointWrapper : public ArrayIterator <Math::Vector3D>
{
	public:
		inline const SimplePointWrapper (Math::Vector3D *const points) : ArrayIterator (points) { /* VOID */ }

	public:
		inline       void            SetPoint (const Math::Vector3D &point, const unsigned short index, const unsigned short/* pathIndex*/) { m_array[index] = point; }
		inline const Math::Vector3D &GetPoint (const unsigned short index) const { return operator [] (index); }
};
class SimplePortalWrapper : public ArrayIterator <const NavigationMesh::BaseNavigationArea::Connection::Portal3D_t>
{
	public:
		inline const SimplePortalWrapper (const NavigationMesh::BaseNavigationArea::Connection::Portal3D_t *const portals) : ArrayIterator (portals) { /* VOID */ }

	public:
		inline const NavigationMesh::BaseNavigationArea::Connection::Portal3D_t &GetPortal (const unsigned short index) const { return operator [] (index); }
};

class BaseNavigator : public SubSystem <YaPB>
{
	friend YaPB;

	friend Task_Normal;

	//
	// Group: Private constants.
	//
	private:
		enum Type_t
		{
			Type_Waypoint,
			Type_NavigationMesh,
			Type_Combined
		};

	//
	// Group: Constants.
	//
	public:
		enum Belief_t
		{
			BELIEF_NONE,

			BELIEF_DANGER,
			BELIEF_SAFETY
		};

		enum Constant_t
		{
			MAX_PATH_TICKS = 200u,

			VIEW_HISTORY_SIZE = 20u,
			VEL_HISTORY_SIZE  = 60u
		};

		// TODO: this may be too big of sizes (check memory on slower computers)
		static const float HISTORY_SAMPLING_RATE;

	//
	// Group: Protected members.
	//
	protected:
	public:
		struct Speed_t
		{
			float move;		// current forward/backward speed
			float strafe;	// current sideways speed

			inline Speed_t (const float inputMove = 0.0f, const float inputStrafe = 0.0f) : move (inputMove), strafe (inputStrafe) { /* VOID */ }

			inline void Reset (void) { move = strafe = 0.0f; }
		};

		Speed_t        m_speed;
		float          m_maximumSpeed;
		Math::Vector3D m_moveDirection;					// bot move direction
		Math::Angles2D m_moveAngles;					// bot move angles
		Math::Vector3D m_destinationOrigin;				// origin of move destination

		float          m_stuckCheckTime;
		float          m_duckTime;						// time to duck
		float          m_jumpTime;						// time last jump happened

		float          m_previousDistanceToClosestPathElementSquared2D;
		float          m_distanceToClosestPathElementCheckTime;

		float          m_prevCheckTime;					// time previously checked movement speed (date under which the bot won't check for being stuck at all)
		Speed_t        m_prevSpeed;						// speed some frames before
		Math::Vector3D m_previousOrigin;				// origin some frames before (previous bot location (for stuck checking))
		float          m_movedDistanceSquared;			// moved distance during one frame
		float          m_stuckAmount;					// how "stuck" the bot is, >= 0.5 = stuck
		float          m_averageVelocity;				// average velocity of this bot over the past 0.5 seconds
		unsigned int   m_averageVelocityFramesCount;	// number of frames the above average elapsed over
		Math::Vector3D m_stuckOrigin;
		float          m_stuckTimestamp;
		bool           m_isStucked;						// set to TRUE if the bot is not moving as fast as it would like to
		static const unsigned char MaximumStuckTable = 10u;
		unsigned char  m_distancesToPreviousOriginSinceStuckCurrentIndex0;
		unsigned char  m_distancesToPreviousOriginSinceStuckCurrentIndex1;
		float          m_distancesToPreviousOriginSinceStuck[MaximumStuckTable];

		float          m_nextClearFailedGoalsTime;

		bool           m_doMovementToGoal;

		float          m_currentPathIndexSetTimestamp;

	// movement
	float movement_time;
	Math::Angles2D prev_v_angle[VIEW_HISTORY_SIZE];
	Math::Vector3D prev_velocity[VEL_HISTORY_SIZE];

	// steering
	Math::Vector3D desiredVelocity;

	Math::Angles2D smoothLook (void);
	Math::Angles2D smoothVelocity (void);

	//
	// Group: (Con/De)structors.
	//
	public:
		inline BaseNavigator (YaPB *const bot) :
			SubSystem <YaPB> (bot),

			m_maximumSpeed (0.0f),

			m_moveDirection (Math::Vector3D::ZeroValue),
			m_moveAngles (Math::Angles2D::ZeroValue),
			m_destinationOrigin (Math::Vector3D::ZeroValue),

			m_stuckCheckTime (0.0f),
			m_duckTime (0.0f),
			m_jumpTime (0.0f),

			m_previousDistanceToClosestPathElementSquared2D (0.0f),
			m_distanceToClosestPathElementCheckTime (0.0f),

			m_prevCheckTime (0.0f),
			m_movedDistanceSquared (0.0f),
			m_stuckAmount (0.0f),
			m_averageVelocity (0.0f),
			m_averageVelocityFramesCount (0u),
			m_stuckTimestamp (0.0f),
			m_isStucked (false),
			m_distancesToPreviousOriginSinceStuckCurrentIndex0 (0u),
			m_distancesToPreviousOriginSinceStuckCurrentIndex1 (0u),

			m_nextClearFailedGoalsTime (0.0f),

			m_doMovementToGoal (false),

			m_currentPathIndexSetTimestamp (0.0f)
		{ /* VOID */ }

		virtual inline ~BaseNavigator (void) { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		virtual inline const Type_t GetType (void) const = 0;

/*		void FixMovement( CUserCmd* pUserCmd, Math::Angles3D& realva )
		{
			// Movement
			Math::Vector3D vecMove( pUserCmd->forwardmove, pUserCmd->sidemove, pUserCmd->upmove );
			float speed = sqrt( vecMove.x*vecMove.x + vecMove.y*vecMove.y );
			Math::Angles3D angMove;
			VectorAngles( vecMove, angMove );

			// Corrected yaw we want to move at
			float yaw = DEG2RAD( pUserCmd->viewangles.y - realva.y + angMove.y );

			// Adjust move
			pUserCmd->forwardmove = cos( yaw ) * speed;
			pUserCmd->sidemove = sin( yaw ) * speed;
		}*/
		inline void PressValidMoveButtonsAndCorrectSpeed (void)
		{
			// Press valid move buttons!
			if (!(m_owner->m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::ButtonBits_InMove))
				return;

			const float yawAngle (m_owner->GetFieldOfView (m_moveAngles.yaw));
			const float radians (Math::DegreeToRadian (yawAngle));	// degrees to radians
			Math::Vector2D move;

			Math::GetCosineSine (radians, move.x, move.y);

			const Speed_t speed (move.x, move.y);

			InternalAssert (move.AreEqual (move.GetNormalized ()));
/*			const Math::Vector2D moveNormalized (move.GetNormalized ());
			if (!move.AreEqual (moveNormalized))
				AddLogEntry (false, LogLevel_Critical, true, "move[%.10f, %.10f] NOT EQUAL ([%.10f, %.10f]moveNormalized!!! [%.10f, %.10f]", move.x, move.y, moveNormalized.x, moveNormalized.y, Math::fabsf (move.x - moveNormalized.x), Math::fabsf (move.y - moveNormalized.y));
*/
			// Reliability check.
			InternalAssert (speed.move != 0.0f || speed.strafe != 0.0f);

			// Release move buttons....
			m_owner->m_edict->variables.buttons &= ~HalfLifeEngine::SDK::Constants::ButtonBits_InMove;

			if (speed.move > Math::RawEpsilon)
				m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_FORWARD;
			else if (speed.move < -Math::RawEpsilon)
				m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_BACK;

			if (speed.strafe > Math::RawEpsilon)
				m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_MOVELEFT;
			else if (speed.strafe < -Math::RawEpsilon)
				m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_MOVERIGHT;
/*! SPEED IS WRONG :(
			// set the move independant movement....

			// speed.move is percentage (-1 to 1) of forward speed,
			// speed.strafe is percentage (-1 to 1) of side speed.
			m_speed.move = m_maximumSpeed * speed.move;

			if (m_strafeTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				m_speed.strafe = m_maximumSpeed * -speed.strafe;	// side speed
*/
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool IsWaypoint       (void) const { return GetType () == Type_Waypoint; }
		inline const bool IsNavigationMesh (void) const { return GetType () == Type_NavigationMesh; }
		inline const bool IsCombined       (void) const { return GetType () == Type_Combined; }

		inline const Speed_t &GetSpeed (void) const { return m_speed; }

		#if defined _DEBUG
			virtual inline void DrawCurrentRoute (void) const = 0;
		#endif	// if defined _DEBUG

		virtual const bool BuildRoute (BaseNavigationElement *const/* destination*/) { return false; }
		virtual const bool BuildRoute (const Math::Vector3D &/*destination*/) { return false; }

		inline void SetDestinationOrigin (const Math::Vector3D &destinationOrigin) { m_destinationOrigin = destinationOrigin; }

		virtual const bool SetMoveTo    (const Math::Vector3D &destination) = 0;
		virtual const bool SetMoveTo    (BaseNavigationElement *const destination) = 0;

		virtual const bool HasRoute (void) const = 0;
		virtual void ClearRoute (void) = 0;

		virtual inline void                               SetGoal     (BaseNavigationElement *const newGoalArea)       = 0;
		virtual inline const BaseNavigationElement *const GetGoal     (void)                                     const = 0;

		virtual inline const bool GoalIsValid (void) const = 0;

		inline void MoveForward (void)
		{
			m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_FORWARD;
//			m_speed.move = m_maximumSpeed;
		}
		inline void MoveBack (void)
		{
			m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_BACK;
//			m_speed.move = -m_maximumSpeed;
		}
		inline void StrafeLeft (void)
		{
			m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_MOVELEFT;
//			m_speed.strafe = -m_maximumSpeed;
		}
		inline void StrafeRight (void)
		{
			m_owner->m_edict->variables.buttons |= HalfLifeEngine::SDK::Constants::IN_MOVERIGHT;
//			m_speed.strafe = m_maximumSpeed;
		}
		inline void Jump (void)
		{
			m_owner->PressJump ();

			if (m_owner->IsOnFloor ())
				m_jumpTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}
		inline void Duck (void)
		{
			m_owner->PressDuck ();

			if (!m_owner->IsDucking ())
				m_duckTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}

		inline void ResetMovement (void)
		{
			// reset all buttons pressed...
			m_owner->m_edict->variables.buttons = HalfLifeEngine::SDK::Constants::ButtonType_None;
			m_owner->m_edict->variables.impulse = HalfLifeEngine::SDK::Constants::PlayerImpulseID_None;

			m_speed.Reset ();
		}

		inline const float GetMaximumSpeed (void) const { return m_maximumSpeed; }
		inline const float GetStuckAmount (void) const { return m_stuckAmount; }
		inline const bool IsStuck (const float amount = 0.5f) const { return GetStuckAmount () >= amount; }
		void CheckStuck (void);
		inline void ResetStuckMonitor (void)
		{
/*			if (m_isStucked && cv_bot_debug.value > 0.0 && IsLocalPlayerWatchingMe ())
				Globals::g_halfLifeEngine->EmitSound (GetEdict (), Constants::SoundChannel_Item, "buttons/bell1.wav");
*/
			m_isStucked = false;
			m_stuckTimestamp = 0.0f;
//			m_wiggleJumpTimestamp = 0.0f;
			m_distancesToPreviousOriginSinceStuckCurrentIndex0 = 0u;
			m_distancesToPreviousOriginSinceStuckCurrentIndex1 = 0u;
			m_currentPathIndexSetTimestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}

		virtual void MoveTowardsPath (void) = 0;

	//
	// Group: Engine callbacks.
	//
	public:
		virtual void Think          (void);
		virtual void AliveSpawnPost (void);
};
#if 0
class Navigator_Waypoint : public BaseNavigator
{
	//
	// Group: Private members.
	//
	private:
		Waypoint            *m_currentWaypoint;		// some info about current bot waypoint
		Waypoint            *m_goalWaypoint;
		Waypoint            *m_lastFailedWaypoint;

		Stack <Waypoint *>   m_currentRoute;

		AStarNode <Waypoint> m_paths[MaximumWaypoints];

		DynamicArray <Waypoint *>   m_failedGoals;

		float                m_belief[MaximumWaypoints];
/*
		typedef DynamicArray <AStarNode <Waypoint> *> AStarListArray_t;

		AStarListArray_t m_openList;
		AStarListArray_t m_closedList;
*/
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Navigator_Waypoint (YaPB *const bot) :
			BaseNavigator (bot),

			m_currentWaypoint (NULL),
			m_goalWaypoint (NULL),
			m_lastFailedWaypoint (NULL)
		{
			for (unsigned short index (0u); index < MaximumWaypoints; ++index)
				m_belief[index] = 0.0f;
		}

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_Waypoint; }

	//
	// Group: Functions.
	//
	public:
//		inline const Math::Vector3D &GetGoalOrigin (void) const { return m_vGoal; }

		// returns true when working out route finishes, not if successful
		const bool BuildRoute (const Waypoint *const source, const Waypoint *const destination, PathNodeArray_t &route);

		inline const bool GetNextRoutePointOrigin (Math::Vector3D &point) const
		{
			if (!m_currentRoute.IsEmpty ())
			{
				InternalAssert (m_currentRoute.GetFirstElement () != NULL);

				point = m_currentRoute.GetFirstElement ()->GetOrigin ();

				return true;
			}

			return false;
		}

		inline const Math::Vector3D &GetNextPoint (void) const { InternalAssert (m_currentWaypoint->IsValid ()); return m_currentWaypoint->GetOrigin (); }	// return the vector of the next point

		void UpdatePosition (void);	// update the bots current walk vector

		inline void FailMove (void)
		{
			m_lastFailedWaypoint = m_currentWaypoint;

			if (!m_failedGoals.IsMember (m_goalWaypoint))
			{
				m_failedGoals += m_goalWaypoint;
				m_nextClearFailedGoalsTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (8.0f, 30.0f);
			}
		}

		inline const bool IsHasNextPoint (void) const { return m_currentWaypoint->IsValid (); }	// if bot has a current position to walk to return the boolean

		inline const bool CanGetTo (const Math::Vector3D &origin) const
		{
			return true;//m_failedGoals.IsMember (g_server->GetWaypointManager ().GetLocationsTable ().GetNearestWaypoint (origin, 100.0f, InvalidWaypointIndex, true, true, NULL, false, m_owner->GetRealTeam ()));
		}

		inline const bool HasRoute (void) const { return !m_currentRoute.IsEmpty (); }

		void rollBackPosition (void);

		void open (AStarNode <Waypoint> *pNode);	// AStar Algorithm: open a waypoint

		AStarNode <Waypoint> *const nextNode (void);	// AStar Algorithm: get the waypoint with lowest cost

		const float distanceTo (const Math::Vector3D &origin)
		{
/*			if (!m_currentWaypoint->IsValid ())
				m_currentWaypoint = g_server->GetWaypointManager ().GetLocationsTable ().GetNearestWaypoint (m_owner->GetOrigin (), WaypointLocationsTable::MaximumReachableRange, InvalidWaypointIndex, true, true, NULL, false, m_owner->GetRealTeam ());

			if (m_currentWaypoint->IsValid ())
			{
				const unsigned short goal = g_server->GetWaypointManager ().GetLocationsTable ().GetNearestWaypoint (origin, WaypointLocationsTable::MaximumReachableRange, InvalidWaypointIndex, true, true, NULL, false, m_owner->GetRealTeam ());

				if (goal != InvalidWaypointIndex)
					return g_server->GetWaypointManager ().GetDistancesTable ().GetPathDistance (m_currentWaypoint, goal);
			}

			return m_owner->distanceFrom (origin);*/
			return 0.0f;
		}

		const float distanceTo (const Waypoint *const waypoint);

		const Math::Vector3D getCoverOrigin (Math::Vector3D vCover);

		void clearOpenList (void);

		//void goBack();

		void belief (Math::Vector3D origin, Math::Vector3D facing, float fBelief, float fStrength, Belief_t iType);

		// nearest cover position to vOrigin only
		const bool getCoverPosition (const Math::Vector3D &vCoverOrigin, Math::Vector3D &vCover);
		// nearest cover postion to both vectors
		const bool getHideSpotPosition (Math::Vector3D vCoverOrigin, Math::Vector3D *vCover);

		inline void GetFailedGoals (DynamicArray <Waypoint *> &goals) { goals = m_failedGoals; }

	//
	// Group: Engine callbacks.
	//
	public:
		void Think          (void);
		void AliveSpawnPost (void);
};
#endif	// if 0
class Navigator_NavigationMesh : public BaseNavigator
{
	//
	// Group: Constants.
	//
	public:
		enum Constant_t
		{
			MaximumPathNodesNumber = 512u
		};

	//
	// Group: Type definitions.
	//
	public:
		struct PathNode_t
		{
//			enum { MaximumPoints = 20u };
//			typedef StaticArray <Math::Vector3D, unsigned char, MaximumPoints> PointsArray_t;

			NavigationMesh::BaseNavigationArea                         *area;
			NavigationMesh::NavTraverseType_t                           traverseType;
//			PointsArray_t                                               points;
			Math::Vector3D                                              point;

			NavigationMesh::BaseNavigationArea::Connection::Portal3D_t  portal;
		};
		typedef StaticArray <PathNode_t, unsigned short, MaximumPathNodesNumber> PathNodeArray_t;

		struct PathPoint_t
		{
			PathNodeArray_t::IndexType_t prevNodeIndex;
			PathNodeArray_t::IndexType_t nextNodeIndex;

			Math::Vector3D               point;
		};
		typedef StaticArray <PathPoint_t, PathNodeArray_t::IndexType_t, PathNodeArray_t::MaximumPossibleSize> PathPointArray_t;

class PointWrapper : public ArrayIterator <PathPoint_t>
{
	public:
		inline const PointWrapper (PathPoint_t *const points) : ArrayIterator (points) { /* VOID */ }

	public:
		inline       void            SetPoint (const Math::Vector3D &point, const unsigned short index, const unsigned short pathIndex)
		{
			m_array[index].prevNodeIndex = pathIndex - 1u;
			m_array[index].nextNodeIndex = pathIndex;
			m_array[index].point = point;
		}
		inline const Math::Vector3D &GetPoint (const unsigned short index) const { return m_array[index].point; }
};
class PortalWrapper : public ArrayIterator <PathNode_t>
{
	public:
		inline const PortalWrapper (PathNode_t *const portals) : ArrayIterator (portals) { /* VOID */ }

	public:
		inline const NavigationMesh::BaseNavigationArea::Connection::Portal3D_t &GetPortal (const unsigned short index) const { return m_array[index].portal; }
};
		template <typename portalFunctor, typename pointFunctor> static const unsigned short stringPull (const portalFunctor &portals, const unsigned short nportals, pointFunctor &pts, const unsigned short maxPts);

	//
	// Group: Private members.
	//
	private:
		PathNodeArray_t               m_path;			// Current path information. (current route)
		PathNodeArray_t::IndexType_t  m_currentPathIndex;
		PathPointArray_t              m_pathPoints2;		// Parallel array of points of straight path, computed by simple stupid funnel algorithm.
		PathPointArray_t::IndexType_t m_currentPathPointIndex;

		NavigationMesh::BaseNavigationArea::Connection::Portal3D_t m_pathPortals[MaximumPathNodesNumber];
		unsigned short m_pathPortalsNumber;
		Math::Vector3D m_pathPoints[MaximumPathNodesNumber];
		unsigned short m_pathPointsNumber;
/*
		typedef DynamicArray <AStarNode <NavigationMesh::NavigationArea> *> AStarListArray_t;

		AStarListArray_t m_openList;
		AStarListArray_t m_closedList;
*/
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Navigator_NavigationMesh (YaPB *const bot) :
			BaseNavigator (bot),

			m_currentPathIndex (0u),
			m_currentPathPointIndex (0u),

			m_pathPortalsNumber (0u),
			m_pathPointsNumber (0u)
		{ /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_NavigationMesh; }

	//
	// Group: Functions.
	//
	public:
		#if defined _DEBUG
			inline void DrawCurrentRoute (void) const
			{
				if (!HasRoute ())
					return;

				for (unsigned short index (1u); index < m_path.GetElementNumber (); ++index)
				{
					// ...now draw line from source to destination...
					HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (m_path[index - 1u].area->GetCenter (), m_path[index].area->GetCenter (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 15u, 0u, Color <> (255u, 100u, 55u), 200u, 5u, 1u);
/*
					const Math::Vector3D point (node->GetData ()->GetClosestPointInPortal3D (node->GetNext ()->GetData (), NavigationMesh::GetOppositeDirection (static_cast <const NavigationMesh::NavDirType_t> (node->GetData ()->GetParentHow ())), node->GetNext ()->GetNext () == NULL ? Math::Vector3D::ZeroValue/*TEMP*//* : node->GetNext ()->GetNext ()->GetData ()->GetCenter ()));

					HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (point, node->GetNext ()->GetData ()->GetCenter (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 15u, 0u, Color <> (255u, 100u, 55u), 200u, 5u, 1u);
*/
				}
			}
		#endif	// if defined _DEBUG

		const bool BuildRoute (BaseNavigationElement *const destination);
		const bool BuildRoute (const Math::Vector3D &destination);
		const bool BuildRoute (NavigationMesh::NavigationArea *const source, NavigationMesh::NavigationArea *const destination, const Math::Vector3D *const destinationVector, PathNodeArray_t &route);
		const bool BuildRoute (NavigationMesh::NavigationArea *const source, NavigationMesh::NavigationArea *const destination, PathNodeArray_t &route);
		const bool BuildRoute (NavigationMesh::NavigationArea *const source, const Math::Vector3D &destination, PathNodeArray_t &route);
		inline const bool RebuildCurrentRoute (void)
		{
			NavigationMesh::NavigationArea *const destinationArea (m_path.GetLastElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ());

			return BuildRoute (destinationArea);
		}

		void SetPathIndex (const PathNodeArray_t::IndexType_t newPathIndex);
		void BuildTrivialPath (const Math::Vector3D &destination, PathNodeArray_t &route);
		void BuildTrivialPath (const Math::Vector3D &destination);
		const bool ComputePathPositions (PathNodeArray_t &route);
		inline const bool ComputePathPositions (void) { return ComputePathPositions (m_path); }

		inline const bool SetMoveTo (const Math::Vector3D &destination)
		{
			if (m_owner->m_lastNavigationArea == NULL)
				return false;

			m_doMovementToGoal = true;

			SetDestinationOrigin (destination);

			const bool result (BuildRoute (destination));

			if (result)
			{
				NavigationMesh::NavigationArea *const goal (m_path.GetFirstElement ().area->GetAreaPointer <NavigationMesh::NavigationArea> ());

				m_previousDistanceToClosestPathElementSquared2D = m_owner->GetOrigin ().GetDistanceSquared2D (goal->GetCenter ());
			}

			return result;
		}
		inline const bool SetMoveTo (BaseNavigationElement *const destination)
		{
			if (m_owner->m_lastNavigationArea == NULL)
				return false;

			m_doMovementToGoal = true;

			SetDestinationOrigin (destination->GetAreaPointer <NavigationMesh::NavigationArea> ()->GetCenter () + m_owner->m_edict->variables.viewOffset * 2.0f);

			const bool result (BuildRoute (destination));

			if (result)
				m_previousDistanceToClosestPathElementSquared2D = m_owner->GetOrigin ().GetDistanceSquared2D (m_path.GetFirstElement ().area->GetCenter ());

			return result;
		}

		inline const bool HasRoute (void) const { return !m_path.IsEmpty (); }
		inline void ClearRoute (void) { m_path.RemoveAll (); }

		inline void                               SetGoal (BaseNavigationElement *const newGoalArea) { SetMoveTo (newGoalArea); }
		inline const BaseNavigationElement *const GetGoal (void) const { return !HasRoute () ? NULL : m_path.GetLastElement ().area; }

		inline const bool GoalIsValid (void) const
		{
			// not decided about a goal or no path calculated
			return HasRoute ();
/*			if (!HasRoute ())
				return false;

			// no nodes needed
			if (m_path.GetLastElement ().area == m_owner->m_lastNavigationArea)
				return true;

			// got path - check if still valid
			return m_currentPathIndex == m_path.GetElementNumber ();*/
		}

		void MoveTowardsPath (void);

	//
	// Group: Engine callbacks.
	//
	public:
		void Think (void);
};
#if 0
class Navigator_Combined : public BaseNavigator/*public Navigator_Waypoint, Navigator_NavigationMesh*/
{
	//
	// Group: Private members.
	//
	private:
		//

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Navigator_Combined (YaPB *const bot) : BaseNavigator (bot)/*Navigator_Waypoint (bot), Navigator_NavigationMesh (bot)*/ { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_Combined; }

	//
	// Group: Functions.
	//
	public:
		//

	//
	// Group: Engine callbacks.
	//
	public:
		//
};
#endif	// if 0
#endif	// ifndef BOT_NAVIGATOR_INCLUDED