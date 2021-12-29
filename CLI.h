//
// Created by Ido on 28/12/2021.
//
#ifndef ANOMALYDETECTOR_H__CLI_H_
#define ANOMALYDETECTOR_H__CLI_H_

#include <vector>
#include <algorithm>
#include <string.h>
#include "commands.h"
using namespace std;

class CLI {
    DefaultIO* dio;
    // you can add data members
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};
#endif //ANOMALYDETECTOR_H__CLI_H_
