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
    int size = a.size();
    Point **points = new Point* [size];
    for (int i = 0; i < size; i++) {
        points[i] = new Point(a[i], b[i]);
    }
    return points;
}

float SimpleAnomalyDetector::findThreshold(Point **points, int num, Line line) {
    float maxDistance = 0;
    for (int i = 0; i < num; i++) {
      Point *point = points[i];
        float distance = std::abs(point->y - line.f(point->x));
        if (distance > maxDistance)
          maxDistance = distance;
    }
    return maxDistance;
}

bool SimpleAnomalyDetector::isContainedInCorrelatedFeas(correlatedFeatures featurePair) {

    for (correlatedFeatures featuresInStorage: correlatedFeas) {
    bool isFirstElementTheSame = !(featuresInStorage.feature1.compare(featurePair.feature1));
    bool isSecondElementTheSame = !(featuresInStorage.feature2.compare(featurePair.feature2));
    if (isFirstElementTheSame && isSecondElementTheSame) {
      return true;
    }
  }
  return false;

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
    //TODO: make this into a separate function

    // get features from ts (ONCE)
    std::vector<std::string> featuresVector =  ts.getFeatures();
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
                    cf[i].feature1 = featuresVector[i];
                    cf[i].feature2 = featuresVector[j];
                }
                // if column j found a new friend
                if (correlation > cf[j].correlation) {
                    cf[j].correlation = correlation;
                    cf[j].feature1 = featuresVector[i];
                    cf[j].feature2 = featuresVector[j];
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


          if (!isContainedInCorrelatedFeas(cf[i])) {
              correlatedFeas.push_back(cf[i]);
            }
        }
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    vector<AnomalyReport> v;
    // for each pair of correlations:
    for_each(correlatedFeas.begin(), correlatedFeas.end(),[&v, &ts, this](correlatedFeatures c) {
      // get both columns
        vector<float> firstColumn = ts.getColumn(c.feature1);
        vector<float> secondColumn = ts.getColumn(c.feature2);
        // for each value pair in the columns
        for (int i = 0; i < firstColumn.size(); i++) {
          if (isAnomal(firstColumn[i], secondColumn[i], c)) {
            string s = c.feature1 + "-" + c.feature2;
            v.push_back(AnomalyReport(s, i + 1));
          }
        }
    });
    return v;
}

bool SimpleAnomalyDetector::isAnomal(float x, float y, correlatedFeatures cf){
  //cout << "DEV: " << std::abs(dev(Point(x,y),cf.lin_reg)) << " THRESHOLD: " << cf.threshold << endl;
  return (std::abs(dev(Point(x,y),cf.lin_reg)) > cf.threshold);
}
