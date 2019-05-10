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
// BotVariableID.hpp
//
// Description: Bot/server console variables indexes used for 'BotConsoleVariableManager' class.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_CONSOLE_VARIABLE_ID_INCLUDED
#define BOT_CONSOLE_VARIABLE_ID_INCLUDED

enum VariableID_t
{
	// Bot console variables....
	VariableID_AutoVacate,
	VariableID_Quota,
	VariableID_QuotaMode,
	VariableID_MinSkill,
	VariableID_MaxSkill,
	VariableID_FollowUser,
	VariableID_TimerSound,
	VariableID_TimerPickup,
	VariableID_TimerGrenade,
	VariableID_DebugGoal,
	VariableID_Chat,
	VariableID_Synth,
	VariableID_KnifeMode,
	VariableID_SkillTags,
	VariableID_Stop,
	VariableID_ThruWalls,
	VariableID_WallSensitivity,
	VariableID_Votes,
	VariableID_Spray,
	VariableID_BotBuy,

	#if defined _DEBUG
		VariableID_Debug,
	#endif	// if defined _DEBUG

	VariableID_Prefix,
	VariableID_Language,
	VariableID_EnableClientLanguages,	// Enables or disables whether YaPB reads a client's "lang" setinfo to set their language for server-side phrase translation. 0 - Translate using default server's language; 1 - Translate using the client's language (default).
	VariableID_Version,
	VariableID_NavPointLookMethod,
	VariableID_AimMethod,
	VariableID_TargetingMethod,
	VariableID_DamperCoeff_X,
	VariableID_DamperCoeff_Y,
	VariableID_Deviation_X,
	VariableID_Deviation_Y,
	VariableID_Influence_XonY,
	VariableID_Influence_YonX,
	VariableID_SlowdownRatio,
	VariableID_OffsetDelay,
	VariableID_SpringStiff_X,
	VariableID_SpringStiff_Y,
	VariableID_AnticipRatio,
	VariableID_Password,
	VariableID_PasswordKey,
	VariableID_ChatterPath,
	VariableID_TKPunish,
	VariableID_Communication,
	VariableID_Economics,
	VariableID_ForceTeam,
	VariableID_RestrictedWeapons,
	VariableID_DangerFactor,
	VariableID_DontShoot,
	VariableID_HardcoreMode,
	VariableID_AutofollowPercent,
	VariableID_ExplosiveGrenadeUsePercent,
	VariableID_FlashBangGrenadeUsePercent,
	VariableID_SmokeGrenadeUsePercent,
	VariableID_LogoTypes,

	#if defined MULTIPLE_MSEC_METHODS
		VariableID_MsecMethod,
	#endif	// if defined MULTIPLE_MSEC_METHODS

	VariableID_LogLevel,
	VariableID_Config_LoadTime,
	VariableID_WalkAllowed,
	VariableID_Deathmatch,
	VariableID_ThinkFPS,
	VariableID_Ping,		// The ping you want displayed (-2 - use unique bot ping, -1 - use "BOT" tag, min: 0, max: 4095).
	VariableID_PingFlux,	// Fake ping fluctuation amount (0 = none).
	VariableID_DeferToHuman,

	// Voice system....
	VariableID_Voice_inputfromfile,
	VariableID_Voice_recordtofile,
	VariableID_Voice_avggain,
	VariableID_Voice_maxgain,
	VariableID_Voice_scale,
	VariableID_Voice_loopback,
	VariableID_Voice_fadeouttime,		// It fades to no sound at the tail end of your voice data when you release the key.

	// Debugging cvars.
	VariableID_Voice_profile,
	VariableID_Voice_showchannels,		// 1 = list channels, 2 = show timing info, etc
	VariableID_Voice_showincoming,		// show incoming voice data

	VariableID_Voice_forcemicrecord,	// Have it force your mixer control settings so waveIn comes from the microphone. CD rippers change your waveIn to come from the CD drive

	VariableID_Voice_overdrive,			// When voice is on, all other sounds are decreased by this factor.
	VariableID_Voice_overdrivefadetime,	// How long it takes to fade in and out of the voice overdrive.

	VariableID_MaximumAFKTime,

	#if defined MULTIPLE_DRAW_METHODS
		VariableID_DrawMethod,
	#endif	// if defined MULTIPLE_DRAW_METHODS

	// Navigation mesh console variables....
	VariableID_nav_edit,
	VariableID_nav_quicksave,
	VariableID_nav_show_hiding_spots,
	VariableID_nav_show_approach_points,
	VariableID_nav_show_danger,
	VariableID_nav_show_player_counts,
	VariableID_nav_show_occupy_time,
	VariableID_nav_show_battlefront,
	VariableID_nav_show_area_info,
	VariableID_nav_snap_to_grid,
	VariableID_nav_create_place_on_ground,
	VariableID_nav_create_area_at_feet,
	VariableID_nav_show_nodes,
	VariableID_nav_show_compass,
	VariableID_nav_slope_limit,
	VariableID_nav_slope_tolerance,
	VariableID_nav_restart_after_analysis,

	#if defined DEBUG_NAV_NODES
		VariableID_nav_show_node_id,
		VariableID_nav_test_node,
		VariableID_nav_test_node_crouch,
		VariableID_nav_test_node_crouch_dir,
	#endif	// if defined DEBUG_NAV_NODES

	VariableID_nav_coplanar_slope_limit,
	VariableID_nav_split_place_on_ground,
	#if defined MULTIPLE_DRAW_METHODS
		VariableID_nav_area_bgcolor,
	#endif	// if defined MULTIPLE_DRAW_METHODS
	VariableID_nav_area_max_size,
	VariableID_nav_corner_adjust_adjacent,

	VariableID_NavigationMesh_MaximumOffPlaneTolerance,
	VariableID_NavigationMesh_MaximumSampleStepTime,

	VariableID_NavigationMesh_GenerateFenceTopsAreas,
	VariableID_NavigationMesh_SquareUpAreas,
/////////////////////////////////////////////////////////////////
	// Total of all bot console variables.
	VariableID_Total	/// @note Unused!
};

enum VariableValue_NavPointLookMethod_t
{
	VariableValue_NavPointLookMethod_None,	/// @note Unused! (Force to start values at 1)

	VariableValue_NavPointLookMethod_1,	// Just look at next navigation point in path.
	VariableValue_NavPointLookMethod_2,	// Look at path with some distance.
	VariableValue_NavPointLookMethod_3,	// Look at last visible path point.

	VariableValue_NavPointLookMethod_Total	/// @note Unused!
};
enum VariableValue_AimMethod_t
{
	VariableValue_AimMethod_None,	/// @note Unused! (Force to start values at 1)

	VariableValue_AimMethod_1,
	VariableValue_AimMethod_2,
	VariableValue_AimMethod_3,
	VariableValue_AimMethod_4,
	VariableValue_AimMethod_5,

	VariableValue_AimMethod_Total	/// @note Unused!
};
enum VariableValue_TargetingMethod_t
{
	VariableValue_TargetingMethod_None,	/// @note Unused! (Force to start values at 1)

	VariableValue_TargetingMethod_OriginPlusOffset,
	VariableValue_TargetingMethod_HitBox,

	VariableValue_TargetingMethod_Total	/// @note Unused!
};
enum VariableValue_Communication_t
{
	VariableValue_Communication_None,		// No communication with other clients.

	VariableValue_Communication_Ignore,		// 'VariableValue_Communication_None' + bots will ignore commands from clients.

	VariableValue_Communication_Radio,		// Bots will use only radio commands.

	// Bots will use chatter together with radio commands.
	VariableValue_Communication_Voice,		// Old YaPB voice method.
	VariableValue_Communication_VoiceNew,	// Bots uses 'SVC_VOICEDATA' transmitting instead of "BotVoice" and "SendAudio" messages.

	VariableValue_Communication_Total	/// @note Unused!
};
enum VariableValue_MsecMethod_t
{
	VariableValue_MsecMethod_None,	/// @note Unused! (Force to start values at 1)

	VariableValue_MsecMethod_TobiasHeimann,
	VariableValue_MsecMethod_PierreMarieBaty,
	VariableValue_MsecMethod_RichWhitehouse,
	VariableValue_MsecMethod_LeonHartwig,
	VariableValue_MsecMethod_TheStorm,

	VariableValue_MsecMethod_Total	/// @note Unused!
};
enum VariableValue_LogLevel_t
{
	VariableValue_LogLevel_None,	// no log messages at all

	VariableValue_LogLevel_Error,	// error log message
	VariableValue_LogLevel_Warning,	// warning log message
	VariableValue_LogLevel_Note,	// note log message
	VariableValue_LogLevel_Default	// default log message
};
enum VariableValue_Config_LoadTime_t
{
	VariableValue_Config_LoadTime_Once,				// At first server spawn.
	VariableValue_Config_LoadTime_PerServerActivate	// At each server spawn.
};

#if defined MULTIPLE_DRAW_METHODS
	enum VariableValue_DrawMethod_t
	{
		VariableValue_DrawMethod_None,	/// @note Unused! (Force to start values at 1)

		VariableValue_DrawMethod_Default,
		VariableValue_DrawMethod_OpenGL,

		VariableValue_DrawMethod_Total	/// @note Unused!
	};
#endif	// if defined MULTIPLE_DRAW_METHODS

#if defined DEBUG_NAV_NODES
	enum VariableValue_nav_show_nodes_t
	{
		VariableValue_nav_show_nodes_None,

		VariableValue_nav_show_nodes_GridUnderCursor,
		VariableValue_nav_show_nodes_InRadiusUnderCursor,
		VariableValue_nav_show_nodes_All
	};
#endif	// if defined DEBUG_NAV_NODES

#endif	// ifndef BOT_CONSOLE_VARIABLE_ID_INCLUDED