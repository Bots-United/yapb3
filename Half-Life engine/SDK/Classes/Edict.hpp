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
// Edict.hpp
//
// Class: Edict
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef EDICT_INCLUDED
#define EDICT_INCLUDED

class Edict
{
	//
	// Group: Members.
	//
	public:
/*! off=0(0) */		BOOL                           isFree;
/*! off=4(1) */		int                            serialNumber;
/*! off=8(2) */		Structures::Link_t             area;		// linked to a division node or leaf
/*! off=16(4) */		int                            headNode;	// -1 to use normal leaf check
/*! off=20(5) */		int                            numLeafs;
/*! off=24(6) */		short                          leafNums[Constants::MaximumEntityLeafs];
/*! off=120(30) */		float                          freeTime;	// sv.time when the object was freed
/*! off=124(31) */		BaseEntity                    *privateData;	// allocated and freed by engine, used by DLLs
/*! off=128(32) */		Structures::EntityVariables_t  variables;	// C exported fields from progs

	// Other fields from progs come immediately after.

	//
	// Group: Functions.
	//
	public:
		template <typename elementType> inline void SetPrivateData (const Constants::EntityPrivateDataOffset_t offset, const elementType &newValue)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			MemoryUtilities::SetOffsetAddressReference <elementType> (privateData, static_cast <const unsigned short> (offset), newValue);
		}
		template <typename elementType> inline const elementType &GetPrivateDataReference (const Constants::EntityPrivateDataOffset_t offset) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return MemoryUtilities::GetOffsetAddressReference <elementType> (privateData, static_cast <const unsigned short> (offset));
		}

		//
		// Function: GetOffset
		//
		// Description: Gets the engine entity offset.
		//
		// Returns: Entity offset.
		//
		inline const TypeDefinitions::EntityOffset_t GetOffset          (void) const               { return Globals::g_halfLifeEngine->GetEntityOffset (this); }

		//
		// Function: GetEntityIndex
		//
		// Description: Gets the engine entity index.
		//
		// Returns: Entity index.
		//
		inline const unsigned short    GetIndex          (void) const               { return Globals::g_halfLifeEngine->GetEdictIndex (this); }

		//
		// Function: IsValid
		//
		// Description: Checks whether entity is valid.
		//
		// Returns: True if entity is valid, false otherwise.
		//
		inline const bool    IsValid            (void) const                { return this != NULL && GetOffset () >/*=*/ 0 && isFree == FALSE && !(variables.flags & Constants::FL_KILLME); }

		//
		// Function: IsWorldspawn
		//
		// Description: Checks whether entity is worldspawn.
		//
		// Returns: True if entity is worldspawn, false otherwise.
		//
		inline const bool    IsWorldspawn       (void) const                { return GetIndex () == 0u; }

		//
		// Function: IsImmortal
		//
		// Description: Checks whether entity is immortal.
		//
		// Returns: True if entity is immortal, false otherwise.
		//
		inline const bool    IsImmortal         (void) const                { return variables.takeDamageType == Constants::DAMAGE_NO; }

		inline const bool    IsTransparent      (void) const                { return variables.renderMode != Constants::kRenderNormal && !(variables.flags & Constants::FL_WORLDBRUSH); }

		inline const bool IsBreakable           (void) const
		{
			const DynamicString className (GetClassName ());

			return className == "func_breakable" || className == "func_pushable";
		}
		inline const bool IsBreakableExplodable (void) const { return variables.impulse > 0; }
		inline const bool IsBreakableGlass      (void) const { return variables.playerClass == 1; }
		inline const bool IsBreakableShootable  (void) const
		{
			// This function is checking that pointed by 'breakableEntity' pointer obstacle, can be destroyed.

			return GetPrivateDataReference <unsigned int> (Constants::Offset_FunctionBreakable_MaterialType) != Constants::FunctionBreakableMaterialType_UnbreakableGlass && !(variables.spawnFlags & Constants::SF_BREAK_TRIGGER_ONLY);
//			return ((GetClassName () == "func_breakable" && (IsBreakableGlass () || GetHealth () == 0 || (GetHealth () > 1 && GetHealth () < 1000) || variables.renderMode == Constants::kRenderTransAlpha)) || (GetClassName () == "func_pushable" && (variables.spawnFlags & Constants::SF_PUSH_BREAKABLE) && GetHealth () < 1000)) && !IsBreakableExplodable () && !(variables.spawnFlags & Constants::SF_BREAK_TRIGGER_ONLY)/* && !(variables.flags & Constants::FL_WORLDBRUSH)*/;
		}

		//
		// Function: IsPlayerOnLadder
		//
		// Description: Checks whether player entity is on ladder and/or flying.
		//
		// Returns: True if player entity is currently standing on the ladder, false otherwise.
		//
		inline const bool IsPlayerOnLadder (void) const
		{
			// Read the gravity state directly from player entity variables....
			return variables.moveType == Constants::MOVETYPE_FLY;
		}

		//
		// Function: IsPlayerOnTrain
		//
		// Description: Checks whether player entity is on train.
		//
		// Returns: True if player entity is currently standing on the train, false otherwise.
		//
		inline const bool IsPlayerOnTrain (void) const { return variables.iuser4 == 1u; }

		//
		// Function: IsPlayer
		//
		// Description: Checks whether entity is player.
		//
		// Returns: True if entity is player, false otherwise.
		//
		inline const bool    IsPlayer              (void) const
		{
			return Utilities::IsPlayerEdictIndex (GetIndex ());
		}
		inline const bool    IsNotWorldspawnPlayer (void) const
		{
			return Utilities::IsNotWorldspawnPlayerEdictIndex (GetIndex ());
		}

		inline const bool    IsArmoury              (void) const
		{
			return GetClassName () == "armoury_entity";
		}
		inline const bool    IsWeaponBox            (void) const
		{
			return GetClassName () == "weaponbox";
		}
		inline const bool IsGrenade (void) const
		{
			return GetClassName () == "grenade";
		}
		inline const bool IsC4 (void) const
		{
			return IsGrenade () && IsGrenadeIsC4 ();
		}
		inline const bool IsGrenadeIsC4 (void) const
		{
			return GetPrivateDataReference <bool> (Constants::Offset_Grenade_IsC4);
		}
		inline const bool IsSmokeGrenade (void) const
		{
			return IsGrenade () && IsGrenadeIsSmokeGrenade ();
		}
		inline const bool IsGrenadeIsSmokeGrenade (void) const
		{
			return GetGrenadeTypeID () == Constants::GrenadeTypeID_Smoke;
		}
		inline const Constants::GrenadeTypeID_t GetGrenadeTypeID (void) const
		{
			return GetPrivateDataReference <Constants::GrenadeTypeID_t> (Constants::OFFSET_Grenade_TypeID);
		}
		inline const Constants::GrenadeType_t GetGrenadeType (void) const
		{
			return static_cast <Constants::GrenadeType_t> (GetGrenadeTypeID () & Constants::GrenadeType_Hand_Total);
		}

		inline const Constants::WeaponID_t GetWeaponID (void) const
		{
			return GetPrivateDataReference <Constants::WeaponID_t> (Constants::OFFSET_WEAPON_ID);
		}

		inline void SetPlayerTeam (const Constants::TeamID_t newTeam)
		{
			SetPrivateData <Constants::TeamID_t> (Constants::OFFSET_TEAM, newTeam);

			// This makes the model get updated right away.
			Globals::g_halfLifeEngine->ClientUserInfoChanged (this, GetInfoKeyBuffer ());	// If this causes any problems for WON, do this line only in STEAM builds.

			// And update scoreboard by sending 'TeamInfo' message.
			Globals::g_halfLifeEngine->MessageBegin (Constants::MSG_ALL, Globals::g_halfLifeEngine->GetGameMessageContainer ().GetRegisteredGameMessageIndexByType (Engine::GameMessageContainer::Type_TeamInfo), NULL, NULL);	// begin message
				Globals::g_halfLifeEngine->WriteByte (static_cast <const unsigned char> (GetIndex ()));
				Globals::g_halfLifeEngine->WriteString (Constants::TeamNames[newTeam]);
			Globals::g_halfLifeEngine->MessageEnd ();	// end of message
		}
		inline const Constants::TeamID_t GetPlayerTeam (void) const { return GetPrivateDataReference <Constants::TeamID_t> (Constants::OFFSET_TEAM); }

		inline const bool           IsHostage             (void) const { return GetClassName () == "hostage_entity"; }
		inline const bool           IsHostageValid        (void) const { return variables.takeDamageType == Constants::DAMAGE_YES; }
		inline const bool           IsHostageAlive        (void) const
		{
			// Read the dead flag directly from hostage entity variables....
			return variables.deadValue == Constants::DEAD_NO;
		}
		inline const bool           IsHostageHasRescuer   (void) const { return GetHostageRescuer () != NULL; }
		inline const Edict *const   GetHostageRescuer     (void) const;	// Note: This function declared in Core.hpp.
		inline const Math::Vector3D GetHostageOrigin      (void) const { return GetBModelOrigin (); }
		inline const float          GetHostageNextUseTime (void) const { return GetPrivateDataReference <float> (Constants::OFFSET_HOSTAGE_NEXT_USE); }	// Returns time that the hostage will be next used.
		inline const float          GetHostageLastUseTime (void) const { return GetPrivateDataReference <float> (Constants::OFFSET_HOSTAGE_LAST_USE); }	// Returns time that the hostage was last used.

		//
		// Function: IsFakeClient
		//
		// Description: Checks whether entity is bot (fake client).
		//
		// Returns: True if entity is bot (fake client), false otherwise.
		//
		inline const bool    IsFakeClient       (void) const { return IsPlayer () && IsPlayerFakeClient (); }

		//
		// Function: IsPlayerFakeClient
		//
		// Description: Checks whether player entity is bot (fake client).
		//
		// Returns: True if player entity is bot (fake client), false otherwise.
		//
		inline const bool    IsPlayerFakeClient (void) const { return (variables.flags & Constants::FL_FAKECLIENT) > 0u; }

		//
		// Function: IsHuman
		//
		// Description: Checks whether entity is human.
		//
		// Returns: True if entity is human, false otherwise.
		//
		inline const bool    IsHuman            (void) const { return IsPlayer () && IsPlayerHuman (); }

		//
		// Function: IsPlayerHuman
		//
		// Description: Checks whether player entity is human.
		//
		// Returns: True if player entity is human, false otherwise.
		//
		inline const bool    IsPlayerHuman      (void) const { return IsPlayerFakeClient () == false; }

		inline const bool    IsPlayerZBot       (void) const;	// Note: This function declared in Core.hpp.

		//
		// Function: HasBoundingBox
		//
		// Description: Checks whether entity has it's own bounding boxes.
		//
		// Returns: True if entity has bounding box, false otherwise.
		//
		inline const bool    HasBoundingBox     (void) const
		{
			// this function returns TRUE if entity has a valid bounding box, FALSE otherwise.

//			return !variables.absoluteBoundingBox.mins.IsZero () || !variables.absoluteBoundingBox.maxs.IsZero ();
			return variables.absoluteBoundingBox.mins != Math::Vector3D::ZeroValue || variables.absoluteBoundingBox.maxs != Math::Vector3D::ZeroValue;
		}

		//
		// Function: GetClassName
		//
		// Description: Gets entity class name.
		//
		// Returns: Class name of entity.
		//
		inline const DynamicString  GetClassName       (void) const                { return Globals::g_halfLifeEngine->GetStringFromOffset (variables.className); }

		inline const DynamicString  GetNetName         (void) const                { return Globals::g_halfLifeEngine->GetStringFromOffset (variables.netName); }
		inline const DynamicString  GetWeaponModelName (void) const                { return Globals::g_halfLifeEngine->GetStringFromOffset (variables.weaponModelName); }
		//
		// Function: GetModel
		//
		// Description: Gets the entity model name.
		//
		// Returns: Model name of entity.
		//
		inline const DynamicString  GetModel           (void) const
		{
			return IsPlayer () ? GetPlayerModel () : GetNonPlayerModel ();
		}
		//
		// Function: GetPlayerModel
		//
		// Description: Gets the entity model name.
		//
		// Returns: Model name of entity.
		//
		inline const DynamicString  GetPlayerModel     (void) const
		{
			return Globals::g_halfLifeEngine->GetInfoKeyValue (GetInfoKeyBuffer (), "model");
		}
		//
		// Function: GetNonPlayerModel
		//
		// Description: Gets the entity model name.
		//
		// Returns: Model name of entity.
		//
		inline const DynamicString  GetNonPlayerModel  (void) const
		{
			return Globals::g_halfLifeEngine->GetStringFromOffset (variables.model);
		}
		//
		// Function: GetViewModel
		//
		// Description: Gets entity viewable model.
		//
		// Returns: Viewable model of entity.
		//
		inline const DynamicString  GetViewModelName   (void) const                { return Globals::g_halfLifeEngine->GetStringFromOffset (variables.viewModelName); }
		//
		// Function: GetHealth
		//
		// Description: Gets the health value of entity.
		//
		// Returns: Health value.
		//
		inline const int     GetHealth          (void) const                { return static_cast <int> (variables.health); }
		//
		// Function: GetMaximumHealth
		//
		// Description: Gets the maximum health value of entity.
		//
		// Returns: Maximum health value.
		//
		inline const int     GetMaximumHealth   (void) const                { return static_cast <int> (variables.maximumHealth); }

		inline const int     GetArmor           (void) const                { return static_cast <int> (variables.armorValue); }
		//
		// Function: GetCenter
		//
		// Description: Gets the entity center.
		//
		// Returns: Center point origin of entity.
		//
		inline const Math::Vector3D  GetCenter         (void) const              { return variables.absoluteBoundingBox.maxs.GetMidPoint (variables.absoluteBoundingBox.mins); }
		//
		// Function: GetBModelOrigin
		//
		// Description: Calculates origin of a bmodel from absoluteBoundingBox.mins/size because all bmodel origins are 0 0 0.
		//
		// Returns: Origin point of bmodel.
		//
		inline const Math::Vector3D  GetBModelOrigin   (void) const              { return variables.size * 0.5f + variables.absoluteBoundingBox.mins; }
		inline const Math::Vector3D  GetBottomBModelOrigin (void) const
		{
			// this expanded function returns the vector origin of the bottom of an entity, assuming that
			// any entity that has a bounding box has its center at the center of the bounding box itself.
			// Its bottom is then given by lowering this center to the mins height.

			// compute its bottom center
			return Math::Vector3D (variables.size.x * 0.5f + variables.absoluteBoundingBox.mins.x, variables.size.y * 0.5f + variables.absoluteBoundingBox.mins.y, variables.absoluteBoundingBox.mins.z);
		}
		//
		// Function: GetOrigin
		//
		// Description: Gets entity origin in world space.
		//
		// Returns: Entity origin.
		//
		inline const Math::Vector3D  GetOrigin         (void) const              { return GetComputedOrigin (); }
		inline const Math::Vector3D  GetComputedOrigin (void) const              { return HasBoundingBox () ? GetCenter () : variables.origin == Math::Vector3D::ZeroValue ? GetBModelOrigin () : variables.origin; }
		//
		// Function: GetEyePosition
		//
		// Description: Gets eye position of entity.
		//
		// Returns: Eye position of entity.
		//
		inline const Math::Vector3D  GetEyePosition    (void) const              { return variables.origin + variables.viewOffset; }	// position of eyes/ears/gun
		//
		// Function: GetVelocity
		//
		// Description: Gets the entity velocity.
		//
		// Returns: Velocity of entity.
		//
//		inline const Math::Vector3D  GetVelocity       (void) const              { return variables.velocity; }
		//
		// Function: GetSpeed
		//
		// Description: Gets the entity speed.
		//
		// Returns: Speed of entity.
		//
		inline const float   GetSpeed           (void) const                { return variables.velocity.GetLength (); }
		inline const float   GetSpeedSquared    (void) const                { return variables.velocity.GetLengthSquared (); }
		inline const float   GetSpeed2D         (void) const                { return variables.velocity.GetLength2D (); }
		inline const float   GetSpeedSquared2D  (void) const                { return variables.velocity.GetLengthSquared2D (); }

		inline const Constants::SpectatorObserverMode_t GetPlayerObserverMode             (void) const { return static_cast <Constants::SpectatorObserverMode_t> (variables.iuser1); }
		inline const unsigned char                      GetPlayerObserverTargetEdictIndex (void) const { return static_cast <unsigned char> (variables.iuser2); }

		//
		// Function: GetTarget
		//
		// Description: Gets the entity target.
		//
		// Returns: Target of entity.
		//
		inline const DynamicString  GetTarget          (void) const                { return Globals::g_halfLifeEngine->GetStringFromOffset (variables.target); }

		//
		// Function: GetTargetName
		//
		// Description: Gets the entity target name.
		//
		// Returns: Target name of entity.
		//
		inline const DynamicString  GetTargetName      (void) const                { return Globals::g_halfLifeEngine->GetStringFromOffset (variables.targetName); }
/*
		inline void          SetAngles          (const Math::Angles3D &viewAngles)
		{
			variables.viewAngles = viewAngles;
			variables.modelAngles = variables.viewAngles.Clamp ();	// set the body angles to point the gun correctly

			variables.modelAngles.pitch *= -(1.0f / 3.0f);	// invert for engine (adjust the view angle pitch to aim correctly)
		}
*/
		inline const Math::Angles2D &GetPunchAngles                   (void) const { return variables.punchAngles; }
		inline const Math::Angles2D  GetFakeClientModifiedPunchAngles (void) const { return Math::Angles2D (-GetPunchAngles ().pitch, GetPunchAngles ().yaw); }
		inline const Math::Angles2D  GetHumanModifiedPunchAngles      (void) const { return GetFakeClientModifiedPunchAngles () * 2.0f; }

		//
		// Function: IsVisible
		//
		// Description: Checks whether entity is visible.
		//
		// Returns: True if entity is visible, false otherwise.
		//
		inline const bool    IsVisible          (void) const                { return !(variables.effects & Constants::EntityEffect_NoDraw) && !GetModel ().IsEmpty (); }

		//
		// Function: IsAlive
		//
		// Description: Checks whether entity is alive.
		//
		// Returns: True if entity is alive, false otherwise.
		//
		inline const bool    IsAlive            (void) const
		{
			// Read the dead flag, health and move type states directly from entity variables....
			return variables.deadValue == Constants::DEAD_NO && GetHealth () > 0/* && !(variables.flags & Constants::FL_NOTARGET) && variables.moveType != Constants::MOVETYPE_NOCLIP*/;
		}

		//
		// Function: IsInWater
		//
		// Description: Checks whether entity is in water.
		//
		// Returns: True if entity is in water, false otherwise.
		//
		inline const bool    IsInWater          (void) const              { return variables.waterLevel >= Constants::EntityWaterLevel_2; }
		inline const bool    IsInSlime          (void) const              { return Globals::g_halfLifeEngine->GetPointContents (GetOrigin () - Math::Vector3D (0.0f, 0.0f, 23.0f)) == Constants::Content_Slime; }
		inline const bool    IsInLava           (void) const              { return Globals::g_halfLifeEngine->GetPointContents (GetOrigin () - Math::Vector3D (0.0f, 0.0f, 23.0f)) == Constants::Content_Lava; }
		//
		// Function: IsOnFloor
		//
		// Description: Checks whether entity is standing on the floor.
		//
		// Returns: True if entity is on floor, false otherwise.
		//
		inline const bool    IsOnFloor          (void) const               { return IsPlayer () ? IsPlayerOnFloor () : IsNonPlayerOnFloor (); }
		//
		// Function: IsPlayerOnFloor
		//
		// Description: Checks whether entity is standing on the floor.
		//
		// Returns: True if entity is on floor, false otherwise.
		//
		inline const bool    IsPlayerOnFloor    (void) const               { return (variables.flags & (Constants::FL_ONGROUND | Constants::FL_PARTIALGROUND)) != 0/*variables.fallVelocity != 0.0f*/; }
		//
		// Function: IsNonPlayerOnFloor
		//
		// Description: Checks whether entity is standing on the floor.
		//
		// Returns: True if entity is on floor, false otherwise.
		//
		inline const bool    IsNonPlayerOnFloor (void) const               { return Globals::g_halfLifeEngine->IsEntityOnFloor (this); }
		//
		// Function: IsFallingDown
		//
		// Description: Checks whether entity is falling down.
		//
		// Returns: True if entity is falling down, false otherwise.
		//
		inline const bool    IsFallingDown      (void) const               { return variables.fallVelocity > 0.0f; }
		//
		// Function: IsMoving
		//
		// Description: Checks whether entity is moving.
		//
		// Returns: True if entity is moving, false otherwise.
		//
		inline const bool    IsMoving           (void) const                { return !Math::IsZero (GetSpeed ()); }
		//
		// Function: IsDucking
		//
		// Description: Checks whether entity is ducking.
		//
		// Returns: True, if entity is ducking, false otherwise.
		//
		inline const bool    IsDucking          (void) const                { return (variables.flags & Constants::FL_DUCKING) > 0u; }

//		const bool           IsInViewCone       (Math::Vector3D &origin, float fov = 0.0f) const;
//		const bool           IsInViewCone       (Edict *entity, float fov = 0.0f) const;

//		const bool           IsVisible          (const Math::Vector3D &destination) const;
//		const bool           IsVisible          (Edict *entity) const;

//		const bool           IsBoxVisible       (Edict *entity, Math::Vector3D *pvHit = NULL, unsigned char *ucBodyPart = NULL) const;

//		void                 ChangeAngles       (const Math::Vector3D &idealAngles, float speed = 1.0f);	// speed: 0.1 - 1

	void Print (const char *const filename = NULL) const
	{
		// yay yet another PMB function

		char temp[128u];

		if (IsNullOrEmptyString (filename))
			sprintf (temp, "bot%u.txt", GetIndex ());
		else
			strcpy (temp, filename);

		STDIOFile file (temp, "at");

		// now just dump the entity variables
		file.PrintFormat ("\nclassName = %i (\"%s\")\n", variables.className, GetClassName ().GetData ());
		file.PrintFormat ("variables.globalName = %i (\"%s\")\n", variables.globalName, Globals::g_halfLifeEngine->GetStringFromOffset (variables.globalName));
		file.PrintFormat ("variables.origin = Vector (%.1f, %.1f, %.1f)\n", variables.origin.x, variables.origin.y, variables.origin.z);
		file.PrintFormat ("variables.oldOrigin = Vector (%.1f, %.1f, %.1f)\n", variables.oldOrigin.x, variables.oldOrigin.y, variables.oldOrigin.z);
		file.PrintFormat ("variables.velocity = Vector (%.1f, %.1f, %.1f) - Length %.3f - Length2D %.3f\n", variables.velocity.x, variables.velocity.y, variables.velocity.z, GetSpeed (), GetSpeed2D ());
		file.PrintFormat ("variables.baseVelocity = Vector (%.1f, %.1f, %.1f) - Length %.3f - Length2D %.3f\n", variables.baseVelocity.x, variables.baseVelocity.y, variables.baseVelocity.z, GetSpeed (), GetSpeed2D ());
		file.PrintFormat ("variables.clBaseVelocity = Vector (%.1f, %.1f, %.1f) - Length %.3f - Length2D %.3f\n", variables.clBaseVelocity.x, variables.clBaseVelocity.y, variables.clBaseVelocity.z, GetSpeed (), GetSpeed2D ());
		file.PrintFormat ("variables.moveDirection = Vector (%.1f, %.1f, %.1f)\n", variables.moveDirection.x, variables.moveDirection.y, variables.moveDirection.z);
		file.PrintFormat ("variables.modelAngles = Angles3D (%.1f, %.1f, %.1f)\n", variables.modelAngles.pitch, variables.modelAngles.yaw, variables.modelAngles.roll);
		file.PrintFormat ("variables.angleVelocity = Angles3D (%.1f, %.1f, %.1f) - Length %.3f - Length2D %.3f\n", variables.angleVelocity.pitch, variables.angleVelocity.yaw, variables.angleVelocity.roll, GetSpeed (), GetSpeed2D ());
		file.PrintFormat ("variables.punchAngles = Angles3D (%.1f, %.1f, %.1f)\n", variables.punchAngles.pitch, variables.punchAngles.yaw, variables.punchAngles.roll);
		file.PrintFormat ("variables.viewAngles = Angles3D (%.1f, %.1f, %.1f)\n", variables.viewAngles.pitch, variables.viewAngles.yaw, variables.viewAngles.roll);
		file.PrintFormat ("variables.endPosition = Vector (%.1f, %.1f, %.1f)\n", variables.endPosition.x, variables.endPosition.y, variables.endPosition.z);
		file.PrintFormat ("variables.startPosition = Vector (%.1f, %.1f, %.1f)\n", variables.startPosition.x, variables.startPosition.y, variables.startPosition.z);
		file.PrintFormat ("variables.impactTime = %.3f\n", variables.impactTime);
		file.PrintFormat ("variables.startTime = %.3f\n", variables.startTime);
		file.PrintFormat ("variables.fixAngle = %i (%s)\n", variables.fixAngle, variables.fixAngle == Constants::FixAngleType_None ? "nothing" : variables.fixAngle == Constants::FixAngleType_Absolute ? "force view angles" : variables.fixAngle == Constants::FixAngleType_Relative ? "add velocity" : "???");
		file.PrintFormat ("variables.idealPitch = %.3f\n", variables.idealPitch);
		file.PrintFormat ("variables.pitchSpeed = %.3f\n", variables.pitchSpeed);
		file.PrintFormat ("variables.idealYaw = %.3f\n", variables.idealYaw);
		file.PrintFormat ("variables.yawSpeed = %.3f\n", variables.yawSpeed);
		file.PrintFormat ("variables.modelIndex = %i\n", variables.modelIndex);
		file.PrintFormat ("variables.model = %i (\"%s\")\n", variables.model, GetModel ().GetData ());
		file.PrintFormat ("variables.viewModelName = %i (\"%s\")\n", variables.viewModelName, GetViewModelName ().GetData ());
		file.PrintFormat ("variables.weaponModelName = %i (\"%s\")\n", variables.weaponModelName, Globals::g_halfLifeEngine->GetStringFromOffset (variables.weaponModelName));
		file.PrintFormat ("variables.absoluteBoundingBox.mins = Vector (%.1f, %.1f, %.1f)\n", variables.absoluteBoundingBox.mins.x, variables.absoluteBoundingBox.mins.y, variables.absoluteBoundingBox.mins.z);
		file.PrintFormat ("variables.absoluteBoundingBox.maxs = Vector (%.1f, %.1f, %.1f)\n", variables.absoluteBoundingBox.maxs.x, variables.absoluteBoundingBox.maxs.y, variables.absoluteBoundingBox.maxs.z);
		file.PrintFormat ("variables.boundingBox.mins = Vector (%.1f, %.1f, %.1f)\n", variables.boundingBox.mins.x, variables.boundingBox.mins.y, variables.boundingBox.mins.z);
		file.PrintFormat ("variables.boundingBox.maxs = Vector (%.1f, %.1f, %.1f)\n", variables.boundingBox.maxs.x, variables.boundingBox.maxs.y, variables.boundingBox.maxs.z);
		file.PrintFormat ("variables.size = Vector (%.1f, %.1f, %.1f)\n", variables.size.x, variables.size.y, variables.size.z);
		file.PrintFormat ("variables.localTime = %.3f (current time is %.3f)\n", variables.localTime, Globals::g_halfLifeEngine->GetTime ());
		file.PrintFormat ("variables.nextThinkTime = %.3f (current time is %.3f)\n", variables.nextThinkTime, Globals::g_halfLifeEngine->GetTime ());
		file.PrintFormat ("variables.moveType = %i (%s)\n", variables.moveType, variables.moveType == Constants::MOVETYPE_NONE ? "MOVETYPE_NONE" : variables.moveType == Constants::MOVETYPE_WALK ? "MOVETYPE_WALK" : variables.moveType == Constants::MOVETYPE_STEP ? "MOVETYPE_STEP" : variables.moveType == Constants::MOVETYPE_FLY ? "MOVETYPE_FLY" : variables.moveType == Constants::MOVETYPE_TOSS ? "MOVETYPE_TOSS" : variables.moveType == Constants::MOVETYPE_PUSH ? "MOVETYPE_PUSH" : variables.moveType == Constants::MOVETYPE_NOCLIP ? "MOVETYPE_NOCLIP" : variables.moveType == Constants::MOVETYPE_FLYMISSILE ? "MOVETYPE_FLYMISSILE" : variables.moveType == Constants::MOVETYPE_BOUNCE ? "MOVETYPE_BOUNCE" : variables.moveType == Constants::MOVETYPE_BOUNCEMISSILE ? "MOVETYPE_BOUNCEMISSILE" : variables.moveType == Constants::MOVETYPE_FOLLOW ? "MOVETYPE_FOLLOW" : variables.moveType == Constants::MOVETYPE_PUSHSTEP ? "MOVETYPE_PUSHSTEP" : "???");
		file.PrintFormat ("variables.solid = %i (%s)\n", variables.solid, variables.solid == Constants::SOLID_NOT ? "SOLID_NOT" : variables.solid == Constants::SOLID_TRIGGER ? "SOLID_TRIGGER" : variables.solid == Constants::SOLID_BoundingBox ? "SOLID_BoundingBox" : variables.solid == Constants::SOLID_SLIDEBOX ? "SOLID_SLIDEBOX" : variables.solid == Constants::SOLID_BSP ? "SOLID_BSP" : "???");
		file.PrintFormat ("variables.skin = %i (\"%s\")\n", variables.skin, Globals::g_halfLifeEngine->GetStringFromOffset (variables.skin).GetData ());
		file.PrintFormat ("variables.body = %i (\"%s\")\n", variables.body, Globals::g_halfLifeEngine->GetStringFromOffset (variables.body).GetData ());

		sprintf (temp, "");	// prepare pattern string

		if (variables.effects & Constants::EF_BRIGHTFIELD)
			strcat (temp, "EF_BRIGHTFIELD | ");

		if (variables.effects & Constants::EF_MUZZLEFLASH)
			strcat (temp, "EF_MUZZLEFLASH | ");

		if (variables.effects & Constants::EF_BRIGHTLIGHT)
			strcat (temp, "EF_BRIGHTLIGHT | ");

		if (variables.effects & Constants::EntityEffect_DimLight)
			strcat (temp, "EntityEffect_DimLight | ");

		if (variables.effects & Constants::EF_INVLIGHT)
			strcat (temp, "EF_INVLIGHT | ");

		if (variables.effects & Constants::EF_NOINTERP)
			strcat (temp, "EF_NOINTERP | ");

		if (variables.effects & Constants::EF_LIGHT)
			strcat (temp, "EF_LIGHT | ");

		if (variables.effects & Constants::EntityEffect_NoDraw)
			strcat (temp, "EntityEffect_NoDraw | ");

		if (GetStringLength <unsigned short> (temp) > 2u)
			temp[GetStringLength <unsigned short> (temp) - 3u] = '\0';	// remove trailing " | "

		file.PrintFormat ("variables.effects = %i (%s)\n", variables.effects, temp);
		file.PrintFormat ("variables.gravity = %.3f (fraction /1 of normal)\n", variables.gravity);
		file.PrintFormat ("variables.friction = %.3f\n", variables.friction);
		file.PrintFormat ("variables.lightLevel = %i\n", variables.lightLevel);
		file.PrintFormat ("variables.sequence = %i (\"%s\")\n", variables.sequence, Globals::g_halfLifeEngine->GetStringFromOffset (variables.sequence).GetData ());
		file.PrintFormat ("variables.gaitSequence = %i (\"%s\")\n", variables.gaitSequence, Globals::g_halfLifeEngine->GetStringFromOffset (variables.gaitSequence).GetData ());
		file.PrintFormat ("variables.frame = %.3f (position /255 of total in sequence)\n", variables.frame);
		file.PrintFormat ("variables.animationTime = %.3f (current time is %.3f)\n", variables.animationTime, Globals::g_halfLifeEngine->GetTime ());
		file.PrintFormat ("variables.frameRate = %.3f (times normal speed)\n", variables.frameRate);
		file.PrintFormat ("variables.controller = {%i, %i, %i, %i}\n", variables.controller[0u], variables.controller[1u], variables.controller[2u], variables.controller[3u]);
		file.PrintFormat ("variables.blending = {%i, %i}\n", variables.blending[0u], variables.blending[1u]);
		file.PrintFormat ("variables.scale = %.3f\n", variables.scale);
		file.PrintFormat ("variables.renderMode = %i (%s)\n", variables.renderMode, variables.renderMode == Constants::kRenderNormal ? "kRenderNormal" : variables.renderMode == Constants::kRenderTransColor ? "kRenderTransColor" : variables.renderMode == Constants::kRenderTransTexture ? "kRenderTransTexture" : variables.renderMode == Constants::kRenderGlow ? "kRenderGlow" : variables.renderMode == Constants::kRenderTransAlpha ? "kRenderTransAlpha" : variables.renderMode == Constants::kRenderTransAdd ? "kRenderTransAdd" : "???");
		file.PrintFormat ("variables.renderAmount = %.3f\n", variables.renderAmount);
		file.PrintFormat ("variables.renderColor = Vector (%.1f, %.1f, %.1f) (RGB)\n", variables.renderColor.x, variables.renderColor.y, variables.renderColor.z);
		file.PrintFormat ("variables.renderFx = %i (%s)\n", variables.renderFx, variables.renderFx == Constants::kRenderFxNone ? "kRenderFxNone" : variables.renderFx == Constants::kRenderFxPulseSlow ? "kRenderFxPulseSlow" : variables.renderFx == Constants::kRenderFxPulseFast ? "kRenderFxPulseFast" : variables.renderFx == Constants::kRenderFxPulseSlowWide ? "kRenderFxPulseSlowWide" : variables.renderFx == Constants::kRenderFxPulseFastWide ? "kRenderFxPulseFastWide" : variables.renderFx == Constants::kRenderFxFadeSlow ? "kRenderFxFadeSlow" : variables.renderFx == Constants::kRenderFxFadeFast ? "kRenderFxFadeFast" : variables.renderFx == Constants::kRenderFxSolidSlow ? "kRenderFxSolidSlow" : variables.renderFx == Constants::kRenderFxSolidFast ? "kRenderFxSolidFast" : variables.renderFx == Constants::kRenderFxStrobeSlow ? "kRenderFxStrobeSlow" : variables.renderFx == Constants::kRenderFxStrobeFast ? "kRenderFxStrobeFast" : variables.renderFx == Constants::kRenderFxStrobeFaster ? "kRenderFxStrobeFaster" : variables.renderFx == Constants::kRenderFxFlickerSlow ? "kRenderFxFlickerSlow" : variables.renderFx == Constants::kRenderFxFlickerFast ? "kRenderFxFlickerFast" : variables.renderFx == Constants::kRenderFxNoDissipation ? "kRenderFxNoDissipation" : variables.renderFx == Constants::kRenderFxDistort ? "kRenderFxDistort" : variables.renderFx == Constants::kRenderFxHologram ? "kRenderFxHologram" : variables.renderFx == Constants::kRenderFxDeadPlayer ? "kRenderFxDeadPlayer" : variables.renderFx == Constants::kRenderFxExplode ? "kRenderFxExplode" : variables.renderFx == Constants::kRenderFxGlowShell ? "kRenderFxGlowShell" : variables.renderFx == Constants::kRenderFxClampMinScale ? "kRenderFxClampMinScale" : "???");
		file.PrintFormat ("variables.health = %.3f\n", variables.health);
		file.PrintFormat ("variables.frags = %.3f\n", variables.frags);

		for (unsigned char index (Constants::WeaponID_None); index < Constants::MaximumWeapons; ++index)
			temp[index] = (variables.weapons & BIT (index)) ? '1' : '0';

		file.PrintFormat ("variables.weapons = %u (%s)\n", variables.weapons, temp);
		file.PrintFormat ("variables.takeDamageType = %.3f (%s)\n", variables.takeDamageType, variables.takeDamageType == Constants::DAMAGE_NO ? "DAMAGE_NO" : variables.takeDamageType == Constants::DAMAGE_YES ? "DAMAGE_YES" : variables.takeDamageType == Constants::DAMAGE_AIM ? "DAMAGE_AIM" : "???");
		file.PrintFormat ("variables.deadValue = %i (%s)\n", variables.deadValue, variables.deadValue == Constants::DEAD_NO ? "DEAD_NO" : variables.deadValue == Constants::DEAD_DYING ? "DEAD_DYING" : variables.deadValue == Constants::DEAD_DEAD ? "DEAD_DEAD" : variables.deadValue == Constants::DEAD_RESPAWNABLE ? "DEAD_RESPAWNABLE" : variables.deadValue == Constants::DEAD_DISCARDBODY ? "DEAD_DISCARDBODY" : "???");
		file.PrintFormat ("variables.viewOffset = Vector (%.1f, %.1f, %.1f)\n", variables.viewOffset.x, variables.viewOffset.y, variables.viewOffset.z);

		sprintf (temp, "");	// prepare pattern string

		if (variables.buttons & Constants::IN_ATTACK)
			strcat (temp, "IN_ATTACK | ");

		if (variables.buttons & Constants::IN_JUMP)
			strcat (temp, "IN_JUMP | ");

		if (variables.buttons & Constants::IN_DUCK)
			strcat (temp, "IN_DUCK | ");

		if (variables.buttons & Constants::IN_FORWARD)
			strcat (temp, "IN_FORWARD | ");

		if (variables.buttons & Constants::IN_BACK)
			strcat (temp, "IN_BACK | ");

		if (variables.buttons & Constants::IN_USE)
			strcat (temp, "IN_USE | ");

		if (variables.buttons & Constants::IN_CANCEL)
			strcat (temp, "IN_CANCEL | ");

		if (variables.buttons & Constants::IN_LEFT)
			strcat (temp, "IN_LEFT | ");

		if (variables.buttons & Constants::IN_RIGHT)
			strcat (temp, "IN_RIGHT | ");

		if (variables.buttons & Constants::IN_MOVELEFT)
			strcat (temp, "IN_MOVELEFT | ");

		if (variables.buttons & Constants::IN_MOVERIGHT)
			strcat (temp, "IN_MOVERIGHT | ");

		if (variables.buttons & Constants::IN_ATTACK2)
			strcat (temp, "IN_ATTACK2 | ");

		if (variables.buttons & Constants::IN_RUN)
			strcat (temp, "IN_RUN | ");

		if (variables.buttons & Constants::IN_RELOAD)
			strcat (temp, "IN_RELOAD | ");

		if (variables.buttons & Constants::IN_ALT1)
			strcat (temp, "IN_ALT1 | ");

		if (variables.buttons & Constants::IN_SCORE)
			strcat (temp, "IN_SCORE | ");

		if (GetStringLength <unsigned short> (temp) > 2u)
			temp[GetStringLength <unsigned short> (temp) - 3u] = '\0';	// remove trailing " | "

		file.PrintFormat ("variables.buttons = %i (%s)\n", variables.buttons, temp);
		file.PrintFormat ("variables.impulse = %i\n", variables.impulse);

		if (variables.chain->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.chain->GetIndex (), variables.chain->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.chain = %s\n", temp);

		if (variables.damageInflictor->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.damageInflictor->GetIndex (), variables.damageInflictor->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.damageInflictor = %s\n", temp);

		if (variables.enemy->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.enemy->GetIndex (), variables.enemy->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.enemy = %s\n", temp);

		if (variables.aimEntity->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.aimEntity->GetIndex (), variables.aimEntity->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.aimEntity = %s\n", temp);

		if (variables.owner->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.owner->GetIndex (), variables.owner->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.owner = %s\n", temp);

		if (variables.groundEntity->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.groundEntity->GetIndex (), variables.groundEntity->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.groundEntity = %s\n", temp);
		file.PrintFormat ("variables.spawnFlags = %i\n", variables.spawnFlags);

		sprintf (temp, "");	// prepare pattern string

		if (variables.flags & Constants::FL_FLY)
			strcat (temp, "FL_FLY | ");

		if (variables.flags & Constants::FL_SWIM)
			strcat (temp, "FL_SWIM | ");

		if (variables.flags & Constants::FL_CONVEYOR)
			strcat (temp, "FL_CONVEYOR | ");

		if (variables.flags & Constants::FL_CLIENT)
			strcat (temp, "FL_CLIENT | ");

		if (variables.flags & Constants::FL_INWATER)
			strcat (temp, "FL_INWATER | ");

		if (variables.flags & Constants::FL_MONSTER)
			strcat (temp, "FL_MONSTER | ");

		if (variables.flags & Constants::FL_GODMODE)
			strcat (temp, "FL_GODMODE | ");

		if (variables.flags & Constants::FL_NOTARGET)
			strcat (temp, "FL_NOTARGET | ");

		if (variables.flags & Constants::FL_SKIPLOCALHOST)
			strcat (temp, "FL_SKIPLOCALHOST | ");

		if (variables.flags & Constants::FL_ONGROUND)
			strcat (temp, "FL_ONGROUND | ");

		if (variables.flags & Constants::FL_PARTIALGROUND)
			strcat (temp, "FL_PARTIALGROUND | ");

		if (variables.flags & Constants::FL_WATERJUMP)
			strcat (temp, "FL_WATERJUMP | ");

		if (variables.flags & Constants::FL_FROZEN)
			strcat (temp, "FL_FROZEN | ");

		if (variables.flags & Constants::FL_FAKECLIENT)
			strcat (temp, "FL_FAKECLIENT | ");

		if (variables.flags & Constants::FL_DUCKING)
			strcat (temp, "FL_DUCKING | ");

		if (variables.flags & Constants::FL_FLOAT)
			strcat (temp, "FL_FLOAT | ");

		if (variables.flags & Constants::FL_GRAPHED)
			strcat (temp, "FL_GRAPHED | ");

		if (variables.flags & Constants::FL_IMMUNE_WATER)
			strcat (temp, "FL_IMMUNE_WATER | ");

		if (variables.flags & Constants::FL_IMMUNE_SLIME)
			strcat (temp, "FL_IMMUNE_SLIME | ");

		if (variables.flags & Constants::FL_IMMUNE_LAVA)
			strcat (temp, "FL_IMMUNE_LAVA | ");

		if (variables.flags & Constants::FL_PROXY)
			strcat (temp, "FL_PROXY | ");

		if (variables.flags & Constants::FL_ALWAYSTHINK)
			strcat (temp, "FL_ALWAYSTHINK | ");

		if (variables.flags & Constants::FL_BASEVELOCITY)
			strcat (temp, "FL_BASEVELOCITY | ");

		if (variables.flags & Constants::FL_MONSTERCLIP)
			strcat (temp, "FL_MONSTERCLIP | ");

		if (variables.flags & Constants::FL_ONTRAIN)
			strcat (temp, "FL_ONTRAIN | ");

		if (variables.flags & Constants::FL_WORLDBRUSH)
			strcat (temp, "FL_WORLDBRUSH | ");

		if (variables.flags & Constants::FL_SPECTATOR)
			strcat (temp, "FL_SPECTATOR | ");

		if (variables.flags & Constants::FL_CUSTOMENTITY)
			strcat (temp, "FL_CUSTOMENTITY | ");

		if (variables.flags & Constants::FL_KILLME)
			strcat (temp, "FL_KILLME | ");

		if (variables.flags & Constants::FL_DORMANT)
			strcat (temp, "FL_DORMANT | ");

		if (GetStringLength <unsigned short> (temp) > 2u)
			temp[GetStringLength <unsigned short> (temp) - 3u] = '\0';	// remove trailing " | "

		file.PrintFormat ("variables.flags = %i (%s)\n", variables.flags, temp);
		file.PrintFormat ("variables.colorMap = %i (0x%X)\n", variables.colorMap, variables.colorMap);
		file.PrintFormat ("variables.team = %i\n", variables.team);
		file.PrintFormat ("variables.maximumHealth = %.3f\n", variables.maximumHealth);
		file.PrintFormat ("variables.teleportTime = %.3f\n", variables.teleportTime);
		file.PrintFormat ("variables.armorType = %.3f\n", variables.armorType);
		file.PrintFormat ("variables.armorValue = %.3f\n", variables.armorValue);
		file.PrintFormat ("variables.waterLevel = %i (%s)\n", variables.waterLevel, variables.waterLevel == Constants::EntityWaterLevel_No ? "not in water" : variables.waterLevel == Constants::EntityWaterLevel_2 ? "walking in water" : variables.waterLevel == Constants::EntityWaterLevel_3 ? "swimming in water" : "???");
		file.PrintFormat ("variables.waterType = %i\n", variables.waterType);
		file.PrintFormat ("variables.target = %i (\"%s\")\n", variables.target, GetTarget ().GetData ());
		file.PrintFormat ("variables.targetName = %i (\"%s\")\n", variables.targetName, GetTargetName ().GetData ());
		file.PrintFormat ("variables.netName = %i (\"%s\")\n", variables.netName, GetNetName ().GetData ());
		file.PrintFormat ("variables.message = %i (\"%s\")\n", variables.message, Globals::g_halfLifeEngine->GetStringFromOffset (variables.message));
		file.PrintFormat ("variables.damageTake = %.3f\n", variables.damageTake);
		file.PrintFormat ("variables.damageSave = %.3f\n", variables.damageSave);
		file.PrintFormat ("variables.damage = %.3f\n", variables.damage);
		file.PrintFormat ("variables.damageTime = %.3f (current time is %.3f)\n", variables.damageTime, Globals::g_halfLifeEngine->GetTime ());
		file.PrintFormat ("variables.noise = %i (\"%s\")\n", variables.noise, Globals::g_halfLifeEngine->GetStringFromOffset (variables.noise));
		file.PrintFormat ("variables.noise1 = %i (\"%s\")\n", variables.noise1, Globals::g_halfLifeEngine->GetStringFromOffset (variables.noise1));
		file.PrintFormat ("variables.noise2 = %i (\"%s\")\n", variables.noise2, Globals::g_halfLifeEngine->GetStringFromOffset (variables.noise2));
		file.PrintFormat ("variables.noise3 = %i (\"%s\")\n", variables.noise3, Globals::g_halfLifeEngine->GetStringFromOffset (variables.noise3));
		file.PrintFormat ("variables.speed = %.3f\n", variables.speed);
		file.PrintFormat ("variables.airFinished = %.3f\n", variables.airFinished);
		file.PrintFormat ("variables.painFinished = %.3f\n", variables.painFinished);
		file.PrintFormat ("variables.radsuitFinished = %.3f\n", variables.radsuitFinished);

		if (variables.containingEntity->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.containingEntity->GetIndex (), variables.containingEntity->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.containingEntity = %s\n", temp);
		file.PrintFormat ("variables.playerClass = %i\n", variables.playerClass);
		file.PrintFormat ("variables.maximumSpeed = %.3f\n", variables.maximumSpeed);
		file.PrintFormat ("variables.fov = %.3f\n", variables.fov);
		file.PrintFormat ("variables.weaponAnimation = %i\n", variables.weaponAnimation);
		file.PrintFormat ("variables.pushMsec = %i\n", variables.pushMsec);
		file.PrintFormat ("variables.isInDuck = %i (%s)\n", variables.isInDuck, variables.isInDuck == TRUE ? "true" : "false");
		file.PrintFormat ("variables.timeStepSound = %i (current time is %.3f)\n", variables.timeStepSound, Globals::g_halfLifeEngine->GetTime ());
		file.PrintFormat ("variables.swimTime = %i (current time is %.3f)\n", variables.swimTime, Globals::g_halfLifeEngine->GetTime ());
		file.PrintFormat ("variables.duckTime = %i (current time is %.3f)\n", variables.duckTime, Globals::g_halfLifeEngine->GetTime ());
		file.PrintFormat ("variables.stepLeft = %i\n", variables.stepLeft);
		file.PrintFormat ("variables.fallVelocity = %.3f\n", variables.fallVelocity);
		file.PrintFormat ("variables.gameState = %i\n", variables.gameState);
		file.PrintFormat ("variables.oldButtons = %i\n", variables.oldButtons);
		file.PrintFormat ("variables.groupInfo = %u\n", variables.groupInfo);
		file.PrintFormat ("variables.iuser1 = %i\n", variables.iuser1);
		file.PrintFormat ("variables.iuser2 = %i\n", variables.iuser2);
		file.PrintFormat ("variables.iuser3 = %i\n", variables.iuser3);
		file.PrintFormat ("variables.iuser4 = %i\n", variables.iuser4);
		file.PrintFormat ("variables.fuser1 = %.3f\n", variables.fuser1);
		file.PrintFormat ("variables.fuser2 = %.3f\n", variables.fuser2);
		file.PrintFormat ("variables.fuser3 = %.3f\n", variables.fuser3);
		file.PrintFormat ("variables.fuser4 = %.3f\n", variables.fuser4);
		file.PrintFormat ("variables.vuser1 = Vector (%.1f, %.1f, %.1f)\n", variables.vuser1.x, variables.vuser1.y, variables.vuser1.z);
		file.PrintFormat ("variables.vuser2 = Vector (%.1f, %.1f, %.1f)\n", variables.vuser2.x, variables.vuser2.y, variables.vuser2.z);
		file.PrintFormat ("variables.vuser3 = Vector (%.1f, %.1f, %.1f)\n", variables.vuser3.x, variables.vuser3.y, variables.vuser3.z);
		file.PrintFormat ("variables.vuser4 = Vector (%.1f, %.1f, %.1f)\n", variables.vuser4.x, variables.vuser4.y, variables.vuser4.z);

		if (variables.euser1->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.euser1->GetIndex (), variables.euser1->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.euser1 = %s\n", temp);

		if (variables.euser2->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.euser2->GetIndex (), variables.euser2->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.euser2 = %s\n", temp);

		if (variables.euser3->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.euser3->GetIndex (), variables.euser3->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.euser3 = %s\n", temp);

		if (variables.euser4->IsValid ())
			sprintf (temp, "Entity #%i (\"%s\")", variables.euser4->GetIndex (), variables.euser4->GetClassName ().GetData ());
		else
			sprintf (temp, "NULL");

		file.PrintFormat ("variables.euser4 = %s\n\n", temp);
	}

	//
	// Group: Engine functions.
	//
	public:
		//
		// Function: SetOrigin
		//
		// Description: Sets the entity origin.
		//
		// Parameters:
		//	newOrigin - New origin of entity.
		//
		inline void          SetOrigin          (const Math::Vector3D &newOrigin) { Globals::g_halfLifeEngine->SetEntityOrigin (this, newOrigin); }

		//
		// Function: SetModel
		//
		// Description: Sets the entity model.
		//
		// Parameters:
		//	newModelName - New model name of entity.
		//
		inline void          SetModel          (const DynamicString &newModelName) { Globals::g_halfLifeEngine->SetEntityModel (this, newModelName); }

		inline char *const   GetInfoKeyBuffer  (void) const                 { return Globals::g_halfLifeEngine->GetEntityInfoKeyBuffer (this); }
		inline void          FreePrivateData   (void)                       { Globals::g_halfLifeEngine->FreeEntityPrivateData (this); }
		inline void          RunPlayerMove     (const Math::Angles3D &moveAngles, const float forwardMoveSpeed, const float sideMoveSpeed, const unsigned char msecValue)
		{
			Globals::g_halfLifeEngine->RunPlayerMove (this, moveAngles, forwardMoveSpeed, sideMoveSpeed, 0.0f, static_cast <unsigned short> (variables.buttons), static_cast <Constants::PlayerImpulseID_t> (variables.impulse), msecValue);
		}

		inline Structures::StudioHeader_t *const GetModelPointer (void) const { return Globals::g_halfLifeEngine->GetEdictModelPointer (this); }

		//
		// Function: GetIllumination
		//
		// Description: Gets the entity illumination.
		//
		// Returns: Value between 0 and 255 corresponding to the entity's illumination.
		//
		inline const unsigned char GetIllumination (void) const
		{
			// Ask the engine for the entity illumination and filter it so as to return a usable value.
			return Globals::g_halfLifeEngine->GetEntityIllumination (this);
		}

		//
		// Function: GetPlayerIllumination
		//
		// Description: Gets the player illumination. (plus virtual muzzle flash)
		//
		// Returns: Value between 0 and 255 corresponding to the player's illumination.
		//
		inline const unsigned char GetPlayerIllumination (void) const
		{
			// Ask the engine for the player illumination and filter it so as to return a usable value.
			return static_cast <const unsigned char> (Math::GetMinimumValueBetween <unsigned short> (static_cast <const unsigned short> (variables.lightLevel + GetPrivateDataReference <unsigned int> (Constants::OFFSET_WEAPON_FLASH)), Constants::MaximumIlluminationValue));
		}

		//
		// Function: GetIlluminationPercentage
		//
		// Description: Gets the entity illumination percentage.
		//
		// Returns: Value between 0.0 and 100.0 corresponding to the entity's illumination percentage.
		//
		inline const float GetIlluminationPercentage (void) const
		{
			// Ask the engine for the entity illumination and filter it so as to return a usable value.
			return static_cast <const float> (GetIllumination ()) / Constants::MaximumIlluminationValue * 100.0f;
		}

		//
		// Function: GetPlayerIlluminationPercentage
		//
		// Description: Gets the entity illumination percentage. (plus virtual muzzle flash)
		//
		// Returns: Value between 0.0 and 100.0 corresponding to the entity's illumination percentage.
		//
		inline const float GetPlayerIlluminationPercentage (void) const
		{
			// Ask the engine for the entity illumination and filter it so as to return a usable value.
			return static_cast <const float> (GetPlayerIllumination ()) / Constants::MaximumIlluminationValue * 100.0f;
		}

		inline void EmitSound (const Constants::SoundChannel_t channel, const DynamicString &sample, const float volume = Constants::SoundVolume_Normal, const float attenuation = Constants::SoundAttenuation_Normal, const unsigned int flags = 0u, const Constants::SoundPitch_t pitch = Constants::SoundPitch_Normal) const
		{
			Globals::g_halfLifeEngine->EmitSound (this, channel, sample, volume, attenuation, flags, pitch);
		}
};	// sizeof (Edict) == 804 (201)

inline const TypeDefinitions::EntityOffset_t BaseEntity::GetOffset (void) const { return GetEdict ()->GetOffset (); }
inline const unsigned short                  BaseEntity::GetIndex  (void) const { return GetEdict ()->GetIndex (); }

#endif	// ifndef EDICT_INCLUDED