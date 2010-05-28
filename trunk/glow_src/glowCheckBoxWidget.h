/*
===============================================================================

	FILE:  glowCheckBoxWidget.h
	
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


#ifndef GLOW_CHECKBOXWIDGET__H
#define GLOW_CHECKBOXWIDGET__H


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


class GlowCheckBoxWidget;
class GlowCheckBoxMessage;
class GlowCheckBoxParams;
typedef TReceiver<const GlowCheckBoxMessage&> GlowCheckBoxReceiver;


/*
===============================================================================
	CLASS GlowCheckBoxWidget
	
	Check box widget
===============================================================================
*/

class GlowCheckBoxWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// States
		enum State
		{
			off = 0,
			on = 1,
			half = 2
		};
		
		// Behaviors
		enum Behavior
		{
			defaultBehavior = 0,
			onFollowsOff = 0,
			halfFollowsOff = 1,
			offFollowsOn = 0,
			halfFollowsOn = 2,
			onFollowsHalf = 0,
			offFollowsHalf = 4
		};
	
	public:
	
		inline GlowCheckBoxWidget();
		inline GlowCheckBoxWidget(
			GlowWidget* parent,
			const GlowCheckBoxParams& params);
		inline GlowCheckBoxWidget(
			GlowWidgetRoot* root,
			const GlowCheckBoxParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowCheckBoxParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowCheckBoxParams& params);
		
		virtual ~GlowCheckBoxWidget();
	
	public:
	
		inline const char* GetText() const;
		void SetText(
			const char* text);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline int GetSpacing() const;
		inline void SetSpacing(
			int spacing);
		
		inline Behavior GetBehavior() const;
		inline void SetBehavior(
			Behavior behavior = defaultBehavior);
		
		inline State GetState() const;
		inline void SetState(
			State state);
		void ToggleState();
		
		inline void Hit(
			Glow::MouseButton mouseButton = Glow::leftButton,
			Glow::Modifiers modifiers = Glow::noModifier);
		
		void CropWidth();
		
		inline GlowColor GetBoxColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetCheckColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetHiliteCheckColor() const;
		inline GlowColor GetDisableBoxColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableCheckColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBoxColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetCheckColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetHiliteCheckColor(
			GlowColor c);
		inline void SetDisableBoxColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableCheckColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
		
		inline TSender<const GlowCheckBoxMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Subclsss-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowCheckBoxParams& params);
	
	
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
		
		virtual void OnHit(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		State _state;
		char* _label;
		GlowFont _font;
		Behavior _behavior;
		bool _down;
		bool _inside;
		Glow::MouseButton _button;
		Glow::Modifiers _modifiers;
		int _labelWidth;
		int _spacing;
		TSender<const GlowCheckBoxMessage&> _sender;
		
		GlowColor _boxColor;
		GlowColor _textColor;
		GlowColor _checkColor;
		GlowColor _hiliteBoxColor;
		GlowColor _hiliteTextColor;
		GlowColor _hiliteCheckColor;
		GlowColor _disableBoxColor;
		GlowColor _disableTextColor;
		GlowColor _disableCheckColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
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

GLOW_INTERNAL_SETUPENUMBITFIELD(GlowCheckBoxWidget::Behavior)


/*
===============================================================================
	CLASS GlowCheckBoxMessage
	
	Action for checkbox toggle
===============================================================================
*/

class GlowCheckBoxMessage
{
	public:
	
		GlowCheckBoxWidget* widget;
		GlowCheckBoxWidget::State state;
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};




/*
===============================================================================
	CLASS GlowCheckBoxParams
	
	Check box params
===============================================================================
*/

class GlowCheckBoxParams :
	public GlowWidgetParams
{
	public:
	
		const char* text;
		GlowFont font;
		GlowCheckBoxWidget::State state;
		GlowCheckBoxWidget::Behavior behavior;
		int spacing;
		GlowCheckBoxReceiver* receiver;
		GlowColor boxColor;
		GlowColor textColor;
		GlowColor checkColor;
		GlowColor hiliteBoxColor;
		GlowColor hiliteTextColor;
		GlowColor hiliteCheckColor;
		GlowColor disableBoxColor;
		GlowColor disableTextColor;
		GlowColor disableCheckColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowCheckBoxParams defaults;
		
		GlowCheckBoxParams();
	
	protected:
	
		GlowCheckBoxParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowCheckBoxWidget.inl.h"


#endif


