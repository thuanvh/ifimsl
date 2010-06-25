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
#include <mrpt/gui/CMyGLCanvasBase.h>

int main(int argc, char *argv[]) {
  MonoslamSettings ms;
  if (argc > 3)
    ms.readSettings(argv[3]);



  //mrpt::gui::CMyGLCanvasBase::InitGL();


  //  win.doRender();

  //CDisplayWindow3D* win = new CDisplayWindow3D("mywindows");
//  CDisplayWindow3D win("mywindows");
//  win.repaint();
//  //win.InitGL();
//
//  COpenGLScenePtr& mrpt3dsceneMain = win.get3DSceneAndLock();
//
//  COpenGLScenePtr mrpt3dscene = COpenGLScene::Create();
//
//  CPose3DPDFParticles curPDF; // current particle filter
//
//  mrpt3dscene = COpenGLScene::Create();
//
//  // The ground:
//  mrpt::opengl::CGridPlaneXYPtr groundPlane = mrpt::opengl::CGridPlaneXY::Create(-200, 200, -200, 200, 0, 2);
//  groundPlane->setColor(0.4, 0.4, 0.4);
//  mrpt3dscene->insert(groundPlane);
//
//  CPointCloudPtr pointClound = mrpt::opengl::CPointCloud::Create();
//  pointClound->insertPoint(1, 1, 1);
//  CEllipsoidPtr objEllip = CEllipsoid::Create();
//
//  objEllip->setLocation(1, 1, 1);
//  CMatrixDouble matrix;
//  matrix.unit(3);
//  int size = 3;
//
//  //S = [1 .2 -.3;.2 2 .4;-.3 .4 3];
//  matrix.resize(size, size);
//  matrix(0, 0) = 1;
//  matrix(0, 1) = 0.2;
//  matrix(0, 2) = -0.3;
//  matrix(1, 0) = 0.3;
//  matrix(1, 1) = 2;
//  matrix(1, 2) = 0.4;
//  matrix(2, 0) = -0.3;
//  matrix(2, 1) = 0.4;
//  matrix(2, 2) = 3;
//
//  //  for (int i = 0; i < size; i++) {
//  //    for (int j = 0; j < size; j++) {
//  //      (matrix) (j, i) = (i + j) / 6;
//  //      cout << (matrix) (j, i) << " ";
//  //    }
//  //  }
//
//  objEllip->setCovMatrix(matrix);
//
//  objEllip->setColor(0, 1, 1);
//  objEllip->enableDrawSolid3D(false);
//  mrpt3dscene->insert(objEllip);
//
//  CSetOfLinesPtr lineGroup = CSetOfLines::Create();
//  lineGroup->appendLine(0, 0, 0, 5, 5, 5);
//  lineGroup->setColor(1, 0, 1);
//  mrpt3dscene->insert(lineGroup);
//
//  mrpt3dscene->insert(pointClound);
//  CFileGZOutputStream("map.3Dscene") << *mrpt3dscene;
//
//  mrpt3dsceneMain = mrpt3dscene;
//
//  win.unlockAccess3DScene();
//
//  // Update window, if required
//  win.forceRepaint();
//  //win.waitForKey(false,NULL);
//
//  win.waitForKey(false, NULL);

  //Glow::isGLInit = true;
  // Initialize GLOW runtime
  Glow::Init(argc, argv);

  // Create one window to display one data object
  MonoSLAMGlow monoslamglow(argc, argv, ms);

  // Initialise the random seed to something random
  //  srand48(time(0)); // For true random behaviour
  srand48(0); // Always the same seed (pick a number), so deterministic

  // Enter event loop
  Glow::MainLoop();

  return 0;
}
