

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
    string feature1, feature2;  // names of the correlated features.
    float correlation;
    Line lin_reg;
    float threshold;
};

class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector {
protected:
    float threshold = 0.9;
    vector<correlatedFeatures> correlatedFeas;
public:
    bool isContainedInCorrelatedFeas(correlatedFeatures featurePair);
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    float findThreshold(Point** points, int num, Line line);
    vector<correlatedFeatures> getNormalModel() {
        return this->correlatedFeas;
    }
    Point** toPoints(vector<float> x, vector<float> y);
    bool isAnomal(float x, float y,correlatedFeatures cf);
    //friend std::ostream& operator << (std::ostream& out, const SimpleAnomalyDetector& detector);

};

#endif //ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H

