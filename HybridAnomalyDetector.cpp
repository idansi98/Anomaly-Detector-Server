
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

void HybridAnomalyDetector::learnNormalHelper(const TimeSeries &ts, correlatedFeatures cf [],
                                              const int rowCount, int i) {
    SimpleAnomalyDetector::learnNormalHelper(ts, cf, rowCount, i);
    if (cf[i].corrlation < threshold && cf[i].corrlation > hybridtreshhold) {
        Point **points = SimpleAnomalyDetector::toPoints(ts.getColumn(cf[i].feature1),
                                  ts.getColumn(cf[i].feature2));
        Circle circle = findMinCircle(points, rowCount);
        cf[i].x = circle.center.x;
        cf[i].y= circle.center.y;
        cf[i].radius = circle.radius;
        cf[i].threshold = cf[i].radius * 1.1f;
        if (!isContainedInCorrelatedFeas(cf[i])) {
            correlatedFeas.push_back(cf[i]);
        }
        // delete points
        for(size_t i = 0;i < rowCount;i++)
            delete points[i];
        delete[] points;
    }
}