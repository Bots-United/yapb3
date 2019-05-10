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
// SDK_definitions.hpp
//
// Description: Does the major work of calling the original Engine Functions.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SDK_DEFINITIONS_INCLUDED
#define SDK_DEFINITIONS_INCLUDED

namespace Structures
{
	struct Client_t;
	struct ClientEngineFunction_t;
}

namespace Classes
{
	class BasePlayerItem;
	class GameRules;

	class ConsoleVariable;
	class Edict;

	namespace Interface
	{
		class ICareerUI;
	}
}

namespace VGUI
{
	class Panel;
}

namespace TypeDefinitions
{
	#include <Half-Life engine/SDK/TypeDefinitions.hpp>
}

namespace Constants
{
	#include <Half-Life engine/SDK/Constants/Constants_definitions.hpp>
}

namespace Classes
{
	namespace Steam
	{
		#include <Half-Life engine/SDK/Classes/Steam/Definitions.hpp>
	}
}

namespace Structures
{
	#include <Half-Life engine/SDK/Structures/Structures_definitions.hpp>
}

namespace Classes
{
	#include <Half-Life engine/SDK/Classes/Classes_definitions.hpp>
}

namespace Voice
{
	#include <Half-Life engine/SDK/Voice/Voice definitions.hpp>
}

namespace VGUI
{
	#include <Half-Life engine/SDK/VGUI/Definitions.hpp>
}

namespace Classes
{
	namespace Interface
	{
		#include <Half-Life engine/SDK/Classes/Interfaces/Interface_GameConsole.hpp>	// For 'VGUI::VPANEL' typedef.
	}
}

namespace TypeDefinitions
{
	typedef unsigned char GameMessageBuffer_t[Constants::MaximumUserMessageSize];

	typedef Math::Matrix3x4 RotationMatrix_t;
	typedef Math::Matrix3x4 BoneTransformMatrix_t[Constants::MaximumStudioBones];

	typedef void (CALLBACK *FunctionPointer_t)       (Structures::EngineFunction_t *engineFunctionTable, Structures::GlobalVariables_t *globalVariables);
	typedef BOOL (FAR      *EntityAPI_t)             (Structures::DLLFunction_t    *DLLFunctionTable,    int  interfaceVersion);
	typedef BOOL (FAR      *NewEntityAPI_t)          (Structures::NewDLLFunction_t *newDLLFunctionTable, int *interfaceVersion);
	typedef BOOL (FAR      *BlendAPI_t)              (int version, Structures::ServerBlendingInterface_t **blendInterface, Structures::ServerStudioAPI_t *studio, RotationMatrix_t *const rotationMatrix, BoneTransformMatrix_t *const boneTransform);

	typedef void (FAR      *EntityFunctionPointer_t) (Structures::EntityVariables_t *entityVariables);
}

namespace Network
{
inline const bool NET_IsReservedAdr (const Structures::netadr_t &address)
{
	// reserved addresses are not routeable, so they can all be used in a LAN game

	if (address.type == Structures::netadr_t::NA_LOOPBACK)
		return true;

	if (address.type != Structures::netadr_t::NA_IP)
		return false;

	return
	(
		address.ip[0u] == 10u ||														// 10.x.x.x is reserved
		address.ip[0u] == 127u ||														// 127.x.x.x
		(address.ip[0u] == 172u && address.ip[1u] >= 16u && address.ip[1u] <= 31u) ||	// 172.16.x.x  - 172.31.x.x
		(address.ip[0u] == 192u && address.ip[1u] >= 168u)								// 192.168.x.x
	);
}
}

#endif	// ifndef SDK_DEFINITIONS_INCLUDED