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
// Functions.hpp
//
// Description: From HLSDK dlls/cbase.h
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef EXPORT_FUNCTIONS_INCLUDED
#define EXPORT_FUNCTIONS_INCLUDED

// Standard HL SDK interface function prototypes.
EXPORT BOOL GetEntityAPI2           (SDK::Structures::DLLFunction_t *functionTable, int *interfaceVersion);
EXPORT BOOL GetEntityAPI2_Post      (SDK::Structures::DLLFunction_t *functionTable, int *interfaceVersion);

// Additional SDK-like interface function prototypes.
EXPORT BOOL GetNewDLLFunctions      (SDK::Structures::NewDLLFunction_t *newDLLFunctionTable, int *interfaceVersion);
EXPORT BOOL GetEngineFunctions      (SDK::Structures::EngineFunction_t *functionTable, int *interfaceVersion);
EXPORT BOOL GetEngineFunctions_Post (SDK::Structures::EngineFunction_t *functionTable, int *interfaceVersion);

EXPORT BOOL Server_GetBlendingInterface (int version, SDK::Structures::ServerBlendingInterface_t **blendInterface, SDK::Structures::ServerStudioAPI_t *studio, SDK::TypeDefinitions::RotationMatrix_t *const rotationMatrix, SDK::TypeDefinitions::BoneTransformMatrix_t *const boneTransform);

#endif	// ifndef EXPORT_FUNCTIONS_INCLUDED