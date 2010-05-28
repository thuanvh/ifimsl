/*
===============================================================================

	FILE:  glowWidget.h
	
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


#ifndef GLOW_WIDGET__H
#define GLOW_WIDGET__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <climits>

#ifndef GLOW__H
	#include "glow.h"
#endif
#ifndef GLOW_AUX__H
	#include "glowAux.h"
#endif
#ifndef GLOW_UTILITIES__H
	#include "glowUtilities.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Forwards
===============================================================================
*/

class GlowWidgetRoot;
class GlowWidget;


/*
===============================================================================
	CLASS GlowWidgetParams
	
	Widget params (base)
===============================================================================
*/

class GlowWidgetParams
{
	public:
	
		bool clipping;
		int x;
		int y;
		int width;
		int height;
		long refcon;
		
		inline GlowWidgetParams();
	
	protected:
	
		GlowWidgetParams(bool);
};


/*
===============================================================================
	CLASS GlowWidgetRootParams
	
	Widget root params
===============================================================================
*/

class GlowWidgetSubwindowParams :
	public GlowSubwindowParams
{
	public:
	
		GlowColor backColor;
		
		static GlowWidgetSubwindowParams defaults;
		
		GlowWidgetSubwindowParams();
	
	protected:
	
		GlowWidgetSubwindowParams(bool);
};


/*
===============================================================================
	CLASS GlowWidgetWindowParams
	
	Widget window params
===============================================================================
*/

class GlowWidgetWindowParams :
	public GlowWindowParams
{
	public:
	
		GlowColor backColor;
		
		static GlowWidgetWindowParams defaults;
		
		GlowWidgetWindowParams();
	
	protected:
	
		GlowWidgetWindowParams(bool);
};


/*
===============================================================================
	CLASS Glow_OldWidgetState

	Deferred widget operation info
===============================================================================
*/

class Glow_OldWidgetState
{
	friend class GlowWidget;
	
	private:
	
		inline Glow_OldWidgetState(
			bool vis,
			bool keyfoc,
			int wid,
			int hei,
			int xp,
			int yp);
		
		bool visible;
		bool keyboardFocus;
		int width;
		int height;
		int x;
		int y;
};


/*
===============================================================================
	CLASS GlowWidget
	
	Widget base class
===============================================================================
*/

class GlowWidget :
	public GlowComponent
{
	friend class GlowWidgetRoot;
	friend class Glow;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// Packing error codes
		enum AutoPackError
		{
			noAutoPackError = 0,
			hAutoPackError = 1,
			vAutoPackError = 2,
			_maximum = 255
		};
		
		// Sizing/justification options
		enum AutoPackOptions
		{
			noReshape = 0,
			preferredSize = 1,
			expandPreferredSize = 2,
			forcedSize = 3,
			sizeOptionMask = 15,
			noMove = 0,
			leftPos = 16,
			topPos = 16,
			rightPos = 32,
			bottomPos = 32,
			centerPos = 48,
			posOptionMask = 240
		};
		
		// Unspecified size/position
		enum {
			unspecifiedSize = INT_MIN,
			unspecifiedPos = INT_MIN
		};
	
	public:
	
		inline GlowWidget();
		inline GlowWidget(
			GlowWidgetRoot* root,
			const GlowWidgetParams& params);
		inline GlowWidget(
			GlowWidget* parent,
			const GlowWidgetParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowWidgetParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowWidgetParams& params);
		
		virtual ~GlowWidget();
	
	public:
	
		inline GlowWidgetRoot* Root() const;
		
		inline void Move(
			int x,
			int y);
		inline void Reshape(
			int width,
			int height);
		inline int Width() const;
		inline int Height() const;
		inline int PositionX() const;
		inline int PositionY() const;
		int RootPositionX() const;
		int RootPositionY() const;
		inline int GlobalPositionX() const;
		inline int GlobalPositionY() const;
		
		void Hide();
		void Show();
		inline bool IsVisible() const;
		inline bool IsVisibleMasked() const;
		inline bool IsInvisible() const;
		
		inline void GrabKeyboardFocus();
		inline void RelinquishKeyboardFocus();
		inline bool HasKeyboardFocus() const;
		
		inline bool IsClipping() const;
		inline void SetClipping(
			bool clipping);
		
		inline long GetRefCon() const;
		inline void SetRefCon(
			long refcon);
		
		inline void Refresh();
		inline bool IsRefreshEnabled() const;
		inline void SetRefreshEnabled(
			bool enable);
		
		inline AutoPackError AutoPack(
			int leftLimit,
			int rightLimit,
			int topLimit,
			int bottomLimit,
			AutoPackOptions hOptions,
			AutoPackOptions vOptions);
		AutoPackError AutoPack(
			int leftLimit,
			int rightLimit,
			int topLimit,
			int bottomLimit,
			AutoPackOptions hOptions,
			AutoPackOptions vOptions,
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
		inline AutoPackError AutoReshape();
		inline AutoPackError AutoReshape(
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual AutoPackError OnAutoPack(
			int hSize,
			int vSize,
			AutoPackOptions hOption,
			AutoPackOptions vOption,
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
	
	protected:
	
		virtual void OnWidgetPaint();
	
	protected:
	
		virtual void OnWidgetMouseDown(
			Glow::MouseButton which,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseUp(
			Glow::MouseButton which,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseDrag(
			int x,
			int y);
	
	protected:
	
		virtual void OnWidgetKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
	
	protected:
	
		virtual void OnGotKeyboardFocus();
		virtual void OnLostKeyboardFocus();
	
	protected:
	
		virtual void OnWidgetMove();
		virtual void OnWidgetReshape();
		virtual void OnWidgetVisible();
		virtual void OnWidgetInvisible();
		virtual void OnWidgetActivate();
		virtual void OnWidgetDeactivate();
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowWidgetParams& params);
		
		inline void RegisterMouseEvents();
		inline void RegisterKeyboardEvents();
		inline void UnregisterMouseEvents();
		inline void UnregisterKeyboardEvents();
		
		inline void NormalizeCoordinates(
			int x,
			int y,
			GLfloat& xn,
			GLfloat& yn) const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnActivate();
		virtual void OnDeactivate();
		virtual bool OnBeginPaint();
		virtual void OnEndPaint();
	
	private:
	
		inline void _SetHasKeyboardFocus(
			bool val);
		
		void _BroadcastNotifyList();
		void _BroadcastMask(
			bool unmasking);
		void _AddToNotifyList();
		void _NotifyOne(
			const Glow_OldWidgetState& oldState);
	
	private:
	
		// List of deferred widget operations
		static GLOW_STD::map<GlowWidget*, Glow_OldWidgetState> _notifyList;
		static void _ExecuteNotify();
	
	private:
	
		int _xpos;
		int _ypos;
		int _width;
		int _height;
		
		bool _refreshEnabled;
		bool _clipping;
		GLint _oldScissor[4];
		GlowColor _backColor;
		
		GlowWidgetRoot* _root;
		GlowWidget* _parentWidget;
		
		bool _receivingMouse;
		bool _receivingKeyboard;
		bool _hasFocus;
		short _visibility;
		
		long _refcon;
};

GLOW_INTERNAL_SETUPENUMBITFIELD(GlowWidget::AutoPackOptions)


class GlowWidgetKeyboardData;
class GlowWidgetKeyboardFilter;

/*
===============================================================================
	CLASS GlowWidgetRoot
	
	Widget root base class
===============================================================================
*/

class GlowWidgetRoot
{
	friend class GlowWidget;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		virtual ~GlowWidgetRoot();
	
	public:
	
		inline GlowColor GetBackColor() const;
		inline void SetBackColor(
			GlowColor c);
		
		void SetKeyboardFocus(
			GlowWidget* widget);
		inline GlowWidget* GetKeyboardFocus();
		void AdvanceKeyboardFocus();
		
		GlowWidget* FindWidget(
			int& x,
			int& y);
		
		inline GlowSubwindow* Subwindow() const;
		
		// Filters
		inline void RegisterFilter(
			GlowWidgetKeyboardFilter* filter);
		inline void UnregisterFilter(
			GlowWidgetKeyboardFilter* filter);
		inline void UnregisterAllFilters();
		inline unsigned int NumRegisteredFilters() const;
		inline bool IsFilterRegistered(
			GlowWidgetKeyboardFilter* filter) const;
	
	
	//-------------------------------------------------------------------------
	//	Subclass interface
	//-------------------------------------------------------------------------
	
	protected:
	
		inline GlowWidgetRoot();
		void Init(
			GlowSubwindow* subwindow,
			GlowColor backColor);
	
	protected:
	
		void WRMouseDown(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		void WRMouseUp(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		void WRMouseDrag(
			int x,
			int y);
		void WRKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
		bool WRBeginPaint();
		void WREndPaint();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		inline void _RegisterMouseWidget(
			GlowWidget* widget);
		inline void _RegisterKeyboardWidget(
			GlowWidget* widget);
		void _UnregisterMouseWidget(
			GlowWidget* widget);
		void _UnregisterKeyboardWidget(
			GlowWidget* widget);
	
	private:
	
		GlowColor _backColor;
		GLOW_STD::list<GlowWidget*> _mouseWidgets;
		GLOW_STD::list<GlowWidget*> _keyboardWidgets;
		GLOW_STD::list<GlowWidget*>::iterator _curKeyboardFocus;
		
		GlowWidget* _leftButton;
		GlowWidget* _middleButton;
		GlowWidget* _rightButton;
		
		GlowSubwindow* _subwindow;
		
		// Event filter senders
		TSender<GlowWidgetKeyboardData&> _keyboardFilters;
};


/*
===============================================================================
	Event filters
===============================================================================
*/

class GlowWidgetKeyboardData
{
	friend class GlowWidgetRoot;
	friend class GlowWidgetKeyboardFilter;
	
	public:
	
		GlowWidgetRoot* root;
		Glow::KeyCode key;
		int x;
		int y;
		Glow::Modifiers modifiers;
	
	private:
	
		inline GlowWidgetKeyboardData();
	
	private:
	
		bool _continue;
};


class GlowWidgetKeyboardFilter :
	public TReceiver<GlowWidgetKeyboardData&>
{
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual bool OnFilter(
			GlowWidgetKeyboardData& data) = 0;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage(
			GlowWidgetKeyboardData& message);
};


/*
===============================================================================
	Useful filters
===============================================================================
*/

class GlowWidgetTabFilter :
	public GlowWidgetKeyboardFilter
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowWidgetTabFilter();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual bool OnFilter(
			GlowWidgetKeyboardData& data);
};


/*
===============================================================================
	CLASS GlowWidgetSubwindow
	
	Subwindow containing widgets
===============================================================================
*/

class GlowWidgetSubwindow :
	public GlowSubwindow,
	public GlowWidgetRoot
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowWidgetSubwindow();
		inline GlowWidgetSubwindow(
			GlowComponent* parent,
			const GlowWidgetSubwindowParams& params);
		inline GlowWidgetSubwindow(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode = GlowWidgetSubwindowParams::defaults.mode,
			Glow::EventMask eventMask = GlowWidgetSubwindowParams::defaults.eventMask,
			GlowColor backColor = GlowWidgetSubwindowParams::defaults.backColor);
		inline void Init(
			GlowComponent* parent,
			const GlowWidgetSubwindowParams& params);
		void Init(
			GlowComponent* parent,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode = GlowWidgetSubwindowParams::defaults.mode,
			Glow::EventMask eventMask = GlowWidgetSubwindowParams::defaults.eventMask,
			GlowColor backColor = GlowWidgetSubwindowParams::defaults.backColor);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
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
		virtual void OnMouseDrag(
			int x,
			int y);
		virtual void OnKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual bool OnBeginPaint();
		virtual void OnEndPaint();
};


/*
===============================================================================
	CLASS GlowWidgetWindow
	
	Toplevel window containing widgets
===============================================================================
*/

class GlowWidgetWindow :
	public GlowWindow,
	public GlowWidgetRoot
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowWidgetWindow();
		inline GlowWidgetWindow(
			const GlowWidgetWindowParams& params);
		inline GlowWidgetWindow(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode = GlowWidgetWindowParams::defaults.mode,
			Glow::EventMask eventMask = GlowWidgetWindowParams::defaults.eventMask,
			GlowColor backColor = GlowWidgetWindowParams::defaults.backColor);
		inline void Init(
			const GlowWidgetWindowParams& params);
		void Init(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode = GlowWidgetWindowParams::defaults.mode,
			Glow::EventMask eventMask = GlowWidgetWindowParams::defaults.eventMask,
			GlowColor backColor = GlowWidgetWindowParams::defaults.backColor);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
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
		virtual void OnMouseDrag(
			int x,
			int y);
		virtual void OnKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual bool OnBeginPaint();
		virtual void OnEndPaint();
};


/*
===============================================================================
	CLASS GlowFixedSizeWidgetWindow
	
	Fixed-size toplevel window containing widgets
===============================================================================
*/

class GlowFixedSizeWidgetWindow :
	public GlowFixedSizeWindow,
	public GlowWidgetRoot
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowFixedSizeWidgetWindow();
		inline GlowFixedSizeWidgetWindow(
			const GlowWidgetWindowParams& params);
		inline GlowFixedSizeWidgetWindow(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode = GlowWidgetWindowParams::defaults.mode,
			Glow::EventMask eventMask = GlowWidgetWindowParams::defaults.eventMask,
			GlowColor backColor = GlowWidgetWindowParams::defaults.backColor);
		inline void Init(
			const GlowWidgetWindowParams& params);
		void Init(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode = GlowWidgetWindowParams::defaults.mode,
			Glow::EventMask eventMask = GlowWidgetWindowParams::defaults.eventMask,
			GlowColor backColor = GlowWidgetWindowParams::defaults.backColor);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
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
		virtual void OnMouseDrag(
			int x,
			int y);
		virtual void OnKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual bool OnBeginPaint();
		virtual void OnEndPaint();
};


/*
===============================================================================
	CLASS GlowSubwindowInWidget
	
	Base class for subwindows inside widgets
===============================================================================
*/

class GlowSubwindowInWidget :
	public GlowSubwindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowSubwindowInWidget();
		inline GlowSubwindowInWidget(
			GlowWidget* parent,
			const GlowSubwindowParams& params);
		inline GlowSubwindowInWidget(
			GlowWidget* parent,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
		inline void Init(
			GlowWidget* parent,
			const GlowSubwindowParams& params);
		inline void Init(
			GlowWidget* parent,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
		
		inline GlowWidget* ParentWidget() const;
		inline GlowWidgetRoot* Root() const;
		inline int WidgetPositionX() const;
		inline int WidgetPositionY() const;
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowWidget.inl.h"


#endif

