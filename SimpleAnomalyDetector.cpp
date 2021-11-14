/*
 * SimpleAnomalyDetector.cpp
 *
 * Authors:  206821258 Idan Simai
 *           206534299 Ido Tziony
 *
 */

#include "SimpleAnomalyDetector.h"
//#include "anomaly_detection_util.h"
//#include "timeseries.h"
//#include "AnomalyDetector.h"
#include <vector>
//#include <string>
#include <algorithm>
#include <unordered_set>


SimpleAnomalyDetector::SimpleAnomalyDetector() {
    //TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    //TODO Auto-generated destructor stub
}

Point** SimpleAnomalyDetector::toPoints(vector <float> a, vector <float> b) {
    Point **points = new Point* [a.size()];
    for (int i = 0; i < a.size(); i++) {
        points[i] = new Point(a[i], b[i]);
    }
    return points;
}

float SimpleAnomalyDetector::findThreshold(Point **points, int num, Line line) {
    float m = 0;
    for (int i = 0; i < num; i++) {
        float distance = std::abs(points[i]->y - line.f(points[i]->x));
        if (distance > m)
            m = distance;
    }
    return m;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    // consts
    const int columnCount = ts.getColumnCount();
    const int rowCount = ts.getRowCount();
    // create a correlated features array
    correlatedFeatures cf[columnCount];
    // reset default correlation to be 0
    for (int i = 0; i < columnCount; ++i) {
        cf[i].correlation = 0;
    }
    // for each column i
    for (int i = 0; i < columnCount; i++) {
        // the best correlation
        float bestCorrelation = 0;
        // the index to the column of the most correlating column
        int correlationFriend = -1;
        // for each column j>i
        for (int j = (i + 1); j < columnCount; ++j) {
          // get correlation from columns i:j
            float correlation = (std::abs(pearson(&ts.getColumn(i)[0], &ts.getColumn(j)[0], ts.getRowCount())));
            // if correlation is higher than previous correlations -> swap it
            if (correlation > bestCorrelation) {
              bestCorrelation = correlation;
              correlationFriend = j;
              // if column i found a new friend
                if (correlation > cf[i].correlation) {
                    cf[i].correlation = correlation;
                    cf[i].feature1 = ts.getFeature(i);
                    cf[i].feature2 = ts.getFeature(j);
                }
                // if column j found a new friend
                if (correlation > cf[j].correlation) {
                    cf[j].correlation = correlation;
                    cf[j].feature1 = ts.getFeature(i);
                    cf[j].feature2 = ts.getFeature(j);
                }
            }
        }
    }
    // if here: done getting most correlating pairs + correlation number

    //for all columns of pairs:
    for (int i = 0; i < columnCount; ++i) {
      // if correlation is high enough -> make it into an array of points.
        if (cf[i].correlation >= threshold) {
            Point **points = toPoints(ts.getColumn(cf[i].feature1),
                                      ts.getColumn(cf[i].feature2));
            //  get the linear regression + threshold of points and put it in cf
            cf[i].lin_reg = linear_reg(points, rowCount);
            cf[i].threshold = 1.1f * findThreshold(points, rowCount, cf[i].lin_reg);

            if (&correlatedFeas[0] == nullptr) {
                correlatedFeas.push_back(cf[i]);
            }
            int len = correlatedFeas.size();
            int i = 1;
            for (correlatedFeatures c:correlatedFeas) {
                if ((!c.feature1.compare(cf[i].feature1)) && (!c.feature1.compare(cf[i].feature2))) {
                    break;
                }
                if (i == len) {
                    correlatedFeas.push_back(cf[i]);
                }
            }
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    /*vector<AnomalyReport> v;
    for_each(correlatedFeas.begin(), correlatedFeas.end(),[&v, &ts, this](correlatedFeatures c) {
        vector<float> x = ts.getColumn(c.feature1);
        vector<float> y = ts.getColumn(c.feature2);
        for (int i = 0; i < x.size(); i++) {
            Point p = Point(x[i], y[i]);
            if (c.corrlation > threshold) {
                if (isAnomal(x[i], y[i], c)) {
                    string s = c.feature1 + "-" + c.feature2;
                    v.push_back(AnomalyReport(s, (i + 1)));
                }
            } else {
                string d = c.feature1 + "-" + c.feature2;
                v.push_back(AnomalyReport(d, (i + 1)));

            }
        }
    });
    return v;*/
}

bool SimpleAnomalyDetector::isAnomal(float x, float y, correlatedFeatures cf){
    return (std::abs(y - cf.lin_reg.f(x) )> cf.threshold);
}
