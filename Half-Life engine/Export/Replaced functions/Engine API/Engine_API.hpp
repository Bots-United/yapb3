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
// Engine_API.hpp
//
// Description: Prototypes for Half-Life engine functions.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENGINE_API_INCLUDED
#define ENGINE_API_INCLUDED

void                pfnSetModel      (SDK::Classes::Edict *entity, const char *modelName);
void                pfnChangeLevel   (const char *const s1, const char *const s2);
void                pfnEmitSound     (SDK::Classes::Edict *entity, SDK::Constants::SoundChannel_t channel, const char *sample, float volume, float attenuation, SDK::Constants::SoundFlag_t flags, SDK::Constants::SoundPitch_t pitch);
void                pfnClientCommand (SDK::Classes::Edict *client, char *format, ...);
void                pfnLightStyle    (const int style, char *const value);
void                pfnMessageBegin  (const SDK::Constants::MessageDestination_t destination, const unsigned int type, const Math::Vector3D *const origin, SDK::Classes::Edict *const to);
void                pfnMessageEnd    (void);
void                pfnWriteByte     (int value);
void                pfnWriteChar     (int value);
void                pfnWriteShort    (int value);
void                pfnWriteLong     (int value);
void                pfnWriteAngle    (float value);
void                pfnWriteCoord    (float value);
void                pfnWriteString   (const char *const string);
void                pfnWriteEntity   (int value);
int                 pfnRegUserMsg    (const char *name, int size);
void                pfnClientPrintf  (SDK::Classes::Edict *client, SDK::Constants::PrintType_t printType, const char *message);
const char         *pfnCmd_Args      (void);
const char         *pfnCmd_Argv      (unsigned int argc);
const unsigned int  pfnCmd_Argc      (void);
void                pfnSetView       (const SDK::Classes::Edict *const client, const SDK::Classes::Edict *const viewEntity);

#endif	// ifndef ENGINE_API_INCLUDED