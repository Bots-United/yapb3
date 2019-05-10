//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <Core.hpp>

#if defined MULTIPLE_DRAW_METHODS
// Some math headers don't have PI defined.
static const float PI = 3.14159265f;

void imguifree(void* ptr, void* userptr);
void* imguimalloc(size_t size, void* userptr);

#define STBTT_malloc(x,y)    imguimalloc(x,y)
#define STBTT_free(x,y)      imguifree(x,y)
#define STB_TRUETYPE_IMPLEMENTATION
#include <Navigation mesh/Recast navigation/RecastDemo/Contrib/stb_truetype.h>

void imguifree(void* ptr, void* /*userptr*/)
{
	delete [] ptr;
}

void* imguimalloc(size_t size, void* /*userptr*/)
{
	return new unsigned char[size];
}

static const unsigned TEMP_COORD_COUNT = 100;
static float g_tempCoords[TEMP_COORD_COUNT*2];
static float g_tempNormals[TEMP_COORD_COUNT*2];

static const int CIRCLE_VERTS = 8*4;
static float g_circleVerts[CIRCLE_VERTS*2];

static stbtt_bakedchar g_cdata[96]; // ASCII 32..126 is 95 glyphs
static GLuint g_ftex = 0;

inline unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return (r) | (g << 8) | (b << 16) | (a << 24);
}

static void drawPolygon(const float* coords, unsigned numCoords, float r, unsigned int col)
{
	if (numCoords > TEMP_COORD_COUNT) numCoords = TEMP_COORD_COUNT;
	
	for (unsigned i = 0, j = numCoords-1; i < numCoords; j=i++)
	{
		const float* v0 = &coords[j*2];
		const float* v1 = &coords[i*2];
		float dx = v1[0] - v0[0];
		float dy = v1[1] - v0[1];
		float d = Math::sqrtf(dx*dx+dy*dy);
		if (d > 0)
		{
			d = 1.0f/d;
			dx *= d;
			dy *= d;
		}
		g_tempNormals[j*2+0] = dy;
		g_tempNormals[j*2+1] = -dx;
	}
	
	for (unsigned i = 0, j = numCoords-1; i < numCoords; j=i++)
	{
		float dlx0 = g_tempNormals[j*2+0];
		float dly0 = g_tempNormals[j*2+1];
		float dlx1 = g_tempNormals[i*2+0];
		float dly1 = g_tempNormals[i*2+1];
		float dmx = (dlx0 + dlx1) * 0.5f;
		float dmy = (dly0 + dly1) * 0.5f;
		float	dmr2 = dmx*dmx + dmy*dmy;
		if (dmr2 > 0.000001f)
		{
			float	scale = 1.0f / dmr2;
			if (scale > 10.0f) scale = 10.0f;
			dmx *= scale;
			dmy *= scale;
		}
		g_tempCoords[i*2+0] = coords[i*2+0]+dmx*r;
		g_tempCoords[i*2+1] = coords[i*2+1]+dmy*r;
	}
	
	unsigned int colTrans = RGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, 0);
	
	g_drawInterface->glBegin(GL_TRIANGLES);
	
	g_drawInterface->glColor4ubv((GLubyte*)&col);
	
	for (unsigned i = 0, j = numCoords-1; i < numCoords; j=i++)
	{
		g_drawInterface->glVertex2fv(&coords[i*2]);
		g_drawInterface->glVertex2fv(&coords[j*2]);
		g_drawInterface->glColor4ubv((GLubyte*)&colTrans);
		g_drawInterface->glVertex2fv(&g_tempCoords[j*2]);
		
		g_drawInterface->glVertex2fv(&g_tempCoords[j*2]);
		g_drawInterface->glVertex2fv(&g_tempCoords[i*2]);
		
		g_drawInterface->glColor4ubv((GLubyte*)&col);
		g_drawInterface->glVertex2fv(&coords[i*2]);
	}
	
	g_drawInterface->glColor4ubv((GLubyte*)&col);
	for (unsigned i = 2; i < numCoords; ++i)
	{
		g_drawInterface->glVertex2fv(&coords[0]);
		g_drawInterface->glVertex2fv(&coords[(i-1)*2]);
		g_drawInterface->glVertex2fv(&coords[i*2]);
	}
	
	g_drawInterface->glEnd();
}

static void drawRect(float x, float y, float w, float h, float fth, unsigned int col)
{
	float verts[4*2] =
	{
		x+0.5f, y+0.5f,
		x+w-0.5f, y+0.5f,
		x+w-0.5f, y+h-0.5f,
		x+0.5f, y+h-0.5f,
	};
	drawPolygon(verts, 4, fth, col);
}

/*
static void drawEllipse(float x, float y, float w, float h, float fth, unsigned int col)
{
	float verts[CIRCLE_VERTS*2];
	const float* cverts = g_circleVerts;
	float* v = verts;
	
	for (int i = 0; i < CIRCLE_VERTS; ++i)
	{
		*v++ = x + cverts[i*2]*w;
		*v++ = y + cverts[i*2+1]*h;
	}
	
	drawPolygon(verts, CIRCLE_VERTS, fth, col);
}
*/

static void drawRoundedRect(float x, float y, float w, float h, float r, float fth, unsigned int col)
{
	const unsigned n = CIRCLE_VERTS/4;
	float verts[(n+1)*4*2];
	const float* cverts = g_circleVerts;
	float* v = verts;
	
	for (unsigned i = 0; i <= n; ++i)
	{
		*v++ = x+w-r + cverts[i*2]*r;
		*v++ = y+h-r + cverts[i*2+1]*r;
	}
	
	for (unsigned i = n; i <= n*2; ++i)
	{
		*v++ = x+r + cverts[i*2]*r;
		*v++ = y+h-r + cverts[i*2+1]*r;
	}
	
	for (unsigned i = n*2; i <= n*3; ++i)
	{
		*v++ = x+r + cverts[i*2]*r;
		*v++ = y+r + cverts[i*2+1]*r;
	}
	
	for (unsigned i = n*3; i < n*4; ++i)
	{
		*v++ = x+w-r + cverts[i*2]*r;
		*v++ = y+r + cverts[i*2+1]*r;
	}
	*v++ = x+w-r + cverts[0]*r;
	*v++ = y+r + cverts[1]*r;
	
	drawPolygon(verts, (n+1)*4, fth, col);
}


static void drawLine(float x0, float y0, float x1, float y1, float r, float fth, unsigned int col)
{
	float dx = x1-x0;
	float dy = y1-y0;
	float d = Math::sqrtf(dx*dx+dy*dy);
	if (d > 0.0001f)
	{
		d = 1.0f/d;
		dx *= d;
		dy *= d;
	}
	float nx = dy;
	float ny = -dx;
	float verts[4*2];
	r -= fth;
	r *= 0.5f;
	if (r < 0.01f) r = 0.01f;
	dx *= r;
	dy *= r;
	nx *= r;
	ny *= r;
	
	verts[0] = x0-dx-nx;
	verts[1] = y0-dy-ny;
	
	verts[2] = x0-dx+nx;
	verts[3] = y0-dy+ny;
	
	verts[4] = x1+dx+nx;
	verts[5] = y1+dy+ny;
	
	verts[6] = x1+dx-nx;
	verts[7] = y1+dy-ny;
	
	drawPolygon(verts, 4, fth, col);
}


bool imguiRenderGLInit(const char* fontpath)
{
	for (int i = 0; i < CIRCLE_VERTS; ++i)
	{
		float a = (float)i/(float)CIRCLE_VERTS * PI*2;
		g_circleVerts[i*2+0] = Math::cosf(a);
		g_circleVerts[i*2+1] = Math::sinf(a);
	}

	// Load font.
	FILE* fp = fopen(fontpath, "rb");
	if (!fp) return false;
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	unsigned char* ttfBuffer = new unsigned char[size]; 
	if (!ttfBuffer)
	{
		fclose(fp);
		return false;
	}
	
	fread(ttfBuffer, 1, size, fp);
	fclose(fp);
	fp = 0;
	
	unsigned char bmap[512 * 512];
	
	stbtt_BakeFontBitmap(ttfBuffer,0, 15.0f, bmap,512,512, 32,96, g_cdata);
	
	// can free ttf_buffer at this point
	g_drawInterface->glGenTextures(1, &g_ftex);
	g_drawInterface->glBindTexture(GL_TEXTURE_2D, g_ftex);
	g_drawInterface->glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bmap);
	g_drawInterface->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	g_drawInterface->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete [] ttfBuffer;

	return true;
}

void imguiRenderGLDestroy()
{
	if (g_ftex)
	{
		g_drawInterface->glDeleteTextures(1, &g_ftex);
		g_ftex = 0;
	}
}

static void getBakedQuad(stbtt_bakedchar *chardata, int pw, int ph, int char_index,
						 float *xpos, float *ypos, stbtt_aligned_quad *q)
{
	stbtt_bakedchar *b = chardata + char_index;
	int round_x = STBTT_ifloor(*xpos + b->xoff);
	int round_y = STBTT_ifloor(*ypos - b->yoff);
	
	q->x0 = (float)round_x;
	q->y0 = (float)round_y;
	q->x1 = (float)round_x + b->x1 - b->x0;
	q->y1 = (float)round_y - b->y1 + b->y0;
	
	q->s0 = b->x0 / (float)pw;
	q->t0 = b->y0 / (float)pw;
	q->s1 = b->x1 / (float)ph;
	q->t1 = b->y1 / (float)ph;
	
	*xpos += b->xadvance;
}

static const float g_tabStops[4] = {150, 210, 270, 330};

static float getTextLength(stbtt_bakedchar *chardata, const char* text)
{
	float xpos = 0;
	float len = 0;
	while (*text)
	{
		int c = (unsigned char)*text;
		if (c == '\t')
		{
			for (int i = 0; i < 4; ++i)
			{
				if (xpos < g_tabStops[i])
				{
					xpos = g_tabStops[i];
					break;
				}
			}
		}
		else if (c >= 32 && c < 128)
		{
			stbtt_bakedchar *b = chardata + c-32;
			int round_x = STBTT_ifloor((xpos + b->xoff) + 0.5f);
			len = round_x + b->x1 - b->x0 + 0.5f;
			xpos += b->xadvance;
		}
		++text;
	}
	return len;
}

static void drawText(float x, float y, const char *text, int align, unsigned int col)
{
	if (!g_ftex) return;
	if (!text) return;
	
	if (align == IMGUI_ALIGN_CENTER)
		x -= getTextLength(g_cdata, text)/2;
	else if (align == IMGUI_ALIGN_RIGHT)
		x -= getTextLength(g_cdata, text);
	
	g_drawInterface->glColor4ub(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff);
	
	g_drawInterface->glEnable(GL_TEXTURE_2D);
	
	// assume orthographic projection with units = screen pixels, origin at top left
	g_drawInterface->glBindTexture(GL_TEXTURE_2D, g_ftex);
	
	g_drawInterface->glBegin(GL_TRIANGLES);
	
	const float ox = x;
	
	while (*text)
	{
		int c = (unsigned char)*text;
		if (c == '\t')
		{
			for (int i = 0; i < 4; ++i)
			{
				if (x < g_tabStops[i]+ox)
				{
					x = g_tabStops[i]+ox;
					break;
				}
			}
		}
		else if (c >= 32 && c < 128)
		{			
			stbtt_aligned_quad q;
			getBakedQuad(g_cdata, 512,512, c-32, &x,&y,&q);
			
			g_drawInterface->glTexCoord2f(q.s0, q.t0);
			g_drawInterface->glVertex2f(q.x0, q.y0);
			g_drawInterface->glTexCoord2f(q.s1, q.t1);
			g_drawInterface->glVertex2f(q.x1, q.y1);
			g_drawInterface->glTexCoord2f(q.s1, q.t0);
			g_drawInterface->glVertex2f(q.x1, q.y0);
			
			g_drawInterface->glTexCoord2f(q.s0, q.t0);
			g_drawInterface->glVertex2f(q.x0, q.y0);
			g_drawInterface->glTexCoord2f(q.s0, q.t1);
			g_drawInterface->glVertex2f(q.x0, q.y1);
			g_drawInterface->glTexCoord2f(q.s1, q.t1);
			g_drawInterface->glVertex2f(q.x1, q.y1);
		}
		++text;
	}
	
	g_drawInterface->glEnd();	
	g_drawInterface->glDisable(GL_TEXTURE_2D);
}


void imguiRenderGLDraw()
{
	const imguiGfxCmd* q = imguiGetRenderQueue();
	int nq = imguiGetRenderQueueSize();

	const float s = 1.0f/8.0f;

	g_drawInterface->glDisable(GL_SCISSOR_TEST);
	for (int i = 0; i < nq; ++i)
	{
		const imguiGfxCmd& cmd = q[i];
		if (cmd.type == IMGUI_GFXCMD_RECT)
		{
			if (cmd.rect.r == 0)
			{
				drawRect((float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
						 (float)cmd.rect.w*s-1, (float)cmd.rect.h*s-1,
						 1.0f, cmd.col);
			}
			else
			{
				drawRoundedRect((float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
								(float)cmd.rect.w*s-1, (float)cmd.rect.h*s-1,
								(float)cmd.rect.r*s, 1.0f, cmd.col);
			}
		}
		else if (cmd.type == IMGUI_GFXCMD_LINE)
		{
			drawLine(cmd.line.x0*s, cmd.line.y0*s, cmd.line.x1*s, cmd.line.y1*s, cmd.line.r*s, 1.0f, cmd.col);
		}
		else if (cmd.type == IMGUI_GFXCMD_TRIANGLE)
		{
			if (cmd.flags == 1)
			{
				const float verts[3*2] =
				{
					(float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
					(float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s-1, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s/2-0.5f,
					(float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
				};
				drawPolygon(verts, 3, 1.0f, cmd.col);
			}
			if (cmd.flags == 2)
			{
				const float verts[3*2] =
				{
					(float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
					(float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s/2-0.5f, (float)cmd.rect.y*s+0.5f,
					(float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s-1, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
				};
				drawPolygon(verts, 3, 1.0f, cmd.col);
			}
		}
		else if (cmd.type == IMGUI_GFXCMD_TEXT)
		{
			drawText(cmd.text.x, cmd.text.y, cmd.text.text, cmd.text.align, cmd.col);
		}
		else if (cmd.type == IMGUI_GFXCMD_SCISSOR)
		{
			if (cmd.flags)
			{
				g_drawInterface->glEnable(GL_SCISSOR_TEST);
				g_drawInterface->glScissor(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h);
			}
			else
			{
				g_drawInterface->glDisable(GL_SCISSOR_TEST);
			}
		}
	}
	g_drawInterface->glDisable(GL_SCISSOR_TEST);
}
#endif	// if defined MULTIPLE_DRAW_METHODS