/*
===============================================================================

	FILE:  glowPanelWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Panel widget for GLOW
	
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

#ifndef GLOW_PANELWIDGET__H
	#include "glowPanelWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowPanelParams GlowPanelParams::defaults(true);

GlowPanelParams::GlowPanelParams()
{
	*this = defaults;
}

GlowPanelParams::GlowPanelParams(bool) :
GlowWidgetParams(true),
backColor(0.8f, 0.8f, 0.8f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.4f, 0.4f, 0.4f)
{
	clipping = true;
	width = 100;
	height = 100;
	style = GlowPanelWidget::plainStyle;
}


/*
===============================================================================
	Methods of GlowPanelWidget
===============================================================================
*/

void GlowPanelWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowPanelParams& params)
{
	GLOW_DEBUGSCOPE("GlowPanelWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	_style = params.style;
	_backColor = params.backColor;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
}


GlowWidget::AutoPackError GlowPanelWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowPanelWidget::OnAutoPack");
	
	int hnew = Width();
	if ((hOption == preferredSize || hOption == expandPreferredSize) &&
		hSize != unspecifiedSize && hnew > hSize)
	{
		return hAutoPackError;
	}
	else if (hOption == forcedSize)
	{
		if (hSize < 4)
		{
			return hAutoPackError;
		}
		hnew = hSize;
	}
	
	int vnew = Height();
	if ((vOption == preferredSize || vOption == expandPreferredSize) &&
		vSize != unspecifiedSize && vnew > vSize)
	{
		return vAutoPackError;
	}
	else if (vOption == forcedSize)
	{
		if (vSize < 4)
		{
			return vAutoPackError;
		}
		vnew = vSize;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowPanelWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowPanelWidget::OnWidgetPaint");
	
	if (_style == transparentStyle)
	{
		return;
	}
	
	_backColor.Apply();
	::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	
	float bevelHeight = float(4)/float(Height());
	float bevelWidth = float(4)/float(Width());
	float etchHeight = 2.1f/float(Height());
	float etchWidth = 2.1f/float(Width());
	
	if (_style == etchedStyle)
	{
		_lightBevelColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f+etchWidth, -1.0f);
		::glVertex2f(-1.0f+etchWidth, 1.0f-etchHeight-etchHeight);
		::glVertex2f(1.0f-etchWidth, 1.0f-etchHeight-etchHeight);
		::glVertex2f(1.0f-etchWidth, -1.0f);
		::glEnd();
		_darkBevelColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, -1.0f+etchHeight);
		::glVertex2f(-1.0f, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth-etchWidth, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth-etchWidth, -1.0f+etchHeight);
		::glEnd();
	}
	else if (_style == raisedStyle || _style == loweredStyle)
	{
		if (_style == raisedStyle)
		{
			_lightBevelColor.Apply();
		}
		else
		{
			_darkBevelColor.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(-1.0f, 1.0f);
		::glVertex2f(-1.0f+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();

		if (_style == raisedStyle)
		{
			_darkBevelColor.Apply();
		}
		else
		{
			_lightBevelColor.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(1.0f, -1.0f);
		::glVertex2f(1.0f-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

