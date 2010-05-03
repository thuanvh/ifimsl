/* 
 * File:   monoslamMRPT3DScene.h
 * Author: thuan
 *
 * Created on April 19, 2010, 10:07 PM
 */

#ifndef _MONOSLAMMRPT3DSCENE_H
#define	_MONOSLAMMRPT3DSCENE_H
#include "main.h"
// Scene includes

//#include "Scene/kalman.h"
//#include "Scene/sim_or_rob.h"

#include <cstring>
#include <string>



using namespace mrpt;
using namespace mrpt::slam;
using namespace mrpt::opengl;
using namespace mrpt::gui;
using namespace mrpt::math;
using namespace mrpt::utils;
using namespace mrpt::system;
using namespace mrpt::random;

// Local includes
//#include "robot.h"
using namespace std;

class monoslamMRPT3DScene {
public:

    monoslamMRPT3DScene();
    void writeMRPT3DScene();

    const Scene_Single* getScene() const {
        return scene;
    }

    void setScene(const Scene_Single* scene) {
        this->scene = scene;
    }

    string getSceneDirPath() const {
        return sceneDirPath;
    }

    void setSceneDirPath(string sceneDirPath) {
        this->sceneDirPath = sceneDirPath;
    }

    int getStep() const {
        return step;
    }

    void setStep(int step) {
        this->step = step;
    }
//
//    void setLastRobotPosition(VNL::Vector<double> a){
//        for(int i=0; i<a.size(); i++)
//
//    }

protected:
    const Scene_Single *scene;
    string sceneDirPath;
    int step;
    CSetOfLinesPtr lineGroup;
//    VNL::Vector<double> lastRobotPosition;
    CPose3D lastRobotPosition;
    bool isSceneStart;
};

#endif	/* _MONOSLAMMRPT3DSCENE_H */

