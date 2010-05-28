/*
===============================================================================

	FILE:  glowPushButtonWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Pushbutton widget for GLOW
	
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

#ifndef GLOW_PUSHBUTTONWIDGET__H
	#include "glowPushButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowPushButtonParams GlowPushButtonParams::defaults(true);

GlowPushButtonParams::GlowPushButtonParams()
{
	*this = defaults;
}

GlowPushButtonParams::GlowPushButtonParams(bool) :
GlowWidgetParams(true),
upBoxColor(0.7f, 0.7f, 0.7f),
upTextColor(0.0f, 0.0f, 0.0f),
downBoxColor(0.5f, 0.5f, 0.5f),
downTextColor(0.8f, 0.8f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(1.0f, 0.7f, 0.7f),
disableUpBoxColor(0.7f, 0.7f, 0.7f),
disableDownBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	width = 100;
	height = 25;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	receiver = 0;
	down = false;
	behavior = GlowPushButtonWidget::normalBehavior;
}


/*
===============================================================================
	Methods of GlowPushButton
===============================================================================
*/

void GlowPushButtonWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowPushButtonParams& params)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	_upBoxColor = params.upBoxColor;
	_upTextColor = params.upTextColor;
	_downBoxColor = params.downBoxColor;
	_downTextColor = params.downTextColor;
	_hiliteBoxColor = params.hiliteBoxColor;
	_hiliteTextColor = params.hiliteTextColor;
	_disableUpBoxColor = params.disableUpBoxColor;
	_disableDownBoxColor = params.disableDownBoxColor;
	_disableTextColor = params.disableTextColor;
	_disableOutlineColor = params.disableOutlineColor;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
	_label = new char[GLOW_CSTD::strlen(params.text)+1];
	GLOW_CSTD::strcpy(_label, params.text);
	_font = params.font;
	_labelWidth = _font.StringWidth(_label);
	_down = false;
	_inside = false;
	_state = params.down;
	_behavior = params.behavior;
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
	RegisterMouseEvents();
}


GlowPushButtonWidget::~GlowPushButtonWidget()
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::~GlowPushButtonWidget");
	
	delete[] _label;
}


void GlowPushButtonWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::SetText");
	
	delete[] _label;
	_label = new char[GLOW_CSTD::strlen(text)+1];
	GLOW_CSTD::strcpy(_label, text);
	_labelWidth = _font.StringWidth(_label);
	Refresh();
}


GlowWidget::AutoPackError GlowPushButtonWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnAutoPack");
	
	int hnew = Width();
	int preferred = _labelWidth + 20;
	if (hSize != unspecifiedSize && hSize < preferred-10)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = preferred;
	}
	
	int vnew = Height();
	preferred = _font.Leading() + 12;
	if (vSize != unspecifiedSize && vSize < preferred-4)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == preferredSize || vOption == expandPreferredSize)
	{
		vnew = preferred;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowPushButtonWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetPaint");
	
	float bevelHeight = float(4)/float(Height());
	float bevelWidth = float(4)/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	
	// Box
	if (_down && _inside)
	{
		_hiliteBoxColor.Apply();
	}
	else if (!IsActive())
	{
		if (_state)
		{
			_disableDownBoxColor.Apply();
		}
		else
		{
			_disableUpBoxColor.Apply();
		}
	}
	else
	{
		if (_state)
		{
			_downBoxColor.Apply();
		}
		else
		{
			_upBoxColor.Apply();
		}
	}
	::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	
	// Bevels
	if (IsActive())
	{
		if (_down && _inside)
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
		::glVertex2f(-1.0f, 1.0f);
		::glVertex2f(-1.0f+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		if (_down && _inside)
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
		::glVertex2f(1.0f, -1.0f);
		::glVertex2f(1.0f-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
	else
	{
		_disableOutlineColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, -1.0f);
		::glEnd();
	}
	
	// Text label
	if (IsActive())
	{
		if (_down && _inside)
		{
			_hiliteTextColor.Apply();
		}
		else if (_state)
		{
			_downTextColor.Apply();
		}
		else
		{
			_upTextColor.Apply();
		}
	}
	else
	{
		_disableTextColor.Apply();
	}
	::glRasterPos2f(-float(_labelWidth)/float(Width()),
		float(_font.Leading()-_font.BaselinePos()*2)/float(Height()));
	int labellen = GLOW_CSTD::strlen(_label);
	for (int i=0; i<labellen; i++)
	{
		::glutBitmapCharacter(_font, _label[i]);
	}
}


void GlowPushButtonWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetMouseDown");
	
	_down = true;
	_inside = true;
	_button = button;
	_modifiers = modifiers;
	Refresh();
}


void GlowPushButtonWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetMouseUp");
	
	if (_down)
	{
		_down = false;
		Refresh();
		if (_inside)
		{
			OnHit(_button, _modifiers);
		}
	}
}


void GlowPushButtonWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnWidgetMouseDrag");
	
	bool inside = (x>=0 && y>=0 && x<=Width() && y<=Height());
	if (inside == !_inside)
	{
		Refresh();
	}
	_inside = inside;
}


void GlowPushButtonWidget::ToggleState()
{
	if (_behavior == toggleBehavior)
	{
		_state = !_state;
	}
	else if (_behavior == stickDownBehavior)
	{
		_state = true;
	}
}


void GlowPushButtonWidget::OnHit(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowPushButtonWidget::OnHit");
	
	ToggleState();
	GlowPushButtonMessage msg;
	msg.widget = this;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	_sender.Send(msg);
}


/*
===============================================================================
	Methods of GlowDismissPushButtonWidget
===============================================================================
*/

void GlowDismissPushButtonWidget::OnHit(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowDismissPushButtonWidget::OnHit");
	
	GlowPushButtonWidget::OnHit(button, modifiers);
	if (_todismiss)
	{
		_todismiss->Close();
	}
}


/*
===============================================================================
	Methods of GlowWidgetMapToPushButtonFilter
===============================================================================
*/

bool GlowWidgetMapToPushButtonFilter::OnFilter(
	GlowWidgetKeyboardData& data)
{
	GLOW_DEBUGSCOPE("GlowWidgetMapToPushButtonFilter::OnFilter");
	
	if (_widget != 0 && _keyCode == data.key &&
		(_modifiers & data.modifiers) == _modifiers)
	{
		_widget->Hit();
		return false;
	}
	return true;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

