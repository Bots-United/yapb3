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
// message_type.hpp
//
// Enumeration: ServerMessageType_t
//
// Description: For pfnMessageBegin() function. (Server to client messages)
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MESSAGE_TYPE_INCLUDED
#define MESSAGE_TYPE_INCLUDED

// For 'SVC_ROOMTYPE' message.
enum RoomType_t
{
	RoomType_Normal,			// Normal (off)
	RoomType_Generic,			// Generic
	RoomType_MetalSmall,		// Metal Small
	RoomType_MetalMedium,		// Metal Medium
	RoomType_MetalLarge,		// Metal Large
	RoomType_TunnelSmall,		// Tunnel Small
	RoomType_TunnelMedium,		// Tunnel Medium
	RoomType_TunnelLarge,		// Tunnel Large
	RoomType_ChamberSmall,		// Chamber Small
	RoomType_ChamberMedium,		// Chamber Medium
	RoomType_ChamberLarge,		// Chamber Large
	RoomType_BrightSmall,		// Bright Small
	RoomType_BrightMedium,		// Bright Medium
	RoomType_BrightLarge,		// Bright Large
	RoomType_Water1,			// Water 1
	RoomType_Water2,			// Water 2
	RoomType_Water3,			// Water 3
	RoomType_ConcreteSmall,		// Concrete Small
	RoomType_ConcreteMedium,	// Concrete Medium
	RoomType_ConcreteLarge,		// Concrete Large
	RoomType_Big1,				// Big 1
	RoomType_Big2,				// Big 2
	RoomType_Big3,				// Big 3
	RoomType_CavernSmall,		// Cavern Small
	RoomType_CavernMedium,		// Cavern Medium
	RoomType_CavernLarge,		// Cavern Large
	RoomType_Weirdo1,			// Weirdo 1
	RoomType_Weirdo2,			// Weirdo 2
	RoomType_Weirdo3			// Weirdo 3
};

enum ServerMessageType_t
{
	SVC_BAD,					// Not actually message....

	SVC_NOP,
	SVC_DISCONNECT,
	SVC_EVENT,
	SVC_VERSION,				// [long] server version
	SVC_SETVIEW,				// [short] entity number
	SVC_SOUND,					// <see code>
	SVC_TIME,					// [float] server time

	// Show the given string in the client console.
	SVC_PRINT,					// [string] null terminated string
	// string (message to display in the client console)

	// Send command for the client to execute.
	SVC_STUFFTEXT,				// [string] stuffed into client's console buffer. The string should be '\n' terminated
	// string (command line to execute remotely)

	SVC_SETANGLE,				// [angle3] set the view angle to this absolute value
	SVC_SERVERINFO,				// [long] version, [string] signon string, [string]..[0]model cache, [string]...[0]sounds cache
	SVC_LIGHTSTYLE,				// [byte] [string]
	SVC_UPDATEUSERINFO,			// [byte] player slot (from 0) [long] userid [string] userinfo[MaximumInfoStringLength - 1] [buffer] hashedcdkey[16]
	SVC_DELTADESCRIPTION,
	SVC_CLIENTDATA,				// <shortbits + data>
	SVC_STOPSOUND,				// <see code>

	SVC_PINGS,					// Send pings for client score board. (Contains ping, loss for several players)
	// Note: Flag is 1 - rest data contains PlayerID, Ping and Loss at least for 1 player.
	// Note: Flag is 0 - end of the message, skip remaining bits.
	// Note: ... - Flag + PlayerID + Ping + Loss - N times.
	// Structure:
	//	1  bit  - Flag 1
	//	5  bits - PlayerID
	//	12 bits - Ping
	//	7  bits - Loss
	//	...     - ...
	//	1  bit  - Flag 0
	//	n  bits - Align to byte with zero bits

	SVC_PARTICLE,
	SVC_DAMAGE,
	SVC_SPAWNSTATIC,
	SVC_EVENT_RELIABLE,
	SVC_SPAWNBASELINE,
	SVC_TEMPENTITY,
	SVC_SETPAUSE,				// [byte] on / off
	SVC_SIGNONNUM,				// [byte]  used for the signon sequence
	SVC_CENTERPRINT,			// [string] to put in center of the screen
	SVC_KILLEDMONSTER,
	SVC_FOUNDSECRET,
	SVC_SPAWNSTATICSOUND,		// [coord3] [byte] samp [byte] vol [byte] aten
	SVC_INTERMISSION,
	SVC_FINALE,					// [string] music [string] text
	SVC_CDTRACK,				// [byte] track [byte] looptrack
	SVC_RESTORE,
	SVC_CUTSCENE,
	SVC_WEAPONANIM,
	SVC_DECALNAME,				// [byte] index [string] name
	SVC_ROOMTYPE,				// [byte] roomtype (dsp effect)
	SVC_ADDANGLE,				// [angle3] set the view angle to this absolute value
	SVC_NEWUSERMSG,
	SVC_PACKETENTITIES,			// [...]  Non-delta compressed entities
	SVC_DELTAPACKETENTITIES,	// [...]  Delta compressed entities
	SVC_CHOKE,					// # of packets held back on channel because too much data was flowing.
	SVC_RESOURCELIST,
	SVC_NEWMOVEVARS,			// Send 'Structures::PlayerMove_t::Variables_t' structure.
	SVC_RESOURCEREQUEST,
	SVC_CUSTOMIZATION,
	SVC_CROSSHAIRANGLE,			// [char] pitch * 5 [char] yaw * 5
	SVC_SOUNDFADE,				// char percent, char holdtime, char fadeouttime, char fadeintime
	SVC_FILETXFERFAILED,
	SVC_HLTV,					// With sub commands from 'TempEntity_HLTV_SubCommand_t' enumeration.
	SVC_DIRECTOR,				// With sub commands from 'TempEntity_Director_SubCommand_t' enumeration. [byte] command full length in bytes, [byte] sub command from 'TempEntity_Director_SubCommand_t', [...] specified arguments of sub command.
	SVC_VOICEINIT,
	SVC_VOICEDATA,				// Voicestream data from the server
	SVC_SENDEXTRAINFO,
	SVC_TIMESCALE,
	SVC_RESOURCELOCATION,
	SVC_SENDCVARVALUE,
	SVC_SENDCVARVALUE2,

	ServerToClientMessageID_Total,

	ServerToClientMessageID_FreeSlotsNumber = 6u,

	ServerToClientMessageID_Maximum = 64u
};

// Game message "BotProgress" types....
enum ProgressBarType_t
{
	ProgressBarType_Update,		// Need to send all arguments!
	ProgressBarType_CreateNew,	// Send only Flag and Header
	ProgressBarType_Remove		// Send only Flag
};

enum GameMessageID_t
{
	GameMessageID_Start = ServerToClientMessageID_Maximum,	// Start since 64 because 0, ... 63 are reserved by engine!

	GameMessageID_VoiceMask = GameMessageID_Start,
	GameMessageID_ReqState,
	GameMessageID_CurWeapon,
	GameMessageID_Geiger,
	GameMessageID_Flashlight,
	GameMessageID_FlashBat,
	GameMessageID_Health,
	GameMessageID_Damage,
	GameMessageID_Battery,
	GameMessageID_Train,
	GameMessageID_HudTextPro,
	GameMessageID_HudText,
	GameMessageID_SayText,
	GameMessageID_TextMsg,
	GameMessageID_WeaponList,
	GameMessageID_ResetHUD,
	GameMessageID_InitHUD,
	GameMessageID_ViewMode,
	GameMessageID_GameTitle,
	GameMessageID_DeathMsg,
	GameMessageID_ScoreAttrib,
	GameMessageID_ScoreInfo,
	GameMessageID_TeamInfo,
	GameMessageID_TeamScore,
	GameMessageID_GameMode,
	GameMessageID_MOTD,
	GameMessageID_ServerName,
	GameMessageID_AmmoPickup,
	GameMessageID_WeapPickup,
	GameMessageID_ItemPickup,
	GameMessageID_HideWeapon,
	GameMessageID_SetFOV,
	GameMessageID_ShowMenu,
	GameMessageID_ScreenShake,
	GameMessageID_ScreenFade,
	GameMessageID_AmmoX,
	GameMessageID_SendAudio,
	GameMessageID_RoundTime,
	GameMessageID_Money,
	GameMessageID_ArmorType,
	GameMessageID_BlinkAcct,
	GameMessageID_StatusValue,
	GameMessageID_StatusText,
	GameMessageID_StatusIcon,
	GameMessageID_BarTime,
	GameMessageID_ReloadSound,
	GameMessageID_Crosshair,
	GameMessageID_NVGToggle,
	GameMessageID_Radar,
	GameMessageID_Spectator,
	GameMessageID_VGUIMenu,
	GameMessageID_TutorText,
	GameMessageID_TutorLine,
	GameMessageID_TutorState,
	GameMessageID_TutorClose,
	GameMessageID_AllowSpec,
	GameMessageID_BombDrop,
	GameMessageID_BombPickup,
	GameMessageID_ClCorpse,
	GameMessageID_HostagePos,
	GameMessageID_HostageK,
	GameMessageID_HLTV,
	GameMessageID_SpecHealth,
	GameMessageID_ForceCam,
	GameMessageID_ADStop,
	GameMessageID_ReceiveW,
	GameMessageID_CZCareer,
	GameMessageID_CZCareerHUD,
	GameMessageID_ShadowIdx,
	GameMessageID_TaskTime,
	GameMessageID_Scenario,
	GameMessageID_BotVoice,
	GameMessageID_BuyClose,
	GameMessageID_SpecHealth2,
	GameMessageID_BarTime2,
	GameMessageID_ItemStatus,
	GameMessageID_Location,
	GameMessageID_BotProgress,
	GameMessageID_Brass,
	GameMessageID_Fog,
	GameMessageID_ShowTimer,
	GameMessageID_HudTextArgs,

	UserMessageID_Total,

	GameMessageID_Total = UserMessageID_Total - GameMessageID_Start,

	GameMessageID_Maximum = MaximumUserMessages	// maximum number of user messages the engine can register (hardcoded in the engine)
};

// Client to server messages.
enum ClientMessageType_t
{
	CLC_BAD,

	CLC_NOP,
	CLC_MOVE,		// [Structures::UserCommand_t]
	CLC_STRINGCMD,	// [string] message
	CLC_DELTA,		// [byte] sequence number, requests delta compression of message
	CLC_RESOURCELIST,
	CLC_TMOVE,		// teleport request, spectator only
	CLC_FILECONSISTIENCY,
	CLC_VOICEDATA,	// Voicestream data from a client
	CLC_HLTV,
	CLC_CVARVALUE,
	CLC_CVARVALUE2
};

const char *const g_serverToClientUserMessageNames[] =
{
	"SVC_BAD",

	"SVC_NOP",
	"SVC_DISCONNECT",
	"SVC_EVENT",
	"SVC_VERSION",
	"SVC_SETVIEW",
	"SVC_SOUND",
	"SVC_TIME",
	"SVC_PRINT",
	"SVC_STUFFTEXT",
	"SVC_SETANGLE",
	"SVC_SERVERINFO",
	"SVC_LIGHTSTYLE",
	"SVC_UPDATEUSERINFO",
	"SVC_DELTADESCRIPTION",
	"SVC_CLIENTDATA",
	"SVC_STOPSOUND",
	"SVC_PINGS",
	"SVC_PARTICLE",
	"SVC_DAMAGE",
	"SVC_SPAWNSTATIC",
	"SVC_EVENT_RELIABLE",
	"SVC_SPAWNBASELINE",
	"SVC_TEMPENTITY",
	"SVC_SETPAUSE",
	"SVC_SIGNONNUM",
	"SVC_CENTERPRINT",
	"SVC_KILLEDMONSTER",
	"SVC_FOUNDSECRET",
	"SVC_SPAWNSTATICSOUND",
	"SVC_INTERMISSION",
	"SVC_FINALE",
	"SVC_CDTRACK",
	"SVC_RESTORE",
	"SVC_CUTSCENE",
	"SVC_WEAPONANIM",
	"SVC_DECALNAME",
	"SVC_ROOMTYPE",
	"SVC_ADDANGLE",
	"SVC_NEWUSERMSG",
	"SVC_PACKETENTITIES",
	"SVC_DELTAPACKETENTITIES",
	"SVC_CHOKE",
	"SVC_RESOURCELIST",
	"SVC_NEWMOVEVARS",
	"SVC_RESOURCEREQUEST",
	"SVC_CUSTOMIZATION",
	"SVC_CROSSHAIRANGLE",
	"SVC_SOUNDFADE",
	"SVC_FILETXFERFAILED",
	"SVC_HLTV",
	"SVC_DIRECTOR",
	"SVC_VOICEINIT",
	"SVC_VOICEDATA",
	"SVC_SENDEXTRAINFO",
	"SVC_TIMESCALE",
	"SVC_RESOURCELOCATION",
	"SVC_SENDCVARVALUE",
	"SVC_SENDCVARVALUE2",

	"SVC_FreeSlot1",
	"SVC_FreeSlot2",
	"SVC_FreeSlot3",
	"SVC_FreeSlot4",
	"SVC_FreeSlot5",

	"VoiceMask",
	"ReqState",
	"CurWeapon",
	"Geiger",
	"Flashlight",
	"FlashBat",
	"Health",
	"Damage",
	"Battery",
	"Train",
	"HudTextPro",
	"HudText",
	"SayText",
	"TextMsg",
	"WeaponList",
	"ResetHUD",
	"InitHUD",
	"ViewMode",
	"GameTitle",
	"DeathMsg",
	"ScoreAttrib",
	"ScoreInfo",
	"TeamInfo",
	"TeamScore",
	"GameMode",
	"MOTD",
	"ServerName",
	"AmmoPickup",
	"WeapPickup",
	"ItemPickup",
	"HideWeapon",
	"SetFOV",
	"ShowMenu",
	"ScreenShake",
	"ScreenFade",
	"AmmoX",
	"SendAudio",
	"RoundTime",
	"Money",
	"ArmorType",
	"BlinkAcct",
	"StatusValue",
	"StatusText",
	"StatusIcon",
	"BarTime",
	"ReloadSound",
	"Crosshair",
	"NVGToggle",
	"Radar",
	"Spectator",
	"VGUIMenu",
	"TutorText",
	"TutorLine",
	"TutorState",
	"TutorClose",
	"AllowSpec",
	"BombDrop",
	"BombPickup",
	"ClCorpse",
	"HostagePos",
	"HostageK",
	"HLTV",
	"SpecHealth",
	"ForceCam",
	"ADStop",
	"ReceiveW",
	"CZCareer",
	"CZCareerHUD",
	"ShadowIdx",
	"TaskTime",
	"Scenario",
	"BotVoice",
	"BuyClose",
	"SpecHealth2",
	"BarTime2",
	"ItemStatus",
	"Location",
	"BotProgress",
	"Brass",
	"Fog",
	"ShowTimer",
	"HudTextArgs"
};

#endif	// ifndef MESSAGE_TYPE_INCLUDED