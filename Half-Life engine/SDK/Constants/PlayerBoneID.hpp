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
// PlayerBoneID.hpp
//
// Description: Counter-Strike player bone ID's. (for pfnGetBonePosition())
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PLAYER_BONE_ID_INCLUDED
#define PLAYER_BONE_ID_INCLUDED

// (gsg9)
enum PlayerBoneID_t
{
	PlayerBoneID_1 = 1u,		// Name: "Bip01"
	PlayerBoneID_Pelvis,		// Name: "Bip01 Pelvis" (bone number for the "pelvis" bone of players (bottom of spine))
	PlayerBoneID_Spine,			// Name: "Bip01 Spine" (bone number for the "spine" bone of players (bottom 1/4 of spine))
	PlayerBoneID_Spine1,		// Name: "Bip01 Spine1" (bone number for the "spine1" bone of players (half of spine))
	PlayerBoneID_Spine2,		// Name: "Bip01 Spine2" (bone number for the "spine2" bone of players (bottom 3/4 of spine))
	PlayerBoneID_Spine3,		// Name: "Bip01 Spine3" (bone number for the "spine3" bone of players (top of spine, between clavicles))
	PlayerBoneID_Neck,			// Name: "Bip01 Neck" (bone number for the "neck" bone of players (bottom of neck))
	PlayerBoneID_Head,			// Name: "Bip01 Head" (bone number for the "head" bone of players (center of neck))
	PlayerBoneID_9,				// Name: "Bone01"
	PlayerBoneID_LeftClavicle,	// Name: "Bip01 L Clavicle" (bone number for the "left clavicle" bone of players (left shoulder))
	PlayerBoneID_LeftUpperArm,	// Name: "Bip01 L UpperArm" (bone number for the "left upperarm" bone of players (left elbow))
	PlayerBoneID_LeftForearm,	// Name: "Bip01 L Forearm" (bone number for the "left forearm" bone of players (left wrist))
	PlayerBoneID_LeftHand,		// Name: "Bip01 L Hand" (bone number for the "left hand" bone of players (center of hand))
	PlayerBoneID_LeftFinger0,	// Name: "Bip01 L Finger0" (bone number for the "left_finger0" bone of players (half of thumb))
	PlayerBoneID_LeftFinger01,	// Name: "Bip01 L Finger01" (bone number for the "left_finger01" bone of players (extremity of thumb))
	PlayerBoneID_LeftFinger1,	// Name: "Bip01 L Finger1" (bone number for the "left_finger1" bone of players (half of other fingers))
	PlayerBoneID_LeftFinger11,	// Name: "Bip01 L Finger11" (bone number for the "left_finger11" bone of players (extremity of fingers))
	PlayerBoneID_18,			// Name: "-- L knuckle"
//	PlayerBoneID_19,			// Name: "-- L Forearm twist"
	PlayerBoneID_20,			// Name: "-- L wrist"
	PlayerBoneID_21,			// Name: "-- L Elbow"
	PlayerBoneID_22,			// Name: "-- L bicep twist"
	PlayerBoneID_23,			// Name: "-- L shoulder outside"
	PlayerBoneID_24,			// Name: "-- L Shoulder inside"
	PlayerBoneID_RightClavicle,	// Name: "Bip01 R Clavicle" (bone number for the "right clavicle" bone of players (right shoulder))
	PlayerBoneID_RightUpperArm,	// Name: "Bip01 R UpperArm" (bone number for the "right upperarm" bone of players (right elbow))
	PlayerBoneID_RightForearm,	// Name: "Bip01 R Forearm" (bone number for the "right forearm" bone of players (right wrist))
	PlayerBoneID_RightHand,		// Name: "Bip01 R Hand" (bone number for the "right hand" bone of players (center of hand))
	PlayerBoneID_RightFinger0,	// Name: "Bip01 R Finger0" (bone number for the "right_finger0" bone of players (half of thumb))
	PlayerBoneID_RightFinger01,	// Name: "Bip01 R Finger01" (bone number for the "right_finger01" bone of players (extremity of thumb))
	PlayerBoneID_RightFinger1,	// Name: "Bip01 R Finger1" (bone number for the "right_finger1" bone of players (half of other fingers))
	PlayerBoneID_RightFinger11,	// Name: "Bip01 R Finger11" (bone number for the "right_finger11" bone of players (extremity of fingers))
	PlayerBoneID_33,			// Name: "-- R knuckle"
	PlayerBoneID_34,			// Name: "-- R wrist"
//	PlayerBoneID_35,			// Name: "-- R forearm twist"
	PlayerBoneID_36,			// Name: "-- R Elbow"
	PlayerBoneID_37,			// Name: "-- R bicep twist"
	PlayerBoneID_38,			// Name: "-- R Shoulder inside"
	PlayerBoneID_39,			// Name: "-- R shoulder outside"
	PlayerBoneID_40,			// Name: "-- Neck smooth"
	PlayerBoneID_41,			// Name: "-- R Butt"
	PlayerBoneID_42,			// Name: "-- L butt"
	PlayerBoneID_LeftThigh,		// Name: "Bip01 L Thigh" (bone number for the "left_thigh" bone of players (what it says))
	PlayerBoneID_LeftCalf,		// Name: "Bip01 L Calf" (bone number for the "left_calf" bone of players (what it says))
	PlayerBoneID_LeftFoot,		// Name: "Bip01 L Foot" (bone number for the "left_foot" bone of players (extremity of toes))
	PlayerBoneID_46,			// Name: "Bip01 L Toe0"
	PlayerBoneID_47,			// Name: "-- L ankle"
	PlayerBoneID_48,			// Name: "-- L Knee"
	PlayerBoneID_RightThigh,	// Name: "Bip01 R Thigh" (bone number for the "right_thigh" bone of players (what it says))
	PlayerBoneID_RightCalf,		// Name: "Bip01 R Calf" (bone number for the "right_calf" bone of players (what it says))
	PlayerBoneID_RightFoot,		// Name: "Bip01 R Foot" (bone number for the "right_foot" bone of players (extremity of toes))
	PlayerBoneID_52,			// Name: "Bip01 R Toe0"
	PlayerBoneID_53,			// Name: "-- R Ankle"
	PlayerBoneID_54,			// Name: "-- R Knee"
	PlayerBoneID_55				// Name: "Bomb" ("Object04")
};

// (gsg9)
enum CZ_PlayerBoneID_t
{
	CZ_PlayerBoneID_1 = 1u,			// Name: "Bip01"
	CZ_PlayerBoneID_Pelvis,			// Name: "Bip01 Pelvis" (bone number for the "pelvis" bone of players (bottom of spine))
	CZ_PlayerBoneID_Spine,			// Name: "Bip01 Spine" (bone number for the "spine" bone of players (bottom 1/4 of spine))
	CZ_PlayerBoneID_Spine1,			// Name: "Bip01 Spine1" (bone number for the "spine1" bone of players (half of spine))
	CZ_PlayerBoneID_Spine2,			// Name: "Bip01 Spine2" (bone number for the "spine2" bone of players (bottom 3/4 of spine))
	CZ_PlayerBoneID_Spine3,			// Name: "Bip01 Spine3" (bone number for the "spine3" bone of players (top of spine, between clavicles))
	CZ_PlayerBoneID_Neck,			// Name: "Bip01 Neck" (bone number for the "neck" bone of players (bottom of neck))
	CZ_PlayerBoneID_Head,			// Name: "Bip01 Head" (bone number for the "head" bone of players (center of neck))
	CZ_PlayerBoneID_LeftClavicle,	// Name: "Bip01 L Clavicle" (bone number for the "left clavicle" bone of players (left shoulder))
	CZ_PlayerBoneID_LeftUpperArm,	// Name: "Bip01 L UpperArm" (bone number for the "left upperarm" bone of players (left elbow))
	CZ_PlayerBoneID_LeftForearm,	// Name: "Bip01 L Forearm" (bone number for the "left forearm" bone of players (left wrist))
	CZ_PlayerBoneID_LeftHand,		// Name: "Bip01 L Hand" (bone number for the "left hand" bone of players (center of hand))
	CZ_PlayerBoneID_LeftFinger0,	// Name: "Bip01 L Finger0" (bone number for the "left_finger0" bone of players (half of thumb))
	CZ_PlayerBoneID_LeftFinger01,	// Name: "Bip01 L Finger01" (bone number for the "left_finger01" bone of players (extremity of thumb))
	CZ_PlayerBoneID_LeftFinger1,	// Name: "Bip01 L Finger1" (bone number for the "left_finger1" bone of players (half of other fingers))
	CZ_PlayerBoneID_LeftFinger11,	// Name: "Bip01 L Finger11" (bone number for the "left_finger11" bone of players (extremity of fingers))
	CZ_PlayerBoneID_17,				// Name: "-- L Elbow"
	CZ_PlayerBoneID_RightClavicle,	// Name: "Bip01 R Clavicle" (bone number for the "right clavicle" bone of players (right shoulder))
	CZ_PlayerBoneID_RightUpperArm,	// Name: "Bip01 R UpperArm" (bone number for the "right upperarm" bone of players (right elbow))
	CZ_PlayerBoneID_RightForearm,	// Name: "Bip01 R Forearm" (bone number for the "right forearm" bone of players (right wrist))
	CZ_PlayerBoneID_RightHand,		// Name: "Bip01 R Hand" (bone number for the "right hand" bone of players (center of hand))
	CZ_PlayerBoneID_RightFinger0,	// Name: "Bip01 R Finger0" (bone number for the "right_finger0" bone of players (half of thumb))
	CZ_PlayerBoneID_RightFinger01,	// Name: "Bip01 R Finger01" (bone number for the "right_finger01" bone of players (extremity of thumb))
	CZ_PlayerBoneID_RightFinger1,	// Name: "Bip01 R Finger1" (bone number for the "right_finger1" bone of players (half of other fingers))
	CZ_PlayerBoneID_RightFinger11,	// Name: "Bip01 R Finger11" (bone number for the "right_finger11" bone of players (extremity of fingers))
	CZ_PlayerBoneID_26,				// Name: "-- R Elbow"
	CZ_PlayerBoneID_27,				// Name: "-- L butt"
	CZ_PlayerBoneID_LeftThigh,		// Name: "Bip01 L Thigh" (bone number for the "left_thigh" bone of players (what it says))
	CZ_PlayerBoneID_LeftCalf,		// Name: "Bip01 L Calf" (bone number for the "left_calf" bone of players (what it says))
	CZ_PlayerBoneID_LeftFoot,		// Name: "Bip01 L Foot" (bone number for the "left_foot" bone of players (extremity of toes))
	CZ_PlayerBoneID_31,				// Name: "Bip01 L Toe0"
	CZ_PlayerBoneID_RightThigh,		// Name: "Bip01 R Thigh" (bone number for the "right_thigh" bone of players (what it says))
	CZ_PlayerBoneID_RightCalf,		// Name: "Bip01 R Calf" (bone number for the "right_calf" bone of players (what it says))
	CZ_PlayerBoneID_RightFoot,		// Name: "Bip01 R Foot" (bone number for the "right_foot" bone of players (extremity of toes))
	CZ_PlayerBoneID_35,				// Name: "Bip01 R Toe0"
	CZ_PlayerBoneID_36				// Name: "Bomb" ("Object04")
};

#endif	// ifndef PLAYER_BONE_ID_INCLUDED