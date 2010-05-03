ARCH = $(shell arch )
OS = $(shell uname -s)

#---Generic include stuff---
VWDIR = 	../VW34
#VWLIBS =	-L$(VWDIR)/lib -lVWGLOW -lVWGL -lVWFirewire -lVW -lVNL
VWLIBS =	-L$(VWDIR)/lib -lVWGLOW -lVWGL -lVW -lVNL
VWHEADERS = 	-I$(VWDIR)/include 
GLLIBS =	-L/usr/X11R6/lib -lglut -lGLU -lGL
GLHEADERS =
GLOWDIRECTORY = ../glow_104/glow_src
GLOWLIBS = 	-L$(GLOWDIRECTORY) -lglow
GLOWHEADERS = 	-I$(GLOWDIRECTORY)
PTHREADLIBS =	-lpthread
#FIREWIRELIBS = 	-ldc1394_control -lraw1394
FIREWIRELIBS = 	
FIREWIREHEADERS = -I/usr/include/libdc1394 
SCENEBASEDIR = 	../SceneLib
SCENELIBS =	-L$(SCENEBASEDIR)/lib -lMonoSLAM -lSceneImproc -lScene
SCENEHEADERS = 	-I$(SCENEBASEDIR)/include
#MRPTLIBS
MRPTLIBS = -lmrpt-core
MRPTINC= -I/usr/include/mrpt/mrpt-config
# Target executable
EXECUTABLE = monoslam

#---Uncomment -D_REALTIME_ to compile realtime version (make clean
#---if you have changed this)
CFLAGS = $(GLOWHEADERS) $(VWHEADERS) $(SCENEHEADERS) $(MRPTINC) -Wall -D$(OS) -D$(ARCH) -D_REENTRANT -O3 -g #-D_REALTIME_

#--- Comment out $(FIREWIRELIBS) to compile on a machine without firewire
#--- libraries installed (for off-line mode only of course)
#--- You should also comment out -lVWFirewire from the VWLIBS line above.
LINKFLAGS = $(SCENELIBS) $(VWLIBS) $(GLOWLIBS) $(GLLIBS) -Wall -O2 $(PTHREADLIBS) $(MATHSLIBS) $(FIREWIRELIBS) $(MRPTLIBS)

# Source files
CPPFILES = mainglow.cpp monoslamglow.cpp monoslamMRPT3DScene.cpp monoslamSettings.cpp
# Object files
OFILES = $(addsuffix .o,$(basename $(CPPFILES)))

# Executable
$(EXECUTABLE): $(OFILES)
	g++ $(OFILES) -o $(EXECUTABLE) $(LINKFLAGS)

$(OFILES): %.o: %.cpp .depend_done
	g++ -c $(CFLAGS) $< -o $@

# Dependencies
.depend_done: 
	rm -f depend.mk
	- g++ -M $(CFLAGS) $(CPPFILES) >> depend.mk
	touch .depend_done

# Clean out directory
clean:
	rm -f $(EXECUTABLE)
	rm -f *~ *.o depend.mk .depend_done core

-include depend.mk

