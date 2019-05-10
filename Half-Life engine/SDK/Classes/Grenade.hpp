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
// Grenade.hpp
//
// Class: CGrenade
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_GRENADE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_GRENADE_INCLUDED

// Contact Grenade / Timed grenade / Satchel Charge
class CGrenade : public BaseMonster
{
	public:
		enum SATCHELCODE { SATCHEL_DETONATE, SATCHEL_RELEASE };

	public:
		// Info for defusing.
/*!off=384(96)*/	bool                   m_isStartDefuse;				// is c4 defusing?
/*!off=385(96.25)*/	bool                   m_isC4;						// is c4?
/*!off=388(97)*/	EntityHandle           m_pBombDefuser;
/*!off=396(99)*/	float                  m_defuseCountDownTime;
/*!off=400(100)*/	float                  m_flC4BlowTime;				// get c4 time to explode

		// Info for the beeping.
/*!off=404(101)*/		float                  m_flNextFreqIntervalTime;
/*!off=408(102)*/		float                  m_flNextBeepTime;			// Beep interval time.
/*!off=412(103)*/		float                  m_flNextFreqTime;
/*!off=416(104)*/		const char            *m_sBeepName;
/*!off=420(105)*/		float                  m_beepAttenuation;

/*!off=424(106)*/		float                  m_flNextBlinkTime;			// Info for blinking.
/*!off=428(107)*/		float                  m_fNextDefuseTime;
/*!off=432(108)*/		bool                   m_bJustBlew;
/*!off=436(109)*/		Constants::TeamID_t    m_HEOwnerTeam;				// Note: for other type of grenade this always == 0(TeamID_Unassigned)!
/*!off=440(110)*/		Constants::C4WaveID_t  m_iCurWave;					// Which sound we're playing.
/*!off=444(111)*/		Edict                 *m_currentBombTarget;
/*!off=448(112)*/		unsigned int           m_smokeGrenadePuffsNumber;	// Increases each second, maximum amount is 21.
/*!off=452(113)*/		unsigned int           m_smokeGrenade_UNKNOWN0;
/*!off=456(114)*/		unsigned short         m_SGSmoke;					// Event index. Grenade type (flash=0, he=25, smoke=26, C4=0) (as GrenadeTypeID_t (but can as 'GrenadeType_t') - Need to truncate the result by GrenadeType_Hand_Total! (GetPrivateDataReference() & GrenadeType_Hand_Total)). Note: For C4 this always == 0! - C4 can be found by 'Offset_Grenade_IsC4'.
/*!off=458(114.5)*/		bool                   m_bLightSmoke;
/*!off=459(114.75)*/		bool                   m_smokeGrenadeIsDetonated;
/*!off=460(115)*/		Math::Vector3D         m_smokeGrenadeDetonationOrigin;
/*!off=472(118)*/		unsigned int           m_numberFloorHits;
/*!off=476(119)*/		BOOL                   m_fRegisteredSound;			// whether or not this grenade has issued its DANGER sound to the world sound list yet.
#if 0
public:
	void Spawn( void );

	static CGrenade *ShootTimed( Structures::EntityVariables_t *pevOwner, Math::Vector3D vecStart, Math::Vector3D vecVelocity, float time );
	static CGrenade *ShootContact( Structures::EntityVariables_t *pevOwner, Math::Vector3D vecStart, Math::Vector3D vecVelocity );
	static CGrenade *ShootSatchelCharge( Structures::EntityVariables_t *pevOwner, Math::Vector3D vecStart, Math::Vector3D vecVelocity );
	static void UseSatchelCharges( Structures::EntityVariables_t *pevOwner, SATCHELCODE code );

	void Explode( Math::Vector3D vecSrc, Math::Vector3D vecAim );
	void Explode( TraceResult *pTrace, int bitsDamageType );
	void EXPORT Smoke( void );

	void EXPORT BounceTouch( BaseEntity *pOther );
	void EXPORT SlideTouch( BaseEntity *pOther );
	void EXPORT ExplodeTouch( BaseEntity *pOther );
	void EXPORT DangerSoundThink( void );
	void EXPORT PreDetonate( void );
	void EXPORT Detonate( void );
	void EXPORT DetonateUse( BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value );
	void EXPORT TumbleThink( void );

	virtual void BounceSound( void );
	virtual int	BloodColor( void ) { return DONT_BLEED; }
	virtual void Killed( Structures::EntityVariables_t *pevAttacker, int iGib );
#endif	// if 0
};	// sizeof (CGrenade) == 480 (120)

inline       CGrenade *const BaseEntity::GetGrenadePointer (void)       { return static_cast <CGrenade *const> (this); }
inline const CGrenade *const BaseEntity::GetGrenadePointer (void) const { return static_cast <const CGrenade *const> (this); }

#endif // HALF_LIFE_ENGINE_SDK_CLASSES_GRENADE_INCLUDED