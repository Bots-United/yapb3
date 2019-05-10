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
// Breakable.hpp
//
// Class: Breakable
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BREAKABLE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_BREAKABLE_INCLUDED

#define	NUM_SHARDS 6	// this many shards spawned when breakable objects break;

class Breakable : public BaseDelay
{
public:
#if 0
	// basic functions
	void Spawn( void );
	void Precache( void );
	void KeyValue( KeyValueData* pkvd);
	void EXPORT BreakTouch( BaseEntity *pOther );
	void Use( BaseEntity *pActivator, BaseEntity *pCaller, Constants::UseType_t useType, float value );
	void DamageSound( void );

	// breakables use an overridden takedamage
	virtual int TakeDamage( Structures::EntityVariables_t* pevInflictor, Structures::EntityVariables_t* pevAttacker, float flDamage, Constants::DamageType_t bitsDamageType );
	// To spark when hit
	void TraceAttack( Structures::EntityVariables_t *pevAttacker, float flDamage, Math::Vector3D vecDir, TraceResult *ptr, Constants::DamageType_t bitsDamageType );

	BOOL IsBreakable( void );
	BOOL SparkWhenHit( void );

	int	 DamageDecal (Constants::DamageType_t bitsDamageType );

	void EXPORT		Die( void );
	virtual int		ObjectCaps( void ) { return (BaseEntity::ObjectCaps() & ~Constants::CapBit_ACROSS_TRANSITION); }
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );

	inline BOOL		Explodable( void ) { return ExplosionMagnitude() > 0; }
	inline int		ExplosionMagnitude( void ) { return m_variables->impulse; }
	inline void		ExplosionSetMagnitude( int magnitude ) { m_variables->impulse = magnitude; }

	static void MaterialSoundPrecache( Constants::FunctionBreakableMaterialType_t precacheMaterial );
	static void MaterialSoundRandom( Edict *pEdict, Constants::FunctionBreakableMaterialType_t soundMaterial, float volume );
	static const char **MaterialSoundList( Constants::FunctionBreakableMaterialType_t precacheMaterial, int &soundCount );

	static const char *pSoundsWood[];
	static const char *pSoundsFlesh[];
	static const char *pSoundsGlass[];
	static const char *pSoundsMetal[];
	static const char *pSoundsConcrete[];
	static const char *pSpawnObjects[];

	static TYPEDESCRIPTION m_SaveData[];
#endif	// if 0
	Constants::FunctionBreakableMaterialType_t	m_Material;
	Constants::FunctionBreakableExplosionType_t	m_Explosion;
	int			m_idShard;
	float		m_angle;
	int			m_iszGibModel;
	int			m_iszSpawnObject;
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_BREAKABLE_INCLUDED