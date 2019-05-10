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
// Radian.hpp
//
// Class: Radian
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_RADIAN_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_RADIAN_INCLUDED

/** Wrapper class which indicates a given angle value is in Radians.
	@remarks
		Radian values are interchangeable with Degree values, and conversions
		will be done automatically between them.
*/
class Radian
{
	private:
		float m_value;

	public:
		explicit inline Radian (const float r = 0.0f) : m_value (r) { /* VOID */ }
		inline Radian (const Degree &d);

	public:
		inline const float ValueDegrees (void) const { return Math::RadianToDegree (m_value); }
		inline const float ValueRadians (void) const { return m_value; }

	public:
		inline Radian &operator = (const float &f) { m_value = f; return *this; }
		inline Radian &operator = (const Radian &r) { m_value = r.m_value; return *this; }
		inline Radian &operator = (const Degree &d);

        inline const Radian &operator +  (void) const { return *this; }
		inline       Radian  operator +  (const Radian &r) const { return Radian (m_value + r.m_value); }
		inline       Radian  operator +  (const Degree &d) const;
		inline       Radian &operator += (const Radian &r) { m_value += r.m_value; return *this; }
		inline       Radian &operator += (const Degree &d);
		inline       Radian  operator -  (void) const { return Radian (-m_value); }
		inline       Radian  operator -  (const Radian &r) const { return Radian (m_value - r.m_value); }
		inline       Radian  operator -  (const Degree &d) const;
		inline       Radian &operator -= (const Radian &r) { m_value -= r.m_value; return *this; }
		inline       Radian &operator -= (const Degree &d);
		inline       Radian  operator *  (const float f) const { return Radian (m_value * f); }
        inline       Radian  operator *  (const Radian &f) const { return Radian (m_value * f.m_value); }
		inline       Radian &operator *= (const float f) { m_value *= f; return *this; }
		inline       Radian  operator /  (const float f) const { return Radian (m_value / f); }
		inline       Radian &operator /= (const float f) { m_value /= f; return *this; }

		inline const bool operator <  (const Radian &r) const { return m_value <  r.m_value; }
		inline const bool operator <= (const Radian &r) const { return m_value <= r.m_value; }
		inline const bool operator == (const Radian &r) const { return m_value == r.m_value; }
		inline const bool operator != (const Radian &r) const { return m_value != r.m_value; }
		inline const bool operator >= (const Radian &r) const { return m_value >= r.m_value; }
		inline const bool operator >  (const Radian &r) const { return m_value >  r.m_value; }

		friend inline Radian operator * (const float a, const Radian &b)
		{
			return Radian (a * b.ValueRadians ());
		}
		friend inline Radian operator / (const float a, const Radian &b)
		{
			return Radian (a / b.ValueRadians ());
		}
/*
		inline _OgreExport friend std::ostream &operator << (std::ostream &o, const Radian &v)
		{
			o << "Radian(" << v.ValueRadians () << ")";
			return o;
		}*/
};

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_RADIAN_INCLUDED