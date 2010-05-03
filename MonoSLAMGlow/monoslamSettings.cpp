
#include "monoslamSettings.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <algorithm>
using namespace std;

MonoslamSettings::MonoslamSettings() {
  delta_t = 1.0 / 30.0;
  NUMBER_OF_FEATURES_TO_SELECT = 10;
  NUMBER_OF_FEATURES_TO_KEEP_VISIBLE = 12;
  MAX_FEATURES_TO_INIT_AT_ONCE = 1;
  MIN_LAMBDA = 0.5;
  MAX_LAMBDA = 5.0;
  NUMBER_OF_PARTICLES = 100;
  STANDARD_DEVIATION_DEPTH_RATIO = 0.3;
  MIN_NUMBER_OF_PARTICLES = 20;
  PRUNE_PROBABILITY_THRESHOLD = 0.05;
  ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS = 10;
}

void MonoslamSettings::readSettings(const char* file) {
  char str[2000];
  fstream file_op(file);
  while (!file_op.eof()) {
    file_op.getline(str, 2000);
    string a(str);
    int pos = a.find('=');
    string abc = a.substr(0, pos - 1);

    string strValue = a.substr(pos + 1);

    std::remove(abc.begin(), abc.end(), ' ');
    std::remove(strValue.begin(), strValue.end(), ' ');
    cout << abc << strValue;
    
    if (abc.compare("delta_t") == 0) {
      delta_t = atof(strValue.c_str());
      continue;
    }
    if (abc.compare("NUMBER_OF_FEATURES_TO_SELECT") == 0) {
      NUMBER_OF_FEATURES_TO_SELECT = atoi(strValue.c_str());
      continue;
    }
    if (abc.compare("NUMBER_OF_FEATURES_TO_KEEP_VISIBLE") == 0) {
      NUMBER_OF_FEATURES_TO_KEEP_VISIBLE = atoi(strValue.c_str());
      continue;
    }
    if (abc.compare("MAX_FEATURES_TO_INIT_AT_ONCE") == 0) {
      MAX_FEATURES_TO_INIT_AT_ONCE = atoi(strValue.c_str());
      continue;
    }
    if (abc.compare("MIN_LAMBDA") == 0) {
      MIN_LAMBDA = atof(strValue.c_str());
      continue;
    }
    if (abc.compare("MAX_LAMBDA") == 0) {
      MAX_LAMBDA = atof(strValue.c_str());
      continue;
    }
    if (abc.compare("NUMBER_OF_PARTICLES") == 0) {
      NUMBER_OF_PARTICLES = atoi(strValue.c_str());
      continue;
    }
    if (abc.compare("STANDARD_DEVIATION_DEPTH_RATIO") == 0) {
      STANDARD_DEVIATION_DEPTH_RATIO = atof(strValue.c_str());
      continue;
    }
    if (abc.compare("MIN_NUMBER_OF_PARTICLES") == 0) {
      MIN_NUMBER_OF_PARTICLES = atoi(strValue.c_str());
      continue;
    }
    if (abc.compare("PRUNE_PROBABILITY_THRESHOLD") == 0) {
      PRUNE_PROBABILITY_THRESHOLD = atof(strValue.c_str());
      continue;
    }
    if (abc.compare("ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS") == 0) {
      ERASE_PARTIALLY_INIT_FEATURE_AFTER_THIS_MANY_ATTEMPTS = atoi(strValue.c_str());
      continue;
    }
  }
  file_op.close();
  //cout << endl;

}
