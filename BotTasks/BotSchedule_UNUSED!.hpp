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
// BotSchedule.hpp
//
// Class: Schedule
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_SCHEDULE_INCLUDED
#define BOT_SCHEDULE_INCLUDED

// Just Task container.
class Schedule
{
	friend ScheduleManager;

	//
	// Group: Constants.
	//
	public:
/*
		#define BITS_SCHED_PASS_INT		BIT (0u)
		#define BITS_SCHED_PASS_FLOAT	BIT (1u)
		#define BITS_SCHED_PASS_VECTOR	BIT (2u)
		#define BITS_SCHED_PASS_EDICT	BIT (3u)
*/
		enum ID_t
		{
			ID_Normal,
			ID_Fight,
			ID_Attack,
			ID_RunForCover,
			ID_GoToOrigin,
			ID_GoodHideSpot,
			ID_Heal,
			ID_Snipe,
			ID_Pickup,
			ID_LookAfterSentry,
			ID_Defend,
			ID_PerformDoubleJump,
			ID_AutoBuy,
			ID_SprayLogo
		};

	//
	// Group: Private members.
	//
	private:
		LinkedList <Task *, unsigned char> m_tasks;
		ID_t                               m_ID;
		DynamicString                      m_name;
		bool                               m_isFailed;

	//
	// Group: Protected (con/de)structors.
	//
	protected:
		inline Schedule (const ID_t ID, const DynamicString &name) : m_ID (ID), m_name (name), m_isFailed (false) { /*VOID*/ }

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule (Task *const task, const ID_t ID, const DynamicString &name) : m_ID (ID), m_name (name), m_isFailed (false)
		{
			// add
			AddTask (task);
		}
		virtual inline ~Schedule (void)
		{
			// Delete all tasks....
			m_tasks.DeleteAll ();
		}

	//
	// Group: Functions.
	//
	public:
		inline void AddTask (Task *const task)
		{
			// Reliability check.
			InternalAssert (task != NULL);
			InternalAssert (!m_tasks.IsMember (task));

			// add
			m_tasks += task;
		}
/*
		inline Task *const GetCurrentTask (void)
		{
			if (m_tasks.IsEmpty ())
				return NULL;

			return m_tasks.GetFirstElement ();
		}
*/
		inline const ID_t           GetID   (void) const { return m_ID; }
		inline const DynamicString &GetName (void) const { return m_name; }

		inline const bool IsEmpty (void) const
		{
			return m_tasks.IsEmpty ();
		}
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

			Task *const task (m_tasks.GetFirstElement ());

			m_tasks.PopFirst ();

			delete task;
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Execute (/*YaPB *const bot*/void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			// current task
			Task *const task (m_tasks.GetFirstElement ());
/*
			static Task::State_t state;

			state = task->isInterrupted (bot);

			if (state == Task::State_Fail)
				task->m_state = Task::State_Fail;
			else if (state == Task::State_Complete)
				task->m_state = Task::State_Complete;
			else	// still running*/
			{
/*				#if defined _DEBUG
					if (CClients::clientsDebugging ())
					{
						char dbg[512];

						task->debugString (dbg);

						CClients::clientDebugMsg (BOT_DEBUG_TASK, dbg, bot);
					}
				#endif	// if defined _DEBUG
*/
				task->Execute (/*this*/);	// run
			}

			if (task->m_state == Task::State_Fail)
				m_isFailed = true;
			else if (task->m_state == Task::State_Complete)
				RemoveTop ();
		}
};
/*
class Schedule_GoToOrigin : public Schedule
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule_GoToOrigin (const Math::Vector3D &origin) : Schedule (ID_GoToOrigin, "GoToOrigin")
		{
			AddTask (new Task_FindPath (origin));	// first
			AddTask (new Task_MoveTo (origin));	// second
		}
};
*/
class Schedule_PerformDoubleJump : public NEW::Schedule
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule_PerformDoubleJump (YaPB *const bot, const Client *const client, const Math::Vector3D &origin) :
			Schedule (bot/*ID_PerformDoubleJump, "PerformDoubleJump"*/)
		{
//			AddTask (new Task_FindPath (bot, origin));	// first
			AddTask (new Task_DoubleJump (bot, client, origin));	// second
		}

	//
	// Group: Functions.
	//
	public:
		inline const unsigned char GetID (void) const { return NEW::Task::ID_PerformDoubleJump; }
};

class ScheduleManager
{
	//
	// Group: Private members.
	//
	private:
		LinkedList <Schedule *, unsigned char> m_schedules;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ~ScheduleManager (void)
		{
			// Delete all schedules....
			m_schedules.DeleteAll ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool HasSchedule (const Schedule::ID_t schedule) const
		{
			for (unsigned char iterator (m_schedules.GetFirst ()); iterator < m_schedules.BadPosition; iterator = m_schedules.GetNext (iterator))
				if (m_schedules[iterator]->GetID () == schedule)
					return true;

			return false;
		}

		inline const bool IsCurrentSchedule (const Schedule::ID_t schedule) const
		{
			if (IsEmpty ())
				return false;

			return m_schedules.GetFirstElement ()->GetID () == schedule;
		}

		void RemoveSchedule (const Schedule::ID_t schedule)
		{
			for (unsigned char iterator (m_schedules.GetFirst ()); iterator < m_schedules.BadPosition; iterator = m_schedules.GetNext (iterator))
				if (m_schedules[iterator]->GetID () == schedule)
				{
					m_schedules.Delete (iterator);

					break;
				}
		}

		inline void RemoveTop (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Schedule *const schedule (m_schedules.GetFirstElement ());

			m_schedules.PopFirst ();

			delete schedule;
		}

		inline void FreeMemory (void)
		{
			m_schedules.DeleteAndPurgeAll ();
		}

		inline void AddFront (Schedule *const schedule)
		{
			// Reliability check.
			InternalAssert (schedule != NULL);
			InternalAssert (!m_schedules.IsMember (schedule));

			m_schedules.PushFront (schedule);
		}

		inline void AddSchedule (Schedule *const schedule)
		{
			// Reliability check.
			InternalAssert (schedule != NULL);
			InternalAssert (!m_schedules.IsMember (schedule));

			// add
			m_schedules += schedule;
		}

		inline const bool IsEmpty (void) const
		{
			return m_schedules.IsEmpty ();
		}
/*
		inline Task *const GetCurrentTask (void)
		{
			if (IsEmpty ())
				return NULL;

			return m_schedules.GetFirstElement ()->GetCurrentTask ();
		}*/

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Think (/*YaPB *const bot*/void)
		{
			if (IsEmpty ())
				return;

			Schedule *const schedule (m_schedules.GetFirstElement ());

			schedule->Execute (/*bot*/);

			if (schedule->m_tasks.IsEmpty () || schedule->m_isFailed)
				RemoveTop ();
		}
};

#endif	// ifndef BOT_SCHEDULE_INCLUDED