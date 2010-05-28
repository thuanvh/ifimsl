/*
===============================================================================

	FILE:  glowQuickPalette.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Quick palette API for GLOW
	
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


#ifndef GLOW_QUICKPALETTE__H
#define GLOW_QUICKPALETTE__H


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
#ifndef GLOW_CHECKBOXWIDGET__H
	#include "glowCheckBoxWidget.h"
#endif
#ifndef GLOW_PANELWIDGET__H
	#include "glowPanelWidget.h"
#endif
#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif
#ifndef GLOW_MENUBUTTONWIDGET__H
	#include "glowMenuButtonWidget.h"
#endif
#ifndef GLOW_PUSHBUTTONWIDGET__H
	#include "glowPushButtonWidget.h"
#endif
#ifndef GLOW_RADIOBUTTONWIDGET__H
	#include "glowRadioButtonWidget.h"
#endif
#ifndef GLOW_SCROLLBARWIDGET__H
	#include "glowScrollBarWidget.h"
#endif
#ifndef GLOW_SEPARATORWIDGET__H
	#include "glowSeparatorWidget.h"
#endif
#ifndef GLOW_SLIDERWIDGET__H
	#include "glowSliderWidget.h"
#endif
#ifndef GLOW_TEXTFIELDWIDGET__H
	#include "glowTextFieldWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowQuickPalette;
class GlowQuickPanelWidget;
class GlowQuickRadioGroupWidget;


/*
===============================================================================
	CLASS GlowQuickPalette
	
	Defines constants and methods for QuickPalette
===============================================================================
*/

class GlowQuickPalette
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum {
			parentSetting = -1
		};
		
		enum Arrangement
		{
			parentArrangement = parentSetting,
			vertical = 0,
			horizontal = 1
		};
		
		enum Alignment
		{
			parentAlignment = parentSetting,
			alignTop = 0,
			alignLeft = 0,
			alignCenter = 1,
			alignBottom = 2,
			alignRight = 2,
			alignExpand = 4
		};
	
	public:
	
		GlowQuickPanelWidget* AddPanel(
			GlowPanelWidget::Style style = GlowPanelWidget::plainStyle,
			const char* label = 0,
			Arrangement arrangement = parentArrangement,
			Alignment alignment = parentAlignment,
			int spacing = parentSetting,
			int hmargin = 10,
			int vmargin = 10);
		inline GlowQuickPanelWidget* AddArrangingPanel(
			Arrangement arrangement = parentArrangement,
			Alignment alignment = parentAlignment,
			int spacing = parentSetting,
			int inset = 0);
		GlowPanelWidget* AddFixedPanel(
			int width,
			int height,
			GlowPanelWidget::Style style = GlowPanelWidget::plainStyle);
		GlowSeparatorWidget* AddSeparator(
			GlowSeparatorWidget::Style style = GlowSeparatorWidget::valleyStyle);
		GlowPushButtonWidget* AddPushButton(
			const char* label,
			GlowPushButtonReceiver* receiver = 0);
		GlowDismissPushButtonWidget* AddDismissPushButton(
			const char* label,
			GlowComponent* toDismiss,
			GlowPushButtonReceiver* receiver = 0);
		GlowMenuButtonWidget* AddMenuButton(
			const char* label,
			GlowMenu* menu,
			GlowMenuButtonWidget::IconType iconType = GlowMenuButtonWidget::menuIcon);
		GlowLabeledPopupMenuWidget* AddPopupMenu(
			const char* label,
			GlowPopupMenuReceiver* receiver = 0);
		GlowCheckBoxWidget* AddCheckBox(
			const char* label,
			GlowCheckBoxWidget::State initialValue = GlowCheckBoxWidget::off,
			GlowCheckBoxReceiver* receiver = 0);
		GlowQuickRadioGroupWidget* AddRadioGroup(
			Arrangement arrangement = vertical,
			int spacing = 3,
			GlowRadioButtonReceiver* receiver = 0);
		GlowLabeledSliderWidget* AddSlider(
			float min,
			float max,
			float initial,
			GlowSliderWidget::Options options = GlowSliderWidget::defaultOptions,
			int numTicks = 2,
			const char* valueLabel = 0,
			const char* label = 0,
			GlowSliderReceiver* receiver = 0);
		GlowScrollBarWidget* AddScrollBar(
			long min,
			long max,
			long span,
			long initialTop,
			GlowScrollBarReceiver* receiver = 0);
		GlowLabeledTextFieldWidget* AddTextField(
			int width,
			const char* text = "",
			const char* label = 0);
		GlowLabeledHiddenTextFieldWidget* AddHiddenTextField(
			int width,
			const char* text = "",
			char hideCharacter = '#',
			const char* label = 0);
		GlowLabelWidget* AddLabel(
			const char* text);
		
		inline Arrangement GetArrangement() const;
		inline Alignment GetAlignment() const;
		inline int GetSpacing() const;
		inline int GetHMargin() const;
		inline int GetVMargin() const;
		inline void SetArrangement(
			Arrangement value);
		inline void SetAlignment(
			Alignment value);
		inline void SetSpacing(
			int value);
		inline void SetHMargin(
			int value);
		inline void SetVMargin(
			int value);
		
		inline GlowQuickPanelWidget* QuickPaletteRoot() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		GlowQuickPanelWidget* _panel;
	
	protected:
	
		inline GlowQuickPalette();
};

GLOW_INTERNAL_SETUPENUMBITFIELD(GlowQuickPalette::Alignment)


/*
===============================================================================
	CLASS GlowQuickPanelWidget
	
	Quick panel
===============================================================================
*/

class GlowQuickPanelWidget :
	public GlowQuickPalette,
	public GlowPanelWidget
{
	friend class GlowQuickPalette;
	friend class GlowQuickPaletteWindow;
	friend class GlowQuickPaletteSubwindow;
	
	
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
	
		GlowQuickPanelWidget(
			GlowWidgetRoot* root,
			GlowQuickPanelWidget* parent,
			Style style,
			const char* label,
			Arrangement arrangement,
			Alignment alignment,
			int spacing,
			int hmargin,
			int vmargin);
	
	private:
	
		GlowLabelWidget* _label;
		Arrangement _arrangement;
		Alignment _alignment;
		int _spacing;
		int _hmargin;
		int _vmargin;
};


/*
===============================================================================
	CLASS GlowQuickPaletteWindow
	
	Quick palette window
===============================================================================
*/

class GlowQuickPaletteWindow :
	public GlowQuickPalette,
	public GlowFixedSizeWidgetWindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		GlowQuickPaletteWindow(
			const char* name,
			int x = GlowWindow::autoPosition,
			int y = GlowWindow::autoPosition,
			Arrangement arrangement = vertical,
			Alignment alignment = alignLeft,
			int spacing = 8,
			int hmargin = 10,
			int vmargin = 10);
	
	public:
	
		void Pack();
};


/*
===============================================================================
	CLASS GlowQuickPaletteSubwindow
	
	Quick palette window
===============================================================================
*/

class GlowQuickPaletteSubwindow :
	public GlowQuickPalette,
	public GlowWidgetSubwindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		GlowQuickPaletteSubwindow(
			GlowComponent* parent,
			int x = 0,
			int y = 0,
			int width = GlowSubwindow::parentWindowSize,
			int height = GlowSubwindow::parentWindowSize,
			Arrangement arrangement = vertical,
			Alignment alignment = alignLeft,
			int spacing = 8,
			int hmargin = 10,
			int vmargin = 10);
	
	public:
	
		void Pack(
			bool resizeOnPack = true);
};


/*
===============================================================================
	CLASS GlowQuickRadioGroupWidget
	
	Quick radio button group
===============================================================================
*/

class GlowQuickRadioGroupWidget :
	public GlowRadioGroupWidget
{
	friend class GlowQuickPalette;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		GlowRadioButtonWidget* AddRadioButton(
			const char* label);
	
	
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
	
		GlowQuickRadioGroupWidget(
			GlowWidget* parent,
			GlowRadioButtonReceiver* receiver,
			GlowQuickPalette::Arrangement arrangement,
			int spacing);
	
	private:
	
		GlowQuickPalette::Arrangement _arrangement;
		int _spacing;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowQuickPalette.inl.h"


#endif
