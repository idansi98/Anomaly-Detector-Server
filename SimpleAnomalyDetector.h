

#ifndef ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H
#define ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include "AnomalyDetector.h"
#include <vector>
#include <string>
#include <algorithm>
#include <set>


struct correlatedFeatures {
    string feature1, feature2;  //Names of the correlated features.
    float corrlation;
    Line lin_reg;
    float threshold;
    float radius;
    float x,y;
};

class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector {
protected:
    float threshold = 0.9;
    float hybridtreshhold = 0.5;
    vector<correlatedFeatures> correlatedFeas;
public:
//Check if a pair is contained in the vector.
bool isContainedInCorrelatedFeas(correlatedFeatures featurePair);
//The constructor.
SimpleAnomalyDetector();
//The destructor.
virtual ~SimpleAnomalyDetector();
//Learn the normal behaviour from a given time series.
virtual void learnNormal(const TimeSeries& ts);
//Detect the exceptions.
virtual vector<AnomalyReport> detect(const TimeSeries& ts);
//Find the threshold given a point object array.
float findThreshold(Point** points, int num, Line line);
//Return the correlated features.
vector<correlatedFeatures> getNormalModel() {
    return this->correlatedFeas;
    }
//Convert to vectors of floats to an array of points.
Point** toPoints(vector<float> x, vector<float> y);
//Checks if a given point - represented as (x,y), is an anomaly in relation to the expected line produced by cf.
virtual bool isAnomal(float x, float y,correlatedFeatures cf);
//friend std::ostream& operator << (std::ostream& out, const SimpleAnomalyDetector& detector);

};

#endif //ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H

