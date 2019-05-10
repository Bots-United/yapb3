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
// Task_Camp.hpp
//
// Class: Task_Camp
//
// Description: Camping behaviour.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_CAMP_INCLUDED
#define TASK_CAMP_INCLUDED

class Task_Camp : public Task
{
	//
	// Group: Private members.
	//
	private:
		float          m_executionTime;				// time to camp
		float          m_nextAimDirectionChangeTime;	// time next camp direction change

		Math::Vector3D m_bendLineOfSightApproachPoints[NavigationMesh::NavigationArea::MAX_APPROACH_AREAS];
		unsigned char  m_blendCount;
		unsigned char  m_currentViewPointIndex;		// camp facing direction

	//
	// Group: (Con/De)structors.
	//
	public:
		// going here
		inline Task_Camp (YaPB *const bot, const float executionTime) :
			Task (bot),

			m_executionTime (executionTime),
			m_nextAimDirectionChangeTime (0.0f),
			m_blendCount (0u),
			m_currentViewPointIndex (0u)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_Camp; }

		inline void Run (void)
		{
			if (m_owner->m_lastNavigationArea == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "\"%s\"::Task_Camp::Run(): m_lastNavigationArea == NULL !!!. Exiting....", m_owner->GetName ().GetData ());

				return;
			}

			Math::Vector3D approachPoints[NavigationMesh::NavigationArea::MAX_APPROACH_AREAS];

			for (NavigationMesh::NavigationArea::ApproachArray::IndexType_t index (0u); index < m_owner->m_lastNavigationArea->GetApproachInfoCount (); ++index)
			{
				const NavigationMesh::NavigationArea::ApproachInfo_t &info (m_owner->m_lastNavigationArea->GetApproachInfo (index));

				InternalAssert (info.here != NULL);
				InternalAssert (info.prev != NULL);

				if (info.prevToHereHow > NavigationMesh::GO_WEST)
					approachPoints[index] = info.here->GetCenter ();
				else
				{
					info.prev->ComputePortal (info.here, static_cast <NavigationMesh::NavDirType_t> (info.prevToHereHow), approachPoints[index]);

					approachPoints[index].z = info.here->GetZ (approachPoints[index]);
				}

				if (m_owner->BendLineOfSight (approachPoints[index], m_bendLineOfSightApproachPoints[m_blendCount]))
					++m_blendCount;
			}

			if (m_blendCount == 0u)
			{
				AddLogEntry (true, LogLevel_Warning, false, "\"%s\"::Task_Camp::Run(): m_blendCount == 0 !!!. Exiting....", m_owner->GetName ().GetData ());

				return;
			}

			// Call original function in base class....
			Task::Run ();

			m_executionTime += HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();

			m_owner->SelectBestWeapon ();
/*
			// play info about that
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 95u)
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						m_owner->RadioMessage (m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 400.0f) > 0u ? HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe : HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						m_owner->ChatterMessage (::ChatterManager::ChatterMessage_Camping);

						break;
				}*/
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Execute (void)
		{
			if (m_executionTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				Complete ();

				return;
			}

			m_owner->PressDuck ();

			if (m_nextAimDirectionChangeTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				m_owner->m_aim->Think ();

				return;
			}

			m_nextAimDirectionChangeTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (2.0f, 5.0f);

			if (m_owner->m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Vector)
				m_owner->m_aim->SetLookTask (new Aim::Task_Vector (m_bendLineOfSightApproachPoints[m_currentViewPointIndex]));
			else
				static_cast <Aim::Task_Vector *const> (m_owner->m_aim->GetLookTask ())->SetVector (m_bendLineOfSightApproachPoints[m_currentViewPointIndex]);

			m_owner->m_aim->Think ();

			if (++m_currentViewPointIndex >= m_blendCount)
				m_currentViewPointIndex = 0u;
		}
};

class Schedule_Camp : public Schedule
{
	//
	// Group: Private members.
	//
	private:
		const float m_executionTime;	// time to camp

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule_Camp (YaPB *const bot, const float executionTime) :
			Schedule (bot),

			m_executionTime (executionTime)
		{ /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_Camp; }

		inline void       Run   (void)
		{
			InternalAssert (m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4));
			InternalAssert (!s_bombPlaceAreaMapZones.IsEmpty ());

			AddTask (new Task_MoveToPosition (m_owner, GetRandomPositionInAreaZone (s_bombPlaceAreaMapZones.GetRandomElement ())));	// first
			AddTask (new Task_Camp (m_owner, m_executionTime));	// second

			// Call this function in base class....
			Schedule::Run ();
		}
};

#endif	// ifndef TASK_CAMP_INCLUDED