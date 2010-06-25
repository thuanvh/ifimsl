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
#include "main.h"
#include "monoslamSettings.h"
#include <unistd.h>
#include "MonoSLAM/robot.h"
#include "MonoSLAM/threeddraw.h"
#include "MonoSLAM/model_creators.h"
#include "monoslamglow.h"
#include "monoslamMRPT3DScene.h"
#include "monoslamlog.h"
#include <sys/stat.h>
#include <sys/types.h>

#ifdef DEBUGDUMP
#undef DEBUGDUMP
#endif
#define DEBUGDUMP false

MonoSLAMGlow::MonoSLAMGlow(int argc, char *argv[], MonoslamSettings mns)
: delta_t(mns.delta_t),
NUMBER_OF_FEATURES_TO_SELECT(mns.NUMBER_OF_FEATURES_TO_SELECT),
NUMBER_OF_FEATURES_TO_KEEP_VISIBLE(mns.NUMBER_OF_FEATURES_TO_KEEP_VISIBLE),
MAX_FEATURES_TO_INIT_AT_ONCE(mns.MAX_FEATURES_TO_INIT_AT_ONCE),
MIN_LAMBDA(mns.MIN_LAMBDA),
MAX_LAMBDA(mns.MAX_LAMBDA),
NUMBER_OF_PARTICLES(mns.NUMBER_OF_PARTICLES),
STANDARD_DEVIATION_DEPTH_RATIO(mns.STANDARD_DEVIATION_DEPTH_RATIO),
MIN_NUMBER_OF_PARTICLES(mns.MIN_NUMBER_OF_PARTICLES),
PRUNE_PROBABILITY_THRESHOLD(mns.PRUNE_PROBABILITY_THRESHOLD),
ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS(mns.ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS) {
  //monoslamStateFile=new char[mns.monoslamStateFile.length()];
  //strcpy(mns.monoslamStateFile.c_str(),monoslamStateFile);
  monoslamStateFile=mns.monoslamStateFile;

  // Set up the main MonoSLAM class object
  SetUpMonoSLAM();

  // Reset counters
  output_tracked_image_number = 0;
  output_raw_image_number = 0;


  // Initialise GlowWindow
  // Set big size because this will be reshaped later
  Init("MonoSLAMGlow",
          GlowWindow::autoPosition,
          GlowWindow::autoPosition,
          1000, 1000, Glow::rgbBuffer, Glow::noEvents);

  SetUpButtons();

  SetUp3DDisplays();

  // Folder for get images
  SetUpImageGrabber(argv[1]);

  // Out to 3DScene
  this->mrpt3dScene = new monoslamMRPT3DScene();

  //System::IO::Directory::CreateDirectory(argv[2]);
  if (mkdir(argv[2], 0777) == -1) { // Create the directory
    std::cerr << "Error: " << argv[2];
  }

  string sceneoutput(argv[2]);
  this->mrpt3dScene->setSceneDirPath(sceneoutput);
  this->mrpt3dScene->setScene(monoslaminterface->GetScene());
  //this->mrpt3dScene->setWindow(mrptwindows);

  this->monoslamLog=new monoslamlog();
  monoslamLog->logDirPath=string(sceneoutput)+".log";
  monoslamLog->setScene(monoslaminterface->GetScene());
  monoslamLog->initLog();

  Reshape(controlpanel1->Width() + controlpanel2->Width() +
          threedtool->Width(), threedtool->Height() +
          image_threedtool->Height());
}

void MonoSLAMGlow::SetUpMonoSLAM() {
  // Use standard model creators from MonoSLAM/model_creators.h here
  MonoSLAM_Motion_Model_Creator mm_creator;
  MonoSLAM_Feature_Measurement_Model_Creator fmm_creator;

  //"monoslam_state.ini",
  monoslaminterface =
          new MonoSLAMInterface(monoslamStateFile.c_str(),
          &mm_creator,
          &fmm_creator,
          NULL, // no internal measurement models used here
          NUMBER_OF_FEATURES_TO_SELECT,
          NUMBER_OF_FEATURES_TO_KEEP_VISIBLE,
          MAX_FEATURES_TO_INIT_AT_ONCE,
          MIN_LAMBDA,
          MAX_LAMBDA,
          NUMBER_OF_PARTICLES,
          STANDARD_DEVIATION_DEPTH_RATIO,
          MIN_NUMBER_OF_PARTICLES,
          PRUNE_PROBABILITY_THRESHOLD,
          ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS);
}

void MonoSLAMGlow::SetUpButtons() {
  // Left column of buttons
  controlpanel1 = new GlowQuickPaletteSubwindow(this, 0, 0, 200, 400,
          GlowQuickPalette::vertical,
          GlowQuickPalette::alignCenter,
          10, 10, 10);

  // Buttons which toggle aspects of the graphical displays call
  // the display request redraw functions to update graphics immediately
  controlpanel1->AddLabel("Graphics Toggles");

  rectify_image_display_button =
          controlpanel1->AddCheckBox("Rectify Image Display",
          GlowCheckBoxWidget::off, this);

  display_trajectory_button =
          controlpanel1->AddCheckBox("Display Trajectory",
          GlowCheckBoxWidget::off, this);

  display_3d_features_button =
          controlpanel1->AddCheckBox("Display 3D Features",
          GlowCheckBoxWidget::on, this);

  display_3d_feature_uncertainties_button =
          controlpanel1->AddCheckBox("Display 3D Uncertainties",
          GlowCheckBoxWidget::on, this);

  display_2d_descriptors_button =
          controlpanel1->AddCheckBox("Display 2D Descriptors",
          GlowCheckBoxWidget::on, this);

  display_2d_searches_button =
          controlpanel1->AddCheckBox("Display 2D Search Regions",
          GlowCheckBoxWidget::on, this);

  display_initialisation_button =
          controlpanel1->AddCheckBox("Display Initialisation",
          GlowCheckBoxWidget::on, this);

  controlpanel1->Pack(true);


  // Right column of buttons
  controlpanel2 = new GlowQuickPaletteSubwindow(this,
          controlpanel1->Width(), 0,
          200, 400,
          GlowQuickPalette::vertical,
          GlowQuickPalette::alignCenter,
          10, 10, 10);

  // These control toggles don't do anything when clicked; their
  // status will be queried during the tracking loop
  controlpanel2->AddLabel("Controls Toggles");

  toggle_tracking_button =
          controlpanel2->AddCheckBox("Toggle Tracking",
          GlowCheckBoxWidget::off, this);

  enable_mapping_button =
          controlpanel2->AddCheckBox("Enable Mapping",
          GlowCheckBoxWidget::on, this);

  output_tracked_images_button =
          controlpanel2->AddCheckBox("Output Tracked Images",
          GlowCheckBoxWidget::off, this);

  output_raw_images_button =
          controlpanel2->AddCheckBox("Output Raw Images",
          GlowCheckBoxWidget::off, this);


  // Standard buttons to call action functions
  controlpanel2->AddLabel("Action Controls");

  init_feature_button =
          controlpanel2->AddPushButton("Initialise Manual Feature", this);

  auto_init_feature_button =
          controlpanel2->AddPushButton("Initialise Auto Feature", this);

  print_robot_state_button =
          controlpanel2->AddPushButton("Print Robot State", this);

  delete_feature_button =
          controlpanel2->AddPushButton("Delete Feature", this);

  save_patch_button =
          controlpanel2->AddPushButton("Save Patch", this);

  quit_button =
          controlpanel2->AddPushButton("Quit", this);

  controlpanel2->Pack(true);
}

void MonoSLAMGlow::SetUp3DDisplays() {
  // Set display virtual camera parameters to match those of the real camera
  // being used in MonoSLAM
  const Partially_Initialised_Feature_Measurement_Model *default_pifmm =
          monoslaminterface->GetDefaultFeatureTypeForInitialisation();
  const Line_Init_Wide_Point_Feature_Measurement_Model *default_wide_pifmm =
          (const Line_Init_Wide_Point_Feature_Measurement_Model *) default_pifmm;
  Graphics_Fku = default_wide_pifmm->get_camera().Fku();
  Graphics_Fkv = default_wide_pifmm->get_camera().Fkv();
  Graphics_U0 = default_wide_pifmm->get_camera().U0();
  Graphics_V0 = default_wide_pifmm->get_camera().V0();
  Graphics_Kd1 = default_wide_pifmm->get_camera().Kd1();

  // First display for external 3D view
  threedtool = new ThreeDToolGlowWidget(this,
          controlpanel1->Width() + controlpanel2->Width(), 0,
          monoslaminterface->GetCameraWidth(),
          monoslaminterface->GetCameraHeight());
  threedtool->DrawEvent.Attach(this, &MonoSLAMGlow::Draw3D);
  threedtool->ProcessHitsEvent.Attach(this, &MonoSLAMGlow::ProcessHits);
  threedtool->SetCameraParameters(Graphics_Fku, Graphics_Fkv,
          Graphics_U0, Graphics_V0);


  // Set start position for GL camera in 3D tool
  // This is x, y, z position
  VNL::VectorFixed < 3, double> rthreed(0.0, 0.2, -1.5);
  // This is camera orientation in my normal coordinate system
  // (z forward, y up, x left)
  VW::Quaternion qWRthreed(0.0, 0.0, 0.0, 1.0);
  // We need to adjust by this rotation to fit GL coordinate frame
  // (z backward, y up, x right)
  // So rotate by pi about y axis
  VW::Quaternion qROthreed(0.0, 1.0, 0.0, 0.0);
  VW::Quaternion qWOthreed = qWRthreed * qROthreed;
  threedtool->SetCameraPositionOrientation(rthreed, qWOthreed);

  // Second 3D display for images and augmented reality
  image_threedtool = new ThreeDToolGlowWidget(this,
          controlpanel1->Width() + controlpanel2->Width(),
          threedtool->Height(),
          monoslaminterface->GetCameraWidth(),
          monoslaminterface->GetCameraHeight());
  image_threedtool->DrawEvent.Attach(this, &MonoSLAMGlow::ImageDraw3D);
  image_threedtool->ProcessHitsEvent.Attach(this,
          &MonoSLAMGlow::ImageProcessHits);
  image_threedtool->SetCameraParameters(Graphics_Fku, Graphics_Fkv,
          Graphics_U0, Graphics_V0);

  // Set up initial state of virtual camera for image display to match
  // state vector
  const Scene_Single *scene = monoslaminterface->GetScene();
  scene->get_motion_model()->func_xp(scene->get_xv());
  ThreeD_Motion_Model *threed_motion_model =
          (ThreeD_Motion_Model *) scene->get_motion_model();
  VW::Vector3D r = threed_motion_model->get_rRES();
  VW::Quaternion qWR = threed_motion_model->get_qRES();
  // q is qWR between world frame and Scene robot frame
  // We need to adjust by this rotation to fit GL coordinate frame
  // (z backward, y up, x right)
  // So rotate by pi about y axis
  VW::Quaternion qRO(0.0, 1.0, 0.0, 0.0);
  VW::Quaternion qWO = qWR * qRO;
  image_threedtool->SetCameraPositionOrientation(r, qWO);
}

void MonoSLAMGlow::SetUpImageGrabber(char* imageFolder) {
  grabbed_image = new VW::ImageMono<unsigned char>
          (monoslaminterface->GetCameraWidth(),
          monoslaminterface->GetCameraHeight());

#ifdef _REALTIME_
  realtimeflag = true;

  // Firewire camera
  grabber = new VW::SequencerFirewireGlow();
  grabber->SequencerFirewire::Init();
  if (grabber->GetNumberOfChannels() == 0) {
    cerr << "No cameras!" << endl;
    exit(0);
  } else {
    assert(grabber->GetNumberOfChannels() == 1);
    // We might want to change some camera settings (zoom, focus etc...)
    VW::FirewireCamera* d = grabber->GetCamera(0);
    d->SetImageMode(VW::FirewireParams::M_320x240_YUV422);
    d->SetFramerate(VW::FirewireParams::FPS_30);
    cout << "Camera 0:\n" << (*d) << endl;
  }
#else
  realtimeflag = false;

  // And create sequencer window
  // grabber = new SequencerFileGlow("rawoutput0000.pgm");
  string a("/rawoutput0001.pgm");
  string b(imageFolder);
  grabber = new SequencerFileGlow(b + a);
  grabber->SequencerBase::Init();
  // Set delay between frames on "Continuous" button (milliseconds)
  // Set this high if tracked images are to be saved to disk
  grabber->SetDelay(200);
#endif

  grabber->NewFrameEvent.Attach(this, &MonoSLAMGlow::HandleNewFrameGlow);
}

void MonoSLAMGlow::OnReshape(int width, int height) {
  cerr << "Reshape w = " << width << " height = " << height << endl;
  cerr << "Minimum w = " << controlpanel1->Width() + controlpanel2->Width() +
          int(monoslaminterface->GetCameraWidth()) << " height = " << 2 * int(monoslaminterface->GetCameraHeight())
          << endl;

  // When we reshape the window we'll keep the button panels the same
  // width as usual; and we won't let the 3D displays get smaller than
  // their original size
  if (width < controlpanel1->Width() + controlpanel2->Width() +
          int(monoslaminterface->GetCameraWidth()) ||
          height < 2 * int(monoslaminterface->GetCameraHeight())) {
    int neww = max(width, controlpanel1->Width() + controlpanel2->Width() +
            int(monoslaminterface->GetCameraWidth()));
    int newh = max(height, 2 * int(monoslaminterface->GetCameraHeight()));
    cerr << "Trying to reshape w = " << neww << " h = " << newh << endl;
    // This resizes the window (and then this function will be called again)
    Reshape(neww, newh);
  } else {
    controlpanel1->Reshape(controlpanel1->Width(), height);
    controlpanel1->Refresh();
    controlpanel2->Reshape(controlpanel2->Width(), height);
    controlpanel2->Refresh();

    // MaxWidth of the 3D displays is now defined
    int new3Dmaxwidth = width - controlpanel1->Width()
            - controlpanel2->Width();
    int new3Dmaxheight = height / 2;

    // But we want to keep the 3D displays of a constant aspect ratio
    double width_height_ratio = double(monoslaminterface->GetCameraWidth())
            / double(monoslaminterface->GetCameraHeight());

    int new3Dwidth, new3Dheight;
    if (new3Dmaxwidth / width_height_ratio > new3Dmaxheight) {
      // Wide window; size limited by height
      new3Dwidth = int(width_height_ratio * double(new3Dmaxheight) + 0.5);
      new3Dheight = new3Dmaxheight;
    } else {
      // Tall window; size limited by width
      new3Dwidth = new3Dmaxwidth;
      new3Dheight = int(double(new3Dmaxwidth) / width_height_ratio + 0.5);
    }
    threedtool->Move(controlpanel1->Width() + controlpanel2->Width(), 0);
    threedtool->Reshape(new3Dwidth, new3Dheight);

    image_threedtool->Move(controlpanel1->Width() + controlpanel2->Width(),
            new3Dheight);
    image_threedtool->Reshape(new3Dwidth, new3Dheight);

    threedtool->Refresh();
    image_threedtool->Refresh();
  }

  // Update the viewport
  ::glViewport(0, 0, width, height);
}

void MonoSLAMGlow::OnMessage(const GlowPushButtonMessage& message) {
  if (message.widget == init_feature_button) {
    InitialiseFeatureGlow();
  } else if (message.widget == auto_init_feature_button) {
    AutoInitialiseFeatureGlow();
  } else if (message.widget == print_robot_state_button) {
    PrintRobotStateGlow();
  } else if (message.widget == delete_feature_button) {
    DeleteFeatureGlow();
  } else if (message.widget == save_patch_button) {
    SavePatchGlow();
  } else if (message.widget == quit_button) {
    Quit();
  } else
    assert(0);
}

void MonoSLAMGlow::OnMessage(const GlowCheckBoxMessage& message) {
  if (message.widget == rectify_image_display_button ||
          message.widget == display_trajectory_button ||
          message.widget == display_3d_features_button ||
          message.widget == display_3d_feature_uncertainties_button ||
          message.widget == display_2d_descriptors_button ||
          message.widget == display_2d_searches_button ||
          message.widget == display_initialisation_button) {
    // These buttons have their state queries by the graphics routine
    // so just request redraw
    GraphicalButtonToggled(true);
  } else if (message.widget == toggle_tracking_button ||
          message.widget == enable_mapping_button ||
          message.widget == output_tracked_images_button ||
          message.widget == output_raw_images_button) {
    // These buttons don't need any action because their states are
    // queried by tracking function
  } else
    assert(0);
}

// Drawing function which will be registered with 3D tool

void MonoSLAMGlow::Draw3D(bool select_flag) {
  mutex.Lock();
  //  cout << "Timer on entering Draw3D: " << monoslaminterface->GetTimer1()
  //    << endl;

  if (DEBUGDUMP) cout << "Drawing function for 3D display." << endl;

  External3DDraw(monoslaminterface->GetScene(),
          *threedtool,
          trajectory_store,
          1,
          0,
          display_trajectory_button->GetState(),
          display_3d_features_button->GetState(),
          display_3d_feature_uncertainties_button->GetState(),
          display_3d_features_button->GetState(),
          0);

  // cout << "Timer on leaving Draw3D: " << monoslaminterface->GetTimer1()
  //   << endl;
  mutex.Unlock();
}

void MonoSLAMGlow::ProcessHits(int selected_item,
        double camera_x, double camera_y) {
  // Here selected_item will be the label of a feature plus one if the
  // user has clicked on a mapped feature; 0 if not
  if (selected_item > 0) {
    if (Scene_Single::STATUSDUMP)
      cout << "Toggled feature with label: " << selected_item - 1
            << "; hit Delete to delete." << endl;

    monoslaminterface->GetSceneNoConst()->
            toggle_feature_lab(selected_item - 1);
    monoslaminterface->GetSceneNoConst()->
            mark_feature_by_lab(selected_item - 1);
  }

  threedtool->RequestDraw();
  image_threedtool->RequestDraw();
}

void MonoSLAMGlow::ImageDraw3D(bool select_flag) {
  mutex.Lock();
  // cout << "Timer on entering ImageDraw3D: "
  // << monoslaminterface->GetTimer1() << endl;

  if (DEBUGDUMP) cout << "Drawing function for Image display." << endl;

  if (rectify_image_display_button->GetState()) {
    RectifiedInternal3DDraw(monoslaminterface->GetScene(),
            *image_threedtool,
            trajectory_store,
            grabbed_image,
            Graphics_U0,
            Graphics_V0,
            Graphics_Kd1,
            0,
            display_trajectory_button->GetState(),
            display_3d_features_button->GetState(),
            display_3d_feature_uncertainties_button->GetState(),
            display_3d_features_button->GetState(),
            0,
            1,
            1);
  } else {
    RawInternal3DDraw(monoslaminterface->GetScene(),
            *image_threedtool,
            grabbed_image,
            monoslaminterface->GetSceneNoConst()->get_feature_init_info_vector(),
            display_2d_descriptors_button->GetState(),
            display_2d_searches_button->GetState(),
            display_initialisation_button->GetState(),
            1,
            monoslaminterface->GetInitFeatureSearchRegionDefinedFlag(),
            BOXSIZE,
            monoslaminterface->GetLocationSelectedFlag(),
            monoslaminterface->GetUU(),
            monoslaminterface->GetVV(),
            monoslaminterface->GetInitFeatureSearchUStart(),
            monoslaminterface->GetInitFeatureSearchVStart(),
            monoslaminterface->GetInitFeatureSearchUFinish(),
            monoslaminterface->GetInitFeatureSearchVFinish());
  }

  // cout << "Timer on leaving ImageDraw3D: " << monoslaminterface->GetTimer1()
  //      << endl;
  mutex.Unlock();
}

void MonoSLAMGlow::ImageProcessHits(int selected_item,
        double camera_x, double camera_y) {
  // Here selected_item will be the label of a feature plus one if the
  // user has clicked on a mapped feature; 0 if not
  if (selected_item > 0) {
    if (Scene_Single::STATUSDUMP)
      cout << "Toggled feature with label: " << selected_item - 1
            << "; hit Delete to delete." << endl;

    monoslaminterface->GetSceneNoConst()->
            toggle_feature_lab(selected_item - 1);
    monoslaminterface->GetSceneNoConst()->
            mark_feature_by_lab(selected_item - 1);
  }

  monoslaminterface->GetRobotNoConst()->
          set_image_selection(int (camera_x + 0.5), int(camera_y + 0.5));
  if (Scene_Single::STATUSDUMP)
    cout << "Selected image position: " << int (camera_x + 0.5) << ", "
    << int(camera_y + 0.5) << endl;

  threedtool->RequestDraw();
  image_threedtool->RequestDraw();
}

void MonoSLAMGlow::SaveRawImage() {
  char filename[100];
  sprintf(filename, "rawoutput%04d.ppm", output_raw_image_number++);
  grabbed_image->WriteImage(filename);
  sync();
  if (Scene_Single::STATUSDUMP)
    cout << "Written raw image " << filename << endl;
}

void MonoSLAMGlow::SaveTrackedImage() {
  // Output one composite of image and 3D tool
  VW::ImageRGB<unsigned char>
          output_image(monoslaminterface->GetCameraWidth(),
          monoslaminterface->GetCameraHeight());
  image_threedtool->GetImage(output_image);
  VW::ImageRGB<unsigned char>
          output3d_image(monoslaminterface->GetCameraWidth(),
          monoslaminterface->GetCameraHeight());
  threedtool->GetImage(output3d_image);

  VW::ImageRGB<unsigned char>
          outputcomposite_image(2 * monoslaminterface->GetCameraWidth(),
          monoslaminterface->GetCameraHeight());
  // Fill composite
  for (unsigned int row = 0;
          row < monoslaminterface->GetCameraHeight(); row++) {
    for (unsigned int im1col = 0;
            im1col < monoslaminterface->GetCameraWidth(); im1col++) {
      outputcomposite_image.SetPixel(im1col, row,
              output_image.GetPixel(im1col, row));
    }
    for (int unsigned im2col = 0;
            im2col < monoslaminterface->GetCameraWidth(); im2col++) {
      outputcomposite_image.SetPixel(
              im2col + monoslaminterface->GetCameraWidth(), row,
              output3d_image.GetPixel(im2col, row));
    }
  }
  char filename[100];
  sprintf(filename, "composite%04d.ppm", output_tracked_image_number);
  outputcomposite_image.WriteImage(filename);
  if (Scene_Single::STATUSDUMP)
    cout << "Written image " << filename << endl;
  output_tracked_image_number++;

  // Force write to disk to avoid build-up and later dropped frames
  sync();
}

void MonoSLAMGlow::HandleNewFrameGlow(int frame_number) {
  mutex.Lock();
  monoslaminterface->ResetTimer1();
  if (Scene_Single::STATUSDUMP)
    cout << "Frame acquired: absolute time = "
          << monoslaminterface->GetTimer0()
    << endl;

  grabber->CopyImage(*grabbed_image, 0);
  monoslaminterface->GetRobotNoConst()->load_new_image(grabbed_image);

  if (Scene_Single::STATUSDUMP)
    cout << "Time after copying image: " << monoslaminterface->GetTimer1()
    << endl;

  if (toggle_tracking_button->GetState()) {
    // One tracking step (class MonoSLAM)
    if (!monoslaminterface->GoOneStep(grabbed_image,
            delta_t, enable_mapping_button->GetState())) {
      // If GoOneStep returns false, tracking has gone nasty so stop
      toggle_tracking_button->SetState(GlowCheckBoxWidget::off);
    }
  }
  if (Scene_Single::STATUSDUMP)
    cout << "Time after all vision: " << monoslaminterface->GetTimer1()
    << endl;

  // After tracking, update the displays
  UpdateDisplayParameters();

  mutex.Unlock();

  if (output_raw_images_button->GetState()) {
    SaveRawImage();
  } else {
    // Don't display if we're trying to save an image sequence

    // In real-time mode, only redraw if we have time
    if (!realtimeflag ||
            // Heuristic: we need around 15ms to do graphics
            monoslaminterface->GetTimer1().GetAsSeconds() < delta_t - 0.015) {
      threedtool->RequestDraw();
      image_threedtool->RequestDraw();
    } else
      if (Scene_Single::STATUSDUMP)
      cout << "No time to update displays." << endl;
  }

  if (output_tracked_images_button->GetState()) {
    SaveTrackedImage();
  }
  // Save 3D scene

  //this->mrpt3dScene->setStep(frame_number);
  //cout<<"before"<<endl;
  this->mrpt3dScene->writeMRPT3DScene();
  //cout << "Step scene " << frame_number << " - " << this->mrpt3dScene->getStep() << endl;
  this->monoslamLog->saveLog();
}

void MonoSLAMGlow::UpdateDisplayParameters() {
  // Put the camera in the right place in the image display
  monoslaminterface->GetScene()->get_motion_model()->
          func_xp(monoslaminterface->GetScene()->get_xv());
  ThreeD_Motion_Model *threed_motion_model =
          (ThreeD_Motion_Model *) monoslaminterface->GetScene()->get_motion_model();
  VW::Vector3D r = threed_motion_model->get_rRES();
  VW::Quaternion qWR = threed_motion_model->get_qRES();

  if (display_trajectory_button->GetState()) {
    trajectory_store.push_back(r.GetVNL3());
    if (trajectory_store.size() > 800) {
      trajectory_store.erase(trajectory_store.begin());
    }
  }

  // q is qWR between world frame and Scene robot frame
  // What we need to plot though uses GL object frame O
  // Know qRO: pi rotation about y axis
  // qWO = qWR * qRO
  VW::Quaternion qRO(0.0, 1.0, 0.0, 0.0);
  VW::Quaternion qWO = qWR * qRO;

  image_threedtool->SetCameraPositionOrientation(r, qWO);
}

void MonoSLAMGlow::InitialiseFeatureGlow() {
  monoslaminterface->InitialiseFeature();

  threedtool->RequestDraw();
  image_threedtool->RequestDraw();
}

void MonoSLAMGlow::AutoInitialiseFeatureGlow() {
  // Zero u here
  VNL::Vector<double> u(3);
  u.Fill(0.0);

  monoslaminterface->AutoInitialiseFeature(u, delta_t);
}

void MonoSLAMGlow::DeleteFeatureGlow() {
  monoslaminterface->DeleteFeature();

  threedtool->RequestDraw();
  image_threedtool->RequestDraw();
}

void MonoSLAMGlow::SavePatchGlow() {
  monoslaminterface->SavePatch();
}

void MonoSLAMGlow::Quit() {
  exit(0);
}

void MonoSLAMGlow::PrintRobotStateGlow() {
  monoslaminterface->PrintRobotState();
}

// These functions make sure we don't have an inconsistent set of buttons:
// If display_ellipsoids is set then display_threed_points must be too

void MonoSLAMGlow::HandleThreedButton() {
#if 0
  if (!display_3d_features_button->GetState())
    display_3d_feature_uncertainties_button->set_active(false);

  threedtool->RequestDraw();
  image_threedtool->RequestDraw();
#endif
}

void MonoSLAMGlow::HandleEllipsoidsButton() {
#if 0
  if (display_3d_feature_uncertainties_button->GetState())
    display_3d_features_button->set_active(true);

  threedtool->RequestDraw();
  image_threedtool->RequestDraw();
#endif
}

void MonoSLAMGlow::GraphicalButtonToggled(bool newstate) {
  threedtool->RequestDraw();
  image_threedtool->RequestDraw();
}
