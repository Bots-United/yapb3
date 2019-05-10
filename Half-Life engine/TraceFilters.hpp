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
// TraceFilters.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_TRACE_FILTERS_INCLUDED
#define HALF_LIFE_ENGINE_TRACE_FILTERS_INCLUDED

abstract_class Interface_TraceFilter
{
	//
	// Group: Functions.
	//
	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict) = 0;

	//
	// Group: Private operators.
	//
	private:
		inline Interface_TraceFilter &operator = (const Interface_TraceFilter &/*right*/);	// Avoid "warning C4512: 'Interface_TraceFilter' : assignment operator could not be generated".
};

//-----------------------------------------------------------------------------
// Classes are expected to inherit these + implement the ShouldHitEntity method
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// trace filter methods....
//-----------------------------------------------------------------------------
class TraceFilter_Simple : public Interface_TraceFilter
{
	private:
		const SDK::Classes::Edict *m_passEdict;
		int                        m_collisionGroup;

	public:
		inline TraceFilter_Simple (const SDK::Classes::Edict *const passEdict, const int collisionGroup) :
			m_passEdict (passEdict),
			m_collisionGroup (collisionGroup)
		{ /* VOID */ }

	private:
		//-----------------------------------------------------------------------------
		// A standard filter to be applied to just about everything.
		//-----------------------------------------------------------------------------
		static inline const bool StandardFilterRules (SDK::Classes::BaseEntity *const pCollide/*, int fContentsMask*/)
		{
			// Static prop case...
			if (pCollide == NULL)
				return true;
/*
			SolidType_t solid = pCollide->GetSolid ();
			const Structures::Model_t *pModel = pCollide->GetModel ();

			// This code is used to cull out tests against moving entities
			if (modelinfo->GetModelType (pModel) != mod_brush || (solid != SOLID_BSP && solid != SOLID_VPHYSICS))
			{
				if (!(fContentsMask & CONTENTS_MONSTER))
					return false;
			}

			// This code is used to cull out tests against see-thru entities
			if (!(fContentsMask & CONTENTS_WINDOW) && pCollide->IsTransparent ())
				return false;

			// FIXME: this is to skip BSP models that are entities that can be
			// potentially moved/deleted, similar to a monster but doors don't seem to
			// be flagged as monsters
			// FIXME: the FL_WORLDBRUSH looked promising, but it needs to be set on
			// everything that's actually a worldbrush and it currently isn't
			if (!(fContentsMask & CONTENTS_MOVEABLE) && pCollide->GetMoveType () == MOVETYPE_PUSH)// !(touch->flags & FL_WORLDBRUSH))
				return false;
*/
			return true;
		}

		//-----------------------------------------------------------------------------
		//
		// Shared client/server trace filter code
		//
		//-----------------------------------------------------------------------------
		static inline const bool PassServerEntityFilter (const SDK::Classes::Edict *touch, const SDK::Classes::Edict *const pass);

	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict);
		virtual void SetPassEntity (const SDK::Classes::Edict *const pPassEntity) { m_passEdict = pPassEntity; }
		virtual void SetCollisionGroup (const int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

		inline const SDK::Classes::Edict *const GetPassEntity (void) { return m_passEdict; }
};

//-----------------------------------------------------------------------------
// Classes need not inherit from these
//-----------------------------------------------------------------------------
class TraceFilter_HitWorldOnly : public Interface_TraceFilter
{
	public:
		inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict);
};

class TraceFilter_HitAll : public Interface_TraceFilter
{
	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const/* edict*/) { return true; }
};

class TraceFilter_PassEntity : public Interface_TraceFilter
{
	private:
		const SDK::Classes::Edict *m_passEdict;

	public:
		inline TraceFilter_PassEntity (const SDK::Classes::Edict *const passEdict) :
			m_passEdict (passEdict)
		{ /* VOID */ }

	public:
		inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict)
		{
			return m_passEdict != edict;
		}
};

//-----------------------------------------------------------------------------
// Trace filter that can take a list of entities to ignore
//-----------------------------------------------------------------------------
class TraceFilter_SimpleList : public TraceFilter_Simple
{
	protected:
		DynamicArray <SDK::Classes::Edict *, unsigned char> m_passEntities;

	public:
		inline TraceFilter_SimpleList (const int collisionGroup) : TraceFilter_Simple (NULL, collisionGroup) { /* VOID */ }

	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict)
		{
			if (m_passEntities.IsMember (edict))
				return false;

			return TraceFilter_Simple::ShouldHitEntity (edict);
		}

		//-----------------------------------------------------------------------------
		// Purpose: Add an entity to my list of entities to ignore in the trace
		//-----------------------------------------------------------------------------
		inline void AddEntityToIgnore (SDK::Classes::Edict *const edict)
		{
			m_passEntities += edict;
		}
};

class TraceFilter_OnlyNPCsAndPlayers : public TraceFilter_Simple
{
	public:
		inline TraceFilter_OnlyNPCsAndPlayers (const SDK::Classes::Edict *const passEdict, const int collisionGroup) :
			TraceFilter_Simple (passEdict, collisionGroup)
		{ /* VOID */ }

	public:
		//-----------------------------------------------------------------------------
		// Purpose: Trace filter that only hits NPCs and the player
		//-----------------------------------------------------------------------------
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict);
};

//-----------------------------------------------------------------------------
// Trace filter that can take a classname to ignore
//-----------------------------------------------------------------------------
class TraceFilter_SkipClassName : public TraceFilter_Simple
{
	private:
		const char *const m_className;

	public:
		inline TraceFilter_SkipClassName (const SDK::Classes::Edict *const passEdict, const int collisionGroup, const char *const className) :
			TraceFilter_Simple (passEdict, collisionGroup), m_className (className)
		{ /* VOID */ }

	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict);
};

//-----------------------------------------------------------------------------
// Trace filter that can take a list of entities to ignore
//-----------------------------------------------------------------------------
class TraceFilter_SimpleClassNameList : public TraceFilter_Simple
{
	private:
		DynamicArray <const char *, unsigned char> m_passClassNames;

	public:
		inline TraceFilter_SimpleClassNameList (const SDK::Classes::Edict *const passEdict, const int collisionGroup) :
			TraceFilter_Simple (passEdict, collisionGroup)
		{ /* VOID */ }

	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict);

		//-----------------------------------------------------------------------------
		// Purpose: Add an entity to my list of entities to ignore in the trace
		//-----------------------------------------------------------------------------
		inline void AddClassnameToIgnore (const char *const className)
		{
			m_passClassNames += className;
		}
};

class TraceFilter_Chain : public Interface_TraceFilter
{
	private:
		Interface_TraceFilter &m_traceFilter1;
		Interface_TraceFilter &m_traceFilter2;

	public:
		inline TraceFilter_Chain (Interface_TraceFilter &traceFilter1, Interface_TraceFilter &traceFilter2) :
			m_traceFilter1 (traceFilter1),
			m_traceFilter2 (traceFilter2)
		{ /* VOID */ }

	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict)
		{
			return m_traceFilter1.ShouldHitEntity (edict) && m_traceFilter2.ShouldHitEntity (edict);
		}
};

//-----------------------------------------------------------------------------
// Purpose: Trace filter that only hits anything but NPCs and the players
//-----------------------------------------------------------------------------
class TraceFilter_NoNPCsOrPlayers : public TraceFilter_Simple
{
	public:
		inline TraceFilter_NoNPCsOrPlayers (const SDK::Classes::Edict *const passEdict, const int collisionGroup) :
			TraceFilter_Simple (passEdict, collisionGroup)
		{ /* VOID */ }

	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict);
};

//--------------------------------------------------------------------------------------------------------------
/**
*  Trace filter that ignores players, NPCs, and objects that can be walked through
*/
class TraceFilter_WalkableEntities : public TraceFilter_NoNPCsOrPlayers
{
	public:
		DECLARE_TYPED_ENUMERATION (unsigned char, IgnoreEntityFlag_t)
		{
			IgnoreEntityFlag_None,

			WALK_THRU_DOORS          = BIT (0u),
			WALK_THRU_BREAKABLES     = BIT (1u),
			WALK_THRU_PUSHABLES      = BIT (2u),
//			WALK_THRU_TOGGLE_BRUSHES = BIT (3u),	// MADE BY EDWARD!!! - START
			WALK_THRU_GLASS          = BIT (4u),	// MADE BY EDWARD!!! - END

			WALK_THRU_EVERYTHING     = WALK_THRU_DOORS | WALK_THRU_BREAKABLES | WALK_THRU_PUSHABLES/* | WALK_THRU_TOGGLE_BRUSHES*/ | WALK_THRU_GLASS	// MADE BY EDWARD!!! - START
		};

	private:
		const IgnoreEntityFlag_t m_flags;

	public:
		inline TraceFilter_WalkableEntities (const SDK::Classes::Edict *const passEdict, const int collisionGroup, const IgnoreEntityFlag_t flags) :
			TraceFilter_NoNPCsOrPlayers (passEdict, collisionGroup), m_flags (flags)
		{ /* VOID */ }

	private:
		static inline const bool IsEntityWalkable (const HalfLifeEngine::SDK::Classes::Edict *const entity, const IgnoreEntityFlag_t flags);

	public:
		virtual inline const bool ShouldHitEntity (SDK::Classes::Edict *const edict)
		{
			if (TraceFilter_NoNPCsOrPlayers::ShouldHitEntity (edict))
				return !IsEntityWalkable (edict, m_flags);

			return false;
		}
};

#endif	// ifndef HALF_LIFE_ENGINE_TRACE_FILTERS_INCLUDED