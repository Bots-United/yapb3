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
// BotAim.cpp
//
// Class: Aim
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPB::Aim::Think (void)
{
	// this function is called every frame for every bot. Its purpose is to make the bot
	// move its crosshair to the direction where it wants to look. There is some kind of
	// filtering for the view, to make it human-like.

	m_currentTask->Execute (m_owner);
#if 0
	static Math::Angles2D direction;

	// Adjust all body and view angles to face an absolute vector.
	direction = (m_lookAt - m_owner->m_edict->GetEyePosition ()).ToAngles2D ();

	switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AimMethod)->GetValue <Console::Bot::VariableValue_AimMethod_t> ())
	{
		case Console::Bot::VariableValue_AimMethod_1:
			// If bot in combat he needs to the punch angle compensation.... (Depending on skill value!)
			if (m_currentTask->GetID () == Task::ID_Enemy)
				(direction -= m_owner->GetModifiedPunchAngles () * m_owner->m_profile->skill.GetIncreasingOffset ()).Clamp ();

			// Set the body and view angles....
			m_owner->m_edict->variables.modelAngles = m_owner->m_edict->variables.viewAngles = direction;

			m_owner->m_edict->variables.viewAngles.pitch = -m_owner->GetViewAngles ().pitch;	// Invert for engine (adjust the view angle pitch to aim correctly).
			m_owner->m_edict->variables.modelAngles.pitch *= 1.0f / 3.0f;	// Adjust the body angle pitch to point the gun correctly.

			m_owner->m_edict->variables.idealPitch = m_owner->GetViewAngles ().pitch;
			m_owner->m_edict->variables.idealYaw = m_owner->GetViewAngles ().yaw;

			break;

		// By Whistler.
		case Console::Bot::VariableValue_AimMethod_2:
		{
			const float turn_skill (0.3f * m_owner->m_profile->skill.GetIncreasingOffset ());
			const float frameIntervalTime (m_owner->GetMsecValue () * 0.02f);
			const float speed (m_currentTask->GetID () == Task::ID_Enemy ? 0.7f + turn_skill : 0.2f + turn_skill * 0.5f);	// If bot is aiming at something, aim fast, else take our time.... (speed: 0.1 - 1.0)
			const float da_deadly_math (Math::expf (Math::logf (speed * 0.5f) * frameIntervalTime));	// thanks Tobias "Killaruna" Heimann and Johannes "@$3.1415rin" Lampel for this one

			direction.pitch = -direction.pitch;	// invert for engine

			const Math::Angles2D deviation ((direction - m_owner->GetViewAngles ()).Clamp ());

			m_owner->m_edict->variables.pitchSpeed = (m_owner->m_edict->variables.pitchSpeed * da_deadly_math + speed * deviation.pitch * (1.0f - da_deadly_math)) * frameIntervalTime;
			m_owner->m_edict->variables.yawSpeed = (m_owner->m_edict->variables.yawSpeed * da_deadly_math + speed * deviation.yaw * (1.0f - da_deadly_math)) * frameIntervalTime;

			// influence of y movement on x axis and vice versa (less influence than x on y since it's
			// easier and more natural for the bot to "move its mouse" horizontally than vertically)
			m_owner->m_edict->variables.pitchSpeed += m_owner->m_edict->variables.yawSpeed * (1.0f / 4.5f);
			m_owner->m_edict->variables.yawSpeed += m_owner->m_edict->variables.pitchSpeed * (1.0f / 3.0f);

			if (Math::fabsf (m_owner->m_edict->variables.pitchSpeed) >= Math::fabsf (deviation.pitch) && m_owner->m_edict->variables.pitchSpeed * deviation.pitch >= 0.0f)
				m_owner->m_edict->variables.pitchSpeed = deviation.pitch;

			if (Math::fabsf (m_owner->m_edict->variables.yawSpeed) >= Math::fabsf (deviation.yaw) && m_owner->m_edict->variables.yawSpeed * deviation.yaw >= 0.0f)
				m_owner->m_edict->variables.yawSpeed = deviation.yaw;

			m_owner->m_edict->variables.viewAngles.pitch += m_owner->m_edict->variables.pitchSpeed;	// change pitch angles
			m_owner->m_edict->variables.viewAngles.yaw += m_owner->m_edict->variables.yawSpeed;	// change yaw angles

			// set the body angles to point the gun correctly
			m_owner->m_edict->variables.modelAngles.pitch = -m_owner->GetViewAngles ().pitch * (1.0f / 3.0f);
			m_owner->m_edict->variables.modelAngles.yaw = m_owner->GetViewAngles ().yaw;

			m_owner->m_edict->variables.viewAngles.Clamp ();
			m_owner->m_edict->variables.modelAngles.Clamp ();

			break;
		}

		// By PMB.
		case Console::Bot::VariableValue_AimMethod_3:
		{
			direction.pitch = -direction.pitch;	// invert for engine

			const Math::Angles2D deviation ((direction - m_owner->GetViewAngles ()).Clamp ());

//			const float turn_slowness (1.0f);	// Is bot's aim in slow motion?
			const float updown_turn_ration (2.0f/*1.5f*/);	// how much slower bots aims up and down than side ways?
//			const unsigned char turn_skill (m_owner->m_profile->skill / 10u);
			const unsigned char turn_skill (10u/*1u*/);	// BotAim turn_skill, how good bot is at aiming on enemy origin.
			const float frameIntervalTime (m_owner->GetMsecValue () * 0.02f/*/m_owner->GetFrameIntervalTime () / turn_slowness * 20.0f/*FROM JK BOTTI*/);
			float speed (Math::GetClampedValueInRange (deviation.GetLength () * 0.5f, 0.5f/* lower bound (don't want a too slow aiming) */, 1.4f/* upper bound (don't want superman-style aiming either) */));	// If bot is aiming at something, aim fast, else take our time.... (speed: 0.1 - 1.0) (pythagores)

			if (m_currentTask->GetID () == Task::ID_Enemy)
				speed *= 2.0f;	// if bot is aiming at something, aim faster

			speed = speed * 0.2f + (turn_skill - 1u) / (20.0f / speed);	// compute final speed factor

			// thanks Tobias "Killaruna" Heimann and Johannes "@$3.1415rin" Lampel for this one
			const float da_deadly_math (Math::expf (Math::logf (speed * 0.5f) * frameIntervalTime));

			m_owner->m_edict->variables.pitchSpeed = (m_owner->m_edict->variables.pitchSpeed * da_deadly_math + speed * deviation.pitch * (1.0f - da_deadly_math)) * frameIntervalTime;
			m_owner->m_edict->variables.yawSpeed = (m_owner->m_edict->variables.yawSpeed * da_deadly_math + speed * deviation.yaw * (1.0f - da_deadly_math)) * frameIntervalTime;

			// influence of y movement on x axis and vice versa (less influence than x on y since it's
			// easier and more natural for the bot to "move its mouse" horizontally than vertically)
//			m_owner->m_edict->variables.pitchSpeed += m_owner->m_edict->variables.yawSpeed / (updown_turn_ration * (turn_skill + 1u));
//			m_owner->m_edict->variables.yawSpeed += m_owner->m_edict->variables.pitchSpeed / (turn_skill + 1u);
			// FROM JK BOTTI
			if (m_owner->m_edict->variables.pitchSpeed > 0.0f)
				m_owner->m_edict->variables.pitchSpeed += m_owner->m_edict->variables.yawSpeed / (updown_turn_ration * (turn_skill + 1u));
			else
			{
				m_owner->m_edict->variables.pitchSpeed -= m_owner->m_edict->variables.yawSpeed / (updown_turn_ration * (turn_skill + 1u));

				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "BARADA! (m_owner->m_edict->variables.pitchSpeed <= 0.0f)");
			}

			if (m_owner->m_edict->variables.yawSpeed > 0.0f)
				m_owner->m_edict->variables.yawSpeed += m_owner->m_edict->variables.pitchSpeed / (turn_skill + 1u);
			else
			{
				m_owner->m_edict->variables.yawSpeed -= m_owner->m_edict->variables.pitchSpeed / (turn_skill + 1u);

				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "BARADA! (m_owner->m_edict->variables.yawSpeed <= 0.0f)");
			}
/*
			// don't allow the bot's aiming movement to get past the target point
			if (Math::fabsf (deviation.pitch - m_owner->m_edict->variables.pitchSpeed) > 0.0f && deviation.pitch * m_owner->m_edict->variables.pitchSpeed > 0.0f)
				m_owner->m_edict->variables.pitchSpeed = deviation.pitch;

			if (Math::fabsf (deviation.yaw - m_owner->m_edict->variables.yawSpeed) > 0.0f && deviation.yaw * m_owner->m_edict->variables.yawSpeed > 0.0f)
				m_owner->m_edict->variables.yawSpeed = deviation.yaw;
*/
			// move the aim cursor
			m_owner->m_edict->variables.viewAngles.pitch += m_owner->m_edict->variables.pitchSpeed;	// change pitch angles
			m_owner->m_edict->variables.viewAngles.yaw += m_owner->m_edict->variables.yawSpeed;	// change yaw angles

			m_owner->m_edict->variables.viewAngles.Clamp ();

			// set the body angles to point the gun correctly
			m_owner->m_edict->variables.modelAngles.pitch = -m_owner->GetViewAngles ().pitch * (1.0f / 3.0f);
			m_owner->m_edict->variables.modelAngles.yaw = m_owner->GetViewAngles ().yaw;

			break;
		}

		case Console::Bot::VariableValue_AimMethod_4:
		{
			// If bot in combat he needs to the punch angle compensation.... (Depending on skill value!)
			if (m_currentTask->GetID () == Task::ID_Enemy)
				(direction -= m_owner->GetModifiedPunchAngles () * m_owner->m_profile->skill.GetIncreasingOffset ()).Clamp ();

			direction.pitch = -direction.pitch;	// invert for engine

			const Math::Angles2D deviation ((direction - m_owner->GetViewAngles ()).Clamp ());
			const float turnSkill (0.05f * m_owner->m_profile->skill + 0.5f);
			float aimSpeed (0.17f + turnSkill * 0.06f);
			const float frameCompensation (m_owner->GetFrameIntervalTime ()/*HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime*/ * 10.0f);

//			if ((m_aimFlags & AimPos_Enemy) && !(m_aimFlags & AimPos_Entity))
				aimSpeed *= 1.75f;

			const float momentum ((1.0f - aimSpeed) * 0.5f);

			m_owner->m_edict->variables.pitchSpeed = ((m_owner->m_edict->variables.pitchSpeed * momentum) + aimSpeed * deviation.pitch * (1.0f - momentum)) * frameCompensation;
			m_owner->m_edict->variables.yawSpeed = ((m_owner->m_edict->variables.yawSpeed * momentum) + aimSpeed * deviation.yaw * (1.0f - momentum)) * frameCompensation;

			if (m_owner->m_profile->skill < BotSkillLevel_Godlike)
			{
				// influence of y movement on x axis, based on skill (less influence than x on y since it's
				// easier and more natural for the bot to "move its mouse" horizontally than vertically)
				m_owner->m_edict->variables.pitchSpeed += m_owner->m_edict->variables.yawSpeed / (10.0f * turnSkill);
				m_owner->m_edict->variables.yawSpeed += m_owner->m_edict->variables.pitchSpeed / (10.0f * turnSkill);
			}

			m_owner->m_edict->variables.viewAngles.pitch += m_owner->m_edict->variables.pitchSpeed;	// change pitch angles
			m_owner->m_edict->variables.viewAngles.yaw += m_owner->m_edict->variables.yawSpeed;	// change yaw angles

			// set the body angles to point the gun correctly
			m_owner->m_edict->variables.modelAngles.pitch = -m_owner->GetViewAngles ().pitch * (1.0f / 3.0f);
			m_owner->m_edict->variables.modelAngles.yaw = m_owner->GetViewAngles ().yaw;

			m_owner->m_edict->variables.viewAngles.Clamp ();
			m_owner->m_edict->variables.modelAngles.Clamp ();

			break;
		}
/*
		case Console::Bot::VariableValue_AimMethod_5:
		{
			Math::Vector3D springStiffness (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SpringStiff_X)->GetValue <float> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SpringStiff_Y)->GetValue <float> (), 0.0f);
			Math::Vector3D damperCoefficient (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DamperCoeff_X)->GetValue <float> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_DamperCoeff_Y)->GetValue <float> (), 0.0f);
			Math::Vector3D influence (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Influence_XonY)->GetValue <float> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Influence_YonX)->GetValue <float> (), 0.0f);
			Math::Vector3D randomization (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Deviation_X)->GetValue <float> (), Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Deviation_Y)->GetValue <float> (), 0.0f);
			Math::Vector3D stiffness (Math::Vector3D::ZeroValue);
			Math::Vector3D randomize (Math::Vector3D::ZeroValue);
			const float angularDeviationLength (m_angularDeviation.GetLength ());

			// If bot in combat he needs to the punch angle compensation.... (Depending on skill value!)
			if (m_currentTask->GetID () == Task::ID_Enemy)
				(direction -= m_owner->GetModifiedPunchAngles () * m_owner->m_profile->skill.GetIncreasingOffset ()).Clamp ();

			direction.pitch = -direction.pitch;	// invert for engine

			m_idealAngles.pitch = direction.pitch;
			m_idealAngles.yaw = direction.yaw;
			m_idealAngles.roll = 0.0f;

			m_targetOriginAngularSpeed.Clamp ();
			m_idealAngles.Clamp ();

			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> ())
			{
				influence *= m_skillOffset;
				randomization *= m_skillOffset;
			}

			if (m_aimFlags & (AimPos_Enemy | AimPos_Entity | AimPos_Grenade | AimPos_LastEnemy) || GetCurrentTask ()->taskID == Task_ShootBreakable)
			{
				m_playerTargetTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
				m_randomizedIdealAngles = m_idealAngles;

				if (m_enemy->IsValid ())
				{
					m_targetOriginAngularSpeed = ((m_enemyOrigin - m_owner->GetOrigin () + (m_enemy->pointer->GetVelocity () - m_owner->GetVelocity ()) * 1.5f * m_owner->GetFrameIntervalTime ()/* - HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime*//*).ToAngles3D () - (m_enemyOrigin - m_owner->GetOrigin ()).ToAngles3D ()) * 0.45f * Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AnticipRatio)->GetValue <float> () * m_owner->m_profile->skill.GetIncreasingOffset ();

					if (angularDeviationLength < 5.0f)
						springStiffness = (5.0f - angularDeviationLength) * 0.25f * m_owner->m_profile->skill.GetIncreasingOffset () * springStiffness + springStiffness;

					m_targetOriginAngularSpeed.pitch = -m_targetOriginAngularSpeed.pitch;

					if (m_owner->GetFieldOfView () < 90u && angularDeviationLength >= 5.0f)
						springStiffness = springStiffness * 2.0f;

					m_targetOriginAngularSpeed.Clamp ();
				}
				else
					m_targetOriginAngularSpeed = Math::Vector3D::ZeroValue;

				stiffness = m_owner->m_profile->skill >= BotSkillLevel_Advanced ? springStiffness : springStiffness * (0.2f + m_owner->m_profile->skill / 125.0f);
			}
			else
			{
				// is it time for bot to randomize the aim direction again (more often where moving)?
				if (m_randomizeAnglesTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () && ((m_owner->m_edict->GetSpeed () > 1.0f && angularDeviationLength < 5.0f) || angularDeviationLength < 1.0f))
				{
					// is the bot standing still? - randomize less
					randomize = m_owner->m_edict->GetSpeed () < 1.0f ? randomization * 0.2f : randomization;

					// randomize targeted location a bit (slightly towards the ground)
					m_randomizedIdealAngles = m_idealAngles + Math::Vector3D (g_randomNumberGenerator.GetValueBetween (-randomize.x * 0.5f, randomize.x * 1.5f), g_randomNumberGenerator.GetValueBetween (-randomize.y, randomize.y), 0.0f);

					// set next time to do this
					m_randomizeAnglesTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (0.4f, Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_OffsetDelay)->GetValue <float> ());
				}

				float stiffnessMultiplier (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SlowdownRatio)->GetValue <float> ());

				// take in account whether the bot was targeting someone in the last N seconds
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - (m_playerTargetTime + Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_OffsetDelay)->GetValue <float> ()) < Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_SlowdownRatio)->GetValue <float> () * 10.0f)
				{
					stiffnessMultiplier = 1.0f - (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - m_owner->m_currentWeapon->m_timeLastFired) * 0.1f;

					// don't allow that stiffness multiplier less than zero
					if (stiffnessMultiplier < 0.0f)
						stiffnessMultiplier = 0.5f;
				}

				// also take in account the remaining deviation (slow down the aiming in the last 10°)
				if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> () && (angularDeviationLength < 10.0f))
					stiffnessMultiplier *= angularDeviationLength * 0.1f;

				// slow down even more if we are not moving
				if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> () && m_owner->m_edict->GetSpeed () < 1.0f && GetCurrentTask ()->taskID != Task_Camp && GetCurrentTask ()->taskID != Task_AttackEnemy)
					stiffnessMultiplier *= 0.5f;

				// but don't allow getting below a certain value
				if (stiffnessMultiplier < 0.35f)
					stiffnessMultiplier = 0.35f;

				stiffness = springStiffness * stiffnessMultiplier;	// increasingly slow aim

				// no target means no angular speed to take in account
				m_targetOriginAngularSpeed = Math::Vector3D::ZeroValue;
			}

			// compute randomized angle deviation this time
			m_angularDeviation = m_randomizedIdealAngles + m_targetOriginAngularSpeed - m_owner->GetViewAngles ();
			m_angularDeviation.Clamp ();

			// spring/damper model aiming
			m_aimSpeed.pitch = (stiffness.x * m_angularDeviation.pitch) - (damperCoefficient.x * m_aimSpeed.pitch);
			m_aimSpeed.yaw = (stiffness.y * m_angularDeviation.yaw) - (damperCoefficient.y * m_aimSpeed.yaw);

			// influence of y movement on x axis and vice versa (less influence than x on y since it's
			// easier and more natural for the bot to "move its mouse" horizontally than vertically)
			m_aimSpeed.pitch += m_aimSpeed.yaw * influence.y;
			m_aimSpeed.yaw += m_aimSpeed.pitch * influence.x;

			// move the aim cursor
			m_owner->m_edict->variables.viewAngles += m_owner->GetFrameIntervalTime ()/*HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime*//* * m_aimSpeed;
			m_owner->m_edict->variables.viewAngles.Clamp ();

			break;
		}
*/
		default:
			Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_AimMethod)->SetValue (Console::Bot::VariableValue_AimMethod_3);
	}
#endif	// if 0
	CheckUpdateFieldOfView ();
}

void YaPB::Aim::UpdateFieldOfView (void)
{
	// Return if bot uses zoom.
	if (m_owner->GetFieldOfView () != MaximumFieldOfView)
		return;	/// @todo MAKE CUSTOM BOT FOV'S.

	Math::Angles2D direction;
	unsigned char fieldOfViewIndex (LeftDirectionStartIndex);
	const Math::Vector3D &position (m_owner->GetOrigin ());
	const HalfLifeEngine::SDK::Constants::Hull_t hull (m_owner->IsDucking () ? HalfLifeEngine::SDK::Constants::Hull_Head : HalfLifeEngine::SDK::Constants::Hull_Human);

	direction.pitch = m_owner->GetViewAngles ().pitch;	// store bot's current view angles pitch (only once)

	// scan 90 degrees of bot's field of view from LEFT to RIGHT (yaw angles are inverted)...
	for (char angle (HalfFieldOfView); angle >= -HalfFieldOfView; angle -= MaximumFieldOfViewStep)	// +45 ... -45
	{
		// restore bot's current view angles
		direction.yaw = Math::AngleNormalize (m_owner->GetViewAngles ().yaw + angle);	// pan it from left to right

		direction.MakeForwardVector ();	// build base forward vector in that direction

		// trace hull slightly under eyes level
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (position, position + HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->forward * MaximumScanDistance, true, hull, *m_owner, m_owner->m_traceResult);

		// store the results of the scan in bot's FOV array....
		m_fieldOfView[fieldOfViewIndex].scanAngles     = direction;
		m_fieldOfView[fieldOfViewIndex].distance       = m_owner->m_traceResult.fraction * MaximumScanDistance;	// store distance to obstacle
		m_fieldOfView[fieldOfViewIndex].endPosition    = m_owner->m_traceResult.endPosition;
		m_fieldOfView[fieldOfViewIndex].plane.normal   = m_owner->m_traceResult.planeNormal;
		m_fieldOfView[fieldOfViewIndex].plane.distance = m_owner->m_traceResult.planeDistance;
		m_fieldOfView[fieldOfViewIndex].hitEntity      = m_owner->m_traceResult.hitEntity;
		m_fieldOfView[fieldOfViewIndex].contents       = HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (m_owner->m_traceResult.endPosition);
/*
		#if defined _DEBUG
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (position, m_fieldOfView[fieldOfViewIndex].endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 2u);
		#endif	// if defined _DEBUG
*/
		++fieldOfViewIndex;	// increment FOV array index
	}
}