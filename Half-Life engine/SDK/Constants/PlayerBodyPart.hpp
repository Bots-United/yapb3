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
// PlayerBodyPart.hpp
//
// Description: Counter-Strike player parts of body for hits.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLAYER_BODY_PART_INCLUDED
#define PLAYER_BODY_PART_INCLUDED

enum PlayerBodyPart_t
{
	PlayerBodyPart_Generic,		// None.

	PlayerBodyPart_Head,		// 1 << 1 = 2
	PlayerBodyPart_Chest,		// 1 << 2 = 4
	PlayerBodyPart_Stomach,		// 8
	PlayerBodyPart_LeftArm,		// 16
	PlayerBodyPart_RightArm,	// 32
	PlayerBodyPart_LeftLeg,		// 64
	PlayerBodyPart_RightLeg,	// 128

	PlayerBodyPart_Shield		// 256
};

DECLARE_TYPED_ENUMERATION (unsigned short, PlayerBodyPartBit_t)
{
	PlayerBodyPartBit_Generic  = BIT (PlayerBodyPart_Generic)/* OR 0??????????????*/,	// None.

	PlayerBodyPartBit_Head     = BIT (PlayerBodyPart_Head),		// 1 << 1 = 2
	PlayerBodyPartBit_Chest    = BIT (PlayerBodyPart_Chest),	// 1 << 2 = 4
	PlayerBodyPartBit_Stomach  = BIT (PlayerBodyPart_Stomach),	// 8
	PlayerBodyPartBit_LeftArm  = BIT (PlayerBodyPart_LeftArm),	// 16
	PlayerBodyPartBit_RightArm = BIT (PlayerBodyPart_RightArm),	// 32
	PlayerBodyPartBit_LeftLeg  = BIT (PlayerBodyPart_LeftLeg),	// 64
	PlayerBodyPartBit_RightLeg = BIT (PlayerBodyPart_RightLeg),	// 128

	PlayerBodyPartBit_Shield   = BIT (PlayerBodyPart_Shield)	// 256
};

#endif	// ifndef PLAYER_BODY_PART_INCLUDED