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
// Hostage.hpp
//
// Class: Hostage
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_HOSTAGE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_HOSTAGE_INCLUDED

abstract_class IImprovEvent
{
	public:
		enum MoveToFailureType
		{
			/// @todo !!!
		};

	public:
		virtual void OnMoveToSuccess (const Math::Vector3D &)                    { /* Empty */ }//= 0;
		virtual void OnMoveToFailure (const Math::Vector3D &, MoveToFailureType) { /* Empty */ }//= 0;
		virtual void OnInjury        (float)                                     { /* Empty */ }//= 0;
};

class CImprov : public IImprovEvent
{
	public:
/*!off=4(1)*/		unsigned int m_UNKNOWN0;
/*!off=8(2)*/		unsigned int m_UNKNOWN1;
/*!off=12(3)*/		unsigned int m_UNKNOWN2;
/*!off=16(4)*/		unsigned int m_UNKNOWN3;
/*!off=20(5)*/		BaseEntity  *m_owner;

	public:
		virtual ~CImprov                              (void)                                                                                                     { /* VOID */ }

		virtual void IsAlive                          (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void MoveTo                           (const Math::Vector3D &)                                                                                   { /* Empty */ }//= 0;
		virtual void LookAt                           (const Math::Vector3D &)                                                                                   { /* Empty */ }//= 0;
		virtual void ClearLookAt                      (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void FaceTo                           (const Math::Vector3D &)                                                                                   { /* Empty */ }//= 0;
		virtual void ClearFaceTo                      (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void IsAtMoveGoal                     (float)                                                                                              const { /* Empty */ }//= 0;
		virtual void HasLookAt                        (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void HasFaceTo                        (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsAtFaceGoal                     (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsFriendInTheWay                 (const Math::Vector3D &)                                                                             const { /* Empty */ }//= 0;
		virtual void IsFriendInTheWay                 (BaseEntity *, const Math::Vector3D &)                                                               const { /* Empty */ }//= 0;
		virtual void MoveForward                      (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void MoveBackward                     (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void StrafeLeft                       (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void StrafeRight                      (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void Jump                             (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void Crouch                           (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void StandUp                          (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void TrackPath                        (const Math::Vector3D &, float)                                                                            { /* Empty */ }//= 0;
		virtual void StartLadder                      (const NavigationLadder *, NavTraverseType_t, const Math::Vector3D *, const Math::Vector3D *)              { /* Empty */ }//= 0;
		virtual void TraverseLadder                   (const NavigationLadder *, NavTraverseType_t, const Math::Vector3D *, const Math::Vector3D *, float)       { /* Empty */ }//= 0;
		virtual void GetSimpleGroundHeightWithFloor   (const Math::Vector3D *, float *, Math::Vector3D *)                                                        { /* Empty */ }//= 0;
		virtual void Run                              (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void Walk                             (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void Stop                             (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void GetMoveAngle                     (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void GetFaceAngle                     (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void GetFeet                          (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void GetCentroid                      (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void GetEyes                          (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsRunning                        (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsWalking                        (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsStopped                        (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsCrouching                      (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsJumping                        (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsUsingLadder                    (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsOnGround                       (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsMoving                         (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void CanRun                           (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void CanCrouch                        (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void CanJump                          (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void IsVisible                        (const Math::Vector3D &, bool)                                                                       const { /* Empty */ }//= 0;
		virtual void IsPlayerLookingAtMe              (BasePlayer *, float)                                                                                const { /* Empty */ }//= 0;
		virtual void IsAnyPlayerLookingAtMe           (int, float)                                                                                         const { /* Empty */ }//= 0;
		virtual void GetClosestPlayerByTravelDistance (int, float *)                                                                                       const { /* Empty */ }//= 0;
		virtual void GetLastKnownArea                 (void)                                                                                               const { /* Empty */ }//= 0;
		virtual void OnUpdate                         (float)                                                                                                    { /* Empty */ }//= 0;
		virtual void OnUpkeep                         (float)                                                                                                    { /* Empty */ }//= 0;
		virtual void OnReset                          (void)                                                                                                     { /* Empty */ }//= 0;
		virtual void OnGameEvent                      (Constants::GameEventType_t, BaseEntity *, BaseEntity *)                                                   { /* Empty */ }//= 0;
		virtual void OnTouch                          (BaseEntity *)                                                                                             { /* Empty */ }//= 0;
};

class HostageImprov : public CImprov
{
	public:
		template <typename elementType> class SimpleState
		{
			public:
/*!off=4(1)*/				elementType m_owner;	/// @warning I'M REALLY NOT SHURE ABOUT THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			public:
				virtual ~SimpleState               (void)                { /* Empty */ }//= 0;

				virtual void              OnEnter  (elementType)         { /* Empty */ }//= 0;
				virtual void              OnUpdate (elementType)         { /* Empty */ }//= 0;
				virtual void              OnExit   (elementType)         { /* Empty */ }//= 0;
				virtual const char *const GetName  (void)          const { return NULL; }//= 0;	// Pure virtual.
		};
		class HostageState : public SimpleState <HostageImprov *>, public IImprovEvent	/// @note THIS CLASS HAS MULTIPLE INHERITANCE!!!!
		{
			public:
				virtual void UpdateStationaryAnimation (HostageImprov *) { /* Empty */ }//= 0;
		};	// sizeof (HostageState) == 12 (3)
		class HostageStateMachine : public IImprovEvent
		{
			public:
/*!off=4(1)/56(14)*/				HostageState  *m_currentState;
/*!off=8(2)/60(15)*/				float          m_currentStateSetTimestamp;	// Time when 'm_currentState' was set.
/*!off=12(3)/64(16)*/				HostageImprov *m_owner;
		};	// sizeof (HostageStateMachine) == 16 (4)
		class HostageIdleState : public HostageState
		{
			public:
/*!off=4(1)/80(20)*/				unsigned int m_UNKNOWN0[15u];
				/// @todo !!!
		};	// sizeof (HostageIdleState) == 72 (18)
		class HostageEscapeState : public HostageState
		{
			public:
				/// @todo !!!
		};	// sizeof (HostageEscapeState) == 12 (3)
		class HostageEscapeToCoverState : public HostageState
		{
			public:
/*!off=4(1)/164(41)*/				unsigned int m_UNKNOWN0[7u];
				/// @todo !!!
		};	// sizeof (HostageEscapeToCoverState) == 40 (10)
		class HostageEscapeLookAroundState : public HostageState
		{
			public:
/*!off=4(1)/204(51)*/				unsigned int m_UNKNOWN0[2u];
				/// @todo !!!
		};	// sizeof (HostageEscapeLookAroundState) == 20 (5)
		class HostageRetreatState : public HostageState
		{
			public:
				/// @todo !!!
		};	// sizeof (HostageRetreatState) == 12 (3)
		class HostageFollowState : public HostageState
		{
			public:
/*!off=4(1)/264(66)*/				EntityHandle m_targetEntity;
/*!off=12(3)/272(68)*/				unsigned int m_UNKNOWN0[14u];
				/// @todo !!!
		};	// sizeof (HostageFollowState) == 76 (19)
		class HostageAnimateState : public HostageState
		{
			public:
				/// @todo !!!
		};	// sizeof (HostageAnimateState) == ?? (??)
		class CNavPathFollower
		{
			public:
				/// @todo !!!
		};	// sizeof (CNavPathFollower) == ?? (??)
		class CNavPath
		{
			public:
				enum { MaximumPathNodesNumber = 256u };
				struct PathInformation_t
				{
/*!off=0(0)/696(174)*/				NavigationArea    *area;
/*!off=3(1)/700(175)*/				NavTraverseType_t  traverseType;
/*!off=8(2)/704(176)*/				Math::Vector3D     point;
/*!off=20(5)/716(179)*/				NavigationLadder  *ladder;
				};	// sizeof (PathInformation_t) == 24 (6)

/*!off=0(0)/696(174)*/				PathInformation_t m_path[MaximumPathNodesNumber];
/*!off=6144(1536)/6840(1710)*/				unsigned int      m_currentPathInformationNumber;
		};	// sizeof (CNavPath) == 6148 (1537)

/*!off=24(6)*/		unsigned int                  m_UNKNOWN4[7u];
/*!off=52(13)*/		HostageStateMachine           m_stateMachine;
/*!off=68(17)*/		HostageIdleState              m_idleState;
/*!off=140(35)*/		HostageEscapeState            m_escapeState;
/*!off=152(38)*/		HostageEscapeToCoverState     m_escapeToCoverState;
/*!off=192(48)*/		HostageEscapeLookAroundState  m_escapeLookAroundState;
/*!off=212(53)*/		HostageStateMachine           m_stateMachine1;
/*!off=228(57)*/		unsigned int                  m_UNKNOWN5[3u];
/*!off=240(60)*/		HostageRetreatState           m_retreatState;
/*!off=252(63)*/		HostageFollowState            m_followState;
/*!off=328(82)*/		HostageAnimateState           m_animateState;

/*!off=696(174)*/		CNavPath                      m_path;
/*!off=6844(1711)*/		CNavPathFollower              m_pathFollower;

		/// @todo !!!
};	// sizeof (HostageImprov) == 7308

class Hostage : public BaseMonster
{
	//
	// Group: Private members.
	//
	public:
/*!off=384(96)*/		unsigned int    m_activity;
/*!off=388(97)*/		BOOL            m_isWasUsed;	// Is hostage was used? (Has a CT touched me?) (sets to true, when hostage once in this round used by CT, to give he 150$ and also bonus 100$ for CT team)
/*!off=392(98)*/		BOOL            m_isRescued;	// Is hostage rescued?
/*!off=396(99)*/		float           m_flinchTime;
/*!off=400(100)*/		float           m_nextUseTime;	// get time that the hostage will be next used. (delay is 1 second)
/*!off=404(101)*/		unsigned int    m_UNKNOWN0[3u];	// UNUSED?!?!?
/*!off=416(104)*/		float           m_nextSendHostagePositionMessageCheckTimestamp;	// (delay is 1 second)
/*!off=420(105)*/		BOOL            m_isStopped;	// (Warning: unused in CS: CZ!)
/*!off=424(106)*/		Math::Vector3D  m_spawnOrigin;	// Home position.
/*!off=436(109)*/		Math::Angles3D  m_spawnModelAngles;
/*!off=448(112)*/		unsigned int    m_UNKNOWN1[62u];

		class CLocalNav
		{
			public:
				enum { MaximumNodesNumber = 100u };
				struct Node_t
				{
/*!off=0(0)*/					Math::Vector3D position;
/*!off=12(3)*/					unsigned int   UNKNOWN0;
/*!off=16(4)*/					unsigned int   UNKNOWN1;
/*!off=20(5)*/					unsigned char  UNKNOWN2;
/*!off=24(6)*/					unsigned int   UNKNOWN3;
/*!off=28(7)*/					unsigned int   UNKNOWN4;
				};	// sizeof (Node_t) == 32 (8)

/*!off=4(1)*/				Hostage        *m_owner;

/*!off=8(2)*/				Edict          *m_traceIgnore;
/*!off=12(3)*/				BOOL            m_isTraceHitsIgnoreEdict;
/*!off=16(4)*/				Node_t         *m_nodes/*[MaximumNodesNumber]*/;
/*!off=20(5)*/				unsigned int    m_nodesNumber;
/*!off=24(6)*/				Math::Vector3D  m_goalOrigin;

			public:
				virtual inline ~CLocalNav (void) { /* VOID */ }
		};	// sizeof (CLocalNav) == 36 (9)

/*!off=696(174)*/		CLocalNav      *m_path;
/*!off=700(175)*/		int             m_furthestTraversableNode;	// (Can be -1)
/*!off=704(176)*/		Math::Vector3D  m_pathNodesToFollow[CLocalNav::MaximumNodesNumber];
/*!off=1904(476)*/		EntityHandle    m_previousTarget;			// Player who stopped the hostage.
/*!off=1912(478)*/		float           m_nextIdleThinkTime;
/*!off=1916(479)*/		float           m_UNKNOWN_Delay;
/*!off=1920(480)*/		float           m_UNKNOWN_Timestamp;		// (Used together with 'm_UNKNOWN_Delay')
/*!off=1924(481)*/		unsigned int    m_pathNodesToFollowNumber;
/*!off=1928(482)*/		BOOL            m_hasPath;
/*!off=1932(483)*/		float           m_lastUseTimestamp;			// get time that the hostage was last used. (Warning: unused in CS: CZ!)
/*!off=1936(484)*/		Math::Vector3D  m_lastOrigin;				// Used to check distance since last hostage movement to sent hostage position message for clients.
/*!off=1948(487)*/		unsigned int    m_ID;						// Unique id of a CS hostage.
/*!off=1952(488)*/		BOOL            m_isRescuerTooFar;			// Rescuer is too far away from hostage. (more than 200 units)
/*!off=1956(489)*/		float           m_rescuerLeftTimestamp;		// Time when the rescuer has kept away from hostage more than on 200 units. (Delay is 5 seconds)
/*!off=1960(490)*/		HostageImprov  *m_improvPointer;			// (Activated when map has navigation mesh)
/*!off=1964(491)*/		unsigned int    m_modelIndex;

	//
	// Group: Functions.
	//
	public:
/*		inline const bool IsValid      (void) const { return m_variables->takeDamageType == Constants::DAMAGE_YES; }
		inline const bool IsDead       (void) const { return m_variables->deadValue == Constants::DEAD_DEAD; }
		inline const bool IsFollowing  (const BaseEntity *const entity) const
		{
			if (!IsValid ())
				return false;

			if (m_improvPointer != NULL)
				return m_improvPointer->m_stateMachine.m_currentState == &m_improvPointer->m_followState && m_improvPointer->m_followState.m_targetEntity == entity;

			return !m_isStopped && m_hTargetEnt == entity;
		}
		inline const Math::Vector3D GetOrigin      (void) const { return GetEdict ()->GetBModelOrigin (); }
		inline Client *const        GetRescuer     (void) const
		{
			// Returns the rescuer of this hostage.

			// Reliability check.
			InternalAssert (IsValid ());

			const HalfLifeEngine::SDK::Classes::Edict *const rescuer (GetEdict ()->GetHostageRescuer ());

			// Set to not follow anything?
			return rescuer == NULL ? NULL : g_server->GetClientManager ()->GetClient (rescuer);
		}
		inline const float          GetNextUseTime (void) const { return GetEdict ()->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_HOSTAGE_NEXT_USE); }	// Returns time that the hostage will be next used.
		inline const float          GetLastUseTime (void) const { return GetEdict ()->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_HOSTAGE_LAST_USE); }	// Returns time that the hostage was last used.
*/};	// sizeof (Hostage) == 1968 (492)

inline       Hostage *const BaseEntity::GetHostagePointer (void)       { return static_cast <Hostage *const> (this); }
inline const Hostage *const BaseEntity::GetHostagePointer (void) const { return static_cast <const Hostage *const> (this); }

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_HOSTAGE_INCLUDED