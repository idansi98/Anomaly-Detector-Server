
// compile only once
#pragma once
#include "anomaly_detection_util.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


class TimeSeries {
private:
    int rowCount;
    int columnCount;
    std::ifstream fileReader;
    // this will contain all the features and the vectors they have
    std::map<std::string, std::vector<float>> featureMap;
    // a vector, same siz as rows that contains the list of features
    std::vector<std::string> features;

private:
    std::vector<std::string> readLine();
    void initMap();
    void initVectors();

public:
    explicit TimeSeries(const char *csvFileName);
// get features
    const std::vector<std::string>& getFeatures() const{
        return features;
    }
    // get feature by number
    const std::string& getFeature(int featureNumber) const {
        return features[featureNumber];
    }
    // get column given a feature
    const std::vector<float>& getColumn(std::string feature) const {
        return featureMap.at(feature);
    }
    // get a column by feature number
    const std::vector<float>& getColumn(int featureNumber) const {
        return featureMap.at(getFeature(featureNumber));
    }
    // gets the row count
    int getRowCount() const {
        return columnCount;
    }
    // gets the column count
    int getColumnCount() const {
        return features.size();
    }
    friend std::ostream& operator << (std::ostream& out, const TimeSeries& time_series);
};


