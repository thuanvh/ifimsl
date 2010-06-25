/*  MonoSLAM: Real-Time Single Camera SLAM

    MonoSLAM/monoslamglow.h
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

// Interface to MonoSLAM for GLOW display window

#ifndef _monoslam_monoslamglow_h_
#define _monoslam_monoslamglow_h_
#include "main.h"
#include "monoslamMRPT3DScene.h"
#include "monoslamlog.h"
#include "monoslamSettings.h"
#include <glow.h>
#include <glowQuickPalette.h>
#include <VWGLOW/display.h>
//#include <VWGL/display.h>
#ifdef _REALTIME_
#include <VWGLOW/Interface/sequencerfirewireglow.h>
#else
#include <VWGLOW/Interface/sequencerfileglow.h>
#include <VWGLOW/Display/threedtoolglowwidget.h>
//#include <VWGL/Interface/sequencerfileglow.h>
#endif


#include <MonoSLAM/monoslaminterface.h>

// This include here not ideal but needed to set up 3D display
#include <MonoSLAM/models_wideangle.h>



GLOW_NAMESPACE_USING
        using namespace VW;

class MonoSLAMGlow :
public GlowWindow,
public VWEvents::EventHandler,
public GlowPushButtonReceiver,
public GlowCheckBoxReceiver {
public:
    //MonoSLAMGlow(int argc, char* argv[], MonoslamSettings mns, CDisplayWindow3D* mrptwindows);
    MonoSLAMGlow(int argc, char* argv[], MonoslamSettings mns);

    virtual ~MonoSLAMGlow() {
    }

    // Functions used by constructor
    void SetUpMonoSLAM();
    void SetUpButtons();
    void SetUp3DDisplays();
    void SetUpImageGrabber(char* folder);

    // Methods overridden from GlowWindow
    // How to resize the window
    void OnReshape(int width, int height);
    // How to receive button events
    void OnMessage(const GlowPushButtonMessage& message);
    void OnMessage(const GlowCheckBoxMessage& message);


    // Draw functions and functions for handling mouse interaction for
    // 3D displays
    void Draw3D(bool select_flag);
    void ProcessHits(int selected_item, double camera_x, double camera_y);
    void ImageDraw3D(bool select_flag);
    void ImageProcessHits(int selected_item, double camera_x, double camera_y);

    // Function to save raw image sequence to disk
    void SaveRawImage();
    // Function to save tracking output image sequence to disk
    void SaveTrackedImage();

    // Main function called when a new frame arrives
    void HandleNewFrameGlow(int frame_number);

    // Function to be called after tracking to set new position of camera,
    // etc. for display
    void UpdateDisplayParameters();

    // These functions called by buttons: normally call non-Glow equivalents
    // in MonoSLAM directly to do non-interface-specific stuff
    void InitialiseFeatureGlow();
    void AutoInitialiseFeatureGlow();
    void DeleteFeatureGlow();
    void SavePatchGlow();
    void Quit();
    void PrintRobotStateGlow();
    void HandleThreedButton();
    void HandleEllipsoidsButton();
    // A function called whenever one of the graphical buttons is toggled
    // Its only function is to request an immediate redraw of the displays
    void GraphicalButtonToggled(bool newstate);

protected:

    // The main class where processing happens
    MonoSLAMInterface *monoslaminterface;

    // For saving and drawing the camera trajectory
    std::vector< VNL::VectorFixed < 3, double> > trajectory_store;

    // Sequencers for grabbing images from firewire camera or file sequence
#ifdef _REALTIME_
    SequencerFirewireGlow *grabber;
#else
    SequencerFileGlow *grabber;
#endif

    // Flag so the program knows if we're in real-time mode
    bool realtimeflag;

    // Mutex to make sure we don't draw and update at the same time
    VW::Mutex mutex;

    VW::ImageMono<unsigned char> *grabbed_image;

    // Camera calibration information for graphics
    double Graphics_Fku, Graphics_Fkv, Graphics_U0, Graphics_V0, Graphics_Kd1;

    // GUI Stuff
    // Panels for buttons
    GlowQuickPaletteSubwindow* controlpanel1;
    GlowQuickPaletteSubwindow* controlpanel2;

    // 3D tool widgets
    // For image display
    ThreeDToolGlowWidget *image_threedtool;
    // For external 3D display
    ThreeDToolGlowWidget *threedtool;

    // GUI buttons
    // Check buttons
    GlowCheckBoxWidget* toggle_tracking_button;
    GlowCheckBoxWidget* enable_mapping_button;

    // Select whether to save images to disk
    GlowCheckBoxWidget* output_tracked_images_button;
    GlowCheckBoxWidget* output_raw_images_button;

    // Set mode for drawing image display
    GlowCheckBoxWidget* rectify_image_display_button;

    // For External / Rectified Internal displays
    GlowCheckBoxWidget* display_trajectory_button;
    GlowCheckBoxWidget* display_3d_features_button;
    GlowCheckBoxWidget* display_3d_feature_uncertainties_button;

    // For Rectified / Raw Internal displays
    GlowCheckBoxWidget* display_image_button;

    // For Raw Internal display only
    GlowCheckBoxWidget* display_2d_descriptors_button;
    GlowCheckBoxWidget* display_2d_searches_button;
    GlowCheckBoxWidget* display_initialisation_button;

    // Standard push buttons
    GlowPushButtonWidget* init_feature_button;
    GlowPushButtonWidget* auto_init_feature_button;
    GlowPushButtonWidget* print_robot_state_button;
    GlowPushButtonWidget* delete_feature_button;
    GlowPushButtonWidget* save_patch_button;
    GlowPushButtonWidget* quit_button;

    // Important parameters
    const double delta_t;

    // Some constants which we could set with Glow sliders etc.
    const unsigned int NUMBER_OF_FEATURES_TO_SELECT;
    const unsigned int NUMBER_OF_FEATURES_TO_KEEP_VISIBLE;
    const unsigned int MAX_FEATURES_TO_INIT_AT_ONCE;
    const double MIN_LAMBDA;
    const double MAX_LAMBDA;
    const unsigned int NUMBER_OF_PARTICLES;
    const double STANDARD_DEVIATION_DEPTH_RATIO;
    const unsigned int MIN_NUMBER_OF_PARTICLES;
    const double PRUNE_PROBABILITY_THRESHOLD;
    const unsigned int ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS;

    // Counters for outputting images
    unsigned int output_tracked_image_number;
    unsigned int output_raw_image_number;

    // 3D Mrpt Scene
    monoslamMRPT3DScene* mrpt3dScene;
    std::string monoslamStateFile;

    // Monoslam Log
    monoslamlog* monoslamLog;
};

#endif
