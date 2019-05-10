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
// ThrownGrenadesManager.hpp
//
// Class: ThrownGrenadesManager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef THROWN_GRENADES_MANAGER_INCLUDED
#define THROWN_GRENADES_MANAGER_INCLUDED

class ThrownGrenadesManager
{
	public:
		struct ThrownGrenade_t
		{
			static const float s_explosionOverTime;	/// @note This variable is initialized in Globals_definitions.cpp.

			enum Type_t
			{
				Type_Explosive,
				Type_FlashBang,
				Type_Smoke
			};

			const HalfLifeEngine::SDK::Classes::Edict *const                grenade;
			const Type_t                                                    type;
			const HalfLifeEngine::Utilities::PseudoGrenadeEntityVariables_t pseudoEntityVariables;
			const Client *const                                             owner;	/// @note We should keep owner pointer out of grenade entity variables as grenade owner is nullied before grenade explosed! :(

			inline ThrownGrenade_t (const HalfLifeEngine::SDK::Classes::Edict *const inputGrenade, const Type_t inputType);	/// @note This function is declared in ClientManager.cpp.

			//
			// Group: Private operators.
			//
			private:
				inline ThrownGrenade_t &operator = (const ThrownGrenade_t &/*right*/);	// Avoid "warning C4512: 'ThrownGrenade_t' : assignment operator could not be generated".
		};

	private:
		typedef /*Queue*/DynamicArray <ThrownGrenade_t *, unsigned char> GrenadesArray;	/// @todo MAKE STATIC QUEUE, NOT DYNAMIC!!!!!

//		GrenadesArray m_grenades;

		GrenadesArray m_explosiveGrenades;
		GrenadesArray m_flashBangGrenades;
		GrenadesArray m_smokeGrenades;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ~ThrownGrenadesManager (void)
		{
//			m_grenades.DeleteAll/*DeleteAndPurgeAll*/ ();

			m_explosiveGrenades.DeleteAll/*DeleteAndPurgeAll*/ ();
			m_flashBangGrenades.DeleteAll/*DeleteAndPurgeAll*/ ();
			m_smokeGrenades.DeleteAll/*DeleteAndPurgeAll*/ ();
		}

	private:
		static inline void PutThrownGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade, const ThrownGrenade_t::Type_t type, GrenadesArray &grenades)
		{
			ThrownGrenade_t *const newThrownGrenade (new ThrownGrenade_t (grenade, type));

			#if defined _DEBUG
				// Reliability check.
				if (newThrownGrenade == NULL)
					TerminateOnMalloc ();
			#endif	// if defined _DEBUG

			grenades.PushBack (newThrownGrenade);
		}
		static inline const ThrownGrenade_t *const FindThrownGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade, GrenadesArray &grenades)
		{
			for (GrenadesArray::IndexType_t index (0u); index < grenades.GetElementNumber (); ++index)
			{
				if (grenades[index]->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if (grenades[index]->grenade == grenade)
						return grenades[index];
				}
				else
					grenades.Delete (index, 1u);
			}
/*
			for (ThrownGrenadeLink **tempNode = &grenades.GetHeadReference (); *tempNode != NULL; tempNode = reinterpret_cast <ThrownGrenadeLink **> (&(*tempNode)->GetNextReference ()))
			{
				if ((*tempNode)->GetData ()->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*tempNode)->GetData ()->grenade == grenade)
						return (*tempNode)->GetData ();
				}
				else
				{
					ThrownGrenadeLink *const temp (static_cast <ThrownGrenadeLink *> ((*tempNode)->GetNext ()));

					delete (*tempNode)->GetData ();
					delete *tempNode;

					*tempNode = temp;
				}
			}
/*
			ThrownGrenade_t **a = &grenades;

			while (*a != NULL)
			{
				if ((*a)->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*a)->grenade == grenade)
						return (*a);
				}
				else
				{
					ThrownGrenade_t *const temp ((*a)->next);

					delete *a;

					*a = temp;

					continue;
				}

				a = &(*a)->next;
			}
*/
			return NULL;
		}
		static inline const ThrownGrenade_t *const FindThrownGrenadeByOwner (const Client *const grenadeOwner, GrenadesArray &grenades)
		{
			for (GrenadesArray::IndexType_t index (0u); index < grenades.GetElementNumber (); ++index)
			{
				if (grenades[index]->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if (grenades[index]->owner == grenadeOwner)
						return grenades[index];
				}
				else
					grenades.Delete (index, 1u);
			}
/*
			for (ThrownGrenadeLink **tempNode = &grenades.GetHeadReference (); *tempNode != NULL; tempNode = reinterpret_cast <ThrownGrenadeLink **> (&(*tempNode)->GetNextReference ()))
			{
				if ((*tempNode)->GetData ()->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*tempNode)->GetData ()->owner == grenadeOwner)
						return (*tempNode)->GetData ();
				}
				else
				{
					ThrownGrenadeLink *const temp (static_cast <ThrownGrenadeLink *> ((*tempNode)->GetNext ()));

					delete (*tempNode)->GetData ();
					delete *tempNode;

					*tempNode = temp;
				}
			}
/*
			ThrownGrenade_t **a = &grenades;

			while (*a != NULL)
			{
				if ((*a)->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*a)->owner == grenadeOwner)
						return *a;
				}
				else
				{
					ThrownGrenade_t *const temp ((*a)->next);

					delete *a;

					*a = temp;

					continue;
				}

				a = &(*a)->next;
			}
*/
			return NULL;
		}
		static inline const ThrownGrenade_t *const FindLatestThrownGrenade (GrenadesArray &grenades)
		{
			for (GrenadesArray::IndexType_t index (0u); index < grenades.GetElementNumber (); ++index)
			{
				if (grenades[index]->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
					return grenades[index];

				grenades.Delete (index, 1u);
			}
/*
			for (ThrownGrenadeLink **tempNode = &grenades.GetHeadReference (); *tempNode != NULL; tempNode = reinterpret_cast <ThrownGrenadeLink **> (&(*tempNode)->GetNextReference ()))
			{
				if ((*tempNode)->GetData ()->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
					return (*tempNode)->GetData ();

				ThrownGrenadeLink *const temp (static_cast <ThrownGrenadeLink *> ((*tempNode)->GetNext ()));

				delete (*tempNode)->GetData ();
				delete *tempNode;

				*tempNode = temp;
			}
/*
			ThrownGrenade_t **a = &grenades;

			while (*a != NULL)
			{
				if ((*a)->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*a)->grenade->variables.owner == grenadeOwner)
						return (*a);
				}
				else
				{
					ThrownGrenade_t *const temp ((*a)->next);

					delete *a;

					*a = temp;

					continue;
				}

				a = &(*a)->next;
			}
*/
			return NULL;
		}

	public:
		inline void PutThrownExplosiveGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade) { PutThrownGrenade (grenade, ThrownGrenade_t::Type_Explosive, m_explosiveGrenades); }
		inline void PutThrownFlashBangGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade) { PutThrownGrenade (grenade, ThrownGrenade_t::Type_FlashBang, m_flashBangGrenades); }
		inline void PutThrownSmokeGrenade     (const HalfLifeEngine::SDK::Classes::Edict *const grenade) { PutThrownGrenade (grenade, ThrownGrenade_t::Type_Smoke, m_smokeGrenades); }
/*		inline void PutThrownGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade, const ThrownGrenade_t::Type_t type)
		{
			ThrownGrenade_t *const newThrownGrenade (new ThrownGrenade_t (grenade, type));

			#if defined _DEBUG
				// Reliability check.
				if (newThrownGrenade == NULL)
					TerminateOnMalloc ();
			#endif	// if defined _DEBUG

			m_grenades.PushBack (newThrownGrenade);
		}
*/
		inline void Clear (void)
		{/*
//			m_grenades.RemoveAll ();
			m_grenades.DeleteAndPurgeAll ();
*/
//			m_explosiveGrenades.RemoveAll ();
			m_explosiveGrenades.DeleteAndPurgeAll ();
//			m_flashBangGrenades.RemoveAll ();
			m_flashBangGrenades.DeleteAndPurgeAll ();
//			m_smokeGrenades.RemoveAll ();
			m_smokeGrenades.DeleteAndPurgeAll ();
		}

		inline const ThrownGrenade_t *const FindThrownExplosiveGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade) { return FindThrownGrenade (grenade, m_explosiveGrenades); }
		inline const ThrownGrenade_t *const FindThrownFlashBangGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade) { return FindThrownGrenade (grenade, m_flashBangGrenades); }
		inline const ThrownGrenade_t *const FindThrownSmokeGrenade     (const HalfLifeEngine::SDK::Classes::Edict *const grenade) { return FindThrownGrenade (grenade, m_smokeGrenades); }
#if 0
		inline const ThrownGrenade_t *const FindThrownGrenade (const HalfLifeEngine::SDK::Classes::Edict *const grenade)
		{
			for (GrenadesArray::IndexType_t index (0u); index < m_grenades.GetElementNumber (); ++index)
			{
				if (m_grenades[index]->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if (m_grenades[index]->grenade == grenade)
						return m_grenades[index];
				}
				else
					m_grenades.Delete (index, 1u);
			}
/*
			for (ThrownGrenadeLink **tempNode = &m_grenades.GetHeadReference (); *tempNode != NULL; tempNode = reinterpret_cast <ThrownGrenadeLink **> (&(*tempNode)->GetNextReference ()))
			{
				if ((*tempNode)->GetData ()->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*tempNode)->GetData ()->grenade == grenade)
						return (*tempNode)->GetData ();
				}
				else
				{
					ThrownGrenadeLink *const temp (static_cast <ThrownGrenadeLink *> ((*tempNode)->GetNext ()));

					delete (*tempNode)->GetData ();
					delete *tempNode;

					*tempNode = temp;
				}
			}
/*
			ThrownGrenade_t **a = &m_grenades;

			while (*a != NULL)
			{
				if ((*a)->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*a)->grenade == grenade)
						return (*a);
				}
				else
				{
					ThrownGrenade_t *const temp ((*a)->next);

					delete *a;

					*a = temp;

					continue;
				}

				a = &(*a)->next;
			}
*/
			return NULL;
		}
#endif	// if 0
		inline const ThrownGrenade_t *const FindThrownExplosiveGrenadeByOwner (const Client *const grenadeOwner) { return FindThrownGrenadeByOwner (grenadeOwner, m_explosiveGrenades); }
		inline const ThrownGrenade_t *const FindThrownFlashBangGrenadeByOwner (const Client *const grenadeOwner) { return FindThrownGrenadeByOwner (grenadeOwner, m_flashBangGrenades); }
		inline const ThrownGrenade_t *const FindThrownSmokeGrenadeByOwner     (const Client *const grenadeOwner) { return FindThrownGrenadeByOwner (grenadeOwner, m_smokeGrenades); }
#if 0
		inline const ThrownGrenade_t *const FindThrownGrenadeByOwner (const Client *const grenadeOwner)
		{
			for (GrenadesArray::IndexType_t index (0u); index < m_grenades.GetElementNumber (); ++index)
			{
				if (m_grenades[index]->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if (m_grenades[index]->owner == grenadeOwner)
						return m_grenades[index];
				}
				else
					m_grenades.Delete (index, 1u);
			}
/*
			for (ThrownGrenadeLink **tempNode = &m_grenades.GetHeadReference (); *tempNode != NULL; tempNode = reinterpret_cast <ThrownGrenadeLink **> (&(*tempNode)->GetNextReference ()))
			{
				if ((*tempNode)->GetData ()->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*tempNode)->GetData ()->owner == grenadeOwner)
						return (*tempNode)->GetData ();
				}
				else
				{
					ThrownGrenadeLink *const temp (static_cast <ThrownGrenadeLink *> ((*tempNode)->GetNext ()));

					delete (*tempNode)->GetData ();
					delete *tempNode;

					*tempNode = temp;
				}
			}
/*
			ThrownGrenade_t **a = &m_grenades;

			while (*a != NULL)
			{
				if ((*a)->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*a)->owner == grenadeOwner)
						return *a;
				}
				else
				{
					ThrownGrenade_t *const temp ((*a)->next);

					delete *a;

					*a = temp;

					continue;
				}

				a = &(*a)->next;
			}
*/
			return NULL;
		}
#endif	// if 0
		inline const ThrownGrenade_t *const FindLatestThrownExplosiveGrenade (void) { return FindLatestThrownGrenade (m_explosiveGrenades); }
		inline const ThrownGrenade_t *const FindLatestThrownFlashBangGrenade (void) { return FindLatestThrownGrenade (m_flashBangGrenades); }
		inline const ThrownGrenade_t *const FindLatestThrownSmokeGrenade     (void) { return FindLatestThrownGrenade (m_smokeGrenades); }
#if 0
		inline const ThrownGrenade_t *const FindLatestThrownGrenadeByType (const ThrownGrenade_t::Type_t type)
		{
			for (GrenadesArray::IndexType_t index (0u); index < m_grenades.GetElementNumber (); ++index)
			{
				if (m_grenades[index]->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if (m_grenades[index]->type == type)
						return m_grenades[index];
				}
				else
					m_grenades.Delete (index, 1u);
			}
/*
			for (ThrownGrenadeLink **tempNode = &m_grenades.GetHeadReference (); *tempNode != NULL; tempNode = reinterpret_cast <ThrownGrenadeLink **> (&(*tempNode)->GetNextReference ()))
			{
				if ((*tempNode)->GetData ()->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*tempNode)->GetData ()->type == type)
						return (*tempNode)->GetData ();
				}
				else
				{
					ThrownGrenadeLink *const temp (static_cast <ThrownGrenadeLink *> ((*tempNode)->GetNext ()));

					delete (*tempNode)->GetData ();
					delete *tempNode;

					*tempNode = temp;
				}
			}
/*
			ThrownGrenade_t **a = &m_grenades;

			while (*a != NULL)
			{
				if ((*a)->grenade->variables.damageTime + ThrownGrenade_t::s_explosionOverTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				{
					if ((*a)->grenade->variables.owner == grenadeOwner)
						return (*a);
				}
				else
				{
					ThrownGrenade_t *const temp ((*a)->next);

					delete *a;

					*a = temp;

					continue;
				}

				a = &(*a)->next;
			}
*/
			return NULL;
		}
#endif	// if 0
};

#endif	// ifndef THROWN_GRENADES_MANAGER_INCLUDED