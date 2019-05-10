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
// constants_definitions.hpp
//
// Description: Engine constants.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CONSTANTS_DEFINITIONS_INCLUDED
#define CONSTANTS_DEFINITIONS_INCLUDED

//
// Constants shared by the engine and DLLs
// This header file included by engine files and DLL files.
#include <Half-Life engine/SDK/Constants/alert_message_type.hpp>
#include <Half-Life engine/SDK/Constants/AmbientLevel.hpp>
#include <Half-Life engine/SDK/Constants/button_type.hpp>
#include <Half-Life engine/SDK/Constants/common.hpp>
#include <Half-Life engine/SDK/Constants/Console_variable_flag.hpp>
#include <Half-Life engine/SDK/Constants/DamageType.hpp>
#include <Half-Life engine/SDK/Constants/EdictAreaNode.hpp>
#include <Half-Life engine/SDK/Constants/entity_dead_state.hpp>
#include <Half-Life engine/SDK/Constants/entity_effect_flag.hpp>
#include <Half-Life engine/SDK/Constants/entity_flag.hpp>
#include <Half-Life engine/SDK/Constants/entity_move_type.hpp>
#include <Half-Life engine/SDK/Constants/entity_private_data_offset.hpp>
#include <Half-Life engine/SDK/Constants/entity_private_data_offset_flag.hpp>
#include <Half-Life engine/SDK/Constants/entity_solid_type.hpp>
#include <Half-Life engine/SDK/Constants/Entity_spawn_flag.hpp>
#include <Half-Life engine/SDK/Constants/entity_take_damage_type.hpp>
#include <Half-Life engine/SDK/Constants/EntityCapBit.hpp>
#include <Half-Life engine/SDK/Constants/EntityClassType.hpp>
#include <Half-Life engine/SDK/Constants/EntityVirtualTableFunctionID.hpp>
#include <Half-Life engine/SDK/Constants/EntityWaterLevel.hpp>
#include <Half-Life engine/SDK/Constants/FixAngleType.hpp>
#include <Half-Life engine/SDK/Constants/force_type.hpp>
#include <Half-Life engine/SDK/Constants/FunctionBreakableExplosionType.hpp>
#include <Half-Life engine/SDK/Constants/FunctionBreakableMaterialType.hpp>
#include <Half-Life engine/SDK/Constants/KeyboardKeys.hpp>

namespace GameRules
{
	#include <Half-Life engine/SDK/Constants/GameRules/Definitions.hpp>
}

namespace BotManager
{
	#include <Half-Life engine/SDK/Constants/BotManager.hpp>
}

#include <Half-Life engine/SDK/Constants/GameEventType.hpp>
#include <Half-Life engine/SDK/Constants/GibType.hpp>
#include <Half-Life engine/SDK/Constants/GrenadeType.hpp>
#include <Half-Life engine/SDK/Constants/HUD_print_type.hpp>
#include <Half-Life engine/SDK/Constants/hull.hpp>
#include <Half-Life engine/SDK/Constants/IconStatus.hpp>

namespace Menu
{
	#include <Half-Life engine/SDK/Constants/Menu/Definitions.hpp>
}

#include <Half-Life engine/SDK/Constants/MapZone.hpp>
#include <Half-Life engine/SDK/Constants/message_destination.hpp>
#include <Half-Life engine/SDK/Constants/message_type.hpp>
#include <Half-Life engine/SDK/Constants/ModelType.hpp>
#include <Half-Life engine/SDK/Constants/MonsterActivity.hpp>
#include <Half-Life engine/SDK/Constants/PlayerArmorType.hpp>
#include <Half-Life engine/SDK/Constants/PlayerBodyPart.hpp>
#include <Half-Life engine/SDK/Constants/PlayerBodyPartHitMultiplications.hpp>
#include <Half-Life engine/SDK/Constants/PlayerBoneID.hpp>
#include <Half-Life engine/SDK/Constants/PlayerImpulseID.hpp>
#include <Half-Life engine/SDK/Constants/PlayerModelID.hpp>
#include <Half-Life engine/SDK/Constants/PlayerObserverMode.hpp>
#include <Half-Life engine/SDK/Constants/PlayerPhysicsFlag.hpp>
#include <Half-Life engine/SDK/Constants/PMPlayerTraceFlag.hpp>
#include <Half-Life engine/SDK/Constants/PMTraceLineFlag.hpp>
#include <Half-Life engine/SDK/Constants/point_content_type.hpp>
#include <Half-Life engine/SDK/Constants/print_type.hpp>
#include <Half-Life engine/SDK/Constants/RadioCommand.hpp>
#include <Half-Life engine/SDK/Constants/RadioMenu.hpp>
#include <Half-Life engine/SDK/Constants/render_fx_type.hpp>
#include <Half-Life engine/SDK/Constants/render_mode.hpp>
#include <Half-Life engine/SDK/Constants/ScreenFadeFlag.hpp>
#include <Half-Life engine/SDK/Constants/Server.hpp>
#include <Half-Life engine/SDK/Constants/sound_attenuation_value.hpp>
#include <Half-Life engine/SDK/Constants/sound_channel.hpp>
#include <Half-Life engine/SDK/Constants/sound_pitch_value.hpp>
#include <Half-Life engine/SDK/Constants/sound_volume_value.hpp>
#include <Half-Life engine/SDK/Constants/SoundFlag.hpp>
#include <Half-Life engine/SDK/Constants/SpectatorObserverMode.hpp>
#include <Half-Life engine/SDK/Constants/Studio.hpp>
#include <Half-Life engine/SDK/Constants/TeamID.hpp>
#include <Half-Life engine/SDK/Constants/TempEntityEvent.hpp>
#include <Half-Life engine/SDK/Constants/TempEntity_Director_SubCommand.hpp>
#include <Half-Life engine/SDK/Constants/TempEntity_Explosion_Flag.hpp>
#include <Half-Life engine/SDK/Constants/TempEntity_FireField_Flag.hpp>
#include <Half-Life engine/SDK/Constants/TextureType.hpp>
#include <Half-Life engine/SDK/Constants/TraceIgnore.hpp>
#include <Half-Life engine/SDK/Constants/TrainFlag.hpp>
#include <Half-Life engine/SDK/Constants/UseType.hpp>
#include <Half-Life engine/SDK/Constants/vector_definitions.hpp>
#include <Half-Life engine/SDK/Constants/VGUIMenuID.hpp>
#include <Half-Life engine/SDK/Constants/WeaponID.hpp>
#include <Half-Life engine/SDK/Constants/WeaponBit.hpp>
#include <Half-Life engine/SDK/Constants/WeaponBits.hpp>
#include <Half-Life engine/SDK/Constants/WeaponBulletType.hpp>
#include <Half-Life engine/SDK/Constants/WeaponObstaclePenetrationPower.hpp>
#include <Half-Life engine/SDK/Constants/WeaponSlotID.hpp>
#include <Half-Life engine/SDK/Constants/ArmouryItemType.hpp>
#include <Half-Life engine/SDK/Constants/WeaponProperties.hpp>
#include <Half-Life engine/SDK/Constants/ZoomType.hpp>

namespace Steam
{
	#include <Half-Life engine/SDK/Constants/Steam/Definitions.hpp>
}

#endif	// ifndef CONSTANTS_DEFINITIONS_INCLUDED