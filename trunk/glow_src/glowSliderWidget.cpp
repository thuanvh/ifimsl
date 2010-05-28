/*
===============================================================================

	FILE:  glowSliderWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Slider widget for GLOW
	
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

#include <cmath>
#include <cstdio>

#ifndef GLOW_SLIDERWIDGET__H
	#include "glowSliderWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowSliderParams GlowSliderParams::defaults(true);

GlowSliderParams::GlowSliderParams()
{
	*this = defaults;
}

GlowSliderParams::GlowSliderParams(bool) :
GlowWidgetParams(true),
stripColor(0.5f, 0.5f, 0.5f),
indicatorColor(0.7f, 0.7f, 0.7f),
tickMarkColor(0.0f, 0.0f, 0.0f),
hiliteIndicatorColor(0.8f, 0.6f, 0.6f),
disableStripColor(0.7f, 0.7f, 0.7f),
disableIndicatorColor(0.7f, 0.7f, 0.7f),
disableTickMarkColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.3f, 0.3f, 0.3f)
{
	width = 130;
	height = 30;
	options = GlowSliderWidget::defaultOptions;
	min = 0.0f;
	max = 1.0f;
	initial = 0.0f;
	numTicks = 0;
	receiver = 0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabeledSliderParams GlowLabeledSliderParams::defaults(true);

GlowLabeledSliderParams::GlowLabeledSliderParams()
{
	*this = defaults;
}

GlowLabeledSliderParams::GlowLabeledSliderParams(bool) :
GlowSliderParams(true),
labelColor(0.0f, 0.0f, 0.0f),
disableLabelColor(0.3f, 0.3f, 0.3f),
minmaxColor(0.0f, 0.0f, 0.0f),
disableMinmaxColor(0.3f, 0.3f, 0.3f)
{
	labelTemplate = 0;
	labelFont = GLUT_BITMAP_HELVETICA_12;
	labelPosition = GlowLabeledSliderWidget::leftLabelPosition;
	labelWidth = 100;
	labelHeight = 15;
	labelSpacing = 5;
	minmaxTemplate = 0;
	minmaxFont = GLUT_BITMAP_HELVETICA_12;
	minmaxSize = 15;
}


/*
===============================================================================
	Methods of GlowSliderWidget
===============================================================================
*/

void GlowSliderWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowSliderParams& params)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::Init");
	
	GLOW_DEBUG(params.max < params.min, "max < min while constructing GlowSlider");
	GlowWidget::Init(root, parent, params);
	_dragging = false;
	_type = params.options;
	_min = params.min;
	_max = params.max;
	_value = params.initial;
	if (_value > _max) _value = _max;
	if (_value < _min) _value = _min;
	_numTicks = params.numTicks;
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
	
	_stripColor = params.stripColor;
	_indicatorColor = params.indicatorColor;
	_tickMarkColor = params.tickMarkColor;
	_hiliteIndicatorColor = params.hiliteIndicatorColor;
	_disableStripColor = params.disableStripColor;
	_disableIndicatorColor = params.disableIndicatorColor;
	_disableTickMarkColor = params.disableTickMarkColor;
	_disableOutlineColor = params.disableOutlineColor;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
	
	RegisterMouseEvents();
}


GlowWidget::AutoPackError GlowSliderWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnAutoPack");
	
	int pwidth = 30, pheight = 30;
	if (Width() > Height())
	{
		pwidth = 130;
	}
	else
	{
		pheight = 130;
	}
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == expandPreferredSize || hOption == forcedSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	
	int vnew = Height();
	if (vSize != unspecifiedSize && vSize < pheight)
	{
		return vAutoPackError;
	}
	if (vOption == expandPreferredSize || vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == preferredSize)
	{
		vnew = pheight;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowSliderWidget::_CalcValue(
	int x,
	int y)
{
	if (Width() > Height())
	{
		_value = float(x-Height()/2)/float(Width()-Height());
	}
	else
	{
		_value = float(y-Width()/2)/float(Height()-Width());
	}
	if (_value > 1.0f) _value = 1.0f;
	if (_value < 0.0f) _value = 0.0f;
	if ((_type & logarithmic) != 0)
	{
		if ((_type & decreasing) != 0)
		{
			_value = GLOW_CSTD::pow(_max, 1.0f-_value)*GLOW_CSTD::pow(_min, _value);
		}
		else
		{
			_value = GLOW_CSTD::pow(_min, 1.0f-_value)*GLOW_CSTD::pow(_max, _value);
		}
	}
	else // linear
	{
		if ((_type & decreasing) != 0)
		{
			_value = _max*(1.0f-_value)+_min*_value;
		}
		else
		{
			_value = _min*(1.0f-_value)+_max*_value;
		}
	}
}


void GlowSliderWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetPaint");
	
	float pseudoWidth;
	float pseudoHeight;
	if (Width() < Height())
	{
		if ((_type & ticksOnTop) != 0)
		{
			::glScalef(-1.0f, 1.0f, 1.0f);
		}
		if ((_type & decreasing) == 0)
		{
			::glScalef(1.0f, -1.0f, 1.0f);
		}
		::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		pseudoWidth = Height();
		pseudoHeight = Width();
	}
	else
	{
		if ((_type & ticksOnTop) != 0)
		{
			::glScalef(1.0f, -1.0f, 1.0f);
		}
		if ((_type & decreasing) != 0)
		{
			::glScalef(-1.0f, 1.0f, 1.0f);
		}
		pseudoWidth = Width();
		pseudoHeight = Height();
	}
	
	GLfloat bevelHeight = float(4)/pseudoHeight;
	GLfloat bevelWidth = float(4)/pseudoWidth;
	GLfloat pixelHeight = float(2)/pseudoHeight;
	GLfloat pixelWidth = float(2)/pseudoWidth;
	
	// Draw strip
	float top, bottom;
	if (_numTicks == 0)
	{
		top = 0.25f;
		bottom = -0.25f;
	}
	else
	{
		top = 0.4f;
		bottom = -0.1f;
	}
	
	// Strip back
	if (!IsActive())
	{
		_disableStripColor.Apply();
	}
	else
	{
		_stripColor.Apply();
	}
	::glRectf(-1.0f, bottom, 1.0f, top);
	
	if (IsActive())
	{
		// Left bevel
		_RightBevelColor();
		::glBegin(GL_POLYGON);
		::glVertex2f(-1.0f, bottom);
		::glVertex2f(-1.0f+bevelWidth, bottom+bevelHeight);
		::glVertex2f(-1.0f+bevelWidth, top-bevelHeight);
		::glVertex2f(-1.0f, top);
		::glEnd();
		
		// Top bevel
		_BottomBevelColor();
		::glBegin(GL_POLYGON);
		::glVertex2f(-1.0f, top);
		::glVertex2f(-1.0f+bevelWidth, top-bevelHeight);
		::glVertex2f(1.0f-bevelWidth, top-bevelHeight);
		::glVertex2f(1.0f, top);
		::glEnd();
		
		// Right bevel
		_LeftBevelColor();
		::glBegin(GL_POLYGON);
		::glVertex2f(1.0f, bottom);
		::glVertex2f(1.0f-bevelWidth, bottom+bevelHeight);
		::glVertex2f(1.0f-bevelWidth, top-bevelHeight);
		::glVertex2f(1.0f, top);
		::glEnd();
		
		// Bottom bevel
		_TopBevelColor();
		::glBegin(GL_POLYGON);
		::glVertex2f(-1.0f, bottom);
		::glVertex2f(-1.0f+bevelWidth, bottom+bevelHeight);
		::glVertex2f(1.0f-bevelWidth, bottom+bevelHeight);
		::glVertex2f(1.0f, bottom);
		::glEnd();
	}
	else
	{
		_disableOutlineColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, bottom);
		::glVertex2f(-1.0f, top);
		::glVertex2f(1.0f, top);
		::glVertex2f(1.0f, bottom);
		::glEnd();
	}
	
	// Draw tickmarks if necessary
	float indicRadius = pseudoHeight/pseudoWidth;
	if (!IsActive())
	{
		_disableTickMarkColor.Apply();
	}
	else
	{
		_tickMarkColor.Apply();
	}
	::glLineWidth(2);
	if (_numTicks == 1)
	{
		::glBegin(GL_LINES);
		::glVertex2f(0.0f, -0.5f);
		::glVertex2f(0.0f, -1.0f);
		::glEnd();
	}
	else if (_numTicks > 1)
	{
		for (int i=0; i<_numTicks; i++)
		{
			float pos = float(i)/float(_numTicks-1)*2.0f-1.0f;
			pos *= (1.0-indicRadius);
			::glBegin(GL_LINES);
			::glVertex2f(pos, -0.5f);
			::glVertex2f(pos, -1.0f);
			::glEnd();
		}
	}
	::glLineWidth(1);
	
	// Draw indicator
	if (_max > _min)
	{
		float indicPos;
		if ((_type & logarithmic) != 0)
		{
			indicPos = GLOW_CSTD::log(_value/_min)/GLOW_CSTD::log(_max/_min);
		}
		else
		{
			indicPos = (_value-_min)/(_max-_min);
		}
		indicPos = indicRadius-1.0f+indicPos*
			(pseudoWidth-pseudoHeight)/pseudoWidth*2.0f;
		if (_numTicks > 0)
		{
			// With ticks (pentagons)
			if (!IsActive())
			{
				_disableIndicatorColor.Apply();
			}
			else if (_dragging)
			{
				_hiliteIndicatorColor.Apply();
			}
			else
			{
				_indicatorColor.Apply();
			}
			::glBegin(GL_POLYGON);
			::glVertex2f(indicPos, -0.5f);
			::glVertex2f(indicPos-indicRadius, 0.0f);
			::glVertex2f(indicPos-indicRadius, 1.0f);
			::glVertex2f(indicPos+indicRadius, 1.0f);
			::glVertex2f(indicPos+indicRadius, 0.0f);
			::glEnd();
			
			if (IsActive())
			{
				// Bottom (crooked) bevel
				_BottomBevelColor();
				::glBegin(GL_QUAD_STRIP);
				::glVertex2f(indicPos-indicRadius, 0.0f);
				::glVertex2f(indicPos-indicRadius+bevelWidth, bevelHeight);
				::glVertex2f(indicPos, -0.5f);
				::glVertex2f(indicPos, -0.5f+6.0f/pseudoHeight);
				::glVertex2f(indicPos+indicRadius, 0.0f);
				::glVertex2f(indicPos+indicRadius-bevelWidth, bevelHeight);
				::glEnd();
				
				// Left bevel
				_LeftBevelColor();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, 0.0f);
				::glVertex2f(indicPos-indicRadius+bevelWidth, bevelHeight);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos-indicRadius, 1.0f);
				::glEnd();
				
				// Top bevel
				_TopBevelColor();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, 1.0f);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0f);
				::glEnd();
				
				// Right bevel
				_RightBevelColor();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos+indicRadius, 0.0f);
				::glVertex2f(indicPos+indicRadius-bevelWidth, bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0f-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0f);
				::glEnd();
				
				// Ridges
				float ridgeTop = 1.0f-bevelHeight*2.0f;
				float ridgeBottom = -0.5f+bevelHeight*3.0f;
				_LeftBevelColor();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeTop);
				::glEnd();
				_RightBevelColor();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeTop);
				::glEnd();
			}
			else
			{
				_disableOutlineColor.Apply();
				::glBegin(GL_LINE_LOOP);
				::glVertex2f(indicPos, -0.5f);
				::glVertex2f(indicPos-indicRadius+pixelWidth, 0.0f);
				::glVertex2f(indicPos-indicRadius+pixelWidth, 1.0f-pixelHeight);
				::glVertex2f(indicPos+indicRadius-pixelWidth, 1.0f-pixelHeight);
				::glVertex2f(indicPos+indicRadius-pixelWidth, 0.0f);
				::glEnd();
			}
		}
		else
		{
			// No ticks (rects)
			if (!IsActive())
			{
				_disableIndicatorColor.Apply();
			}
			else if (_dragging)
			{
				_hiliteIndicatorColor.Apply();
			}
			else
			{
				_indicatorColor.Apply();
			}
			::glRectf(indicPos-indicRadius, -1.0f, indicPos+indicRadius, 1.0f);
			
			if (IsActive())
			{
				// Bottom bevel
				_BottomBevelColor();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, -1.0);
				::glVertex2f(indicPos-indicRadius+bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos+indicRadius, -1.0);
				::glEnd();
				
				// Left bevel
				_LeftBevelColor();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, -1.0);
				::glVertex2f(indicPos-indicRadius+bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos-indicRadius, 1.0);
				::glEnd();
				
				// Top bevel
				_TopBevelColor();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos-indicRadius, 1.0);
				::glVertex2f(indicPos-indicRadius+bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0);
				::glEnd();
				
				// Right bevel
				_RightBevelColor();
				::glBegin(GL_QUADS);
				::glVertex2f(indicPos+indicRadius, -1.0);
				::glVertex2f(indicPos+indicRadius-bevelWidth, -1.0+bevelHeight);
				::glVertex2f(indicPos+indicRadius-bevelWidth, 1.0-bevelHeight);
				::glVertex2f(indicPos+indicRadius, 1.0);
				::glEnd();
				
				// Ridges
				float ridgeTop = 1.0-bevelHeight*2.0;
				float ridgeBottom = -1.0+bevelHeight*2.0;
				_LeftBevelColor();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(7)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(5)/pseudoWidth, ridgeTop);
				::glEnd();
				_RightBevelColor();
				::glBegin(GL_LINES);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos-float(5)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(1)/pseudoWidth, ridgeTop);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeBottom);
				::glVertex2f(indicPos+float(7)/pseudoWidth, ridgeTop);
				::glEnd();
			}
			else
			{
				_disableOutlineColor.Apply();
				::glBegin(GL_LINE_LOOP);
				::glVertex2f(indicPos-indicRadius, -1.0f);
				::glVertex2f(indicPos-indicRadius, 1.0f);
				::glVertex2f(indicPos+indicRadius, 1.0f);
				::glVertex2f(indicPos+indicRadius, -1.0f);
				::glEnd();
			}
		}
	}
}


void GlowSliderWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetMouseDown");
	
	if (_max > _min)
	{
		int indicRadius, indicPos;
		float valRatio;
		if ((_type & logarithmic) != 0)
		{
			valRatio = GLOW_CSTD::log(_value/_min)/GLOW_CSTD::log(_max/_min);
		}
		else
		{
			valRatio = (_value-_min)/(_max-_min);
		}
		if ((_type & decreasing) != 0)
		{
			valRatio = 1.0f-valRatio;
		}
		if (Width() > Height())
		{
			indicRadius = Height()/2;
			indicPos = indicRadius+static_cast<int>(float(Width()-2*indicRadius)*valRatio);
			if (x > indicPos-indicRadius && x < indicPos+indicRadius)
			{
				_xoffset = indicPos-x;
				_yoffset = 0;
				_dragging = true;
				_modifiers = modifiers;
				Refresh();
			}
		}
		else
		{
			indicRadius = Width()/2;
			indicPos = indicRadius+static_cast<int>(float(Height()-2*indicRadius)*valRatio);
			if (y > indicPos-indicRadius && y < indicPos+indicRadius)
			{
				_xoffset = 0;
				_yoffset = indicPos-y;
				_dragging = true;
				_modifiers = modifiers;
				Refresh();
			}
		}
	}
}


void GlowSliderWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetMouseUp");
	
	if (_dragging)
	{
		_dragging = false;
		_CalcValue(x+_xoffset, y+_yoffset);
		OnReleased(_button, _modifiers);
		Refresh();
	}
}


void GlowSliderWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnWidgetMouseDrag");
	
	if (_dragging)
	{
		_CalcValue(x+_xoffset, y+_yoffset);
		OnDragged(_button, _modifiers);
		Refresh();
	}
}


void GlowSliderWidget::OnDragged(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnDragged");
	
	GlowSliderMessage msg;
	msg.widget = this;
	msg.released = false;
	msg.value = _value;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	_sender.Send(msg);
}


void GlowSliderWidget::OnReleased(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSliderWidget::OnReleased");
	
	GlowSliderMessage msg;
	msg.widget = this;
	msg.released = true;
	msg.value = _value;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	_sender.Send(msg);
}


/*
===============================================================================
	Methods of GlowLabeledSliderWidget
===============================================================================
*/

void GlowLabeledSliderWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledSliderParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabeledSliderWidget::Init");
	
	GlowSliderWidget::Init(root, parent, params);
	
	// Main label
	_labelSpacing = params.labelSpacing;
	_labelPosition = params.labelPosition;
	_labelWidth = params.labelWidth;
	_labelHeight = params.labelHeight;
	_labelValue = GetValue();
	
	GlowLabelParams lparams;
	char buffer[1000];
	if (params.labelTemplate == 0)
	{
		_labelTemplate = 0;
		lparams.text = 0;
	}
	else
	{
		_labelTemplate = new char[GLOW_CSTD::strlen(params.labelTemplate)+1];
		GLOW_CSTD::strcpy(_labelTemplate, params.labelTemplate);
		GLOW_CSTD::sprintf(buffer, _labelTemplate, _labelValue);
		lparams.text = buffer;
	}
	lparams.font = params.labelFont;
	lparams.textColor = params.labelColor;
	lparams.disableTextColor = params.disableLabelColor;
	_label = new GlowWidgetLabelWidget(this, lparams);
	
	// Min and max labels
	_minLabelValue = GetMinimum();
	_maxLabelValue = GetMaximum();
	_minmaxSize = params.minmaxSize;
	
	lparams.font = params.minmaxFont;
	lparams.textColor = params.minmaxColor;
	lparams.disableTextColor = params.disableMinmaxColor;
	if (params.minmaxTemplate == 0)
	{
		_minmaxTemplate = 0;
		lparams.text = 0;
	}
	else
	{
		_minmaxTemplate = new char[GLOW_CSTD::strlen(params.minmaxTemplate)+1];
		GLOW_CSTD::strcpy(_minmaxTemplate, params.minmaxTemplate);
		GLOW_CSTD::sprintf(buffer, _minmaxTemplate, _minLabelValue);
		lparams.text = buffer;
	}
	_minLabel = new GlowWidgetLabelWidget(this, lparams);
	if (_minmaxTemplate != 0)
	{
		GLOW_CSTD::sprintf(buffer, _minmaxTemplate, _maxLabelValue);
		lparams.text = buffer;
	}
	_maxLabel = new GlowWidgetLabelWidget(this, lparams);
	
	ResizeMinmax();
	RepositionMinmax();
	RepositionLabel();
}


void GlowLabeledSliderWidget::RepositionLabel()
{
	// need to make sure label doesn't hit minmax
	int leftMinmax = 0, rightMinmax = 0, topMinmax = 0, bottomMinmax = 0;
	if (_minmaxTemplate != 0)
	{
		if (Width() > Height())
		{
			// Horizontal slider
			if (GetOptions() & ticksOnTop)
			{
				topMinmax = _minmaxSize;
			}
			else
			{
				bottomMinmax = _minmaxSize;
			}
		}
		else
		{
			// Vertical slider
			if (GetOptions() & ticksOnLeft)
			{
				leftMinmax = _minmaxSize;
			}
			else
			{
				rightMinmax = _minmaxSize;
			}
		}
	}
	switch (_labelPosition)
	{
		case leftLabelPosition:
			_label->AutoPack(
				-leftMinmax-_labelWidth-_labelSpacing,
				-leftMinmax-_labelSpacing,
				(Height()-_labelHeight)/2,
				(Height()+_labelHeight)/2,
				expandPreferredSize | rightPos, expandPreferredSize | centerPos);
			break;
		case rightLabelPosition:
			_label->AutoPack(
				rightMinmax+_labelSpacing,
				rightMinmax+_labelSpacing+_labelWidth,
				(Height()-_labelHeight)/2,
				(Height()+_labelHeight)/2,
				expandPreferredSize | leftPos, expandPreferredSize | centerPos);
			break;
		case topLabelPosition:
			_label->AutoPack(
				(Width()-_labelWidth)/2,
				(Width()+_labelWidth)/2,
				-topMinmax-_labelHeight-_labelSpacing,
				-topMinmax-_labelSpacing,
				expandPreferredSize | centerPos, expandPreferredSize | bottomPos);
			break;
		case bottomLabelPosition:
			_label->AutoPack(
				(Width()-_labelWidth)/2,
				(Width()+_labelWidth)/2,
				bottomMinmax+_labelSpacing,
				bottomMinmax+_labelSpacing+_labelHeight,
				expandPreferredSize | centerPos, expandPreferredSize | topPos);
			break;
	}
}


GlowWidget::AutoPackError GlowLabeledSliderWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledSliderWidget::OnAutoPack");
	
	// Deal with min and max labels
	CheckMinmax();
	ResizeMinmax();
	if (_minmaxTemplate != 0)
	{
		if (Width() > Height())
		{
			// Horizontal slider
			_minmaxSize = GLOW_STD::max(_minLabel->Height(), _maxLabel->Height());
			if (GetOptions() & ticksOnTop)
			{
				topMargin = _minmaxSize;
			}
			else
			{
				bottomMargin = _minmaxSize;
			}
		}
		else
		{
			// Vertical slider
			_minmaxSize = GLOW_STD::max(_minLabel->Width(), _maxLabel->Width());
			if (GetOptions() & ticksOnLeft)
			{
				leftMargin = _minmaxSize;
			}
			else
			{
				rightMargin = _minmaxSize;
			}
		}
	}
	
	// Deal with labels
	if (_labelTemplate != 0)
	{
		_label->FindPreferredSize(_labelWidth, _labelHeight);
		
		// Adjust size according to label
		if (hSize != unspecifiedSize)
		{
			if (_labelPosition == leftLabelPosition ||
				_labelPosition == rightLabelPosition)
			{
				hSize -= _labelWidth + _labelSpacing;
			}
			else if (_labelWidth > Width())
			{
				hSize -= _labelWidth - Width();
			}
			if (hSize < 2)
			{
				return hAutoPackError;
			}
		}
		if (vSize != unspecifiedSize)
		{
			if (_labelPosition == topLabelPosition ||
				_labelPosition == bottomLabelPosition)
			{
				vSize -= _labelHeight + _labelSpacing;
			}
			else if (_labelHeight > Height())
			{
				vSize -= _labelHeight - Height();
			}
			if (vSize < 2)
			{
				return vAutoPackError;
			}
		}
		
		// Adjust margins according to label
		switch (_labelPosition)
		{
			case leftLabelPosition:
				leftMargin += _labelWidth+_labelSpacing;
				break;
			case rightLabelPosition:
				rightMargin +=  _labelWidth+_labelSpacing;
				break;
			case topLabelPosition:
				topMargin +=  _labelHeight+_labelSpacing;
				break;
			case bottomLabelPosition:
				bottomMargin +=  _labelHeight+_labelSpacing;
				break;
		}
		if ((_labelPosition == leftLabelPosition || _labelPosition == rightLabelPosition) &&
			_labelHeight > Height())
		{
			topMargin = GLOW_STD::max(topMargin, (_labelHeight-Height())/2);
			bottomMargin = GLOW_STD::max(bottomMargin, (_labelHeight-Height())/2);
		}
		if ((_labelPosition == topLabelPosition || _labelPosition == bottomLabelPosition) &&
			_labelWidth > Width())
		{
			leftMargin = GLOW_STD::max(leftMargin, (_labelWidth-Width())/2);
			rightMargin = GLOW_STD::max(rightMargin, (_labelWidth-Width())/2);
		}
	}
	
	AutoPackError result = GlowSliderWidget::OnAutoPack(hSize, vSize, hOption,
		vOption, leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	// Position labels
	RepositionMinmax();
	RepositionLabel();
	return noAutoPackError;
}


void GlowLabeledSliderWidget::SetLabelTemplate(
	const char* text)
{
	delete _labelTemplate;
	if (text == 0)
	{
		_labelTemplate = 0;
		_label->SetText(0);
	}
	else
	{
		_labelTemplate = new char[GLOW_CSTD::strlen(text)+1];
		GLOW_CSTD::strcpy(_labelTemplate, text);
		_labelValue = GetValue();
		char buffer[1000];
		GLOW_CSTD::sprintf(buffer, _labelTemplate, _labelValue);
		_label->SetText(buffer);
	}
}


void GlowLabeledSliderWidget::SetMinmaxTemplate(
	const char* text)
{
	delete _minmaxTemplate;
	if (text == 0)
	{
		_minmaxTemplate = 0;
		_minLabel->SetText(0);
		_maxLabel->SetText(0);
	}
	else
	{
		_minmaxTemplate = new char[GLOW_CSTD::strlen(text)+1];
		GLOW_CSTD::strcpy(_minmaxTemplate, text);
		char buffer[1000];
		_minLabelValue = GetMinimum();
		GLOW_CSTD::sprintf(buffer, _minmaxTemplate, _minLabelValue);
		_minLabel->SetText(buffer);
		_maxLabelValue = GetMaximum();
		GLOW_CSTD::sprintf(buffer, _minmaxTemplate, _maxLabelValue);
		_maxLabel->SetText(buffer);
		ResizeMinmax();
		RepositionMinmax();
	}
}


void GlowLabeledSliderWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowLabeledSliderWidget::OnWidgetPaint");
	
	if (_labelValue != GetValue())
	{
		_labelValue = GetValue();
		if (_labelTemplate != 0)
		{
			char buffer[1000];
			GLOW_CSTD::sprintf(buffer, _labelTemplate, _labelValue);
			_label->SetRefreshEnabled(false);
			_label->SetText(buffer);
			_label->SetRefreshEnabled(true);
		}
	}
	if (CheckMinmax())
	{
		ResizeMinmax();
		RepositionMinmax();
	}
	GlowSliderWidget::OnWidgetPaint();
}


bool GlowLabeledSliderWidget::CheckMinmax()
{
	bool needReposition = false;
	if (_minLabelValue != GetMinimum())
	{
		_minLabelValue = GetMinimum();
		if (_minmaxTemplate != 0)
		{
			char buffer[1000];
			GLOW_CSTD::sprintf(buffer, _minmaxTemplate, _minLabelValue);
			_minLabel->SetRefreshEnabled(false);
			_minLabel->SetText(buffer);
			needReposition = true;
			_minLabel->SetRefreshEnabled(true);
		}
	}
	if (_maxLabelValue != GetMaximum())
	{
		_maxLabelValue = GetMaximum();
		if (_minmaxTemplate != 0)
		{
			char buffer[1000];
			GLOW_CSTD::sprintf(buffer, _minmaxTemplate, _maxLabelValue);
			_maxLabel->SetRefreshEnabled(false);
			_maxLabel->SetText(buffer);
			needReposition = true;
			_maxLabel->SetRefreshEnabled(true);
		}
	}
	return needReposition;
}


void GlowLabeledSliderWidget::ResizeMinmax()
{
	int pwidth = 0, pheight = 0;
	_minLabel->FindPreferredSize(pwidth, pheight);
	_minLabel->Reshape(pwidth, pheight);
	_maxLabel->FindPreferredSize(pwidth, pheight);
	_maxLabel->Reshape(pwidth, pheight);
}


void GlowLabeledSliderWidget::RepositionMinmax()
{
	if (Width() > Height())
	{
		// Horizontal slider
		int topLimit, bottomLimit;
		AutoPackOptions vOption;
		if (GetOptions() & ticksOnTop)
		{
			topLimit = -_minmaxSize;
			bottomLimit = 0;
			vOption = bottomPos;
		}
		else
		{
			topLimit = Height();
			bottomLimit = Height()+_minmaxSize;
			vOption = topPos;
		}
		int minLeftLimit, minRightLimit, maxLeftLimit, maxRightLimit;
		AutoPackOptions minHOption, maxHOption;
		if (GetOptions() & GlowSliderWidget::decreasing)
		{
			minLeftLimit = Width()-Height();
			minRightLimit = Width();
			maxLeftLimit = 0;
			maxRightLimit = Height();
			minHOption = (_minLabel->Width() <= Height()) ?
				centerPos : rightPos;
			maxHOption = (_maxLabel->Width() <= Height()) ?
				centerPos : leftPos;
		}
		else
		{
			minLeftLimit = 0;
			minRightLimit = Height();
			maxLeftLimit = Width()-Height();
			maxRightLimit = Width();
			minHOption = (_minLabel->Width() <= Height()) ?
				centerPos : leftPos;
			maxHOption = (_maxLabel->Width() <= Height()) ?
				centerPos : rightPos;
		}
		_minLabel->AutoPack(minLeftLimit, minRightLimit, topLimit, bottomLimit,
			minHOption, vOption);
		_maxLabel->AutoPack(maxLeftLimit, maxRightLimit, topLimit, bottomLimit,
			maxHOption, vOption);
	}
	else
	{
		// Vertical slider
		int leftLimit, rightLimit;
		AutoPackOptions hOption;
		if (GetOptions() & ticksOnLeft)
		{
			leftLimit = -_minmaxSize;
			rightLimit = 0;
			hOption = rightPos;
		}
		else
		{
			leftLimit = Width();
			rightLimit = Width()+_minmaxSize;
			hOption = leftPos;
		}
		int minTopLimit, minBottomLimit, maxTopLimit, maxBottomLimit;
		AutoPackOptions minVOption, maxVOption;
		if (GetOptions() & GlowSliderWidget::decreasing)
		{
			minTopLimit = Height()-Width();
			minBottomLimit = Height();
			maxTopLimit = 0;
			maxBottomLimit = Width();
			minVOption = (_minLabel->Height() <= Width()) ?
				centerPos : bottomPos;
			maxVOption = (_maxLabel->Height() <= Width()) ?
				centerPos : topPos;
		}
		else
		{
			minTopLimit = 0;
			minBottomLimit = Width();
			maxTopLimit = Height()-Width();
			maxBottomLimit = Height();
			minVOption = (_minLabel->Height() <= Width()) ?
				centerPos : topPos;
			maxVOption = (_maxLabel->Height() <= Width()) ?
				centerPos : bottomPos;
		}
		_minLabel->AutoPack(leftLimit, rightLimit, minTopLimit, minBottomLimit,
			hOption, minVOption);
		_maxLabel->AutoPack(leftLimit, rightLimit, maxTopLimit, maxBottomLimit,
			hOption, maxVOption);
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

