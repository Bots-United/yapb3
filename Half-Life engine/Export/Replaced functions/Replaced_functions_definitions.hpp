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
// Replaced_functions_definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef REPLACED_FUNCTIONS_DEFINITIONS_INCLUDED
#define REPLACED_FUNCTIONS_DEFINITIONS_INCLUDED

namespace DLLTable
{
	#include <Half-Life engine/Export/Replaced functions/DLL API/DLL_API.hpp>
}

namespace EngineTable
{
	#include <Half-Life engine/Export/Replaced functions/Engine API/Engine_API.hpp>
}

namespace GameRulesTable
{
	// Note: The below functions are declared in Core.hpp.
	inline void RestartRound (const SDK::Classes::HalfLifeMultiplay::FunctionRestartRound_t originalFunction, SDK::Classes::HalfLifeMultiplay *const gameRules);
}

namespace BotManagerTable
{
	// Note: The below functions are declared in Core.hpp.
	inline void OnEvent (const SDK::Classes::BotManager::FunctionOnEvent_t originalFunction, SDK::Classes::BotManager *const botManager, const SDK::Constants::GameEventType_t gameEvent, SDK::Classes::BaseEntity *const argument1, void *const argument2);
}

namespace Other
{
	// Note: The below functions are declared in Core.hpp.
	inline void               UTIL_LogPrintf_RoundStart (const char *const format/*, ...*/);
	inline const unsigned int SV_CalcPing_IfFakeClient  (const SDK::Structures::Client_t *const engineFakeClient);
}

namespace EntityTable
{
	// Note: The below functions are declared in Core.hpp.
	inline void       Spawn                   (const SDK::Classes::BasePlayer::FunctionSpawn_t            originalFunction, SDK::Classes::BasePlayer *const player);
	inline void       Player_RoundRespawn     (const SDK::Classes::BasePlayer::FunctionRoundRespawn_t     originalFunction, SDK::Classes::BasePlayer *const player);
	inline const BOOL TakeDamage              (const SDK::Classes::BasePlayer::FunctionTakeDamage_t       originalFunction, SDK::Classes::BasePlayer *const entity, SDK::Structures::EntityVariables_t *const inflictor, SDK::Structures::EntityVariables_t *const attacker, float damage, SDK::Constants::DamageType_t damageBits);
	inline void       Hostage_Spawn           (const SDK::Classes::Hostage::FunctionSpawn_t               originalFunction, SDK::Classes::Hostage *const hostage);
	inline const BOOL Hostage_TakeDamage      (const SDK::Classes::Hostage::FunctionTakeDamage_t          originalFunction, SDK::Classes::Hostage *const hostage, SDK::Structures::EntityVariables_t *const inflictor, SDK::Structures::EntityVariables_t *const attacker, float damage, SDK::Constants::DamageType_t damageBits);
	inline void       Hostage_Use             (const SDK::Classes::Hostage::FunctionUse_t                 originalFunction, SDK::Classes::Hostage *const hostage, SDK::Classes::BaseEntity *const activator, SDK::Classes::BaseEntity *const caller, SDK::Constants::UseType_t type, float value);
	inline void       Killed                  (const SDK::Classes::BasePlayer::FunctionKilled_t           originalFunction, SDK::Classes::BasePlayer *const player, SDK::Structures::EntityVariables_t *const killer, SDK::Constants::GibType_t shouldGib);
	inline const BOOL AddPlayerItem           (const SDK::Classes::BasePlayer::FunctionAddPlayerItem_t    originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::BasePlayerItem *const item);
	inline const BOOL RemovePlayerItem        (const SDK::Classes::BasePlayer::FunctionRemovePlayerItem_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::BasePlayerItem *const item);
	inline void       Player_ImpulseCommands  (const SDK::Classes::BasePlayer::FunctionImpulseCommands_t  originalFunction, SDK::Classes::BasePlayer *const player);
	inline void       Player_OnTouchingWeapon (const SDK::Classes::BasePlayer::FunctionOnTouchingWeapon_t originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::WeaponBox *const weaponBox);
	inline void       Player_Touch            (const SDK::Classes::BasePlayer::FunctionTouch_t            originalFunction, SDK::Classes::BasePlayer *const player, SDK::Classes::BaseEntity *const other);
	inline void       Touch                   (const SDK::Classes::BaseEntity::FunctionTouch_t            originalFunction, SDK::Classes::BaseEntity *const entity, SDK::Classes::BaseEntity *const other);
	inline void       Use                     (const SDK::Classes::BaseEntity::FunctionUse_t              originalFunction, SDK::Classes::BaseEntity *const entity, SDK::Classes::BaseEntity *const activator, SDK::Classes::BaseEntity *const caller, SDK::Constants::UseType_t type, float value);
	inline const BOOL Item_Deploy             (const SDK::Classes::BasePlayerItem::FunctionDeploy_t       originalFunction, SDK::Classes::BasePlayerItem *const entity);
	inline void       Weapon_Reload           (const SDK::Classes::BasePlayerWeapon::FunctionReload_t     originalFunction, SDK::Classes::BasePlayerWeapon *const entity);
	inline void       Weapon_Shotgun_Reload   (const SDK::Classes::BasePlayerWeapon::FunctionReload_t     originalFunction, SDK::Classes::BasePlayerWeapon *const shotgun);
}

#endif	// ifndef REPLACED_FUNCTIONS_DEFINITIONS_INCLUDED