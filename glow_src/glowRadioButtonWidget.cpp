/*
===============================================================================

	FILE:  glowRadioButtonWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Radio button widget for GLOW
	
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

#ifndef GLOW_RADIOBUTTONWIDGET__H
	#include "glowRadioButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowRadioButtonParams GlowRadioButtonParams::defaults(true);

GlowRadioButtonParams::GlowRadioButtonParams()
{
	*this = defaults;
}

GlowRadioButtonParams::GlowRadioButtonParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
spotColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(0.8f, 0.0f, 0.0f),
hiliteSpotColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableSpotColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	width = 100;
	height = 16;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	spacing = 5;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowRadioGroupParams GlowRadioGroupParams::defaults(true);

GlowRadioGroupParams::GlowRadioGroupParams()
{
	*this = defaults;
}

GlowRadioGroupParams::GlowRadioGroupParams(bool) :
GlowPanelParams(true)
{
	style = GlowRadioGroupWidget::transparentStyle;
	clipping = false;
	receiver = 0;
}


/*
===============================================================================
	Methods of GlowRadioButton
===============================================================================
*/

void GlowRadioButtonWidget::Init(
	GlowRadioGroupWidget* parent,
	const GlowRadioButtonParams& params)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::Init");
	
	GlowWidget::Init(parent->Root(), parent, params);
	_group = parent;
	_label = new char[GLOW_CSTD::strlen(params.text)+1];
	GLOW_CSTD::strcpy(_label, params.text);
	_font = params.font;
	_labelWidth = _font.StringWidth(_label);
	_spacing = params.spacing;
	_down = false;
	_inside = false;
	_boxColor = params.boxColor;
	_textColor = params.textColor;
	_spotColor = params.spotColor;
	_hiliteBoxColor = params.hiliteBoxColor;
	_hiliteTextColor = params.hiliteTextColor;
	_hiliteSpotColor = params.hiliteSpotColor;
	_disableBoxColor = params.disableBoxColor;
	_disableTextColor = params.disableTextColor;
	_disableSpotColor = params.disableSpotColor;
	_disableOutlineColor = params.disableOutlineColor;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
	_state = _group->_Register(this);
	
	RegisterMouseEvents();
}


GlowRadioButtonWidget::~GlowRadioButtonWidget()
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::~GlowRadioButtonWidget");
	
	_group->_UnRegister(this);
	delete[] _label;
}


void GlowRadioButtonWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::SetText");
	
	delete[] _label;
	_label = new char[GLOW_CSTD::strlen(text)+1];
	GLOW_CSTD::strcpy(_label, text);
	_labelWidth = _font.StringWidth(_label);
	Refresh();
}


GlowWidget::AutoPackError GlowRadioButtonWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnAutoPack");
	
	int vnew = Height();
	int preferred = _font.Leading() + 1;
	if (vSize != unspecifiedSize && vSize < preferred)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == expandPreferredSize || vOption == preferredSize)
	{
		vnew = preferred;
	}
	
	int hnew = Width();
	preferred = _labelWidth+_spacing+vnew+1;
	if (hSize != unspecifiedSize && hSize < preferred)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize || hOption == expandPreferredSize)
	{
		hnew = preferred;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowRadioButtonWidget::CropWidth()
{
	int width = _labelWidth + _spacing + Height() + 1;
	if (width < Width())
	{
		Reshape(width, Height());
	}
}


void GlowRadioButtonWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetPaint");
	
	float boxRight = float(Height())/float(Width())*2.0f-1.0f;
	float bevelHeight = 4.0f/float(Height());
	float bevelWidth = 4.0f/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	
	// Draw box
	if (IsActive())
	{
		if (_down && _inside)
		{
			_hiliteBoxColor.Apply();
		}
		else
		{
			_boxColor.Apply();
		}
	}
	else
	{
		_disableBoxColor.Apply();
	}
	::glRectf(-1.0f, -1.0f, boxRight, 1.0f);
	
	// Draw bevels
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
		::glVertex2f(boxRight, 1.0f);
		::glVertex2f(boxRight-bevelWidth, 1.0f-bevelHeight);
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
		::glVertex2f(boxRight, -1.0f);
		::glVertex2f(boxRight-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(boxRight, 1.0f);
		::glVertex2f(boxRight-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
	else
	{
		_disableOutlineColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f, 1.0f-etchHeight);
		::glVertex2f(boxRight-etchWidth, 1.0f-etchHeight);
		::glVertex2f(boxRight-etchWidth, -1.0f);
		::glEnd();
	}
	
	// Draw mark
	if (_state)
	{
		if (IsActive())
		{
			if (_down && _inside)
			{
				_hiliteSpotColor.Apply();
			}
			else
			{
				_spotColor.Apply();
			}
		}
		else
		{
			_disableSpotColor.Apply();
		}
		float spotInset = float(Height())/float(Width())*0.5f;
		::glRectf(-1.0f+spotInset, -0.5f, boxRight-spotInset, 0.5f);
	}
	
	// Text label
	if (IsActive())
	{
		if (_down && _inside)
		{
			_hiliteTextColor.Apply();
		}
		else
		{
			_textColor.Apply();
		}
	}
	else
	{
		_disableTextColor.Apply();
	}
	::glRasterPos2f(boxRight+float(_spacing*2)/float(Width()),
		float(_font.Leading()-_font.BaselinePos()*2)/float(Height()));
	int labellen = GLOW_CSTD::strlen(_label);
	for (int i=0; i<labellen; i++)
	{
		::glutBitmapCharacter(_font, _label[i]);
	}
}


void GlowRadioButtonWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetMouseDown");
	
	_down = true;
	_inside = true;
	_button = button;
	_modifiers = modifiers;
	Refresh();
}


void GlowRadioButtonWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetMouseUp");
	
	if (_down)
	{
		_down = false;
		Refresh();
		if (_inside)
		{
			_group->OnHit(this, _button, _modifiers);
		}
	}
}


void GlowRadioButtonWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowRadioButtonWidget::OnWidgetMouseDrag");
	
	bool inside = (x>=0 && y>=0 && x<=Width() && y<=Height());
	if (inside == !_inside)
	{
		Refresh();
	}
	_inside = inside;
}


/*
===============================================================================
	Methods of GlowRadioGroupWidget
===============================================================================
*/

void GlowRadioGroupWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowRadioGroupParams& params)
{
	GLOW_DEBUGSCOPE("GlowRadioGroupWidget::Init");
	
	GlowPanelWidget::Init(root, parent, params);
	_state = 0;
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
}


void GlowRadioGroupWidget::OnHit(
	GlowRadioButtonWidget* widget,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowRadioGroupWidget::OnHit");
	
	if (widget != _state)
	{
		GlowRadioButtonMessage message;
		message.groupWidget = this;
		message.buttonWidget = widget;
		message.oldButtonWidget = _state;
		message.mouseButton = mouseButton;
		message.modifiers = modifiers;
		_state = widget;
		message.oldButtonWidget->SetState(false);
		message.buttonWidget->SetState(true);
		_sender.Send(message);
	}
}


void GlowRadioGroupWidget::_UnRegister(
	GlowRadioButtonWidget* button)
{
	if (_state == button)
	{
		for (GlowComponent* child = FirstChild(); true; child = child->Next())
		{
			if (child == 0)
			{
				_state = 0;
				break;
			}
			if (child != button)
			{
				GlowRadioButtonWidget* newbutton =
					dynamic_cast<GlowRadioButtonWidget*>(child);
				if (newbutton != 0)
				{
					_state = newbutton;
					_state->SetState(true);
					break;
				}
			}
		}
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

