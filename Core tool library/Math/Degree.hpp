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
// Degree.hpp
//
// Class: Degree
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_MATH_DEGREE_INCLUDED
#define CORE_TOOL_LIBRARY_MATH_DEGREE_INCLUDED

/** Wrapper class which indicates a given angle value is in Degrees.
	@remarks
		Degree values are interchangeable with Radian values, and conversions
		will be done automatically between them.
*/
class Degree
{
	private:
		float m_value;

	public:
		explicit inline Degree (const float d = 0.0f) : m_value(d) { /* VOID */ }
		inline Degree (const Radian &r) : m_value(r.ValueDegrees ()) { /* VOID */ }

	public:
		inline const float ValueDegrees (void) const { return m_value; }
		inline const float ValueRadians (void) const { return Math::DegreeToRadian (m_value); }

	public:
		inline Degree &operator = (const float &f) { m_value = f; return *this; }
		inline Degree &operator = (const Degree &d) { m_value = d.m_value; return *this; }
		inline Degree &operator = (const Radian &r) { m_value = r.ValueDegrees (); return *this; }

		inline const Degree &operator +  (void) const { return *this; }
		inline       Degree  operator +  (const Degree &d) const { return Degree (m_value + d.m_value); }
		inline       Degree  operator +  (const Radian &r) const { return Degree (m_value + r.ValueDegrees ()); }
		inline       Degree &operator += (const Degree &d) { m_value += d.m_value; return *this; }
		inline       Degree &operator += (const Radian &r) { m_value += r.ValueDegrees (); return *this; }
		inline       Degree  operator -  (void) const { return Degree (-m_value); }
		inline       Degree  operator -  (const Degree &d) const { return Degree (m_value - d.m_value); }
		inline       Degree  operator -  (const Radian &r) const { return Degree (m_value - r.ValueDegrees ()); }
		inline       Degree &operator -= (const Degree &d) { m_value -= d.m_value; return *this; }
		inline       Degree &operator -= (const Radian &r) { m_value -= r.ValueDegrees (); return *this; }
		inline       Degree  operator *  (const float f) const { return Degree (m_value * f); }
        inline       Degree  operator *  (const Degree &f) const { return Degree (m_value * f.m_value); }
		inline       Degree &operator *= (const float f) { m_value *= f; return *this; }
		inline       Degree  operator /  (const float f) const { return Degree (m_value / f); }
		inline       Degree &operator /= (const float f) { m_value /= f; return *this; }

		inline const bool operator <  (const Degree &d) const { return m_value <  d.m_value; }
		inline const bool operator <= (const Degree &d) const { return m_value <= d.m_value; }
		inline const bool operator == (const Degree &d) const { return m_value == d.m_value; }
		inline const bool operator != (const Degree &d) const { return m_value != d.m_value; }
		inline const bool operator >= (const Degree &d) const { return m_value >= d.m_value; }
		inline const bool operator >  (const Degree &d) const { return m_value >  d.m_value; }

		friend inline Degree operator * (const float a, const Degree &b) { return Degree (a * b.ValueDegrees ()); }
		friend inline Degree operator / (const float a, const Degree &b) { return Degree (a / b.ValueDegrees ()); }
/*
		inline _OgreExport friend std::ostream &operator << (std::ostream &o, const Degree &v)
		{
			o << "Degree(" << v.ValueDegrees () << ")";
			return o;
		}*/
};

// these functions could not be defined within the class definition of class
// Radian because they required class Degree to be defined
inline Radian::Radian (const Degree &d) : m_value (d.ValueRadians ()) { /* VOID */ }
inline Radian &Radian::operator = (const Degree &d)
{
	m_value = d.ValueRadians (); return *this;
}
inline Radian Radian::operator + (const Degree &d) const
{
	return Radian (m_value + d.ValueRadians ());
}
inline Radian &Radian::operator += (const Degree &d)
{
	m_value += d.ValueRadians ();
	return *this;
}
inline Radian Radian::operator - (const Degree &d) const
{
	return Radian (m_value - d.ValueRadians());
}
inline Radian &Radian::operator -= (const Degree &d)
{
	m_value -= d.ValueRadians ();
	return *this;
}

#endif	// ifndef CORE_TOOL_LIBRARY_MATH_DEGREE_INCLUDED