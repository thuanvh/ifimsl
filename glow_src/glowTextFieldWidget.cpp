/*
===============================================================================

	FILE:  glowTextFieldWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Editable text field for GLOW
	
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
	
		The GLOW Toolkit -- version 1.0.4  (14 October 2000)
	
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

#ifndef GLOW_TEXTFIELDWIDGET__H
	#include "glowTextFieldWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowTextFieldParams GlowTextFieldParams::defaults(true);

GlowTextFieldParams::GlowTextFieldParams()
{
	*this = defaults;
}

GlowTextFieldParams::GlowTextFieldParams(bool) :
GlowWidgetParams(true),
backColor(1.0f, 1.0f, 1.0f),
textColor(0.0f, 0.0f, 0.0f),
caretColor(0.4f, 0.4f, 0.4f),
focusBackColor(1.0f, 1.0f, 1.0f),
focusTextColor(0.0f, 0.0f, 0.0f),
focusCaretColor(0.0f, 0.0f, 1.0f),
hiliteBackColor(1.0f, 0.8f, 0.8f),
hiliteTextColor(0.0f, 0.0f, 0.0f),
disableBackColor(0.8f, 0.8f, 0.8f),
disableTextColor(0.0f, 0.0f, 0.0f),
disableCaretColor(0.4f, 0.4f, 0.4f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.3f, 0.3f, 0.3f)
{
	width = 100;
	height = 20;
	style = GlowTextFieldWidget::loweredStyle;
	initialText = "";
	font = GLUT_BITMAP_HELVETICA_12;
	height = 24;
	selectionStart = 0;
	selectionEnd = 0;
	blinkInterval = 500;
	autoScrollInterval = 50;
	inset = 5;
	caretInset = 2;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabeledTextFieldParams GlowLabeledTextFieldParams::defaults(true);

GlowLabeledTextFieldParams::GlowLabeledTextFieldParams()
{
	*this = defaults;
}

GlowLabeledTextFieldParams::GlowLabeledTextFieldParams(bool) :
GlowTextFieldParams(true),
labelColor(0.0f, 0.0f, 0.0f),
disableLabelColor(0.3f, 0.3f, 0.3f)
{
	labelText = "";
	labelFont = GLUT_BITMAP_HELVETICA_12;
	labelPosition = GlowWidgetLabelHelper::defaultLabelPosition;
	labelWidth = 100;
	labelHeight = 20;
	labelSpacing = 5;
}


/*
===============================================================================
	Timer function
===============================================================================
*/

void Glow_TextField_BlinkTask::Task()
{
	_field->_blink = !_field->_blink;
	_field->Refresh();
	Schedule(_field->_blinkInterval);
}


/*
===============================================================================
	Autoscroll timer function
===============================================================================
*/

static int _autoScrollTimerID = 0;


class Glow_TextField_AutoScrollTimer :
	public GlowTimerReceiver
{
	public:
	
		inline Glow_TextField_AutoScrollTimer();
		inline void SetTextField(
			GlowTextFieldWidget* field);
		inline void ForceRemove(
			GlowTextFieldWidget* field);
	
	protected:
	
		virtual void OnMessage(
			const GlowTimerMessage& message);
	
	private:
	
		GlowTextFieldWidget* _field;
};


inline Glow_TextField_AutoScrollTimer::Glow_TextField_AutoScrollTimer()
{
	_field = 0;
}


inline void Glow_TextField_AutoScrollTimer::SetTextField(
	GlowTextFieldWidget* field)
{
	_field = field;
}


inline void Glow_TextField_AutoScrollTimer::ForceRemove(
	GlowTextFieldWidget* field)
{
	if (_field == field)
	{
		Glow::UnregisterTimer(_autoScrollTimerID);
	}
}


void Glow_TextField_AutoScrollTimer::OnMessage(
	const GlowTimerMessage& message)
{
	_field->_HandleAutoScrollTimer();
}


static Glow_TextField_AutoScrollTimer* _autoScrollTimer =
	new Glow_TextField_AutoScrollTimer;


/*
===============================================================================
	Methods of GlowTextFieldWidget
===============================================================================
*/

void GlowTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowTextFieldParams& params)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	_style = params.style;
	if (params.initialText != 0)
	{
		_data.assign(params.initialText);
	}
	_font = params.font;
	_hpos = 0;
	_dragStart = params.selectionStart;
	_dragEnd = params.selectionEnd;
	_data.SetSelection(_dragStart, _dragEnd);
	_blinkInterval = params.blinkInterval;
	_autoScrollInterval = params.autoScrollInterval;
	_inset = params.inset;
	_caretInset = params.caretInset;
	_backColor = params.backColor;
	_caretColor = params.caretColor;
	_textColor = params.textColor;
	_focusBackColor = params.focusBackColor;
	_focusTextColor = params.focusTextColor;
	_focusCaretColor = params.focusCaretColor;
	_hiliteBackColor = params.hiliteBackColor;
	_hiliteTextColor = params.hiliteTextColor;
	_disableBackColor = params.disableBackColor;
	_disableTextColor = params.disableTextColor;
	_disableCaretColor = params.disableCaretColor;
	_disableOutlineColor = params.disableOutlineColor;
	_lightBevelColor = params.lightBevelColor;
	_darkBevelColor = params.darkBevelColor;
	_blink = true;
	_toggleAutoScroll = false;
	_blinkTask.Init(this);
	
	RegisterMouseEvents();
	RegisterKeyboardEvents();
}


GlowTextFieldWidget::~GlowTextFieldWidget()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::~GlowTextFieldWidget");
	
	_autoScrollTimer->ForceRemove(this);
}


void GlowTextFieldWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetPaint");
	
	// Backing
	if (!IsActive())
	{
		_disableBackColor.Apply();
	}
	else if (HasKeyboardFocus())
	{
		_focusBackColor.Apply();
	}
	else
	{
		_backColor.Apply();
	}
	::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	
	// Bevels for backing
	float bevelHeight = float(4)/float(Height());
	float bevelWidth = float(4)/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	if (IsActive())
	{
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
	
	// Everything else gets scissored
	GLint oldScissor[4];
	::glGetIntegerv(GL_SCISSOR_BOX, oldScissor);
	::glScissor(RootPositionX()+_inset,
		Root()->Subwindow()->Height()-Height()-RootPositionY()+_inset,
		Width()-_inset-_inset, Height()-_inset-_inset);
	
	// Highlight backing
	if (_data.SelectionLength() > 0 && HasKeyboardFocus())
	{
		_hiliteBackColor.Apply();
		float left = 0;
		float top = 0;
		float right = 0;
		float bottom = 0;
		NormalizeCoordinates(_inset-_hpos+
			_data.PixelPosOf(_font, _data.SelectionStart(), 0),
			_inset, left, top);
		NormalizeCoordinates(_inset-_hpos+
			_data.PixelPosOf(_font, _data.SelectionEnd(), 0),
			_inset+_font.Leading(), right, bottom);
		::glRectf(left, bottom, right, top);
	}
	
	// Text
	if (!IsActive())
	{
		_disableTextColor.Apply();
	}
	else if (HasKeyboardFocus())
	{
		_focusTextColor.Apply();
	}
	else
	{
		_textColor.Apply();
	}
	int startpos = 0;
	int endpos = 0;
	int pixoffset = 0;
	_data.CalcLineDrawInfo(_font, 0, _hpos, Width()-_inset-_inset,
		startpos, endpos, pixoffset);
	const char* dat = _data.data();
	while (pixoffset > _inset)
	{
		pixoffset -= ::glutBitmapWidth(_font, dat[startpos]);
		++startpos;
	}
	float x = 0;
	float y = 0;
	NormalizeCoordinates(_inset-pixoffset,
		_inset+_font.BaselinePos(), x, y);
	::glRasterPos2f(x, y);
	for (int i=startpos; i<endpos; i++)
	{
		::glutBitmapCharacter(_font, dat[i]);
	}
	
	// Insertion caret
	// Special scissoring
	if (_blink || !HasKeyboardFocus() || !IsActive())
	{
		::glScissor(RootPositionX()+_caretInset,
			Root()->Subwindow()->Height()-Height()-RootPositionY()+_caretInset,
			Width()-_caretInset-_caretInset, Height()-_caretInset-_caretInset);
		if (!IsActive())
		{
			_disableCaretColor.Apply();
		}
		else if (HasKeyboardFocus())
		{
			_focusCaretColor.Apply();
		}
		else
		{
			_caretColor.Apply();
		}
		float horiz = 0;
		float top = 0;
		float bottom = 0;
		NormalizeCoordinates(_inset-_hpos+_data.PixelPosOf(_font, _dragEnd, 0),
			_inset, horiz, top);
		NormalizeCoordinates(_inset-_hpos+_data.PixelPosOf(_font, _dragEnd, 0),
			_inset+_font.Leading(), horiz, bottom);
		float hBump = float(2)/float(Width());
		float vBump = float(2)/float(Height());
		::glBegin(GL_LINES);
		::glVertex2f(horiz, bottom);
		::glVertex2f(horiz, top-vBump);
		::glVertex2f(horiz-bevelWidth, bottom);
		::glVertex2f(horiz+bevelWidth+hBump, bottom);
		::glVertex2f(horiz-bevelWidth, top-vBump);
		::glVertex2f(horiz+bevelWidth+hBump, top-vBump);
		::glEnd();
	}
	
	// Unscissor
	::glScissor(oldScissor[0], oldScissor[1], oldScissor[2], oldScissor[3]);
	
	// Activate autoscroll timer
	if (_toggleAutoScroll)
	{
		_toggleAutoScroll = false;
		_autoScrollTimer->SetTextField(this);
		_autoScrollTimerID = Glow::RegisterTimer(_autoScrollInterval, _autoScrollTimer);
	}
}


void GlowTextFieldWidget::_HandleAutoScrollTimer()
{
	_dragEnd = _data.AtPixelPos(_font, 0, _dragX-_inset+_hpos);
	_data.SetSelection(_dragStart, _dragEnd);
	_CheckAutoScroll();
	_toggleAutoScroll = true;
	Refresh();
}


void GlowTextFieldWidget::OnGotKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnGotKeyboardFocus");
	
	_blinkTask.Schedule(_blinkInterval);
}


void GlowTextFieldWidget::OnLostKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnLostKeyboardFocus");
	
	_blinkTask.Unschedule();
}


void GlowTextFieldWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetMouseDown");
	
	if (!HasKeyboardFocus())
	{
		GrabKeyboardFocus();
	}
	_dragX = x;
	if (modifiers & Glow::shiftModifier)
	{
		_dragEnd = _data.AtPixelPos(_font, 0, x-_inset+_hpos);
		_data.SetSelection(_dragStart, _dragEnd);
	}
	else
	{
		_dragStart = _dragEnd = _data.AtPixelPos(_font, 0, x-_inset+_hpos);
		_data.SetSelection(_dragStart);
	}
	_CheckAutoScroll();
	_toggleAutoScroll = true;
	Refresh();
}


void GlowTextFieldWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetMouseUp");
	
	_dragEnd = _data.AtPixelPos(_font, 0, x-_inset+_hpos);
	_data.SetSelection(_dragStart, _dragEnd);
	_CheckAutoScroll();
	Glow::UnregisterTimer(_autoScrollTimerID);
	_toggleAutoScroll = false;
	Refresh();
}


void GlowTextFieldWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetMouseDrag");
	
	_dragX = x;
	_dragEnd = _data.AtPixelPos(_font, 0, x-_inset+_hpos);
	_data.SetSelection(_dragStart, _dragEnd);
	Refresh();
}


void GlowTextFieldWidget::OnWidgetKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnWidgetKeyboard");
	
	if (key >= Glow::specialKeyOffset)
	{
		if (key == Glow::leftArrowKey && _dragEnd > 0)
		{
			if (modifiers & Glow::shiftModifier)
			{
				--_dragEnd;
				_data.SetSelection(_dragStart, _dragEnd);
			}
			else
			{
				_dragStart = _dragEnd = _dragEnd-1;
				_data.SetSelection(_dragStart);
			}
			_CheckAutoScroll();
			Refresh();
		}
		else if (key == Glow::rightArrowKey && _dragEnd < int(_data.size()))
		{
			if (modifiers & Glow::shiftModifier)
			{
				++_dragEnd;
				_data.SetSelection(_dragStart, _dragEnd);
			}
			else
			{
				_dragStart = _dragEnd = _dragEnd+1;
				_data.SetSelection(_dragStart);
			}
			_CheckAutoScroll();
			Refresh();
		}
	}
	else if (key == Glow::backspaceKey || key == Glow::deleteKey)
	{
		if (_data.SelectionLength() > 0)
		{
			_data.DeleteSelection();
			_dragStart = _dragEnd = _data.SelectionStart();
			_CheckAutoScroll();
			Refresh();
		}
		else if (_data.SelectionStart() > 0)
		{
			_dragStart = _dragEnd = _data.SelectionStart()-1;
			_data.SetSelection(_dragStart);
			_data.erase(_dragStart, 1);
			_CheckAutoScroll();
			Refresh();
		}
	}
	else if (key == Glow::tabKey)
	{
		RelinquishKeyboardFocus();
		Refresh();
	}
	else if (key >= 32)
	{
		_data.ReplaceSelectionWith((unsigned char)key);
		_data.SetSelection(_data.SelectionEnd(), _data.SelectionEnd());
		_dragStart = _dragEnd = _data.SelectionEnd();
		_CheckAutoScroll();
		Refresh();
	}
}


GlowWidget::AutoPackError GlowTextFieldWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowTextFieldWidget::OnAutoPack");
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < hnew)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	if (hnew < ::glutBitmapWidth(_font, '0')+_inset+_inset)
	{
		return hAutoPackError;
	}
	
	int vnew = Height();
	int preferred = _font.Leading()+_inset+_inset;
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
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowTextFieldWidget::_CheckAutoScroll()
{
	int pos = _data.PixelPosOf(_font, _dragEnd, 0);
	if (pos < _hpos)
	{
		_hpos = pos;
	}
	else if (pos > _hpos+Width()-_inset-_inset)
	{
		_hpos = pos-Width()+_inset+_inset;
	}
}


/*
===============================================================================
	Methods of GlowHiddenTextFieldWidget
===============================================================================
*/

void GlowHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	GLOW_DEBUGSCOPE("GlowHiddenTextFieldWidget::Init");
	
	GlowTextFieldWidget::Init(root, parent, params);
	
	if (params.initialText != 0)
	{
		_hiddenData.assign(params.initialText);
		_data.assign(_hiddenData.size(), hideCharacter);
	}
	
	_hideCharacter = hideCharacter;
}


void GlowHiddenTextFieldWidget::SetHideCharacter(
	char hc)
{
	if (hc != _hideCharacter)
	{
		_hideCharacter = hc;
		_data.assign(_data.size(), hc);
		_CheckAutoScroll();
		Refresh();
	}
}


void GlowHiddenTextFieldWidget::OnWidgetKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowHiddenTextFieldWidget::OnWidgetKeyboard");
	
	if (key >= 32 && key < Glow::specialKeyOffset)
	{
		_hiddenData.replace(_data.SelectionStart(),
			_data.SelectionEnd()-_data.SelectionStart(), 1, (unsigned char)key);
		GlowTextFieldWidget::OnWidgetKeyboard(Glow::KeyCode(_hideCharacter), x, y, modifiers);
	}
	else
	{
		GlowTextFieldWidget::OnWidgetKeyboard(key, x, y, modifiers);
	}
}


/*
===============================================================================
	Methods of GlowLabeledTextFieldWidget
===============================================================================
*/

void GlowLabeledTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabeledTextFieldWidget::Init");
	
	GlowTextFieldWidget::Init(root, parent, params);
	InitLabel(this, params.labelPosition, params.labelWidth,
		params.labelHeight, params.labelSpacing, params.labelText,
		params.labelFont, params.labelColor, params.disableLabelColor);
}


GlowWidget::AutoPackError GlowLabeledTextFieldWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledTextFieldWidget::OnAutoPack");
	
	// Use helper
	AutoPackError result = HelpAutoPack(hSize, vSize, leftMargin, rightMargin,
		topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	result = GlowTextFieldWidget::OnAutoPack(hSize, vSize, hOption,
		vOption, leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	RepositionLabel();
	return noAutoPackError;
}


/*
===============================================================================
	Methods of GlowLabeledHiddenTextFieldWidget
===============================================================================
*/

void GlowLabeledHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	GLOW_DEBUGSCOPE("GlowLabeledHiddenTextFieldWidget::Init");
	
	GlowHiddenTextFieldWidget::Init(root, parent, params, hideCharacter);
	InitLabel(this, params.labelPosition, params.labelWidth,
		params.labelHeight, params.labelSpacing, params.labelText,
		params.labelFont, params.labelColor, params.disableLabelColor);
}


GlowWidget::AutoPackError GlowLabeledHiddenTextFieldWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledHiddenTextFieldWidget::OnAutoPack");
	
	// Use helper
	AutoPackError result = HelpAutoPack(hSize, vSize, leftMargin, rightMargin,
		topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	result = GlowTextFieldWidget::OnAutoPack(hSize, vSize, hOption,
		vOption, leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	RepositionLabel();
	return noAutoPackError;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

