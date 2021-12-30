//
// Created by Ido on 28/12/2021.
//

/*
 *  TODO: Command class implementation UPLOAD_AND_ANALYZE
 *  TODO: DefaultIO class implementation CLI
 *  TODO: DefaultIO class implementation SV-CL
 */


#ifndef ANOMALYDETECTOR_H__COMMANDS_H_
#define ANOMALYDETECTOR_H__COMMANDS_H_
#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <string>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO {
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}


//Method to read from a file.
void readFile(string fileName) {
    //Create a new ofstream.
    ofstream os;
    os.open(fileName);
    string line = this->read();
    //Continue reading unless you see "done".
    while(line.compare("done")) {
        os << (line + "\n");
        line = this->read();
    }
    //Close the ofstream.
    os.close();
    }
};



//A struct to share the data of the current state.
struct Data {
    float threshold;
    vector<AnomalyReport> anomalyReport;
Data() {
    threshold = 0.9;
}
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

//A class for the first command which is "Upload".
class UPLOAD: public Command {
public:
UPLOAD(DefaultIO* dio):Command(dio) {
    this->description = "1. upload a time series csv file";
    }
virtual void execute(Data* data) {
    dio->write("Please upload your local train CSV file.\n");
    dio->readFile("anomalyTrain.csv");
    dio->write("Upload complete.\n");
    dio->write("Please upload your local test CSV file.\n");
    dio->readFile("anomalyTest.csv");
    dio->write("Upload complete.\n");
    }
};

//A class for the second command which is "Set new threshold".
class THRESHOLD: public Command {
public:
THRESHOLD(DefaultIO* dio): Command(dio) {
    this->description = "2. algorithm settings";
    }
virtual void execute(Data* data) {
    bool check = false;
    dio->write("The current correlation threshold is ");
    dio->write(data->threshold);
    dio->write("\nType a new threshold\n");
    float value;
    dio->read(&value);
    while (!check) {
        if (value > 0 && value <= 1) {
            data->threshold = value;
            check = true;
        } else {
            dio->write("please choose a value between 0 and 1.\n");
        }
    }
}
};

//A class for the third command which is "Detect anomalies".
class DETECT: public Command {
public:
DETECT(DefaultIO* dio): Command(dio) {
    this->description = "3. detect anomalies";
}
virtual void execute(Data* data) {
    TimeSeries train("anomalyTrain.csv");
    TimeSeries test("anomalyTest.csv");
    HybridAnomalyDetector ad;
    ad.learnNormal(train);
    data->anomalyReport = ad.detect(test);
    dio->write("anomaly detection complete.\n");
}
};

//A class for the fourth command which is "Displaying results".
class DISPLAY_RESULTS: public Command {
public:
DISPLAY_RESULTS(DefaultIO* dio): Command(dio) {
    this->description = "4. display results";
}
virtual void execute(Data* data) {
    vector<AnomalyReport> anomalyReport = data->anomalyReport;
    for(AnomalyReport ar : anomalyReport) {
        string timeStep = std::to_string(ar.timeStep);
        dio->write(timeStep + "    " + ar.description + "\n");
    }
    dio->write("Done.\n");
}
};

//A class for the fourth command which is "Displaying results".
class UPLOAD_AND_ANALYZE: public Command {
public:
UPLOAD_AND_ANALYZE(DefaultIO* dio): Command(dio) {
    this->description = "5. upload anomalies and analyze results";
}
virtual void execute(Data* data) {
    //TODO:Complete the implementation.
}

};
//A class for the sixth command which is "Exit".
class Exit: public Command {
public:
Exit(DefaultIO* dio): Command(dio) {
        this->description = "6. exit";
    }
virtual void execute(Data* data) {
}
};
#endif //ANOMALYDETECTOR_H__COMMANDS_H_
