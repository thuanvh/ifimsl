/*
===============================================================================

	FILE:  glowSeparatorWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Separator rule for GLOW
	
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


#ifndef GLOW_SEPARATORWIDGET__H
#define GLOW_SEPARATORWIDGET__H


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


class GlowSeparatorParams;


/*
===============================================================================
	CLASS GlowSeparatorWidget
	
	Separator widget
===============================================================================
*/

class GlowSeparatorWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Style
		{
			transparentStyle = 0,
			ridgeStyle = 1,
			valleyStyle = 2
		};
	
	public:
	
		inline GlowSeparatorWidget();
		inline GlowSeparatorWidget(
			GlowWidgetRoot* root,
			const GlowSeparatorParams& params);
		inline GlowSeparatorWidget(
			GlowWidget* parent,
			const GlowSeparatorParams& params);
		
		inline void Init(
			GlowWidgetRoot* root,
			const GlowSeparatorParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowSeparatorParams& params);
	
	public:
	
		inline Style GetStyle() const;
		inline void SetStyle(
			Style style);
		
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
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
			const GlowSeparatorParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		Style _style;
		
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	protected:
	
		virtual void OnWidgetPaint();
};


/*
===============================================================================
	CLASS GlowSeparatorParams
	
	Separator params
===============================================================================
*/

class GlowSeparatorParams :
	public GlowWidgetParams
{
	public:
	
		GlowSeparatorWidget::Style style;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowSeparatorParams defaults;
		
		GlowSeparatorParams();
	
	protected:
	
		GlowSeparatorParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowSeparatorWidget.inl.h"


#endif
