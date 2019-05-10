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
// C4Manager.hpp
//
// Class: C4Manager
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef C4_MANAGER_INCLUDED
#define C4_MANAGER_INCLUDED

class C4Manager
{
	//
	// Group: Private members.
	//
	private:
		C4 *m_c4;	/// @todo MAKE 'C4Manager' WORK WITH MULTIPLE NUMBER OF BOMBS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//
	// Group: (Con/De)structors.
	//
	public:
		inline C4Manager (void) : m_c4 (NULL) { /* VOID */ }

		//
		// Function: ~C4Manager
		//
		// Description: C4 manager destructor.
		//
		inline ~C4Manager (void) { delete m_c4; }

	//
	// Group: Private operators.
	//
	private:
		inline C4Manager &operator = (const C4Manager &/*right*/);	// Avoid "warning C4512: 'C4Manager' : assignment operator could not be generated".

	//
	// Group: Game message handler callbacks.
	//
	public:
		inline void BombPlanted (void)
		{
			// Reliability check.
			InternalAssert (!IsÑ4Planted ());

			// Find the C4 entity....
			for (HalfLifeEngine::SDK::Classes::Edict *c4 (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "grenade")); c4->IsValid (); c4 = HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (c4, "grenade"))
			{
				if (!c4->IsGrenadeIsC4 ())
					continue;

				m_c4 = new C4 (c4);	// Assign c4 entity pointer.

				#if defined _DEBUG
					// Reliability check.
					if (m_c4 == NULL)
						AddLogEntry (true, LogLevel_Error, false, "C4Manager::BombPlanted(): Couldn't allocate %u bytes for creating the c4 entity wrapper!", sizeof (C4));

					// Done.
					return;
				#endif	// if defined _DEBUG
			}

			// Error.
			#if defined _DEBUG
				AddLogEntry (false, LogLevel_Critical, false, "C4Manager::BombPlanted(): C4 entity is not found!");
			#endif	// if defined _DEBUG
		}

		inline void RoundEnded (void)
		{
			delete m_c4;	// Free memory.

			m_c4 = NULL;	// Reset the c4 entity pointer.
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool      IsÑ4Planted (void) const { return m_c4->IsPlanted (); }
		inline const C4 *const GetC4       (void) const { return m_c4; }
};

#endif	// ifndef C4_MANAGER_INCLUDED