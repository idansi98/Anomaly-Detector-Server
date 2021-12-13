

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual bool isAnomal(float x, float y, correlatedFeatures cf);
    virtual void learnNormalHelper(const TimeSeries &ts, correlatedFeatures  cf [],const int rowCount, int i);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
