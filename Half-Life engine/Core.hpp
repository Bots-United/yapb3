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
// Core.hpp
//
// Description: Contains the Half-Life engine SDK.
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef HALF_LIFE_ENGINE_CORE_INCLUDED
#define HALF_LIFE_ENGINE_CORE_INCLUDED

namespace HalfLifeEngine
{
	class Engine;

	namespace Globals
	{
		#include <Half-Life engine/Globals/Half_Life_engine_globals.hpp>
	}

	namespace Utilities
	{
		inline const float Engine__GetTimeWrapper (void);	/// @note Declared below....

		typedef CountdownTimer <Engine__GetTimeWrapper> CountdownTimer;
		typedef IntervalTimer <Engine__GetTimeWrapper> IntervalTimer;
	}

	namespace SDK
	{
		#include <Half-Life engine/SDK/SDK_definitions.hpp>	// Shared engine/DLL constants/structures/classes.
	}

	namespace Utilities
	{
		#include <Half-Life engine/Utilities.hpp>
	}

	namespace Export
	{
		#include <Half-Life engine/Export/Export_definitions.hpp>
	}

	#include <Half-Life engine/Engine.hpp>

	namespace Utilities
	{
		inline const float Engine__GetTimeWrapper (void) { return Globals::g_halfLifeEngine->GetTime (); }

		#include <Half-Life Engine/FileBuffer.hpp>
		#include <Half-Life Engine/FileSystemFile.hpp>

		inline const bool IsPlayerEdictIndex (const unsigned short index)
		{
			// Use a unsigned overflow....
			return index - 1u < Globals::g_halfLifeEngine->GetMaximumClients ();
		}
		inline const bool IsNotWorldspawnPlayerEdictIndex (const unsigned short index)
		{
			return index <= Globals::g_halfLifeEngine->GetMaximumClients ();
		}

		struct PseudoGrenadeEntityVariables_t
		{
			Math::Vector3D                     origin;
			SDK::Constants::EntityWaterLevel_t waterLevel;
			SDK::Constants::EntityFlag_t       flags;	// flags
		};
		extern void TraceThrownGrenadeToss (const SDK::Classes::Edict *const grenade, PseudoGrenadeEntityVariables_t &pseudoGrenadeEntityVariables);
		extern inline const PseudoGrenadeEntityVariables_t TraceThrownGrenadeToss (const SDK::Classes::Edict *const grenade)
		{
			PseudoGrenadeEntityVariables_t pseudoGrenadeEntityVariables;

			TraceThrownGrenadeToss (grenade, pseudoGrenadeEntityVariables);

			return pseudoGrenadeEntityVariables;
		}
	}

	namespace SDK
	{
		namespace Classes
		{
			#include <Half-Life engine/SDK/Classes/ConsoleVariable.hpp>
			#include <Half-Life engine/SDK/Classes/Edict.hpp>

			inline Edict *const EntityHandle::Get (void) const { return m_edict != NULL && m_edict->serialNumber == m_serialNumber ? m_edict : NULL; }
			inline Edict *const EntityHandle::Set (Edict *const edict)
			{
				m_edict = edict;

				if (edict != NULL)
					m_serialNumber = edict->serialNumber;

				return edict;
			}
			inline BaseEntity *const EntityHandle::GetEntity (void) const
			{
				Edict *const edict (Get ());

				return edict == NULL ? NULL : edict->privateData;
			}
			inline void EntityHandle::SetEntity (BaseEntity *const entity)
			{
				if (entity != NULL)
				{
					m_edict = entity->GetEdict ();

					if (m_edict != NULL)
						m_serialNumber = m_edict->serialNumber;
				}
				else
				{
					m_edict = NULL;
					m_serialNumber = 0;
				}
			}
/*
			#if defined MEMORY_MANAGER
			#	undef new
			#endif	// if defined MEMORY_MANAGER

				// allow engine to allocate instance data
				void *const BaseEntity::operator new (size_t allocateBlockSize, Structures::EntityVariables_t *const newEntityVariables)
				{
					return Globals::g_halfLifeEngine->AllocateEntityPrivateData (newEntityVariables->containingEntity, allocateBlockSize);
				}

			#if defined MEMORY_MANAGER
			#	define new (m_setOwner (__FILE__, __LINE__, __FUNCTION__), false) ? NULL : new
			#endif	// if defined MEMORY_MANAGER
*/
			inline const unsigned char BaseEntity::GetIllumination (void) const
			{
				// Ask the engine for the entity illumination and filter it so as to return a usable value.
				return Globals::g_halfLifeEngine->GetEntityIllumination (m_variables->containingEntity);
			}

			inline const bool HalfLifeMultiplay::IsTeamStacked (const Constants::TeamID_t currentTeamID, const Constants::TeamID_t newTeamID) const
			{
				// This function checks to see if the desired team is stacked, returns true if it is.

				// players are allowed to change to their own team
				if (currentTeamID == newTeamID)
					return false;

				switch (newTeamID)
				{
					case Constants::TeamID_Terrorist:
					{
						const unsigned char limitTeams (Globals::g_halfLifeEngine->ConsoleVariable_GetValue <unsigned char> ("mp_limitteams"));

						if (Utilities::IsSpectatorTeam (currentTeamID))
							return m_playerCount[TeamArrayID_Terrorist] >= m_playerCount[TeamArrayID_CounterTerrorist] + limitTeams;

						return m_playerCount[TeamArrayID_Terrorist] + 1u > m_playerCount[TeamArrayID_CounterTerrorist] + limitTeams - 1u;
					}

					case Constants::TeamID_CounterTerrorist:
					{
						const unsigned char limitTeams (Globals::g_halfLifeEngine->ConsoleVariable_GetValue <unsigned char> ("mp_limitteams"));

						if (Utilities::IsSpectatorTeam (currentTeamID))
							return m_playerCount[TeamArrayID_CounterTerrorist] >= m_playerCount[TeamArrayID_Terrorist] + limitTeams;

						return m_playerCount[TeamArrayID_CounterTerrorist] + 1u > m_playerCount[TeamArrayID_Terrorist] + limitTeams - 1u;
					}

					default:
						return false;
				}
			}
			inline const float HalfLifeMultiplay::GetRoundRemainingTime (void) const
			{
				return m_roundTimeSecs - Globals::g_halfLifeEngine->GetTime () + GetRoundStartTime ();
			}
			inline const float HalfLifeMultiplay::GetRoundElapsedTime (void) const
			{
				return Globals::g_halfLifeEngine->GetTime () - GetRoundStartTime ();
			}
		}
	}

	namespace Utilities
	{
		inline const bool IsInShootCone (const Math::Vector3D &origin, const SDK::Classes::Edict *const edict, const float distance, const float radius, const float minAngle)
		{
			/*
				<----- 'distance' ---->

							  ___....->T^             ^
				   ...----''''          | <- 'radius' |
				 O ------------------->Qv             |
				 ^ '''----....___                     | <- diameter (radius * 2)
				 |               ''''->               v
				 |
				Client(edict)

				T: Target (origin)

				if angle Q-O-T is less than 'minAngle', always return true.
			*/

			if (distance < Math::OnEpsilon)
				return true;

			// angle between forward-view-vector and vector to player (as cos(angle))
			const float dot ((origin - edict->GetEyePosition ()).Normalize () | edict->variables.viewAngles.BuildForwardVector ());

			if (dot > Math::cosf (Math::DegreeToRadian (minAngle)))	// smaller angle, bigger cosine
				return true;

			const Math::Vector2D triangle (distance, radius);

			// full angle of shootcode at this distance (as cos(angle))
			return dot > distance / triangle.GetLength ();	// smaller angle, bigger cosine
		}

		inline SDK::Classes::Edict *const GetEdictFromAreaLink (const SDK::Structures::Link_t *const link)
		{
			return GET_STRUCTURE_FROM_LINK (link, SDK::Classes::Edict, area);
		}
	}

	namespace Export
	{
		namespace Entities
		{
			#include <Half-Life engine/Export/Entities.hpp>
		}
	}

	inline void Engine::GiveNamedItem (SDK::Classes::Edict *const player, const char *const className) const
	{
		SDK::Classes::Edict *const item (CreateNamedEntity (className));

		// Reliability check.
		InternalAssert (item->IsValid ());

		item->variables.origin = player->variables.origin;	// Same origin as owner.
		item->variables.spawnFlags |= SDK::Constants::EntitySpawnFlag_NoRespawn;	// Never respawn.

		Spawn (item);
		Touch (item, player);
	}

	inline void       Engine::WriteEntity    (const SDK::Classes::Edict *const entity) const { WriteEntity (entity->GetIndex ()); }
	inline const char Engine::GetTextureType (SDK::Classes::Edict *const entity, const Math::Vector3D &source, const Math::Vector3D &destination)                                                                                          const
	{
		// Reliability check.
		InternalAssert (entity != NULL);

		// Is worldspawn edict?
		if (entity->IsWorldspawn ())
		{
			// get texture from entity or world (world is ent(0))
			const char *textureName (TraceTexture (entity, source, destination));

			// Reliability check.
			InternalAssert (textureName != NULL);
//			if (textureName != NULL)
//			{
				// strip leading '-0' or '+0~' or '{' or '!'
				if (textureName[0u] == '-' || textureName[0u] == '+')
					textureName += 2u;

				if (textureName[0u] == '{' || textureName[0u] == '!' || textureName[0u] == '~' || textureName[0u] == ' ')
					++textureName;

				// '}}'
				// get texture type
				return FindTextureType (textureName);
//			}
		}
		else if (entity->IsNotWorldspawnPlayer ())	// Is player?
			return SDK::Constants::TextureType_Flesh;	// hit body

		return SDK::Constants::TextureType_None;
	}

	inline void Engine::DirectorHudMessage (SDK::Classes::Edict *const client, const Math::Vector2D &coordinates, const Color <> &color, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const char *const message) const
	{
		const unsigned char messageLength (GetStringLength <unsigned char> (message));

		// Reliability check.
		InternalAssert (messageLength <= SDK::Constants::MAX_DIRECTOR_CMD_STRING);

		MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, SDK::Constants::SVC_DIRECTOR, NULL, client);
			WriteByte (sizeof (unsigned char) + sizeof (unsigned char) + sizeof (unsigned int) + sizeof (Math::Vector2D) + sizeof (float) + sizeof (float) + sizeof (float) + sizeof (float) + messageLength + sizeof ('\0'));	// Total director message length
			WriteByte (SDK::Constants::DRC_CMD_MESSAGE);
			WriteByte (static_cast <const unsigned char> (effect));	// effect (fade in/out)
			WritePackedColor (color);	// Color
			WriteFloat (coordinates.x);	// x coordinates
			WriteFloat (coordinates.y);	// y coordinates
			WriteFloat (fadeInTime);	// fade-in time in seconds
			WriteFloat (fadeOutTime);	// fade-out time in seconds
			WriteFloat (holdTime);	// hold time in seconds
			WriteFloat (fxTime);	// fx time in seconds
			WriteString (message);	// write message
		MessageEnd ();
	}
	inline void Engine::HudMessage (SDK::Classes::Edict *const client, const Math::Vector2D &coordinates, const ColorWithAlpha <> &color, const ColorWithAlpha <> &effectColor, const SDK::Constants::HUDMessageEffect_t effect, const float fxTime, const float holdTime, const float fadeInTime, const float fadeOutTime, const SDK::Constants::HUDMessageChannel_t channel, const char *const message) const
	{
		// Reliability checks.
		InternalAssert (client->IsValid ());
		InternalAssert (client->IsNotWorldspawnPlayer ());
		InternalAssert (GetStringLength <unsigned short> (message) <= SDK::Constants::MaximumClientHudMessageLength);

		MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, SDK::Constants::SVC_TEMPENTITY, NULL, client);
			WriteByte (SDK::Constants::TempEntityEvent_TextMessage);
			WriteByte (static_cast <const char> (channel));	// channel
			WriteShort (Utilities::GetFixedSigned16 (coordinates.x, 13u));	// x coordinates
			WriteShort (Utilities::GetFixedSigned16 (coordinates.y, 13u));	// y coordinates
			WriteByte (static_cast <const unsigned char> (effect));			// effect (fade in/out)
			WriteColor (color);	// Color
			WriteColor (effectColor);	// Effect color
			WriteShort (Utilities::GetFixedUnsigned16 (fadeInTime, 8u));	// fade-in time in seconds
			WriteShort (Utilities::GetFixedUnsigned16 (fadeOutTime, 8u));	// fade-out time in seconds
			WriteShort (Utilities::GetFixedUnsigned16 (holdTime, 8u));		// hold time in seconds

			if (effect == SDK::Constants::HUDMessageEffect_2)
				WriteShort (Utilities::GetFixedUnsigned16 (fxTime, 8u));	// fx time in seconds (effect time - only for effect 2)

			WriteString (message);	// write message
		MessageEnd ();
	}

	inline void Engine::Print (SDK::Classes::Edict *const client, const SDK::Constants::PrintType_t type, const char *const message) const
	{
		// Print a text message to this client.

		// Reliability checks.
		InternalAssert (client->IsValid ());
		InternalAssert (client->IsNotWorldspawnPlayer ());

		ClientPrint (client, type, message/*FormatBuffer ("[%s] %s", PRODUCT_LOG_TAG, message)*/);
	}

	inline void Engine::Print (SDK::Classes::Edict *const client, const SDK::Constants::HUDPrintType_t type, const char *const message) const
	{
		// Reliability checks.
		InternalAssert (client->IsValid ());
		InternalAssert (client->IsNotWorldspawnPlayer ());
		InternalAssert (GetStringLength <unsigned short> (message) <= SDK::Constants::MaximumTextMessageLength);

		MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_Text), NULL, client);
			WriteByte (static_cast <unsigned char> (type));
			WriteString (message);
		MessageEnd ();
	}

	inline void Engine::DrawLine (SDK::Classes::Edict *const client, const Math::Vector3D &source, const Math::Vector3D &destination, const short spriteIndex, const unsigned char width, const unsigned char noise, const Color <> &color, const unsigned char brightness, const unsigned char speed, const unsigned char duration) const	// Purpose: Draws a line from one position to another.
	{
		// This function draws a line visible from the client side of the player whose player entity
		// is pointed to by 'client', from the vector location 'source' to the vector location 'destination',
		// which is supposed to last life tenths seconds, and having the color defined by RGB.

		// Reliability checks.
		InternalAssert (client->IsValid ());
		InternalAssert (client->IsNotWorldspawnPlayer ());

		// send this client a packet telling him to draw a beam using the specified parameters
		MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, SDK::Constants::SVC_TEMPENTITY, NULL, client);
			WriteByte (SDK::Constants::TempEntityEvent_BeamPoints);
			WriteCoordinates (destination);	// source position
			WriteCoordinates (source);	// destination position
			WriteShort (spriteIndex);	// sprite index
			WriteByte (0u);		// starting frame
			WriteByte (10u);	// frame rate in 0.1's
			WriteByte (duration);	// life in 0.1's
			WriteByte (width);	// line width in 0.1's
			WriteByte (noise);	// noise amplitude in 0.01's
			WriteColor (color);	// Color
			WriteByte (brightness);	// brightness
			WriteByte (speed);	// scroll speed in 0.1's
		MessageEnd ();
	}

	inline void Engine::DisplayMenu (SDK::Classes::Edict *const client, const char *text, const SDK::Constants::Menu::KeyBit_t validSlots) const
	{
		// Reliability checks.
		InternalAssert (client->IsValid ());
		InternalAssert (client->IsNotWorldspawnPlayer ());
		InternalAssert (!IsNullOrEmptyString (text));

		unsigned short textLength (GetStringLength <unsigned short> (text));

		// Reliability check.
		InternalAssert (textLength <= SDK::Constants::MaximumMenuFullStringLength);

		while (textLength >= SDK::Constants::MaximumMenuStringLength)
		{
			MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_ShowMenu), NULL, client);
				WriteShort (validSlots);	// used key slots
				WriteCharacter (-1);	// lifetime seconds (forever)
				WriteByte (true);	// needmore? (yes)
/*
				const char tempCharacter (text[SDK::Constants::MaximumMenuStringLength]);	// Save last character....

				text[SDK::Constants::MaximumMenuStringLength] = '\0';	// Terminate the string for WriteString()....

					WriteString (text);	// Menu text to display (character to character)

				text[SDK::Constants::MaximumMenuStringLength] = tempCharacter;	// Save last character....
*/
				for (unsigned char index (0u); index < SDK::Constants::MaximumMenuStringLength; ++index)
					WriteCharacter (text[index]);	// Menu text to display (character to character)

			MessageEnd ();

			text += SDK::Constants::MaximumMenuStringLength;
			textLength -= SDK::Constants::MaximumMenuStringLength;
		}

		// And send the final message....
		MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_ShowMenu), NULL, client);
			WriteShort (validSlots);	// used key slots
			WriteCharacter (-1);	// lifetime seconds (forever)
			WriteByte (false);	// needmore? (no)
			WriteString (text);	// Menu text to display
		MessageEnd ();
	}
	inline void Engine::CloseMenu (SDK::Classes::Edict *const client) const
	{
		// Reliability checks.
		InternalAssert (client->IsValid ());
		InternalAssert (client->IsNotWorldspawnPlayer ());

		// Reset the currently displaying menu.
		MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_ShowMenu), NULL, client);
			WriteShort (SDK::Constants::Menu::KeyBit_None);	// used key slots
			WriteCharacter (0);	// lifetime seconds (none)
			WriteByte (false);	// needmore? (no)
			WriteString (NULL);	// Menu text to display
		MessageEnd ();
	}

	inline void Engine::DisplayMOTD (SDK::Classes::Edict *const client, const DynamicString &name, const char *text) const
	{
		// Reliability check.
		InternalAssert (!IsNullOrEmptyString (text));

		if (!name.IsEmpty ())
		{
			// send the MOTD name
			MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_ServerName), NULL, client);
				WriteString (name);
			MessageEnd ();
		}

		unsigned short textLength (GetStringLength <unsigned short> (text));

		while (textLength >= SDK::Constants::MaximumMOTDTextLength)
		{
			MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_MOTD), NULL, client);
				WriteByte (false);

				for (unsigned short index (0u); index < SDK::Constants::MaximumMOTDTextLength; ++index)
					WriteCharacter (text[index]);

			MessageEnd ();

			text += SDK::Constants::MaximumMOTDTextLength;
			textLength -= SDK::Constants::MaximumMOTDTextLength;
		}

		MessageBegin (SDK::Constants::MSG_ONE_UNRELIABLE, GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (GameMessageContainer::Type_MOTD), NULL, client);
			WriteByte (true);
			WriteString (text);
		MessageEnd ();
	}

	inline const bool Engine::TraceFilter_Simple::PassServerEntityFilter (const SDK::Classes::Edict *touch, const SDK::Classes::Edict *const pass)
	{
		if (pass == NULL)
			return true;

		if (touch == pass)
			return false;

		// don't clip against own missiles or owner
		return touch->variables.owner != pass && pass->variables.owner != touch;
	}
	inline const bool Engine::TraceFilter_Simple::ShouldHitEntity (SDK::Classes::Edict *const edict)
	{
		if (!StandardFilterRules (edict->privateData))
			return false;

		if (m_passEdict != NULL && !PassServerEntityFilter (edict, m_passEdict))
			return false;

		// Don't test if the game code tells us we should ignore this collision...
		SDK::Classes::BaseEntity *const pEntity (edict->privateData);

		if (pEntity == NULL)
			return false;
/*
		if (Globals::g_halfLifeEngine->ShouldCollide (edict, m_passEdict))	// INSTEAD OF BELOW CHECK!!!
			return false;
//		if (!pEntity->ShouldCollide (m_collisionGroup) || !g_pGameRules->ShouldCollide (m_collisionGroup, pEntity->GetCollisionGroup ()))
//			return false;
*/
		return true;
	}
	inline const bool Engine::TraceFilter_NoNPCsOrPlayers::ShouldHitEntity (SDK::Classes::Edict *const edict)
	{
		if (TraceFilter_Simple::ShouldHitEntity (edict))
		{
			SDK::Classes::BaseEntity *const pEntity (edict->privateData);

			if  (pEntity == NULL)
				return NULL;

			if (pEntity->CallClassify () == SDK::Constants::CLASS_PLAYER_ALLY)
				return false;	// CS hostages are CLASS_PLAYER_ALLY but not IsNPC()

			return /*!pEntity->IsNPC () && */!pEntity->CallIsPlayer ();
		}

		return false;
	}
	inline const bool Engine::TraceFilter_WalkableEntities::IsEntityWalkable (const SDK::Classes::Edict *const entity, const IgnoreEntityFlag_t flags)
	{
		// Return true if given entity can be ignored when moving

		if (entity->GetIndex () == SDK::Constants::WorldspawnEntitySlotIndex/* || entity->privateData->CallIsPlayer ()*/)
			return false;
/*
		// if we hit a clip brush, ignore it if it is not BRUSHSOLID_ALWAYS
		if (entity->GetClassName () == "func_brush")
		{
			switch (static_cast <CFuncBrush *> (entity)->m_iSolidity)
			{
				case CFuncBrush::BRUSHSOLID_ALWAYS:
					return false;

				case CFuncBrush::BRUSHSOLID_NEVER:
					return true;

				case CFuncBrush::BRUSHSOLID_TOGGLE:
					return (flags & WALK_THRU_TOGGLE_BRUSHES) > 0u;
			}

			return true;
		}
*/
		// if we hit a door, assume its walkable because it will open when we touch it
		if ((flags & WALK_THRU_DOORS) && entity->GetClassName ().CompareWithCaseCheck ("func_door", GET_STATIC_STRING_LENGTH ("func_door")) == 0)
			return true;

		// if we hit a breakable object, assume its walkable because we will shoot it when we touch it
		if ((flags & WALK_THRU_BREAKABLES) && entity->IsBreakable () && entity->IsBreakableShootable ())
			return true;

		if ((flags & WALK_THRU_PUSHABLES) && entity->GetClassName () == "func_pushable")
			return true;

		if ((flags & WALK_THRU_GLASS) && entity->IsTransparent ())
			return true;

		return false;
	}
	inline const bool Engine::TraceFilter_HitWorldOnly::ShouldHitEntity (SDK::Classes::Edict *const edict)
	{
		return edict->GetIndex () == SDK::Constants::WorldspawnEntitySlotIndex;
	}
	inline const bool Engine::TraceFilter_OnlyNPCsAndPlayers::ShouldHitEntity (SDK::Classes::Edict *const edict)
	{
		if (TraceFilter_Simple::ShouldHitEntity (edict))
		{
			SDK::Classes::BaseEntity *const pEntity (edict->privateData);

			if (pEntity == NULL)
				return false;

			if (pEntity->CallClassify () == SDK::Constants::CLASS_PLAYER_ALLY)
				return true;	// CS hostages are CLASS_PLAYER_ALLY but not IsNPC()

			return /*pEntity->IsNPC () || */pEntity->CallIsPlayer ();
		}

		return false;
	}
	inline const bool Engine::TraceFilter_SkipClassName::ShouldHitEntity (SDK::Classes::Edict *const edict)
	{
		if (edict->privateData == NULL || edict->GetClassName () == m_className)
			return false;

		return TraceFilter_Simple::ShouldHitEntity (edict);
	}
	inline const bool Engine::TraceFilter_SimpleClassNameList::ShouldHitEntity (SDK::Classes::Edict *const edict)
	{
		if (edict->privateData == NULL)
			return false;

		for (unsigned char index (0u); index < m_passClassNames.GetElementNumber (); ++index)
			if (edict->GetClassName () == m_passClassNames[index])
				return false;

		return TraceFilter_Simple::ShouldHitEntity (edict);
	}

	inline void Engine::TraceHullThroughAllEntities (const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::Hull_t hullNumber, Interface_TraceFilter &traceFilter, SDK::Structures::TraceResult_t &traceResult) const
	{
		SDK::Structures::TraceResult_t tempTraceResult;
		SDK::Classes::Edict *entity (GetEdictOfIndex (SDK::Constants::WorldspawnEntitySlotIndex));	// worldspawn
		float worldFraction;

		if (traceFilter.ShouldHitEntity (entity))
		{
			TraceEntity (source, destination, hullNumber, entity, traceResult);

			if (traceResult.fraction == 0.0f)
				return;	// blocked immediately by the world

			worldFraction = traceResult.fraction;	// Save the world collision fraction.
		}
		else
		{
			// Fill in a default trace result
			memset (&traceResult, 0, sizeof (traceResult));

			traceResult.fraction = worldFraction = 1.0f;
			traceResult.endPosition = destination;
		}

		// create the bounding box of the entire move
		// we can limit it to the part of the move not
		// already clipped off by the world, which can be
		// a significant savings for line of sight and shot traces
		Math::Extent3D extent (SDK::Constants::hullSizes[hullNumber].GetWithMovedBounds (source, traceResult.endPosition));

		// For each entity.... (Warning: VERY SLOW!)
		for (unsigned short index (SDK::Constants::WorldspawnEntitySlotIndex + 1u/* Skip worldspawn entity.... (already checked) */); index < m_globalVariables->maximumEntities; ++index)
		{
			++entity;

			if (!entity->IsValid ())
				continue;

			// Crash fix....
			if (entity->variables.boundingBox.mins == Math::Vector3D::ZeroValue || entity->variables.boundingBox.maxs == Math::Vector3D::ZeroValue)
			{
				#if !defined _DEBUG
				#	pragma message ("REMOVE THIS SHIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
				#endif	// if !defined _DEBUG

				InternalAssert (entity->variables.solid == SDK::Constants::SOLID_NOT);

//				entity->Print ("C:/Users/Эдуард/Desktop/ClipMoveToEntities.log");
//				ExitProcess (1u);
				continue;	// deactivated
			}

			// Reliability checks.
			InternalAssert (entity->variables.className > 0);
			InternalAssert (entity->privateData != NULL);

			if (!extent.IsTouching (entity->variables.absoluteBoundingBox))
				continue;	// not touching

			if (!traceFilter.ShouldHitEntity (entity))
				continue;

			TraceEntity (source, destination, hullNumber, entity, tempTraceResult);

			if (tempTraceResult.isAllSolid || tempTraceResult.isStartSolid || tempTraceResult.fraction < traceResult.fraction)
			{
				// Make sure the ray is always shorter than it currently is
/*
				if (!traceFilter.ShouldHitEntity (entity))
					continue;
*/
				if (traceResult.isStartSolid)
				{
					traceResult = tempTraceResult;
					traceResult.isStartSolid = true;
				}
				else
					traceResult = tempTraceResult;

				// might intersect, so do an exact clip
				if (traceResult.isAllSolid)
					break;	// Stop if we're in allsolid

				// Update our extent....
				extent.MoveBounds (source, traceResult.endPosition);
			}
		}

		// Fix up the fraction so it's appropriate given the original unclipped-to-world ray
		traceResult.fraction *= worldFraction;
	}
	inline void Engine::ClipMoveToEntities (const SDK::Structures::EdictAreaNode_t *const node, Math::Extent3D &extent, const Math::Vector3D &source, const Math::Vector3D &destination, const SDK::Constants::Hull_t hullNumber, Interface_TraceFilter &traceFilter, SDK::Structures::TraceResult_t &traceResult) const
	{
		SDK::Structures::TraceResult_t tempTraceResult;

		for (const SDK::Structures::Link_t *link (node->solidEdicts.next), *next; link != &node->solidEdicts; link = next)
		{
			SDK::Classes::Edict *const entity (Utilities::GetEdictFromAreaLink (link));

			next = link->next;

			// Reliability checks.
			InternalAssert (entity->IsValid ());
			InternalAssert (entity->variables.className > 0);
			InternalAssert (entity->privateData != NULL);

			// Crash fix....
			if (entity->variables.boundingBox.mins == Math::Vector3D::ZeroValue || entity->variables.boundingBox.maxs == Math::Vector3D::ZeroValue)
			{
				#if !defined _DEBUG
				#	pragma message ("REMOVE THIS SHIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
				#endif	// if !defined _DEBUG

//				InternalAssert (entity->variables.solid == SDK::Constants::SOLID_NOT);	// OCCURS!!! (de_aztec)

//				entity->Print ("C:/Users/Эдуард/Desktop/ClipMoveToEntities.log");
//				ExitProcess (1u);
				continue;	// deactivated
			}

			if (entity->variables.solid == SDK::Constants::SOLID_NOT)
				continue;	// deactivated

			if (!extent.IsTouching (entity->variables.absoluteBoundingBox))
				continue;	// not touching

			if (!traceFilter.ShouldHitEntity (entity))
			{
//				ServerPrintFormat ("ClipMoveToEntities(): Ignoring entity=%s.\n", entity->GetClassName ().GetData ());

				continue;
			}

//			ServerPrintFormat ("ClipMoveToEntities(): Hitting entity=%s.\n", entity->GetClassName ().GetData ());

			TraceEntity (source, destination, hullNumber, entity, tempTraceResult);

			if (tempTraceResult.isAllSolid || tempTraceResult.isStartSolid || tempTraceResult.fraction < traceResult.fraction)
			{
				// Make sure the ray is always shorter than it currently is
/*
				if (!traceFilter.ShouldHitEntity (entity))
					continue;
*/
				if (traceResult.isStartSolid)
				{
					traceResult = tempTraceResult;
					traceResult.isStartSolid = true;
				}
				else
					traceResult = tempTraceResult;

				// might intersect, so do an exact clip
				if (traceResult.isAllSolid)
					return;	// Stop if we're in allsolid

				// Update our extent....
				extent.MoveBounds (source, traceResult.endPosition);
			}
		}

		if (node->axis == -1)
			return;	// terminal node

		// recurse down both sides....

		if (extent.maxs[node->axis] > node->distance)
			ClipMoveToEntities (node->children[0u], extent, source, destination, hullNumber, traceFilter, traceResult);

		if (extent.mins[node->axis] < node->distance)
			ClipMoveToEntities (node->children[1u], extent, source, destination, hullNumber, traceFilter, traceResult);
	}

	#define DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS(FunctionName, returnType, argumentsDefinitions, argumentsCalls)	\
		inline returnType Engine_WithMetaMod::EngineAPI##FunctionName##FunctionCallbackFromPlugins argumentsDefinitions												\
		{																																							\
			return (*Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->m_EngineAPI##FunctionName##FunctionPointer) argumentsCalls;							\
		}
	#define DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS(FunctionName, returnType, argumentsDefinitions, argumentsCalls)	\
		inline returnType Engine_WithMetaMod::GameDLLAPI##FunctionName##FunctionCallbackFromPlugins argumentsDefinitions											\
		{																																							\
			return (*Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->m_GameDLLAPI##FunctionName##FunctionPointer) argumentsCalls;						\
		}

	// Engine API table....
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (SetModel,      void, (SDK::Classes::Edict *entity, const char *modelName), (entity, modelName));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (EmitSound,     void, (SDK::Classes::Edict *entity, SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, SDK::Constants::SoundFlag_t flags, SDK::Constants::SoundPitch_t pitch), (entity, channel, sample, volume, attenuation, flags, pitch));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientCommand, void, (SDK::Classes::Edict *client, char *format, ...), (client, format));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (LightStyle,    void, (const int style, char *const value), (style, value));

	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (MessageBegin, void, (const SDK::Constants::MessageDestination_t destination, const unsigned int type, const Math::Vector3D *const origin, SDK::Classes::Edict *const to), (destination, type, origin, to));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (MessageEnd,   void, (void), ());
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteByte,    void, (int value), (value));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteChar,    void, (int value), (value));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteShort,   void, (int value), (value));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteLong,    void, (int value), (value));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteAngle,   void, (float value), (value));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteCoord,   void, (float value), (value));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteString,  void, (const char *const string), (string));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (WriteEntity,  void, (int value), (value));

	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientPrintf, void, (SDK::Classes::Edict *client, SDK::Constants::PrintType_t printType, const char *message), (client, printType, message));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Cmd_Args,     const char *, (void), ());
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Cmd_Argv,     const char *, (unsigned int argc), (argc));
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Cmd_Argc,     const unsigned int, (void), ());
	DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (SetView,      void, (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity), (client, viewEntity));

	// Game DLL API table....
	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (Touch, void, (SDK::Classes::Edict *entityTouched, SDK::Classes::Edict *entityOther), (entityTouched, entityOther));
	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientConnect, BOOL, (SDK::Classes::Edict *const client, const char *const name, const char *const address, char rejectReason[128u]), (client, name, address, rejectReason));
	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientDisconnect, void, (SDK::Classes::Edict *client), (client));
///	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientCommand, void, (SDK::Classes::Edict *client), (client));	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ClientUserInfoChanged, void, (SDK::Classes::Edict *client, char *infoBuffer), (client, infoBuffer));
	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (ServerDeactivate, void, (void), ());
	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (StartFrame, void, (void), ());
	DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS (UpdateClientData, void, (const SDK::Classes::Edict *entity, int sendWeapons, SDK::Structures::ClientData_t *clientData), (entity, sendWeapons, clientData));

	#undef DECLARE_DYNAMIC_HOOKABLE_ENGINE_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS
	#undef DECLARE_DYNAMIC_HOOKABLE_GAME_DLL_API_FUNCTION_WITH_METAMOD_WITH_CALLBACKS_FROM_PLUGINS
}

#endif	// ifndef HALF_LIFE_ENGINE_CORE_INCLUDED