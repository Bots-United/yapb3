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
// BotConsoleVariableManager.cpp
//
// Class: VariableManager
//
// Description: Simple bot console variables manager class.
//
// Version: $ID:$
//

#include <Core.hpp>

namespace Console
{
	namespace Bot
	{
		#define DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS(nameString, valueString, flags, name) ExtendedConsoleVariable (PRODUCT_CONSOLE_TAG "_" nameString, valueString, flags, valueString, OnConsoleVariableStringChange_##name, OnConsoleVariablePrintHelp_##name)

		// List if available bot console variables....
		// NOTE: Static, since the Half-Life engine do not allocate memory for a variable (variable should be global or static), but if plugin started under metamod, we do not require the static variable list.
		VariableManager::ExtendedConsoleVariable VariableManager::sm_variableList[VariableID_Total] =
		{
			// YaPB console variables....
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("auto_vacate",                                                             "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, AutoVacate),									// VariableID_AutoVacate
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("quota",                                                                   "0"/*10*/,                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Quota),									// VariableID_Quota
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("quota_mode",                                                              "fill",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, QuotaMode),									// VariableID_QuotaMode
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("min_skill",                                                               "80"/*95*/,                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, MinSkill),									// VariableID_MinSkill
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("max_skill",                                                               "99"/*100*/,                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, MaxSkill),								// VariableID_MaxSkill
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("follow_user",                                                             "3",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, FollowUser),									// VariableID_FollowUser
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("timer_sound",                                                             "0.5",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, TimerSound),									// VariableID_TimerSound
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("timer_pickup",                                                            "0.5"/*0.2*/,                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, TimerPickup),								// VariableID_TimerPickup
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("timer_grenade",                                                           "0.5",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, TimerGrenade),								// VariableID_TimerGrenade
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("debug_goal",                                                              "-1",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, DebugGoal),									// VariableID_DebugGoal
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("chat",                                                                    "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Chat),										// VariableID_Chat
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("synth",                                                                   "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Synth),										// VariableID_Synth
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("knife_mode",                                                              "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, KnifeMode),									// VariableID_KnifeMode
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("skill_tags",                                                              "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, SkillTags),									// VariableID_SkillTags
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("stop",                                                                    "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Stop),										// VariableID_Stop
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("thru_walls",                                                              "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, ThruWalls),									// VariableID_ThruWalls
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("wall_sensitivity",                                                        "2",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, WallSensitivity),							// VariableID_WallSensitivity
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("votes",                                                                   "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Votes),										// VariableID_Votes
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("spray",                                                                   "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Spray),										// VariableID_Spray
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("buy",                                                                     "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, BotBuy),										// VariableID_BotBuy

			#if defined _DEBUG
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("debug",                                                               "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Debug),										// VariableID_Debug
			#endif	// if defined _DEBUG

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("prefix",                                                                  "[" PRODUCT_SHORT_NAME "]",                           HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_PrintableOnly, Prefix),	// VariableID_Prefix
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("language",                                                                g_localizer.GetDefaultLanguageName ()/*"En"*/,                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_PrintableOnly, Language),	// VariableID_Language
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("enable_client_languages",                                                 "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, EnableClientLanguages),						// VariableID_EnableClientLanguages
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("version",                                                                 PRODUCT_VERSION,                                      HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_ServerPartOnly | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_PrintableOnly, Version),	// VariableID_Version - this console variable is not changeable
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("nav_point_look_method",                                                   "2",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, NavPointLookMethod),							// VariableID_NavPointLookMethod
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_method",                                                              "3",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, AimMethod),									// VariableID_AimMethod
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("targeting_method",                                                        "2",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, TargetingMethod),							// VariableID_TargetingMethod
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_damper_coefficient_x",                                                "0.22",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, DamperCoeff_X),								// VariableID_DamperCoeff_X
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_damper_coefficient_y",                                                "0.22",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, DamperCoeff_Y),								// VariableID_DamperCoeff_Y
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_deviation_x",                                                         "2.0",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Deviation_X),								// VariableID_Deviation_X
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_deviation_y",                                                         "1.0",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Deviation_Y),								// VariableID_Deviation_Y
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_influence_x_on_y",                                                    "0.25",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Influence_XonY),								// VariableID_Influence_XonY
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_influence_y_on_x",                                                    "0.17",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Influence_YonX),								// VariableID_Influence_YonX
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_notarget_slowdown_ratio",                                             "0.5",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, SlowdownRatio),								// VariableID_SlowdownRatio
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_offset_delay",                                                        "1.2",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, OffsetDelay),								// VariableID_OffsetDelay
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_spring_stiffness_x",                                                  "13.0",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, SpringStiff_X),								// VariableID_SpringStiff_X
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_spring_stiffness_y",                                                  "13.0",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, SpringStiff_Y),								// VariableID_SpringStiff_Y
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("aim_target_anticipation_ratio",                                           "2.2",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, AnticipRatio),								// VariableID_AnticipRatio
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("password",                                                                "theBot",                                             HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_None,   Password),									// VariableID_Password
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("password_key",                                                            "_" PRODUCT_SHORT_NAME "AdminPasswordKey",            HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_None,   PasswordKey),								// VariableID_PasswordKey
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("chatter_path",                                                            "sound/radio/bot",                                    HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_PrintableOnly, ChatterPath),	// VariableID_ChatterPath
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("tk_punish",                                                               "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, TKPunish),									// VariableID_TKPunish
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("communication",                                                           "2",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Communication),								// VariableID_Communication
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("economics",                                                               "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Economics),									// VariableID_Economics
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("force_team",                                                              "any",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, ForceTeam),									// VariableID_ForceTeam
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("restricted_weapons",                                                      "ump45;p90;elites;tmp;mac10;m3;xm1014",               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_PrintableOnly, RestrictedWeapons),	// VariableID_RestrictedWeapons
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("danger_factor",                                                           "800",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, DangerFactor),								// VariableID_DangerFactor
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("dont_shoot",                                                              "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, DontShoot),									// VariableID_DontShoot
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("hardcore_mode",                                                           "0"/*1*/,                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, HardcoreMode),							// VariableID_HardcoreMode
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("autofollow_percent",                                                      "60"/*50*/,                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, AutofollowPercent),					// VariableID_AutofollowPercent
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("explosive_grenade_use_percent",                                           "98",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, ExplosiveGrenadeUsePercent),					// VariableID_ExplosiveGrenadeUsePercent
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("flash_bang_grenade_use_percent",                                          "80",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, FlashBangGrenadeUsePercent),					// VariableID_FlashBangGrenadeUsePercent
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("smoke_grenade_use_percent",                                               "50",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, SmokeGrenadeUsePercent),						// VariableID_SmokeGrenadeUsePercent
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("logo_types",                                                              "{biohaz;{graf004;{graf005;{lambda06;{target;{hand1", HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, LogoTypes),									// VariableID_LogoTypes

			#if defined MULTIPLE_MSEC_METHODS
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("msec_method",                                                         "4"/*0*/,                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, MsecMethod),							// VariableID_MsecMethod
			#endif	// if defined MULTIPLE_MSEC_METHODS

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("log_level",                                                               "2",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, LogLevel),									// VariableID_LogLevel
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("config_load_time",                                                        "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Config_LoadTime),							// VariableID_Config_LoadTime
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("walk_allowed",                                                            "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, WalkAllowed),								// VariableID_WalkAllowed
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("death_match",                                                             "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Deathmatch),									// VariableID_Deathmatch
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("think_fps",                                                               "30",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, ThinkFPS),									// VariableID_ThinkFPS
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("ping",                                                                    "-1",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Ping),										// VariableID_Ping - The ping you want displayed (min: 0, max: 4095).
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("ping_flux",                                                               "6",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, PingFlux),									// VariableID_PingFlux - Fake ping fluctuation amount (0 = none).
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("defer_to_human",                                                          "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, DeferToHuman),								// VariableID_DeferToHuman

			// Voice system....
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_inputfromfile",                                                     "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_inputfromfile),						// VariableID_Voice_inputfromfile
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_recordtofile",                                                      "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_recordtofile),							// VariableID_Voice_recordtofile
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_avggain",                                                           "0.5",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_avggain),								// VariableID_Voice_avggain
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_maxgain",                                                           "5",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_maxgain),								// VariableID_Voice_maxgain
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_scale",                                                             "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server/* | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Archive*/, Voice_scale),								// VariableID_Voice_scale
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_loopback",                                                          "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_loopback),								// VariableID_Voice_loopback
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_fadeouttime",                                                       "0.1",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_fadeouttime),							// VariableID_Voice_fadeouttime - It fades to no sound at the tail end of your voice data when you release the key.

			// Debugging cvars.
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_profile",                                                           "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_profile),								// VariableID_Voice_profile
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_showchannels",                                                      "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_showchannels),							// VariableID_Voice_showchannels - 1 = list channels, 2 = show timing info, etc
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_showincoming",                                                      "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_showincoming),							// VariableID_Voice_showincoming - show incoming voice data

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_forcemicrecord",                                                    "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server/* | HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Archive*/, Voice_forcemicrecord),						// VariableID_Voice_forcemicrecord - Have it force your mixer control settings so waveIn comes from the microphone. CD rippers change your waveIn to come from the CD drive

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_overdrive",                                                         "2",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_overdrive),							// VariableID_Voice_overdrive - When voice is on, all other sounds are decreased by this factor.
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("voice_overdrivefadetime",                                                 "0.4",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, Voice_overdrivefadetime),					// VariableID_Voice_overdrivefadetime - How long it takes to fade in and out of the voice overdrive.

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("maximum_AFK_time",                                                        "45",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, MaximumAFKTime),								// VariableID_MaximumAFKTime

			#if defined MULTIPLE_DRAW_METHODS
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS ("draw_method",                                                         "2",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, DrawMethod),									// VariableID_DrawMethod
			#endif	// if defined MULTIPLE_DRAW_METHODS

			// Navigation mesh console variables....
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "edit",                        "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_edit),									// VariableID_nav_edit
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "quicksave",                   "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_quicksave),								// VariableID_nav_quicksave
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_hiding_spots",           "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_hiding_spots),						// VariableID_nav_show_hiding_spots
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_approach_points",        "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_approach_points),					// VariableID_nav_show_approach_points
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_danger",                 "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_danger),							// VariableID_nav_show_danger
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_player_counts",          "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_player_counts),						// VariableID_nav_show_player_counts
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_occupy_time",            "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_occupy_time),						// VariableID_nav_show_occupy_time
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_battlefront",            "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_battlefront),						// VariableID_nav_show_battlefront
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_area_info",              "0.5",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_area_info),							// VariableID_nav_show_area_info
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "snap_to_grid",                "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_snap_to_grid),							// VariableID_nav_snap_to_grid
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "create_place_on_ground",      "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_create_place_on_ground),					// VariableID_nav_create_place_on_ground
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "create_area_at_feet",         "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_create_area_at_feet),					// VariableID_nav_create_area_at_feet
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_nodes",                  "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_nodes),								// VariableID_nav_show_nodes
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_compass",                "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_compass),							// VariableID_nav_show_compass
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "slope_limit",                 "0.707106",                                           HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_slope_limit),							// VariableID_nav_slope_limit
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "slope_tolerance",             "0.1",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_slope_tolerance),						// VariableID_nav_slope_tolerance
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "restart_after_analysis",      "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_restart_after_analysis),					// VariableID_nav_restart_after_analysis

			#if defined DEBUG_NAV_NODES
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "show_node_id",            "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_show_node_id),							// VariableID_nav_show_node_id
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "test_node",               "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_test_node),								// VariableID_nav_test_node
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "test_node_crouch",        "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_test_node_crouch),						// VariableID_nav_test_node_crouch
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "test_node_crouch_dir",    "4",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_test_node_crouch_dir),					// VariableID_nav_test_node_crouch_dir
			#endif	// if defined DEBUG_NAV_NODES

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "coplanar_slope_limit",        "0.99",                                               HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_coplanar_slope_limit),					// VariableID_nav_coplanar_slope_limit
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "split_place_on_ground",       "0",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_split_place_on_ground),					// VariableID_nav_split_place_on_ground
			#if defined MULTIPLE_DRAW_METHODS
				DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "area_bgcolor",            "0 0 0 30",                                           HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_area_bgcolor),							// VariableID_nav_area_bgcolor
			#endif	// if defined MULTIPLE_DRAW_METHODS
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "area_max_size",               "50",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_area_max_size),							// VariableID_nav_area_max_size
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "corner_adjust_adjacent",      "18",                                                 HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, nav_corner_adjust_adjacent),					// VariableID_nav_corner_adjust_adjacent

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "maximum_off_plane_tolerance", "5.0",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, NavigationMesh_MaximumOffPlaneTolerance),	// VariableID_NavigationMesh_MaximumOffPlaneTolerance
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "maximum_sample_step_time",    "0.5",                                                HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, NavigationMesh_MaximumSampleStepTime),		// VariableID_NavigationMesh_MaximumSampleStepTime

			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "generate_fence_tops_areas",   "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, NavigationMesh_GenerateFenceTopsAreas),		// VariableID_NavigationMesh_GenerateFenceTopsAreas
			DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS (NAVIGATION_MESH_CONSOLE_VARIABLE_PREFIX "_" "square_up_areas",             "1",                                                  HalfLifeEngine::SDK::Constants::ConsoleVariableFlag_Server, NavigationMesh_SquareUpAreas)				// VariableID_NavigationMesh_SquareUpAreas
		};

		#undef DECLARE_CONSOLE_VARIABLE_ITEM_WITH_CALLBACKS

		DLL_GLOBAL VariableManager variableManager;

		inline void CommandHandler (void)
		{
			// this function is the dedicated server command handler for the new YaPB server command we registered at game start.
			// It will be called by the engine each time a server command that starts with "YaPB" is entered in the server console.
			// It works exactly the same way as ClientCommand() does, using the CmdArgc() and CmdArgv() facilities of the engine.
			// Argv(0) is the server command itself (here "YaPB") and the next ones are its arguments.
			// Just like the stdio command-line parsing in C when you write "int main (int argc, char **argv)".
			// This function is handler for YaPB console commands.

			// This is the real function to handle the console commands.

			#if defined _DEBUG
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "trans")
				{
					g_server->GetHumanManager ()->GetHostClient ()->Print (HalfLifeEngine::SDK::Constants::HUDPrint_Console, g_server->GetHumanManager ()->GetHostClient ()->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ExecutingMapSpecificConfigurationFile));
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "crash")
				{
//					try{
//						DEBUGGER_BREAK ();
						*(int*)NULL = 8;
//					}catch(...){MessageBox (NULL, "HELLO, THERE IS ERROR!", "HELLO, THERE IS ERROR!", MB_OK | MB_ICONERROR);}
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "exit")
				{
//					ExitProcess (1u);
					exit (1u);
					return;
				}
#if 0
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "link")
				{
					const HalfLifeEngine::SDK::Structures::ServerStatic_t &svs (g_server->GetStatic ());
					const unsigned char i (1u);
//					HalfLifeEngine::SDK::Structures::Client_t &cl (svs.clients[i]);
					HalfLifeEngine::SDK::Structures::Client_t &cl (*reinterpret_cast <HalfLifeEngine::SDK::Structures::Client_t *> (reinterpret_cast <unsigned int> (svs.clients) + i * 22552));	// For NOSTEAM

					cl.active = true;
					cl.spawned = true;

					HalfLifeEngine::SDK::Classes::Edict *bot (cl.edict);

					InternalAssert (bot->IsValid ());

//					g_server->GetClientManager ()->ClientPutInServer (bot);

	YaPB::Profile *const profile (g_server->GetYaPBManager ()->GetProfileManager ().PickBotProfile ());

	// Reliability check.
	InternalAssert (profile != NULL);

	YaPB *const newYaPB (new YaPB (bot, profile));

	InternalAssert (0);

	#if defined _DEBUG
		// Reliability check.
		InternalAssert (newYaPB != NULL);

		// Reliability check.
		InternalAssert (newYaPB->IsValid ());
	#endif	// if defined _DEBUG

	const unsigned char botIndex (static_cast <const unsigned char> (bot->GetIndex ()) - 1u);

	// Reliability check.
	InternalAssert (botIndex < HalfLifeEngine::Globals::g_halfLifeEngine->GetMaximumClients ());	// check bot slot index

	// Delete YaPB fake client wrapper - ugly but it works....
	{
		// Reliability checks.
		InternalAssert (g_server->GetClientManager ()->GetClients ()[botIndex] != NULL);
		InternalAssert (g_server->GetClientManager ()->GetClients ()[botIndex]->IsOtherBot ());

		g_server->GetFakeClientNotYaPBManager ()->RemoveOtherBot (g_server->GetClientManager ()->GetClients ()[botIndex]->GetFakeClientNotYaPBPointer ());

		g_server->GetClientManager ()->GetClientsArray () -= g_server->GetClientManager ()->GetClients ()[botIndex];

		delete g_server->GetClientManager ()->GetClients ()[botIndex];
	}

	g_server->GetClientManager ()->GetClients ()[botIndex] = newYaPB;

	g_server->GetClientManager ()->GetClientsArray () += newYaPB;

	g_server->GetYaPBManager ()->YaPBConnect (newYaPB);

	InternalAssert (0);

					return;
				}
#endif	// if 0
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "ping")
				{
					const HalfLifeEngine::SDK::Structures::ServerStatic_t &svs (g_server->GetStatic ());

					for (unsigned char i=0; i<svs.maxclients ; ++i)
					{
						const HalfLifeEngine::SDK::Structures::Client_t &cl (svs.clients[i]);
//						const HalfLifeEngine::SDK::Structures::Client_t &cl (*reinterpret_cast <HalfLifeEngine::SDK::Structures::Client_t *> (reinterpret_cast <unsigned int> (svs.clients) + i * 22552));	// For NOSTEAM

						if ((!cl.active && !cl.spawned && !cl.connected) || cl.edict == NULL || cl.edict->GetNetName ().IsEmpty ()/*cl.name[0u] == '\0'*/)
							continue;

						const unsigned int ping ((*HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_CalcPingFunction ()) (&cl));
//						const unsigned int ping ((*reinterpret_cast <unsigned int (__fastcall *) (const HalfLifeEngine::SDK::Structures::Client_t *)> (HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_CalcPingFunction ())) (&cl));
/*						unsigned int ping (0);
						const HalfLifeEngine::SDK::Structures::Client_t *pcl (&cl);
						void *f (HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_CalcPingFunction ());
						__asm
						{
							push pcl;//eax;
//							push ebx;
							call f;

//							mov ping, eax;
						}
*/
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Client[%u]: name: \"%s\", ping=%u.\n", i, cl.datagram.name, ping);
					}

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "inac")
				{
					(*HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_InactivateClientsFunction ()) ();
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "tp2")
				{
					for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
					{
						Client *const bot (g_server->GetClientManager ()->GetClientsArray ()[index]);

						if (!bot->IsFakeClient () || !bot->IsAlive ())
							continue;

						bot->GetEdict ()->SetOrigin (g_server->GetHumanManager ()->GetHostClient ()->GetEyeForwardPosition (100.0f));
					}

					g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->variables.health = 1000000.0f;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "mouth")
				{
					for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
					{
						YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

						if (!bot->IsAlive ())
							continue;

						HalfLifeEngine::SDK::Structures::ClientEntity_t *g_clientEntities (*reinterpret_cast <HalfLifeEngine::SDK::Structures::ClientEntity_t **> (reinterpret_cast <unsigned int> (GetModuleHandle ("swds.dll")) + 0x1A97BC));

						InternalAssert (g_clientEntities != NULL);

						g_clientEntities[bot->GetEdict ()->GetIndex ()].mouth.mouthopen = g_randomNumberGenerator.GetValueBetween <unsigned char> ();
					}

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "giveammo")
				{
					for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
					{
						YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

						if (!bot->IsAlive ())
							continue;

						bot->GiveItem ("ammo_338magnum");
						bot->GiveItem ("ammo_762nato");
						bot->GiveItem ("ammo_buckshot");
						bot->GiveItem ("ammo_45acp");
						bot->GiveItem ("ammo_556natobox");
						bot->GiveItem ("ammo_57mm");
						bot->GiveItem ("ammo_9mm");
						bot->GiveItem ("ammo_50ae");
						bot->GiveItem ("ammo_357sig");
						bot->GiveItem ("ammo_556nato");
					}

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "give")
				{
					for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
					{
						YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

						if (!bot->IsAlive ())
							continue;

						bot->GiveItem ("weapon_m3");
//						bot->GiveItem ("weapon_aug");
/*
						bot->GiveItem ("weapon_shield");
/*
						bot->StripWeapons ();
						bot->GiveItem ("weapon_m249");
*/					}
//					g_server->GetYaPBManager ()->SetHealth (1999u);

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "arealoop")
				{
					const HalfLifeEngine::SDK::Classes::NavAreaList &TheNavAreaList (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetMemory <HalfLifeEngine::SDK::Classes::NavAreaList> (0x12A828u));
					InternalAssert (TheNavAreaList.m_head != NULL);
					unsigned int index (0u);
					for (HalfLifeEngine::SDK::Classes::NavAreaList::Node_t *node (TheNavAreaList.m_head->next); node != TheNavAreaList.m_head; node = node->next)
					{
						InternalAssert (node != NULL);
						InternalAssert (node->element != NULL);
						InternalAssert (node->next->prev == node);
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\t\tnode[%u]->element = %u\n", ++index, node->element->m_id);
					}
/*					InternalAssert (TheNavAreaList.m_tail != NULL);	// OCCURS!!!!!!!!!!!!!!!!
					for (HalfLifeEngine::SDK::Classes::NavAreaList::Node_t *node (TheNavAreaList.m_tail->next); node != TheNavAreaList.m_tail; node = node->prev)
					{
						InternalAssert (node != NULL);
						InternalAssert (node->element != NULL);
						InternalAssert (node->prev->next == node);
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\t\tnode[%u]->element = %u\n", --index, node->element->m_id);
					}*/
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Areas NUMBER = %u", TheNavAreaList.m_elementNumber);
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "tp")
				{
					if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u) == "me")
					{
						HalfLifeEngine::SDK::Classes::Edict *pushable (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_conveyor"));

						if (pushable->IsValid ())
							g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->SetOrigin (pushable->GetOrigin () + Math::Vector3D (0.0f, 0.0f, 37.0f));

						return;
					}
		for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
		{
			YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

			if (!bot->IsAlive ())
				continue;
/*
			HalfLifeEngine::SDK::Classes::Edict *pushable (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_conveyor"));

			if (pushable->IsValid ())
				bot->GetEdict ()->SetOrigin (pushable->GetOrigin () + Math::Vector3D (0.0f, 0.0f, 37.0f));*/
				bot->GetEdict ()->SetOrigin (g_server->GetHumanManager ()->GetHostClient ()->GetEyeForwardPosition (100.0f));
		}

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "test")
				{
					const HalfLifeEngine::SDK::Structures::ServerStatic_t &svs (g_server->GetStatic ());

/*					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("svs: dll_initialized=%u, maxclients=%u, maxclientslimit=%u, spawncount=%u.\n", svs.dll_initialized, svs.maxclients, svs.maxclientslimit, svs.spawncount);
*/
					for (unsigned char i=0; i<svs.maxclients ; ++i)
					{
//						const HalfLifeEngine::SDK::Structures::Client_t &cl (svs.clients[i]);
						const HalfLifeEngine::SDK::Structures::Client_t &cl (*reinterpret_cast <HalfLifeEngine::SDK::Structures::Client_t *> (reinterpret_cast <unsigned int> (svs.clients) + i * 22552));	// For NOSTEAM

						if ((!cl.active && !cl.spawned && !cl.connected))// || cl.name[0u] == '\0')
							continue;

						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Client[%u]: datagram name: \"%s\", name: \"%s\", edict: \"%s\", active=%u, spawned=%u, connected=%u, fakeclient=%u.\n", i, cl.datagram.name, cl.name, cl.edict == NULL ? "NULL" : cl.edict->GetNetName ().GetData (), cl.active, cl.spawned, cl.connected, cl.fakeclient);
					}
//*/
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "strip")
				{
					g_server->GetHumanManager ()->GetHostClient ()->StripWeapons ();

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "voice" && !HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u).IsEmpty () && !g_server->GetYaPBManager ()->GetYaPBsArray ().IsEmpty ())
				{
					g_server->GetYaPBManager ()->GetYaPBsArray ().GetRandomElement ()->ChatterMessage (ChatterManager::ChatterMessage_LetsDoThis, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u).GetValue <float> ());

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "votelist")
				{
					const HalfLifeEngine::SDK::Structures::MapCycle_t &mapcycle (g_server->GetMapCycleInformation ());

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%X\n", (unsigned int)&mapcycle - (unsigned int)HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ());

					const HalfLifeEngine::SDK::Structures::MapCycle_t::Item_t *item (mapcycle.nextItem);

					if (item == NULL)
						return;

					unsigned char mapIndex (0u);

					// Traverse list
					do
					{
						// Reliability check.
						InternalAssert (item != NULL);

						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%u : \"%s\"\n", ++mapIndex, item->mapName);
					} while ((item = item->next) != mapcycle.nextItem);

					return;
				}
				else if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "print")
				{
					if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u).IsEmpty ())
						g_server->GetHumanManager ()->GetHostClient ()->GetOwningItemsManager ()->PrintItems ();
					else for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
					{
						YaPB *const bot (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]);

						if (!bot->IsAlive ())
							continue;

						bot->GetOwningItemsManager ()->PrintItems ();
					}

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "wavinfo")
				{
					const DynamicString filename (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u).IsEmpty () ? "cstrike/voice_input.wav" : HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u));
					DynamicString output;

					if (!GetWaveSoundInformation (filename, output))
					{
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("ERROR: Can't GetWaveSoundInformation(filename: \"%s\"): output: \"%s\".\n", filename.GetData (), output.GetData ());

						return;
					}

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("GetWaveSoundInformation(filename: \"%s\"): output: \"%s\".\n", filename.GetData (), output.GetData ());

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "wavscan")
				{
					unsigned int totalFileCount (0u), validFileCount (0u);
					DynamicString searchPath ("cstrike/");

					searchPath += HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u).IsEmpty () ? "sound/radio/bot/cooked" : HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u);

					searchPath += '/';

					const DynamicString path (searchPath);

					searchPath += "*.wav";

					WIN32_FIND_DATA pFindFileData;
					HANDLE hFile (FindFirstFile (searchPath, &pFindFileData));	// find first file

					// for each element found... (while handle is valid)
					while (hFile != INVALID_HANDLE_VALUE)
					{
						++totalFileCount;	// increment the count

		{
					const DynamicString filename (path + pFindFileData.cFileName);
					DynamicString output;

					if (!GetWaveSoundInformation (filename, output))
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("ERROR: Can't GetWaveSoundInformation(filename: \"%s\"): output: \"%s\".\n", filename.GetData (), output.GetData ());
					else
					{
						// open the wave file with binary read mode
						STDIOFile wave (filename, "rb");

						// check if got valid handle
						if (!wave.IsValid ())
							InternalAssert (0);

						// WAVE file related structure definitions
						WaveFileHeader_t waveFileHeader;

						// try to read the file
						if (!wave.Read (waveFileHeader))
							InternalAssert (0);

						if (waveFileHeader.format.sampleRate == 8000u && waveFileHeader.format.bytesPerSecond == 16000u && waveFileHeader.format.bytesPerSample == 2u && waveFileHeader.format.bitsPerSample == 16u)
						{
							++validFileCount;

							HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("I FOUND \"%s\", output: \"%s\".\n", filename.GetData (), output.GetData ());
						}
//						else
//							HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("FILE \"%s\" IS WRONG, output: \"%s\".\n", filename.GetData (), output.GetData ());
					}
		}
						// go and find a handle on the next file
						if (!FindNextFile (hFile, &pFindFileData))	// while we can reach next file
							break;
					}

					FindClose (hFile);	// close the file search

					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Search completed! totalFileCount=%u, validFileCount=%u.\n", totalFileCount, validFileCount);

					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "wavconvert")
				{
					XASH::CONVERT_SOUND ("cstrike/voice_input.wav");
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Conversion completed!\n");
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "showsid")
				{
					for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
					{
						const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Client \"%s\", UNKNOWN7_connectionInformation.steamID=%I64u[m_unAccountID=%u, m_unAccountInstance=%u, m_EAccountType=%u, m_EUniverse=%u] *sid: \"%s\".\n", client->GetName ().GetData (), client->GetEngineClient ().UNKNOWN7_connectionInformation.steamID.ConvertToUint64 (), client->GetEngineClient ().UNKNOWN7_connectionInformation.steamID.m_unAccountID, client->GetEngineClient ().UNKNOWN7_connectionInformation.steamID.m_unAccountInstance, client->GetEngineClient ().UNKNOWN7_connectionInformation.steamID.m_EAccountType, client->GetEngineClient ().UNKNOWN7_connectionInformation.steamID.m_EUniverse, HalfLifeEngine::Globals::g_halfLifeEngine->GetInfoKeyValue (client->GetEdict ()->GetInfoKeyBuffer (), "*sid"));
					}
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "showip")
				{
					for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
					{
						const Client *const client (g_server->GetClientManager ()->GetClientsArray ()[index]);

						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Client \"%s\", netchan.remote_address.ip=[%u, %u, %u, %u], UNKNOWN7_connectionInformation.ip=[%u, %u, %u, %u].\n", client->GetName ().GetData (), client->GetEngineClient ().netchan.remote_address.ip[0u], client->GetEngineClient ().netchan.remote_address.ip[1u], client->GetEngineClient ().netchan.remote_address.ip[2u], client->GetEngineClient ().netchan.remote_address.ip[3u], client->GetEngineClient ().UNKNOWN7_connectionInformation.ip[0u], client->GetEngineClient ().UNKNOWN7_connectionInformation.ip[1u], client->GetEngineClient ().UNKNOWN7_connectionInformation.ip[2u], client->GetEngineClient ().UNKNOWN7_connectionInformation.ip[3u]);
					}
					return;
				}
				if (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u) == "decmap")
				{
					g_server->GetNavigationMeshManager ().GetWorldMap ().Mod_Decompile_f ();
					return;
				}
			#endif	// if defined _DEBUG

			g_origins.RemoveAll ();
/*
			const HalfLifeEngine::SDK::Structures::Server_t &sv (g_server->GetServer ());

			InternalAssert (sv.name == reinterpret_cast <const char *> (&sv) + 48);

			AddLogEntry (true, LogLevel_Default, true, "Name: %s(%s), prevMapName: \"%s\", startspot: %s, modelname: '%s', worldmodel: '%s'.", sv.name, reinterpret_cast <const char *> (&sv) + 48, sv.prevMapName, sv.startspot, sv.modelname, sv.worldmodel->name);
*/
			const DynamicString argument1 (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (1u));
			const DynamicString argument2 (HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (2u));
/*
			if (!argument1.IsEmpty ())
			{
			for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
				g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->ExecuteCommand (argument1);//"radio3; menuselect 1; say \"I'M DONE THIS SHIT!!!\"");
			return;
			}
/*
			if (MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[1u]->GetEdict ()->privateData) != MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[2u]->GetEdict ()->privateData))
			{
//				AddLogEntry (false, LogLevel_Critical, true, "pl(%x) != bot(%x).", MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[0u]->GetEdict ()->privateData), MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[1u]->GetEdict ()->privateData));
//				AddLogEntry (false, LogLevel_Critical, true, "diff=-196.", static_cast <int> (MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[1u]->GetEdict ()->privateData) - MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[0u]->GetEdict ()->privateData)));
				AddLogEntry (false, LogLevel_Critical, true, "diff=%i.", static_cast <int> (MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[1u]->GetEdict ()->privateData) - MemoryUtilities::GetElementsTable <void *const> (g_server->GetClientManager ()->GetClientsArray ()[2u]->GetEdict ()->privateData)));
			}*/
#if 0
			for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
			{
				const HalfLifeEngine::SDK::Classes::Edict *const clientEdict (g_server->GetClientManager ()->GetClientsArray ()[index]->GetEdict ());

				AddLogEntry (true, LogLevel_Default, true, "clientEdict: \"%s\", fastcall: BasePlayer::IsBot()=%u, thiscall: BasePlayer::IsBot()=%u.", clientEdict->GetNetName ().GetData (), static_cast <bool (__fastcall *) (HalfLifeEngine::SDK::Classes::BasePlayer *const /*this*/, int /*dummy*/)> (MemoryUtilities::GetElementsTable <void *const> (clientEdict->privateData)[81u]) (static_cast <HalfLifeEngine::SDK::Classes::BasePlayer *> (clientEdict->privateData), 0), static_cast <bool (__thiscall *) (HalfLifeEngine::SDK::Classes::BasePlayer *const /*this*/)> (MemoryUtilities::GetElementsTable <void *const> (clientEdict->privateData)[81u]) (static_cast <HalfLifeEngine::SDK::Classes::BasePlayer *> (clientEdict->privateData)));
			}
#endif	// if 0
/*			if (!argument1.IsEmpty ())
			{
				const float radius (argument1.GetValue <float> ());

				AddLogEntry (true, LogLevel_Default, true, "C4 radius is set to %f.", radius);

				const_cast <HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *const> (g_server->GetGameRules ())->m_c4ExplosionRadius = radius;
			}*/
/*			if (!argument1.IsEmpty ())
			{
				for (unsigned char index (0u); index < g_server->GetYaPBManager ()->GetBotsCount (); ++index)
				{
					HalfLifeEngine::SDK::Classes::Edict *const yapbEdict (g_server->GetYaPBManager ()->GetYaPBsArray ()[index]->GetEdict ());

					g_server->GetClientManager ()->GetFakeClientCommand ().ExecuteWithSeparatedArguments (yapbEdict, argument1, argument2, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (3u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (4u));
//					g_server->GetClientManager ()->GetFakeClientCommand ().ExecuteWithSeparatedArguments (yapbEdict, "menuselect", "1");
				}
			}
*/
			// Check status for passed dedicated server command.
			g_consoleCommandsManager.HandleExecutedCommand (g_server->IsRunning () ? g_server->GetHumanManager ()->GetHostClient () : NULL, argument1.IsEmpty () ? '\?' : argument1, argument2, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (3u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (4u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (5u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (6u));
/*			switch (g_server->GetHumanManager ()->GetHostClient ()->BotCommandHandler (argument1.IsEmpty () ? '\?' : argument1, argument2, HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (3u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (4u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (5u), HalfLifeEngine::Globals::g_halfLifeEngine->GetCommandArgument (6u)))
			{
				case BotConsoleCommandReturnType_NotFound:
					AddLogEntry (true, LogLevel_Warning, false, "Unknown command: \"%s\".", argument1.GetData ());

					break;

				case BotConsoleCommandReturnType_ServerOnly:
					AddLogEntry (true, LogLevel_Warning, false, "Command %s, can only be executed from server console.", argument1.GetData ());

					break;

				case BotConsoleCommandReturnType_RequireAccess:
					AddLogEntry (true, LogLevel_Warning, false, "Console variable " PRODUCT_CONSOLE_TAG "_%s, can be only set via RCON access.", argument2.GetData ());

					break;
			}*/
		}

		void VariableManager::RegisterVariables (void)
		{
			unsigned char registeredVariablesNumber (0u);

			// Loop through all bot console variables....
			for (unsigned char variableIndex (VariableID_AutoVacate); variableIndex < VariableID_Total; ++variableIndex)
			{
				// Register current console variable....
				HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_Register (sm_variableList[variableIndex]);

				// Get pointer to this already registered console variable.
				HalfLifeEngine::SDK::Classes::ConsoleVariable *const pointer (HalfLifeEngine::Globals::g_halfLifeEngine->ConsoleVariable_GetPointer (sm_variableList[variableIndex].name));

				// Reliability check.
				if (pointer->IsValid ())
				{
					// Reliability check.
///					InternalAssert (m_consoleVariables[variableIndex] == pointer);	/// @warning THIS ALWAYS FAIL UNDER META MOD!

					m_consoleVariables[variableIndex] = pointer;	// Update our pointer (to 'sm_variableList') to engine one.

					++registeredVariablesNumber;	// Increase number of successfully registered console variables.

					continue;
				}

				// Error!
				// If failed to register the console variable, then we will simply use our pointer (Which is already set in constructor).... (HACK)
				AddLogEntry (true, LogLevel_Error, false, "Unable to register console variable \"%s\"! Using default value \"%s\"....", sm_variableList[variableIndex].name, sm_variableList[variableIndex].m_defaultString);
			}

			AddLogEntry (true, LogLevel_Default, false, "%u of %u " PRODUCT_SHORT_NAME " console variables successfully registered.", registeredVariablesNumber, VariableID_Total);

			// Execute main configuration file (is put here second time, that the initialization of configuration bot files in ServerActivate() function passed without mistakes).
			HalfLifeEngine::Globals::g_halfLifeEngine->ExecuteServerCommandNow ("exec Addons/YaPB/Configs/YaPB.cfg\n");

			// tell the engine that a new server command is being declared, in addition to the standard ones, whose name is 'commandName' (in our case - 'yapb'/'yb').
			// The engine is thus supposed to be aware that for every 'commandName' server command it receives,
			// it should call the function pointed to by 'function' (in our case - 'CommandHandler') in order to handle it.
			HalfLifeEngine::Globals::g_halfLifeEngine->RegisterServerCommand (PRODUCT_CONSOLE_TAG, CommandHandler);
			HalfLifeEngine::Globals::g_halfLifeEngine->RegisterServerCommand (PRODUCT_CONSOLE_TAG_OTHER, CommandHandler);

			GetCalling_Con_Printf_VariableValueIsHooker ().DoPatch ();
		}

		/*static */void VariableManager::OnConsoleVariableStringChange_Communication (const char *const newString)
		{
			const unsigned char newValue (static_cast <unsigned char> (atof (newString)));

			// Check ranges....
			if (newValue >= VariableValue_Communication_Total)
			{
				AddLogEntry (true, LogLevel_Warning, false, "Value of '%s' should be in range %u...%u!", variableManager.GetVariable (VariableID_Communication)->GetName ().GetData (), VariableValue_Communication_None, VariableValue_Communication_Total - 1u);

				// Fail!
				return;
			}

			if (g_server->IsRunning ())
			{
				if (newValue == VariableValue_Communication_VoiceNew)
				{
					if (Server::variableManager.GetVariable (Server::VariableID_VoiceEnable)->GetValue <bool> () && !g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized () && g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialize (Server::variableManager.GetVariable (Server::VariableID_Voice_codec)->GetString ()))
					{
						// Set new value here, becaus this variable will be used in OnChangeCodec() function, before we call original function, so old value is still set....
						// Call the original function....
						g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Communication), newString);

						g_server->GetYaPBManager ()->OnChangeCodec (Server::variableManager.GetVariable (Server::VariableID_Voice_quality)->GetValue <unsigned char> ());

						return;
					}
				}
				else if (g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized ())
					g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Deinitialize ();
			}

			// Call the original function....
			g_consoleVariableValueChangeHooker.CallOriginalFunction (variableManager.GetVariable (VariableID_Communication), newString);
		}
	}
}