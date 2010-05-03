/*  MonoSLAM: Real-Time Single Camera SLAM

    MonoSLAMGlow/monoslamglow.cpp
    Copyright (C) 2005 University of Oxford 

    Author
    Andrew Davison
    ajd@robots.ox.ac.uk
    Scene home page: http://www.robots.ox.ac.uk/~ajd/Scene/

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "monoslamglow.h"
#include "monoslamSettings.h"

int main(int argc, char *argv[]) {
  MonoslamSettings ms;
  if(argc>3)
    ms.readSettings(argv[3]);
  // Initialize GLOW runtime
  Glow::Init(argc, argv);
	
  // Create one window to display one data object
  MonoSLAMGlow monoslamglow(argc, argv,ms);

  // Initialise the random seed to something random
//  srand48(time(0)); // For true random behaviour
  srand48(0); // Always the same seed (pick a number), so deterministic
	
  // Enter event loop
  Glow::MainLoop();
	
  return 0;
}
