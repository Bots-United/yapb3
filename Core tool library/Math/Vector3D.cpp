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
// Vector3D.hpp
//
// Class: Vector3D
//
// Description: Encapsulates 3D (math) vector used by the both Gold and Source engines.
//
// Version: $ID:$
//

#include <Core.hpp>

namespace CoreToolLibrary
{
	namespace Math
	{
		CLASS_GLOBAL const Vector3D Vector3D::ZeroValue (0.0f, 0.0f, 0.0f);	// Initialize zero vector value.

		CLASS_GLOBAL const Vector3D Vector3D::UnitX         ( 1.0f, 0.0f, 0.0f);
		CLASS_GLOBAL const Vector3D Vector3D::UnitY         ( 0.0f, 1.0f, 0.0f);
		CLASS_GLOBAL const Vector3D Vector3D::UnitZ         ( 0.0f, 0.0f, 1.0f);
		CLASS_GLOBAL const Vector3D Vector3D::NegativeUnitX (-UnitX);
		CLASS_GLOBAL const Vector3D Vector3D::NegativeUnitY (-UnitY);
		CLASS_GLOBAL const Vector3D Vector3D::NegativeUnitZ (-UnitZ);
		CLASS_GLOBAL const Vector3D Vector3D::UnitScale     ( 1.0f, 1.0f, 1.0f);

		// From Cry SDK....
		/*static */const Vector3D::AxisID_t Vector3D::inc_mod[AxisID_Total] = {AxisID_Y, AxisID_Z, AxisID_X};
		/*static */const Vector3D::AxisID_t Vector3D::dec_mod[AxisID_Total] = {AxisID_Z, AxisID_X, AxisID_Y};

		/*inline */void Vector3D::MakeRightVector (void) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->m_globalVariables->right = BuildRightVector ();
		}
		/*inline */void Vector3D::MakeUpwardVector (void) const
		{
			HalfLifeEngine::Globals::g_halfLifeEngine->m_globalVariables->upward = BuildUpwardVector ();
		}
		/*inline */void Vector3D::MakeVectors (void) const
		{
			BuildVectors (/*HalfLifeEngine::Globals::g_halfLifeEngine->m_globalVariables->forward, */HalfLifeEngine::Globals::g_halfLifeEngine->m_globalVariables->right, HalfLifeEngine::Globals::g_halfLifeEngine->m_globalVariables->upward);
		}
	}
}