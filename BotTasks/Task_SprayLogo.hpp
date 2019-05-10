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
// Task_SprayLogo.hpp
//
// Class: Task_SprayLogo
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_SPRAY_LOGO_INCLUDED
#define TASK_SPRAY_LOGO_INCLUDED

class Task_SprayLogo : public Task
{
	//
	// Group: Private members.
	//
	private:
		float          m_executionTime;
		Math::Vector3D m_sprayOrigin;

	//
	// Group: (Con/De)structors.
	//
	public:
		// going here
		inline Task_SprayLogo (YaPB *const bot, const float executionTime) :
			Task (bot),

			m_executionTime (executionTime),
			m_sprayOrigin (Math::Vector3D::ZeroValue)
		{
			if (bot->m_currentWeapon->GetNextAttackDelay () <= 0.0f)
				CalculateSprayPoint ();
		}

	//
	// Group: Private functions.
	//
	private:
		const bool DecalTrace (void) const;
		inline void CalculateSprayPoint (void)
		{
			const Math::Vector3D &destination (m_owner->GetEyeForwardPosition (HalfLifeEngine::SDK::Constants::MaximumSprayLogoDistance));

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_owner->m_edict->GetEyePosition (), destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_owner->m_edict, m_owner->m_traceResult);

			// No wall in front? - Look down
			if (m_owner->m_traceResult.fraction == 1.0f)
			{
				if (m_owner->IsOnFloor ())
					HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_owner->m_edict->GetEyePosition (), destination - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::MaximumSprayLogoDistance), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_owner->m_edict, m_owner->m_traceResult);
				else
				{
					Fail ();

					return;
				}

				// No wall in front? - Fail
				if (m_owner->m_traceResult.fraction == 1.0f)
				{
					Fail ();

					return;
				}
			}

			m_sprayOrigin = m_owner->m_traceResult.endPosition;
		}

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_SprayLogo; }

		inline void       Run   (void)
		{
			// Call original function in base class....
			Task::Run ();

			m_executionTime += HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);
};

#endif	// ifndef TASK_SPRAY_LOGO_INCLUDED