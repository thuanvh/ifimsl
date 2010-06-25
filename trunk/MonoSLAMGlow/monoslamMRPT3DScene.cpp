

#include "monoslamMRPT3DScene.h"
#include <opencv/cxcore.h>
#include <VNL/matlabprint.h>
#include <VW/Utils/timer.h>
#include <VWGL/Display/drawfirei.h>

#include <MonoSLAM/identifier_imagemono.h>
#include <MonoSLAM/robot.h>

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
  this->lineGroup->setColor(1, 0, 1);
  lastRobotPosition.x(0);
  lastRobotPosition.y(0);
  lastRobotPosition.z(0);
  isSceneStart = true;

  //win=new CDisplayWindow3D("My window");
}

void drawCameraStateEllipse(VNL::Vector<double> location, VNL::Matrix<double> Pxx, COpenGLScenePtr & mrpt3dscene){
  CEllipsoidPtr objEllip = CEllipsoid::Create();

  objEllip->setLocation(location.Get(0), location.Get(1), location.Get(2));

  CMatrixDouble33 COV3;
  int size=3;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      COV3(i, j) = Pxx.Get(i, j);
    }
  }
  objEllip->setCovMatrix(COV3, COV3(2, 2) == 0 ? 2 : 3);
  objEllip->setColor(1, 1, 0);
  objEllip->enableDrawSolid3D(true);
  mrpt3dscene->insert(objEllip);
}

void drawEllipse(Feature* feature, COpenGLScenePtr & mrpt3dscene) {
  VNL::Vector<double> estimateY = feature->get_y();//get_feature_measurement_model()->get_yigraphicsRES();
  //VNL::Matrix<double> matrixPyy = feature->get_feature_measurement_model()->get_PyiyigraphicsRES();
  VNL::Matrix<double> matrixPyy = feature->get_Pyy();
  CEllipsoidPtr objEllip = CEllipsoid::Create();

  objEllip->setLocation(estimateY.Get(0), estimateY.Get(1), estimateY.Get(2));
//  CMatrixDouble matrix;
//  CMatrixDouble66 COV;
  CMatrixDouble33 COV3;
  int size=3;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      COV3(i, j) = matrixPyy.Get(i, j);
      //cout << (COV3) (j, i) << " ";
    }
    //cout << endl;
  }
  objEllip->setCovMatrix(COV3, COV3(2, 2) == 0 ? 2 : 3);
  //int size = 3;
  //cout << "Size: " << b->Rows() << "-" << b->Cols() << endl;

  //    matrix.resize(size, size);
  //    for (int i = 0; i < size; i++) {
  //      for (int j = 0; j < size; j++) {
  //        (matrix) (j, i) = feature->get_Pxy().Get(j, i);
  //        cout << (matrix) (j, i) << " ";
  //      }
  //      cout << endl;
  //    }
  //    cout << endl;
  //convertMatrix(&matrix, &feature->get_Pxy());
  //    objEllip->setCovMatrix(matrix, 3);
  //objEllip->setCovMatrix(COV3, COV3(2, 2) == 0 ? 2 : 3);

  objEllip->setColor(0, 1, 1);
  objEllip->enableDrawSolid3D(true);
  mrpt3dscene->insert(objEllip);
}
CPose3D monoslamMRPT3DScene::getRobotState() {
  // Put the camera in the right place in the image display
  scene->get_motion_model()->func_xp(scene->get_xv());
  ThreeD_Motion_Model *threed_motion_model =
          (ThreeD_Motion_Model *) scene->get_motion_model();
  VW::Vector3D r = threed_motion_model->get_rRES();
  VW::Quaternion qWR = threed_motion_model->get_qRES();
    // q is qWR between world frame and Scene robot frame
  // What we need to plot though uses GL object frame O
  // Know qRO: pi rotation about y axis
  // qWO = qWR * qRO
  VW::Quaternion qRO(0.0, 1.0, 0.0, 0.0);
  VW::Quaternion qWO = qWR * qRO;
  CPose3D position;
  double yaw,pitch,roll;
  qWO.GetZYXEuler(yaw,pitch,roll);
  position.setFromValues(r._x,r._y,r._z,yaw,pitch,roll);
  return position;
//  if (display_trajectory_button->GetState()) {
//    trajectory_store.push_back(r.GetVNL3());
//    if (trajectory_store.size() > 800) {
//      trajectory_store.erase(trajectory_store.begin());
//    }
//  }
//
//
//
//  image_threedtool->SetCameraPositionOrientation(r, qWO);
}
COpenGLScenePtr monoslamMRPT3DScene::writeMRPT3DScene() {
  step++;
  cout << step << endl;
  // Save a 3D mrpt3dscene view of the mapping process:
  COpenGLScenePtr mrpt3dscene = COpenGLScene::Create(); // = mrptwindows->get3DSceneAndLock();
  COpenGLScenePtr mrpt3dsceneWithEllipsoid = COpenGLScene::Create(); //
  COpenGLScenePtr mrpt3dsceneLocalisation = COpenGLScene::Create(); //
  CPose3DPDFParticles curPDF; // current particle filter?????????
  //  if (SAVE_3D_SCENE) {
  //mrpt3dscene = COpenGLScene::Create();

  // The ground:
  mrpt::opengl::CGridPlaneXYPtr groundPlane = mrpt::opengl::CGridPlaneXY::Create(-400, 400, -400, 400, 0, 5);
  //mrpt::opengl::CGridPlaneXZPtr xzPlane = mrpt::opengl::CGridPlaneXZ::Create(-400, 400, -400, 400, 0, 5);
  mrpt::opengl::CAxisPtr axisptr=CAxis::Create(-400,-400,-400,400,400,400,5,1,true);

  axisptr->setColor(1,0.5,1);
  groundPlane->setColor(0.4, 0.4, 0.4);
  //xzPlane->setColor(0.4, 0.4, 0.4);
  mrpt3dscene->insert(axisptr);
  mrpt3dscene->insert(groundPlane);
  //mrpt3dscene->insert(xzPlane);

  mrpt3dsceneWithEllipsoid->insert(axisptr);
  mrpt3dsceneWithEllipsoid->insert(groundPlane);

  mrpt3dsceneLocalisation->insert(axisptr);
  mrpt3dsceneLocalisation->insert(groundPlane);
  


  // The camera pointing to the current robot pose:
  //  if (CAMERA_3DSCENE_FOLLOWS_ROBOT) {
  mrpt::opengl::CCameraPtr objCam = mrpt::opengl::CCamera::Create();
  CPose3D robotPose;
  // robot state = state of camera
  VNL::Vector<double> state = this->scene->get_xv();
  VNL::Matrix<double> covState=this->scene->get_Pxx();

  //draw trajectoire of robot
  assign3D(&robotPose, state);
  if (isSceneStart) {
    //  cout << "abc0" << endl;
    //assign3D(&lastRobotPosition, state);
    isSceneStart = false;
  } else {
    this->lineGroup->appendLine(state.Get(0), state.Get(1), state.Get(2),lastRobotPosition.x(), lastRobotPosition.y(), lastRobotPosition.z());
  }

  drawCameraStateEllipse(state,covState,mrpt3dsceneLocalisation);
  //  cout << "abc1" << endl;
  assign3D(&lastRobotPosition, state);
  //  cout << "abc2" << endl;
  mrpt3dscene->insert(lineGroup);
  mrpt3dsceneWithEllipsoid->insert(lineGroup);
  mrpt3dsceneLocalisation->insert(lineGroup);
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

  //this->scene->get
  CPointCloudPtr pointClound = mrpt::opengl::CPointCloud::Create();
  cout << "okie" << endl;
  for (int i = 0; i < listFeatures.size(); i++) {
    Feature* feature = listFeatures.at(i);
    VNL::Vector<double> estimateY = feature->get_y();

    string label(format("%d", feature->get_label()));
    CTextPtr text = CText::Create();
    text->setString(label);
    text->setLocation(estimateY.Get(0), estimateY.Get(1), estimateY.Get(2));
    mrpt3dscene->insert(text);
    mrpt3dsceneWithEllipsoid->insert(text);
    mrpt3dsceneLocalisation->insert(text);

    if (false) {
      // We know that our descriptors are image patches
      ExtraData* feature_data = feature->get_identifier();
      ImageMonoExtraData* patch_ptr = dynamic_cast<ImageMonoExtraData*> (feature_data);

      // Where to draw the patches?
      double draw_patch_x = -1, draw_patch_y = -1;
      if (feature->get_selected_flag() &&
              feature->get_successful_measurement_flag()) {
        // If we just successfully matched this feature,
        // then draw match position
        draw_patch_x = feature->get_z()(0);
        draw_patch_y = feature->get_z()(1);
      } else {
        Fully_Initialised_Feature_Measurement_Model *fully_init_fmm = dynamic_cast<Fully_Initialised_Feature_Measurement_Model*> (
                feature->get_feature_measurement_model());
        // Otherwise current estimated position after update
        draw_patch_x = fully_init_fmm->get_hiRES()(0);
        draw_patch_y = fully_init_fmm->get_hiRES()(1);
      }
      //            Draw2DPatch(threedtool, draw_patch_x, draw_patch_y,
      //            BOXSIZE, *patch_ptr,
      //            (*it)->get_label() + 1);
      //PixelMono* pxptr=patch_ptr->GetImageBuffer();
      IplImage* imgpatch = cvCreateImage(cvSize(BOXSIZE, BOXSIZE), 8, 1);
      uchar* ptr = (uchar*) imgpatch->imageData;
      for (int i = 0; i < BOXSIZE; i++) {
        for (int j = 0; j < BOXSIZE; j++) {
          *ptr = patch_ptr->GetPixel(i, j).y;
          ptr++;
          //*ptr= (pxptr+(i*BOXSIZE+j))
        }
      }
      CSetOfTexturedTrianglesPtr cstptr = CSetOfTexturedTriangles::Create();
      //CTexturedPlanePtr txtptr=CTexturedPlane::Create();

      //CTexturedObjectPtr ptr;
      //CTexturedObject txtobj;
      CImagePtr cimag = CImage::Create();
      cimag->setFromIplImage(imgpatch);
      cstptr->assignImage(*cimag);
      cstptr->setLocation(estimateY.Get(0), estimateY.Get(1), estimateY.Get(2));
      mrpt3dscene->insert(cstptr);
    }


    //    CPose3D poseY;
    //    assign3D(&poseY, estimateY);
    //    mrpt3dscene->insert(poseY);
    pointClound->insertPoint(estimateY.Get(0), estimateY.Get(1), estimateY.Get(2));

    drawEllipse(feature,mrpt3dsceneWithEllipsoid);
  }
  pointClound->setColor(1,0,1);
  mrpt3dscene->insert(pointClound);
  mrpt3dsceneWithEllipsoid->insert(pointClound);
  mrpt3dsceneLocalisation->insert(pointClound);
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
  CFileGZOutputStream(format("%s.ellipsoid/buildingmap_ellipsoid_%05u.3Dscene", sceneDirPath.c_str(), step)) << *mrpt3dsceneWithEllipsoid;
  CFileGZOutputStream(format("%s.localisation/buildingmap_localisation_%05u.3Dscene", sceneDirPath.c_str(), step)) << *mrpt3dsceneLocalisation;
  //mrptwindows->unlockAccess3DScene();

  // Update window, if required
  //mrptwindows->forceRepaint();
  return mrpt3dscene;
  //  }
}
