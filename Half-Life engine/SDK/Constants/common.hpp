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
// Common.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED

// useful cosines
#define DOT_1DEGREE   0.9998476951564f
#define DOT_2DEGREE   0.9993908270191f
#define DOT_3DEGREE   0.9986295347546f
#define DOT_4DEGREE   0.9975640502598f
#define DOT_5DEGREE   0.9961946980917f
#define DOT_6DEGREE   0.9945218953683f
#define DOT_7DEGREE   0.9925461516413f
#define DOT_8DEGREE   0.9902680687416f
#define DOT_9DEGREE   0.9876883405951f
#define DOT_10DEGREE  0.9848077530122f
#define DOT_15DEGREE  0.9659258262891f
#define DOT_20DEGREE  0.9396926207859f
#define DOT_25DEGREE  0.9063077870367f
#define DOT_30DEGREE  0.866025403784f

#define CMD_MAXBACKUP 30u

// Hud Element hiding flags
DECLARE_TYPED_ENUMERATION (unsigned int, HideHUDBit_t)
{
	HideHUDBit_None,

	HIDEHUD_WEAPONS           = BIT (0u),	// Hide viewmodel, ammo count & weapon selection (crosshair, ammo, weapons list)
	HIDEHUD_FLASHLIGHT        = BIT (1u),	// (flashlight, +)
	HIDEHUD_ALL               = BIT (2u),	// (ALL)
	HIDEHUD_HEALTH            = BIT (3u),	// Hide health & armor / suit battery (radar, health, armor, +)

	HIDEHUD_TIMER             = BIT (4u),	// (timer, +)
	HIDEHUD_MONEY             = BIT (5u),	// (money, +)
	HIDEHUD_CROSSHAIR         = BIT (6u),	// (crosshair)
	HIDEHUD_DEFAULT_CROSSHAIR = BIT (7u)	// An additional crosshair will be drawn. That crosshair looks exactly like the one from "Crosshair" message.
};

enum StatusIcon_StatusType_t
{
	StatusIcon_StatusType_Hide,	// Disable/Draw icon
	StatusIcon_StatusType_Show,	// Enable/Remove icon

	StatusIcon_StatusType_Flash	// Blink icon
};

enum GroupOpType_t
{
	GROUP_OP_AND,	// And.
	GROUP_OP_NAND	// Not and.
};

enum HUDMessageChannel_t
{
	HUDMessageChannel_AutoChoose = -1,

	HUDMessageChannel_0,
	HUDMessageChannel_1,
	HUDMessageChannel_2,
	HUDMessageChannel_3,
	HUDMessageChannel_4
};
enum HUDMessageEffect_t
{
	HUDMessageEffect_None,	// No effect

	HUDMessageEffect_1,	// Flicker with 2nd color.
	HUDMessageEffect_2	// Print out as 2nd color, fade into 1st color. 'effectTime' decides fade time between colors. 'fadeInTime' decides how fast the letters should be printed out.
};

// constant items
enum PlayerItemType_t
{
	PlayerItemType_HealthKit = 1u,
	PlayerItemType_Antidote,
	PlayerItemType_Security,
	PlayerItemType_Battery,

	PlayerItemType_Total	// hard coded item types
};

const unsigned char BombDefuseTime[] =
{
	10u,	// Without defuse kit.
	5u		// With defuse kit.
};

enum CommonConstant_t
{
	WorldspawnEntitySlotIndex              = 0u,
	ListenServerHostClientSlotIndex        = 0u,	// Slot index of listen server host client. (Not edict index, but array!)
	ListenServerHostClientEdictSlotIndex   = 1u,	// Slot index of listen server host client edict.
	MaximumPlayableTeamsNumber             = 2u,	// maximum number of playable teams in Counter-Strike
	MaximumItemTypes                       = 4u,
	MaximumSeenChatMessages                = 4u,	// maximum number of chat messages that can be seen on the screen at once in Counter-Strike
	MaximumPlayerModels                    = 4u,	// maximum number of player models in Counter-Strike
	MaximumPlayerModels_ConditionZero      = 5u,	// maximum number of player models in Counter-Strike: Condition Zero
	MaximumPlayerItemTypes                 = 6u,	// hud item selection slots
	MaximumHostagesNumber                  = 8u,
	MaximumUserMessageNameLength           = 12u,
	MaximumTeamNameLength                  = 16u,
	MaximumWeaponPositions                 = 20u,	// max number of items within a slot
	MaximumClientNameLength                = 32u,	// maximum client name length (a player name may have 31 chars + '\0')
	MaximumPlayerEquip                     = 32u,
	MaximumLogoNameLength                  = 32u,	// maximum logo name length
	MaximumWeapons                         = 32u,	// Max number of weapons available
	MaximumClients                         = 32u,	// maximum clients number on the server.

	MaximumEntityLeafs                     = 48u,
	HealthChargerRechargeTime              = 60u,
	MaximumMOTDChunk                       = 60u,
	MaximumPathNameLength                  = 64u,	// maximum length of a quake game pathname
	MaximumPlayerUseDistance               = 64u,
	MaximumPlayerUseDistanceSquared        = MATH_GET_SQUARED (MaximumPlayerUseDistance),
	MaximumMoveEntities                    = 64u,
	MaximumCommandArgumens                 = 80u,
	MaximumCommandArgumenLength            = 516u,
	DefaultPlayerFieldOfView               = 90u,
	MaximumVoteMapListLength               = 100u,
	MaximumIntermissionTime                = 120u,	// longest the intermission can last, in seconds
	MaximumSprayLogoDistance               = 128u,
	InterfaceVersion                       = 140u,	// Actual engine interface version.
	MaximumPlayerWalkSpeed                 = 150u,	// maximum player walk speed (without making footstep noises) in Counter-Strike
	MaximumTextMessageLength               = 180u,	// maximum buffer size for text messages (TextMsg)
	MaximumUserMessageSize                 = 192u,
	MaximumIlluminationValue               = 255u,	// (hardcoded in the engine)
	MaximumUserMessages                    = 256u,	// maximum number of user messages the engine can register (hardcoded in the engine)
	MaximumClientCommandLength             = 128u/*256u*/,	// maximum client command length
	MaximumDefaultPlayerSpeed              = 250u,	// Speed of player, holding knife, or pistol....
	MaximumPlayerSpeed                     = 260u,	// Speed of player, holding scout....
	MaximumPhysInfoStringLength            = 256u,
	MaximumInfoStringLength                = 256u,
	MaximumClientCommandArgumentLength     = 256u,
	MaximumLightStyleValue                 = 256u,
	MaximumClientHudMessageLength          = 480u,	// maximum client hud message length
	MaximumWeaponReloadingHearDistance     = 512u,

	MaximumMenuStringLength                = MaximumUserMessageSize - (sizeof (/*Constants::Menu::KeyBit_t*/unsigned short) + sizeof (char)/* lifeTime */ + sizeof (bool)/* needMore */),	// The length of part of full menu string per messages....
	MaximumMenuFullStringLength            = 512u,	// The length of full (splitted) menu string....

	MaximumPlayerSafeFallSpeed             = 580u,	// maximum player safe fall speed in Counter-Strike
	MaximumPhysEntities                    = 600u,	// Must have room for all entities in the world.
	MaximumServerCommandLength             = 1024u,	// maximum server command length
	MaximumAlertMessageLength              = 1024u,	// maximum alert message length (pfnAlertMessage())
	MaximumClientMessageLength             = 1312u/*3971u*/,	// maximum buffer size for client messages	// FOR PRINT CENTR MESSAGE SIZE SHOULD BE == 1312u (BUT PRINT ONLY 40u CHARACTERS), FOR CONSOLE == 3971u, FOR CHAT == 1312u.
	MaximumFlashRadiusLength               = 1500u,
	MaximumMOTDTextLength                  = 1536u,	// maximum MOTD text length (MaximumMOTDChunk * 4)
	MaximumHearingDistance                 = 3250u,	// maximum distance at which a sound can be heard in the virtual world
	MaximumServerMessageLength             = 4096u,	// maximum server message length

	PlayerMaximumSafeFallSpeed             = 580u/*500u == MP.DLL*/,	// Approx 20 feet
	PlayerFatalFallSpeed                   = 1024u/*980, ~1100u == MP.DLL*/,	// Approx 60 feet
	PlayerMaximumClimbableSpeed            = 200u,	// (ladder speed)
	PlayerFallPunchThreshhold              = 250u,	// Won't punch player's screen/make scrape noise unless player falling at least this fast.
	PlayerMinimumBounceSpeed               = 350u,

	PlayerMaximumMoneyAmount               = 16000u,

	/// @todo MAKE RIGHT VALUES FOR FOUR BOTTOM CONSTANTS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	MaximumHEGrenadeDamage                 = 250u,
	MaximumHEGrenadeDamageRadius           = 500u,
	MaximumFlashGrenadeDamage              = 250u,
	MaximumFlashGrenadeDamageRadius        = 500u,

	MaximumTextureNameLength               = 17u,	// only load first n chars of name
	MaximumTexturesNumber                  = 1024u,	// maximum number of textures loaded

	HalfMapSize                            = 4096u,	// need to know half (negative + positive halves = max)
	MapSize                                = HalfMapSize * 2u,

	MaximumPlayerWalkSpeedSquared          = MATH_GET_SQUARED (MaximumPlayerWalkSpeed)	// maximum player walk speed (without making footstep noises) in Counter-Strike (squared)
};

enum BSPFileConstant_t
{
	MAX_MAP_LIGHTS        = 4u,

	MAX_MAP_MODELS        = 400u,		// variable, but more would stress out the engine and network code
	MAX_MAP_TEXTURES      = 512u,
	MAX_MAP_BRUSHES       = 4096u,
	MAX_MAP_ENTITIES      = 4096u,
	MAX_MAP_LEAFS         = 8192u,
	MAX_MAP_TextureInfo   = 8192u,
	MAX_MAP_PLANES        = 32767u,		// more than this in a map and the engine will drop faces
	MAX_MAP_NODES         = 32767u,		// because negative shorts are contents
	MAX_MAP_CLIP_NODES    = 32767u,
	MAX_MAP_VERTS         = 65535u,		// hard limit (data structures store them as unsigned short)
	MAX_MAP_FACES         = 65535u,		// hard limit (data structures store them as unsigned short)
	MAX_MAP_MARK_SURFACES = 65535u,
	MAX_MAP_PORTALS       = 65536u,
	MAX_MAP_EDGES         = 256000u,	// arbitrary
	MAX_MAP_SURF_EDGES    = 512000u,	// arbitrary
	MAX_MAP_ENTSTRING     = 128u * MAX_MAP_ENTITIES,
	MAX_MAP_TEXTURE_LUMP  = 4194304u,
	MAX_MAP_LIGHTING      = 4194304u,
	MAX_MAP_VISIBILITY    = 2097152u,

	// key/value pair sizes
	MAX_KEY               = 32u,
	MAX_VALUE             = 4096u
};

const float MaximumTraceLength (MapSize * 2u * Math::sqrtf (3.0f));	// Maximum traceable distance (assumes cubic world and trace from one corner to opposite).

/// @todo CHECK ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const float SniperSightCircleRadius90 (35.05f);	/// @note FOV == 90. (FOV multiplier == 1.2857142857142857142857142857143)
const float SniperSightCircleRadius40 (14.24f);	/// @note FOV == 40. (FOV multiplier == 1.4044943820224719101123595505618)
const float SniperSightCircleRadius15 (5.24f);	/// @note FOV == 15. (FOV multiplier == 1.4312977099236641221374045801527)
const float SniperSightCircleRadius10 (3.49f);	/// @note FOV == 10. (FOV multiplier == 1.4326647564469914040114613180516)
const float SniperSightCircleFOVCommonMultiplierInverted (1.0f / 1.4228189494643758120870751495887f);	/// @note common FOV multiplier

const float TimeToDuck (0.4f);	// Ducking time

const float StepHeight (18.0f);			///< if delta Z is greater than this, we have to jump to get up.
const float JumpHeight (44.0f + 1.0f/* - FROM RACC BOT */);//41.8f);	///< if delta Z is less than this, we can jump up on it.	// MADE BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!!
const float JumpCrouchHeight (62.0f + 1.0f/* - FROM RACC BOT */);//58.0f);	///< (48) if delta Z is less than or equal to this, we can jumpcrouch up on it.	// MADE BY EDWARD!!!!!!!!!!!!!!!!!!!!!!!!

const float DeathDrop (200.0f);			///< (300) distance at which we will die if we fall - should be about 600, and pay attention to fall damage during pathfind.

const float HumanStandViewOffset (17.0f);

const float HalfHumanWidth (16.0f);
const float HumanWidth (HalfHumanWidth * 2.0f);
const float HumanDuckViewOffset (12.0f);
const float HalfDuckHumanHeight (18.0f);
const float DuckHumanHeight (HalfDuckHumanHeight * 2.0f);
const float HalfHumanHeight (DuckHumanHeight);
const float HumanHeight (HalfHumanHeight * 2.0f);

const float HumanDeadViewOffset (-8.0f);

const float StopEpsilon (0.1f);

const float JumpCrouchHeightsWithHelpers[] =
{
	JumpCrouchHeight,
	HumanHeight + JumpCrouchHeight,			// When some player helps us to reach some height.
	HumanHeight * 2.0f + JumpCrouchHeight	// When two some players helps us to reach some height.
};

const float GrenadeExplodeDelayTime (1.5f);	// Grenade delay time before explosion occurs.

enum C4_t
{
	DefaultC4ExplosionRadius = 500u
};
static const float ExplosionDamageMultiplier (3.5f);

static const float DamageForFallSpeed ((PlayerFatalFallSpeed - PlayerMaximumSafeFallSpeed) * 0.01f/*0.1666666716337204f * 1.25f == MP.DLL*/);	// Damage per unit per second.

static const float FlashlightChargeTime (0.2f);
static const float ChatIntervalTime (0.3f);
static const float RadioIntervalTime (1.5f);

// Dot products for view cone checking.
static const float VIEW_FIELD_FULL         (-1.0f);	// +-180 degrees
static const float VIEW_FIELD_WIDE         (-0.7f);	// +-135 degrees 0.1	// +-85 degrees, used for full FOV checks
static const float VIEW_FIELD_NARROW       (0.7f);	// +-45 degrees, more narrow check used to set up ranged attacks
static const float VIEW_FIELD_ULTRA_NARROW (0.9f);	// +-25 degrees, more narrow check used to set up ranged attacks

DECLARE_TYPED_ENUMERATION (unsigned char, RegularSequence_t)
{
	RegularSequence_Idle,

	RegularSequence_Shoot,
	RegularSequence_Reload,
	RegularSequence_Die          = BIT (2u),
	RegularSequence_Throw        = BIT (3u),
	RegularSequence_ArmC4        = BIT (4u),
	RegularSequence_ClosedShield = BIT (5u),
	RegularSequence_OpenShield   = BIT (6u)
};

// sequence info data
const RegularSequence_t RegularSequencesInformation[] =
{
	RegularSequence_Idle,                               RegularSequence_Idle,       RegularSequence_Idle,                               RegularSequence_Idle,                                RegularSequence_Idle,                               RegularSequence_Idle,                               RegularSequence_Idle,                                RegularSequence_Idle,         RegularSequence_Idle,                               RegularSequence_Idle,		//   0...9
	RegularSequence_Idle,                               RegularSequence_Shoot,      RegularSequence_Reload,                             RegularSequence_Idle,                                RegularSequence_Shoot,                              RegularSequence_Reload,                             RegularSequence_Idle,                                RegularSequence_Shoot,        RegularSequence_Reload,                             RegularSequence_Idle,		//  10...19
	RegularSequence_Shoot,                              RegularSequence_Reload,     RegularSequence_Idle,                               RegularSequence_Shoot,                               RegularSequence_Shoot,                              RegularSequence_Reload,                             RegularSequence_Idle,                                RegularSequence_Shoot,        RegularSequence_Shoot,                              RegularSequence_Reload,		//  20...29
	RegularSequence_Idle,                               RegularSequence_Shoot,      RegularSequence_Reload,                             RegularSequence_Idle,                                RegularSequence_Shoot,                              RegularSequence_Reload,                             RegularSequence_Idle,                                RegularSequence_Shoot,        RegularSequence_Reload,                             RegularSequence_Idle,		//  30...39
	RegularSequence_Shoot,                              RegularSequence_Reload,     RegularSequence_Idle,                               RegularSequence_Shoot,                               RegularSequence_Reload,                             RegularSequence_Idle,                               RegularSequence_Shoot,                               RegularSequence_Reload,       RegularSequence_Idle,                               RegularSequence_Shoot,		//  40...49
	RegularSequence_Reload,                             RegularSequence_Idle,       RegularSequence_Shoot,                              RegularSequence_Reload,                              RegularSequence_Idle,                               RegularSequence_Idle,                               RegularSequence_Idle,                                RegularSequence_Throw,        RegularSequence_Idle,                               RegularSequence_Throw,		//  50...59
	RegularSequence_Idle,                               RegularSequence_ArmC4,      RegularSequence_Idle,                               RegularSequence_ArmC4,                               RegularSequence_Idle,                               RegularSequence_Idle,                               RegularSequence_Shoot,                               RegularSequence_Shoot,        RegularSequence_Reload,                             RegularSequence_Idle,		//  60...69
	RegularSequence_Shoot,                              RegularSequence_Shoot,      RegularSequence_Reload,                             RegularSequence_Idle,                                RegularSequence_Shoot,                              RegularSequence_Idle,                               RegularSequence_Shoot,                               RegularSequence_Idle,         RegularSequence_Shoot,                              RegularSequence_Reload,		//  70...79
	RegularSequence_Idle,                               RegularSequence_Shoot,      RegularSequence_Reload,                             RegularSequence_OpenShield,                          RegularSequence_Throw | RegularSequence_OpenShield, RegularSequence_OpenShield,                         RegularSequence_Throw | RegularSequence_OpenShield,  RegularSequence_OpenShield,   RegularSequence_Shoot | RegularSequence_OpenShield, RegularSequence_OpenShield,	//  80...89
   	RegularSequence_Shoot | RegularSequence_OpenShield, RegularSequence_OpenShield, RegularSequence_Shoot | RegularSequence_OpenShield, RegularSequence_Reload | RegularSequence_OpenShield, RegularSequence_OpenShield,                         RegularSequence_Shoot | RegularSequence_OpenShield, RegularSequence_Reload | RegularSequence_OpenShield, RegularSequence_ClosedShield, RegularSequence_ClosedShield,                       RegularSequence_Die,		//  90...99
	RegularSequence_Die,                                RegularSequence_Die,        RegularSequence_Die,                                RegularSequence_Die,                                 RegularSequence_Die,                                RegularSequence_Die,                                RegularSequence_Die,                                 RegularSequence_Die,          RegularSequence_Die,                                RegularSequence_Die,		// 100...109
	RegularSequence_Die																																																																																																													// 110
};

enum GaitSequence_t
{
	GaitSequence_Dummy,			// dummy
	GaitSequence_Standing,		// idle1
	GaitSequence_Crouching,		// crouch_idle
	GaitSequence_Walking,		// walk
	GaitSequence_Running,		// run
	GaitSequence_CrouchRunning,	// crouchrun
	GaitSequence_Jumping,		// jump
	GaitSequence_LongJumping,	// longjump
	GaitSequence_Swimming		// swim
};

// Find by "World triggered \"Round_Start\"\n" string, pushed in UTIL_LogPrintf() from CHalfLifeMultiplay::Think() function. (Offset == 0)
const unsigned char signature_Calling_UTIL_LogPrintf_RoundStart[] = {0x2A, 0x2A, 0x2A, 0x2A, 0x6A, 0x03, 0x53, 0x89, 0x5D, 0x04, 0xFF, 0x15, 0x2A, 0x2A, 0x2A, 0x2A, 0x83, 0xC4, 0x0C, 0x2B, 0xC3};

// String, pushed in Con_Printf() from Cvar_Command() function.
const unsigned char signature_Calling_Con_Printf_VariableValueIs[] = {"\"%s\" is \"%s\"\n"};

// Find by "#Map_Vote_Extend" string, pushed in CHalfLifeMultiplay::DisplayMaps() function. (Offset == 0)
const unsigned char signature_mapCycleStructure[] = {0x2A, 0x2A, 0x2A, 0x2A, 0x33, 0xF6, 0x83, 0xC4, 0x04, 0x33, 0xED, 0x3B, 0xCE, 0x8B, 0xD8, 0x89, 0x74, 0x24, 0x14};

enum ClientCommandOffset_t
{/*! @note WORKS ALSO, BUT cstrike_amxx.dll MODULE CHANGES BYTES WHERE WE GET ADDRESS OF 'BotArgs', SO BELOW IS SOLUTION WITH DIFFERENT OFFSET ('CS_CLICMD_OFFS_BOTARGS')!!!
	CS_CLICMD_OFFS_USEBOTARGS = GET_STATIC_STRING_LENGTH ("\x8A\x15"),
	CS_CLICMD_OFFS_BOTARGS    = CS_CLICMD_OFFS_USEBOTARGS + GET_STATIC_STRING_LENGTH ("\xA0\x91\x12\x10\x81\xEC\x94\x00\x00\x00\x84\xD2\x53\x55\x56\x57\x74\x0C\x8B\x1D")
*/
	CS_CLICMD_OFFS_BOTARGS    = GET_STATIC_STRING_LENGTH ("\x8A\x15\xA0\x91\x12\x10\x81\xEC\x94\x00\x00\x00\x84\xD2\x53\x55\x56\x57\x74\x0C\x8B\x1D"),
	CS_CLICMD_OFFS_USEBOTARGS = CS_CLICMD_OFFS_BOTARGS + GET_STATIC_STRING_LENGTH ("\x90\x91\x12\x10\x89\x5C\x24\x14\xEB\x17\x6A\x00\xFF\x15\x8C\x24\x16\x10\x8A\x15")
};

// Radio and chat strings can have control characters embedded to set colors. For the control characters to be used, one must be at the start of the string.
// The control characters can be copied and pasted in notepad.
// This only works at the start of the string!
enum ChatColor_t
{
	ChatColor_Yellow           = '',	// = 0x01 (SOH) - Use normal (default) color from this point forward.
	#define CHAT_COLOR_YELLOW    ""

	ChatColor_TeamForAllString = '',	// = 0x02 (STX) - Use team color up to the end of the player name.
	#define CHAT_COLOR_TEAM_FOR_ALL_STRING ""

	ChatColor_Team             = '',	// = 0x03 (ETX) - Use team color from this point forward.
	#define CHAT_COLOR_TEAM      ""

	ChatColor_Green            = ''	// = 0x04 (EOT) - Use location color from this point forward.
	#define CHAT_COLOR_GREEN     ""
};

/// @note Not same for different resolutions.
const unsigned char charactersHeight (18u);
const unsigned char charactersWidths[MximumCharactersNumber] =
{
	6u,		// index == 0('\0').
	6u,		// index == 1('').
	6u,		// index == 2('').
	6u,		// index == 3('').
	6u,		// index == 4('').
	6u,		// index == 5('').
	6u,		// index == 6('').
	6u,		// index == 7('').
	6u,		// index == 8('').
	6u,		// index == 9('\t').
	6u,		// index == 10('\n').
	6u,		// index == 11('').
	6u,		// index == 12('').
	6u,		// index == 13('\r').
	6u,		// index == 14('').
	6u,		// index == 15('').
	6u,		// index == 16('').
	6u,		// index == 17('').
	6u,		// index == 18('').
	6u,		// index == 19('').
	6u,		// index == 20('').
	6u,		// index == 21('').
	6u,		// index == 22('').
	6u,		// index == 23('').
	6u,		// index == 24('').
	6u,		// index == 25('').
	6u,		// index == 26(('SUB') - fatal error C1071: unexpected end of file found in comment).
	6u,		// index == 27('').
	6u,		// index == 28('').
	6u,		// index == 29('').
	6u,		// index == 30('').
	6u,		// index == 31('').
	4u,		// index == 32(' ').
	4u,		// index == 33('!').
	4u,		// index == 34('"').
	7u,		// index == 35('#').
	7u,		// index == 36('$').
	9u,		// index == 37('%').
	8u,		// index == 38('&').
	3u,		// index == 39(''').
	4u,		// index == 40('(').
	4u,		// index == 41(')').
	5u,		// index == 42('*').
	7u,		// index == 43('+').
	4u,		// index == 44(',').
	4u,		// index == 45('-').
	4u,		// index == 46('.').
	5u,		// index == 47('/').
	7u,		// index == 48('0').
	7u,		// index == 49('1').
	7u,		// index == 50('2').
	7u,		// index == 51('3').
	7u,		// index == 52('4').
	7u,		// index == 53('5').
	7u,		// index == 54('6').
	7u,		// index == 55('7').
	7u,		// index == 56('8').
	7u,		// index == 57('9').
	4u,		// index == 58(':').
	4u,		// index == 59(';').
	7u,		// index == 60('<').
	7u,		// index == 61('=').
	7u,		// index == 62('>').
	5u,		// index == 63('?').
	9u,		// index == 64('@').
	8u,		// index == 65('A').
	7u,		// index == 66('B').
	7u,		// index == 67('C').
	8u,		// index == 68('D').
	7u,		// index == 69('E').
	7u,		// index == 70('F').
	8u,		// index == 71('G').
	8u,		// index == 72('H').
	3u,		// index == 73('I').
	6u,		// index == 74('J').
	7u,		// index == 75('K').
	7u,		// index == 76('L').
	10u,	// index == 77('M').
	8u,		// index == 78('N').
	8u,		// index == 79('O').
	7u,		// index == 80('P').
	9u,		// index == 81('Q').
	7u,		// index == 82('R').
	6u,		// index == 83('S').
	7u,		// index == 84('T').
	8u,		// index == 85('U').
	8u,		// index == 86('V').
	11u,	// index == 87('W').
	7u,		// index == 88('X').
	8u,		// index == 89('Y').
	7u,		// index == 90('Z').
	5u,		// index == 91('[').
	4u,		// index == 92('\').
	5u,		// index == 93(']').
	7u,		// index == 94('^').
	7u,		// index == 95('_').
	7u,		// index == 96('`').
	6u,		// index == 97('a').
	7u,		// index == 98('b').
	6u,		// index == 99('c').
	7u,		// index == 100('d').
	7u,		// index == 101('e').
	4u,		// index == 102('f').
	6u,		// index == 103('g').
	7u,		// index == 104('h').
	4u,		// index == 105('i').
	4u,		// index == 106('j').
	7u,		// index == 107('k').
	4u,		// index == 108('l').
	10u,	// index == 109('m').
	7u,		// index == 110('n').
	7u,		// index == 111('o').
	7u,		// index == 112('p').
	7u,		// index == 113('q').
	5u,		// index == 114('r').
	5u,		// index == 115('s').
	5u,		// index == 116('t').
	7u,		// index == 117('u').
	6u,		// index == 118('v').
	10u,	// index == 119('w').
	7u,		// index == 120('x').
	7u,		// index == 121('y').
	6u,		// index == 122('z').
	5u,		// index == 123('{').
	7u,		// index == 124('|').
	5u,		// index == 125('}').
	7u,		// index == 126('~').
	6u,		// index == 127('').
	6u,		// index == 128('Ä').
	6u,		// index == 129('Å').
	6u,		// index == 130('Ç').
	6u,		// index == 131('É').
	6u,		// index == 132('Ñ').
	6u,		// index == 133('Ö').
	6u,		// index == 134('Ü').
	6u,		// index == 135('á').
	6u,		// index == 136('à').
	6u,		// index == 137('â').
	6u,		// index == 138('ä').
	6u,		// index == 139('ã').
	6u,		// index == 140('å').
	6u,		// index == 141('ç').
	6u,		// index == 142('é').
	6u,		// index == 143('è').
	6u,		// index == 144('ê').
	6u,		// index == 145('ë').
	6u,		// index == 146('í').
	6u,		// index == 147('ì').
	6u,		// index == 148('î').
	6u,		// index == 149('ï').
	6u,		// index == 150('ñ').
	6u,		// index == 151('ó').
	6u,		// index == 152('ò').
	6u,		// index == 153('ô').
	6u,		// index == 154('ö').
	6u,		// index == 155('õ').
	6u,		// index == 156('ú').
	6u,		// index == 157('ù').
	6u,		// index == 158('û').
	6u,		// index == 159('ü').
	4u,		// index == 160('†').
	4u,		// index == 161('°').
	7u,		// index == 162('¢').
	7u,		// index == 163('£').
	7u,		// index == 164('§').
	8u,		// index == 165('•').
	7u,		// index == 166('¶').
	6u,		// index == 167('ß').
	7u,		// index == 168('®').
	9u,		// index == 169('©').
	5u,		// index == 170('™').
	7u,		// index == 171('´').
	7u,		// index == 172('¨').
	4u,		// index == 173('≠').
	9u,		// index == 174('Æ').
	7u,		// index == 175('Ø').
	7u,		// index == 176('∞').
	7u,		// index == 177('±').
	5u,		// index == 178('≤').
	5u,		// index == 179('≥').
	7u,		// index == 180('¥').
	7u,		// index == 181('µ').
	6u,		// index == 182('∂').
	4u,		// index == 183('∑').
	7u,		// index == 184('∏').
	5u,		// index == 185('π').
	5u,		// index == 186('∫').
	7u,		// index == 187('ª').
	10u,	// index == 188('º').
	10u,	// index == 189('Ω').
	10u,	// index == 190('æ').
	5u,		// index == 191('ø').
	8u,		// index == 192('¿').
	8u,		// index == 193('¡').
	8u,		// index == 194('¬').
	8u,		// index == 195('√').
	8u,		// index == 196('ƒ').
	8u,		// index == 197('≈').
	11u,	// index == 198('∆').
	7u,		// index == 199('«').
	7u,		// index == 200('»').
	7u,		// index == 201('…').
	7u,		// index == 202(' ').
	7u,		// index == 203('À').
	3u,		// index == 204('Ã').
	3u,		// index == 205('Õ').
	3u,		// index == 206('Œ').
	3u,		// index == 207('œ').
	8u,		// index == 208('–').
	8u,		// index == 209('—').
	8u,		// index == 210('“').
	8u,		// index == 211('”').
	8u,		// index == 212('‘').
	8u,		// index == 213('’').
	8u,		// index == 214('÷').
	7u,		// index == 215('◊').
	8u,		// index == 216('ÿ').
	8u,		// index == 217('Ÿ').
	8u,		// index == 218('⁄').
	8u,		// index == 219('€').
	8u,		// index == 220('‹').
	8u,		// index == 221('›').
	7u,		// index == 222('ﬁ').
	7u,		// index == 223('ﬂ').
	6u,		// index == 224('‡').
	6u,		// index == 225('·').
	6u,		// index == 226('‚').
	6u,		// index == 227('„').
	6u,		// index == 228('‰').
	6u,		// index == 229('Â').
	10u,	// index == 230('Ê').
	6u,		// index == 231('Á').
	7u,		// index == 232('Ë').
	7u,		// index == 233('È').
	7u,		// index == 234('Í').
	7u,		// index == 235('Î').
	4u,		// index == 236('Ï').
	4u,		// index == 237('Ì').
	4u,		// index == 238('Ó').
	4u,		// index == 239('Ô').
	7u,		// index == 240('').
	7u,		// index == 241('Ò').
	7u,		// index == 242('Ú').
	7u,		// index == 243('Û').
	7u,		// index == 244('Ù').
	7u,		// index == 245('ı').
	7u,		// index == 246('ˆ').
	7u,		// index == 247('˜').
	7u,		// index == 248('¯').
	7u,		// index == 249('˘').
	7u,		// index == 250('˙').
	7u,		// index == 251('˚').
	7u,		// index == 252('¸').
	7u,		// index == 253('˝').
	7u,		// index == 254('˛').
	7u		// index == 255('ˇ').
};

#endif	// ifndef COMMON_INCLUDED