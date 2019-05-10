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
// Color.hpp
//
// Class: Color
//
// Description: Simple color class.
//
// Purpose: Basic handler for an RGB set of colors.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef COLOR_INCLUDED
#define COLOR_INCLUDED

template <typename elementType> class ColorWithAlpha;

template <typename elementType = unsigned char> class Color
{
	//
	// Group: Constants.
	//
	public:
		enum ID_t
		{
			ID_Red,
			ID_Green,
			ID_Blue,

			ID_Total
		};

	//
	// Group: Red, green, blue color components (0 - 255).
	//
	public:
		elementType red, green, blue;

	//
	// Group: Special points.
	//
	public:
		CLASS_GLOBAL static const Color ZeroValue;	// Zero color value.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Color (const elementType color = 0) : red (color), green (color), blue (color) { /* VOID */ }
		inline Color (const elementType *const color) : red (color[ID_Red]), green (color[ID_Green]), blue (color[ID_Blue]) { /* VOID */ }
		inline Color (const elementType inputRed, const elementType inputGreen, const elementType inputBlue) : red (inputRed), green (inputGreen), blue (inputBlue) { /* VOID */ }
		inline Color (const Color &right) : red (right.red), green (right.green), blue (right.blue) { /* VOID */ }
//		inline Color (const ColorWithAlpha <elementType> &right);	/// @note Declared below....

	//
	// Group: Functions.
	//
	public:
		inline const unsigned int GetRawColor (void) const
		{
			// Reliability check.
//			CompileTimeAssert (sizeof (*this) == sizeof (ColorWithAlpha <>));

			return blue | (green << 8u) | (red << 16u);
//			return red | (green << 8u) | (blue << 16u);
		}
		inline void               SetRawColor (const unsigned int color32)
		{
			// Reliability check.
//			CompileTimeAssert (sizeof (*this) == sizeof (ColorWithAlpha <>));

			red   = (color32 >> 16u) & 255u;
//			red   = color32 & 255u;
			green = (color32 >> 8u) & 255u;
//			green = (color32 >> 8u) & 255u;
			blue  = color32 & 255u;
//			blue  = (color32 >> 16u) & 255u;
		}

		//
		// Function: GetDistance
		//
		// Description: Gets the color's distance to another color.
		//
		// Parameters:
		//	other - Color which, together with this one, will be used to calculate the distance.
		//
		// Returns: Distance to another color.
		//
		inline const float GetDistance (const Color &other) const
		{
			return static_cast <float> ((red - other.red) * (red - other.red) + (red - other.green) * (green - other.green) + (red - other.blue) * (blue - other.blue));
		}

		//
		// Function: GetNormalized
		//
		// Description: Gets the normalized color.
		//
		// Parameters:
		//	norm - The normalize value to be stored.
		//
		// Note: The alpha value is not modified.
		//
		// Returns: Normalized color.
		//
		inline const Color GetNormalized (float &norm) const
		{
			Color out;

			norm = GetMaximumValueBetween (GetMaximumValueBetween (red, green), blue);

			if (norm > 1.0f)
			{
				const float invNorm (1.0f / f);

				out.red   = red * invNorm;
				out.green = green * invNorm;
				out.blue  = blue * invNorm;
			}
			else
				out = *this;

			return out;
		}
		//
		// Function: GetNormalized
		//
		// Description: Gets the normalized color.
		//
		// Note: The alpha value is not modified.
		//
		// Returns: Normalized color.
		//
		inline const Color GetNormalized (void) const
		{
			float norm;

			return GetNormalized (norm);
		}
		//
		// Function: Normalize
		//
		// Description: Normalizes a color.
		//
		// Note: The alpha value is not modified.
		//
		// Returns: Normalized color.
		//
		inline Color &Normalize (void)
		{
			return *this = GetNormalized ();
		}

		inline const Color GetInterpolated (const Color &destination, const float fraction) const { return Math::GetInterpolated (*this, destination, fraction); }
		inline Color &Interpolate (const Color &source, const Color &destination, const float fraction) { return Math::Interpolate (*this, source, destination, fraction); }

		inline const elementType GetAvg (void) const { return (red + green + blue) / ID_Total; }

	//
	// Group: Operators.
	//
	public:
		//
		// Function: operator =
		//
		// Description: Assigns the value to existing color.
		//
		// Parameters:
		//	value - Value that should be assigned.
		//
		// Returns: Reassigned color.
		//
		inline Color               &operator =  (const elementType value)
		{
			red = green = blue = value;

			return *this;
		}
		inline Color               &operator =  (const elementType *const color)
		{
			// Reliability check.
			InternalAssert (color != NULL);

			red = color[ID_Red];
			green = color[ID_Green];
			blue = color[ID_Blue];

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current color with specified one.
		//
		// Parameters:
		//	right - Other color that should be assigned.
		//
		// Returns: Reassigned color.
		//
		inline Color               &operator =  (const Color &right)
		{
			red = right.red;
			green = right.green;
			blue = right.blue;

			return *this;
		}
//		inline Color               &operator =  (const ColorWithAlpha <elementType> &right);	/// @note Declared below....

		// casting operators
		inline                        operator       elementType *const (void)       { return &red; }	// color will now automatically convert to 'elementType *' when needed
		inline                        operator const elementType *const (void) const { return &red; }	// color will now automatically convert to 'const elementType *const' when needed
//		inline                        operator       ColorWithAlpha <elementType>       (void) const;	// color will now automatically convert to 'ColorWithAlpha' when needed
//		inline                        operator const ColorWithAlpha <elementType>       (void) const;	// color will now automatically convert to 'const ColorWithAlpha' when needed

		// equality
		inline const bool           operator == (const Color &right) const { return red == right.red && green == right.green && blue == right.blue; }
		inline const bool           operator != (const Color &right) const { return red != right.red || green != right.green || blue != right.blue; }

		//
		// Function: operator <
		//
		// Description: Checks whether color's all components are smaller that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are smaller that the value it is compared against, false otherwise.
		//
		inline const bool           operator <                  (const elementType value)                 const { return red < value && green < value && blue < value; }

		//
		// Function: operator <
		//
		// Description: Checks whether color's components are all smaller that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all smaller that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator <                  (const Color &right)                        const { return red < right.red && green < right.green && blue < right.blue; }

		//
		// Function: operator <=
		//
		// Description: Checks whether color's all components are smaller or equal that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are smaller or equal that the value it is compared against, false otherwise.
		//
		inline const bool           operator <=                 (const elementType value)                 const { return red <= value && green <= value && blue <= value; }

		//
		// Function: operator <=
		//
		// Description: Checks whether color's components are all smaller or equal that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all smaller or equal that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator <=                 (const Color &right)                        const { return red <= right.red && green <= right.green && blue <= right.blue; }

		//
		// Function: operator >
		//
		// Description: Checks whether color's all components are greater that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are greater that the value it is compared against, false otherwise.
		//
		inline const bool           operator >                  (const elementType value)                 const { return red > value && green > value && blue > value; }

		//
		// Function: operator >
		//
		// Description: Checks whether color's components are all greater that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all greater that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator >                  (const Color &right)                        const { return red < right.red && green < right.green && blue < right.blue; }

		//
		// Function: operator >=
		//
		// Description: Checks whether color's all components are greater or equal that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are greater or equal that the value it is compared against, false otherwise.
		//
		inline const bool           operator >=                 (const elementType value)                 const { return red >= value && green >= value && blue >= value; }

		//
		// Function: operator >=
		//
		// Description: Checks whether color's components are all greater or equal that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all greater or equal that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator >=                 (const Color &right)                        const { return red >= right.red && green >= right.green && blue >= right.blue; }

		// array access
		inline       elementType &operator []                 (const ID_t colorID)                              { InternalAssert (colorID < ID_Total); return (&red)[colorID]; }
		inline const elementType &operator []                 (const ID_t colorID)                        const { InternalAssert (colorID < ID_Total); return (&red)[colorID]; }

		// arithmetic operations
		inline const Color         operator +                  (const elementType scaler)                        const { return Color (red + scaler, green + scaler, blue + scaler); }
		inline const Color         operator +                  (const Color &right)                     const { return Color (red + right.red, green + right.green, blue + right.blue); }
		inline       Color        &operator +=                 (const elementType scaler)
		{
			red += scaler;
			green += scaler;
			blue += scaler;

			return *this;
		}
		inline       Color        &operator +=                 (const Color &right)
		{
			red += right.red;
			green += right.green;
			blue += right.blue;

			return *this;
		}

		inline const Color         operator -                  (const elementType scaler)                        const { return Color (red - scaler, green - scaler, blue - scaler); }
		inline       Color         operator -                  (const Color &right)                     const { return Color (red - right.red, green - right.green, blue - right.blue); }
		inline       Color        &operator -=                 (const elementType scaler)
		{
			red -= scaler;
			green -= scaler;
			blue -= scaler;

			return *this;
		}
		inline       Color        &operator -=                 (const Color &right)
		{
			red -= right.red;
			green -= right.green;
			blue -= right.blue;

			return *this;
		}

		inline const Color         operator *                  (const elementType scaler)                        const { return Color (red * scaler, green * scaler, blue * scaler); }
		inline const Color         operator *                  (const Color &right)                     const { return Color (red * right.red, green * right.green, blue * right.blue); }
		friend inline const Color  operator *                  (const elementType scaler, const Color &right)       { return right * scaler; }
		inline       Color        &operator *=                 (const elementType scaler)
		{
			red *= scaler;
			green *= scaler;
			blue *= scaler;

			return *this;
		}
		inline       Color        &operator *=                 (const Color &right)
		{
			red *= right.red;
			green *= right.green;
			blue *= right.blue;

			return *this;
		}

		inline const Color         operator /                  (const elementType scaler)                const { InternalAssert (scaler != 0); return Color (red / scaler, green / scaler, blue / scaler); }
		inline const Color         operator /                  (const Color &right)                     const
		{
			// Reliability check.
			InternalAssert (right.red != 0 && right.green != 0 && right.blue != 0);

			return Color (red / right.red, green / right.green, blue / right.blue);
		}
		inline       Color        &operator /=                 (const elementType scaler)
		{
			// Reliability check.
			InternalAssert (scaler != 0);

			red /= scaler;
			green /= scaler;
			blue /= scaler;

			return *this;
		}
		inline       Color        &operator /=                 (const Color &right)
		{
			// Reliability check.
			InternalAssert (right.red != 0 && right.green != 0 && right.blue != 0);

			red /= right.red;
			green /= right.green;
			blue /= right.blue;

			return *this;
		}
		inline       Color        &operator >>=                (const elementType scaler)
		{
			red >>= scaler;
			green >>= scaler;
			blue >>= scaler;

			return *this;
		}
};

template <typename elementType = unsigned char> class ColorWithAlpha
{
	//
	// Group: Constants.
	//
	public:
		enum ID_t
		{
			ID_Red,
			ID_Green,
			ID_Blue,
			ID_Alpha,

			ID_Total
		};

	//
	// Group: Red, green, blue and alpha (controls transparency (0 - transparent, 255 - opaque)) color components (0 - 255).
	//
	public:
		elementType red, green, blue, alpha;

	//
	// Group: Special points.
	//
	public:
		CLASS_GLOBAL static const ColorWithAlpha ZeroValue;	// Zero color value.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ColorWithAlpha (const elementType color = 0) : red (color), green (color), blue (color), alpha (color) { /* VOID */ }
		inline ColorWithAlpha (const elementType *const color) : red (color[ID_Red]), green (color[ID_Green]), blue (color[ID_Blue]), alpha (color[ID_Alpha]) { /* VOID */ }
		inline ColorWithAlpha (const elementType inputRed, const elementType inputGreen, const elementType inputBlue, const elementType inputAlpha = 255u) : red (inputRed), green (inputGreen), blue (inputBlue), alpha (inputAlpha) { /* VOID */ }
		inline ColorWithAlpha (const Color <elementType> &right) : red (right.red), green (right.green), blue (right.blue), alpha (255u) { /* VOID */ }
		inline ColorWithAlpha (const Color <elementType> &right, const elementType inputAlpha) : red (right.red), green (right.green), blue (right.blue), alpha (inputAlpha) { /* VOID */ }
		inline ColorWithAlpha (const ColorWithAlpha &right) : red (right.red), green (right.green), blue (right.blue), alpha (right.alpha) { /* VOID */ }
//		explicit inline ColorWithAlpha (const unsigned int color32) { SetRawColor (color32); }

	//
	// Group: Functions.
	//
	public:
		static inline const ColorWithAlpha GetFromRaw (const unsigned int rawColor32)
		{
			ColorWithAlpha color;

			color.SetRawColor (rawColor32);

			return color;
		}

		inline       unsigned int &GetRawColor (void)
		{
			// Reliability check.
			CompileTimeAssert (sizeof (*this) == sizeof (ColorWithAlpha <>));

			return reinterpret_cast <unsigned int *const> (this)[0u];
		}
		inline const unsigned int &GetRawColor (void)                       const
		{
			// Reliability check.
			CompileTimeAssert (sizeof (*this) == sizeof (ColorWithAlpha <>));

			return reinterpret_cast <const unsigned int *const> (this)[0u];
		}
		inline void                SetRawColor (const unsigned int color32)
		{
			// Reliability check.
			CompileTimeAssert (sizeof (*this) == sizeof (ColorWithAlpha <>));

			reinterpret_cast <unsigned int *const> (this)[0u] = color32;
		}

		//
		// Function: GetDistance
		//
		// Description: Gets the color's distance to another color.
		//
		// Parameters:
		//	other - Color which, together with this one, will be used to calculate the distance.
		//
		// Returns: Distance to another color.
		//
		inline const float GetDistance (const ColorWithAlpha &other) const
		{
			return static_cast <float> ((red - other.red) * (red - other.red) + (red - other.green) * (green - other.green) + (red - other.blue) * (blue - other.blue) + (red - other.alpha) * (alpha - other.alpha));
		}

		//
		// Function: GetNormalized
		//
		// Description: Gets the normalized color.
		//
		// Parameters:
		//	norm - The normalize value to be stored.
		//
		// Note: The alpha value is not modified.
		//
		// Returns: Normalized color.
		//
		inline const ColorWithAlpha GetNormalized (float &norm) const
		{
			ColorWithAlpha out;

			norm = GetMaximumValueBetween (GetMaximumValueBetween (red, green), blue);

			if (norm > 1.0f)
			{
				const float invNorm (1.0f / f);

				out.red   = red * invNorm;
				out.green = green * invNorm;
				out.blue  = blue * invNorm;
				out.alpha = alpha;
			}
			else
				out = *this;

			return out;
		}
		//
		// Function: GetNormalized
		//
		// Description: Gets the normalized color.
		//
		// Note: The alpha value is not modified.
		//
		// Returns: Normalized color.
		//
		inline const ColorWithAlpha GetNormalized (void) const
		{
			float norm;

			return GetNormalized (norm);
		}
		//
		// Function: Normalize
		//
		// Description: Normalizes a color.
		//
		// Note: The alpha value is not modified.
		//
		// Returns: Normalized color.
		//
		inline ColorWithAlpha &Normalize (void)
		{
			return *this = GetNormalized ();
		}

		inline ColorWithAlpha &SetBrightness (const elementType brightness)
		{
			// Reliability check.
			CompileTimeAssert (sizeof (*this) == sizeof (ColorWithAlpha <>));

			const float value (brightness / 255.0f);

			red *= value;
			green *= value;
			blue *= value;

			return *this;
		}
		inline ColorWithAlpha GetWithBrightness (const elementType brightness) const
		{
			// Reliability check.
			CompileTimeAssert (sizeof (*this) == sizeof (ColorWithAlpha <>));

			const float value (brightness / 255.0f);

			return ColorWithAlpha (static_cast <elementType> (red * value), static_cast <elementType> (green * value), static_cast <elementType> (blue * value), alpha);
		}

	//
	// Group: Operators.
	//
	public:
		//
		// Function: operator =
		//
		// Description: Assigns the value to existing color.
		//
		// Parameters:
		//	value - Value that should be assigned.
		//
		// Returns: Reassigned color.
		//
		inline ColorWithAlpha               &operator =  (const elementType value)
		{
			red = green = blue = alpha = value;

			return *this;
		}
		inline ColorWithAlpha               &operator =  (const elementType *const color)
		{
			// Reliability check.
			InternalAssert (color != NULL);

			red = color[ID_Red];
			green = color[ID_Green];
			blue = color[ID_Blue];
			alpha = color[ID_Alpha];

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current color with specified one.
		//
		// Parameters:
		//	right - Other color that should be assigned.
		//
		// Returns: Reassigned color.
		//
		inline ColorWithAlpha               &operator =  (const Color <elementType> &right)
		{
			red = right.red;
			green = right.green;
			blue = right.blue;
			alpha = 255u;

			return *this;
		}
		inline ColorWithAlpha               &operator =  (const ColorWithAlpha &right)
		{
			red = right.red;
			green = right.green;
			blue = right.blue;
			alpha = right.alpha;

			return *this;
		}
//		inline ColorWithAlpha               &operator =  (const unsigned int color32) { SetRawColor (color32); }

		// casting operators
		inline                        operator       elementType *const (void)       { return &red; }	// color will now automatically convert to 'elementType *' when needed
		inline                        operator const elementType *const (void) const { return &red; }	// color will now automatically convert to 'const elementType *const' when needed
		inline                        operator       unsigned int &     (void)       { return GetRawColor (); }	// color will now automatically convert to 'unsigned int &' when needed
		inline                        operator const unsigned int       (void) const { return GetRawColor (); }	// color will now automatically convert to 'const unsigned int' when needed
		inline                        operator       Color <elementType>                (void) const { return Color <elementType> (red, green, blue); }	// color will now automatically convert to 'ColorWithAlpha' when needed
//		inline                        operator const Color <elementType>                (void) const { return Color <elementType> (red, green, blue); }	// color will now automatically convert to 'const ColorWithAlpha' when needed

		// equality
		inline const bool           operator == (const ColorWithAlpha &right) const { return red == right.red && green == right.green && blue == right.blue && alpha == right.alpha/*GetRawColor () == right.GetRawColor ()*/; }
		inline const bool           operator != (const ColorWithAlpha &right) const { return red != right.red || green != right.green || blue != right.blue || alpha != right.alpha; }

		//
		// Function: operator <
		//
		// Description: Checks whether color's all components are smaller that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are smaller that the value it is compared against, false otherwise.
		//
		inline const bool           operator <                  (const elementType value)                 const { return red < value && green < value && blue < value && alpha < value; }

		//
		// Function: operator <
		//
		// Description: Checks whether color's components are all smaller that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all smaller that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator <                  (const ColorWithAlpha &right)                        const { return red < right.red && green < right.green && blue < right.blue && alpha < right.alpha; }

		//
		// Function: operator <=
		//
		// Description: Checks whether color's all components are smaller or equal that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are smaller or equal that the value it is compared against, false otherwise.
		//
		inline const bool           operator <=                 (const elementType value)                 const { return red <= value && green <= value && blue <= value && alpha <= value; }

		//
		// Function: operator <=
		//
		// Description: Checks whether color's components are all smaller or equal that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all smaller or equal that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator <=                 (const ColorWithAlpha &right)                        const { return red <= right.red && green <= right.green && blue <= right.blue && alpha <= right.alpha; }

		//
		// Function: operator >
		//
		// Description: Checks whether color's all components are greater that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are greater that the value it is compared against, false otherwise.
		//
		inline const bool           operator >                  (const elementType value)                 const { return red > value && green > value && blue > value && alpha > value; }

		//
		// Function: operator >
		//
		// Description: Checks whether color's components are all greater that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all greater that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator >                  (const ColorWithAlpha &right)                        const { return red > right.red && green > right.green && blue > right.blue && alpha > right.alpha; }

		//
		// Function: operator >=
		//
		// Description: Checks whether color's all components are greater or equal that the value it is compared against.
		//
		// Parameters:
		//	value - Value to compare with.
		//
		// Returns: Returns true if the color's all components are greater or equal that the value it is compared against, false otherwise.
		//
		inline const bool           operator >=                 (const elementType value)                 const { return red >= value && green >= value && blue >= value && alpha >= value; }

		//
		// Function: operator >=
		//
		// Description: Checks whether color's components are all greater or equal that the ones of the color it is compared against.
		//
		// Parameters:
		//	right - Other color to compare with.
		//
		// Returns: Returns true if the color's components are all greater or equal that the ones of the color it is compared against, false otherwise.
		//
		inline const bool           operator >=                 (const ColorWithAlpha &right)                        const { return red >= right.red && green >= right.green && blue >= right.blue && alpha >= right.alpha; }

		// array access
		inline       elementType &operator []                 (const ID_t colorID)                              { InternalAssert (colorID < ID_Total); return (&red)[colorID]; }
		inline const elementType &operator []                 (const ID_t colorID)                        const { InternalAssert (colorID < ID_Total); return (&red)[colorID]; }

		inline const ColorWithAlpha          operator /                  (const elementType scaler)                const { InternalAssert (scaler != 0); return ColorWithAlpha (red / scaler, green / scaler, blue / scaler, alpha / scaler); }
};
/*
template <typename elementType> inline Color <elementType>::Color (const ColorWithAlpha <elementType> &right) : red (right.red), green (right.green), blue (right.blue) { /* VOID *//* }

template <typename elementType> inline Color <elementType> &Color <elementType>::operator = (const ColorWithAlpha <elementType> &right)
{
	red = right.red;
	green = right.green;
	blue = right.blue;

	return *this;
}*/
//template <typename elementType> inline Color <elementType>::operator       ColorWithAlpha <elementType> (void) const { return ColorWithAlpha <elementType> (*this); }	// color will now automatically convert to 'ColorWithAlpha' when needed
//template <typename elementType> inline Color <elementType>::operator const ColorWithAlpha <elementType> (void) const { return ColorWithAlpha <elementType> (*this); }	// color will now automatically convert to 'const ColorWithAlpha' when needed

inline const unsigned int LerpColors (const unsigned int ca, const unsigned int cb, const unsigned int u)
{
	const unsigned int ra = ca & 0xff;
	const unsigned int ga = (ca >> 8) & 0xff;
	const unsigned int ba = (ca >> 16) & 0xff;
	const unsigned int aa = (ca >> 24) & 0xff;
	const unsigned int rb = cb & 0xff;
	const unsigned int gb = (cb >> 8) & 0xff;
	const unsigned int bb = (cb >> 16) & 0xff;
	const unsigned int ab = (cb >> 24) & 0xff;
	
	unsigned char r = static_cast <unsigned char> ((ra*(255-u) + rb*u)/255);
	unsigned char g = static_cast <unsigned char> ((ga*(255-u) + gb*u)/255);
	unsigned char b = static_cast <unsigned char> ((ba*(255-u) + bb*u)/255);
	unsigned char a = static_cast <unsigned char> ((aa*(255-u) + ab*u)/255);
	return ColorWithAlpha <> (r,g,b,a);
}

inline const Color <> GetColorFromHSV (float hue, const unsigned char saturation, const unsigned char value)
{
	// Based on C Code in "Computer Graphics -- Principles and Practice",
	// Foley et al, 1996, p. 593.
	//
	// hue = 0.0 to 360.0 (corresponding to 0...360 degrees around hexcone)
	// saturation = 0 (shade of gray) to 255 (pure color)
	// value = 0 (black) to 255 (white)

	// Used for color cycling the HSV model
	// Code adapted from http://www.efg2.com/Lab/Graphics/Colors/HSV.htm, "HSV Lab Report"
	// saturation: 0 is dull gray, 255 is full color, value: 0 is dark, 255 is full luminance

	// Check input
	InternalAssert (hue >= 0.0f && hue < 360.0f);
	InternalAssert (saturation >= 0u && hue <= 255u);
	InternalAssert (value >= 0u && hue <= 255u);

	if (saturation == 0u)
		return Color <> (value);	// achromatic: shades of gray

	// chromatic color

	// Convert
	hue /= 60.0f;	// hue is now IN [0, 6)

	const unsigned char i (static_cast <unsigned char> (Math::floorf (hue)));	// largest integer <= hue
	const float f (hue - i);	// fractional part of hue
	const unsigned char p ((value * (255u - saturation)) / 255u);
	const unsigned char q (static_cast <unsigned char> ((value * (255u - saturation * f)) / 255u));
	const unsigned char t (static_cast <unsigned char> ((value * (255u - saturation * (1.0f - f))) / 255u));

	switch (i)
	{
		case 0u:
			return Color <> (value, t, p);

		case 1u:
			return Color <> (q, value, p);

		case 2u:
			return Color <> (p, value, t);

		case 3u:
			return Color <> (p, q, value);

		case 4u:
			return Color <> (t, p, value);

		// 5
		default:
			return Color <> (value, p, q);
	}
}

// Color enumeration. Contains names for the 256 colors in the color palette.
enum ColorIndex_t
{
	Black,
	BlackGray,
	DimGray,
	DarkGray,
	Gray,
	SlateGray,
	Thistle,
	Silver,

	LightGray,
	Gainsboro,
	Lavender,
	AntiqueWhite,
	MistyRose,
	GhostWhite,
	LavenderBlush,
	Linen,

	DarkSalmon,
	Peru,
	MetallicBronze,
	Punga,
	WestCoast,
	Kumera,
	Shadow,
	DarkLeather,

	YellowMetal,
	TobaccoBrown,
	Hemlock,
	Millbrook,
	Birch,
	Zeus,
	Oil,
	NightRider,

	MidGray,
	Dolphin,
	SmokeGray,
	Topaz,
	Waterloo,
	Abbey,
	Mako,
	GunPowder,

	MineShaft,
	Masala,
	CodGray,
	CocoaBrown,
	Thunder,
	Tundora,
	Emperer,
	Jon,

	Domino,
	Makara,
	Coffee,
	Cement,
	Tussock,
	Whiskey,
	DiSienna,
	RawSienna,

	FuelYellow,
	Dixie,
	Geebung,
	Pizza,
	RenoSand,
	ButteredRum,
	MaiTai,
	Woodrush,

	TallPoppy,
	BurntUmber,
	MuleFawn,
	RedRobin,
	Mocha,
	FaluRed,
	Redwood,
	DarkBurgundy,

	IndianTan,
	Bean,
	RusticRed,
	Mahogany,
	Rebel,
	Kilamanjaro,
	Chocolate,
	Nero,

	SpicyMix,
	Beaver,
	DarkChestnut,
	Toast,
	MediumWood,
	CoralTree,
	Quincy,
	RomanCoffee,

	WoodBurn,
	Cola,
	DarkBirch,
	BlackMagic,
	WoodBark,
	PineTree,
	SealBrown,
	MightAsWellBeBlack,

	Russet,
	BakersChocolate,
	RawUmber,
	YukonGold,
	DarkRusset,
	DarkBrown1,	// Ran out of brown color names :P
	DarkBrown2,
	DarkBrown3,

	AppleBlossum,
	ChestNut,
	Contessa,
	BrandyRose,
	LinkWater,
	Spindle,
	LightSteelBlue,
	RegentStBlue,

	Seagull,
	PictonBlue,
	SummerSky,
	Perlorous,
	PacificBlue,
	CuriousBlue,
	BostonBlue,
	Scooter,

	Sapphire,
	SherpaBlue,
	PrussianBlue,
	Cyprus,
	BlueWhale,
	Tangaroa,
	MidnightExpress,
	BlackPearl,

	RoseTaupe,
	SolidPink,
	Tosca,
	TawnyPort,
	Espresso,
	AuChico,
	Lotus,
	VanCleef,

	BulgarianRose,
	MoroccoBrown,
	Volcano,
	Moccaccino,
	LoneStar,
	RustRed,
	Heath,
	Bordeaux,

	Sage,
	Avocado,
	GreenSmoke,
	Asparagus,
	ChelseaCucumber,
	Highball,
	Wasabi,
	Sushi,

	TrendyGreen,
	Pesto,
	Olivetone,
	Rainforest,
	FijiGreen,
	ArmyGreen,
	VerdunGreen,
	Bronzetone,

	Cognac,
	MilanoRed,
	TiaMaria,
	RockSpray,
	Brown,
	MandarianOrange,
	DarkFaluRed,
	SaddleBrown,

	Rust,
	Sangria,
	DarkRed,
	GuardsmanOrange,
	FireBrick,
	VenetianRed,
	Maroon,
	Scarlett,

	MoodyBlue,
	BlueBell,
	HavelockBlue,
	FireSpeechBlue,
	CeruleanBlue,
	Danube,
	Shakespeare,
	Glacier,

	CornFlowerBlue,
	PortGore,
	DeepKoamaru,
	ParisM,
	Christalle,
	RegalBlue,
	CatalinaBlue,
	ResolutionBlue,

	Locust,
	Limerick,
	Lima,
	Christi,
	Citrus,
	Olive,
	PestoMesto,
	Himalaya,

	Caper,
	Olivine,
	Brass,
	Bahia,
	Lucky,
	LaRioja,
	Bilbao,
	Zuchini,

	Green,
	Lime,	// ScreaminGreen
	FireSpeechGreen,
	Harlequin,
	MintGreen,
	Malachite,
	PastelGreen,
	White,

	Cream,
	LemonChiffon,
	Canary,
	LaserLemon,
	ParisDaisy,
	Gorse,
	Lemon,
	Gold,

	HarvestGold,
	RobRoy,
	TulipTree,
	FireBush,
	ButterCup,
	Gamboge,
	MangoTango,
	Tawny,

	Sangeria,
	SaddledBrown,
	Harley,
	PersianRed,
	Grenadier,
	RedOxide,
	CaputMortuum,
	VanClef,

	Red,
	NeonBlue,
	PatriotRed,
	PatriotBlue,
	Karaka,
	Maire,
	BokaraGray,
	TonysPink,

	Sunglo,
	Piper,
	PeruTan,
	Solitaire,
	SourDough,
	Mongoose,
	DonkeyBrown,
	TransparentPink,

	ColorIndex_Total
};

typedef Color <> ColorPalette_t[ColorIndex_Total];

// "palette to rgb" list
const ColorPalette_t colorPalette =
{
	Color <> (  0u,   0u,   0u),
	Color <> ( 15u,  15u,  15u),
	Color <> ( 31u,  31u,  31u),
	Color <> ( 47u,  47u,  47u),
	Color <> ( 63u,  63u,  63u),
	Color <> ( 75u,  75u,  75u),
	Color <> ( 91u,  91u,  91u),
	Color <> (107u, 107u, 107u),
	Color <> (123u, 123u, 123u),
	Color <> (139u, 139u, 139u),
	Color <> (155u, 155u, 155u),
	Color <> (171u, 171u, 171u),
	Color <> (187u, 187u, 187u),
	Color <> (203u, 203u, 203u),
	Color <> (219u, 219u, 219u),
	Color <> (235u, 235u, 235u),
	Color <> ( 99u,  75u,  35u),
	Color <> ( 91u,  67u,  31u),
	Color <> ( 83u,  63u,  31u),
	Color <> ( 79u,  59u,  27u),
	Color <> ( 71u,  55u,  27u),
	Color <> ( 63u,  47u,  23u),
	Color <> ( 59u,  43u,  23u),
	Color <> ( 51u,  39u,  19u),
	Color <> ( 47u,  35u,  19u),
	Color <> ( 43u,  31u,  19u),
	Color <> ( 39u,  27u,  15u),
	Color <> ( 35u,  23u,  15u),
	Color <> ( 27u,  19u,  11u),
	Color <> ( 23u,  15u,  11u),
	Color <> ( 19u,  15u,   7u),
	Color <> ( 15u,  11u,   7u),
	Color <> ( 95u,  95u, 111u),
	Color <> ( 91u,  91u, 103u),
	Color <> ( 91u,  83u,  95u),
	Color <> ( 87u,  79u,  91u),
	Color <> ( 83u,  75u,  83u),
	Color <> ( 79u,  71u,  75u),
	Color <> ( 71u,  63u,  67u),
	Color <> ( 63u,  59u,  59u),
	Color <> ( 59u,  55u,  55u),
	Color <> ( 51u,  47u,  47u),
	Color <> ( 47u,  43u,  43u),
	Color <> ( 39u,  39u,  39u),
	Color <> ( 35u,  35u,  35u),
	Color <> ( 27u,  27u,  27u),
	Color <> ( 23u,  23u,  23u),
	Color <> ( 19u,  19u,  19u),
	Color <> (143u, 119u,  83u),
	Color <> (123u,  99u,  67u),
	Color <> (115u,  91u,  59u),
	Color <> (103u,  79u,  47u),
	Color <> (207u, 151u,  75u),
	Color <> (167u, 123u,  59u),
	Color <> (139u, 103u,  47u),
	Color <> (111u,  83u,  39u),
	Color <> (235u, 159u,  39u),
	Color <> (203u, 139u,  35u),
	Color <> (175u, 119u,  31u),
	Color <> (147u,  99u,  27u),
	Color <> (119u,  79u,  23u),
	Color <> ( 91u,  59u,  15u),
	Color <> ( 63u,  39u,  11u),
	Color <> ( 35u,  23u,   7u),
	Color <> (167u,  59u,  43u),
	Color <> (159u,  47u,  35u),
	Color <> (151u,  43u,  27u),
	Color <> (139u,  39u,  19u),
	Color <> (127u,  31u,  15u),
	Color <> (115u,  23u,  11u),
	Color <> (103u,  23u,   7u),
	Color <> ( 87u,  19u,   0u),
	Color <> ( 75u,  15u,   0u),
	Color <> ( 67u,  15u,   0u),
	Color <> ( 59u,  15u,   0u),
	Color <> ( 51u,  11u,   0u),
	Color <> ( 43u,  11u,   0u),
	Color <> ( 35u,  11u,   0u),
	Color <> ( 27u,   7u,   0u),
	Color <> ( 19u,   7u,   0u),
	Color <> (123u,  95u,  75u),
	Color <> (115u,  87u,  67u),
	Color <> (107u,  83u,  63u),
	Color <> (103u,  79u,  59u),
	Color <> ( 95u,  71u,  55u),
	Color <> ( 87u,  67u,  51u),
	Color <> ( 83u,  63u,  47u),
	Color <> ( 75u,  55u,  43u),
	Color <> ( 67u,  51u,  39u),
	Color <> ( 63u,  47u,  35u),
	Color <> ( 55u,  39u,  27u),
	Color <> ( 47u,  35u,  23u),
	Color <> ( 39u,  27u,  19u),
	Color <> ( 31u,  23u,  15u),
	Color <> ( 23u,  15u,  11u),
	Color <> ( 15u,  11u,   7u),
	Color <> (111u,  59u,  23u),
	Color <> ( 95u,  55u,  23u),
	Color <> ( 83u,  47u,  23u),
	Color <> ( 67u,  43u,  23u),
	Color <> ( 55u,  35u,  19u),
	Color <> ( 39u,  27u,  15u),
	Color <> ( 27u,  19u,  11u),
	Color <> ( 15u,  11u,   7u),
	Color <> (179u,  91u,  79u),
	Color <> (191u, 123u, 111u),
	Color <> (203u, 155u, 147u),
	Color <> (215u, 187u, 183u),
	Color <> (203u, 215u, 223u),
	Color <> (179u, 199u, 211u),
	Color <> (159u, 183u, 195u),
	Color <> (135u, 167u, 183u),
	Color <> (115u, 151u, 167u),
	Color <> ( 91u, 135u, 155u),
	Color <> ( 71u, 119u, 139u),
	Color <> ( 47u, 103u, 127u),
	Color <> ( 23u,  83u, 111u),
	Color <> ( 19u,  75u, 103u),
	Color <> ( 15u,  67u,  91u),
	Color <> ( 11u,  63u,  83u),
	Color <> (  7u,  55u,  75u),
	Color <> (  7u,  47u,  63u),
	Color <> (  7u,  39u,  51u),
	Color <> (  0u,  31u,  43u),
	Color <> (  0u,  23u,  31u),
	Color <> (  0u,  15u,  19u),
	Color <> (  0u,   7u,  11u),
	Color <> (  0u,   0u,   0u),
	Color <> (139u,  87u,  87u),
	Color <> (131u,  79u,  79u),
	Color <> (123u,  71u,  71u),
	Color <> (115u,  67u,  67u),
	Color <> (107u,  59u,  59u),
	Color <> ( 99u,  51u,  51u),
	Color <> ( 91u,  47u,  47u),
	Color <> ( 87u,  43u,  43u),
	Color <> ( 75u,  35u,  35u),
	Color <> ( 63u,  31u,  31u),
	Color <> ( 51u,  27u,  27u),
	Color <> ( 43u,  19u,  19u),
	Color <> ( 31u,  15u,  15u),
	Color <> ( 19u,  11u,  11u),
	Color <> ( 11u,   7u,   7u),
	Color <> (  0u,   0u,   0u),
	Color <> (151u, 159u, 123u),
	Color <> (143u, 151u, 115u),
	Color <> (135u, 139u, 107u),
	Color <> (127u, 131u,  99u),
	Color <> (119u, 123u,  95u),
	Color <> (115u, 115u,  87u),
	Color <> (107u, 107u,  79u),
	Color <> ( 99u,  99u,  71u),
	Color <> ( 91u,  91u,  67u),
	Color <> ( 79u,  79u,  59u),
	Color <> ( 67u,  67u,  51u),
	Color <> ( 55u,  55u,  43u),
	Color <> ( 47u,  47u,  35u),
	Color <> ( 35u,  35u,  27u),
	Color <> ( 23u,  23u,  19u),
	Color <> ( 15u,  15u,  11u),
	Color <> (159u,  75u,  63u),
	Color <> (147u,  67u,  55u),
	Color <> (139u,  59u,  47u),
	Color <> (127u,  55u,  39u),
	Color <> (119u,  47u,  35u),
	Color <> (107u,  43u,  27u),
	Color <> ( 99u,  35u,  23u),
	Color <> ( 87u,  31u,  19u),
	Color <> ( 79u,  27u,  15u),
	Color <> ( 67u,  23u,  11u),
	Color <> ( 55u,  19u,  11u),
	Color <> ( 43u,  15u,   7u),
	Color <> ( 31u,  11u,   7u),
	Color <> ( 23u,   7u,   0u),
	Color <> ( 11u,   0u,   0u),
	Color <> (  0u,   0u,   0u),
	Color <> (119u, 123u, 207u),
	Color <> (111u, 115u, 195u),
	Color <> (103u, 107u, 183u),
	Color <> ( 99u,  99u, 167u),
	Color <> ( 91u,  91u, 155u),
	Color <> ( 83u,  87u, 143u),
	Color <> ( 75u,  79u, 127u),
	Color <> ( 71u,  71u, 115u),
	Color <> ( 63u,  63u, 103u),
	Color <> ( 55u,  55u,  87u),
	Color <> ( 47u,  47u,  75u),
	Color <> ( 39u,  39u,  63u),
	Color <> ( 35u,  31u,  47u),
	Color <> ( 27u,  23u,  35u),
	Color <> ( 19u,  15u,  23u),
	Color <> ( 11u,   7u,   7u),
	Color <> (155u, 171u, 123u),
	Color <> (143u, 159u, 111u),
	Color <> (135u, 151u,  99u),
	Color <> (123u, 139u,  87u),
	Color <> (115u, 131u,  75u),
	Color <> (103u, 119u,  67u),
	Color <> ( 95u, 111u,  59u),
	Color <> ( 87u, 103u,  51u),
	Color <> ( 75u,  91u,  39u),
	Color <> ( 63u,  79u,  27u),
	Color <> ( 55u,  67u,  19u),
	Color <> ( 47u,  59u,  11u),
	Color <> ( 35u,  47u,   7u),
	Color <> ( 27u,  35u,   0u),
	Color <> ( 19u,  23u,   0u),
	Color <> ( 11u,  15u,   0u),
	Color <> (  0u, 255u,   0u),
	Color <> ( 35u, 231u,  15u),
	Color <> ( 63u, 211u,  27u),
	Color <> ( 83u, 187u,  39u),
	Color <> ( 95u, 167u,  47u),
	Color <> ( 95u, 143u,  51u),
	Color <> ( 95u, 123u,  51u),
	Color <> (255u, 255u, 255u),
	Color <> (255u, 255u, 211u),
	Color <> (255u, 255u, 167u),
	Color <> (255u, 255u, 127u),
	Color <> (255u, 255u,  83u),
	Color <> (255u, 255u,  39u),
	Color <> (255u, 235u,  31u),
	Color <> (255u, 215u,  23u),
	Color <> (255u, 191u,  15u),
	Color <> (255u, 171u,   7u),
	Color <> (255u, 147u,   0u),
	Color <> (239u, 127u,   0u),
	Color <> (227u, 107u,   0u),
	Color <> (211u,  87u,   0u),
	Color <> (199u,  71u,   0u),
	Color <> (183u,  59u,   0u),
	Color <> (171u,  43u,   0u),
	Color <> (155u,  31u,   0u),
	Color <> (143u,  23u,   0u),
	Color <> (127u,  15u,   0u),
	Color <> (115u,   7u,   0u),
	Color <> ( 95u,   0u,   0u),
	Color <> ( 71u,   0u,   0u),
	Color <> ( 47u,   0u,   0u),
	Color <> ( 27u,   0u,   0u),
	Color <> (239u,   0u,   0u),
	Color <> ( 55u,  55u, 255u),
	Color <> (255u,   0u,   0u),
	Color <> (  0u,   0u, 255u),
	Color <> ( 43u,  43u,  35u),
	Color <> ( 27u,  27u,  23u),
	Color <> ( 19u,  19u,  15u),
	Color <> (235u, 151u, 127u),
	Color <> (195u, 115u,  83u),
	Color <> (159u,  87u,  51u),
	Color <> (123u,  63u,  27u),
	Color <> (235u, 211u, 199u),
	Color <> (199u, 171u, 155u),
	Color <> (167u, 139u, 119u),
	Color <> (135u, 107u,  87u),
	Color <> (159u,  91u,  83u)
};

/**
\fn	inline const ColorIndex_t GetColorIndex (const Color <> &color)

\brief	Returns an index from the RGB color 'color'.

\author	Paril
\date	25/05/2010

\param	color	The color, in RGB (A is ignored).

\return	Color index.
**/
inline const ColorIndex_t GetColorIndex (const Color <> &color)
{
	float currentDistance (255u * 255u * 255u);
	ColorIndex_t colorIndex (Black);

	for (unsigned short index (Black); index < ColorIndex_Total; ++index)
	{
		const float distance (color.GetDistance (colorPalette[index]));

		if (currentDistance > distance)
		{
			currentDistance = distance;
			colorIndex = static_cast <ColorIndex_t> (index);
		}
	}

	return colorIndex;
}

/**
\page Utilities
\section Colors

CleanCode offers a header and source file which include a few convenience functions
for working with the Quake II color palette. All 256 colors have been linked to names
generated by an online conversion utility (more or less "closest to" the palette colors).

Due to the nature of some of these names it may be trial and error figuring them out,
however you do have other alternatives that you can use for debugging and finding
colors. The 'GetColorIndex' function is given as a method of finding colors from
the closest RGB value.

\todo Possibly sort the color enumeration by shade or something
**/

#endif	// ifndef COLOR_INCLUDED