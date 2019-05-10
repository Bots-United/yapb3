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
// StudioInt.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_STUDIO_INT_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_STUDIO_INT_INCLUDED

struct EngineStudioAPI_t
{
	#define STUDIO_INTERFACE_VERSION 1

	// Allocate number*size bytes and zero it
	void			*( *Mem_Calloc )				( int number, size_t size );
	// Check to see if pointer is in the cache
	void			*( *Cache_Check )				( CacheUser_t *c );
	// Load file into cache ( can be swapped out on demand )
	void			( *LoadCacheFile )				( const char *path, CacheUser_t *cu );
	// Retrieve model pointer for the named model
	Model_t	*( *Mod_ForName )				( const char *name, BOOL crash_if_missing );
	// Retrieve pointer to studio model data block from a model
	StudioHeader_t			*( *Mod_Extradata )				( Model_t *mod );
	// Retrieve indexed model from client side model precache list
	Model_t	*( *GetModelByIndex )			( int index );
	// Get entity that is set for rendering
	ClientEntity_t * ( *GetCurrentEntity )		( void );
	// Get referenced PlayerInfo_t
	PlayerInfo_t *( *PlayerInfo )			( int index );
	// Get most recently received player state data from network system
	EntityState_t *( *GetPlayerState )		( int index );
	// Get viewentity
	ClientEntity_t * ( *GetViewEntity )			( void );
	// Get current frame count, and last two timestampes on client
	void			( *GetTimes )					( int *framecount, double *current, double *old );
	// Get a pointer to a cvar by name
	Classes::ConsoleVariable	*( *GetCvar )					( const char *name );
	// Get current render origin and view vectors ( up, right and vpn )
	void			( *GetViewInfo )				( Math::Vector3D &origin, Math::Vector3D *upv, Math::Vector3D *rightv, Math::Vector3D *vpnv );
	// Get sprite model used for applying chrome effect
	Model_t	*( *GetChromeSprite )			( void );
	// Get model counters so we can incement instrumentation
	void			( *GetModelCounters )			( int **s, int **a );
	// Get software scaling coefficients
	void			( *GetAliasScale )				( float *x, float *y );

	// Get bone, light, alias, and rotation matrices
	float			****( *StudioGetBoneTransform ) ( void );
	float			****( *StudioGetLightTransform )( void );
	float			***( *StudioGetAliasTransform ) ( void );
	float			***( *StudioGetRotationMatrix ) ( void );

	// Set up body part, and get submodel pointers
	void			( *StudioSetupModel )			( int bodypart, mstudiobodyparts_t **ppbodypart, mstudiomodel_t **ppsubmodel );
	// Check if entity's bbox is in the view frustum
	BOOL				( *StudioCheckBBox )			( void );
	// Apply lighting effects to model
	void			( *StudioDynamicLight )			( ClientEntity_t *ent, alight_t *plight );
	void			( *StudioEntityLight )			( alight_t *plight );
	void			( *StudioSetupLighting )		( alight_t *plighting );

	// Draw mesh vertices
	void			( *StudioDrawPoints )			( void );

	// Draw hulls around bones
	void			( *StudioDrawHulls )			( void );
	// Draw bbox around studio models
	void			( *StudioDrawAbsBBox )			( void );
	// Draws bones
	void			( *StudioDrawBones )			( void );
	// Loads in appropriate texture for model
	void			( *StudioSetupSkin )			( mstudiotexture_t *ptexturehdr, int index );
	// Sets up for remapped colors
	void			( *StudioSetRemapColors )		( int top, int bottom );
	// Set's player model and returns model pointer
	Model_t	*( *SetupPlayerModel )			( int index );
	// Fires any events embedded in animation
	void			( *StudioClientEvents )			( void );
	// Retrieve/set forced render effects flags
	Constants::STUDIO_NF_t				( *GetForceFaceFlags )			( void );
	void			( *SetForceFaceFlags )			( Constants::STUDIO_NF_t flags );
	// Tell engine the value of the studio model header
	void			( *StudioSetHeader )			( StudioHeader_t *header );
	// Tell engine which Model_t * is being renderered
	void			( *SetRenderModel )				( Model_t *model );

	// Final state setup and restore for rendering
	void			( *SetupRenderer )				( Constants::RenderMode_t rendermode );
	void			( *RestoreRenderer )			( void );

	// Set render origin for applying chrome effect
	void			( *SetChromeOrigin )			( void );

	// True if using D3D/OpenGL
	BOOL				( *IsHardware )					( void );

	// Only called by hardware interface
	void			( *GL_StudioDrawShadow )		( void );
	void			( *GL_SetRenderMode )			( Constants::RenderMode_t mode );

	unsigned int (*UNKNOWN1) (unsigned int UNKNOWN);
	void         (*UNKNOWN2) (unsigned int UNKNOWN);
	void         (*UNKNOWN3) (unsigned int UNKNOWN1, unsigned int UNKNOWN2, unsigned int UNKNOWN3, unsigned int UNKNOWN4, unsigned int UNKNOWN5);
};	// sizeof (EngineStudioAPI_t) == 184 (46)

struct ServerStudioAPI_t
{
	// Allocate number*size bytes and zero it
	CacheUser_t			*( *Mem_Calloc )				( int number, size_t size );
	// Check to see if pointer is in the cache
	CacheUser_t			*( *Cache_Check )				( CacheUser_t *c );
	// Load file into cache ( can be swapped out on demand )
	void			( *LoadCacheFile )				( const char *path, CacheUser_t *cu );
	// Retrieve pointer to studio model data block from a model
	StudioHeader_t			*( *Mod_Extradata )				( Model_t *mod );
};

// client blending
struct r_studio_interface_t
{
	int				version;

	BOOL				( *StudioDrawModel	)			( Constants::StudioDrawModelFlag_t flags );
	BOOL				( *StudioDrawPlayer	)			( Constants::StudioDrawModelFlag_t flags, EntityState_t *pplayer );
};

struct ServerBlendingInterface_t
{
	// server blending
	#define SV_BLENDING_INTERFACE_VERSION 1

	int	version;

	void (*SV_StudioSetupBones) (Model_t *const model, const float frame, const int sequence, const Math::Angles3D &angles, const Math::Vector3D &origin, const unsigned char *const controller, const unsigned char *const blending, const int boneID, const Classes::Edict *const edict);
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_STUDIO_INT_INCLUDED