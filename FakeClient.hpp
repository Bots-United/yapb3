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
// FakeClient.hpp
//
// Class: FakeClient
//
// Description: Contains the 'FakeClient' class Function Prototypes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef FAKE_CLIENT_INCLUDED
#define FAKE_CLIENT_INCLUDED

class FakeClient : public Client
{
	DECLARE_CLASS_WITH_BASE (Client, FakeClient);

	//
	// Group: Private members.
	//
	private:
//		#include <FakeClientIlluminationEntity.hpp>

//		IlluminationEntity *const m_illuminationEntity;	// pointer to the dummy entity for getting the correct bot illumination (pfnGetEntityIllum() engine bug fix).
/*
		HalfLifeEngine::SDK::Structures::ClientLightStyle_t m_lightStyle[MAX_LIGHTSTYLES];
		int                                                 m_lightStyleValue[HalfLifeEngine::SDK::Constants::MaximumLightStyleValue];	// 8.8 fraction of base light value
*/
		bool                      m_isBotVoiceMessageActive;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline FakeClient (HalfLifeEngine::SDK::Classes::Edict *const fakeClient) :
			Client (fakeClient),

			// We had to create an invisible entity to correctly retrieve the fake client's illumination (for engine pfnGetEntityIllum() fake client's bug fix).
//			m_illuminationEntity (IlluminationEntity::Create (fakeClient)),

			m_isBotVoiceMessageActive (false)
		{
/*			// Setup lighting information:
			SetupLightStyles ();
*/		}

		virtual inline ~FakeClient (void)
		{
//			m_illuminationEntity->Kill ();	// Kill his light entity.
		}

	//
	// Group: Private functions.
	//
	private:
		virtual inline const Type_t GetType (void) const { return Type_FakeClient; }

		inline void UpdateAngles (void)
		{
			// Update fake client angles - make bot more like real client!
/* TEMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// Update the aim cursor....
			{
				m_edict->variables.viewAngles = m_edict->variables.modelAngles;

				m_edict->variables.viewAngles.pitch *= -3.0f;
			}
*/
			// Add rotational velocity....
			{
				if (m_edict->variables.fixAngle != HalfLifeEngine::SDK::Constants::FixAngleType_Relative)
				{
					m_edict->variables.fixAngle = HalfLifeEngine::SDK::Constants::FixAngleType_Absolute;	// Clear out any 'fixAngle' that has been set.

					return;
				}

				m_edict->variables.modelAngles.yaw += m_edict->variables.angleVelocity.yaw;

				m_edict->variables.modelAngles.ClampYaw ();

				m_edict->variables.viewAngles.yaw = m_edict->variables.modelAngles.yaw;

				// Revert all back to defaults....
				m_edict->variables.angleVelocity.yaw = 0.0f;
				m_edict->variables.fixAngle = HalfLifeEngine::SDK::Constants::FixAngleType_Absolute;
			}
		}
/*
		inline void SetupLightStyles (void)
		{
			// Setup lighting information....

			// store all current light styles
			for (unsigned char index (0u); index < MAX_LIGHTSTYLES; ++index)
				CL_Parse_LightStyle (index, g_server->GetServer ().lightstyles[index]);

			for (unsigned short index (0u); index < HalfLifeEngine::SDK::Constants::MaximumLightStyleValue; ++index)
				m_lightStyleValue[index] = 264;	// normal light value
		}

		inline void R_AnimateLight (void)
		{
			// light animations
			// 'm' is normal light, 'a' is no light, 'z' is double bright
			const int i (static_cast <int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () * 10.0f));
			int k;

			for (unsigned char j (0u); j < MAX_LIGHTSTYLES; ++j)
			{
				if (m_lightStyle[j].length == 0u)
				{
					m_lightStyleValue[j] = 256;

					continue;
				}

				k = m_lightStyle[j].map[i % m_lightStyle[j].length] - 'a';
				k *= 22;

				m_lightStyleValue[j] = k;
			}	
		}
*/
	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsFakeClient
		//
		// Description: Checks whether client is bot (fake client).
		//
		// Returns: True if client is bot (fake client), false otherwise.
		//
		inline const bool                      IsFakeClient              (void) const { return true; }

		//
		// Function: IsHuman
		//
		// Description: Checks whether client is human, not a bot.
		//
		// Returns: True if client is human, false otherwise.
		//
		inline const bool                      IsHuman                   (void) const { return false; }

		//
		// Function: IsHuman
		//
		// Description: Checks whether client is host.
		//
		// Returns: True if client is host, false otherwise.
		//
		inline const bool                      IsHost                    (void) const { return false; }

		//
		// Function: IsPlayer
		//
		// Description: Checks whether client is player.
		//
		// Returns: True if client is player, false otherwise.
		//
		inline const bool                      IsPlayer                  (void) const { return false; }

//		inline const int *const                GetLightStyleValue        (void) const { return m_lightStyleValue; }

		virtual inline const bool              IsSpeaking                (void) const { return m_isBotVoiceMessageActive; }

		inline void                            ExecuteCommand            (const DynamicString &command) const;	/// @note : This function declared in ClientManager.hpp.

		virtual inline const Math::Angles2D    GetModifiedPunchAngles    (void) const { return m_edict->GetFakeClientModifiedPunchAngles (); }
#if 0
/*static ConVar bot_mimic ("bot_mimic", "0", 0, "Bot uses usercmd of player by index.");
bool RunMimicCommand( HalfLifeEngine::SDK::Structures::UserCommand_t& cmd )
{
	if ( bot_mimic.GetInt() <= 0 )
		return false;

	if ( bot_mimic.GetInt() > gpGlobals->maxClients )
		return false;

	
	CBasePlayer *pPlayer = UTIL_PlayerByIndex( bot_mimic.GetInt()  );
	if ( !pPlayer )
		return false;

	if ( !pPlayer->GetLastUserCommand() )
		return false;

	cmd = *pPlayer->GetLastUserCommand();
	return true;
}*/
		inline void RunMove (const Math::Angles3D &moveAngles, const float forwardMoveSpeed, const float sideMoveSpeed, const float upMoveSpeed, const unsigned short buttons, const unsigned char impulse, const unsigned char msecValue)
		{/*
			if (g_server->GetServer ().paused)
				return;
*/
			// Temp save current 'host_client' and 'sv_player' pointers....
			// Store off the globals.. they're gonna get whacked
			HalfLifeEngine::SDK::Structures::Client_t *const oldHostClient (host_client);
			HalfLifeEngine::SDK::Classes::Edict *const oldPlayer (sv_player);
			const float oldFrameTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime);
			const float oldTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());

			host_client = &GetEngineClient ();
			sv_player = GetEdict ();

			HalfLifeEngine::SDK::Structures::UserCommand_t &command (host_client->lastcmd);
			const HalfLifeEngine::SDK::Classes::Edict *const viewEdict (GetViewEdict ());
			const Color <> lightColor (viewEdict->IsNotWorldspawnPlayer () ? R_LightPoint (viewEdict->variables.origin) : R_LightPoint (viewEdict->GetOrigin ()));

			// Convert back to floating point
//			host_client->timebase = g_server->GetServer ().time + host_frametime - msecValue * 0.001f;
			host_client->timebase = oldTime + oldFrameTime - msecValue * 0.001f;

			// Update last command:
//			if (!RunMimicCommand (command))
			{
			command.viewAngles = moveAngles;
			command.forwardMove = forwardMoveSpeed;
			command.sideMove = sideMoveSpeed;
			command.upMove = upMoveSpeed;
			command.lightLevel = lightColor.GetAvg ();
			command.buttons = buttons;
			command.impulse = impulse;
			command.msecValue = msecValue;
			}

			// Run it:
//			SV_PreRunCmd ();
			SV_RunCmd (host_client, &command, g_randomNumberGenerator.GetValueBetween (0u, 0x7FFFFFFFu)/* full range */);

			// Restore all back....
			// Restore the globals...
			host_client = oldHostClient;
			sv_player = oldPlayer;
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime = oldFrameTime;
			HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->time = oldTime;

//			command.buttons = 0;	// avoid multiple fires on lag
		}
#endif	// if 0
	//
	// Group: Engine callbacks.
	//
	public:
		virtual void Think (void);
		virtual void AliveThink (void);
		virtual inline void SpawnPost (void)
		{
			// Is called when the bot is spawned.

			// if fake client bit were removed by some evil action..., so we will set it back :)
			m_edict->variables.flags |= HalfLifeEngine::SDK::Constants::FL_FAKECLIENT;	// restore it

			// Call original function in base class....
			Client::SpawnPost ();
		}
/*		virtual inline void AliveSpawnPost (void)
		{
			// Is called when the bot is spawned.

//			m_illuminationEntity->OnOwnerSpawned ();

			// Call this function in base class....
			Client::AliveSpawnPost ();
		}
		virtual inline void Killed    (const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			// Is called when the bot is killed.

			m_illuminationEntity->OnOwnerKilled ();

			// Call this function in base class....
			Client::Killed (killer);
		}
/*
		inline void CL_Parse_LightStyle (const unsigned char style, const char *const value)
		{
			if (style >= MAX_LIGHTSTYLES)
			{
				AddLogEntry (true, LogLevel_Critical, true, "SVC_LIGHTSTYLE > MAX_LIGHTSTYLES");

				return;
			}

			// OCCURS!
			if (IsNullString (value))
			{
				m_lightStyle[style].length = 0u;
				m_lightStyle[style].map[0u] = '\0';

				return;
			}

			const unsigned short maximumCopyAmount (GET_STATIC_STRING_LENGTH (m_lightStyle[style].map));

			strncpy (m_lightStyle[style].map, value, maximumCopyAmount);

			m_lightStyle[style].map[maximumCopyAmount] = '\0';

			m_lightStyle[style].length = GetStringLength <int> (m_lightStyle[style].map);
		}*/

		virtual inline void SetView (const HalfLifeEngine::SDK::Classes::Edict *const viewEntity)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::SetView(\"%s\"): CALLED!", GetName ().GetData (), viewEntity->IsNotWorldspawnPlayer () ? viewEntity->GetNetName ().GetData () : viewEntity->GetClassName ().GetData ());

			GetEngineClient ().pViewEntity = viewEntity;	/// @note This doesn't change anything related to engine or game mod.
		}
};

inline       FakeClient *const Client::GetFakeClientPointer (void)
{
	// Reliability check.
	InternalAssert (IsFakeClient ());

	return static_cast <FakeClient *const> (this);
}
inline const FakeClient *const Client::GetFakeClientPointer (void) const
{
	// Reliability check.
	InternalAssert (IsFakeClient ());

	return static_cast <const FakeClient *const> (this);
}

#endif	// ifndef FAKE_CLIENT_INCLUDED