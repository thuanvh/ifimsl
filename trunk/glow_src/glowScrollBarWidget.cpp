/*
===============================================================================

	FILE:  glowScrollBarWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Scroll bar widget for GLOW
	
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

#ifndef GLOW_SCROLLBARWIDGET__H
	#include "glowScrollBarWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowScrollBarParams GlowScrollBarParams::defaults(true);

GlowScrollBarParams::GlowScrollBarParams()
{
	*this = defaults;
}

GlowScrollBarParams::GlowScrollBarParams(bool) :
GlowWidgetParams(true),
stripColor(0.5f, 0.5f, 0.5f),
indicatorColor(0.7f, 0.7f, 0.7f),
shadowColor(0.5f, 0.5f, 0.5f),
buttonColor(0.7f, 0.7f, 0.7f),
buttonIconColor(0.0f, 0.0f, 1.0f),
hiliteStripColor(0.6f, 0.5f, 0.5f),
hiliteIndicatorColor(0.8f, 0.6f, 0.6f),
hiliteButtonColor(0.8f, 0.6f, 0.6f),
hiliteButtonIconColor(1.0f, 0.0f, 0.0f),
disableStripColor(0.8f, 0.8f, 0.8f),
disableIndicatorColor(0.7f, 0.7f, 0.7f),
disableButtonColor(0.7f, 0.7f, 0.7f),
disableButtonIconColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.3f, 0.3f, 0.3f)
{
	width = 120;
	height = 20;
	min = 0;
	max = 1;
	span = 1;
	arrowStep = 1;
	pageStep = GlowScrollBarWidget::spanPageStep;
	initialTop = 0;
	firstDelay = 400;
	secondDelay = 50;
	receiver = 0;
}


/*
===============================================================================
	Timer function
===============================================================================
*/

class Glow_ScrollBar_TimerFunc :
	public GlowTimerReceiver
{
	public:
	
		inline Glow_ScrollBar_TimerFunc();
		
		inline void Register(
			int delay,
			GlowScrollBarWidget* bar);
		inline void ForceRemove(
			GlowScrollBarWidget* bar);
		inline void Unregister();
	
	private:
	
		static int _timerID;
	
	protected:
	
		virtual void OnMessage(
			const GlowTimerMessage& message);
	
	private:
	
		GlowScrollBarWidget* _scrollBar;
};


inline Glow_ScrollBar_TimerFunc::Glow_ScrollBar_TimerFunc()
{
	_scrollBar = 0;
}


inline void Glow_ScrollBar_TimerFunc::Register(
	int delay,
	GlowScrollBarWidget* bar)
{
	_timerID = Glow::RegisterTimer(delay, this);
	_scrollBar = bar;
}


inline void Glow_ScrollBar_TimerFunc::ForceRemove(
	GlowScrollBarWidget* bar)
{
	if (_scrollBar == bar)
	{
		Glow::UnregisterTimer(_timerID);
	}
}


inline void Glow_ScrollBar_TimerFunc::Unregister()
{
	Glow::UnregisterTimer(_timerID);
}


void Glow_ScrollBar_TimerFunc::OnMessage(
	const GlowTimerMessage& message)
{
	_scrollBar->_HandlePartPeriodical(false);
}


/*
===============================================================================
	Static globals
===============================================================================
*/

static Glow_ScrollBar_TimerFunc* _timer = new Glow_ScrollBar_TimerFunc;

int Glow_ScrollBar_TimerFunc::_timerID = 0;


/*
===============================================================================
	Methods of GlowScrollBar
===============================================================================
*/

void GlowScrollBarWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowScrollBarParams& params)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::Init");
	
	GLOW_DEBUG(params.max < params.min + params.span,
		"max <= min while constructing GlowScrollBar");
	GlowWidget::Init(root, parent, params);
	_min = params.min;
	_max = params.max;
	_topValue = params.initialTop;
	_span = params.span;
	if (_topValue > _max-_span) _topValue = _max-_span;
	if (_topValue < _min) _topValue = _min;
	_firstDelay = params.firstDelay;
	_secondDelay = params.secondDelay;
	_toggleTimer = 0;
	_arrowStep = params.arrowStep;
	_pageStep = params.pageStep;
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
	_stripColor = params.stripColor;
	_indicatorColor = params.indicatorColor;
	_shadowColor = params.shadowColor;
	_buttonColor = params.buttonColor;
	_buttonIconColor = params.buttonIconColor;
	_hiliteStripColor = params.hiliteStripColor;
	_hiliteIndicatorColor = params.hiliteIndicatorColor;
	_hiliteButtonColor = params.hiliteButtonColor;
	_hiliteButtonIconColor = params.hiliteButtonIconColor;
	_disableStripColor = params.disableStripColor;
	_disableIndicatorColor = params.disableIndicatorColor;
	_disableButtonColor = params.disableButtonColor;
	_disableButtonIconColor = params.disableButtonIconColor;
	_disableOutlineColor = params.disableOutlineColor;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
	_curPart = noPart;
	
	RegisterMouseEvents();
}


GlowScrollBarWidget::~GlowScrollBarWidget()
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::~GlowScrollBarWidget");
	
	_timer->ForceRemove(this);
}


GlowWidget::AutoPackError GlowScrollBarWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnAutoPack");
	
	if (hSize != unspecifiedSize && hSize < 16)
	{
		return hAutoPackError;
	}
	if (vSize != unspecifiedSize && vSize < 16)
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
			vnew = 20;
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
			hnew = 20;
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


void GlowScrollBarWidget::_DrawArrowButton(
	Part part,
	float left,
	float right,
	float bevelWidth,
	float bevelHeight,
	float buttonWidth)
{
	// Quad
	if (!IsActive())
	{
		_disableButtonColor.Apply();
	}
	else if (_curPart == part)
	{
		_hiliteButtonColor.Apply();
	}
	else
	{
		_buttonColor.Apply();
	}
	::glRectf(left, -1.0f, right, 1.0f);
	
	if (IsActive())
	{
		// Left/top bevels
		if (_curPart == part)
		{
			_darkBevelColor.Apply();
		}
		else
		{
			_lightBevelColor.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(left, -1.0f);
		::glVertex2f(left+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(left, 1.0f);
		::glVertex2f(left+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(right, 1.0f);
		::glVertex2f(right-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		
		// Right/bottom bevels
		if (_curPart == part)
		{
			_lightBevelColor.Apply();
		}
		else
		{
			_darkBevelColor.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(right, 1.0f);
		::glVertex2f(right-bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(right, -1.0f);
		::glVertex2f(right-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(left, -1.0f);
		::glVertex2f(left+bevelWidth, -1.0f+bevelHeight);
		::glEnd();
	}
	else
	{
		_disableOutlineColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(right, 1.0f);
		::glVertex2f(right, -1.0f);
		::glVertex2f(left, -1.0f);
		::glVertex2f(left, 1.0f);
		::glEnd();
	}
	
	// Arrow
	if (!IsActive())
	{
		_disableButtonIconColor.Apply();
	}
	else if (_curPart == part)
	{
		_hiliteButtonIconColor.Apply();
	}
	else
	{
		_buttonIconColor.Apply();
	}
	if (part == upButtonPart)
	{
		::glBegin(GL_TRIANGLES);
		::glVertex2f(left+buttonWidth*0.3f, 0.0f);
		::glVertex2f(left+buttonWidth*0.6f, -0.6f);
		::glVertex2f(left+buttonWidth*0.6f, 0.6f);
		::glEnd();
	}
	else
	{
		::glBegin(GL_TRIANGLES);
		::glVertex2f(left+buttonWidth*0.7f, 0.0f);
		::glVertex2f(left+buttonWidth*0.4f, -0.6f);
		::glVertex2f(left+buttonWidth*0.4f, 0.6f);
		::glEnd();
	}
}


// Compute normalized coordinates for parts.
void GlowScrollBarWidget::_ComputePartPositions(
	float& pseudoWidth,
	float& pseudoHeight,
	int& arrowButtons,
	float& buttonWidth,
	float& stripLeft,
	float& stripRight,
	float& indicLeft,
	float& indicRight)
{
	if (Width() < Height())
	{
		pseudoWidth = Height();
		pseudoHeight = Width();
	}
	else
	{
		pseudoWidth = Width();
		pseudoHeight = Height();
	}
	
	arrowButtons = 0;
	if (pseudoWidth > pseudoHeight*8.0f)
	{
		arrowButtons = 2;
	}
	else if (pseudoWidth > pseudoHeight*4.0f)
	{
		arrowButtons = 1;
	}
	
	buttonWidth = pseudoHeight/pseudoWidth*2.0f;
	stripLeft = -1.0f+buttonWidth*arrowButtons;
	stripRight = 1.0f-buttonWidth*arrowButtons;
	
	indicLeft = stripLeft + (stripRight-stripLeft)/float(_max-_min)*float(_topValue-_min);
	float indicWidth = (stripRight-stripLeft)/float(_max-_min)*float(_span);
	if (indicWidth < buttonWidth)
	{
		indicWidth = buttonWidth;
		indicLeft = stripLeft + (stripRight-stripLeft-indicWidth)/float(_max-_min-_span)*float(_topValue-_min);
	}
	indicRight = indicLeft + indicWidth;
}


// Compute normalized position from a mouse position.
// left=-1, right=1, top=-1, bottom=1
// Returns true iff inside the widget
bool GlowScrollBarWidget::_NormalizePosition(
	int x,
	int y,
	float& pos)
{
	if (Width() > Height())
	{
		pos = float(x)*2.0f/float(Width())-1.0f;
		return (pos <= 1.0f && pos >= -1.0f && y <= Height() && y >= 0);
	}
	else
	{
		pos = float(y)*2.0f/float(Height())-1.0f;
		return (pos <= 1.0f && pos >= -1.0f && x <= Width() && x >= 0);
	}
}


// When dragging indicator, update control value according to mouse position.
// Return true iff value changed
bool GlowScrollBarWidget::_UpdateDragValue(
	int x,
	int y)
{
	// Find values
	float pseudoWidth = 0.0f;
	float pseudoHeight = 0.0f;
	int arrowButtons = 0;
	float buttonWidth = 0.0f;
	float stripLeft = 0.0f;
	float stripRight = 0.0f;
	float indicLeft = 0.0f;
	float indicRight = 0.0f;
	_ComputePartPositions(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);
	
	// Interpret position
	float pos = 0.0f;
	_NormalizePosition(x, y, pos);
	float value = float(pos-_posOffset-stripLeft)*float(_max-_span-_min)/
		float(stripRight-stripLeft-(indicRight-indicLeft));
	int newtop = _min + static_cast<int>(GLOW_CSTD::floor(value+0.5f));
	if (newtop < _min) newtop = _min;
	if (newtop > _max-_span) newtop = _max-_span;
	if (newtop != _topValue)
	{
		_topValue = newtop;
		return true;
	}
	return false;
}


// When dragging in button parts or page parts, determine if we're
// still in the part, and update _inside. Return true iff change.
bool GlowScrollBarWidget::_UpdateInsideState(
	int x,
	int y)
{
	// Find values
	float pseudoWidth = 0;
	float pseudoHeight = 0;
	int arrowButtons = 0;
	float buttonWidth = 0;
	float stripLeft = 0;
	float stripRight = 0;
	float indicLeft = 0;
	float indicRight = 0;
	_ComputePartPositions(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);
	
	// Interpret position
	float pos = 0;
	bool ninside = false;
	if (_NormalizePosition(x, y, pos))
	{
		_savePosition = pos;
		if (_curPart == upPagePart)
		{
			ninside = (pos >= stripLeft && pos <= indicLeft);
		}
		else if (_curPart == downPagePart)
		{
			ninside = (pos >= indicRight && pos <= stripRight);
		}
		else if (_curPart == upButtonPart && !_innerButton)
		{
			ninside = (pos >= -1.0f && pos <= buttonWidth-1.0f);
		}
		else if (_curPart == downButtonPart && !_innerButton)
		{
			ninside = (pos >= 1.0f-buttonWidth && pos <= 1.0f);
		}
		else if (_curPart == downButtonPart && _innerButton)
		{
			ninside = (pos >= buttonWidth-1.0f && pos <= buttonWidth*2.0f-1.0f);
		}
		else if (_curPart == upButtonPart && _innerButton)
		{
			ninside = (pos >= 1.0f-buttonWidth*2.0f && pos <= 1.0f-buttonWidth);
		}
		else
		{
			ninside = false;
		}
	}
	
	// Check for changes
	if (ninside && !_inside)
	{
		_inside = true;
		_toggleTimer = 1;
		return true;
	}
	else if (!ninside && _inside)
	{
		_inside = false;
		_timer->Unregister();
		return true;
	}
	return false;
}


void GlowScrollBarWidget::_HandlePartPeriodical(
	bool first)
{
	// Handle part stuff
	OnPart(_curPart, _button, _modifiers);
	Refresh();
	
	if ((_curPart == upPagePart || _curPart == downPagePart) && _inside)
	{
		// Check to make sure we're still in page region
		float pseudoWidth = 0;
		float pseudoHeight = 0;
		int arrowButtons = 0;
		float buttonWidth = 0;
		float stripLeft = 0;
		float stripRight = 0;
		float indicLeft = 0;
		float indicRight = 0;
		_ComputePartPositions(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
			stripLeft, stripRight, indicLeft, indicRight);
		
		if (_curPart == upPagePart)
		{
			if (_savePosition < stripLeft || _savePosition > indicLeft)
			{
				_inside = false;
				_toggleTimer = 0;
			}
		}
		else //if (_curPart == downPagePart)
		{
			if (_savePosition < indicRight || _savePosition > stripRight)
			{
				_inside = false;
				_toggleTimer = 0;
			}
		}
	}
	
	if (_inside)
	{
		if (first)
		{
			_toggleTimer = _firstDelay;
		}
		else
		{
			_toggleTimer = _secondDelay;
		}
	}
}


void GlowScrollBarWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetPaint");
	
	// Find values
	float pseudoWidth = 0;
	float pseudoHeight = 0;
	int arrowButtons = 0;
	float buttonWidth = 0;
	float stripLeft = 0;
	float stripRight = 0;
	float indicLeft = 0;
	float indicRight = 0;
	_ComputePartPositions(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);
	GLfloat bevelHeight = 4.0f/pseudoHeight;
	GLfloat bevelWidth = 4.0f/pseudoWidth;
	
	// Apply transform if upright scroll bar
	if (Width() < Height())
	{
		::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		::glScalef(1.0f, -1.0f, 1.0f);
	}
	
	// Draw strip back
	if (!IsActive())
	{
		_disableStripColor.Apply();
	}
	else if (_curPart == upPagePart)
	{
		_hiliteStripColor.Apply();
	}
	else
	{
		_stripColor.Apply();
	}
	::glRectf(-1.0f, -1.0f, indicLeft, 1.0f);
	if (IsActive())
	{
		if (_curPart == downPagePart)
		{
			_hiliteStripColor.Apply();
		}
		else
		{
			_stripColor.Apply();
		}
	}
	::glRectf(indicRight, -1.0f, 1.0f, 1.0f);
	if (!IsActive())
	{
		_disableOutlineColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, 1.0f);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(1.0f, -1.0f);
		::glVertex2f(1.0f, 1.0f);
		::glEnd();
	}
	
	// Draw shadow
	if (_curPart == indicatorPart)
	{
		_shadowColor.Apply();
		::glRectf(_savePosition, -1.0f,
			_savePosition+indicRight-indicLeft, 1.0f);
		
		// Top/left bevels
		_darkBevelColor.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(_savePosition+indicRight-indicLeft, 1.0f);
		::glVertex2f(_savePosition+indicRight-indicLeft-bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(_savePosition, 1.0f);
		::glVertex2f(_savePosition+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(_savePosition, -1.0f);
		::glVertex2f(_savePosition+bevelWidth, -1.0f+bevelHeight);
		::glEnd();
		
		// Bottom/right bevel
		_lightBevelColor.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(_savePosition, -1.0f);
		::glVertex2f(_savePosition+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(_savePosition+indicRight-indicLeft, -1.0f);
		::glVertex2f(_savePosition+indicRight-indicLeft-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(_savePosition+indicRight-indicLeft, 1.0f);
		::glVertex2f(_savePosition+indicRight-indicLeft-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
		
	// Draw strip bevels
/*	if (IsActive())
	{
		// Top bevel
		_darkBevelColor.Apply();
		::glBegin(GL_QUADS);
		::glVertex2f(stripLeft-bevelWidth, 1.0f);
		::glVertex2f(stripLeft, 1.0-bevelHeight);
		::glVertex2f(stripRight, 1.0-bevelHeight);
		::glVertex2f(stripRight+bevelWidth, 1.0f);
		::glEnd();
		
		// Bottom bevel
		_lightBevelColor.Apply();
		::glBegin(GL_QUADS);
		::glVertex2f(stripLeft-bevelWidth, -1.0f);
		::glVertex2f(stripLeft, -1.0+bevelHeight);
		::glVertex2f(stripRight, -1.0+bevelHeight);
		::glVertex2f(stripRight+bevelWidth, -1.0f);
		::glEnd();
	}*/
	
	// Draw buttons
	if (arrowButtons != 0)
	{
		// Left up button
		_DrawArrowButton(upButtonPart, -1.0f, -1.0f+buttonWidth,
			bevelWidth, bevelHeight, buttonWidth);
		// Right down button
		_DrawArrowButton(downButtonPart, 1.0f-buttonWidth, 1.0f,
			bevelWidth, bevelHeight, buttonWidth);
	}
	if (arrowButtons == 2)
	{
		// Left down button
		_DrawArrowButton(downButtonPart, -1.0f+buttonWidth, -1.0f+buttonWidth*2.0f,
			bevelWidth, bevelHeight, buttonWidth);
		// Right up button
		_DrawArrowButton(upButtonPart, 1.0f-buttonWidth*2.0f, 1.0f-buttonWidth,
			bevelWidth, bevelHeight, buttonWidth);
	}
	
	// Draw indicator
	if (!IsActive())
	{
		_disableIndicatorColor.Apply();
	}
	else if (_curPart == indicatorPart)
	{
		_hiliteIndicatorColor.Apply();
	}
	else
	{
		_indicatorColor.Apply();
	}
	::glRectf(indicLeft, -1.0f, indicRight, 1.0f);
		
	if (IsActive())
	{
		// Top bevel
		_lightBevelColor.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(indicRight, 1.0f);
		::glVertex2f(indicRight-bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(indicLeft, 1.0f);
		::glVertex2f(indicLeft+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(indicLeft, -1.0f);
		::glVertex2f(indicLeft+bevelWidth, -1.0f+bevelHeight);
		::glEnd();
		
		// Bottom bevel
		_darkBevelColor.Apply();
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(indicLeft, -1.0f);
		::glVertex2f(indicLeft+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(indicRight, -1.0f);
		::glVertex2f(indicRight-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(indicRight, 1.0f);
		::glVertex2f(indicRight-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		
		// Ridges
		float ridgeTop = 1.0f-bevelHeight*2.0f;
		float ridgeBottom = -1.0f+bevelHeight*3.0f;
		float indicCenter = (indicRight+indicLeft)*0.5f;
		_lightBevelColor.Apply();
		::glBegin(GL_LINES);
		::glVertex2f(indicCenter-float(9)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter-float(9)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter-float(1)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter-float(1)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter+float(7)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter+float(7)/pseudoWidth, ridgeTop);
		::glEnd();
		_darkBevelColor.Apply();
		::glBegin(GL_LINES);
		::glVertex2f(indicCenter-float(7)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter-float(7)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter+float(1)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter+float(1)/pseudoWidth, ridgeTop);
		::glVertex2f(indicCenter+float(9)/pseudoWidth, ridgeBottom);
		::glVertex2f(indicCenter+float(9)/pseudoWidth, ridgeTop);
		::glEnd();
	}
	else
	{
		_disableOutlineColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(indicLeft, 1.0f);
		::glVertex2f(indicLeft, -1.0f);
		::glVertex2f(indicRight, -1.0f);
		::glVertex2f(indicRight, 1.0f);
		::glEnd();
	}
	
	// If a timer start is scheduled, then start it
	if (_toggleTimer > 0)
	{
		_timer->Register(_toggleTimer, this);
		_toggleTimer = 0;
	}
}


void GlowScrollBarWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetMouseDown");
	
	if (_curPart != noPart)
	{
		return;
	}
	
	// Find values
	float pseudoWidth = 0;
	float pseudoHeight = 0;
	int arrowButtons = 0;
	float buttonWidth = 0;
	float stripLeft = 0;
	float stripRight = 0;
	float indicLeft = 0;
	float indicRight = 0;
	_ComputePartPositions(pseudoWidth, pseudoHeight, arrowButtons, buttonWidth,
		stripLeft, stripRight, indicLeft, indicRight);

	// Interpret position
	float pos = 0;
	_NormalizePosition(x, y, pos);
	_savePosition = pos;
	_button = button;
	_modifiers = modifiers;
	_inside = true;
	if (arrowButtons > 0)
	{
		if (pos < buttonWidth-1.0f)
		{
			_curPart = upButtonPart;
			_innerButton = false;
			_HandlePartPeriodical(true);
		}
		else if (pos > 1.0f-buttonWidth)
		{
			_curPart = downButtonPart;
			_innerButton = false;
			_HandlePartPeriodical(true);
		}
		else if (arrowButtons == 2)
		{
			if (pos < buttonWidth*2.0f-1.0f)
			{
				_curPart = downButtonPart;
				_innerButton = true;
				_HandlePartPeriodical(true);
			}
			else if (pos > 1.0f-buttonWidth*2.0f)
			{
				_curPart = upButtonPart;
				_innerButton = true;
				_HandlePartPeriodical(true);
			}
		}
	}
	if (_curPart == noPart)
	{
		if (pos < indicLeft)
		{
			_curPart = upPagePart;
			_HandlePartPeriodical(true);
		}
		else if (pos > indicRight)
		{
			_curPart = downPagePart;
			_HandlePartPeriodical(true);
		}
		else
		{
			_curPart = indicatorPart;
			_posOffset = pos-indicLeft;
			_savePosition = indicLeft;
			Refresh();
		}
	}
}


void GlowScrollBarWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetMouseUp");
	
	Part savePart = _curPart;
	if (_curPart != noPart)
	{
		if (_curPart == indicatorPart)
		{
			_UpdateDragValue(x, y);
		}
		else
		{
			_timer->Unregister();
		}
		_curPart = noPart;
		_inside = false;
		OnReleased(savePart, _button, _modifiers);
		Refresh();
	}
}


void GlowScrollBarWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnWidgetMouseDrag");
	
	if (_curPart != noPart)
	{
		if (_curPart == indicatorPart)
		{
			// Dragging indicator
			if (_UpdateDragValue(x, y))
			{
				OnDragged(_button, _modifiers);
				Refresh();
			}
		}
		else
		{
			// Dragging button or page
			if (_UpdateInsideState(x, y))
			{
				Refresh();
			}
		}
	}
}


void GlowScrollBarWidget::OnDragged(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnDragged");
	
	GlowScrollBarMessage msg;
	msg.widget = this;
	msg.part = indicatorPart;
	msg.released = false;
	msg.topValue = _topValue;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	_sender.Send(msg);
}


void GlowScrollBarWidget::OnPart(
	Part part,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnPart");
	
	switch (part)
	{
		case upButtonPart:
			MoveTopValue(-_arrowStep);
			break;
		
		case downButtonPart:
			MoveTopValue(_arrowStep);
			break;
		
		case upPagePart:
			if (_pageStep == spanPageStep)
			{
				MoveTopValue(-_span);
			}
			else
			{
				MoveTopValue(-_pageStep);
			}
			break;
		
		case downPagePart:
			if (_pageStep == spanPageStep)
			{
				MoveTopValue(_span);
			}
			else
			{
				MoveTopValue(_pageStep);
			}
			break;
		
		default:
			return;
	}
	
	GlowScrollBarMessage msg;
	msg.widget = this;
	msg.part = part;
	msg.released = false;
	msg.topValue = _topValue;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	_sender.Send(msg);
}


void GlowScrollBarWidget::OnReleased(
	Part part,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowScrollBarWidget::OnReleased");
	
	GlowScrollBarMessage msg;
	msg.widget = this;
	msg.part = part;
	msg.released = true;
	msg.topValue = _topValue;
	msg.mouseButton = mouseButton;
	msg.modifiers = modifiers;
	_sender.Send(msg);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

