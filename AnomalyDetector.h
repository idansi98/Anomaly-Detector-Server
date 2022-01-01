/*
 * AnomalyDetector.h
 *
 * Authors:     206534299 Ido Tziony
 *              206821258 Idan Simai
 *
 *
 */

#ifndef ANOMALY_DETECTION_UTIL_CPP_ANOMALYDETECTOR_H
#define ANOMALY_DETECTION_UTIL_CPP_ANOMALYDETECTOR_H

#include "timeseries.h"
#include "math.h"
#include <string>
#include <vector>
using namespace std;

class AnomalyReport {
public:
    const string description;
    const long timeStep;
    AnomalyReport(string description, long timeStep) :
            description(description),timeStep(timeStep){}
};
class TimeSeriesAnomalyDetector {
public:
//Learn the normal regression.
    virtual void learnNormal(const TimeSeries& ts) = 0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts) = 0;
    virtual ~TimeSeriesAnomalyDetector(){}
};
#endif //ANOMALY_DETECTION_UTIL_CPP_ANOMALYDETECTOR_H

