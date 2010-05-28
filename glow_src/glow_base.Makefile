
#===============================================================================
#
#	FILE:  glow_base.Makefile
#	
#	PROJECT:
#	
#		The GLOW Toolkit
#	
#	CONTENTS:
#	
#		A makefile base you can use to develop GLOW programs
#	
#	PROGRAMMERS:
#	
#		Daniel Azuma (DA) <dazuma@kagi.com>
#	
#	COPYRIGHT:
#	
#		Copyright (C) 1997-2000  Daniel Azuma  (dazuma@kagi.com)
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


_ARCH:=$(shell uname -s)
_FILEOPTS:=$(patsubst .glowmake.%,%,$(wildcard .glowmake.*))
_DEBUGFILEOPT:=$(filter DEBUG,$(_FILEOPTS))
_STRIPFILEOPT:=$(filter STRIP,$(_FILEOPTS))
_GLOWLIBFILEOPT:=$(filter GLOWLIB,$(_FILEOPTS))
_OTHERFILEOPTS:=$(filter-out DEBUG STRIP GLOWLIB,$(_FILEOPTS))
_FIRSTFILEOPT:=$(firstword $(_OTHERFILEOPTS))

ifneq ($(origin PROG_$(_ARCH)),undefined)
PROG:=$(PROG_$(_ARCH))
endif
ifneq ($(origin PROG_$(_FIRSTFILEOPT)),undefined)
PROG:=$(PROG_$(_FIRSTFILEOPT))
endif
ifneq ($(origin PROG_$(_ARCH)_$(_FIRSTFILEOPT)),undefined)
PROG:=$(PROG_$(_ARCH)_$(_FIRSTFILEOPT))
endif
ifndef PROG
_MISSINGSTUFF:=PROG
endif

ifneq ($(origin SRCDIR_$(_ARCH)),undefined)
SRCDIR:=$(SRCDIR_$(_ARCH))
endif
ifneq ($(origin SRCDIR_$(_FIRSTFILEOPT)),undefined)
SRCDIR:=$(SRCDIR_$(_FIRSTFILEOPT))
endif
ifneq ($(origin SRCDIR_$(_ARCH)_$(_FIRSTFILEOPT)),undefined)
SRCDIR:=$(SRCDIR_$(_ARCH)_$(_FIRSTFILEOPT))
endif
ifndef SRCDIR
_MISSINGSTUFF+=SRCDIR
endif

ifneq ($(origin GLOWDIR_$(_ARCH)),undefined)
GLOWDIR:=$(GLOWDIR_$(_ARCH))
endif
ifneq ($(origin GLOWDIR_$(_FIRSTFILEOPT)),undefined)
GLOWDIR:=$(GLOWDIR_$(_FIRSTFILEOPT))
endif
ifneq ($(origin GLOWDIR_$(_ARCH)_$(_FIRSTFILEOPT)),undefined)
GLOWDIR:=$(GLOWDIR_$(_ARCH)_$(_FIRSTFILEOPT))
endif
ifndef GLOWDIR
_MISSINGSTUFF+=GLOWDIR
endif

ifneq ($(origin MODULES_$(_ARCH)),undefined)
MODULES:=$(MODULES_$(_ARCH))
endif
MODULES+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(MODULES_$(_TEMP)))
ifeq ($(strip $(MODULES)),)
_MISSINGSTUFF+=MODULES
endif

ifneq ($(origin GLOWMODULES_$(_ARCH)),undefined)
GLOWMODULES:=$(GLOWMODULES_$(_ARCH))
endif
GLOWMODULES+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(GLOWMODULES_$(_TEMP)))

ifneq ($(origin COMPILE_$(_ARCH)),undefined)
COMPILE:=$(COMPILE_$(_ARCH))
endif
ifneq ($(origin COMPILE_$(_FIRSTFILEOPT)),undefined)
COMPILE:=$(COMPILE_$(_FIRSTFILEOPT))
endif
ifneq ($(origin COMPILE_$(_ARCH)_$(_FIRSTFILEOPT)),undefined)
COMPILE:=$(COMPILE_$(_ARCH)_$(_FIRSTFILEOPT))
endif
ifndef COMPILE
_MISSINGSTUFF+=COMPILE
endif

ifneq ($(origin LINK_$(_ARCH)),undefined)
LINK:=$(LINK_$(_ARCH))
endif
ifneq ($(origin LINK_$(_FIRSTFILEOPT)),undefined)
LINK:=$(LINK_$(_FIRSTFILEOPT))
endif
ifneq ($(origin LINK_$(_ARCH)_$(_FIRSTFILEOPT)),undefined)
LINK:=$(LINK_$(_ARCH)_$(_FIRSTFILEOPT))
endif
ifndef LINK
LINK:=$(COMPILE) -o $(PROG)
endif

ifneq ($(origin SHELL_$(_ARCH)),undefined)
SHELL:=$(SHELL_$(_ARCH))
endif
ifneq ($(origin SHELL_$(_FIRSTFILEOPT)),undefined)
SHELL:=$(SHELL_$(_FIRSTFILEOPT))
endif
ifneq ($(origin SHELL_$(_ARCH)_$(_FIRSTFILEOPT)),undefined)
SHELL:=$(SHELL_$(_ARCH)_$(_FIRSTFILEOPT))
endif
ifndef SHELL
SHELL:=/bin/sh
endif

ifneq ($(origin DEPFLAGS_$(_ARCH)),undefined)
DEPFLAGS:=$(DEPFLAGS_$(_ARCH))
endif
ifndef DEPFLAGS
DEPFLAGS:=-M
endif

ifneq ($(origin CFLAGS_$(_ARCH)),undefined)
CFLAGS:=$(CFLAGS_$(_ARCH))
endif
CFLAGS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(CFLAGS_$(_TEMP)))

ifneq ($(origin LDFLAGS_$(_ARCH)),undefined)
LDFLAGS:=$(LDFLAGS_$(_ARCH))
endif
LDFLAGS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(LDFLAGS_$(_TEMP)))

ifneq ($(origin LIBS_$(_ARCH)),undefined)
LIBS:=$(LIBS_$(_ARCH))
endif
LIBS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(LIBS_$(_TEMP)))

ifneq ($(origin DEBUGCFLAGS_$(_ARCH)),undefined)
DEBUGCFLAGS:=$(DEBUGCFLAGS_$(_ARCH))
endif
ifndef DEBUGCFLAGS
DEBUGCFLAGS:=-g -DGLOW_OPTION_DEBUG
endif
DEBUGCFLAGS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(DEBUGCFLAGS_$(_TEMP)))

ifneq ($(origin NODEBUGCFLAGS_$(_ARCH)),undefined)
NODEBUGCFLAGS:=$(NODEBUGCFLAGS_$(_ARCH))
endif
ifndef NODEBUGCFLAGS
NODEBUGCFLAGS:=-DNODEBUG
endif
NODEBUGCFLAGS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(NODEBUGCFLAGS_$(_TEMP)))

ifneq ($(origin DEBUGLDFLAGS_$(_ARCH)),undefined)
DEBUGLDFLAGS:=$(DEBUGLDFLAGS_$(_ARCH))
endif
DEBUGLDFLAGS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(DEBUGLDFLAGS_$(_TEMP)))

ifneq ($(origin NODEBUGLDFLAGS_$(_ARCH)),undefined)
NODEBUGLDFLAGS:=$(NODEBUGLDFLAGS_$(_ARCH))
endif
NODEBUGLDFLAGS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(NODEBUGLDFLAGS_$(_TEMP)))

ifneq ($(origin DEBUGLIBS_$(_ARCH)),undefined)
DEBUGLIBS:=$(DEBUGLIBS_$(_ARCH))
endif
DEBUGLIBS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(DEBUGLIBS_$(_TEMP)))

ifneq ($(origin NODEBUGLIBS_$(_ARCH)),undefined)
NODEBUGLIBS:=$(NODEBUGLIBS_$(_ARCH))
endif
NODEBUGLIBS+=$(foreach _TEMP,$(_OTHERFILEOPTS),$(NODEBUGLIBS_$(_TEMP)))

CLEANFILES+=

ifeq ($(_DEBUGFILEOPT),DEBUG)
CFLAGS+=$(DEBUGCFLAGS)
LDFLAGS+=$(DEBUGLDFLAGS)
LIBS+=$(DEBUGLIBS)
else
CFLAGS+=$(NODEBUGCFLAGS)
LDFLAGS+=$(NODEBUGLDFLAGS)
LIBS+=$(NODEBUGLIBS)
endif

ifndef DEPSUFFIX
DEPSUFFIX:=dep
endif

ifeq ($(_GLOWLIBFILEOPT),GLOWLIB)
_OBJS:=$(addsuffix .o,$(MODULES))
_DEPS:=$(addsuffix .$(DEPSUFFIX),$(MODULES))
else
_OBJS:=$(addsuffix .o,$(GLOWMODULES) $(MODULES))
_DEPS:=$(addsuffix .$(DEPSUFFIX),$(GLOWMODULES) $(MODULES))
endif
VPATH=$(SRCDIR):$(GLOWDIR)

$(PROG): $(_OBJS)
ifdef _MISSINGSTUFF
	@echo
	@echo Symbols missing from makefile:
	@echo "  $(_MISSINGSTUFF)"
	@echo
	@echo +++ Terminating build +++
	@echo
else
	@echo
	@echo --- Linker: $@ ---
ifeq ($(_GLOWLIBFILEOPT),GLOWLIB)
ifeq ($(_DEBUGFILEOPT),DEBUG)
	$(LINK) $(LDFLAGS) $^ -lglowdebug $(LIBS)
else
	$(LINK) $(LDFLAGS) $^ -lglow $(LIBS)
endif
else
	$(LINK) $(LDFLAGS) $^ $(LIBS)
endif
	@echo
ifeq ($(_STRIPFILEOPT),STRIP)
	@echo --- Post-linker: $@ ---
	strip $@
	@echo
endif
	@echo +++ Finished build: $@ +++
	@echo
	@echo Notes:
	@echo "  Architecture: $(_ARCH)"
ifeq ($(_DEBUGFILEOPT),DEBUG)
	@echo "  Debug version"
else
	@echo "  Non-debug version"
endif
ifneq ($(strip $(_OTHERFILEOPTS)),)
	@echo "  File-options: $(_OTHERFILEOPTS)"
endif
	@echo
endif

.PHONY: all
all: $(PROG)

.DELETE_ON_ERROR:

.PHONY: info
info:
	@echo
	@echo +++ Information on make options +++
	@echo
	@echo PROGRAM: $(PROG)
	@echo SOURCE-DIRECTORY: $(SRCDIR)
	@echo GLOW-SOURCE-DIRECTORY: $(GLOWDIR)
	@echo
	@echo ARCHITECTURE: $(_ARCH)
ifeq ($(_GLOWLIBFILEOPT),GLOWLIB)
	@echo LINKING WITH GLOW LIBRARY
else
	@echo COMPILING GLOW SOURCES
endif
ifeq ($(_DEBUGFILEOPT),DEBUG)
	@echo DEBUG VERSION
else
	@echo NON-DEBUG VERSION
endif
ifeq ($(_STRIPFILEOPT),STRIP)
	@echo STRIP FINAL BINARY
endif
ifeq ($(strip $(_OTHERFILEOPTS)),)
	@echo NO CUSTOM FILE-OPTIONS
else
	@echo CUSTOM FILE-OPTIONS: $(_OTHERFILEOPTS)
endif
	@echo
	@echo COMPILER: $(COMPILE)
	@echo SHELL: $(SHELL)
	@echo
	@echo MODULES: $(MODULES)
	@echo
	@echo GLOW-MODULES: $(GLOWMODULES)
	@echo
	@echo COMPILER-FLAGS: $(CFLAGS)
	@echo
	@echo LINKER-FLAGS: $(LDFLAGS)
	@echo
	@echo LIBRARIES: $(LIBS)
	@echo
	@echo +++ End information +++
	@echo

.PHONY: cleanobjs
cleanobjs:
	@echo
	@echo --- Cleaning object files: $(PROG) ---
	rm -f *.o
	@echo
	@echo +++ Finished cleanobjs +++
	@echo

.PHONY: cleandeps
cleandeps:
	@echo
	@echo --- Cleaning dependencies: $(PROG) ---
	rm -f *.$(DEPSUFFIX)
	@echo
	@echo +++ Finished cleandeps +++
	@echo

.PHONY: clean
clean:
	@echo
	@echo --- Cleaning: $(PROG) ---
	rm -f -r $(PROG) *.o *.$(DEPSUFFIX) ii_files so_locations $(CLEANFILES)
	@echo
	@echo +++ Finished clean +++
	@echo

.PHONY: depend
depend:
	@echo
	@echo +++ Finished dependencies +++
	@echo

%.$(DEPSUFFIX): %.cpp
	@echo
	@echo --- Dependency generator: $* ---
	$(SHELL) -ec '$(COMPILE) $(DEPFLAGS) $(CFLAGS) -I$(GLOWDIR) $< \
	| sed '\''s/'$*\.o:'/'$*\.o' '$*\.$(DEPSUFFIX):'/g'\'' > $@'

%.o: %.cpp
	@echo
	@echo --- Compiler: $* ---
	$(COMPILE) -c $(CFLAGS) -I$(GLOWDIR) -o $@ $<

%.h:
	@echo File in dependency missing: $@

IGNOREDEPTARGETS+=info cleanobjs cleandeps clean

ifndef _MISSINGSTUFF
ifeq ($(MAKECMDGOALS),)
ifneq ($(filter .,$(IGNOREDEPTARGETS)),.)
include $(_DEPS)
endif
else
ifneq ($(filter $(MAKECMDGOALS),$(IGNOREDEPTARGETS)),$(MAKECMDGOALS))
include $(_DEPS)
endif
endif
endif

