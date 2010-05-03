

#include "monoslamMRPT3DScene.h"
using namespace mrpt;
using namespace mrpt::slam;
using namespace mrpt::opengl;
using namespace mrpt::gui;
using namespace mrpt::math;
using namespace mrpt::utils;
using namespace mrpt::system;
using namespace mrpt::random;
using namespace std;

#ifdef SAVE_3D_SCENE
#undef SAVE_3D_SCENE
#endif
#define SAVE_3D_SCENE true
#define CAMERA_3DSCENE_FOLLOWS_ROBOT true

void assign3D(CPose3D* pose3d, VNL::Vector<double> state) {
  pose3d->x(state.Get(0));
  pose3d->y(state.Get(1));
  pose3d->z(state.Get(2));
}

void convertMatrix(CMatrixDouble* a, const VNL::Matrix<double>* b) {
  int size = 3;
  cout << "Size: " << b->Rows() << "-" << b->Cols() << endl;

  a->resize(size, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      (*a)(j, i) = b->Get(j, i);
      cout << (*a)(j, i) << " ";
    }
  }
  //int size=b->Rows()>b->Cols()?b->Rows():b->Cols();
  //a->resize(size,size);
}

monoslamMRPT3DScene::monoslamMRPT3DScene() {
  cout << "init mrpt" << endl;
  lineGroup = CSetOfLines::Create();
  this->lineGroup->setColor(1,0,1);
  lastRobotPosition.x(0);
  lastRobotPosition.y(0);
  lastRobotPosition.z(0);
  isSceneStart=true;
}

void monoslamMRPT3DScene::writeMRPT3DScene() {
  step++;
  // Save a 3D mrpt3dscene view of the mapping process:
  COpenGLScenePtr mrpt3dscene;
  CPose3DPDFParticles curPDF; // current particle filter
  //  if (SAVE_3D_SCENE) {
  mrpt3dscene = COpenGLScene::Create();

  // The ground:
  mrpt::opengl::CGridPlaneXYPtr groundPlane = mrpt::opengl::CGridPlaneXY::Create(-200, 200, -200, 200, 0, 5);
  groundPlane->setColor(0.4, 0.4, 0.4);
  mrpt3dscene->insert(groundPlane);

  // The camera pointing to the current robot pose:
  //  if (CAMERA_3DSCENE_FOLLOWS_ROBOT) {
  mrpt::opengl::CCameraPtr objCam = mrpt::opengl::CCamera::Create();
  CPose3D robotPose;
  // robot state = state of camera
  VNL::Vector<double> state = this->scene->get_xv();

  assign3D(&robotPose, state);
  if (isSceneStart) {
    //  cout << "abc0" << endl;
    //assign3D(&lastRobotPosition, state);
    isSceneStart=false;
  } else {
    this->lineGroup->appendLine(state.Get(0), state.Get(1), state.Get(2),
            lastRobotPosition.x(), lastRobotPosition.y(), lastRobotPosition.z());
  }
  //  cout << "abc1" << endl;
  assign3D(&lastRobotPosition, state);
  //  cout << "abc2" << endl;
  mrpt3dscene->insert(lineGroup);
  //  cout << "abc3" << endl;

  //    robotPose.x(state.Get(0));
  //    robotPose.y(state.Get(1));
  //    robotPose.z(state.Get(2));

  //curPDF.getMean(robotPose);

  //  objCam->setPointingAt(robotPose);
  //  objCam->setAzimuthDegrees(-30);
  //  objCam->setElevationDegrees(30);

  //  mrpt3dscene->insert(objCam);

  //  }

  vector<Feature*> listFeatures = this->scene->get_feature_list();
  CPointCloudPtr pointClound = mrpt::opengl::CPointCloud::Create();
  for (int i = 0; i < listFeatures.size(); i++) {
    Feature* feature = listFeatures.at(i);
    VNL::Vector<double> estimateY = feature->get_y();
    //    CPose3D poseY;
    //    assign3D(&poseY, estimateY);
    //    mrpt3dscene->insert(poseY);
    pointClound->insertPoint(estimateY.Get(0), estimateY.Get(1), estimateY.Get(2));
    CEllipsoidPtr objEllip = CEllipsoid::Create();

    objEllip->setLocation(estimateY.Get(0), estimateY.Get(1), estimateY.Get(2));
    CMatrixDouble matrix;
    CMatrixDouble66 COV;
    CMatrixDouble33 COV3 = CMatrixDouble33(COV);
    int size = 3;
    //cout << "Size: " << b->Rows() << "-" << b->Cols() << endl;

    //    matrix.resize(size, size);
    //    for (int i = 0; i < size; i++) {
    //      for (int j = 0; j < size; j++) {
    //        (matrix)(j, i) = feature->get_Pxy().Get(j,i);
    //        cout << (matrix)(j, i) << " ";
    //      }
    //    }
    //convertMatrix(&matrix, &feature->get_Pxy());
    //    objEllip->setCovMatrix(matrix, 3);
    //objEllip->setCovMatrix(COV3, COV3(2, 2) == 0 ? 2 : 3);

    objEllip->setColor(0, 1, 1);
    objEllip->enableDrawSolid3D(false);
    mrpt3dscene->insert(objEllip);
  }
  mrpt3dscene->insert(pointClound);
  //  // Draw the map(s):
  //  mrpt::opengl::CSetOfObjectsPtr objs = mrpt::opengl::CSetOfObjects::Create();
  //  mostLikMap->getAs3DObject(objs);
  //  mrpt3dscene->insert(objs);

  //  // Draw the robot particles:
  //  size_t M = mapBuilder.mapPDF.particlesCount();
  //  mrpt::opengl::CSetOfLinesPtr objLines = mrpt::opengl::CSetOfLines::Create();
  //  objLines->setColor(0, 1, 1);
  //  for (size_t i = 0; i < M; i++) {
  //    std::deque<TPose3D> path;
  //    mapBuilder.mapPDF.getPath(i, path);
  //
  //    float x0 = 0, y0 = 0, z0 = 0;
  //    for (size_t k = 0; k < path.size(); k++) {
  //      objLines->appendLine(
  //              x0, y0, z0 + 0.001,
  //              path[k].x, path[k].y, path[k].z + 0.001);
  //      x0 = path[k].x;
  //      y0 = path[k].y;
  //      z0 = path[k].z;
  //    }
  //  }
  //  mrpt3dscene->insert(objLines);

  // An ellipsoid:
  //  CPose3D lastMeanPose;
  //  float minDistBtwPoses = -1;
  //  std::deque<TPose3D> dummyPath;
  //  mapBuilder.mapPDF.getPath(0, dummyPath);
  //  for (int k = (int) dummyPath.size() - 1; k >= 0; k--) {
  //    CPose3DPDFParticles poseParts;
  //    mapBuilder.mapPDF.getEstimatedPosePDFAtTime(k, poseParts);
  //
  //    CPose3D meanPose;
  //    CMatrixDouble66 COV;
  //    poseParts.getCovarianceAndMean(COV, meanPose);
  //
  //    if (meanPose.distanceTo(lastMeanPose) > minDistBtwPoses) {
  //      CMatrixDouble33 COV3 = CMatrixDouble33(COV);
  //
  //      minDistBtwPoses = 6 * sqrt(COV3(0, 0) + COV3(1, 1));
  //
  //      opengl::CEllipsoidPtr objEllip = opengl::CEllipsoid::Create();
  //      objEllip->setLocation(meanPose.x(), meanPose.y(), meanPose.z() + 0.001);
  //      objEllip->setCovMatrix(COV3, COV3(2, 2) == 0 ? 2 : 3);
  //
  //      objEllip->setColor(0, 0, 1);
  //      objEllip->enableDrawSolid3D(false);
  //      mrpt3dscene->insert(objEllip);
  //
  //      lastMeanPose = meanPose;
  //    }
  //  }
  //  } // end if show or save 3D mrpt3dscene->

  //  if (SAVE_3D_SCENE) { // Save as file:
  CFileGZOutputStream(format("%s/buildingmap_%05u.3Dscene", sceneDirPath.c_str(), step)) << *mrpt3dscene;
  //  }
}
