/*
===============================================================================

	FILE:  glowSliderWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Slider widget for GLOW
	
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


#ifndef GLOW_SLIDERWIDGET__H
#define GLOW_SLIDERWIDGET__H


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


class GlowSliderWidget;
class GlowSliderParams;
class GlowLabeledSliderParams;
class GlowSliderMessage;
typedef TReceiver<const GlowSliderMessage&> GlowSliderReceiver;


/*
===============================================================================
	CLASS GlowSliderWidget
	
	Slider widget
===============================================================================
*/

class GlowSliderWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Options
		{
			defaultOptions = 0,
			ticksOnBottom = 0,
			ticksOnRight = 0,
			ticksOnTop = 1,
			ticksOnLeft = 1,
			increasing = 0,
			decreasing = 2,
			linear = 0,
			logarithmic = 4
		};
	
	public:
	
		inline GlowSliderWidget();
		inline GlowSliderWidget(
			GlowWidget* parent,
			const GlowSliderParams& params);
		inline GlowSliderWidget(
			GlowWidgetRoot* root,
			const GlowSliderParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowSliderParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowSliderParams& params);
	
	public:
	
		inline double GetValue() const;
		inline double GetMinimum() const;
		inline double GetMaximum() const;
		inline void SetValue(
			double val);
		inline void SetMaximum(
			double val);
		inline void SetMinimum(
			double val);
		
		inline int GetNumTickMarks() const;
		inline void SetNumTickMarks(
			int numTicks);
		
		inline Options GetOptions() const;
		inline void SetOptions(
			Options options);
		
		inline TSender<const GlowSliderMessage&>& Notifier();
		
		inline GlowColor GetStripColor() const;
		inline GlowColor GetIndicatorColor() const;
		inline GlowColor GetTickMarkColor() const;
		inline GlowColor GetHiliteIndicatorColor() const;
		inline GlowColor GetDisableStripColor() const;
		inline GlowColor GetDisableIndicatorColor() const;
		inline GlowColor GetDisableTickMarkColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetStripColor(
			GlowColor c);
		inline void SetIndicatorColor(
			GlowColor c);
		inline void SetTickMarkColor(
			GlowColor c);
		inline void SetHiliteIndicatorColor(
			GlowColor c);
		inline void SetDisableStripColor(
			GlowColor c);
		inline void SetDisableIndicatorColor(
			GlowColor c);
		inline void SetDisableTickMarkColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowSliderParams& params);
	
	
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
		virtual void OnReleased(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		TSender<const GlowSliderMessage&> _sender;
		Options _type;
		int _xoffset;
		int _yoffset;
		double _min;
		double _max;
		double _value;
		int _numTicks;
		Glow::MouseButton _button;
		Glow::Modifiers _modifiers;
		bool _dragging;
		
		GlowColor _stripColor;
		GlowColor _indicatorColor;
		GlowColor _tickMarkColor;
		GlowColor _hiliteIndicatorColor;
		GlowColor _disableStripColor;
		GlowColor _disableIndicatorColor;
		GlowColor _disableTickMarkColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
	
	private:
	
		void _CalcValue(
			int x,
			int y);
		inline void _LeftBevelColor();
		inline void _RightBevelColor();
		inline void _TopBevelColor();
		inline void _BottomBevelColor();
	
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

GLOW_INTERNAL_SETUPENUMBITFIELD(GlowSliderWidget::Options)


/*
===============================================================================
	CLASS GlowLabeledSliderWidget
	
	Labeled slider widget
===============================================================================
*/

class GlowLabeledSliderWidget :
	public GlowSliderWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum LabelPosition
		{
			defaultLabelPosition = 0,
			leftLabelPosition = 0,
			rightLabelPosition = 1,
			topLabelPosition = 2,
			bottomLabelPosition = 3
		};
	
	public:
	
		inline GlowLabeledSliderWidget();
		inline GlowLabeledSliderWidget(
			GlowWidget* parent,
			const GlowLabeledSliderParams& params);
		inline GlowLabeledSliderWidget(
			GlowWidgetRoot* root,
			const GlowLabeledSliderParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowLabeledSliderParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowLabeledSliderParams& params);
	
	public:
	
		inline bool HasLabel() const;
		
		inline const char* GetLabelTemplate() const;
		inline void SetLabelTemplate(
			const char* text);
		inline GlowFont GetLabelFont() const;
		inline void SetLabelFont(
			GlowFont font);
		
		inline GlowColor GetLabelTextColor() const;
		inline void SetLabelTextColor(
			GlowColor c);
		inline GlowColor GetLabelDisableTextColor() const;
		inline void SetLabelDisableTextColor(
			GlowColor c);
		
		inline int GetLabelSpacing() const;
		inline void SetLabelSpacing(
			int spacing);
		inline LabelPosition GetLabelPosition() const;
		inline void SetLabelPosition(
			LabelPosition position);
		inline int GetLabelWidth() const;
		inline void SetLabelWidth(
			int width);
		inline int GetLabelHeight() const;
		inline void SetLabelHeight(
			int height);
		
		void RepositionLabel();
	
	public:
	
		inline bool HasMinmax() const;
		
		inline const char* GetMinmaxTemplate() const;
		inline void SetMinmaxTemplate(
			const char* text);
		inline GlowFont GetMinmaxFont() const;
		inline void SetMinmaxFont(
			GlowFont font);
		
		inline GlowColor GetMinmaxTextColor() const;
		inline void SetMinmaxTextColor(
			GlowColor c);
		inline GlowColor GetMinmaxDisableTextColor() const;
		inline void SetMinmaxDisableTextColor(
			GlowColor c);
		
		inline int GetMinmaxSize() const;
		inline void SetMinmaxSize(
			int size);
	
	
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
			const GlowLabeledSliderParams& params);
		
		void RepositionMinmax();
		void ResizeMinmax();
		bool CheckMinmax();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowWidgetLabelWidget* _label;
		char* _labelTemplate;
		float _labelValue;
		LabelPosition _labelPosition;
		int _labelWidth;
		int _labelHeight;
		int _labelSpacing;
		
		GlowWidgetLabelWidget* _minLabel;
		GlowWidgetLabelWidget* _maxLabel;
		char* _minmaxTemplate;
		float _minLabelValue;
		float _maxLabelValue;
		int _minmaxSize;
};


/*
===============================================================================
	CLASS GlowSliderParams
	
	Slider params
===============================================================================
*/

class GlowSliderParams :
	public GlowWidgetParams
{
	public:
	
		GlowSliderWidget::Options options;
		double min;
		double max;
		double initial;
		int numTicks;
		GlowSliderReceiver* receiver;
		GlowColor stripColor;
		GlowColor indicatorColor;
		GlowColor tickMarkColor;
		GlowColor hiliteIndicatorColor;
		GlowColor disableStripColor;
		GlowColor disableIndicatorColor;
		GlowColor disableTickMarkColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowSliderParams defaults;
		
		GlowSliderParams();
	
	protected:
	
		GlowSliderParams(bool);
};


/*
===============================================================================
	CLASS GlowLabeledSliderParams
	
	Slider label params
===============================================================================
*/

class GlowLabeledSliderParams :
	public GlowSliderParams
{
	public:
	
		const char* labelTemplate;
		GlowFont labelFont;
		GlowLabeledSliderWidget::LabelPosition labelPosition;
		int labelWidth;
		int labelHeight;
		int labelSpacing;
		GlowColor labelColor;
		GlowColor disableLabelColor;
		const char* minmaxTemplate;
		GlowFont minmaxFont;
		int minmaxSize;
		GlowColor minmaxColor;
		GlowColor disableMinmaxColor;
		
		static GlowLabeledSliderParams defaults;
		
		GlowLabeledSliderParams();
	
	protected:
	
		GlowLabeledSliderParams(bool);
};


/*
===============================================================================
	CLASS GlowSliderMessage
	
	Action for slider being dragged
===============================================================================
*/

class GlowSliderMessage
{
	public:
	
		GlowSliderWidget* widget;
		double value;
		bool released;
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowSliderWidget.inl.h"


#endif

