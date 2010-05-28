/*
===============================================================================

	FILE:  glow.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Basic classes in GLOW
	
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

#include <cstring>
#include <cstdlib>

#ifndef GLOW__H
	#include "glow.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowSubwindowParams GlowSubwindowParams::defaults(true);

GlowSubwindowParams::GlowSubwindowParams()
{
	*this = defaults;
}

GlowSubwindowParams::GlowSubwindowParams(bool)
{
	x = 0;
	y = 0;
	width = -1;
	height = -1;
	eventMask = Glow::noEvents;
	inactiveEventMask = Glow::noEvents;
	mode = Glow::rgbBuffer | Glow::doubleBuffer;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowWindowParams GlowWindowParams::defaults(true);

GlowWindowParams::GlowWindowParams()
{
	*this = defaults;
}

GlowWindowParams::GlowWindowParams(bool)
{
	x = -1;
	y = -1;
	width = 100;
	height = 100;
	eventMask = Glow::noEvents;
	inactiveEventMask = Glow::noEvents;
	mode = Glow::rgbBuffer | Glow::doubleBuffer;
	title = "";
	iconTitle = 0;
}


/*
===============================================================================
	GLUT-integration idle function receiver
===============================================================================
*/

class Glow_IdleFuncReceiver :
	public GlowIdleReceiver
{
	friend class Glow;
	
	private:
	
		inline Glow_IdleFuncReceiver(
			void (*func)());
	
	protected:
	
		virtual void OnMessage(
			const GlowIdleMessage& message);
	
	private:
	
		void (*_funcPtr)();
};


inline Glow_IdleFuncReceiver::Glow_IdleFuncReceiver(
	void (*func)())
{
	_funcPtr = func;
}


void Glow_IdleFuncReceiver::OnMessage(
	const GlowIdleMessage& message)
{
	_funcPtr();
}


/*
===============================================================================
	Event filter receivers
===============================================================================
*/

void GlowKeyboardFilter::OnMessage(
	GlowKeyboardData& message)
{
	if (message._continue)
	{
		message._continue = OnFilter(message);
	}
}


void GlowMouseFilter::OnMessage(
	GlowMouseData& message)
{
	if (message._continue)
	{
		message._continue = OnFilter(message);
	}
}


/*
===============================================================================
	Private globals for Glow
===============================================================================
*/

// Map window IDs to GlowSubwindow*s
GLOW_STD::map<int, GlowSubwindow*> Glow::_windowRegistry;
// Map menu IDs to GlowMenu*s
GLOW_STD::map<int, GlowMenu*> Glow::_menuRegistry;

// List of components to activate in deferred activation
GLOW_STD::map<GlowComponent*, bool> Glow::_activateNotifyList;
// List of components to close in deferred delete (bool is unused)
GLOW_STD::map<GlowComponent*, bool> Glow::_closeList;
// Special pointer to a function to call during deferred event time
// used by widget system to implement deferred widget events
void (*Glow::_widgetNotifier)() = 0;
// Each subwindow contains a local clock value, and there's this global
// clock here that gets incremented before every event handler. If local
// clock == global clock, then the subwindow's data members contain the
// correct position. If !=, the subwindow needs to update its knowledge
// of position by querying glutGet.
// This rather convoluted system is necessary to maintain the semantics
// of GLOW state querying in the face of GLUT deferred execution. After
// calling Move(), the GLOW state should reflect the new position (the
// local clock is updated and the data members are set to the new pos
// even though glutGet() would still return the old values.)
// If the window is moved by the user, the local clock will be inc'd
// at the beginning of the next event handling cycle, so GLOW knows that
// it is possible the cached data member values are stale.
unsigned long Glow::_clock = 0;

// Subwindow that spawned the current menu. Set if a menu is down.
GlowSubwindow* Glow::_menuWindow = 0;
// Coordinates of the click that spawned the current menu.
int Glow::_menuXClick = -1;
int Glow::_menuYClick = -1;
// Is a menu down?
bool Glow::_menuInUse = false;

// Modal window stack
GLOW_STD::vector<GlowWindow*> Glow::_modalWindows;

// Sender for idle events
TSender<const GlowIdleMessage&> Glow::_idleSender;
// List of senders for timer events. Maps Timer IDs to senders.
GLOW_STD::map<int, TSender<const GlowTimerMessage&>*> Glow::_timerSenders;
// The next timer ID that will be assigned.
int Glow::_nextTimerID = 1;

// Special idle function receiver implementing GLUT-style idle func
Glow_IdleFuncReceiver* Glow::_idleFuncReceiver = 0;
// GLUT-style menu status function pointer
void (*Glow::_userMenuStatusFunc)(int status, int x, int y) = 0;

// Global filters for mouse and keyboard events
TSender<GlowMouseData&> Glow::_mouseFilters;
TSender<GlowKeyboardData&> Glow::_keyboardFilters;

// Number of toplevel windows in existence
int Glow::_numToplevelWindows = 0;
// If true, GLOW will automatically quit when the last toplevel window
// is deleted.
bool Glow::_autoQuitting = false;

// GLUT seems to have a bug that sometimes causes windows to be "locked
// out" of the worklist (i.e. unable to receive events.) It often happens
// when you re-post a redisplay event from within the same window's
// display callback. This option causes GLOW to defer such re-postings
// until the next event.
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
// Window ID currently in display callback, or 0 for not displaying
int Glow::_curDisplayWindow = 0;
// Window ID for which we should repost redisplay, or 0 for none.
int Glow::_refreshMe = 0;
#endif


/*
===============================================================================
	Methods for Glow
===============================================================================
*/
bool Glow::isGLInit = false;
void Glow::Init(
	int& argc,
	char** argv)
{
  if(!Glow::isGLInit){
    ::glutInit(&argc, argv);
  }
	::glutMenuStatusFunc(Glow::_MenuStatusFunc);
}


void Glow::MainLoop()
{
	::glutMainLoop();
}


double Glow::Version()
{
	return GLOW_VERSION;
}


int Glow::APIVersion()
{
	return GLOW_APIVERSION;
}


/*
===============================================================================
	Registry functions
===============================================================================
*/

void Glow::_AddWindow(
	GlowSubwindow* window,
	int windowNum)
{
	if (dynamic_cast<GlowWindow*>(window) != 0)
	{
		++_numToplevelWindows;
	}
	_windowRegistry.insert(_WindowRegistryEntry(windowNum, window));
}


void Glow::_RemoveWindow(
	int windowNum)
{
	// Find entry in registry
	_WindowRegistryIterator iter = _windowRegistry.find(windowNum);
	if (iter != _windowRegistry.end())
	{
		// Update numToplevelWindows
		GlowSubwindow* wind = (*iter).second;
		if (dynamic_cast<GlowWindow*>(wind) != 0)
		{
			--_numToplevelWindows;
		}
		// Are there modal windows?
		if (!_modalWindows.empty())
		{
			// Is window to remove the current top modal window?
			if (wind == _modalWindows.back())
			{
				PopModalWindow();
			}
			else
			{
				// Is window to remove in the modal window stack at all?
				for (GLOW_STD::vector<GlowWindow*>::iterator miter = _modalWindows.begin();
					miter != _modalWindows.end(); ++miter)
				{
					if (*miter == wind)
					{
						_modalWindows.erase(miter);
						break;
					}
				}
			}
		}
		_windowRegistry.erase(iter);
		// Auto-quit
		if (_autoQuitting && _windowRegistry.empty())
		{
			GLOW_CSTD::exit(0);
		}
	}
}


GlowSubwindow* Glow::ResolveWindow(
	int windowNum)
{
	_WindowRegistryIterator iter = _windowRegistry.find(windowNum);
	return (iter == _windowRegistry.end()) ? 0 : (*iter).second;
}


void Glow::_AddMenu(
	GlowMenu *menu,
	int menuNum)
{
	_menuRegistry.insert(_MenuRegistryEntry(menuNum, menu));
}


void Glow::_RemoveMenu(
	int menuNum)
{
	_menuRegistry.erase(menuNum);
}


GlowMenu* Glow::ResolveMenu(
	int menuNum)
{
	_MenuRegistryIterator iter = _menuRegistry.find(menuNum);
	return (iter == _menuRegistry.end()) ? 0 : (*iter).second;
}


/*
===============================================================================
	Idle functions
===============================================================================
*/

void Glow::RegisterIdle(
	GlowIdleReceiver* idle)
{
	GLOW_DEBUGSCOPE("Glow::RegisterIdle");
	
	// Bind to this receiver if it's not already
	if (!_idleSender.IsBoundTo(idle))
	{
		_idleSender.Bind(idle);
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		if (_idleSender.NumReceivers() == 1 && _refreshMe == 0)
#else
		if (_idleSender.NumReceivers() == 1)
#endif
		{
			::glutIdleFunc(_IdleFunc);
		}
	}
}


void Glow::UnregisterIdle(
	GlowIdleReceiver* idle)
{
	GLOW_DEBUGSCOPE("Glow::UnregisterIdle");
	
	// Unbind this receiver if it's bound
	if (_idleSender.IsBoundTo(idle))
	{
		_idleSender.Unbind(idle);
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		if (_idleSender.NumReceivers() == 0 && _refreshMe == 0)
#else
		if (_idleSender.NumReceivers() == 0)
#endif
		{
			::glutIdleFunc(0);
		}
	}
}


void Glow::SetIdleFunc(
	void (*func)())
{
	GLOW_DEBUGSCOPE("Glow::SetIdleFunc");
	
	// GLOW version of glutIdleFunc. 
	if (func == 0)
	{
		// Unregister the GLUT-style idle function receiver
		if (_idleFuncReceiver != 0)
		{
			UnregisterIdle(_idleFuncReceiver);
			delete _idleFuncReceiver;
			_idleFuncReceiver = 0;
		}
	}
	else
	{
		// Set the function pointer if the receiver is already registered
		if (_idleFuncReceiver != 0)
		{
			_idleFuncReceiver->_funcPtr = func;
		}
		else
		// Otherwise register a GLUT-style idle function receiver
		{
			_idleFuncReceiver = new Glow_IdleFuncReceiver(func);
			RegisterIdle(_idleFuncReceiver);
		}
	}
}


/*
===============================================================================
	Timer functions
===============================================================================
*/

int Glow::RegisterTimer(
	unsigned int msecs,
	GlowTimerReceiver* timer)
{
	GLOW_DEBUGSCOPE("Glow::RegisterTimer");
	
	// Get the next available timer ID.
	int timerID = _nextTimerID;
	while (_timerSenders.find(timerID) != _timerSenders.end() || timerID == 0)
	{
		++timerID;
	}
	_nextTimerID = timerID + 1;
	// Create a sender for this timer
	TSender<const GlowTimerMessage&>* sender = new TSender<const GlowTimerMessage&>;
	sender->Bind(timer);
	_timerSenders[timerID] = sender;
	// Register timer event
	::glutTimerFunc(msecs, _TimerFunc, timerID);
	return timerID;
}


void Glow::UnregisterTimer(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::UnregisterTimer");
	
	// Find the timer sender with this id. Delete it if it exists.
	// The timer function will still be called, but it will notice that
	// this id no longer exists, and it will ignore the callback.
	// This will, of course, fail if the id gets reused. However, I don't
	// think anyone will be scheduling 4 billion timers in that short
	// a space of time.
	GLOW_STD::map<int, TSender<const GlowTimerMessage&>*>::iterator iter = _timerSenders.find(id);
	if (iter != _timerSenders.end())
	{
		delete (*iter).second;
		_timerSenders.erase(iter);
	}
}


/*
===============================================================================
	Callbacks
===============================================================================
*/

void Glow::_TimerFunc(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::_TimerFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Which timer was invoked?
	GLOW_STD::map<int, TSender<const GlowTimerMessage&>*>::iterator iter = _timerSenders.find(id);
	if (iter != _timerSenders.end())
	{
		// Remove the timer sender, and send the message.
		TSender<const GlowTimerMessage&>* sender = (*iter).second;
		_timerSenders.erase(iter);
		sender->Send(GlowTimerMessage(id));
		delete sender;
		_ExecuteDeferred();
	}
}


void Glow::_DisplayFunc()
{
	GLOW_DEBUGSCOPE("Glow::_DisplayFunc");
	
	++_clock;
	// Paint the window
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_curDisplayWindow = ::glutGetWindow();
	if (_curDisplayWindow != _refreshMe)
	{
		_RaiseDeferredRefresh();
	}
	GlowSubwindow* window = ResolveWindow(_curDisplayWindow);
#else
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
#endif
	if (window != 0)
	{
		window->Paint();
		window->_FinishRender();
		_ExecuteDeferred();
	}
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_curDisplayWindow = 0;
#endif
}


void Glow::_ReshapeFunc(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("Glow::_ReshapeFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window, set the width and height, and handle the event
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->_width = width;
		window->_height = height;
		window->OnReshape(width, height);
		_ExecuteDeferred();
	}
}


void Glow::_KeyboardFunc(
	unsigned char key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_KeyboardFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global keyboard filters
		GlowKeyboardData filterData;
		filterData.subwindow = window;
		filterData.key = KeyCode(key);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		_keyboardFilters.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			filterData.subwindow->OnKeyboard(filterData.key,
				filterData.x, filterData.y, filterData.modifiers);
		}
		_ExecuteDeferred();
	}
}


void Glow::_MouseFunc(
	int button,
	int state,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_MouseFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global mouse filters
		GlowMouseData filterData;
		filterData.subwindow = window;
		filterData.type = (state == GLUT_DOWN) ?
			GlowMouseData::mouseDown : GlowMouseData::mouseUp;
		filterData.button = MouseButton(button);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		_mouseFilters.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			if (filterData.type == GlowMouseData::mouseDown)
			{
				filterData.subwindow->OnMouseDown(filterData.button,
					filterData.x, filterData.y, filterData.modifiers);
			}
			else if (filterData.type == GlowMouseData::mouseUp)
			{
				filterData.subwindow->OnMouseUp(filterData.button,
					filterData.x, filterData.y, filterData.modifiers);
			}
		}
		_ExecuteDeferred();
	}
}


void Glow::_MotionFunc(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_MotionFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window and invoke event handler
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnMouseDrag(x, y);
		_ExecuteDeferred();
	}
}


void Glow::_PassiveMotionFunc(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_PassiveMotionFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window and invoke event handler
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		window->OnMouseMotion(x, y);
		_ExecuteDeferred();
	}
}


void Glow::_VisibilityFunc(
	int state)
{
	GLOW_DEBUGSCOPE("Glow::_VisibilityFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window and invoke event handler
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		if (state == GLUT_VISIBLE)
		{
			window->OnVisible();
		}
		else if (state == GLUT_NOT_VISIBLE)
		{
			window->OnInvisible();
		}
		_ExecuteDeferred();
	}
}


void Glow::_EntryFunc(
	int state)
{
	GLOW_DEBUGSCOPE("Glow::_EntryFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window and invoke event handler
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		if (state == GLUT_ENTERED)
		{
			window->OnMouseEnter();
		}
		else if (state == GLUT_LEFT)
		{
			window->OnMouseExit();
		}
		_ExecuteDeferred();
	}
}


void Glow::_SpecialFunc(
	int key,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_SpecialFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the window
	GlowSubwindow* window = ResolveWindow(::glutGetWindow());
	if (window != 0)
	{
		// Send the event through the global keyboard filters
		GlowKeyboardData filterData;
		filterData.subwindow = window;
		filterData.key = KeyCode(key+Glow::specialKeyOffset);
		filterData.x = x;
		filterData.y = y;
		filterData.modifiers = Modifiers(::glutGetModifiers());
		_keyboardFilters.Send(filterData);
		// If the event wasn't consumed, send it to the event handler
		if (filterData._continue)
		{
			filterData.subwindow->OnKeyboard(filterData.key,
				filterData.x, filterData.y, filterData.modifiers);
		}
		_ExecuteDeferred();
	}
}


void Glow::_MenuStatusFunc(
	int status,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("Glow::_MenuStatusFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Give GLUT-style function a chance to handle the event
	if (_userMenuStatusFunc != 0)
	{
		_userMenuStatusFunc(status, x, y);
	}
	if (status == GLUT_MENU_IN_USE)
	{
		// Menu-down event
		_menuWindow = ResolveWindow(::glutGetWindow());
		_menuXClick = x;
		_menuYClick = y;
		_menuInUse = true;
		if (_menuWindow != 0)
		{
			_menuWindow->OnMenuDown(x, y);
		}
	}
	else if (status == GLUT_MENU_NOT_IN_USE)
	{
		// Menu-up event
		if (_menuWindow != 0)
		{
			_menuWindow->OnMenuUp();
		}
		_menuInUse = false;
	}
	_ExecuteDeferred();
}


void Glow::_MenuFunc(
	int value)
{
	GLOW_DEBUGSCOPE("Glow::_MenuFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Find the menu and invoke event handler
	GlowMenu* menu = ResolveMenu(::glutGetMenu());
	if (menu != 0)
	{
		menu->OnHit(value, _menuWindow, _menuXClick, _menuYClick);
		_ExecuteDeferred();
	}
}


void Glow::_IdleFunc()
{
	GLOW_DEBUGSCOPE("Glow::_IdleFunc");
	
	++_clock;
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	_RaiseDeferredRefresh();
#endif
	// Invoke idle event handlers
	_idleSender.Send(GlowIdleMessage());
	_ExecuteDeferred();
}


/*
===============================================================================
	Finalizers (deferred execution)
===============================================================================
*/

void Glow::_ExecuteDeferred()
{
	GLOW_DEBUGSCOPE("Glow::_ExecuteDeferred");
	
	// Widget notifier
	if (_widgetNotifier != 0)
	{
		_widgetNotifier();
	}
	
	// Deferred activation and deactivation
	for (GLOW_STD::map<GlowComponent*, bool>::iterator iter = _activateNotifyList.begin();
		iter != _activateNotifyList.end(); ++iter)
	{
		if ((*iter).second)
		{
			(*iter).first->OnActivate();
		}
		else
		{
			(*iter).first->OnDeactivate();
		}
	}
	_activateNotifyList.clear();
	
	// Deferred deletion of components
	while (!_closeList.empty())
	{
		delete (*(_closeList.begin())).first;
	}
}


#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
void Glow::_RaiseDeferredRefresh()
{
	if (_refreshMe != 0)
	{
		RefreshGlutWindow(_refreshMe);
		_refreshMe = 0;
		if (_idleSender.NumReceivers() == 0)
		{
			::glutIdleFunc(0);
		}
	}
}
#endif


/*
===============================================================================
	Modal window management
===============================================================================
*/

void Glow::PushModalWindow(
	GlowWindow* wind)
{
	GLOW_DEBUGSCOPE("Glow::PushModalWindow");
	
	if (_modalWindows.empty())
	{
		// This is the first modal window
		// Go through and deactivate all toplevel windows except the one
		// to be made modal
		for (_WindowRegistryIterator iter = _windowRegistry.begin();
			iter != _windowRegistry.end(); iter++)
		{
			GlowSubwindow* subwind = (*iter).second;
			if (subwind->IsTopLevel() && subwind != wind)
			{
				subwind->_BroadcastStandby(false, false);
			}
		}
	}
	else
	{
		// There's a modal window already. Deactivate that one and
		// activate the new modal window
		_modalWindows.back()->_BroadcastStandby(false, false);
		wind->_BroadcastStandby(true, false);
	}
	// push window onto stack, and bring to front
	_modalWindows.push_back(wind);
	wind->Raise();
}


void Glow::PopModalWindow()
{
	GLOW_DEBUGSCOPE("Glow::PopModalWindow");
	
	// Make sure there's a modal window.
	if (!_modalWindows.empty())
	{
		// pop modal window off stack
		GlowWindow* wind = _modalWindows.back();
		_modalWindows.pop_back();
		if (_modalWindows.empty())
		{
			// This was the last modal window. Reactivate windows
			for (_WindowRegistryIterator iter = _windowRegistry.begin();
				iter != _windowRegistry.end(); iter++)
			{
				GlowSubwindow* subwind = (*iter).second;
				if (subwind->IsTopLevel() && subwind != wind)
				{
					subwind->_BroadcastStandby(true, false);
				}
			}
		}
		else
		{
			// There are more windows on the stack
			// Deactivate popped window and reactivate next modal window
			wind->_BroadcastStandby(false, false);
			_modalWindows.back()->_BroadcastStandby(true, false);
			_modalWindows.back()->Raise();
		}
	}
}


/*
===============================================================================
	Miscellaneous
===============================================================================
*/

bool Glow::IsExtensionSupported(
	const char* extensionName)
{
	GLOW_DEBUGSCOPE("Glow::IsExtensionSupported");
	
	// If there's no window, there's no OpenGL context.
	// We'll also provide a default behavior: for no windows we return false.
	// If there's just no current window, we'll use the first one registered.
	if (::glutGetWindow() == 0)
	{
		GLOW_DEBUG(_windowRegistry.empty(),
			"Glow::IsExtensionSupported() called when no windows are open");
		if (_windowRegistry.empty()) return false;
		::glutSetWindow((*(_windowRegistry.begin())).first);
	}
	
	// Need to copy the string because glutExtensionSupported wants a
	// non-const pointer.
	char* buf = new char[GLOW_CSTD::strlen(extensionName)+1];
	GLOW_CSTD::strcpy(buf, extensionName);
	bool ret = (::glutExtensionSupported(buf) != 0);
	delete[] buf;
	return ret;
}


bool Glow::IsBufferTypeSupported(
	BufferType mode)
{
	GLOW_DEBUGSCOPE("Glow::IsBufferTypeSupported");
	
	// Test GLUT_DISPLAY_MODE_POSSIBLE. Preserve the old mode.
	int oldMode = ::glutGet(GLenum(GLUT_INIT_DISPLAY_MODE));
	::glutInitDisplayMode(mode);
	bool ret = (::glutGet(GLenum(GLUT_DISPLAY_MODE_POSSIBLE)) != 0);
	::glutInitDisplayMode(oldMode);
	return ret;
}


void Glow::RefreshGlutWindow(
	int id)
{
	GLOW_DEBUGSCOPE("Glow::RefreshGlutWindow");
	
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	if (_curDisplayWindow == id)
	{
		// Workaround for apparent GLUT bug that affects re-posting refresh events
		// from within a display function. Instead, we defer posting the refresh
		// until the next event handled. (We register an idle event to make sure
		// another event is raised.)
		if (_refreshMe == 0)
		{
			_refreshMe = id;
			if (_idleSender.NumReceivers() == 0)
			{
				::glutIdleFunc(_IdleFunc);
			}
		}
	}
	else
	{
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 11)
		// According to the GLUT spec, this version is optimized by
		// not requiring an OpenGL context switch.
		::glutPostWindowRedisplay(id);
#else
		int saveWind = ::glutGetWindow();
		if (saveWind != id)
		{
			::glutSetWindow(id);
		}
		::glutPostRedisplay();
		if (saveWind != 0 && saveWind != id)
		{
			::glutSetWindow(saveWind);
		}
#endif
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
	}
#endif
}


/*
===============================================================================
	Methods for GlowComponent
===============================================================================
*/

void GlowComponent::Init(
	GlowComponent* parent)
{
	GLOW_DEBUGSCOPE("GlowComponent::Init");
	
	_activeState = 1;
	_firstChild = _lastChild = 0;
	_prev = _next = 0;
	_numChildren = 0;
	_parent = 0;
	if (parent != 0)
	{
		parent->_AddChild(this);
		if (!parent->IsActive())
		{
			_activeState = 2;
		}
	}
	_initializeState = 1;
}


GlowComponent::~GlowComponent()
{
	GLOW_DEBUGSCOPE("GlowComponent::~GlowComponent");
	
	KillChildren();
	if (_parent != 0)
	{
		_parent->_RemoveChild(this);
	}
	GLOW_STD::map<GlowComponent*, bool>::iterator iter =
		Glow::_activateNotifyList.find(this);
	if (iter != Glow::_activateNotifyList.end())
	{
		Glow::_activateNotifyList.erase(iter);
	}
	iter = Glow::_closeList.find(this);
	if (iter != Glow::_closeList.end())
	{
		Glow::_closeList.erase(iter);
	}
}


void GlowComponent::Close()
{
	GLOW_DEBUGSCOPE("GlowComponent::Close");
	
	GLOW_STD::map<GlowComponent*, bool>::iterator iter =
		Glow::_closeList.find(this);
	if (iter == Glow::_closeList.end())
	{
		Glow::_closeList.insert(
			GLOW_STD::pair<GlowComponent* const, bool>(this, true));
	}
	_initializeState = 0;
}


void GlowComponent::_AddChild(
	GlowComponent* child)
{
	GLOW_DEBUGSCOPE("GlowComponent::_AddChild");
	GLOW_ASSERT(child->_parent == 0);
	
	child->_parent = this;
	if (_firstChild == 0)
	{
		_firstChild = child;
	}
	else
	{
		_lastChild->_next = child;
	}
	child->_prev = _lastChild;
	child->_next = 0;
	_lastChild = child;
	++_numChildren;
}


void GlowComponent::_RemoveChild(
	GlowComponent* child)
{
	GLOW_DEBUGSCOPE("GlowComponent::_RemoveChild");
	GLOW_ASSERT(child->_parent == this);
	
	if (child->_next == 0)
	{
		_lastChild = child->_prev;
	}
	else
	{
		child->_next->_prev = child->_prev;
	}
	if (child->_prev == 0)
	{
		_firstChild = child->_next;
	}
	else
	{
		child->_prev->_next = child->_next;
	}
	child->_prev = child->_next = child->_parent = 0;
	--_numChildren;
}


void GlowComponent::Paint()
{
	GLOW_DEBUGSCOPE("GlowComponent::Paint");
	
	if (OnBeginPaint())
	{
		for (GlowComponent* child = _firstChild; child != 0; child = child->Next())
		{
			if (dynamic_cast<GlowSubwindow*>(child) == 0)
			{
				(child)->Paint();
			}
		}
	}
	OnEndPaint();
}


void GlowComponent::_BroadcastStandby(
	bool activating,
	bool first)
{
	GLOW_DEBUGSCOPE("GlowComponent::_BroadcastStandby");
	
	bool changing = false;
	
	if (activating && (_activeState == 2 || (first && _activeState == 0)))
	{
		changing = true;
		_activeState = 1;
	}
	else if (!activating && _activeState == 1)
	{
		changing = true;
		_activeState = (first ? 0 : 2);
	}
	if (changing)
	{
		GlowSubwindow* subwindow = dynamic_cast<GlowSubwindow*>(this);
		if (subwindow != 0)
		{
			subwindow->_EventsForActivation(activating);
		}
		GLOW_STD::map<GlowComponent*, bool>::iterator iter =
			Glow::_activateNotifyList.find(this);
		if (iter != Glow::_activateNotifyList.end())
		{
			if (activating != (*iter).second)
			{
				Glow::_activateNotifyList.erase(iter);
			}
		}
		else
		{
			Glow::_activateNotifyList.insert(
				GLOW_STD::pair<GlowComponent* const, bool>(this, activating));
		}
		for (GlowComponent* child = _firstChild; child != 0; child = child->Next())
		{
			child->_BroadcastStandby(activating, false);
		}
		WhichWindow()->Refresh();
	}
}


void GlowComponent::Activate()
{
	GLOW_DEBUGSCOPE("GlowComponent::Activate");
	
	if (_activeState == 0)
	{
		if ((IsTopLevel() && (Glow::NumModalWindows() == 0 || Glow::TopModalWindow() == this)) ||
			(!IsTopLevel() && Parent()->IsActive()))
		{
			_BroadcastStandby(true, true);
		}
		else
		{
			_activeState = 2;
		}
	}
}


void GlowComponent::Deactivate()
{
	GLOW_DEBUGSCOPE("GlowComponent::Deactivate");
	
	if (_activeState == 1)
	{
		_BroadcastStandby(false, true);
	}
	else if (_activeState == 2)
	{
		_activeState = 0;
	}
}


GlowSubwindow* GlowComponent::WhichWindow()
{
	return _parent->WhichWindow();
}


GlowWindow* GlowComponent::ToplevelWindow()
{
	return (_parent == 0) ? static_cast<GlowWindow*>(this) : _parent->ToplevelWindow();
}


void GlowComponent::ReorderChild(
	GlowComponent* child,
	GlowComponent* before)
{
	GLOW_DEBUGSCOPE("GlowComponent::ReorderChild");
	GLOW_ASSERT(child->Parent() == this);
	GLOW_ASSERT(before->Parent() == this);
	
	if (child == before) return;
	
	_RemoveChild(child);
	if (before == 0)
	{
		_AddChild(child);
	}
	else if (before == _firstChild)
	{
		_firstChild->_prev = child;
		child->_next = _firstChild;
		child->_prev = 0;
		_firstChild = child;
		child->_parent = this;
		++_numChildren;
	}
	else
	{
		child->_next = before;
		child->_prev = before->_prev;
		before->_prev = child;
		child->_prev->_next = child;
		child->_parent = this;
		++_numChildren;
	}
}


void GlowComponent::KillChildren()
{
	GLOW_DEBUGSCOPE("GlowComponent::KillChildren");
	
	while (_numChildren > 0)
	{
		delete _firstChild;
	}
}


bool GlowComponent::OnBeginPaint()
{
	return true;
}


void GlowComponent::OnEndPaint()
{
}


void GlowComponent::OnActivate()
{
}


void GlowComponent::OnDeactivate()
{
}


/*
===============================================================================
	Methods for GlowSubwindow
===============================================================================
*/


void GlowSubwindow::Init(
	GlowComponent* parent,
	const GlowSubwindowParams& params)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Init");
	
	GLOW_DEBUG(params.width != parentWindowSize && params.width <= 0,
		"Subwindow width too small");
	GLOW_DEBUG(params.height != parentWindowSize && params.height <= 0,
		"Subwindow height too small");
	
	GlowComponent::Init(parent);
	::glutInitDisplayMode(params.mode);
	_width = params.width;
	if (_width == parentWindowSize)
	{
		_width = parent->WhichWindow()->Width() - params.x;
	}
	_height = params.height;
	if (_height == parentWindowSize)
	{
		_height = parent->WhichWindow()->Height() - params.y;
	}
	_windowNum = ::glutCreateSubWindow(parent->WhichWindow()->_windowNum,
		params.x, params.y, _width, _height);
	_eventMask = params.eventMask;
	_inactiveEventMask = params.inactiveEventMask;
	_bufferType = params.mode;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_RegisterCallbacks(_eventMask);
	_saveCursor = GLUT_CURSOR_INHERIT;
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((params.mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
}


void GlowSubwindow::Init(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Init");
	
	GLOW_DEBUG(width != parentWindowSize && width <= 0,
		"Subwindow width too small");
	GLOW_DEBUG(height != parentWindowSize && height <= 0,
		"Subwindow height too small");
	
	GlowComponent::Init(parent);
	::glutInitDisplayMode(mode);
	if (width == parentWindowSize)
	{
		width = parent->WhichWindow()->Width() - x;
	}
	if (height == parentWindowSize)
	{
		height = parent->WhichWindow()->Height() - y;
	}
	_windowNum = ::glutCreateSubWindow(parent->WhichWindow()->_windowNum,
		x, y, width, height);
	_width = width;
	_height = height;
	_eventMask = eventMask;
	_inactiveEventMask = GlowSubwindowParams::defaults.inactiveEventMask;
	_bufferType = mode;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_RegisterCallbacks(_eventMask);
	_saveCursor = GLUT_CURSOR_INHERIT;
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
}


GlowSubwindow::~GlowSubwindow()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::~GlowSubwindow");
	
	KillChildren();
	Glow::_RemoveWindow(_windowNum);
	::glutDestroyWindow(_windowNum);
}


void GlowSubwindow::_RegisterCallbacks(
	Glow::EventMask eventMask)
{
	::glutDisplayFunc(Glow::_DisplayFunc);
	::glutReshapeFunc(Glow::_ReshapeFunc);
	
	if (eventMask & Glow::keyboardEvents)
	{
		::glutKeyboardFunc(Glow::_KeyboardFunc);
		::glutSpecialFunc(Glow::_SpecialFunc);
	}
	else
	{
		::glutKeyboardFunc(0);
		::glutSpecialFunc(0);
	}
	if (eventMask & Glow::mouseEvents)
	{
		::glutMouseFunc(Glow::_MouseFunc);
	}
	else
	{
		::glutMouseFunc(0);
	}
	if (eventMask & Glow::dragEvents)
	{
		::glutMotionFunc(Glow::_MotionFunc);
	}
	else
	{
		::glutMotionFunc(0);
	}
	if (eventMask & Glow::motionEvents)
	{
		::glutPassiveMotionFunc(Glow::_PassiveMotionFunc);
	}
	else
	{
		::glutPassiveMotionFunc(0);
	}
	if (eventMask & Glow::visibilityEvents)
	{
		::glutVisibilityFunc(Glow::_VisibilityFunc);
	}
	else
	{
		::glutVisibilityFunc(0);
	}
	if (eventMask & Glow::focusEvents)
	{
		::glutEntryFunc(Glow::_EntryFunc);
	}
	else
	{
		::glutEntryFunc(0);
	}
	if (eventMask & Glow::menuEvents)
	{
		int saveMenu = ::glutGetMenu();
		if (_leftMenu != 0)
		{
			::glutSetMenu(_leftMenu->_menuNum);
			::glutAttachMenu(Glow::leftButton);
		}
		if (_centerMenu != 0)
		{
			::glutSetMenu(_centerMenu->_menuNum);
			::glutAttachMenu(Glow::middleButton);
		}
		if (_rightMenu != 0)
		{
			::glutSetMenu(_rightMenu->_menuNum);
			::glutAttachMenu(Glow::rightButton);
		}
		if (saveMenu != 0)
		{
			::glutSetMenu(saveMenu);
		}
	}
	else
	{
		if (_leftMenu != 0)
		{
			::glutDetachMenu(Glow::leftButton);
		}
		if (_centerMenu != 0)
		{
			::glutDetachMenu(Glow::middleButton);
		}
		if (_rightMenu != 0)
		{
			::glutDetachMenu(Glow::rightButton);
		}
	}
}


GlowSubwindow* GlowSubwindow::WhichWindow()
{
	return this;
}


bool GlowSubwindow::OnBeginPaint()
{
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}


void GlowSubwindow::OnReshape(
	int width,
	int height)
{
	::glViewport(0, 0, width, height);
}


void GlowSubwindow::OnMouseEnter()
{
}


void GlowSubwindow::OnMouseExit()
{
}


void GlowSubwindow::OnMouseDown(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowSubwindow::OnMouseUp(
	Glow::MouseButton button,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowSubwindow::OnMenuDown(
	int x,
	int y)
{
}


void GlowSubwindow::OnMenuUp()
{
}


void GlowSubwindow::OnMouseMotion(
	int x,
	int y)
{
}


void GlowSubwindow::OnMouseDrag(
	int x,
	int y)
{
}


void GlowSubwindow::OnKeyboard(
	Glow::KeyCode key,
	int x,
	int y,
	Glow::Modifiers modifiers)
{
}


void GlowSubwindow::OnVisible()
{
}


void GlowSubwindow::OnInvisible()
{
}


void GlowSubwindow::OnDirectMenuHit(
	const GlowMenuMessage& message)
{
}


void GlowSubwindow::SetEventMask(
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetEventMask");
	
	_eventMask = eventMask;
	if (IsActive())
	{
		int saveWind = ::glutGetWindow();
		if (saveWind != _windowNum)
		{
			::glutSetWindow(_windowNum);
		}
		_RegisterCallbacks(eventMask);
		if (saveWind != 0 && saveWind != _windowNum)
		{
			::glutSetWindow(saveWind);
		}
	}
}


void GlowSubwindow::SetInactiveEventMask(
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetInactiveEventMask");
	
	_inactiveEventMask = eventMask;
	if (!IsActive())
	{
		int saveWind = ::glutGetWindow();
		if (saveWind != _windowNum)
		{
			::glutSetWindow(_windowNum);
		}
		_RegisterCallbacks(eventMask);
		if (saveWind != 0 && saveWind != _windowNum)
		{
			::glutSetWindow(saveWind);
		}
	}
}


void GlowSubwindow::Move(
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Move");
	
	_clock = Glow::_clock;
	_globalXPos = x;
	_globalYPos = y;
	GlowSubwindow* parent = ParentWindow();
	if (parent != 0)
	{
		_globalXPos += parent->GlobalPositionX();
		_globalYPos += parent->GlobalPositionY();
	}
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutPositionWindow(x, y);
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Reshape(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Reshape");
	
	GLOW_ASSERT(width > 0);
	GLOW_ASSERT(height > 0);
	
	_width = width;
	_height = height;
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutReshapeWindow(width, height);
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Raise()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Raise");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutPopWindow();
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Lower()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Lower");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutPushWindow();
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Show()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Show");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutShowWindow();
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::Hide()
{
	GLOW_DEBUGSCOPE("GlowSubwindow::Hide");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutHideWindow();
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::SetCursor(
	int cursor)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetCursor");
	
	_saveCursor = cursor;
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutSetCursor(cursor);
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowSubwindow::SetMenu(
	Glow::MouseButton button,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowSubwindow::SetMenu");
	
	if (button == Glow::leftButton)
	{
		_leftMenu = menu;
	}
	else if (button == Glow::middleButton)
	{
		_centerMenu = menu;
	}
	else //if (button == Glow::rightButton)
	{
		_rightMenu = menu;
	}
	if (menu != 0)
	{
		if ((IsActive() && (_eventMask & Glow::menuEvents) != 0) ||
			(!IsActive() && (_inactiveEventMask & Glow::menuEvents) != 0))
		{
			int saveWind = ::glutGetWindow();
			if (saveWind != _windowNum)
			{
				::glutSetWindow(_windowNum);
			}
			int saveMenu = ::glutGetMenu();
			if (saveMenu != menu->_menuNum)
			{
				::glutSetMenu(menu->_menuNum);
			}
			::glutAttachMenu(button);
			if (saveWind != 0 && saveWind != _windowNum)
			{
				::glutSetWindow(saveWind);
			}
			if (saveMenu != 0 && saveMenu != menu->_menuNum)
			{
				::glutSetMenu(saveMenu);
			}
		}
	}
	else
	{
		int saveWind = ::glutGetWindow();
		if (saveWind != _windowNum)
		{
			::glutSetWindow(_windowNum);
		}
		::glutDetachMenu(button);
		if (saveWind != 0 && saveWind != _windowNum)
		{
			::glutSetWindow(saveWind);
		}
	}
}


int GlowSubwindow::GlutInfo(
	GLenum key) const
{
	GLOW_DEBUGSCOPE("GlowSubwindow::GlutInfo");
	
	int ret;
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	ret = ::glutGet(key);
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
	return ret;
}


void GlowSubwindow::_EventsForActivation(
	bool activating)
{
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	if (activating)
	{
		::glutSetCursor(_saveCursor);
		_RegisterCallbacks(_eventMask);
	}
	else
	{
		::glutSetCursor(GLUT_CURSOR_INHERIT);
		_RegisterCallbacks(_inactiveEventMask);
	}
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
	Refresh();
}


/*
===============================================================================
	Methods for GlowWindow
===============================================================================
*/

void GlowWindow::Init(
	const GlowWindowParams& params)
{
	GLOW_DEBUGSCOPE("GlowWindow::Init");
	
	GLOW_ASSERT(params.width > 0);
	GLOW_ASSERT(params.height > 0);
	
	GlowComponent::Init(0);
	_eventMask = params.eventMask;
	_inactiveEventMask = params.inactiveEventMask;
	_bufferType = params.mode;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_saveCursor = GLUT_CURSOR_INHERIT;
	_width = params.width;
	_height = params.height;
	_title = new char[GLOW_CSTD::strlen(params.title)+1];
	GLOW_CSTD::strcpy(_title, params.title);
	if (params.iconTitle == 0)
	{
		_iconTitle = new char[GLOW_CSTD::strlen(params.title)+1];
		GLOW_CSTD::strcpy(_iconTitle, params.title);
	}
	else
	{
		_iconTitle = new char[GLOW_CSTD::strlen(params.iconTitle)+1];
		GLOW_CSTD::strcpy(_iconTitle, params.iconTitle);
	}
	::glutInitWindowSize(params.width, params.height);
	::glutInitWindowPosition(params.x, params.y);
	::glutInitDisplayMode(params.mode);
	_windowNum = ::glutCreateWindow(_title);
	::glutSetIconTitle(_iconTitle);
	_RegisterCallbacks(params.eventMask);
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((params.mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
	if (Glow::NumModalWindows() != 0)
	{
		_activeState = 2;
		Glow::TopModalWindow()->Raise();
	}
}


void GlowWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	GLOW_DEBUGSCOPE("GlowWindow::Init");
	
	GLOW_ASSERT(width > 0);
	GLOW_ASSERT(height > 0);
	
	GlowComponent::Init(0);
	_eventMask = eventMask;
	_inactiveEventMask = GlowWindowParams::defaults.inactiveEventMask;
	_bufferType = mode;
	_leftMenu = 0;
	_centerMenu = 0;
	_rightMenu = 0;
	_saveCursor = GLUT_CURSOR_INHERIT;
	_width = width;
	_height = height;
	int len = GLOW_CSTD::strlen(title)+1;
	_title = new char[len];
	GLOW_CSTD::strcpy(_title, title);
	_iconTitle = new char[len];
	GLOW_CSTD::strcpy(_iconTitle, title);
	::glutInitWindowSize(width, height);
	::glutInitWindowPosition(x, y);
	::glutInitDisplayMode(mode);
	_windowNum = ::glutCreateWindow(_title);
	_RegisterCallbacks(eventMask);
	Glow::_AddWindow(this, _windowNum);
	_needSwapBuffers = ((mode & Glow::doubleBuffer) != 0);
	_refreshEnabled = true;
	_autoSwapBuffers = true;
	_clock = Glow::_clock;
	_globalXPos = ::glutGet((GLenum)GLUT_WINDOW_X);
	_globalYPos = ::glutGet((GLenum)GLUT_WINDOW_Y);
	if (Glow::NumModalWindows() != 0)
	{
		_activeState = 2;
		Glow::TopModalWindow()->Raise();
	}
}


void GlowWindow::Maximize()
{
	GLOW_DEBUGSCOPE("GlowWindow::Maximize");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutFullScreen();
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowWindow::Iconify()
{
	GLOW_DEBUGSCOPE("GlowWindow::Iconify");
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	::glutIconifyWindow();
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowWindow::SetTitle(
	const char* name)
{
	GLOW_DEBUGSCOPE("GlowWindow::SetTitle");
	
	GLOW_ASSERT(name != 0);
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	delete[] _title;
	_title = new char[GLOW_CSTD::strlen(name)+1];
	GLOW_CSTD::strcpy(_title, name);
	::glutSetWindowTitle(_title);
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


void GlowWindow::SetIconTitle(
	const char* name)
{
	GLOW_DEBUGSCOPE("GlowWindow::SetIconTitle");
	
	GLOW_ASSERT(name != 0);
	
	int saveWind = ::glutGetWindow();
	if (saveWind != _windowNum)
	{
		::glutSetWindow(_windowNum);
	}
	delete[] _iconTitle;
	_iconTitle = new char[GLOW_CSTD::strlen(name)+1];
	GLOW_CSTD::strcpy(_iconTitle, name);
	::glutSetIconTitle(_iconTitle);
	if (saveWind != 0 && saveWind != _windowNum)
	{
		::glutSetWindow(saveWind);
	}
}


/*
===============================================================================
	Methods for GlowMenu
===============================================================================
*/

GlowMenu::GlowMenu()
{
	GLOW_DEBUGSCOPE("GlowMenu::GlowMenu");
	
	_menuNum = ::glutCreateMenu(Glow::_MenuFunc);
	Glow::_AddMenu(this, _menuNum);
	_bindState = bindNormal;
}


GlowMenu::~GlowMenu()
{
	GLOW_DEBUGSCOPE("GlowMenu::~GlowMenu");
	
	Glow::_RemoveMenu(_menuNum);
	::glutDestroyMenu(_menuNum);
	for (ItemIterator iter = _itemData.begin(); iter != _itemData.end(); ++iter)
	{
		delete[] (*iter).label;
		delete[] (*iter).mark;
	}
}


void GlowMenu::OnHit(
	int code,
	GlowSubwindow* window,
	int x,
	int y)
{
	GLOW_DEBUGSCOPE("GlowMenu::OnHit");
	
	if (_bindState != bindNone)
	{
		GlowMenuMessage msg;
		msg.code = code;
		msg.menu = this;
		msg.window = window;
		msg.x = x;
		msg.y = y;
		if (_bindState == bindSubwindow)
		{
			window->OnDirectMenuHit(msg);
		}
		else
		{
			_sender.Send(msg);
		}
	}
}


void GlowMenu::_SetupItem(
	int itemNum,
	int untilNum) const
{
	if (untilNum == -1) untilNum = itemNum+1;
	char buf[200];
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	ItemConstIterator iter = _itemData.begin()+itemNum;
	for (int i=itemNum; i<untilNum; ++i)
	{
		buf[0] = 0;
		if ((*iter).mark != 0)
		{
			GLOW_CSTD::strcpy(buf, (*iter).mark);
		}
		GLOW_CSTD::strcat(buf, (*iter).label);
		if ((*iter).subMenu != 0)
		{
			if (i < ::glutGet(GLenum(GLUT_MENU_NUM_ITEMS)))
			{
				::glutChangeToSubMenu(i+1, buf, (*iter).subMenu->_menuNum);
			}
			else
			{
				::glutAddSubMenu(buf, (*iter).subMenu->_menuNum);
			}
		}
		else
		{
			if (i < ::glutGet(GLenum(GLUT_MENU_NUM_ITEMS)))
			{
				::glutChangeToMenuEntry(i+1, buf, (*iter).code);
			}
			else
			{
				::glutAddMenuEntry(buf, (*iter).code);
			}
		}
		++iter;
	}
	
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


void GlowMenu::AddEntry(
	const char* label,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::AddEntry");
	
	Glow_MenuItem item;
	int labellen = GLOW_CSTD::strlen(label);
	item.label = new char[labellen+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = code;
	item.subMenu = 0;
	_itemData.push_back(item);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	::glutAddMenuEntry((char*)label, code);
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


void GlowMenu::AddSubmenu(
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::AddSubmenu");
	
	Glow_MenuItem item;
	int labellen = GLOW_CSTD::strlen(label);
	item.label = new char[labellen+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = 0;
	item.subMenu = menu;
	_itemData.push_back(item);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	::glutAddSubMenu((char*)label, menu->_menuNum);
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


void GlowMenu::InsertEntry(
	int itemNum,
	const char* label,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::InsertEntry");
	GLOW_DEBUG(itemNum<0 || itemNum>int(_itemData.size()),
		"itemNum out of range in GlowMenu::InsertEntry");
	
	Glow_MenuItem item;
	item.label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = code;
	item.subMenu = 0;
	_itemData.insert(_itemData.begin()+itemNum, item);
	
	_SetupItem(itemNum, _itemData.size());
}


void GlowMenu::InsertSubmenu(
	int itemNum,
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::InsertSubmenu");
	GLOW_DEBUG(itemNum<0 || itemNum>int(_itemData.size()),
		"itemNum out of range in GlowMenu::InsertSubmenu");
	
	Glow_MenuItem item;
	item.label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(item.label, label);
	item.mark = 0;
	item.code = 0;
	item.subMenu = menu;
	_itemData.insert(_itemData.begin()+itemNum, item);
	
	_SetupItem(itemNum, _itemData.size());
}


void GlowMenu::RemoveItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowMenu::RemoveItem");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	delete[] (*iter).mark;
	_itemData.erase(iter);
	
	int saveMenu = ::glutGetMenu();
	::glutSetMenu(_menuNum);
	::glutRemoveMenuItem(itemNum+1);
	if (saveMenu != 0)
	{
		::glutSetMenu(saveMenu);
	}
}


int GlowMenu::NextMarkedItem(
	int start) const
{
	GLOW_DEBUGSCOPE("GlowMenu::NextMarkedItem");
	
	GLOW_DEBUG(start<-1, "starting itemnum out of range");
	
	int max = _itemData.size();
	for (int i=start+1; i<max; i++)
	{
		if (_itemData[i].mark != 0)
			return i;
	}
	return -1;
}


void GlowMenu::SetItemLabel(
	int itemNum,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemLabel");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	GLOW_ASSERT(label);
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	_SetupItem(itemNum);
}


void GlowMenu::SetItemMark(
	int itemNum,
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemMark");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).mark;
	if (mark == 0)
	{
		(*iter).mark = 0;
	}
	else
	{
		int marklen = GLOW_CSTD::strlen(mark);
		(*iter).mark = new char[marklen+1];
		GLOW_CSTD::strcpy((*iter).mark, mark);
	}
	_SetupItem(itemNum);
}


void GlowMenu::ToggleItemMark(
	int itemNum,
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowMenu::ToggleItemMark");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	if ((*iter).mark == 0)
	{
		int marklen = GLOW_CSTD::strlen(mark);
		(*iter).mark = new char[marklen+1];
		GLOW_CSTD::strcpy((*iter).mark, mark);
	}
	else
	{
		delete[] (*iter).mark;
		(*iter).mark = 0;
	}
	_SetupItem(itemNum);
}


void GlowMenu::UnmarkItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowMenu::UnmarkItem");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).mark;
	(*iter).mark = 0;
	_SetupItem(itemNum);
}


void GlowMenu::UnmarkAllItems()
{
	GLOW_DEBUGSCOPE("GlowMenu::UnmarkAllItems");
	
	int itemNum = 0;
	for (ItemIterator iter = _itemData.begin(); iter != _itemData.end(); ++iter)
	{
		delete[] (*iter).mark;
		(*iter).mark = 0;
		_SetupItem(itemNum);
		itemNum++;
	}
}


void GlowMenu::ChangeToEntry(
	int itemNum,
	const char* label,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::ChangeToEntry");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	GLOW_ASSERT(label);
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	delete[] (*iter).mark;
	(*iter).mark = 0;
	(*iter).code = code;
	(*iter).subMenu = 0;
	_SetupItem(itemNum);
}


void GlowMenu::SetItemCode(
	int itemNum,
	int code)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemCode");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	(*iter).code = code;
	(*iter).subMenu = 0;
	_SetupItem(itemNum);
}


void GlowMenu::SetCodesToItemNumbers()
{
	GLOW_DEBUGSCOPE("GlowMenu::SetCodesToItemNumbers");
	
	int mx = _itemData.size();
	for (int i=0; i<mx; ++i)
	{
		if (_itemData[i].subMenu == 0)
		{
			_itemData[i].code = i;
		}
	}
}


void GlowMenu::ChangeToSubmenu(
	int itemNum,
	const char* label,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::ChangeToSubmenu");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	delete[] (*iter).label;
	int labellen = GLOW_CSTD::strlen(label);
	(*iter).label = new char[labellen+1];
	GLOW_CSTD::strcpy((*iter).label, label);
	delete[] (*iter).mark;
	(*iter).mark = 0;
	(*iter).code = 0;
	(*iter).subMenu = menu;
	_SetupItem(itemNum);
}


void GlowMenu::SetItemSubmenu(
	int itemNum,
	GlowMenu* menu)
{
	GLOW_DEBUGSCOPE("GlowMenu::SetItemSubmenu");
	
	GLOW_DEBUG(itemNum<0 || itemNum>=int(_itemData.size()),
		"itemnum out of range");
	
	ItemIterator iter = _itemData.begin()+itemNum;
	(*iter).code = 0;
	(*iter).subMenu = menu;
	_SetupItem(itemNum);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

