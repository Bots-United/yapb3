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
// engine_function.hpp
//
// Description: Engine hands this to DLLs for functionality callbacks.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENGINE_FUNCTION_INCLUDED
#define ENGINE_FUNCTION_INCLUDED

struct EngineFunction_t
{
	int                              (*pfnPrecacheModel)                    (const char *modelName);
	int                              (*pfnPrecacheSound)                    (const char *soundName);
	void                             (*pfnSetModel)                         (Classes::Edict *const entity, const char *const modelName);
	int                              (*pfnModelIndex)                       (const char *const modelName);
	int                              (*pfnModelFrames)                      (int modelIndex);
	void                             (*pfnSetSize)                          (Classes::Edict *entity, const Math::Vector3D &min, const Math::Vector3D &max);
	void                             (*pfnChangeLevel)                      (const char *const s1, const char *const s2);
	void                             (*pfnGetSpawnParms)                    (Classes::Edict *entity);
	void                             (*pfnSaveSpawnParms)                   (Classes::Edict *entity);
	float                            (*pfnVecToYaw)                         (const Math::Vector3D &vector);
	void                             (*pfnVecToAngles)                      (const Math::Vector3D &vectorIn, Math::Angles3D &anglesOut);
	void                             (*pfnMoveToOrigin)                     (Classes::Edict *entity, const Math::Vector3D &goal, float distance, BOOL isFlyMoveType);
	void                             (*pfnChangeYaw)                        (Classes::Edict *entity);
	void                             (*pfnChangePitch)                      (Classes::Edict *entity);
	Classes::Edict                  *(*pfnFindEntityByString)               (const Classes::Edict *entityEdictStartSearchAfter, const char *field, const char *value);
	int                              (*pfnGetEntityIllum)                   (const Classes::Edict *entity);
	Classes::Edict                  *(*pfnFindEntityInSphere)               (const Classes::Edict *entityEdictStartSearchAfter, const Math::Vector3D &origin, float radius);
	Classes::Edict                  *(*pfnFindClientInPVS)                  (Classes::Edict *entityEdict);
	Classes::Edict                  *(*pfnEntitiesInPVS)                    (Classes::Edict *player);
	void                             (*pfnMakeVectors)                      (const Math::Angles3D &angles);
	void                             (*pfnAngleVectors)                     (const Math::Angles3D &angles, Math::Vector3D *forward/* = NULL*/, Math::Vector3D *right/* = NULL*/, Math::Vector3D *up/* = NULL*/);	// vector helpers
	Classes::Edict                  *(*pfnCreateEntity)                     (void);
	void                             (*pfnRemoveEntity)                     (Classes::Edict *entity);
	Classes::Edict                  *(*pfnCreateNamedEntity)                (const TypeDefinitions::StringOffset_t className);
	void                             (*pfnMakeStatic)                       (Classes::Edict *entity);
	int                              (*pfnEntIsOnFloor)                     (Classes::Edict *const entity);
	int                              (*pfnDropToFloor)                      (Classes::Edict *const entity);
	int                              (*pfnWalkMove)                         (Classes::Edict *entity, float yaw, float distance, int mode);
	void                             (*pfnSetOrigin)                        (Classes::Edict *const entity, const Math::Vector3D &newOrigin);
	void                             (*pfnEmitSound)                        (Classes::Edict *entity, Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, Constants::SoundFlag_t flags, Constants::SoundPitch_t pitch);
	void                             (*pfnEmitAmbientSound)                 (Classes::Edict *entity, Math::Vector3D &origin, const char *sample, float volume, float attenuation, Constants::SoundFlag_t flags, Constants::SoundPitch_t pitch);

	void                             (*pfnTraceLine)                        (const Math::Vector3D &source, const Math::Vector3D &destination, Constants::TraceIgnore_t noMonsters, Classes::Edict *entityToSkip, TraceResult_t &traceResult);
	void                             (*pfnTraceToss)                        (Classes::Edict *entity, Classes::Edict *entityToSkip, TraceResult_t &traceResult);
	int                              (*pfnTraceMonsterHull)                 (Classes::Edict *entityEdict, const Math::Vector3D &source, const Math::Vector3D &destination, int noMonsters, Classes::Edict *entityToSkip, TraceResult_t &traceResult);
	void                             (*pfnTraceHull)                        (const Math::Vector3D &source, const Math::Vector3D &destination, int noMonsters, Constants::Hull_t hullNumber, Classes::Edict *entityToSkip, TraceResult_t &traceResult);
	void                             (*pfnTraceEntity)                      (const Math::Vector3D &source, const Math::Vector3D &destination, Constants::Hull_t hullNumber, Classes::Edict *entity, TraceResult_t &traceResult);	// As of TraceLine(), but check ONLY for an entity who can't get a hit. (example: weaponbox) (Old name was 'pfnTraceModel')
	const char                      *(*pfnTraceTexture)                     (Classes::Edict *textureEntity, const Math::Vector3D &source, const Math::Vector3D &destination);
	void                             (*pfnTraceSphere)                      (const Math::Vector3D &source, const Math::Vector3D &destination, int noMonsters, float radius, Classes::Edict *entityToSkip, TraceResult_t &traceResult);	// Not yet implemented!

	void                             (*pfnGetAimVector)                     (Classes::Edict *entity, float speed, Math::Vector3D &result);
	void                             (*pfnServerCommand)                    (const char *string);	// (Last character must be '\n' or ';'! Original name is: PF_localcmd_I())
	void                             (*pfnServerExecute)                    (void);
	void                             (*pfnClientCommand)                    (Classes::Edict *client, char *format, ...);	// 'SVC_STUFFTEXT' client message. (Last character must be '\n' or ';'! Original name is: PF_stuffcmd_I())
	void                             (*pfnParticleEffect)                   (const Math::Vector3D &origin, const Math::Vector3D &direction, float color/* as unsigned char */, float count/* as unsigned char */);
	void                             (*pfnLightStyle)                       (const int style, char *const value);
	int                              (*pfnDecalIndex)                       (const char *name);
	Constants::PointContentType_t    (*pfnPointContents)                    (const Math::Vector3D &vector);

	void                             (*pfnMessageBegin)                     (const Constants::MessageDestination_t destination, const unsigned int type, const Math::Vector3D *const origin, Classes::Edict *const to);
	void                             (*pfnMessageEnd)                       (void);
	void                             (*pfnWriteByte)                        (int value);
	void                             (*pfnWriteChar)                        (int value);
	void                             (*pfnWriteShort)                       (int value);
	void                             (*pfnWriteLong)                        (int value);
	void                             (*pfnWriteAngle)                       (float value);
	void                             (*pfnWriteCoord)                       (float value);
	void                             (*pfnWriteString)                      (const char *const string);	/// @note 'string' can be NULL!! If so, the engine will sent empty ("") string to client.
	void                             (*pfnWriteEntity)                      (int entityIndex);

	// console variables...
	void                             (*pfnCVarRegister)                     (Classes::ConsoleVariable &consoleVariable);	// Calls 'Cvar_RegisterVariable'. (Notes: this function automaticly applies 'Constants::ConsoleVariableFlag_ExternalDLL' flag, this function ignores user-defined 'Classes::ConsoleVariable::value' and 'Classes::ConsoleVariable::next' members!)
	float                            (*pfnCVarGetFloat)                     (const char *const variableName);
	const char                      *(*pfnCVarGetString)                    (const char *const variableName);
	void                             (*pfnCVarSetFloat)                     (const char *const variableName, const float value);
	void                             (*pfnCVarSetString)                    (const char *const variableName, const char *const value);

	void                             (*pfnAlertMessage)                     (Constants::AlertType_t alertType, const char *format, ...);
	void                             (*pfnEngineFprintf)                    (void *file, const char *format, ...);	// Obsolete....
	void                            *(*pfnPvAllocEntPrivateData)            (Classes::Edict *const entity, const size_t allocateBlockSize);
	void                            *(*pfnPvEntPrivateData)                 (Classes::Edict *entity);
	void                             (*pfnFreeEntPrivateData)               (Classes::Edict *entity);
	const char                      *(*pfnSzFromIndex)                      (TypeDefinitions::StringOffset_t string);
	TypeDefinitions::StringOffset_t  (*pfnAllostring)                       (const char *const string);
	EntityVariables_t               *(*pfnGetVarsOfEnt)                     (Classes::Edict *entity);
	Classes::Edict                  *(*pfnPEntityOfEntOffset)               (TypeDefinitions::EntityOffset_t entityOffset);
	TypeDefinitions::EntityOffset_t  (*pfnEntOffsetOfPEntity)               (const Classes::Edict *entity);
	int                              (*pfnIndexOfEdict)                     (const Classes::Edict *entity);
	Classes::Edict                  *(*pfnPEntityOfEntIndex)                (int entityIndex);
	Classes::Edict                  *(*pfnFindEntityByVars)                 (EntityVariables_t *entityVariables);
	StudioHeader_t                  *(*pfnGetModelPtr)                      (const Classes::Edict *const entity);
	int                              (*pfnRegUserMsg)                       (const char *name, int size);
	void                             (*pfnAnimationAutomove)                (const Classes::Edict *entity, float time);
	void                             (*pfnGetBonePosition)                  (const Classes::Edict *entity, int boneID, Math::Vector3D &boneOrigin, Math::Angles3D *boneAngles/* = NULL*/);
	unsigned int                     (*pfnFunctionFromName)                 (const char *name);
	const char                      *(*pfnNameForFunction)                  (unsigned int function);
	void                             (*pfnClientPrintf)                     (Classes::Edict *client, Constants::PrintType_t printType, const char *message);	// JOHN: engine callbacks so game.DLL can print messages to individual clients
	void                             (*pfnServerPrint)                      (const char *message);	// (Calls Con_Printf() function)

	const char                      *(*pfnCmd_Args)                         (void);	// these 3 added
	const char                      *(*pfnCmd_Argv)                         (unsigned int argc);	// so game.DLL can easily
	const unsigned int               (*pfnCmd_Argc)                         (void);	// access client 'cmd' strings

	void                             (*pfnGetAttachment)                    (const Classes::Edict *entity, int attachment, Math::Vector3D &attachmentOrigin, Math::Angles3D *attachmentAngles/* = NULL*/);

	void                             (*pfnCRC32_Init)                       (TypeDefinitions::CRC32_t *CRC);
	void                             (*pfnCRC32_ProcessBuffer)              (TypeDefinitions::CRC32_t *CRC, void *pointer, int length);
	void                             (*pfnCRC32_ProcessByte)                (TypeDefinitions::CRC32_t *CRC, unsigned char character);
	TypeDefinitions::CRC32_t         (*pfnCRC32_Final)                      (TypeDefinitions::CRC32_t CRC);

	// random number generation part...
	int                              (*pfnRandomLong)                       (int low, int high);
	float                            (*pfnRandomFloat)                      (float low, float high);

	void                             (*pfnSetView)                          (const Classes::Edict *client, const Classes::Edict *viewEntity);
	float                            (*pfnTime)                             (void);	// Sys_FloatTime()
	void                             (*pfnCrosshairAngle)                   (const Classes::Edict *client, float pitch, float yaw);

	unsigned char *const             (*pfnLoadFileForMe)                    (const char *const filename, unsigned int *const length);
	void                             (*pfnFreeFile)                         (unsigned char *const fileBuffer);

	void                             (*pfnEndSection)                       (const char *sectionName);	// trigger_endsection
	int                              (*pfnCompareFileTime)                  (const char *filename1, char *filename2, int *compare);
	void                             (*pfnGetGameDir)                       (char *gameDirectory);
	void                             (*pfnCvar_RegisterVariable)            (Classes::ConsoleVariable &variable);
	void                             (*pfnFadeClientVolume)                 (const Classes::Edict *entityEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
	void                             (*pfnSetClientMaxspeed)                (Classes::Edict *const client, const float newMaximumSpeed);
	Classes::Edict                  *(*pfnCreateFakeClient)                 (const char *netName);	// returns NULL if fake client can't be created
	void                             (*pfnRunPlayerMove)                    (Classes::Edict *fakeClient, const Math::Angles3D &moveAngles, float forwardMoveSpeed, float sideMoveSpeed, float upMoveSpeed, unsigned short buttons, unsigned char impulse, unsigned char msecValue);
	int                              (*pfnNumberOfEntities)                 (void);
	char                            *(*pfnGetInfoKeyBuffer)                 (const Classes::Edict *const entity);	// passing in NULL gets the server info
	char                            *(*pfnInfoKeyValue)                     (char *infoBuffer, const char *key);
	void                             (*pfnSetKeyValue)                      (char *infoBuffer, const char *key, const char *value);
	void                             (*pfnSetClientKeyValue)                (int clientIndex, char *infoBuffer, const char *key, const char *value);
	int                              (*pfnIsMapValid)                       (const char *filename);
	void                             (*pfnStaticDecal)                      (const Math::Vector3D &origin, int decalIndex, int entityIndex, int modelIndex);
	int                              (*pfnPrecacheGeneric)                  (const char *s);
	const int                        (*pfnGetPlayerUserId)                  (const Classes::Edict *const player);	// returns the server assigned userid for this player. Useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients
	void                             (*pfnBuildSoundMsg)                    (Classes::Edict *entity, Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, Constants::SoundFlag_t flags, Constants::SoundPitch_t pitch, int destination, int type, const Math::Vector3D *origin, Classes::Edict *to);
	BOOL                             (*pfnIsDedicatedServer)                (void);	// is this a dedicated server?
	Classes::ConsoleVariable        *(*pfnCVarGetPointer)                   (const char *variableName);
	unsigned int                     (*pfnGetPlayerWONId)                   (Classes::Edict *player);	// returns the server assigned WONid for this player. Useful for logging frags, etc. returns -1 if the edict couldn't be found in the list of clients

	// YWB 8/1/99 TFF Physics additions
	void                             (*pfnInfo_RemoveKey)                   (char *s, const char *key);
	const char                      *(*pfnGetPhysicsKeyValue)               (const Classes::Edict *client, const char *key);
	void                             (*pfnSetPhysicsKeyValue)               (const Classes::Edict *client, const char *key, const char *value);
	const char                      *(*pfnGetPhysicsInfoString)             (const Classes::Edict *client);
	unsigned short                   (*pfnPrecacheEvent)                    (int type, const char *psz);
	void                             (*pfnPlaybackEvent)                    (event_info_t::Flag_t flags, const Classes::Edict *invoker, unsigned short eventIndex, float delay, Math::Vector3D &origin, Math::Angles3D &angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	unsigned char                   *(*pfnSetFatPVS)                        (const Math::Vector3D &origin);
	unsigned char                   *(*pfnSetFatPAS)                        (const Math::Vector3D &origin);
	int                              (*pfnCheckVisibility)                  (const Classes::Edict *entity, const unsigned char *const pset);

	void                             (*pfnDeltaSetField)                    (Delta_t *fields, const char *fieldName);
	void                             (*pfnDeltaUnsetField)                  (Delta_t *fields, const char *fieldName);
	void                             (*pfnDeltaAddEncoder)                  (char *name, void (*conditionalEncode) (Delta_t *fields, const unsigned char *from, const unsigned char *to));

	int                              (*pfnGetCurrentPlayer)                 (void);	// Get 'host_client' index, -1 if error.
	BOOL                             (*pfnCanSkipPlayer)                    (const Classes::Edict *player);

	int                              (*pfnDeltaFindField)                   (Delta_t *fields, const char *fieldName);
	void                             (*pfnDeltaSetFieldByIndex)             (Delta_t *fields, int fieldNumber);
	void                             (*pfnDeltaUnsetFieldByIndex)           (Delta_t *fields, int fieldNumber);

	void                             (*pfnSetGroupMask)                     (int mask, Constants::GroupOpType_t op);
	int                              (*pfnCreateInstancedBaseline)          (TypeDefinitions::StringOffset_t className, EntityState_t *baseline);
	void                             (*pfnCvar_DirectSet)                   (Classes::ConsoleVariable *variable, const char *value);

	// Forces the client and server to be running with the same version of the specified file (e.g., a player model).
	// Calling this has no effect in single player
	void                             (*pfnForceUnmodified)                  (Constants::ForceType_t type, Math::Vector3D &mins, Math::Vector3D &maxs, const char *filename);
	void                             (*pfnGetPlayerStats)                   (const Classes::Edict *client, int &ping, int &packetLoss);

	// Called by the init functions of other parts of the program to register commands and functions to call for them.
	// The 'commandName' is referenced later, so it should not be in temp memory if function is NULL,
	// the command will be forwarded to the server instead of executed locally.
	void                             (*pfnAddServerCommand)                 (const char *const commandName, const TypeDefinitions::ConsoleCommandCallbackFunction_t callbackFunction);

	// for voice communications, set which clients hear eachother.
	// NOTE: these functions take player entity indices (starting at 1).
	const BOOL                       (*pfnVoice_GetClientListening)         (const unsigned int receiver, const unsigned int sender);
	const BOOL                       (*pfnVoice_SetClientListening)         (const unsigned int receiver, const unsigned int sender, const BOOL listen);

	const char                      *(*pfnGetPlayerAuthId)                  (Classes::Edict *player);

	// new functions...
	SequenceEntry_t                 *(*pfnSequenceGet)                      (const char *filename, const char *entryName);
	SentenceEntry_t                 *(*pfnSequencePickSentence)             (const char *groupName, int pickMethod, int *picked);

	int                              (*pfnGetFileSize)                      (const char *filename);	// LH: Give access to filesize via filesystem (-1 if file not found)
	unsigned int                     (*pfnGetApproxWavePlayLen)             (const char *filePath);	// This function returns the wave sound play duration (in seconds).

	Classes::Interface::ICareerUI   *(*pfnIsCareerMatch)                    (void);	// MDC: Added for CZ career-mode.
	int                              (*pfnGetLocalizedStringLength)         (const char *label);	// BGC: return the number of characters of the localized string referenced by using "label"

	// BGC: added to facilitate persistent storage of tutor message decay values for different career game profiles. Also needs to persist regardless of mp.DLL being destroyed and recreated.
	void                             (*pfnRegisterTutorMessageShown)        (int mid);
	int                              (*pfnGetTimesTutorMessageShown)        (int mid);
	void                             (*pfnProcessTutorMessageDecayBuffer)   (int *buffer, int bufferLength);
	void                             (*pfnConstructTutorMessageDecayBuffer) (int *buffer, int bufferLength);
	void                             (*pfnResetTutorMessageDecayData)       (void);
	void                             (*pfnQueryClientCvarValue)             (const Classes::Edict *const player, const char *const consoleVariableName);
	void                             (*pfnQueryClientCvarValue2)            (const Classes::Edict *const player, const char *const consoleVariableName, const int requestID);

	// This function returns the launch parameter index in 'com_argv' array, 0 if nothing found. (*argument == com_argv[pfnCheckParameter()]) (Only latest steam versions has this function....)
	const unsigned int               (*pfnCheckParameter)                   (const char *const parameter, const char **const argument/* = NULL*/);	// (Example: "-nobots") (Added: 15 Jun 2009, build 4554)
};

// client DLL interface declarations

// this file is included by both the engine and the client-DLL, so make sure engine declarations aren't done twice

typedef int HSPRITE;	// handle to a graphic

struct ScreenInformation_t
{
	DECLARE_TYPED_ENUMERATION (unsigned int, Flag_t)
	{
		SCRINFO_SCREENFLASH = BIT (0u),
		SCRINFO_STRETCHED   = BIT (1u)
	};

	int     size;	// sizeof (ScreenInformation_t)

	int     width;
	int     height;
	Flag_t  flags;
	int     charactersHeight;
	short   charactersWidths[MximumCharactersNumber];
};	// sizeof (ScreenInformation_t) == 532 (133)

struct wrect_t
{
	int				left, right, top, bottom;
};

struct client_sprite_t
{
	char szName[64];
	char szSprite[64];
	int hspr;
	int iRes;
	wrect_t rc;
};

struct client_textmessage_t
{
	int		effect;
	unsigned char	r1, g1, b1, a1;		// 2 colors for effects
	unsigned char	r2, g2, b2, a2;
	float	x;
	float	y;
	float	fadein;
	float	fadeout;
	float	holdtime;
	float	fxtime;
	const char *pName;
	const char *pMessage;
};

struct hud_player_info_t
{
	char *name;
	short ping;
	unsigned char thisplayer;  // TRUE if this is the calling player

	// stuff that's unused at the moment,  but should be done
	unsigned char spectator;
	unsigned char packetloss;

	char *model;
	short topcolor;
	short bottomcolor;
};

// 4-23-98  JOHN

//
//  This DLL is linked by the client when they first initialize.
// This DLL is responsible for the following tasks:
//		- Loading the HUD graphics upon initialization
//		- Drawing the HUD graphics every frame
//		- Handling the custum HUD-update packets
//
typedef int (*pfnUserMsgHook) (const char *const name, const int size, void *const buffer);

struct usermsg_t
{
/*! CODE TO GET ADDRES TO LIST OF USER MESSAGES:
	// Credits: LanceVorgin
	DWORD dwUserMsgList = (DWORD)*gEngfuncs.pfnHookUserMsg + 0x1B;
	dwUserMsgList += *(DWORD *)(dwUserMsgList) + 0x4;
	dwUserMsgList += 0xD;
	usermsg_t **ppUserMsgList = (usermsg_t **)(*(DWORD*)dwUserMsgList);
	usermsg_t *pUserMsg = *ppUserMsgList;
*/
/*!off=0(0)*/	Constants::ServerMessageType_t number;	// 'SVC_*' number
/*!off=4(1)*/	int size;	// if size == -1, size come from first byte after svcnum
/*!off=8(2)*/	char name[16u];
/*!off=24(6)*/	usermsg_t *next;
/*!off=28(7)*/	pfnUserMsgHook pfn;	// user-defined function
};	// sizeof (usermsg_t) == 32 (8)

typedef void (*pfnEventHook) (event_info_t::event_args_t *args);

struct user_event_t
{
/*!off=0(0)*/	user_event_t *next;
/*!off=4(1)*/	char         *name;
/*!off=8(2)*/	pfnEventHook  func;	// user-defined function
};	// sizeof (user_event_t) == 12 (3)

struct con_nprint_t
{
	enum { MAX_DBG_NOTIFY = 32u };

	int           index;		// Row # (0 -> MAX_DBG_NOTIFY)
	float         time_to_live;	// # of seconds before it dissappears
	Color <float> color;		// RGB colors (0.0 -> 1.0 scale)
};

struct TriangleAPI_t
{
	#define TRI_API_VERSION		1

	enum PrimitiveCode_t
	{
		TRI_TRIANGLES,
		TRI_TRIANGLE_FAN,
		TRI_QUADS,
		TRI_POLYGON,
		TRI_LINES,
		TRI_TRIANGLE_STRIP,
		TRI_QUAD_STRIP
	};
	enum CullStyle_t
	{
		CullStyle_Front,
		CullStyle_None
	};

	int version;

	void (*RenderMode)    (Constants::RenderMode_t mode);
	void (*Begin)         (PrimitiveCode_t primitiveCode);
	void (*End)           (void);

	void (*Color4f)       (float red, float green, float blue, float alpha);
	void (*Color4ub)      (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
	void (*TexCoord2f)    (float u, float v);
	void (*Vertex3fv)     (const Math::Vector3D &worldPoint);
	void (*Vertex3f)      (float x, float y, float z);
	void (*Brightness)    (float brightness);
	void (*CullFace)      (CullStyle_t style);
	int  (*SpriteTexture) (Model_t *spriteModel, int frame);
	BOOL (*WorldToScreen) (const Math::Vector3D &world, Math::Vector2D &screen);	// Returns TRUE if it's z clipped.
	void (*Fog)           (const Color <float> &fogColor, float start, float end, BOOL on);	// Works just like GL_FOG, 'fogColor' is red/green/blue.
	void (*ScreenToWorld) (Math::Vector2D &screen, Math::Vector3D &world);
};

struct particle_t
{
	enum ptype_t
	{
		pt_static,
		pt_grav,
		pt_slowgrav,
		pt_fire,
		pt_explode,
		pt_explode2,
		pt_blob,
		pt_blob2,
		pt_vox_slowgrav,
		pt_vox_grav,
		pt_clientcustom	// Must have callback function specified
	};

// driver-usable fields
	Math::Vector3D		org;
	short		color;
	short		packedColor;
// drivers never touch the following fields
	particle_t	*next;
	Math::Vector3D		vel;
	float		ramp;
	float		die;
	ptype_t		type;
	void		(*deathfunc)( particle_t *particle );

	// for pt_clientcusttom, we'll call this function each frame
	void		(*callback)( particle_t *particle, float frametime );

	// For deathfunc, etc.
	unsigned char context;
};

/*
// FOR REFERENCE, These are the built-in tracer colors. Note, color 4 is the one
//  that uses the tracerred/tracergreen/tracerblue and traceralpha cvar settings
Color <> gTracerColors[] =
{
	{ 255, 255, 255 },		// White
	{ 255, 0, 0 },			// Red
	{ 0, 255, 0 },			// Green
	{ 0, 0, 255 },			// Blue
	{ 0, 0, 0 },			// Tracer default, filled in from cvars, etc.
	{ 255, 167, 17 },		// Yellow-orange sparks
	{ 255, 130, 90 },		// Yellowish streaks (garg)
	{ 55, 60, 144 },		// Blue egon streak
	{ 255, 130, 90 },		// More Yellowish streaks (garg)
	{ 255, 140, 90 },		// More Yellowish streaks (garg)
	{ 200, 130, 90 },		// More red streaks (garg)
	{ 255, 120, 70 }		// Darker red streaks (garg)
};
*/

struct TEMPENTITY
{
// Temporary entity array
#define TENTPRIORITY_LOW	0
#define TENTPRIORITY_HIGH	1

// TEMPENTITY flags
#define	FTENT_NONE				0x00000000
#define	FTENT_SINEWAVE			0x00000001
#define	FTENT_GRAVITY			0x00000002
#define FTENT_ROTATE			0x00000004
#define	FTENT_SLOWGRAVITY		0x00000008
#define FTENT_SMOKETRAIL		0x00000010
#define FTENT_COLLIDEWORLD		0x00000020
#define FTENT_FLICKER			0x00000040
#define FTENT_FADEOUT			0x00000080
#define FTENT_SPRANIMATE		0x00000100
#define FTENT_HITSOUND			0x00000200
#define FTENT_SPIRAL			0x00000400
#define FTENT_SPRCYCLE			0x00000800
#define FTENT_COLLIDEALL		0x00001000 // will collide with world and slideboxes
#define FTENT_PERSIST			0x00002000 // tent is not removed when unable to draw
#define FTENT_COLLIDEKILL		0x00004000 // tent is removed upon collision with anything
#define FTENT_PLYRATTACHMENT	0x00008000 // tent is attached to a player (owner)
#define FTENT_SPRANIMATELOOP	0x00010000 // animating sprite doesn't die when last frame is displayed
#define FTENT_SPARKSHOWER		0x00020000
#define FTENT_NOMODEL			0x00040000 // Doesn't have a model, never try to draw ( it just triggers other things )
#define FTENT_CLIENTCUSTOM		0x00080000 // Must specify callback.  Callback function is responsible for killing tempent and updating fields ( unless other flags specify how to do things )

	int			flags;
	float		die;
	float		frameMax;
	float		x;
	float		y;
	float		z;
	float		fadeSpeed;
	float		bounceFactor;
	int			hitSound;
	void		( *hitcallback )	( TEMPENTITY *ent, PMTraceResult_t &ptr );
	void		( *callback )		( TEMPENTITY *ent, float frametime, float currenttime );
	TEMPENTITY	*next;
	int			priority;
	short		clientIndex;	// if attached, this is the index of the client to stick to
								// if COLLIDEALL, this is the index of the client to ignore
								// TENTS with FTENT_PLYRATTACHMENT MUST set the clientindex!

	Math::Vector3D		tentOffset;		// if attached, client origin + tentOffset = tent origin.
	ClientEntity_t	entity;

	// baseline.origin		- velocity
	// baseline.renderamt	- starting fadeout intensity
	// baseline.angles		- angle velocity
};

struct BEAM
{
	enum
	{
		MAX_BEAMS = 128u	// Max simultaneous beams
	};
	enum Flag_t
	{
		FBEAM_STARTENTITY  = 0x00000001,
		FBEAM_ENDENTITY    = 0x00000002,
		FBEAM_FADEIN       = 0x00000004,
		FBEAM_FADEOUT      = 0x00000008,
		FBEAM_SINENOISE    = 0x00000010,
		FBEAM_SOLID        = 0x00000020,
		FBEAM_SHADEIN      = 0x00000040,
		FBEAM_SHADEOUT     = 0x00000080,
		FBEAM_STARTVISIBLE = 0x10000000,	// Has this client actually seen this beam's start entity yet?
		FBEAM_ENDVISIBLE   = 0x20000000,	// Has this client actually seen this beam's end entity yet?
		FBEAM_ISACTIVE     = 0x40000000,
		FBEAM_FOREVER      = 0x80000000
	};

	BEAM		*next;	// Next beam in list
	Constants::TempEntityEvent_t type;	// Type of beam
	Flag_t			flags;
	Math::Vector3D		source;
	Math::Vector3D		target;
	Math::Vector3D		delta;
	float		t;		// 0 ... 1 over lifetime of beam
	float		freq;
	float		die;	// Time when beam should die
	float		width;
	float		amplitude;
	float		red, green, blue;	// Color
	float		brightness;
	float		speed;	// Speed

	// Animation
	float		frameRate;
	float		frame;
	int			segments;

	int			startEntity;
	int			endEntity;
	int			modelIndex;	// Model info
	int			frameCount;
	Model_t		*pFollowModel;
	particle_t	*particles;
};

struct dlight_t
{
	Math::Vector3D	origin;
	float	radius;
	Color <>	color;
	float	die;				// stop lighting after this time
	float	decay;				// drop this each second
	float	minlight;			// don't add when contributing less
	int		key;
	BOOL	dark;			// subtracts light instead of adding
};

struct efx_api_t
{
	particle_t  *( *R_AllocParticle )			( void ( *callback ) ( particle_t *particle, float frametime ) );
	void		( *R_BlobExplosion )			( Math::Vector3D &org );
	void		( *R_Blood )					( Math::Vector3D &org, Math::Vector3D &dir, int pcolor, int speed );
	void		( *R_BloodSprite )				( Math::Vector3D &org, int colorindex, int modelIndex, int modelIndex2, float size );
	void		( *R_BloodStream )				( Math::Vector3D &org, Math::Vector3D &dir, int pcolor, int speed );
	void		( *R_BreakModel )				( Math::Vector3D &pos, float *size, Math::Vector3D &dir, float random, float life, int count, int modelIndex, char flags );
	void		( *R_Bubbles )					( Math::Vector3D &mins, Math::Vector3D &maxs, float height, int modelIndex, int count, float speed );
	void		( *R_BubbleTrail )				( Math::Vector3D &start, Math::Vector3D &end, float height, int modelIndex, int count, float speed );
	void		( *R_BulletImpactParticles )	( Math::Vector3D &pos );
	void		( *R_EntityParticles )			( ClientEntity_t *ent );
	void		( *R_Explosion )				( Math::Vector3D &pos, int model, float scale, float framerate, int flags );
	void		( *R_FizzEffect )				( ClientEntity_t *pent, int modelIndex, int density );
	void		( *R_FireField ) 				( Math::Vector3D &org, int radius, int modelIndex, int count, int flags, float life );
	void		( *R_FlickerParticles )			( Math::Vector3D &org );
	void		( *R_FunnelSprite )				( Math::Vector3D &org, int modelIndex, int reverse );
	void		( *R_Implosion )				( Math::Vector3D &end, float radius, int count, float life );
	void		( *R_LargeFunnel )				( Math::Vector3D &org, int reverse );
	void		( *R_LavaSplash )				( Math::Vector3D &org );
	void		( *R_MultiGunshot )				( Math::Vector3D &org, Math::Vector3D &dir, float *noise, int count, int decalCount, int *decalIndices );
	void		( *R_MuzzleFlash )				( Math::Vector3D &pos1, int type );
	void		( *R_ParticleBox )				( Math::Vector3D &mins, Math::Vector3D &maxs, unsigned char red, unsigned char green, unsigned char blue, float life );
	void		( *R_ParticleBurst )			( Math::Vector3D &pos, int size, int color, float life );
	void		( *R_ParticleExplosion )		( Math::Vector3D &org );
	void		( *R_ParticleExplosion2 )		( Math::Vector3D &org, int colorStart, int colorLength );
	void		( *R_ParticleLine )				( Math::Vector3D &start, Math::Vector3D &end, unsigned char red, unsigned char green, unsigned char blue, float life );
	void		( *R_PlayerSprites )			( int client, int modelIndex, int count, int size );
	void		( *R_Projectile )				( Math::Vector3D &origin, Math::Vector3D &velocity, int modelIndex, int life, int owner, void (*hitcallback)( TEMPENTITY *ent, PMTraceResult_t &ptr ) );
	void		( *R_RicochetSound )			( Math::Vector3D &pos );
	void		( *R_RicochetSprite )			( Math::Vector3D &pos, Model_t *pmodel, float duration, float scale );
	void		( *R_RocketFlare )				( Math::Vector3D &pos );
	void		( *R_RocketTrail )				( Math::Vector3D &start, Math::Vector3D &end, int type );
	void		( *R_RunParticleEffect )		( Math::Vector3D &org, Math::Vector3D &dir, int color, int count );
	void		( *R_ShowLine )					( Math::Vector3D &start, Math::Vector3D &end );
	void		( *R_SparkEffect )				( Math::Vector3D &pos, int count, int velocityMin, int velocityMax );
	void		( *R_SparkShower )				( Math::Vector3D &pos );
	void		( *R_SparkStreaks )				( Math::Vector3D &pos, int count, int velocityMin, int velocityMax );
	void		( *R_Spray )					( Math::Vector3D &pos, Math::Vector3D &dir, int modelIndex, int count, int speed, int spread, int rendermode );
	void		( *R_Sprite_Explode )			( TEMPENTITY *pTemp, float scale, int flags );
	void		( *R_Sprite_Smoke )				( TEMPENTITY *pTemp, float scale );
	void		( *R_Sprite_Spray )				( Math::Vector3D &pos, Math::Vector3D &dir, int modelIndex, int count, int speed, int iRand );
	void		( *R_Sprite_Trail )				( int type, Math::Vector3D &start, Math::Vector3D &end, int modelIndex, int count, float life, float size, float amplitude, int renderamt, float speed );
	void		( *R_Sprite_WallPuff )			( TEMPENTITY *pTemp, float scale );
	void		( *R_StreakSplash )				( Math::Vector3D &pos, Math::Vector3D &dir, int color, int count, float speed, int velocityMin, int velocityMax );
	void		( *R_TracerEffect )				( Math::Vector3D &start, Math::Vector3D &end );
	void		( *R_UserTracerParticle )		( Math::Vector3D &org, Math::Vector3D &vel, float life, int colorIndex, float length, unsigned char deathcontext, void ( *deathfunc)( particle_t *particle ) );
	particle_t *( *R_TracerParticles )			( Math::Vector3D &org, Math::Vector3D &vel, float life );
	void		( *R_TeleportSplash )			( Math::Vector3D &org );
	void		( *R_TempSphereModel )			( Math::Vector3D &pos, float speed, float life, int count, int modelIndex );
	TEMPENTITY	*( *R_TempModel )				( Math::Vector3D &pos, Math::Vector3D &dir, Math::Angles3D &angles, float life, int modelIndex, int soundtype );
	TEMPENTITY	*( *R_DefaultSprite )			( Math::Vector3D &pos, int spriteIndex, float framerate );
	TEMPENTITY	*( *R_TempSprite )				( Math::Vector3D &pos, Math::Vector3D &dir, float scale, int modelIndex, int rendermode, int renderfx, float a, float life, int flags );
	int			( *Draw_DecalIndex )			( int id );
	int			( *Draw_DecalIndexFromName )	( char *name );
	void		( *R_DecalShoot )				( int textureIndex, int entity, int modelIndex, Math::Vector3D &position, int flags );
	void		( *R_AttachTentToPlayer )		( int client, int modelIndex, float zoffset, float life );
	void		( *R_KillAttachedTents )		( int client );
	BEAM		*( *R_BeamCirclePoints )		( int type, Math::Vector3D &start, Math::Vector3D &end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float red, float green, float blue );
	BEAM		*( *R_BeamEntPoint )			( int startEnt, Math::Vector3D &end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float red, float green, float blue );
	BEAM		*( *R_BeamEnts )				( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float red, float green, float blue );
	BEAM		*( *R_BeamFollow )				( int startEnt, int modelIndex, float life, float width, float red, float green, float blue, float brightness );
	void		( *R_BeamKill )					( int deadEntity );
	BEAM		*( *R_BeamLightning )			( Math::Vector3D &start, Math::Vector3D &end, int modelIndex, float life, float width, float amplitude, float brightness, float speed );
	BEAM		*( *R_BeamPoints )				( const Math::Vector3D &start, const Math::Vector3D &end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float red, float green, float blue );
	BEAM		*( *R_BeamRing )				( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float red, float green, float blue );
	dlight_t	*( *CL_AllocDlight )			( int key );
	dlight_t	*( *CL_AllocElight )			( int key );
	TEMPENTITY	*( *CL_TempEntAlloc )			( Math::Vector3D &org, Model_t *model );
	TEMPENTITY	*( *CL_TempEntAllocNoModel )	( Math::Vector3D &org );
	TEMPENTITY	*( *CL_TempEntAllocHigh )		( Math::Vector3D &org, Model_t *model );
	TEMPENTITY	*( *CL_TentEntAllocCustom )		( Math::Vector3D &origin, Model_t *model, int high, void ( *callback ) ( TEMPENTITY *ent, float frametime, float currenttime ) );
	void		( *R_GetPackedColor )			( short *packed, short color );
	short		( *R_LookupColor )				( unsigned char red, unsigned char green, unsigned char blue );
	void		( *R_DecalRemoveAll )			( int textureIndex ); //textureIndex points to the decal index in the array, not the actual texture index.
};

struct event_api_t
{
#define EVENT_API_VERSION 1

	int		version;

	void	( *EV_PlaySound ) ( int ent, Math::Vector3D &origin, Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, Constants::SoundFlag_t fFlags, Constants::SoundPitch_t pitch );
	void	( *EV_StopSound ) ( int ent, Constants::SoundChannel_t channel, const char *sample );
	int		( *EV_FindModelIndex )( const char *pmodel );
	int		( *EV_IsLocal ) ( int playernum );
	int		( *EV_LocalPlayerDucking ) ( void );
	void	( *EV_LocalPlayerViewheight ) ( Math::Vector3D &);
	void	( *EV_LocalPlayerBounds ) ( int hull, Math::Vector3D &mins, Math::Vector3D &maxs );
	int		( *EV_IndexFromTrace) ( PMTraceResult_t &pTrace );
	PhysEntity_t *( *EV_GetPhysent ) ( int idx );
	void	( *EV_SetUpPlayerPrediction ) ( int dopred, int bIncludeLocalClient );
	void	( *EV_PushPMStates ) ( void );
	void	( *EV_PopPMStates ) ( void );
	void	( *EV_SetSolidPlayers ) (int playernum);
	void	( *EV_SetTraceHull ) ( int hull );
	void	( *EV_PlayerTrace ) ( Math::Vector3D &start, Math::Vector3D &end, int traceFlags, int ignore_pe, PMTraceResult_t &tr );
	void	( *EV_WeaponAnimation ) ( int sequence, int body );
	unsigned short ( *EV_PrecacheEvent ) ( int type, const char *psz );
	void	( *EV_PlaybackEvent ) ( event_info_t::Flag_t flags, const Classes::Edict *pInvoker, unsigned short eventindex, float delay, Math::Vector3D &origin, Math::Angles3D &angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	const char *( *EV_TraceTexture ) ( int ground, Math::Vector3D &vstart, Math::Vector3D &vend );
	void	( *EV_StopAllSounds ) ( int entnum, int entchannel );
	void    ( *EV_KillEvents ) ( int entnum, const char *eventname );
};

struct demo_api_t
{
	int		( *IsRecording )	( void );
	int		( *IsPlayingback )	( void );
	int		( *IsTimeDemo )		( void );
	void	( *WriteBuffer )	( int size, unsigned char *buffer );
};

enum
{
	NETAPI_REQUEST_SERVERLIST,  // Doesn't need a remote address
	NETAPI_REQUEST_PING,
	NETAPI_REQUEST_RULES,
	NETAPI_REQUEST_PLAYERS,
	NETAPI_REQUEST_DETAILS
};

// Set this flag for things like broadcast requests, etc. where the engine should not
//  kill the request hook after receiving the first response
#define FNETAPI_MULTIPLE_RESPONSE ( 1<<0 )

struct net_adrlist_t
{
	net_adrlist_t	*next;
	netadr_t				remote_address;
};

struct net_response_t
{
	DECLARE_TYPED_ENUMERATION (unsigned int, ErrorFlag_t)
	{
		NET_SUCCESS,

		NET_ERROR_TIMEOUT           = BIT (0u),
		NET_ERROR_PROTO_UNSUPPORTED = BIT (1u),
		NET_ERROR_UNDEFINED         = BIT (2u)
	};

	// NET_SUCCESS or an error code
	ErrorFlag_t error;

	// Context ID
	int			context;
	// Type
	int			type;

	// Server that is responding to the request
	netadr_t	remote_address;

	// Response RTT ping time
	double		ping;
	// Key/Value pair string ( separated by backlash '\' characters )
	// WARNING: You must copy this buffer in the callback function, because it is freed by the engine right after the call!!!!
	// ALSO: For NETAPI_REQUEST_SERVERLIST requests, this will be a pointer to a linked list of net_adrlist_t's
	union /* Unnamed */
	{
		char          *responseString;
		net_adrlist_t *response;
	};
};

typedef void ( *net_api_response_func_t ) ( net_response_t *response );

struct net_status_t
{
	// Connected to remote server? 1 == yes, 0 otherwise
	BOOL			connected;
	// Client's IP address
	netadr_t	local_address;
	// Address of remote server
	netadr_t	remote_address;
	// Packet Loss ( as a percentage )
	int			packet_loss;
	// Latency, in seconds ( multiply by 1000.0 to get milliseconds )
	double		latency;
	// Connection time, in seconds
	double		connection_time;
	// Rate setting ( for incoming data )
	double		rate;
};

struct net_api_t
{
	// APIs
	void		( *InitNetworking )( void );
	void		( *Status ) ( net_status_t *status );
	void		( *SendRequest) ( int context, int request, int flags, double timeout, netadr_t *remote_address, net_api_response_func_t response );
	void		( *CancelRequest ) ( int context );
	void		( *CancelAllRequests ) ( void );
	char		*( *AdrToString ) ( netadr_t *a );
	int			( *CompareAdr ) ( netadr_t *a, netadr_t *blue );
	int			( *StringToAdr ) ( char *s, netadr_t *a );
	const char *( *ValueForKey ) ( const char *s, const char *key );
	void		( *RemoveKey ) ( char *s, const char *key );
	void		( *SetValueForKey ) (char *s, const char *key, const char *value, int maxsize );
};

struct IVoiceTweak;	// Declared in 'Interface_VoiceTweak.hpp' file.

struct screenfade_t
{
	float		fadeSpeed;		// How fast to fade (tics / second) (+ fade in, - fade out)
	float		fadeEnd;		// When the fading hits maximum
	float		fadeTotalEnd;	// Total End Time of the fade (used for FFADE_OUT)
	float		fadeReset;		// When to reset to not fading (for fadeout and hold)
	ColorWithAlpha <> fadeColor;	// Fade color
	int			fadeFlags;		// Fading flags
};

struct cmd_function_t
{
	typedef void (*xcommand_t) (void);

	cmd_function_t	*next;
	char			*name;
	xcommand_t		function;
};
struct cmdalias_t
{
#define	MAX_ALIAS_NAME	32

	cmdalias_t	*next;
	char		name[MAX_ALIAS_NAME];
	char		*value;
};

struct ClientEngineFunction_t
{
#define CLDLL_INTERFACE_VERSION		7

	// sprite handlers
/*! Off=0(0) */	HSPRITE						( *pfnSPR_Load )			( const char *szPicName );	// Loads a sprite into memory, and returns a handle to it.
/*! Off=4(1) */	int							( *pfnSPR_Frames )			( HSPRITE hPic );	// Returns the number of frames stored in the specified sprite.
/*! Off=8(2) */	int							( *pfnSPR_Height )			( HSPRITE hPic, int frame );	// Returns the height, in pixels, of a sprite at the specified frame. Returns 0 is the frame number or the sprite handle is invalid.
/*! Off=12(3) */	int							( *pfnSPR_Width )			( HSPRITE hPic, int frame );	// Returns the width, in pixels, of a sprite at the specified frame. Returns 0 is the frame number or the sprite handle is invalid.
/*! Off=16(4) */	void						( *pfnSPR_Set )				( HSPRITE hPic, int red, int green, int blue );	// Prepares a sprite about to be drawn. RBG color values are applied to the sprite at this time.
/*! Off=20(5) */	void						( *pfnSPR_Draw )			( int frame, int x, int y, const wrect_t *prc );	// Precondition: SPR_Set has already been called for a sprite. Draws the currently active sprite to the screen, at position (x,y), where (0,0) is the top left-hand corner of the screen.
/*! Off=24(6) */	void						( *pfnSPR_DrawHoles )		( int frame, int x, int y, const wrect_t *prc );	// Precondition: SPR_Set has already been called for a sprite. Draws the currently active sprite to the screen. Color index #255 is treated as transparent.
/*! Off=28(7) */	void						( *pfnSPR_DrawAdditive )	( int frame, int x, int y, const wrect_t *prc );	// Precondition: SPR_Set has already been called for a sprite. Draws the currently active sprite to the screen, adding it's color values to the background.
/*! Off=32(8) */	void						( *pfnSPR_EnableScissor )	( int x, int y, int width, int height );	// Creates a clipping rectangle. No pixels will be drawn outside the specified area. Will stay in effect until either the next frame, or SPR_DisableScissor is called.
/*! Off=36(9) */	void						( *pfnSPR_DisableScissor )	( void );	// Disables the effect of an SPR_EnableScissor call.
/*! Off=40(10) */	client_sprite_t				*( *pfnSPR_GetList )			( char *psz, int *piCount );

	// screen handlers
/*! Off=44(11) */	void						( *pfnFillRGBA )			( int x, int y, int width, int height, int red, int green, int blue, int alpha );
/*! Off=48(12) */	int							( *pfnGetScreenInfo ) 		( ScreenInformation_t *pscrinfo );
/*! Off=52(13) */	void						( *pfnSetCrosshair )		( HSPRITE hspr, wrect_t rc, int red, int green, int blue );

	// console variable handlers
/*! Off=56(14) */	Classes::ConsoleVariable				*( *pfnRegisterVariable )	( char *szName, char *szValue, int flags );
/*! Off=60(15) */	float						( *pfnGetCvarFloat )		( char *szName );
/*! Off=64(16) */	char                       *( *pfnGetCvarString )		( char *szName );

	// command handlers
/*! Off=68(17) */	int							( *pfnAddCommand )			( char *cmd_name, void (*function) (void) );
/*! Off=72(18) */	int							( *pfnHookUserMsg )			( char *szMsgName, pfnUserMsgHook pfn );
/*! Off=76(19) */	int							( *pfnServerCmd )			( char *szCmdString );
/*! Off=80(20) */	int							( *pfnClientCmd )			( char *szCmdString );	// sends a command to the server, just as if the client had typed the 'szCmdString' at the console.

/*! Off=84(21) */	void						( *pfnGetPlayerInfo )		( int ent_num, hud_player_info_t *pinfo );	/// @note Filled from 'PlayerInfo_t cl.players[Constants::MaximumClients]' array

	// sound handlers
/*! Off=88(22) */	void						( *pfnPlaySoundByName )		( char *szSound, float volume );	// plays the sound 'szSound' at the specified volume. Loads the sound if it hasn't been cached. If it can't find the sound, it displays an error message and plays no sound.
/*! Off=92(23) */	void						( *pfnPlaySoundByIndex )	( int iSound, float volume );	// Precondition: 'iSound' has been precached. Plays the sound, from the precache list.

	// vector helpers
/*! Off=96(24) */	void						( *pfnAngleVectors )		( const Math::Angles3D &vecAngles, Math::Vector3D *forward, Math::Vector3D *right, Math::Vector3D *up );

	// text message system
/*! Off=100(25) */	client_textmessage_t		*( *pfnTextMessageGet )		( const char *pName );
/*! Off=104(26) */	int							( *pfnDrawCharacter )		( int x, int y, int character, int red, int green, int blue );
/*! Off=108(27) */	int							( *pfnDrawConsoleString )	( int x, int y, const char *string );
/*! Off=112(28) */	void						( *pfnDrawSetTextColor )	( float red, float green, float blue );
/*! Off=116(29) */	void						( *pfnDrawConsoleStringLen )(  const char *string, int *length, int *height );

/*! Off=120(30) */	void						( *pfnConsolePrint )		( const char *string );
/*! Off=124(31) */	void						( *pfnCenterPrint )			( const char *string );

// Added for user input processing
/*! Off=128(32) */	int							( *GetWindowCenterX )		( void );
/*! Off=132(33) */	int							( *GetWindowCenterY )		( void );
/*! Off=136(34) */	void						( *GetViewAngles )			( Math::Angles3D &);
/*! Off=140(35) */	void						( *SetViewAngles )			( Math::Angles3D &);
/*! Off=144(36) */	int							( *GetMaxClients )			( void );
/*! Off=148(37) */	void						( *Cvar_SetValue )			( const char *cvar, float value );

/*! Off=152(38) */	int       					(*Cmd_Argc)					(void);
/*! Off=156(39) */	char						*( *Cmd_Argv )				( unsigned int arg );

/*! Off=160(40) */	void						( *Con_Printf )				( const char *fmt, ... );	// (Maximum string length is 4096!)
/*! Off=164(41) */	void						( *Con_DPrintf )			( const char *fmt, ... );	// Developer print to console (yellow color) (Maximum string length is 4096!)
/*! Off=168(42) */	void						( *Con_NPrintf )			( int pos, const char *fmt, ... );	// (Maximum string length is 80!)
/*! Off=172(43) */	void						( *Con_NXPrintf )			( const con_nprint_t *info, const char *fmt, ... );	// (Maximum string length is 80!)

/*! Off=176(44) */	const char					*( *PhysInfo_ValueForKey )	( const char *key );
/*! Off=180(45) */	const char					*( *ServerInfo_ValueForKey )( const char *key );
/*! Off=184(46) */	float						( *GetClientMaxspeed )		( void );
/*! Off=188(47) */	int							( *CheckParm )				( char *parm, char **ppnext );
/*! Off=192(48) */	void						( *Key_Event )				( int key, BOOL down );
/*! Off=196(49) */	void						( *GetMousePosition )		( int *mx, int *my );
/*! Off=200(50) */	int							( *IsNoClipping )			( void );

/*! Off=204(51) */	ClientEntity_t			*( *GetLocalPlayer )		( void );
/*! Off=208(52) */	ClientEntity_t			*( *GetViewModel )			( void );
/*! Off=212(53) */	ClientEntity_t			*( *GetEntityByIndex )		( int idx );

/*! Off=216(54) */	float						( *GetClientTime )			( void );
/*! Off=220(55) */	void						( *V_CalcShake )			( void );
/*! Off=224(56) */	void						( *V_ApplyShake )			( Math::Vector3D &origin, Math::Angles3D &angles, float factor );

/*! Off=228(57) */	int							( *PM_PointContents )		( Math::Vector3D &point, int *truecontents );
/*! Off=232(58) */	int							( *PM_WaterEntity )			( Math::Vector3D &p );
/*! Off=236(59) */	PMTraceResult_t			*( *PM_TraceLine )			( Math::Vector3D &start, Math::Vector3D &end, int flags, int usehull, int ignore_pe );

/*! Off=240(60) */	Model_t				*( *CL_LoadModel )			( const char *modelname, int *index );
/*! Off=244(61) */	int							( *CL_CreateVisibleEntity )	( int type, ClientEntity_t *ent );

/*! Off=248(62) */	const Model_t *		( *GetSpritePointer )		( HSPRITE hSprite );
/*! Off=252(63) */	void						( *pfnPlaySoundByNameAtLocation )	( char *szSound, float volume, Math::Vector3D &origin );

/*! Off=256(64) */	unsigned short				( *pfnPrecacheEvent )		( int type, const char *psz );
/*! Off=260(65) */	void						( *pfnPlaybackEvent )		( event_info_t::Flag_t flags, const Classes::Edict *pInvoker, unsigned short eventindex, float delay, Math::Vector3D &origin, Math::Angles3D &angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
/*! Off=264(66) */	void						( *pfnWeaponAnim )			( int iAnim, int body );
/*! Off=268(67) */	float						( *pfnRandomFloat )			( float flLow, float flHigh );
/*! Off=272(68) */	long						( *pfnRandomLong )			( long lLow, long lHigh );
/*! Off=276(69) */	void						( *pfnHookEvent )			( char *name, pfnEventHook pfnEvent);
/*! Off=280(70) */	int							(*Con_IsVisible)			(void);
/*! Off=284(71) */	const char					*( *pfnGetGameDirectory )	( void );
/*! Off=288(72) */	Classes::ConsoleVariable				*( *pfnGetCvarPointer )		( const char *szName );
/*! Off=292(73) */	const char					*( *Key_LookupBinding )		( const char *pBinding );
/*! Off=296(74) */	const char					*( *pfnGetLevelName )		( void );
/*! Off=300(75) */	void						( *pfnGetScreenFade )		( screenfade_t *fade );
/*! Off=304(76) */	void						( *pfnSetScreenFade )		( screenfade_t *fade );
/*! Off=308(77) */	VGUI::Panel                *( *VGui_GetPanel )         (void);
/*! Off=312(78) */	void                         ( *VGui_ViewportPaintBackground ) (int extents[4]);

/*! Off=316(79) */	unsigned char				*(*COM_LoadFile)				( char *path, int usehunk, int *pLength );
/*! Off=320(80) */	char						*(*COM_ParseFile)			( char *data, char *token );
/*! Off=324(81) */	void						(*COM_FreeFile)				( void *buffer );

/*! Off=328(82) */	TriangleAPI_t		*triangleAPI;
/*! Off=332(83) */	efx_api_t			*pEfxAPI;
/*! Off=336(84) */	event_api_t			*pEventAPI;
/*! Off=340(85) */	demo_api_t			*pDemoAPI;
/*! Off=344(86) */	net_api_t			*pNetAPI;
/*! Off=348(87) */	IVoiceTweak		*pVoiceTweak;

	// returns 1 if the client is a spectator only (connected to a proxy), 0 otherwise or 2 if in dev_overview mode
/*! Off=352(88) */	int							( *IsSpectateOnly ) ( void );
/*! Off=356(89) */	Model_t				*( *LoadMapSprite )			( const char *filename );

	// file search functions
/*! Off=360(90) */	void						( *COM_AddAppDirectoryToSearchPath ) ( const char *pszBaseDir, const char *appName );
/*! Off=364(91) */	int							( *COM_ExpandFilename)				 ( const char *fileName, char *nameOutBuffer, int nameOutBufferSize );

	// User info
	// playerNum is in the range (1, MaxClients)
	// returns NULL if player doesn't exit
	// returns "" if no value is set
/*! Off=368(92) */	const char					*( *PlayerInfo_ValueForKey )( int playerNum, const char *key );
/*! Off=372(93) */	void						( *PlayerInfo_SetValueForKey )( const char *key, const char *value );

	// Gets a unique ID for the specified player. This is the same even if you see the player on a different server.
	// iPlayer is an entity index, so client 0 would use iPlayer=1.
	// Returns false if there is no player on the server in the specified slot.
/*! Off=376(94) */	BOOL					(*GetPlayerUniqueID)(int iPlayer, char playerID[16u]);

	// TrackerID access
/*! Off=380(95) */	int							(*GetTrackerIDForPlayer)(int playerSlot);
/*! Off=384(96) */	int							(*GetPlayerForTrackerID)(int trackerID);

	// Same as pfnServerCmd, but the message goes in the unreliable stream so it can't clog the net stream (but it might not get there).
/*! Off=388(97) */	int							( *pfnServerCmdUnreliable )( char *szCmdString );

/*! Off=392(98) */	void						( *pfnGetMousePos )( POINT *ppt );
/*! Off=396(99) */	void						( *pfnSetMousePos )( int x, int y );
/*! Off=400(100) */	void						( *pfnSetMouseEnable )( BOOL fEnable );

/*! Off=404(101) */	Classes::ConsoleVariable      *(*pfnCvar_GetFirstVariable)  (void);	// return 'cvar_vars' (Other name: "pfnGetCvarList")
/*! Off=408(102) */	cmd_function_t                *(*pfnCmd_GetFirstCmd)  (void);	// return 'cmd_functions' (Other name: "pfnGetCmdList")
/*! Off=412(103) */	char *const                    (*pfnGetCvarName)  (Classes::ConsoleVariable *const cvar);
/*! Off=416(104) */	char *const                    (*pfnGetCmdName)  (cmd_function_t *const cmd);
/*! Off=420(105) */	double                         (*pfnGetServerTime)  (void);
/*! Off=424(106) */	double                         (*pfnGetGravityValue)  (void);
/*! Off=428(107) */	const Model_t *const           (*pfnPrecacheSprite)  (int spr);	// (OLD name: "pfnfs_precache_timings" - @todo CHECK ME!!!!!!!!!!!!!!!!!!!!!!!)
/*! Off=432(108) */	void                           (*pfnOverrideLightmap)  (int override);
/*! Off=436(109) */	void                           (*pfnSetLightmapColor)  (float r, float g, float b);
/*! Off=440(110) */	void                           (*pfnSetLightmapDarkness) (float dark);
/*! Off=444(111) */	int                            (*pfnGetSequenceByName) (int flags, const char *const seq);	// this will always fail with the current engine
/*! Off=448(112) */	void                           (*pfnSPR_DrawGeneric) (int frame, int x, int y, const wrect_t *prc, int blendsrc, int blenddst, int unknown1, int unknown2);
/*! Off=452(113) */	int                            (*pfnLoadSentence) (const char *Str2, char a2, int *const a3);	// this will always fail with engine, don't call it, cuz actually this function has paramenters but i dunno what they do
/*! Off=456(114) */	int                            (*pfnDrawLocalizedHudString) (int x, int y, const char *const str, int r, int g, int b);	// localizes hud string, uses Legacy font from skin def. Also supports unicode strings
/*! Off=460(115) */	int                            (*pfnDrawLocalizedConsoleString) (int x, int y, const char *const str);	// i can't get this to work for some reason, don't use this
/*! Off=464(116) */	const char                    *(*pfnLocalPlayerInfo_ValueForKey) (const char *key);	// gets keyvalue for local player, useful for querying vgui menus or autohelp
/*! Off=468(117) */	void                           (*pfnDrawText_0) (int x, int y, const char *const text, unsigned long font);	// another vgui2 text drawing function, i dunno how it works. It doesn't localize though
/*! Off=472(118) */	int                            (*pfnDrawUnicodeCharacter) (int x, int y, short number, int r, int g, int b, unsigned long hfont);
/*! Off=476(119) */	unsigned int                   (*pfnCOM_GetApproxWavePlayLength) (const char *filePath);	// checks sound header of a sound file, determines if its a supported type
/*! Off=480(120) */	Classes::Interface::ICareerUI *(*pfnGetCareerUI) (void);	// for condition zero, returns interface from GameUI
/*! Off=484(121) */	void                           (*pfnCvar_Set) (const char *const name, const char *const string);
/*! Off=488(122) */	int                            (*pfnGetCareerState) (void);	// this actually checks for if the CareerGameInterface is found and if a server is being run (Other name: "pfnIsSinglePlayer" ?? - @todo CHECK ME!!!!!!!!!!!!!!!!!!)
/*! Off=492(123) */	void                           (*pfnStartDynamicSound1) (const char *const name, const float volume, const int pitchShift);	// "SendAudio" ?? (Other name: "pfnPlaySound")
/*! Off=496(124) */	void                           (*pfnPlayMp3) (const char *const mp3, int flags);
/*! Off=500(125) */	double                         (*pfnSys_FloatTime) (void);	// get the systems current time as a float

/*! Off=504(126) */	void                           (*pfnSetArray) (int *const array, int size);
/*! Off=508(127) */	void                           (*pfnSetClearArray) (int *const array, int size);
/*! Off=512(128) */	void                           (*pfnClearArray) (void);

/*! Off=516(129) */	void                           (*pfnStartDynamicSound2) (const char *const name, const float volume, const int pitchShift);	// "SendAudio" with sentence ?? (Other name: "pfnPlaySound2")
/*! Off=520(130) */	void                           (*pfnTintRGBA) (int x, int y, int width, int height, int r, int g, int b, int a);
/*! Off=524(131) */	int                            (*pfnUNKNOWN31) (void);
/*! Off=528(132) */	cmdalias_t                    *(*pfnCmd_GetFirstCmdAlias) (void);	// return 'cmd_alias' (Other name: "pfnGetCmdAliasList")
};	// sizeof (ClientEngineFunction_t) == 532 (133)

struct ref_params_t
{
	// Output
/*!off=0(0)*/	Math::Vector3D	vieworg;
/*!off=12(3)*/	Math::Angles3D	viewangles;

/*!off=24(6)*/	Math::Vector3D	forward;
/*!off=36(9)*/	Math::Vector3D	right;
/*!off=48(12)*/	Math::Vector3D   up;

	// Client frametime;
/*!off=60(15)*/	float	frametime;
	// Client time
/*!off=64(16)*/	float	time;

	// Misc
/*!off=68(17)*/	BOOL		intermission;
/*!off=72(18)*/	BOOL		paused;
/*!off=76(19)*/	BOOL		spectator;
/*!off=80(20)*/	int		onground;
/*!off=84(21)*/	int		waterlevel;

/*!off=88(22)*/	Math::Vector3D	simvel;
/*!off=100(25)*/	Math::Vector3D	simorg;

/*!off=112(28)*/	Math::Vector3D	viewheight;
/*!off=124(31)*/	float	idealpitch;

/*!off=128(32)*/	Math::Angles3D	cl_viewangles;

/*!off=140(35)*/	int		health;
/*!off=144(36)*/	Math::Angles3D	crosshairangle;
/*!off=156(39)*/	float	viewsize;

/*!off=160(40)*/	Math::Angles3D	punchangle;
/*!off=172(43)*/	int		maxclients;
/*!off=176(44)*/	int		viewentity;
/*!off=180(45)*/	int		playernum;
/*!off=184(46)*/	int		max_entities;
/*!off=188(47)*/	int		demoplayback;
/*!off=192(48)*/	BOOL		hardware;

/*!off=196(49)*/	int		smoothing;

	// Last issued usercmd
/*!off=200(50)*/	UserCommand_t *cmd;

	// Movevars
/*!off=204(51)*/	PlayerMove_t::Variables_t *movevars;

/*!off=208(52)*/	int		viewport[4];		// the viewport coordinates x, y, width, height

/*!off=224(56)*/	int		nextView;			// the renderer calls ClientDLL_CalcRefdef() and Renderview
								// so long in cycles until this value is 0 (multiple views)
/*!off=228(57)*/	BOOL		onlyClientDraw;		// if != FALSE nothing is drawn by the engine except clientDraw functions
};	// sizeof (ref_params_t) == 232 (58)

struct ClientDLLExportTable_t
{
	BOOL	(*Initialize)( ClientEngineFunction_t *pEnginefuncs, int iVersion );
	void	(*HUD_Init)( void );
	BOOL	(*HUD_VidInit)( void );
	BOOL	(*HUD_Redraw)( float flTime, BOOL intermission );
	BOOL	(*HUD_UpdateClientData)( ClientData_t *cdata, float flTime );
	void	(*HUD_Reset) ( void );
	void	(*HUD_PlayerMove)( PlayerMove_t *ppmove, BOOL server );
	void	(*HUD_PlayerMoveInit)( PlayerMove_t *ppmove );
	char	(*HUD_PlayerMoveTexture)( char *name );
	void	(*IN_ActivateMouse)( void );
	void	(*IN_DeactivateMouse)( void );
	void	(*IN_MouseEvent)(int mstate);
	void	(*IN_ClearStates)(void);
	void	(*IN_Accumulate)(void);
	void	(*CL_CreateMove) ( float frametime, UserCommand_t *cmd, BOOL active );
	BOOL	(*CL_IsThirdPerson)( void );
	void	(*CL_CameraOffset)( Math::Vector3D &ofs );
	struct	kbutton_s *(*KB_Find)( const char *name );
	void	(*CAM_Think)( void );
	void	(*V_CalcRefdef)(ref_params_t *pparams);
	BOOL	(*HUD_AddEntity)(int type, ClientEntity_t *ent, const char *modelname);
	void	(*HUD_CreateEntities)(void);
	void	(*HUD_DrawNormalTriangles)(void);
	void	(*HUD_DrawTransparentTriangles)(void);
	void	(*HUD_StudioEvent)(const mstudioevent_t *, const ClientEntity_t *);
	void	(*HUD_PostRunCmd)( local_state_t *from, local_state_t *to, UserCommand_t *cmd, BOOL runfuncs, double time, unsigned int random_seed);
	void	(*HUD_Shutdown)( void );
	void	(*HUD_TxferLocalOverrides)(EntityState_t *, const ClientData_t *);
	void	(*HUD_ProcessPlayerState)(EntityState_t *, const EntityState_t *);
	void	(*HUD_TxferPredictionData)(EntityState_t *, const EntityState_t *, ClientData_t *, const ClientData_t *, WeaponData_t *, const WeaponData_t *);
	void	(*Demo_ReadBuffer)(int, unsigned char *);
	BOOL	(*HUD_ConnectionlessPacket)( const netadr_t *net_from, const char *args, char *response_buffer, int *response_buffer_size );
	BOOL	(*HUD_GetHullBounds)( PlayerMove_t::UseHullType_t hullNumber, Math::Vector3D &mins, Math::Vector3D &maxs );
	void	(*HUD_Frame)( double time );
	BOOL	(*HUD_Key_Event)( int eventcode, int keynum, const char *pszCurrentBinding );
	void	(*HUD_TempEntUpdate)(double, double, double, struct tempent_s **, struct tempent_s **, int (*Callback_AddVisibleEntity) (ClientEntity_t *), void (*Callback_TempEntPlaySound) (struct tempent_s *pTemp, float damp ));
	ClientEntity_t *(*HUD_GetUserEntity)(int index);
	void	(*HUD_VoiceStatus)(int entindex, BOOL bTalking);
	void	(*HUD_DirectorMessage) (int iSize, void *pbuf);
	int		(*HUD_GetStudioModelInterface)(int, r_studio_interface_t **, EngineStudioAPI_t *);
	void	(*HUD_ChatInputPosition)(int *x, int *y);
	int		(*HUD_GetPlayerTeam)(unsigned int playerIndex);
	unsigned long (*ClientFactory )(void);
};	// sizeof (ClientDLLExportTable_t) == 172 (43)

// Data definition of pThis
typedef struct
{
	double                    m_dummy;

	double                    m_clTime;													// Client clock
	double                    m_clOldTime;												// Old Client clock
	int                       m_fDoInterp;												// Do interpolation?
	int                       m_fGaitEstimation;										// Do gait estimation?
	int                       m_nFrameCount;											// Current render frame #

	// Cvars that studio model code needs to reference
	Classes::ConsoleVariable *m_pCvarHiModels;											// Use high quality models?
	Classes::ConsoleVariable *m_pCvarDeveloper;											// Developer debug output desired?
	Classes::ConsoleVariable *m_pCvarDrawEntities;										// Draw entities bone hit boxes, etc?

	ClientEntity_t           *m_pCurrentEntity;											// The entity which we are currently rendering.
	Model_t                  *m_pRenderModel;											// The model for the entity being rendered
	PlayerInfo_t             *m_pPlayerInfo;											// Player info for current player, if drawing a player
	int                       m_nPlayerIndex;											// The index of the player being drawn
	float                     m_flGaitMovement;											// The player's gait movement
	StudioHeader_t           *m_pStudioHeader;											// Pointer to header block for studio model data

	// Pointers to current body part and submodel
	mstudiobodyparts_t       *m_pBodyPart;
	mstudiomodel_t           *m_pSubModel;

	// Palette substition for top and bottom of model
	int                       m_nTopColor;
	int                       m_nBottomColor;

	Model_t                  *m_pChromeSprite;											// Sprite model used for drawing studio model chrome

	// Caching
	int                       m_nCachedBones;											// Number of bones in bone cache
	char                      m_nCachedBoneNames[Constants::MaximumStudioBones][32];	// Names of cached bones
	// Cached bone & light transformation matrices
	float                     m_rgCachedBoneTransform [Constants::MaximumStudioBones][3][4];
	float                     m_rgCachedLightTransform[Constants::MaximumStudioBones][3][4];

	float                     m_fSoftwareXScale, m_fSoftwareYScale;						// Software renderer scale factors

	// Current view vectors and render origin
	Math::Vector3D            m_vUp;
	Math::Vector3D            m_vRight;
	Math::Vector3D            m_vNormal;

	Math::Vector3D            m_vRenderOrigin;

	int                      *m_pStudioModelCount;										// Model render counters (from engine)
	int                      *m_pModelsDrawn;

	// Matrices
	float                   (*m_protationmatrix)[3][4];									// Model to world transformation
	float                   (*m_paliastransform)[3][4];									// Model to view transformation

	// Concatenated bone and light transforms
	float                   (*m_pbonetransform) [Constants::MaximumStudioBones][3][4];
	float                   (*m_plighttransform) [Constants::MaximumStudioBones][3][4];
} *StudioModelRenderer_d;

enum AxisID_t
{
	AxisID_X,
	AxisID_Y,
	AxisID_Z,
	AxisID_W,

	AxisID_Total
};
typedef float Vector4D_t[AxisID_Total];	// x, y, z, w

struct StudioModelRenderer_t
{
	void (*CStudioModelRendererDestructor)(void);
	void (*Init)(void);
	BOOL (*StudioDrawModel) (Constants::StudioDrawModelFlag_t flags);
	BOOL (*StudioDrawPlayer)(Constants::StudioDrawModelFlag_t flags, EntityState_t *pplayer);
	mstudioanim_t (*StudioGetAnim)( Model_t *m_pSubModel, mstudioseqdesc_t *pseqdesc );
	void (*StudioSetUpTransform) (BOOL trivial_accept);
	void (*StudioSetupBones) ( void );
	void (*StudioCalcAttachments) ( void );
	void (*StudioSaveBones)( void );
	void (*StudioMergeBones)( Model_t *m_pSubModel );
	float (*StudioEstimateInterpolant)( void );
	float (*StudioEstimateFrame)( mstudioseqdesc_t *pseqdesc );
	void (*StudioFxTransform)( ClientEntity_t *ent, float transform[3][4] );
	void (*StudioSlerpBones)( Vector4D_t q1[], Math::Vector3D pos1[], Vector4D_t q2[], Math::Vector3D pos2[], float s );
	void (*StudioCalcBoneAdj) ( float dadt, float *adj, const unsigned char *pcontroller1, const unsigned char *pcontroller2, unsigned char mouthopen );
	void (*StudioCalcBoneQuaterion)( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, Vector4D_t *q );
	void (*StudioCalcBonePosition)( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, Math::Vector3D &pos );
	void (*StudioCalcRotations)( Math::Vector3D pos[], Vector4D_t *q, mstudioseqdesc_t *pseqdesc, mstudioanim_t *panim, float f );
	void (*StudioRenderModel) ( void );
	void (*StudioRenderFinal) (void);
	void (*StudioRenderFinal_Software) ( void );
	void (*StudioRenderFinal_Hardware) ( void );
	void (*StudioPlayerBlend) ( mstudioseqdesc_t *pseqdesc, int *pBlend, float *pPitch );
	void (*StudioEstimateGait) ( EntityState_t *pplayer );
	void (*StudioProcessGait) ( EntityState_t *pplayer );

	bool (*UNKNOWN1) (unsigned int UNKNOWN1, unsigned int UNKNOWN2);
	void (*UNKNOWN2) (unsigned int UNKNOWN);
	void (*UNKNOWN3) (unsigned int UNKNOWN);
};

#endif	// ifndef ENGINE_FUNCTION_INCLUDED