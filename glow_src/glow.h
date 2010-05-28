/*
===============================================================================

	FILE:  glow.h
	
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


#ifndef GLOW__H
#define GLOW__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

// Microsoft cluelessness: the win32 gl.h depends on windows.h but
// doesn't include it.
#if defined(_WIN32) || defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#ifdef GLOW_COMPAT_NOINCLUDESUBDIRECTORIES
	#include <glut.h>
#else
	#include <GL/glut.h>
#endif
#include <map>
#include <vector>

#ifndef GLOW_SENDERRECEIVER__H
	#include "glowSenderReceiver.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Declarations
===============================================================================
*/

class Glow;
class GlowComponent;
class GlowSubwindow;
class GlowWindow;
class GlowMenu;
class GlowSubwindowParams;
class GlowWindowParams;
class GlowKeyboardData;
class GlowMouseData;
class GlowKeyboardFilter;
class GlowMouseFilter;


/*
===============================================================================
	Message types
===============================================================================
*/

class GlowIdleMessage
{
	friend class Glow;
	
	private:
	
		inline GlowIdleMessage();
};


class GlowTimerMessage
{
	friend class Glow;
	
	private:
	
		inline GlowTimerMessage(
			int i);
	
	public:
	
		int id;
};


typedef TReceiver<const GlowIdleMessage&> GlowIdleReceiver;
typedef TSender<const GlowIdleMessage&> Glow_IdleSender;

typedef TReceiver<const GlowTimerMessage&> GlowTimerReceiver;
typedef TSender<const GlowTimerMessage&> Glow_TimerSender;


/*
===============================================================================
	CLASS Glow

	General utility routines
===============================================================================
*/

class Glow_IdleFuncReceiver;

class Glow
{
	friend class GlowWindow;
	friend class GlowComponent;
	friend class GlowSubwindow;
	friend class GlowMenu;
	friend class GlowWidget;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// Event masks
		enum EventMask
		{
			noEvents = 0x0000,
			keyboardEvents = 0x0001,
			mouseEvents = 0x0002,
			dragEvents = 0x0004,
			motionEvents = 0x0008,
			visibilityEvents = 0x0010,
			focusEvents = 0x0020,
			menuEvents = 0x0040,
			allEvents = 0xffff
		};
		
		// Mouse button specification
		enum MouseButton
		{
			leftButton = GLUT_LEFT_BUTTON,
			middleButton = GLUT_MIDDLE_BUTTON,
			rightButton = GLUT_RIGHT_BUTTON
		};
		
		// Frame buffer type
		enum BufferType
		{
			noBuffer = 0,
			rgbBuffer = GLUT_RGB,
			alphaBuffer = GLUT_ALPHA,
			rgbaBuffer = GLUT_RGBA | GLUT_ALPHA,
			stencilBuffer = GLUT_STENCIL,
			depthBuffer = GLUT_DEPTH,
			accumBuffer = GLUT_ACCUM,
			doubleBuffer = GLUT_DOUBLE,
			stereoBuffer = GLUT_STEREO,
			multisampleBuffer = GLUT_MULTISAMPLE
		};
		
		// Keyboard modifiers
		enum Modifiers
		{
			noModifier = 0,
			shiftModifier = GLUT_ACTIVE_SHIFT,
			ctrlModifier = GLUT_ACTIVE_CTRL,
			altModifier = GLUT_ACTIVE_ALT
		};
		
		// Special key constants
		//typedef int KeyCode;
		enum KeyCode
		{
			backspaceKey = 8,
			tabKey = 9,
			enterKey = 13,
			escapeKey = 27,
			deleteKey = 127,
			specialKeyOffset = 256,
			f1Key = specialKeyOffset+GLUT_KEY_F1,
			f2Key = specialKeyOffset+GLUT_KEY_F2,
			f3Key = specialKeyOffset+GLUT_KEY_F3,
			f4Key = specialKeyOffset+GLUT_KEY_F4,
			f5Key = specialKeyOffset+GLUT_KEY_F5,
			f6Key = specialKeyOffset+GLUT_KEY_F6,
			f7Key = specialKeyOffset+GLUT_KEY_F7,
			f8Key = specialKeyOffset+GLUT_KEY_F8,
			f9Key = specialKeyOffset+GLUT_KEY_F9,
			f10Key = specialKeyOffset+GLUT_KEY_F10,
			f11Key = specialKeyOffset+GLUT_KEY_F11,
			f12Key = specialKeyOffset+GLUT_KEY_F12,
			leftArrowKey = specialKeyOffset+GLUT_KEY_LEFT,
			upArrowKey = specialKeyOffset+GLUT_KEY_UP,
			rightArrowKey = specialKeyOffset+GLUT_KEY_RIGHT,
			downArrowKey = specialKeyOffset+GLUT_KEY_DOWN,
			pageUpKey = specialKeyOffset+GLUT_KEY_PAGE_UP,
			pageDownKey = specialKeyOffset+GLUT_KEY_PAGE_DOWN,
			homeKey = specialKeyOffset+GLUT_KEY_HOME,
			endKey = specialKeyOffset+GLUT_KEY_END,
			insertKey = specialKeyOffset+GLUT_KEY_INSERT
		};
	
	public:

		// General
		static void Init(
			int& argc,
			char** argv);
                static bool isGLInit;
		static void MainLoop();
		static double Version();
		static int APIVersion();
		
		// Idle tasks
		static void RegisterIdle(
			GlowIdleReceiver* idle);
		static void UnregisterIdle(
			GlowIdleReceiver* idle);
		inline static void UnregisterAllIdle();
		inline static unsigned int NumRegisteredIdle();
		inline static bool IsIdleRegistered(
			GlowIdleReceiver* idle);
		
		// Timer tasks
		static int RegisterTimer(
			unsigned int msecs,
			GlowTimerReceiver* timer);
		static void UnregisterTimer(
			int id);
		inline static bool IsTimerPending(
			int id);
		
		// Filters
		inline static void RegisterFilter(
			GlowMouseFilter* filter);
		inline static void RegisterFilter(
			GlowKeyboardFilter* filter);
		inline static void UnregisterFilter(
			GlowMouseFilter* filter);
		inline static void UnregisterFilter(
			GlowKeyboardFilter* filter);
		inline static void UnregisterAllMouseFilters();
		inline static void UnregisterAllKeyboardFilters();
		inline static unsigned int NumRegisteredMouseFilters();
		inline static unsigned int NumRegisteredKeyboardFilters();
		inline static bool IsFilterRegistered(
			GlowMouseFilter* filter);
		inline static bool IsFilterRegistered(
			GlowKeyboardFilter* filter);
		
		// Resolution of window and menu ids
		static GlowSubwindow* ResolveWindow(
			int windowNum);
		static GlowMenu* ResolveMenu(
			int menuNum);
		
		// Modal windows
		static void PushModalWindow(
			GlowWindow* wind);
		static void PopModalWindow();
		inline static int NumModalWindows();
		inline static GlowWindow* TopModalWindow();
		
		// Event simulation
		inline static void DeliverKeyboardEvt(
			GlowSubwindow* receiver,
			Glow::KeyCode key,
			int x,
			int y,
			Modifiers modifiers);
		inline static void DeliverMouseDownEvt(
			GlowSubwindow* receiver,
			Glow::MouseButton button,
			int x,
			int y,
			Modifiers modifiers);
		inline static void DeliverMouseUpEvt(
			GlowSubwindow* receiver,
			Glow::MouseButton button,
			int x,
			int y,
			Modifiers modifiers);
		inline static void DeliverMouseDragEvt(
			GlowSubwindow* receiver,
			int x,
			int y);
		inline static void DeliverMouseMotionEvt(
			GlowSubwindow* receiver,
			int x,
			int y);
		inline static void DeliverMouseEnterEvt(
			GlowSubwindow* receiver);
		inline static void DeliverMouseExitEvt(
			GlowSubwindow* receiver);
		
		// Drawing
		inline static void SwapBuffers();
		static void RefreshGlutWindow(
			int id);
		
		// GLUT compatibility
		static void SetIdleFunc(
			void (*func)());
		inline static void SetMenuStatusFunc(
			void (*func)(int status, int x, int y));
		
		// Miscellaneous state
		inline static bool IsMenuInUse();
		inline static int GetMilliseconds();
		static bool IsExtensionSupported(
			const char* extensionName);
		static bool IsBufferTypeSupported(
			BufferType mode);
		inline static int NumMouseButtons();
		
		// Toplevel window counter
		inline static int NumToplevelWindows();
		inline static bool IsAutoQuitting();
		inline static void SetAutoQuitting(
			bool autoQuit = true);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		typedef GLOW_STD::map<int, GlowSubwindow*>::value_type _WindowRegistryEntry;
		typedef GLOW_STD::map<int, GlowMenu*>::value_type _MenuRegistryEntry;
		typedef GLOW_STD::map<int, GlowSubwindow*>::iterator _WindowRegistryIterator;
		typedef GLOW_STD::map<int, GlowMenu*>::iterator _MenuRegistryIterator;
	
	private:
	
		// Prevent instantiation
		inline Glow() {}
	
	private:
	
		// Map glut ids to window and menu pointers
		static GLOW_STD::map<int, GlowSubwindow*> _windowRegistry;
		static GLOW_STD::map<int, GlowMenu*> _menuRegistry;
		
		// List of deferred activate notifications
		static GLOW_STD::map<GlowComponent*, bool> _activateNotifyList;
		// List of deferred component deletions
		static GLOW_STD::map<GlowComponent*, bool> _closeList;
		// Function to notify for widgets
		static void (*_widgetNotifier)();
		// Used to sync up global position to glut's
		static unsigned long _clock;
		
		// Menu state
		static GlowSubwindow* _menuWindow;
		static int _menuXClick;
		static int _menuYClick;
		static bool _menuInUse;
		
		// Modal window stack
		static GLOW_STD::vector<GlowWindow*> _modalWindows;
		
		// Idle, timer and finalizer senders
		static TSender<const GlowIdleMessage&> _idleSender;
		static GLOW_STD::map<int, TSender<const GlowTimerMessage&>*> _timerSenders;
		
		// Next timer id to assign
		static int _nextTimerID;
		
		// GLUT compatibility callbacks
		static Glow_IdleFuncReceiver* _idleFuncReceiver;
		static void (*_userMenuStatusFunc)(int status, int x, int y);
		
		// Event filter senders
		static TSender<GlowMouseData&> _mouseFilters;
		static TSender<GlowKeyboardData&> _keyboardFilters;
		
		// Toplevel window counter
		static int _numToplevelWindows;
		static bool _autoQuitting;
		
		// Workaround for GLUT redisplay bug
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		static int _curDisplayWindow;
		static int _refreshMe;
#endif
	
	private:
	
		// Manage glut id mappings
		static void _AddWindow(
			GlowSubwindow* window,
			int windowNum);
		static void _RemoveWindow(
			int windowNum);
		
		static void _AddMenu(
			GlowMenu* menu,
			int menuNum);
		static void _RemoveMenu(
			int menuNum);
		
		// Call backs
		static void _TimerFunc(
			int id);
		static void _DisplayFunc();
		static void _ReshapeFunc(
			int width,
			int height);
		static void _KeyboardFunc(
			unsigned char key,
			int x,
			int y);
		static void _MouseFunc(
			int button,
			int state,
			int x,
			int y);
		static void _MotionFunc(
			int x,
			int y);
		static void _PassiveMotionFunc(
			int x,
			int y);
		static void _VisibilityFunc(
			int state);
		static void _EntryFunc(
			int state);
		static void _SpecialFunc(
			int key,
			int x,
			int y);
		static void _MenuStatusFunc(
			int status,
			int x,
			int y);
		static void _MenuFunc(
			int value);
		static void _IdleFunc();
		
		// Deferred execution
		static void _ExecuteDeferred();
#ifdef GLOW_OPTION_GLUTREDISPLAYFIX
		static void _RaiseDeferredRefresh();
#endif
};

GLOW_INTERNAL_SETUPENUMBITFIELD(Glow::EventMask)
GLOW_INTERNAL_SETUPENUMBITFIELD(Glow::BufferType)
GLOW_INTERNAL_SETUPENUMBITFIELD(Glow::Modifiers)


/*
===============================================================================
	Event filters
===============================================================================
*/

class GlowKeyboardData
{
	friend class Glow;
	friend class GlowKeyboardFilter;
	
	public:
	
		GlowSubwindow* subwindow;
		Glow::KeyCode key;
		int x;
		int y;
		Glow::Modifiers modifiers;
	
	private:
	
		inline GlowKeyboardData();
	
	private:
	
		bool _continue;
};


class GlowMouseData
{
	friend class Glow;
	friend class GlowMouseFilter;
	
	public:
	
		enum EventType
		{
			mouseDown = 1,
			mouseUp = 2
		};
	
	public:
	
		GlowSubwindow* subwindow;
		EventType type;
		Glow::MouseButton button;
		int x;
		int y;
		Glow::Modifiers modifiers;
	
	private:
	
		inline GlowMouseData();
	
	private:
	
		bool _continue;
};


class GlowKeyboardFilter :
	public TReceiver<GlowKeyboardData&>
{
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual bool OnFilter(
			GlowKeyboardData& data) = 0;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage(
			GlowKeyboardData& message);
};


class GlowMouseFilter :
	public TReceiver<GlowMouseData&>
{
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual bool OnFilter(
			GlowMouseData& data) = 0;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage(
			GlowMouseData& message);
};


/*
===============================================================================
	CLASS GlowComponent

	User interface component class
===============================================================================
*/

class GlowComponent
{
	friend class GlowSubwindow;
	friend class GlowWindow;
	friend class Glow;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowComponent(
			GlowComponent* parent);
		inline GlowComponent();
		void Init(
			GlowComponent* parent);
		
		virtual ~GlowComponent();
		void Close();
		
		inline GlowComponent* Next() const;
		inline GlowComponent* Prev() const;
		
		inline GlowComponent* Parent() const;
		virtual GlowSubwindow* WhichWindow();
		inline GlowSubwindow* ParentWindow() const;
		GlowWindow* ToplevelWindow();
		inline bool IsTopLevel() const;
		
		inline int NumChildren() const;
		inline GlowComponent* FirstChild() const;
		inline GlowComponent* LastChild() const;
		void ReorderChild(
			GlowComponent* child,
			GlowComponent* before);
		void KillChildren();
		
		void Activate();
		void Deactivate();
		inline bool IsActive() const;
		inline bool IsActiveStandby() const;
		inline bool IsInactive() const;
		
		inline bool IsInitialized() const;
		inline bool IsClosing() const;
		
		void Paint();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnActivate();
		virtual void OnDeactivate();
	
	protected:
	
		virtual bool OnBeginPaint();
		virtual void OnEndPaint();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowComponent* _parent;
		GlowComponent* _next;
		GlowComponent* _prev;
		int _numChildren;
		GlowComponent* _firstChild;
		GlowComponent* _lastChild;
		short _activeState;
		short _initializeState;
	
	private:
	
		void _AddChild(
			GlowComponent* child);
		void _RemoveChild(
			GlowComponent* child);
		
		void _BroadcastStandby(
			bool activating,
			bool first);
};


/*
===============================================================================
	CLASS GlowMenuMessage

	Message sent on menu hit
===============================================================================
*/

class GlowMenuMessage
{
	public:
	
		int code;
		GlowMenu* menu;
		GlowSubwindow* window;
		int x;
		int y;
};


typedef TReceiver<const GlowMenuMessage&> GlowMenuReceiver;


/*
===============================================================================
	CLASS GlowSubwindow

	User interface component that can receive events
===============================================================================
*/

class GlowSubwindow :
	public GlowComponent
{
	friend class GlowWindow;
	friend class Glow;
	friend class GlowMenu;
	friend class GlowComponent;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum {
			parentWindowSize = -1
		};
	
	public:
	
		inline GlowSubwindow();
		inline GlowSubwindow(
			GlowComponent* parent,
			const GlowSubwindowParams& params);
		inline GlowSubwindow(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
		void Init(
			GlowComponent* parent,
			const GlowSubwindowParams& params);
		void Init(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
		
		virtual ~GlowSubwindow();
	
	public:
	
		inline void MakeCurGlutWindow();
		int GlutInfo(
			GLenum key) const;
		inline int GlutWindowNum() const;
		
		inline void Refresh();
		inline bool IsRefreshEnabled() const;
		inline void SetRefreshEnabled(
			bool enable);
		inline bool IsAutoSwapBuffersEnabled() const;
		inline void SetAutoSwapBuffersEnabled(
			bool enable);
		
		void Move(
			int x,
			int y);
		void Reshape(
			int width,
			int height);
		void Raise();
		void Lower();
		void Show();
		void Hide();
		
		void SetMenu(
			Glow::MouseButton button,
			GlowMenu* menu = 0);
		inline GlowMenu* GetMenu(
			Glow::MouseButton button) const;
		inline void UnsetMenu(
			Glow::MouseButton button);
		
		inline int PositionX() const;
		inline int PositionY() const;
		inline int GlobalPositionX() const;
		inline int GlobalPositionY() const;
		inline int Width() const;
		inline int Height() const;
		inline void NormalizeCoordinates(
			int x,
			int y,
			GLfloat& xn,
			GLfloat& yn) const;
		virtual GlowSubwindow* WhichWindow();
		
		inline int GetCursor() const;
		void SetCursor(
			int cursor = GLUT_CURSOR_INHERIT);
		inline Glow::EventMask GetEventMask() const;
		void SetEventMask(
			Glow::EventMask eventMask);
		inline Glow::EventMask GetInactiveEventMask() const;
		void SetInactiveEventMask(
			Glow::EventMask eventMask);
		inline Glow::BufferType GetBufferType() const;
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnReshape(
			int width,
			int height);
		virtual void OnMouseEnter();
		virtual void OnMouseExit();
		virtual void OnMouseDown(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnMouseUp(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnMenuDown(
			int x,
			int y);
		virtual void OnMenuUp();
		virtual void OnMouseMotion(
			int x,
			int y);
		virtual void OnMouseDrag(
			int x,
			int y);
		virtual void OnKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnVisible();
		virtual void OnInvisible();
		virtual void OnDirectMenuHit(
			const GlowMenuMessage& message);
		virtual bool OnBeginPaint();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _width;
		int _height;
		int _windowNum;
		Glow::EventMask _eventMask;
		Glow::EventMask _inactiveEventMask;
		Glow::BufferType _bufferType;
		int _saveCursor;
		mutable unsigned long _clock;
		mutable int _globalXPos;
		mutable int _globalYPos;
		GlowMenu* _leftMenu;
		GlowMenu* _centerMenu;
		GlowMenu* _rightMenu;
		bool _needSwapBuffers;
		bool _refreshEnabled;
		bool _autoSwapBuffers;
	
	private:
	
		void _RegisterCallbacks(
			Glow::EventMask eventMask);
		inline void _FinishRender() const;
		void _EventsForActivation(
			bool activating);
};


/*
===============================================================================
	CLASS GlowWindow

	Top-level window
===============================================================================
*/

class GlowWindow :
	public GlowSubwindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum {
			autoPosition = -1
		};
	
	public:
	
		inline GlowWindow(
			const GlowWindowParams& params);
		inline GlowWindow(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
		inline GlowWindow();
		void Init(
			const GlowWindowParams& params);
		void Init(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
		
		void Maximize();
		void Iconify();
		
		inline const char* GetTitle() const;
		void SetTitle(
			const char* name);
		inline const char* GetIconTitle() const;
		void SetIconTitle(
			const char* name);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		char* _title;
		char* _iconTitle;
};


/*
===============================================================================
	Params
===============================================================================
*/

class GlowSubwindowParams
{
	public:
	
		int width;
		int height;
		int x;
		int y;
		Glow::EventMask eventMask;
		Glow::EventMask inactiveEventMask;
		Glow::BufferType mode;
		
		static GlowSubwindowParams defaults;
		
		GlowSubwindowParams();
	
	protected:
	
		GlowSubwindowParams(bool);
};


class GlowWindowParams :
	public GlowSubwindowParams
{
	public:
	
		const char* title;
		const char* iconTitle;
		
		static GlowWindowParams defaults;
		
		GlowWindowParams();
	
	protected:
	
		GlowWindowParams(bool);
};


/*
===============================================================================
	CLASS Glow_MenuItem

	Menu entry class
===============================================================================
*/

class Glow_MenuItem
{
	friend class GlowMenu;
	
	private:
	
		char* label;
		char* mark;
		int code;
		GlowMenu* subMenu;
};


/*
===============================================================================
	CLASS GlowMenu

	Menu class
	Note: item numbers are 0-based
===============================================================================
*/

class GlowMenu
{
	friend class Glow;
	friend class GlowSubwindow;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum BindState
		{
			bindNone = 0,
			bindNormal = 1,
			bindSubwindow = 2
		};
	
	public:
	
		GlowMenu();
		virtual ~GlowMenu();
	
	public:
	
		// Add and remove items
		void AddEntry(
			const char* label,
			int code);
		void AddSubmenu(
			const char* label,
			GlowMenu* menu);
		void InsertEntry(
			int itemNum,
			const char* label,
			int code);
		void InsertSubmenu(
			int itemNum,
			const char* label,
			GlowMenu* menu);
		void RemoveItem(
			int itemNum);
		inline int NumItems() const;
		
		// Manipulate label
		inline const char* GetItemLabel(
			int itemNum) const;
		void SetItemLabel(
			int itemNum,
			const char* label);
		
		// Manipulate mark
		inline const char* GetItemMark(
			int itemNum) const;
		void SetItemMark(
			int itemNum,
			const char* mark);
		void ToggleItemMark(
			int itemNum,
			const char* mark);
		void UnmarkItem(
			int itemNum);
		inline bool IsItemMarked(
			int itemNum) const;
		int NextMarkedItem(
			int itemNum) const;
		void UnmarkAllItems();
		
		// Manipulate code and submenu data
		inline int GetItemCode(
			int itemNum) const;
		void SetItemCode(
			int itemNum,
			int code);
		void SetCodesToItemNumbers();
		inline GlowMenu* GetItemSubmenu(
			int itemNum) const;
		void SetItemSubmenu(
			int itemNum,
			GlowMenu* menu);
		
		// Query and change item type
		inline bool IsEntry(
			int itemNum) const;
		inline bool IsSubmenu(
			int itemNum) const;
		void ChangeToEntry(
			int itemNum,
			const char* label,
			int code);
		void ChangeToSubmenu(
			int itemNum,
			const char* label,
			GlowMenu* menu);
		
		// Event reporting
		inline void SetBindState(
			BindState bindState);
		inline BindState GetBindState() const;
		inline TSender<const GlowMenuMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnHit(
			int code,
			GlowSubwindow* window,
			int x,
			int y);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _menuNum;
		BindState _bindState;
		GLOW_STD::vector<Glow_MenuItem> _itemData;
		TSender<const GlowMenuMessage&> _sender;
	
	private:
	
		typedef GLOW_STD::vector<Glow_MenuItem>::iterator ItemIterator;
		typedef GLOW_STD::vector<Glow_MenuItem>::const_iterator ItemConstIterator;
	
	private:
	
		void _SetupItem(
			int itemNum,
			int untilNum = -1) const;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glow.inl.h"


#endif

