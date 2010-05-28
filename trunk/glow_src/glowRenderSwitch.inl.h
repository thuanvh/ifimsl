/*
===============================================================================

	FILE:  glowRenderSwitch.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Render on/off switch for GLOW
	
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

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowRenderSwitch
===============================================================================
*/

inline GlowRenderSwitch::GlowRenderSwitch(
	GlowComponent* parent) :
GlowComponent(parent)
{
	_enabled = true;
}


inline GlowRenderSwitch::GlowRenderSwitch()
{
}


inline void GlowRenderSwitch::Init(
	GlowComponent* parent)
{
	GlowComponent::Init(parent);
	_enabled = true;
}


inline void GlowRenderSwitch::Enable()
{
	_enabled = true;
	ParentWindow()->Refresh();
}


inline void GlowRenderSwitch::Disable()
{
	_enabled = false;
	ParentWindow()->Refresh();
}


inline void GlowRenderSwitch::SetEnabled(
	bool enabled)
{
	_enabled = enabled;
	ParentWindow()->Refresh();
}


inline bool GlowRenderSwitch::IsEnabled() const
{
	return _enabled;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

