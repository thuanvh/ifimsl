/*
===============================================================================

	FILE:  glowCheckBoxWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Check box widget for GLOW
	
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

#ifndef GLOW_CHECKBOXWIDGET__H
	#include "glowCheckBoxWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowCheckBoxParams GlowCheckBoxParams::defaults(true);

GlowCheckBoxParams::GlowCheckBoxParams()
{
	*this = defaults;
}

GlowCheckBoxParams::GlowCheckBoxParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
checkColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.4f, 0.4f, 0.4f),
hiliteTextColor(0.8f, 0.0f, 0.0f),
hiliteCheckColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableCheckColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	width = 100;
	height = 16;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	spacing = 5;
	state = GlowCheckBoxWidget::off;
	behavior = GlowCheckBoxWidget::defaultBehavior;
	receiver = 0;
}


/*
===============================================================================
	Methods of GlowCheckBoxWidget
===============================================================================
*/

void GlowCheckBoxWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowCheckBoxParams& params)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	_label = new char[GLOW_CSTD::strlen(params.text)+1];
	GLOW_CSTD::strcpy(_label, params.text);
	_font = params.font;
	_labelWidth = _font.StringWidth(_label);
	_spacing = params.spacing;
	_state = params.state;
	_behavior = params.behavior;
	_down = false;
	_inside = false;
	_boxColor = params.boxColor;
	_textColor = params.textColor;
	_checkColor = params.checkColor;
	_hiliteBoxColor = params.hiliteBoxColor;
	_hiliteTextColor = params.hiliteTextColor;
	_hiliteCheckColor = params.hiliteCheckColor;
	_disableBoxColor = params.disableBoxColor;
	_disableTextColor = params.disableTextColor;
	_disableCheckColor = params.disableCheckColor;
	_disableOutlineColor = params.disableOutlineColor;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
	RegisterMouseEvents();
}


GlowCheckBoxWidget::~GlowCheckBoxWidget()
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::~GlowCheckBoxWidget");
	
	delete[] _label;
}


void GlowCheckBoxWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::SetText");
	
	delete[] _label;
	_label = new char[GLOW_CSTD::strlen(text)+1];
	GLOW_CSTD::strcpy(_label, text);
	_labelWidth = _font.StringWidth(_label);
	Refresh();
}


GlowWidget::AutoPackError GlowCheckBoxWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnAutoPack");
	
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


void GlowCheckBoxWidget::CropWidth()
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::CropWidth");
	
	int width = _labelWidth + _spacing + Height() + 1;
	if (width < Width())
	{
		Reshape(width, Height());
	}
}


void GlowCheckBoxWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetPaint");
	
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
	if (_state != off)
	{
		if (IsActive())
		{
			if (_down && _inside)
			{
				_hiliteCheckColor.Apply();
			}
			else
			{
				_checkColor.Apply();
			}
		}
		else
		{
			_disableCheckColor.Apply();
		}
		
		::glBegin(GL_QUADS);
		float hscale = (boxRight+1.0f)/2.0f;
		if (_state == on)
		{
			const float markOffset = 0.1f;
			
			::glVertex2f((0.5f-markOffset)*hscale-1.0f, -0.5f+markOffset);
			::glVertex2f((0.5f+markOffset)*hscale-1.0f, -0.5f-markOffset);
			::glVertex2f((1.5f+markOffset)*hscale-1.0f, 0.5f-markOffset);
			::glVertex2f((1.5f-markOffset)*hscale-1.0f, 0.5f+markOffset);
			
			::glVertex2f((0.5f+markOffset)*hscale-1.0f, 0.5f+markOffset);
			::glVertex2f((0.5f-markOffset)*hscale-1.0f, 0.5f-markOffset);
			::glVertex2f((1.5f-markOffset)*hscale-1.0f, -0.5f-markOffset);
			::glVertex2f((1.5f+markOffset)*hscale-1.0f, -0.5f+markOffset);
		}
		else //if (_state == half)
		{
			const float markOffset = 0.2;
			
			::glVertex2f(0.5f*hscale-1.0f, markOffset);
			::glVertex2f(0.5f*hscale-1.0f, -markOffset);
			::glVertex2f(1.5f*hscale-1.0f, -markOffset);
			::glVertex2f(1.5f*hscale-1.0f, markOffset);
		}
		::glEnd();
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


void GlowCheckBoxWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetMouseDown");
	
	_down = true;
	_inside = true;
	_button = button;
	_modifiers = modifiers;
	Refresh();
}


void GlowCheckBoxWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetMouseUp");
	
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


void GlowCheckBoxWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnWidgetMouseDrag");
	
	bool inside = (x>=0 && y>=0 && x<=Width() && y<=Height());
	if (inside == !_inside)
	{
		Refresh();
	}
	_inside = inside;
}


void GlowCheckBoxWidget::ToggleState()
{
	if (_state == off)
	{
		_state = ((_behavior & halfFollowsOff) != 0) ? half : on;
	}
	else if (_state == on)
	{
		_state = ((_behavior & halfFollowsOn) != 0) ? half : off;
	}
	else //if (_state == half)
	{
		_state = ((_behavior & offFollowsHalf) != 0) ? off : on;
	}
}


void GlowCheckBoxWidget::OnHit(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowCheckBoxWidget::OnHit");
	
	ToggleState();
	GlowCheckBoxMessage msg;
	msg.widget = this;
	msg.state = _state;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	_sender.Send(msg);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

