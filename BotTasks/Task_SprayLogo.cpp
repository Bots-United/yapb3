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
// Task_SprayLogo.cpp
//
// Class: Task_SprayLogo
//
// Version: $ID:$
//

#include <Core.hpp>

const bool YaPB::Task_SprayLogo::DecalTrace (void) const
{
	// This function draw spraypaint depending on the tracing results.

	if (m_owner->m_traceResult.fraction == 1.0f)
		return false;

	unsigned short entityIndex (0u);
	unsigned short decalIndex (HalfLifeEngine::Globals::g_halfLifeEngine->GetDecalIndex (m_owner->m_profile->logo));
	HalfLifeEngine::SDK::Constants::TempEntityEvent_t message (HalfLifeEngine::SDK::Constants::TempEntityEvent_Decal);

	// only decal BSP models
	if (m_owner->m_traceResult.hitEntity->IsValid ())
	{
		if (m_owner->m_traceResult.hitEntity->variables.solid != HalfLifeEngine::SDK::Constants::SOLID_BSP && m_owner->m_traceResult.hitEntity->variables.moveType != HalfLifeEngine::SDK::Constants::MOVETYPE_PUSHSTEP)
			return false;

		entityIndex = m_owner->m_traceResult.hitEntity->GetIndex ();

		if (decalIndex > 255u)
		{
			message = HalfLifeEngine::SDK::Constants::TempEntityEvent_DecalHigh;

			decalIndex -= 256u;
		}
	}
	else
	{
		message = HalfLifeEngine::SDK::Constants::TempEntityEvent_WorldDecal;

		if (decalIndex > 255u)
		{
			message = HalfLifeEngine::SDK::Constants::TempEntityEvent_WorldDecalHigh;

			decalIndex -= 256u;
		}
	}

	if (/*(m_owner->m_profile->logo & '{')*/m_owner->m_profile->logo[0u] == '{' && entityIndex > 0u)
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_BROADCAST, HalfLifeEngine::SDK::Constants::SVC_TEMPENTITY, NULL, NULL);
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (HalfLifeEngine::SDK::Constants::TempEntityEvent_PlayerDecal);
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (static_cast <const unsigned char> (m_owner->m_edict->GetIndex ()));
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteCoordinates (m_owner->m_traceResult.endPosition);
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (entityIndex);
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (static_cast <unsigned char> (decalIndex));
		HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();
	}
	else
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_BROADCAST, HalfLifeEngine::SDK::Constants::SVC_TEMPENTITY, NULL, NULL);
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (static_cast <unsigned char> (message));
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteCoordinates (m_owner->m_traceResult.endPosition);
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (static_cast <const unsigned char> (decalIndex));

			if (entityIndex > 0u)
				HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (entityIndex);

		HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();
	}

	// Emit sprayer sound
	m_owner->m_edict->EmitSound (HalfLifeEngine::SDK::Constants::SoundChannel_Voice, "player/sprayer.wav");

	return true;
}

void YaPB::Task_SprayLogo::Execute (void)
{
	// bot sprays messy logos all over the place...

	// Check if time is elapsed.
	if (m_executionTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		Fail ();

		return;
	}

	m_owner->m_aim->Think ();
//	m_owner->m_navigator->Think ();
	m_owner->m_chatterManager.Think (m_owner);

	// fight the enemy if we found one
	if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
	{
		// Reliability check.
		InternalAssert (m_owner->m_enemy->IsValid ());

		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_SprayLogo::Execute(): Find enemy \"%s\" - break task....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

		m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));

		Complete ();

		return;
	}

	// bot didn't spray this round?
	if (m_owner->m_currentWeapon->GetNextAttackDelay () > 0.0f)
	{
		if (m_sprayOrigin == Math::Vector3D::ZeroValue)
			CalculateSprayPoint ();

		return;
	}

	// no wall in front? - Look down
	/*m_owner->m_campOrigin*/m_owner->m_aim->m_lookAt = m_sprayOrigin;

	if (m_owner->m_aim->GetLookTask ()->GetID () != Aim::Task::ID_Vector)
		m_owner->m_aim->SetLookTask (new Aim::Task_Vector (m_sprayOrigin));
	else
		static_cast <Aim::Task_Vector *const> (m_owner->m_aim->GetLookTask ())->SetVector (m_sprayOrigin);

//	m_owner->m_aimFlags |= AimPos_Override;

	// spray logo when finished looking down
//	if (m_tasks->time - (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AimMethod)->GetValue <unsigned char> () == 1u ? 0.95f : 0.5f) > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	if (m_owner->GetShootingConeDeviation (/*m_owner->m_campOrigin*/m_owner->m_aim->m_lookAt) < 0.5f)
		return;

	// paint actual logo decal, end emit sprayer sound, if it has turned out...
	if (!DecalTrace ())
	{
		Fail ();

		return;
	}

	// update timers

	m_owner->m_edict->SetPrivateData (HalfLifeEngine::SDK::Constants::OFFSET_PLAYER_NEXT_DECAL_TIME, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetValue <float> ("decalfrequency"));

	m_owner->m_timeLogoSpray = m_owner->m_edict->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_PLAYER_NEXT_DECAL_TIME) + g_randomNumberGenerator.GetValueBetween (0.5f, 15.0f);

//	m_owner->m_navigationTimeSet = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();

	Complete ();
}