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
    correlatedFeatures cf[ts.getColumnCount()];
    for (int i = 0; i < ts.getColumnCount(); ++i) {
        cf[i].corrlation = 0;
    }
    for (int i = 0; i < ts.getColumnCount(); ++i) {
        float m = 0;
        int c = -1;
        for (int j = (i + 1); j < ts.getColumnCount(); ++j) {
            float f = (std::abs(pearson(&ts.getColumn(i)[0], &ts.getColumn(j)[0], ts.getRowCount())));
            if (f > m) {
                m = f, c = j;
                if (f > cf[i].corrlation) {
                    cf[i].corrlation = f;
                    cf[i].feature1 = ts.getFeature(i);
                    cf[i].feature2 = ts.getFeature(j);
                }
                if (f > cf[j].corrlation) {
                    cf[j].corrlation = f;
                    cf[j].feature1 = ts.getFeature(i);
                    cf[j].feature2 = ts.getFeature(j);
                }
            }
        }
    }
    for (int i = 0; i < ts.getColumnCount(); ++i) {
        if (cf[i].corrlation >= threshold) {
            Point **points = toPoints(ts.getColumn(cf[i].feature1),
                                      ts.getColumn(cf[i].feature2));
            cf[i].lin_reg = linear_reg(points, ts.getRowCount());
            cf[i].threshold = 1.1 * findThreshold(points, ts.getRowCount(), cf[i].lin_reg);
            if (&correlatedFeas[0] == nullptr) {
                correlatedFeas.push_back(cf[i]);
            }
            int len = correlatedFeas.size();
            int j = 1;
            for (correlatedFeatures c:correlatedFeas) {
                if ((!c.feature1.compare(cf[i].feature1)) && (!c.feature1.compare(cf[i].feature2))) {
                    break;
                }
                if (j == len) {
                    correlatedFeas.push_back(cf[i]);
                }
                j++;
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
