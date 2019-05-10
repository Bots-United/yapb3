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
// FakeClientIlluminationEntity.hpp
//
// Class: IlluminationEntity
//
// Description: Dummy entity for getting the correct bot illumination (pfnGetEntityIllum() engine bug fix).
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef FAKE_CLIENT_ILLUMINATION_ENTITY_INCLUDED
#define FAKE_CLIENT_ILLUMINATION_ENTITY_INCLUDED

class IlluminationEntity : public HalfLifeEngine::SDK::Classes::PointEntity
{
	private:
		inline void UpdatePosition (void)
		{
			m_variables->containingEntity->SetOrigin (m_variables->owner->variables.origin);	// Make his light entity follow him....

			m_variables->nextThinkTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.5f;	// make it think at 5 Hertz

			// Ask the engine for the illumination entity illumination and filter it so as to return a usable value.
			m_variables->owner->variables.lightLevel = m_variables->containingEntity->GetIllumination ();
/*
			#if defined _DEBUG
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "IlluminationEntity::UpdatePosition(): Bot \"%s\" GetIllumination()=%u.\n", m_variables->owner->GetNetName ().GetData (), m_variables->owner->variables.lightLevel);
				HalfLifeEngine::Globals::g_halfLifeEngine->DrawCross3D (m_variables->origin, 15.0f, HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), Color <> (0u, 255u, 0u), 1u);
			#endif	// if defined _DEBUG*/
		}
		inline void Think (void)
		{
			UpdatePosition ();
		}

	public:
		static inline IlluminationEntity *const Create (HalfLifeEngine::SDK::Classes::Edict *const fakeClient)
		{
			HalfLifeEngine::SDK::Classes::Edict *const illuminationEdict (HalfLifeEngine::Globals::g_halfLifeEngine->CreateNamedEntity ("info_target"));

			// check is illumination entity created (reliability check)
			InternalAssert (illuminationEdict->IsValid ());

			IlluminationEntity *const illuminationEntity (static_cast <IlluminationEntity *> (illuminationEdict->privateData));
//			IlluminationEntity *const illuminationEntity (HalfLifeEngine::Globals::g_halfLifeEngine->AllocateEntity <IlluminationEntity> ());	/// THIS DON'T WORK, SINCE IlluminationEntity HASN'T REAL VIRTUAL TABLES!!!!

			illuminationEntity->Spawn (fakeClient);

			return illuminationEntity;
		}

		inline void Spawn (HalfLifeEngine::SDK::Classes::Edict *const fakeClient)
		{
			// set some entity variable values...,
			m_variables->className = HalfLifeEngine::Globals::g_halfLifeEngine->GetOffsetFromString ("fake_client_illumination_entity");	// set it's a class name (sets a name for it)

			m_variables->moveType  = HalfLifeEngine::SDK::Constants::MOVETYPE_NOCLIP/*MOVETYPE_FOLLOW*/;	// set it's movement to no clipping
//			m_variables->aimEntity = fakeClient;	// set it's aim entity
			m_variables->owner     = fakeClient;	// set it's owner

			// needed to make it think....
			m_pfnThink = static_cast <HalfLifeEngine::SDK::Classes::BaseEntity::FunctionThink_t> (&IlluminationEntity::Think);

			// set it's a model.
			m_variables->containingEntity->SetModel ("models/mechgibs.mdl");

			#if defined _DEBUG
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("IlluminationEntity::Spawn() called for \"%s\".\n", fakeClient->GetNetName ().GetData ());
			#endif	// if defined _DEBUG
		}

		inline void OnOwnerSpawned (void)
		{
			m_variables->moveType = HalfLifeEngine::SDK::Constants::MOVETYPE_NOCLIP;	// set it's movement to no clipping
			m_variables->effects  = HalfLifeEngine::SDK::Constants::EntityEffectFlag_None;
			m_variables->flags    = HalfLifeEngine::SDK::Constants::EntityFlag_None;

			UpdatePosition ();	// needed to make it think
		}
		inline void OnOwnerKilled (void)
		{
			m_variables->containingEntity->SetOrigin (Math::Vector3D::ZeroValue);	// move his light entity out of the map....

			m_variables->moveType      = HalfLifeEngine::SDK::Constants::MOVETYPE_NONE;	// Don't move
			m_variables->effects       = HalfLifeEngine::SDK::Constants::EntityEffect_NoDraw;	// don't draw
			m_variables->nextThinkTime = -1.0f;	// Don't think.
			m_variables->flags         = HalfLifeEngine::SDK::Constants::FL_DORMANT;
		}
		inline void Kill (void)
		{
			m_variables->flags = HalfLifeEngine::SDK::Constants::FL_KILLME;
		}
};

#endif	// ifndef FAKE_CLIENT_ILLUMINATION_ENTITY_INCLUDED