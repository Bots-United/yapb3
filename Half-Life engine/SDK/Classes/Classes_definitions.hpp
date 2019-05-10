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
// Classes_definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CLASSES_DEFINITIONS_INCLUDED
#define CLASSES_DEFINITIONS_INCLUDED

class VoiceGameManager;

class SquadMonster;

class ConsoleVariable;
class Edict;
class BaseEntity;
class BasePlayer;
class NonCZBasePlayer;
class Bot;
class NonCZBot;
class CSBot;
class NonCZCSBot;
class CGrenade;
class Hostage;
class BasePlayerItem;
class BasePlayerWeapon;

template <typename elementType> class CUtlMemory	/// @see 'CUtlMemory' class from Source engine.... (utlmemory.h)
{
	/// @todo Copy full prototype from Source engine and move into appropriate header file....

	public:
		enum { EXTERNAL_BUFFER_MARKER = -1 };

	public:
/*!off=0(0)*/		elementType *m_pMemory;
/*!off=4(1)*/		int          m_nAllocationCount;
/*!off=8(2)*/		int          m_nGrowSize;
};	// sizeof (CUtlMemory) == 12 (3)
template <typename elementType> class CUtlVector	/// @see 'CUtlVector' class from Source engine.... (utlvector.h)
{
	/// @todo Copy full prototype from Source engine and move into appropriate header file....

	public:
		typedef elementType ElementType_t;

	public:
/*!off=0(0)*/		CUtlMemory <elementType> m_Memory;
/*!off=12(3)*/		int m_Size;

		// For easier access to the elements through the debugger
		// it's in release builds so this can be used in libraries correctly
/*!off=16(4)*/		elementType *m_pElements;
};	// sizeof (CUtlMemory) == 20 (5)

#include <Half-Life engine/SDK/Classes/NavigationArea.hpp>

#include <Half-Life engine/SDK/Classes/BaseEntity.hpp>

#include <Half-Life engine/SDK/Classes/EntityHandle.hpp>

class BasePlayerAmmo : public BaseEntity
{
};

class WeaponBox : public BaseEntity
{
};

	#include <Half-Life engine/SDK/Classes/Item.hpp>
	#include <Half-Life engine/SDK/Classes/PointEntity.hpp>
	#include <Half-Life engine/SDK/Classes/BaseDelay.hpp>
		#include <Half-Life engine/SDK/Classes/Breakable.hpp>
		#include <Half-Life engine/SDK/Classes/BaseAnimating.hpp>
			#include <Half-Life engine/SDK/Classes/BasePlayerItem.hpp>
				#include <Half-Life engine/SDK/Classes/BasePlayerWeapon.hpp>
			#include <Half-Life engine/SDK/Classes/BaseToggle.hpp>
				#include <Half-Life engine/SDK/Classes/BaseDoor.hpp>
				#include <Half-Life engine/SDK/Classes/BaseMonster.hpp>
					#include <Half-Life engine/SDK/Classes/Grenade.hpp>
					#include <Half-Life engine/SDK/Classes/Hostage.hpp>
					#include <Half-Life engine/SDK/Classes/BasePlayer.hpp>

#include <Half-Life engine/SDK/Classes/GameRules.hpp>

#include <Half-Life engine/SDK/Classes/BotManager.hpp>

					#include <Half-Life engine/SDK/Classes/Bot.hpp>

namespace Interface
{
	#include <Half-Life engine/SDK/Classes/Interfaces/Interface definitions.hpp>
}

// Reliability checks....
CompileTimeAssert (sizeof (NonCZBasePlayer) == 2480u);
CompileTimeAssert (sizeof (NonCZCSBot) == 11404u);
CompileTimeAssert (sizeof (HalfLifeMultiplay) == 720u);
CompileTimeAssert (sizeof (HalfLifeTraining) == 744u);
CompileTimeAssert (sizeof (CSBotManager) == 740u);
CompileTimeAssert (sizeof (Steam::ID) == 8u);
CompileTimeAssert (sizeof (Interface::ICareerUI) == 280u);

#endif	// ifndef CLASSES_DEFINITIONS_INCLUDED