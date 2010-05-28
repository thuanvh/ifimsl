/*
===============================================================================

	FILE:  glowAux.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Auxiliary classes in GLOW
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <cstring>

#ifndef GLOW_AUX__H
	#include "glowAux.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	GlowColors
===============================================================================
*/

const GlowColor GlowColor::black(0);
const GlowColor GlowColor::blue(GLubyte(0), GLubyte(0), GLubyte(255));
const GlowColor GlowColor::green(GLubyte(0), GLubyte(255), GLubyte(0));
const GlowColor GlowColor::cyan(GLubyte(0), GLubyte(255), GLubyte(255));
const GlowColor GlowColor::red(GLubyte(255), GLubyte(0), GLubyte(0));
const GlowColor GlowColor::magenta(GLubyte(255), GLubyte(0), GLubyte(255));
const GlowColor GlowColor::yellow(GLubyte(255), GLubyte(255), GLubyte(0));
const GlowColor GlowColor::white(255);
const GlowColor GlowColor::gray(GLubyte(128), GLubyte(128), GLubyte(128));


/*
===============================================================================
	Methods and data for GlowFont
===============================================================================
*/

GlowFont GlowFont::fixed8by13 = GLUT_BITMAP_8_BY_13;
GlowFont GlowFont::fixed9by15 = GLUT_BITMAP_9_BY_15;
GlowFont GlowFont::helvetica10 = GLUT_BITMAP_HELVETICA_10;
GlowFont GlowFont::helvetica12 = GLUT_BITMAP_HELVETICA_12;
GlowFont GlowFont::helvetica18 = GLUT_BITMAP_HELVETICA_18;
GlowFont GlowFont::timesRoman10 = GLUT_BITMAP_TIMES_ROMAN_10;
GlowFont GlowFont::timesRoman24 = GLUT_BITMAP_TIMES_ROMAN_24;

int GlowFont::_leading8by13 = 15;
int GlowFont::_leading9by15 = 18;
int GlowFont::_leadingHelve10 = 12;
int GlowFont::_leadingHelve12 = 14;
int GlowFont::_leadingHelve18 = 22;
int GlowFont::_leadingTimes10 = 12;
int GlowFont::_leadingTimes24 = 28;

int GlowFont::_baseline8by13 = 11;
int GlowFont::_baseline9by15 = 13;
int GlowFont::_baselineHelve10 = 10;
int GlowFont::_baselineHelve12 = 11;
int GlowFont::_baselineHelve18 = 17;
int GlowFont::_baselineTimes10 = 9;
int GlowFont::_baselineTimes24 = 21;


int GlowFont::Leading() const
{
	if (_glutFont == GLUT_BITMAP_8_BY_13)
	{
		return _leading8by13;
	}
	else if (_glutFont == GLUT_BITMAP_9_BY_15)
	{
		return _leading9by15;
	}
	else if (_glutFont == GLUT_BITMAP_HELVETICA_10)
	{
		return _leadingHelve10;
	}
	else if (_glutFont == GLUT_BITMAP_HELVETICA_12)
	{
		return _leadingHelve12;
	}
	else if (_glutFont == GLUT_BITMAP_HELVETICA_18)
	{
		return _leadingHelve18;
	}
	else if (_glutFont == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		return _leadingTimes10;
	}
	else if (_glutFont == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		return _leadingTimes24;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::Leading");
	return 0;
}


int GlowFont::BaselinePos() const
{
	if (_glutFont == GLUT_BITMAP_8_BY_13)
	{
		return _baseline8by13;
	}
	else if (_glutFont == GLUT_BITMAP_9_BY_15)
	{
		return _baseline9by15;
	}
	else if (_glutFont == GLUT_BITMAP_HELVETICA_10)
	{
		return _baselineHelve10;
	}
	else if (_glutFont == GLUT_BITMAP_HELVETICA_12)
	{
		return _baselineHelve12;
	}
	else if (_glutFont == GLUT_BITMAP_HELVETICA_18)
	{
		return _baselineHelve18;
	}
	else if (_glutFont == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		return _baselineTimes10;
	}
	else if (_glutFont == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		return _baselineTimes24;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::BaselinePos");
	return 0;
}


void GlowFont::SetLeadingMetric(
	GlowFont font,
	int value)
{
	if (font == GLUT_BITMAP_8_BY_13)
	{
		_leading8by13 = value;
	}
	else if (font == GLUT_BITMAP_9_BY_15)
	{
		_leading9by15 = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_10)
	{
		_leadingHelve10 = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_12)
	{
		_leadingHelve12 = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_18)
	{
		_leadingHelve18 = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		_leadingTimes10 = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		_leadingTimes24 = value;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::SetLeadingMetric");
}


void GlowFont::SetBaselineMetric(
	GlowFont font,
	int value)
{
	if (font == GLUT_BITMAP_8_BY_13)
	{
		_baseline8by13 = value;
	}
	else if (font == GLUT_BITMAP_9_BY_15)
	{
		_baseline9by15 = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_10)
	{
		_baselineHelve10 = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_12)
	{
		_baselineHelve12 = value;
	}
	else if (font == GLUT_BITMAP_HELVETICA_18)
	{
		_baselineHelve18 = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_10)
	{
		_baselineTimes10 = value;
	}
	else if (font == GLUT_BITMAP_TIMES_ROMAN_24)
	{
		_baselineTimes24 = value;
	}
	GLOW_DEBUG(true, "Unknown font in GlowFont::SetBaselineMetric");
}


int GlowFont::StringWidth(
	const char* str) const
{
	GLOW_DEBUGSCOPE("GlowFont::StringWidth");
	
	int ret = 0;
	int len = GLOW_CSTD::strlen(str);
	for (int i=0; i<len; i++)
	{
		ret += ::glutBitmapWidth(_glutFont, str[i]);
	}
	return ret;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

