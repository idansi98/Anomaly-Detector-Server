/*
 * SimpleAnomalyDetector.cpp
 *
 * Authors:    206821258 Idan Simai
 *             206534299 Ido Tziony
 *
 */

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
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

//Gets 2 float vectors, and returns a vector of points.
Point** SimpleAnomalyDetector::toPoints(vector <float> a, vector <float> b) {
    int size = a.size();
    Point **points = new Point* [size];
    for (int i = 0; i < size; i++) {
        points[i] = new Point(a[i], b[i]);
    }
    return points;
}

//Finds the threshold out of an array of points[num] in relation to a line.
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

//True: feature already exists in an object.
bool SimpleAnomalyDetector::isContainedInCorrelatedFeas(correlatedFeatures featurePair) {
    for (const correlatedFeatures& featuresInStorage: correlatedFeas) {
    bool isFirstElementTheSame = !(featuresInStorage.feature1.compare(featurePair.feature1));
    bool isSecondElementTheSame = !(featuresInStorage.feature2.compare(featurePair.feature2));
    if (isFirstElementTheSame && isSecondElementTheSame) {
      return true;
    }
  }
  return false;
}

//Learns and initializes correlatedFeas from the given TimeSeries object.
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    //Consts.
    const int columnCount = ts.getColumnCount();
    const int rowCount = ts.getRowCount();
    //Create a correlated features array.
    correlatedFeatures cf[columnCount];
    //Reset default correlation to be 0.
    for (int i = 0; i < columnCount; ++i) {
        cf[i].corrlation = 0;
    }
    //TODO: make this into a separate function

    //Get features from ts (ONCE).
    std::vector<std::string> featuresVector =  ts.getFeatures();
    //For each column i.
    for (int i = 0; i < columnCount; i++) {
        //The best correlation.
        float bestCorrelation = 0;
        //The index to the column of the most correlating column.
        int correlationFriend = -1;
        //For each column j > i.
        for (int j = (i + 1); j < columnCount; ++j) {
          //Get correlation from columns i : j.
            float correlation = (std::abs(pearson(&ts.getColumn(i)[0],
                                                  &ts.getColumn(j)[0], ts.getRowCount())));
            //If correlation is higher than previous correlations -> swap it.
            if (correlation > bestCorrelation) {
              bestCorrelation = correlation;
              correlationFriend = j;
              //If column i found a new friend.
                if (correlation > cf[i].corrlation) {
                    cf[i].corrlation = correlation;
                    cf[i].feature1 = featuresVector[i];
                    cf[i].feature2 = featuresVector[j];
                }
                //If column j found a new friend.
                if (correlation > cf[j].corrlation) {
                    cf[j].corrlation = correlation;
                    cf[j].feature1 = featuresVector[i];
                    cf[j].feature2 = featuresVector[j];
                }
            }
        }
    }
    int len = ts.getRowCount();
    //If here: done getting most correlating pairs + correlation number.

    //For all columns of pairs:
    for (int i = 0; i < columnCount; ++i) {
      //If correlation is high enough -> make it into an array of points.
        if (cf[i].corrlation >= threshold) {
            Point **points = toPoints(ts.getColumn(cf[i].feature1),
                                      ts.getColumn(cf[i].feature2));
            //Get the linear regression + threshold of points and put it in cf.
            cf[i].lin_reg = linear_reg(points, rowCount);
            cf[i].threshold = 1.15f * findThreshold(points, rowCount, cf[i].lin_reg);
          if (!isContainedInCorrelatedFeas(cf[i])) {
              correlatedFeas.push_back(cf[i]);
              // delete points
              for(size_t i = 0;i < len;i++)
                  delete points[i];
              delete[] points;
            }
        }
        if (cf[i].corrlation < threshold && cf[i].corrlation > hybridtreshhold) {
            Point **points = toPoints(ts.getColumn(cf[i].feature1),
                                      ts.getColumn(cf[i].feature2));
            Circle circle = findMinCircle(points, len);
            cf[i].x = circle.center.x;
            cf[i].y= circle.center.y;
            cf[i].radius = circle.radius;
            cf[i].threshold = cf[i].radius * 1.1f;
            if (!isContainedInCorrelatedFeas(cf[i])) {
                correlatedFeas.push_back(cf[i]);
            }
            // delete points
            for(size_t i = 0;i < len;i++)
                delete points[i];
            delete[] points;
        }

    }
}

//Returns a vector of all anomalies in given TimeSeries ts (based on prev. learned data).
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    vector<AnomalyReport> v;
    //For each pair of correlations:
    for_each(correlatedFeas.begin(), correlatedFeas.end(),[&v, &ts, this](correlatedFeatures c) {
      //Get both columns.
        vector<float> firstColumn = ts.getColumn(c.feature1);
        vector<float> secondColumn = ts.getColumn(c.feature2);
        //For each value pair in the columns.
        for (int i = 0; i < firstColumn.size(); i++) {
          if (isAnomal(firstColumn[i], secondColumn[i], c)) {
            string s = c.feature1 + "-" + c.feature2;
            v.push_back(AnomalyReport(s, i + 1));
          }
        }
    });
    return v;
}

//Checks if a given point - represented as (x,y), is an anomaly in relation to the expected line produced by cf.
bool SimpleAnomalyDetector::isAnomal(float x, float y, correlatedFeatures cf) {
  return (std::abs(dev(Point(x,y),cf.lin_reg)) > cf.threshold);
}
/*
std::ostream& operator <<(std::ostream &out, const SimpleAnomalyDetector &detector) {

  return out;
}
 */
