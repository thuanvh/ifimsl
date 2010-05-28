/*
===============================================================================

	FILE:  glowWidget.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Widget for GLOW
	
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

#include <iostream>

#ifndef GLOW_WIDGET__H
	#include "glowWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Static data
===============================================================================
*/

GLOW_STD::map<GlowWidget*, Glow_OldWidgetState> GlowWidget::_notifyList;


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWidgetParams::GlowWidgetParams(bool)
{
	clipping = false;
	x = 0;
	y = 0;
	width = 100;
	height = 100;
	refcon = 0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWidgetSubwindowParams GlowWidgetSubwindowParams::defaults(true);

GlowWidgetSubwindowParams::GlowWidgetSubwindowParams()
{
	*this = defaults;
}

GlowWidgetSubwindowParams::GlowWidgetSubwindowParams(bool) :
GlowSubwindowParams(true),
backColor(0.8f, 0.8f, 0.8f)
{
//	mode = Glow::rgbaBuffer | Glow::depthBuffer | 
//		Glow::stencilBuffer | Glow::doubleBuffer;
	eventMask = Glow::mouseEvents | Glow::dragEvents | Glow::keyboardEvents;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWidgetWindowParams GlowWidgetWindowParams::defaults(true);

GlowWidgetWindowParams::GlowWidgetWindowParams()
{
	*this = defaults;
}

GlowWidgetWindowParams::GlowWidgetWindowParams(bool) :
GlowWindowParams(true),
backColor(0.8f, 0.8f, 0.8f)
{
//	mode = Glow::rgbaBuffer | Glow::depthBuffer | 
//		Glow::stencilBuffer | Glow::doubleBuffer;
	eventMask = Glow::mouseEvents | Glow::dragEvents | Glow::keyboardEvents;
}


/*
===============================================================================
	Methods of GlowWidget
===============================================================================
*/

void GlowWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowWidgetParams& params)
{
	GLOW_DEBUGSCOPE("GlowWidget::Init");
	
	if (Glow::_widgetNotifier == 0)
	{
		Glow::_widgetNotifier = GlowWidget::_ExecuteNotify;
	}
	
	if (parent == 0)
	{
		GlowComponent::Init(root->Subwindow());
	}
	else
	{
		GlowComponent::Init(parent);
	}
	_refcon = params.refcon;
	_xpos = params.x;
	_ypos = params.y;
	_width = params.width;
	_height = params.height;
	_clipping = params.clipping;
	_parentWidget = parent;
	_root = root;
	_receivingMouse = false;
	_receivingKeyboard = false;
	_hasFocus = false;
	_refreshEnabled = true;
	if (parent == 0 || parent->IsVisible())
	{
		_visibility = 1;
	}
	else
	{
		_visibility = 2;
	}
	Refresh();
}


GlowWidget::~GlowWidget()
{
	GLOW_DEBUGSCOPE("GlowWidget::~GlowWidget");
	
	UnregisterMouseEvents();
	UnregisterKeyboardEvents();
	Refresh();
	
	GLOW_STD::map<GlowWidget*, Glow_OldWidgetState>::iterator iter =
		_notifyList.find(this);
	if (iter != _notifyList.end())
	{
		_notifyList.erase(iter);
	}
}


int GlowWidget::RootPositionX() const
{
	return _parentWidget == 0 ?
		_xpos : _parentWidget->RootPositionX() + _xpos;
}


int GlowWidget::RootPositionY() const
{
	return _parentWidget == 0 ?
		_ypos : _parentWidget->RootPositionY() + _ypos;
}


GlowWidget::AutoPackError GlowWidget::AutoPack(
	int leftLimit,
	int rightLimit,
	int topLimit,
	int bottomLimit,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowWidget::AutoPack");
	
	// Make sure positions are specified for justified moves
	GLOW_DEBUG(
		(hOption & posOptionMask) == leftPos && leftLimit == unspecifiedPos,
		"unspecified left position");
	GLOW_DEBUG(
		(hOption & posOptionMask) == rightPos && rightLimit == unspecifiedPos,
		"unspecified right position");
	GLOW_DEBUG(
		(hOption & posOptionMask) == centerPos && leftLimit == unspecifiedPos,
		"unspecified horizontal center position");
	GLOW_DEBUG(
		(vOption & posOptionMask) == topPos && topLimit == unspecifiedPos,
		"unspecified top position");
	GLOW_DEBUG(
		(vOption & posOptionMask) == bottomPos && bottomLimit == unspecifiedPos,
		"unspecified bottom position");
	GLOW_DEBUG(
		(vOption & posOptionMask) == centerPos && topLimit == unspecifiedPos,
		"unspecified vertical center position");
	
	// Determine sizes
	int hSize = (leftLimit == unspecifiedPos || rightLimit == unspecifiedPos) ?
		unspecifiedSize : rightLimit - leftLimit;
	int vSize = (topLimit == unspecifiedPos || bottomLimit == unspecifiedPos) ?
		unspecifiedSize : bottomLimit - topLimit;
	
	// Make sure size is specified for requestedSize or forcedSize
	GLOW_DEBUG(hSize == unspecifiedSize &&
		((hOption & sizeOptionMask) == expandPreferredSize ||
		(hOption & sizeOptionMask) == forcedSize),
		"Trying to fill unspecified horizontal size");
	GLOW_DEBUG(vSize == unspecifiedSize &&
		((vOption & sizeOptionMask) == expandPreferredSize ||
		(vOption & sizeOptionMask) == forcedSize),
		"Trying to fill unspecified vertical size");
	
	// Reshape
	leftMargin = 0;
	rightMargin = 0;
	topMargin = 0;
	bottomMargin = 0;
	AutoPackError result = OnAutoPack(hSize, vSize,
		hOption & sizeOptionMask, vOption & sizeOptionMask,
		leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	// Adjust limits according to margins
	if (leftLimit != unspecifiedPos)
	{
		leftLimit += leftMargin;
	}
	if (rightLimit != unspecifiedPos)
	{
		rightLimit -= rightMargin;
	}
	if (topLimit != unspecifiedPos)
	{
		topLimit += topMargin;
	}
	if (bottomLimit != unspecifiedPos)
	{
		bottomLimit -= bottomMargin;
	}
	
	// Move and justify
	int hpos = PositionX();
	if ((hOption & posOptionMask) == leftPos)
	{
		hpos = leftLimit;
	}
	else if ((hOption & posOptionMask) == rightPos)
	{
		hpos = rightLimit-Width();
	}
	else if ((hOption & posOptionMask) == centerPos)
	{
		if (rightLimit == unspecifiedPos)
		{
			rightLimit = leftLimit;
		}
		hpos = (rightLimit+leftLimit-Width())/2;
	}
	
	int vpos = PositionY();
	if ((vOption & posOptionMask) == topPos)
	{
		vpos = topLimit;
	}
	else if ((vOption & posOptionMask) == bottomPos)
	{
		vpos = bottomLimit-Height();
	}
	else if ((vOption & posOptionMask) == centerPos)
	{
		if (bottomLimit == unspecifiedPos)
		{
			bottomLimit = topLimit;
		}
		vpos = (bottomLimit+topLimit-Height())/2;
	}
	
	Move(hpos, vpos);
	
	return noAutoPackError;
}


GlowWidget::AutoPackError GlowWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowWidget::OnAutoPack");
	
	if (hOption != noReshape &&
		((hSize != unspecifiedSize && hSize < Width()) ||
		(hOption == forcedSize && hSize != Width())))
	{
		return hAutoPackError;
	}
	if (vOption != noReshape &&
		((vSize != unspecifiedSize && vSize < Height()) ||
		(vOption == forcedSize && vSize != Height())))
	{
		return vAutoPackError;
	}
	
	return noAutoPackError;
}


void GlowWidget::OnActivate()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnActivate");
	
	if (_visibility == 1)
	{
		if (_receivingMouse)
		{
			_root->_RegisterMouseWidget(this);
		}
		if (_receivingKeyboard)
		{
			_root->_RegisterKeyboardWidget(this);
		}
		Refresh();
	}
	OnWidgetActivate();
}


void GlowWidget::OnDeactivate()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnDeactivate");
	
	if (_visibility == 1)
	{
		if (_receivingMouse)
		{
			_root->_UnregisterMouseWidget(this);
		}
		if (_receivingKeyboard)
		{
			_root->_UnregisterKeyboardWidget(this);
		}
		Refresh();
	}
	OnWidgetDeactivate();
}


bool GlowWidget::OnBeginPaint()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnBeginPaint");
	
	if (_visibility != 1)
	{
		return false;
	}
	::glViewport(
		RootPositionX(), _root->Subwindow()->Height()-RootPositionY()-_height,
		_width, _height);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	if (_clipping)
	{
		::glGetIntegerv(GL_SCISSOR_BOX, _oldScissor);
		int x = RootPositionX();
		if (x < _oldScissor[0]) x = _oldScissor[0];
		int y = _root->Subwindow()->Height()-Height()-RootPositionY();
		if (y < _oldScissor[1]) y = _oldScissor[1];
		int w = Width();
		if (x+w>_oldScissor[0]+_oldScissor[2]) w = _oldScissor[0]+_oldScissor[2]-x;
		int h = Height();
		if (y+h>_oldScissor[1]+_oldScissor[3]) h = _oldScissor[1]+_oldScissor[3]-y;
		::glScissor(x, y, w, h);
	}
	OnWidgetPaint();
	return true;
}


void GlowWidget::OnEndPaint()
{
	GLOW_DEBUGSCOPE("GlowWidget::OnEndPaint");
	
	if (_clipping && _visibility == 1)
	{
		::glScissor(_oldScissor[0], _oldScissor[1], _oldScissor[2], _oldScissor[3]);
	}
}


void GlowWidget::_ExecuteNotify()
{
	GLOW_DEBUGSCOPE("GlowWidget::_ExecuteNotify");
	
	for (GLOW_STD::map<GlowWidget*, Glow_OldWidgetState>::iterator iter =
		_notifyList.begin(); iter != _notifyList.end(); ++iter)
	{
		(*iter).first->_NotifyOne((*iter).second);
	}
	_notifyList.clear();
}


void GlowWidget::_BroadcastNotifyList()
{
	GLOW_DEBUGSCOPE("GlowWidget::_BroadcastNotifyList");
	
	_AddToNotifyList();
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
	{
		GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
		if (childWidget != 0)
		{
			childWidget->_BroadcastNotifyList();
		}
	}
}


void GlowWidget::_AddToNotifyList()
{
	GLOW_DEBUGSCOPE("GlowWidget::_AddToNotifyList");
	
	GLOW_STD::map<GlowWidget*, Glow_OldWidgetState>::iterator iter =
		_notifyList.find(this);
	if (iter == _notifyList.end())
	{
		_notifyList.insert(
			GLOW_STD::pair<GlowWidget* const, Glow_OldWidgetState>(this,
				Glow_OldWidgetState(_visibility==1, _hasFocus, _width, _height,
					RootPositionX(), RootPositionY())));
	}
}


void GlowWidget::_NotifyOne(
	const Glow_OldWidgetState& oldState)
{
	GLOW_DEBUGSCOPE("GlowWidget::_NotifyOne");
	
	if (!oldState.visible && IsVisible())
	{
		OnWidgetVisible();
	}
	if (oldState.width != _width ||
		oldState.height != _height)
	{
		OnWidgetReshape();
	}
	if (oldState.x != RootPositionX() ||
		oldState.y != RootPositionY())
	{
		OnWidgetMove();
	}
	if (oldState.keyboardFocus && !HasKeyboardFocus())
	{
		OnLostKeyboardFocus();
	}
	else if (!oldState.keyboardFocus && HasKeyboardFocus())
	{
		OnGotKeyboardFocus();
	}
	if (oldState.visible && !IsVisible())
	{
		OnWidgetInvisible();
	}
}


void GlowWidget::_BroadcastMask(
	bool unmasking)
{
	if (unmasking && _visibility == 2)
	{
		_AddToNotifyList();
		_visibility = 1;
		if (IsActive())
		{
			if (_receivingMouse)
			{
				_root->_RegisterMouseWidget(this);
			}
			if (_receivingKeyboard)
			{
				_root->_RegisterKeyboardWidget(this);
			}
		}
		for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
		{
			GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
			if (childWidget != 0)
			{
				childWidget->_BroadcastMask(true);
			}
		}
	}
	else if (!unmasking && _visibility == 1)
	{
		_AddToNotifyList();
		_visibility = 2;
		if (IsActive())
		{
			if (_receivingMouse)
			{
				_root->_UnregisterMouseWidget(this);
			}
			if (_receivingKeyboard)
			{
				_root->_UnregisterKeyboardWidget(this);
			}
		}
		for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
		{
			GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
			if (childWidget != 0)
			{
				childWidget->_BroadcastMask(false);
			}
		}
	}
}


void GlowWidget::Show()
{
	GLOW_DEBUGSCOPE("GlowWidget::Show");
	
	if (_visibility == 0)
	{
		if (_parentWidget==0 || _parentWidget->IsVisible())
		{
			_AddToNotifyList();
			_visibility = 1;
			if (IsActive())
			{
				if (_receivingMouse)
				{
					_root->_RegisterMouseWidget(this);
				}
				if (_receivingKeyboard)
				{
					_root->_RegisterKeyboardWidget(this);
				}
			}
			for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
			{
				GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
				if (childWidget != 0)
				{
					childWidget->_BroadcastMask(true);
				}
			}
			Refresh();
		}
		else
		{
			_visibility = 2;
		}
	}
}


void GlowWidget::Hide()
{
	GLOW_DEBUGSCOPE("GlowWidget::Hide");
	
	if (_visibility == 1)
	{
		_AddToNotifyList();
		_visibility = 0;
		if (IsActive())
		{
			if (_receivingMouse)
			{
				_root->_UnregisterMouseWidget(this);
			}
			if (_receivingKeyboard)
			{
				_root->_UnregisterKeyboardWidget(this);
			}
		}
		for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
		{
			GlowWidget* childWidget = dynamic_cast<GlowWidget*>(child);
			if (childWidget != 0)
			{
				childWidget->_BroadcastMask(false);
			}
		}
		Refresh();
	}
	else if (_visibility == 2)
	{
		_visibility = 0;
	}
}


void GlowWidget::OnWidgetPaint()
{
}


void GlowWidget::OnWidgetMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowWidget::OnWidgetMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowWidget::OnWidgetMouseDrag(
	int x,
	int y)
{
}


void GlowWidget::OnWidgetKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowWidget::OnGotKeyboardFocus()
{
}


void GlowWidget::OnLostKeyboardFocus()
{
}


void GlowWidget::OnWidgetMove()
{
}


void GlowWidget::OnWidgetReshape()
{
}


void GlowWidget::OnWidgetVisible()
{
}


void GlowWidget::OnWidgetInvisible()
{
}


void GlowWidget::OnWidgetActivate()
{
}


void GlowWidget::OnWidgetDeactivate()
{
}


/*
===============================================================================
	Methods of GlowWidgetRoot
===============================================================================
*/

void GlowWidgetRoot::Init(
	GlowSubwindow* subwindow,
	GlowColor backColor)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::Init");
	
	_subwindow = subwindow;
	_backColor = backColor;
	_curKeyboardFocus = _keyboardWidgets.end();
	_leftButton = 0;
	_middleButton = 0;
	_rightButton = 0;
}


GlowWidgetRoot::~GlowWidgetRoot()
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::~GlowWidgetRoot");
	
	GLOW_STD::list<GlowWidget*>::iterator iter;
	while ((iter = _mouseWidgets.begin()) != _mouseWidgets.end())
	{
		(*iter)->UnregisterMouseEvents();
	}
	while ((iter = _keyboardWidgets.begin()) != _keyboardWidgets.end())
	{
		(*iter)->UnregisterKeyboardEvents();
	}
}


void GlowWidgetRoot::SetKeyboardFocus(
	GlowWidget* widget)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::SetKeyboardFocus");
	
	if (widget == 0)
	{
		if (_curKeyboardFocus != _keyboardWidgets.end())
		{
			(*_curKeyboardFocus)->_SetHasKeyboardFocus(false);
			_curKeyboardFocus = _keyboardWidgets.end();
		}
	}
	else
	{
		GLOW_STD::list<GlowWidget*>::iterator iter = GLOW_STD::find(
			_keyboardWidgets.begin(), _keyboardWidgets.end(), widget);
		GLOW_DEBUG(iter == _keyboardWidgets.end(), "Illegal keyboard focus");
		if (iter != _curKeyboardFocus)
		{
			if (_curKeyboardFocus != _keyboardWidgets.end())
			{
				(*_curKeyboardFocus)->_SetHasKeyboardFocus(false);
			}
			widget->_SetHasKeyboardFocus(true);
			_curKeyboardFocus = iter;
		}
	}
}


void GlowWidgetRoot::AdvanceKeyboardFocus()
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::AdvanceKeyboardFocus");
	
	if (_curKeyboardFocus != _keyboardWidgets.end())
	{
		(*_curKeyboardFocus)->_SetHasKeyboardFocus(false);
		++_curKeyboardFocus;
	}
	if (_curKeyboardFocus == _keyboardWidgets.end())
	{
		_curKeyboardFocus = _keyboardWidgets.begin();
	}
	if (_curKeyboardFocus != _keyboardWidgets.end())
	{
		(*_curKeyboardFocus)->_SetHasKeyboardFocus(true);
	}
}


GlowWidget* GlowWidgetRoot::FindWidget(
	int& x,
	int& y)
{
	for (GLOW_STD::list<GlowWidget*>::iterator iter = _mouseWidgets.begin();
		iter != _mouseWidgets.end(); ++iter)
	{
		GlowWidget* widget = (*iter);
		int xmin = widget->RootPositionX();
		int ymin = widget->RootPositionY();
		if (x>=xmin && y>=ymin &&
			x<=xmin+widget->Width() && y<=ymin+widget->Height())
		{
			x -= xmin;
			y -= ymin;
			return widget;
		}
	}
	return 0;
}


void GlowWidgetRoot::WRMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRMouseDown");
	
	GlowWidget* widget = FindWidget(x, y);
	if (widget != 0)
	{
		if (button == Glow::leftButton)
		{
			_leftButton = widget;
		}
		else if (button == Glow::middleButton)
		{
			_middleButton = widget;
		}
		else //if (button == Glow::rightButton)
		{
			_rightButton = widget;
		}
		widget->OnWidgetMouseDown(button, x, y, modifiers);
	}
}


void GlowWidgetRoot::WRMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRMouseUp");
	
	GlowWidget* widget = 0;
	if (button == Glow::leftButton)
	{
		widget = _leftButton;
		_leftButton = 0;
	}
	else if (button == Glow::middleButton)
	{
		widget = _middleButton;
		_middleButton = 0;
	}
	else if (button == Glow::rightButton)
	{
		widget = _rightButton;
		_rightButton = 0;
	}
	if (widget != 0)
	{
		widget->OnWidgetMouseUp(button, x-widget->RootPositionX(),
			y-widget->RootPositionY(), modifiers);
	}
}


void GlowWidgetRoot::WRMouseDrag(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRMouseDrag");
	
	if (_leftButton != 0)
	{
		int xmin = _leftButton->RootPositionX();
		int ymin = _leftButton->RootPositionY();
		_leftButton->OnWidgetMouseDrag(x-xmin, y-ymin);
	}
	if (_middleButton != 0 && _middleButton != _leftButton)
	{
		int xmin = _middleButton->RootPositionX();
		int ymin = _middleButton->RootPositionY();
		_middleButton->OnWidgetMouseDrag(x-xmin, y-ymin);
	}
	if (_rightButton != 0 && _rightButton != _leftButton && _rightButton != _middleButton)
	{
		int xmin = _rightButton->RootPositionX();
		int ymin = _rightButton->RootPositionY();
		_rightButton->OnWidgetMouseDrag(x-xmin, y-ymin);
	}
}


void GlowWidgetRoot::WRKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRKeyboard");
	
	GlowWidgetKeyboardData filterData;
	filterData.root = this;
	filterData.key = key;
	filterData.x = x;
	filterData.y = y;
	filterData.modifiers = modifiers;
	_keyboardFilters.Send(filterData);
	if (filterData._continue &&
		filterData.root->_curKeyboardFocus != _keyboardWidgets.end())
	{
		GlowWidget* widget = *(filterData.root->_curKeyboardFocus);
		widget->OnWidgetKeyboard(filterData.key,
			filterData.x-widget->RootPositionX(),
			filterData.y-widget->RootPositionY(), filterData.modifiers);
	}
}


bool GlowWidgetRoot::WRBeginPaint()
{
	GLOW_DEBUGSCOPE("GlowWidgetRoot::WRBeginPaint");
	
	::glViewport(0, 0, _subwindow->Width(), _subwindow->Height());
	::glClearColor(float(_backColor[0])/255.0f, float(_backColor[1])/255.0f,
		float(_backColor[2])/255.0f, 0.0f);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::glEnable(GL_SCISSOR_TEST);
	::glScissor(0, 0, _subwindow->Width(), _subwindow->Height());
	return true;
}


void GlowWidgetRoot::WREndPaint()
{
	::glDisable(GL_SCISSOR_TEST);
}


void GlowWidgetRoot::_UnregisterMouseWidget(
	GlowWidget* widget)
{
	_mouseWidgets.remove(widget);
	if (_leftButton == widget)
	{
		_leftButton = 0;
	}
	if (_middleButton == widget)
	{
		_middleButton = 0;
	}
	if (_rightButton == widget)
	{
		_rightButton = 0;
	}
}


void GlowWidgetRoot::_UnregisterKeyboardWidget(
	GlowWidget* widget)
{
	if (_curKeyboardFocus != _keyboardWidgets.end() && widget == *_curKeyboardFocus)
	{
		widget->_SetHasKeyboardFocus(false);
		++_curKeyboardFocus;
		if (_curKeyboardFocus == _keyboardWidgets.end())
		{
			_curKeyboardFocus = _keyboardWidgets.begin();
		}
		if (widget == *_curKeyboardFocus)
		{
			_curKeyboardFocus = _keyboardWidgets.end();
		}
		if (_curKeyboardFocus != _keyboardWidgets.end())
		{
			(*_curKeyboardFocus)->_SetHasKeyboardFocus(true);
		}
	}
	_keyboardWidgets.remove(widget);
}


/*
===============================================================================
	Methods of GlowWidgetKeyboardFilter
===============================================================================
*/

void GlowWidgetKeyboardFilter::OnMessage(
	GlowWidgetKeyboardData& message)
{
	if (message._continue)
	{
		message._continue = OnFilter(message);
	}
}


/*
===============================================================================
	Methods of GlowWidgetTabFilter
===============================================================================
*/

bool GlowWidgetTabFilter::OnFilter(
	GlowWidgetKeyboardData& data)
{
	if (data.key == Glow::tabKey)
	{
		data.root->AdvanceKeyboardFocus();
	}
	return data.key != Glow::tabKey;
}


/*
===============================================================================
	Methods of GlowWidgetSubwindow
===============================================================================
*/

void GlowWidgetSubwindow::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseDown(button, x, y, modifiers);
}


void GlowWidgetSubwindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseUp(button, x, y, modifiers);
}


void GlowWidgetSubwindow::OnMouseDrag(
	int x,
	int y)
{
	WRMouseDrag(x, y);
}


void GlowWidgetSubwindow::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRKeyboard(key, x, y, modifiers);
}


bool GlowWidgetSubwindow::OnBeginPaint()
{
	return WRBeginPaint();
}


void GlowWidgetSubwindow::OnEndPaint()
{
	WREndPaint();
}


/*
===============================================================================
	Methods of GlowWidgetWindow
===============================================================================
*/

void GlowWidgetWindow::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseDown(button, x, y, modifiers);
}


void GlowWidgetWindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseUp(button, x, y, modifiers);
}


void GlowWidgetWindow::OnMouseDrag(
	int x,
	int y)
{
	WRMouseDrag(x, y);
}


void GlowWidgetWindow::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRKeyboard(key, x, y, modifiers);
}


bool GlowWidgetWindow::OnBeginPaint()
{
	return WRBeginPaint();
}


void GlowWidgetWindow::OnEndPaint()
{
	WREndPaint();
}


/*
===============================================================================
	Methods of GlowFixedSizeWidgetWindow
===============================================================================
*/

void GlowFixedSizeWidgetWindow::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseDown(button, x, y, modifiers);
}


void GlowFixedSizeWidgetWindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRMouseUp(button, x, y, modifiers);
}


void GlowFixedSizeWidgetWindow::OnMouseDrag(
	int x,
	int y)
{
	WRMouseDrag(x, y);
}


void GlowFixedSizeWidgetWindow::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	WRKeyboard(key, x, y, modifiers);
}


bool GlowFixedSizeWidgetWindow::OnBeginPaint()
{
	return WRBeginPaint();
}


void GlowFixedSizeWidgetWindow::OnEndPaint()
{
	WREndPaint();
}


/*
===============================================================================
	Methods of GlowSubwindowInWidget
===============================================================================
*/

void GlowSubwindowInWidget::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowSubwindowInWidget::OnKeyboard");
	
	Glow::DeliverKeyboardEvt(
		static_cast<GlowWidget*>(Parent())->Root()->Subwindow(),
		key, x, y, modifiers);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

