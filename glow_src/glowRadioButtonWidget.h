/*
===============================================================================

	FILE:  glowRadioButtonWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Radio button widget for GLOW
	
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


#ifndef GLOW_RADIOBUTTONWIDGET__H
#define GLOW_RADIOBUTTONWIDGET__H


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
#ifndef GLOW_PANELWIDGET__H
	#include "glowPanelWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowRadioButtonWidget;
class GlowRadioGroupWidget;
class GlowRadioButtonMessage;
class GlowRadioButtonParams;
class GlowRadioGroupParams;
typedef TReceiver<const GlowRadioButtonMessage&> GlowRadioButtonReceiver;


/*
===============================================================================
	CLASS GlowRadioButtonWidget
	
	Radio button widget
===============================================================================
*/

class GlowRadioButtonWidget :
	public GlowWidget
{
	friend class GlowRadioGroupWidget;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowRadioButtonWidget();
		inline GlowRadioButtonWidget(
			GlowRadioGroupWidget* parent,
			const GlowRadioButtonParams& params);
		
		void Init(
			GlowRadioGroupWidget* parent,
			const GlowRadioButtonParams& params);
		
		virtual ~GlowRadioButtonWidget();
	
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
		
		inline bool GetState() const;
		
		void CropWidth();
		
		inline GlowColor GetBoxColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetSpotColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetHiliteSpotColor() const;
		inline GlowColor GetDisableBoxColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableSpotColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetTextColor(
			GlowColor c);
		inline void SetBoxColor(
			GlowColor c);
		inline void SetSpotColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetHiliteSpotColor(
			GlowColor c);
		inline void SetDisableBoxColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableSpotColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
	
	
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
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowRadioGroupWidget* _group;
		bool _state;
		char* _label;
		GlowFont _font;
		bool _down;
		bool _inside;
		Glow::MouseButton _button;
		Glow::Modifiers _modifiers;
		int _labelWidth;
		int _spacing;
		
		GlowColor _boxColor;
		GlowColor _textColor;
		GlowColor _spotColor;
		GlowColor _hiliteBoxColor;
		GlowColor _hiliteTextColor;
		GlowColor _hiliteSpotColor;
		GlowColor _disableBoxColor;
		GlowColor _disableTextColor;
		GlowColor _disableSpotColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	private:
	
		inline void SetState(
			bool state);
	
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
	CLASS GlowRadioGroupWidget
	
	Radio button group widget
===============================================================================
*/

class GlowRadioGroupWidget :
	public GlowPanelWidget
{
	friend class GlowRadioButtonWidget;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowRadioGroupWidget();
		inline GlowRadioGroupWidget(
			GlowWidgetRoot* root,
			const GlowRadioGroupParams& params);
		inline GlowRadioGroupWidget(
			GlowWidget* parent,
			const GlowRadioGroupParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowRadioGroupParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowRadioGroupParams& params);
		
		inline void SetState(
			GlowRadioButtonWidget* button);
		inline GlowRadioButtonWidget* GetState() const;
		
		inline TSender<const GlowRadioButtonMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowRadioGroupParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnHit(
			GlowRadioButtonWidget* buttonWidget,
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowRadioButtonWidget* _state;
		TSender<const GlowRadioButtonMessage&> _sender;
	
	private:
	
		inline bool _Register(
			GlowRadioButtonWidget* button);
		void _UnRegister(
			GlowRadioButtonWidget* button);
};


/*
===============================================================================
	CLASS GlowRadioButtonMessage
	
	Action for radio button press
===============================================================================
*/

class GlowRadioButtonMessage
{
	public:
	
		GlowRadioGroupWidget* groupWidget;
		GlowRadioButtonWidget* buttonWidget;
		GlowRadioButtonWidget* oldButtonWidget;
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};


/*
===============================================================================
	CLASS GlowRadioButtonParams
	
	Radio button params
===============================================================================
*/

class GlowRadioButtonParams :
	public GlowWidgetParams
{
	public:
	
		const char* text;
		GlowFont font;
		int spacing;
		GlowColor boxColor;
		GlowColor textColor;
		GlowColor spotColor;
		GlowColor hiliteBoxColor;
		GlowColor hiliteTextColor;
		GlowColor hiliteSpotColor;
		GlowColor disableBoxColor;
		GlowColor disableTextColor;
		GlowColor disableSpotColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowRadioButtonParams defaults;
		
		GlowRadioButtonParams();
	
	protected:
	
		GlowRadioButtonParams(bool);
};


/*
===============================================================================
	CLASS GlowRadioGroupParams
	
	Radio group params
===============================================================================
*/

class GlowRadioGroupParams :
	public GlowPanelParams
{
	public:
	
		GlowRadioButtonReceiver* receiver;
		
		static GlowRadioGroupParams defaults;
		
		GlowRadioGroupParams();
	
	protected:
	
		GlowRadioGroupParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowRadioButtonWidget.inl.h"


#endif

