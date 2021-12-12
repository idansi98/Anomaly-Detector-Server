
#include "HybridAnomalyDetector.h"
#include "SimpleAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

bool HybridAnomalyDetector::isAnomal(float x, float y,correlatedFeatures cf) {
    return (cf.corrlation >= cf.threshold && (std::abs(dev(Point(x,y),cf.lin_reg)) > cf.threshold)) ||
           (cf.corrlation > 0.5 && cf.corrlation < cf.threshold &&
           distance(Point(cf.x, cf.y),Point(x, y)) > cf.threshold);
}