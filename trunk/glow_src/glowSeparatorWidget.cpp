/*
===============================================================================

	FILE:  glowSeparatorWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Separator rule for GLOW
	
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

#ifndef GLOW_SEPARATORWIDGET__H
	#include "glowSeparatorWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowSeparatorParams GlowSeparatorParams::defaults(true);

GlowSeparatorParams::GlowSeparatorParams()
{
	*this = defaults;
}

GlowSeparatorParams::GlowSeparatorParams(bool) :
GlowWidgetParams(true),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.3f, 0.3f, 0.3f)
{
	width = 100;
	height = 4;
	style = GlowSeparatorWidget::valleyStyle;
}


/*
===============================================================================
	Methods of GlowSeparator
===============================================================================
*/

void GlowSeparatorWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowSeparatorParams& params)
{
	GLOW_DEBUGSCOPE("GlowSeparatorWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	_style = params.style;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
}


GlowWidget::AutoPackError GlowSeparatorWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowSeparatorWidget::OnAutoPack");
	
	if (hSize != unspecifiedSize && hSize < 2)
	{
		return hAutoPackError;
	}
	if (vSize != unspecifiedSize && vSize < 2)
	{
		return vAutoPackError;
	}
	
	int hnew = Width();
	int vnew = Height();
	
	if (Width() > Height())
	{
		if (hOption != noReshape)
		{
			if (hSize != unspecifiedSize)
			{
				hnew = hSize;
			}
		}
		if (vOption == forcedSize)
		{
			vnew = vSize;
		}
		else if (vOption == preferredSize || vOption == expandPreferredSize)
		{
			vnew = 4;
		}
	}
	else
	{
		if (hOption == forcedSize)
		{
			hnew = hSize;
		}
		else if (hOption == preferredSize || hOption == expandPreferredSize)
		{
			hnew = 4;
		}
		if (vOption != noReshape)
		{
			if (vSize != unspecifiedSize)
			{
				vnew = vSize;
			}
		}
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowSeparatorWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowSeparatorWidget::OnWidgetPaint");
	
	if (_style == transparentStyle)
	{
		return;
	}
	
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	
	if (_style == ridgeStyle)
	{
		_lightBevelColor.Apply();
	}
	else
	{
		_darkBevelColor.Apply();
	}
	::glBegin(GL_LINES);
	if (Width() > Height())
	{
		::glVertex2f(-1.0f, 0.0f);
		::glVertex2f(1.0f, 0.0f);
	}
	else
	{
		::glVertex2f(0.0f-etchWidth, -1.0f);
		::glVertex2f(0.0f-etchWidth, 1.0f);
	}
	::glEnd();
	if (_style == valleyStyle)
	{
		_lightBevelColor.Apply();
	}
	else
	{
		_darkBevelColor.Apply();
	}
	::glBegin(GL_LINES);
	if (Width() > Height())
	{
		::glVertex2f(-1.0f, 0.0f-etchHeight);
		::glVertex2f(1.0f, 0.0f-etchHeight);
	}
	else
	{
		::glVertex2f(0.0f, -1.0f);
		::glVertex2f(0.0f, 1.0f);
	}
	::glEnd();
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

