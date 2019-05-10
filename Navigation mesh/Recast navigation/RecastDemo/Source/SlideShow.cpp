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
#include <Navigation mesh/Recast navigation/RecastDemo/Contrib/stb_image.h>

SlideShow::SlideShow() :
	m_width(0),
	m_height(0),
	m_texId(0),
	m_showCurSlide(true),
	m_slideAlpha(1.0f),
	m_curSlide(-1),
	m_nextSlide(0)
{
}

SlideShow::~SlideShow()
{
	purgeImage();
}

void SlideShow::purgeImage()
{
	if (m_texId)
	{
		g_drawInterface->glDeleteTextures(1, (GLuint*)&m_texId);
		m_texId = 0;
		m_width = 0;
		m_height = 0;
	}
}

bool SlideShow::loadImage(const char* path)
{
	purgeImage();
	
	int bpp;
	unsigned char* data = stbi_load(path, &m_width, &m_height, &bpp, 4);
	if (!data)
	{
		printf("Could not load file '%s': %s\n", path, stbi_failure_reason());
		return false;
	}
	
	g_drawInterface->glGenTextures(1, (GLuint*)&m_texId);
	g_drawInterface->glBindTexture(GL_TEXTURE_RECTANGLE_ARB, m_texId);
	g_drawInterface->glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, m_width, m_height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	g_drawInterface->glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	g_drawInterface->glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	g_drawInterface->glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	g_drawInterface->glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	stbi_image_free(data);
	
	return true;
}

bool SlideShow::init(const char* path)
{
	strcpy(m_path, path);
	m_files.ScanDirectory(m_path, ".png");
	
	return true;
}

void SlideShow::nextSlide()
{
	setSlide(m_nextSlide+1);
}

void SlideShow::prevSlide()
{
	setSlide(m_nextSlide-1);
}

void SlideShow::setSlide(int n)
{
	const int maxIdx = m_files.GetElementNumber () ? m_files.GetElementNumber ()-1 : 0;
	m_nextSlide = n;
	if (m_nextSlide < 0) m_nextSlide = 0;
	if (m_nextSlide > maxIdx) m_nextSlide = maxIdx; 
}

void SlideShow::updateAndDraw(float dt, const float w, const float h)
{
	float slideAlphaTarget = (m_showCurSlide && m_texId) ? 1.0f : 0.0f;
	if (m_curSlide != m_nextSlide)
		slideAlphaTarget = 0;
	
	if (slideAlphaTarget > m_slideAlpha)
		m_slideAlpha += dt*4;
	else if (slideAlphaTarget < m_slideAlpha)
		m_slideAlpha -= dt*4;
	if (m_slideAlpha < 0) m_slideAlpha = 0;
	if (m_slideAlpha > 1) m_slideAlpha = 1; 
	
	if (m_curSlide != m_nextSlide && m_slideAlpha < 0.01f)
	{
		m_curSlide = m_nextSlide;
		if (m_curSlide >= 0 && m_curSlide < m_files.GetElementNumber ())
		{
			char path[256];
			strcpy(path, m_path);
			strcat(path, m_files[static_cast <FileList::IndexType_t> (m_curSlide)]);
			loadImage(path);
		}
	}
	
	if (m_slideAlpha > 0.01f && m_texId)
	{
		unsigned char alpha = (unsigned char)(m_slideAlpha*255.0f);
		
		g_drawInterface->glEnable(GL_TEXTURE_RECTANGLE_ARB);
		g_drawInterface->glBindTexture(GL_TEXTURE_RECTANGLE_ARB, m_texId);
		
		const float tw = (float)m_width;
		const float th = (float)m_height;
		const float hw = w*0.5f;
		const float hh = h*0.5f;
		
		g_drawInterface->glColor4ub(255,255,255,alpha);
		g_drawInterface->glBegin(GL_QUADS);
		g_drawInterface->glTexCoord2f(0,th);
		g_drawInterface->glVertex2f(hw-tw/2,hh-th/2);
		g_drawInterface->glTexCoord2f(tw,th);
		g_drawInterface->glVertex2f(hw+tw/2,hh-th/2);
		g_drawInterface->glTexCoord2f(tw,0);
		g_drawInterface->glVertex2f(hw+tw/2,hh+th/2);
		g_drawInterface->glTexCoord2f(0,0);
		g_drawInterface->glVertex2f(hw-tw/2,hh+th/2);
		g_drawInterface->glEnd();
		
		g_drawInterface->glDisable(GL_TEXTURE_RECTANGLE_ARB);
	}
}
#endif	// if defined MULTIPLE_DRAW_METHODS