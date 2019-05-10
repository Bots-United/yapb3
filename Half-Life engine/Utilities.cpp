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
// Utilities.cpp
//
// Version: $ID:$
//

#include <Core.hpp>

namespace HalfLifeEngine
{
namespace Utilities
{
//	ZBotInGameEnabler g_ZBotInGameEnabler;

namespace Light
{
//const SDK::Structures::MPlane_t *lightplane (NULL);
//Math::Vector3D lightspot;
Color <unsigned int> g_pointColor;

template <typename nodeType, typename surfaceType> const bool RecursiveLightPoint (const int *const d_lightstylevalue, const SDK::Structures::Model_t *const worldmodel, const nodeType *const node, const Math::Vector3D &start, const Math::Vector3D &end)
{
	float front, back, frac;
	int side;
	const SDK::Structures::MPlane_t *plane;
	Math::Vector3D mid;
	const surfaceType *surf;
	int s, t, ds, dt;
	int i;
	const SDK::Structures::mtexinfo_t *tex;
	const Color <> *lightmap;
	unsigned int scale;
	unsigned char maps;

	// Reliability check.
	InternalAssert (node != NULL);

	if (node->contents < 0)
		return false;	// didn't hit anything

	// Determine which side of the node plane our points are on
	// FIXME: optimize for axial
	plane = node->plane;
	front = plane->GetDistanceToPoint (start);
	back = plane->GetDistanceToPoint (end);
	side = front < 0.0f;

	// If they're both on the same side of the plane, don't bother to split just check the appropriate child
	if ((back < 0.0f) == side)
		return RecursiveLightPoint <nodeType, surfaceType> (d_lightstylevalue, worldmodel, reinterpret_cast <nodeType *const> (node->children[side]), start, end);

	// calculate mid point
	frac = front / (front - back);
	mid.Interpolate (start, end, frac);

	// go down front side	
	if (RecursiveLightPoint <nodeType, surfaceType> (d_lightstylevalue, worldmodel, reinterpret_cast <nodeType *const> (node->children[side]), start, mid))
		return true;	// hit something

	// Blow it off if it doesn't split the plane...
	if ((back < 0.0f) == side)
		return false;	// didn't hit anything

	// check for impact on this node
//	lightspot = mid;
//	lightplane = plane;

	surf = reinterpret_cast <surfaceType *const> (worldmodel->surfaces) + node->firstsurface;
	for (i = 0; i < node->numsurfaces; ++i, ++surf)
	{
		if (surf->flags & SURF_DRAWTILED)
			continue;	// no lightmaps

		tex = surf->texinfo;

		// See where in lightmap space our intersection point is
		s = static_cast <int> ((mid | Math::Vector3D (tex->vecs[0])) + tex->vecs[0][3]);
		t = static_cast <int> ((mid | Math::Vector3D (tex->vecs[1])) + tex->vecs[1][3]);

		// Not in the bounds of our lightmap? punt...
		if (s < surf->texturemins[0] || t < surf->texturemins[1])
			continue;

		// assuming a square lightmap (FIXME: which ain't always the case),
		// lets see if it lies in that rectangle. If not, punt...
		ds = s - surf->texturemins[0];
		dt = t - surf->texturemins[1];

		if (ds > surf->extents[0] || dt > surf->extents[1])
			continue;

		if (surf->samples == NULL)
			return true;

		ds >>= 4;
		dt >>= 4;

		g_pointColor = Color <unsigned int>::ZeroValue;	// Reset point color.

		const int smax ((surf->extents[0] >> 4) + 1);
		const int tmax ((surf->extents[1] >> 4) + 1);
		const int size (smax * tmax);

		lightmap = surf->samples + dt * smax + ds;

		// Compute the lightmap color at a particular point
		for (maps = 0u; maps < MAXLIGHTMAPS && surf->styles[maps] != 255u; ++maps)
		{
			scale = d_lightstylevalue[surf->styles[maps]];

//			g_pointColor += *lightmap * scale;
			g_pointColor.red += lightmap->red * scale;
			g_pointColor.green += lightmap->green * scale;
			g_pointColor.blue += lightmap->blue * scale;

			lightmap += size;	// skip to next lightmap
		}

		g_pointColor >>= 8u;

		return true;
	}

	// go down back side
	return RecursiveLightPoint <nodeType, surfaceType> (d_lightstylevalue, worldmodel, reinterpret_cast <nodeType *const> (node->children[!side]), mid, end);
}
const bool ActualRecursiveLightPoint (const int *const d_lightstylevalue, const SDK::Structures::Model_t *const worldmodel, const Math::Vector3D &start, const Math::Vector3D &end)
{
	return Globals::g_halfLifeEngine->IsSoftwareRenderingMode () ?
		RecursiveLightPoint <SDK::Structures::mnode_t, SDK::Structures::msurface_t> (d_lightstylevalue, worldmodel, worldmodel->nodes, start, end) :
		RecursiveLightPoint <SDK::Structures::GL_mnode_t, SDK::Structures::GL_msurface_t> (d_lightstylevalue, worldmodel, reinterpret_cast <SDK::Structures::GL_mnode_t *const> (worldmodel->nodes), start, end);
}
}

/*static */const unsigned int SharedRandomNumberGenerator::sm_seed_table[MaximumSeedTableSize] =
{
	28985u, 27138u, 26457u, 9451u, 17764u, 10909u, 28790u, 8716u, 6361u, 4853u, 17798u, 21977u, 19643u, 20662u, 10834u, 20103u,
	27067u, 28634u, 18623u, 25849u, 8576u, 26234u, 23887u, 18228u, 32587u, 4836u, 3306u, 1811u, 3035u, 24559u, 18399u, 315u,
	26766u, 907u, 24102u, 12370u, 9674u, 2972u, 10472u, 16492u, 22683u, 11529u, 27968u, 30406u, 13213u, 2319u, 23620u, 16823u,
	10013u, 23772u, 21567u, 1251u, 19579u, 20313u, 18241u, 30130u, 8402u, 20807u, 27354u, 7169u, 21211u, 17293u, 5410u, 19223u,
	10255u, 22480u, 27388u, 9946u, 15628u, 24389u, 17308u, 2370u, 9530u, 31683u, 25927u, 23567u, 11694u, 26397u, 32602u, 15031u,
	18255u, 17582u, 1422u, 28835u, 23607u, 12597u, 20602u, 10138u, 5212u, 1252u, 10074u, 23166u, 19823u, 31667u, 5902u, 24630u,
	18948u, 14330u, 14950u, 8939u, 23540u, 21311u, 22428u, 22391u, 3583u, 29004u, 30498u, 18714u, 4278u, 2437u, 22430u, 3439u,
	28313u, 23161u, 25396u, 13471u, 19324u, 15287u, 2563u, 18901u, 13103u, 16867u, 9714u, 14322u, 15197u, 26889u, 19372u, 26241u,
	31925u, 14640u, 11497u, 8941u, 10056u, 6451u, 28656u, 10737u, 13874u, 17356u, 8281u, 25937u, 1661u, 4850u, 7448u, 12744u,
	21826u, 5477u, 10167u, 16705u, 26897u, 8839u, 30947u, 27978u, 27283u, 24685u, 32298u, 3525u, 12398u, 28726u, 9475u, 10208u,
	617u, 13467u, 22287u, 2376u, 6097u, 26312u, 2974u, 9114u, 21787u, 28010u, 4725u, 15387u, 3274u, 10762u, 31695u, 17320u,
	18324u, 12441u, 16801u, 27376u, 22464u, 7500u, 5666u, 18144u, 15314u, 31914u, 31627u, 6495u, 5226u, 31203u, 2331u, 4668u,
	12650u, 18275u, 351u, 7268u, 31319u, 30119u, 7600u, 2905u, 13826u, 11343u, 13053u, 15583u, 30055u, 31093u, 5067u, 761u,
	9685u, 11070u, 21369u, 27155u, 3663u, 26542u, 20169u, 12161u, 15411u, 30401u, 7580u, 31784u, 8985u, 29367u, 20989u, 14203u,
	29694u, 21167u, 10337u, 1706u, 28578u, 887u, 3373u, 19477u, 14382u, 675u, 7033u, 15111u, 26138u, 12252u, 30996u, 21409u,
	25678u, 18555u, 13256u, 23316u, 22407u, 16727u, 991u, 9236u, 5373u, 29402u, 6117u, 15241u, 27715u, 19291u, 19888u, 19847u
};

SharedRandomNumberGenerator g_sharedRandomNumberGenerator;

/*
=============================================================================

						PARSING STUFF

=============================================================================
*/

#define COM_TOKEN_MAX_LENGTH 1024u
char com_token[COM_TOKEN_MAX_LENGTH];
bool s_com_token_unget (false);
bool com_ignorecolons (false);	// YWB: Ignore colons as token separators in COM_Parse

/*
==============
COM_Parse

	Parse a token out of a string (into global: char com_token[COM_TOKEN_MAX_LENGTH])
==============
*/
char *const COM_Parse (char *data)
{
	if (s_com_token_unget)
	{
		s_com_token_unget = false;

		return data;
	}

	com_token[0u] = '\0';

	if (data == NULL)
		return NULL;

	char c;
	unsigned int len (0u);
	static const CharacterSet breakSet[] = {CharacterSet ("{}()':,"), CharacterSet ("{}()',")};	// wordbreak parsing set
	const CharacterSet &breaks (breakSet[com_ignorecolons]);

// skip whitespace
skipwhite:
// eat whitespace (LF,CR,etc.) at the end of this line
	while ((c = *data) <= ' ')
	{
		if (c == '\0')
			return NULL;	// end of file;

		++data;
	}

	// skip // comments
	if (c == '/' && data[sizeof ('/')] == '/')
	{
		++data;	// skip single comment character '/'

		while (*++data != '\0' && *data != '\n');

		goto skipwhite;
	}

	// handle quoted strings specially
	if (c == '"')
		for (;;)
		{
			c = *++data;

			if (c == '"' || c == '\0' || len >= GET_STATIC_STRING_LENGTH (com_token))
			{
				com_token[len] = '\0';

				return data + sizeof (c);
			}

			com_token[len] = c;
			++len;
		}

	// parse single characters
	if (breaks[c])
	{
		com_token[0u] = c;
		com_token[sizeof (c)] = '\0';

		return data + sizeof (c);
	}

	// parse a regular word
	do
	{
		com_token[len] = c;
		++data;
		++len;
		c = *data;
	} while (!breaks[c] && len < GET_STATIC_STRING_LENGTH (com_token) && c > ' ');

	com_token[len] = '\0';

	return data;
}
}
}