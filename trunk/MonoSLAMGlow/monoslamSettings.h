/* 
 * File:   monoslamSettings.h
 * Author: thuan
 *
 * Created on May 3, 2010, 8:52 PM
 */

#ifndef _MONOSLAMSETTINGS_H
#define	_MONOSLAMSETTINGS_H
#include <iostream>
class MonoslamSettings {
public:
    MonoslamSettings();
    double delta_t;
    unsigned int NUMBER_OF_FEATURES_TO_SELECT;
    unsigned int NUMBER_OF_FEATURES_TO_KEEP_VISIBLE;
    unsigned int MAX_FEATURES_TO_INIT_AT_ONCE;
    double MIN_LAMBDA;
    double MAX_LAMBDA;
    unsigned int NUMBER_OF_PARTICLES;
    double STANDARD_DEVIATION_DEPTH_RATIO;
    unsigned int MIN_NUMBER_OF_PARTICLES;
    double PRUNE_PROBABILITY_THRESHOLD;
    unsigned int ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS;
    std::string monoslamStateFile;
    void readSettings(const char* file);
};
//    double delta_t = 1.0 / 30.0;
//    unsigned int NUMBER_OF_FEATURES_TO_SELECT = 10;
//    unsigned int NUMBER_OF_FEATURES_TO_KEEP_VISIBLE = 12;
//    unsigned int MAX_FEATURES_TO_INIT_AT_ONCE = 1;
//    double MIN_LAMBDA = 0.5;
//    double MAX_LAMBDA = 5.0;
//    unsigned int NUMBER_OF_PARTICLES = 100;
//    double STANDARD_DEVIATION_DEPTH_RATIO = 0.3;
//    unsigned int MIN_NUMBER_OF_PARTICLES = 20;
//    double PRUNE_PROBABILITY_THRESHOLD = 0.05;
//    unsigned int ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS = 10;
#endif	/* _MONOSLAMSETTINGS_H */

