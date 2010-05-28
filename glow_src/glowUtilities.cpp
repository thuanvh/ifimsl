/*
===============================================================================

	FILE:  glowUtilities.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Extensions and utilities for GLOW
	
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

#ifndef GLOW_UTILITIES__H
	#include "glowUtilities.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods for GlowDeferredTask
===============================================================================
*/

GlowDeferredTask::~GlowDeferredTask()
{
	GLOW_DEBUGSCOPE("GlowDeferredTask::~GlowDeferredTask");
	
	if (_curTimerID != 0)
	{
		Glow::UnregisterTimer(_curTimerID);
	}
}


void GlowDeferredTask::OnMessage(
	const GlowTimerMessage& message)
{
	GLOW_DEBUGSCOPE("GlowDeferredTask::OnMessage");
	
	_curTimerID = 0;
	Task();
}


/*
===============================================================================
	Methods of GlowFixedSizeWindow
===============================================================================
*/

void GlowFixedSizeWindow::OnReshape(
	int width,
	int height)
{
	GLOW_DEBUGSCOPE("GlowFixedSizeWindow::OnReshape");
	
	if (width != _canonicalWidth || height != _canonicalHeight)
	{
		Reshape(_canonicalWidth, _canonicalHeight);
	}
	else
	{
//		::glViewport(0, 0, _canonicalWidth, _canonicalHeight);
		GlowSubwindow::OnReshape(_canonicalWidth, _canonicalHeight);
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

