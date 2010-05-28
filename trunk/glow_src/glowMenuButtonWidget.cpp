/*
===============================================================================

	FILE:  glowMenuButtonWidget.cpp
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_MENUBUTTONWIDGET__H
	#include "glowMenuButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	GlowMenuButtonParams defaults
===============================================================================
*/

GlowMenuButtonParams GlowMenuButtonParams::defaults(true);

GlowMenuButtonParams::GlowMenuButtonParams()
{
	*this = defaults;
}

GlowMenuButtonParams::GlowMenuButtonParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
iconColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(1.0f, 0.7f, 0.7f),
hiliteIconColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableIconColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	width = 100;
	height = 25;
	menu = 0;
	spacing = 10;
	iconType = GlowMenuButtonWidget::menuIcon;
}


/*
===============================================================================
	GlowPopupMenuParams defaults
===============================================================================
*/

GlowPopupMenuParams GlowPopupMenuParams::defaults(true);

GlowPopupMenuParams::GlowPopupMenuParams()
{
	*this = defaults;
}

GlowPopupMenuParams::GlowPopupMenuParams(bool) :
GlowWidgetParams(true),
boxColor(0.7f, 0.7f, 0.7f),
textColor(0.0f, 0.0f, 0.0f),
iconColor(0.0f, 0.0f, 1.0f),
hiliteBoxColor(0.5f, 0.5f, 0.5f),
hiliteTextColor(1.0f, 0.7f, 0.7f),
hiliteIconColor(1.0f, 0.7f, 0.7f),
disableBoxColor(0.7f, 0.7f, 0.7f),
disableTextColor(0.3f, 0.3f, 0.3f),
disableIconColor(0.3f, 0.3f, 0.3f),
disableOutlineColor(0.3f, 0.3f, 0.3f),
lightBevelColor(1.0f, 1.0f, 1.0f),
darkBevelColor(0.2f, 0.2f, 0.2f)
{
	items = 0;
	width = 200;
	height = 24;
	initial = -1;
	mark = "> ";
	font = GLUT_BITMAP_HELVETICA_12;
	spacing = 10;
	receiver = 0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabeledPopupMenuParams GlowLabeledPopupMenuParams::defaults(true);

GlowLabeledPopupMenuParams::GlowLabeledPopupMenuParams()
{
	*this = defaults;
}

GlowLabeledPopupMenuParams::GlowLabeledPopupMenuParams(bool) :
GlowPopupMenuParams(true),
labelColor(0.0f, 0.0f, 0.0f),
hiliteLabelColor(0.8f, 0.0f, 0.0f),
disableLabelColor(0.3f, 0.3f, 0.3f)
{
	labelText = "";
	labelFont = GLUT_BITMAP_HELVETICA_12;
	labelPosition = GlowWidgetLabelHelper::defaultLabelPosition;
	labelWidth = 100;
	labelHeight = 20;
	labelSpacing = 5;
}


/*
===============================================================================
	Methods of Glow_MenuButtonSubwindow
===============================================================================
*/

Glow_MenuButtonSubwindow::Glow_MenuButtonSubwindow(
	GlowWidget* parent,
	const char* label,
	GlowFont font,
	GlowMenuButtonWidget::IconType iconType,
	int leftSpacing)
{
	GLOW_DEBUGSCOPE("Glow_MenuButtonSubwindow::Glow_MenuButtonSubwindow");
	
	GlowSubwindowInWidget::Init(parent, 0, 0,
		parent->Width(), parent->Height(),
		Glow::rgbBuffer | Glow::doubleBuffer, Glow::menuEvents);
	_label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(_label, label);
	_font = font;
	_labelWidth = _font.StringWidth(_label);
	_down = false;
	_iconType = iconType;
	_leftSpacing = leftSpacing;
}


Glow_MenuButtonSubwindow::~Glow_MenuButtonSubwindow()
{
	delete[] _label;
}


void Glow_MenuButtonSubwindow::_SetLabel(
	const char* label)
{
	delete[] _label;
	_label = new char[GLOW_CSTD::strlen(label)+1];
	GLOW_CSTD::strcpy(_label, label);
	_labelWidth = _font.StringWidth(_label);
	Refresh();
}


void Glow_MenuButtonSubwindow::PreferredSize(
	int& width,
	int& height,
	const char* str)
{
	height = _font.Leading() + 12;
	if (str == 0)
	{
		width = _labelWidth + _leftSpacing + _leftSpacing;
	}
	else
	{
		width = _font.StringWidth(str) +
			_leftSpacing + _leftSpacing;
	}
	if (_iconType != 0)
	{
		width += height*3/4;
	}
}


void Glow_MenuButtonSubwindow::OnEndPaint()
{
	GLOW_DEBUGSCOPE("Glow_MenuButtonSubwindow::OnEndPaint");
	
	float bevelHeight = float(4)/float(Height());
	float bevelWidth = float(4)/float(Width());
	float etchHeight = float(2)/float(Height());
	float etchWidth = float(2)/float(Width());
	
	// Box
	if (_down)
	{
		_hiliteBoxColor.Apply();
	}
	else if (!IsActive())
	{
		_disableBoxColor.Apply();
	}
	else
	{
		_boxColor.Apply();
	}
	::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	
	// Bevels
	if (IsActive())
	{
		if (_down)
		{
			_darkBevelColor.Apply();
		}
		else
		{
			_lightBevelColor.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(-1.0f, 1.0f);
		::glVertex2f(-1.0f+bevelWidth, 1.0f-bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
		if (_down)
		{
			_lightBevelColor.Apply();
		}
		else
		{
			_darkBevelColor.Apply();
		}
		::glBegin(GL_QUAD_STRIP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f+bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(1.0f, -1.0f);
		::glVertex2f(1.0f-bevelWidth, -1.0f+bevelHeight);
		::glVertex2f(1.0f, 1.0f);
		::glVertex2f(1.0f-bevelWidth, 1.0f-bevelHeight);
		::glEnd();
	}
	else
	{
		_disableOutlineColor.Apply();
		::glBegin(GL_LINE_LOOP);
		::glVertex2f(-1.0f, -1.0f);
		::glVertex2f(-1.0f, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, 1.0f-etchHeight);
		::glVertex2f(1.0f-etchWidth, -1.0f);
		::glEnd();
	}
	
	// Label
	if (_down)
	{
		_hiliteTextColor.Apply();
	}
	else if (!IsActive())
	{
		_disableTextColor.Apply();
	}
	else
	{
		_textColor.Apply();
	}
	::glRasterPos2f(float(-1)+float(_leftSpacing*2)/float(Width()),
		float(_font.Leading()-_font.BaselinePos()*2)/float(Height()));
	int labellen = GLOW_CSTD::strlen(_label);
	for (int i=0; i<labellen; i++)
	{
		::glutBitmapCharacter(_font, _label[i]);
	}
	
	// Menu icon
	if (_iconType & (GlowMenuButtonWidget::arrowIcon | GlowMenuButtonWidget::menuIcon))
	{
		if (_down)
		{
			_hiliteIconColor.Apply();
		}
		else if (!IsActive())
		{
			_disableIconColor.Apply();
		}
		else
		{
			_iconColor.Apply();
		}
		if (_iconType == GlowMenuButtonWidget::arrowIcon)
		{
			float icontop = 0.3f;
			float iconbottom = -0.3f;
			float iconright = 1.0f-0.5f*float(Height())/float(Width());
			float iconleft = iconright - float(Height())/float(Width());
			::glBegin(GL_TRIANGLES);
			::glVertex2f(iconright, icontop);
			::glVertex2f(iconleft, icontop);
			::glVertex2f((iconleft+iconright)*0.5f, iconbottom);
			::glEnd();
		}
		else if (_iconType == GlowMenuButtonWidget::menuIcon)
		{
			float icontop = 0.65f;
			float iconbottom = -0.5f;
			float iconmid = 0.3f;
			float iconright = 1.0f-0.5f*float(Height())/float(Width());
			float iconleft = iconright - float(Height())/float(Width());
			::glBegin(GL_QUADS);
			::glVertex2f(iconleft, iconmid);
			::glVertex2f(iconleft, icontop);
			::glVertex2f(iconright, icontop);
			::glVertex2f(iconright, iconmid);
			::glVertex2f(iconleft+bevelWidth, iconbottom-bevelHeight);
			::glVertex2f(iconleft+bevelWidth, iconmid-bevelHeight);
			::glVertex2f(iconright+bevelWidth, iconmid-bevelHeight);
			::glVertex2f(iconright+bevelWidth, iconbottom-bevelHeight);
			GlowColor::white.Apply();
			::glVertex2f(iconleft, iconbottom);
			::glVertex2f(iconleft, iconmid);
			::glVertex2f(iconright, iconmid);
			::glVertex2f(iconright, iconbottom);
			::glEnd();
			if (_down)
			{
				_hiliteIconColor.Apply();
			}
			else if (!IsActive())
			{
				_disableIconColor.Apply();
			}
			else
			{
				_iconColor.Apply();
			}
			::glBegin(GL_LINE_LOOP);
			::glVertex2f(iconleft, iconbottom);
			::glVertex2f(iconleft, iconmid);
			::glVertex2f(iconright, iconmid);
			::glVertex2f(iconright, iconbottom);
			::glEnd();
		}
	}
}


void Glow_MenuButtonSubwindow::OnMenuDown(
	int x,
	int y)
{
	_down = true;
	Refresh();
}


void Glow_MenuButtonSubwindow::OnMenuUp()
{
	_down = false;
	Refresh();
}


/*
===============================================================================
	Methods of Glow_PopupMenuSubwindow
===============================================================================
*/

void Glow_PopupMenuSubwindow::OnMenuDown(
	int x,
	int y)
{
	_down = true;
	Refresh();
	_widget->Refresh();
}


void Glow_PopupMenuSubwindow::OnMenuUp()
{
	_down = false;
	Refresh();
	_widget->Refresh();
}


void Glow_PopupMenuSubwindow::OnDirectMenuHit(
	const GlowMenuMessage& message)
{
	_widget->OnHit(message.code);
}


/*
===============================================================================
	Methods of GlowMenuButton
===============================================================================
*/

void GlowMenuButtonWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowMenuButtonParams& params)
{
	GLOW_DEBUGSCOPE("GlowMenuButtonWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	_subwindow = new Glow_MenuButtonSubwindow(this, params.text,
		params.font, params.iconType, params.spacing);
	_subwindow->_boxColor = params.boxColor;
	_subwindow->_textColor = params.textColor;
	_subwindow->_iconColor = params.iconColor;
	_subwindow->_hiliteBoxColor = params.hiliteBoxColor;
	_subwindow->_hiliteTextColor = params.hiliteTextColor;
	_subwindow->_hiliteIconColor = params.hiliteIconColor;
	_subwindow->_disableBoxColor = params.disableBoxColor;
	_subwindow->_disableTextColor = params.disableTextColor;
	_subwindow->_disableIconColor = params.disableIconColor;
	_subwindow->_disableOutlineColor = params.disableOutlineColor;
	_subwindow->_lightBevelColor = params.lightBevelColor;
	_subwindow->_darkBevelColor = params.darkBevelColor;
	if (params.menu != 0)
	{
		_subwindow->_SetMenu(params.menu);
	}
}


GlowWidget::AutoPackError GlowMenuButtonWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowMenuButtonWidget::OnAutoPack");
	
	int pwidth = 0, pheight = 0;
	_subwindow->PreferredSize(pwidth, pheight);
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	
	int vnew = Height();
	if (vSize != unspecifiedSize && vSize < pheight)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == expandPreferredSize || vOption == preferredSize)
	{
		vnew = pheight;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowMenuButtonWidget::OnWidgetMove()
{
	_subwindow->Move(RootPositionX(), RootPositionY());
}


void GlowMenuButtonWidget::OnWidgetReshape()
{
	_subwindow->Reshape(Width(), Height());
}


void GlowMenuButtonWidget::OnWidgetVisible()
{
	_subwindow->Show();
}


void GlowMenuButtonWidget::OnWidgetInvisible()
{
	_subwindow->Hide();
}


/*
===============================================================================
	Methods of GlowPopupMenu
===============================================================================
*/

void GlowPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowPopupMenuParams& params)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	
	// Mark
	if (params.mark != 0)
	{
		_mark = new char[GLOW_CSTD::strlen(params.mark)+1];
		GLOW_CSTD::strcpy(_mark, params.mark);
	}
	else
	{
		_mark = 0;
	}
	
	// Menu
	_menu = new GlowMenu;
	_menu->SetBindState(GlowMenu::bindSubwindow);
	
	// Fill in items
	if (params.items != 0)
	{
		char* tempbuf = new char[GLOW_CSTD::strlen(params.items)+1];
		GLOW_CSTD::strcpy(tempbuf, params.items);
		for (int i=0; ; ++i)
		{
			char* tok = GLOW_CSTD::strtok(i==0 ? tempbuf : 0, "\t");
			if (tok == 0) break;
			_menu->AddEntry(tok, i);
		}
		delete[] tempbuf;
	}
	
	// Find and mark initial current item
	_curItem = params.initial;
	if (_curItem < 0) _curItem = 0;
	if (_curItem >= _menu->NumItems()) _curItem = _menu->NumItems()-1;
	if (_mark != 0 && _curItem != -1)
	{
		_menu->SetItemMark(_curItem, _mark);
	}
	
	// Menubutton subwindow
	_subwindow = new Glow_PopupMenuSubwindow(this, "", params.font,
		GlowMenuButtonWidget::arrowIcon, params.spacing);
	_subwindow->_boxColor = params.boxColor;
	_subwindow->_textColor = params.textColor;
	_subwindow->_iconColor = params.iconColor;
	_subwindow->_hiliteBoxColor = params.hiliteBoxColor;
	_subwindow->_hiliteTextColor = params.hiliteTextColor;
	_subwindow->_hiliteIconColor = params.hiliteIconColor;
	_subwindow->_disableBoxColor = params.disableBoxColor;
	_subwindow->_disableTextColor = params.disableTextColor;
	_subwindow->_disableIconColor = params.disableIconColor;
	_subwindow->_disableOutlineColor = params.disableOutlineColor;
	_subwindow->_lightBevelColor = params.lightBevelColor;
	_subwindow->_darkBevelColor = params.darkBevelColor;
	_subwindow->_SetMenu(_menu);
	if (_curItem >= 0)
	{
		_subwindow->_SetLabel(_menu->GetItemLabel(_curItem));
	}
	
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
}


GlowPopupMenuWidget::~GlowPopupMenuWidget()
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::~GlowPopupMenuWidget");
	
	delete[] _mark;
}


void GlowPopupMenuWidget::SetMark(
	const char* mark)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::SetMark");
	
	delete _mark;
	_mark = new char[GLOW_CSTD::strlen(mark)+1];
	GLOW_CSTD::strcpy(_mark, mark);
	_menu->UnmarkItem(_curItem);
	if (_mark != 0)
	{
		_menu->SetItemMark(_curItem, _mark);
	}
}


GlowWidget::AutoPackError GlowPopupMenuWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::OnAutoPack");
	
	int pwidth = 0, pheight = 0;
	_subwindow->PreferredSize(pwidth, pheight, "");
	for (int i=0; i<NumItems(); i++)
	{
		int w = 0;
		_subwindow->PreferredSize(w, pheight, GetItemLabel(i));
		if (w > pwidth) pwidth = w;
	}
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	
	int vnew = Height();
	if (vSize != unspecifiedSize && vSize < pheight)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize)
	{
		vnew = vSize;
	}
	else if (vOption == expandPreferredSize || vOption == preferredSize)
	{
		vnew = pheight;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


int GlowPopupMenuWidget::AddItem(
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::AddItem");
	
	_menu->AddEntry(label, _menu->NumItems());
	if (_menu->NumItems() == 1)
	{
		_curItem = 0;
		_subwindow->_SetLabel(_menu->GetItemLabel(_curItem));
		if (_mark != 0)
		{
			_menu->SetItemMark(_curItem, _mark);
		}
		_subwindow->Refresh();
	}
	return _menu->NumItems()-1;
}


void GlowPopupMenuWidget::RemoveItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::RemoveItem");
	
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < _menu->NumItems());
	
	int max = _menu->NumItems();
	for (int i=itemNum+1; i<max; i++)
	{
		_menu->SetItemCode(i, i-1);
	}
	if (_curItem == itemNum)
	{
		if (max == 1)
		{
			_curItem = -1;
			_subwindow->_SetLabel("");
		}
		else
		{
			_curItem = 0;
			_subwindow->_SetLabel(_menu->GetItemLabel(_curItem));
		}
		_subwindow->Refresh();
	}
	else if (_curItem > itemNum)
	{
		_curItem--;
	}
	_menu->RemoveItem(itemNum);
}


void GlowPopupMenuWidget::SetItemLabel(
	int itemNum,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::SetItemLabel");
	
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < _menu->NumItems());
	
	_menu->SetItemLabel(itemNum, label);
	if (_curItem == itemNum)
	{
		_subwindow->_SetLabel(label);
		_subwindow->Refresh();
	}
}


void GlowPopupMenuWidget::SetCurItem(
	int itemNum)
{
	GLOW_DEBUGSCOPE("GlowPopupMenuWidget::SetCurItem");
	
	GLOW_ASSERT(itemNum >= 0);
	GLOW_ASSERT(itemNum < _menu->NumItems());
	
	if (itemNum != _curItem)
	{
		_menu->UnmarkItem(_curItem);
		_curItem = itemNum;
		_subwindow->_SetLabel(_menu->GetItemLabel(_curItem));
		if (_mark != 0)
		{
			_menu->SetItemMark(_curItem, _mark);
		}
	}
}


void GlowPopupMenuWidget::OnWidgetMove()
{
	_subwindow->Move(RootPositionX(), RootPositionY());
}


void GlowPopupMenuWidget::OnWidgetReshape()
{
	_subwindow->Reshape(Width(), Height());
}


void GlowPopupMenuWidget::OnWidgetVisible()
{
	_subwindow->Show();
}


void GlowPopupMenuWidget::OnWidgetInvisible()
{
	_subwindow->Hide();
}


void GlowPopupMenuWidget::OnHit(
	int item)
{
	GlowPopupMenuMessage outmsg;
	outmsg.oldItem = _curItem;
	if (_curItem != item)
	{
		_menu->UnmarkItem(_curItem);
		_curItem = item;
		_subwindow->_SetLabel(_menu->GetItemLabel(_curItem));
		if (_mark != 0)
		{
			_menu->SetItemMark(_curItem, _mark);
		}
	}
	outmsg.item = item;
	outmsg.widget = this;
	_sender.Send(outmsg);
}


/*
===============================================================================
	Methods of GlowLabeledPopupMenuWidget
===============================================================================
*/

void GlowLabeledPopupMenuWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabeledPopupMenuParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabeledPopupMenuWidget::Init");
	
	GlowPopupMenuWidget::Init(root, parent, params);
	InitLabel(this, params.labelPosition, params.labelWidth,
		params.labelHeight, params.labelSpacing, params.labelText,
		params.labelFont, params.labelColor, params.disableLabelColor);
	_upLabelColor = params.labelColor;
	_downLabelColor = params.hiliteLabelColor;
}


void GlowLabeledPopupMenuWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowLabeledPopupMenuWidget::OnWidgetPaint");
	
	Label()->SetRefreshEnabled(false);
	if (IsDown())
	{
		Label()->SetTextColor(_downLabelColor);
	}
	else
	{
		Label()->SetTextColor(_upLabelColor);
	}
	Label()->SetRefreshEnabled(true);
}


GlowWidget::AutoPackError GlowLabeledPopupMenuWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabeledPopupMenuWidget::OnAutoPack");
	
	// Use helper
	AutoPackError result = HelpAutoPack(hSize, vSize, leftMargin, rightMargin,
		topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	result = GlowPopupMenuWidget::OnAutoPack(hSize, vSize, hOption,
		vOption, leftMargin, rightMargin, topMargin, bottomMargin);
	if (result != noAutoPackError)
	{
		return result;
	}
	
	RepositionLabel();
	return noAutoPackError;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

