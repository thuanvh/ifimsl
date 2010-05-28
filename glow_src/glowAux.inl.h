/*
===============================================================================

	FILE:  glowAux.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Auxiliary classes for GLOW
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
	COPYRIGHT:
	
		Copyright (C) 1997-2000  Daniel Azuma  (dazuma@kagi.com)
		
		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2.1 of the License, or (at your option) any later version.
		
		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
		GNU Lesser General Public License for more details.
		
		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 1.0.3  (10 September 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods for GlowFont
===============================================================================
*/

inline GlowFont::GlowFont()
{
	_glutFont = GLUT_BITMAP_HELVETICA_12;
}


inline GlowFont::GlowFont(
	void* glutFont)
{
	GLOW_DEBUG(glutFont != GLUT_BITMAP_HELVETICA_10 &&
		glutFont != GLUT_BITMAP_HELVETICA_12 &&
		glutFont != GLUT_BITMAP_HELVETICA_18 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_10 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_24 &&
		glutFont != GLUT_BITMAP_8_BY_13 &&
		glutFont != GLUT_BITMAP_9_BY_15, "Unknown GLUT font");
	_glutFont = glutFont;
}


inline GlowFont::operator void*()
{
	return _glutFont;
}


inline GlowFont::operator const void*() const
{
	return _glutFont;
}


inline GlowFont& GlowFont::operator=(
	const GlowFont& font)
{
	_glutFont = font._glutFont;
	return *this;
}


inline GlowFont& GlowFont::operator=(
	void* glutFont)
{
	GLOW_DEBUG(glutFont != GLUT_BITMAP_HELVETICA_10 &&
		glutFont != GLUT_BITMAP_HELVETICA_12 &&
		glutFont != GLUT_BITMAP_HELVETICA_18 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_10 &&
		glutFont != GLUT_BITMAP_TIMES_ROMAN_24 &&
		glutFont != GLUT_BITMAP_8_BY_13 &&
		glutFont != GLUT_BITMAP_9_BY_15, "Unknown GLUT font");
	_glutFont = glutFont;
	return *this;
}


inline bool GlowFont::operator==(
	void* op2) const
{
	return _glutFont == op2;
}


inline int GlowFont::CharWidth(
	int ch) const
{
	return ::glutBitmapWidth(_glutFont, ch);
}


/*
===============================================================================
	Inline methods for GlowColor
===============================================================================
*/

inline GlowColor::GlowColor(
	GLubyte rgbval,
	GLubyte aval)
{
	_rgba[0] = _rgba[1] = _rgba[2] = rgbval;
	_rgba[3] = aval;
}


inline GlowColor::GlowColor(
	GLubyte r,
	GLubyte g,
	GLubyte b,
	GLubyte a)
{
	_rgba[0] = r;
	_rgba[1] = g;
	_rgba[2] = b;
	_rgba[3] = a;
}


inline GlowColor::GlowColor(
	GLfloat r,
	GLfloat g,
	GLfloat b,
	GLfloat a)
{
	_rgba[0] = GLubyte(int(r*255.0f));
	_rgba[1] = GLubyte(int(g*255.0f));
	_rgba[2] = GLubyte(int(b*255.0f));
	_rgba[3] = GLubyte(int(a*255.0f));
}


inline GlowColor::GlowColor(
	GLdouble r,
	GLdouble g,
	GLdouble b,
	GLdouble a)
{
	_rgba[0] = GLubyte(int(r*255.0));
	_rgba[1] = GLubyte(int(g*255.0));
	_rgba[2] = GLubyte(int(b*255.0));
	_rgba[3] = GLubyte(int(a*255.0));
}


inline void GlowColor::Set(
	GLubyte rgbval,
	GLubyte aval)
{
	_rgba[0] = _rgba[1] = _rgba[2] = rgbval;
	_rgba[3] = aval;
}


inline void GlowColor::Set(
	GLubyte r,
	GLubyte g,
	GLubyte b,
	GLubyte a)
{
	_rgba[0] = r;
	_rgba[1] = g;
	_rgba[2] = b;
	_rgba[3] = a;
}


inline void GlowColor::Set(
	GLfloat r,
	GLfloat g,
	GLfloat b,
	GLfloat a)
{
	_rgba[0] = GLubyte(int(r*255.0f));
	_rgba[1] = GLubyte(int(g*255.0f));
	_rgba[2] = GLubyte(int(b*255.0f));
	_rgba[3] = GLubyte(int(a*255.0f));
}


inline void GlowColor::Set(
	GLdouble r,
	GLdouble g,
	GLdouble b,
	GLdouble a)
{
	_rgba[0] = GLubyte(int(r*255.0));
	_rgba[1] = GLubyte(int(g*255.0));
	_rgba[2] = GLubyte(int(b*255.0));
	_rgba[3] = GLubyte(int(a*255.0));
}


inline GlowColor::operator GLubyte*()
{
	return _rgba;
}


inline GlowColor::operator const GLubyte*() const
{
	return _rgba;
}


inline GLubyte& GlowColor::operator[](
	int i)
{
	GLOW_ASSERT(i>=0 && i<4);
	return _rgba[i];
}


inline const GLubyte& GlowColor::operator[](
	int i) const
{
	GLOW_ASSERT(i>=0 && i<4);
	return _rgba[i];
}


inline bool GlowColor::operator==(
	const GlowColor& op2) const
{
	return _rgba[0] == op2._rgba[0] &&
		_rgba[1] == op2._rgba[1] &&
		_rgba[2] == op2._rgba[2] &&
		_rgba[3] == op2._rgba[3];
}


inline bool GlowColor::operator!=(
	const GlowColor& op2) const
{
	return _rgba[0] != op2._rgba[0] ||
		_rgba[1] != op2._rgba[1] ||
		_rgba[2] != op2._rgba[2] ||
		_rgba[3] != op2._rgba[3];
}


inline void GlowColor::Apply() const
{
	::glColor4ubv(_rgba);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

