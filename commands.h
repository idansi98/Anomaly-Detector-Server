//
// Created by Ido on 28/12/2021.
//

/*  Abstract Command class
 *  Abstract DefaultIO class
 *  TODO: Command class implementation UPLOAD
 *  TODO: Command class implementation THRESHOLD
 *  TODO: Command class implementation DETECT
 *  TODO: Command class implementation DISPLAY_RESULTS
 *  TODO: Command class implementation UPLOAD_AND_ANALYZE_RESULTS
 *  TODO: Command class implementation EXIT
 *  TODO: DefaultIO class implementation CLI
 *  TODO: DefaultIO class implementation SV-CL
 *
 *
 */


#ifndef ANOMALYDETECTOR_H__COMMANDS_H_
#define ANOMALYDETECTOR_H__COMMANDS_H_
#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}

    // you may add additional methods here
};

// you may add here helper classes
struct Data {
    float threshold;

};

// you may edit this class
class Command {
protected:
    DefaultIO* dio;
    string description;
public:
    Command(DefaultIO* dio):dio(dio){}
    virtual void execute(Data* data)=0;
    virtual ~Command(){}
};

class UPLOAD:public Command {
public:
    UPLOAD(DefaultIO* dio):Command(dio) {
        this->description = "upload a time series csv file";
    }
    virtual void execute(Data* data) {
        dio->write("Please upload your local train CSV file.\n");

    }

};
#endif //ANOMALYDETECTOR_H__COMMANDS_H_
