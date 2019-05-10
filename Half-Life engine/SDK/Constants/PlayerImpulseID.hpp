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
// PlayerImpulseID.hpp
//
// Description: Counter-Strike player impulse ID's.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLAYER_IMPULSE_ID_INCLUDED
#define PLAYER_IMPULSE_ID_INCLUDED

enum PlayerImpulseID_t
{
	PlayerImpulseID_None                = 0u,	// None.
	PlayerImpulseID_CreateMonster       = 76u,	// Creates a monster human grunt.
	PlayerImpulseID_Logo                = 99u,	// Shows/removes logo.
	PlayerImpulseID_ToggleFlashlight    = 100u,	// Turns flishlight on/off.
	PlayerImpulseID_GiveMoney           = 101u,	// Gives $16000.
	PlayerImpulseID_Gibbage             = 102u,	// Gibbage!!!
	PlayerImpulseID_PrintMonsterState   = 103u,	// Prints monster state on which you looking. (HLSDK: What the hell are you doing?)
	PlayerImpulseID_DumpGlobalState     = 104u,	// Dump all of the global state varaibles (and global entity names).
	PlayerImpulseID_ToggleSounds        = 105u,	// Player makes no sound for monsters to hear.
	PlayerImpulseID_PrintEntityNames    = 106u,	// Prints class and target names of entity which you looking. (HLSDK: Give me the class name and target name of this entity)
	PlayerImpulseID_PrintTextureName    = 107u,	// Prints texture name on which you looking.
	PlayerImpulseID_CreateNodeFly       = 195u,	// Show shortest paths for entire level to nearest node.
	PlayerImpulseID_CreateNodeLarge     = 196u,	// Show shortest paths for entire level to nearest node.
	PlayerImpulseID_CreateNodeHuman     = 197u,	// Show shortest paths for entire level to nearest node.
	PlayerImpulseID_ShowNodeConnections = 199u,	// Show nearest node and all connections.
	PlayerImpulseID_PaintDecal          = 201u,	// Paints decal on a clooser wall.
	PlayerImpulseID_SpawnBloodSplatter  = 202u,	// Random blood splatter.
	PlayerImpulseID_RemoveEntity        = 203u	// Remove creature.
};

#endif	// ifndef PLAYER_IMPULSE_ID_INCLUDED