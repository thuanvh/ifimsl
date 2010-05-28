/*
===============================================================================

	FILE:  glowRenderSwitch.h
	
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


#ifndef GLOW_RENDERSWITCH__H
#define GLOW_RENDERSWITCH__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW__H
	#include "glow.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS GlowRenderSwitch

	Render on/off switch
===============================================================================
*/

class GlowRenderSwitch :
	public GlowComponent
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowRenderSwitch(
			GlowComponent* parent);
		inline GlowRenderSwitch();
		inline void Init(
			GlowComponent* parent);
		
		inline void Enable();
		inline void Disable();
		inline void SetEnabled(
			bool enabled);
		inline bool IsEnabled() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		bool _enabled;
	
	protected:
	
		virtual bool OnBeginPaint();
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowRenderSwitch.inl.h"


#endif
