
#===============================================================================
#
#	FILE:  Makefile
#	
#	PROJECT:
#	
#		The GLOW Toolkit
#	
#	CONTENTS:
#	
#		Makefile for building GLOW libraries.
#		(Temporary version until I get autoconf working)
#	
#	PROGRAMMERS:
#	
#		Daniel Azuma (DA) <dazuma@kagi.com>
#	
#	COPYRIGHT:
#	
#		Copyright (C) 2000  Daniel Azuma  (dazuma@kagi.com)
#		
#		This library is free software; you can redistribute it and/or
#		modify it under the terms of the GNU Lesser General Public
#		License as published by the Free Software Foundation; either
#		version 2.1 of the License, or (at your option) any later version.
#		
#		This library is distributed in the hope that it will be useful,
#		but WITHOUT ANY WARRANTY; without even the implied warranty of
#		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#		GNU Lesser General Public License for more details.
#		
#		You should have received a copy of the GNU Lesser General Public
#		License along with this library; if not, write to
#			Free Software Foundation, Inc.
#			59 Temple Place, Suite 330
#			Boston, MA 02111-1307 USA
#	
#	VERSION:
#	
#		The GLOW Toolkit -- version 1.0.3  (10 September 2000)
#	
#	CHANGES:
#	
#		Original release (DA)
#
#===============================================================================


.PHONY: everything debug nodebug

everything: debug nodebug

nodebug:
	rm -f .glowmake.DEBUG
	$(MAKE) clean
	touch .glowmake.SHARED
	$(MAKE) libglow.so
	mv libglow.so libglow.so.1.0.2
	rm -r .glowmake.SHARED
	$(MAKE) libglow.a
	mv libglow.a libglow.a.1.0.2

debug:
	touch .glowmake.DEBUG
	$(MAKE) clean
	touch .glowmake.SHARED
	$(MAKE) libglow.so
	mv libglow.so libglowdebug.so.1.0.2
	rm -r .glowmake.SHARED
	$(MAKE) libglow.a
	mv libglow.a libglowdebug.a.1.0.2

CLEANFILES:=libglow.so libglow.a
IGNOREDEPTARGETS:=everything debug nodebug .

PROG_SHARED=libglow.so
PROG=libglow.a

SRCDIR=.
GLOWDIR=.

MODULES=\
	glow \
	glowAux \
	glowCheckBoxWidget \
	glowDebug \
	glowLabelWidget \
	glowMenuButtonWidget \
	glowMessageWindow \
	glowPanelWidget \
	glowPushButtonWidget \
	glowQuickPalette \
	glowRadioButtonWidget \
	glowRenderSwitch \
	glowScrollBarWidget \
	glowSenderReceiver \
	glowSeparatorWidget \
	glowSliderWidget \
	glowTextData \
	glowTextFieldWidget \
	glowTextFieldWindow \
	glowUtilities \
	glowVectorAlgebra \
	glowWidget


COMPILE_Linux=gcc
COMPILE_IRIX=CC
LINK_Linux=ar crs libglow.a
LINK_IRIX=CC -ar -o libglow.a
LINK_Linux_SHARED=gcc -shared -o libglow.so
LINK_IRIX_SHARED=CC -shared -o libglow.a
SHELL=/bin/sh

#LIBS=-lglut -lpthread -lGL -lGLU -lXi -lXmu -lX11 -lm -lstdc++
#LIBS_IRIX=-lCio -lglut -lX11 -lXmu -lXext -lGL -lGLU -lm

DEPFLAGS_Linux=-MM
DEPFLAGS_IRIX=-M

CFLAGS_Linux=-Wall \
	-DGLOW_PLATFORM_LINUX \
	-DGLOW_PLATFORM_LITTLEENDIAN
NODEBUGCFLAGS_Linux=-O2 -DNODEBUG

CFLAGS_IRIX=-fullwarn -no_auto_include \
	-LANG:std \
	-woff 1209,3201,1424,1375 \
	-OPT:Olimit=0 \
	-I../Compat-SGI \
	-DGLOW_COMPAT_CLIBNOSTDNAMESPACE \
	-DGLOW_PLATFORM_IRIX \
	-DGLOW_PLATFORM_BIGENDIAN
NODEBUGCFLAGS_IRIX=-O2 -DNODEBUG


include $(GLOWDIR)/glow_base.Makefile
