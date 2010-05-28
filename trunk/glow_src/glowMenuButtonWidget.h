/*
===============================================================================

	FILE:  glowMenuButtonWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Menu button for GLOW
	
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


#ifndef GLOW_MENUBUTTONWIDGET__H
#define GLOW_MENUBUTTONWIDGET__H


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
#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowPopupMenuWidget;
class GlowPopupMenuMessage;
class GlowMenuButtonParams;
class GlowPopupMenuParams;
class GlowLabeledPopupMenuParams;
typedef TReceiver<const GlowPopupMenuMessage&> GlowPopupMenuReceiver;


class Glow_MenuButtonSubwindow;
class Glow_PopupMenuSubwindow;
class Glow_PopupMenuInternalReceiver;


/*
===============================================================================
	CLASS GlowMenuButtonWidget
	
	Menu button widget
===============================================================================
*/

class GlowMenuButtonWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum IconType
		{
			noIcon = 0,
			menuIcon = 1,
			arrowIcon = 2
		};
	
	public:
	
		inline GlowMenuButtonWidget();
		inline GlowMenuButtonWidget(
			GlowWidget* parent,
			const GlowMenuButtonParams& params);
		inline GlowMenuButtonWidget(
			GlowWidgetRoot* root,
			const GlowMenuButtonParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowMenuButtonParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowMenuButtonParams& params);
	
	public:
	
		inline bool IsDown() const;
		
		inline const char* GetText() const;
		inline void SetText(
			const char* text);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline int GetSpacing() const;
		inline void SetSpacing(
			int spacing);
		
		inline IconType GetIconType() const;
		inline void SetIconType(
			IconType iconType);
		
		inline GlowMenu* GetMenu() const;
		inline void SetMenu(
			GlowMenu* menu);
		
		inline GlowColor GetBoxColor() const;
		inline GlowColor GetIconColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetHiliteIconColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetDisableBoxColor() const;
		inline GlowColor GetDisableIconColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBoxColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetIconColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetHiliteIconColor(
			GlowColor c);
		inline void SetDisableBoxColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableIconColor(
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
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowMenuButtonParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		Glow_MenuButtonSubwindow* _subwindow;
	
	protected:
	
		virtual void OnWidgetMove();
		virtual void OnWidgetReshape();
		virtual void OnWidgetVisible();
		virtual void OnWidgetInvisible();
};


/*
===============================================================================
	CLASS GlowPopupMenuWidget
	
	Popup menu widget
===============================================================================
*/

class GlowPopupMenuWidget :
	public GlowWidget
{
	friend class Glow_PopupMenuSubwindow;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowPopupMenuWidget();
		inline GlowPopupMenuWidget(
			GlowWidget* parent,
			const GlowPopupMenuParams& params);
		inline GlowPopupMenuWidget(
			GlowWidgetRoot* root,
			const GlowPopupMenuParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowPopupMenuParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowPopupMenuParams& params);
		
		virtual ~GlowPopupMenuWidget();
	
	public:
	
		inline bool IsDown() const;
		
		inline const char* GetMark() const;
		void SetMark(
			const char* mark);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline int GetSpacing() const;
		inline void SetSpacing(
			int spacing);
		
		int AddItem(
			const char* label);
		void RemoveItem(
			int itemNum);
		inline int NumItems() const;
		
		inline const char* GetItemLabel(
			int itemNum) const;
		void SetItemLabel(
			int itemNum,
			const char* label);
		
		inline int GetCurItem() const;
		void SetCurItem(
			int itemNum);
		
		inline GlowColor GetBoxColor() const;
		inline GlowColor GetIconColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetHiliteBoxColor() const;
		inline GlowColor GetHiliteIconColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetDisableBoxColor() const;
		inline GlowColor GetDisableIconColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBoxColor(
			GlowColor c);
		inline void SetIconColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetHiliteBoxColor(
			GlowColor c);
		inline void SetHiliteIconColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetDisableBoxColor(
			GlowColor c);
		inline void SetDisableIconColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
		
		inline TSender<const GlowPopupMenuMessage&>& Notifier();
	
	
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
			int item);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowPopupMenuParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		char* _mark;
		GlowMenu* _menu;
		int _curItem;
		Glow_PopupMenuSubwindow* _subwindow;
		TSender<const GlowPopupMenuMessage&> _sender;
	
	protected:
	
		virtual void OnWidgetMove();
		virtual void OnWidgetReshape();
		virtual void OnWidgetVisible();
		virtual void OnWidgetInvisible();
};


/*
===============================================================================
	CLASS GlowLabeledPopupMenuWidget
	
	Labeled popup menu widget
===============================================================================
*/

class GlowLabeledPopupMenuWidget :
	public GlowPopupMenuWidget,
	public GlowWidgetLabelHelper
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowLabeledPopupMenuWidget();
		inline GlowLabeledPopupMenuWidget(
			GlowWidget* parent,
			const GlowLabeledPopupMenuParams& params);
		inline GlowLabeledPopupMenuWidget(
			GlowWidgetRoot* root,
			const GlowLabeledPopupMenuParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowLabeledPopupMenuParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowLabeledPopupMenuParams& params);
	
	
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
		
		virtual void OnWidgetPaint();
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowLabeledPopupMenuParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowColor _upLabelColor;
		GlowColor _downLabelColor;
};


/*
===============================================================================
	CLASS GlowPopupMenuMessage
	
	Action for popup menu change
===============================================================================
*/


class GlowPopupMenuMessage
{
	public:
	
		GlowPopupMenuWidget* widget;
		int item;
		int oldItem;
};


/*
===============================================================================
	CLASS GlowMenuButtonParams
	
	Menu button params
===============================================================================
*/

class GlowMenuButtonParams :
	public GlowWidgetParams
{
	public:
	
		GlowMenu* menu;
		const char* text;
		GlowFont font;
		GlowMenuButtonWidget::IconType iconType;
		int spacing;
		GlowColor boxColor;
		GlowColor textColor;
		GlowColor iconColor;
		GlowColor hiliteBoxColor;
		GlowColor hiliteTextColor;
		GlowColor hiliteIconColor;
		GlowColor disableBoxColor;
		GlowColor disableTextColor;
		GlowColor disableIconColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowMenuButtonParams defaults;
		
		GlowMenuButtonParams();
	
	protected:
	
		GlowMenuButtonParams(bool);
};


/*
===============================================================================
	CLASS GlowPopupMenuParams
	
	Popup menu params
===============================================================================
*/

class GlowPopupMenuParams :
	public GlowWidgetParams
{
	public:
	
		const char* items;
		int initial;
		const char* mark;
		GlowFont font;
		int spacing;
		GlowPopupMenuReceiver* receiver;
		GlowColor boxColor;
		GlowColor textColor;
		GlowColor iconColor;
		GlowColor hiliteBoxColor;
		GlowColor hiliteTextColor;
		GlowColor hiliteIconColor;
		GlowColor disableBoxColor;
		GlowColor disableTextColor;
		GlowColor disableIconColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowPopupMenuParams defaults;
		
		GlowPopupMenuParams();
	
	protected:
	
		GlowPopupMenuParams(bool);
};


/*
===============================================================================
	CLASS GlowLabeledPopupMenuParams
	
	Labeled popup menu params
===============================================================================
*/

class GlowLabeledPopupMenuParams :
	public GlowPopupMenuParams
{
	public:
	
		const char* labelText;
		GlowFont labelFont;
		GlowLabeledPopupMenuWidget::LabelPosition labelPosition;
		int labelWidth;
		int labelHeight;
		int labelSpacing;
		GlowColor labelColor;
		GlowColor hiliteLabelColor;
		GlowColor disableLabelColor;
		
		static GlowLabeledPopupMenuParams defaults;
		
		GlowLabeledPopupMenuParams();
	
	protected:
	
		GlowLabeledPopupMenuParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowMenuButtonWidget.inl.h"


#endif
