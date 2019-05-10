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
// Bot.cpp
//
// Class: Bot
//
// Version: $ID:$
//

#include <Core.hpp>

namespace HalfLifeEngine
{
namespace SDK
{
namespace Classes
{
const bool CBot::Initialize (Profile *const profile)
{
	m_profile = profile;

	return true;
}

void CBot::Spawn (void)
{
	CBasePlayer::Spawn ();

	m_variables->flags |= (Constants::FL_CLIENT | Constants::FL_FAKECLIENT);
	m_pfnThink = NULL;
	m_variables->nextThinkTime = -1.0f;
	m_nextCommandExecutionTime = gpGlobals->time + g_flBotCommandInterval;
	m_nextThinkExecutionTime = gpGlobals->time + g_flBotFullThinkInterval;
	m_lastThinkTime = gpGlobals->time;
	m_doRun = true;
	m_doCrouch = false;
	m_postureContextsNumber = 0u;
	m_jumpTimestamp = 0.0f;

	ResetCommand ();
	SpawnBot ();
}

Math::Vector3D CBot::GetAutoaimVector (float delta)
{
	UTIL_MakeVectors (m_variables->viewAngles + m_variables->punchAngles);

	return gpGlobals->forward;
}

void CBot::BotThink (void)
{
	if (m_nextCommandExecutionTime > gpGlobals->time)
		return;

	m_nextCommandExecutionTime = gpGlobals->time + g_flBotCommandInterval;

	Upkeep ();

	if (gpGlobals->time >= m_nextThinkExecutionTime)
	{
		m_nextThinkExecutionTime = gpGlobals->time + g_flBotFullThinkInterval;

		ResetCommand ();
		Update ();
	}

	ExecuteCommand ();
}

const float CBot::GetMoveSpeed (void) const
{
	return m_doRun || m_doCrouch ? m_variables->maximumSpeed : m_variables->maximumSpeed * 0.4f;
}

void CBot::MoveForward (void)
{
	m_speedMove = GetMoveSpeed ();

	m_pressedButtons &= ~Constants::IN_BACK;
	m_pressedButtons |= Constants::IN_FORWARD;
}

void CBot::MoveBackward (void)
{
	m_speedMove = -GetMoveSpeed ();

	m_pressedButtons &= ~Constants::IN_FORWARD;
	m_pressedButtons |= Constants::IN_BACK;
}

void CBot::StrafeLeft (void)
{
	m_speedStrafe = -GetMoveSpeed ();

	m_pressedButtons &= ~Constants::IN_MOVERIGHT;
	m_pressedButtons |= Constants::IN_MOVELEFT;
}

void CBot::StrafeRight (void)
{
	m_speedStrafe = GetMoveSpeed ();

	m_pressedButtons &= ~Constants::IN_MOVELEFT;
	m_pressedButtons |= Constants::IN_MOVERIGHT;
}

const bool CBot::Jump (const bool a2)
{
	if (IsJumping () || m_doCrouch || (!a2 && v5 < 0.9f) || v5 < 0.3f)
		return false;

	m_pressedButtons |= Constants::IN_JUMP;
	m_jumpTimestamp = gpGlobals->time;

	return true;
}

void CBot::ClearMovement (void)
{
	ResetCommand ();
}
void CBot::ResetCommand (void)
{
	m_speedMove = 0.0f;
	m_speedStrafe = 0.0f;
	m_speedVertical = 0.0f;

	m_pressedButtons = Constants::ButtonType_None;
}

const bool CBot::IsJumping (void) const
{
	const float delta (gpGlobals->time - m_jumpTimestamp);

	return delta <= 3.0f && (delta < 1.0f || (m_variables->flags & Constants::FL_ONGROUND) == 0u);
}

void CBot::Crouch (void)
{
	m_doCrouch = true;
}

void CBot::StandUp (void)
{
	m_doCrouch = false;
}

void CBot::Run (void)
{
	m_doRun = true;
}

void CBot::Walk (void)
{
	m_doRun = false;
}

void CBot::UseEnvironment (void)
{
	m_pressedButtons |= Constants::IN_USE;
}

void CBot::PrimaryAttack (void)
{
	m_pressedButtons |= Constants::IN_ATTACK;
}

void CBot::ClearPrimaryAttack (void)
{
	m_pressedButtons &= ~Constants::IN_ATTACK;
}

void CBot::TogglePrimaryAttack (void)
{
	if (m_pressedButtons & Constants::IN_ATTACK)
		ClearPrimaryAttack ();
	else
		PrimaryAttack ();
}

void CBot::SecondaryAttack (void)
{
	m_pressedButtons |= Constants::IN_ATTACK2;
}

void CBot::Reload (void)
{
	m_pressedButtons |= Constants::IN_RELOAD;
}

const float CBot::GetActiveWeaponAmmoRatio (void) const
{
	if (m_pActiveItem == NULL)
		return 0.0f;

	if (m_pActiveItem->m_iClip < 0)
		return 1.0f;

	return static_cast <float> (m_pActiveItem->m_iClip) / m_pActiveItem->iMaxClip ();
}

const bool CBot::IsActiveWeaponClipEmpty (void) const
{
	return m_pActiveItem != NULL && m_pActiveItem->m_iClip == 0;
}

const bool CBot::IsActiveWeaponOutOfAmmo (void) const
{
	return m_pActiveItem == NULL || (m_pActiveItem->m_iClip == 0 && m_rgAmmo[m_pActiveItem->m_iPrimaryAmmoType] <= 0);
}

const bool CBot::IsUsingScope (void) const { return m_iFOV < 90; }

const unsigned char CBot::ThrottledMsec (void) const
{
	const unsigned short msec (static_cast <unsigned short> ((gpGlobals->time - m_lastThinkTime) * 1000.0f));

	return msec > 255u ? 255u : static_cast <const unsigned char> (msec);
}

void CBot::ExecuteCommand (void)
{
	const unsigned char msec (ThrottledMsec ());

	m_variables->modelAngles = m_variables->viewAngles;

	m_variables->modelAngles.pitch *= -(1.0f / 3.0f);

	m_lastThinkTime = gpGlobals->time;

	if (m_doCrouch)
		m_pressedButtons |= Constants::IN_DUCK;

	g_engfuncs.pfnRunPlayerMove (GetEdict (), m_variables->viewAngles, m_speedMove, m_speedStrafe, m_speedVertical, m_pressedButtons, Constants::PlayerImpulseID_None, msec);
}

void CBot::ClientCommand (const char *const command, const char *const argument1/* = NULL*/, const char *const argument2/* = NULL*/, const char *const argument3/* = NULL*/)
{
	BotArgs[0u] = command;
	BotArgs[1u] = argument1;
	BotArgs[2u] = argument2;
	BotArgs[3u] = argument3;

	UseBotArgs = true;

		ClientCommand (GetEdict ());

	UseBotArgs = false;
}

const bool CBot::IsEnemy (const BaseEntity *const entity) const
{
	return entity->IsPlayer () && entity->IsAlive () && entity->m_team != m_team;
}

const unsigned char CBot::GetEnemiesRemaining (void) const
{
	unsigned char enemiesRemainingNumber (0u);

	for (unsigned char playerIndex (1u); playerIndex <= gpGlobals->maximumClients; ++playerIndex)
	{
		BaseEntity *const player (UTIL_PlayerByIndex (playerIndex));

		if (player != NULL && !FNullEnt (player->m_variables) && strcmp (player->GetNetName (), "") != 0 && IsEnemy (player))
			++enemiesRemainingNumber;
	}

	return enemiesRemainingNumber;
}

const unsigned char CBot::GetFriendsRemaining (void) const
{
	unsigned char friendsRemainingNumber (0u);

	for (unsigned char playerIndex (1u); playerIndex <= gpGlobals->maximumClients; ++playerIndex)
	{
		BaseEntity *const player (UTIL_PlayerByIndex (playerIndex));

		if (player != NULL && !FNullEnt (player->m_variables) && strcmp (player->GetNetName (), "") != 0 && !IsEnemy (player) && player->IsAlive () && player != this)
			++friendsRemainingNumber;
	}

	return friendsRemainingNumber;
}

const bool CBot::IsLocalPlayerWatchingMe (void) const
{
	return m_variables != NULL
			&& (v2 = g_engfuncs.pfnIndexOfEdict (GetEdict ()), (v3 = UTIL_GetLocalPlayer ()) != NULL)
			&& ((v4 = v3->m_variables, v4->flags & Constants::FL_SPECTATOR) || v3->m_team == Constants::TeamID_Spectator)
			&& v4->iuser2 == v2
			&& (v5 = v4->iuser1, v5 > 0)
			&& (v5 <= 2 || v5 == 4);
}

inline const unsigned int CBot::GetID (void) const { return m_ID; }
inline BOOL CBot::IsBot (void) { return true; }
inline const bool CBot::IsRunning (void) const { return m_doRun; }
inline const bool CBot::IsCrouching (void) const { return m_doCrouch; }
inline const float CBot::GetJumpTimestamp (void) const { return m_jumpTimestamp; }
inline const Profile *const CBot::GetProfile (void) const { return m_profile; }
inline const BasePlayerItem *const CBot::GetActiveWeapon (void) const { return m_pActiveItem; }
inline const bool CBot::IsActiveWeaponReloading (void) const { return m_pActiveItem != NULL && (m_pActiveItem->m_fInReload || m_pActiveItem->m_fInSpecialReload); }
inline const bool CBot::IsActiveWeaponRecoilHigh (void) const { return m_pActiveItem != NULL && m_pActiveItem->m_flAccuracy > 0.4f; }

void CBot::PushPostureContext (void)
{
	if (m_postureContextsNumber == MaximumPostureContexts)
	{
		if (m_variables != NULL)
			PrintIfWatched ("PushPostureContext() overflow error!\n");
	}
	else
	{
		m_postureContexts[m_postureContextsNumber].doRun = m_doRun;
		m_postureContexts[m_postureContextsNumber].doCrouch = m_doCrouch;

		++m_postureContextsNumber;
	}
}

void CBot::PopPostureContext (void)
{
	if (m_postureContextsNumber == 0u)
	{
		if (m_variables != NULL)
			PrintIfWatched ("PopPostureContext() underflow error!\n");

		m_doRun = true;
		m_doCrouch = false;
	}
	else
	{
		--m_postureContextsNumber;

		m_doRun = m_postureContexts[m_postureContextsNumber].doRun;
		m_doCrouch = m_postureContexts[m_postureContextsNumber].doCrouch;
	}
}

void CBot::Print (const char *const format, ...) const
{
	char buffer[1024u];
	va_list va;

	va_start (va, format);
	sprintf (buffer, "%s: ", GetNetName ().GetData ());
	g_engfuncs.pfnServerPrint (buffer);
	vsprintf (buffer, format, va);
	g_engfuncs.pfnServerPrint (buffer);
}

void CBot::PrintIfWatched (const char *const format, ...) const
{
	if (cv_bot_debug.value == 0.0f)
		return;

	if ((v2 = m_variables) != NULL
		&& (v3 = g_engfuncs.pfnIndexOfEdict (GetEdict ()), (v4 = UTIL_GetLocalPlayer ()) != NULL)
		&& ((v5 = v4->m_variables, v5->flags & Constants::FL_SPECTATOR) || v4->m_team == Constants::TeamID_Spectator)
		&& v5->iuser2 == v3
		&& (v6 = v5->iuser1, v6 > 0)
		&& (v6 <= 2 || v6 == 4)
		&& (cv_bot_debug.value == 1.0f || cv_bot_debug.value == 3.0f)
		|| cv_bot_debug.value == 2.0f
		|| cv_bot_debug.value == 4.0f)
	{
		char buffer[1024u];
		va_list va;

		va_start (va, format);

		v7 = m_variables;
		if (v7 != NULL)
		{
			v8 = STRING (v7->netName);

			if (v8 == NULL)
				v8 = "(NULL netname)";
		}
		else
			v8 = "(NULL pev)";

		sprintf (buffer, "%s: ", v8);

		g_engfuncs.pfnServerPrint (buffer);

		vsprintf (buffer, format, va);

		g_engfuncs.pfnServerPrint (buffer);
	}
}

void CBot::SetModel (const char *const newModelName)
{
	g_engfuncs.pfnSetClientKeyValue (g_engfuncs.pfnIndexOfEdict (GetEdict ()), g_engfuncs.pfnGetInfoKeyBuffer (GetEdict ()), "model", newModelName);
}
}
}
}