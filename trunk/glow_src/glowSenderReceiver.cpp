/*
===============================================================================

	FILE:  glowSenderReceiver.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Messaging system
	
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

#ifndef GLOW_SENDERRECEIVER__H
	#include "glowSenderReceiver.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of Receiver_Base
===============================================================================
*/

Receiver_Base::~Receiver_Base()
{
	for (GLOW_STD::list<Sender_Base*>::iterator iter = _senders.begin();
		iter != _senders.end(); iter++)
	{
		(*iter)->_RemoveReceiver(this);
	}
}


/*
===============================================================================
	Methods of Sender_Base
===============================================================================
*/

void Sender_Base::UnbindAll()
{
	for (GLOW_STD::list<Receiver_Base*>::iterator iter = _receivers.begin();
		iter != _receivers.end(); iter++)
	{
		(*iter)->_RemoveSender(this);
	}
	_receivers.erase(_receivers.begin(), _receivers.end());
}


Sender_Base::~Sender_Base()
{
	for (GLOW_STD::list<Receiver_Base*>::iterator iter = _receivers.begin();
		iter != _receivers.end(); iter++)
	{
		(*iter)->_RemoveSender(this);
	}
}


/*
===============================================================================
	Methods of ReceiverTracker
===============================================================================
*/

ReceiverTracker::~ReceiverTracker()
{
	UnbindAll();
}


void ReceiverTracker::DeleteAllReceivers()
{
	while (!_receivers.empty())
	{
		delete _receivers.front();
	}
}


void ReceiverTracker::Unbind(
	Receiver_Base* receiver)
{
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver);
	GLOW_DEBUG(iter == _receivers.end(),
		"ReceiverTracker not tracking TReceiver");
	receiver->_RemoveTracker(this);
	_receivers.erase(iter);
	if ((receiver->NumTrackers() == 0 && _options == referenceCountDelete) ||
		_options == alwaysDelete)
	{
		delete receiver;
	}
}


void ReceiverTracker::UnbindAll()
{
	for (GLOW_STD::list<Receiver_Base*>::iterator iter = _receivers.begin();
		iter != _receivers.end(); iter++)
	{
		(*iter)->_RemoveTracker(this);
		if (((*iter)->NumTrackers() == 0 && _options == referenceCountDelete) ||
			_options == alwaysDelete)
		{
			delete (*iter);
		}
	}
	_receivers.erase(_receivers.begin(), _receivers.end());
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

