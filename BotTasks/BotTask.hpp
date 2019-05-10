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
// BotTask.hpp
//
// Class: Task
//
// Description: Base bot task class.
//
// Note: First of all bot choose Task_Normal, then if some shit occurs,
//       his choose other task/schedule by situation and
//       already previous task - i.e. Task_Normal goes to currentTask->m_next pointer.
//       This is made to choose previous task after current task fail, or success.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_TASK_INCLUDED
#define BOT_TASK_INCLUDED

class AINetwork
{
	public:
// AI Node class
class AINode : public SubSystem <YaPB>
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline AINode (YaPB *const bot) : SubSystem <YaPB> (bot) { /* VOID */ }

		virtual inline ~AINode (void) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		virtual void       OnEnter (void) { /* Nothing, used by sub classes.... */ }
		virtual const bool Run     (void) = 0;	// This function should returns true when this node changed to new one.
		virtual void       OnExit  (void) { /* Nothing, used by sub classes.... */ }

		#if defined _DEBUG
			virtual const char *const GetDescription (void) const = 0;
		#endif	// if if defined _DEBUG
};

		#if defined _DEBUG
			static const unsigned char MAX_NODESWITCHES = 50u;
		#endif	// if defined _DEBUG

	private:
		AINode *m_currentNode;

	public:
		inline AINetwork (void) : m_currentNode (NULL) { /* VOID */ }

		inline ~AINetwork (void) { delete m_currentNode; }

	public:
		inline void EnterNewNode (AINode *const newNode)
		{
			// Enter new AI node
			// usage: EnterNewNode (new AINode_xxxx(botPointer, ...));

			// Reliability check.
//			InternalAssert (newNode != NULL);
			#if defined _DEBUG
				if (newNode == NULL)
					AddLogEntry (false, LogLevel_Critical, true, "AINetwork::EnterNewNode(): newNode is NULL!");

				if (newNode->GetOwner () == NULL)
					AddLogEntry (false, LogLevel_Critical, true, "AINetwork::EnterNewNode(): newNode->m_owner is NULL!");
			#endif	// if defined _DEBUG

			if (m_currentNode != NULL)
			{
				#if defined _DEBUG
					m_currentNode->GetOwner ()->DebugMessageFormat ("** AI Node Exiting: %s **", m_currentNode->GetDescription ());
				#endif	// if defined _DEBUG

				m_currentNode->OnExit ();	// execute the exit event

				delete m_currentNode;	// free the memory allocated for this node

				m_currentNode = NULL;	// null out the pointer
			}

			#if defined _DEBUG
				newNode->GetOwner ()->DebugMessageFormat ("** AI Node Entering: %s **", newNode->GetDescription ());
			#endif	// if defined _DEBUG

			newNode->OnEnter ();	// execute the enter event

			m_currentNode = newNode;	// remember this node
		}

		inline void Run (void)
		{
			// Run the AI network (Finite State Machine)

			// Reliability check.
//			InternalAssert (m_currentNode != NULL);
			#if defined _DEBUG
				// if the bot has no ai node
				if (m_currentNode == NULL)
					AddLogEntry (false, LogLevel_Critical, true, "AINetwork::Run(): no ai node!");

				static unsigned char count;

				count = 0u;	// Reset 'count' value.
			#endif	// if defined _DEBUG

			// execute AI nodes
			while (m_currentNode->Run ())
			#if !defined _DEBUG
				;
			#else	// if defined _DEBUG
				if (++count > MAX_NODESWITCHES)	// if the bot executed too many AI nodes
					m_currentNode->GetOwner ()->DebugMessageFormat ("AINetwork::Run(): %s at %1.1f switched more than %d AI nodes.", m_currentNode->GetOwner ()->GetName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetTime (), MAX_NODESWITCHES);
			#endif	// if defined _DEBUG
		}

	public:
#define DEFINE_AINODE(ClassName, BotClass, FuncRun)									\
	class ClassName : public AINode													\
	{																				\
		public:																		\
			ClassName (YaPB *bot) : AINode (bot) { /* VOID */ }						\
																					\
			const bool Run (void) { return m_owner->FuncRun (); }					\
																					\
			const char *const GetDescription (void) const { return #ClassName; }	\
	}

#define DEFINE_AINODE_ONENTER(ClassName, BotClass, FuncRun, FuncEnter)				\
	class ClassName : public AINode													\
	{																				\
		public:																		\
			ClassName (YaPB *bot) : AINode (bot) { /* VOID */ }						\
																					\
			void       OnEnter (void) { m_owner->FuncEnter (); }					\
			const bool Run     (void) { return m_owner->FuncRun (); }				\
																					\
			const char *const GetDescription (void) const { return #ClassName; }	\
	}

#define DEFINE_AINODE_ONEXIT(ClassName, BotClass, FuncRun, FuncExit)				\
	class ClassName : public AINode													\
	{																				\
		public:																		\
			ClassName (YaPB *bot) : AINode (bot) { /* VOID */ }						\
																					\
			const bool Run    (void) { return m_owner->FuncRun (); }				\
			void       OnExit (void) { m_owner->FuncExit (); }						\
																					\
			const char *const GetDescription (void) const { return #ClassName; }	\
	}

#define DEFINE_AINODE_ONENTEREXIT(ClassName, BotClass, FuncRun, FuncEnter, FuncExit)	\
	class ClassName : public AINode														\
	{																					\
		public:																			\
			ClassName (YaPB *bot) : AINode (bot) { /* VOID */ }							\
																						\
			void       OnEnter (void) { m_owner->FuncEnter (); }						\
			const bool Run     (void) { return m_owner->FuncRun (); }					\
			void       OnExit  (void) { m_owner->FuncExit (); }							\
																						\
			const char *const GetDescription (void) const { return #ClassName; }		\
	}
};

/// Abstract base class for Behavior Tree Nodes
class Action : public SubSystem <YaPB>, public DoubleLink
{
	friend ActionManager;

	//
	// Group: Constants.
	//
	public:
		enum State_t
		{
			State_Idle,		// Just added, not started.

			State_Running,	// Just running action.
			State_Fail,		// Failed action. :{O
			State_Complete	// Completed, get next action.

			#if defined _DEBUG
				, State_Total
			#endif	// if if defined _DEBUG
		};
		typedef unsigned char ID_t;
		typedef unsigned int  Bit_t;

	//
	// Group: Protected members.
	//
	protected:
		State_t m_state;	// Current action state.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Action (YaPB *const bot, Action *const prev = NULL, Action *const next = NULL) :
			SubSystem <YaPB> (bot),
			DoubleLink (prev, next),

			m_state (State_Idle)
		{ /* VOID */ }

		virtual inline ~Action (void) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		//
		// Function: GetState
		//
		// Description: Gets a action state.
		//
		// Returns: Action state.
		//
		inline const State_t                 GetState           (void) const { return m_state; }

		#if defined _DEBUG
			//
			// Function: GetStateString
			//
			// Description: Gets a action state string.
			//
			// Returns: Action state string.
			//
			inline const char *const         GetStateString     (void) const
			{
				static const char *const stateString[State_Total] =
				{
					"idle",

					"running",
					"fail",
					"complete"
				};

				return stateString[GetState ()];
			}

			virtual inline const char *const GetName            (void) const { return typeid (*this).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"); }
		#endif	// if if defined _DEBUG

		//
		// Function: GetID
		//
		// Description: Gets a action ID.
		//
		// Returns: Action ID.
		//
		virtual inline const ID_t            GetID              (void) const =/*>*/ 0;	// Nothing, used by sub classes....

		//
		// Function: GetBit
		//
		// Description: Gets a action bitsum.
		//
		// Returns: Action bitsum.
		//
		virtual inline const Bit_t           GetBit             (void) const { return BIT (GetID ()); }

		virtual inline void                  Fail               (void) { m_state = State_Fail; }
		virtual inline void                  Complete           (void) { m_state = State_Complete; }
		virtual inline void                  Run                (void) { m_state = State_Running; }

		inline void                          CompleteAndDelete  (void)
		{
			// Reliability checks.
			InternalAssert (!m_owner->m_scheduleManager.IsEmpty ());
			InternalAssert (m_owner->m_scheduleManager.GetCurrentTask () == this);	// OCCURS!!!!

			Complete ();

			m_owner->m_scheduleManager.RemoveCurrentTask ();
		}

		virtual inline const bool            IsIdling           (void) const { return GetState () == State_Idle; }
		virtual inline const bool            IsFailed           (void) const { return GetState () == State_Fail; }
		virtual inline const bool            IsCompleted        (void) const { return GetState () == State_Complete; }
		virtual inline const bool            IsRunning          (void) const { return GetState () == State_Running; }

		virtual inline       Task *const     GetCurrentTask     (void)       =/*>*/ 0;	// Nothing, used by sub classes....
		virtual inline const Task *const     GetCurrentTask     (void) const =/*>*/ 0;	// Nothing, used by sub classes....

		virtual inline const bool            IsTask             (void) const =/*>*/ 0;	// Nothing, used by sub classes....
		virtual inline const bool            IsSchedule         (void) const =/*>*/ 0;	// Nothing, used by sub classes....

		inline       Task     *const GetTaskPointer     (void)
		{
			// Reliability check.
			InternalAssert (IsTask ());

			return static_cast <Task *const> (this);
		}
		inline const Task     *const GetTaskPointer     (void) const
		{
			// Reliability check.
			InternalAssert (IsTask ());

			return static_cast <const Task *const> (this);
		}
		inline       Schedule *const GetSchedulePointer (void)
		{
			// Reliability check.
			InternalAssert (IsSchedule ());

			return static_cast <Schedule *const> (this);
		}
		inline const Schedule *const GetSchedulePointer (void) const
		{
			// Reliability check.
			InternalAssert (IsSchedule ());

			return static_cast <const Schedule *const> (this);
		}

	//
	// Group: Engine callbacks.
	//
	public:
		virtual /*const State_t*/void        Execute                (void) =/*>*/ 0;	// Nothing, used by sub classes....

		virtual inline void                  OnOwnerSpawn           (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnTouched              (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnReactOnRadio         (const RadioCommandsManager::RadioCommand *const/* radioMessage*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnTakeDamage           (const HalfLifeEngine::SDK::Classes::Edict *const/* inflictor*/, const unsigned short/* damage*/, const unsigned char/* armor*/, const HalfLifeEngine::SDK::Constants::DamageType_t/* bits*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnTakeBlinded          (const float/* duration*/, const Client *const/* flasher*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnKilled               (const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnKill                 (const Client *const/* victim*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnClientKilled         (Client *const/* victim*/, const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnClientDisconnect     (Client *const/* client*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnEnteredBombPlace     (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnLeaveBombPlace       (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnEnteredRescueZone    (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnLeaveEscapeZone      (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnEnteredEscapeZone    (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnLeaveRescueZone      (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnEnteredVIPSafetyZone (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnLeaveVIPSafetyZone   (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnEnteredBuyZone       (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnLeaveBuyZone         (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnAmmoAmountChanged    (const HalfLifeEngine::SDK::Constants::WeaponID_t/* ammoID*/, const unsigned char/* newAmount*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnRoundStarted         (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnRoundEnded           (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnBombPlanted          (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnBombDetonated        (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnBombDefused          (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnItemDeployPost       (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnWeaponReloadPost     (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnAddWeapon            (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnRemoveWeapon         (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnTouchingWeapon       (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnUse                  (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/, const HalfLifeEngine::SDK::Classes::Edict *const/* caller*/, const HalfLifeEngine::SDK::Constants::UseType_t/* type*/, const float/* value*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnTouchPost            (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnHostageKilled        (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/, const Client *const/* killer*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnHostageRescued       (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnHostageUnusePost     (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/, const Client *const/* currentRescuer*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnHostageUsePost       (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/, const Client *const/* previousRescuer*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnAllHostagesRescued   (void) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnImpulseCommand       (const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t/* impulseCommand*/) { /* Nothing, used by sub classes.... */ }
		virtual inline void                  OnHearNoisedSound      (const Client *const/* from*/, const NoisedSound_t &/*noisedSound*/) { /* Nothing, used by sub classes.... */ }
};

class Task : public Action
{
	//
	// Group: Constants.
	//
	public:
		// Defines bots tasks.
		enum
		{
			ID_Normal,

			ID_SelectTeam,
			ID_SelectModel,

			ID_AttackEnemy,
			ID_Idle,
			ID_Camp,
			ID_SprayLogo,
			ID_FollowUser,
			ID_MoveToPosition,
			ID_MoveToRescueZoneWithHostages,
			ID_MoveAndPickupItem,
			ID_DefuseBomb,
			ID_PickupItem,
			ID_PlantBomb,
			ID_RescueHostages,
			ID_SeekCover,
			ID_HuntEnemy,

			ID_ThrowExplosiveGrenade,
			ID_ThrowFlashBangGrenade,
			ID_ThrowSmokeGrenade,

			ID_PerformDoubleJump,
			ID_EscapeFromBomb,
			ID_ShootAtBreakable,
			ID_Hide,
			ID_Blinded,
			ID_AutoBuy,

			ID_Total
		};
		enum
		{
			Bit_Normal                = BIT (ID_Normal),

			Bit_SelectTeam            = BIT (ID_SelectTeam),
			Bit_SelectModel           = BIT (ID_SelectModel),

			Bit_AttackEnemy           = BIT (ID_AttackEnemy),
			Bit_Idle                  = BIT (ID_Idle),
			Bit_Camp                  = BIT (ID_Camp),
			Bit_SprayLogo             = BIT (ID_SprayLogo),
			Bit_FollowUser            = BIT (ID_FollowUser),
			Bit_MoveToPosition        = BIT (ID_MoveToPosition),
			Bit_MoveToRescueZoneWithHostages = BIT (ID_MoveToRescueZoneWithHostages),
			Bit_DefuseBomb            = BIT (ID_DefuseBomb),
			Bit_PickupItem            = BIT (ID_PickupItem),
			Bit_PlantBomb             = BIT (ID_PlantBomb),
			Bit_RescueHostages        = BIT (ID_RescueHostages),
			Bit_SeekCover             = BIT (ID_SeekCover),
			Bit_HuntEnemy             = BIT (ID_HuntEnemy),

			Bit_ThrowExplosiveGrenade = BIT (ID_ThrowExplosiveGrenade),
			Bit_ThrowFlashBangGrenade = BIT (ID_ThrowFlashBangGrenade),
			Bit_ThrowSmokeGrenade     = BIT (ID_ThrowSmokeGrenade),

			Bit_PerformDoubleJump     = BIT (ID_PerformDoubleJump),
			Bit_EscapeFromBomb        = BIT (ID_EscapeFromBomb),
			Bit_ShootAtBreakable      = BIT (ID_ShootAtBreakable),
			Bit_Hide                  = BIT (ID_Hide),
			Bit_Blinded               = BIT (ID_Blinded),
			Bit_AutoBuy               = BIT (ID_AutoBuy)
		};
/*		DECLARE_TYPED_ENUMERATION (unsigned int, Condition_t)
		{
			Condition_None,

			Condition_EnemyObscured = BIT (0u),
			Condition_NoWeapon      = BIT (1u),
			Condition_OutOfAmmo     = BIT (2u),
			Condition_SeeCurEnemy   = BIT (3u),
			Condition_EnemyDead     = BIT (4u),
			Condition_SeeWaypoint   = BIT (5u),
			Condition_NeedAmmo      = BIT (6u),
			Condition_NeedHealth    = BIT (7u),
			Condition_SeeLookVector = BIT (8u),
			Condition_PointCaptured = BIT (9u)
		};

	//
	// Group: Protected members.
	//
	protected:
		unsigned int m_flags;	// flags

		// conditions that may happen to fail task
		Condition_t  m_failInterruptConditions;
		Condition_t  m_completeInterruptConditions;
*/
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task (YaPB *const bot, Action *const prev = NULL, Action *const next = NULL) : Action (bot, prev, next)/*, m_flags (0u), m_failInterruptConditions (Condition_None), m_completeInterruptConditions (Condition_None)*/ { /* VOID */ }

		virtual inline ~Task (void) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
/*
		inline const bool HasFlag (const unsigned int flag) const { return (m_flags & flag) == flag; }
		inline void       SetFlag (const unsigned int flag)       { m_flags |= flag; }

		virtual inline const State_t IsInterrupted (void) const
		{
			if (m_completeInterruptConditions > Condition_None)
				if (m_owner->HasSomeConditions (m_completeInterruptConditions))
					return State_Complete;

			if (m_failInterruptConditions > Condition_None)
				if (m_owner->HasSomeConditions (m_failInterruptConditions))
					return State_Fail;

			return State_Running;
		}
		inline void SetCompleteInterrupt (const Condition_t interrupt) { m_completeInterruptConditions = interrupt; }
		inline void SetFailInterrupt     (const Condition_t interrupt) { m_failInterruptConditions = interrupt; }
		inline void ClearFailInterrupts  (void)                        { m_failInterruptConditions = Condition_None; }
*/
		inline       Task *const     GetCurrentTask     (void)       { return this; }
		inline const Task *const     GetCurrentTask     (void) const { return this; }

		inline const bool            IsTask             (void) const { return true; }
		inline const bool            IsSchedule         (void) const { return false; }

	//
	// Group: Engine callbacks.
	//
	public:
		virtual /*const State_t*/void Execute           (void) =/*>*/ 0;	// Nothing, used by sub classes....
};

// Just Task container.
class Schedule : public Action
{
	//
	// Group: Constants.
	//
	public:
		// Defines bots schedules.
		enum
		{
			ID_Attack = Task::ID_Total,
			ID_RunForCover,
			ID_PlantBomb,
			ID_DefuseBomb,
			ID_Camp,
			ID_RescueHostages,
			ID_GoodHideSpot,
			ID_Snipe,
			ID_Pickup,
			ID_LookAfterSentry,
			ID_Defend,
			ID_PerformDoubleJump,

			ID_Total
		};
/*		enum
		{
			Bit_Attack            = BIT (ID_Attack),
			Bit_RunForCover       = BIT (ID_RunForCover),
			Bit_PlantBomb         = BIT (ID_PlantBomb),
			Bit_DefuseBomb        = BIT (ID_DefuseBomb),
			Bit_Camp              = BIT (ID_Camp),
			Bit_RescueHostages    = BIT (ID_RescueHostages),
			Bit_GoodHideSpot      = BIT (ID_GoodHideSpot),
			Bit_Snipe             = BIT (ID_Snipe),
			Bit_Pickup            = BIT (ID_Pickup),
			Bit_LookAfterSentry   = BIT (ID_LookAfterSentry),
			Bit_Defend            = BIT (ID_Defend),
			Bit_PerformDoubleJump = BIT (ID_PerformDoubleJump)
		};
*/
		typedef DynamicArray <Task *, unsigned char> TasksContainer_t;	/// @todo PROBABLY MUST BE A QUEUE!!!

	/// Enumerates the options for when a parallel node is considered to have failed.
	/**
	- FAIL_ON_ONE indicates that the node will return failure as soon as one of its children fails.
	- FAIL_ON_ALL indicates that all of the node's children must fail before it returns failure.

	If FAIL_ON_ONE and SUCEED_ON_ONE are both active and are both trigerred in the same time step, failure will take precedence.
	*/
	enum FailurePolicy_t { FAIL_ON_ONE, FAIL_ON_ALL };
	/// Enumerates the options for when a parallel node is considered to have succeeded.
	/**
	- SUCCEED_ON_ONE indicates that the node will return success as soon as one of its children succeeds.
	- SUCCEED_ON_ALL indicates that all of the node's children must succeed before it returns success.
	*/
	enum SuccessPolicy_t { SUCCEED_ON_ONE, SUCCEED_ON_ALL };

	//
	// Group: Protected members.
	//
	protected:
		TasksContainer_t m_tasks;

	//
	// Group: Protected (con/de)structors.
	//
	protected:
		inline Schedule (YaPB *const bot, Action *const prev = NULL, Action *const next = NULL) : Action (bot, prev, next) { /* VOID */ }

		virtual inline ~Schedule (void)
		{
			// Delete all tasks....
			m_tasks.DeleteAll ();
		}

	//
	// Group: Private functions.
	//
	private:
		inline void Delete (const unsigned char taskIndex) { m_tasks.Delete (taskIndex, 1u); }

	//
	// Group: Functions.
	//
	public:
		inline const bool HasTask (const ID_t taskID) const
		{
			for (unsigned char taskIndex (0u); taskIndex < m_tasks.GetElementNumber (); ++taskIndex)
				if (m_tasks[taskIndex]->GetID () == taskID)
					return true;

			return false;
		}
		inline const bool HasTasks (const Bit_t tasksBits) const
		{
			for (unsigned char taskIndex (0u); taskIndex < m_tasks.GetElementNumber (); ++taskIndex)
				if (BIT (m_tasks[taskIndex]->GetID ()) & tasksBits)
					return true;

			return false;
		}

		inline void AddTask (Task *const task)
		{
			// Reliability checks.
			InternalAssert (task != NULL);
//			InternalAssert (!HasTask (task->GetID ()));

			// add
			m_tasks += task;
		}

		inline Task *const GetCurrentTask (void)
		{
			// Reliability check.
//			InternalAssert (!IsEmpty ());	// OCCURS!!!!!
			#if defined _DEBUG
				if (IsEmpty ())
					AddLogEntry (false, LogLevel_Critical, true, "%s::GetCurrentTask(): called for empty tasks array!!!, state: %s.", GetName (), GetStateString ());
			#endif	// if defined _DEBUG

			return m_tasks.GetFirstElement ();
		}

		inline const Task *const GetCurrentTask (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_tasks.GetFirstElement ();
		}

		inline const bool            IsEmpty            (void) const { return m_tasks.IsEmpty (); }
/*
		inline void FreeMemory (void)
		{
			m_tasks.DeleteAndPurgeAll ();
		}
*/
		inline void RemoveTop (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			m_tasks.DeleteFront ();
		}

		inline const bool            IsTask             (void) const { return false; }
		inline const bool            IsSchedule         (void) const { return true; }

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline /*const State_t*/void Execute (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			// current task
			Task *const currentTask (m_tasks.GetFirstElement ());
//			static State_t state;

			// Is current task was idling?
			if (currentTask->IsIdling ())
			{
				currentTask->Run ();	// Change task state.

				InternalAssert (!currentTask->IsIdling ());
			}

			if (!currentTask->IsRunning ())
			{
				if (currentTask->IsFailed ())
				{
					#if defined _DEBUG
						AddLogEntry (true, LogLevel_Warning, true, "%s::Execute(): Bot \"%s\" current task %s is failed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
					#endif	// if defined _DEBUG

					RemoveTop ();

					Fail ();

					return;
				}

				// Else....

				#if defined _DEBUG
					AddLogEntry (true, LogLevel_Default, true, "%s::Execute(): Bot \"%s\" current task %s is completed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
				#endif	// if defined _DEBUG

				RemoveTop ();

				if (IsEmpty ())
					Complete ();

				return;
			}
/*
			state = currentTask->IsInterrupted ();

			if (state == State_Fail)
				currentTask->Fail ();
			else if (state == State_Complete)
				currentTask->Complete ();
			else	// still running*/
			{
/*				#if defined _DEBUG
					if (CClients::clientsDebugging ())
						CClients::clientDebugMsg (BOT_DEBUG_TASK, GetName (), bot);
				#endif	// if defined _DEBUG
*/
				currentTask->Execute ();	// run
			}

			// Still running?
			if (currentTask->IsRunning ())
				return;

			if (currentTask->IsFailed ())
			{
				#if defined _DEBUG
					AddLogEntry (true, LogLevel_Warning, true, "%s::Execute(): Bot \"%s\" current task %s is failed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
				#endif	// if defined _DEBUG

				RemoveTop ();

				Fail ();

				return;
			}

			// Else....

			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Default, true, "%s::Execute(): Bot \"%s\" current task %s is completed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
			#endif	// if defined _DEBUG

			RemoveTop ();

			if (IsEmpty ())
				Complete ();
		}

		/// @todo REMOVE THIS SHIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		#if defined _DEBUG
		#	define IS_EMPTY_CHECK	\
				if (IsEmpty ())		\
				{	\
					AddLogEntry (true, LogLevel_Error, true, "\"%s\"::%s::%s(): Function called for empty tasks array! My state: %s, first task: %s!", m_owner->GetName ().GetData (), GetName (), __FUNCTION__ + GET_STATIC_STRING_LENGTH ("YaPB::Schedule::"), GetStateString (), m_owner->m_scheduleManager.GetCurrentAction ()->GetSchedulePointer ()->IsEmpty () ? "NONE" : FormatBuffer ("%s, state: %s", typeid (*m_owner->m_scheduleManager.GetCurrentTask ()).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"), m_owner->m_scheduleManager.GetCurrentTask ()->GetStateString ()));	\
					/*DEBUGGER_BREAK ();*/	\
					return;	\
				}
		#else
		#	define IS_EMPTY_CHECK
		#endif	// if !defined _DEBUG

		virtual inline void                         OnOwnerSpawn           (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnOwnerSpawn ();
		}
		virtual inline void                         OnTouched              (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
//			IS_EMPTY_CHECK;
//			InternalAssert (!IsEmpty ());	// OCCURS!!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnTouched (entity);
		}
		virtual inline void                         OnReactOnRadio         (const RadioCommandsManager::RadioCommand *const radioMessage)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnReactOnRadio (radioMessage);
		}
		virtual inline void                         OnTakeDamage           (const HalfLifeEngine::SDK::Classes::Edict *const inflictor, const unsigned short damage, const unsigned char armor, const HalfLifeEngine::SDK::Constants::DamageType_t bits)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnTakeDamage (inflictor, damage, armor, bits);
		}
		virtual inline void                         OnTakeBlinded          (const float duration, const Client *const flasher)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnTakeBlinded (duration, flasher);
		}
		virtual inline void                         OnKilled               (const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnKilled (killer);
		}
		virtual inline void                         OnKill                 (const Client *const victim)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnKill (victim);
		}
		virtual inline void                         OnClientKilled         (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnClientKilled (victim, killer);
		}
		virtual inline void                         OnClientDisconnect     (Client *const client)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnClientDisconnect (client);
		}
		virtual inline void                         OnEnteredBombPlace     (void)
		{
//			InternalAssert (!IsEmpty ());	// OCCURS!!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnEnteredBombPlace ();
		}
		virtual inline void                         OnLeaveBombPlace       (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnLeaveBombPlace ();
		}
		virtual inline void                         OnEnteredRescueZone    (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnEnteredRescueZone ();
		}
		virtual inline void                         OnLeaveEscapeZone      (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnLeaveEscapeZone ();
		}
		virtual inline void                         OnEnteredEscapeZone    (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnEnteredEscapeZone ();
		}
		virtual inline void                         OnLeaveRescueZone      (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnLeaveRescueZone ();
		}
		virtual inline void                         OnEnteredVIPSafetyZone (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnEnteredVIPSafetyZone ();
		}
		virtual inline void                         OnLeaveVIPSafetyZone   (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnLeaveVIPSafetyZone ();
		}
		virtual inline void                         OnEnteredBuyZone       (void)
		{
//			InternalAssert (!IsEmpty ());	// OCCURS!!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnEnteredBuyZone ();
		}
		virtual inline void                         OnLeaveBuyZone         (void)
		{
//			InternalAssert (!IsEmpty ());	// OCCURS!!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnLeaveBuyZone ();
		}
		virtual inline void                         OnAmmoAmountChanged    (const HalfLifeEngine::SDK::Constants::WeaponID_t ammoID, const unsigned char newAmount)
		{
//			IS_EMPTY_CHECK;
//			InternalAssert (!IsEmpty ());	// OCCURS!!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnAmmoAmountChanged (ammoID, newAmount);
		}
		virtual inline void                         OnRoundStarted         (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnRoundStarted ();
		}
		virtual inline void                         OnRoundEnded           (void)
		{
//			InternalAssert (!IsEmpty ());	// OCCURS!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnRoundEnded ();
		}
		virtual inline void                         OnBombPlanted          (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnBombPlanted ();
		}
		virtual inline void                         OnBombDetonated        (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnBombDetonated ();
		}
		virtual inline void                         OnBombDefused          (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnBombDefused ();
		}
		virtual inline void                         OnItemDeployPost       (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
//			InternalAssert (!IsEmpty ());	// OCCURS!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnItemDeployPost (weapon);
		}
		virtual inline void                         OnWeaponReloadPost     (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnWeaponReloadPost (weapon);
		}
		virtual inline void                         OnAddWeapon            (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
//			InternalAssert (!IsEmpty ());	// OCCURS!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnAddWeapon (weapon);
		}
		virtual inline void                         OnRemoveWeapon         (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnRemoveWeapon (weapon);
		}
		virtual inline void                         OnTouchingWeapon       (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
//			InternalAssert (!IsEmpty ());	// OCCURS!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnTouchingWeapon (weapon);
		}
		virtual inline void                         OnUse                  (const HalfLifeEngine::SDK::Classes::Edict *const entity, const HalfLifeEngine::SDK::Classes::Edict *const caller, const HalfLifeEngine::SDK::Constants::UseType_t type, const float value)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnUse (entity, caller, type, value);
		}
		virtual inline void                         OnTouchPost            (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
//			IS_EMPTY_CHECK;
//			InternalAssert (!IsEmpty ());	// OCCURS!!!
			if (!IsEmpty ())	// OCCURS!!!
				m_tasks.GetFirstElement ()->OnTouchPost (entity);
		}
		virtual inline void                         OnHostageKilled        (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const killer)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnHostageKilled (hostage, killer);
		}
		virtual inline void                         OnHostageRescued       (const HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnHostageRescued (hostage);
		}
		virtual inline void                         OnHostageUnusePost     (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const currentRescuer)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnHostageUnusePost (hostage, currentRescuer);
		}
		virtual inline void                         OnHostageUsePost       (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const previousRescuer)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnHostageUsePost (hostage, previousRescuer);
		}
		virtual inline void                         OnAllHostagesRescued   (void)
		{
			InternalAssert (!IsEmpty ());
//			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnAllHostagesRescued ();
		}
		virtual inline void                         OnImpulseCommand       (const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t impulseCommand)
		{
//			IS_EMPTY_CHECK;
//			InternalAssert (!IsEmpty ());	// OCCURS!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnImpulseCommand (impulseCommand);
		}
		virtual inline void                         OnHearNoisedSound      (const Client *const from, const NoisedSound_t &noisedSound)
		{
//			IS_EMPTY_CHECK;
//			InternalAssert (!IsEmpty ());	// OCCURS!!!
			if (!IsEmpty ())
				m_tasks.GetFirstElement ()->OnHearNoisedSound (from, noisedSound);
		}
};

class Schedule_Parallel : public Schedule
{
	//
	// Group: Protected (con/de)structors.
	//
	protected:
		inline Schedule_Parallel (YaPB *const bot, Action *const prev = NULL, Action *const next = NULL) : Schedule (bot, prev, next) { /* VOID */ }

		virtual inline ~Schedule_Parallel (void) { /* VOID */ }

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline /*const State_t*/void Execute (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			// current task
			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
			{
			Task *const currentTask (m_tasks[index]);
//			static State_t state;

			// Is current task was idling?
			if (currentTask->IsIdling ())
				currentTask->Run ();	// Change task state.

			if (!currentTask->IsRunning ())
			{
				if (currentTask->IsFailed ())
				{
					#if defined _DEBUG
						AddLogEntry (true, LogLevel_Warning, true, "%s::Execute(): Bot \"%s\" current task %s is failed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
					#endif	// if defined _DEBUG

					RemoveTop ();

					--index;	// Do step back!

					Fail ();

					continue;
				}

				// Else....

				#if defined _DEBUG
					AddLogEntry (true, LogLevel_Default, true, "%s::Execute(): Bot \"%s\" current task %s is completed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
				#endif	// if defined _DEBUG

				RemoveTop ();

				--index;	// Do step back!

				if (IsEmpty ())
					Complete ();

				continue;
			}
/*
			state = currentTask->IsInterrupted ();

			if (state == State_Fail)
				currentTask->Fail ();
			else if (state == State_Complete)
				currentTask->Complete ();
			else	// still running*/
			{
/*				#if defined _DEBUG
					if (CClients::clientsDebugging ())
						CClients::clientDebugMsg (BOT_DEBUG_TASK, GetName (), bot);
				#endif	// if defined _DEBUG
*/
				currentTask->Execute ();	// run
			}

			// Still running?
			if (currentTask->IsRunning ())
				continue;

			if (currentTask->IsFailed ())
			{
				#if defined _DEBUG
					AddLogEntry (true, LogLevel_Warning, true, "%s::Execute(): Bot \"%s\" current task %s is failed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
				#endif	// if defined _DEBUG

				RemoveTop ();

				--index;	// Do step back!

				Fail ();

				continue;
			}

			// Else....

			#if defined _DEBUG
				AddLogEntry (true, LogLevel_Default, true, "%s::Execute(): Bot \"%s\" current task %s is completed!", GetName (), currentTask->GetOwner ()->GetName ().GetData (), typeid (*currentTask).name () + GET_STATIC_STRING_LENGTH ("class YaPB::"));
			#endif	// if defined _DEBUG

			RemoveTop ();

			--index;	// Do step back!

			if (IsEmpty ())
				Complete ();
			}
		}

		virtual inline void                         OnOwnerSpawn           (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnOwnerSpawn ();
		}
		virtual inline void                         OnTouched              (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			InternalAssert (!IsEmpty ());	// OCCURS!!!!
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnTouched (entity);
		}
		virtual inline void                         OnReactOnRadio         (const RadioCommandsManager::RadioCommand *const radioMessage)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnReactOnRadio (radioMessage);
		}
		virtual inline void                         OnTakeDamage           (const HalfLifeEngine::SDK::Classes::Edict *const inflictor, const unsigned short damage, const unsigned char armor, const HalfLifeEngine::SDK::Constants::DamageType_t bits)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnTakeDamage (inflictor, damage, armor, bits);
		}
		virtual inline void                         OnTakeBlinded          (const float duration, const Client *const flasher)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnTakeBlinded (duration, flasher);
		}
		virtual inline void                         OnKilled               (const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnKilled (killer);
		}
		virtual inline void                         OnKill                 (const Client *const victim)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnKill (victim);
		}
		virtual inline void                         OnClientKilled         (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnClientKilled (victim, killer);
		}
		virtual inline void                         OnClientDisconnect     (Client *const client)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnClientDisconnect (client);
		}
		virtual inline void                         OnEnteredBombPlace     (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnEnteredBombPlace ();
		}
		virtual inline void                         OnLeaveBombPlace       (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnLeaveBombPlace ();
		}
		virtual inline void                         OnEnteredRescueZone    (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnEnteredRescueZone ();
		}
		virtual inline void                         OnLeaveEscapeZone      (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnLeaveEscapeZone ();
		}
		virtual inline void                         OnEnteredEscapeZone    (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnEnteredEscapeZone ();
		}
		virtual inline void                         OnLeaveRescueZone      (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnLeaveRescueZone ();
		}
		virtual inline void                         OnEnteredVIPSafetyZone (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnEnteredVIPSafetyZone ();
		}
		virtual inline void                         OnLeaveVIPSafetyZone   (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnLeaveVIPSafetyZone ();
		}
		virtual inline void                         OnEnteredBuyZone       (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnEnteredBuyZone ();
		}
		virtual inline void                         OnLeaveBuyZone         (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnLeaveBuyZone ();
		}
		virtual inline void                         OnAmmoAmountChanged    (const HalfLifeEngine::SDK::Constants::WeaponID_t ammoID, const unsigned char newAmount)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnAmmoAmountChanged (ammoID, newAmount);
		}
		virtual inline void                         OnRoundStarted         (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnRoundStarted ();
		}
		virtual inline void                         OnRoundEnded           (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnRoundEnded ();
		}
		virtual inline void                         OnBombPlanted          (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnBombPlanted ();
		}
		virtual inline void                         OnBombDetonated        (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnBombDetonated ();
		}
		virtual inline void                         OnBombDefused          (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnBombDefused ();
		}
		virtual inline void                         OnItemDeployPost       (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnItemDeployPost (weapon);
		}
		virtual inline void                         OnWeaponReloadPost     (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnWeaponReloadPost (weapon);
		}
		virtual inline void                         OnAddWeapon            (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnAddWeapon (weapon);
		}
		virtual inline void                         OnRemoveWeapon         (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnRemoveWeapon (weapon);
		}
		virtual inline void                         OnTouchingWeapon       (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnTouchingWeapon (weapon);
		}
		virtual inline void                         OnUse                  (const HalfLifeEngine::SDK::Classes::Edict *const entity, const HalfLifeEngine::SDK::Classes::Edict *const caller, const HalfLifeEngine::SDK::Constants::UseType_t type, const float value)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnUse (entity, caller, type, value);
		}
		virtual inline void                         OnTouchPost            (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnTouchPost (entity);
		}
		virtual inline void                         OnHostageKilled        (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const killer)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnHostageKilled (hostage, killer);
		}
		virtual inline void                         OnHostageRescued       (const HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnHostageRescued (hostage);
		}
		virtual inline void                         OnHostageUnusePost     (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const currentRescuer)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnHostageUnusePost (hostage, currentRescuer);
		}
		virtual inline void                         OnHostageUsePost       (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const previousRescuer)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnHostageUsePost (hostage, previousRescuer);
		}
		virtual inline void                         OnAllHostagesRescued   (void)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnAllHostagesRescued ();
		}
		virtual inline void                         OnImpulseCommand       (const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t impulseCommand)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnImpulseCommand (impulseCommand);
		}
		virtual inline void                         OnHearNoisedSound      (const Client *const from, const NoisedSound_t &noisedSound)
		{
			InternalAssert (!IsEmpty ());
//			if (IsEmpty ())
//				return;

			for (TasksContainer_t::IndexType_t index (0u); index < m_tasks.GetElementNumber (); ++index)
				m_tasks[index]->OnHearNoisedSound (from, noisedSound);
		}
};

#endif	// ifndef BOT_TASK_INCLUDED