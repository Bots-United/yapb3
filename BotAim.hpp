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
// BotAim.hpp
//
// Class: Aim
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_AIM_INCLUDED
#define BOT_AIM_INCLUDED

class Aim : public SubSystem <YaPB>
{
	friend YaPB;

	friend BaseNavigator;

	friend Task_SprayLogo;
	friend Task_DoubleJump;

	public:
class Task
{
	//
	// Group: Constants.
	//
	public:
		// Defines bots aim tasks.
		enum ID_t
		{
			ID_None,			// do not look

			ID_Enemy,			// aim at enemy
			ID_LastEnemy,		// aim at last enemy
			ID_PredictEnemy,	// aim at predicted path
			ID_Vector,			// aim at spray logo origin, etc....
			ID_NavigationPoint,	// aim at navigation point (next waypoint of navigation area)
			ID_CampPoint,		// aim at camp vector
			ID_Around,			// do look around
			ID_HurtOrigin,		// aim at hurt origin
			ID_Edict,			// aim at entity like buttons, hostages
			ID_Noise,			// aim at some heared sound origin
			ID_Grenade,			// aim for grenade throw
			ID_Override,		// overrides all others (blinded)

			ID_Total
		};
/*		DECLARE_TYPED_ENUMERATION (unsigned int, Bit_t)
		{
			Bit_None            = ID_None,					// do not look

			Bit_Enemy           = BIT (ID_Enemy),			// aim at enemy
			Bit_LastEnemy       = BIT (ID_LastEnemy),		// aim at last enemy
			Bit_PredictEnemy    = BIT (ID_PredictEnemy),	// aim at predicted path
			Bit_Vector          = BIT (ID_Vector),			// aim at spray logo origin, etc....
			Bit_NavigationPoint = BIT (ID_NavigationPoint),	// aim at navigation point (next waypoint or navigation area)
			Bit_CampPoint       = BIT (ID_CampPoint),		// aim at camp vector
			Bit_Around          = BIT (ID_Around),			// do look around
			Bit_HurtOrigin      = BIT (ID_HurtOrigin),		// aim at hurt origin
			Bit_Edict           = BIT (ID_Edict),			// aim at entity like buttons, hostages
			Bit_Noise           = BIT (ID_Noise),			// aim at some heared sound origin
			Bit_Grenade         = BIT (ID_Grenade),			// aim for grenade throw
			Bit_Override        = BIT (ID_Override)			// overrides all others (blinded)
		};
*/
	#if defined _DEBUG
		//
		// Group: Private members.
		//
		private:
			const DynamicString m_informationString;
	#endif	// if defined _DEBUG

	//
	// Group: (Con/De)structors.
	//
	public:
		#if defined _DEBUG
			inline Task (const DynamicString &informationString = "") : m_informationString (informationString) { /* VOID */ }
		#endif	// if defined _DEBUG

		virtual inline ~Task (void) { /* VOID */ }

	//
	// Group: Private operators.
	//
	private:
		inline Task &operator = (const Task &/*right*/);	// Avoid "warning C4512: 'Task' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		//
		// Function: GetID
		//
		// Description: Gets a action ID.
		//
		// Returns: Action ID.
		//
		virtual inline const ID_t GetID   (void) const { return ID_None; }
/*
		//
		// Function: GetBit
		//
		// Description: Gets a action bitsum.
		//
		// Returns: Action bitsum.
		//
		inline const Bit_t        GetBit  (void) const { return BIT (GetID ()); }
*/
		#if defined _DEBUG
			virtual inline const DynamicString GetInformationString (const YaPB *const/* bot*/) const { return m_informationString; }
			virtual inline const char *const GetName (void) const { return typeid (*this).name () + GET_STATIC_STRING_LENGTH ("class YaPB::Aim::Task_"); }
		#endif	// if defined _DEBUG

	//
	// Group: Engine callbacks.
	//
	public:
		virtual inline void       Execute (YaPB *const/* bot*/) { /* VOID */ }
};

class Task_Enemy : public Task
{
	//
	// Group: Private members.
	//
	private:
		

	//
	// Group: Private functions.
	//
	private:
		

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_Enemy; }

		inline const DynamicString GetInformationString (const YaPB *const bot) const
		{
			// Reliability check.
			if (!bot->m_enemy->IsValid ())
				return "Visibility: NONE (NO ENEMY! ERROR!!!)";

			if (bot->m_enemy->visibility == Visibility_None)
				return "Visibility: NONE";

			DynamicString string ("Visibility: ");
			const DynamicString types[] =
			{
				"head",
				"body",
				"other"
			};

			STORE_FLAGS_TO_STRING (bot->m_enemy->visibility, string, 0u, GET_STATIC_ARRAY_SIZE (types), types, "+");

			return string;
		}

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (YaPB *const bot)
		{
			if (!bot->m_enemy->IsValid ())	// OCCURS!!!!!!!!!!!!!!!!!
				return;
#if 0
//			#define BONE_SPINE4		4	// trojuholnik
//			#define BONE_SPINE4		HalfLifeEngine::SDK::Constants::PlayerBoneID_Spine3	// trojuholnik
			#define BONE_SPINE4		HalfLifeEngine::SDK::Constants::PlayerBoneID_Head	// trojuholnik

			Math::Vector3D boneOrigin;

			HalfLifeEngine::Globals::g_halfLifeEngine->GetBonePosition (*bot->m_enemy->pointer, BONE_SPINE4, boneOrigin);

			{
/*				int index = pEnt->index;
				HalfLifeEngine::SDK::Structures::ClientEntity_t* ent = vPlayers[index].getEnt();
				vec3_t vFromOrigin, vToOrigin, delta;

			const unsigned char predback (0u);

				int historyIndex = ((int)(ent->current_position + HISTORY_MAX - predback)) % HISTORY_MAX;

				vFromOrigin = ent->ph[historyIndex].origin;
				vToOrigin   = ent->ph[ent->current_position].origin;

				delta[0] = vToOrigin[0] - vFromOrigin[0];
				delta[1] = vToOrigin[1] - vFromOrigin[1];
				delta[2] = vToOrigin[2] - vFromOrigin[2];

			const float predahead (0.08f);

				delta[0] *= predahead;
				delta[1] *= predahead;
				delta[2] *= predahead;

				vPlayers[index].vHitbox[0] += delta[0];
				vPlayers[index].vHitbox[1] += delta[1];
				vPlayers[index].vHitbox[2] += delta[2];*/
				boneOrigin += (bot->m_enemy->pointer->GetVelocity () - bot->GetVelocity ()) * bot->GetFrameIntervalTime () * 1.5f;
			}

			Math::Vector3D forward, right, upward;

			Math::Angles3D (0.0f, bot->m_enemy->pointer->GetViewAngles ().yaw, 0.0f).BuildVectors (forward, right, upward);

			forward.z = -forward.z;	// Invert for engine (adjust the view angle pitch to aim correctly).

			const float adjust_forward (0.5f);
			const float adjust_height (0.5f);
			const float adjust_right (0.5f);

			boneOrigin += forward * adjust_forward;
			boneOrigin += upward * adjust_height;
			boneOrigin += right * adjust_right;

			boneOrigin -= bot->m_edict->GetEyePosition ();

			Math::Angles2D newAngles (boneOrigin.ToAngles2D ());

			if (newAngles.pitch > 180.0f)
				newAngles.pitch -= 360.0f;

			if (newAngles.yaw > 180.0f)
				newAngles.yaw -= 360.0f;
#endif	// if 0
			Math::Vector3D boneOrigin (bot->GetEnemyAimPosition ());

			bot->m_aim->m_lookAt = boneOrigin;

			boneOrigin -= bot->m_edict->GetEyePosition ();

			Math::Angles2D newAngles (boneOrigin.ToAngles2D ());

			// GO INSANE!!!
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> ()/* >= 2u*/ && bot->GetProfile ()->skill == BotSkillLevel_Godlike)
			{
				newAngles.pitch = -newAngles.pitch;	// Invert for engine (adjust the view angle pitch to aim correctly).

				newAngles -= bot->GetCurrentWeapon ()->GetSpreadOffsetAngles (newAngles);	// Compensate spread.

				newAngles.pitch = -newAngles.pitch;	// Invert pitch back - temp.

				newAngles.Clamp ();
			}

			// Bot needs to the punch angle compensation.... (Depending on skill value!)
			if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_HardcoreMode)->GetValue <bool> ())
			{
				Math::Angles3D punchAngles (bot->GetPunchAngles ()/*/bot->GetModifiedPunchAngles ()*/);

//				if (!Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <bool> ())
//					punchAngles = bot->m_edict->variables.punchAngles = 0.0f;
//				else// if (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <unsigned char> () == 1)
				HalfLifeEngine::Utilities::DropPunchAngles (punchAngles, bot->GetMsecValue ()/*! @warning 'm_msec' VALUE IS FROM OLD FRAME, NEED TO COMPITE IT FOR CURRENT FRAME!!!!!! */ * 0.001f);

				punchAngles.pitch = -punchAngles.pitch;	// Invert for engine (adjust the punch angle pitch to aim correctly).

				newAngles -= punchAngles.AsAngles2D () * bot->m_profile->skill.GetIncreasingOffset ();	// Compensate recoil.
				newAngles.Clamp ();
			}

			// Set the body and view angles....
			bot->m_edict->variables.modelAngles = bot->m_edict->variables.viewAngles = newAngles;
			bot->m_edict->variables.viewAngles.pitch = -bot->GetViewAngles ().pitch;	// Invert for engine (adjust the view angle pitch to aim correctly).
			bot->m_edict->variables.modelAngles.pitch *= 1.0f / 3.0f;	// Adjust the body angle pitch to point the gun correctly.

			bot->m_edict->variables.idealPitch = bot->GetViewAngles ().pitch;
			bot->m_edict->variables.idealYaw = bot->GetViewAngles ().yaw;
		}
};

class Task_Vector : public Task
{
	//
	// Group: Private members.
	//
	private:
		Math::Vector3D m_vector;	// Point to look at.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_Vector (const Math::Vector3D &point) : m_vector (point) { /* VOID */ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_Vector; }

		inline void                  SetVector (const Math::Vector3D &vector)       { m_vector = vector; }
		inline const Math::Vector3D &GetVector (void)                         const { return m_vector; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Execute (YaPB *const bot)
		{
			bot->m_aim->m_lookAt = m_vector;

			// Set the body and view angles....
			bot->m_edict->variables.modelAngles = bot->m_edict->variables.viewAngles = (m_vector - bot->m_edict->GetEyePosition ()).ToAngles2D ();

			bot->m_edict->variables.viewAngles.pitch = -bot->GetViewAngles ().pitch;	// Invert for engine (adjust the view angle pitch to aim correctly).
			bot->m_edict->variables.modelAngles.pitch *= 1.0f / 3.0f;	// Adjust the body angle pitch to point the gun correctly.

			bot->m_edict->variables.idealPitch = bot->GetViewAngles ().pitch;
			bot->m_edict->variables.idealYaw = bot->GetViewAngles ().yaw;
		}
};

class Task_Edict : public Task
{
	//
	// Group: Private members.
	//
	private:
		const HalfLifeEngine::SDK::Classes::Edict *m_edict;	// Point to look at.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_Edict (const HalfLifeEngine::SDK::Classes::Edict *const edict) : m_edict (edict) { /* VOID */ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_Edict; }

		inline void                                             SetEdict (const HalfLifeEngine::SDK::Classes::Edict *const edict)       { m_edict = edict; }
		inline const HalfLifeEngine::SDK::Classes::Edict *const GetEdict (void)                                                   const { return m_edict; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Execute (YaPB *const bot)
		{
			InternalAssert (m_edict->IsValid ());

			bot->m_aim->m_lookAt = m_edict->GetOrigin ();

			// Set the body and view angles....
			bot->m_edict->variables.modelAngles = bot->m_edict->variables.viewAngles = (m_edict->GetOrigin () - bot->m_edict->GetEyePosition ()).ToAngles2D ();

			bot->m_edict->variables.viewAngles.pitch = -bot->GetViewAngles ().pitch;	// Invert for engine (adjust the view angle pitch to aim correctly).
			bot->m_edict->variables.modelAngles.pitch *= 1.0f / 3.0f;	// Adjust the body angle pitch to point the gun correctly.

			bot->m_edict->variables.idealPitch = bot->GetViewAngles ().pitch;
			bot->m_edict->variables.idealYaw = bot->GetViewAngles ().yaw;
		}
};

	//
	// Group: Private members.
	//
	private:
		Task           *m_currentTask;

		Math::Vector3D  m_lookAt;

		Math::Angles3D  m_idealAngles;				// angle wanted
		Math::Angles3D  m_randomizedIdealAngles;	// angle wanted with noise
		Math::Angles3D  m_angularDeviation;			// angular deviation from current to ideal angles
		Math::Angles2D  m_aimSpeed;					// aim speed calculated
		Math::Angles3D  m_targetOriginAngularSpeed;	// target/enemy angular speed

		// fov line structure definition
		struct FieldOfViewLineInformation_t
		{
			Math::Angles2D                                      scanAngles;		// angles describing the trace line direction for this scan element
			float                                               distance;		// distance to first obstacle when tracing at 'scanAngles'
			Math::Vector3D                                      endPosition;	// location of first obstacle when tracing at 'scanAngles'
			Math::Plane                                         plane;			// plane of first obstacle surface when tracing at 'scanAngles' (to describe the plane)
			const HalfLifeEngine::SDK::Classes::Edict          *hitEntity;		// pointer to obstacle entity
			HalfLifeEngine::SDK::Constants::PointContentType_t  contents;		// end point contents

			inline FieldOfViewLineInformation_t (void) :
				scanAngles (Math::Angles2D::ZeroValue),
				distance (0.0f),
				endPosition (Math::Vector3D::ZeroValue),
				plane (),
				hitEntity (NULL),
				contents (HalfLifeEngine::SDK::Constants::PointContentType_None)
			{ /* VOID */ }
		};
		enum
		{
			MaximumFieldOfView       = HalfLifeEngine::SDK::Constants::DefaultPlayerFieldOfView,
			HalfFieldOfView          = MaximumFieldOfView / 2u,

			MaximumFieldOfViewStep   = 5u,
			MaximumFieldOfViewWidth  = MaximumFieldOfView / MaximumFieldOfViewStep + 1u,	// amount of vision cells in the bot's eye

			CenterDirectionIndex     = HalfFieldOfView / MaximumFieldOfViewStep,
			LeftDirectionStartIndex  = 0u,
			LeftDirectionEndIndex    = CenterDirectionIndex - 1u,
			LeftDirectionTotalIndex  = LeftDirectionEndIndex - LeftDirectionStartIndex,
			RightDirectionStartIndex = CenterDirectionIndex + 1u,
			RightDirectionEndIndex   = MaximumFieldOfViewWidth - 1u,
			RightDirectionTotalIndex = RightDirectionEndIndex - RightDirectionStartIndex,

			MaximumScanDistance      = 10000u	/// @warning Do not change this value to 'HalfLifeEngine::SDK::Constants::MapSize', or something else!
		};
		FieldOfViewLineInformation_t m_fieldOfView[MaximumFieldOfViewWidth];	// bot's field of view is 'MaximumFieldOfViewWidth' elements width
		float                        m_sampleTime;								// date at which the bot will next sample his field of view

	//
	// Group: Members.
	//
	public:
		

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Aim (YaPB *const bot) :
			SubSystem <YaPB> (bot),	// Set bot edict.

			m_currentTask (new Task ()),

			m_idealAngles (0.0f, 0.0f, 0.0f),
			m_randomizedIdealAngles (0.0f, 0.0f, 0.0f),
			m_angularDeviation (0.0f, 0.0f, 0.0f),
			m_aimSpeed (0.0f, 0.0f),
			m_targetOriginAngularSpeed (0.0f, 0.0f, 0.0f),

			m_sampleTime (0.0f)
		{ /* VOID */ }

		inline ~Aim (void) { delete m_currentTask; }

	//
	// Group: Private functions.
	//
	private:
		void UpdateFieldOfView (void);
		inline void CheckUpdateFieldOfView (void)
		{
			if (m_sampleTime > HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
				return;	// don't go further if not time to sample the field of view yet

			// we just can't match up with the human eye's refresh rate... bots will sample less often
			m_sampleTime = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + 0.2f;	// next sampling in 200 ms

			UpdateFieldOfView ();
		}

	//
	// Group: Functions.
	//
	public:
		inline       Task *const GetLookTask (void)                         { return m_currentTask; }
		inline const Task *const GetLookTask (void)                   const { return m_currentTask; }
		inline void                 SetLookTask (Task *const newTask)       { delete m_currentTask; m_currentTask = newTask; }

	//
	// Group: Engine callbacks.
	//
	public:
		void Think (void);
};

#endif	// ifndef BOT_AIM_INCLUDED