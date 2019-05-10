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
// Localizer.cpp
//
// Class: Localizer
//
// Description: Texts localizer.
//
// Version: $ID:$
//

#include <Core.hpp>

Localizer g_localizer;
LocalizerManager g_localizerManager;

/*static */const Localizer::LanguageName_t Localizer::sm_defaultLanguageName ("En");

void Localizer::LoadOriginalStrings (void)
{
	m_currentLanguageName = GetDefaultLanguageName ();

	m_strings[LocalizedStringID_TODO]                                  = "TRANSLATION WILL BE ADDED LATER!!!";

	m_strings[LocalizedStringID_UnableToCreateYaPB_Reason]             = "Unable to create YaPB: %s";
	m_strings[LocalizedStringID_BothTeamsReachedMaximumPlayers]        = "Unable to create YaPB: Both teams reached the maximum number of players (T's: %u/CT's: %u).";
	m_strings[LocalizedStringID_MaximumPlayersReached]                 = "Unable to create YaPB: Maximum players reached (%u/%u).";
	m_strings[LocalizedStringID_MaximumPlayersReached]                 = "Unable to create YaPB: Team is stacked (to disable this check, set mp_limitteams and mp_autoteambalance to zero and restart the round).";
	m_strings[LocalizedStringID_InvalidSkillGivenUsingRandomSkill]     = "Invalid skill given. Using random skill.";
	m_strings[LocalizedStringID_ConnectingBot]                         = "Connecting '%s'... (skill %u)";
	m_strings[LocalizedStringID_FillServerWithBots]                    = "Fill Server with %s bots....";
	m_strings[LocalizedStringID_YaPBs]                                 = "YaPB's";
	m_strings[LocalizedStringID_Clients]                               = "clients";
	m_strings[LocalizedStringID_Humans]                                = "humans";
	m_strings[LocalizedStringID_FakeClients]                           = "fake clients";
	m_strings[LocalizedStringID_AllKickedFromServer]                   = "All %s are kicked from server.";
	m_strings[LocalizedStringID_YaPBKicked]                            = "YaPB '%s' kicked.";
	m_strings[LocalizedStringID_AllKilled]                             = "All %s are killed.";
	m_strings[LocalizedStringID_BotsHealthIsSetTo]                     = "Bots health is set to %u%%.";
	m_strings[LocalizedStringID_ThereAreNoYaPBOnTheServer]             = "There are no YaPB on the server.";
	m_strings[LocalizedStringID_WeaponModeSelected]                    = "%s weapon mode selected.";
	m_strings[LocalizedStringID_ChooseWeaponFrom1To7Range]             = "Choose weapon from 1 to 7 range!";
	m_strings[LocalizedStringID_PleaseSpecifyOrderCommand]             = "Please specify order command.\n";
	m_strings[LocalizedStringID_PlayerIsNotYaPB]                       = "Player '%s' is NOT a YaPB!\n";
	m_strings[LocalizedStringID_AllBotsHasExecutedCommand]             = "All bots has executed command \"%s\".";
	m_strings[LocalizedStringID_BotHasExecutedCommand]                 = "Bot '%s' has executed command \"%s\".";
	m_strings[LocalizedStringID_BotWithThatUserIDNotFound]             = "Bot with that user ID not found.";
	m_strings[LocalizedStringID_BotWithThatNameNotFound]               = "Bot with that name not found.";
	m_strings[LocalizedStringID_CurrentTimeIs]                         = "--- Current Time: %02u:%02u:%02u ---";
	m_strings[LocalizedStringID_CurrentDateIs]                         = "--- Current Date: %02u/%02u/%04u ---";
	m_strings[LocalizedStringID_PleaseSpecifyHealth]                   = "Please specify health.";
	m_strings[LocalizedStringID_YouDeadAndHaveNoAccessToThisMenu]      = "You're dead, and have no access to this menu";
	m_strings[LocalizedStringID_ExecutingMainConfigurationFile]        = "Executing main configuration file....";
	m_strings[LocalizedStringID_NoMainConfigurationFileFound]          = "No main configuration file found, using default variable values.";
	m_strings[LocalizedStringID_ExecutingMapSpecificConfigurationFile] = "Executing map-specific configuration file....";
	m_strings[LocalizedStringID_NoMapSpecificConfigurationFileFound]   = "No map-specific configuration file found, using default variable values.";
	m_strings[LocalizedStringID_PlayerHadLostRemoteAccessToYaPB]       = "Player '%s' had lost remote access to YaPB.";
	m_strings[LocalizedStringID_PlayerHadGainedFullRemoteAccessToYaPB] = "Player '%s' had gained full remote access to YaPB.";
	m_strings[LocalizedStringID_CommandNotSupportedOnDedicatedServer]  = "Command not supported on dedicated server.";
	m_strings[LocalizedStringID_AutoSelect]                            = "Auto-select";

	m_strings[LocalizedStringID_MOTD_Title_BotsList]                   = "YaPB's list";

	// Waypoints....
	m_strings[LocalizedStringID_MapNotWaypointed]                      = "Unable to create YaPB: Map not waypointed.";
	m_strings[LocalizedStringID_WaypointsHasBeenChanged]               = "Waypoints has been changed. Load waypoints again....";
	m_strings[LocalizedStringID_PathAlreadyExists]                     = "Denied path creation from %u to %u (path already exists).";
	m_strings[LocalizedStringID_PathAddedFromTo]                       = "Path added from %u to %u.";
	m_strings[LocalizedStringID_ThisIsNotCampingWaypoint]              = "This is not Camping waypoint!";
	m_strings[LocalizedStringID_CannotConnectWaypointToItself]         = "Cannot connect waypoint to itself!";

	// Navigation mesh....
	m_strings[LocalizedStringID_NavigationMesh_NotExits] = "Navigation mesh is not exits! (Do '" PRODUCT_CONSOLE_TAG " navmesh generate' to create it)";

	// Console variables....
	m_strings[LocalizedStringID_Variable_AutoVacate]                              = "Specifies if one slot is keep free for player connection. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Quota]                                   = "Specifies number of bots playing on the server machine. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_QuotaMode]                               = "Determines the type of quota.\nAllowed values: 'normal', 'fill', and 'match'.\nIf 'fill', the server will adjust bots to keep N players in the game, where N is 'yb_quota'.\nIf 'match', the server will maintain a 1:N ratio of humans to bots, where N is 'yb_quota'.\n(Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_MinSkill]                                = "Specifies minimum bots skill, when creating bot with random parameters. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_MaxSkill]                                = "Specifies maximum bots skill, when creating bot with random parameters. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_FollowUser]                              = "Specifies number of bots that can follow player, that used Follow me radio message. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_TimerSound]                              = "Specifies time interval, used with bot hearing system. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_TimerPickup]                             = "Specifies time interval, used to check for items around the bot. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_TimerGrenade]                            = "Specifies time interval, used to check for grenade throw. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_DebugGoal]                               = "Goal to move. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Chat]                                    = "Specifies if bots able to chat each-other while they are dead. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Synth]                                   = "Specifies if bot dll able to comment some of the commands, by half-life speech engine. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_KnifeMode]                               = "Specifies mode, where bots able to fight only on knives. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_SkillTags]                               = "Specifies, if skill value for bot will be appended to it name, after bot creation. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Stop]                                    = "Specifies, if all bots on the server stopbots function, while this variable set to 1. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_ThruWalls]                               = "Specifies, if bots able to shoot through wall when suspectiong an enemy. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_WallSensitivity]                         = " (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Votes]                                   = "This option turns on/off Bots voting against human teamkillers, and for the maps. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Spray]                                   = "Specifies, if bots are able to spraypaints graphical logos in a game. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_BotBuy]                                  = "Specifies, if bots able to purchase weapon and items at start of the each round. (Default value: \"%s\")";

	#if defined _DEBUG
		m_strings[LocalizedStringID_Variable_Debug]                               = "Specifies level for bot debugging engine. For developers and betatesting team only. (Default value: \"%s\")";
	#endif	// if defined _DEBUG

	m_strings[LocalizedStringID_Variable_Prefix]                                  = "Specifies nameprefix which will be added to start of the bot name, at the creation time. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Language]                                = "Specifies the language in which all bot menus and comments are displayed. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_EnableClientLanguages]                   = ". (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Version]                                 = "Prints current YaPB version.";
	m_strings[LocalizedStringID_Variable_NavPointLookMethod]                      = "Specifies method for looking of bots when moving towards path. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_AimMethod]                               = "Specifies method for aiming of bots. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_TargetingMethod]                         = "Specifies method for targeting on players of bots. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_DamperCoeff_X]                           = "VERTICAL (up/down) damping coefficient of the aiming system. This value also affects the quickness of the crosshair movement, and the amplitude of the resulting oscillations. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_DamperCoeff_Y]                           = "HORIZONTAL (left/right) damping coefficient of the aiming system. This value also affects the quickness of the crosshair movement, and the amplitude of the resulting oscillations. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Deviation_X]                             = "VERTICAL (up/down) error margin of the aiming system. This value affects how far (in game length units) the bot will allow its crosshair to derivate from the ideal direction when not targeting any player in particular. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Deviation_Y]                             = "HORIZONTAL (left/right) error margin of the aiming system. This value affects how far (in game length units) the bot will allow its crosshair to derivate from the ideal direction when not targeting any player in particular. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Influence_XonY]                          = "Perpendicular influence of the vertical axis on a HORIZONTAL movement. This value affects how much (in fraction of 1) the bot will be disturbed when moving its crosshair on an axis by the inherent movement on the other axis. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Influence_YonX]                          = "Perpendicular influence of the horizontal axis on a VERTICAL movement. This value affects how much (in fraction of 1) the bot will be disturbed when moving its crosshair on an axis by the inherent movement on the other axis. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_SlowdownRatio]                           = "Fraction of the full speed the aiming system will adopt as speed when aiming at nothing in particular. This affects how slow the bot will move its crosshair when targeting nobody, relatively to its full speed capacity. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_OffsetDelay]                             = "Maximal value in seconds after which the aiming system will re-evaluate its ideal direction. This affects how often a bot will attempt to correct an imprecise crosshair placement by moving the crosshair inside the error margin bounds defined by the botaim_deviation CVARs. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_SpringStiff_X]                           = "VERTICAL (up/down) spring stiffness of the aiming system. This value affects the quickness of the crosshair movement, as well as the frequency of the resulting oscillations. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_SpringStiff_Y]                           = "HORIZONTAL (left/right) spring stiffness of the aiming system. This value affects the quickness of the crosshair movement, as well as the frequency of the resulting oscillations. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_AnticipRatio]                            = "Fraction of the estimated target's velocity the aiming system will rely on when aiming at a moving target. This affects how well the bot will be able to track moving targets on the fly, and whether it will have a tendancy to aim ahead of it or behind it. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Password]                                = "Specifies password for accessing bot menu and commands on the server running YaPB. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_PasswordKey]                             = "Specifies pasword key for setinfo information to acccess to the bot menu and command. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_ChatterPath]                             = "Specifies path to the Official CSBot's chatter files. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_TKPunish]                                = "Specifies if bot's teamkiller will be punshied for his heroic action. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Communication]                           = "Specifies method for commtype engine for bots. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Economics]                               = "Specifies if bots are able to use team-ecorounds to save money. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_ForceTeam]                               = "Specifies the team, in which all creating bots will be forced to join. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_RestrictedWeapons]                       = "Specifies list of the weapons, that are banned for botbuying and picking up. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_DangerFactor]                            = "Specifies danger factor for bots experience system, higher values resulting more tactical bots. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_DontShoot]                               = "Specifies if bots are able to shoot their enemies (for debugging). (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_HardcoreMode]                            = "Specifies if bots can only be running in hard mode. Enable only if you playing very well. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_AutofollowPercent]                       = "Specifies percent of bots that can autofollow human player at start of each round. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_ExplosiveGrenadeUsePercent]              = "Specifies percent of explosive grenades buy/usage by bots. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_FlashBangGrenadeUsePercent]              = "Specifies percent of flash bang grenades buy/usage by bots. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_SmokeGrenadeUsePercent]                  = "Specifies percent of smoke grenades buy/usage by bots. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_LogoTypes]                               = "Specifies list of the logotypes used by bot. (Default value: \"%s\")";

	#if defined MULTIPLE_MSEC_METHODS
		m_strings[LocalizedStringID_Variable_MsecMethod]                          = "Specifies method for calculating the msec value. (Default value: \"%s\")";
	#endif	// if defined MULTIPLE_MSEC_METHODS

	m_strings[LocalizedStringID_Variable_LogLevel]                                = "Specifies log level for bots messages. Critical errors will be logged anyway. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Config_LoadTime]                         = "Specifies time when main configuration file will be loaded. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_WalkAllowed]                             = "Specifies if bot can use shift while moving in battle. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Deathmatch]                              = "Specifies csdm gameplay mode. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_ThinkFPS]                                = "Specifies amount of bot FPS. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Ping]                                    = "Specifies amount of bots fake ping. (Default value: \"%s\")";	// The ping you want displayed (min: 0] = ""; max: 4095).
	m_strings[LocalizedStringID_Variable_PingFlux]                                = "Specifies amount of bots fake ping fluctuation. (Default value: \"%s\")";	// Fake ping fluctuation amount (0 = none).
	m_strings[LocalizedStringID_Variable_DeferToHuman]                            = "If nonzero and there is a human on the team, the bots will not do the scenario tasks. (Default value: \"%s\")";

	// Voice system....
	m_strings[LocalizedStringID_Variable_Voice_inputfromfile]                     = "Record mic data and decompressed voice data into 'voice_micdata.wav' and 'voice_decompressed.wav'. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_recordtofile]                      = "Get voice input from 'voice_input.wav' rather than from the microphone. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_avggain]                           = " (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_maxgain]                           = " (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_scale]                             = " (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_loopback]                          = " (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_fadeouttime]                       = "It fades to no sound at the tail end of your voice data when you release the key. (Default value: \"%s\")";

	// Debugging cvars.
	m_strings[LocalizedStringID_Variable_Voice_profile]                           = " (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_showchannels]                      = "1 = list channels, 2 = show timing info, etc (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_showincoming]                      = "show incoming voice data (Default value: \"%s\")";

	m_strings[LocalizedStringID_Variable_Voice_forcemicrecord]                    = "Have it force your mixer control settings so waveIn comes from the microphone. CD rippers change your waveIn to come from the CD drive (Default value: \"%s\")";

	m_strings[LocalizedStringID_Variable_Voice_overdrive]                         = "When voice is on, all other sounds are decreased by this factor. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_Voice_overdrivefadetime]                 = "How long it takes to fade in and out of the voice overdrive. (Default value: \"%s\")";

	m_strings[LocalizedStringID_Variable_MaximumAFKTime]                          = "Specifies time interval, used to check for human AFK. (Default value: \"%s\")";

	#if defined MULTIPLE_DRAW_METHODS
		m_strings[LocalizedStringID_Variable_DrawMethod]                          = " (Default value: \"%s\")";
	#endif	// if defined MULTIPLE_DRAW_METHODS

/////////////////////////////////////////////////////////////////
	m_strings[LocalizedStringID_Variable_nav_edit]                                = "Set to one to interactively edit the Navigation Mesh. Set to zero to leave edit mode. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_quicksave]                           = "Set to one to skip the time consuming phases of the analysis. Useful for data collection and testing. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_hiding_spots]                   = "Show Hidding Spots in the Navigation Mesh. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_approach_points]                = "Show Approach Points in the Navigation Mesh. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_danger]                         = "Show current 'danger' levels. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_player_counts]                  = "Show current player counts in each area. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_occupy_time]                    = "Show when each nav area can first be reached by each team. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_battlefront]                    = "Show areas where rushing players will initially meet. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_area_info]                      = "Duration in seconds to show navigation area ID and attributes while editing. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_snap_to_grid]                        = "Snap to the navigation generation grid when creating new navigation areas. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_create_place_on_ground]              = "If true, navigation areas will be placed flush with the ground when created by hand. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_create_area_at_feet]                 = "Anchor nav_begin_area Z to editing player's feet. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_nodes]                          = "Show generated navigation nodes. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_show_compass]                        = "Show compass. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_slope_limit]                         = "The ground unit normal's Z component must be greater than this for navigation areas to be generated. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_slope_tolerance]                     = "The ground unit normal's Z component must be this close to the navigation area's Z component to be generated. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_restart_after_analysis]              = "When navigation " PRODUCT_CONSOLE_TAG "_nav_restart_after_analysis finishes, restart the server. Turning this off can cause crashes, but is useful for incremental generation. (Default value: \"%s\")";

	#if defined DEBUG_NAV_NODES
		m_strings[LocalizedStringID_Variable_nav_show_node_id]                    = "Show Navigation Node in the Navigation Mesh by it's ID. (Default value: \"%s\")";
		m_strings[LocalizedStringID_Variable_nav_test_node]                       = "Enter Navigation Node ID to test it. (Default value: \"%s\")";
		m_strings[LocalizedStringID_Variable_nav_test_node_crouch]                = " (Default value: \"%s\")";
		m_strings[LocalizedStringID_Variable_nav_test_node_crouch_dir]            = " (Default value: \"%s\")";
	#endif	// if defined DEBUG_NAV_NODES

	m_strings[LocalizedStringID_Variable_nav_coplanar_slope_limit]                = "Specifies amount of maximum co-planar slope limit for Navigation Areas. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_split_place_on_ground]               = "If true, navigation areas will be placed flush with the ground when split. (Default value: \"%s\")";
	#if defined MULTIPLE_DRAW_METHODS
		m_strings[LocalizedStringID_Variable_nav_area_bgcolor]                    = "RGB color to draw as the background color for navigation areas while editing. (Default value: \"%s\")";
	#endif	// if defined MULTIPLE_DRAW_METHODS
	m_strings[LocalizedStringID_Variable_nav_area_max_size]                       = "Maximum area size created in navigation generation. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_nav_corner_adjust_adjacent]              = "Radius used to raise/lower corners in nearby areas when raising/lowering corners. (Default value: \"%s\")";

	m_strings[LocalizedStringID_Variable_NavigationMesh_MaximumOffPlaneTolerance] = "Specifies maximum off plane tolerance when connect navigation areas. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_NavigationMesh_MaximumSampleStepTime]    = "Specifies maximum sample step execution time. (Default value: \"%s\")";

	m_strings[LocalizedStringID_Variable_NavigationMesh_GenerateFenceTopsAreas]   = "Autogenerate navigation areas on fence and obstacle tops. (Default value: \"%s\")";
	m_strings[LocalizedStringID_Variable_NavigationMesh_SquareUpAreas]            = "If true, navigation areas will be squared up after generation complete. (Default value: \"%s\")";

	// Console commands....

	// Main ------------------------------------------------------------------------------------------------------------------------
	m_strings[LocalizedStringID_ConsoleCommandCanOnlyBeExecutedFromServerConsole]                        = "Command '%s' can only be executed from server console.";
	m_strings[LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedFromServerConsole]   = "Client \"%s\" has executed command '%s', that can only be executed from server console.";
	m_strings[LocalizedStringID_ConsoleVariableCanBeOnlySetViaRCONAccess]                                = "Console variable '" PRODUCT_CONSOLE_TAG "_%s' can be only set via RCON access.";
	m_strings[LocalizedStringID_ClientHasExecutedConsoleVariableCanBeOnlySetViaRCONAccess]               = "Client \"%s\" has executed console variable '" PRODUCT_CONSOLE_TAG "_%s', that can be only set via RCON access.";
	m_strings[LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning]                      = "Command '%s' can only be executed when server is running.";
	m_strings[LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedWhenServerIsRunning] = "Client \"%s\" has executed command '%s', that can only be executed when server is running.";
	m_strings[LocalizedStringID_UnknownConsoleCommand]                                                   = "Unknown console command: '%s'.";
	m_strings[LocalizedStringID_ClientHasExecutedUnknownConsoleCommand]                                  = "Client \"%s\" has executed unknown console command: '%s'.";

	// Localizer -------------------------------------------------------------------------------------------------------------------
	m_strings[LocalizedStringID_ConsoleCommand_Localizer] = "Displays main localizer console commands container.";
		m_strings[LocalizedStringID_ConsoleCommand_Localizer_Reload]              = "Reload localizer for current language. ('" PRODUCT_CONSOLE_TAG "_language' variable)";
		m_strings[LocalizedStringID_ConsoleCommand_Localizer_LoadOriginalStrings] = "Load original strings for default English language.";

	// Bot -------------------------------------------------------------------------------------------------------------------------
	m_strings[LocalizedStringID_DisplaysBotConsoleCommandsContainer] = "Displays main YaPB console commands container.";
		m_strings[LocalizedStringID_DisplaysVersionInformation]            = "Displays version information.";
		m_strings[LocalizedStringID_ShowsAboutBotInformation]              = "Shows about bot information.";
		m_strings[LocalizedStringID_FillsServerWithRandomBots]             = "Fills the server with random bots.";
		m_strings[LocalizedStringID_DisconnectsAllBotsFromCurrentGame]     = "Disconnects all bots from current game.";
		m_strings[LocalizedStringID_KillsAllBotsInCurrentGame]             = "Kills all bots in current game.";
		m_strings[LocalizedStringID_CreatesBotInCurrentGame]               = "Creates a bot in current game.";
		m_strings[LocalizedStringID_DisconnectsRandomBotFromGame]          = "Disconnects one random bot from game.";
		m_strings[LocalizedStringID_SelectsBotWeaponMode]                  = "Selects bot weapon mode.";
		m_strings[LocalizedStringID_DisplaysBotsProfilesDatabase]          = "Displays the bots profiles database.";
		m_strings[LocalizedStringID_DisplaysBotsCommandMenu]               = "Displays bots command menu.";
		m_strings[LocalizedStringID_DisplaysBotsListCurrentlyPlaying]      = "Displays bots list currently playing.";
		m_strings[LocalizedStringID_ExecutesSpecificCommandBySpecifiedBot] = "Executes specific command by specified bot.";
		m_strings[LocalizedStringID_DisplaysCurrentTimeOnServer]           = "Displays current time on server.";

	// Navigation mesh -------------------------------------------------------------------------------------------------------------
	m_strings[LocalizedStringID_NavigationMesh_DisplaysConsoleCommandsContainer] = "Displays main navigation mesh console commands container.";
		m_strings[LocalizedStringID_NavigationMesh_Generate]                     = "Generate a Navigation Mesh for the current map and save it to disk.";
		m_strings[LocalizedStringID_NavigationMesh_Analyze]                      = "Re-analyze the current Navigation Mesh and save it to disk.";
		m_strings[LocalizedStringID_NavigationMesh_Save]                         = "Saves the current Navigation Mesh to disk.";
		m_strings[LocalizedStringID_NavigationMesh_Load]                         = "Loads the Navigation Mesh for the current map.";
		m_strings[LocalizedStringID_NavigationMesh_ReloadPlaceDatabase]          = "Reloads the place names from Places.db file.";
		m_strings[LocalizedStringID_NavigationMesh_PrintPlaceList]               = "Lists all place names used in the map.";
		m_strings[LocalizedStringID_NavigationMesh_TogglePlaceMode]              = "Toggle the editor into and out of Place mode. Place mode allows labelling of Area with Place names.";
		m_strings[LocalizedStringID_NavigationMesh_MakeSniperSpots]              = "Chops the marked area into disconnected sub-areas suitable for sniper spots.";
		m_strings[LocalizedStringID_NavigationMesh_Transient]                    = "Toggles the 'area is transient and may become blocked' flag used by the AI system.";
		m_strings[LocalizedStringID_NavigationMesh_Delete]                       = "Deletes the currently highlighted Area.";
		m_strings[LocalizedStringID_NavigationMesh_TogglePlacePainting]          = "Toggles Place Painting mode. When Place Painting, pointing at an Area will 'paint' it with the current Place.";
		m_strings[LocalizedStringID_NavigationMesh_PrintAllPlaces]               = "Lists all place names loaded from Places.db file.";
		m_strings[LocalizedStringID_NavigationMesh_UsePlace]                     = "If used without arguments, all available Places will be listed. If a Place argument is given, the current Place is set.";
		m_strings[LocalizedStringID_NavigationMesh_PlaceReplace]                 = "Replaces all instances of the first place with the second place.";
		m_strings[LocalizedStringID_NavigationMesh_Mark]                         = "Marks the Area or Ladder under the cursor for manipulation by subsequent editing commands.";
		m_strings[LocalizedStringID_NavigationMesh_Draw]                         = "Draw navigation mesh.";
		m_strings[LocalizedStringID_NavigationMesh_PrintBSPEntities]             = "Print all entities from current map BSP file.";
		m_strings[LocalizedStringID_NavigationMesh_CreateWalkFacesBMPFile]       = "Create walk faces bitmap file.";
		m_strings[LocalizedStringID_NavigationMesh_CreateNavigationAreasBMPFile] = "Create navigation areas bitmap file.";

	// Waypoints -------------------------------------------------------------------------------------------------------------------
	m_strings[LocalizedStringID_DisplaysWaypointConsoleCommandsContainer] = "Displays main waypoint console commands container.";
		m_strings[LocalizedStringID_TogglesAutowppointing] = "Toggles autowppointing.";

	// Other -----------------------------------------------------------------------------------------------------------------------
	//

	// Menus....

	// Bot -------------------------------------------------------------------------------------------------------------------------
	// Main YaPB menu.
	m_strings[LocalizedStringID_Menu_Bot_Main_Title] = "YaPB Main Menu";
		m_strings[LocalizedStringID_Menu_Bot_Main_Item_Control]    = "YaPB Control";
		m_strings[LocalizedStringID_Menu_Bot_Main_Item_Features]   = "Features";
		m_strings[LocalizedStringID_Menu_Bot_Main_Item_FillServer] = "Fill Server";
		m_strings[LocalizedStringID_Menu_Bot_Main_Item_EndRound]   = "End Round";

	// Bot features menu.
	m_strings[LocalizedStringID_Menu_Bot_Features_Title] = "YaPB Features";
		m_strings[LocalizedStringID_Menu_Bot_Features_Item_WeaponModeMenu]    = "Weapon Mode Menu";
		m_strings[LocalizedStringID_Menu_Bot_Features_Item_WaypointMenu]      = "Waypoint Menu";
		m_strings[LocalizedStringID_Menu_Bot_Features_Item_SelectPersonality] = "Select Personality";
		m_strings[LocalizedStringID_Menu_Bot_Features_Item_ToggleDebugMode]   = "Toggle Debug Mode";
		m_strings[LocalizedStringID_Menu_Bot_Features_Item_CommandMenu]       = "Command Menu";

	// Bot control menu.
	m_strings[LocalizedStringID_Menu_Bot_Control_Title] = "Control Menu";
		m_strings[LocalizedStringID_Menu_Bot_Control_Item_AddBotQuick]     = "Add a Bot, Quick";
		m_strings[LocalizedStringID_Menu_Bot_Control_Item_AddBotSpecified] = "Add a Bot, Specified";
		m_strings[LocalizedStringID_Menu_Bot_Control_Item_KickRandomBot]   = "Kick Random Bot";
		m_strings[LocalizedStringID_Menu_Bot_Control_Item_KickAllBots]     = "Kick All Bots";
		m_strings[LocalizedStringID_Menu_Bot_Control_Item_KickBotMenu]     = "Kick Bot Menu";

	// Bot weapon mode select menu.
	m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Title] = "YaPB Weapon Mode";
		m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_KnivesOnly]        = "Knives only";
		m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_PistolsOnly]       = "Pistols only";
		m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_ShotgunsOnly]      = "Shotguns only";
		m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_MachineGunsOnly]   = "Machine Guns only";
		m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_RiflesOnly]        = "Rifles only";
		m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_SniperWeaponsOnly] = "Sniper Weapons only";
		m_strings[LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_AllWeapons]        = "All Weapons";

	// Bot personality select menu.
	m_strings[LocalizedStringID_Menu_Bot_PersonalitySelect_Title] = "YaPB Personality";
		m_strings[LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Normal]     = "Normal";
		m_strings[LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Aggressive] = "Aggressive";
		m_strings[LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Careful]    = "Careful";
		m_strings[LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Random]     = "Random";

	// Bot skill select menu.
	m_strings[LocalizedStringID_Menu_Bot_SkillSelect_Title] = "YaPB Skill Level";
		m_strings[LocalizedStringID_Menu_Bot_SkillSelect_Item_Stupid]       = "Stupid (1-20)";
		m_strings[LocalizedStringID_Menu_Bot_SkillSelect_Item_Newbie]       = "Newbie (20-40)";
		m_strings[LocalizedStringID_Menu_Bot_SkillSelect_Item_Average]      = "Average (40-60)";
		m_strings[LocalizedStringID_Menu_Bot_SkillSelect_Item_Advanced]     = "Advanced (60-80)";
		m_strings[LocalizedStringID_Menu_Bot_SkillSelect_Item_Professional] = "Professional (80-99)";
		m_strings[LocalizedStringID_Menu_Bot_SkillSelect_Item_Godlike]      = "Godlike (100)";

	// Bot team select menu.
	m_strings[LocalizedStringID_Menu_Bot_TeamSelect_Title] = "Select a team";
		m_strings[LocalizedStringID_Menu_Bot_TeamSelect_Item_Terrorist]        = "Terrorist Force";
		m_strings[LocalizedStringID_Menu_Bot_TeamSelect_Item_CounterTerrorist] = "Counter-Terrorist Force";

	// Bot terrorist/counter-terrorist model select menu title.
	m_strings[LocalizedStringID_Menu_Bot_ModelSelect_Title] = "Select an appearance";
		// Bot terrorist model select menu items.
		m_strings[LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Phoenix]  = "Phoenix Connexion";
		m_strings[LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Leet]     = "L337 Krew";
		m_strings[LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Arctic]   = "Arctic Avengers";
		m_strings[LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Guerilla] = "Guerilla Warfare";
		// Bot counter-terrorist model select menu items.
		m_strings[LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_Urban] = "Seal Team 6 (DEVGRU)";
		m_strings[LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GSG9]  = "German GSG-9";
		m_strings[LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_SAS]   = "UK SAS";
		m_strings[LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GIGN]  = "French GIGN";

	// Bot kick menu title.
	m_strings[LocalizedStringID_Menu_Bot_Kick_Title] = "YaPB Kick Menu";

	// Bot command menu.
	m_strings[LocalizedStringID_Menu_Bot_Command] = "Bot Command Menu";
		m_strings[LocalizedStringID_Menu_Bot_Command_Item_MakeDoubleJump]   = "Make Double Jump";
		m_strings[LocalizedStringID_Menu_Bot_Command_Item_FinishDoubleJump] = "Finish Double Jump";
		m_strings[LocalizedStringID_Menu_Bot_Command_Item_DropC4Bomb]       = "Drop the C4 Bomb";
		m_strings[LocalizedStringID_Menu_Bot_Command_Item_DropWeapon]       = "Drop the Weapon";

	// Waypoints -------------------------------------------------------------------------------------------------------------------
	// Waypoint main menu.
	m_strings[LocalizedStringID_Menu_Waypoint_Main_Title] = "Waypoint Operations";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_ToggleWaypoints]     = "Show/Hide waypoints";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_CacheWaypoint]       = "Cache waypoint";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_CreatePath]          = "Create path";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_DeletePath]          = "Delete path";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_AddWaypoint]         = "Add waypoint";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_DeleteWaypoint]      = "Delete waypoint";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_SetAutopathDistance] = "Set Autopath Distance";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_SetRadius]           = "Set Radius";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_WaypointStats]       = "Waypoint stats";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_ToggleAutoWaypoint]  = "Auto-waypoint on/off";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_SetFlags]            = "Set flags";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_SaveWaypoints]       = "Save waypoints";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_SaveWithoutChecking] = "Save without checking";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_LoadWaypoints]       = "Load waypoints";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_CheckWaypoints]      = "Check waypoints";
		m_strings[LocalizedStringID_Menu_Waypoint_Main_Item_ToggleNoclipCheat]   = "Noclip cheat on/off";

	// Waypoint radius select menu.
	m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Title] = "Waypoint Set Radius";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_0]   = "Radius 0.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_8]   = "Radius 8.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_16]  = "Radius 16.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_32]  = "Radius 32.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_48]  = "Radius 48.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_64]  = "Radius 64.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_80]  = "Radius 80.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_96]  = "Radius 96.0";
		m_strings[LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_128] = "Radius 128.0";

	// Waypoint add menu.
	m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Title] = "Waypoint Set Type";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Normal]                    = "Normal";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_TerroristImportant]        = "Terrorist Important";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CounterTerroristImportant] = "Counter-Terrorist Important";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_BlockWithHostageOrLadder]  = "Block with hostage / Ladder";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_RescueZone]                = "Rescue Zone";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Camping]                   = "Camping";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CampEnd]                   = "Camp End";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_MapGoal]                   = "Map Goal";
		m_strings[LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Jump]                      = "Jump";

	// Waypoint flag set menu.
	m_strings[LocalizedStringID_Menu_Waypoint_FlagSet_Title] = "Toggle waypoint Flags";
		m_strings[LocalizedStringID_Menu_Waypoint_FlagSet_Item_BlockWithHostageOrLadder] = "Block with Hostage";
		m_strings[LocalizedStringID_Menu_Waypoint_FlagSet_Item_TerroristSpecific]        = "Terrorists Specific";
		m_strings[LocalizedStringID_Menu_Waypoint_FlagSet_Item_CounterTerroristSpecific] = "Counter-Terrorists Specific";
		m_strings[LocalizedStringID_Menu_Waypoint_FlagSet_Item_UseElevator]              = "Use Elevator";
		m_strings[LocalizedStringID_Menu_Waypoint_FlagSet_Item_SniperPoint]              = "Sniper Point (\\yFor Camp Points Only!\\w)";

	// Waypoint auto-path maximum distance set.
	m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Title] = "Auto Path Distance";
		m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_0]   = "Distance 0.0";
		m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_100] = "Distance 100.0";
		m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_130] = "Distance 130.0";
		m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_160] = "Distance 160.0";
		m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_190] = "Distance 190.0";
		m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_220] = "Distance 220.0";
		m_strings[LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_250] = "Distance 250.0 (Default)";

	// Waypoint path connections set.
	m_strings[LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Title] = "Create Path (Choose Direction)";
		m_strings[LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Outgoing]      = "Outgoing Path";
		m_strings[LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Incoming]      = "Incoming Path";
		m_strings[LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Bidirectional] = "Bidirectional (Both Ways)";

	m_strings[LocalizedStringID_Menu_Item_Next]     = "Next";
	m_strings[LocalizedStringID_Menu_Item_Previous] = "Previous";
	m_strings[LocalizedStringID_Menu_Item_Exit]     = "Exit";
}

const bool Localizer::LoadConfigurationFile (const LanguageName_t &languageName)
{
	// LOCALIZER INITITALIZATION

	STDIOFile file;
	const DynamicString fullFilename (languageName + "_" LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION);

	AddLogEntry (true, LogLevel_Default, false, "Trying to open configuration file %s for language \"%s\"....", fullFilename.GetData (), languageName.GetData ());

	// check is file is exists for this language - opens file readonly
	if (!file.Open (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Configs/Languages/" + fullFilename))
	{
		AddLogEntry (true, LogLevel_Error, false, "Specified language configuration file not found.");

		return false;
	}
/*
	// opens file readonly
	if (!OpenConfigurationFile (LOCALIZER_CONFIG_FILENAME, "Specified language configuration file not found.", file))
		return false;
*/
	DynamicString line;
	unsigned int lineIndex (0u);
	unsigned short loadedTranslationsNumber (0u);
	bool isParsed[LocalizedStringID_Total];
/*	struct Item_t
	{
		LocalizedStringID_t id;
		DynamicString       name;
	};
	typedef DynamicArray <Item_t> ItemArray_t;
	ItemArray_t remainingLocalizedStringIDs =
	{
		{LocalizedStringID_UnableToCreateYaPB_Reason, "UnableToCreateYaPB_Reason"},
		...
	};
*/
	for (/* Empty */; loadedTranslationsNumber < LocalizedStringID_Total; ++loadedTranslationsNumber)
		isParsed[loadedTranslationsNumber] = false;

	loadedTranslationsNumber = 0u;

	// Reads line per line
	while (file.ReadLine (line, 512u))
	{
		++lineIndex;

		line.Trim ();	// Trim all the blanks or linefeeds.

		// skip all comment lines or empty lines
		if (IsEmptyOrCommentedFileBufferLine (line))
			continue;

		DynamicArray <DynamicString, unsigned char> pairs (line.Split <unsigned char> ('='));

		if (pairs.GetElementNumber () != 2u)
		{
			AddLogEntry (true, LogLevel_Error, false, "Localizer configuration file is not valid on a line: %u! Line should contain title and it's localization.", lineIndex);

			continue;
		}

		// trim all the blanks or linefeeds
		pairs.GetFirstElement ().Trim ();
/*
		ItemArray_t::IndexType_t index (0u);

		for (/* Empty *//*; index < remainingLocalizedStringIDs.GetElementNumber (); ++index)
			if (pairs.GetFirstElement ().CompareWithoutCaseCheck (remainingLocalizedStringIDs[index].name) == 0)
			{
				if (isParsed[remainingLocalizedStringIDs[index].id])
				{
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has been already parsed!", remainingLocalizedStringIDs[index].name.GetData (), languageName.GetData (), lineIndex);

					continue;
				}

				// Trim all the blanks or linefeeds.
				pairs.GetLastElement ().Trim ();

				if (pairs.GetLastElement ().IsEmpty ())
				{
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has no translation, using default string!", remainingLocalizedStringIDs[index].name.GetData (), languageName.GetData (), lineIndex);

					continue;
				}

				m_strings[remainingLocalizedStringIDs[index].id] = pairs.GetLastElement ();

				isParsed[remainingLocalizedStringIDs[index].id] = true;

				remainingLocalizedStringIDs.Remove (index, 1u);

				++loadedTranslationsNumber;

				continue;
			}

		if (index == remainingLocalizedStringIDs.GetElementNumber ())	/// @warning MAYBE WRONG!!!
			AddLogEntry (true, LogLevel_Error, false, "Entry in %s_" LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", languageName.GetData (), lineIndex);
*/
		#define PARSE_LOCALIZED_STRING_ID(localizedStringID)																																																											\
			if (/*SOME SPEEDUP INSTEAD OF INFORMATING - !isParsed[localizedStringID] && */pairs.GetFirstElement ().CompareWithoutCaseCheck (#localizedStringID + GET_STATIC_STRING_LENGTH ("LocalizedStringID_")) == 0)																					\
			{																																																																							\
				if (isParsed[localizedStringID])																																																														\
				{																																																																						\
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has been already parsed! - Ignoring....", #localizedStringID + GET_STATIC_STRING_LENGTH ("LocalizedStringID_"), languageName.GetData (), lineIndex);		\
																																																																										\
					continue;																																																																			\
				}																																																																						\
																																																																										\
				/* Trim all the blanks or linefeeds. */																																																													\
				pairs.GetLastElement ().Trim ();																																																														\
																																																																										\
				if (pairs.GetLastElement ().IsEmpty ())																																																													\
				{																																																																						\
					AddLogEntry (true, LogLevel_Error, false, "\"%s\" entry in %s_" LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u has no translation, using default string!", #localizedStringID + GET_STATIC_STRING_LENGTH ("LocalizedStringID_"), languageName.GetData (), lineIndex);	\
																																																																										\
					continue;																																																																			\
				}																																																																						\
																																																																										\
				m_strings[localizedStringID] = pairs.GetLastElement ();																																																									\
																																																																										\
				isParsed[localizedStringID] = true;	/* Mark as parsed.... */																																																							\
																																																																										\
				++loadedTranslationsNumber;																																																																\
																																																																										\
				continue;																																																																				\
			}

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_UnableToCreateYaPB_Reason);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_BothTeamsReachedMaximumPlayers);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_MaximumPlayersReached);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_InvalidSkillGivenUsingRandomSkill);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ConnectingBot);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_FillServerWithBots);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_YaPBs);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Clients);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Humans);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_FakeClients);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_AllKickedFromServer);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_YaPBKicked);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_AllKilled);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_BotsHealthIsSetTo);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ThereAreNoYaPBOnTheServer);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_WeaponModeSelected);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ChooseWeaponFrom1To7Range);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_PleaseSpecifyOrderCommand);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_PlayerIsNotYaPB);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_AllBotsHasExecutedCommand);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_BotHasExecutedCommand);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_BotWithThatUserIDNotFound);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_BotWithThatNameNotFound);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_CurrentTimeIs);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_CurrentDateIs);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_PleaseSpecifyHealth);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_YouDeadAndHaveNoAccessToThisMenu);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ExecutingMainConfigurationFile);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NoMainConfigurationFileFound);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ExecutingMapSpecificConfigurationFile);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NoMapSpecificConfigurationFileFound);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_PlayerHadLostRemoteAccessToYaPB);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_PlayerHadGainedFullRemoteAccessToYaPB);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_CommandNotSupportedOnDedicatedServer);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_AutoSelect);

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_MOTD_Title_BotsList);

		// Waypoints....
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_MapNotWaypointed);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_WaypointsHasBeenChanged);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_PathAlreadyExists);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_PathAddedFromTo);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ThisIsNotCampingWaypoint);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_CannotConnectWaypointToItself);

		// Navigation mesh...
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_NotExits);

		// Console variables....
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_AutoVacate);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Quota);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_QuotaMode);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_MinSkill);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_MaxSkill);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_FollowUser);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_TimerSound);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_TimerPickup);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_TimerGrenade);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_DebugGoal);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Chat);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Synth);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_KnifeMode);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_SkillTags);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Stop);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_ThruWalls);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_WallSensitivity);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Votes);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Spray);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_BotBuy);

		#if defined _DEBUG
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Debug);
		#endif	// if defined _DEBUG

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Prefix);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Language);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_EnableClientLanguages);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Version);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_NavPointLookMethod);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_AimMethod);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_TargetingMethod);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_DamperCoeff_X);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_DamperCoeff_Y);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Deviation_X);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Deviation_Y);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Influence_XonY);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Influence_YonX);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_SlowdownRatio);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_OffsetDelay);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_SpringStiff_X);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_SpringStiff_Y);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_AnticipRatio);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Password);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_PasswordKey);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_ChatterPath);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_TKPunish);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Communication);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Economics);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_ForceTeam);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_RestrictedWeapons);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_DangerFactor);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_DontShoot);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_HardcoreMode);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_AutofollowPercent);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_ExplosiveGrenadeUsePercent);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_FlashBangGrenadeUsePercent);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_SmokeGrenadeUsePercent);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_LogoTypes);

		#if defined MULTIPLE_MSEC_METHODS
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_MsecMethod);
		#endif	// if defined MULTIPLE_MSEC_METHODS

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_LogLevel);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Config_LoadTime);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_WalkAllowed);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Deathmatch);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_ThinkFPS);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Ping);		// The ping you want displayed (min: 0, max: 4095).
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_PingFlux);	// Fake ping fluctuation amount (0 = none).
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_DeferToHuman);

		// Voice system....
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_inputfromfile);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_recordtofile);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_avggain);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_maxgain);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_scale);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_loopback);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_fadeouttime);		// It fades to no sound at the tail end of your voice data when you release the key.

		// Debugging cvars.
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_profile);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_showchannels);		// 1 = list channels, 2 = show timing info, etc
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_showincoming);		// show incoming voice data

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_forcemicrecord);	// Have it force your mixer control settings so waveIn comes from the microphone. CD rippers change your waveIn to come from the CD drive

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_overdrive);			// When voice is on, all other sounds are decreased by this factor.
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_Voice_overdrivefadetime);	// How long it takes to fade in and out of the voice overdrive.

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_MaximumAFKTime);

		#if defined MULTIPLE_DRAW_METHODS
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_DrawMethod);
		#endif	// if defined MULTIPLE_DRAW_METHODS

		/////////////////////////////////////////////////////////////////
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_edit);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_quicksave);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_hiding_spots);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_approach_points);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_danger);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_player_counts);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_occupy_time);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_battlefront);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_area_info);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_snap_to_grid);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_create_place_on_ground);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_nodes);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_compass);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_slope_limit);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_slope_tolerance);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_restart_after_analysis);

		#if defined DEBUG_NAV_NODES
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_show_node_id);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_test_node);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_test_node_crouch);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_test_node_crouch_dir);
		#endif	// if defined DEBUG_NAV_NODES

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_coplanar_slope_limit);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_split_place_on_ground);
		#if defined MULTIPLE_DRAW_METHODS
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_area_bgcolor);
		#endif	// if defined MULTIPLE_DRAW_METHODS
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_area_max_size);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_nav_corner_adjust_adjacent);

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_NavigationMesh_MaximumOffPlaneTolerance);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_NavigationMesh_MaximumSampleStepTime);

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_NavigationMesh_GenerateFenceTopsAreas);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Variable_NavigationMesh_SquareUpAreas);

		// Console commands....

		// Main ------------------------------------------------------------------------------------------------------------------------
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ConsoleCommandCanOnlyBeExecutedFromServerConsole);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedFromServerConsole);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ConsoleVariableCanBeOnlySetViaRCONAccess);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ClientHasExecutedConsoleVariableCanBeOnlySetViaRCONAccess);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedWhenServerIsRunning);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_UnknownConsoleCommand);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ClientHasExecutedUnknownConsoleCommand);

		// Localizer -------------------------------------------------------------------------------------------------------------------
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ConsoleCommand_Localizer);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ConsoleCommand_Localizer_Reload);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ConsoleCommand_Localizer_LoadOriginalStrings);

		// Bot -------------------------------------------------------------------------------------------------------------------------
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisplaysBotConsoleCommandsContainer);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisplaysVersionInformation);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ShowsAboutBotInformation);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_FillsServerWithRandomBots);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisconnectsAllBotsFromCurrentGame);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_KillsAllBotsInCurrentGame);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_CreatesBotInCurrentGame);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisconnectsRandomBotFromGame);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_SelectsBotWeaponMode);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisplaysBotsProfilesDatabase);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisplaysBotsCommandMenu);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisplaysBotsListCurrentlyPlaying);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_ExecutesSpecificCommandBySpecifiedBot);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisplaysCurrentTimeOnServer);

		// Navigation mesh -------------------------------------------------------------------------------------------------------------
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_DisplaysConsoleCommandsContainer);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Generate);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Analyze);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Save);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Load);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_ReloadPlaceDatabase);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_PrintPlaceList);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_TogglePlaceMode);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_MakeSniperSpots);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Transient);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Delete);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_TogglePlacePainting);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_PrintAllPlaces);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_UsePlace);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_PlaceReplace);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Mark);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_Draw);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_PrintBSPEntities);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_CreateWalkFacesBMPFile);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_NavigationMesh_CreateNavigationAreasBMPFile);

		// Waypoints -------------------------------------------------------------------------------------------------------------------
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_DisplaysWaypointConsoleCommandsContainer);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_TogglesAutowppointing);

		// Other -----------------------------------------------------------------------------------------------------------------------
		//

		// Menus....

		// Bot -------------------------------------------------------------------------------------------------------------------------
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Main_Title);	// Main YaPB menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Main_Item_Control);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Main_Item_Features);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Main_Item_FillServer);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Main_Item_EndRound);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Features_Title);	// Bot features menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Features_Item_WeaponModeMenu);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Features_Item_WaypointMenu);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Features_Item_SelectPersonality);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Features_Item_ToggleDebugMode);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Features_Item_CommandMenu);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Control_Title);	// Bot control menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Control_Item_AddBotQuick);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Control_Item_AddBotSpecified);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Control_Item_KickRandomBot);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Control_Item_KickAllBots);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Control_Item_KickBotMenu);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Title);	// Bot weapon mode select menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_KnivesOnly);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_PistolsOnly);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_ShotgunsOnly);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_MachineGunsOnly);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_RiflesOnly);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_SniperWeaponsOnly);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_WeaponModeSelect_Item_AllWeapons);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_PersonalitySelect_Title);	// Bot personality select menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Normal);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Aggressive);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Careful);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_PersonalitySelect_Item_Random);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_SkillSelect_Title);	// Bot skill select menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_SkillSelect_Item_Stupid);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_SkillSelect_Item_Newbie);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_SkillSelect_Item_Average);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_SkillSelect_Item_Advanced);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_SkillSelect_Item_Professional);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_SkillSelect_Item_Godlike);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_TeamSelect_Title);	// Bot team select menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_TeamSelect_Item_Terrorist);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_TeamSelect_Item_CounterTerrorist);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_ModelSelect_Title);	// Bot terrorist model select menu.
			// Bot terrorist model select menu items.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Phoenix);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Leet);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Arctic);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_TerroristModelSelect_Item_Guerilla);
			// Bot counter-terrorist model select menu items.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_Urban);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GSG9);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_SAS);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_CounterTerroristModelSelect_Item_GIGN);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Kick_Title);	// Bot kick menu.
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Command);	// Bot command menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Command_Item_MakeDoubleJump);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Command_Item_FinishDoubleJump);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Command_Item_DropC4Bomb);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Bot_Command_Item_DropWeapon);

		// Waypoints ------------------------------------------------------------------------------------------------------------------
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Title);	// Waypoint main menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_ToggleWaypoints);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_CacheWaypoint);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_CreatePath);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_DeletePath);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_AddWaypoint);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_DeleteWaypoint);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_SetAutopathDistance);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_SetRadius);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_WaypointStats);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_ToggleAutoWaypoint);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_SetFlags);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_SaveWaypoints);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_SaveWithoutChecking);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_LoadWaypoints);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_CheckWaypoints);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_Main_Item_ToggleNoclipCheat);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Title);	// Waypoint radius select menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_0);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_8);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_16);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_32);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_48);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_64);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_80);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_96);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_RadiusSelect_Item_128);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Title);	// Waypoint add menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Normal);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_TerroristImportant);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CounterTerroristImportant);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_BlockWithHostageOrLadder);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_RescueZone);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Camping);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_CampEnd);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_MapGoal);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_TypeSelect_Item_Jump);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_FlagSet_Title);	// Waypoint flag set menu.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_FlagSet_Item_BlockWithHostageOrLadder);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_FlagSet_Item_TerroristSpecific);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_FlagSet_Item_CounterTerroristSpecific);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_FlagSet_Item_UseElevator);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_FlagSet_Item_SniperPoint);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Title);	// Waypoint auto-path maximum distance set.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_0);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_100);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_130);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_160);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_190);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_220);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_AutoPathMaximumDistanceSet_Item_250);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Title);	// Waypoint path connections set.
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Outgoing);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Incoming);
			PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Waypoint_PathConnectionsSet_Item_Bidirectional);

		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Item_Next);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Item_Previous);
		PARSE_LOCALIZED_STRING_ID (LocalizedStringID_Menu_Item_Exit)

		else
			AddLogEntry (true, LogLevel_Error, false, "Entry in %s_" LOCALIZER_CONFIG_FILENAME_WITH_EXTENTION ", on a line: %u is unknown!", languageName.GetData (), lineIndex);
	}

	if (loadedTranslationsNumber > 0u)
		m_currentLanguageName = languageName;

	AddLogEntry (true, LogLevel_Default, false, "%u of %u translations loaded.", loadedTranslationsNumber, LocalizedStringID_Total);

	return true;
}