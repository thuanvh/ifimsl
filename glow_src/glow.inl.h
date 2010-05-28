/*
===============================================================================

	FILE:  glow.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Basic classes for GLOW
	
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

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods for message types
===============================================================================
*/

inline GlowTimerMessage::GlowTimerMessage(
	int i)
{
	id = i;
}


inline GlowIdleMessage::GlowIdleMessage()
{
}


/*
===============================================================================
	Inline methods for event filter types
===============================================================================
*/

inline GlowKeyboardData::GlowKeyboardData()
{
	_continue = true;
}


inline GlowMouseData::GlowMouseData()
{
	_continue = true;
}


/*
===============================================================================
	Inline methods for Glow
===============================================================================
*/

/*
-------------------------------------------------------------------------------
	General methods
-------------------------------------------------------------------------------
*/

inline bool Glow::IsMenuInUse()
{
	return _menuInUse;
}


inline int Glow::NumMouseButtons()
{
	return ::glutDeviceGet(GLenum(GLUT_NUM_MOUSE_BUTTONS));
}


/*
-------------------------------------------------------------------------------
	Modal window methods
-------------------------------------------------------------------------------
*/

inline int Glow::NumModalWindows()
{
	return _modalWindows.size();
}


inline GlowWindow* Glow::TopModalWindow()
{
	return _modalWindows.empty() ? 0 : _modalWindows.back();
}


/*
-------------------------------------------------------------------------------
	Idle function methods
-------------------------------------------------------------------------------
*/

inline void Glow::UnregisterAllIdle()
{
	_idleSender.UnbindAll();
	::glutIdleFunc(0);
}


inline unsigned int Glow::NumRegisteredIdle()
{
	return _idleSender.NumReceivers();
}


inline bool Glow::IsIdleRegistered(
	GlowIdleReceiver* idle)
{
	return _idleSender.IsBoundTo(idle);
}


/*
-------------------------------------------------------------------------------
	Timer function methods
-------------------------------------------------------------------------------
*/

inline bool Glow::IsTimerPending(
	int id)
{
	return _timerSenders.find(id) != _timerSenders.end();
}


/*
-------------------------------------------------------------------------------
	Filter methods
-------------------------------------------------------------------------------
*/

inline void Glow::UnregisterAllMouseFilters()
{
	_mouseFilters.UnbindAll();
}


inline void Glow::UnregisterAllKeyboardFilters()
{
	_keyboardFilters.UnbindAll();
}


inline unsigned int Glow::NumRegisteredMouseFilters()
{
	return _mouseFilters.NumReceivers();
}


inline unsigned int Glow::NumRegisteredKeyboardFilters()
{
	return _keyboardFilters.NumReceivers();
}


inline bool Glow::IsFilterRegistered(
	GlowMouseFilter* filter)
{
	return _mouseFilters.IsBoundTo(filter);
}


inline bool Glow::IsFilterRegistered(
	GlowKeyboardFilter* filter)
{
	return _keyboardFilters.IsBoundTo(filter);
}


inline void Glow::RegisterFilter(
	GlowMouseFilter* filter)
{
	if (!_mouseFilters.IsBoundTo(filter))
	{
		_mouseFilters.Bind(filter);
	}
}


inline void Glow::RegisterFilter(
	GlowKeyboardFilter* filter)
{
	if (!_keyboardFilters.IsBoundTo(filter))
	{
		_keyboardFilters.Bind(filter);
	}
}


inline void Glow::UnregisterFilter(
	GlowMouseFilter* filter)
{
	if (_mouseFilters.IsBoundTo(filter))
	{
		_mouseFilters.Unbind(filter);
	}
}


inline void Glow::UnregisterFilter(
	GlowKeyboardFilter* filter)
{
	if (_keyboardFilters.IsBoundTo(filter))
	{
		_keyboardFilters.Unbind(filter);
	}
}


/*
-------------------------------------------------------------------------------
	Event simulation methods
-------------------------------------------------------------------------------
*/

inline void Glow::DeliverKeyboardEvt(
	GlowSubwindow* receiver,
	Glow::KeyCode key,
	int x,
	int y,
	Modifiers modifiers)
{
	receiver->OnKeyboard(key, x, y, modifiers);
}


inline void Glow::DeliverMouseDownEvt(
	GlowSubwindow* receiver,
	Glow::MouseButton button,
	int x,
	int y,
	Modifiers modifiers)
{
	receiver->OnMouseDown(button, x, y, modifiers);
}


inline void Glow::DeliverMouseUpEvt(
	GlowSubwindow* receiver,
	Glow::MouseButton button,
	int x,
	int y,
	Modifiers modifiers)
{
	receiver->OnMouseUp(button, x, y, modifiers);
}


inline void Glow::DeliverMouseDragEvt(
	GlowSubwindow* receiver,
	int x,
	int y)
{
	receiver->OnMouseDrag(x, y);
}


inline void Glow::DeliverMouseMotionEvt(
	GlowSubwindow* receiver,
	int x,
	int y)
{
	receiver->OnMouseMotion(x, y);
}


inline void Glow::DeliverMouseEnterEvt(
	GlowSubwindow* receiver)
{
	receiver->OnMouseEnter();
}


inline void Glow::DeliverMouseExitEvt(
	GlowSubwindow* receiver)
{
	receiver->OnMouseExit();
}


/*
-------------------------------------------------------------------------------
	Random drawing method
-------------------------------------------------------------------------------
*/

inline void Glow::SwapBuffers()
{
	::glutSwapBuffers();
}


/*
-------------------------------------------------------------------------------
	Miscellaneous methods
-------------------------------------------------------------------------------
*/

inline int Glow::GetMilliseconds()
{
	return ::glutGet((GLenum)GLUT_ELAPSED_TIME);
}


inline void Glow::SetMenuStatusFunc(
	void (*func)(int status, int x, int y))
{
	_userMenuStatusFunc = func;
}


/*
-------------------------------------------------------------------------------
	Toplevel window methods
-------------------------------------------------------------------------------
*/

inline int Glow::NumToplevelWindows()
{
	return _numToplevelWindows;
}


inline bool Glow::IsAutoQuitting()
{
	return _autoQuitting;
}


inline void Glow::SetAutoQuitting(
	bool autoQuit)
{
	_autoQuitting = autoQuit;
}


/*
===============================================================================
	Inline methods for GlowComponent
===============================================================================
*/

inline GlowComponent::GlowComponent()
{
	_initializeState = -1;
	_parent = 0;
}


inline GlowComponent::GlowComponent(
	GlowComponent* parent)
{
	_initializeState = -1;
	Init(parent);
}


inline GlowComponent* GlowComponent::Next() const
{
	return _next;
}


inline GlowComponent* GlowComponent::Prev() const
{
	return _prev;
}


inline GlowComponent* GlowComponent::Parent() const
{
	return _parent;
}


inline GlowSubwindow* GlowComponent::ParentWindow() const
{
	if (_parent == 0)
	{
		return 0;
	}
	else
	{
		return _parent->WhichWindow();
	}
}


inline bool GlowComponent::IsTopLevel() const
{
	return _parent == 0;
}


inline int GlowComponent::NumChildren() const
{
	return _numChildren;
}


inline GlowComponent* GlowComponent::FirstChild() const
{
	return _firstChild;
}


inline GlowComponent* GlowComponent::LastChild() const
{
	return _lastChild;
}


inline bool GlowComponent::IsActive() const
{
	return _activeState == 1;
}


inline bool GlowComponent::IsActiveStandby() const
{
	return _activeState == 2;
}


inline bool GlowComponent::IsInactive() const
{
	return _activeState == 0;
}


inline bool GlowComponent::IsInitialized() const
{
	return _initializeState != -1;
}


inline bool GlowComponent::IsClosing() const
{
	return _initializeState == 0;
}


/*
===============================================================================
	Inline methods for GlowSubwindow
===============================================================================
*/

inline GlowSubwindow::GlowSubwindow()
{
}


inline GlowSubwindow::GlowSubwindow(
	GlowComponent* parent,
	const GlowSubwindowParams& params)
{
	Init(parent, params);
}


inline GlowSubwindow::GlowSubwindow(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	Init(parent, x, y, width, height, mode, eventMask);
}


inline bool GlowSubwindow::IsRefreshEnabled() const
{
	return _refreshEnabled;
}


inline void GlowSubwindow::SetRefreshEnabled(
	bool enable)
{
	_refreshEnabled = enable;
}


inline void GlowSubwindow::Refresh()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Refresh");
	
	if (_refreshEnabled)
	{
		Glow::RefreshGlutWindow(_windowNum);
	}
}


inline bool GlowSubwindow::IsAutoSwapBuffersEnabled() const
{
	return _autoSwapBuffers;
}


inline void GlowSubwindow::SetAutoSwapBuffersEnabled(
	bool enable)
{
	_autoSwapBuffers = enable;
}


inline void GlowSubwindow::MakeCurGlutWindow()
{
	::glutSetWindow(_windowNum);
}


inline int GlowSubwindow::GlutWindowNum() const
{
	return _windowNum;
}


inline int GlowSubwindow::PositionX() const
{
	if (_parent == 0)
	{
		return GlobalPositionX();
	}
	else
	{
		return GlobalPositionX() -
			ParentWindow()->GlobalPositionX();
	}
}


inline int GlowSubwindow::PositionY() const
{
	if (_parent == 0)
	{
		return GlobalPositionY();
	}
	else
	{
		return GlobalPositionY() -
			ParentWindow()->GlobalPositionY();
	}
}


inline int GlowSubwindow::GlobalPositionX() const
{
	if (_clock != Glow::_clock)
	{
		_clock = Glow::_clock;
		_globalXPos = GlutInfo((GLenum)GLUT_WINDOW_X);
		_globalYPos = GlutInfo((GLenum)GLUT_WINDOW_Y);
	}
	return _globalXPos;
}


inline int GlowSubwindow::GlobalPositionY() const
{
	if (_clock != Glow::_clock)
	{
		_clock = Glow::_clock;
		_globalXPos = GlutInfo((GLenum)GLUT_WINDOW_X);
		_globalYPos = GlutInfo((GLenum)GLUT_WINDOW_Y);
	}
	return _globalYPos;
}


inline int GlowSubwindow::Width() const
{
	return _width;
}


inline int GlowSubwindow::Height() const
{
	return _height;
}


inline int GlowSubwindow::GetCursor() const
{
	return GlutInfo((GLenum)GLUT_WINDOW_CURSOR);
}


inline GlowMenu* GlowSubwindow::GetMenu(
	Glow::MouseButton button) const
{
	if (button == Glow::leftButton)
	{
		return _leftMenu;
	}
	else if (button == Glow::middleButton)
	{
		return _centerMenu;
	}
	else //if (button == Glow::rightButton)
	{
		return _rightMenu;
	}
}


inline void GlowSubwindow::UnsetMenu(
	Glow::MouseButton button)
{
	SetMenu(button, 0);
}


inline Glow::EventMask GlowSubwindow::GetEventMask() const
{
	return _eventMask;
}


inline Glow::EventMask GlowSubwindow::GetInactiveEventMask() const
{
	return _inactiveEventMask;
}


inline Glow::BufferType GlowSubwindow::GetBufferType() const
{
	return _bufferType;
}


inline void GlowSubwindow::NormalizeCoordinates(
	int x,
	int y,
	GLfloat& xn,
	GLfloat& yn) const
{
	xn = GLfloat(x)*2.0f/GLfloat(_width)-1.0f;
	yn = 1.0f-GLfloat(y)*2.0f/GLfloat(_height);
}


inline void GlowSubwindow::_FinishRender() const
{
	::glFlush();
	if (_needSwapBuffers && _autoSwapBuffers)
	{
		::glutSwapBuffers();
	}
}


/*
===============================================================================
	Inline methods for GlowWindow
===============================================================================
*/

inline GlowWindow::GlowWindow()
{
}


inline GlowWindow::GlowWindow(
	const GlowWindowParams& params)
{
	Init(params);
}


inline GlowWindow::GlowWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	Init(title, x, y, width, height, mode, eventMask);
}


inline const char* GlowWindow::GetTitle() const
{
	return _title;
}


inline const char* GlowWindow::GetIconTitle() const
{
	return _iconTitle;
}


/*
===============================================================================
	Inline methods for GlowMenu
===============================================================================
*/

inline bool GlowMenu::IsEntry(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(_itemData.size()));
	
	return _itemData[itemNum].subMenu == 0;
}


inline bool GlowMenu::IsSubmenu(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(_itemData.size()));
	
	return _itemData[itemNum].subMenu != 0;
}


inline const char* GlowMenu::GetItemLabel(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(_itemData.size()));
	
	return _itemData[itemNum].label;
}


inline const char* GlowMenu::GetItemMark(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(_itemData.size()));
	
	return _itemData[itemNum].mark;
}


inline int GlowMenu::GetItemCode(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(_itemData.size()));
	GLOW_DEBUG(_itemData[itemNum].subMenu != 0,
		"GlowMenu::GetItemCode on submenu item");
	
	return _itemData[itemNum].code;
}


inline GlowMenu* GlowMenu::GetItemSubmenu(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(_itemData.size()));
	GLOW_DEBUG(_itemData[itemNum].subMenu == 0,
		"GlowMenu::GetItemSubmenu on entry item");
	
	return _itemData[itemNum].subMenu;
}


inline bool GlowMenu::IsItemMarked(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < int(_itemData.size()));
	
	return _itemData[itemNum].mark != 0;
}


inline int GlowMenu::NumItems() const
{
	return _itemData.size();
}


inline void GlowMenu::SetBindState(
	GlowMenu::BindState bindState)
{
	_bindState = bindState;
}


inline GlowMenu::BindState GlowMenu::GetBindState() const
{
	return _bindState;
}


inline TSender<const GlowMenuMessage&>& GlowMenu::Notifier()
{
	return _sender;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

