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
// BaseToggle.hpp
//
// Class: BaseToggle
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BASE_TOGGLE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_BASE_TOGGLE_INCLUDED

class BaseToggle : public BaseAnimating
{
	public:
//		void KeyValue (Structures::KeyValueData_t *pkvd);

		Constants::ToggleState_t m_toggle_state;
		float m_flActivateFinished;//like attack_finished, but for doors
		float m_flMoveDistance;// how far a door should slide or rotate
		float m_flWait;
		float m_flLip;
		float m_flTWidth;// for plats
		float m_flTLength;// for plats

		Math::Vector3D m_vecPosition1;
		Math::Vector3D m_vecPosition2;
		Math::Vector3D m_vecAngle1;
		Math::Vector3D m_vecAngle2;

		int m_cTriggersLeft;	// trigger_counter only, # of activations remaining
		float m_flHeight;
		EntityHandle m_hActivator;
		void (BaseToggle::*m_pfnCallWhenMoveDone) (void);
		Math::Vector3D m_vecFinalDest;
		Math::Vector3D m_vecFinalAngle;

		int m_bitsDamageInflict;	// DMG_ damage type that the door or tigger does
#if 0
		virtual int  Save (CSave &save);
		virtual int  Restore (CRestore &restore);

		static TYPEDESCRIPTION m_SaveData[];

		virtual int  GetToggleState (void) { return m_toggle_state; }
		virtual float GetDelay (void) { return m_flWait; }

		// common member functions
		void LinearMove (Math::Vector3D vecDest, float flSpeed);
		void EXPORT LinearMoveDone (void);
		void AngularMove (Math::Vector3D vecDestAngle, float flSpeed);
		void EXPORT AngularMoveDone (void);
		BOOL IsLockedByMaster (void);

		static float  AxisValue (int flags, const Math::Vector3D &angles);
		static void   AxisDir (Structures::EntityVariables_t *pev);
		static float  AxisDelta (int flags, const Math::Vector3D &angle1, const Math::Vector3D &angle2);
#endif	// if 0
		TypeDefinitions::StringOffset_t m_sMaster;	// If this button has a master switch, this is the targetname.
													// A master switch must be of the multisource type. If all
													// of the switches in the multisource have been triggered, then
													// the button will be allowed to operate. Otherwise, it will be deactivated.
};

#define SetMoveDone(a) m_pfnCallWhenMoveDone = static_cast <void (BaseToggle::*) (void)> (a)

#endif // HALF_LIFE_ENGINE_SDK_CLASSES_BASE_TOGGLE_INCLUDED