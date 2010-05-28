/*
===============================================================================

	FILE:  glowPanelWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Panel widget for GLOW
	
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


#ifndef GLOW_PANELWIDGET__H
#define GLOW_PANELWIDGET__H


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


class GlowPanelParams;


/*
===============================================================================
	CLASS GlowPanelWidget
	
	Panel widget
===============================================================================
*/

class GlowPanelWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Style
		{
			transparentStyle = 0,
			plainStyle = 1,
			etchedStyle = 2,
			raisedStyle = 3,
			loweredStyle = 4
		};
	
	public:
	
		inline GlowPanelWidget();
		inline GlowPanelWidget(
			GlowWidgetRoot* root,
			const GlowPanelParams& params);
		inline GlowPanelWidget(
			GlowWidget* parent,
			const GlowPanelParams& params);
		
		inline void Init(
			GlowWidgetRoot* root,
			const GlowPanelParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowPanelParams& params);
	
	public:
	
		inline Style GetStyle() const;
		inline void SetStyle(
			Style style);
		
		inline GlowColor GetBackColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBackColor(
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
			const GlowPanelParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		Style _style;
		
		GlowColor _backColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	protected:
	
		virtual void OnWidgetPaint();
};


/*
===============================================================================
	CLASS GlowPanelParams
	
	Panel widget params
===============================================================================
*/

class GlowPanelParams :
	public GlowWidgetParams
{
	public:
	
		GlowPanelWidget::Style style;
		GlowColor backColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowPanelParams defaults;
		
		GlowPanelParams();
	
	protected:
	
		GlowPanelParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowPanelWidget.inl.h"


#endif
