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
// EntityHandle.hpp
//
// Class: EntityHandle
//
// Description: Safe way to point to BaseEntities who may die between frames.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_ENTITY_HANDLE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_ENTITY_HANDLE_INCLUDED

class EntityHandle
{
	private:
/*! off=0(0) */		Edict *m_edict;
/*! off=4(1) */		int    m_serialNumber;

	public:
		inline operator const bool (void) const { return Get () != NULL; }

		inline operator BaseEntity *const (void) const { return GetEntity (); }

		inline BaseEntity *const operator = (BaseEntity *const entity)
		{
			SetEntity (entity);

			return entity;
		}

		inline BaseEntity *const operator -> (void) const { return GetEntity (); }

	// Note: Below functions are declared in 'Half-Life engine/Core.hpp'
	public:
		inline Edict      *const Get       (void) const;
		inline Edict      *const Set       (Edict *const edict);
		inline BaseEntity *const GetEntity (void) const;
		inline void              SetEntity (BaseEntity *const entity);
};	// sizeof (EntityHandle) == 8 (2)

#endif // HALF_LIFE_ENGINE_SDK_CLASSES_ENTITY_HANDLE_INCLUDED