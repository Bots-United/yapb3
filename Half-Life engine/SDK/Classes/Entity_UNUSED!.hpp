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
// Entity.hpp
//
// Class: Entity
//
// Description: Simple wrapping all entity handling stuff in a class. (Thanks to GINA bot an Whistler for this).
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

class Entity
{
	//
	// Group: Protected members.
	//
	protected:
		SDK::Classes::Edict *m_edict;	// Don't need to save/restore this pointer, the engine resets it.

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: Entity
		//
		// Description: Constructs layer entity, from engine entity.
		//
		inline Entity (SDK::Classes::Edict *edict = NULL) : m_edict (edict) { /*VOID*/ }

		//
		// Function: Entity
		//
		// Description: Constructs layer entity, from engine constant entity variables.
		//
		inline Entity (const SDK::Structures::EntityVariables_t *const entityVariables) : m_edict (Utilities::IsNullEntity (entityVariables) ? NULL : entityVariables->containingEntity) { /*VOID*//* }

		//
		// Function: Entity
		//
		// Description: Entity copying constructor.
		//
		// Parameters:
		//	entity - Entity edict to assign.
		//
		inline Entity (const Entity &entity) : m_edict (entity.m_edict) { /*VOID*/ }

		//
		// Function: ~Entity
		//
		// Description: Empty entity destructor.
		//
		virtual inline ~Entity (void) { /*VOID*/ }

	//
	// Group: Operators.
	//
	public:
//		inline Entity &operator = (SDK::Classes::Edict *edict)                    { m_edict = edict; return *this; }
//		inline Entity &operator = (const SDK::Structures::EntityVariables_t *const entityVariables) { m_edict = Utilities::IsNullEntity (entityVariables) ? NULL : entityVariables->containingEntity; return *this; }
//		inline Entity &operator = (const Entity &entity)                               { if (this != &entity) { m_edict = entity.m_edict; } return *this; }

		inline       SDK::Classes::Edict *      operator () (void)       { return m_edict; }
		inline const SDK::Classes::Edict *const operator () (void) const { return m_edict; }

		// client will now automatically convert to 'SDK::Classes::Edict *' and 'const SDK::Classes::Edict *const' when needed
		inline operator       SDK::Classes::Edict *      (void)       { return m_edict; }
		inline operator       SDK::Classes::Edict *      (void) const { return m_edict; }	// needed too :{O
		inline operator const SDK::Classes::Edict *const (void) const { return m_edict; }

		// client will now automatically convert to 'SDK::Structures::EntityVariables_t *' and 'const SDK::Structures::EntityVariables_t *const' when needed
		inline operator       SDK::Structures::EntityVariables_t *      (void)       { InternalAssert (m_edict->IsValid ()); return &m_edict->variables; }
		inline operator       SDK::Structures::EntityVariables_t *      (void) const { InternalAssert (m_edict->IsValid ()); return &m_edict->variables; }	// needed too :{O
		inline operator const SDK::Structures::EntityVariables_t *const (void) const { InternalAssert (m_edict->IsValid ()); return &m_edict->variables; }
/*
		// Equality.
		inline const bool operator        == (const Entity &right)                                               const { return m_edict == right.m_edict; }
		inline const bool operator        == (const SDK::Classes::Edict *const edict)                            const { return m_edict == edict; }
		friend inline const bool operator == (const SDK::Classes::Edict *const edict, const Entity &otherEntity)       { return otherEntity == edict; }
		inline const bool operator        != (const Entity &right)                                               const { return !(*this == right); }
		inline const bool operator        != (const SDK::Classes::Edict *const edict)                            const { return !(*this == edict); }
		friend inline const bool operator != (const SDK::Classes::Edict *const edict, const Entity &otherEntity)       { return otherEntity != edict; }
*/
	//
	// Group: Functions.
	//
	public:
		template <typename elementType> inline const elementType &GetPrivateData (const SDK::Constants::EntityPrivateDataOffset_t offset) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return static_cast <const elementType *> (m_edict->privateData)[offset];
		}

		// From Half-Life SDK cbase.h:

		//
		// Function: GetEntity
		//
		// Description: Gets the engine entity pointer.
		//
		// Returns: Engine entity pointer.
		//
//		inline SDK::Classes::Edict      *GetEdict           (void)                     { return m_edict; }
		//
		// Function: GetEntity
		//
		// Description: Gets the constant engine entity pointer.
		//
		// Returns: Constant engine entity pointer.
		//
//		inline const SDK::Classes::Edict *const GetEdict    (void) const               { return m_edict; }

		//
		// Function: GetEntityIndex
		//
		// Description: Gets the engine entity index.
		//
		// Returns: Entity index.
		//
		inline const unsigned int    GetIndex          (void) const               { return static_cast <unsigned int> (GET_EDICT_INDEX (m_edict)); }

		virtual inline const bool    IsFakeClient      (const bool YaPB = false) const { return IsPlayer () && (m_edict->variables.flags & SDK::Constants::FL_FAKECLIENT) != 0; }

		//
		// Function: IsValid
		//
		// Description: Checks whether entity is valid.
		//
		// Returns: True if entity is valid, false otherwise.
		//
		inline const bool    IsValid            (void) const                { return this != NULL && !Utilities::IsNullEntity (m_edict); }

		//
		// Function: IsPlayer
		//
		// Description: Checks whether entity is player.
		//
		// Returns: True if player, false otherwise.
		//
		virtual /*inline */const bool    IsPlayer           (void) const;

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

//			return !Math::IsZero (m_edict->variables.absoluteBoundingBox.mins.x) || !Math::IsZero (m_edict->variables.absoluteBoundingBox.mins.y) || !Math::IsZero (m_edict->variables.absoluteBoundingBox.mins.z) || !Math::IsZero (m_edict->variables.absoluteBoundingBox.maxs.x) || !Math::IsZero (m_edict->variables.absoluteBoundingBox.maxs.y) || !Math::IsZero (m_edict->variables.absoluteBoundingBox.maxs.z);
			return m_edict->variables.absoluteBoundingBox.mins != Math::Vector3D::ZeroValue || m_edict->variables.absoluteBoundingBox.maxs != Math::Vector3D::ZeroValue;
		}
/*
		//
		// Function: GetAbsMin
		//
		// Description: Gets absolute entity mins.
		//
		// Returns: Mins of entity in world space.
		//
		inline const Math::Vector3D  GetAbsoluteMin    (void) const              { return m_edict->variables.absoluteBoundingBox.mins; }
		//
		// Function: GetAbsMax
		//
		// Description: Gets absolute entity maxs.
		//
		// Returns: Maxs of entity in world space.
		//
		inline const Math::Vector3D  GetAbsoluteMax    (void) const              { return m_edict->variables.absoluteBoundingBox.maxs; }
		//
		// Function: GetMins
		//
		// Description: Gets entity maxs.
		//
		// Returns: Mins of entity.
		//
		inline const Math::Vector3D  GetMins           (void) const              { return m_edict->variables.boundingBox.mins; }
		//
		// Function: GetMaxs
		//
		// Description: Gets entity maxs.
		//
		// Returns: Maxs of entity.
		//
		inline const Math::Vector3D  GetMaxs           (void) const              { return m_edict->variables.boundingBox.maxs; }
*/
		//
		// Function: GetTakeDamageType
		//
		// Description: Gets entity take damage type.
		//
		// Returns: damage type of entity.
		//
		inline const float  GetTakeDamageType   (void) const                { return m_edict->variables.takeDamageType; }

		//
		// Function: GetClassName
		//
		// Description: Gets entity class name.
		//
		// Returns: Class name of entity.
		//
		inline const DynamicString  GetClassName       (void) const                { return Utilities::GetStringFromOffset (m_edict->variables.className); }

		inline const DynamicString  GetNetName         (void) const                { return Utilities::GetStringFromOffset (m_edict->variables.netName); }
		//
		// Function: GetModel
		//
		// Description: Gets the entity model name.
		//
		// Returns: Model name of entity.
		//
		inline const DynamicString  GetModel           (void) const
		{
			return IsPlayer () ? INFOKEY_VALUE (GET_INFOKEYBUFFER (m_edict), "model") : Utilities::GetStringFromOffset (m_edict->variables.model);
		}
		//
		// Function: GetViewModel
		//
		// Description: Gets entity viewable model.
		//
		// Returns: Viewable model of entity.
		//
		inline const DynamicString  GetViewModel       (void) const                { return Utilities::GetStringFromOffset (m_edict->variables.viewModelName); }
		//
		// Function: GetHealth
		//
		// Description: Gets the health value of entity.
		//
		// Returns: Health value.
		//
		inline const int     GetHealth          (void) const                { return static_cast <int> (m_edict->variables.health); }
		//
		// Function: GetCenter
		//
		// Description: Gets the entity center.
		//
		// Returns: Center point origin of entity.
		//
		inline const Math::Vector3D  GetCenter         (void) const              { return m_edict->variables.absoluteBoundingBox.maxs.GetMidPoint (m_edict->variables.absoluteBoundingBox.mins); }
		//
		// Function: GetOrigin
		//
		// Description: Gets entity origin in world space.
		//
		// Returns: Entity origin.
		//
//		inline const Math::Vector3D  GetOrigin         (void) const              { return HasBoundingBox () ? GetCenter () : m_edict->variables.origin; }
		inline const Math::Vector3D  GetOrigin         (void) const              { return HasBoundingBox () ? GetCenter () : m_edict->variables.origin == Math::Vector3D::ZeroValue ? m_edict->variables.absoluteBoundingBox.mins + (m_edict->variables.size * 0.5f) : m_edict->variables.origin; }
		//
		// Function: GetEyePosition
		//
		// Description: Gets eye position of entity.
		//
		// Returns: Eye position of entity.
		//
		inline const Math::Vector3D  GetEyePosition    (void) const              { return m_edict->variables.origin + m_edict->variables.viewOffset; }	// position of eyes/ears/gun
		//
		// Function: GetVelocity
		//
		// Description: Gets the entity velocity.
		//
		// Returns: Velocity of entity.
		//
//		inline const Math::Vector3D  GetVelocity       (void) const              { return m_edict->variables.velocity; }
		//
		// Function: GetSpeed
		//
		// Description: Gets the entity speed.
		//
		// Returns: Speed of entity.
		//
		inline const float   GetSpeed           (void) const                { return m_edict->variables.velocity.GetLength (); }
		inline const float   GetSpeed2D         (void) const                { return m_edict->variables.velocity.GetLength2D (); }
		//
		// Function: GetArmor
		//
		// Description: Gets the armor value of entity.
		//
		// Returns: Armor value of entity.
		//
//		inline const float   GetArmor           (void) const                { return m_edict->variables.armorValue; }
//		inline const float   GetArmorType       (void) const             { return m_edict->variables.armorType; }
		//
		// Function: GetMaximumSpeed
		//
		// Description: Gets the maximum speed of entity.
		//
		// Returns: Maximum speed of entity.
		//
		inline const float   GetMaximumSpeed    (void) const                { return m_edict->variables.maximumSpeed; }
		//
		// Function: GetBodyAngles
		//
		// Description: Gets body angles of entity,
		//
		// Returns: Body angles of entity.
		//
//		inline const Math::Vector3D  GetBodyAngles      (void) const              { return m_edict->variables.modelAngles; }
		//
		// Function: GetViewAngles
		//
		// Description: Gets view angles of entity.
		//
		// Returns: View angles of entity.
		//
		inline const Math::Vector3D  GetViewAngles      (void) const              { return m_edict->variables.viewAngles; }
		//
		// Function: GetFov
		//
		// Description: Gets the field of view of entity.
		//
		// Returns: FOV of entity.
		//
//		inline const float   GetFov             (void) const                { return m_edict->variables.fov; }

//		inline const SDK::Constants::MoveType_t GetMoveType     (void) const                { return m_edict->variables.moveType; }
//		inline const int     GetSpawnFlags      (void) const              { return m_edict->variables.spawnFlags; }
		//
		// Function: GetFlags
		//
		// Description: Gets the entity flags.
		//
		// Returns: Entity flags currently set.
		//
//		inline const int     GetFlags           (void) const                { return m_edict->variables.flags; }
		//
		// Function: GetWaterLevel
		//
		// Description: Gets the water level.
		//
		// Returns: Water level for entity.
		//
//		inline const SDK::Constants::EntityWaterLevel_t     GetWaterLevel      (void) const                { return m_edict->variables.waterLevel; }

		//
		// Function: GetTargetName
		//
		// Description: Gets the entity target name.
		//
		// Returns: Target name of entity.
		//
		inline const DynamicString  GetTargetName      (void) const                { return Utilities::GetStringFromOffset (m_edict->variables.targetName); }
		//
		// Function: GetTarget
		//
		// Description: Gets the entity target.
		//
		// Returns: Target of entity.
		//
		inline const DynamicString  GetTarget          (void) const                { return Utilities::GetStringFromOffset (m_edict->variables.target); }
/*
		//
		// Function: ReleaseButtons
		//
		// Description: Releases all pressed buttons.
		//
		inline void          ReleaseAllButtons  (void)                      { m_edict->variables.buttons = 0; }
		//
		// Function: PushButton
		//
		// Description: Pushes specified button.
		//
		// Parameters:
		//	button - Button to be pushed.
		//
		inline void          PushButton         (const SDK::Constants::ButtonType_t button) { m_edict->variables.buttons |= button; }
		//
		// Function: ReleaseButton
		//
		// Description: Releases specified button.
		//
		// Parameters:
		//	button - Button to be released.
		//
		inline void          ReleaseButton      (const SDK::Constants::ButtonType_t button) { m_edict->variables.buttons &= ~button; }*/
		//
		// Function: GetButtons
		//
		// Description: Gets the entity pushed buttons bitmask.
		//
		// Returns: Bitmask of pushed buttons.
		//
		inline const int     GetPushedButtons   (void) const                { return m_edict->variables.buttons; }

		inline void          SetAngles          (const Math::Vector3D &viewAngles)
		{
			m_edict->variables.modelAngles = m_edict->variables.viewAngles = viewAngles;
			m_edict->variables.modelAngles.x /= -3.0f;

			m_edict->variables.modelAngles.ClampAngles ();
			m_edict->variables.viewAngles.ClampAngles ();
		}

		//
		// Function: GetWeaponAnim
		//
		// Description: Gets the weapon animation.
		//
		// Returns: Weapon Animation.
		//
		inline const int     GetWeaponAnimation (void) const                { return m_edict->variables.weaponAnimation; }
		//
		// Function: GetWeapons
		//
		// Description: Gets the owned weapons bitmask.
		//
		// Returns: Bitmask of owned weapons.
		//
		inline const int     GetWeapons         (void) const                { return m_edict->variables.weapons; }

		//
		// Function: GetOwner
		//
		// Description: Gets the entity owner.
		//
		// Returns: Pointer to entity owner.
		//
		inline Entity       *GetOwner           (void)//                      { return &Entity (m_edict->variables.owner); }
		{
			static Entity owner;

			owner = m_edict->variables.owner;

			return &owner;
		}
		//
		// Function: GetGroundEntity
		//
		// Description: Gets the entities ground entity.
		//
		// Returns: Pointer to ground entity.
		//
		inline Entity       *GetGroundEntity    (void)//                      { return &Entity (m_edict->variables.groundEntity); }
		{
			static Entity groundEntity;

			groundEntity = m_edict->variables.groundEntity;

			return &groundEntity;
		}
		//
		// Function: GetDamageInflictor
		//
		// Description: Gets the entity damage inflictor.
		//
		// Returns: Pointer to damage inflictor entity.
		//
		inline const Entity  GetDamageInflictor    (void) const                { return m_edict->variables.damageInflictor; }
		inline Entity        GetDamageInflictor    (void)                      { return m_edict->variables.damageInflictor; }
/*		inline Entity       *GetDamageInflictor    (void)                      { return &Entity (m_edict->variables.damageInflictor); }
		{
			static Entity damageInflictor;

			damageInflictor = m_edict->variables.damageInflictor;

			return &damageInflictor;
		}*/

		//
		// Function: IsVisible
		//
		// Description: Checks whether entity is visible.
		//
		// Returns: True if entity is visible, false otherwise.
		//
//		inline const bool    IsVisible          (void) const                { return !(m_edict->variables.effects & SDK::Constants::EntityEffect_NoDraw) && !GetModel ().IsEmpty (); }
		//
		// Function: IsAlive
		//
		// Description: Checks whether entity is alive.
		//
		// Returns: True if entity is alive, false otherwise.
		//
		inline const bool    IsAlive            (void) const                { return m_edict->variables.deadValue == SDK::Constants::DEAD_NO && GetHealth () > 0/* && !(m_edict->variables.flags & SDK::Constants::FL_NOTARGET) && variables.moveType != Constants::MOVETYPE_NOCLIP*/; }
		//
		// Function: IsInWater
		//
		// Description: Checks whether entity is in water.
		//
		// Returns: True if entity is in water, false otherwise.
		//
		inline const bool    IsInWater          (void) const              { return GetWaterLevel () >= SDK::Constants::EntityWaterLevel_2; }
		inline const bool    IsInSlime          (void) const              { return Globals::g_halfLifeEngine->GetPointContents (GetOrigin () - Math::Vector3D (0.0f, 0.0f, 23.0f)) == SDK::Constants::Content_Slime; }
		inline const bool    IsInLava           (void) const              { return Globals::g_halfLifeEngine->GetPointContents (GetOrigin () - Math::Vector3D (0.0f, 0.0f, 23.0f)) == SDK::Constants::Content_Lava; }
		//
		// Function: IsOnFloor
		//
		// Description: Checks whether entity is standing on the floor.
		//
		// Returns: True if entity is on floor, false otherwise.
		//
		inline const bool    IsOnFloor          (void) const                { return IsPlayer () ? (m_edict->variables.flags & (SDK::Constants::FL_ONGROUND | SDK::Constants::FL_PARTIALGROUND)) != 0 : ENT_IS_ON_FLOOR (m_edict) > 0; }
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
		inline const bool    IsDucking          (void) const                { return (m_edict->variables.flags & SDK::Constants::FL_DUCKING) != 0; }

//		const bool           IsInViewCone       (Math::Vector3D &origin, float fov = 0.0) const;
//		const bool           IsInViewCone       (Entity *entity, float fov = 0.0) const;

//		const bool           IsVisible          (const Math::Vector3D &destination) const;
//		const bool           IsVisible          (Entity *entity) const;

//		const bool           IsBoxVisible       (Entity *entity, Math::Vector3D *pvHit = NULL, unsigned char *ucBodyPart = NULL) const;

//		inline void	         SetSpawnFlags      (const int newFlags)      { m_edict->variables.spawnflags |= newFlags; }
		//
		// Function: SetFlags
		//
		// Description: Sets the entity flags.
		//
		// Parameters:
		//	newFlags - New flags.
		//
//		inline void	         SetFlags           (const SDK::Constants::EntityFlag_t newFlags) { m_edict->variables.flags |= newFlags; }

		//
		// Function: SetBodyAngles
		//
		// Description: Sets sets new body angles.
		//
		// Parameters:
		//	newAngles - Angles that should be set.
		//
//		inline void	         SetBodyAngles      (const Math::Vector3D &newAngles) { m_edict->variables.modelAngles = newAngles; }
		//
		// Function: SetViewAngles
		//
		// Description: Sets new view angles.
		//
		// Parameters:
		//	newAngles - Angles that should be set.
		//
//		inline void	         SetViewAngles      (const Math::Vector3D &newAngles) { m_edict->variables.viewAngles = newAngles; }

//		void                 ChangeAngles       (const Math::Vector3D &idealAngles, float speed = 1.0f);	// speed: 0.1 - 1

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
		inline void          SetOrigin          (const Math::Vector3D &newOrigin) { SET_ORIGIN (m_edict, newOrigin); }

		//
		// Function: SetModel
		//
		// Description: Sets the entity model.
		//
		// Parameters:
		//	newModelName - New model name of entity.
		//
		inline void          SetModel          (const DynamicString &newModelName) { SET_MODEL (m_edict, newModelName); }

		//
		// Function: GetIlluminationPercentage
		//
		// Description: Gets the entity illumination percentage.
		//
		// Returns: Value between 0.0 and 100.0 corresponding to the entity's illumination percentage.
		//
		virtual inline const float GetIlluminationPercentage (void) const
		{
			// Ask the engine for the entity illumination and filter it so as to return a usable value.
			return GETENTITYILLUM (m_edict) / 255 * 100.0f;
		}
};

#endif	// ifndef ENTITY_INCLUDED