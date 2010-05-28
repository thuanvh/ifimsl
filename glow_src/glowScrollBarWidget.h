/*
===============================================================================

	FILE:  glowScrollBarWidget.h
	
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


#ifndef GLOW_SCROLLBARWIDGET__H
#define GLOW_SCROLLBARWIDGET__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_WIDGET__H
	#include "glowWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowScrollBarWidget;
class Glow_ScrollBarTimerFunc;
class GlowScrollBarParams;
class GlowScrollBarMessage;
typedef TReceiver<const GlowScrollBarMessage&> GlowScrollBarReceiver;


/*
===============================================================================
	CLASS GlowScrollBarWidget
	
	Scroll bar widget
===============================================================================
*/

class GlowScrollBarWidget :
	public GlowWidget
{
	friend class Glow_ScrollBar_TimerFunc;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Part
		{
			noPart = 0,
			upButtonPart = 1,
			downButtonPart = 2,
			upPagePart = 3,
			downPagePart = 4,
			indicatorPart = 5
		};
		
		enum {
			spanPageStep = -1
		};
	
	public:
	
		inline GlowScrollBarWidget();
		inline GlowScrollBarWidget(
			GlowWidget* parent,
			const GlowScrollBarParams& params);
		inline GlowScrollBarWidget(
			GlowWidgetRoot* root,
			const GlowScrollBarParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowScrollBarParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowScrollBarParams& params);
		
		virtual ~GlowScrollBarWidget();
	
	public:
	
		inline long GetTopValue() const;
		inline long GetBottomValue() const;
		inline long GetMinimum() const;
		inline long GetMaximum() const;
		inline long GetSpan() const;
		
		inline void SetTopValue(
			long val);
		inline void MoveTopValue(
			long delta);
		inline void SetMaximum(
			long val);
		inline void SetMinimum(
			long val);
		inline void SetSpan(
			long val);
		
		inline int GetFirstDelay() const;
		inline void SetFirstDelay(
			int msecs);
		inline int GetSecondDelay() const;
		inline void SetSecondDelay(
			int msecs);
		
		inline long GetPageStep() const;
		inline void SetPageStep(
			long pageStep);
		inline long GetArrowStep() const;
		inline void SetArrowStep(
			long arrowStep);
		
		inline TSender<const GlowScrollBarMessage&>& Notifier();
		
		inline GlowColor GetStripColor() const;
		inline GlowColor GetIndicatorColor() const;
		inline GlowColor GetShadowColor() const;
		inline GlowColor GetButtonColor() const;
		inline GlowColor GetButtonIconColor() const;
		inline GlowColor GetHiliteStripColor() const;
		inline GlowColor GetHiliteIndicatorColor() const;
		inline GlowColor GetHiliteButtonColor() const;
		inline GlowColor GetHiliteButtonIconColor() const;
		inline GlowColor GetDisableStripColor() const;
		inline GlowColor GetDisableIndicatorColor() const;
		inline GlowColor GetDisableButtonColor() const;
		inline GlowColor GetDisableButtonIconColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetStripColor(
			GlowColor color);
		inline void SetIndicatorColor(
			GlowColor color);
		inline void SetShadowColor(
			GlowColor color);
		inline void SetButtonColor(
			GlowColor color);
		inline void SetButtonIconColor(
			GlowColor color);
		inline void SetHiliteStripColor(
			GlowColor color);
		inline void SetHiliteIndicatorColor(
			GlowColor color);
		inline void SetHiliteButtonColor(
			GlowColor color);
		inline void SetHiliteButtonIconColor(
			GlowColor color);
		inline void SetDisableStripColor(
			GlowColor color);
		inline void SetDisableIndicatorColor(
			GlowColor color);
		inline void SetDisableButtonColor(
			GlowColor color);
		inline void SetDisableButtonIconColor(
			GlowColor color);
		inline void SetDisableOutlineColor(
			GlowColor color);
		inline void SetLightBevelColor(
			GlowColor color);
		inline void SetDarkBevelColor(
			GlowColor color);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowScrollBarParams& params);
	
	
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
		
		virtual void OnDragged(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
		virtual void OnPart(
			Part part,
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
		virtual void OnReleased(
			Part part,
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		TSender<const GlowScrollBarMessage&> _sender;
		float _posOffset;
		long _min;
		long _max;
		long _topValue;
		long _span;
		Glow::MouseButton _button;
		Glow::Modifiers _modifiers;
		Part _curPart;
		bool _innerButton;
		bool _inside;
		int _firstDelay;
		int _secondDelay;
		int _toggleTimer;
		float _savePosition;
		long _pageStep;
		long _arrowStep;
		
		GlowColor _stripColor;
		GlowColor _indicatorColor;
		GlowColor _shadowColor;
		GlowColor _buttonColor;
		GlowColor _buttonIconColor;
		GlowColor _hiliteStripColor;
		GlowColor _hiliteIndicatorColor;
		GlowColor _hiliteButtonColor;
		GlowColor _hiliteButtonIconColor;
		GlowColor _disableStripColor;
		GlowColor _disableIndicatorColor;
		GlowColor _disableButtonColor;
		GlowColor _disableButtonIconColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	private:
	
		void _DrawArrowButton(
			Part part,
			float left,
			float right,
			float bevelWidth,
			float bevelHeight,
			float buttonWidth);
		void _ComputePartPositions(
			float& pseudoWidth,
			float& pseudoHeight,
			int& arrowButtons,
			float& buttonWidth,
			float& stripLeft,
			float& stripRight,
			float& indicLeft,
			float& indicRight);
		bool _NormalizePosition(
			int x,
			int y,
			float& pos);
		bool _UpdateDragValue(
			int x,
			int y);
		bool _UpdateInsideState(
			int x,
			int y);
		void _HandlePartPeriodical(
			bool first);
	
	protected:
	
		virtual void OnWidgetPaint();
		
		virtual void OnWidgetMouseDown(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseUp(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseDrag(
			int x,
			int y);
};


/*
===============================================================================
	CLASS GlowScrollBarMessage
	
	Action for scroll bar
===============================================================================
*/

class GlowScrollBarMessage
{
	public:
	
		GlowScrollBarWidget* widget;
		GlowScrollBarWidget::Part part;
		long topValue;
		bool released;
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};


/*
===============================================================================
	CLASS GlowScrollBarParams
	
	Scroll bar params
===============================================================================
*/

class GlowScrollBarParams :
	public GlowWidgetParams
{
	public:
	
		long min;
		long max;
		long span;
		long initialTop;
		int firstDelay;
		int secondDelay;
		long arrowStep;
		long pageStep;
		GlowScrollBarReceiver* receiver;
		GlowColor stripColor;
		GlowColor indicatorColor;
		GlowColor shadowColor;
		GlowColor buttonColor;
		GlowColor buttonIconColor;
		GlowColor hiliteStripColor;
		GlowColor hiliteIndicatorColor;
		GlowColor hiliteButtonColor;
		GlowColor hiliteButtonIconColor;
		GlowColor disableStripColor;
		GlowColor disableIndicatorColor;
		GlowColor disableButtonColor;
		GlowColor disableButtonIconColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowScrollBarParams defaults;
		
		GlowScrollBarParams();
	
	protected:
	
		GlowScrollBarParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowScrollBarWidget.inl.h"


#endif
