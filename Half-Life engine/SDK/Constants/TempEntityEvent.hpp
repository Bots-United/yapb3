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
// TempEntityEvent.hpp
//
// Description: Temp entity events.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TEMP_ENTITY_EVENT_INCLUDED
#define TEMP_ENTITY_EVENT_INCLUDED

enum TempEntityEvent_t
{
	// beam effect between two points
	TempEntityEvent_BeamPoints,
	// coord, coord, coord (start position)
	// coord, coord, coord (end position)
	// short (sprite index)
	// byte (starting frame)
	// byte (frame rate in 0.1's)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte (noise amplitude in 0.01's)
	// byte, byte, byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	// beam effect between point and entity
	TempEntityEvent_BeamEntityPoint,
	// short (start entity)
	// coord, coord, coord (end position)
	// short (sprite index)
	// byte (starting frame)
	// byte (frame rate in 0.1's)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte (noise amplitude in 0.01's)
	// byte, byte, byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	// particle effect plus ricochet sound
	TempEntityEvent_GunShot,
	// coord, coord, coord (position)

	// additive sprite, 2 dynamic lights, flickering particles, explosion sound, move vertically 8 pps
	TempEntityEvent_Explosion,
	// coord, coord, coord (position)
	// short (sprite index)
	// byte (scale in 0.1's)
	// byte (framerate)
	// byte (flags)

	// quake 1 "tarbaby" explosion with sound
	TempEntityEvent_TarExplosion,
	// coord, coord, coord (position)

	// alphablend sprite, move vertically 30 pps
	TempEntityEvent_Smoke,
	// coord, coord, coord (position)
	// short (sprite index)
	// byte (scale in 0.1's)
	// byte (framerate)

	// tracer effect from point to point
	TempEntityEvent_Tracer,
	// coord, coord, coord (start)
	// coord, coord, coord (end)

	// TempEntityEvent_BeamPoints with simplified parameters
	TempEntityEvent_Lightning,
	// coord, coord, coord (start)
	// coord, coord, coord (end)
	// byte (life in 0.1's)
	// byte (width in 0.1's)
	// byte (amplitude in 0.01's)
	// short (sprite model index)

	TempEntityEvent_BeamEntities,
	// short (start entity)
	// short (end entity)
	// short (sprite index)
	// byte (starting frame)
	// byte (frame rate in 0.1's)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte (noise amplitude in 0.01's)
	// byte, byte, byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	// 8 random tracers with gravity, ricochet sprite
	TempEntityEvent_Sparks,
	// coord, coord, coord (position)

	// quake 1 lava splash
	TempEntityEvent_LavaSplash,
	// coord, coord, coord (position)

	// quake 1 teleport splash
	TempEntityEvent_Teleport,
	// coord, coord, coord (position)

	// quake 1 colormaped (base palette) particle explosion with sound
	TempEntityEvent_Explosion2,
	// coord, coord, coord (position)
	// byte (starting color)
	// byte (num colors)

	// decal from the .BSP file
	TempEntityEvent_BSPDecal,
	// coord, coord, coord (x, y, z), decal position (center of texture in world)
	// short (texture index of precached decal texture name)
	// short (entity index)
	// [optional - only included if previous short is non-zero (not the world)] short (index of model of above entity)

	// tracers moving toward a point
	TempEntityEvent_Implosion,
	// coord, coord, coord (position)
	// byte (radius)
	// byte (count)
	// byte (life in 0.1's)

	// line of moving glow sprites with gravity, fadeout, and collisions
	TempEntityEvent_SpriteTrail,
	// coord, coord, coord (start)
	// coord, coord, coord (end)
	// short (sprite index)
	// byte (count)
	// byte (life in 0.1's)
	// byte (scale in 0.1's)
	// byte (velocity along vector in 10's)
	// byte (randomness of velocity in 10's)

	// obsolete
	TempEntityEvent_Beam,

	// additive sprite, plays 1 cycle
	TempEntityEvent_Sprite,
	// coord, coord, coord (position)
	// short (sprite index)
	// byte (scale in 0.1's)
	// byte (brightness)

	// a beam with a sprite at the end
	TempEntityEvent_BeamSprite,
	// coord, coord, coord (start position)
	// coord, coord, coord (end position)
	// short (beam sprite index)
	// short (end sprite index)

	// screen aligned beam ring, expands to max radius over lifetime
	TempEntityEvent_BeamTorus,
	// coord, coord, coord (center position)
	// coord, coord, coord (axis and radius)
	// short (sprite index)
	// byte (starting frame)
	// byte (frame rate in 0.1's)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte (noise amplitude in 0.01's)
	// byte, byte, byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	// disk that expands to max radius over lifetime
	TempEntityEvent_BeamDisk,
	// coord, coord, coord (center position)
	// coord, coord, coord (axis and radius)
	// short (sprite index)
	// byte (starting frame)
	// byte (frame rate in 0.1's)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte (noise amplitude in 0.01's)
	// byte, byte, byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	// cylinder that expands to max radius over lifetime
	TempEntityEvent_BeamCylinder,
	// coord, coord, coord (center position)
	// coord, coord, coord (axis and radius)
	// short (sprite index)
	// byte (starting frame)
	// byte (frame rate in 0.1's)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte (noise amplitude in 0.01's)
	// byte, byte, byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	// create a line of decaying beam segments until entity stops moving
	TempEntityEvent_BeamFollow,
	// short (entity:attachment to follow)
	// short (sprite index)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte, byte, byte (color)
	// byte (brightness)

	TempEntityEvent_GlowSprite,
	// coord, coord, coord (pos) short (model index) byte (scale / 10)

	// connect a beam ring to two entities
	TempEntityEvent_BeamRing,
	// short (start entity)
	// short (end entity)
	// short (sprite index)
	// byte (starting frame)
	// byte (frame rate in 0.1's)
	// byte (life in 0.1's)
	// byte (line width in 0.1's)
	// byte (noise amplitude in 0.01's)
	// byte, byte, byte (color)
	// byte (brightness)
	// byte (scroll speed in 0.1's)

	// oriented shower of tracers
	TempEntityEvent_StreakSplash,
	// coord, coord, coord (start position)
	// coord, coord, coord (direction vector)
	// byte (color)
	// short (count)
	// short (base speed)
	// short (ramdon velocity)

	// obsolete
	TempEntityEvent_BeamHose,

	// dynamic light, effect world, minor entity effect
	TempEntityEvent_DdynamicLight,
	// coord, coord, coord (pos)
	// byte (radius in 10's)
	// byte byte byte (color)
	// byte (brightness)
	// byte (life in 10's)
	// byte (decay rate in 10's)

	// point entity light, no world effect
	TempEntityEvent_EntityLight,
	// short (entity: attachment to follow)
	// coord, coord, coord (initial position)
	// coord (radius)
	// byte byte byte (color)
	// byte (life in 0.1's)
	// coord (decay rate)

	TempEntityEvent_TextMessage,
	// short 1.2.13 x (-1 = center)
	// short 1.2.13 y (-1 = center)
	// byte effect 0 = fade in/fade out
	// 1 is flickery credits
	// 2 is write out (training room)
	// 4 bytes r, g, b, a color1 (text color)
	// 4 bytes r, g, b, a color2 (effect color)
	// ushort 8.8 fadein time
	// ushort 8.8 fadeout time
	// ushort 8.8 hold time
	// optional ushort 8.8 fxtime (time the highlight lags behing the leading text in effect 2)
	// string text message (512 chars max sz string)

	TempEntityEvent_Line,
	// coord, coord, coord start position
	// coord, coord, coord end position
	// short life in 0.1 s
	// 3 bytes r, g, b

	TempEntityEvent_Box,
	// coord, coord, coord	boxmins
	// coord, coord, coord	boxmaxs
	// short life in 0.1 s
	// 3 bytes r, g, b

	// kill all beams attached to entity
	TempEntityEvent_KillBeam = 99u,
	// short (entity)

	TempEntityEvent_LargeFunnel,
	// coord, coord, coord (funnel position)
	// short (sprite index)
	// short (flags)

	// particle spray
	TempEntityEvent_BloodStream,
	// coord, coord, coord (start position)
	// coord, coord, coord (spray vector)
	// byte (color)
	// byte (speed)

	// line of particles every 5 units, dies in 30 seconds
	TempEntityEvent_ShowLine,
	// coord, coord, coord (start position)
	// coord, coord, coord (end position)

	// particle spray
	TempEntityEvent_Blood,
	// coord, coord, coord (start position)
	// coord, coord, coord (spray vector)
	// byte (color)
	// byte (speed)

	// decal applied to a brush entity (not the world)
	TempEntityEvent_Decal,
	// coord, coord, coord (x, y, z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name)
	// short (entity index)

	// create alpha sprites inside of entity, float upwards
	TempEntityEvent_Fizz,
	// short (entity)
	// short (sprite index)
	// byte (density)

	// create a moving model that bounces and makes a sound when it hits
	TempEntityEvent_Model,
	// coord, coord, coord (position)
	// coord, coord, coord (velocity)
	// angle (initial yaw)
	// short (model index)
	// byte (bounce sound type)
	// byte (life in 0.1's)

	// spherical shower of models, picks from set
	TempEntityEvent_ExplodeModel,
	// coord, coord, coord (origin)
	// coord (velocity)
	// short (model index)
	// short (count)
	// byte (life in 0.1's)

	// box of models or sprites
	TempEntityEvent_BreakModel,
	// coord, coord, coord (position)
	// coord, coord, coord (size)
	// coord, coord, coord (velocity)
	// byte (random velocity in 10's)
	// short (sprite or model index)
	// byte (count)
	// byte (life in 0.1 secs)
	// byte (flags)

	// decal and ricochet sound
	TempEntityEvent_GunShotDecal,
	// coord, coord, coord (position)
	// short (entity index???)
	// byte (decal???)

	// spay of alpha sprites
	TempEntityEvent_SpriteSpray,
	// coord, coord, coord (position)
	// coord, coord, coord (velocity)
	// short (sprite index)
	// byte (count)
	// byte (speed)
	// byte (noise)

	// quick spark sprite, client ricochet sound
	TempEntityEvent_ArmorRicochet,
	// coord, coord, coord (position)
	// byte (scale in 0.1's)

	// ???
	TempEntityEvent_PlayerDecal,
	// byte (player index)
	// coord, coord, coord (position)
	// short (entity???)
	// byte (decal number???)
	// [optional] short (model index???)

	// create alpha sprites inside of box, float upwards
	TempEntityEvent_Bubbles,
	// coord, coord, coord (min start position)
	// coord, coord, coord (max start position)
	// coord (float height)
	// short (model index)
	// byte (count)
	// coord (speed)

	// create alpha sprites along a line, float upwards
	TempEntityEvent_BubbleTrail,
	// coord, coord, coord (min start position)
	// coord, coord, coord (max start position)
	// coord (float height)
	// short (model index)
	// byte (count)
	// coord (speed)

	// spray of opaque sprite1's that fall, single sprite2 for 1..2 secs (this is a high-priority tent)
	TempEntityEvent_BloodSprite,
	// coord, coord, coord (position)
	// short (sprite1 index)
	// short (sprite2 index)
	// byte (color)
	// byte (scale)

	// decal applied to the world brush
	TempEntityEvent_WorldDecal,
	// coord, coord, coord (x, y, z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name)

	// decal (with texture index > 256) applied to world brush
	TempEntityEvent_WorldDecalHigh,
	// coord, coord, coord (x, y, z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name - 256)

	// same as TempEntityEvent_Decal, but the texture index was greater than 256
	TempEntityEvent_DecalHigh,
	// coord, coord, coord (x, y, z), decal position (center of texture in world)
	// byte (texture index of precached decal texture name - 256)
	// short (entity index)

	// makes a projectile (like a nail) (this is a high-priority tent)
	TempEntityEvent_Projectile,
	// coord, coord, coord (position)
	// coord, coord, coord (velocity)
	// short (model index)
	// byte (life)
	// byte (owner) projectile won't collide with owner (if owner == 0, projectile will hit any client).

	// throws a shower of sprites or models
	TempEntityEvent_Spray,
	// coord, coord, coord (position)
	// coord, coord, coord (direction)
	// short (model index)
	// byte (count)
	// byte (speed)
	// byte (noise)
	// byte (render mode)

	// sprites emit from a player's bounding box (ONLY use for players!)
	TempEntityEvent_PlayerSprites,
	// byte (player num)
	// short (sprite model index)
	// byte (count)
	// byte (variance) (0 = no variance in size) (10 = 10% variance in size)

	// very similar to lavasplash
	TempEntityEvent_ParticleBurst,
	// coord (origin)
	// short (radius)
	// byte (particle color)
	// byte (duration * 10) (will be randomized a bit)

	// makes a field of fire
	TempEntityEvent_FireField,
	// coord (origin)
	// short (radius) (fire is made in a square around origin. -radius, -radius to radius, radius)
	// short (modelIndex)
	// byte (count)
	// byte (flags)
	// byte (duration (in seconds) * 10) (will be randomized a bit)

	// attaches a TENT to a player (this is a high-priority tent)
	TempEntityEvent_PlayerAttachment,
	// byte (entity index of player)
	// coord (vertical offset) (attachment origin.z = player origin.z + vertical offset)
	// short (model index)
	// short (life * 10);

	// will expire all TENTS attached to a player
	TempEntityEvent_KillPlayerAttachments,
	// byte (entity index of player)

	// much more compact shotgun message
	TempEntityEvent_MultiGunShot,
	// This message is used to make a client approximate a 'spray' of gunfire.
	// Any weapon that fires more than one bullet per frame and fires in a bit of a spread is a good candidate for MULTIGUNSHOT use. (shotguns)
	//
	// NOTE: This effect makes the client do traces for each bullet, these client traces ignore entities that have studio models. Traces are 4096 long.
	//
	// coord (origin)
	// coord (origin)
	// coord (origin)
	// coord (direction)
	// coord (direction)
	// coord (direction)
	// coord (x noise * 100)
	// coord (y noise * 100)
	// byte (count)
	// byte (bullethole decal texture index)

	// larger message than the standard tracer, but allows some customization
	TempEntityEvent_UseRTracer
	// coord (origin)
	// coord (origin)
	// coord (origin)
	// coord (velocity)
	// coord (velocity)
	// coord (velocity)
	// byte (life * 10)
	// byte (color) this is an index into an array of color vectors in the engine. (0 -)
	// byte (length * 10)
};

#endif	// ifndef TEMP_ENTITY_EVENT_INCLUDED