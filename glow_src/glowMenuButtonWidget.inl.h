/*
===============================================================================

	FILE:  glowMenuButtonWidget.inl.h
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#include <cstring>

GLOW_INTERNAL_USINGSTD

#include "glowDebug.h"

GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS Glow_MenuButtonSubwindow
	
	Menu button subwindow class (private)
===============================================================================
*/

class Glow_MenuButtonSubwindow :
	public GlowSubwindowInWidget
{
	//-------------------------------------------------------------------------
	//	Private interface
	//-------------------------------------------------------------------------
	
	friend class GlowMenuButtonWidget;
	friend class GlowPopupMenuWidget;
	
	
	protected:
	
		Glow_MenuButtonSubwindow(
			GlowWidget* parent,
			const char* label,
			GlowFont font,
			GlowMenuButtonWidget::IconType iconType,
			int leftSpacing);
		
		virtual ~Glow_MenuButtonSubwindow();
	
	protected:
	
		inline const char* _GetLabel() const;
		void _SetLabel(
			const char* label);
		inline GlowMenu* _GetMenu() const;
		inline void _SetMenu(
			GlowMenu* menu);
		
		void PreferredSize(
			int& width,
			int& height,
			const char* str = 0);
	
	protected:
	
		virtual void OnMenuDown(
			int x,
			int y);
		virtual void OnMenuUp();
		
		virtual void OnEndPaint();
	
	private:
	
		char* _label;
		GlowFont _font;
		GlowMenuButtonWidget::IconType _iconType;
		int _labelWidth;
		int _leftSpacing;
	
	protected:
	
		bool _down;
		
		GlowColor _boxColor;
		GlowColor _textColor;
		GlowColor _iconColor;
		GlowColor _hiliteBoxColor;
		GlowColor _hiliteTextColor;
		GlowColor _hiliteIconColor;
		GlowColor _disableBoxColor;
		GlowColor _disableTextColor;
		GlowColor _disableIconColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
};


/*
===============================================================================
	CLASS Glow_PopupMenuSubwindow
	
	Menu button subwindow class (private)
===============================================================================
*/

class Glow_PopupMenuSubwindow :
	public Glow_MenuButtonSubwindow
{
	friend class GlowPopupMenuWidget;
	
	protected:
	
		inline Glow_PopupMenuSubwindow(
			GlowPopupMenuWidget* parent,
			const char* label,
			GlowFont font,
			GlowMenuButtonWidget::IconType iconType,
			int leftSpacing);
	
	protected:
	
		virtual void OnMenuDown(
			int x,
			int y);
		virtual void OnMenuUp();
		virtual void OnDirectMenuHit(
			const GlowMenuMessage& message);
	
	private:
	
		GlowPopupMenuWidget* _widget;
};


/*
===============================================================================
	Inline methods of Glow_MenuButtonSubwindow
===============================================================================
*/

inline const char* Glow_MenuButtonSubwindow::_GetLabel() const
{
	return _label;
}


inline void Glow_MenuButtonSubwindow::_SetMenu(
	GlowMenu* menu)
{
	SetMenu(Glow::leftButton, menu);
	SetMenu(Glow::middleButton, menu);
	SetMenu(Glow::rightButton, menu);
}


inline GlowMenu* Glow_MenuButtonSubwindow::_GetMenu() const
{
	return GetMenu(Glow::rightButton);
}


/*
===============================================================================
	Inline methods of Glow_PopupMenuSubwindow
===============================================================================
*/

inline Glow_PopupMenuSubwindow::Glow_PopupMenuSubwindow(
	GlowPopupMenuWidget* parent,
	const char* label,
	GlowFont font,
	GlowMenuButtonWidget::IconType iconType,
	int leftSpacing) :
Glow_MenuButtonSubwindow(parent, label, font, iconType, leftSpacing)
{
	_widget = parent;
}


/*
===============================================================================
	Inline methods of GlowMenuButtonWidget
===============================================================================
*/

inline GlowMenuButtonWidget::GlowMenuButtonWidget()
{
}


inline GlowMenuButtonWidget::GlowMenuButtonWidget(
	GlowWidget* parent,
	const GlowMenuButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowMenuButtonWidget::GlowMenuButtonWidget(
	GlowWidgetRoot* root,
	const GlowMenuButtonParams& params)
{
	Init(root, 0, params);
}


inline void GlowMenuButtonWidget::Init(
	GlowWidget* parent,
	const GlowMenuButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowMenuButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowMenuButtonParams& params)
{
	Init(root, 0, params);
}


inline bool GlowMenuButtonWidget::IsDown() const
{
	return _subwindow->_down;
}


inline void GlowMenuButtonWidget::SetText(
	const char* text)
{
	_subwindow->_SetLabel(text);
}


inline const char* GlowMenuButtonWidget::GetText() const
{
	return _subwindow->_GetLabel();
}


inline GlowFont GlowMenuButtonWidget::GetFont() const
{
	return _subwindow->_font;
}


inline void GlowMenuButtonWidget::SetFont(
	GlowFont font)
{
	_subwindow->_font = font;
	_subwindow->Refresh();
}


inline int GlowMenuButtonWidget::GetSpacing() const
{
	return _subwindow->_leftSpacing;
}


inline void GlowMenuButtonWidget::SetSpacing(
	int spacing)
{
	_subwindow->_leftSpacing = spacing;
	_subwindow->Refresh();
}


inline GlowMenuButtonWidget::IconType GlowMenuButtonWidget::GetIconType() const
{
	return _subwindow->_iconType;
}


inline void GlowMenuButtonWidget::SetIconType(
	IconType iconType)
{
	_subwindow->_iconType = iconType;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetMenu(
	GlowMenu* menu)
{
	_subwindow->_SetMenu(menu);
}


inline GlowMenu* GlowMenuButtonWidget::GetMenu() const
{
	return _subwindow->_GetMenu();
}


inline GlowColor GlowMenuButtonWidget::GetBoxColor() const
{
	return _subwindow->_boxColor;
}


inline GlowColor GlowMenuButtonWidget::GetIconColor() const
{
	return _subwindow->_iconColor;
}


inline GlowColor GlowMenuButtonWidget::GetTextColor() const
{
	return _subwindow->_textColor;
}


inline GlowColor GlowMenuButtonWidget::GetHiliteBoxColor() const
{
	return _subwindow->_hiliteBoxColor;
}


inline GlowColor GlowMenuButtonWidget::GetHiliteIconColor() const
{
	return _subwindow->_hiliteIconColor;
}


inline GlowColor GlowMenuButtonWidget::GetHiliteTextColor() const
{
	return _subwindow->_hiliteTextColor;
}


inline GlowColor GlowMenuButtonWidget::GetDisableBoxColor() const
{
	return _subwindow->_disableBoxColor;
}


inline GlowColor GlowMenuButtonWidget::GetDisableIconColor() const
{
	return _subwindow->_disableIconColor;
}


inline GlowColor GlowMenuButtonWidget::GetDisableTextColor() const
{
	return _subwindow->_disableTextColor;
}


inline GlowColor GlowMenuButtonWidget::GetDisableOutlineColor() const
{
	return _subwindow->_disableOutlineColor;
}


inline GlowColor GlowMenuButtonWidget::GetLightBevelColor() const
{
	return _subwindow->_lightBevelColor;
}


inline GlowColor GlowMenuButtonWidget::GetDarkBevelColor() const
{
	return _subwindow->_darkBevelColor;
}


inline void GlowMenuButtonWidget::SetBoxColor(
	GlowColor c)
{
	_subwindow->_boxColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetTextColor(
	GlowColor c)
{
	_subwindow->_textColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetIconColor(
	GlowColor c)
{
	_subwindow->_iconColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	_subwindow->_hiliteBoxColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	_subwindow->_hiliteTextColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetHiliteIconColor(
	GlowColor c)
{
	_subwindow->_hiliteIconColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableBoxColor(
	GlowColor c)
{
	_subwindow->_disableBoxColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	_subwindow->_disableTextColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableIconColor(
	GlowColor c)
{
	_subwindow->_disableIconColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_subwindow->_disableOutlineColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	_subwindow->_lightBevelColor = c;
	_subwindow->Refresh();
}


inline void GlowMenuButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	_subwindow->_darkBevelColor = c;
	_subwindow->Refresh();
}


/*
===============================================================================
	Inline methods of GlowPopupMenuWidget
===============================================================================
*/

inline GlowPopupMenuWidget::GlowPopupMenuWidget()
{
}


inline GlowPopupMenuWidget::GlowPopupMenuWidget(
	GlowWidget* parent,
	const GlowPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowPopupMenuWidget::GlowPopupMenuWidget(
	GlowWidgetRoot* root,
	const GlowPopupMenuParams& params)
{
	Init(root, 0, params);
}


inline void GlowPopupMenuWidget::Init(
	GlowWidget* parent,
	const GlowPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	const GlowPopupMenuParams& params)
{
	Init(root, 0, params);
}


inline bool GlowPopupMenuWidget::IsDown() const
{
	return _subwindow->_down;
}


inline const char* GlowPopupMenuWidget::GetMark() const
{
	return _mark;
}


inline GlowFont GlowPopupMenuWidget::GetFont() const
{
	return _subwindow->_font;
}


inline void GlowPopupMenuWidget::SetFont(
	GlowFont font)
{
	_subwindow->_font = font;
	_subwindow->Refresh();
}


inline int GlowPopupMenuWidget::GetSpacing() const
{
	return _subwindow->_leftSpacing;
}


inline void GlowPopupMenuWidget::SetSpacing(
	int spacing)
{
	_subwindow->_leftSpacing = spacing;
	_subwindow->Refresh();
}


inline int GlowPopupMenuWidget::NumItems() const
{
	return _menu->NumItems();
}


inline int GlowPopupMenuWidget::GetCurItem() const
{
	return _curItem;
}


inline const char* GlowPopupMenuWidget::GetItemLabel(
	int itemNum) const
{
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < _menu->NumItems());
	
	return _menu->GetItemLabel(itemNum);
}


inline GlowColor GlowPopupMenuWidget::GetBoxColor() const
{
	return _subwindow->_boxColor;
}


inline GlowColor GlowPopupMenuWidget::GetIconColor() const
{
	return _subwindow->_iconColor;
}


inline GlowColor GlowPopupMenuWidget::GetTextColor() const
{
	return _subwindow->_textColor;
}


inline GlowColor GlowPopupMenuWidget::GetHiliteBoxColor() const
{
	return _subwindow->_hiliteBoxColor;
}


inline GlowColor GlowPopupMenuWidget::GetHiliteIconColor() const
{
	return _subwindow->_hiliteIconColor;
}


inline GlowColor GlowPopupMenuWidget::GetHiliteTextColor() const
{
	return _subwindow->_hiliteTextColor;
}


inline GlowColor GlowPopupMenuWidget::GetDisableBoxColor() const
{
	return _subwindow->_disableBoxColor;
}


inline GlowColor GlowPopupMenuWidget::GetDisableIconColor() const
{
	return _subwindow->_disableIconColor;
}


inline GlowColor GlowPopupMenuWidget::GetDisableTextColor() const
{
	return _subwindow->_disableTextColor;
}


inline GlowColor GlowPopupMenuWidget::GetDisableOutlineColor() const
{
	return _subwindow->_disableOutlineColor;
}


inline GlowColor GlowPopupMenuWidget::GetLightBevelColor() const
{
	return _subwindow->_lightBevelColor;
}


inline GlowColor GlowPopupMenuWidget::GetDarkBevelColor() const
{
	return _subwindow->_darkBevelColor;
}


inline void GlowPopupMenuWidget::SetBoxColor(
	GlowColor c)
{
	_subwindow->_boxColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetIconColor(
	GlowColor c)
{
	_subwindow->_iconColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetTextColor(
	GlowColor c)
{
	_subwindow->_textColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetHiliteBoxColor(
	GlowColor c)
{
	_subwindow->_hiliteBoxColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetHiliteIconColor(
	GlowColor c)
{
	_subwindow->_hiliteIconColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetHiliteTextColor(
	GlowColor c)
{
	_subwindow->_hiliteTextColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableBoxColor(
	GlowColor c)
{
	_subwindow->_disableBoxColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableIconColor(
	GlowColor c)
{
	_subwindow->_disableIconColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableTextColor(
	GlowColor c)
{
	_subwindow->_disableTextColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_subwindow->_disableOutlineColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetLightBevelColor(
	GlowColor c)
{
	_subwindow->_lightBevelColor = c;
	_subwindow->Refresh();
}


inline void GlowPopupMenuWidget::SetDarkBevelColor(
	GlowColor c)
{
	_subwindow->_darkBevelColor = c;
	_subwindow->Refresh();
}


inline TSender<const GlowPopupMenuMessage&>& GlowPopupMenuWidget::Notifier()
{
	return _sender;
}


/*
===============================================================================
	Inline methods of GlowLabeledPopupMenuWidget
===============================================================================
*/

inline GlowLabeledPopupMenuWidget::GlowLabeledPopupMenuWidget()
{
}


inline GlowLabeledPopupMenuWidget::GlowLabeledPopupMenuWidget(
	GlowWidget* parent,
	const GlowLabeledPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabeledPopupMenuWidget::GlowLabeledPopupMenuWidget(
	GlowWidgetRoot* root,
	const GlowLabeledPopupMenuParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabeledPopupMenuWidget::Init(
	GlowWidget* parent,
	const GlowLabeledPopupMenuParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabeledPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledPopupMenuParams& params)
{
	Init(root, 0, params);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

