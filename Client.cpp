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
// Client.cpp
//
// Class: Client
//
// Version: $ID:$
//

#include <Core.hpp>

void Client::AliveSpawnPost (void)
{
	// Is called when the client/bot is spawned.

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::AliveSpawnPost(): Client \"%s\" spawned.\n", GetName ().GetData ());

	SetAlive ();	// I'm alive now!

	m_currentWeapon->OnOwnerSpawned ();

	{
		m_lastNavigationArea = g_server->GetNavigationMeshManager ().GetNavArea (GetOrigin ());//NULL;	// WHICH ONE?!?!?!?!

//		m_lastPlaceName.MakeEmpty ();
	}
}

void Client::Use (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/, const HalfLifeEngine::SDK::Classes::Edict *const/* caller*/, const HalfLifeEngine::SDK::Constants::UseType_t/* type*/, const float/* value*/)
{
	/// @TODO: Do some actions here...

//	if (caller.IsValid ())
//		AddLogEntry (false, LogLevel_Critical, false, "Client %s used \"%s\" entity, what's used \"%s\" entity with \"%s\" type and \"%f\" value :{P.", GetName ().GetData (), entity.GetClassName ().GetData (), caller.GetClassName ().GetData (), type == UseType_Off ? "off" : type == UseType_On ? "on" : type == UseType_Set ? "set" : type == UseType_Toggle ? "toggle" : "ERROR!!!", value);
//	else
//		AddLogEntry (false, LogLevel_Critical, false, "Client %s used \"%s\" entity with \"%s\" type and \"%f\" value :{P.", GetName ().GetData (), entity.GetClassName ().GetData (), type == UseType_Off ? "off" : type == UseType_On ? "on" : type == UseType_Set ? "set" : type == UseType_Toggle ? "toggle" : "ERROR!!!", value);
}

void Client::EmitSound (const DynamicString &sample, const float volume, const float attenuation, const HalfLifeEngine::SDK::Constants::SoundFlag_t flags, const HalfLifeEngine::SDK::Constants::SoundPitch_t pitch)
{
/*	{
	const DynamicString fullFilename ("/sound/" + sample);
	const DynamicString CSfullFilename (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + fullFilename);

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::EmitSound(\"%s\", %.2f) called, sample duration = %lf!", sample.GetData (), volume, STDIOFile::IsExists (CSfullFilename, "rb") ? GetWaveSoundPlayDuration (CSfullFilename) : GetWaveSoundPlayDuration ("valve" + fullFilename));
	}
*/
	m_currentNoisedSound.Initialize (GetEdict (), sample, volume, attenuation, flags, pitch);

	UpdateLastNoisedSound ();
	TryReceiveLastNoisedSound ();
#if 0
	// hit sound?
	if (strncmp ("player/bhit_flesh-", sample, GET_STATIC_STRING_LENGTH ("player/bhit_flesh-")) == 0)
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("player/bhit_flesh-")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.32f;	// (0.316372)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 0.34f;	// (0.344898)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 0.29f;	// (0.293651)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("player/headshot", sample, GET_STATIC_STRING_LENGTH ("player/headshot")) == 0)	// fall sound?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("player/headshot")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.87f;	// (0.865760)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 1.0f;	// (1.004082)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 0.71f;	/// @todo NEED TO CALCULATE!!!

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("items/gunpickup", sample, GET_STATIC_STRING_LENGTH ("items/gunpickup")) == 0)	// weapon pickup?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("items/gunpickup")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.95f;	/// @todo NEED TO CALCULATE!!!

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 0.66f;	// (0.658821)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 0.93f;	/// @todo NEED TO CALCULATE!!!

				break;

			case '4':
				m_currentNoisedSound.maximumLastingTime = 1.04f;	/// @todo NEED TO CALCULATE!!!

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strcmp ("weapons/zoom.wav", sample) == 0)		// sniper zooming?
	{
		m_currentNoisedSound.hearingDistance = 512.0f * volume;
		m_currentNoisedSound.maximumLastingTime = 0.36f;	// (0.356599)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strcmp ("weapons/dryfire_pistol.wav", sample) == 0)		// pistol clip is empty?
	{
		m_currentNoisedSound.hearingDistance = 512.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 0.12f;	// (0.116780)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strcmp ("weapons/dryfire_rifle.wav", sample) == 0)		// rifle clip is empty?
	{
		m_currentNoisedSound.hearingDistance = 512.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 0.14f;	// (0.136825)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strcmp ("items/flashlight1.wav", sample) == 0)	// someone used flashlight?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 0.07f;	// (0.067120)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strcmp ("weapons/he_bounce-1.wav", sample) == 0)	// grenade impact?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 0.49f;	// (0.485351)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("weapons/debris", sample, GET_STATIC_STRING_LENGTH ("weapons/debris")) == 0)	// HE grenade exploded?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 1.0f;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("weapons/grenade_hit", sample, GET_STATIC_STRING_LENGTH ("weapons/grenade_hit")) == 0)	// flashbang/smoke grenade impact?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 0.49f;	// (0.489161)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("weapons/flashbang-", sample, GET_STATIC_STRING_LENGTH ("weapons/flashbang-")) == 0)	// flashbang grenade exploded?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 1.4f;	// (1.414921)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strcmp ("weapons/sg_explode.wav", sample) == 0)	// smoke grenade exploded?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 1.0f;	// (1.010340)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("items/nvg_o", sample, GET_STATIC_STRING_LENGTH ("items/nvg_o")) == 0)	// someone has switched on/off nigt vision goggles?
	{
		m_currentNoisedSound.hearingDistance = 512.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("items/nvg_o")])
		{
			// activate (nvg_oN)
			case 'n':
				m_currentNoisedSound.maximumLastingTime = 1.76f;	/// @todo NEED TO CALCULATE!!!

				break;

			// deactivate (nvg_oFf)
			case 'f':
				m_currentNoisedSound.maximumLastingTime = 0.26f;	/// @todo NEED TO CALCULATE!!!

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("player/pl_", sample, GET_STATIC_STRING_LENGTH ("player/pl_")) == 0)	// someone has switched on/off nigt vision goggles?
	{
		if (strncmp (&sample[GET_STATIC_STRING_LENGTH ("player/pl_")], "swim", GET_STATIC_STRING_LENGTH ("swim")) == 0)
		{
			m_currentNoisedSound.hearingDistance = 512.0f * volume;	/// @todo NEED TO CALCULATE!!!

			switch (sample[GET_STATIC_STRING_LENGTH ("player/pl_swim")])
			{
				case '1':
					m_currentNoisedSound.maximumLastingTime = 0.6f;	// (0.604717)

					break;

				case '2':
					m_currentNoisedSound.maximumLastingTime = 1.44f;	// (1.439093)

					break;

				case '3':
					m_currentNoisedSound.maximumLastingTime = 0.78f;	// (0.782857)

					break;
			}

			m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;
		}
		else if (strncmp (&sample[GET_STATIC_STRING_LENGTH ("player/pl_")], "wade", GET_STATIC_STRING_LENGTH ("wade")) == 0)
		{
			m_currentNoisedSound.hearingDistance = 512.0f * volume;	/// @todo NEED TO CALCULATE!!!

			switch (sample[GET_STATIC_STRING_LENGTH ("player/pl_wade")])
			{
				case '1':
					m_currentNoisedSound.maximumLastingTime = 1.2f;	// (1.201542)

					break;

				case '2':
					m_currentNoisedSound.maximumLastingTime = 0.9f;	// (0.897188)

					break;
			}

			m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;
		}

		return;
	}

	if (strcmp ("player/sprayer.wav", sample) == 0)	// someone painted decal?
	{
		m_currentNoisedSound.hearingDistance = 768.0f * volume;
		m_currentNoisedSound.maximumLastingTime = 0.9f;	// (0.906576)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}
#if 0
	// this doesn't work since the reload sound is played on client-side :(
	if (strncmp ("weapons/reload", sample, GET_STATIC_STRING_LENGTH ("weapons/reload")) == 0)	// reloading?
	{
		m_currentNoisedSound.hearingDistance = 512.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("weapons/reload")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.22f;	/// @todo NEED TO CALCULATE!!!

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 0.27f;	/// @todo NEED TO CALCULATE!!!

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 0.61f;	/// @todo NEED TO CALCULATE!!!

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}
#endif	// if 0
	// the following sounds don't have the player entity associated, so we need to search the nearest player
	if (strncmp ("items/9mmclip", sample, GET_STATIC_STRING_LENGTH ("items/9mmclip")) == 0)	// ammo pickup?
	{
		m_currentNoisedSound.hearingDistance = 512.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("items/9mmclip")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.3f;	// (0.299637)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 0.45f;	/// @todo NEED TO CALCULATE!!!

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("hostage/hos", sample, GET_STATIC_STRING_LENGTH ("hostage/hos")) == 0)		// CT used hostage?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("hostage/hos")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 1.04f;	// (1.037188)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 1.16f;	// (1.162222)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 0.91f;	// (0.913605)

				break;

			case '4':
				m_currentNoisedSound.maximumLastingTime = 1.48f;	// (1.476599)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("weapons/c4_beep", sample, GET_STATIC_STRING_LENGTH ("weapons/c4_beep")) == 0)		// bomb ticking?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("weapons/c4_beep")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 1.5f;	// (1.500590)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 1.5f;	// (1.500590)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 1.5f;	// (1.500590)

				break;

			case '4':
				m_currentNoisedSound.maximumLastingTime = 1.5f;	// (1.500590)

				break;

			case '5':
				m_currentNoisedSound.maximumLastingTime = 1.5f;	// (1.500590)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("debris/bustglass", sample, GET_STATIC_STRING_LENGTH ("debris/bustglass")) == 0)	// broke glass?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("debris/bustglass")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.76f;	// (0.760000)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 0.96f;	// (0.958367)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("debris/wood", sample, GET_STATIC_STRING_LENGTH ("debris/wood")) == 0)	// hit wood?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("debris/wood")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.56f;	// (0.562449)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 0.58f;	// (0.580590)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 0.54f;	// (0.542676)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("debris/metal", sample, GET_STATIC_STRING_LENGTH ("debris/metal")) == 0)	// hit metal?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("debris/metal")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.56f;	// (0.561088)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 0.55f;	// (0.547120)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("debris/bustmetal", sample, GET_STATIC_STRING_LENGTH ("debris/bustmetal")) == 0)	// broke bustmetal?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("debris/bustmetal")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.87f;	// (0.872834)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 1.17f;	// (1.171791)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("debris/bustcrate", sample, GET_STATIC_STRING_LENGTH ("debris/bustcrate")) == 0)	// broke bustcrate?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;

		switch (sample[GET_STATIC_STRING_LENGTH ("debris/bustcrate")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 0.6f;	// (0.602177)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 0.49f;	// (0.490794)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("debris/bustflesh", sample, GET_STATIC_STRING_LENGTH ("debris/bustflesh")) == 0 || strncmp ("debris/glass", sample, GET_STATIC_STRING_LENGTH ("debris/glass")) == 0)	// broke something?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 1.0f;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("debris/pushbox", sample, GET_STATIC_STRING_LENGTH ("debris/pushbox")) == 0)	// move something?
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;	/// @todo NEED TO CALCULATE!!!

		switch (sample[GET_STATIC_STRING_LENGTH ("debris/pushbox")])
		{
			case '1':
				m_currentNoisedSound.maximumLastingTime = 1.17f;	// (1.168980)

				break;

			case '2':
				m_currentNoisedSound.maximumLastingTime = 1.19f;	// (1.188027)

				break;

			case '3':
				m_currentNoisedSound.maximumLastingTime = 1.19f;	// (1.188571)

				break;
		}

		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strcmp ("common/wpn_select.wav", sample) == 0)	// unuse something?
	{
		m_currentNoisedSound.hearingDistance = 512.0f * volume;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.maximumLastingTime = 0.13f;	// (0.129705)
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;

		return;
	}

	if (strncmp ("doors/doormove", sample, GET_STATIC_STRING_LENGTH ("doors/doormove")) == 0)	// someone opened a door
	{
		m_currentNoisedSound.hearingDistance = 1024.0f * volume;
		m_currentNoisedSound.maximumLastingTime = 2.0f;	/// @todo NEED TO CALCULATE!!!
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;
	}
#endif	// if 0
}
/*
HalfLifeEngine::SDK::Structures::TraceResult_t m_traceResult;
		inline const bool ItemIsVisible (const Math::Vector3D &destination, const HalfLifeEngine::SDK::Classes::Edict *const item, const HalfLifeEngine::SDK::Constants::TraceIgnore_t traceIgnore)
		{
			HalfLifeEngine::SDK::Classes::Edict *const m_edict (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ());

			// trace a line from bot's eyes to destination..
			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), destination, traceIgnore, m_edict, m_traceResult);

			// check if line of sight to object is not blocked (i.e. visible), check for standard items
			if (m_traceResult.fraction == 1.0f || m_traceResult.hitEntity == item)
				return true;

			return m_traceResult.fraction > 0.98f && m_traceResult.hitEntity->GetClassName ().CompareWithoutCaseCheck ("csdmw_", GET_STATIC_STRING_LENGTH ("csdmw_")) == 0;
		}
*
void DoAirStrafes (Host *const host)
{
	const float CVARwiggle (120.0f);
	const float CVARwiggleangle (12.0f);
	static float g_wiggleangle (12.0f);
	static bool g_wiggleleft (true);
	bool g_doWiggle (false);

//CreateMove:
//	if (CVARwiggle > 0.0f && (pCmd->buttons & HalfLifeEngine::SDK::Constants::IN_USE) && !(me.pmFlags & HalfLifeEngine::SDK::Constants::FL_ONGROUND))
	if (CVARwiggle > 0.0f && (host->GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE) && !host->IsOnFloor ())
	{
		g_doWiggle = true;

		if (g_wiggleleft)
//			pCmd->sidemove -= 250.0f;
			host->GetEdict ()->variables.buttons = HalfLifeEngine::SDK::Constants::IN_MOVELEFT;
		else
//			pCmd->sidemove += 250.0f;
			host->GetEdict ()->variables.buttons = HalfLifeEngine::SDK::Constants::IN_MOVERIGHT;
	}
	else
		g_doWiggle = false;

//	#define ANGLES_YAW host->GetViewAngles ().yaw
	#define ANGLES_YAW host->GetEdict ()->variables.modelAngles.yaw

//PreV_Calcrefdef:
	if (g_doWiggle)
	{
		const float speed (host->GetEdict ()->GetSpeed2D ());

		if (speed != 0.0f)
		{
			float wiggy (CVARwiggle / speed);
			const float halfWiggleAngle (CVARwiggleangle * 0.5f);

			if (wiggy > halfWiggleAngle)
				wiggy = halfWiggleAngle;

			if (g_wiggleleft)
			{
				if (g_wiggleangle + wiggy > CVARwiggleangle)
					wiggy = CVARwiggleangle - g_wiggleangle;

				g_wiggleangle += wiggy;

//				pParams->cl_viewangles.yaw += wiggy;
				ANGLES_YAW += wiggy;
			}
			else
			{
				if (g_wiggleangle - wiggy < -CVARwiggleangle)
					wiggy = CVARwiggleangle + g_wiggleangle;

				g_wiggleangle -= wiggy;

//				pParams->cl_viewangles.yaw -= wiggy;
				ANGLES_YAW -= wiggy;
			}

			if (g_wiggleangle >= CVARwiggleangle)
				g_wiggleleft = false;
			else if (g_wiggleangle <= -CVARwiggleangle)
				g_wiggleleft = true;
		}
	}
	else if (g_wiggleangle != 0.0f)
	{
		g_wiggleangle = 0.0f;

//		pParams->cl_viewangles.yaw -= g_wiggleangle;
		ANGLES_YAW -= g_wiggleangle;
	}

	host->GetEdict ()->variables.fixAngle = HalfLifeEngine::SDK::Constants::FixAngleType_Absolute;
}
*/
static void SV_ParseVoiceData (Client *const client, const bool isFinalData)
{
	// Gets voice data from a client and forwards it to anyone who can hear this client.

	if (/*cls.state != ca_active || */!/*HalfLifeEngine::SDK::Voice::*/client->GetYaPBPointer ()->GetChatterManager ().Voice_IsRecording ())
		return;

	// Disable voice?
	if (!Console::Server::variableManager.GetVariable (Console::Server::VariableID_VoiceEnable)->GetValue <bool> () || Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> () != Console::Bot::VariableValue_Communication_VoiceNew)
		return;

	// Get whatever compressed data there is and and send it.
	char voiceData[2048u];
//	const unsigned short voiceDataLength (HalfLifeEngine::SDK::Voice::Voice_GetCompressedData (voiceData, sizeof (voiceData), isFinalData));
	const unsigned short voiceDataLength (client->GetYaPBPointer ()->GetChatterManager ().Voice_GetCompressedData (voiceData, sizeof (voiceData), isFinalData));

	if (voiceDataLength == 0u)
		return;

	const unsigned char sourceClientIndex (static_cast <unsigned char> (&client->GetEngineClient () - g_server->GetStatic ().clients));

	for (unsigned char destinationClientIndex (0u); destinationClientIndex < HalfLifeEngine::SDK::Constants::MaximumClients; ++destinationClientIndex)
	{
		const bool isLocalClient (sourceClientIndex == destinationClientIndex);

		// Skip bot ourself!
		if (isLocalClient)
			continue;

		// Does the game code want 'sourceClientIndex' sending to this client?
		if (!(client->GetEngineClient ().m_VoiceStreams[destinationClientIndex >> 5u] & BIT (destinationClientIndex & (HalfLifeEngine::SDK::Constants::MaximumClients - 1u)))/*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientVoiceListening (destinationClientIndex + 1u, sourceClientIndex + 1u)*//* && !isLocalClient*/)
			continue;

		const HalfLifeEngine::SDK::Structures::Client_t &destinationClient (g_server->GetStatic ().clients[destinationClientIndex]);

		// Is this client even on the server?
		if (!destinationClient.active && !destinationClient.connected/* && !isLocalClient*/)
			continue;

		unsigned short sendVoiceDataLength (voiceDataLength);
/*
		// Is loopback enabled?
		if (isLocalClient && !destinationClient.m_bLoopback)
			sendVoiceDataLength = 0u;	// Still send something, just zero length (this is so the client can display something that shows knows the server knows it's talking).
*/
		// Is there room to write this data in?
		if (destinationClient.datagram.cursize + sizeof (unsigned char) + 2u + sizeof (sourceClientIndex) + sizeof (sendVoiceDataLength) + sendVoiceDataLength/*voiceDataLength - I COMMENTED IT CUZ WE SENT 'sendVoiceDataLength' INSTEAD OF IT!!!*/ >= destinationClient.datagram.maxsize)
			continue;

		HalfLifeEngine::Globals::g_halfLifeEngine->MessageBegin (HalfLifeEngine::SDK::Constants::MSG_ONE_UNRELIABLE, HalfLifeEngine::SDK::Constants::SVC_VOICEDATA, NULL, destinationClient.edict);	// begin message
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (sourceClientIndex/* zero based */);
			HalfLifeEngine::Globals::g_halfLifeEngine->WriteShort (sendVoiceDataLength);

			for (unsigned short index (0u); index < sendVoiceDataLength; ++index)
				HalfLifeEngine::Globals::g_halfLifeEngine->WriteByte (static_cast <unsigned char> (voiceData[index]));

		HalfLifeEngine::Globals::g_halfLifeEngine->MessageEnd ();	// end of message
	}
}

void Client::AliveThink (void)
{
/*
	HalfLifeEngine::SDK::Classes::Edict *const ladder (NavigationMesh::NavigationLadder::FindEdict (GetOrigin ()));
	if (ladder != NULL)
	{
		g_server->GetHumanManager ()->GetHostClient ()->DrawBox (ladder->variables.absoluteBoundingBox.mins, ladder->variables.absoluteBoundingBox.maxs, Color <> (0u, 255u, 0u), 1u, 255u, 1u, false);
		g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (ladder->GetCenter (), 15.0f, Color <> (0u, 255u, 0u), 1u, 255u, 1u, false);
	}*/
	{
static HalfLifeEngine::SDK::Classes::ConsoleVariable g_cvar_drawHitBoxes (PRODUCT_CONSOLE_TAG "_" "draw_hit_boxes", "0", HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server);
static HalfLifeEngine::SDK::Classes::ConsoleVariable *g_p_cvar_drawHitBoxes (NULL);

		if (g_p_cvar_drawHitBoxes == NULL)
		{
			// Register current console variable....
			HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_Register (g_cvar_drawHitBoxes);

			// Get pointer to this already registered console variable.
			g_p_cvar_drawHitBoxes = HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetPointer (g_cvar_drawHitBoxes.name);
		}
		else if (g_p_cvar_drawHitBoxes->GetValue <bool> () && !IsHost () && IsAlive ())
		{
			StudioSetupBones ();	// all bones used

			const HalfLifeEngine::SDK::Structures::StudioHeader_t *const studioHeader (GetEdict ()->GetModelPointer ());

			InternalAssert (studioHeader != NULL)

			const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const pbboxes (reinterpret_cast <const HalfLifeEngine::SDK::Structures::mstudiobbox_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->hitboxindex));

			for (unsigned char hitBoxIndex (0u); hitBoxIndex < studioHeader->numhitboxes; ++hitBoxIndex)
			{
			const HalfLifeEngine::SDK::Structures::mstudiobbox_t &bbox (pbboxes[hitBoxIndex]);
			const Math::Vector3D &bbmin (bbox.boundingBox.mins);
			const Math::Vector3D &bbmax (bbox.boundingBox.maxs);
			Math::Vector3D box[8u];

			box[0u].x = bbmin.x;
			box[0u].y = bbmax.y;
			box[0u].z = bbmin.z;

			box[1u] = bbmin;

			box[2u].x = bbmax.x;
			box[2u].y = bbmax.y;
			box[2u].z = bbmin.z;

			box[3u].x = bbmax.x;
			box[3u].y = bbmin.y;
			box[3u].z = bbmin.z;

			box[4u] = bbmax;

			box[5u].x = bbmax.x;
			box[5u].y = bbmin.y;
			box[5u].z = bbmax.z;

			box[6u].x = bbmin.x;
			box[6u].y = bbmax.y;
			box[6u].z = bbmax.z;

			box[7u].x = bbmin.x;
			box[7u].y = bbmin.y;
			box[7u].z = bbmax.z;

//			StudioSetupBones (static_cast <char> (bbox.bone));

			const HalfLifeEngine::SDK::TypeDefinitions::BoneTransformMatrix_t &boneTransformMatrix (HalfLifeEngine::Globals::g_halfLifeEngine->GetBoneTransformMatrix ());
			const Math::Vector3D transformedBox[8u] =
			{
				box[0u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[1u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[2u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[3u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[4u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[5u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[6u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[7u].GetTransformed (boneTransformMatrix[bbox.bone])
			};
//			/*const */Math::Vector3D center ((transformedBox[0u] + transformedBox[1u] + transformedBox[2u] + transformedBox[3u] + transformedBox[4u] + transformedBox[5u] + transformedBox[6u] + transformedBox[7u]) / GET_STATIC_ARRAY_SIZE (transformedBox));
			/*const */Math::Vector3D center (bbox.boundingBox.GetCenter ().GetTransformed (boneTransformMatrix[bbox.bone]));

			g_server->GetHumanManager ()->GetHostClient ()->DrawBox (transformedBox, Color <> (0u, 255u, 0u), 1u, 255u, 1u, false);
//			g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (center, 5.0f, Color <> (255u, 0u, 0u), 1u, 255u, 1u, false);
//			g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (GetEyePosition (), 5.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (255u, 0u, 0u), 1u);
			}
		}
	}

//	const Math::Vector3D &r_origin (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetMemory <Math::Vector3D> (0xE957D0u));

	const float flAspectRatio (1280.0f / 1024.0f/*/4.0f / 3.0f*/);
	m_frustum.GeneratePerspectiveFrustum (/*r_origin/*/GetEyePosition (), GetViewAngles ()/*, 4.0f*/, HalfLifeEngine::SDK::Constants::MapSize/*sv_zmax console variable value - for clients, for YaPB must be Math::GetMinimumValueBetween (sv_zmax.value, m_maximumViewDistance)*/, GetFieldOfView (), flAspectRatio);

//	if (IsHuman ())
	{
//		m_edict->variables.punchAngles = 0.0f;
//		if (m_edict->privateData->GetBasePlayerPointer ()->m_pActiveItem != NULL)
//			static_cast <HalfLifeEngine::SDK::Classes::BasePlayerWeapon *> (m_edict->privateData->GetBasePlayerPointer ()->m_pActiveItem)->m_flAccuracy = 0.0f;

//		m_edict->variables.health = 1000.0f;
//		m_edict->privateData->GetBasePlayerPointer ()->m_punishVelocityModifier = 1.0f;
	}
	if (m_currentWeapon->IsValid () && !m_currentWeapon->IsKnife () && !m_currentWeapon->IsShield () && !m_currentWeapon->IsGrenade () && !m_currentWeapon->IsC4 ())
		const_cast <HalfLifeEngine::SDK::Classes::Edict *> (m_currentWeapon->GetEdict ())->SetPrivateData <int> (HalfLifeEngine::SDK::Constants::OFFSET_WEAPON_CLIP_AMMO, m_currentWeapon->GetMaximumAmmoInClip () - 1u);
#if 0
	if (IsHuman () && g_server->GetYaPBManager ()->GetBotsCount () > 0u)
	{
		YaPB *const bot (g_server->GetYaPBManager ()->GetRandomBot ());
		const float radius (HalfLifeEngine::SDK::Constants::HalfHumanWidth);

		g_drawInterface->DrawSphere (bot->GetOrigin (), radius, Color <> (0u, 255u, 0u));
		GetHumanPointer ()->DrawBox (bot->GetEdict ()->variables.absoluteBoundingBox.mins, bot->GetEdict ()->variables.absoluteBoundingBox.maxs, Color <> (0u, 255u, 0u), 1u);
/*
		const Math::Extent3D ext (m_frustum.GetCalculatdBoundingBox ());
		GetHumanPointer ()->DrawBox (ext.mins, ext.maxs, Color <> (255u, 0u, 0u), 1u);
*//*
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "IsInViewCone(\"%s\"->GetOrigin())=%u", bot->GetName ().GetData (), IsInViewCone (bot->GetOrigin ()));
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "IsInViewCone(\"%s\"->GetOrigin(), fov * (3/4))=%u (3/4)", bot->GetName ().GetData (), IsInViewCone (bot->GetOrigin (), GetFieldOfView () * (3.0f / 4.0f)));
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "IsInViewConeWithRadius(\"%s\"->GetOrigin(), radius=%.1f)=%u", bot->GetName ().GetData (), radius, IsInViewConeWithRadius (bot->GetOrigin (), radius));
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "m_frustum.R_CullBox(\"%s\")=%u, fov=%.2f, CalcFovX()=%.2f, CalcFovY()=%.2f", bot->GetName ().GetData (), m_frustum.R_CullBox (bot->GetEdict ()->variables.absoluteBoundingBox.mins, bot->GetEdict ()->variables.absoluteBoundingBox.maxs), GetFieldOfView (), Math::Frustum::CalcFovX (GetFieldOfView (), flAspectRatio), Math::Frustum::CalcFovY (GetFieldOfView (), flAspectRatio));
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "m_frustum.R_CullPoint(\"%s\")=%u, fov=%.2f, CalcFovX()=%.2f, CalcFovY()=%.2f", bot->GetName ().GetData (), m_frustum.R_CullPoint (bot->GetOrigin ()), GetFieldOfView (), Math::Frustum::CalcFovX (GetFieldOfView (), flAspectRatio), Math::Frustum::CalcFovY (GetFieldOfView (), flAspectRatio));
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "IsInViewCone(\"%s\"->GetOrigin(), 28.48f)=%u", bot->GetName ().GetData (), IsInViewCone (bot->GetOrigin (), 28.48f));

		const Math::Angles2D deviation (((bot->GetOrigin () - GetEyePosition ()).ToAngles2D () - GetViewAngles ()).Clamp ());
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\", deviation2D=[%.2f, %.2f] (yaw cos=%.2f)", bot->GetName ().GetData (), deviation.pitch, deviation.yaw, Math::cosf (Math::DegreeToRadian (deviation.yaw)));
*/
		GetHumanPointer ()->DrawCross3D (bot->GetOrigin (), 15.0f, Color <> (255u, 0u, 0u), 1u, 255u, 1u, false);
		GetHumanPointer ()->DrawCross3D (r_origin, 15.0f, Color <> (255u, 0u, 0u), 1u, 255u, 1u, false);

		const float &fov_x (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetMemory <float> (0x1774E4u));
		const int &glwidth (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetMemory <int> (0xA83B44u));
		const int &glheight (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetMemory <int> (0xA83B04u));
		typedef BOOL (*R_CullBox_t) (Math::Vector3D &mins, Math::Vector3D &maxs);
		const BOOL result ((*HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetFunction <R_CullBox_t> (0x4F760u)) (bot->GetEdict ()->variables.absoluteBoundingBox.mins, bot->GetEdict ()->variables.absoluteBoundingBox.maxs));
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "eng R_CullBox(\"%s\")=%u, fov_x=%.2f, glwidth=%i, glheight=%i.", bot->GetName ().GetData (), result, fov_x, glwidth, glheight);
	}
#endif	// if 0
#if 0
/*	for
	(
		HalfLifeEngine::SDK::Classes::Edict *ent (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_bomb_target"));
		ent->IsValid ();
		ent = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ent, "info_bomb_target")
	)*/
	HalfLifeEngine::SDK::Classes::Edict *ent (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_bomb_target"));
	if (ent->IsValid () || (ent = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_bomb_target"))->IsValid ())
	{
		// vol=0.1, attn=1.5, hear dist=~560.0, lvol=0, rvol=7. (default radius=666.6(*vol=66.6), DIFF=106.6)
		// vol=0.5, attn=1.5, hear dist=~645.0, lvol=0, rvol=8. (default radius=666.6(*vol=333.3), DIFF=21.6)
		// vol=1.0, attn=1.5, hear dist=~656.0, lvol=0, rvol=8. (default radius=666.6(*vol=333.3), DIFF=10.6)

		// vol=1.0, attn=1.0, hear dist=~984.0, lvol=0, rvol=8. (default radius=1000.0(*vol=1000.0), DIFF=16.0)
		// vol=0.5, attn=1.0, hear dist=~967.0, lvol=0, rvol=8. (default radius=1000.0(*vol=1000.0), DIFF=33.0)

		const Math::Vector3D org (ent->GetOrigin ());
//		const float vol (1.0f/*0.5f/*HalfLifeEngine::SDK::Constants::SoundVolume_Normal*/);
//		const float attn (1.0f);
		const float vol (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Spray)->GetValue <float> ());
		const float attn (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <float> ());

		Math::Vector3D directionNormalized (GetOrigin () - org);
			float distance (directionNormalized.GetLength ());
			float rightScale, leftScale;

//			if (GetProfile ()->hasStereo)
			{
				directionNormalized /= distance;	// Normalize.

				const float dot (GetViewAngles ().BuildRightVector () | directionNormalized);

				rightScale = 1.0f + dot;
				leftScale = 1.0f - dot;
			}
//			else
//				rightScale = leftScale = 1.0f;

			distance = 1.0f - distance * (attn / HalfLifeEngine::SDK::Constants::SoundNominalClipDistance);

			// add in distance effect
			float volumeScale (distance * rightScale);
			int rightvol (static_cast <int> (static_cast <unsigned char> (vol * 255.0f) * volumeScale));	// 0-255 volume
			if (rightvol < 0)
				rightvol = 0;

			volumeScale = distance * leftScale;
			int leftvol (static_cast <int> (static_cast <unsigned char> (vol * 255.0f) * volumeScale));	// 0-255 volume
			if (leftvol < 0)
				leftvol = 0;

		GetHumanPointer ()->DrawCross3D (org, 15.0f, Color <> (255u, 0u, 0u), 1u, 255u, 1u, false);
		static float t (0.0f);
		if (t <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
		t = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.4f;
		HalfLifeEngine::Globals::g_halfLifeEngine->EmitSound (ent, HalfLifeEngine::SDK::Constants::SoundChannel_Voice, "weapons/c4_beep1.wav", vol, attn, HalfLifeEngine::SDK::Constants::SoundFlag_None, HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
//		HalfLifeEngine::Globals::g_halfLifeEngine->EmitSound (ent, HalfLifeEngine::SDK::Constants::SoundChannel_Voice, "weapons/c4_beep1.wav", vol, 1.5f, HalfLifeEngine::SDK::Constants::SoundFlag_None, HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
//		HalfLifeEngine::Globals::g_halfLifeEngine->EmitSound (ent, HalfLifeEngine::SDK::Constants::SoundChannel_Voice, "weapons/c4_beep2.wav", vol, 1.0f, HalfLifeEngine::SDK::Constants::SoundFlag_None, HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
//		HalfLifeEngine::Globals::g_halfLifeEngine->EmitSound (ent, HalfLifeEngine::SDK::Constants::SoundChannel_Voice, "weapons/c4_beep3.wav", vol, 0.8f, HalfLifeEngine::SDK::Constants::SoundFlag_None, HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
//		HalfLifeEngine::Globals::g_halfLifeEngine->EmitSound (ent, HalfLifeEngine::SDK::Constants::SoundChannel_Voice, "weapons/c4_beep4.wav", vol, 0.5f, HalfLifeEngine::SDK::Constants::SoundFlag_None, HalfLifeEngine::SDK::Constants::SoundPitch_Normal);
		}
		GetHumanPointer ()->DrawLine (GetOrigin (), org, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "1.5f: radius=%.2f(*vol=%.2f), lvol=%i, rvol=%i, Dist=%.2f", SOUND_ATTENUATION_TO_RADIUS (1.5f), SOUND_ATTENUATION_TO_RADIUS (1.5f) * vol, leftvol, rightvol, GetOrigin ().GetDistance (org));
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "1.0f: radius=%.2f(*vol=%.2f)", SOUND_ATTENUATION_TO_RADIUS (1.0f), SOUND_ATTENUATION_TO_RADIUS (1.0f) * vol);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "0.8f: radius=%.2f(*vol=%.2f)", SOUND_ATTENUATION_TO_RADIUS (0.8f), SOUND_ATTENUATION_TO_RADIUS (0.8f) * vol);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "0.5f: radius=%.2f(*vol=%.2f)", SOUND_ATTENUATION_TO_RADIUS (0.5f), SOUND_ATTENUATION_TO_RADIUS (0.5f) * vol);
	}
#endif	// if 0
//	for (WorldMap::WindingArray_t::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetWorldMap ().m_windings.GetElementNumber (); ++index)
//		g_drawInterface->DrawWinding (*g_server->GetNavigationMeshManager ().GetWorldMap ().m_windings[index], Color <> (255u, 255u, 255u), 255u, 2u, false);
#if 0
	const Math::Vector3D eyePos (GetEyeForwardPosition (200.0f));
	const float crossSize (45.0f);
//	g_server->GetHumanManager ()->GetHostClient ()->DrawCross2D (eyePos, crossSize, Color <> (0u, 0u, 255u), 1u, 255u, 1u, false);
	g_server->GetHumanManager ()->GetHostClient ()->DrawLine (eyePos/* + Math::Vector2D (crossSize, 0.0f)*/, eyePos + Math::Vector2D (crossSize, 0.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 15u, 0u, Color <> (255u, 0u, 0u), 255u, 5u, 1u, false);
	g_server->GetHumanManager ()->GetHostClient ()->DrawLine (eyePos/* + Math::Vector2D (0.0f, crossSize)*/, eyePos + Math::Vector2D (0.0f, crossSize), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 15u, 0u, Color <> (0u, 255u, 0u), 255u, 5u, 1u, false);
#endif	// if 0
#if 0
	/// @warning ONLY FOR CS: CZ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (IsZBot ())
	{
		HalfLifeEngine::SDK::Classes::CSBot *const ZBot (GetEdict ()->privateData->GetCSBotPointer ());
		const Math::Vector3D offset (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::HalfHumanHeight);

		for (unsigned int index (1u); index < ZBot->m_currentPathInformationNumber; ++index)
			g_server->GetHumanManager ()->GetHostClient ()->DrawLine (ZBot->m_path[index - 1u].point + offset, ZBot->m_path[index].point + offset, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 75u, 0u), 250u, 5u, 1u);
/*		static float time (0.0f);
		if (time <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
		time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.1f;
		typedef void (HalfLifeEngine::SDK::Classes::CSBot::*DrawPathFunction_t) (void);
		(ZBot->*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunction <DrawPathFunction_t> (0x2D170)) ();
		}
/*/		HalfLifeEngine::Globals::g_halfLifeEngine->HudMessageFormat
		(
			Math::Vector2D (-1.0f, 0.14f),
			ColorWithAlpha <> (0u, 255u, 0u),
			ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
			HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.1f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

			"ZBot: \"%s\".\n"
			"Path: number = %u, current index = %u,\n"
			"Current state: \"%s\".",

			ZBot->m_name,//GetName ().GetData (),
			ZBot->m_currentPathInformationNumber, ZBot->m_currentPathIndex,
			ZBot->m_currentState->GetName ()
		);
	}
#endif	// if 0/*/
/*	// Initialize voice system....
	{
		static bool isVoiceInitialized (false);

		if (isVoiceInitialized == false)
		{
			InternalAssert (HalfLifeEngine::SDK::Voice::Voice_Init (Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_codec)->GetString (), Console::Server::variableManager.GetVariable (Console::Server::VariableID_Voice_quality)->GetValue <unsigned char> ()));

			isVoiceInitialized = true;
		}
	}
*/
//#if 0
	if (IsHuman () && g_server->GetYaPBManager ()->GetBotsCount () > 0u)
	{
		YaPB *const bot (g_server->GetYaPBManager ()->GetRandomBot ());

		if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
		{
			if (!/*HalfLifeEngine::SDK::Voice::*/bot->GetChatterManager ().Voice_IsRecording ())
			{
				const char *pUncompressedFile (NULL);
				const char *pDecompressedFile (NULL);
				const char *pInputFile (NULL);

				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_recordtofile)->GetValue <bool> ())
				{
					// Set default filenames.
					pUncompressedFile = "voice_micdata.wav";
					pDecompressedFile = "voice_decompressed.wav";
				}

				// Wave file should be: Signed 16 bit PCM, 1 channel, 8000 Hz
				if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_inputfromfile)->GetValue <bool> ())
					pInputFile = "voice_input.wav";	// Set default filename.
				else if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_inputfromfile)->GetString ().IsEmpty () && !isdigit (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_inputfromfile)->string[0u]))
					pInputFile = Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Voice_inputfromfile)->string;

//				HalfLifeEngine::SDK::Voice::Voice_RecordStart (pUncompressedFile, pDecompressedFile, pInputFile);
				bot->GetChatterManager ().Voice_RecordStart (pUncompressedFile, pDecompressedFile, pInputFile);
			}
			else
				SV_ParseVoiceData (bot, false);
		}
		else if (/*HalfLifeEngine::SDK::Voice::*/bot->GetChatterManager ().Voice_IsRecording ())
		{
			SV_ParseVoiceData (bot, true);	// Send the final data packet....

//			HalfLifeEngine::SDK::Voice::Voice_RecordStop ();
			bot->GetChatterManager ().Voice_RecordStop ();
		}
	}
//#endif	// if 0
	const unsigned short currentWaypointIndex (g_server->GetWaypointManager ().GetLocationsTable ().GetNearestWaypoint (GetOrigin ()));

	m_currentWaypoint = currentWaypointIndex != InvalidWaypointIndex ? g_server->GetWaypointManager ().GetWaypoint (currentWaypointIndex) : NULL;

	LastNavigationAreaUpdate ();
//	SoundSimulateUpdate ();

//	m_hostageManager.ShowHostages ();

//	return;
/*
	if (m_currentWeapon->IsValid () && !m_currentWeapon->IsKnife () && !m_currentWeapon->IsShield () && !m_currentWeapon->IsGrenade () && !m_currentWeapon->IsC4 ())
		const_cast <HalfLifeEngine::SDK::Classes::Edict *> (m_currentWeapon->GetEdict ())->SetPrivateData <int> (HalfLifeEngine::SDK::Constants::OFFSET_WEAPON_CLIP_AMMO, m_currentWeapon->GetMaximumAmmoInClip () - 1u);
*/
//	if (IsYaPB ())
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fixAngle=%i, angles: view=[%.2f, %.2f, %.2f], model=[%.2f, %.2f, %.2f], velocity=[%.2f, %.2f, %.2f].", m_edict->variables.fixAngle, GetViewAngles ().pitch, GetViewAngles ().yaw, GetViewAngles ().roll, m_edict->variables.modelAngles.pitch, m_edict->variables.modelAngles.yaw, m_edict->variables.modelAngles.roll, m_edict->variables.angleVelocity.pitch, m_edict->variables.angleVelocity.yaw, m_edict->variables.angleVelocity.roll);
	if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
//		DoAirStrafes (GetHostPointer ());
//	if (0)
	{
//	HalfLifeEngine::Globals::g_halfLifeEngine->DirectorHudMessage (Math::Vector2D (-1.0f, 0.65f), Color <> (0u, 160u, 0u), HalfLifeEngine::SDK::Constants::HUDMessageEffect_2, 6.0f, 3.0f, 0.1f, 1.5f, "Hello, MR. EDWARD!!!");
#if 0
	if (IsYaPB () && m_lastNavigationArea != NULL)
	{
		Math::Vector3D m_bendLineOfSightApproachPoints[NavigationMesh::NavigationArea::MAX_APPROACH_AREAS];
		unsigned char m_bendCount (0u);
			Math::Vector3D approachPoints[NavigationMesh::NavigationArea::MAX_APPROACH_AREAS];

			for (NavigationMesh::NavigationArea::ApproachArray::IndexType_t index (0u); index < m_lastNavigationArea->GetApproachInfoCount (); ++index)
			{
				const NavigationMesh::NavigationArea::ApproachInfo_t &info (m_lastNavigationArea->GetApproachInfo (index));

				InternalAssert (info.here != NULL);
				InternalAssert (info.prev != NULL);

				if (info.prevToHereHow > NavigationMesh::GO_WEST)
					approachPoints[index] = info.here->GetCenter ();
				else
				{
					info.prev->ComputePortal (info.here, static_cast <NavigationMesh::NavDirType_t> (info.prevToHereHow), approachPoints[index]);

					approachPoints[index].z = info.here->GetZ (approachPoints[index]);
				}

				HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (approachPoints[index], 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 0u, 255u), 1u);

//				if (!static_cast <YaPB *> (this)->BendLineOfSight (approachPoints[index], m_bendLineOfSightApproachPoints[m_bendCount]))
				void *const address (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ()) + 0x324B0u));
				typedef bool (HalfLifeEngine::SDK::Classes::CSBot::*BendLineOfSightFunction_t) (const Math::Vector3D &source, const Math::Vector3D &destination, Math::Vector3D &result);
				if (!(*GetEdict ()->privateData->GetCSBotPointer ()->*reinterpret_cast <BendLineOfSightFunction_t &> (address)) (GetOrigin (), approachPoints[index], m_bendLineOfSightApproachPoints[m_bendCount]))
					continue;

//				m_bendLineOfSightApproachPoints[m_bendCount] = approachPoints[index];

				HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (m_bendLineOfSightApproachPoints[m_bendCount], 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

				++m_bendCount;
			}
	}
#endif	// if 0
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fixAngle=%i, modelAngles=[%.2f, %.2f, %.2f], angleVelocity=[%.2f, %.2f, %.2f].", m_edict->variables.fixAngle, m_edict->variables.modelAngles.pitch, m_edict->variables.modelAngles.yaw, m_edict->variables.modelAngles.roll, m_edict->variables.angleVelocity.pitch, m_edict->variables.angleVelocity.yaw, m_edict->variables.angleVelocity.roll);
/*		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), GetEyeForwardPosition (200.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_None, m_edict, traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (200.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_None, m_edict, traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
		const float distanceToLookAt (GetOrigin ().GetDistance (traceResult.endPosition));
		const float first (Math::tanf ((Math::fabsf (m_edict->GetPunchAngles ().yaw) + Math::fabsf (m_edict->GetPunchAngles ().pitch)) * Math::Pi / 360.0f) * (distanceToLookAt + distanceToLookAt / 4.0f));
		const float second (Math::tanf (Math::sqrtf (Math::DegreeToRadian (Math::fabsf (m_edict->GetPunchAngles ().yaw)) * Math::DegreeToRadian (Math::fabsf (m_edict->GetPunchAngles ().yaw)) + Math::DegreeToRadian (Math::fabsf (m_edict->GetPunchAngles ().pitch)) * Math::DegreeToRadian (Math::fabsf (m_edict->GetPunchAngles ().pitch)))) * distanceToLookAt);
		if (first != 0.0f || second != 0.0f)
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "1=%f, 2=%f", first, second);
*/
//#if 0
//		static float t (0.0f);
//		if (t <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
//		t = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.0f;
		for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
		{
			/*const */YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

			if (!bot->IsAlive ())
				continue;
//#if 0
//			bot->StudioSetupBones ();

			const HalfLifeEngine::SDK::Structures::StudioHeader_t *const studioHeader (bot->GetEdict ()->GetModelPointer ());

			InternalAssert (studioHeader != NULL)

			unsigned char hitBoxIndex (/*11);//*/Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <unsigned char> ());

			if (hitBoxIndex >= studioHeader->numhitboxes)
			{
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "hitBoxIndex(%u) >= (%u)studioHeader->numhitboxes !!!\n", hitBoxIndex, studioHeader->numhitboxes);

				break;
			}

			const HalfLifeEngine::SDK::Structures::mstudiobbox_t *const pbboxes (reinterpret_cast <const HalfLifeEngine::SDK::Structures::mstudiobbox_t *> (reinterpret_cast <const unsigned char *> (studioHeader) + studioHeader->hitboxindex));

			HalfLifeEngine::SDK::Structures::ClientEntity_t *clEnt ((*HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetEntityByIndex) (bot->GetEdict ()->GetIndex ()));

			InternalAssert (clEnt != NULL);

//			for (; hitBoxIndex < studioHeader->numhitboxes; ++hitBoxIndex)
			{
			const HalfLifeEngine::SDK::Structures::mstudiobbox_t &bbox (pbboxes[hitBoxIndex]);
			const Math::Vector3D &bbmin (bbox.boundingBox.mins);
			const Math::Vector3D &bbmax (bbox.boundingBox.maxs);
			Math::Vector3D box[8u];

			box[0u].x = bbmin.x;
			box[0u].y = bbmax.y;
			box[0u].z = bbmin.z;

			box[1u] = bbmin;

			box[2u].x = bbmax.x;
			box[2u].y = bbmax.y;
			box[2u].z = bbmin.z;

			box[3u].x = bbmax.x;
			box[3u].y = bbmin.y;
			box[3u].z = bbmin.z;

			box[4u] = bbmax;

			box[5u].x = bbmax.x;
			box[5u].y = bbmin.y;
			box[5u].z = bbmax.z;

			box[6u].x = bbmin.x;
			box[6u].y = bbmax.y;
			box[6u].z = bbmax.z;

			box[7u].x = bbmin.x;
			box[7u].y = bbmin.y;
			box[7u].z = bbmax.z;

			bot->StudioSetupBones (static_cast <char> (bbox.bone));

			const HalfLifeEngine::SDK::TypeDefinitions::BoneTransformMatrix_t &boneTransformMatrix (HalfLifeEngine::Globals::g_halfLifeEngine->GetBoneTransformMatrix ());
			const Math::Vector3D transformedBox[8u] =
			{
				box[0u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[1u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[2u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[3u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[4u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[5u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[6u].GetTransformed (boneTransformMatrix[bbox.bone]),
				box[7u].GetTransformed (boneTransformMatrix[bbox.bone])
			};
//			/*const */Math::Vector3D center ((transformedBox[0u] + transformedBox[1u] + transformedBox[2u] + transformedBox[3u] + transformedBox[4u] + transformedBox[5u] + transformedBox[6u] + transformedBox[7u]) / GET_STATIC_ARRAY_SIZE (transformedBox));
			/*const */Math::Vector3D center (bbox.boundingBox.GetCenter ().GetTransformed (boneTransformMatrix[bbox.bone]));

const float distance (center.GetDistance (GetEyePosition ()));
const float factor1 (/*1.0f*/Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <float> ());
const float factor2 (/*1.0f*/Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <float> ());

const float offset (3.8f/*HalfLifeEngine::SDK::Constants::HumanWidth + 1.0f*/);
const float distanceSquared (MATH_GET_SQUARED (distance));
const float distanceSquaredWithOffset (Math::sqrtf (distanceSquared + MATH_GET_SQUARED (offset)));
const float coneMinimum (distanceSquared / MATH_GET_SQUARED (distanceSquaredWithOffset));
const float distanceSquaredWithOffset2 (distanceSquared + MATH_GET_SQUARED (offset));
const float coneMinimum2 (distanceSquared / distanceSquaredWithOffset2);

InternalAssert (m_currentWeapon->IsValid ());
//HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" dist=%.2f, curCone=%f, conMin=%f, conMin2=%f (eq=%u, diff=%f), ZBotIdealCone=%.2f, OTHERIdealCone=%f.", GetName ().GetData (), distance, GetShootingConeDeviationWithPunchAngles (center), coneMinimum, coneMinimum2, coneMinimum == coneMinimum2, Math::fabsf (coneMinimum - coneMinimum2), 1.0f - Math::cosf (Math::atan2f (1.0f, (m_currentWeapon->IsSniperRifle () ? 16.0f : 16.0f + 16.0f) / distance)), 1.0f - Math::cosf (Math::atan2f (factor1, factor2 / distance)));
//HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot \"%s\" dist=%.2f, curCone=%f, conMin=%f, conMin2=%f (eq=%u, diff=%f).", GetName ().GetData (), distance, GetShootingConeDeviationWithPunchAngles (center), coneMinimum, coneMinimum2, coneMinimum == coneMinimum2, Math::fabsf (coneMinimum - coneMinimum2));
/*
//			{
		Math::Vector3D vPredictedOrigin = clEnt->origin;//center;
		double fTargetTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();//HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetClientTime() - 0.0f;//cvar.predtime;
		int iHistIndex = clEnt->current_position + HISTORY_MAX;
		int iMaxIndex = iHistIndex - HISTORY_MAX;
		int iFoundHistIndex = 0;
		bool bFoundInHistory = false;

		while ( !bFoundInHistory  && iHistIndex > iMaxIndex)
		{
			if ((fTargetTime < (clEnt->ph[iHistIndex % HISTORY_MAX].animtime)) &&  (fTargetTime > (clEnt->ph[(iHistIndex-1) % HISTORY_MAX].animtime)) )
			{
				iFoundHistIndex = (iHistIndex-1) % HISTORY_MAX;
				bFoundInHistory = true;
			}
			iHistIndex--;
		}

		Math::Vector3D vFromOrigin , vToOrigin , vDeltaOrigin;
		float fFromTime,fToTime,fTimeFraction;

		if (bFoundInHistory)
		{
			int iFoundHistIndexNext = (iFoundHistIndex+1) % HISTORY_MAX;
			vFromOrigin = clEnt->ph[iFoundHistIndex].origin;
			vToOrigin = clEnt->ph[iFoundHistIndexNext].origin;
			fFromTime = clEnt->ph[iFoundHistIndex].animtime;
			fToTime = clEnt->ph[iFoundHistIndexNext].animtime;
			fTimeFraction = 0;
			if ( (fToTime - fFromTime) != 0 )
				fTimeFraction = (fTargetTime - fFromTime) / (fToTime - fFromTime);
			vDeltaOrigin = vToOrigin - vFromOrigin;
			vPredictedOrigin = vFromOrigin + (vDeltaOrigin * fTimeFraction);
		}
		else
		{
			int iCurHInd = clEnt->current_position;
			int iPrevHInd = ((iCurHInd - 1) + HISTORY_MAX) % HISTORY_MAX;
			vFromOrigin = clEnt->ph[iCurHInd].origin;
			vToOrigin = vFromOrigin + (vFromOrigin - clEnt->ph[iPrevHInd].origin);
			fFromTime = clEnt->ph[iCurHInd].animtime;
			fToTime = fFromTime + (fFromTime - clEnt->ph[iPrevHInd].animtime);
			fTimeFraction = 0;
			if ( (fToTime - fFromTime) != 0 )
				fTimeFraction = (fTargetTime - fFromTime) / (fToTime - fFromTime);
			if (fTimeFraction > 0.0f)//cvar.predmax)
				fTimeFraction = 0.0f;//cvar.predmax;
			vDeltaOrigin = vToOrigin - vFromOrigin;
			vPredictedOrigin = vFromOrigin + (vDeltaOrigin * fTimeFraction);
		}
//		center = vPredictedOrigin;
//			}*/
//			center += bot->GetVelocity ().AsVector2D () * (HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime * Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <float> ()/*1.5f*/);

			GetHumanPointer ()->DrawBox (transformedBox, Color <> (0u, 255u, 0u), 1u);
			GetHumanPointer ()->DrawCross3D (center, 5.0f, Color <> (255u, 0u, 0u), 1u, 255u, 1u, false);
//			GetHumanPointer ()->DrawCross3D (bot->GetEyePosition (), 5.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (255u, 0u, 0u), 1u);

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "bot velocity=[%.2f, %.2f], base velocity=[%.2f, %.2f].\n", bot->GetVelocity ().x, bot->GetVelocity ().y, bot->GetEdict ()->variables.baseVelocity.x, bot->GetEdict ()->variables.baseVelocity.y);
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "bbox: [bone=%i, group=%i].\n", bbox.bone, bbox.group);
/*
	if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <unsigned char> () == 0)
		center.z += Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <float> ();
*/
			// 1337 aiming
//			const float posun (bot->GetOrigin ().GetDistance (GetOrigin ()) * (1.0f / (Math::Pi * Math::Pi * 1000.0f)));
//			const Math::Angles2D offset (-posun);
//			const Math::Angles2D offset (-posun, 0.0f);

		// red = current view angles
		GetHumanPointer ()->DrawLine (GetEyePosition (), GetEyeForwardPositionWithPunchAngles (300.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, bot->IsShootableThruObstacle (m_edict->GetEyePosition (), center) ? Color <> (0u, 255u, 0u) : Color <> (255u, 0u, 0u), 250u, 5u, 1u);

//m_edict->variables.punchAngles = 0.0f;
//#if 0
			Math::Angles2D aimAngles ((/*vPredictedOrigin/*/center/*/bot->GetEyePosition ()*/ - m_edict->GetEyePosition ()).ToAngles2D ()/* + offset*/);
			{
			aimAngles.pitch = -aimAngles.pitch;
			aimAngles -= m_currentWeapon->GetSpreadOffsetAngles (aimAngles)/* * 2.0f*/;
			aimAngles.pitch = -aimAngles.pitch;
			aimAngles.Clamp ();
			}
			{
//			aimAngles -= GetModifiedPunchAngles ();
			Math::Angles3D punchAngles (GetPunchAngles ());

			HalfLifeEngine::Utilities::DropPunchAngles (punchAngles, HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime);

			punchAngles.pitch *= -2.0f;	// Invert for engine (adjust the punch angle pitch to aim correctly).
			punchAngles.yaw *= 2.0f;

			aimAngles -= punchAngles.AsAngles2D ();	// Compensate recoil.
			aimAngles.Clamp ();
			}
			// Set the body and view angles....
			m_edict->variables.modelAngles = aimAngles;
			m_edict->variables.modelAngles.pitch = -m_edict->variables.modelAngles.pitch;	// Invert for engine (adjust the view angle pitch to aim correctly).
			m_edict->variables.fixAngle = HalfLifeEngine::SDK::Constants::FixAngleType_Absolute;
			#if 0
			if (/*!(GetEdict ()->variables.oldButtons & HalfLifeEngine::SDK::Constants::IN_ATTACK) && */(GetEdict ()->variables.buttons & HalfLifeEngine::SDK::Constants::IN_ATTACK))
			{
		GetHumanPointer ()->DrawLine (GetOrigin (), m_edict->variables.modelAngles.BuildForwardVector () * 300.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

	const float MY_vecSpread (GetCurrentWeapon ()->GetSpreadOffset ());
	const Math::Vector2D MY_result (GetCurrentWeapon ()->GetSpreadOffset2D ());
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%s::AliveThink(): MY_vecSpread=%.5f, random_seed=%u, MY_result=[%.10f, %.10f].\n", GetName ().GetData (), MY_vecSpread, GetEdict ()->privateData->GetBasePlayerPointer ()->random_seed, MY_result.x, MY_result.y);
			}
			#endif	// if 0
//#endif	// if 0
/*{
	const unsigned char approximateBaseDamage (static_cast <unsigned char> (bot->GetApproximateBaseDamage (m_edict->GetEyePosition (), center) * HalfLifeEngine::SDK::Constants::PlayerBodyPartHitMultiplications[HalfLifeEngine::SDK::Constants::PlayerBodyPart_Head]));

	GetHumanPointer ()->DrawLine (GetOrigin (), center, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, approximateBaseDamage == 0u ? Color <> (255u, 0u, 0u) : Color <> (0u, 255u, 0u), 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "approximate damage to \"%s\" is %u.", GetName ().GetData (), approximateBaseDamage);
}*/
			}
//#endif	// if 0
//			clEnt->mouth.mouthopen = g_randomNumberGenerator.GetValueBetween <unsigned char> ();
/*
			// draw history:
			{
				int historyIndex = clEnt->current_position;
				for(int i=0;i<HISTORY_MAX;++i)
				{
//NOT WORK!!!					gDrawFilledBoxAtLocation(g_server->GetHumanManager ()->GetHostClient (), clEnt->ph[historyIndex].origin,0xFFFFFFFF,1);
					HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (clEnt->ph[historyIndex].origin - Math::Vector3D (0.0f, 0.0f, 36.0f), clEnt->ph[historyIndex].origin + Math::Vector3D (0.0f, 0.0f, 36.0f + 10.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

					// step back
					if (--historyIndex < 0)
						historyIndex = HISTORY_MAX-1;
				}
			}
*/
			break;
		}
		}
//#endif	// if 0
/*	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		const Math::Vector3D destination (GetEyeForwardPositionWithPunchAngles (150.0f));
		float z;
		Math::Vector3D normal;

		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), destination, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);

		if (NavigationMesh::GetGroundHeight (destination, &z, &normal))
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (Math::Vector3D (destination.x, destination.y, z), 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "GetGroundHeight(): true, z=%.2f, normal=%s, Z diff from start=%.2f.\n", z, normal.ToString (), z - destination.z);
		}
		else
			HalfLifeEngine::Globals::g_halfLifeEngine->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "GetGroundHeight(): false.\n");
	}
/*/
#if 0
	{
	static float t (0.0f);
	if (t <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
	g_server->GetHumanManager ()->GetHostClient ()->DrawHudString (100, 100, false, false, Color <> (120,120,255), "**** DEBUG BUILD ****");
	g_server->GetHumanManager ()->GetHostClient ()->DrawConsoleText (640, 512, "**** DEBUG BUILD ****");

	t = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 1.0f;
	}
	}
#endif	// if 0
/*
//	const Math::Vector3D hull (HalfLifeEngine::SDK::Constants::VEC_HULL_MAX);
//	const Math::Vector3D hull (HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX);
	const Math::Vector3D hull (NavigationMesh::GenerationHull);
//	const Math::Vector3D hull (Math::Vector3D (100.0f, 100.0f, 36.0f));
	RayTrace trace
	(
		m_edict->GetEyePosition (),
		GetEyeForwardPositionWithPunchAngles (200.0f)/*,

		RayTrace::Flag_Face | RayTrace::Flag_Texture,
		RayTrace::Type_AxisAlignedBox,
		hull*//*
	);

	trace.Execute ();

	const RayTrace::Result_t &traceResult (trace.GetResult ());

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allSol=%s,startSol=%s,open=%s,wtr=%s,fract=%.2f,planeNrm=[%.2f,%.2f,%.2f].\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.plane.normal.x, traceResult.plane.normal.y, traceResult.plane.normal.z);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
/*	HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult.endPosition - hull, traceResult.endPosition + hull, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
	const Math::Plane plane (traceResult.plane.normal, traceResult.plane.distance);
	Math::Plane::Points_t points;

	plane.GetWinding (traceResult.endPosition, points, 50.0f);

	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), (points[0] + points[1] + points[2] + points[3]) / 4.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[0], points[1], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[1], points[2], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[2], points[3], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[3], points[0], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
*//*
	if (traceResult.faceNum > -1)
	{
	const HalfLifeEngine::SDK::Structures::DFace_t &face (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[static_cast <unsigned short> (traceResult.faceNum)]);
	const HalfLifeEngine::SDK::Structures::DPlane_t &wplane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (face.planeNum)]);
	Math::Vector3D corner, center;

		// face MAY be walkable, check if it's not a top or side face (useless in the mesh...)
		for (unsigned short cornerIndex (0u); cornerIndex < face.surfEdgesNum; ++cornerIndex)
		{
			corner = g_server->GetNavigationMeshManager ().GetWorldMap ().GetFaceCornerByIndex (face, cornerIndex);	// get each corner successively...

			center += corner;	// add this corner for the center's computation
		}

		center /= face.surfEdgesNum;	// Final computation center.

	HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (center, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (255u, 0u, 0u), 1u);

	const Math::Plane plane (wplane.normal, wplane.distance);
	Math::Plane::Points_t points;

	plane.GetWinding (center, points, 50.0f);

	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), (points[0] + points[1] + points[2] + points[3]) / 4.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[0], points[1], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[1], points[2], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[2], points[3], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[3], points[0], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "EXTRA: face[%u]: side=%u, plane[%u]: [dst=%.2f, %.2f,%.2f,%.2f].\n", traceResult.faceNum, face.side, traceResult.plane.distance, traceResult.plane.normal.x, traceResult.plane.normal.y, traceResult.plane.normal.z);
	}
/*
	if (m_edict->variables.sequence >= 0 && m_edict->variables.sequence < GET_STATIC_ARRAY_SIZE (HalfLifeEngine::SDK::Constants::RegularSequencesInformation))
	{
	DynamicString sequenses;

	if (HalfLifeEngine::SDK::Constants::RegularSequencesInformation[m_edict->variables.sequence] & HalfLifeEngine::SDK::Constants::RegularSequence_Shoot)
		sequenses += "shoot";

	if (HalfLifeEngine::SDK::Constants::RegularSequencesInformation[m_edict->variables.sequence] & HalfLifeEngine::SDK::Constants::RegularSequence_Reload)
		sequenses += sequenses.IsEmpty () ? "reload" : " + reload";

	if (HalfLifeEngine::SDK::Constants::RegularSequencesInformation[m_edict->variables.sequence] & HalfLifeEngine::SDK::Constants::RegularSequence_Die)
		sequenses += sequenses.IsEmpty () ? "die" : " + die";

	if (HalfLifeEngine::SDK::Constants::RegularSequencesInformation[m_edict->variables.sequence] & HalfLifeEngine::SDK::Constants::RegularSequence_Throw)
		sequenses += sequenses.IsEmpty () ? "throw" : " + throw";

	if (HalfLifeEngine::SDK::Constants::RegularSequencesInformation[m_edict->variables.sequence] & HalfLifeEngine::SDK::Constants::RegularSequence_ArmC4)
		sequenses += sequenses.IsEmpty () ? "arm C4" : " + arm C4";

	if (HalfLifeEngine::SDK::Constants::RegularSequencesInformation[m_edict->variables.sequence] & HalfLifeEngine::SDK::Constants::RegularSequence_OpenShield)
		sequenses += sequenses.IsEmpty () ? "shield open" : " + shield open";

	if (HalfLifeEngine::SDK::Constants::RegularSequencesInformation[m_edict->variables.sequence] & HalfLifeEngine::SDK::Constants::RegularSequence_ClosedShield)
		sequenses += sequenses.IsEmpty () ? "shield closed" : " + shield closed";

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "cur sequense = %i, sequenses: \"%s\", gaitSequence=%i.\n", m_edict->variables.sequence, sequenses.GetData (), m_edict->variables.gaitSequence);
	}
	else
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "cur sequense = %i, max index = %u, gaitSequence=%i.\n", m_edict->variables.sequence, GET_STATIC_ARRAY_SIZE (HalfLifeEngine::SDK::Constants::RegularSequencesInformation), m_edict->variables.gaitSequence);
*/
/*	if (IsZBot ())
	{
		const HalfLifeEngine::SDK::Constants::TeamID_t team (GetRealTeam ());
		const ColorWithAlpha <> color (team == HalfLifeEngine::SDK::Constants::TeamID_Terrorist ? ColorWithAlpha <> (255u, 70u, 0u) : ColorWithAlpha <> (0u, 70u, 255u));
		HalfLifeEngine::Globals::g_halfLifeEngine->HudMessageFormat
		(
			Math::Vector2D (-1.0f, 0.14f),
			color,
			ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
			HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, g_server->GetYaPBManager ()->GetDebugBotUpdateDelay (), 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

			"[%s]\n"
			"[%s] [%s] [%s]\n"
			"[%s]  [%s]",

			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_FORWARD)   ? "W"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVELEFT)  ? "A"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_BACK)      ? "S"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_MOVERIGHT) ? "D"    : "-",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_DUCK)      ? "DUCK" : "----",
			(m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_JUMP)      ? "JUMP" : "----"
		);

/*		const HalfLifeEngine::SDK::Classes::Bot::Profile *const profile (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Classes::Bot::Profile *const> (static_cast <HalfLifeEngine::SDK::Constants::EntityPrivateDataOffset_t> (HalfLifeEngine::SDK::Constants::Offset_Bot_Profile - HalfLifeEngine::SDK::Constants::Offset_Player_DifferenceBetweenOriginalAndConditionZero)));

		InternalAssert (profile != NULL);

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "ZBot \"%s\" profile: {name: \"%s\", aggression=%.2f, skill=%.2f, teamwork=%.2f, isUNKNOWN=%u}.\n", GetName ().GetData (), profile->name, profile->aggression, profile->skill, profile->teamwork, profile->isUNKNOWN);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "ZBot \"%s\" is looking at: \"%s\".\n", GetName ().GetData (), m_edict->GetPrivateDataReference <char *> (static_cast <HalfLifeEngine::SDK::Constants::EntityPrivateDataOffset_t> (HalfLifeEngine::SDK::Constants::Offset_CSBot_LookAtString - HalfLifeEngine::SDK::Constants::Offset_Player_DifferenceBetweenOriginalAndConditionZero)));
	}*/
#if 0
	const float searchRadius (HalfLifeEngine::SDK::Constants::MaximumPlayerUseDistance);
	Math::Vector3D hostageOrigin;

	for (HalfLifeEngine::SDK::Classes::Edict *m_currentHostageToRescue = HalfLifeEngine::Globals::g_halfLifeEngine->FindEntityInSphere (NULL/*m_currentHostageToRescue*/, GetOrigin (), searchRadius); m_currentHostageToRescue->IsValid (); m_currentHostageToRescue = HalfLifeEngine::Globals::g_halfLifeEngine->FindEntityInSphere (m_currentHostageToRescue, GetOrigin (), searchRadius))
	{
		if (!m_currentHostageToRescue->IsHostage ())
			continue;

		hostageOrigin = m_currentHostageToRescue->GetHostageOrigin ();

		// never pickup dead hostage
		// check if line of sight to object is not blocked (i.e. visible) AND if bot is not using grenade
//		if (!m_currentHostageToRescue->IsHostageAlive () || !ItemIsVisible (hostageOrigin = m_currentHostageToRescue->GetHostageOrigin (), m_currentHostageToRescue, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters))
		if (!m_currentHostageToRescue->IsHostageAlive ())
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Hostage %u is dead! (health=%i, dead flag=%i)\n", m_currentHostageToRescue->GetIndex (), m_currentHostageToRescue->GetHealth (), m_currentHostageToRescue->variables.deadValue);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), hostageOrigin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u);

			continue;
		}
		if (!ItemIsVisible (hostageOrigin/* = m_currentHostageToRescue->GetHostageOrigin ()*/, m_currentHostageToRescue, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters))
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Hostage %u is not visible! (hit=%s, frct=%.2f)\n", m_currentHostageToRescue->GetIndex (), m_traceResult.hitEntity != NULL ? m_traceResult.hitEntity->GetClassName ().GetData () : "NULL", m_traceResult.fraction);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), m_traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 255u, 255u), 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), hostageOrigin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 0u, 255u), 250u, 5u, 1u);

			continue;
		}

		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), hostageOrigin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
	}
#endif	// if 0
#if 0
//	const YaPB::AreaMapZonesArrays &zones (YaPB::s_bombPlaceAreaMapZones);
//	const YaPB::AreaMapZonesArray &zones (YaPB::s_hostageRescueAreaMapZones);
	const YaPB::AreaMapZonesArray &zones (YaPB::s_hostageAreaMapZones);

	InternalAssert (!zones.IsEmpty ());

	for (unsigned short index (0u); index < zones.GetElementNumber (); ++index)
	{
		if (zones[index].origin.GetDistanceSquared (GetOrigin ()) > MATH_GET_SQUARED (400.0f))
			continue;

		for (unsigned char index2 (0u); index2 < zones[index].containingNavigationUnits.GetElementNumber (); ++index2)
			zones[index].containingNavigationUnits[index2]->Draw ();
/*
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (zones[index].absoluteBoundingBox.mins, zones[index].absoluteBoundingBox.maxs, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);

	static float time (0.0f);
	static Math::Vector3D pointOrigin;
	if (time <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
	{
		pointOrigin = YaPB::GetRandomPositionInAreaZone (zones[index]);

		time = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 2.0f;
	}
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), pointOrigin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (pointOrigin, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
*/
}
#endif	// if 0
//#if 0
	if (!IsHost ())
		return;
#if 0
	for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
	{
		const YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

		if (!bot->IsAlive ())
			continue;

		const unsigned short yawAngleAbsolute (GetAbsoluteFieldOfView (bot->GetOrigin ()));
		const float radiansAbsolute (Math::DegreeToRadian (yawAngleAbsolute));	// degrees to radians
		float absoluteCosine, absoluteSine;
		const float yawAngle (GetFieldOfView (bot->GetOrigin ()));
		const float radians (Math::DegreeToRadian (yawAngle));	// degrees to radians
		float cosine, sine;

		Math::GetCosineSine (radiansAbsolute, absoluteCosine, absoluteSine);
		Math::GetCosineSine (radians, cosine, sine);

		// move.x is percentage (0 to 1) of forward speed,
		// move.y is percentage (0 to 1) of side speed.
		const Math::Vector2D moveAbsolute (Math::Vector2D (absoluteCosine, absoluteSine).Normalize ());
		const Math::Vector2D move (Math::Vector2D (cosine, sine).Normalize ());

		const Math::Vector3D directionOld ((bot->GetOrigin () - (GetOrigin () + GetVelocity () * HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->frameTime)).Normalize ());
		Math::Angles3D moveAngles (directionOld.ToAngles3D ());	// Adjust bots ideal angles to face an absolute vector

		moveAngles.pitch *= -1.0f;	// invert for engine

		const float yawAngle1 (GetFieldOfView (moveAngles.yaw));
		const float radians1 (Math::DegreeToRadian (yawAngle));	// degrees to radians

		Math::GetCosineSine (radians1, cosine, sine);

		const Math::Vector2D move1 (Math::Vector2D (cosine, sine).Normalize ());
		const float maximumSpeed (250.0f);
		const float speedMove (maximumSpeed * move1.x), speedStrafe (maximumSpeed * -move1.y);

		const Math::Vector2D move2 (cosine, sine);
   float SpeedForward = maximumSpeed * move2.x;
   float SpeedSide = maximumSpeed * move2.y;
   SpeedForward -= 0.0f * move2.y;
   SpeedSide += 0.0f * move2.x;

		HalfLifeEngine::Globals::g_halfLifeEngine->HudMessageFormat
		(
			Math::Vector2D (-1.0f, 0.14f),
			ColorWithAlpha <> (0u, 255u, 0u),
			ColorWithAlpha <> (g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), g_randomNumberGenerator.GetValueBetween <unsigned char> (230u, 255u), 200u),
			HalfLifeEngine::SDK::Constants::HUDMessageEffect_None, 2.5f, 0.0f, 0.0f, 0.1f, HalfLifeEngine::SDK::Constants::HUDMessageChannel_AutoChoose,

			"yawAngleAbsolute=%u,\n"
			"radiansAbsolute=%.2f,\n"
			"yawAngle=%.2f,\n"
			"radians=%.2f,\n"
			"move absolute multiplier=[%.2f, %.2f],\n"
			"move multiplier=[%.2f, %.2f],\n"
			"\n"
			"yawAngle1=%.2f,\n"
			"radians1=%.2f,\n"
			"move multiplier1=[%.2f, %.2f],\n"
			"speed: [move=%.2f, strafe=%.2f],\n"
			"\n"
			"move multiplier2: [%.2f, %.2f],\n"
			"speed1: [move=%.2f, strafe=%.2f],\n"
			"\n"
			"view angles=[%.2f, %.2f, %.2f],\n"
			"model angles=[%.2f, %.2f, %.2f],\n"
			"move angles=[%.2f, %.2f, %.2f]\n"
			"\n"
			" [%s]\n"
			"[%s] [%s] [%s]",

			yawAngleAbsolute,
			radiansAbsolute,
			yawAngle,
			radians,
			moveAbsolute.x, moveAbsolute.y,
			move.x, move.y,

			yawAngle1,
			radians1,
			move1.x, move1.y,
			speedMove, speedStrafe,

			move2.x, move2.y,
			SpeedForward, SpeedSide,

			GetViewAngles ().pitch, GetViewAngles ().yaw, GetViewAngles ().roll,
			m_edict->variables.modelAngles.pitch, m_edict->variables.modelAngles.yaw, m_edict->variables.modelAngles.roll,
			moveAngles.pitch, moveAngles.yaw, moveAngles.roll,

			move1.x >  Math::RawEpsilon ? "W" : "-",
			move1.y >  Math::RawEpsilon ? "A" : "-",
			move1.x < -Math::RawEpsilon ? "S" : "-",
			move1.y < -Math::RawEpsilon ? "D" : "-"
		);
	}
#endif	// if 0
//	for (unsigned int index (0u); index < g_server->GetZBotManager ()->m_mapZonesNumber; ++index)	// CZ!!!!!!
//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), g_server->GetZBotManager ()->m_mapZones[index].entity->m_variables->containingEntity->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "m_lastSeenEnemyTimestamp=%f.\n", g_server->GetZBotManager ()->m_lastSeenEnemyTimestamp - HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ());
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "g_server->GetGameRules ()->GetRoundRemainingTime ()=%f.\n", g_server->GetGameRules ()->GetRoundRemainingTime ());
#if 0
	for (HalfLifeEngine::SDK::Classes::Edict *ladder (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "hostage_entity")); ladder->IsValid (); ladder = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ladder, "hostage_entity"))
	{
		if (ladder->GetOrigin ().GetDistanceSquared (GetOrigin ()) > MATH_GET_SQUARED (200.0f))
			continue;
#if 0
		typedef void (Trampolines::EmptyClass::*FunctionHostageRemove_t) (void);
		FunctionHostageRemove_t remove (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <FunctionHostageRemove_t> ("CHostage::Remove"/*"?Remove@CHostage@@QAEXXZ"*/));

		if (remove == NULL)
			remove = HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <FunctionHostageRemove_t> ("?Remove@CHostage@@QAEXXZ");

		InternalAssert (remove != NULL);

		(reinterpret_cast <Trampolines::EmptyClass *> (ladder->privateData)->*remove) ();
#endif	// if 0
//#if 0
	const HalfLifeEngine::SDK::Classes::Edict *const rescuer (ladder->GetPrivateDataReference <HalfLifeEngine::SDK::Classes::Edict *const> (HalfLifeEngine::SDK::Constants::OFFSET_HOSTAGE_TARGET));
	const HalfLifeEngine::SDK::Classes::HostageImprov *const hostageImprov (ladder->GetPrivateDataReference <HalfLifeEngine::SDK::Classes::HostageImprov *> (HalfLifeEngine::SDK::Constants::OFFSET_Hostage_ImprovPointer));

	InternalAssert (hostageImprov != NULL);

	const HalfLifeEngine::SDK::Classes::Edict *const CZrescuer (MemoryUtilities::GetOffsetAddressReference <HalfLifeEngine::SDK::Classes::Edict *const> (hostageImprov, HalfLifeEngine::SDK::Constants::OFFSET_HostageImprov_Target));
	const unsigned int CZUNKNOWN0 (MemoryUtilities::GetOffsetAddressReference <unsigned int> (hostageImprov, HalfLifeEngine::SDK::Constants::OFFSET_HostageImprov_UNKNOWN0));
	const unsigned int CZUNKNOWN1 (reinterpret_cast <const int> (&MemoryUtilities::GetOffsetAddressReference <unsigned int> (hostageImprov, HalfLifeEngine::SDK::Constants::OFFSET_HostageImprov_UNKNOWN1)));
/*
	if (CZUNKNOWN0 != NULL)
		InternalAssert (CZUNKNOWN0->m_variables->containingEntity->IsValid ());

	if (CZUNKNOWN1 != NULL)
		InternalAssert (CZUNKNOWN1->m_variables->containingEntity->IsValid ());
*/
	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), ladder->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat
	(
		HalfLifeEngine::SDK::Constants::HUDPrint_Talk,
		"Hostage: ID=%i, is stopped=%u, is was used=%u, is improve=%u, CZ rescuer: %s, CZUNKNOWN0=%i, CZUNKNOWN1=%i, rescuer is null=%u.\n",
		ladder->GetPrivateDataReference <int> (HalfLifeEngine::SDK::Constants::OFFSET_Hostage_ID),
		ladder->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::OFFSET_Hostage_IsStopped),
		ladder->GetPrivateDataReference <bool> (HalfLifeEngine::SDK::Constants::Offset_Hostage_IsWasUsed),
		hostageImprov != NULL,
//		hostageImprov->m_variables->containingEntity->IsValid (),
		CZUNKNOWN0 == CZUNKNOWN1 && CZrescuer->IsValid () ? CZrescuer->GetNetName ().GetData () : "NULL",
		CZUNKNOWN0,
		CZUNKNOWN1,
		rescuer != NULL
	);
//#endif	// if 0
	}
#endif	// if 0
//	m_edict->variables.takeDamageType = HalfLifeEngine::SDK::Constants::DAMAGE_NO;
	m_edict->variables.moveType = IsUsesFlashlight () ? HalfLifeEngine::SDK::Constants::MOVETYPE_NOCLIP : HalfLifeEngine::SDK::Constants::MOVETYPE_WALK;
//#if 0
	if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
	{
#if 0
{
/*		const Sector_t &sector (g_server->GetNavigationMeshManager ().GetWorldMap ().GetSectorUnder (GetOrigin ()));

//		ServerConsole_printf ("Drawing sector, %u walkfaces involved\n", sector.GetElementNumber ());

		g_server->GetNavigationMeshManager ().GetWorldMap ().DrawSector (GetHostPointer (), sector, Color <> (100u, 255u, 0u), 1u);

		for (unsigned short index (0u); index < sector.GetElementNumber (); ++index)
			g_server->GetNavigationMeshManager ().GetWorldMap ().DrawWalkFace (GetHostPointer (), *sector[index], Color <> (100u, 255u, 255u), 1u);
*/
		const WalkFace_t *const walkFace (g_server->GetNavigationMeshManager ().GetWorldMap ().GetWalkFaceUnder (m_edict));

		if (walkFace != NULL)
			g_server->GetNavigationMeshManager ().GetWorldMap ().DrawWalkFace (GetHostPointer (), *walkFace, Color <> (100u, 255u, 255u), 1u);
}
#endif	// if 0
/*
			{
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), GetEyeForwardPosition (200.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_None, m_edict, traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allS=%u, startS=%u, open=%u, wtr=%u(%u), fr=%.2f, pn=[%.2f, %.2f, %.2f], hit=%s.", traceResult.isAllSolid, traceResult.isStartSolid, traceResult.isInOpen, traceResult.isInWater, HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (traceResult.endPosition) == HalfLifeEngine::SDK::Constants::Content_Water, traceResult.fraction, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z, traceResult.hitEntity == NULL ? "NULL" : traceResult.hitEntity->GetClassName ().GetData ());
}*/
#if 0
		{
		const Math::Vector3D source (m_edict->GetEyePosition ());
		const Math::Vector3D destination (GetEyeForwardPosition (200.0f));

		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
//		HalfLifeEngine::Engine::TraceFilter_WalkableEntities traceFilter (/*m_edict*/NULL, 0, HalfLifeEngine::Engine::TraceFilter_WalkableEntities::WALK_THRU_EVERYTHING);
		HalfLifeEngine::Engine::TraceFilter_PassEntity traceFilter (m_edict);

//		double timeMY (g_commonTimer.GetMilliseconds <double> ());

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHullThroughSolidEntities (source, destination, HalfLifeEngine::SDK::Constants::Hull_Point, traceFilter, traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHullThroughAllEntities (source, destination, HalfLifeEngine::SDK::Constants::Hull_Point, traceFilter, traceResult);
/*
		double timeHL (g_commonTimer.GetMilliseconds <double> ());
		timeMY = timeHL - timeMY;

//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_None, m_edict, traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (source, destination, false, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, traceResult);

		timeHL = g_commonTimer.GetMilliseconds <double> () - timeHL;

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "timeMY=%lf, timeHL=%lf.", timeMY, timeHL);
/*/
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allS=%u, startS=%u, open=%u, wtr=%u, fr=%.2f, pn=[%.2f, %.2f, %.2f], hit=%s.", traceResult.isAllSolid, traceResult.isStartSolid, traceResult.isInOpen, traceResult.isInWater, traceResult.fraction, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z, traceResult.hitEntity == NULL ? "NULL" : traceResult.hitEntity->GetClassName ().GetData ());
		}
#endif	// if 0
#if 0
{
	for (unsigned short index (0u); index < HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->maximumEntities; ++index)
	{
		HalfLifeEngine::SDK::Classes::Edict *ladder (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (index));

//		InternalAssert (ladder != NULL);
		if (ladder == NULL)
		{
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Entity is NULL on index %u!!!", index);
			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Entity is NULL on index %u!!!\n", index);
//			AddLogEntry (false, LogLevel_Critical, true, "Entity is NULL on index %u!!!", index);

			continue;
		}

		if (!ladder->IsValid () || ladder == m_edict || ladder->variables.boundingBox.mins == Math::Vector3D::ZeroValue || ladder->variables.boundingBox.maxs == Math::Vector3D::ZeroValue)
			continue;
/*
		AddLogEntry (false, LogLevel_Default, true, "Tracing thru \"%s\" (%u), maximumEntities=%u.", ladder->GetClassName ().GetData (), index, HalfLifeEngine::Globals::g_halfLifeEngine->GetGlobalVariables ()->maximumEntities);

		ladder->Print (FormatBuffer ("C:/Users/Эдуард/Desktop/tmp/%u.log", index));
*/
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceEntity (m_edict->GetEyePosition (), GetEyeForwardPosition (200.0f), HalfLifeEngine::SDK::Constants::Hull_Point, ladder, traceResult);
		if (traceResult.fraction < 1.0f)
		{
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allS=%u, startS=%u, open=%u, wtr=%u, fr=%.2f, pn=[%.2f, %.2f, %.2f], hit=%s.", traceResult.isAllSolid, traceResult.isStartSolid, traceResult.isInOpen, traceResult.isInWater, traceResult.fraction, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z, traceResult.hitEntity == NULL ? "NULL" : traceResult.hitEntity->GetClassName ().GetData ());
		}
	}
}
#endif	// if 0
/*	for (HalfLifeEngine::SDK::Classes::Edict *ladder (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_ladder")); ladder->IsValid (); ladder = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ladder, "func_ladder"))
	{
		if (ladder->GetOrigin ().GetDistanceSquared (GetOrigin ()) > MATH_GET_SQUARED (300.0f))
			continue;

		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceEntity (m_edict->GetEyePosition (), GetEyeForwardPosition (200.0f), HalfLifeEngine::SDK::Constants::Hull_Point, ladder, traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allS=%u, startS=%u, open=%u, wtr=%u, fr=%.2f, pn=[%.2f, %.2f, %.2f], hit=%s.", traceResult.isAllSolid, traceResult.isStartSolid, traceResult.isInOpen, traceResult.isInWater, traceResult.fraction, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z, traceResult.hitEntity == NULL ? "NULL" : traceResult.hitEntity->GetClassName ().GetData ());
	}
/*		{
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		Math::Vector3D m_vMoveTo (GetOrigin () + GetEyeForwardDirection () * 200.0f);
		const Math::Vector3D dir ((m_vMoveTo - GetOrigin ()).Normalize () * 30.0f);

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (GetOrigin (), m_vMoveTo, HalfLifeEngine::SDK::Constants::TraceIgnore_None, m_edict, traceResult);

//		if (traceResult.fraction < 1.0f)
		if (traceResult.hitEntity->IsValid ())
		{
//			const Math::Vector3D m_vAvoidOrigin (traceResult.endPosition);
			const Math::Vector3D m_vAvoidOrigin (traceResult.hitEntity->GetOrigin ());
			const Math::Vector3D vMove ((m_vMoveTo - GetOrigin ()) ^ Math::Vector3D::UnitZ);
			const Math::Vector3D vLeft (GetOrigin () - vMove * 64.0f);
			const Math::Vector3D vRight (GetOrigin () + vMove * 64.0f);

			if (m_vAvoidOrigin.GetDistanceSquared (vLeft) < m_vAvoidOrigin.GetDistanceSquared (vRight))
				m_vMoveTo = vLeft;
			else
				m_vMoveTo = vRight;

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin () + dir, vLeft + dir, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin () + dir, vRight + dir, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

//			m_vMoveTo += dir;
		}

//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin () + dir, m_vMoveTo, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
		}*/
#if 0
{
		if (g_server->GetC4Manager ().IsС4Planted ())
		{
//			const Math::Angles2D fov (GetAbsoluteFieldOfView2D (g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.origin));

//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "C4: GetAbsoluteFieldOfView2D()=[%.2f, %.2f].\n", fov.pitch, fov.yaw);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "C4: GetShootingConeDeviation()=%lf.\n", GetShootingConeDeviation (g_server->GetC4Manager ().GetC4 ()->GetEdict ()->GetBModelOrigin ()/*variables.origin*/));
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), g_server->GetC4Manager ().GetC4 ()->GetEdict ()->GetBModelOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), g_server->GetC4Manager ().GetC4 ()->GetEdict ()->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
		}
		else for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
		{
/*			const Math::Angles2D fov (GetAbsoluteFieldOfView2D (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->GetOrigin ()));

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot: GetAbsoluteFieldOfView2D()=[%.2f, %.2f], IsLookingAtPosition()=%u.\n", fov.pitch, fov.yaw, IsLookingAtPosition (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->GetOrigin ()));
*/			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot: IsLookingAtPosition()=%u, GetShootingConeDeviation()=%lf.\n", IsLookingAtPosition (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->GetOrigin ()), GetShootingConeDeviation (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->GetOrigin ()));
/*
			const Math::Vector3D eye (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->GetEyePosition ());
			const float shootingConeDeviation (GetShootingConeDeviation (eye));

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Bot: GetShootingConeDeviation()=%lf.\n", shootingConeDeviation);
*/		}
}
#endif	// if 0
/*	// Find the conveyor entity....
	for (HalfLifeEngine::SDK::Classes::Edict *conveyor (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_conveyor")); conveyor->IsValid (); conveyor = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (conveyor, "func_conveyor"))
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), conveyor->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	}
	for (HalfLifeEngine::SDK::Classes::Edict *conveyor (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "trigger_push")); conveyor->IsValid (); conveyor = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (conveyor, "trigger_push"))
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), conveyor->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
	}*/
/*	for (HalfLifeEngine::SDK::Classes::Edict *conveyor (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_breakable")); conveyor->IsValid (); conveyor = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (conveyor, "func_breakable"))
	{
		if (conveyor->GetOrigin ().GetDistanceSquared (GetOrigin ()) > MATH_GET_SQUARED (200.0f))
			continue;

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "spawnFlags=%u.", conveyor->variables.spawnFlags);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), conveyor->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
	}*/
#if 0
{
	HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

	int face_index, edges, edge_index;
	const unsigned char vertexesNumber (4u);
	Math::Vector3D vertexes[vertexesNumber];	// hold up to four vertexes for each face
	int front_face_index (-1);
	float face_width, maximumFaceWidth;
	Math::Vector3D v_top[2], v_bottom[2], v_temp;
	Math::Vector3D v_forward, v_backward, v_outward;
	enum LadderPosition_t
	{
		LadderPosition_Center,
		LadderPosition_Top,
		LadderPosition_Bottom,

		LadderPosition_Total
	};
	Math::Vector3D ladder_points[LadderPosition_Total];  // origin, "top" and "bottom" of ladder
	Math::Vector3D v_start, v_end;
	unsigned char loop;
	int num_segments;
	bool outward_found;
	float dist;
//	int LadderCount;

	struct FaceInfo_t
	{
		unsigned short planeIndex;
		float          vertexesLengthSquared;
		struct MidSide_t
		{
			Math::Vector3D segment[2u];
			float          lengthSquared;
		} midSides[2u];
	};
	FaceInfo_t tempFaceInfo;
	typedef DynamicArray <FaceInfo_t, unsigned char> FaceInfoArray;
	FaceInfoArray tempFaceInfoArray;
	FaceInfoArray faceInfoArray;
	typedef DynamicArray <FaceInfoArray, unsigned char> FaceInfoArrays;
	FaceInfoArrays faceInfoArrays;

	// loop through all the entities looking for "func_ladder"...
	for (unsigned short entityIndex (0u); entityIndex < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities.GetElementNumber (); ++entityIndex)
	{
		const WorldMap::Entity_t &entity (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().entities[entityIndex]);

		if (entity.className != "func_ladder" || GetOrigin ().GetDistanceSquared (entity.origin) > MATH_GET_SQUARED (200.0f))
			continue;

		const HalfLifeEngine::SDK::Structures::DModel_t &model (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[entity.brushModelIndex]);

			faceInfoArray.RemoveAll ();

	/// @note This works only for trivial ladders!

			for (face_index = 0u; face_index < model.facesNum; ++face_index)
			{
				const HalfLifeEngine::SDK::Structures::DFace_t &face (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[static_cast <unsigned short> (model.firstFaceIndex + face_index)]);

				// Reliability check.
				InternalAssert (face.surfEdgesNum == vertexesNumber);

				tempFaceInfo.planeIndex = face.planeNum;
				tempFaceInfo.vertexesLengthSquared = 0.0f;
				tempFaceInfo.midSides[0u].segment[0u] = Math::Vector3D::ZeroValue;
				tempFaceInfo.midSides[0u].segment[1u] = Math::Vector3D::ZeroValue;
				tempFaceInfo.midSides[0u].lengthSquared = 0.0f;
				tempFaceInfo.midSides[1u].segment[0u] = Math::Vector3D::ZeroValue;
				tempFaceInfo.midSides[1u].segment[1u] = Math::Vector3D::ZeroValue;
				tempFaceInfo.midSides[1u].lengthSquared = 0.0f;

				for (edges = 0u; edges < face.surfEdgesNum; ++edges)
				{
					// get the coordinates of the vertex of this edge...
					edge_index = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().surfEdges[static_cast <unsigned int> (face.firstSurfEdgeIndex + edges)];

					if (edge_index < 0)
					{
						const HalfLifeEngine::SDK::Structures::DEdge_t &e (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().edges[static_cast <unsigned int> (-edge_index)]);

						vertexes[edges] = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().vertexes[e.vertices[1]].point;
					}
					else
					{
						const HalfLifeEngine::SDK::Structures::DEdge_t &e (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().edges[static_cast <unsigned int> (edge_index)]);

						vertexes[edges] = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().vertexes[e.vertices[0]].point;
					}

					if (edges >= 1u)
					{
						// S = a * b
						tempFaceInfo.vertexesLengthSquared += vertexes[edges].GetDistanceSquared (vertexes[edges - 1u]);
					}
				}

				// S = a * b
				tempFaceInfo.vertexesLengthSquared += vertexes[face.surfEdgesNum - 1u].GetDistanceSquared (vertexes[0u]);

				tempFaceInfo.midSides[0u].segment[0u] = vertexes[0u].GetMidPoint (vertexes[1u]);
				tempFaceInfo.midSides[0u].segment[1u] = vertexes[2u].GetMidPoint (vertexes[3u]);

				tempFaceInfo.midSides[0u].lengthSquared = tempFaceInfo.midSides[0u].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[0u].segment[1u]);

				tempFaceInfo.midSides[1u].segment[0u] = vertexes[1u].GetMidPoint (vertexes[2u]);
				tempFaceInfo.midSides[1u].segment[1u] = vertexes[3u].GetMidPoint (vertexes[0u]);

				tempFaceInfo.midSides[1u].lengthSquared = tempFaceInfo.midSides[1u].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[1u].segment[1u]);

				if (tempFaceInfo.midSides[0u].segment[0u].z > tempFaceInfo.midSides[0u].segment[1u].z)
					SwapElements (tempFaceInfo.midSides[0u].segment[0u], tempFaceInfo.midSides[0u].segment[1u]);

				if (tempFaceInfo.midSides[1u].segment[0u].z > tempFaceInfo.midSides[1u].segment[1u].z)
					SwapElements (tempFaceInfo.midSides[1u].segment[0u], tempFaceInfo.midSides[1u].segment[1u]);

				if (tempFaceInfo.midSides[0u].lengthSquared < tempFaceInfo.midSides[1u].lengthSquared || tempFaceInfo.midSides[0u].segment[1u].z - tempFaceInfo.midSides[0u].segment[0u].z < tempFaceInfo.midSides[1u].segment[1u].z - tempFaceInfo.midSides[1u].segment[0u].z)
					SwapElements (tempFaceInfo.midSides[0u], tempFaceInfo.midSides[1u]);

				FaceInfoArray::IndexType_t index (0u);

				for (/* Empty */; index < faceInfoArray.GetElementNumber (); ++index)
					if (faceInfoArray[index].vertexesLengthSquared > tempFaceInfo.vertexesLengthSquared)
					{
						faceInfoArray.Insert (index, tempFaceInfo);

						break;
					}

				if (index == faceInfoArray.GetElementNumber ())
					faceInfoArray += tempFaceInfo;
			}

		InternalAssert (faceInfoArray.GetElementNumber () == model.facesNum);
		InternalAssert (faceInfoArray.GetElementNumber () % 2u == 0u);

		faceInfoArrays.RemoveAll ();
		tempFaceInfoArray.RemoveAll ();

		tempFaceInfoArray += faceInfoArray.GetFirstElement ();

		for (FaceInfoArray::IndexType_t index (1u/* Skip smallest element */); index < faceInfoArray.GetElementNumber (); ++index)
		{
			if (tempFaceInfoArray.GetLastElement ().vertexesLengthSquared != faceInfoArray[index].vertexesLengthSquared)
			{
				InternalAssert (tempFaceInfoArray.GetElementNumber () % 2u == 0u);

				faceInfoArrays += tempFaceInfoArray;

				tempFaceInfoArray.RemoveAll ();
			}

			FaceInfoArray::IndexType_t index1 (0u);

			for (/* Empty */; index1 < tempFaceInfoArray.GetElementNumber (); ++index1)
				if (tempFaceInfoArray[index1].midSides[1u].lengthSquared <= faceInfoArray[index].midSides[1u].lengthSquared)
				{
					tempFaceInfoArray.Insert (index1, faceInfoArray[index]);

					break;
				}

			if (index1 == tempFaceInfoArray.GetElementNumber ())
				tempFaceInfoArray += faceInfoArray[index];
		}

		InternalAssert (!tempFaceInfoArray.IsEmpty ());
		InternalAssert (tempFaceInfoArray.GetElementNumber () % 2u == 0u);

		faceInfoArrays += tempFaceInfoArray;

		InternalAssert (faceInfoArrays.GetElementNumber () == faceInfoArray.GetElementNumber () / 2u);

		faceInfoArrays.PopFront ();	// Remove smallest element blindly.

		// Special case for ladders with faces more than 6 (these ladders are divided on two with the same planes for each!)
		if (faceInfoArrays.GetElementNumber () > 3u/*faceInfoArrays.GetElementNumber () == 5u*/)
		{
			for (FaceInfoArrays::IndexType_t index0 (faceInfoArrays.GetElementNumber ()); index0 > 0u/* Skip smallest element */; /* Empty */)
			{
				--index0;

				FaceInfoArray &faceInfoArrayWithBiggestLength (faceInfoArrays[index0]);

				for (FaceInfoArray::IndexType_t index (0u); index < faceInfoArrayWithBiggestLength.GetElementNumber (); ++index)
				{
					for (FaceInfoArrays::IndexType_t index1 (0u/* Skip smallest element */); index1 < index0/* Skip biggest element */; ++index1)
					{
						const FaceInfoArray &facesToSplit (faceInfoArrays[index1]);

						if (faceInfoArrayWithBiggestLength[index].planeIndex != facesToSplit[index].planeIndex)
							continue;

						for (unsigned char midSideIndex (0u); midSideIndex < 2u; ++midSideIndex)
						{
							if (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u] == facesToSplit[index].midSides[midSideIndex].segment[1u])
							{
								InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u].IsPointBelongsToSegment (facesToSplit[index].midSides[midSideIndex].segment[0u], facesToSplit[index].midSides[midSideIndex].segment[1u]));

								const Math::Vector3D &otherSegmentOldMid (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

								InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]) == otherSegmentOldMid);

								faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u] = facesToSplit[index].midSides[midSideIndex].segment[0u];

								const Math::Vector3D &segmentMid (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]));
								const Math::Vector3D &otherSegmentOldHalf ((otherSegmentOldMid - faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

								// Update other mid side....
								faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u] = segmentMid + otherSegmentOldHalf;
								faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u] = segmentMid - otherSegmentOldHalf;

								// Recalculate length....
								faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].lengthSquared = tempFaceInfo.midSides[midSideIndex].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[midSideIndex].segment[1u]);

								// Is both faces checked?
								if (index + 1u == faceInfoArrayWithBiggestLength.GetElementNumber ())
								{
									faceInfoArrays.Remove (index1, 1u);

									index1 = index0;	// Stop (this will break loop 'index1 < index0')
								}

								break;
							}
							else if (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u] == facesToSplit[index].midSides[midSideIndex].segment[0u])
							{
								InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].IsPointBelongsToSegment (facesToSplit[index].midSides[midSideIndex].segment[1u], facesToSplit[index].midSides[midSideIndex].segment[0u]));

								const Math::Vector3D &otherSegmentOldMid (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

								InternalAssert (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]) == otherSegmentOldMid);

								faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u] = facesToSplit[index].midSides[midSideIndex].segment[1u];

								const Math::Vector3D &segmentMid (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[0u].GetMidPoint (faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].segment[1u]));
								const Math::Vector3D &otherSegmentOldHalf ((otherSegmentOldMid - faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u]));

								// Update other mid side....
								faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[0u] = segmentMid + otherSegmentOldHalf;
								faceInfoArrayWithBiggestLength[index].midSides[!midSideIndex].segment[1u] = segmentMid - otherSegmentOldHalf;

								// Recalculate length....
								faceInfoArrayWithBiggestLength[index].midSides[midSideIndex].lengthSquared = tempFaceInfo.midSides[midSideIndex].segment[0u].GetDistanceSquared (tempFaceInfo.midSides[midSideIndex].segment[1u]);

								// Is both faces checked?
								if (index + 1u == faceInfoArrayWithBiggestLength.GetElementNumber ())
								{
									faceInfoArrays.Remove (index1, 1u);

									index1 = index0;	// Stop (this will break loop 'index1 < index0')
								}

								break;
							}
						}
					}
				}
			}
		}

	InternalAssert (faceInfoArrays.GetElementNumber () == 2u);
/*
	for (FaceInfoArrays::IndexType_t index (0u); index < faceInfoArrays.GetElementNumber (); ++index)
		for (FaceInfoArray::IndexType_t index2 (0u); index2 < faceInfoArrays[index].GetElementNumber (); ++index2)
		{
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (faceInfoArrays[index][index2].planeIndex)]);

//			for (unsigned char midSideIndex (0u); midSideIndex < 2u; ++midSideIndex)
			{
				const Math::Vector3D &adjust (plane.normal * HalfLifeEngine::SDK::Constants::HalfHumanWidth), &bottom (faceInfoArrays[index][index2].midSides[0u].segment[0u] + adjust), &top (faceInfoArrays[index][index2].midSides[0u].segment[1u] + adjust);

//				UTIL_TraceHull (bottom, top, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (bottom, top, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, traceResult);

				HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (bottom, top, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);

				if (traceResult.isStartSolid || traceResult.fraction < 1.0f)
				{
					faceInfoArrays[index].Remove (index2, 1u);

					--index2;	// Step back!
				}
			}
		}
/*
		typedef WorldMap::LadderFaceInfoArrays FaceInfoArrays;
		FaceInfoArrays faceInfoArrays;

		g_server->GetNavigationMeshManager ().GetWorldMap ().GetLadderPossibleClimbableMidSides (model, faceInfoArrays);
*/
//#if 0
{
//		for (FaceInfoArrays::IndexType_t index (0u/* Skip smallest element */); index < faceInfoArrays.GetElementNumber (); ++index)
		const FaceInfoArrays::IndexType_t index (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <unsigned char> ());
		if (index < faceInfoArrays.GetElementNumber ())
//		const FaceInfoArrays::IndexType_t index (faceInfoArrays.GetElementNumber () - 1u);
		{
//			for (FaceInfoArray::IndexType_t index2 (0u); index2 < faceInfoArrays[index].GetElementNumber (); ++index2)
			const FaceInfoArray::IndexType_t index2 (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Stop)->GetValue <unsigned char> ());
			if (index2 < faceInfoArrays[index].GetElementNumber ())
			{
/*				for (unsigned char vertexIndex (1u); vertexIndex < vertexesNumber; ++vertexIndex)
//				unsigned char vertexIndex (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <unsigned char> ());
//				if (vertexIndex < vertexesNumber)
				{
					HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (faceInfoArrays[index][index2].vertexes[vertexIndex - 1u], faceInfoArrays[index][index2].vertexes[vertexIndex], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
//					HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (faceInfoArrays[index][index2].vertexes[vertexIndex], 3.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
				}*/
//				for (unsigned char midSideIndex (0u); midSideIndex < 2u; ++midSideIndex)
				unsigned char midSideIndex (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <unsigned char> ());
				if (midSideIndex < 2u)
					HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (faceInfoArrays[index][index2].midSides[midSideIndex].segment[0u], faceInfoArrays[index][index2].midSides[midSideIndex].segment[1u], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
			}
		}
}
//#endif	// if 0
//#endif	// if 0
#if 0
		// find the "front" and "back" of the ladder. Do this by looking
		// for a faces whose vertex Z values are not all the same (i.e. it's
		// not the top face or the bottom face). Then look for the face
		// that is the widest. This should be either the front or the back.

		maximumFaceWidth = 0.0f;

			// loop though each face for this model...
			for (face_index = 0u; face_index < model.facesNum; ++face_index)
			{
				const HalfLifeEngine::SDK::Structures::DFace_t &face (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[static_cast <unsigned short> (model.firstFaceIndex + face_index)]);

				// Reliability check.
				InternalAssert (face.surfEdgesNum == vertexesNumber);

				for (edges = 0u; edges < face.surfEdgesNum; ++edges)
				{
					// get the coordinates of the vertex of this edge...
					edge_index = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().surfEdges[static_cast <unsigned int> (face.firstSurfEdgeIndex + edges)];

					if (edge_index < 0)
					{
						const HalfLifeEngine::SDK::Structures::DEdge_t &e (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().edges[static_cast <unsigned int> (-edge_index)]);

						vertexes[edges] = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().vertexes[e.vertices[1]].point;
					}
					else
					{
						const HalfLifeEngine::SDK::Structures::DEdge_t &e (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().edges[static_cast <unsigned int> (edge_index)]);

						vertexes[edges] = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().vertexes[e.vertices[0]].point;
					}
				}

				// now look through the 4 vertexs to see if the Z coordinates are the "same".

				if (abs (static_cast <int> (vertexes[0].z - vertexes[1].z)) < 8 && abs (static_cast <int> (vertexes[1].z - vertexes[2].z)) < 8 &&
					abs (static_cast <int> (vertexes[2].z - vertexes[3].z)) < 8 && abs (static_cast <int> (vertexes[3].z - vertexes[0].z)) < 8)
					continue;	// continue with next face if this is top or bottom

				// now calculate the 2D "width" of this face (ignoring Z axis)

				// is this a "base" (i.e. are the Z coordinates the "same")
				if (abs (static_cast <int> (vertexes[0].z - vertexes[1].z)) < 8)
				{
					v_temp = vertexes[0] - vertexes[1];
					face_width = v_temp.GetLength2D ();

					if (face_width > maximumFaceWidth)	// widest so far?
					{
						maximumFaceWidth = face_width;
						front_face_index = face_index;	// save it for later

						// compare the Z coordinates of the 2 "bases"
						if (vertexes[0].z > vertexes[2].z)	// vertexes[0] higher than vertexes[2]?
						{
							v_top[0] = vertexes[0];	// save the top for later
							v_top[1] = vertexes[1];
							v_bottom[0] = vertexes[2];	// save the bottom for later
							v_bottom[1] = vertexes[3];
						}
						else
						{
							v_top[0] = vertexes[2];	// save the top for later
							v_top[1] = vertexes[3];
							v_bottom[0] = vertexes[0];	// save the bottom for later
							v_bottom[1] = vertexes[1];
						}
					}
				}

				if (abs (static_cast <int> (vertexes[1].z - vertexes[2].z)) < 8)	// is this a "base"?
				{
					v_temp = vertexes[1] - vertexes[2];
					face_width = v_temp.GetLength2D ();

					if (face_width > maximumFaceWidth)	// widest so far?
					{
						maximumFaceWidth = face_width;
						front_face_index = face_index;	// save it for later

						// compare the Z coordinates of the 2 "bases"
						if (vertexes[1].z > vertexes[0].z)	// vertexes[0] higher than vertexes[2]?
						{
							v_top[0] = vertexes[1];	// save the top for later
							v_top[1] = vertexes[2];
							v_bottom[0] = vertexes[0];	// save the bottom for later
							v_bottom[1] = vertexes[3];
						}
						else
						{
							v_top[0] = vertexes[0];	// save the top for later
							v_top[1] = vertexes[3];
							v_bottom[0] = vertexes[1];	// save the bottom for later
							v_bottom[1] = vertexes[2];
						}
					}
				}
//				}
			}

			InternalAssert (front_face_index != -1);

			// now we know that front_face_index is either the front or back
			// trace vectors out from the ladder to see if world collisions or
			// brush model collisions occur to find out which direction this
			// func_ladder is aproachable from.

			const HalfLifeEngine::SDK::Structures::DFace_t &face (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces[static_cast <unsigned short> (model.firstFaceIndex + front_face_index)]);
			const HalfLifeEngine::SDK::Structures::DPlane_t &plane (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (face.planeNum)]);

			// create points in the middle of the ladder lengthwise...

			ladder_points[LadderPosition_Center] = model.mins.GetMidPoint (model.maxs);	// middle of the ladder

			ladder_points[LadderPosition_Top] = v_top[0].GetMidPoint (v_top[1]);	// middle of the top
			ladder_points[LadderPosition_Bottom] = v_bottom[0].GetMidPoint (v_bottom[1]);	// middle of the bottom

			// adjust top middle and bottom middle by 10 units (towards the center)
			v_temp = ladder_points[LadderPosition_Bottom] - ladder_points[LadderPosition_Top];
			v_temp.Normalize ();
			v_temp *= 10.0f;
			ladder_points[LadderPosition_Top] += v_temp;
			v_temp.Invert ();
			ladder_points[LadderPosition_Bottom] += v_temp;

			// loop though the ladder points tracing a line outward in both the
			// forward and backward directions to see if we hit the world.

			v_forward = v_backward = plane.normal * 20.0f;

			v_backward.Invert ();

			outward_found = false;

			loop = LadderPosition_Center;

			do
			{
				v_start = ladder_points[loop];
				v_end = ladder_points[loop] + v_forward;

//				BotmanTraceLine (v_start, v_end, &traceResult);
				HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (v_start, v_end, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, traceResult);

				if (traceResult.fraction < 1.0f)	// we hit something going forward
				{
					v_outward = v_backward;	// outward is v_backward
					v_outward.Normalize ();	// make unit vector
					outward_found = true;
				}
				else
				{
					v_start = ladder_points[loop];
					v_end = ladder_points[loop] + v_backward;

//					BotmanTraceLine (v_start, v_end, &traceResult);
					HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (v_start, v_end, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, traceResult);

					if (traceResult.fraction < 1.0f)	// we hit something going backward
					{
						v_outward = v_forward;	// outward is forward
						v_outward.Normalize ();	// make unit vector
						outward_found = true;
					}
				}
			} while (++loop < LadderPosition_Total && !outward_found);

			// if outward_found is still FALSE here then need to check for
			// collisions with other world brushes (other than func_ladder)...

			// ADD THIS CODE LATER - FIXME!!!


			if (outward_found)	// do we now know which way is outward?
			{
//		++LadderCount;
				// create a waypoint at the top of the ladder
				// (remember that ladder_points[LadderPosition_Top] is already down 10 units...

				v_temp = ladder_points[LadderPosition_Top] - ladder_points[LadderPosition_Bottom];

				v_temp.Normalize ();

				v_end = ladder_points[LadderPosition_Top] + v_temp * (32.0f + 10.0f);	// top of ladder (a little bit lower than half of the player height...)

				v_end += v_outward * 20.0f;	// move outward 20 units

//				AddNode (v_end);	// add top waypoint
				HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (v_end, 5.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

				// create a waypoint at the bottom of the ladder
				// (remember that ladder_points[LadderPosition_Bottom] is already up 10 units...
				// a little bit higher than half of the player height...
				v_start = ladder_points[LadderPosition_Bottom] + v_temp * (44.0f - 10.0f);

				v_start += v_outward * 20.0f;	// move outward 20 units

//				AddNode (v_start);	// add bottom waypoint
				HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (v_start, 5.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

				// now determine how many waypoints to place in between
				// the start and end points on the ladder...

				v_temp = v_end - v_start;
				dist = v_temp.GetLength ();

#define NODE_ZONE		45

				num_segments = static_cast <unsigned short> ((dist + 100.0f) / NODE_ZONE);	// place every ?? units

				v_temp /= dist;

				dist /= num_segments;	// distance apart

				v_temp *= dist;	// delta vector for steps

				for (loop = 1u; loop < num_segments; ++loop)
				{
					v_start += v_temp;
//					AddNode (v_start);	// add middle waypoints
					HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (v_start, 5.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
				}
			}
			else
				AddLogEntry (true, LogLevel_Error, false, "Ladder found, but can't tell which way is outward!");
#endif	// if 0
	}
}
#endif	// if 0
#if 0
{
		const Math::Vector3D from (m_edict->GetEyePosition ()), to (GetEyeForwardPosition (200.0f));
		const Color <> color1 (0u, 255u, 0u), color2 (0u, 0u, 255u);
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

		UTIL_TraceHull (from, to, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), GetEyeForwardPosition (100.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, traceResult);
/*
		const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (g_server->GetNavigationMeshManager ().GetWorldMap ().GetHullPointContents (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[0u].headNode[HalfLifeEngine::SDK::Constants::Hull_Head], traceResult.endPosition));

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "point contents=%s.\n", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");
*/		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "1: allSol=%s,startSol=%s,open=%s,wtr=%s,frct=%.2f,nrm=[%.2f,%.2f,%.2f],dist=%.2f,planeID=%u.\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z, traceResult.planeDistance, g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[static_cast <unsigned short> (traceResult.hitGroup)].planeNum);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color1, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color1, 1u);
//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color1, 1u);

		const Math::Plane plane (traceResult.planeNormal, traceResult.planeDistance);
/*		const Math::Vector3D offset
		(
			plane.normal.x >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].mins.x : HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs.x,
			plane.normal.y >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].mins.y : HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs.y,
			plane.normal.z >= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].mins.z : HalfLifeEngine::SDK::Constants::hullSizes[hullIndex].maxs.z
		);
		const float newPlaneDistance (offset | plane.normal);*/
		Math::Plane::Points_t points;

		plane.GetWinding (traceResult.endPosition, points, 50.0f);

		const Math::Vector3D center1 ((points[0] + points[1] + points[2] + points[3]) / 4.0f);

//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), center1, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color1, 250u, 5u, 1u);

		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[0], points[1], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color1, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[1], points[2], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color1, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[2], points[3], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color1, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points[3], points[0], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color1, 250u, 5u, 1u);
#if 0
/*! HOW APPENDS HULL OFFSET FROM ZHLT SOURCES??????????
		if (normal.x != 0.0f)
			origin.x += normal.x * (normal.x < 0.0f ? -hullSizes[hullIndex].mins.x : hullSizes[hullIndex].maxs.x);

		if (normal.y != 0.0f)
			origin.y += normal.y * (normal.y < 0.0f ? -hullSizes[hullIndex].mins.y : hullSizes[hullIndex].maxs.y);

		if (normal.z != 0.0f)
			origin.z += normal.z * (normal.z < 0.0f ? -hullSizes[hullIndex].mins.z : hullSizes[hullIndex].maxs.z);
////////OTHER METHOD////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		origin.x += normal.x <= 0.0f ? hullSizes[hullIndex].mins.x : hullSizes[hullIndex].maxs.x;
		origin.y += normal.y <= 0.0f ? hullSizes[hullIndex].mins.y : hullSizes[hullIndex].maxs.y;
		origin.z += normal.z <= 0.0f ? hullSizes[hullIndex].mins.z : hullSizes[hullIndex].maxs.z;
*/
		float newPlaneDistance (traceResult.planeDistance);
/*
		{
		if (traceResult.planeNormal.x != 0.0f)
			newPlaneDistance -= traceResult.planeNormal.x * (traceResult.planeNormal.x < 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.x : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.x);

		if (traceResult.planeNormal.y != 0.0f)
			newPlaneDistance -= traceResult.planeNormal.y * (traceResult.planeNormal.y < 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.y : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.y);

		if (traceResult.planeNormal.z != 0.0f)
			newPlaneDistance -= traceResult.planeNormal.z * (traceResult.planeNormal.z < 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.z : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.z);
/*//*
////////OTHER METHOD////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		newPlaneDistance += traceResult.planeNormal.x <= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.x : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.x;
		newPlaneDistance += traceResult.planeNormal.y <= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.y : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.y;
		newPlaneDistance += traceResult.planeNormal.z <= 0.0f ? HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].mins.z : HalfLifeEngine::SDK::Constants::hullSizes[HalfLifeEngine::SDK::Constants::Hull_Head].maxs.z;
/*
		}
*/
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult2;

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (from, to, true, HalfLifeEngine::SDK::Constants::Hull_Point, m_edict, traceResult2);
/*
		const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (g_server->GetNavigationMeshManager ().GetWorldMap ().GetHullPointContents (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[0u].headNode[HalfLifeEngine::SDK::Constants::Hull_Head], traceResult2.endPosition));

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "point contents=%s.\n", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");
*/		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "2: allSol=%s,startSol=%s,open=%s,wtr=%s,frct=%.2f,nrm=[%.2f,%.2f,%.2f],dist=%.2f,diff=%.2f(new=%.2f).\n", traceResult2.isAllSolid ? "true" : "false", traceResult2.isStartSolid ? "true" : "false", traceResult2.isInOpen ? "true" : "false", traceResult2.isInWater ? "true" : "false", traceResult2.fraction, traceResult2.planeNormal.x, traceResult2.planeNormal.y, traceResult2.planeNormal.z, traceResult2.planeDistance, traceResult.planeDistance - traceResult2.planeDistance, newPlaneDistance - traceResult2.planeDistance);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult2.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color2, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult2.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color2, 1u);
//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult2.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult2.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), color2, 1u);

		const Math::Plane plane2 (traceResult2.planeNormal, traceResult2.planeDistance);
		Math::Plane::Points_t points2;

		plane2.GetWinding (traceResult2.endPosition, points2, 50.0f);

		const Math::Vector3D center2 ((points2[0] + points2[1] + points2[2] + points2[3]) / 4.0f);

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "plane differences: [X == %.2f, Y == %.2f, Z == %.2f].\n", center1.x - center2.x, center1.y - center2.y, center1.z - center2.z);

//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), center2, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color2, 250u, 5u, 1u);

		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points2[0], points2[1], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color2, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points2[1], points2[2], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color2, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points2[2], points2[3], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color2, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (points2[3], points2[0], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, color2, 250u, 5u, 1u);
#endif	// if 0
}
#endif	// if 0
/*{
//		const unsigned char maximumToDraw (32u);
//		unsigned int number (0u);

//		for (unsigned char index (0u); index < HalfLifeEngine::SDK::Constants::EdictAreaNode_Total; ++index)
		{
			for (HalfLifeEngine::SDK::Structures::Link_t *link (g_server->GetEdictAreaNodes ()->solidEdicts.next), *next; link != &g_server->GetEdictAreaNodes ()->solidEdicts; link = next)
			{
				InternalAssert (link != NULL);

				next = link->next;

//				if (++number > maximumToDraw)
//					continue;

				HalfLifeEngine::SDK::Classes::Edict *const touch (HalfLifeEngine::Utilities::GetEdictFromAreaLink (link));

				InternalAssert (touch != NULL);

				const Math::Vector3D origin (touch->GetOrigin ());

//				if (GetOrigin ().GetDistanceSquared (origin) > MATH_GET_SQUARED (300.0f))
//					continue;

				GetHostPointer ()->DrawLine (GetOrigin (), origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
				GetHostPointer ()->DrawCross3D (origin, 10.0f, Color <> (255u, 0u, 0u), 1u, false);
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Solid edict: '%s'", touch->GetClassName ().GetData ());
			}

			for (HalfLifeEngine::SDK::Structures::Link_t *link (g_server->GetEdictAreaNodes ()->triggerEdicts.next), *next; link != &g_server->GetEdictAreaNodes ()->triggerEdicts; link = next)
			{
				InternalAssert (link != NULL);

				next = link->next;

//				if (++number > maximumToDraw)
//					continue;

				HalfLifeEngine::SDK::Classes::Edict *const touch (HalfLifeEngine::Utilities::GetEdictFromAreaLink (link));

				InternalAssert (touch != NULL);

				const Math::Vector3D origin (touch->GetOrigin ());

//				if (GetOrigin ().GetDistanceSquared (origin) > MATH_GET_SQUARED (300.0f))
//					continue;

				GetHostPointer ()->DrawLine (GetOrigin (), origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u, false);
				GetHostPointer ()->DrawCross3D (origin, 10.0f, Color <> (0u, 255u, 0u), 1u, false);
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Trigger edict: '%s'", touch->GetClassName ().GetData ());
			}
		}
}*/
}
//#endif	// if 0
	}
//#endif	// if 0
}

void Client::DeathThink (void)
{
	//! @todo Make some actions here....
}

void Client::Think (void)
{
	// The main client think function....

	if (IsHuman ())
	{
		Human *const human (GetHumanPointer ());

		human->SetVoiceRecieverListenType (Human::VoiceRecieverListenType_Loopback);
		human->SetVoiceRecieverBot (g_server->GetYaPBManager ()->GetBotsCount () == 0u ? NULL : g_server->GetYaPBManager ()->GetRandomBot ());
	}

	CallThinkFunction ();

	// record some stats of all players on the server

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "GetRoundRemainingTime()=%f, GetRoundElapsedTime()=%f, GetRoundStartTime()=%f.\n", g_server->GetGameRules ()->GetRoundRemainingTime (), g_server->GetGameRules ()->GetRoundElapsedTime (), g_server->GetGameRules ()->GetRoundStartTime ());
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "TeamAliveClients: T=%u, CT=%u.\n", g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_Terrorist), g_server->GetClientManager ()->GetAliveClientsNumber (HalfLifeEngine::SDK::Constants::TeamID_CounterTerrorist));
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "121=%i.\n", MemoryUtilities::GetOffsetAddressReference <int> (m_edict->privateData, 121));
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "116=%u.\n", MemoryUtilities::GetOffsetAddressReference <int> (m_edict->privateData, 116));

//	const HalfLifeEngine::SDK::Classes::Edict *const edict (MemoryUtilities::GetOffsetAddressReference <HalfLifeEngine::SDK::Classes::Edict *const> (m_edict->privateData, 236));

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "234u=%u, GetSpecialMapZones()=%u.\n", MemoryUtilities::GetOffsetAddressReference <unsigned int> (m_edict->privateData, 234), GetSpecialMapZones ()/*, edict->IsValid () ? edict->IsNotWorldspawnPlayer () ? edict->GetNetName ().GetData () : edict->GetClassName ().GetData () : "NULL"*/);
#if 0
	// is this player alive?
	if (!IsAlive ())
		return;

	const unsigned short currentWaypointIndex (g_server->GetWaypointManager ().GetLocationsTable ().GetNearestWaypoint (GetOrigin ()));

	m_currentWaypoint = currentWaypointIndex != InvalidWaypointIndex ? g_server->GetWaypointManager ().GetWaypoint (currentWaypointIndex) : NULL;

	LastNavigationAreaUpdate ();
	CheckWalk ();
//	SoundSimulateUpdate ();

	if (!IsHost ())
		return;
#if 0
	if (IsUsesFlashlight ())
	{
		const unsigned char team (GetTeam ());

		// Loop through all the clients/bots...
		for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
		{
			// temporarily keep this slot
			const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

			if (!client->IsAlive () || client->GetTeam () == team || client == this || client->m_edict->IsImmortal ())
				continue;	// skip myself, our teammates and immortals....

			// adjust all body and view angles to face an absolute vector
			// Set the body and view angles....
			m_edict->variables.modelAngles = ((client->m_edict->GetEyePosition () - m_edict->GetEyePosition ()).ToAngles2D () + m_edict->GetPunchAngles ()).Clamp ();

			m_edict->variables.modelAngles.pitch *= 1.0f / 3.0f;	// Adjust the body angle pitch to point the gun correctly.

			m_edict->variables.fixAngle = HalfLifeEngine::SDK::Constants::FixAngleType_Absolute;

			break;

		}
	}
#endif	// if 0
	if (g_server->GetC4Manager ().IsС4Planted ())
	{
		const float distance (g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.origin.GetDistance (GetOrigin ()));

		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.origin, GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
/*
		if (distance > C4_Radius)
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "C4 dmg=%.2f, distance=%.2f, possible damage=[1=0(%.2f), 2=0(%.2f)].\n", g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.damage, distance, damage1, damage2);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Dist=%.2f, dmg=[1=0(%.2f), 2=0(%.2f), 3=0(%.2f), 4=0(%.2f), 5=0(%.2f), 6=0(%.2f)].\n", distance, damage1, damage2, damage3, damage4, damage5, damage6);
		else
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "C4 dmg=%.2f, distance=%.2f, possible damage=[1=%.2f, 2=%.2f].\n", g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.damage, distance, damage1, damage2);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Dist=%.2f, dmg=[1=%.2f, 2=%.2f, 3=%.2f, 4=%.2f, 5=%.2f, 6=%.2f].\n", distance, damage1, damage2, damage3, damage4, damage5, damage6);
*/
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Dist=%.2f, dmg=%.2f, mywater=%i, water=%i, under water: %s.\n", distance, g_server->GetC4Manager ().GetC4 ()->GetExplosionDamage (m_edict->variables), m_edict->variables.waterLevel, g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.waterLevel, HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (Math::Vector3D (g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.origin.x, g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.origin.y, g_server->GetC4Manager ().GetC4 ()->GetEdict ()->variables.origin.z + 1.0f)) == HalfLifeEngine::SDK::Constants::Content_Water ? "true" : "false");
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "111: %s.\n", MemoryUtilities::GetOffsetAddressReference <HalfLifeEngine::SDK::Classes::Edict *const> (g_server->GetC4Manager ().GetC4 ()->GetEdict ()->privateData, 111)->GetClassName ().GetData ());
	}

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "OFFSET_PLAYER_NEXT_ATTACK_DELAY1=%.2f.\n", m_edict->GetPrivateDataReference <float> (HalfLifeEngine::SDK::Constants::OFFSET_PLAYER_NEXT_ATTACK_DELAY1));
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "41=%i.\n", MemoryUtilities::GetOffsetAddressReference <int> (g_server->GetGameRules (), 41));
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "13:f=%f, i=%i.\n", MemoryUtilities::GetOffsetAddressReference <float> (g_server->GetGameRules (), 13), MemoryUtilities::GetOffsetAddressReference <int> (g_server->GetGameRules (), 13));
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "192=%i.\n", MemoryUtilities::GetOffsetAddressReference <int> (m_edict->privateData, 192u));
/*
	if (IsUsesFlashlight ())
		reinterpret_cast <float *> (const_cast <HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *> (g_server->GetGameRules ()))[13u] = 5.0f;
*/
	m_edict->variables.moveType = IsUsesFlashlight () ? HalfLifeEngine::SDK::Constants::MOVETYPE_NOCLIP : HalfLifeEngine::SDK::Constants::MOVETYPE_WALK;

	if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
	{
	// Find the conveyor entity....
	for (HalfLifeEngine::SDK::Classes::Edict *conveyor (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_conveyor")); conveyor->IsValid (); conveyor = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (conveyor, "func_conveyor"))
	{
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), conveyor->variables.origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
	}

		if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <unsigned char> () == 1u)
		{
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

			UTIL_TraceHull (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (200.0f), HalfLifeEngine::SDK::Constants::Hull_Human, traceResult);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allSol=%s,startSol=%s,open=%s,wtr=%s,fract=%.2f,planeNrm=[%.2f,%.2f,%.2f].\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
		}
		else
		{
			Math::Vector3D source (m_edict->GetEyePosition ());
			Math::Vector3D destination (GetEyeForwardPositionWithPunchAngles (200.0f));
			const HalfLifeEngine::SDK::Structures::PMTraceResult_t &traceResult (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerMovePointer ()->PM_PlayerTrace (source, destination, HalfLifeEngine::SDK::Constants::PMPlayerTraceFlag_Normal, -1));

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allSol=%s,startSol=%s,open=%s,wtr=%s,fract=%.2f,planeNrm=[%.2f,%.2f,%.2f].\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.plane.normal.x, traceResult.plane.normal.y, traceResult.plane.normal.z);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
		}
//#endif	// if 0
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

		UTIL_TraceHull (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (200.0f), HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allSol=%s,startSol=%s,open=%s,wtr=%s,fract=%.2f,cn=%u,plane=%u[nrm=[%.2f,%.2f,%.2f],d=%.2f].\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.hitGroup, g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[static_cast <unsigned short> (traceResult.hitGroup)].planeNum, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z, traceResult.planeDistance);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
/*#if 0
		RayTrace trace (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (200.0f));

		trace.Execute ();

		const RayTrace::Result_t &traceResult (trace.GetResult ());

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allSol=%s,startSol=%s,open=%s,wtr=%s,fract=%.2f,planeNrm=[%.2f,%.2f,%.2f].\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.plane.normal.x, traceResult.plane.normal.y, traceResult.plane.normal.z);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
*
		static bool done (false);

		if (!done)
		{
			for (unsigned short leafIndex (0u); leafIndex < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs.GetElementNumber (); ++leafIndex)
			{
//				if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs[leafIndex].contents == HalfLifeEngine::SDK::Constants::Content_Clip)
//					HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "I found clip content! index=%u.", leafIndex);

					const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (static_cast <HalfLifeEngine::SDK::Constants::PointContentType_t> (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs[leafIndex].contents));

//					HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs[%u].contents=%s.", leafIndex, pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Origin ? "origin" : pointContents == HalfLifeEngine::SDK::Constants::Content_Clip ? "clip" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_0 ? "current 0" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_90 ? "current 90" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_180 ? "current 180" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_270 ? "current 270" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Up ? "current up" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Down ? "current down" : pointContents == HalfLifeEngine::SDK::Constants::Content_Translucent ? "translucent" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");
					AddLogEntry (true, LogLevel_Default, true, "g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().leafs[%u].contents=%s.", leafIndex, pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Origin ? "origin" : pointContents == HalfLifeEngine::SDK::Constants::Content_Clip ? "clip" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_0 ? "current 0" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_90 ? "current 90" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_180 ? "current 180" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_270 ? "current 270" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Up ? "current up" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Down ? "current down" : pointContents == HalfLifeEngine::SDK::Constants::Content_Translucent ? "translucent" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");
			}

			for (unsigned short clipNodeIndex (0u); clipNodeIndex < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes.GetElementNumber (); ++clipNodeIndex)
				for (unsigned char index (0u); index < 2u; ++index)
				{
					if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[clipNodeIndex].children[index] >= 0)
						continue;

					const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (static_cast <HalfLifeEngine::SDK::Constants::PointContentType_t> (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[clipNodeIndex].children[index]));

//					HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[%u].children[%u]=%s.", clipNodeIndex, index, pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Origin ? "origin" : pointContents == HalfLifeEngine::SDK::Constants::Content_Clip ? "clip" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_0 ? "current 0" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_90 ? "current 90" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_180 ? "current 180" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_270 ? "current 270" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Up ? "current up" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Down ? "current down" : pointContents == HalfLifeEngine::SDK::Constants::Content_Translucent ? "translucent" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");
					AddLogEntry (true, LogLevel_Default, true, "g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[%u].children[%u]=%s.", clipNodeIndex, index, pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Origin ? "origin" : pointContents == HalfLifeEngine::SDK::Constants::Content_Clip ? "clip" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_0 ? "current 0" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_90 ? "current 90" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_180 ? "current 180" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_270 ? "current 270" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Up ? "current up" : pointContents == HalfLifeEngine::SDK::Constants::Content_Current_Down ? "current down" : pointContents == HalfLifeEngine::SDK::Constants::Content_Translucent ? "translucent" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");
				}

			done = true;
		}
*/
/*		Math::Vector3D source (m_edict->GetEyePosition ());
		Math::Vector3D destination (GetEyeForwardPositionWithPunchAngles (200.0f));
		HalfLifeEngine::SDK::Structures::TraceResult_t MYtraceResult;

		UTIL_TraceHull (source, destination, HalfLifeEngine::SDK::Constants::Hull_Human, MYtraceResult);

		const HalfLifeEngine::SDK::Structures::PMTraceResult_t &traceResult (HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerMovePointer ()->PM_PlayerTrace (source, destination, HalfLifeEngine::SDK::Constants::PMPlayerTraceFlag_Normal, -1));
//		const HalfLifeEngine::SDK::Structures::PMTraceResult_t &traceResult (*HalfLifeEngine::Globals::g_halfLifeEngine->GetPlayerMovePointer ()->PM_TraceLine (source, destination, HalfLifeEngine::SDK::Constants::PMTraceLineFlag_PhysEntitiesOnly, 2, -1));
/*		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);
*/
/*		if (MYtraceResult.fraction != traceResult.fraction)
			AddLogEntry (false, LogLevel_Critical, true, "MYtraceResult.fraction(%.10f) != (%.10f)traceResult.fraction!\n\nDiff=%.10f.", MYtraceResult.fraction, traceResult.fraction, MYtraceResult.fraction - traceResult.fraction);
		if (MYtraceResult.endPosition != traceResult.endPosition)
			AddLogEntry (false, LogLevel_Critical, true, "MYtraceResult.endPosition(%.10f,%.10f,%.10f) != (%.10f,%.10f,%.10f)traceResult.endPosition!\n\nDiff=(%.10f,%.10f,%.10f).", MYtraceResult.endPosition.x, MYtraceResult.endPosition.y, MYtraceResult.endPosition.z, traceResult.endPosition.x, traceResult.endPosition.y, traceResult.endPosition.z, MYtraceResult.endPosition.x - traceResult.endPosition.x, MYtraceResult.endPosition.y - traceResult.endPosition.y, MYtraceResult.endPosition.z - traceResult.endPosition.z);
		InternalAssert (MYtraceResult.isAllSolid == traceResult.isAllSolid);
		InternalAssert (MYtraceResult.isStartSolid == traceResult.isStartSolid);
		InternalAssert (MYtraceResult.isInOpen == traceResult.isInOpen);
		InternalAssert (MYtraceResult.isInWater == traceResult.isInWater);
		InternalAssert (MYtraceResult.planeNormal == traceResult.plane.normal);
		InternalAssert (MYtraceResult.planeDistance == traceResult.plane.distance);
/*
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allSol=%s,strtSol=%s,open=%s,wtr=%s,fract=%.2f,ent=%s,planeNrm=[%.2f,%.2f,%.2f],hit=%i.\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.hitEntityIndex != -1 ? HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (static_cast <unsigned short> (traceResult.hitEntityIndex))->GetClassName ().GetData () : "NULL", traceResult.plane.normal.x, traceResult.plane.normal.y, traceResult.plane.normal.z, g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[static_cast <unsigned short> (traceResult.hitGroup)].planeNum);
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "allSol=%s,strtSol=%s,open=%s,wtr=%s,fract=%.2f,ent=%s,planeNrm=[%.2f,%.2f,%.2f],hit=%i.\n", traceResult.isAllSolid ? "true" : "false", traceResult.isStartSolid ? "true" : "false", traceResult.isInOpen ? "true" : "false", traceResult.isInWater ? "true" : "false", traceResult.fraction, traceResult.hitEntity != NULL ? traceResult.hitEntity->GetClassName ().GetData () : "NULL", traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z, g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().clipNodes[static_cast <unsigned short> (traceResult.hitGroup)].planeNum);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Texture: [name: \"%s\", type=\"%c\"].", HalfLifeEngine::Globals::g_halfLifeEngine->TraceTexture (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (0u), source, destination), HalfLifeEngine::Globals::g_halfLifeEngine->GetTextureType (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (0u), source, destination));
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Texture name: \"%s\".", HalfLifeEngine::Globals::g_halfLifeEngine->TraceTexture (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (0u), source, destination));
/*
		for (unsigned short modelIndex (0u); modelIndex < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models.GetElementNumber (); ++modelIndex)
			if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[modelIndex].origin.GetDistance (GetOrigin ()) <= 150.0f)
				HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[modelIndex].origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
//				HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[modelIndex].origin, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
*//*
		HalfLifeEngine::SDK::Structures::DFace_t *face;
		int faceIndex;
		int edgeIndex, edge;
		unsigned char edgeBoundary;
		WalkFace_t *walkFace;
		Math::Vector3D bound, middle;

		// loop through all the faces of the BSP file and count the number of walkable faces...
		for (faceIndex = 0; faceIndex < g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[0u].facesNum; ++faceIndex)
		{
			// quick access to the face
			face = g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().faces + (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().models[0u].firstFace + faceIndex);

			// if this face is NOT walkable (i.e, normal NOT pointing straight up OR it's a ceiling)
			if (g_server->GetNavigationMeshManager ().GetWorldMap ().GetBSPFile ().planes[static_cast <unsigned short> (face->planeNum)].normal.z >= Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_slope_limit)->GetValue <float> ())	// a simple check to see whether plane is flat or walkable < 45 degree
				continue;	// discard this face

			//
		}*/
	}
#if 0
//	if (m_currentWeapon->IsValid ())
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client m_currentWeapon->GetNextAttackDelay() == %.2f, in reload 1: %s, 2: %s.\n", m_currentWeapon->GetNextAttackDelay (), m_currentWeapon->IsInReload () ? "true" : "false", m_currentWeapon->IsInReload2 () ? "true" : "false");

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client in run: %s, punch angle == [%.2f, %.2f, %.2f].\n", (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_RUN) ? "true" : "false", m_edict->GetPunchAngles ().pitch, m_edict->GetPunchAngles ().yaw, m_edict->GetPunchAngles ().roll);

//	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (m_edict->GetEyePosition (), GetEyeForwardPosition (300.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u);
//	HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (300.0f)/*GetEyeForwardPosition (300.0f) + m_edict->GetPunchAngles ()*/, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u);

//	if (g_server->GetC4Manager ().IsС4Planted ())
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "C4 is defusing: %s.\n", g_server->GetC4Manager ().GetC4 ()->IsDefusing () ? "true" : "false");

	{
		static DynamicArray <Math::Vector3D> route;

		if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
		{
			route.RemoveAll ();

			if (NavAreaBuildPath (g_server->GetNavigationMeshManager ().GetMarkedArea (), m_lastNavigationArea, NULL, ShortestPathCost ()))
				for (const NavigationMesh::NavigationArea *area (m_lastNavigationArea); area != NULL; area = area->GetParent ())
					route += area->GetCenter ();
		}

		for (unsigned short index (1u); index < route.GetElementNumber (); ++index)
		{
			// ...now draw line from source to destination...
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (route[index - 1u], route[index], HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 15u, 0u, Color <> (255u, 100u, 55u), 200u, 5u, 1u);
		}
	}
#if 0
	if (m_edict->variables.buttons & HalfLifeEngine::SDK::Constants::IN_USE)
	{
/*		const Math::Vector3D &source (m_edict->GetEyePosition ());
		const Math::Vector3D &destination (GetEyeForwardPositionWithPunchAngles (200.0f));
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_All, NULL, traceResult);

		const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (traceResult.endPosition));

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "point contents: %s, hit entity: %s.\n", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL", traceResult.hitEntity->GetClassName ().GetData ());
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
*/
#if 0
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		static bool done (false);

//		if (!done)
		{
		for (HalfLifeEngine::SDK::Classes::Edict *ladder (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_ladder")); ladder->IsValid (); ladder = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (ladder, "func_ladder"))
		{
			unsigned char illusionaryCount (0u);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "ladder: %u.\n", ladder->GetIndex ());

//			for (HalfLifeEngine::SDK::Classes::Edict *illusionary (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_illusionary")); illusionary->IsValid (); illusionary = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (illusionary, "func_illusionary"))
			for (HalfLifeEngine::SDK::Classes::Edict *illusionary (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_wall")); illusionary->IsValid (); illusionary = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (illusionary, "func_wall"))
			{
				if (!IsBoundingBoxesTouching (illusionary->variables.absoluteBoundingBox.mins, illusionary->variables.absoluteBoundingBox.maxs, ladder->variables.absoluteBoundingBox.mins, ladder->variables.absoluteBoundingBox.maxs))
					continue;

				++illusionaryCount;

//				if (!done)
//					illusionary->Print (FormatBuffer ("C:/Users/Елин Эдуард/Desktop/%s_%s.txt", illusionary->GetClassName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ()));
/*
				illusionary->variables.className = HalfLifeEngine::Globals::g_halfLifeEngine->GetOffsetFromString ("func_wall");
				illusionary->variables.moveType = HalfLifeEngine::SDK::Constants::MOVETYPE_PUSH;
				illusionary->variables.solid = HalfLifeEngine::SDK::Constants::SOLID_BSP;
				illusionary->variables.skin = -1;
				illusionary->variables.renderMode = HalfLifeEngine::SDK::Constants::kRenderNormal;
				illusionary->variables.renderAmount = 0.0f;
				illusionary->variables.flags = HalfLifeEngine::SDK::Constants::FL_WORLDBRUSH;
*/
				HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (illusionary->variables.absoluteBoundingBox.mins, illusionary->variables.absoluteBoundingBox.maxs, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);

//				if (illusionaryCount == 1u)
//					continue;

//				HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), illusionary->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u);
				HalfLifeEngine::Globals::g_halfLifeEngine->DrawQuadrangle (illusionary->variables.absoluteBoundingBox.mins, illusionary->variables.absoluteBoundingBox.maxs, Math::Vector3D (illusionary->variables.absoluteBoundingBox.mins.x, illusionary->variables.absoluteBoundingBox.maxs.y, illusionary->variables.absoluteBoundingBox.mins.z), Math::Vector3D (illusionary->variables.absoluteBoundingBox.maxs.x, illusionary->variables.absoluteBoundingBox.mins.y, illusionary->variables.absoluteBoundingBox.maxs.z), Color <> (0u, 255u, 0u), 1u);
			}

			Math::Vector3D normal;
			const Math::Vector3D top (Math::GetMidPoint (ladder->variables.absoluteBoundingBox.mins.x, ladder->variables.absoluteBoundingBox.maxs.x), Math::GetMidPoint (ladder->variables.absoluteBoundingBox.mins.y, ladder->variables.absoluteBoundingBox.maxs.y), ladder->variables.absoluteBoundingBox.maxs.z);
			const Math::Vector3D bottom (top.x, top.y, ladder->variables.absoluteBoundingBox.mins.z);
			const Math::Vector2D size (ladder->variables.absoluteBoundingBox.maxs.x - ladder->variables.absoluteBoundingBox.mins.x, ladder->variables.absoluteBoundingBox.maxs.y - ladder->variables.absoluteBoundingBox.mins.y);

			if (size.x <= size.y)
			{
				// ladder is facing east or west - determine which way
				AddDirectionVector (normal, TraceLineThruWalkableEntities (bottom + Math::Vector3D (GenerationStepSize, 0.0f, GenerationStepSize * 0.5f), top + Math::Vector3D (GenerationStepSize, 0.0f, -GenerationStepSize * 0.5f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult) ? EAST : WEST, 1.0f);	// worst-case, we have the NavDirType as a normal
			}
			else
			{
				// ladder is facing north or south - determine which way
				// "pull in" traceline from bottom and top in case ladder abuts floor and/or ceiling
				AddDirectionVector (normal, TraceLineThruWalkableEntities (bottom + Math::Vector3D (0.0f, GenerationStepSize, GenerationStepSize * 0.5f), top + Math::Vector3D (0.0f, GenerationStepSize, -GenerationStepSize * 0.5f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult) ? SOUTH : NORTH, 1.0f);	// worst-case, we have the NavDirType as a normal
			}
/*
			const Math::Vector3D corner1 (Math::GetMidPoint (ladder->variables.absoluteBoundingBox.mins.x, ladder->variables.absoluteBoundingBox.maxs.x), ladder->variables.absoluteBoundingBox.maxs.y, ladder->variables.absoluteBoundingBox.mins.z);
			const Math::Vector3D corner2 (Math::GetMidPoint (ladder->variables.absoluteBoundingBox.mins.x, ladder->variables.absoluteBoundingBox.maxs.x), ladder->variables.absoluteBoundingBox.mins.y, ladder->variables.absoluteBoundingBox.mins.z);
			const Math::Vector3D &edge1 (corner1 - corner2);
			const Math::Vector3D &edge2 (top - corner2);
			const Math::Vector3D &normal2 (edge1.GetUnitCrossProduct (edge2));

			DrawTriangle (corner1, corner2, top, Color <> (0u, 255u, 0u), 1u);
*/
			const Math::Vector3D &from (top.GetMidPoint (bottom) + normal * 5.0f);
			const Math::Vector3D &to (from - normal * 32.0f);

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (from, to, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);

			const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (traceResult.endPosition));

			if (pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder)
				normal = traceResult.planeNormal;

//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (from, to, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);

//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (ladder->variables.absoluteBoundingBox.mins, ladder->variables.absoluteBoundingBox.maxs, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);

//			ladder->Print (FormatBuffer ("C:/Users/Елин Эдуард/Desktop/%s_%s.txt", ladder->GetClassName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ()));

//			if (pointContents != HalfLifeEngine::SDK::Constants::Content_Ladder/* || normal != normal2*/)
			{
//				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "contents: %s, hit entity: %s, normal: %s, normal2: %s.\n", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL", traceResult.hitEntity->GetClassName ().GetData (), normal.ToString (), normal2.ToString ());
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "contents: %s, hit entity: %s(%u), normal: %s.\n", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL", traceResult.hitEntity->GetClassName ().GetData (), traceResult.hitEntity->GetIndex (), normal.ToString ());
//				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "normal: [%.10f, %.10f, %.10f], normal2: [%.10f, %.10f, %.10f].\n", normal.x, normal.y, normal.z, normal2.x, normal2.y, normal2.z);
				HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), ladder->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u);
			}
		}
			done = true;
		}

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (200.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, NULL, traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (200.0f), true, HalfLifeEngine::SDK::Constants::Hull_Point, NULL, traceResult);

//		if (traceResult.hitEntity->IsValid ())
//			traceResult.hitEntity->Print (FormatBuffer ("C:/Users/Елин Эдуард/Desktop/%s_%s.txt", traceResult.hitEntity->GetClassName ().GetData (), HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName ().GetData ()));

		const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (traceResult.endPosition));

		HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult.endPosition, 5.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "ME: contents: %s, hit entity: %s, plane normal: %s.\n", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL", traceResult.hitEntity != NULL ? traceResult.hitEntity->GetClassName ().GetData () : "NULL", traceResult.planeNormal.ToString ());
#endif	// if 0
/*		Math::Vector3D destination (GetEyeForwardPositionWithPunchAngles (200.0f));

		if (NavigationMesh::GetGroundHeight (destination, &destination.z))
		{
			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
/*
			for (NavigationNode *node (NavigationNode::GetFirst ()); node != NULL; node = node->GetNext ())
			{
				if (destination.z == node->GetPosition ().z || destination.GetDistanceSquared (node->GetPosition ()) > 25.0f)
					continue;

				destination = node->GetPosition ();
*//*
			TraceHullThruWalkableEntities (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), destination - Math::Vector3D (0.0f, 0.0f, 4095.0f)/* + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z)*//*, true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult.endPosition/* + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z)*//* + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_DUCK.z) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
			const Math::Vector3D tmp (traceResult.endPosition);
			TraceHullThruWalkableEntities (traceResult.endPosition/* + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z)*//*, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_DUCK.z), true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (tmp + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle");
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s, destination.z(%f) != (%f)node->GetPosition ().z.\n", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle", destination.z, node->GetPosition ().z);
//			}
//			for (unsigned char corner (NORTH_WEST); corner < NUM_CORNERS; ++corner)
//				HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (destination, destination + GetCornerVector2D (static_cast <NavCornerType> (corner)) * HalfLifeEngine::SDK::Constants::HalfHumanWidth, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
/*			HalfLifeEngine::SDK::Structures::TraceResult_t traceResult, traceResult2;

//			TraceHullThruWalkableEntities (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
			TraceHullThruWalkableEntities (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult);
			TraceHullThruWalkableEntities (traceResult.endPosition/* + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z)*//*, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z * 2.0f), true, HalfLifeEngine::SDK::Constants::Hull_Head, traceResult2);

//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, destination + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_HUMAN_HULL_MAX.z - HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle");

//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z * 2.0f) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
//			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (traceResult2.endPosition + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult2.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z * 2.0f) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "HULL: fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult2.fraction, traceResult2.isStartSolid ? "true" : "fasle", traceResult2.isAllSolid ? "true" : "fasle");
//			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "HULL: fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult2.fraction, traceResult2.isStartSolid ? "true" : "fasle", traceResult2.isAllSolid ? "true" : "fasle");
/*
			TraceLineThruWalkableEntities (traceResult.endPosition/* + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z)*//*, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z * 2.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (traceResult.endPosition/* + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z)*//*, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z * 2.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "LINE: fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult2.fraction, traceResult2.isStartSolid ? "true" : "fasle", traceResult2.isAllSolid ? "true" : "fasle");

			TraceLineThruWalkableEntities (traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z) + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z * 2.0f), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult);

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z) + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z * 2.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "LINE2: fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult2.fraction, traceResult2.isStartSolid ? "true" : "fasle", traceResult2.isAllSolid ? "true" : "fasle");*/
//		}

/*		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult, traceResult2;
		const Math::Vector3D &source (m_edict->GetEyePosition ());
		const Math::Vector3D &destination (GetEyeForwardPositionWithPunchAngles (200.0f));

//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (destination, destination - Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::MapSize), HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, traceResult);
		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (destination, source, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, traceResult);
//		const bool result (TraceLineThruWalkableEntities (destination, source, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, traceResult));

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "hit: %s, fr=%.2f, startSolid: %s, allSolid: %s, open: %s, water: %s, pl: dis=%.2f, nrm=(%.2f, %.2f, %.2f).\n", traceResult.hitEntity != NULL ? traceResult.hitEntity->GetClassName ().GetData () : "NULL", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle", traceResult.isInOpen ? "true" : "fasle", traceResult.isInWater ? "true" : "fasle", traceResult.planeDist, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z);
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "r: %s, hit: %s, fr=%.2f, startSolid: %s, allSolid: %s, open: %s, water: %s, pl: dis=%.2f, nrm=(%.2f, %.2f, %.2f).\n", result ? "true" : "fasle", traceResult.hitEntity != NULL ? traceResult.hitEntity->GetClassName ().GetData () : "NULL", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle", traceResult.isInOpen ? "true" : "fasle", traceResult.isInWater ? "true" : "fasle", traceResult.planeDist, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z);
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "r: %s, fr=%.2f, startSolid: %s, allSolid: %s, open: %s, water: %s, pl: dis=%.2f, nrm=(%.2f, %.2f, %.2f).\n", result ? "true" : "fasle", traceResult.fraction, traceResult.isStartSolid ? "true" : "fasle", traceResult.isAllSolid ? "true" : "fasle", traceResult.isInOpen ? "true" : "fasle", traceResult.isInWater ? "true" : "fasle", traceResult.planeDist, traceResult.planeNormal.x, traceResult.planeNormal.y, traceResult.planeNormal.z);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), destination, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);

//		if (!traceResult.isStartSolid)
//		{
			const Math::Vector3D start (traceResult.endPosition.x, traceResult.endPosition.y, traceResult.endPosition.z + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z - Math::RawEpsilon);

			HalfLifeEngine::Globals::g_halfLifeEngine->TraceHull (start, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z), true, HalfLifeEngine::SDK::Constants::Hull_Head, m_edict, traceResult2);
//			TraceHull (start, traceResult.endPosition, Math::Vector2D (-HalfLifeEngine::SDK::Constants::HalfHumanWidth, -HalfLifeEngine::SDK::Constants::HalfHumanWidth), Math::Vector2D (HalfLifeEngine::SDK::Constants::HalfHumanWidth, HalfLifeEngine::SDK::Constants::HalfHumanWidth), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, traceResult2);

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawBox (start + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN, traceResult.endPosition + Math::Vector3D (0.0f, 0.0f, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX.z) + HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (start, traceResult2.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (traceResult2.endPosition, 5.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 255u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult2.fraction, traceResult2.isStartSolid ? "true" : "fasle", traceResult2.isAllSolid ? "true" : "fasle");
/*
			TraceHull (start, start/*traceResult.endPosition*//*, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MIN/*Math::Vector2D (-HalfLifeEngine::SDK::Constants::HalfHumanWidth, -HalfLifeEngine::SDK::Constants::HalfHumanWidth)*//*, HalfLifeEngine::SDK::Constants::VEC_DUCK_HULL_MAX/*Math::Vector2D (HalfLifeEngine::SDK::Constants::HalfHumanWidth, HalfLifeEngine::SDK::Constants::HalfHumanWidth)*//*, HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, traceResult2);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "MY: fraction=%.2f, isStartSolid: %s, isAllSolid: %s.\n", traceResult2.fraction, traceResult2.isStartSolid ? "true" : "fasle", traceResult2.isAllSolid ? "true" : "fasle");
		}*/
/*		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
		const Math::Vector3D &destination (GetEyeForwardPositionWithPunchAngles (150.0f));
		float z;
		Math::Vector3D normal;

		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (GetOrigin (), destination, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);

		if (NavigationMesh::GetGroundHeight (destination, &z, &normal))
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (Math::Vector3D (destination.x, destination.y, z), 10.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "GetGroundHeight(): true, z=%.2f, normal=%s.\n", z, normal.ToString ());
		}
		else
			HalfLifeEngine::Globals::g_halfLifeEngine->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "GetGroundHeight(): false.\n");
*
		HalfLifeEngine::SDK::Structures::TraceResult_t traceResult;
//		const Math::Vector3D &source (GetEyeForwardPositionWithPunchAngles (200.0f));
		const Math::Vector3D &source (m_edict->GetEyePosition () + Math::Angles3D (0.0f, GetViewAngles ().yaw, 0.0f).BuildForwardVector () * 200.0f);
		const Math::Vector3D &destination (m_edict->GetEyePosition ());

		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (destination, source, HalfLifeEngine::SDK::Constants::TraceIgnore_None, m_edict, traceResult);
//		HalfLifeEngine::Globals::g_halfLifeEngine->TraceLine (source, destination, HalfLifeEngine::SDK::Constants::TraceIgnore_All, m_edict, traceResult);
//		TraceHull (source, destination, Math::Vector2D (-HalfLifeEngine::SDK::Constants::HalfHumanWidth, -HalfLifeEngine::SDK::Constants::HalfHumanWidth), Math::Vector2D (HalfLifeEngine::SDK::Constants::HalfHumanWidth, HalfLifeEngine::SDK::Constants::HalfHumanWidth), HalfLifeEngine::SDK::Constants::TraceIgnore_Monsters, m_edict, traceResult);

//		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (source, traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (destination, traceResult.endPosition, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fraction=%.2f, hitEntity: %s.\n", traceResult.fraction, traceResult.hitEntity != NULL ? traceResult.hitEntity->GetClassName ().GetData () : "NULL");
/*
		if (traceResult.isStartSolid)
		{
			// set conditions....
			const Math::Vector3D &direction ((destination - source).Normalize ());	// 1 unit long
			Math::Vector3D newSource (source + direction);

			while (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (newSource) == HalfLifeEngine::SDK::Constants::Content_Solid)
				newSource += direction;

			InternalAssert (traceResult.endPosition = destination);

			// calculate time we left a solid, only valid if we started in solid
			const float leftFromSolidFraction (newSource.GetDistance (source) / destination.GetDistance (source));

			HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (newSource, newSource + Math::Vector3D (0.0f, 0.0f, leftFromSolidFraction * 1000.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "fraction=%.2f, leftFromSolidFraction=%.2f.\n", traceResult.fraction, leftFromSolidFraction);
		}
/*
		const HalfLifeEngine::SDK::Constants::PointContentType_t pointContents (HalfLifeEngine::Globals::g_halfLifeEngine->GetPointContents (GetEyeForwardPositionWithPunchAngles (100.0f)));

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "point contents: %s.\n", pointContents == HalfLifeEngine::SDK::Constants::Content_Empty ? "empty" : pointContents == HalfLifeEngine::SDK::Constants::Content_Solid ? "solid" : pointContents == HalfLifeEngine::SDK::Constants::Content_Water ? "water" : pointContents == HalfLifeEngine::SDK::Constants::Content_Slime ? "slime" : pointContents == HalfLifeEngine::SDK::Constants::Content_Lava ? "lava" : pointContents == HalfLifeEngine::SDK::Constants::Content_Sky ? "sky" : pointContents == HalfLifeEngine::SDK::Constants::Content_Ladder ? "ladder" : pointContents == HalfLifeEngine::SDK::Constants::Content_FlyField ? "fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_GravityFlyField ? "gravity fly field" : pointContents == HalfLifeEngine::SDK::Constants::Content_Fog ? "fog" : "NULL");
		HalfLifeEngine::Globals::g_halfLifeEngine->DrawLine (m_edict->GetEyePosition (), GetEyeForwardPositionWithPunchAngles (100.0f), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Arrow), 5u, 0u, 255u, 250u, 5u, 1u);*/
	}
#endif	// if 0
#endif	// if 0
#endif	// if 0
}
/*
template <const unsigned char maximumArgumentsNumber> inline const unsigned char GetActualArguments (DynamicString arguments[maximumArgumentsNumber])
{
	unsigned char currentArgumentsNumber;

	if (g_server->GetUseBotArgsReference ())
	{
		currentArgumentsNumber = 0u;

		for (unsigned char index (3u); index < static_cast <unsigned char> (-1); --index)
		{
			if (g_server->GetBotArgsReference (index) == NULL)
				continue;

			currentArgumentsNumber = index + 1u;

			arguments[index] = g_server->GetBotArgsReference (index);

			InternalAssert (!arguments[index].IsEmpty ());
		}
	}
	else
	{
		currentArgumentsNumber = Math::GetMinimumValueBetween (maximumArgumentsNumber, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentsCount_FromAll ());

		for (unsigned char index (0u); index < currentArgumentsNumber; ++index)
		{
			arguments[index] = HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument_FromAll (index);

			InternalAssert (!arguments[index].IsEmpty ());
		}
	}

	InternalAssert (currentArgumentsNumber > 0u);

	return currentArgumentsNumber;
}
*/
const Client::HandleExecutedCommandResult_t Client::HandleExecutedCommand (const DynamicString &command, const DynamicString &argument1)
{
	// this function is called whenever the client whose player entity is 'client' issues a client
	// command. How it works is that clients all have a global string in their client DLL that
	// stores the command string; if ever that string is filled with characters, the client DLL
	// sends it to the engine as a command to be executed. When the engine has executed that
	// command, that string is reset to zero. By the server side, we can access this string
	// by asking the engine with the CmdArgv(), CmdArgs() and CmdArgc() functions that work just
	// like executable files argument processing work in C (argc gets the number of arguments,
	// command included, args returns the whole string, and argv returns the wanted argument
	// only). Here is a good place to set up either bot debug commands the listen server client
	// could type in his game console, or real new client commands, but we wouldn't want to do
	// so as this is just a bot DLL, not a MOD. The purpose is not to add functionality to
	// clients. Hence it can lack of commenting a bit, since this code is very subject to change.

	// this player has issued a message; handle this message
/*
	const unsigned char maximumArgumentsNumber (6u);
	DynamicString arguments[maximumArgumentsNumber];
	const unsigned char currentArgumentsNumber (GetActualArguments <maximumArgumentsNumber> (arguments));
/*
	#if defined _DEBUG
		AddLogEntry (true, LogLevel_Default, true, "Client(\"%s\")::HandleExecutedCommand(command: \"%s\", argument1: \"%s\"): called!", GetName ().GetData (), command.GetData (), argument1.GetData ());
	#endif	// if defined _DEBUG
*/
	// Check if this player alive, and issue something....
	if (IsAlive ())
	{
		// Detect sent radio messages here....
		if (command.CompareWithCaseCheck ("radio", GET_STATIC_STRING_LENGTH ("radio")) == 0)
		{
			m_radioSelect = command.GetValue <HalfLifeEngine::SDK::Constants::RadioMenu_t> (GET_STATIC_STRING_LENGTH ("radio"));

			// Reliability check in case if server uses "radio4" giving by some AMXX plugin....
			if (m_radioSelect > HalfLifeEngine::SDK::Constants::RadioMenu_3)
				m_radioSelect = HalfLifeEngine::SDK::Constants::RadioMenu_None;

			return HandleExecutedCommandResult_Handled;
		}
		else if (GetNextRadioTime () <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
		{
			if (command == "menuselect")
			{
				if (argument1.IsEmpty ()/*currentArgumentsNumber == 1u*/)
					return HandleExecutedCommandResult_Ignored;

				// if this player's radio message menu is shown
				if (m_radioSelect > HalfLifeEngine::SDK::Constants::RadioMenu_None)
				{
					const unsigned char menuSelect (argument1.GetValue <unsigned char> ());

					// Reliability check. ('menuSelect' also can be zero in case the 'argument1' is empty)
					if (menuSelect >= 1u && (m_radioSelect == HalfLifeEngine::SDK::Constants::RadioMenu_3 ? menuSelect <= 9u : menuSelect <= 6u))
					{
						// Handle this message actually...
						g_server->GetYaPBManager ()->ReceiveRadioCommand (this, static_cast <HalfLifeEngine::SDK::Constants::RadioCommand_t> ((m_radioSelect - 1u) * 6u + menuSelect - 1u));

						// Reset handled radio command.
						m_radioSelect = HalfLifeEngine::SDK::Constants::RadioMenu_None;
					}

					return HandleExecutedCommandResult_Handled;
				}

				return HandleExecutedCommandResult_Ignored;
			}
			// Is radio alias?
			else for (unsigned char radioCommandIndex (HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe); radioCommandIndex < HalfLifeEngine::SDK::Constants::RadioCommand_Total; ++radioCommandIndex)
			{
				if (command != HalfLifeEngine::SDK::Constants::RadioCommandAliasNames[radioCommandIndex])
					continue;

				g_server->GetYaPBManager ()->ReceiveRadioCommand (this, static_cast <HalfLifeEngine::SDK::Constants::RadioCommand_t> (radioCommandIndex));	// Handle this message actually...

				return HandleExecutedCommandResult_Handled;
			}
		}
/*NOT SUPPORT YET!!!
		if (argument1.IsEmpty ()/*currentArgumentsNumber == 1u*//*)
			return HandleExecutedCommandResult_Ignored;

		if (command == "say" || command == "say_team")
		{
			if (argument1.CompareWithoutCaseCheck ("drop", GET_STATIC_STRING_LENGTH ("drop")) == 0 || argument1.CompareWithoutCaseCheck ("drp") == 0)
			{
				YaPB *bot;

				if (FindNearestPlayer (reinterpret_cast <void **> (&bot), this, 300.0f, true, true, true))
					bot->DiscardWeaponForUser (this, argument1.CompareWithoutCaseCheck ("c4", 2u) == 0 || argument1.CompareWithoutCaseCheck ("bomb", 4u) == 0);

				return HandleExecutedCommandResult_Handled;
			}
		}*/
	}
/*NOT SUPPORT YET!!!
	if (argument1.IsEmpty ()/*currentArgumentsNumber == 1u*//*)
		return HandleExecutedCommandResult_Ignored;

	bool isTeamSay (false);

	if (command == "say" || (isTeamSay = command == "say_team") == true)
	{
#if 0
/// @note OTHER METHOD - MORE FASTER AND STRANGE - I THINK.
	if (command.CompareWithoutCaseCheck ("say", GET_STATIC_STRING_LENGTH ("say")) == 0)
	{
		bool isTeamSay (false);

		if (command.GetElementNumber () > GET_STATIC_STRING_LENGTH ("say") && !(isTeamSay = command.CompareWithoutCaseCheck (GET_STATIC_STRING_LENGTH ("say"), "_team") == 0))
			return HandleExecutedCommandResult_Ignored;
#endif	// if 0
		// Reliability check.
		InternalAssert (!HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentString ().IsEmpty ());	// OCCURS!!!!!!!!!!!!!!

//		const unsigned char clientEdictIndex (static_cast <const unsigned char> (m_edict->GetIndex ()));

		for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
		{
			YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

			if ((isTeamSay && GetRealTeam () != bot->GetRealTeam ()) || IsAlive () != bot->IsAlive ())
				continue;
/*
			bot->m_sayTextBuffer.entityIndex = clientEdictIndex;

			strcpy (bot->m_sayTextBuffer.sayText, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgumentString ());

			bot->m_sayTextBuffer.timeNextChat = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + bot->m_sayTextBuffer.chatDelay;
*//*
			ReplyToMessage (bot, DynamicString (argument1), this, isTeamSay);
		}

		return HandleExecutedCommandResult_Handled;
	}
//*/
	return HandleExecutedCommandResult_Ignored;
}

void Client::LastNavigationAreaUpdate (void)
{
	NavigationMesh::NavigationArea *const currentNavigationArea (g_server->GetNavigationMeshManager ().GetNavArea (GetOrigin ()));
/*
	if (currentNavigationArea == NULL || currentNavigationArea == m_lastNavigationArea)
		return;
*/
	if (currentNavigationArea == m_lastNavigationArea)
		return;

	// Client entered a new navigation area.

	if (m_lastNavigationArea != NULL)
	{
		m_lastNavigationArea->RemovePlayer (this);

		if (currentNavigationArea != NULL)
			currentNavigationArea->SetClearedTimestamp (GetRealTeam ());
	}

	// generate event
	LastNavigationAreaChanged (currentNavigationArea);

	m_lastNavigationArea = currentNavigationArea;	// Store current navigation area as last known.

	if (currentNavigationArea == NULL)
		return;

	currentNavigationArea->AddPlayer (this);
/*
	if (currentNavigationArea->GetPlace () != NavigationMesh::UNDEFINED_PLACE)
	{
		const DynamicString &placeName (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (currentNavigationArea->GetPlace ()));

		if (!placeName.IsEmpty ())
//			m_lastPlaceName.Assign (placeName, 0u, Math::GetMinimumValueBetween <unsigned short> (placeName.GetElementNumber (), NavigationMesh::MAX_PLACE_NAME_LENGTH));
			m_lastPlaceName = placeName;
	}*/
}

void Client::LastNavigationAreaChanged (NavigationMesh::NavigationArea *const/* newNavigationArea*/)
{
	/// @note This function called before newNavigationArea->AddPlayer(this) call!!!
/*
	#if defined _DEBUG
		if (newNavigationArea == NULL)
			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::LastNavigationAreaChanged(): Client: \"%s\" last area %s changed to new NULL.\n", GetName ().GetData (), m_lastNavigationArea == NULL ? "NULL" : FormatBuffer ("%u", m_lastNavigationArea->GetID ()));
		else
		{
			const DynamicString &placeName (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (newNavigationArea->GetPlace ()));

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::LastNavigationAreaChanged(): Client: \"%s\" last area %s changed to new %u (place name: \"%s\").\n", GetName ().GetData (), m_lastNavigationArea == NULL ? "NULL" : FormatBuffer ("%u", m_lastNavigationArea->GetID ()), newNavigationArea->GetID (), placeName.IsEmpty () ? "None" : placeName.GetData ());
		}
	#endif	// if defined _DEBUG
*/
}
#if 0
void Client::SoundSimulateUpdate (void)
{
	// This function tries to simulate playing of sounds to let the bots hear sounds which aren't captured through server sound hooking.

	m_currentNoisedSound.hearingDistance = m_currentNoisedSound.timeLasting = 0.0f;
	m_currentNoisedSound.maximumLastingTime = 0.5f;

	// pressed attack button?
	if (IsAttacking ())
	{
		m_currentNoisedSound.hearingDistance = 3072.0f;
		m_currentNoisedSound.maximumLastingTime = 0.3f;
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;
	}
	else if (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::ButtonType_t> (HalfLifeEngine::SDK::Constants::Offset_Player_ButtonsPressed) & HalfLifeEngine::SDK::Constants::IN_USE)	// pressed used button?
	{
		m_currentNoisedSound.hearingDistance = 512.0f;
		m_currentNoisedSound.maximumLastingTime = 0.5f;
		m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;
	}
	else if (IsOnLadder ())	// uses ladder?
	{
		// moves fast enough?
		if (m_edict->GetSpeedSquared () > HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeedSquared)
		{
			m_currentNoisedSound.hearingDistance = 1024.0f;
			m_currentNoisedSound.maximumLastingTime = 0.3f;
			m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;
		}
	}
	else if (Console::Server::variableManager.GetVariable (Console::Server::VariableID_FootSteps)->GetValue <bool> ())	// foot steps?
	{
		if (IsOnFloor ())
		{
			const float currentSpeed2D (m_edict->GetSpeed2D ());

			// moves fast enough?
			if (currentSpeed2D > HalfLifeEngine::SDK::Constants::MaximumPlayerWalkSpeed)
			{
				m_currentNoisedSound.hearingDistance = 1280.0f * (currentSpeed2D / 240.0f);
				m_currentNoisedSound.maximumLastingTime = 0.3f;
				m_currentNoisedSound.timeLasting = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + m_currentNoisedSound.maximumLastingTime;
			}
		}
	}
	else	// didn't issue sound? - return
		return;

	UpdateLastNoisedSound ();
	TryReceiveLastNoisedSound ();
}
#endif	// if 0
void Client::CurrentWeaponChanged (const bool state, const HalfLifeEngine::SDK::Constants::WeaponID_t newWeaponIndex, const char newAmmoInClip)
{
	// ammo amount decreased? Must have fired a bullet...
	if (newWeaponIndex == m_currentWeapon->GetID () && newAmmoInClip < m_ammo[newWeaponIndex].clip)
		m_currentWeapon->OnFire ();

	// update the ammo in clip
	m_ammo[newWeaponIndex].clip = newAmmoInClip;
/*
	#if defined _DEBUG
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Client::CurrentWeaponChanged() called for client: \"%s\", newWeapon: \"%s\", newClip=%i, state=%s.\n", GetName ().GetData (), HalfLifeEngine::SDK::Constants::weaponProperties[newWeaponIndex].className.GetData (), newAmmoInClip, state ? "true" : "false");
	#endif	// if defined _DEBUG
/*
	if (state && m_currentWeapon->GetID () != newWeaponIndex)
		m_currentWeapon->m_id = newWeaponIndex;	// TEMP!!!
*/
	// Is this message called from BasePlayer::RemoveAllItems() function?
	if (!state && newWeaponIndex == HalfLifeEngine::SDK::Constants::WeaponID_None && newAmmoInClip == 0)
		m_owningItemsManager->RemoveAllItems ();
}

void Client::Killed (const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/)
{
	// Is called when the client/bot is killed.
	// Note: 'killer' could be not "player" entity if client dies by fall/acid/radiation/fire/etc damage/lack of oxygen or by touch to a "trigger_hurt" entity.

	SetDead ();	// Not alive.

	// Only count alive players.
	if (m_lastNavigationArea != NULL)
	{
		m_lastNavigationArea->RemovePlayer (this);

		m_lastNavigationArea = NULL;
	}
/* AUTOMATICLY FREES BY CurrentWeaponChanged() FUNCTION....
	// Client/bot is killed - reset his items....
	m_owningItemsManager->RemoveAllItems ();
*/
	{
		delete m_currentWeapon;

		m_currentWeapon = new CurrentWeapon (this);
	}
/*
	#if defined _DEBUG
		const HalfLifeEngine::SDK::Constants::PlayerBodyPart_t hitGroup (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::PlayerBodyPart_t> (HalfLifeEngine::SDK::Constants::Offset_BaseMonster_LastHitGroup));
		const HalfLifeEngine::SDK::Constants::DamageType_t damageTypes (m_edict->GetPrivateDataReference <HalfLifeEngine::SDK::Constants::DamageType_t> (HalfLifeEngine::SDK::Constants::Offset_BaseMonster_LastDamageTypeBits));
		const DynamicString hitGroupStrings[] =
		{
			"generic",	// None.

			"head",
			"chest",
			"stomach",
			"left arm",
			"right arm",
			"left leg",
			"right leg",
			"shield"
		};
		DynamicString damageTypesString;
		const DynamicString damageTypeStrings[] =
		{
			"crush",			// Crushed by falling or moving object.
			"bullet",			// Shot.
			"slash",			// Cut, clawed, stabbed.
			"burn",				// Heat burned.
			"freeze",			// Frozen.
			"fall",				// Fell too far.
			"blast",			// Explosive blast damage.
			"club",				// Crowbar, punch, headbutt.
			"shock",			// Electric shock.
			"sonic",			// Sound pulse shockwave.
			"energy beam",		// Laser or other high energy beam.

			"UNKNOWN",			// Unused by HL engine ?!? :(

			"never gib",		// With this bit OR'd in, no damage type will be able to gib victims upon death.
			"always gib",		// With this bit OR'd in, any damage type can be made to gib victims upon death.
			"drown",			// Drowning.
			"paralyze",			// Slows affected creature down.
			"nerve gas",		// Nerve toxins, very bad.
			"poison",			// Blood poisioning.
			"radiation",		// Radiation exposure.
			"drown recover",	// Drowning recovery.
			"acid",				// Toxic chemicals or acid burns.
			"slow burn",		// In an oven.
			"slow freeze",		// In a subzero freezer.
			"mortar",			// Hit by air raid (done to distinguish grenade from mortar).
			"HE grenade"		// Explosive grenade.
		};

		if (damageTypes == HalfLifeEngine::SDK::Constants::DamageType_Generic)
			damageTypesString = "generic";	// Generic damage was done.
//		else if (damageTypes & HalfLifeEngine::SDK::Constants::DamageType_TimeBased)
//			damageTypesString = "time based";	// Mask for time-based damage.
		else
		{
			STORE_FLAGS_TO_STRING (damageTypes, damageTypesString, 0u, GET_STATIC_ARRAY_SIZE (damageTypeStrings), damageTypeStrings, "+");
		}

		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Client::Killed(): Called for client: \"%s\", killer: \"%s\", hit group: %s, damage types: '%s'.\n", GetName ().GetData (), killer->IsPlayer () ? g_server->GetClientManager ()->GetClient (killer)->GetName ().GetData () : killer->GetClassName ().GetData (), hitGroupStrings[hitGroup].GetData (), damageTypesString.GetData ());
	#endif	// if defined _DEBUG*/
}

void Client::RoundStarted (void)
{
	// This function is called at the start of each round.

	// Reset some client variables....

	m_hostageManager.RoundStarted ();

//	m_radioSelect = 0;

	m_lastNoisedSound.Reset ();
}