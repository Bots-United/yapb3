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
// Structures_definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef STRUCTURES_DEFINITIONS_INCLUDED
#define STRUCTURES_DEFINITIONS_INCLUDED

// Some forward-declarations and also just a void, unnecessary structures....
struct Delta_t;
struct SequenceEntry_t;
struct SentenceEntry_t;

struct Model_t;
struct ModelTraceResult_t;

#include <Half-Life engine/SDK/structures/BaseParseFunction.hpp>
#include <Half-Life engine/SDK/structures/DBoundingBox.hpp>
#include <Half-Life engine/SDK/structures/DClipNode.hpp>
#include <Half-Life engine/SDK/structures/DEdge.hpp>
#include <Half-Life engine/SDK/structures/DFace.hpp>
#include <Half-Life engine/SDK/structures/DLeaf.hpp>
#include <Half-Life engine/SDK/structures/DNode.hpp>
#include <Half-Life engine/SDK/structures/DPlane.hpp>
#include <Half-Life engine/SDK/structures/DTexture.hpp>
#include <Half-Life engine/SDK/structures/DTextureInformation.hpp>
#include <Half-Life engine/SDK/structures/DTextureLump.hpp>
#include <Half-Life engine/SDK/structures/DVertex.hpp>
#include <Half-Life engine/SDK/structures/DemoFileHeader.hpp>
#include <Half-Life engine/SDK/structures/DModel.hpp>
#include <Half-Life engine/SDK/structures/BSPMapFileHeader.hpp>
#include <Half-Life engine/SDK/structures/Link.hpp>
#include <Half-Life engine/SDK/structures/EdictAreaNode.hpp>
#include <Half-Life engine/SDK/structures/EntityState.hpp>
#include <Half-Life engine/SDK/structures/EntityVariables.hpp>
#include <Half-Life engine/SDK/structures/EventInformation.hpp>
#include <Half-Life engine/SDK/structures/EventState.hpp>

namespace GCFFile
{
	#include <Half-Life engine/SDK/structures/GCFFile.hpp>
}

#include <Half-Life engine/SDK/structures/SaveRestoreData.hpp>
#include <Half-Life engine/SDK/structures/LockSound.hpp>
#include <Half-Life engine/SDK/structures/key_value_data.hpp>
#include <Half-Life engine/SDK/structures/PhysEntity.hpp>
#include <Half-Life engine/SDK/structures/MPlane.hpp>
#include <Half-Life engine/SDK/structures/Hull.hpp>
#include <Half-Life engine/SDK/structures/TypeDescription.hpp>
#include <Half-Life engine/SDK/structures/MapCycle.hpp>
#include <Half-Life engine/SDK/structures/Model.hpp>
#include <Half-Life engine/SDK/structures/PMTraceResult.hpp>
#include <Half-Life engine/SDK/structures/UserCommand.hpp>
#include <Half-Life engine/SDK/structures/Resource.hpp>
#include <Half-Life engine/SDK/structures/Customization.hpp>
#include <Half-Life engine/SDK/structures/PlayerInfo.hpp>
#include <Half-Life engine/SDK/structures/Server.hpp>
#include <Half-Life engine/SDK/structures/ClientLightStyle.hpp>
#include <Half-Life engine/SDK/structures/Studio.hpp>
#include <Half-Life engine/SDK/structures/StudioInt.hpp>
#include <Half-Life engine/SDK/structures/PlayerMove.hpp>
#include <Half-Life engine/SDK/structures/trace_result.hpp>
#include <Half-Life engine/SDK/structures/GlobalVariables.hpp>
#include <Half-Life engine/SDK/structures/UserMessages.hpp>

// Defines entity interface between engine and DLL's. This header file included by engine files and DLL files.
#include <Half-Life engine/SDK/structures/DLL_function.hpp>
#include <Half-Life engine/SDK/structures/engine_function.hpp>
#include <Half-Life engine/SDK/structures/new_DLL_function.hpp>

struct AllDLLFunction_t : DLLFunction_t, NewDLLFunction_t { /* Empty */ };	//! @todo USE THIS INSTEAD MetaModEngine::SDK::Structures::GameDLLFunction_t?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?

// Reliability checks....
CompileTimeAssert (sizeof (Server_t) == 287768u);
CompileTimeAssert (sizeof (Client_t) == 20488u);
CompileTimeAssert (sizeof (ServerStatic_t) == 136u/*132u*/);
CompileTimeAssert (sizeof (ClientEntity_t) == 3000u);

#endif	// ifndef STRUCTURES_DEFINITIONS_INCLUDED